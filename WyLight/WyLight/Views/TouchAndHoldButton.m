//
//  TouchAndHoldButton.m
//  WyLight
//
//  Created by Nils Weiß on 08.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "TouchAndHoldButton.h"



@interface TouchAndHoldButton ()

@property (nonatomic, strong) NSTimer *holdTimer;
@property (nonatomic, assign) NSTimeInterval dt;
@property (nonatomic, strong) NSValue *repeatSelectorAsValue;
@property (nonatomic, weak) id targetOfRepeatSel;

@end

@implementation TouchAndHoldButton	

- (void)addTarget:(id)target action:(SEL)action forTouchAndHoldControlEventWithTimeInterval:(NSTimeInterval)periodTime {
    self.dt = periodTime;
    self.targetOfRepeatSel = target;
    [self addTarget:self action:@selector(sourceTouchUp:) forControlEvents:UIControlEventTouchUpInside];
    [self addTarget:self action:@selector(sourceTouchUp:) forControlEvents:UIControlEventTouchUpOutside];
    [self addTarget:self action:@selector(sourceTouchDown:) forControlEvents:UIControlEventTouchDown];
    self.repeatSelectorAsValue = [NSValue valueWithBytes:&action objCType:@encode(SEL)];
}


- (void) sourceTouchUp:(UIButton*) sender {
    if (self.holdTimer != nil) {
        [self.holdTimer invalidate];
        self.holdTimer = nil;
    }
}

- (void) sourceTouchDown:(UIButton*) sender {
    SEL selector;
    if (strcmp([self.repeatSelectorAsValue objCType], @encode(SEL)) == 0) {
        [self.repeatSelectorAsValue getValue:&selector];
        self.holdTimer = [NSTimer scheduledTimerWithTimeInterval:self.dt  target:self.targetOfRepeatSel selector:selector userInfo:nil repeats: YES];
    }
}

@end
