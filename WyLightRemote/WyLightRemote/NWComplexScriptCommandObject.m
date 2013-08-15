//
//  NWComplexScriptCommandObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWComplexScriptCommandObject.h"
#import "WCWiflyControlWrapper.h"
#import "NWScriptEffectCommandObject.h"

@implementation NWComplexScriptCommandObject

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control
{
	if ([self isWaitCommand]) {
		[control setWaitTimeInTenMilliSecondsIntervals:self.duration];
	} else {
		[self prepareForSendToWCWiflyControl];
		for (NWCommandObject *command in self.itsScriptObjects) {
			[command sendToWCWiflyControl:control];
		}
	}
}

- (NSMutableArray *)itsScriptObjects
{
	if (!_itsScriptObjects) {
		_itsScriptObjects = [[NSMutableArray alloc]init];
	}
	return _itsScriptObjects;
}

- (void)prepareForSendToWCWiflyControl
{
	for (NWScriptEffectCommandObject *command in self.itsScriptObjects) {
		command.parallel = YES;
		command.duration = self.duration;
	}
	
	[self.itsScriptObjects.lastObject setParallel:NO];
}

- (NSArray *)colors
{
	NSMutableArray *outPutColors = [[NSMutableArray alloc]init];
	uint32_t compareMask = 0x00000001;
	for (unsigned int i = 0; i < [[[self.itsScriptObjects lastObject] colors]count]; i++) {
		NSUInteger j = self.itsScriptObjects.count;
		while (j--) {
			NWScriptEffectCommandObject *currentObj = [self.itsScriptObjects objectAtIndex:j];
			if ([currentObj respondsToSelector:@selector(address)]) {
				const uint32_t bitmask = [currentObj performSelector:@selector(address)];
				if (bitmask & compareMask) {
					[outPutColors addObject:currentObj.colors[i]];
					break;
				}
			}
		}
		if (i > outPutColors.count) {
			[outPutColors addObject:self.backgroundColor];
		}
		compareMask = compareMask << 1;
	}
	return outPutColors;
}


@end
