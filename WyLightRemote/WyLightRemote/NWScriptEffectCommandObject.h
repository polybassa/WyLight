//
//  NWScriptEffectCommandObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptCommandObject.h"

@interface NWScriptEffectCommandObject : NWScriptCommandObject

@property (nonatomic, readonly, strong) NSArray *colors;
@property (nonatomic, strong) UIColor *backgroundColor;
@property (nonatomic) BOOL parallel;

@end
