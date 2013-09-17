//
//  NWScriptEffectCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptEffectCommandObject.h"

@implementation NWScriptEffectCommandObject

#define PARALLEL_KEY @"WyLightRemote.NWScriptEffectCommandObject.parallel"

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	if (self) {
		_parallel = [aDecoder decodeBoolForKey:PARALLEL_KEY];
	}
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
	[super encodeWithCoder:aCoder];
	[aCoder encodeBool:_parallel forKey:PARALLEL_KEY];
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
	[[[NSException alloc] initWithName:@"Function not implemented" reason:@"Implement function sendToWCWiflyControl:" userInfo:nil] raise];
}

@end
