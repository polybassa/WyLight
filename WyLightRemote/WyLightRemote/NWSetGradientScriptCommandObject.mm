//
//  NWSetGradientScriptCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWSetGradientScriptCommandObject.h"
#import "WCWiflyControlWrapper.h"
#include "wifly_cmd.h"
#include "WiflyColor.h"

@implementation NWSetGradientScriptCommandObject

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control
{
	WyLight::WiflyColor color1, color2;
	
	CGFloat r, g, b, a;
	[self.color1 getRed:&r green:&g blue:&b alpha:&a];
	
	color1.red((uint8_t)r);
	color1.green((uint8_t)g);
	color1.blue((uint8_t)b);
	
	[self.color2 getRed:&r green:&g blue:&b alpha:&a];
	
	color2.red((uint8_t)r);
	color2.green((uint8_t)g);
	color2.blue((uint8_t)b);
	
	[control setGradientWithColor:color1.argb() colorTwo:color2.argb() time:self.duration parallelFade:self.parallel gradientLength:self.numberOfLeds startPosition:self.offset];
}

- (void)setOffset:(uint8_t)offset
{
	if (offset + self.numberOfLeds < NUM_OF_LED && offset >= 0) {
		_offset = offset;
	}
}

- (void)setNumberOfLeds:(uint8_t)numberOfLeds
{
	if ((self.offset + numberOfLeds) <= NUM_OF_LED) {
		_numberOfLeds = numberOfLeds;
	} else if (self.offset > 0 && numberOfLeds <= NUM_OF_LED)
	{
		_offset = NUM_OF_LED - numberOfLeds;
		_numberOfLeds = numberOfLeds;
	}
	if (_numberOfLeds < 1) {
		_numberOfLeds = 1;
	}
}

- (NSArray *)colors
{
	NSMutableArray *mutableColorArray = [[NSMutableArray alloc]init];
	CGFloat deltaRed, deltaGreen, deltaBlue, color1Red, color2Red, color1Green, color2Green, color1Blue, color2Blue;
	
	[self.color1 getRed:&color1Red green:&color1Green blue:&color1Blue alpha:NULL];
	[self.color2 getRed:&color2Red green:&color2Green blue:&color2Blue alpha:NULL];
	
	deltaRed = color2Red - color1Red;
	deltaGreen = color2Green - color1Green;
	deltaBlue =	color2Blue - color1Blue;
	
	float startRed, startGreen, startBlue;
	startRed = color1Red;
	startGreen = color1Green;
	startBlue = color1Blue;
	
	uint8_t offset = self.offset;
	uint8_t numOfLeds = self.numberOfLeds;
	
	const uint8_t endPosition = offset + numOfLeds;
	
	for (unsigned int i = 0; i < NUM_OF_LED; i++) {
		if (i > endPosition) {
			[mutableColorArray addObject:[self.backgroundColor copy]];
		} else if (i == endPosition ) {
			[mutableColorArray addObject:[UIColor colorWithRed:color2Red
														 green:color2Green
														  blue:color2Blue
														 alpha:1.0]];
			
		} else if (i >= offset) {
			[mutableColorArray addObject:[UIColor colorWithRed:startRed
														 green:startGreen
														  blue:startBlue
														 alpha:1.0]];
			startRed += deltaRed / (self.numberOfLeds);
			startGreen += deltaGreen / (self.numberOfLeds);
			startBlue += deltaBlue / (self.numberOfLeds);
		} else {
			[mutableColorArray addObject:[self.backgroundColor copy]];
		}
	}
	return mutableColorArray;
}

@end
