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

@property (nonatomic) uint32_t ipAdress;
@property (nonatomic) uint16_t port;
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *adressString;
@property (nonatomic) uint8_t score;

- (id) initWithIpAdress:(uint32_t)ip port:(uint16_t)port name:(NSString *)name score:(uint8_t)score;
- (id) initWithEndpointFromWCBroadcastReceiverWrapper:(WCBroadcastReceiverWrapper*)receiver atIndex:(size_t)index;
@end
