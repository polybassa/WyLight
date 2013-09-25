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
		self.scriptObjectSpacing  = 2.0;
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
			xPosition += width + self.scriptObjectSpacing;
			
			[self fixLocationOfTimeInfoView:subview.tag];
		}
	}
	for (UIView *subview in self.subviews) {
		if ([subview isKindOfClass:[NWAddScriptObjectView class]]) {
			subview.frame = CGRectMake(xPosition, yPosition, height / 3, height);
			xPosition += width + self.scriptObjectSpacing;
		}
	}
	[self setContentSize:CGSizeMake(xPosition + width, self.bounds.size.height)];
}

#define Y_SPACE 10
#define HEIGTH 30

- (void)fixLocationOfTimeInfoView:(NSInteger)tag {
	CGRect subviewFrame;
	CGFloat subviewAlpha;
	for (UIView *subview in self.subviews) {
		if ((subview.tag == tag) && [subview isKindOfClass:[NWScriptObjectView class]]) {
			subviewFrame = subview.frame;
			subviewAlpha = subview.alpha;
			break;
		}
	}
	for (UIView *subview in self.subviews) {
		if ((subview.tag == tag) && [subview isKindOfClass:[NWTimeInfoView class]]) {
			subview.frame = CGRectMake(subviewFrame.origin.x, subviewFrame.origin.y + subviewFrame.size.height + Y_SPACE, subviewFrame.size.width, HEIGTH);
			((NWTimeInfoView *)subview).timeScaleFactor = self.timeScaleFactor;
			subview.alpha = subviewAlpha;
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
		[subview removeFromSuperview];
	}
	
	for (NSUInteger index = 0; index < [self.dataSource numberOfObjectsInScriptView:self]; index++) {
		UIView *subview = [self.dataSource scriptView:self objectViewForIndex:index];
		[self addSubview:subview];
		
		if ([subview isKindOfClass:[NWScriptObjectControl class]]) {
			[self addSubview:[self.dataSource scriptView:self timeInfoViewForIndex:index]];
		}
	}
	[self fixLocationsOfSubviews];
}


@end
