//
//  NWDefaultColorPickerViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol NWDefaultColorControllerDelegate;
@class WCWiflyControlWrapper;

@interface NWDefaultColorPickerViewController : UIViewController

@property (nonatomic, strong) UIColor *selectedColor;
@property (nonatomic, weak) id <NWDefaultColorControllerDelegate> colorPickerDelegate;

@end

@protocol NWDefaultColorControllerDelegate
- (void) defaultColorController:(NWDefaultColorPickerViewController *)controller didChangeColor:(UIColor *)color;
@end