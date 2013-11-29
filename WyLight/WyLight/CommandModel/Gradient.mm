//
//  Gradient.m
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Gradient.h"
#import "WCWiflyControlWrapper.h"
#import	"UIColor+argbValue.h"
#include "wifly_cmd.h"
#import "ComplexEffect.h"

@implementation Gradient

@dynamic color1;
@dynamic color2;
@dynamic offset;
@dynamic numberOfLeds;

- (id)copy {
    Gradient *newGradient = [Gradient insertNewObjectIntoContext:self.managedObjectContext];
    newGradient.color1 = self.color1;
    newGradient.color2 = self.color2;
    newGradient.offset = self.offset;
    newGradient.numberOfLeds = self.numberOfLeds;
    newGradient.duration = self.duration;
    newGradient.backgroundColor = self.backgroundColor;
    return newGradient;
}

+ (NSString *)entityName {
    return @"Gradient";
}

+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context {
    return [NSEntityDescription insertNewObjectForEntityForName:[self entityName]
                                         inManagedObjectContext:context];
}

- (void)resetCalculatedAttributes {
    [self willChangeValueForKey:@"colors"];
    [self setPrimitiveValue:nil forKey:@"colors"];
    [self didChangeValueForKey:@"colors"];
    [self willChangeValueForKey:@"address"];
    [self setPrimitiveValue:nil forKey:@"address"];
    [self didChangeValueForKey:@"address"];
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
    [control setGradientWithColor:[self.color1 getARGB]
                         colorTwo:[self.color2 getARGB]
                             time:self.duration.unsignedIntegerValue
                     parallelFade:self.parallel.boolValue
                   gradientLength:self.numberOfLeds.unsignedIntegerValue
                    startPosition:self.offset.unsignedIntegerValue];
}

- (void)setBackgroundColor:(UIColor *)backgroundColor {
    [self willAccessValueForKey:@"backgroundColor"];
    UIColor *oldBackground = [self primitiveValueForKey:@"backgroundColor"];
    [self didAccessValueForKey:@"backgroundColor"];
    
    if ([oldBackground isEqual:backgroundColor]) {
        return;
    }    
    
    [self willChangeValueForKey:@"backgroundColor"];
    [self setPrimitiveValue:backgroundColor forKey:@"backgroundColor"];
    [self didChangeValueForKey:@"backgroundColor"];
    
    [self setColors:nil];
}

- (NSNumber *)address {
    [self willAccessValueForKey:@"address"];
    NSNumber *address = [self primitiveValueForKey:@"address"];
    [self didAccessValueForKey:@"address"];
    
    if (address == nil || address.unsignedIntegerValue == 0) {
        NSLog(@"Gradient calc address");
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
        address = @(outputBitmask);
        [self willChangeValueForKey:@"address"];
        [self setAddress:address];
        [self willChangeValueForKey:@"address"];
        
        if (self.complexEffect) {
            [self.complexEffect setColors:nil];
        }
    }
    return address;
}

+ (NSUInteger)maximalNumberOfLeds {
	return NUM_OF_LED;
}

+ (NSUInteger)minimalNumberOfLeds {
    return 1;
}

- (NSArray *)colors {
    [self willAccessValueForKey:@"colors"];
    NSArray *colors = [self primitiveValueForKey:@"colors"];
    [self didAccessValueForKey:@"colors"];
    
    if (colors == nil) {
        NSLog(@"Gradient calc Colors");
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
        colors = [mutableColorArray copy];
        [self willChangeValueForKey:@"colors"];
        [self setPrimitiveValue:colors forKey:@"colors"];
        [self didChangeValueForKey:@"colors"];
        
        if (self.complexEffect) {
            [self.complexEffect setColors:nil];
        }
    }
    return colors;
}

#pragma mark - Custom Setter
- (void)setOffset:(NSNumber *)offset {
    if (offset.unsignedIntegerValue + self.numberOfLeds.unsignedIntegerValue <= [Gradient maximalNumberOfLeds] && offset >= 0) {
		[self willChangeValueForKey:@"offset"];
        [self setPrimitiveValue:offset forKey:@"offset"];
        [self didChangeValueForKey:@"offset"];
        
        [self resetCalculatedAttributes];
	}
}

- (void)setNumberOfLeds:(NSNumber *)numberOfLeds {
    if ((self.offset.unsignedIntegerValue + numberOfLeds.unsignedIntegerValue) <= [Gradient maximalNumberOfLeds] && numberOfLeds.unsignedIntegerValue >= [Gradient minimalNumberOfLeds]) {
        [self willChangeValueForKey:@"numberOfLeds"];
        [self setPrimitiveValue:numberOfLeds forKey:@"numberOfLeds"];
        [self didChangeValueForKey:@"numberOfLeds"];
        
        [self resetCalculatedAttributes];
    }
    else if (self.offset.unsignedIntegerValue > 0 && numberOfLeds.unsignedIntegerValue <= [Gradient maximalNumberOfLeds] && numberOfLeds.unsignedIntegerValue >= [Gradient minimalNumberOfLeds])
	{
        self.offset = @([Gradient maximalNumberOfLeds] - numberOfLeds.unsignedIntegerValue);
        
        [self resetCalculatedAttributes];
    }
}

- (void)setColor1:(UIColor *)color1 {
    [self willChangeValueForKey:@"color1"];
    [self setPrimitiveValue:color1 forKey:@"color1"];
    [self didChangeValueForKey:@"color1"];

    [self resetCalculatedAttributes];
}

- (void)setColor2:(UIColor *)color2 {
    [self willChangeValueForKey:@"color2"];
    [self setPrimitiveValue:color2 forKey:@"color2"];
    [self didChangeValueForKey:@"color2"];
    
    [self resetCalculatedAttributes];
}
@end