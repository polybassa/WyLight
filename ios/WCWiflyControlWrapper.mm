//
//  WCWiflyControlWrapper.m
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres, Nils Weiß. All rights reserved.
//

#import "WCWiflyControlWrapper.h"
#import "WCEndpoint.h"
#include "WiflyControlNoThrow.h"
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

@property (nonatomic, strong) WCEndpoint *endpoint;

-(void) callFatalErrorDelegate:(NSNumber *)errorCode;
-(void) callWiflyControlHasDisconnectedDelegate;

@end

@implementation WCWiflyControlWrapper

#pragma mark - Object

- (id)init
{
    @throw ([NSException exceptionWithName:@"Wrong init-method" reason:@"Use -initWithEndpoint:establishConnection:" userInfo:nil]);
}

- (id)initWithWCEndpoint:(WCEndpoint *)endpoint establishConnection:(BOOL)connect
{
    self = [super init];
    if (self)
    {
		self.endpoint = endpoint;
		if (connect) {
			if ([self connect] != 0) {
				return nil;
			}
		}
	}
    return self;
}

- (int)connect
{
	NSLog(@"Start WCWiflyControlWrapper\n");
	try {
		mControl = std::make_shared<WyLight::ControlNoThrow>(self.endpoint.ipAdress,self.endpoint.port);
	} catch (std::exception &e) {
		NSLog(@"%s", e.what());
		return -1;
	}
	gCtrlMutex = std::make_shared<std::mutex>();
	mCmdQueue = std::make_shared<WyLight::MessageQueue<ControlMessage>>();
	mCmdQueue->setMessageLimit(15);
	mCtrlThread = std::make_shared<std::thread>
	([=]{
		uint32_t retVal;
		while(true)
		{
			auto tup = mCmdQueue->receive();
		
			if(std::get<0>(tup))
			{
				NSLog(@"WCWiflyControlWrapper: Terminate runLoop\n");
				break;
			}
			{
				std::lock_guard<std::mutex> ctrlLock(*gCtrlMutex);
				dispatch_async(dispatch_get_main_queue(), ^{
					[self setNetworkActivityIndicatorVisible:YES];
				});
				retVal = std::get<1>(tup)();
				
				double delayInSeconds = 0.3;
				dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
				dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
					[self setNetworkActivityIndicatorVisible:NO];
				});
			}
		
			if(retVal != WyLight::NO_ERROR)
			{
				dispatch_async(dispatch_get_main_queue(), ^{
					[self callFatalErrorDelegate:[NSNumber numberWithUnsignedInt:retVal]];
				});
			}
			else if(retVal == WyLight::NO_ERROR && std::get<2>(tup) == 1)	//do we have to notify after execution?
			{
				dispatch_async(dispatch_get_main_queue(), ^{
					[self callWiflyControlHasDisconnectedDelegate];
				});
			}
		}
	});
	return 0;
}

- (void)disconnect
{
	dispatch_async(dispatch_queue_create("disconnectQ", NULL), ^{
		if(mCmdQueue && mCtrlThread) {
			NSLog(@"Disconnect WCWiflyControlWrapper\n");
			
			mCmdQueue->clear_and_push_front(std::make_tuple(true, [=]{return 0xdeadbeef;}, 0));
			mCtrlThread->join();
		}
		
		mCtrlThread.reset();
		mCmdQueue.reset();
		gCtrlMutex.reset();
		mControl.reset();

	});
}

- (void)dealloc
{
	NSLog(@"Dealloc WCWiflyControlWrapper\n");
}

- (void)setNetworkActivityIndicatorVisible:(BOOL)setVisible {
    static NSInteger NumberOfCallsToSetVisible = 0;
    if (setVisible)
        NumberOfCallsToSetVisible++;
    else
        NumberOfCallsToSetVisible--;
	
    // The assertion helps to find programmer errors in activity indicator management.
    // Since a negative NumberOfCallsToSetVisible is not a fatal error,
    // it should probably be removed from production code.
    NSAssert(NumberOfCallsToSetVisible >= 0, @"Network Activity Indicator was asked to hide more often than shown");
    
    // Display the indicator as long as our static counter is > 0.
    [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:(NumberOfCallsToSetVisible > 0)];
}

