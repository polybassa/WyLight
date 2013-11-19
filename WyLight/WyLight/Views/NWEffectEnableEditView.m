//
//  NWEffectEnableEditView.m
//  WyLight
//
//  Created by Nils Weiß on 18/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWEffectEnableEditView.h"
#import <QuartzCore/QuartzCore.h>
#import "ComplexEffect.h"

@interface NWEffectEnableEditView ()

@property (nonatomic, strong) UILabel *waitInfoLabel;
@property (nonatomic, strong) UISwitch *waitSwitch;
@property (nonatomic, strong) UILabel *switchInfoLabel;

@end

@implementation NWEffectEnableEditView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
		self.clipsToBounds = YES;
        
		self.waitInfoLabel = [[UILabel alloc]initWithFrame:CGRectZero];
		self.waitInfoLabel.text = NSLocalizedStringFromTable(@"WaitInfoLabelKey", @"ScriptObjectEditViewsLocalization", @"");
		self.waitInfoLabel.textAlignment = NSTextAlignmentCenter;
        [self.waitInfoLabel setFont:[UIFont fontWithName:@"Arial-BoldMT" size:18]];

        [self addSubview:self.waitInfoLabel];
		
		self.waitSwitch = [[UISwitch alloc] initWithFrame:CGRectZero];
		[self.waitSwitch addTarget:self action:@selector(switchValueChanged:) forControlEvents:UIControlEventValueChanged];
		[self addSubview:self.waitSwitch];
        
        self.switchInfoLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        self.switchInfoLabel.lineBreakMode = NSLineBreakByWordWrapping;
        self.switchInfoLabel.textAlignment = NSTextAlignmentCenter;
        self.switchInfoLabel.text = NSLocalizedStringFromTable(@"SwitchInfoLabelKey", @"ScriptObjectEditViewsLocalization", @"");
        self.switchInfoLabel.numberOfLines = 5;
        [self addSubview:self.switchInfoLabel];
        
		[self setFrame:frame];
    }
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	{
		self.waitInfoLabel.frame = CGRectMake(self.bounds.size.width / 10, self.bounds.size.height / 10, self.bounds.size.width / 2, self.bounds.size.height / 4);
		self.waitSwitch.frame = CGRectMake(self.bounds.size.width * 2 / 3, self.bounds.size.height / 10, self.bounds.size.width / 3, self.bounds.size.height / 4);
        self.waitInfoLabel.center = CGPointMake(self.waitInfoLabel.center.x, self.waitSwitch.center.y);
        
        self.switchInfoLabel.frame = CGRectMake(self.bounds.size.width / 10, self.bounds.size.height / 4, self.bounds.size.width - self.bounds.size.width / 5, self.bounds.size.height - self.bounds.size.height / 4);
    }
}

- (void)setCornerRadius:(CGFloat)cornerRadius {
	self.layer.cornerRadius = cornerRadius;
	[self setNeedsDisplay];
}

- (void)setCommand:(ComplexEffect *)command {
	_command = command;
	[self reloadData];
}

- (void)reloadData {
	self.waitInfoLabel.hidden = NO;
	self.waitSwitch.hidden = NO;
	self.waitSwitch.on = ![[self.command waitCommand] boolValue];
}

- (void)switchValueChanged:(UISwitch *)sender {
	if (self.delegate) {
		[self.delegate EffectEnableEditView:self switchValueChanged:sender.on];
	}
}

@end
