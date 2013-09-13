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

@implementation NWScriptView

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
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	[self fixLocationsOfSubviews];
}

- (void)setDataSource:(id<NWScriptViewDataSource>)dataSource {
	_dataSource = dataSource;
	[self reloadData];
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
			
			subview.frame = CGRectMake(xPosition, 10, width, self.frame.size.height - 20);
			xPosition += width + 2;
		}
	}
	[self setContentSize:CGSizeMake(xPosition + width, self.bounds.size.height)];

}

@end
