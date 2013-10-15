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
	
	if (_quivering == quivering) {
		return;
	} else {
		_quivering = quivering;
		if (quivering) {
			[self startQuivering];
		} else {
			[self stopQuivering];
		}
	}
}

#define DOWNSCALEFACTOR 0.8

- (void)setDownscale:(BOOL)downscale {
	if (_downscale == downscale) {
		//nothing to do
		return;
	} else {
		if (downscale) {
			self.layer.transform = CATransform3DMakeScale(DOWNSCALEFACTOR, DOWNSCALEFACTOR, 1.0);
		} else {
			self.layer.transform = CATransform3DMakeScale(1.0, 1.0, 1.0);
		}
		_downscale = downscale;
	}
}

- (void)setFrame:(CGRect)frame {
	if (self.downscale) {
		self.layer.transform = CATransform3DMakeScale(1.0, 1.0, 1.0);
		[super setFrame:frame];
		self.layer.transform = CATransform3DMakeScale(DOWNSCALEFACTOR, DOWNSCALEFACTOR, 1.0);
	} else {
		[super setFrame:frame];
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
