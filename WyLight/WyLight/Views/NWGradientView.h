//
//  NWGradientView.h
//  WyLightRemote
//
//  Created by Nils Weiß on 16.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NWGradientView : UIView

- (void)setColorsAnimatedWithDuration:(NSTimeInterval)duration startColor:(UIColor *)startColor endColor:(UIColor *)endColor;

@property (nonatomic, strong) UIColor *startColor;
@property (nonatomic, strong) UIColor *endColor;

@end
