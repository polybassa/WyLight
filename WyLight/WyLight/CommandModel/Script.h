//
//  Script.h
//  WyLight
//
//  Created by Nils Weiß on 04/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Command.h"

@class ComplexEffect;

@interface Script : Command

+ (NSString *)entityName;
+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context;

@property (nonatomic, retain) NSNumber * repeatsWhenFinished;
@property (nonatomic, retain) NSString * title;
@property (nonatomic, retain) NSOrderedSet *effects;
@end

@interface Script (CoreDataGeneratedAccessors)

- (void)insertObject:(ComplexEffect *)value inEffectsAtIndex:(NSUInteger)idx;
- (void)removeObjectFromEffectsAtIndex:(NSUInteger)idx;
- (void)insertEffects:(NSArray *)value atIndexes:(NSIndexSet *)indexes;
- (void)removeEffectsAtIndexes:(NSIndexSet *)indexes;
- (void)replaceObjectInEffectsAtIndex:(NSUInteger)idx withObject:(ComplexEffect *)value;
- (void)replaceEffectsAtIndexes:(NSIndexSet *)indexes withEffects:(NSArray *)values;
- (void)addEffectsObject:(ComplexEffect *)value;
- (void)removeEffectsObject:(ComplexEffect *)value;
- (void)addEffects:(NSOrderedSet *)values;
- (void)removeEffects:(NSOrderedSet *)values;
@end
