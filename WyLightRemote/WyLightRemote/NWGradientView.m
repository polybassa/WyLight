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
	_endColor = endColor;
	[self setNeedsDisplay];
}

- (void)setStartColor:(UIColor *)startColor {
	_startColor = startColor;
	[self setNeedsDisplay];
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
