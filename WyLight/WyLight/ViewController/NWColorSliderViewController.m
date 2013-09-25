//
//  NWColorSliderViewController.m
//  WyLight
//
//  Created by Nils Weiß on 20.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWColorSliderViewController.h"
#import "WCWiflyControlWrapper.h"

@interface NWColorSliderViewController () <UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UISlider *redSlider;
@property (weak, nonatomic) IBOutlet UISlider *greenSlider;
@property (weak, nonatomic) IBOutlet UISlider *blueSlider;

@property (weak, nonatomic) IBOutlet UITextField *redTextField;
@property (weak, nonatomic) IBOutlet UITextField *greenTextField;
@property (weak, nonatomic) IBOutlet UITextField *blueTextField;

@property (strong, nonatomic) UIColor *currentColor;
@property (nonatomic) BOOL sendClearCommandToControlHandleFirst;
@end

@implementation NWColorSliderViewController

@synthesize currentColor = _currentColor;

#define CURRENT_COLOR_KEY @"WyLightRemote.NWColorSliderViewController.currentColor"

- (void)viewDidLoad {
    [super viewDidLoad];
	NSData *colorData = [[NSUserDefaults standardUserDefaults] objectForKey:CURRENT_COLOR_KEY];
	if (colorData) {
		self.currentColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorData];
		
		CGFloat red, green, blue;
		[self.currentColor getRed:&red green:&green blue:&blue alpha:NULL];
		self.redSlider.value = red;
		self.greenSlider.value = green;
		self.blueSlider.value = blue;
	}
	
	self.redTextField.enabled = NO;
	self.greenTextField.enabled = NO;
	self.blueTextField.enabled = NO;
	
	//self.redTextField.
}

- (void)viewWillDisappear:(BOOL)animated {
	NSData *colorData = [NSKeyedArchiver archivedDataWithRootObject:self.currentColor];
	[[NSUserDefaults standardUserDefaults] setObject:colorData forKey:CURRENT_COLOR_KEY];
	[super viewWillDisappear:animated];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	self.sendClearCommandToControlHandleFirst = YES;
	self.view.backgroundColor = self.currentColor;
}

- (void)sendColor:(UIColor *)color {
	if (self.controlHandle) {
		if (self.sendClearCommandToControlHandleFirst) {
			[self.controlHandle clearScript];
			self.sendClearCommandToControlHandleFirst = NO;
		}
		[self.controlHandle setColorDirect:color];
	}
}

- (UIColor *)currentColor {
	if (!_currentColor) {
		_currentColor = [UIColor colorWithRed:self.redSlider.value green:self.greenSlider.value blue:self.blueSlider.value alpha:1.0];
	}
	return _currentColor;
}

- (void)setCurrentColor:(UIColor *)currentColor {
	_currentColor = currentColor;
	self.view.backgroundColor = _currentColor;
	
	CGFloat red, green, blue;
	[_currentColor getRed:&red green:&green blue:&blue alpha:NULL];
	
	self.redTextField.text = [NSString stringWithFormat:@"%lu %%", (unsigned long)(red * 100)];
	self.greenTextField.text = [NSString stringWithFormat:@"%lu %%", (unsigned long)(green * 100)];
	self.blueTextField.text = [NSString stringWithFormat:@"%lu %%", (unsigned long)(blue * 100)];
	
	[self sendColor:currentColor];

}

- (IBAction)greenChanged:(UISlider *)sender {
	
	CGFloat red, green, blue;
	[self.currentColor getRed:&red green:&green blue:&blue alpha:NULL];
	
	self.currentColor = [UIColor colorWithRed:red green:sender.value blue:blue alpha:1.0];
}

- (IBAction)blueChanged:(UISlider *)sender {
	CGFloat red, green, blue;
	[self.currentColor getRed:&red green:&green blue:&blue alpha:NULL];
	
	self.currentColor = [UIColor colorWithRed:red green:green blue:sender.value alpha:1.0];
}

- (IBAction)redChanged:(UISlider *)sender {
	CGFloat red, green, blue;
	[self.currentColor getRed:&red green:&green blue:&blue alpha:NULL];
	
	self.currentColor = [UIColor colorWithRed:sender.value green:green blue:blue alpha:1.0];
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
