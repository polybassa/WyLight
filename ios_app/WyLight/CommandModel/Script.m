//
//  Script.m
//  WyLight
//
//  Created by Nils Weiß on 25/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Script.h"
#import "ComplexEffect.h"
#import "WCWiflyControlWrapper.h"

@implementation Script

@dynamic repeatsWhenFinished;
@dynamic snapshot;
@dynamic title;
@dynamic outdatedSnapshot;
@dynamic effects;

+ (NSString *)entityName {
    return @"Script";
}

+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context {
    return [NSEntityDescription insertNewObjectForEntityForName:[self entityName]
                                         inManagedObjectContext:context];
}

- (void)setSnapshot:(UIImage *)snapshot {
    UIImage* snapshotBeforSet = self.snapshot;
    
    [self willChangeValueForKey:@"snapshot"];
    [self setPrimitiveValue:snapshot forKey:@"snapshot"];
    [self didChangeValueForKey:@"snapshot"];
    
    if (snapshotBeforSet) {
        [self setOutdatedSnapshot:snapshotBeforSet];
    }
}

- (void)setRepeatsWhenFinished:(NSNumber *)repeatsWhenFinished {
	[self willAccessValueForKey:@"repeatsWhenFinished"];
	NSNumber* oldValue = [self primitiveValueForKey:@"repeatsWhenFinished"];
	[self didAccessValueForKey:@"repeatsWhenFinished"];

	if (oldValue.unsignedIntegerValue != repeatsWhenFinished.unsignedIntegerValue) {
		[self willChangeValueForKey:@"repeatsWhenFinished"];
		[self setPrimitiveValue:repeatsWhenFinished forKey:@"repeatsWhenFinished"];
		[self didChangeValueForKey:@"repeatsWhenFinished"];

		for (ComplexEffect* effect in self.effects) {
			effect.colors = [effect computeColors];
		}
		((ComplexEffect *)self.effects.firstObject).colors = nil;
	}
}

- (NSNumber *)totalDurationInTmms {
    NSUInteger totalDuration = 0;
	for (ComplexEffect *obj in self.effects) {
		totalDuration = totalDuration + obj.duration.unsignedIntegerValue;
	}
	return @(totalDuration);
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
    [control clearScript];
    [control loopOn];
    for (ComplexEffect* command in self.effects) {
        [command sendToWCWiflyControl:control];
    }
    if (self.repeatsWhenFinished.boolValue) {
        [control loopOffAfterNumberOfRepeats:0];
    } else {
        [control loopOffAfterNumberOfRepeats:1];
    }
}


@end
