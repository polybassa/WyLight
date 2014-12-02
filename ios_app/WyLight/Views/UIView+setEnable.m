//
//  UIView+setEnable.m
//  WyLight
//
//  Created by Nils Weiß on 19/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "UIView+setEnable.h"
#import "NWColorEditView.h"
#import "NWGradientEditView.h"
#import "TouchAndHoldButton.h"

@implementation UIView (setEnable)

#define DISABLEVIEW_TAG 16994

- (void)setEnable:(BOOL)enable {
    if (enable) {
        UIView *disableView = [self viewWithTag:DISABLEVIEW_TAG];
        if (disableView) {
            [disableView removeFromSuperview];
        }
    } else {
        UIView *disableView = [self viewWithTag:DISABLEVIEW_TAG];
        if (disableView) {
            [disableView removeFromSuperview];
            disableView = nil;
        }
        if (!disableView) {
            disableView = [[UIView alloc] initWithFrame:self.frame];
            disableView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
            disableView.tag = DISABLEVIEW_TAG;
            disableView.userInteractionEnabled = YES;
            disableView.backgroundColor = [UIColor colorWithRed:0.3 green:0.3 blue:0.3 alpha:0.3];
            [self addSubview:disableView];
        }
        [self bringSubviewToFront:disableView];
    }
    if ([self isKindOfClass:[NWColorEditView class]]) {
        for (UIButton *btn in ((NWColorEditView*)self).colorButtons) {
            btn.enabled = enable;
        }
    }
    if ([self isKindOfClass:[NWGradientEditView class]]) {
        ((NWGradientEditView *)self).gradientDownButton.enabled = enable;
        ((NWGradientEditView *)self).gradientMinusButton.enabled = enable;
        ((NWGradientEditView *)self).gradientPlusButton.enabled = enable;
        ((NWGradientEditView *)self).gradientUpButton.enabled = enable;
        if (enable) {
            [((NWGradientEditView *)self) reloadData];
        }
    }
}

@end
