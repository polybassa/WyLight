//
//  NWScriptCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptCommandObject.h"

@implementation NWScriptCommandObject

@synthesize backgroundColor = _backgroundColor;

- (NSArray*)colors {
	return nil;
}

- (UIColor *)backgroundColor {
	if (_backgroundColor == nil)
	{
		_backgroundColor = [UIColor blackColor];
	}
	return _backgroundColor;
} 

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
	[[[NSException alloc] initWithName:@"Function not implemented" reason:@"Implement function sendToWCWiflyControl:" userInfo:nil] raise];
}

@end
