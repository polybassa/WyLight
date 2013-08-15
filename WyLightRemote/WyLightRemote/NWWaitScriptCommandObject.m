//
//  NWWaitScriptCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWWaitScriptCommandObject.h"
#import "WCWiflyControlWrapper.h"

@implementation NWWaitScriptCommandObject

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control
{
	[control setWaitTimeInTenMilliSecondsIntervals:self.duration];
}

- (BOOL)parallel
{
	return NO;
}

@end
