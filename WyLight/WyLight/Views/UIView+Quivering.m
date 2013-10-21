//
//  UIView+Quivering.m
//  WyLight
//
//  Created by Nils Weiß on 10/21/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "UIView+Quivering.h"
#import <QuartzCore/QuartzCore.h>

@implementation UIView (Quivering)

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
