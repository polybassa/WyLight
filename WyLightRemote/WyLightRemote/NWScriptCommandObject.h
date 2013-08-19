//
//  NWScriptCommandObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWCommandObject.h"

@interface NWScriptCommandObject : NWCommandObject

@property (nonatomic, assign) uint16_t duration;
@property (nonatomic, readonly, strong) NSArray *colors;
@property (nonatomic, strong) UIColor *backgroundColor;

@end
