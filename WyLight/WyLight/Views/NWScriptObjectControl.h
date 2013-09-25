//
//  NWScriptObjectControl.h
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectView.h"

@protocol NWScriptObjectControlDelegate;

@interface NWScriptObjectControl : NWScriptObjectView

@property (nonatomic, weak) id<NWScriptObjectControlDelegate> delegate;
@property (nonatomic, getter = isQuivering) BOOL quivering;

- (void)pinchWidth:(UIPinchGestureRecognizer *)gesture;

@end

@protocol NWScriptObjectControlDelegate

@optional
- (void)scriptObjectView:(NWScriptObjectView *)view changedWidthTo:(CGFloat)width deltaOfChange:(CGFloat)delta;
- (void)scriptObjectView:(NWScriptObjectView *)view finishedWidthChange:(CGFloat)width;

@end
