//
//  NWScript.h
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NWSendableCommand.h"

@class NWComplexScriptCommandObject;

@interface NWScript : NSObject <NSCoding, NWSendableCommand>

@property (nonatomic, readonly, strong) NSMutableArray *scriptArray;
@property (nonatomic, strong) NSString *title;
@property (nonatomic, assign) BOOL needsUpdate;
@property (nonatomic, assign) BOOL repeatWhenFinished;

- (void)addObject:(NWComplexScriptCommandObject *)anObject;
- (void)removeObjectAtIndex:(NSUInteger)index;

- (NSNumber *)totalDurationInTmms;

@end
