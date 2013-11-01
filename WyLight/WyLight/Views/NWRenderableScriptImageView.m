//
//  NWRenderableScriptImageView.m
//  WyLight
//
//  Created by Nils Weiß on 30/10/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWRenderableScriptImageView.h"

@interface NWRenderableScriptImageView ()

@property (nonatomic, strong) UIActivityIndicatorView *activityIndicator;

@end

@implementation NWRenderableScriptImageView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.activityIndicator = [[UIActivityIndicatorView alloc] initWithFrame:self.frame];
        self.activityIndicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyleWhiteLarge;
        self.activityIndicator.hidden = NO;
        [self addSubview:self.activityIndicator];
        self.activityIndicator.hidesWhenStopped = YES;
        [self bringSubviewToFront:self.activityIndicator];
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

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
