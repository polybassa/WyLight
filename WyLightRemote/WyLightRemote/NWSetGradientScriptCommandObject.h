//
//  NWSetGradientScriptCommandObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptEffectCommandObject.h"

@interface NWSetGradientScriptCommandObject : NWScriptEffectCommandObject <NSCopying, NSCoding>

@property (nonatomic, strong) UIColor *color1;
@property (nonatomic, strong) UIColor *color2;
@property (nonatomic) uint8_t offset;
@property (nonatomic) uint8_t numberOfLeds;
@property (nonatomic, readonly) uint32_t address;

@end
