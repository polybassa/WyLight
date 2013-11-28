//
//  WCEndpoint.m
//  WyLightExample
//
//  Created by Nils Weiß on 03.06.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "WCEndpoint.h"
#import "WCBroadcastReceiverWrapper.h"

@interface WCEndpoint ()

@property (nonatomic, readwrite) uint32_t ipAdress;
@property (nonatomic, readwrite) uint16_t port;
@property (nonatomic, strong, readwrite) NSString *name;
@property (nonatomic, readwrite) uint8_t score;

@end

@implementation WCEndpoint

- (id) initWithIpAdress:(uint32_t)ip port:(uint16_t)port name:(NSString *)name score:(uint8_t)score
{
	self = [super init];
	if(self) {
		_name = name;
		_port = port;
		_ipAdress = ip;
		_score = score;
		return self;
	}
	return nil;
}

- (NSString *)adressString
{
	return [NSString stringWithFormat:@"%d.%d.%d.%d : %d",
		(self.ipAdress >> 24) & 0xff,
		(self.ipAdress >> 16) & 0xff,
		(self.ipAdress >> 8) & 0xff,
		(self.ipAdress >> 0) & 0xff,
		self.port];
}

@end
