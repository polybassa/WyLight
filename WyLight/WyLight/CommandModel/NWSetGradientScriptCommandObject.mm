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

#define COLOR1_KEY @"WyLightRemote.NWSetGradientScriptCommandObject.color1"
#define COLOR2_KEY @"WyLightRemote.NWSetGradientScriptCommandObject.color2"
#define OFFSET_KEY @"WyLightRemote.NWSetGradientScriptCommandObject.offset"
#define NUM_OF_LED_KEY @"WyLightRemote.NWSetGradientScriptCommandObject.numberOfLeds"

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	if (self) {
		_color1 = [aDecoder decodeObjectForKey:COLOR1_KEY];
		_color2 = [aDecoder decodeObjectForKey:COLOR2_KEY];
		_offset = [aDecoder decodeInt32ForKey:OFFSET_KEY];
		_numberOfLeds = [aDecoder decodeInt32ForKey:NUM_OF_LED_KEY];
	}
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
	[super encodeWithCoder:aCoder];
	[aCoder encodeObject:_color1 forKey:COLOR1_KEY];
	[aCoder encodeObject:_color2 forKey:COLOR2_KEY];
	[aCoder encodeInt32:_offset forKey:OFFSET_KEY];
	[aCoder encodeInt32:_numberOfLeds forKey:NUM_OF_LED_KEY];
}

- (BOOL)isEqual:(id)object {
	if ([object isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		NWSetGradientScriptCommandObject *otherGradient = (NWSetGradientScriptCommandObject *)object;
		if (otherGradient.duration == self.duration &&
			[otherGradient.color1 isEqual:self.color1] &&
			[otherGradient.color2 isEqual:self.color2] &&
			self.offset == otherGradient.offset &&
			self.numberOfLeds == otherGradient.numberOfLeds) {
			return [super isEqual:object];
		}
	}
	return NO;
}

- (id)copyWithZone:(NSZone *)zone {
	NWSetGradientScriptCommandObject *other = [[NWSetGradientScriptCommandObject alloc]init];
	other.parallel = self.parallel;
	other.offset = self.offset;
	other.numberOfLeds = self.numberOfLeds;
	other.backgroundColor = [self.backgroundColor copyWithZone:zone];
	other.color1 = [self.color1 copyWithZone:zone];
	other.color2 = [self.color2 copyWithZone:zone];
	other.duration = self.duration;
	return other;
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
	WyLight::WiflyColor color1, color2;
	
	CGFloat r, g, b, a;
	[self.color1 getRed:&r green:&g blue:&b alpha:&a];
	
	color1.red((uint8_t)(r * 255));
	color1.green((uint8_t)(g * 255));
	color1.blue((uint8_t)(b * 255));
	
	[self.color2 getRed:&r green:&g blue:&b alpha:&a];
	
	color2.red((uint8_t)(r * 255));
	color2.green((uint8_t)(g * 255));
	color2.blue((uint8_t)(b * 255));
	
	[control setGradientWithColor:color1.argb() colorTwo:color2.argb() time:self.duration parallelFade:self.parallel gradientLength:self.numberOfLeds startPosition:self.offset];
}

- (void)setOffset:(uint8_t)offset {
	if (offset + self.numberOfLeds <= [NWSetGradientScriptCommandObject maximalNumberOfLeds] && offset >= 0) {
		_offset = offset;
	}
}

- (void)setNumberOfLeds:(uint8_t)numberOfLeds {
	if ((self.offset + numberOfLeds) <= [NWSetGradientScriptCommandObject maximalNumberOfLeds]) {
		_numberOfLeds = numberOfLeds;
	} else if (self.offset > 0 && numberOfLeds <= [NWSetGradientScriptCommandObject maximalNumberOfLeds])
	{
		_offset = [NWSetGradientScriptCommandObject maximalNumberOfLeds] - numberOfLeds;
		_numberOfLeds = numberOfLeds;
	}
	if (!_numberOfLeds) {
		_numberOfLeds = 1;
	}
}

- (NSArray *)colors {
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
	
	const uint8_t offset = self.offset;
	const uint8_t numOfLeds = self.numberOfLeds;
	
	const uint8_t endPosition = offset + numOfLeds;
	
	for (unsigned int i = 0; i < [NWSetGradientScriptCommandObject maximalNumberOfLeds]; i++) {
		if (i >= endPosition) {
			[mutableColorArray addObject:self.backgroundColor];
		} else if (i == (endPosition - 1 )) {
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
			[mutableColorArray addObject:self.backgroundColor];
		}
	}
	return mutableColorArray;
}

- (uint32_t)address {
	const uint8_t offset = self.offset;
	const uint8_t numOfLeds = self.numberOfLeds;
	const uint8_t endPosition = offset + numOfLeds;
	
	uint32_t outputBitmask = 0x00;
	uint32_t compareBitmask = 0x00000001;
	
	for (unsigned int i = 0; i < [NWSetGradientScriptCommandObject maximalNumberOfLeds]; i++) {
		if (i >= endPosition) {
			//is empty
		} else if (i == (endPosition - 1)) {
			outputBitmask = outputBitmask | compareBitmask;
		} else if (i >= offset) {
			outputBitmask = outputBitmask | compareBitmask;
		}
		compareBitmask = compareBitmask << 1;
	}
	return outputBitmask;

}

+ (NSUInteger)maximalNumberOfLeds {
	return NUM_OF_LED;
}

@end
