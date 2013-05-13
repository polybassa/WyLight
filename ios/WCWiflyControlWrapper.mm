//
//  WCWiflyControlWrapper.m
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres. All rights reserved.
//

#import "WCWiflyControlWrapper.h"
#include "WiflyControlNoThrow.h"
#include <time.h>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <atomic>

@interface WCWiflyControlWrapper ()

@property (nonatomic) WyLight::ControlNoThrow *mControl;
@property (nonatomic) std::thread *mCtrlThread;
@property (nonatomic) std::mutex *gCtrlMutex, *gCmdQueueMutex;
@property (nonatomic) std::deque<std::function<uint32_t(void)>> *mCmdQueue;
@property (nonatomic) std::atomic<bool> *ctrlIsRunning;

-(void) postNotification;

@end

void executerFunction(WCWiflyControlWrapper* ctrl, NSThread* targetThread)
{
		while((ctrl.ctrlIsRunning))
		{
			if(!(ctrl.mCmdQueue->empty()))
			{
				ctrl.gCmdQueueMutex->lock();
				auto call = ctrl.mCmdQueue->front();
				ctrl.mCmdQueue->pop_front();
				ctrl.gCmdQueueMutex->unlock();
				
				ctrl.gCtrlMutex->lock();
				uint32_t retVal = call();
				ctrl.gCtrlMutex->unlock();
				
				if(retVal != WyLight::NO_ERROR)
				{
					[ctrl performSelector:@selector(postNotification) onThread:targetThread withObject:nil waitUntilDone:NO];
				}
			}
		}
}

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
		self.ctrlIsRunning = new std::atomic<bool>(true);
        self.mControl = new WyLight::ControlNoThrow(ip,port);
		self.mCmdQueue	= new std::deque<std::function<uint32_t(void)>>();
		self.gCtrlMutex = new std::mutex();
		self.gCmdQueueMutex = new std::mutex();
		NSThread* currentThread = [NSThread currentThread];
		self.mCtrlThread = new std::thread(std::bind(executerFunction, self, currentThread));
	}
    return self;
}

-(void)dealloc
{
	(*self.ctrlIsRunning) = false;
	(*self.mCtrlThread).join();
	
	delete self.ctrlIsRunning;
	delete self.mCtrlThread;
	delete self.mCmdQueue;
	delete self.gCmdQueueMutex;
	delete self.gCtrlMutex;
    delete self.mControl;
    
	self.ctrlIsRunning = NULL;
	self.mCtrlThread = NULL;
	self.mCmdQueue = NULL;
	self.gCtrlMutex = NULL;
	self.gCmdQueueMutex = NULL;
    self.mControl = NULL;
}

#pragma mark - Configuration WLAN-Module

- (void)configurateWlanModuleAsClient:(NSString *)ssid password:(NSString *)password name:(NSString *)name
{
    const std::string ssidCString([ssid cStringUsingEncoding:NSASCIIStringEncoding]);
    const std::string passwordCString([password cStringUsingEncoding:NSASCIIStringEncoding]);
	const std::string nameCString([name cStringUsingEncoding:NSASCIIStringEncoding]);
    
	std::lock_guard<std::mutex> lock(*(self.gCtrlMutex));
    uint32_t returnValue = (*self.mControl).ConfModuleForWlan(passwordCString, ssidCString, nameCString);
	
	if(returnValue != WyLight::NO_ERROR)
	{
		[self postNotification];
	}
}

- (void)configurateWlanModuleAsSoftAP:(NSString *)ssid
{
    const std::string ssidCString([ssid cStringUsingEncoding:NSASCIIStringEncoding]);
    
	std::lock_guard<std::mutex> lock(*(self.gCtrlMutex));
    uint32_t returnValue = (*self.mControl).ConfModuleAsSoftAP(ssidCString);
	
	if(returnValue != WyLight::NO_ERROR)
	{
		[self postNotification];
	}
}

- (uint32_t)rebootWlanModul
{
	std::lock_guard<std::mutex> lock(*(self.gCtrlMutex));
	uint32_t returnValue = (*self.mControl).ConfRebootWlanModule();
	
	if(returnValue != WyLight::NO_ERROR)
	{
		[self postNotification];
	}
}

