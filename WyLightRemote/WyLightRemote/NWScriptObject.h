//
//  NWScriptObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "wifly_cmd.h"

@interface NWScriptObject : NSObject

@property (nonatomic, assign) struct led_cmd command;
@property (nonatomic, weak) NWScriptObject *nextScriptObject;
@property (nonatomic, weak) NWScriptObject *prevScriptObject;

@property (nonatomic, readonly, strong) NSArray *startColors;
@property (nonatomic, readonly, strong) NSArray *endColors;
@property (nonatomic, strong) NSNumber *duration;
@property (nonatomic) BOOL parallel;

+ (struct led_cmd)randomCommand;

+ (NSArray *)mergedColorsFromArray:(NSArray *)array;

@end
