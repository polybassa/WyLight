//
//  WCWiflyControlWrapper.h
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres, Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WCEndpoint;

@protocol WCWiflyControlDelegate;

@interface WCWiflyControlWrapper : NSObject {}

@property (nonatomic, weak) id <WCWiflyControlDelegate> delegate;

// Configuration
- (id)initWithWCEndpoint:(WCEndpoint *)endpoint establishConnection:(BOOL)connect doStartup:(BOOL)doStartup;

/**
 * ATTENTION: After executing one of the next three command's you have to disconnect your WCWiflyControlWrapper object
 * The CommandExecutedNotification tell's you the succesfull execution of a command
 */
- (void)configurateWlanModuleAsClientForNetwork:(NSString *)ssid password:(NSString *)password name:(NSString *)name;
- (void)configurateWlanModuleAsSoftAP:(NSString *)ssid;
- (void)rebootWlanModul;


// Firmware methods
- (void)setColorDirect:(UIColor *)newColor;
- (void)setColorDirectWithColors:(NSArray *)newColors;
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
- (NSDate *)readRtcTime;
- (void)writeRtcTime;

- (int)connectWithStartup:(BOOL)doStartup;
- (void)disconnect;

@end

@protocol WCWiflyControlDelegate

// dealloc this object to disconnet, if this error occure
- (void) wiflyControl:(WCWiflyControlWrapper *)sender fatalErrorOccured:(NSNumber *)errorCode;

// This delegate tells you that a wlan configuration command finished successfull. !!!! dealloc this object if you receive this message.
// Internally the control object has to disconnect after a wlan configuration command
- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender;

@optional
// send clearscript to target to fix this error
- (void) wiflyControl:(WCWiflyControlWrapper *)sender scriptBufferErrorOccured:(NSNumber *)errorCode;

- (void) wiflyControl:(WCWiflyControlWrapper *)sender connectionStartupStateChanged:(NSNumber *)state;
@end


