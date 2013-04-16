//
//  WCWiflyControlWrapper.h
//  WiflyCommander
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WCWiflyControlWrapper : NSObject

- (id)initWithIP:(uint32_t)ip withPort:(uint16_t)port;
- (uint32_t)setWlanSsid:(NSString *)ssid password:(NSString *)password;
- (uint32_t)setDefaultConfiguration;

- (uint32_t)setColorDirect:(const uint8_t*)pointerBuffer bufferLength:(size_t)length;
- (uint32_t)setWaitTimeInTenMilliSecondsIntervals:(uint16_t)time;
- (uint32_t)setFade:(uint32_t)colorInARGB;
- (uint32_t)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue;
- (uint32_t)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue address:(uint32_t)address;
- (uint32_t)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue address:(uint32_t)address parallelFade:(BOOL)parallel;
- (uint32_t)loopOn;
- (uint32_t)loopOffWithNumberOfRepeats:(uint8_t)repeats;
- (uint32_t)clearScript;
- (uint32_t)readRtcTime:(NSDate **)date;
- (uint32_t)writeRtcTime;
- (uint32_t)readCurrentFirmwareVersionFromFirmware:(NSString **)currentFirmwareVersionStringPlaceholder;
- (uint32_t)enterBootloader;

- (uint32_t)readCurrentFirmwareVersionFromBootloder:(NSString **)currentFirmwareVersionStringPlaceholder;
- (uint32_t)programFlash;
- (uint32_t)leaveBootloader;

@end
