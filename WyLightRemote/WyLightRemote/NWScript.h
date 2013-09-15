//
//  NWScript.h
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@class NWComplexScriptCommandObject;

@interface NWScript : NSObject

@property (nonatomic, readonly, strong) NSMutableArray *scriptArray;

- (void)addObject:(NWComplexScriptCommandObject *)anObject;
- (void)removeObjectAtIndex:(NSUInteger)index;

- (NSNumber *)totalDurationInTmms;

@end
