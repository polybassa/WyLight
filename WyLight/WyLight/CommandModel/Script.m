//
//  Script.m
//  WyLight
//
//  Created by Nils Weiß on 04/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Script.h"
#import "ComplexEffect.h"
#import "WCWiflyControlWrapper.h"

@implementation Script

@dynamic title;
@dynamic repeatsWhenFinished;
@dynamic effects;
@dynamic snapshot;

+ (NSString *)entityName {
    return @"Script";
}

+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context {
    return [NSEntityDescription insertNewObjectForEntityForName:[self entityName]
                                         inManagedObjectContext:context];
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
