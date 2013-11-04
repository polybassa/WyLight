//
//  ComplexEffect.h
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Effect.h"

@class ComplexEffect;

@interface ComplexEffect : Effect

@property (nonatomic, retain) NSNumber * waitCommand;
@property (nonatomic, retain) NSOrderedSet *effects;
@property (nonatomic, retain) ComplexEffect *next;
@property (nonatomic, retain) ComplexEffect *prev;
@end

@interface ComplexEffect (CoreDataGeneratedAccessors)

- (void)insertObject:(NSManagedObject *)value inEffectsAtIndex:(NSUInteger)idx;
- (void)removeObjectFromEffectsAtIndex:(NSUInteger)idx;
- (void)insertEffects:(NSArray *)value atIndexes:(NSIndexSet *)indexes;
- (void)removeEffectsAtIndexes:(NSIndexSet *)indexes;
- (void)replaceObjectInEffectsAtIndex:(NSUInteger)idx withObject:(NSManagedObject *)value;
- (void)replaceEffectsAtIndexes:(NSIndexSet *)indexes withEffects:(NSArray *)values;
- (void)addEffectsObject:(NSManagedObject *)value;
- (void)removeEffectsObject:(NSManagedObject *)value;
- (void)addEffects:(NSOrderedSet *)values;
- (void)removeEffects:(NSOrderedSet *)values;
@end
