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

- (WCWiflyControlWrapper *)controlHandle
{
	if (!_controlHandle && self.delegate) {
		[self.delegate performUnwindSegue];
		return nil;
	}
	return _controlHandle;
}


@end
