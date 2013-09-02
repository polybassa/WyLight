//
//  NWScriptObjectControl.m
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectControl.h"
#import <QuartzCore/QuartzCore.h>

#define MARGIN 1
static UIImage *deleteButtonImg;

@implementation NWScriptObjectControl

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.deleteButton = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 30, 30)];
		if (!deleteButtonImg)
		{
			CGRect buttonFrame = self.deleteButton.frame;
			UIGraphicsBeginImageContext(buttonFrame.size);
			CGFloat sz = MIN(buttonFrame.size.width, buttonFrame.size.height);
			UIBezierPath *path = [UIBezierPath bezierPathWithArcCenter:CGPointMake(buttonFrame.size.width/2, buttonFrame.size.height/2) radius:sz/2-MARGIN startAngle:0 endAngle:M_PI * 2 clockwise:YES];
			[path moveToPoint:CGPointMake(MARGIN, MARGIN)];
			[path addLineToPoint:CGPointMake(sz-MARGIN, sz-MARGIN)];
			[path moveToPoint:CGPointMake(MARGIN, sz-MARGIN)];
			[path addLineToPoint:CGPointMake(sz-MARGIN, MARGIN)];
			[[UIColor redColor] setFill];
			[[UIColor whiteColor] setStroke];
			[path setLineWidth:3.0];
			[path fill];
			[path stroke];
			deleteButtonImg = UIGraphicsGetImageFromCurrentImageContext();
			UIGraphicsEndImageContext();
		}
		[self.deleteButton setImage:deleteButtonImg forState:UIControlStateNormal];
		[self addSubview:self.deleteButton];
		self.showDeleteButton = NO;
    }
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	self.deleteButton.frame = CGRectMake(0, 0, 30, 30);
	[self setNeedsDisplay];
	[self.deleteButton setNeedsDisplay];
}

- (void)setShowDeleteButton:(BOOL)showDeleteButton {
	if (showDeleteButton) {
		self.deleteButton.layer.opacity = 1.0;
		[self bringSubviewToFront:self.deleteButton];
		[self startQuivering];
	} else {
		self.deleteButton.layer.opacity = 0.0;
		[self stopQuivering];
	}
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

- (void)startQuivering {
    CABasicAnimation *quiverAnim = [CABasicAnimation animationWithKeyPath:@"transform.rotation"];
    float startAngle = (-1) * M_PI/180.0;
    float stopAngle = -startAngle;
    quiverAnim.fromValue = [NSNumber numberWithFloat:startAngle];
    quiverAnim.toValue = [NSNumber numberWithFloat:2 * stopAngle];
    quiverAnim.autoreverses = YES;
    quiverAnim.duration = 0.2;
    quiverAnim.repeatCount = HUGE_VALF;
    float timeOffset = (float)(arc4random() % 100)/100 - 0.50;
    quiverAnim.timeOffset = timeOffset;
    CALayer *layer = self.layer;
    [layer addAnimation:quiverAnim forKey:@"quivering"];
}

- (void)stopQuivering {
    CALayer *layer = self.layer;
    [layer removeAnimationForKey:@"quivering"];
}


/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
