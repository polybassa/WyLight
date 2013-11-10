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
#import "NWSendableCommand.h"
#import "wifly_cmd.h"

@implementation NWComplexScriptCommandObject

- (id)copyWithZone:(NSZone *)zone {
	NWComplexScriptCommandObject *other = [[NWComplexScriptCommandObject alloc]init];
	other.next = self.next;
	other.prev = self.prev;
	other.waitCommand = self.waitCommand;
	other.scriptObjects = [[NSMutableArray alloc]initWithArray:self.scriptObjects copyItems:YES];
	other.duration = self.duration;
	
	return other;
}

- (void)setWaitCommand:(BOOL)waitCommand {
    if (waitCommand != _waitCommand) {
        self.needsUpdate = YES;
        _waitCommand = waitCommand;
    }
}

#define NEXT_KEY @"WyLightRemote.NWComplexScriptCommandObject.next"
#define PREV_KEY @"WyLightRemote.NWComplexScriptCommandObject.prev"
#define WAIT_KEY @"WyLightRemote.NWComplexScriptCommandObject.wait"
#define SCRIPTOBJECTS_KEY @"WyLightRemote.NWComplexScriptCommandObject.scriptObjects"

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	if (self) {
		_next = [aDecoder decodeObjectForKey:NEXT_KEY];
		_prev = [aDecoder decodeObjectForKey:PREV_KEY];
		_waitCommand = [aDecoder decodeBoolForKey:WAIT_KEY];
		_scriptObjects = [aDecoder decodeObjectForKey:SCRIPTOBJECTS_KEY];
	}
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
	[super encodeWithCoder:aCoder];
	[aCoder encodeObject:_scriptObjects forKey:SCRIPTOBJECTS_KEY];
	[aCoder encodeObject:_next forKey:NEXT_KEY];
	[aCoder encodeObject:_prev forKey:PREV_KEY];
	[aCoder encodeBool:_waitCommand forKey:WAIT_KEY];
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
	if ([self isWaitCommand]) {
		[control setWaitTimeInTenMilliSecondsIntervals:self.duration];
	} else {
		[self prepareForSendToWCWiflyControl];
		for (id<NWSendableCommand> command in self.scriptObjects) {
			[command sendToWCWiflyControl:control];
		}
	}
}

- (NSMutableArray *)scriptObjects {
	if (!_scriptObjects) {
		_scriptObjects = [[NSMutableArray alloc]init];
	}
	return _scriptObjects;
}

- (void)prepareForSendToWCWiflyControl {
	for (NWScriptEffectCommandObject *command in self.scriptObjects) {
		command.parallel = YES;
		command.duration = self.duration;
	}
	[[self.scriptObjects lastObject] setParallel:NO];
}

- (void)setBackgroundColor:(UIColor *)backgroundColor {
	for (id<NWDrawableCommand> cmd in self.scriptObjects) {
		cmd.backgroundColor = backgroundColor;
	}
}

- (NSArray *)colors
{
	if (self.isWaitCommand) {
		if (self.prev.colors) {
			return self.prev.colors;
		}
		NSMutableArray *outPutColors = [[NSMutableArray alloc] init];
		for (NSUInteger i = 0; i < NUM_OF_LED; i++) {
			[outPutColors addObject:self.backgroundColor];
		}
		return outPutColors;
	}
	NSMutableArray *outPutColors;
	if (self.prev) {
		outPutColors = [[NSMutableArray alloc]initWithArray:self.prev.colors copyItems:YES];
	} else {
		outPutColors = [[NSMutableArray alloc]init];
	}
	uint32_t compareMask = 0x00000001;
	for (unsigned int i = 0; i < NUM_OF_LED; i++) {  //i = 0 - 31
		NSUInteger j = self.scriptObjects.count;
		while (j--) {
			NWScriptEffectCommandObject *currentObj = [self.scriptObjects objectAtIndex:j];
			if ([currentObj respondsToSelector:@selector(address)]) {
				const uint32_t bitmask = (uint32_t)[currentObj performSelector:@selector(address)];
				if (bitmask & compareMask) {
					if (self.prev) {
						[outPutColors replaceObjectAtIndex:i withObject:currentObj.colors[i]];
					} else {
						[outPutColors addObject:currentObj.colors[i]];
					}
					break;
				}
			}
		}
		if (i >= outPutColors.count) {
			[outPutColors addObject:self.backgroundColor];
		}
		compareMask = compareMask << 1;
	}
	return [outPutColors copy];
}


@end