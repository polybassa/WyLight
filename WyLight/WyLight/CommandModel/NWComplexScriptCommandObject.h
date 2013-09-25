//
//  NWComplexScriptCommandObject.h
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptCommandObject.h"

@interface NWComplexScriptCommandObject : NWScriptCommandObject <NSCopying, NSCoding>

@property (nonatomic, weak) NWComplexScriptCommandObject *next;
@property (nonatomic, weak) NWComplexScriptCommandObject *prev;

@property (nonatomic, getter = isWaitCommand) BOOL waitCommand;

@property (nonatomic, strong) NSMutableArray *scriptObjects;

@end
