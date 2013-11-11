//
//  SimpelEffect.h
//  WyLight
//
//  Created by Nils Weiß on 04/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Effect.h"

@class ComplexEffect;

@interface SimpelEffect : Effect

@property (nonatomic, retain) NSNumber * address;
@property (nonatomic, retain) NSNumber * parallel;
@property (nonatomic, retain) ComplexEffect *complexEffect;

@end
