//
//  Effect.h
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Command.h"


@interface Effect : Command

@property (nonatomic, retain) UIColor * backgroundColor;
@property (nonatomic, retain) NSNumber * duration;

- (NSArray *)colors;

@end
