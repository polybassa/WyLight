//
//  KZColorWheelView.h
//
//  Created by Alex Restrepo on 5/11/11.
//  Copyright 2011 KZLabs http://kzlabs.me
//  All rights reserved.
//

#import <UIKit/UIKit.h>

@class KZColorPickerHSWheel;
@class KZColorPickerBrightnessSlider;
@class KZColorPickerSwatchView;

@interface KZColorPicker : UIControl
{
	KZColorPickerHSWheel *colorWheel;
	KZColorPickerBrightnessSlider *brightnessSlider;
	
    NSMutableArray *swatches;
    
	UIColor *selectedColor;
    BOOL displaySwatches;
}

@property (nonatomic, retain) UIColor *selectedColor;
- (void) setSelectedColor:(UIColor *)color animated:(BOOL)animated;
@end
