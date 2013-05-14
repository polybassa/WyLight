//
//  WCWiflyControlWrapper.m
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres, Nils Weiß. All rights reserved.
//

#import "WCWiflyControlWrapper.h"
#include "WiflyControlNoThrow.h"
#include <time.h>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <atomic>

@interface WCWiflyControlWrapper () {
	WyLight::ControlNoThrow *mControl;
	std::thread *mFirstCtrlThread, *mSecondCtrlThread;
	std::function<uint32_t(void)> *mFunction;
	std::mutex *gCtrlMutex, *gCmdQueueMutex, *gFunctionMutex;
	std::deque<std::function<uint32_t(void)>> *mCmdQueue;
	std::atomic<bool> *ctrlIsRunning;
}

@property (nonatomic) NSThread* threadOfOwner;

-(void) postNotification;
-(void) callFatalErrorDelegate:(NSNumber*)errorCode;

@end

@implementation WCWiflyControlWrapper

@synthesize delegate;

NSString *const CommandExecutedNotification = @"CommandExecutedNotification";

#pragma mark - Object

- (id)init
{
    @throw ([NSException exceptionWithName:@"Wrong init-method" reason:@"Use -initWithIP:withPort:" userInfo:nil]);
}

- (id)initWithIP:(uint32_t)ip port:(uint16_t)port
{
    self = [super init];
    if (self)
    {
		mControl = new WyLight::ControlNoThrow(ip,port);
		self.threadOfOwner = [NSThread currentThread];
		mFunction = NULL;
		ctrlIsRunning = new std::atomic<bool>(true);
		mCmdQueue	= new std::deque<std::function<uint32_t(void)>>();
		gCtrlMutex = new std::mutex();
		gCmdQueueMutex = new std::mutex();
		gFunctionMutex = new std::mutex();
		mFirstCtrlThread = new std::thread([=]	{
													uint32_t retVal;
													std::function<uint32_t(void)> call;
													while(ctrlIsRunning)
													{
														if(!(mCmdQueue->empty()))
														{
															{
																std::lock_guard<std::mutex> queueLock(*gCmdQueueMutex);
																call = mCmdQueue->front();
																mCmdQueue->pop_front();
															}
															{
																std::lock_guard<std::mutex> ctrlLock(*gCtrlMutex);
																retVal = call();
															}
															if(retVal != WyLight::NO_ERROR)
															{
																[self performSelector:@selector(callFatalErrorDelegate:) onThread:[self threadOfOwner] withObject:[NSNumber numberWithUnsignedInt:retVal] waitUntilDone:NO];
															}
														}
													}
												}
											);
		mSecondCtrlThread = new std::thread([=]	{
													uint32_t retVal;
													while(ctrlIsRunning)
													{
														if(mFunction != NULL)
														{
															{
																std::lock_guard<std::mutex> funcLock(*gFunctionMutex);
																std::lock_guard<std::mutex> ctrlLock(*gCtrlMutex);
																retVal = (*mFunction)();
															
																delete mFunction;
																mFunction = NULL;
															}
															
															if(retVal != WyLight::NO_ERROR)
															{
																[self performSelector:@selector(callFatalErrorDelegate:) onThread:[self threadOfOwner] withObject:[NSNumber numberWithUnsignedInt:retVal] waitUntilDone:NO];
															}
															else
															{
																[self performSelector:@selector(postNotification) onThread:[self threadOfOwner] withObject:nil waitUntilDone:NO];
															}

														}
													}
												 }
											);
	}
    return self;
}



-(void)dealloc
{
	ctrlIsRunning = false;
	mFirstCtrlThread->join();
	mSecondCtrlThread->join();
	
	delete ctrlIsRunning;
	delete mFirstCtrlThread;
	delete mSecondCtrlThread;
	delete mCmdQueue;
	delete gCmdQueueMutex;
	delete gCtrlMutex;
	delete gFunctionMutex;
	delete mControl;
	
	if(mFunction != NULL) delete mFunction;
    
	mFunction = NULL;
	ctrlIsRunning = NULL;
	mFirstCtrlThread = NULL;
	mSecondCtrlThread = NULL;
	mCmdQueue = NULL;
	gCtrlMutex = NULL;
	gFunctionMutex = NULL;
	gCmdQueueMutex = NULL;
	mControl = NULL;
}

