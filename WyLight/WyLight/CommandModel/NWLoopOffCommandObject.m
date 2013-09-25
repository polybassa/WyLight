//
//  NWLoopOffCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWLoopOffCommandObject.h"
#import "WCWiflyControlWrapper.h"

@implementation NWLoopOffCommandObject

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control
{
	[control loopOffAfterNumberOfRepeats:self.numberOfRepeats];
}

@end
