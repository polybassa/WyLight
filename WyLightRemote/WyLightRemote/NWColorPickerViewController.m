//
//  NWColorPickerViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 07.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWColorPickerViewController.h"
#import "KZColorPicker.h"
#import "UIColor-Expanded.h"
#import "WCWiflyControlWrapper.h"

@interface NWColorPickerViewController ()

@property (nonatomic, strong) UIColor *selectedColor;

@end

@implementation NWColorPickerViewController

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{
	UIView *container = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	
	container.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	container.backgroundColor = [UIColor clearColor];
	self.view = container;
	
	KZColorPicker *picker = [[KZColorPicker alloc] initWithFrame:container.bounds];
    picker.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	picker.selectedColor = self.selectedColor;
    picker.oldColor = self.selectedColor;
	[picker addTarget:self action:@selector(pickerChanged:) forControlEvents:UIControlEventValueChanged];
	[self.view addSubview:picker];
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

- (void) pickerChanged:(KZColorPicker *)cp
{
    self.selectedColor = cp.selectedColor;
	[self.controlHandle setColorDirect:self.selectedColor];
}

@end

