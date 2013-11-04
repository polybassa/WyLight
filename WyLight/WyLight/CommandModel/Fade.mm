//
//  Fade.m
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Fade.h"
#import "WCWiflyControlWrapper.h"
#include "WiflyColor.h"
#include "wifly_cmd.h"

@implementation Fade

@dynamic color;

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
	WyLight::WiflyColor color;
	
    UIColor *colorData = self.color;
    
	CGFloat r, g, b, a;
	[colorData getRed:&r green:&g blue:&b alpha:&a];
	
	color.red((uint8_t)(r * 255));
	color.green((uint8_t)(g * 255));
	color.blue((uint8_t)(b * 255));
	
	[control setFade:color.argb() time:self.duration.unsignedIntegerValue address:self.address.unsignedIntValue parallelFade:self.parallel.boolValue];
}

- (NSArray *)colors {
    NSMutableArray *mutableColorArray = [[NSMutableArray alloc]init];
	uint32_t compareMask = 0x00000001;
	
	for (int i = 0; i < NUM_OF_LED; i++) {
		if (compareMask & self.address.unsignedIntegerValue) {
			[mutableColorArray addObject:self.color];
		} else {
			[mutableColorArray addObject:self.backgroundColor];
		}
		compareMask = compareMask << 1;
	}
	return mutableColorArray;
}

@end
