//
//  NWScriptObjectControl.m
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectControl.h"
#import <QuartzCore/QuartzCore.h>

@interface NWScriptObjectControl ()

@property (nonatomic) CGRect originalFrame;

@end

@implementation NWScriptObjectControl

- (void)setQuivering:(BOOL)quivering {
	
	if (quivering && (quivering != _quivering)) {
		[self startQuivering];
	} else if (!quivering && (quivering != _quivering)) {
		[self stopQuivering];
	}
	if (quivering != _quivering) {
		_quivering = quivering;
		self.frame = self.frame;
		if (quivering == NO) {
			self.frame = self.originalFrame;
		}
	}
}

- (void)setFrame:(CGRect)frame {
	if (self.quivering) {
		self.originalFrame = frame;
		super.frame = CGRectInset(frame, frame.size.width * 0.1, 10);
	} else {
		super.frame = frame;
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
