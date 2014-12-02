//
//  NWEffectEnableEditView.h
//  WyLight
//
//  Created by Nils Weiß on 18/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ComplexEffect, NWEffectEnableEditView;

@protocol NWEffectEnableEditViewDelegate <NSObject>

- (void)EffectEnableEditView:(NWEffectEnableEditView *)view switchValueChanged:(BOOL)on;

@end

@interface NWEffectEnableEditView : UIView


@property (nonatomic, weak) ComplexEffect *command;
@property (nonatomic, weak) id<NWEffectEnableEditViewDelegate> delegate;
@property (nonatomic) CGFloat cornerRadius;

- (void)reloadData;

@end
