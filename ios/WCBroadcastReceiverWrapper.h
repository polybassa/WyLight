//
//  WCBroadcastReceiverWrapper.h
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres, Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WCBroadcastReceiverWrapper : NSObject

FOUNDATION_EXPORT NSString *const NewTargetAddedNotification;

- (size_t)numberOfTargets;
- (uint32_t)ipAdressOfTarget:(size_t)index;
- (uint16_t)portOfTarget:(size_t)index;
- (NSString *)deviceNameOfTarget:(size_t)index;
- (void)increaseScoreOfIpAdress:(uint32_t)ipAdress;
- (void)SetScoreOfIpAdress:(uint32_t)ipAdress Score:(uint8_t)score;
- (uint8_t)scoreOfTarget:(size_t)index;
- (void)saveTargets;
- (void)stop;
@end
