//
//  NWScriptObjectView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 28.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWScriptObjectView.h"
#import "NWScriptView.h"

@implementation NWScriptObjectView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
		self.layer.shouldRasterize = YES;
		
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
        self.clipsToBounds = YES;
        self.layer.opacity = 1;
        self.layer.borderColor = [UIColor darkGrayColor].CGColor;
	}
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	float dim = MIN(self.bounds.size.width, self.bounds.size.height);
	if (self.cornerRadius >= 1) {
		self.layer.cornerRadius = self.cornerRadius;
	} else {
		self.layer.cornerRadius = dim/8;
	}
	if (self.borderWidth >= 1) {
		self.layer.borderWidth = self.borderWidth;
	} else {
		self.layer.borderWidth = 2.0;
	}
	[self setNeedsDisplay];
}

- (void)pinchWidth:(UIPinchGestureRecognizer *)gesture {
	if (gesture.state == UIGestureRecognizerStateChanged) {
		
		CGFloat x = self.frame.origin.x;
		CGFloat y = self.frame.origin.y;
		CGFloat oldWidth = self.frame.size.width;
		CGFloat width = self.frame.size.width * gesture.scale;
		CGFloat height = self.frame.size.height;
		
		[self.delegate scriptObjectView:self changedWidthTo:width deltaOfChange:width - oldWidth];
		self.frame = CGRectMake(x, y, width, height);
		gesture.scale = 1;
	}
	if (gesture.state == UIGestureRecognizerStateEnded) {
		gesture.scale = 1;
		[self.delegate scriptObjectView:self finishedWidthChange:self.frame.size.width];
	}
}

- (void)setEndColors:(NSArray *)endColors {
	_endColors = endColors;
	[self setNeedsDisplay];
}

- (void)setStartColors:(NSArray *)startColors {
	_startColors = startColors;
	[self setNeedsDisplay];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[super touchesBegan:touches withEvent:event];
	self.latestTouchBegan = touches.anyObject;
}

#pragma mark - DRAWING

- (void)drawRect:(CGRect)rect {
	CGContextRef context = UIGraphicsGetCurrentContext();
	const CGFloat heightFract = self.bounds.size.height / self.endColors.count;
		
	for (unsigned int i = 0; i < self.endColors.count; i++) {
		CGContextSaveGState(context);
		CGContextAddRect(context, CGRectMake(self.bounds.origin.x, i * heightFract, self.bounds.size.width, heightFract));
		CGContextClip(context);
		CGContextDrawLinearGradient(context,
									[NWScriptObjectView createGradientWithStartColor:self.startColors[i] endColor:self.endColors[i]],
									CGPointMake(self.bounds.origin.x, i * heightFract + heightFract / 2),
									CGPointMake(self.bounds.size.width, i * heightFract + heightFract / 2), 0);
		CGContextRestoreGState(context);
	}
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
