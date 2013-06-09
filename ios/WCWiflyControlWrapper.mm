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
#include <functional>
#include "MessageQueue.h"
#include <tuple>

typedef std::function<uint32_t(void)> ControlCommand;
//tupel <1> == true: terminate task; tuple <2>: command to execute; tuple<3> == 0: no notification after execution command, == 1: notification after execution command
typedef std::tuple<bool, ControlCommand, unsigned int> ControlMessage;

@interface WCWiflyControlWrapper () {
	std::shared_ptr<WyLight::ControlNoThrow> mControl;
	std::shared_ptr<std::thread> mCtrlThread;
	std::shared_ptr<std::mutex> gCtrlMutex;
	std::shared_ptr<WyLight::MessageQueue<ControlMessage>> mCmdQueue;
}

#if !__has_feature(objc_arc)
@property (nonatomic, unsafe_unretained) NSThread* threadOfOwner;
#else
@property (nonatomic, weak) NSThread* threadOfOwner;
#endif

-(void) callFatalErrorDelegate:(NSNumber*)errorCode;
-(void) callWiflyControlHasDisconnectedDelegate;

@end

@implementation WCWiflyControlWrapper

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
		NSLog(@"Start WCWiflyControlWrapper\n");
		mControl = std::make_shared<WyLight::ControlNoThrow>(ip,port);
		self.threadOfOwner = [NSThread currentThread];
		gCtrlMutex = std::make_shared<std::mutex>();
		mCmdQueue = std::make_shared<WyLight::MessageQueue<ControlMessage>>();
		mCtrlThread = std::make_shared<std::thread>([=]{
													uint32_t retVal;
													while(true)
													{
														auto tup = mCmdQueue->receive();
														
														if(std::get<0>(tup))
														{
															NSLog(@"WCWiflyControlWrapper: Terminate runLoop\n");
															break;
														}
														
														{	std::lock_guard<std::mutex> ctrlLock(*gCtrlMutex);
															retVal = std::get<1>(tup)(); }
														
														if(retVal != WyLight::NO_ERROR)
														{
															[self performSelector:@selector(callFatalErrorDelegate:) onThread:self.threadOfOwner withObject:[NSNumber numberWithUnsignedInt:retVal] waitUntilDone:NO];
														}
														else if(retVal == WyLight::NO_ERROR && std::get<2>(tup) == 1)	//do we have to notify after execution?
														{
															[self performSelector:@selector(callWiflyControlHasDisconnectedDelegate) onThread:self.threadOfOwner withObject:nil waitUntilDone:NO];
														}
													}
												}
											);
	}
    return self;
}

- (void)disconnect
{
	if(mCmdQueue && mCtrlThread) {
		NSLog(@"Disconnect WCWiflyControlWrapper\n");
		mCmdQueue->sendOnlyThis(std::make_tuple(true, [=]{return 0xdeadbeef;}, 0));
		mCtrlThread->join();
	}
	
	mCtrlThread.reset();
	mCmdQueue.reset();
	gCtrlMutex.reset();
	mControl.reset();
}

- (void)dealloc
{
	NSLog(@"Dealloc WCWiflyControlWrapper\n");
	[self disconnect];
#if !__has_feature(objc_arc)
    //Do manual memory management...
	[super dealloc];
#else
    //Usually do nothing...
#endif
}

#pragma mark - Configuration WLAN-Module

- (void)configurateWlanModuleAsClientForNetwork:(NSString *)ssid password:(NSString *)password name:(NSString *)name
{
    const std::string ssidCString([ssid cStringUsingEncoding:NSASCIIStringEncoding]);
    const std::string passwordCString([password cStringUsingEncoding:NSASCIIStringEncoding]);
	const std::string nameCString([name cStringUsingEncoding:NSASCIIStringEncoding]);
    
	mCmdQueue->sendOnlyThis(std::make_tuple(false,
											std::bind(&WyLight::ControlNoThrow::ConfModuleForWlan, std::ref(*mControl), passwordCString, ssidCString, nameCString),
											1));
}

- (void)configurateWlanModuleAsSoftAP:(NSString *)ssid
{
    const std::string ssidCString([ssid cStringUsingEncoding:NSASCIIStringEncoding]);
	
	mCmdQueue->sendOnlyThis(std::make_tuple(false,
											std::bind(&WyLight::ControlNoThrow::ConfModuleAsSoftAP, std::ref(*mControl), ssidCString),
											1));
}

- (void)rebootWlanModul
{
	mCmdQueue->sendOnlyThis(std::make_tuple(false,
											std::bind(&WyLight::ControlNoThrow::ConfRebootWlanModule, std::ref(*mControl)),
											1));
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
	mCmdQueue->sendDirect(std::make_tuple(false,
										  std::bind(&WyLight::ControlNoThrow::FwSetColorDirect, std::ref(*mControl), pointerBuffer, length),
										  0));
}

- (void)setWaitTimeInTenMilliSecondsIntervals:(uint16_t)time
{
	mCmdQueue->send(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwSetWait, std::ref(*mControl), time),
									0));
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
	mCmdQueue->send(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwSetFade, std::ref(*mControl), colorInARGB, timeValue, address, parallel),
									0));
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
	mCmdQueue->send(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwSetGradient, std::ref(*mControl), colorOneInARGB, colorTwoInARGB, timeValue, parallel, length, offset),
									0));
}

- (void)loopOn
{
	mCmdQueue->send(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwLoopOn, std::ref(*mControl)),
									0));
}

- (void)loopOffAfterNumberOfRepeats:(uint8_t)repeats
{
	mCmdQueue->send(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwLoopOff, std::ref(*mControl), repeats),
									0)); // 0: Endlosschleife / 255: Maximale Anzahl
}

- (void)clearScript
{
    mCmdQueue->send(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwClearScript, std::ref(*mControl)),
									0));
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
    
	mCmdQueue->sendDirect(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwSetRtc, std::ref(*mControl), *timeInfo),
									0));
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
	mCmdQueue->sendOnlyThis(std::make_tuple(false,
										  std::bind(&WyLight::ControlNoThrow::FwStartBl, std::ref(*mControl)),
										  0));
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
	NSString *filePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"main" ofType:@"hex"]; //Whatever your file - extension is

	mCmdQueue->sendOnlyThis(std::make_tuple(false,
										  std::bind(&WyLight::ControlNoThrow::BlProgramFlash, std::ref(*mControl), std::string([filePath cStringUsingEncoding:NSASCIIStringEncoding])),
										  0));
}

- (void)leaveBootloader
{
	mCmdQueue->sendOnlyThis(std::make_tuple(false,
											std::bind(&WyLight::ControlNoThrow::BlRunApp, std::ref(*mControl)),
											0));
}

- (void)callFatalErrorDelegate:(NSNumber*)errorCode
{
	NSLog(@"ErrorCode %@", errorCode);
	if([errorCode intValue] == WyLight::SCRIPT_FULL)
	{
		[_delegate scriptFullErrorOccured:self errorCode:errorCode];
	}
	else
	{
		[_delegate fatalErrorOccured:self errorCode:errorCode];
	}
}

- (void)callWiflyControlHasDisconnectedDelegate
{
	[_delegate wiflyControlHasDisconnected:self];
}

@end
