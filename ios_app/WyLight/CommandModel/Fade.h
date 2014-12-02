//
//  Fade.h
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "SimpelEffect.h"


@interface Fade : SimpelEffect

@property (nonatomic, retain) UIColor * color;

+ (NSString *)entityName;
+ (instancetype)insertNewObjectIntoContext:(NSManagedObjectContext *)context;

@end
