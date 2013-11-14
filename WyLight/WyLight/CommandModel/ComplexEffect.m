//
//  ComplexEffect.m
//  WyLight
//
//  Created by Nils Weiß on 14/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "ComplexEffect.h"
#import "Script.h"
#import "SimpelEffect.h"
#import "WCWiflyControlWrapper.h"
#import "wifly_cmd.h"


@implementation ComplexEffect

@dynamic waitCommand;
@dynamic effects;
@dynamic script;


+ (NSString *)entityName {
    return @"ComplexEffect";
}

+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context {
    return [NSEntityDescription insertNewObjectForEntityForName:[self entityName]
                                         inManagedObjectContext:context];
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
    if (self.waitCommand.boolValue) {
        [control setWaitTimeInTenMilliSecondsIntervals:self.duration.unsignedIntegerValue];
    } else {
        [self prepareForSendToWCWiflyControl];
        for (id command in self.effects) {
            if ([command respondsToSelector:@selector(sendToWCWiflyControl:)]) {
                [command sendToWCWiflyControl:control];
            }
        }
    }
}

- (void)prepareForSendToWCWiflyControl {
    for (SimpelEffect *command in self.effects) {
        if ([command isKindOfClass:[SimpelEffect class]]) {
            [command setParallel:@(YES)];
            [command setDuration:self.duration];
        }
    }
    [[self.effects lastObject] setParallel:@(NO)];
}

- (void)setBackgroundColor:(UIColor *)backgroundColor {
    
    [self willChangeValueForKey:@"backgroundColor"];
    [self setPrimitiveValue:backgroundColor forKey:@"backgroundColor"];
    [self didChangeValueForKey:@"backgroundColor"];
    
    for (SimpelEffect *command in self.effects) {
        command.backgroundColor = backgroundColor;
    }
}

- (ComplexEffect *)prev {
    NSUInteger currentIdx = [self.script.effects indexOfObject:self];
    if (currentIdx == 0) {
        return nil;
    }
    ComplexEffect *prev = [self.script.effects objectAtIndex:currentIdx - 1];
    return prev != self ? prev : nil;
}

- (ComplexEffect *)next {
    NSUInteger currentIdx = [self.script.effects indexOfObject:self];
    currentIdx++;
    if (currentIdx >= self.script.effects.count) {
        return nil;
    }
    ComplexEffect *next = [self.script.effects objectAtIndex:currentIdx];
    return next != self ? next : nil;
}

- (NSArray *)colors {
	if (self.waitCommand.boolValue) {
		if (self.prev.colors) {
			return self.prev.colors;
		}
		NSMutableArray *outPutColors = [[NSMutableArray alloc] init];
		for (NSUInteger i = 0; i < NUM_OF_LED; i++) {
			[outPutColors addObject:self.backgroundColor];
		}
		return outPutColors;
	}
	NSMutableArray *outPutColors;
	if (self.prev) {
		outPutColors = [[NSMutableArray alloc]initWithArray:self.prev.colors copyItems:YES];
	} else {
		outPutColors = [[NSMutableArray alloc]init];
	}
    
	uint32_t compareMask = 0x00000001;
	for (unsigned int i = 0; i < NUM_OF_LED; i++) {  //i = 0 - 31
		NSUInteger j = self.effects.count;
		while (j--) {
			SimpelEffect *currentObj = [self.effects objectAtIndex:j];
			const uint32_t bitmask = currentObj.address.unsignedIntegerValue;
            if (bitmask & compareMask) {
                if (self.prev) {
					[outPutColors replaceObjectAtIndex:i withObject:currentObj.colors[i]];
				} else {
					[outPutColors addObject:currentObj.colors[i]];
				}
                break;
			}
		}
		if (i >= outPutColors.count) {
			[outPutColors addObject:self.backgroundColor];
		}
		compareMask = compareMask << 1;
	}
	return [outPutColors copy];
}


@end
