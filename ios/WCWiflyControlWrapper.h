//
//  WCWiflyControlWrapper.h
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WCWiflyControlWrapper : NSObject

// Configuration
- (id)initWithIP:(uint32_t)ip port:(uint16_t)port;
- (uint32_t)configurateWlanModuleAsSoftAP:(NSString *)ssid password:(NSString *)password name:(NSString *)name;
- (uint32_t)configurateWlanModuleAsSoftAP:(NSString *)ssid;
- (uint32_t)rebootWlanModul;

// Firmware methods
- (uint32_t)setColorDirect:(UIColor *)newColor;
- (uint32_t)setColorDirect:(const uint8_t*)pointerBuffer bufferLength:(size_t)length;
- (uint32_t)setWaitTimeInTenMilliSecondsIntervals:(uint16_t)time;
- (uint32_t)setFade:(uint32_t)colorInARGB;
- (uint32_t)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue;
- (uint32_t)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue address:(uint32_t)address;
- (uint32_t)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue address:(uint32_t)address parallelFade:(BOOL)parallel;
- (uint32_t)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB;
- (uint32_t)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue;
- (uint32_t)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel;
- (uint32_t)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel gradientLength:(uint8_t)length;
- (uint32_t)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel gradientLength:(uint8_t)length startPosition:(uint8_t)offset;

- (uint32_t)loopOn;
- (uint32_t)loopOffAfterNumberOfRepeats:(uint8_t)repeats;
- (uint32_t)clearScript;
- (uint32_t)readRtcTime:(NSDate **)date;
- (uint32_t)writeRtcTime;
- (uint32_t)readCurrentFirmwareVersionFromFirmware:(NSString **)currentFirmwareVersionStringPlaceholder;

- (uint32_t)enterBootloader;

// Bootloader methods
- (uint32_t)readCurrentFirmwareVersionFromBootloder:(NSString **)currentFirmwareVersionStringPlaceholder;
- (uint32_t)programFlash;
- (uint32_t)leaveBootloader;

@end
