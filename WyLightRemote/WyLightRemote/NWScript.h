//
//  NWScript.h
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NWScript : NSObject

@property (nonatomic, readonly, strong) NSArray *completeScript;
@property (nonatomic, readonly, strong) NSDictionary *allVisibleScriptBlocks;

- (void)addObject:(id)anObject;
- (void)insertObject:(id)anObject atIndex:(NSUInteger)index;
- (void)removeObject:(id)anObject;
- (void)removeObjectAtIndex:(NSUInteger)index;

- (NSNumber *)totalDurationInTmms;

@end
