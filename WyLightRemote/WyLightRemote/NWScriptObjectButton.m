//
//  NWScriptObjectButton.m
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectButton.h"

@implementation NWScriptObjectButton

@synthesize startColors = _startColors;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
		self.opaque = NO;
	}
    return self;
}

- (void)setEndColors:(NSArray *)endColors
{
	_endColors = endColors;
	[self setNeedsDisplay];
}

- (void)setStartColors:(NSArray *)startColors
{
	_startColors = startColors;
	[self setNeedsDisplay];
}


#pragma mark - GESTURE

- (void)pinch:(UIPinchGestureRecognizer *)gesture
{
    if ((gesture.state == UIGestureRecognizerStateChanged) ||
        (gesture.state == UIGestureRecognizerStateEnded)) {		
		self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, self.frame.size.width * gesture.scale, self.frame.size.height);
        gesture.scale = 1;
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[super touchesBegan:touches withEvent:event];
	self.latestTouchBegan = touches.anyObject;
}

#pragma mark - DRAWING

- (void)drawRect:(CGRect)rect
{
	CGContextRef context = UIGraphicsGetCurrentContext();
	const CGFloat heightFract = self.bounds.size.height / self.endColors.count;
	
	for (unsigned int i = 0; i < self.endColors.count; i++) {
		CGContextSaveGState(context);
		CGContextAddRect(context, CGRectMake(self.bounds.origin.x, i * heightFract, self.bounds.size.width, heightFract));
		CGContextClip(context);
		CGContextDrawLinearGradient(context,
									[NWScriptObjectButton createGradientWithStartColor:self.startColors[i] endColor:self.endColors[i]],
									CGPointMake(self.bounds.origin.x, i * heightFract + heightFract / 2),
									CGPointMake(self.bounds.size.width, i * heightFract + heightFract / 2), 0);
		CGContextRestoreGState(context);
	}
}

+ (CGGradientRef)createGradientWithStartColor:(UIColor*)startColor endColor:(UIColor*)endColor
{
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
