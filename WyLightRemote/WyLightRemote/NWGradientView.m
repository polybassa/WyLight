//
//  NWGradientView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 16.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWGradientView.h"

@implementation NWGradientView

- (void)setEndColor:(UIColor *)endColor {
	if ([_endColor isEqual:endColor]) {
		_endColor = endColor;
	} else {
		_endColor = endColor;
		[self setNeedsDisplay];
	}
}

- (void)setStartColor:(UIColor *)startColor {
	if ([_startColor isEqual:startColor]) {
		_startColor = startColor;
	} else {
		_startColor = startColor;
		[self setNeedsDisplay];
	}
}

- (void)setColorsAnimatedWithDuration:(NSTimeInterval)duration startColor:(UIColor *)startColor endColor:(UIColor *)endColor {
	if ( (![startColor isEqual:self.startColor]) && (![endColor isEqual:self.endColor]) ) {
		NWGradientView *tempView = [[NWGradientView alloc] initWithFrame:self.frame];
		tempView.endColor = self.endColor;
		tempView.startColor = self.startColor;
		[self.superview addSubview:tempView];
		self.alpha = 0.0;
		self.endColor = endColor;
		self.startColor = startColor;
		
		[UIView animateWithDuration:duration delay:0.0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
			tempView.alpha = 0.0;
			self.alpha = 1.0;
		} completion:^(BOOL finished) {
			[tempView removeFromSuperview];
		}];
	}
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
	CGGradientRef currentGradient = [NWGradientView createGradientWithStartColor:self.startColor endColor:self.endColor];
	CGContextDrawLinearGradient(context, currentGradient,
									CGPointMake(0, 0),
									CGPointMake(rect.size.width, 0), 0);
}

+ (CGGradientRef)createGradientWithStartColor:(UIColor*)startColor endColor:(UIColor*)endColor {
	if ([endColor isEqual:[UIColor blackColor]]) {
		endColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:1];
	}
	if ([startColor isEqual:[UIColor blackColor]]) {
		startColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:1];
	}
	
    CGGradientRef result;
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGFloat locations[2] = {0.0f, 1.0f};
    CGFloat startRed, startGreen, startBlue, startAlpha;
    CGFloat endRed, endGreen, endBlue, endAlpha;
	
    [endColor getRed:&endRed green:&endGreen blue:&endBlue alpha:&endAlpha];
    [startColor getRed:&startRed green:&startGreen blue:&startBlue alpha:&startAlpha];
	
    CGFloat componnents[8] = {
        startRed, startGreen, startBlue, startAlpha,
        endRed, endGreen, endBlue, endAlpha
    };
    result = CGGradientCreateWithColorComponents(colorSpace, componnents, locations, 2);
    CGColorSpaceRelease(colorSpace);
    return result;
}


@end
