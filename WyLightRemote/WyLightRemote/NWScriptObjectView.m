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

@end

@implementation NWScriptObjectView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
		self.layer.shouldRasterize = YES;
		
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
        self.clipsToBounds = YES;
        self.layer.opacity = 1;
        self.layer.borderColor = [UIColor darkGrayColor].CGColor;
		self.backgroundColor = [UIColor blackColor];
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
	[super setFrame:frame];
	float dim = MIN(self.bounds.size.width, self.bounds.size.height);
	if (self.cornerRadius >= 1) {
		self.layer.cornerRadius = self.cornerRadius;
	} else {
		self.layer.cornerRadius = dim/8;
	}
	if (self.borderWidth >= 1) {
		self.layer.borderWidth = self.borderWidth;
	} else {
		self.layer.borderWidth = 2.0;
	}
	[self drawAllColorViews];
}

- (void)drawAllColorViews {
	
	for (UIView *view in self.gradientViews) {
		[view removeFromSuperview];
	}
	[self.gradientViews removeAllObjects];
	
	const CGFloat heightFract = self.bounds.size.height / self.endColors.count;
	
	for (unsigned int i = 0; i < self.endColors.count; i++) {
		CGFloat rectOriginY = floorf(i * heightFract);
		CGFloat nextRectOriginY = floorf((i + 1) * heightFract);
		CGFloat rectHeight = nextRectOriginY - rectOriginY;
		NWGradientView *gradientView = [[NWGradientView alloc] initWithFrame: CGRectMake(self.bounds.origin.x, rectOriginY, self.bounds.size.width, rectHeight)];
		UIColor *startColor;
		if ((self.startColors) && (i < self.startColors.count)) {
			startColor = [self.startColors objectAtIndex:i];
		} else {
			startColor = self.backgroundColor;
		}
		gradientView.startColor = startColor;
		gradientView.endColor = self.endColors[i];
		[self addSubview:gradientView];
		[self.gradientViews addObject:gradientView];
	}
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

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[super touchesBegan:touches withEvent:event];
	self.latestTouchBegan = touches.anyObject;
}

- (void)setBorderWidth:(CGFloat)borderWidth {
	_borderWidth = borderWidth;
	self.layer.borderWidth = _borderWidth;
}

- (void)setCornerRadius:(CGFloat)cornerRadius {
	_cornerRadius = cornerRadius;
	self.layer.cornerRadius = _cornerRadius;
}

- (void)setBorderColor:(UIColor *)borderColor {
	_borderColor = borderColor;
	self.layer.borderColor = _borderColor.CGColor;
}

@end
