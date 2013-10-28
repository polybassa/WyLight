//
//  NWScriptCellView.m
//  WyLight
//
//  Created by Nils Weiß on 04.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptCellView.h"

@implementation NWScriptCellView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
		
		CGRect scriptObjectViewFrame, timeInfoViewFrame;
		CGFloat timeInfoViewHeigth = frame.size.height / TIMEINFOVIEW_HEIGTH_FACTOR;
		
		scriptObjectViewFrame = CGRectMake(0, 0, frame.size.width, frame.size.height - timeInfoViewHeigth);
		timeInfoViewFrame = CGRectMake(0, frame.size.height - timeInfoViewHeigth, frame.size.width, timeInfoViewHeigth);
		
		
		self.scriptObjectView = [[NWScriptObjectControl alloc] initWithFrame:scriptObjectViewFrame];
		self.timeInfoView = [[NWTimeInfoView alloc] initWithFrame:timeInfoViewFrame];
		
		[self addSubview:self.scriptObjectView];
		[self addSubview:self.timeInfoView];
		
        // Initialization code
    }
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	
	CGRect scriptObjectViewFrame, timeInfoViewFrame;
	CGFloat timeInfoViewHeigth = frame.size.height / TIMEINFOVIEW_HEIGTH_FACTOR;
	
	scriptObjectViewFrame = CGRectMake(0, 0, frame.size.width, frame.size.height - timeInfoViewHeigth);
	timeInfoViewFrame = CGRectMake(0, frame.size.height - timeInfoViewHeigth, frame.size.width, timeInfoViewHeigth);
	
	[self.scriptObjectView setFrame:scriptObjectViewFrame];
	[self.timeInfoView setFrame:timeInfoViewFrame];
}

- (void)setTag:(NSInteger)tag {
	[super setTag:tag];
	self.scriptObjectView.tag = tag;
	self.timeInfoView.tag = tag;
}

- (void)setAlpha:(CGFloat)alpha {
	[super setAlpha:alpha];
	self.scriptObjectView.alpha = alpha;
	self.timeInfoView.alpha = alpha;
}

- (void)pinchWidth:(UIPinchGestureRecognizer *)gesture {
	if (gesture.state == UIGestureRecognizerStateChanged) {
		CGFloat x = self.frame.origin.x;
		CGFloat y = self.frame.origin.y;
		CGFloat oldWidth = self.frame.size.width;
		CGFloat width = self.frame.size.width * gesture.scale;
		CGFloat height = self.frame.size.height;
		
		[self.delegate scriptCellView:self changedWidthTo:width deltaOfChange:width - oldWidth];
		self.frame = CGRectMake(x, y, width, height);
		gesture.scale = 1;
	}
	if (gesture.state == UIGestureRecognizerStateEnded) {
		gesture.scale = 1;
		[self.delegate scriptCellView:self finishedWidthChange:self.frame.size.width];
	}
}


@end
