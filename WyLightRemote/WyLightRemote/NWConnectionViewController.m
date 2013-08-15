//
//  NWConnectionViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 06.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWConnectionViewController.h"
#import "WCEndpoint.h"
#import "WCWiflyControlWrapper.h"

@implementation NWConnectionViewController

@synthesize controlHandle = _controlHandle;

- (void) setControlHandle:(WCWiflyControlWrapper *)controlHandle
{
	_controlHandle = controlHandle;
}

- (WCWiflyControlWrapper *)controlHandle
{
	if (!_controlHandle) {
		[self.delegate performUnwindSegue];
		return nil;
	}
	return _controlHandle;
}


@end
