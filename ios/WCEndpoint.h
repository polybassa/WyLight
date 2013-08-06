//
//  WCEndpoint.h
//  WyLightExample
//
//  Created by Nils Weiß on 03.06.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WCBroadcastReceiverWrapper;

@interface WCEndpoint : NSObject

@property (nonatomic, readonly) uint32_t ipAdress;
@property (nonatomic, readonly) uint16_t port;
@property (nonatomic, strong, readonly) NSString *name;
@property (nonatomic, strong, readonly) NSString *adressString;
@property (nonatomic, readonly) uint8_t score;

- (id) initWithIpAdress:(uint32_t)ip port:(uint16_t)port name:(NSString *)name score:(uint8_t)score;

@end
