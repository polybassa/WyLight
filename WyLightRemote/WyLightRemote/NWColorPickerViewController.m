//
//  NWColorPickerViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 07.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWColorPickerViewController.h"
#import "KZColorPicker.h"
#import "WCWiflyControlWrapper.h"

@implementation NWColorPickerViewController

- (void) defaultColorController:(NWDefaultColorPickerViewController *)controller didChangeColor:(UIColor *)color
{
	[self.controlHandle setColorDirect:color];
}

- (void) viewDidLoad
{
	[self setColorPickerDelegate:self];
}
@end

