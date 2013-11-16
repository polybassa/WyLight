//
//  Effect.h
//  WyLight
//
//  Created by Nils Weiß on 16/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Command.h"

@interface Effect : Command

@property (nonatomic, retain) UIColor * backgroundColor;
@property (nonatomic, retain) NSNumber * duration;
@property (nonatomic, retain) NSArray * colors;
@property (nonatomic, retain) UIImage * snapshot;

@end
