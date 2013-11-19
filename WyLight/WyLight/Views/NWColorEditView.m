//
//  NWColorEditView.m
//  WyLight
//
//  Created by Nils Weiß on 13.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWColorEditView.h"
#import "NWScriptObjectView.h"

@interface NWColorEditView ()

@property (nonatomic, strong) NSArray *colorImages;
@property (nonatomic, strong, readwrite) NSArray *colorButtons;
@property (nonatomic, strong) UILabel *titleLabel;

@end

@implementation NWColorEditView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
        self.clipsToBounds = YES;
		
		self.titleLabel = [[UILabel alloc] initWithFrame:CGRectZero];
		self.titleLabel.textAlignment = NSTextAlignmentCenter;
        [self.titleLabel setFont:[UIFont fontWithName:@"Arial-BoldMT" size:18]];
		[self addSubview:self.titleLabel];
		
		for (UIButton *button in self.colorButtons) {
			[self addSubview:button];
		}
		
		[self setFrame:frame];
    }
    return self;
}

- (void)setTitle:(NSString *)title {
	self.titleLabel.text = title;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	
	self.titleLabel.frame = CGRectMake(self.bounds.size.width / 10, self.bounds.size.width / 16, self.bounds.size.width - self.bounds.size.width / 5, self.bounds.size.height / 6);
	
	CGRect buttonArea = CGRectMake(1, self.bounds.size.width / 5, self.bounds.size.width - 2, self.bounds.size.height * 2 / 3);
	
	CGFloat buttonSpacing = 2;
	NSUInteger buttonRows = 2;
	NSUInteger buttonsInARow = self.colorButtons.count / buttonRows;
	CGFloat buttonWidth = (buttonArea.size.width - (buttonsInARow + 1) * buttonSpacing) / buttonsInARow;
	CGFloat buttonHeigth = (buttonArea.size.height - (buttonRows + 1) * buttonSpacing) / buttonRows;
	
	NSUInteger index = 0;
	NSUInteger xPos = buttonArea.origin.x + buttonSpacing;
	NSUInteger yPos = buttonArea.origin.y + buttonSpacing;
	for (NSUInteger j = 0; j < buttonRows; j++) {
		for (NSUInteger i = 0; i < buttonsInARow; i++) {
			UIButton *button = [self.colorButtons objectAtIndex:index++];
			button.frame = CGRectMake(xPos, yPos, buttonWidth, buttonHeigth);
			xPos += buttonSpacing + buttonWidth;
		}
		xPos = buttonArea.origin.x + buttonSpacing;
		yPos += buttonSpacing + buttonHeigth;
	}
}

- (void)setCornerRadius:(CGFloat)cornerRadius {
	self.layer.cornerRadius = cornerRadius;
	[self setNeedsDisplay];
}

+ (NSArray *)allColors {
	return @[[UIColor greenColor], [UIColor yellowColor], [UIColor orangeColor], [UIColor redColor], [UIColor magentaColor], [UIColor purpleColor], [UIColor cyanColor],  [UIColor blueColor], [UIColor colorWithRed:1 green:1 blue:1 alpha:1]];
}

- (NSArray *)colorImages {
	if (!_colorImages) {
		NSMutableArray *tempArray = [[NSMutableArray alloc]init];
		
		for (UIColor *color in [NWColorEditView allColors]) {
			[tempArray addObject:[NWColorEditView imageWithColors:@[color]]];
		}
		
		[tempArray addObject:[NWColorEditView imageWithColors:[NWColorEditView allColors]]];
		_colorImages = tempArray;
	}
	return _colorImages;
}

- (NSArray *)colorButtons {
	if (!_colorButtons) {
		NSMutableArray *tempArray = [[NSMutableArray alloc]init];
		NSUInteger index = 0;
		for (UIImage *image in self.colorImages) {
			UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
			[button setBackgroundImage:image forState:UIControlStateNormal];
			[button addTarget:self action:@selector(buttonTouchUpInside:) forControlEvents:UIControlEventTouchUpInside];
			button.tag = index++;
			[tempArray addObject:button];
		}
		_colorButtons = tempArray;
	}
	return _colorButtons;
}

+ (UIImage *)imageWithView:(UIView *)view {
    UIGraphicsBeginImageContextWithOptions(view.bounds.size, view.opaque, 0.0);
    [view.layer renderInContext:UIGraphicsGetCurrentContext()];
	
    UIImage * img = UIGraphicsGetImageFromCurrentImageContext();
	
    UIGraphicsEndImageContext();
	
    return img;
}

+ (UIImage *)imageWithColors:(NSArray *)colors {
	NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 64, 64)];
	view.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.8 alpha:0.7];
	view.opaque = NO;
	view.endColors = colors;
	return [NWColorEditView imageWithView:view];
}

- (void)buttonTouchUpInside:(UIButton *)sender {
	if (sender.tag < [NWColorEditView allColors].count) {
		[self.delegate NWColorEditView:self changedColor:[[NWColorEditView allColors] objectAtIndex:sender.tag]];
	} else {
		//ColorPicker
		[self.delegate NWColorEditViewNeedsColorPicker:self];
	}
}

@end
