//
//  NWSetFadeScriptCommandObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptEffectCommandObject.h"

@interface NWSetFadeScriptCommandObject : NWScriptEffectCommandObject <NSCopying>

@property (nonatomic) uint32_t address;
@property (nonatomic, strong) UIColor *color;


@end
