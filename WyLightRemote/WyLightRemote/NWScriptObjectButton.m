//
//  NWScriptObjectButton.m
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectButton.h"

@interface NWScriptObjectButton	()

@property (nonatomic) CGPoint originalPosition;
@property (nonatomic) CGPoint originalOutsidePosition;
@property (nonatomic) BOOL isInScrollView;

@end

@implementation NWScriptObjectButton

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
		self.isInScrollView = YES;
		self.userInteractionEnabled = YES;
	}
    return self;
}
/*
#pragma mark - GESTURE

- (void)pinch:(UIPinchGestureRecognizer *)gesture {
    if ((gesture.state == UIGestureRecognizerStateChanged) ||
        (gesture.state == UIGestureRecognizerStateEnded)) {		
		self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, self.frame.size.width * gesture.scale, self.frame.size.height);
        gesture.scale = 1;
    }
}*/

#pragma mark - DRAG N DROP
/*
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[super touchesBegan:touches withEvent:event];
	self.originalPosition = self.center;
	self.scrollParent.scrollEnabled = NO;
	
	if (self.isInScrollView) {
		CGPoint newLoc = CGPointZero;
		newLoc = [[self superview] convertPoint:self.center toView:self.mainView];
		self.originalOutsidePosition = newLoc;
		
		[self removeFromSuperview];
		
		self.center = newLoc;
		[self.mainView addSubview:self];
		[self.mainView bringSubviewToFront:self];
		self.isInScrollView = NO;
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	[super touchesMoved:touches withEvent:event];
	[UIView beginAnimations:@"stalk" context:nil];
	[UIView setAnimationDuration:.001];
	[UIView setAnimationBeginsFromCurrentState:YES];
	
	UITouch *touch = [touches anyObject];
	self.center = [touch locationInView:self.superview];
	
	[UIView commitAnimations];
	
	if ([self.delegate isInNewPosition:self touching:NO]) {
		self.originalPosition = [self.delegate newOriginalPosition:self];
		self.originalOutsidePosition = CGPointMake(self.originalPosition.x, self.originalOutsidePosition.y);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	[super touchesEnded:touches withEvent:event];
	
	[UIView beginAnimations:@"goback" context:nil];
	[UIView setAnimationDuration:0.4f];
	[UIView setAnimationBeginsFromCurrentState:YES];
	self.center = self.originalOutsidePosition;
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector: @selector(animationDidStop:finished:context:)];
	[UIView commitAnimations];
}

- (void)animationDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	if ([animationID isEqualToString:@"goback"] && finished) {
		[self removeFromSuperview];
       self.center = self.originalPosition;
       [self.scrollParent addSubview:self];
       self.isInScrollView = YES;
		self.scrollParent.scrollEnabled = YES;
	}
}*/

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
	return YES;
}

- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
	return YES;
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
	return YES;
}

@end