#pragma mark - Configuration WLAN-Module

- (void)configurateWlanModuleAsClientForNetwork:(NSString *)ssid password:(NSString *)password name:(NSString *)name
{
    const std::string ssidCString([ssid cStringUsingEncoding:NSASCIIStringEncoding]);
    const std::string passwordCString([password cStringUsingEncoding:NSASCIIStringEncoding]);
	const std::string nameCString([name cStringUsingEncoding:NSASCIIStringEncoding]);
    
	std::lock_guard<std::mutex> lock(*gFunctionMutex);
	mFunction = new std::function<uint32_t(void)>(std::bind(&WyLight::ControlNoThrow::ConfModuleForWlan, std::ref(*mControl), passwordCString, ssidCString, nameCString));
}

- (void)configurateWlanModuleAsSoftAP:(NSString *)ssid
{
    const std::string ssidCString([ssid cStringUsingEncoding:NSASCIIStringEncoding]);
	
	std::lock_guard<std::mutex> lock(*gFunctionMutex);
	mFunction = new std::function<uint32_t(void)>(std::bind(&WyLight::ControlNoThrow::ConfModuleAsSoftAP, std::ref(*mControl), ssidCString));
}

- (void)rebootWlanModul
{
	std::lock_guard<std::mutex> lock(*gFunctionMutex);
	mFunction = new std::function<uint32_t(void)>(std::bind(&WyLight::ControlNoThrow::ConfRebootWlanModule, std::ref(*mControl)));
}

#pragma mark - Firmware methods

- (void)setColorDirect:(UIColor *)newColor
{
    float redPart;
    float greenPart;
    float bluePart;
    [newColor getRed:&redPart green:&greenPart blue:&bluePart alpha:nil];
    
    int sizeColorArray = NUM_OF_LED * 3;
    
    uint8_t colorArray[sizeColorArray];
    uint8_t *pointer = colorArray;
    
    for (int i = 0; i < sizeColorArray; i++)
    {
        switch (i%3)
        {
            case 0:
                *pointer++ = (uint8_t)(bluePart * 255);
                break;
            case 1:
                *pointer++ = (uint8_t)(greenPart * 255);
                break;
            case 2:
                *pointer++ = (uint8_t)(redPart * 255);
                break;
        }
    }
    
	[self setColorDirect:colorArray bufferLength:sizeColorArray];
}

- (void)setColorDirect:(const uint8_t*)pointerBuffer bufferLength:(size_t)length
{
	std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_front(std::bind(&WyLight::ControlNoThrow::FwSetColorDirect, std::ref(*mControl), pointerBuffer, length));
}

- (void)setWaitTimeInTenMilliSecondsIntervals:(uint16_t)time
{
	std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwSetWait, std::ref(*mControl), time));
}

- (void)setFade:(uint32_t)colorInARGB
{
	[self setFade:colorInARGB time:0];
}

- (void)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue
{
	[self setFade:colorInARGB time:timeValue address:0xffffffff];
}

- (void)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue address:(uint32_t)address
{
    [self setFade:colorInARGB time:timeValue address:address parallelFade:false];
}

- (void)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue address:(uint32_t)address parallelFade:(BOOL)parallel
{
    std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwSetFade, std::ref(*mControl), colorInARGB, timeValue, address, parallel));
}

- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB
{
    [self setGradientWithColor:colorOneInARGB colorTwo:colorTwoInARGB time:0];
}

- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue
{
    [self setGradientWithColor:colorOneInARGB colorTwo:colorTwoInARGB time:timeValue parallelFade:false];
}

- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel
{
    [self setGradientWithColor:colorOneInARGB colorTwo:colorTwoInARGB time:timeValue parallelFade:parallel gradientLength:NUM_OF_LED];
}

- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel gradientLength:(uint8_t)length
{
    [self setGradientWithColor:colorOneInARGB colorTwo:colorTwoInARGB time:timeValue parallelFade:parallel gradientLength:length startPosition:0];
}

- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel gradientLength:(uint8_t)length startPosition:(uint8_t)offset
{
	std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwSetGradient, std::ref(*mControl), colorOneInARGB, colorTwoInARGB, timeValue, parallel, length, offset));
}

- (void)loopOn
{
	std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwLoopOn, std::ref(*mControl)));
}

