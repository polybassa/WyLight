//
//  NWGradientEditView.h
//  WyLight
//
//  Created by Nils Weiß on 12.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NWGradientEditView, Effect, TouchAndHoldButton;

@protocol NWGradientEditViewDelegate <NSObject>

- (void)NWGradientEditViewDelegateUpButtonPressed:(NWGradientEditView *)view;
- (void)NWGradientEditViewDelegateDownButtonPressed:(NWGradientEditView *)view;
- (void)NWGradientEditViewDelegatePlusButtonPressed:(NWGradientEditView *)view;
- (void)NWGradientEditViewDelegateMinusButtonPressed:(NWGradientEditView *)view;

@end

@interface NWGradientEditView : UIView

@property (nonatomic, weak) Effect *command;
@property (nonatomic, weak) id<NWGradientEditViewDelegate> delegate;
@property (nonatomic) CGFloat cornerRadius;

@property (nonatomic, strong, readonly) TouchAndHoldButton *gradientUpButton;
@property (nonatomic, strong, readonly) TouchAndHoldButton *gradientDownButton;
@property (nonatomic, strong, readonly) TouchAndHoldButton *gradientPlusButton;
@property (nonatomic, strong, readonly) TouchAndHoldButton *gradientMinusButton;

- (void)reloadData;

@end
