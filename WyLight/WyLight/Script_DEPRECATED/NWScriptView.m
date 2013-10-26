//
//  NWScriptView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWScriptView.h"
#import "NWAddScriptObjectView.h"
#import "NWScriptCellView.h"

@implementation NWScriptView

#pragma mark - SETUP

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		[self setup];
		self.scriptObjectSpacing  = 0.0;
	}
    return self;
}

- (void)setup {
	self.showsHorizontalScrollIndicator = NO;
	self.showsVerticalScrollIndicator = NO;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	[self fixLocationsOfSubviews];
}

- (void)fixLocationsOfSubviews {
	CGFloat xPosition = self.insets.left;
	CGFloat yPosition = self.insets.top;
	CGFloat height = self.frame.size.height - yPosition - self.insets.bottom;
	CGFloat width = 0;
	for (UIView *subview in self.subviews) {
		if ([subview isKindOfClass:[NWScriptCellView class]] || ![subview isKindOfClass:[NWAddScriptObjectView class]]) {
			width = [self.dataSource scriptView:self widthOfObjectAtIndex:subview.tag];
			subview.frame = CGRectMake(xPosition, yPosition, width, height);
			xPosition += floorf(width) + self.scriptObjectSpacing;
		}
	}
	for (UIView *subview in self.subviews) {
		if ([subview isKindOfClass:[NWAddScriptObjectView class]]) {
			width = height / 3;
			
			subview.frame = CGRectMake(xPosition, yPosition, width, height - height / TIMEINFOVIEW_HEIGTH_FACTOR);
			xPosition += width + self.scriptObjectSpacing;
		}
	}
	[self setContentSize:CGSizeMake(xPosition + width, self.bounds.size.height)];
}

#pragma mark - SETTER

- (void)setDataSource:(id<NWScriptViewDataSource>)dataSource {
	_dataSource = dataSource;
	[self reloadData];
}

- (void)reloadData {
	for (UIView *subview in self.subviews) {
		[subview removeFromSuperview];
	}
	
	for (NSUInteger index = 0; index < [self.dataSource numberOfObjectsInScriptView:self]; index++) {
		UIView *subview = [self.dataSource scriptView:self cellViewForIndex:index];
		[self addSubview:subview];
	}
	[self fixLocationsOfSubviews];
}

@end