- (void)loopOffAfterNumberOfRepeats:(uint8_t)repeats
{
    std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwLoopOff, std::ref(*mControl), repeats)); // 0: Endlosschleife / 255: Maximale Anzahl
}

- (void)clearScript
{
    std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwClearScript, std::ref(*mControl)));
}

- (void)readRtcTime:(NSDate **)date
{
    struct tm timeInfo;
	std::lock_guard<std::mutex> lock(*gCtrlMutex);
	
    uint32_t returnValue = mControl->FwGetRtc(timeInfo);
    
    if(returnValue != WyLight::NO_ERROR)
	{
		[self callFatalErrorDelegate:[NSNumber numberWithUnsignedInt:returnValue]];
		*date = nil;
	}
	else
		*date = [NSDate dateWithTimeIntervalSince1970:mktime(&timeInfo)];
}

- (void)writeRtcTime
{
    //NSDate *date = [NSDate date];
    //NSTimeZone = [NSTimeZone locald]
    
    NSTimeInterval timeInterval = [[NSDate date] timeIntervalSince1970];
    struct tm* timeInfo;
    time_t rawTime = (time_t)timeInterval;
    
    //time(&rawTime);
    timeInfo = localtime(&rawTime);
    
	std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_front(std::bind(&WyLight::ControlNoThrow::FwSetRtc, std::ref(*mControl), *timeInfo));
}

- (void)readCurrentFirmwareVersionFromFirmware:(NSString **)currentFirmwareVersionStringPlaceholder
{
    std::string firmwareVersionString;
    
	std::lock_guard<std::mutex> lock(*gCtrlMutex);
    uint32_t returnValue = mControl->FwGetVersion(firmwareVersionString);
    
    if(returnValue != WyLight::NO_ERROR)
	{
		[self callFatalErrorDelegate:[NSNumber numberWithUnsignedInt:returnValue]];
		*currentFirmwareVersionStringPlaceholder = nil;
	}
	else
	{
		*currentFirmwareVersionStringPlaceholder = [NSString stringWithCString:firmwareVersionString.c_str() encoding:NSASCIIStringEncoding];
	}
}

- (void)enterBootloader
{
	std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->clear();
	mCmdQueue->push_front(std::bind(&WyLight::ControlNoThrow::FwStartBl, std::ref(*mControl)));
}

#pragma mark - Bootloader methods

- (void)readCurrentFirmwareVersionFromBootloder:(NSString **)currentFirmwareVersionStringPlaceholder
{
    std::string firmwareVersionString;
	std::lock_guard<std::mutex> lock(*gCtrlMutex);
    uint32_t returnValue = mControl->BlReadFwVersion(firmwareVersionString);
    *currentFirmwareVersionStringPlaceholder = [NSString stringWithCString:firmwareVersionString.c_str() encoding:NSASCIIStringEncoding];
    
    if(returnValue != WyLight::NO_ERROR)
	{
		[self callFatalErrorDelegate:[NSNumber numberWithUnsignedInt:returnValue]];
		*currentFirmwareVersionStringPlaceholder = nil;
	}
	else
	{
		*currentFirmwareVersionStringPlaceholder = [NSString stringWithCString:firmwareVersionString.c_str() encoding:NSASCIIStringEncoding];
	}

}

- (void)programFlash
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectoryPath = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectoryPath stringByAppendingPathComponent:@"main.hex"];
    
	std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::BlProgramFlash, std::ref(*mControl), std::string([filePath cStringUsingEncoding:NSASCIIStringEncoding])));
}

- (void)leaveBootloader
{
	std::lock_guard<std::mutex> lock(*gCmdQueueMutex);
	mCmdQueue->clear();
	mCmdQueue->push_front(std::bind(&WyLight::ControlNoThrow::BlRunApp, std::ref(*mControl)));
}

- (void)postNotification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:CommandExecutedNotification object:self];
}

- (void)callFatalErrorDelegate:(NSNumber*)errorCode
{
	NSLog(@"ErrorCode %@", errorCode);
	if([errorCode intValue] == WyLight::SCRIPT_FULL)
	{
		[delegate scriptFullErrorOccured:self errorCode:errorCode];
	}
	else
	{
		[delegate fatalErrorOccured:self errorCode:errorCode];
	}
}

@end
