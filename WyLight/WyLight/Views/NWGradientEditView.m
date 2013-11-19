//
//  NWGradientEditView.m
//  WyLight
//
//  Created by Nils Weiß on 12.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWGradientEditView.h"
#import "TouchAndHoldButton.h"
#import "Gradient.h"


@interface NWGradientEditView ()

@property (nonatomic, strong, readwrite) TouchAndHoldButton *gradientUpButton;
@property (nonatomic, strong, readwrite) TouchAndHoldButton *gradientDownButton;
@property (nonatomic, strong, readwrite) TouchAndHoldButton *gradientPlusButton;
@property (nonatomic, strong, readwrite) TouchAndHoldButton *gradientMinusButton;

@end

@implementation NWGradientEditView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
        self.clipsToBounds = YES;
		
		self.gradientDownButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
		[self.gradientDownButton setImage:[UIImage imageNamed:@"Down_Icon"] forState:UIControlStateNormal];
		[self.gradientDownButton addTarget:self action:@selector(downButtonTouchUpInside:) forTouchAndHoldControlEventWithTimeInterval:0.3];
		[self.gradientDownButton addTarget:self action:@selector(downButtonTouchUpInside:) forControlEvents:UIControlEventTouchDown];
		[self addSubview:self.gradientDownButton];
		
		self.gradientUpButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
		[self.gradientUpButton setImage:[UIImage imageNamed:@"Up_Icon"] forState:UIControlStateNormal];
		[self.gradientUpButton addTarget:self action:@selector(upButtonTouchUpInside:) forTouchAndHoldControlEventWithTimeInterval:0.3];
		[self.gradientUpButton addTarget:self action:@selector(upButtonTouchUpInside:) forControlEvents:UIControlEventTouchDown];
		[self addSubview:self.gradientUpButton];
		
		self.gradientMinusButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
		[self.gradientMinusButton setImage:[UIImage imageNamed:@"Minus_Icon"] forState:UIControlStateNormal];
		[self.gradientMinusButton addTarget:self action:@selector(minusButtonTouchUpInside:) forTouchAndHoldControlEventWithTimeInterval:0.3];
		[self.gradientMinusButton addTarget:self action:@selector(minusButtonTouchUpInside:) forControlEvents:UIControlEventTouchDown];
		[self addSubview:self.gradientMinusButton];
		
		self.gradientPlusButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
		[self.gradientPlusButton setImage:[UIImage imageNamed:@"Plus_Icon"] forState:UIControlStateNormal];
		[self.gradientPlusButton addTarget:self action:@selector(plusButtonTouchUpInside:) forTouchAndHoldControlEventWithTimeInterval:0.3];
		[self.gradientPlusButton addTarget:self action:@selector(plusButtonTouchUpInside:) forControlEvents:UIControlEventTouchDown];
		[self addSubview:self.gradientPlusButton];
		
		[self setFrame:frame];
        // Initialization code
    }
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	{
		CGFloat buttonSize = self.bounds.size.width / 5;
		
		self.gradientDownButton.frame = CGRectMake(self.bounds.size.width / 2 - buttonSize / 2, self.bounds.size.height * 3 / 4 - buttonSize / 2, buttonSize, buttonSize);
		self.gradientUpButton.frame = CGRectMake(self.bounds.size.width / 2 - buttonSize / 2, self.bounds.size.height / 4 - buttonSize / 2, buttonSize, buttonSize);
		self.gradientMinusButton.frame = CGRectMake(self.bounds.size.width / 4 - buttonSize / 2, self.bounds.size.height / 2 - buttonSize / 2, buttonSize, buttonSize);
		self.gradientPlusButton.frame = CGRectMake(self.bounds.size.width * 3 / 4 - buttonSize / 2, self.bounds.size.height / 2 - buttonSize / 2, buttonSize, buttonSize);
	}
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
	if ([self.command isKindOfClass:[Gradient class]]) {
		Gradient *currentGradientCommand = (Gradient *)self.command;
		
		if (currentGradientCommand.numberOfLeds.unsignedIntegerValue <= 1) {
			self.gradientMinusButton.enabled = NO;
		} else {
			self.gradientMinusButton.enabled = YES;
		}
		
		if (currentGradientCommand.numberOfLeds.unsignedIntegerValue >= [Gradient maximalNumberOfLeds]) {
			self.gradientPlusButton.enabled = NO;
		} else {
			self.gradientPlusButton.enabled = YES;
		}
		
		if (currentGradientCommand.address.unsignedIntegerValue & 0x80000000) {
			self.gradientDownButton.enabled = NO;
		} else {
			self.gradientDownButton.enabled = YES;
		}
		
		if (currentGradientCommand.address.unsignedIntegerValue & 0x00000001) {
			self.gradientUpButton.enabled = NO;
		} else {
			self.gradientUpButton.enabled = YES;
		}
	} else {
		self.gradientMinusButton.enabled = NO;
		self.gradientPlusButton.enabled = NO;
		self.gradientDownButton.enabled = NO;
		self.gradientUpButton.enabled = NO;
	}
}

- (IBAction)upButtonTouchUpInside:(id)sender {
	[self.delegate NWGradientEditViewDelegateUpButtonPressed:self];
}

- (IBAction)downButtonTouchUpInside:(id)sender {
	[self.delegate NWGradientEditViewDelegateDownButtonPressed:self];
}

- (IBAction)plusButtonTouchUpInside:(id)sender {
	[self.delegate NWGradientEditViewDelegatePlusButtonPressed:self];
}

- (IBAction)minusButtonTouchUpInside:(id)sender {
	[self.delegate NWGradientEditViewDelegateMinusButtonPressed:self];
}


@end
