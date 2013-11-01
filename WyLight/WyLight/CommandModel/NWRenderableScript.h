//
//  NWRenderableScript.h
//  WyLight
//
//  Created by Nils Weiß on 26/10/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScript.h"

@protocol NWRenderableScriptDelegate;

@interface NWRenderableScript : NWScript <NSCoding>

@property (atomic, assign, readonly, getter = isRendering) BOOL rendering;
@property (nonatomic, strong) UIImage *snapshot;
@property (nonatomic, weak) id<NWRenderableScriptDelegate> delegate;

- (void)startRenderingWithRect:(CGRect)rect;

- (BOOL)needsRendering;

@end

@protocol NWRenderableScriptDelegate <NSObject>

- (void)NWRenderableScriptFinishedRendering:(NWRenderableScript *)script;

@end
