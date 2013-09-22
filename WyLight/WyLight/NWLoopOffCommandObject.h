//
//  NWLoopOffCommandObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWSendableCommand.h"

@interface NWLoopOffCommandObject : NSObject <NWSendableCommand>

@property (nonatomic) uint8_t numberOfRepeats;

@end
