//
//  WCEndpoint.m
//  WyLightExample
//
//  Created by Nils Weiß on 03.06.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "WCEndpoint.h"
#import "WCBroadcastReceiverWrapper.h"

@implementation WCEndpoint

- (id) initWithIpAdress:(uint32_t)ip port:(uint16_t)port name:(NSString *)name score:(uint8_t)score
{
	self = [super init];
	if(self) {
		_name = name;
		_port = port;
		_ipAdress = ip;
		_score = score;
		_adressString = [NSString stringWithFormat:@"%d.%d.%d.%d : %d", (self.ipAdress >> 24) & 0xff, (self.ipAdress >> 16) & 0xff, (self.ipAdress >> 8) & 0xff, (self.ipAdress >> 0) & 0xff, self.port];
		return self;
	}
	return nil;
}

-(id)initWithEndpointFromWCBroadcastReceiverWrapper:(WCBroadcastReceiverWrapper*)receiver atIndex:(size_t)index
{
	self = [super init];
	if(self) {
		_name = [receiver deviceNameOfTarget:index];
		_port = [receiver portOfTarget:index];
		_ipAdress = [receiver ipAdressOfTarget:index];
		_score = [receiver scoreOfTarget:index];
		_adressString = [NSString stringWithFormat:@"%d.%d.%d.%d : %d", (self.ipAdress >> 24) & 0xff, (self.ipAdress >> 16) & 0xff, (self.ipAdress >> 8) & 0xff, (self.ipAdress >> 0) & 0xff, self.port];
		return self;
	}
	return nil;
}

@end