#pragma mark - Configuration WLAN-Module

- (void)configurateWlanModuleAsClientForNetwork:(NSString *)ssid password:(NSString *)password name:(NSString *)name
{
    const std::string ssidCString([ssid cStringUsingEncoding:NSASCIIStringEncoding]);
    const std::string passwordCString([password cStringUsingEncoding:NSASCIIStringEncoding]);
	const std::string nameCString([name cStringUsingEncoding:NSASCIIStringEncoding]);
    
	mCmdQueue->push_back(std::make_tuple(false,
											std::bind(&WyLight::ControlNoThrow::ConfModuleForWlan, std::ref(*mControl), passwordCString, ssidCString, nameCString),
											1));
}

- (void)configurateWlanModuleAsSoftAP:(NSString *)ssid
{
    const std::string ssidCString([ssid cStringUsingEncoding:NSASCIIStringEncoding]);
	
	mCmdQueue->push_back(std::make_tuple(false,
											std::bind(&WyLight::ControlNoThrow::ConfModuleAsSoftAP, std::ref(*mControl), ssidCString),
											1));
}

- (void)configurateWlanModuleChannelAsync:(BOOL)async
{
	if (async) {
		mCmdQueue->push_front(std::make_tuple(false,
										 std::bind(&WyLight::ControlNoThrow::ConfChangeWlanChannel, std::ref(*mControl)),
										 0));
	} else {
		
		std::lock_guard<std::mutex> lock(*gCtrlMutex);
		uint32_t returnValue = mControl->ConfChangeWlanChannel();

	}
}

- (void)rebootWlanModul
{
	mCmdQueue->push_back(std::make_tuple(false,
											std::bind(&WyLight::ControlNoThrow::ConfRebootWlanModule, std::ref(*mControl)),
											1));
}

#pragma mark - Firmware methods

- (void)setColorDirectWithColors:(NSArray *)newColors
{
	CGFloat redPart;
    CGFloat greenPart;
    CGFloat bluePart;
	CGFloat alphaPart;
    
    
	size_t sizeColorArray = MIN(NUM_OF_LED, newColors.count);
    
    uint8_t colorArray[NUM_OF_LED * 3];
    uint8_t *pointer = colorArray;
    
    for (int i = 0; i < NUM_OF_LED; i++)
    {
		redPart = 0;
		greenPart = 0;
		bluePart = 0;
		alphaPart = 0;
		
		if (newColors[i]) {
			[newColors[i] getRed:&redPart green:&greenPart blue:&bluePart alpha:&alphaPart];
		}
		else {
			[[UIColor blackColor] getRed:&redPart green:&greenPart blue:&bluePart alpha:&alphaPart];
		}
        
		*pointer++ = (uint8_t)(redPart * alphaPart * 255);
		*pointer++ = (uint8_t)(greenPart * alphaPart * 255);
		*pointer++ = (uint8_t)(bluePart * alphaPart * 255);
	}
    
	[self setColorDirect:colorArray bufferLength:sizeColorArray * 3];
}

- (void)setColorDirect:(UIColor *)newColor
{
    CGFloat redPart;
    CGFloat greenPart;
	CGFloat bluePart;
    [newColor getRed:&redPart green:&greenPart blue:&bluePart alpha:nil];
    
    const size_t sizeColorArray = NUM_OF_LED * 3;
    
    uint8_t colorArray[sizeColorArray];
    uint8_t *pointer = colorArray;
    
    for (int i = 0; i < sizeColorArray; i++)
    {
        switch (i%3)
        {
            case 2:
                *pointer++ = (uint8_t)(bluePart * 255);
                break;
            case 1:
                *pointer++ = (uint8_t)(greenPart * 255);
                break;
            case 0:
                *pointer++ = (uint8_t)(redPart * 255);
                break;
        }
    }
    
	[self setColorDirect:colorArray bufferLength:sizeColorArray];
}

