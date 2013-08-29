//
//  NWScriptObjectButton.h
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectView.h"

@protocol NWScriptObjectButtonDelegate;

@interface NWScriptObjectButton : NWScriptObjectView <UIGestureRecognizerDelegate>

@property (nonatomic, weak) UIView *mainView;
@property (nonatomic, weak) UIScrollView *scrollParent;

@property (nonatomic, weak) id<NWScriptObjectButtonDelegate> delegate;

//- (void)pinch:(UIPinchGestureRecognizer *)gesture;

@property (nonatomic) NSUInteger position;

@end

@protocol NWScriptObjectButtonDelegate

- (BOOL)isInNewPosition:(NWScriptObjectButton *)button touching:(BOOL)finished;
- (CGPoint)newOriginalPosition:(NWScriptObjectButton *)button;
@end
