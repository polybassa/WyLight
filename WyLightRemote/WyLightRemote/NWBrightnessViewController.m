//
//  NWBrightnessViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 07.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWBrightnessViewController.h"
#import "WCWiflyControlWrapper.h"

@interface NWBrightnessViewController ()
@property (weak, nonatomic) IBOutlet UISlider *brightnessSlider;
@property (weak, nonatomic) IBOutlet UIStepper *brightnessStepper;

@end

@implementation NWBrightnessViewController

- (void) viewDidLoad
{
	self.brightnessSlider.value = self.brightnessStepper.value;
}

- (IBAction)sliderValueChanged:(UISlider *)sender {
	[self.controlHandle setColorDirect:[UIColor colorWithRed:sender.value green:sender.value blue:sender.value alpha:1.0]];
	self.brightnessStepper.value = sender.value;
	
}

- (IBAction)stepperValueChanged:(UIStepper *)sender {
	[self.controlHandle setColorDirect:[UIColor colorWithRed:sender.value green:sender.value blue:sender.value alpha:1.0]];
	self.brightnessSlider.value = sender.value;
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
	[super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
	
	if (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || toInterfaceOrientation == UIInterfaceOrientationLandscapeRight) {
		[self.tabBarController.tabBar setHidden:YES];
	}
	else {
		[self.tabBarController.tabBar setHidden:NO];
	}
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	[super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
	
	if (UIInterfaceOrientationIsLandscape([[UIApplication sharedApplication] statusBarOrientation]))
	{
		CGRect biggerFrame = self.tabBarController.view.frame;
		biggerFrame.size.height += self.tabBarController.tabBar.frame.size.height;
		self.tabBarController.view.frame = biggerFrame ;
	}
}

@end
