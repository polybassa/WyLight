//
//  NWScriptCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptCommandObject.h"

@interface NWScriptCommandObject ()

@property (nonatomic, strong) UIColor *shadowBackgroundColor;

@end

@implementation NWScriptCommandObject

@synthesize backgroundColor = _backgroundColor;

#define DURATION_KEY @"WyLightRemote.NWScriptCommandObject.duration"
#define DEFAULT_DURATION 100
#define DEFAULT_BACKGROUNDCOLOR [UIColor blackColor]

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super init];
	if (self) {
		_duration = (uint16_t)[aDecoder decodeInt32ForKey:DURATION_KEY];
	}
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
	[aCoder encodeInt32:_duration forKey:DURATION_KEY];
}

- (uint16_t)duration {
	if (!_duration) {
		_duration = DEFAULT_DURATION;
	}
	return _duration;
}

- (NSArray*)colors {
	return nil;
}

- (UIColor *)backgroundColor {
	if (!self.shadowBackgroundColor) {
		self.shadowBackgroundColor = DEFAULT_BACKGROUNDCOLOR;
	}
	return self.shadowBackgroundColor;
}

- (void)setBackgroundColor:(UIColor *)backgroundColor {
	self.shadowBackgroundColor = backgroundColor;
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
	[[[NSException alloc] initWithName:@"Function not implemented" reason:@"Implement function sendToWCWiflyControl:" userInfo:nil] raise];
}

@end
