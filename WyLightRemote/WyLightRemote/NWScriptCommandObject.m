//
//  NWScriptCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptCommandObject.h"

@implementation NWScriptCommandObject

- (NSArray*)colors
{
	return nil;
}

- (UIColor *)backgroundColor
{
	if (_backgroundColor == nil)
	{
		_backgroundColor = [[UIColor blackColor] copy];
	}
	return _backgroundColor;
}

@end
