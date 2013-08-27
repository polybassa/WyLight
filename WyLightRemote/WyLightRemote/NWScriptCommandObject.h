//
//  NWScriptCommandObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWSendableCommand.h"
#import "NWDrawableCommand.h"

@interface NWScriptCommandObject : NSObject <NWSendableCommand, NWDrawableCommand>

@property (nonatomic, assign) uint16_t duration;

@end
