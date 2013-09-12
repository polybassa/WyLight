//
//  NWTimeLineView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 01.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWTimeLineView.h"

@implementation NWTimeLineView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
		self.contentOffset = CGPointMake(0, 0);
		self.contentFrame = CGRectZero;
		self.strokeWidth = 2.0;
		self.timeLineWidth = self.bounds.size.width;
		self.timeLineScaleFactor = 1.0;
		self.scaleResolution = 100;
    }
    return self;
}

- (void)setTimeLineScaleFactor:(CGFloat)timeLineScaleFactor {
	_timeLineScaleFactor = timeLineScaleFactor;
	[self setNeedsDisplay];
}

- (void)setScaleResolution:(CGFloat)scaleResolution {
	_scaleResolution = scaleResolution;
	[self setNeedsDisplay];
}

- (void)setStrokeWidth:(CGFloat)strokeWidth {
	_strokeWidth = strokeWidth;
	[self setNeedsDisplay];
}
- (void)setContentOffset:(CGPoint)contentOffset {
	_contentOffset = contentOffset;
	[self setNeedsDisplay];
}

- (void)setContentFrame:(CGRect)contentFrame {
	_contentFrame = contentFrame;
	[self setNeedsDisplay];
}

#define ARROWSIZE 5
#define SCALESIZE 5
 
- (void)drawRect:(CGRect)rect
{
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	// Drawing lines with a white stroke color
	CGContextSetRGBStrokeColor(context, 1.0, 1.0, 1.0, 1.0);
	// Draw them with a 2.0 stroke width so they are a bit more visible.
	CGContextSetLineWidth(context, self.strokeWidth);
	CGContextSetLineCap(context, kCGLineCapRound);
	
	// Draw a single line from left to right
	
	CGContextMoveToPoint(context, self.contentOffset.x + self.contentFrame.size.width + self.contentFrame.origin.x, self.contentFrame.size.height + self.contentFrame.origin.y);
	CGContextAddLineToPoint(context, self.contentOffset.x + self.contentFrame.origin.x, self.contentFrame.size.height + self.contentFrame.origin.y);
	CGContextAddLineToPoint(context, self.contentOffset.x + self.contentFrame.origin.x, self.contentFrame.origin.y);
	
	CGContextMoveToPoint(context, self.contentOffset.x + self.contentFrame.origin.x, self.contentFrame.origin.y);
	CGContextAddLineToPoint(context, self.contentOffset.x + self.contentFrame.origin.x + ARROWSIZE, self.contentFrame.origin.y + ARROWSIZE);
	
	CGContextMoveToPoint(context, self.contentOffset.x + self.contentFrame.origin.x, self.contentFrame.origin.y);
	CGContextAddLineToPoint(context, self.contentOffset.x + self.contentFrame.origin.x - ARROWSIZE, self.contentFrame.origin.y - ARROWSIZE);
		
	for (CGFloat i = self.contentOffset.x + self.contentFrame.origin.x;
		 i < self.contentFrame.size.width + self.contentFrame.origin.x;
		 i = i + self.scaleResolution * self.timeLineScaleFactor) {
		
		CGContextMoveToPoint(context, i, self.contentFrame.size.height + self.contentFrame.origin.y - SCALESIZE);
		CGContextAddLineToPoint(context, i, self.contentFrame.size.height + self.contentFrame.origin.y + SCALESIZE);

	}
	CGContextStrokePath(context);
	
}



@end
