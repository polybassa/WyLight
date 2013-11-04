//
//  Gradient.m
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Gradient.h"
#import "WCWiflyControlWrapper.h"
#include "WiflyColor.h"
#include "wifly_cmd.h"

@implementation Gradient

@dynamic color1;
@dynamic color2;
@dynamic offset;
@dynamic numberOfLeds;

+ (NSString *)entityName {
    return @"Gradient";
}

+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context {
    return [NSEntityDescription insertNewObjectForEntityForName:[self entityName]
                                         inManagedObjectContext:context];
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
    WyLight::WiflyColor mColor1, mColor2;
	   
	CGFloat r, g, b, a;
	[self.color1 getRed:&r green:&g blue:&b alpha:&a];
	
	mColor1.red((uint8_t)(r * 255));
	mColor1.green((uint8_t)(g * 255));
	mColor1.blue((uint8_t)(b * 255));
    
	[self.color2 getRed:&r green:&g blue:&b alpha:&a];
	
	mColor2.red((uint8_t)(r * 255));
	mColor2.green((uint8_t)(g * 255));
	mColor2.blue((uint8_t)(b * 255));
    
    [control setGradientWithColor:mColor1.argb()
                         colorTwo:mColor2.argb()
                             time:self.duration.unsignedIntegerValue
                     parallelFade:self.parallel.boolValue
                   gradientLength:self.numberOfLeds.unsignedIntegerValue
                    startPosition:self.offset.unsignedIntegerValue];
}

- (NSNumber *)address {
    const uint8_t offset = self.offset.unsignedIntegerValue;
	const uint8_t numOfLeds = self.numberOfLeds.unsignedIntegerValue;
	const uint8_t endPosition = offset + numOfLeds;
	
	uint32_t outputBitmask = 0x00;
	uint32_t compareBitmask = 0x00000001;
	
	for (unsigned int i = 0; i < [Gradient maximalNumberOfLeds]; i++) {
		if (i >= endPosition) {
			//is empty
		} else if (i == (endPosition - 1)) {
			outputBitmask = outputBitmask | compareBitmask;
		} else if (i >= offset) {
			outputBitmask = outputBitmask | compareBitmask;
		}
		compareBitmask = compareBitmask << 1;
	}
	return @(outputBitmask);
}

+ (NSUInteger)maximalNumberOfLeds {
	return NUM_OF_LED;
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
	
	const uint8_t offset = self.offset.unsignedIntegerValue;
	const uint8_t numOfLeds = self.numberOfLeds.unsignedIntegerValue;
	
	const uint8_t endPosition = offset + numOfLeds;
	
	for (unsigned int i = 0; i < [Gradient maximalNumberOfLeds]; i++) {
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
			startRed += deltaRed / (self.numberOfLeds.unsignedIntegerValue);
			startGreen += deltaGreen / (self.numberOfLeds.unsignedIntegerValue);
			startBlue += deltaBlue / (self.numberOfLeds.unsignedIntegerValue);
		} else {
			[mutableColorArray addObject:self.backgroundColor];
		}
	}
	return mutableColorArray;
}

#pragma mark - Custom Setter
- (void)setOffset:(NSNumber *)offset {
    if (offset.unsignedIntegerValue + self.numberOfLeds.unsignedIntegerValue <= [Gradient maximalNumberOfLeds] && offset >= 0) {
		[self willChangeValueForKey:@"offset"];
        [self setOffset:offset];
        [self didChangeValueForKey:@"offset"];
	}
}

- (void)setNumberOfLeds:(NSNumber *)numberOfLeds {
    if ((self.offset.unsignedIntegerValue + numberOfLeds.unsignedIntegerValue) <= [Gradient maximalNumberOfLeds]) {
        [self willChangeValueForKey:@"numberOfLeds"];
        [self setNumberOfLeds:numberOfLeds];
        [self didChangeValueForKey:@"numberOfLeds"];
    }
    else if (self.offset.unsignedIntegerValue > 0 && numberOfLeds.unsignedIntegerValue <= [Gradient maximalNumberOfLeds])
	{
        self.offset = @([Gradient maximalNumberOfLeds] - numberOfLeds.unsignedIntegerValue);
        
		[self willChangeValueForKey:@"numberOfLeds"];
        [self setNumberOfLeds:numberOfLeds];
        [self didChangeValueForKey:@"numberOfLeds"];
	}
}
@end