- (void)setColorDirect:(const uint8_t*)pointerBuffer bufferLength:(size_t)length
{
	std::vector<uint8_t> buffer(pointerBuffer, pointerBuffer + length);
	mCmdQueue->push_back(std::make_tuple(false,
										  std::bind(&WyLight::ControlNoThrow::FwSetColorDirect, std::ref(*mControl), buffer),
										  0));
}

- (void)setWaitTimeInTenMilliSecondsIntervals:(uint16_t)time
{
	mCmdQueue->push_back(std::make_tuple(false,
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
	mCmdQueue->push_back(std::make_tuple(false,
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
	mCmdQueue->push_back(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwSetGradient, std::ref(*mControl), colorOneInARGB, colorTwoInARGB, timeValue, parallel, length, offset),
									0));
}

- (void)loopOn
{
	mCmdQueue->push_back(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwLoopOn, std::ref(*mControl)),
									0));
}

- (void)loopOffAfterNumberOfRepeats:(uint8_t)repeats
{
	mCmdQueue->push_back(std::make_tuple(false,
									std::bind(&WyLight::ControlNoThrow::FwLoopOff, std::ref(*mControl), repeats),
									0)); // 0: Endlosschleife / 255: Maximale Anzahl
}

- (void)clearScript
{
    mCmdQueue->push_back(std::make_tuple(false,
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
    
	mCmdQueue->push_front(std::make_tuple(false,
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
	mCmdQueue->clear_and_push_front(std::make_tuple(false,
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

- (void)eraseEepromAsync:(BOOL)async
{
	if (async) {
		mCmdQueue->push_back(std::make_tuple(false,
											 std::bind(&WyLight::ControlNoThrow::BlEraseEeprom, std::ref(*mControl)),
											 0));
	} else {
		std::lock_guard<std::mutex> lock(*gCtrlMutex);
		uint32_t returnValue = mControl->BlEraseEeprom();
		
		if(returnValue != WyLight::NO_ERROR)
		{
			[self callFatalErrorDelegate:[NSNumber numberWithUnsignedInt:returnValue]];
		}
	}
}

- (void)programFlashAsync:(BOOL)async
{
	NSString *filePath = [[NSBundle bundleForClass:[self class]] pathForResource:@"main" ofType:@"hex"]; //Whatever your file - extension is
	
	if (async) {
		mCmdQueue->push_back(std::make_tuple(false,
											 std::bind(&WyLight::ControlNoThrow::BlProgramFlash, std::ref(*mControl), std::string([filePath cStringUsingEncoding:NSASCIIStringEncoding])),
											 0));
	} else
	{
		std::lock_guard<std::mutex> lock(*gCtrlMutex);
		uint32_t returnValue = mControl->BlProgramFlash([filePath cStringUsingEncoding:NSASCIIStringEncoding]);
		
		if(returnValue != WyLight::NO_ERROR)
		{
			[self callFatalErrorDelegate:[NSNumber numberWithUnsignedInt:returnValue]];
		}
	}
}

- (void)leaveBootloader
{
	mCmdQueue->push_back(std::make_tuple(false,
											std::bind(&WyLight::ControlNoThrow::BlRunApp, std::ref(*mControl)),
											0));
}

- (void)callFatalErrorDelegate:(NSNumber*)errorCode
{
	NSLog(@"ErrorCode %@", errorCode);
	if([errorCode intValue] == WyLight::SCRIPT_FULL)
	{
		[self.delegate scriptFullErrorOccured:self errorCode:errorCode];
	}
	else
	{
		[self.delegate fatalErrorOccured:self errorCode:errorCode];
	}
}

- (void)callWiflyControlHasDisconnectedDelegate
{
	[self.delegate wiflyControlHasDisconnected:self];
}

@end
