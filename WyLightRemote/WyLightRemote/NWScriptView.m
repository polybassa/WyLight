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
#import "NWTimeLineView.h"

@implementation NWScriptView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		[self setup];
	}
    return self;
}

- (void)setup {
	self.timelineScaleFactor = 1;
	self.showsHorizontalScrollIndicator = NO;
	self.showsVerticalScrollIndicator = NO;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	[self fixLocationsOfSubviews];
}

- (void)setDataSource:(id<NWScriptViewDataSource>)dataSource {
	_dataSource = dataSource;
	[self reloadData];
}

- (void)setTimelineScaleFactor:(CGFloat)timelineScaleFactor {
	_timelineScaleFactor = timelineScaleFactor;
	[self fixLocationsOfSubviews];
	[self setNeedsDisplay];
}

- (void)reloadData {
	for (NWScriptObjectView *subview in self.subviews) {
		[subview removeFromSuperview];
	}
	
	for (NSUInteger index = 0; index < [self.dataSource numberOfObjectsInScriptView:self]; index++) {
		NWScriptObjectControl *subview = [self.dataSource scriptView:self objectForIndex:index];
		subview.tag = index;
		[self addSubview:subview];
	}
	[self fixLocationsOfSubviews];
}

- (void)fixLocationsOfSubviews {
	CGFloat xPosition = 20;
	CGFloat width = 0;
	for (NWScriptObjectView *subview in self.subviews) {
		if ([subview isKindOfClass:[NWScriptObjectView class]]) {
			width = [self.dataSource scriptView:self widthOfObjectAtIndex:subview.tag];
			
			NSUInteger heightFactor = 5;
			if (subview.endColors.count) {
				heightFactor = @((self.bounds.size.height - 10) / subview.endColors.count).unsignedIntegerValue;
			}
			subview.frame = CGRectMake(xPosition, 10, width, subview.endColors.count * heightFactor);
			xPosition += width + 2;
		}
	}
	[self setContentSize:CGSizeMake(xPosition + width, self.bounds.size.height)];

	for (NWScriptObjectView *subview in self.subviews) {
		if ([subview isKindOfClass:[NWTimeLineView class]]) {
			subview.frame = CGRectMake(10, 10, self.contentSize.width, self.contentSize.height);
			NWTimeLineView *timeLine = (NWTimeLineView *)subview;
			timeLine.contentOffset = CGPointZero;
			timeLine.contentFrame = self.frame;
		}
	}
}

@end
