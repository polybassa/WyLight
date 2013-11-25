//
//  Script.h
//  WyLight
//
//  Created by Nils Weiß on 25/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Command.h"

@class ComplexEffect;

@interface Script : Command

+ (NSString *)entityName;
+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context;

- (NSNumber *)totalDurationInTmms;

@property (nonatomic, retain) NSNumber * repeatsWhenFinished;
@property (nonatomic, retain) UIImage* snapshot;
@property (nonatomic, retain) NSString * title;
@property (nonatomic, retain) UIImage* outdatedSnapshot;
@property (nonatomic, retain) NSOrderedSet *effects;
@end
