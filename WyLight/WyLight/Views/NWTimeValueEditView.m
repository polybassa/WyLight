//
//  NWTimeValueEditView.m
//  WyLight
//
//  Created by Nils Weiß on 12.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWTimeValueEditView.h"
#import "NWScriptCommandObject.h"
#import "NWComplexScriptCommandObject.h"

@interface NWTimeValueEditView ()

@property (nonatomic, strong) UISlider *timeSlider;
@property (nonatomic, strong) UILabel *timeLabel;
@property (nonatomic, strong) UILabel *waitInfoLabel;
@property (nonatomic, strong) UISwitch *waitSwitch;

@end

@implementation NWTimeValueEditView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
		self.clipsToBounds = YES;

		self.timeSlider = [[UISlider alloc] initWithFrame:CGRectZero];
		self.timeSlider.continuous = YES;
		self.timeSlider.maximumValue = @(0xffff).floatValue;
		self.timeSlider.minimumValue = 0;
		[self.timeSlider addTarget:self action:@selector(sliderValueChanged:) forControlEvents:UIControlEventValueChanged];
		[self addSubview:self.timeSlider];
		
		self.timeLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.timeLabel.textAlignment = NSTextAlignmentCenter;
		[self addSubview:self.timeLabel];
		
		self.waitInfoLabel = [[UILabel alloc]initWithFrame:CGRectZero];
		self.waitInfoLabel.text = NSLocalizedStringFromTable(@"WaitInfoLabelKey", @"ScriptObjectEditViewsLocalization", @"");
		[self addSubview:self.waitInfoLabel];
		
		self.waitSwitch = [[UISwitch alloc] initWithFrame:CGRectZero];
		[self.waitSwitch addTarget:self action:@selector(switchValueChanged:) forControlEvents:UIControlEventValueChanged];
		[self addSubview:self.waitSwitch];
		
		[self setFrame:frame];
        // Initialization code
    }
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	{
		self.waitInfoLabel.frame = CGRectMake(self.bounds.size.width / 10, self.bounds.size.height / 10, self.bounds.size.width / 2, self.bounds.size.height / 4);
		self.waitSwitch.frame = CGRectMake(self.bounds.size.width * 3 / 4, self.bounds.size.height / 10, self.bounds.size.width / 3, self.bounds.size.height / 4);
		self.timeSlider.frame =  CGRectMake(self.bounds.size.width / 10, self.bounds.size.height / 2, self.bounds.size.width - self.bounds.size.width / 5 , self.bounds.size.height / 4);
		self.timeLabel.frame =  CGRectMake(self.bounds.size.width / 10, self.bounds.size.height * 3 / 4,  self.bounds.size.width - self.bounds.size.width / 5 , self.bounds.size.height / 4);
	}
}

- (void)setCornerRadius:(CGFloat)cornerRadius {
	self.layer.cornerRadius = cornerRadius;
	[self setNeedsDisplay];
}

- (void)setCommand:(NWScriptCommandObject *)command {
	_command = command;
	[self reloadData];
}

- (void)reloadData {
	self.timeSlider.value = self.command.duration;
	self.timeLabel.text =  [NSString stringWithFormat:@"%@ %2.1f s",
                            NSLocalizedStringFromTable(@"TimeLabelKey", @"ScriptObjectEditViewsLocalization", @""),
                            ((float)self.command.duration / 100)];
	
	if ([self.command isKindOfClass:[NWComplexScriptCommandObject class]]) {
		self.waitInfoLabel.hidden = NO;
		self.waitSwitch.hidden = NO;
		self.waitSwitch.on = !((NWComplexScriptCommandObject *)self.command).waitCommand;
	} else {
		self.waitSwitch.hidden = YES;
		self.waitInfoLabel.hidden = YES;
	}
}

- (void)sliderValueChanged:(UISlider *)sender {
	if (self.delegate) {
		[self.delegate TimeValueEditView:self sliderValueChanged:sender.value];
	}
}

- (void)switchValueChanged:(UISwitch *)sender {
	if (self.delegate) {
		[self.delegate TimeValueEditView:self switchValueChanged:sender.on];
	}
}

@end
