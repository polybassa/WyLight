//
//  WCWiflyControlWrapper.h
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres, Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol WCWiflyControlDelegate;

@interface WCWiflyControlWrapper : NSObject {}

#if !__has_feature(objc_arc)
@property (nonatomic, unsafe_unretained) id <WCWiflyControlDelegate> delegate;
#else
@property (nonatomic, weak) id <WCWiflyControlDelegate> delegate;
#endif

// Configuration
- (id)initWithIP:(uint32_t)ip port:(uint16_t)port;

/**
 * Attention: After executing one of the next three command's you have to dealloc your WCWiflyControlWrapper object
 * The CommandExecutedNotification tell's you the succesfull execution of a command
 */
- (void)configurateWlanModuleAsClientForNetwork:(NSString *)ssid password:(NSString *)password name:(NSString *)name;
- (void)configurateWlanModuleAsSoftAP:(NSString *)ssid;
- (void)rebootWlanModul;

// Firmware methods
- (void)setColorDirect:(UIColor *)newColor;
- (void)setColorDirect:(const uint8_t*)pointerBuffer bufferLength:(size_t)length;
- (void)setWaitTimeInTenMilliSecondsIntervals:(uint16_t)time;
- (void)setFade:(uint32_t)colorInARGB;
- (void)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue;
- (void)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue address:(uint32_t)address;
- (void)setFade:(uint32_t)colorInARGB time:(uint16_t)timeValue address:(uint32_t)address parallelFade:(BOOL)parallel;
- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB;
- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue;
- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel;
- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel gradientLength:(uint8_t)length;
- (void)setGradientWithColor:(uint32_t)colorOneInARGB colorTwo:(uint32_t)colorTwoInARGB time:(uint16_t)timeValue parallelFade:(BOOL)parallel gradientLength:(uint8_t)length startPosition:(uint8_t)offset;

- (void)loopOn;
- (void)loopOffAfterNumberOfRepeats:(uint8_t)repeats;
- (void)clearScript;
- (void)readRtcTime:(NSDate **)date;
- (void)writeRtcTime;
- (void)readCurrentFirmwareVersionFromFirmware:(NSString **)currentFirmwareVersionStringPlaceholder;

- (void)enterBootloader;

// Bootloader methods
- (void)readCurrentFirmwareVersionFromBootloder:(NSString **)currentFirmwareVersionStringPlaceholder;
- (void)programFlash;
- (void)leaveBootloader;

- (void)disconnect;

@end

@protocol WCWiflyControlDelegate

@optional
// dealloc this object to disconnet, if this error occure
- (void) fatalErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber *)errorCode;

// send clearscript to target to fix this error
- (void) scriptFullErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber*)errorCode;

// This delegate tells you that a wlan configuration command finished successfull. !!!! dealloc this object if you receive this message.
// Internally the control object has to disconnect after a wlan configuration command
- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender;
@end


