//
//  ComplexEffect.h
//  WyLight
//
//  Created by Nils Weiß on 14/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Effect.h"

@class Script, SimpelEffect;

@interface ComplexEffect : Effect

+ (NSString *)entityName;
+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context;
+ (instancetype)insertNewComplexEffectIntoScript:(Script *)script;

- (ComplexEffect *)prev;
- (ComplexEffect *)next;

- (void)prepareForSendToWCWiflyControl;

@property (nonatomic, retain) NSNumber * waitCommand;
@property (nonatomic, retain) NSOrderedSet *effects;
@property (nonatomic, retain) Script *script;
@end