#pragma mark - Firmware methods

- (void)setColorDirect:(UIColor *)newColor
{
    float redPart;
    float greenPart;
    float bluePart;
    [newColor getRed:&redPart green:&greenPart blue:&bluePart alpha:nil];
    
    int sizeColorArray = 32 * 3;
    
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
	std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwSetColorDirect, std::ref(*(self->_mControl)), pointerBuffer, length));
}

- (void)setWaitTimeInTenMilliSecondsIntervals:(uint16_t)time
{
	std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwSetWait, std::ref(*(self->_mControl)), time));
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
    std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwSetFade, std::ref(*(self->_mControl)), colorInARGB, timeValue, address, parallel));
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
	std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwSetGradient, std::ref(*(self->_mControl)), colorOneInARGB, colorTwoInARGB, timeValue, parallel, length, offset));
}

- (void)loopOn
{
	std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwLoopOn, std::ref(*(self->_mControl))));
}

- (void)loopOffAfterNumberOfRepeats:(uint8_t)repeats
{
    std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwLoopOff, std::ref(*(self->_mControl)), repeats)); // 0: Endlosschleife / 255: Maximale Anzahl
}

- (void)clearScript
{
    std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->push_back(std::bind(&WyLight::ControlNoThrow::FwClearScript, std::ref(*(self->_mControl))));
}

- (void)readRtcTime:(NSDate **)date
{
    struct tm timeInfo;
	std::lock_guard<std::mutex> lock(*(self.gCtrlMutex));
    uint32_t returnValue = (*self.mControl).FwGetRtc(timeInfo);
    
    
    if(returnValue != WyLight::NO_ERROR)
	{
		[self postNotification];
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
    
	std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->push_front(std::bind(&WyLight::ControlNoThrow::FwSetRtc, std::ref(*(self->_mControl)), *timeInfo));
}

- (void)readCurrentFirmwareVersionFromFirmware:(NSString **)currentFirmwareVersionStringPlaceholder
{
    std::string firmwareVersionString;
    
	std::lock_guard<std::mutex> lock(*(self.gCtrlMutex));
    uint32_t returnValue = (*self.mControl).FwGetVersion(firmwareVersionString);
    
    if(returnValue != WyLight::NO_ERROR)
	{
		[self postNotification];
		*currentFirmwareVersionStringPlaceholder = nil;
	}
	else
	{
		*currentFirmwareVersionStringPlaceholder = [NSString stringWithCString:firmwareVersionString.c_str() encoding:NSASCIIStringEncoding];
	}
}

- (void)enterBootloader
{
	std::lock_guard<std::mutex> lock(*(self.gCmdQueueMutex));
	self->_mCmdQueue->clear();
	self->_mCmdQueue->push_front(std::bind(&WyLight::ControlNoThrow::FwStartBl, std::ref(*(self->_mControl))));
}

#pragma mark - Bootloader methods

- (void)readCurrentFirmwareVersionFromBootloder:(NSString **)currentFirmwareVersionStringPlaceholder
{
    std::string firmwareVersionString;
	std::lock_guard<std::mutex> lock(*(self.gCtrlMutex));
    uint32_t returnValue = (*self.mControl).BlReadFwVersion(firmwareVersionString);
    *currentFirmwareVersionStringPlaceholder = [NSString stringWithCString:firmwareVersionString.c_str() encoding:NSASCIIStringEncoding];
    
    if(returnValue != WyLight::NO_ERROR)
	{
		[self postNotification];
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
    
	std::lock_guard<std::mutex> lock(*(self.gCtrlMutex));
    uint32_t returnValue = (*self.mControl).BlProgramFlash(std::string([filePath cStringUsingEncoding:NSASCIIStringEncoding]));
	
	if(returnValue != WyLight::NO_ERROR)
	{
		[self postNotification];
	}
}

- (void)leaveBootloader
{
	std::lock_guard<std::mutex> lock(*(self.gCtrlMutex));
    uint32_t returnValue = (*self.mControl).BlRunApp();
	
	if(returnValue != WyLight::NO_ERROR)
	{
		[self postNotification];
	}
}

- (void)postNotification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"FatalErrorNotification" object:self];
}

@end
