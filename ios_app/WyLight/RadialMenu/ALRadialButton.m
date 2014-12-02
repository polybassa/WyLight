//
//  ALRadialButton.m
//  ALRadial
//
//  Created by andrew lattis on 12/10/14.
//  Copyright (c) 2012 andrew lattis. All rights reserved.
//  http://andrewlattis.com
//

#import "ALRadialButton.h"

@implementation ALRadialButton

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}



- (void)willAppear {
	//rotate the button upsidedown so its right side up after the 180 degree rotation while its moving out
	//[self.imageView setTransform:CGAffineTransformRotate(CGAffineTransformIdentity, 180/180*M_PI)];
	
	CGRect frame = self.frame;
	self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, 0, 0);
	
	[UIView animateWithDuration:.25f delay:0.0f options:UIViewAnimationOptionCurveEaseOut animations:^{
		
		self.frame = frame;
		[self setCenter:self.bouncePoint];
		self.alpha = 1.0;
		
	} completion:^(BOOL finished) {
		
		[UIView animateWithDuration:.15f animations:^{
			//a little bounce back at the end
			[self setCenter:self.centerPoint];
		}];
		
	}];
}


- (void)willDisappear {
	
	CGRect frame = self.frame;
	
	
	[UIView animateWithDuration:.35f delay:0.0f options:UIViewAnimationOptionCurveEaseIn animations:^{
		
		self.frame = CGRectInset(self.frame, self.frame.size.width/2, self.frame.size.height/2);
		[self setCenter:self.originPoint];
		self.alpha = 0.0f;
	
	} completion:^(BOOL finished) {
		if (finished) {
			self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, frame.size.width, frame.size.height);
			[self.delegate buttonDidFinishAnimation:self];
		}
	}];
}

@end
