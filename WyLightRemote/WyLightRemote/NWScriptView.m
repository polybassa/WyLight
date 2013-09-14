//
//  NWScriptView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWScriptView.h"
#import "NWScriptObjectControl.h"
#import "NWTimeInfoView.h"
#import "NWAddScriptObjectView.h"

@implementation NWScriptView

#pragma mark - SETUP

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		[self setup];
	}
    return self;
}

- (void)setup {
	self.showsHorizontalScrollIndicator = NO;
	self.showsVerticalScrollIndicator = NO;
	self.timeScaleFactor = 1;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	[self fixLocationsOfSubviews];
}

- (void)fixLocationsOfSubviews {
	CGFloat xPosition = 20;
	CGFloat yPosition = 30;
	CGFloat height = self.frame.size.height - yPosition * 3;
	CGFloat width = 0;
	for (UIView *subview in self.subviews) {
		if ([subview isKindOfClass:[NWScriptObjectView class]]) {
			width = [self.dataSource scriptView:self widthOfObjectAtIndex:subview.tag];
			
			subview.frame = CGRectMake(xPosition, yPosition, width, height);
			xPosition += width + 2;
			
			[self fixLocationOfTimelineView:subview.tag];
		}
		if ([subview isKindOfClass:[NWAddScriptObjectView class]]) {
			subview.frame = CGRectMake(xPosition, yPosition, height / 3, height);
			xPosition += width + 2;
		}
	}
	[self setContentSize:CGSizeMake(xPosition + width, self.bounds.size.height)];
}

#define Y_SPACE 10
#define HEIGTH 30

- (void)fixLocationOfTimelineView:(NSInteger)tag {
	CGRect subviewFrame;
	for (UIView *subview in self.subviews) {
		if ((subview.tag == tag) && [subview isKindOfClass:[NWScriptObjectView class]]) {
			subviewFrame = subview.frame;
			break;
		}
	}
	for (UIView *subview in self.subviews) {
		if ((subview.tag == tag) && [subview isKindOfClass:[NWTimeInfoView class]]) {
			subview.frame = CGRectMake(subviewFrame.origin.x, subviewFrame.origin.y + subviewFrame.size.height + Y_SPACE, subviewFrame.size.width, HEIGTH);
			((NWTimeInfoView *)subview).timeScaleFactor = self.timeScaleFactor;
			break;
		}
	}
			

}

#pragma mark - SETTER

- (void)setDataSource:(id<NWScriptViewDataSource>)dataSource {
	_dataSource = dataSource;
	[self reloadData];
}

- (void)setTimeScaleFactor:(CGFloat)timeScaleFactor {
	_timeScaleFactor = timeScaleFactor;
	[self fixLocationsOfSubviews];
}

- (void)reloadData {
	for (UIView *subview in self.subviews) {
		if ([subview isKindOfClass:[NWScriptObjectControl class]]) {
			[subview removeFromSuperview];
		}
		if ([subview isKindOfClass:[NWTimeInfoView class]]) {
			[subview removeFromSuperview];
		}
		if ([subview isKindOfClass:[NWAddScriptObjectView class]]) {
			[subview removeFromSuperview];
		}

	}
	
	for (NSUInteger index = 0; index < [self.dataSource numberOfObjectsInScriptView:self]; index++) {
		UIView *subview = [self.dataSource scriptView:self objectForIndex:index];
		subview.tag = index;
		[self addSubview:subview];
		
		if ([subview isKindOfClass:[NWScriptObjectControl class]]) {
			NWTimeInfoView *timeInfoView = [[NWTimeInfoView alloc] initWithFrame:CGRectZero];
			timeInfoView.timeScaleFactor = self.timeScaleFactor;
			timeInfoView.tag = index;
			[self addSubview:timeInfoView];
		}
	}
	[self fixLocationsOfSubviews];
}


@end
