//
//  Fade.m
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Fade.h"
#import "WCWiflyControlWrapper.h"
#import "ComplexEffect.h"
#include "wifly_cmd.h"
#import "UIColor+argbValue.h"

@implementation Fade

@dynamic color;

- (id)copy {
    Fade *newFade = [Fade insertNewObjectIntoContext:self.managedObjectContext];
    newFade.color = self.color;
    newFade.address = self.address;
    newFade.backgroundColor = self.backgroundColor;
    newFade.duration = self.duration;
    return newFade;
}

+ (NSString *)entityName {
    return @"Fade";
}

+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context {
    return [NSEntityDescription insertNewObjectForEntityForName:[self entityName]
                                         inManagedObjectContext:context];
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
	[control setFade:[self.color getARGB] time:self.duration.unsignedIntegerValue address:self.address.unsignedIntValue parallelFade:self.parallel.boolValue];
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

- (void)setColor:(UIColor *)color {
    [self willChangeValueForKey:@"color"];
    [self setPrimitiveValue:color forKey:@"color"];
    [self didChangeValueForKey:@"color"];
    
    [self willChangeValueForKey:@"colors"];
    [self setPrimitiveValue:nil forKey:@"colors"];
    [self didChangeValueForKey:@"colors"];
}

- (void)setAddress:(NSNumber *)address {
    [self willChangeValueForKey:@"address"];
    [self setPrimitiveValue:address forKey:@"address"];
    [self didChangeValueForKey:@"address"];
    
    [self willChangeValueForKey:@"colors"];
    [self setPrimitiveValue:nil forKey:@"colors"];
    [self didChangeValueForKey:@"colors"];
}

- (NSArray *)colors {
    [self willAccessValueForKey:@"colors"];
    NSArray *colors = [self primitiveValueForKey:@"colors"];
    [self didAccessValueForKey:@"colors"];
    
    if (colors == nil)
    {
        NSLog(@"Fade calc colors");
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
        colors = mutableColorArray;
        [self willChangeValueForKey:@"colors"];
        [self setPrimitiveValue:colors forKey:@"colors"];
        [self didChangeValueForKey:@"colors"];
        
        if (self.complexEffect) {
            [self.complexEffect setColors:nil];
        }
    }
    return colors;
}

@end
