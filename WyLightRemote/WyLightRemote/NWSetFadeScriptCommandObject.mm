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

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control
{
	WyLight::WiflyColor color;
	
	CGFloat r, g, b, a;
	[self.color getRed:&r green:&g blue:&b alpha:&a];
	
	color.red((uint8_t)r);
	color.green((uint8_t)g);
	color.blue((uint8_t)b);
	
	[control setFade:color.argb() time:self.duration address:self.address parallelFade:self.parallel];
}

- (NSArray *)colors
{
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
