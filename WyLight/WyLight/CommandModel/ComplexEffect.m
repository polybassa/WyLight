//
//  ComplexEffect.m
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "ComplexEffect.h"
#import "WCWiflyControlWrapper.h"
#import "SimpelEffect.h"
#import "Effect.h"
#include "wifly_cmd.h"

@implementation ComplexEffect

@dynamic waitCommand;
@dynamic effects;
@dynamic next;
@dynamic prev;

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
    [self setBackgroundColor:backgroundColor];
    [self didChangeValueForKey:@"backgroundColor"];
    
    for (SimpelEffect *command in self.effects) {
        command.backgroundColor = self.backgroundColor;
    }
}

- (NSArray *)colors
{
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
			Effect *currentObj = [self.effects objectAtIndex:j];
			if ([currentObj respondsToSelector:@selector(address)]) {
				const uint32_t bitmask = (uint32_t)[currentObj performSelector:@selector(address)];
				if (bitmask & compareMask) {
					if (self.prev) {
						[outPutColors replaceObjectAtIndex:i withObject:currentObj.colors[i]];
					} else {
						[outPutColors addObject:currentObj.colors[i]];
					}
					break;
				}
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
