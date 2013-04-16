//
//  WCBroadcastReceiverWrapper.h
//  WiflyCommander
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WCBroadcastReceiverWrapper : NSObject

- (size_t)numberOfTargets;
- (uint32_t)ipAdressOfTarget:(size_t)index;
- (uint16_t)portOfTarget:(size_t)index;
- (NSString *)deviceNameOfTarget:(size_t)index;

@end
