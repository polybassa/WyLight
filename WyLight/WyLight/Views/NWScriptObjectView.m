//
//  NWScriptObjectView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 28.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWScriptObjectView.h"
#import "NWGradientView.h"

@interface NWScriptObjectView ()

@property (nonatomic, strong) NSMutableArray *gradientViews;
@property (nonatomic, readwrite, strong) UITouch *latestTouchBegan;

@end

@implementation NWScriptObjectView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
		self.clipsToBounds = YES;
        self.opaque = YES;
	}
    return self;
}

- (NSMutableArray *)gradientViews {
	if (!_gradientViews) {
		_gradientViews = [[NSMutableArray alloc] init];
	}
	return _gradientViews;
}

- (void)setFrame:(CGRect)frame {
	
	if (CGRectEqualToRect(frame, super.frame)) {
		return;
	}
	
	[super setFrame:frame];
	
	float dim = MIN(self.bounds.size.width, self.bounds.size.height);
	if (self.cornerRadius != 0.0) {
		self.layer.cornerRadius = self.cornerRadius;
	} else {
		self.layer.cornerRadius = dim/8;
	}
	//find a fix to change the frame of a drawn gradient view
	[self setFrameOfAllColorViews:self.bounds];
	//[self drawAllColorViews];
}

- (void)setFrameOfAllColorViews:(CGRect)frame {
	
	const CGFloat heightFract = self.gradientViews.count ? frame.size.height / self.gradientViews.count : 1;
	
	for (unsigned int i = 0; i < self.gradientViews.count; i++) {
		CGFloat rectOriginY = floorf(i * heightFract);
		CGFloat nextRectOriginY = floorf((i + 1) * heightFract);
		CGFloat rectHeight = nextRectOriginY - rectOriginY;
		((UIView*)self.gradientViews[i]).frame = CGRectMake(0, rectOriginY, frame.size.width, rectHeight);
	}
}

- (void)drawAllColorViews {
	
	for (UIView *view in self.gradientViews) {
		[view removeFromSuperview];
	}
	[self.gradientViews removeAllObjects];
	
	const CGFloat heightFract = self.bounds.size.height / self.endColors.count;
	
    UIColor *startColor = [UIColor clearColor];
    
	for (unsigned int i = 0; i < self.endColors.count; i++) {
		CGFloat rectOriginY = floorf(i * heightFract);
		CGFloat nextRectOriginY = floorf((i + 1) * heightFract);
		CGFloat rectHeight = nextRectOriginY - rectOriginY;
		NWGradientView *gradientView = [[NWGradientView alloc] initWithFrame: CGRectMake(self.bounds.origin.x, rectOriginY, self.bounds.size.width, rectHeight)];
		gradientView.opaque = NO;
		
		if ((self.startColors) && (i < self.startColors.count)) {
			startColor = [self.startColors objectAtIndex:i];
		} else {
			startColor = self.backgroundColor;
		}
		gradientView.startColor = [startColor copy];
		gradientView.endColor = self.endColors[i];
		[self addSubview:gradientView];
		[self.gradientViews addObject:gradientView];
	}
    [self setNeedsDisplay];
}

- (void)setColorsAnimatedWithDuration:(NSTimeInterval)duration startColors:(NSArray *)startColors endColor:(NSArray *)endColors {

	for (NSUInteger i = 0; i < startColors.count && i < endColors.count && i < self.gradientViews.count; i++) {
		[((NWGradientView *)self.gradientViews[i]) setColorsAnimatedWithDuration:duration
																	  startColor:startColors[i]
																		endColor:endColors[i]];
	}
	_startColors = startColors;
	_endColors = endColors;
}

- (void)setEndColors:(NSArray *)endColors {
	_endColors = endColors;
	[self drawAllColorViews];
}

- (void)setStartColors:(NSArray *)startColors {
	_startColors = startColors;
	[self drawAllColorViews];
}

- (void)setStartColors:(NSArray *)startColors withEndcolors:(NSArray *)endColors {
    _startColors = startColors;
    _endColors = endColors;
    [self drawAllColorViews];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[super touchesBegan:touches withEvent:event];
	self.latestTouchBegan = touches.anyObject;
}

- (void)setCornerRadius:(CGFloat)cornerRadius {
	self.layer.cornerRadius = cornerRadius;
}

- (CGFloat)cornerRadius {
	return self.layer.cornerRadius;
}

- (void)setOrientation:(CGFloat)orientation {
	if (_orientation != orientation) {
		
		self.transform = CGAffineTransformMakeRotation(orientation - _orientation);
		_orientation = orientation;
	}
}

- (void)setHidden:(BOOL)hidden {
	if (hidden == super.hidden) {
		return;
	}
	[super setHidden:hidden];
	if (!hidden) {
		[self drawAllColorViews];
	}
}
@end
