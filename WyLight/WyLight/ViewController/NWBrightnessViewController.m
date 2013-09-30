//
//  NWBrightnessViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 07.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWBrightnessViewController.h"
#import "WCWiflyControlWrapper.h"
#import "HSV.h"
#import "KZColorPicker.h"

@interface NWBrightnessViewController ()

@property (weak, nonatomic) IBOutlet UISlider *brightnessSlider;
@property (weak, nonatomic) IBOutlet UIStepper *brightnessStepper;
@property (nonatomic) BOOL sendClearCommandToControlHandleFirst;
@property (nonatomic, strong) UIColor *currentColor;

@end

@implementation NWBrightnessViewController

- (void) viewDidLoad {
	[super viewDidLoad];
	self.brightnessSlider.value = self.brightnessStepper.value;
}

#define SELECTED_COLOR_KEY CURRENT_COLOR_KEY

- (void)viewDidAppear:(BOOL)animated {
	[super viewDidAppear:animated];
	self.sendClearCommandToControlHandleFirst = YES;
	[self deserializeColor];
}

- (void)deserializeColor {
	NSData *colorData = [[NSUserDefaults standardUserDefaults] objectForKey:SELECTED_COLOR_KEY];
	if (colorData) {
		self.currentColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorData];
		
		RGBType rgb = rgbFromUIColor(self.currentColor);
		HSVType hsv = RGB_to_HSV(rgb);
	
		self.brightnessSlider.value = self.brightnessStepper.value = hsv.v;
	}
}

- (void)viewWillDisappear:(BOOL)animated {
	NSData *colorData = [NSKeyedArchiver archivedDataWithRootObject:self.currentColor];
	[[NSUserDefaults standardUserDefaults] setObject:colorData forKey:SELECTED_COLOR_KEY];
	[super viewWillDisappear:animated];
}

RGBType rgbFromUIColor(UIColor *color)
{
	const CGFloat *components = CGColorGetComponents(color.CGColor);
	
	CGFloat r,g,b;
	
	switch (CGColorSpaceGetModel(CGColorGetColorSpace(color.CGColor)))
	{
		case kCGColorSpaceModelMonochrome:
			r = g = b = components[0];
			break;
		case kCGColorSpaceModelRGB:
			r = components[0];
			g = components[1];
			b = components[2];
			break;
		default:	// We don't know how to handle this model
			return RGBTypeMake(0, 0, 0);
	}
	
	return RGBTypeMake(r, g, b);
}

#pragma mark - VALUE CHANGED CALLBACK's

- (void)sendCurrentColor {
	if (self.controlHandle) {
		if (self.sendClearCommandToControlHandleFirst) {
			[self.controlHandle clearScript];
			self.sendClearCommandToControlHandleFirst = NO;
		}
		[self.controlHandle setColorDirect:self.currentColor];
	}
}

- (void)setCurrentColorWithHsvValue:(float)hsvValue {
	RGBType rgb = rgbFromUIColor(self.currentColor);
	HSVType hsv = RGB_to_HSV(rgb);
	
	hsv.v = hsvValue;
	
	rgb = HSV_to_RGB(hsv);
	self.currentColor = [UIColor colorWithRed:rgb.r green:rgb.g blue:rgb.b alpha:1.0];
}

- (IBAction)sliderValueChanged:(UISlider *)sender {
	
	self.brightnessStepper.value = sender.value;
	[self setCurrentColorWithHsvValue:sender.value];
	[self sendCurrentColor];
}

- (IBAction)stepperValueChanged:(UIStepper *)sender {
	self.brightnessSlider.value = sender.value;
	[self setCurrentColorWithHsvValue:sender.value];
	[self sendCurrentColor];
}

#pragma mark - HANDLE ROTATION

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
	[super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
	
	if (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || toInterfaceOrientation == UIInterfaceOrientationLandscapeRight) {
		[self.tabBarController.tabBar setHidden:YES];
	}
	else {
		[self.tabBarController.tabBar setHidden:NO];
	}
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	[super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
	
	if (UIInterfaceOrientationIsLandscape([[UIApplication sharedApplication] statusBarOrientation]))
	{
		CGRect biggerFrame = self.tabBarController.view.frame;
		biggerFrame.size.height += self.tabBarController.tabBar.frame.size.height;
		self.tabBarController.view.frame = biggerFrame ;
	}
}

@end
