//
//  NWLoopOffCommandObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWCommandObject.h"

@interface NWLoopOffCommandObject : NWCommandObject

@property (nonatomic) uint8_t numberOfRepeats;

@end
