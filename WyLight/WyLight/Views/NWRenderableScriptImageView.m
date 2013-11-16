//
//  NWRenderableScriptImageView.m
//  WyLight
//
//  Created by Nils Weiß on 30/10/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWRenderableScriptImageView.h"
#import "UIView+Quivering.h"

@interface NWRenderableScriptImageView ()

@property (nonatomic, strong) UIActivityIndicatorView *activityIndicator;
@property (nonatomic, strong) CAGradientLayer *gradientLayer;
@property (nonatomic, strong) UIImageView *reflectionView;

@end

@implementation NWRenderableScriptImageView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.activityIndicator = [[UIActivityIndicatorView alloc] initWithFrame:self.frame];
        self.activityIndicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyleWhiteLarge;
        self.activityIndicator.hidden = NO;
        [self addSubview:self.activityIndicator];
        self.activityIndicator.hidesWhenStopped = YES;
        [self bringSubviewToFront:self.activityIndicator];
        
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
		self.clipsToBounds = YES;
        self.opaque = YES;
    }
    return self;
}

- (void)setShowActivityIndicator:(BOOL)showActivityIndicator {
    if (showActivityIndicator) {
        [self.activityIndicator startAnimating];
    } else {
        [self.activityIndicator stopAnimating];
    }
}

#define DOWNSCALEFACTOR 0.8

- (void)setFrame:(CGRect)frame {
    if (self.downscale) {
        [UIView setAnimationsEnabled:NO];
		self.layer.transform = CATransform3DMakeScale(1.0, 1.0, 1.0);
		[super setFrame:frame];
		self.layer.transform = CATransform3DMakeScale(DOWNSCALEFACTOR, DOWNSCALEFACTOR, 1.0);
        [UIView setAnimationsEnabled:YES];
	} else {
		[super setFrame:frame];
	}

	float dim = MIN(self.bounds.size.width, self.bounds.size.height);
	if (self.cornerRadius != 0.0) {
		self.layer.cornerRadius = self.cornerRadius;
	} else {
		self.layer.cornerRadius = dim/8;
	}
}

- (void)setQuivering:(BOOL)quivering {
	
	if (_quivering == quivering) {
		return;
	} else {
		_quivering = quivering;
		if (quivering) {
			[self startQuivering];
		} else {
			[self stopQuivering];
		}
	}
}

- (void)setDownscale:(BOOL)downscale {
	if (_downscale == downscale) {
		//nothing to do
		return;
	} else {
		if (downscale) {
			self.layer.transform = CATransform3DMakeScale(DOWNSCALEFACTOR, DOWNSCALEFACTOR, 1.0);
		} else {
			self.layer.transform = CATransform3DMakeScale(1.0, 1.0, 1.0);
		}
		_downscale = downscale;
	}
}

- (void)setImage:(UIImage *)image {
    super.image = image;
    
    if (self.animateSetImage) {
        CATransition *transition = [CATransition animation];
        transition.duration = 0.3f;
        transition.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
        transition.type = kCATransitionFade;
    
        [super.layer addAnimation:transition forKey:nil];
    }
}

@end
