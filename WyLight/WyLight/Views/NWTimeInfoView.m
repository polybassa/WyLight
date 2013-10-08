//
//  NWTimeInfoView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 14.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWTimeInfoView.h"

@interface NWTimeInfoView ()

@end

@implementation NWTimeInfoView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
		self.opaque = NO;
		self.backgroundColor = [UIColor clearColor];
		self.timeScaleFactor = 1;
		
	}
    return self;
}

- (void)setTimeScaleFactor:(CGFloat)timeScaleFactor {
	_timeScaleFactor = timeScaleFactor;
	[self setNeedsDisplay];
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	[self setNeedsDisplay];
}

- (void)drawRect:(CGRect)rect
{
	CGContextRef context = UIGraphicsGetCurrentContext();
	
	// Drawing lines with a white stroke color
	CGContextSetRGBStrokeColor(context, 0.0, 0.0, 0.0, 1.0);
	// Draw them with a 2.0 stroke width so they are a bit more visible.
	CGContextSetLineWidth(context, 4.0);
	CGContextSetLineCap(context, kCGLineCapRound);
	
	// Draw a single line from left to right
	
	CGContextMoveToPoint(context, rect.origin.x, rect.origin.y + rect.size.height / 1.5);
	CGContextAddLineToPoint(context, rect.origin.x + rect.size.width, rect.origin.y + rect.size.height / 1.5);
	
	CGContextSetLineWidth(context, 2.0);
	CGContextMoveToPoint(context, rect.origin.x + 1, rect.origin.y + rect.size.height / 3 + 1);
	CGContextAddLineToPoint(context, rect.origin.x + 1, rect.origin.y + rect.size.height - 1);
	
	CGContextMoveToPoint(context, rect.origin.x + rect.size.width - 1, rect.origin.y + rect.size.height / 3 + 1);
	CGContextAddLineToPoint(context, rect.origin.x + rect.size.width - 1, rect.origin.y + rect.size.height - 1);
	
	CGContextStrokePath(context);
	
	NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
	paragraphStyle.alignment = NSTextAlignmentCenter;
	
	CGFloat fontSize = 15;
	UIFont *font;
	NSAttributedString *text;
	
	do {
		font = [UIFont systemFontOfSize:fontSize];
		
		text = [[NSAttributedString alloc]
					initWithString:[NSString stringWithFormat:@"%2.1f s", (self.frame.size.width / self.timeScaleFactor) / 100]
					attributes:@{
					NSParagraphStyleAttributeName : paragraphStyle,
					NSForegroundColorAttributeName : [UIColor blueColor],
					NSFontAttributeName : font }];
		fontSize -= 1.0;		
	} while ([text size].width > rect.size.width - 10 && fontSize > 1.0);
	
	CGRect textBounds;
	textBounds.size = [text size];
	textBounds.origin = CGPointMake(rect.origin.x + rect.size.width / 2 - [text size].width / 2, rect.origin.y + rect.size.height / 6);
	
	[[UIColor clearColor] setFill];
	CGContextFillRect(context, CGRectInset(textBounds, -2, 0));
	[text drawInRect:textBounds];
}


@end
