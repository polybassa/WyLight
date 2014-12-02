//
//  NWScriptView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWScriptView.h"
#import "NWScriptCellView.h"

@implementation NWScriptView

#pragma mark - SETUP

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		[self setup];
		self.scriptObjectSpacing  = 0.0;
        self.tag = -1;
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
	for (NSUInteger i = 0; i < self.subviews.count; i++) {
        UIView *subview = [self viewWithTag:i];
		if (subview && [subview isKindOfClass:[UIView class]]) {
			width = [self.dataSource scriptView:self widthOfObjectAtIndex:subview.tag];
			subview.frame = CGRectMake(xPosition, yPosition, width, height);
			xPosition += floorf(width) + self.scriptObjectSpacing;
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

- (void)reloadViewAtIndex:(NSUInteger)idx animated:(BOOL)animated {
    UIView *oldView = [self viewWithTag:idx];
    if (oldView == nil) {
        NSLog(@"%d: %s --> FAILED", __LINE__, __FUNCTION__);
        return;
    }
    UIView *newView = [self.dataSource scriptView:self cellViewForIndex:idx];
    newView.frame = oldView.frame;
    newView.tag = oldView.tag;
    
    if (animated) {
        newView.alpha = 0;
    }
    [self addSubview:newView];
    if (animated) {
        [UIView animateWithDuration:0.3 animations:^{
            newView.alpha = 1;
            oldView.alpha = 0;
        }];
    }
    [oldView removeFromSuperview];
}

@end
