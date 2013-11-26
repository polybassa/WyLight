//
//  WCBroadcastReceiverWrapper.h
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres, Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WCEndpoint;

@interface WCBroadcastReceiverWrapper : NSObject

	FOUNDATION_EXPORT NSString *const TargetsChangedNotification;

@property (nonatomic, strong) NSArray *targets; //of WCEndpoint

- (void)setWCEndpointAsFavorite:(WCEndpoint *)endpoint;

- (void)unsetWCEndpointAsFavorite:(WCEndpoint *)endpoint;

- (void)saveTargets;

- (void)start;

- (void)stop;

- (void)clearTargets;

@end
