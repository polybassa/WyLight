//
//  KZColorWheelView.m
//
//  Created by Alex Restrepo on 5/11/11.
//  Copyright 2011 KZLabs http://kzlabs.me
//  All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "KZColorPicker.h"
#import "KZColorPickerHSWheel.h"
#import "KZColorPickerBrightnessSlider.h"
#import "HSV.h"
#import "UIColor-Expanded.h"
#import "KZColorPickerSwatchView.h"

#define IS_IPAD ([[UIDevice currentDevice] respondsToSelector:@selector(userInterfaceIdiom)] && [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)

@interface KZColorPicker()
@property (nonatomic, retain) KZColorPickerHSWheel *colorWheel;
@property (nonatomic, retain) KZColorPickerBrightnessSlider *brightnessSlider;
@property (nonatomic, retain) NSMutableArray *swatches;
- (void) fixLocations;
@end


@implementation KZColorPicker
@synthesize colorWheel;
@synthesize brightnessSlider;
@synthesize selectedColor;
@synthesize swatches;

- (void) setup
{
	// set the frame to a fixed 300 x 300
	self.backgroundColor = IS_IPAD ? [UIColor clearColor] : [UIColor whiteColor];
    
    
	// HS wheel
	KZColorPickerHSWheel *wheel = [[KZColorPickerHSWheel alloc] initAtOrigin:CGPointMake(40, 79)];
	[wheel addTarget:self action:@selector(colorWheelColorChanged:) forControlEvents:UIControlEventValueChanged];
	[self addSubview:wheel];
	self.colorWheel = wheel;
	[wheel release];
	
	// brightness slider
	KZColorPickerBrightnessSlider *slider = [[KZColorPickerBrightnessSlider alloc] initWithFrame:CGRectMake(24, 
																											341,
																											272,
																											38)];
	[slider addTarget:self action:@selector(brightnessChanged:) forControlEvents:UIControlEventValueChanged];
	[self addSubview:slider];
	self.brightnessSlider = slider;
	[slider release];
    
	// swatches.	    
    NSMutableArray *colors = [NSMutableArray array];    
    for(float angle = 0; angle < 360; angle += 60)
    {
        CGFloat h = 0;
        h = (M_PI / 180.0 * angle) / (2 * M_PI);            
        [colors addObject:[UIColor colorWithHue:h  saturation:1.0 brightness:1.0 alpha:1.0]];                        
    }

    KZColorPickerSwatchView *swatch = nil;	
    self.swatches = [NSMutableArray array];
    for (UIColor *color in colors)
    {
        swatch = [[KZColorPickerSwatchView alloc] initWithFrame:CGRectZero];
        swatch.color = color;
        [swatch addTarget:self action:@selector(swatchAction:) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:swatch];
        [swatches addObject:swatch];
        [swatch release];
    }	
	
	self.selectedColor = [UIColor whiteColor];//[UIColor colorWithRed:0.349 green:0.613 blue:0.378 alpha:1.000];
    [self fixLocations];
}

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) 
	{
        // Initialization code
		[self setup];
    }
    return self;
}

- (void)dealloc 
{
	[selectedColor release];
	[colorWheel release];
	[brightnessSlider release];
    [swatches release];
    [super dealloc];
}

- (void) awakeFromNib
{
	[self setup];
}

RGBType rgbWithUIColor(UIColor *color)
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

- (void) setSelectedColor:(UIColor *)color animated:(BOOL)animated
{
	if (animated) 
	{
		[UIView beginAnimations:nil context:nil];
		self.selectedColor = color;
		[UIView commitAnimations];
	}
	else 
	{
		self.selectedColor = color;
	}
}

- (void) setSelectedColor:(UIColor *)c
{
	[c retain];
	[selectedColor release];
	selectedColor = c;
	
	RGBType rgb = rgbWithUIColor(c);
	HSVType hsv = RGB_to_HSV(rgb);
	
	self.colorWheel.currentHSV = hsv;
	self.brightnessSlider.value = hsv.v;
	
    UIColor *keyColor = [UIColor colorWithHue:hsv.h 
                                   saturation:hsv.s
                                   brightness:1.0
                                        alpha:1.0];
	[self.brightnessSlider setKeyColor:keyColor];
    
    keyColor = [UIColor colorWithHue:hsv.h 
                          saturation:hsv.s
                          brightness:hsv.v
                               alpha:1.0];
	
    [self sendActionsForControlEvents:UIControlEventValueChanged];
}

- (void) colorWheelColorChanged:(KZColorPickerHSWheel *)wheel
{
	HSVType hsv = wheel.currentHSV;
	self.selectedColor = [UIColor colorWithHue:hsv.h
									saturation:hsv.s
									brightness:self.brightnessSlider.value
										 alpha:1.0];
	
}

- (void) brightnessChanged:(KZColorPickerBrightnessSlider *)slider
{
	HSVType hsv = self.colorWheel.currentHSV;
	
	self.selectedColor = [UIColor colorWithHue:hsv.h
									saturation:hsv.s
									brightness:self.brightnessSlider.value
										 alpha:1.0];
	
}

- (void) swatchAction:(KZColorPickerSwatchView *)sender
{
	[self setSelectedColor:sender.color animated:YES];
	//[self sendActionsForControlEvents:UIControlEventValueChanged];
}

- (void) fixLocations
{
    //horizontal
    if(self.bounds.size.width < self.bounds.size.height)
    {
        CGFloat totalWidth = self.bounds.size.width - 40.0;
        CGFloat swatchCellWidth = totalWidth / 6.0;
        
		int sx = 20;
        int sy = 384;
		if (self.bounds.size.height > 500) {
			sy = 464;
		}
        for (KZColorPickerSwatchView *swatch in self.swatches)
        {
            swatch.frame = CGRectMake(sx + swatchCellWidth * 0.5 - 18.0,
                                      sy, 36.0, 36.0);
            sx += swatchCellWidth;
        }
        
		if (self.bounds.size.height > 500) {
			self.brightnessSlider.frame = CGRectMake(24, 381, 272, 38);
		} else {
        	self.brightnessSlider.frame = CGRectMake(24, 341, 272, 38);
		}
		self.colorWheel.frame = CGRectMake(40, 79, self.colorWheel.frame.size.width, self.colorWheel.frame.size.height);

    }
    else
    {
        CGFloat totalWidth = 160.0;
        CGFloat swatchCellWidth = totalWidth / 3.0;
        
        int sx = 302;
        int sy = 204;
		
		if (self.bounds.size.width > 500) {
			sx = 340;
		}
		
        int index = 0;
        for (KZColorPickerSwatchView *swatch in self.swatches)
        {
            swatch.frame = CGRectMake(sx + swatchCellWidth * 0.5 - 18.0,
                                      sy, 36.0, 36.0);
            sx += swatchCellWidth;
            if(++index % 3 == 0)
            {
                sx = 302;
				if (self.bounds.size.width > 500) {
					sx = 340;
				}
                sy += swatchCellWidth;
            }
        }
        if (self.bounds.size.width > 500) {
			self.brightnessSlider.frame = CGRectMake(340, self.bounds.size.height * 0.5 - 58, 165, 38);
		}
		else {
        	self.brightnessSlider.frame = CGRectMake(302, self.bounds.size.height * 0.5 - 58, 165, 38);
		}
		self.colorWheel.frame = CGRectMake(40, 70, self.colorWheel.frame.size.width, self.colorWheel.frame.size.height);
        
    }
}

- (void) layoutSubviews
{
    [UIView beginAnimations:nil context:nil];
    
    [self fixLocations];
    
    [UIView commitAnimations];
}

@end
