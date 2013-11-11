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
    if (self.repeatsWhenFinished) {
        [control loopOffAfterNumberOfRepeats:0];
    } else {
        [control loopOffAfterNumberOfRepeats:1];
    }
}

- (void)insertObject:(ComplexEffect *)value inEffectsAtIndex:(NSUInteger)idx {
    NSMutableOrderedSet *set = [[NSMutableOrderedSet alloc] initWithOrderedSet:self.effects];
    ComplexEffect *prev, *next;
    if (self.effects.count > (idx + 1)) {
        next = [self.effects objectAtIndex:idx + 1];
    }
    if (idx > 0) {
        prev = [self.effects objectAtIndex:idx - 1];
    }
    
    value.next = next;
    value.prev = prev;
    [set insertObject:value atIndex:idx];
    self.effects = set;
}

- (void)removeObjectFromEffectsAtIndex:(NSUInteger)idx {
    ComplexEffect *tempObj = [self.effects objectAtIndex:idx];
	if (tempObj.prev) {
		tempObj.prev.next = tempObj.next;
	}
	if (tempObj.next) {
		tempObj.next.prev = tempObj.prev;
	}
    NSMutableOrderedSet *set = [[NSMutableOrderedSet alloc] initWithOrderedSet:self.effects];
    [set removeObjectAtIndex:idx];
    self.effects = set;
}

- (void)insertEffects:(NSArray *)value atIndexes:(NSIndexSet *)indexes {
    NSUInteger i = [indexes firstIndex];
    for (ComplexEffect *effect in value) {
        [self insertObject:effect inEffectsAtIndex:i];
        i = [indexes indexGreaterThanIndex:i];
    }
}

- (void)removeEffectsAtIndexes:(NSIndexSet *)indexes {
    NSUInteger idx = [indexes firstIndex];
    for (int i = 0; i < indexes.count; i++) {
        [self removeObjectFromEffectsAtIndex:idx];
        idx = [indexes indexGreaterThanIndex:idx];
    }
}

- (void)replaceObjectInEffectsAtIndex:(NSUInteger)idx withObject:(ComplexEffect *)value {
    ComplexEffect *oldEffect = [self.effects objectAtIndex:idx];
    value.next = oldEffect.next;
    value.prev = oldEffect.prev;
    
    NSMutableOrderedSet *set = [[NSMutableOrderedSet alloc] initWithOrderedSet:self.effects];
    [set replaceObjectAtIndex:idx withObject:value];
    self.effects = set;
}

- (void)replaceEffectsAtIndexes:(NSIndexSet *)indexes withEffects:(NSArray *)values {
    NSUInteger i = [indexes firstIndex];
    for (ComplexEffect *effect in values) {
        [self replaceObjectInEffectsAtIndex:i withObject:effect];
        i = [indexes indexGreaterThanIndex:i];
    }
}

- (void)addEffectsObject:(ComplexEffect *)value {
    ComplexEffect *last = self.effects.lastObject;
    value.prev = last;
    last.next = value;
    value.next = nil;
    
    NSMutableOrderedSet *set = [[NSMutableOrderedSet alloc] initWithOrderedSet:self.effects];
    [set addObject:value];
    self.effects = set;
}

- (void)addEffects:(NSOrderedSet *)values {
    for (ComplexEffect *effect in values) {
        [self addEffectsObject:effect];
    }
}

- (void)removeEffects:(NSOrderedSet *)values {
    NSMutableOrderedSet *set = [[NSMutableOrderedSet alloc] initWithOrderedSet:self.effects];
    for (ComplexEffect *effect in values) {
        NSUInteger index = [set indexOfObject:effect];
        [self removeObjectFromEffectsAtIndex:index];
    }
    self.effects = set;
}

@end
