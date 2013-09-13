//
//  NWSetFadeScriptCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWSetFadeScriptCommandObject.h"
#include "wifly_cmd.h"
#include "WiflyColor.h"
#import "WCWiflyControlWrapper.h"

@implementation NWSetFadeScriptCommandObject

- (BOOL)isEqual:(id)object {
	if ([object isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
		NWSetFadeScriptCommandObject *otherFade = (NWSetFadeScriptCommandObject *)object;
		if (otherFade.duration == self.duration && self.address == otherFade.address && [self.color isEqual:otherFade.color]) {
			return [super isEqual:object];
		}
	}
	return NO;
}

- (id)copyWithZone:(NSZone *)zone {
	NWSetFadeScriptCommandObject *other = [[NWSetFadeScriptCommandObject alloc]init];
	other.parallel = self.parallel;
	other.backgroundColor = [self.backgroundColor copyWithZone:zone];
	other.color = [self.color copyWithZone:zone];
	other.address = self.address;
	other.duration = self.duration;
	return other;
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
	WyLight::WiflyColor color;
	
	CGFloat r, g, b, a;
	[self.color getRed:&r green:&g blue:&b alpha:&a];
	
	color.red((uint8_t)(r * 255));
	color.green((uint8_t)(g * 255));
	color.blue((uint8_t)(b * 255));
	
	[control setFade:color.argb() time:self.duration address:self.address parallelFade:self.parallel];
}

- (NSArray *)colors {
	NSMutableArray *mutableColorArray = [[NSMutableArray alloc]init];
	uint32_t compareMask = 0x00000001;
	
	for (int i = 0; i < NUM_OF_LED; i++) {
		if (compareMask & self.address) {
			[mutableColorArray addObject:[self.color copy]];
		} else {
			[mutableColorArray addObject:[self.backgroundColor copy]];
		}
		compareMask = compareMask << 1;
	}
	return mutableColorArray;
}

@end
