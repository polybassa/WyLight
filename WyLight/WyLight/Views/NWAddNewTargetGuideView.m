//
//  NWAddNewTargetGuideView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 07.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAddNewTargetGuideView.h"

@implementation NWAddNewTargetGuideView

- (void)setCurrentPageIndex:(NSUInteger)currentPageIndex
{
	if (currentPageIndex < [self.pageImageStrings count]) {
		_currentPageIndex = currentPageIndex;
	}
	[self setNeedsDisplay];
}

- (void)setPageImageStrings:(NSArray *)pageImageStrings
{
	_pageImageStrings = pageImageStrings;
	[self setNeedsDisplay];
}

#define DEFAULT_SCALE_FACTOR 0.90
- (void)drawRect:(CGRect)rect
{
	UIBezierPath *roundedRect = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(
																				   self.bounds.origin.x + 20,
																				   self.bounds.origin.y + 20,
																				   self.bounds.size.width - 40,
																				   self.bounds.size.height - 40)
														   cornerRadius:25];
	[roundedRect addClip];
	[[UIColor clearColor] setFill];
	UIRectFill(self.bounds);

    UIImage *image = [UIImage imageNamed:self.pageImageStrings[self.currentPageIndex]];
	if(image)
	{
		[image drawInRect:self.bounds];
	}
}

@end
