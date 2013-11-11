//
//  ComplexEffect.h
//  WyLight
//
//  Created by Nils Weiß on 04/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Effect.h"

@class ComplexEffect, Script, SimpelEffect;

@interface ComplexEffect : Effect

+ (NSString *)entityName;
+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context;

@property (nonatomic, retain) NSNumber * waitCommand;
@property (nonatomic, retain) ComplexEffect *next;
@property (nonatomic, retain) ComplexEffect *prev;
@property (nonatomic, retain) Script *script;
@property (nonatomic, retain) NSOrderedSet *effects;
@end

@interface ComplexEffect (CoreDataGeneratedAccessors)

- (void)insertObject:(SimpelEffect *)value inEffectsAtIndex:(NSUInteger)idx;
- (void)removeObjectFromEffectsAtIndex:(NSUInteger)idx;
- (void)insertEffects:(NSArray *)value atIndexes:(NSIndexSet *)indexes;
- (void)removeEffectsAtIndexes:(NSIndexSet *)indexes;
- (void)replaceObjectInEffectsAtIndex:(NSUInteger)idx withObject:(SimpelEffect *)value;
- (void)replaceEffectsAtIndexes:(NSIndexSet *)indexes withEffects:(NSArray *)values;
- (void)addEffectsObject:(SimpelEffect *)value;
- (void)removeEffectsObject:(SimpelEffect *)value;
- (void)addEffects:(NSOrderedSet *)values;
- (void)removeEffects:(NSOrderedSet *)values;
@end
