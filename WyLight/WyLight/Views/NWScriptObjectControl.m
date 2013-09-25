//
//  NWScriptObjectControl.m
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectControl.h"
#import <QuartzCore/QuartzCore.h>

@implementation NWScriptObjectControl

- (void)setQuivering:(BOOL)quivering {
	_quivering = quivering;
	
	if (quivering) {
		[self startQuivering];
	} else {
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

@end
