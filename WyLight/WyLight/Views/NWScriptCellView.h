//
//  NWScriptCellView.h
//  WyLight
//
//  Created by Nils Weiß on 04.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NWTimeInfoView.h"
#import "NWScriptObjectControl.h"
#import "NWRenderableScriptImageView.h"

#define TIMEINFOVIEW_HEIGTH_FACTOR 6

@protocol NWScriptCellViewDelegate;

@interface NWScriptCellView : UIView

@property (nonatomic, strong) NWTimeInfoView *timeInfoView;
@property (nonatomic, strong) NWRenderableScriptImageView *scriptObjectImageView;
@property (nonatomic, weak) id<NWScriptCellViewDelegate> delegate;

- (void)pinchWidth:(UIPinchGestureRecognizer *)gesture;

@end

@protocol NWScriptCellViewDelegate

@optional
- (void)scriptCellView:(NWScriptCellView *)view changedWidthTo:(CGFloat)width deltaOfChange:(CGFloat)delta;
- (void)scriptCellView:(NWScriptCellView *)view finishedWidthChange:(CGFloat)width;

@end
