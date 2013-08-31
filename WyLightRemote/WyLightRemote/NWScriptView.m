//
//  NWScriptView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWScriptView.h"
#import "NWScriptObjectView.h"

@implementation NWScriptView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
		[self setup];
	}
    return self;
}

- (void)setup {
	self.timelineScaleFactor = 1;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	[self fixLocationsOfSubviews];
}

- (void)setDataSource:(id<NWScriptViewDataSource>)dataSource {
	_dataSource = dataSource;
	[self reloadData];
}

- (void)setTimelineScaleFactor:(CGFloat)timelineScaleFactor
{
	_timelineScaleFactor = timelineScaleFactor;
	[self fixLocationsOfSubviews];
	[self setNeedsDisplay];
}

- (void)reloadData {
	for (NWScriptObjectView *subview in self.subviews) {
		[subview removeFromSuperview];
	}
	
	for (NSUInteger index = 0; index < [self.dataSource numberOfObjectsInScriptView:self]; index++) {
		NWScriptObjectView *subview = [self.dataSource scriptView:self objectForIndex:index];
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
				heightFactor = @(self.bounds.size.height / subview.endColors.count).unsignedIntegerValue;
			}
			subview.frame = CGRectMake(xPosition, 0, width, subview.endColors.count * heightFactor);
			xPosition += width + 2;
		}
	}
	[self setContentSize:CGSizeMake(xPosition + width, self.bounds.size.height)];
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
