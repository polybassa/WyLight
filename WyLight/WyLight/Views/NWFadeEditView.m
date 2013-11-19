//
//  NWFadeEditView.m
//  WyLight
//
//  Created by Nils Weiß on 13.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWFadeEditView.h"
#import "NWScriptObjectView.h"
#import "Fade.h"

@interface NWFadeEditView ()

@property (nonatomic, strong) NWScriptObjectView *fadeEditView;
@property (nonatomic, assign) uint32_t addressMaskForFadeEditView;
@property (nonatomic, strong) UILabel *titleLabel;

@end

@implementation NWFadeEditView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
		self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
        self.clipsToBounds = YES;
		
		UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapOnFadeEditView:)];
				
		self.fadeEditView = [[NWScriptObjectView alloc] initWithFrame:CGRectZero];
		self.fadeEditView.backgroundColor = [UIColor colorWithRed:0.7 green:0.7 blue:0.7 alpha:0.2];
        self.fadeEditView.opaque = YES;
		[self.fadeEditView addGestureRecognizer:tap];
		self.fadeEditView.orientation = 90 * M_PI / 180;
		self.fadeEditView.cornerRadius = 5.0;
		[self addSubview:self.fadeEditView];
		
		self.titleLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.titleLabel.text = NSLocalizedStringFromTable(@"FadeInfoLabelKey", @"ScriptObjectEditViewsLocalization", @"");
		self.titleLabel.textAlignment = NSTextAlignmentCenter;
        [self.titleLabel setFont:[UIFont fontWithName:@"Arial-BoldMT" size:18]];
		[self addSubview:self.titleLabel];
    
		[self setFrame:frame];
	}
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	
	self.titleLabel.frame = CGRectMake(self.bounds.size.width / 10, self.bounds.size.width / 16, self.bounds.size.width - self.bounds.size.width / 5, self.bounds.size.height / 6);
	
	self.fadeEditView.frame = CGRectMake(1, self.bounds.size.width / 5, self.bounds.size.width - 2, self.bounds.size.height * 2 / 3);
}

- (void)setCornerRadius:(CGFloat)cornerRadius {
	self.layer.cornerRadius = cornerRadius;
	[self setNeedsDisplay];
}

- (void)setCommand:(Effect *)command {
	_command = command;
	[self reloadData];
}

- (void)reloadData {
	id currentCommand = self.command;
	
	if ([currentCommand isKindOfClass:[Fade class]]) {
		Fade *currentFadeCommand = (Fade *)currentCommand;
		self.addressMaskForFadeEditView = currentFadeCommand.address.unsignedIntegerValue;
        [self.fadeEditView setStartColors:currentFadeCommand.colors withEndcolors:currentFadeCommand.colors];
	} else {
		self.fadeEditView.endColors = @[[UIColor colorWithRed:0.7 green:0.7 blue:0.7 alpha:0.2]];
        self.fadeEditView.startColors = @[[UIColor colorWithRed:0.7 green:0.7 blue:0.7 alpha:0.2]];
	}
}

- (IBAction)tapOnFadeEditView:(UITapGestureRecognizer *)sender {
	if (sender.state == UIGestureRecognizerStateEnded) {
		id currentCommand = self.command;
		
		if ([currentCommand isKindOfClass:[Fade class]]) {
			Fade *currentFadeCommand = (Fade *)currentCommand;
			self.addressMaskForFadeEditView = currentFadeCommand.address.unsignedIntegerValue;
		}
		CGFloat heigthOfOneGradientStrip = self.fadeEditView.bounds.size.height / 8;
		CGPoint touchLocationInGradientView = [self.fadeEditView.latestTouchBegan locationInView:self.fadeEditView];
		[self flipAddressBitsAtIndex:(NSUInteger)(touchLocationInGradientView.y / heigthOfOneGradientStrip)];
	}
}

- (void)flipAddressBitsAtIndex:(NSUInteger)index {
	id currentCommand = self.command;
	if ([currentCommand isKindOfClass:[Fade class]]) {
		Fade *currentFadeCommand = (Fade *)currentCommand;
		
		uint32_t flipBitmask = 0x0000000f << index * 4;
		uint32_t newAddressMask = currentFadeCommand.address.unsignedIntegerValue ^ flipBitmask;
		
		if ((flipBitmask & self.addressMaskForFadeEditView & currentFadeCommand.address.unsignedIntegerValue) || (flipBitmask & ~self.addressMaskForFadeEditView & ~currentFadeCommand.address.unsignedIntegerValue)) {
			[self.delegate NWFadeEditView:self bitmaskChanged:newAddressMask];
		}
	}
}


@end
