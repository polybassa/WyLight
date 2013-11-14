//
//  Gradient.h
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "SimpelEffect.h"


@interface Gradient : SimpelEffect

+ (NSString *)entityName;
+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context;
+ (NSUInteger)maximalNumberOfLeds;

@property (nonatomic, retain) UIColor * color1;
@property (nonatomic, retain) UIColor * color2;
@property (nonatomic, retain) NSNumber * numberOfLeds;
@property (nonatomic, retain) NSNumber * offset;

@end
