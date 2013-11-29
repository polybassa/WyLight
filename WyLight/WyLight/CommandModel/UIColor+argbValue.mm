//
//  UIColor+argbValue.m
//  WyLight
//
//  Created by Nils Weiß on 29/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "UIColor+argbValue.h"
#include "WiflyColor.h"

@implementation UIColor (argbValue)

- (uint32_t)getARGB {
	WyLight::WiflyColor color;
	
	CGFloat r, g, b, a;
	[self getRed:&r green:&g blue:&b alpha:&a];
	
	color.red((uint8_t)(r * 255));
	color.green((uint8_t)(g * 255));
	color.blue((uint8_t)(b * 255));
	
	return color.argb();
}

@end
