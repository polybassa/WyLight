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

@interface NWComplexScriptCommandObject	()

@property (nonatomic, strong) NSArray *itsColors;
@property (nonatomic, strong) NSArray *shadowCopyOfScriptObjects;

@end

@implementation NWComplexScriptCommandObject

- (id)copyWithZone:(NSZone *)zone {
	NWComplexScriptCommandObject *other = [[NWComplexScriptCommandObject alloc]init];
	other.next = self.next;
	other.prev = self.prev;
	other.waitCommand = self.waitCommand;
	other.itsScriptObjects = [[NSMutableArray alloc]initWithArray:self.itsScriptObjects copyItems:YES];
	other.duration = self.duration;
	
	return other;
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
		_itsScriptObjects = [aDecoder decodeObjectForKey:SCRIPTOBJECTS_KEY];
	}
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
	[super encodeWithCoder:aCoder];
	[aCoder encodeObject:_itsScriptObjects forKey:SCRIPTOBJECTS_KEY];
	[aCoder encodeObject:_next forKey:NEXT_KEY];
	[aCoder encodeObject:_prev forKey:PREV_KEY];
	[aCoder encodeBool:_waitCommand forKey:WAIT_KEY];
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control
{
	if ([self isWaitCommand]) {
		[control setWaitTimeInTenMilliSecondsIntervals:self.duration];
	} else {
		[self prepareForSendToWCWiflyControl];
		for (id<NWSendableCommand> command in self.itsScriptObjects) {
			[command sendToWCWiflyControl:control];
		}
	}
}

- (NSMutableArray *)itsScriptObjects {
	if (!_itsScriptObjects) {
		_itsScriptObjects = [[NSMutableArray alloc]init];
	}
	return _itsScriptObjects;
}

- (void)prepareForSendToWCWiflyControl {
	for (NWScriptEffectCommandObject *command in self.itsScriptObjects) {
		command.parallel = YES;
		command.duration = self.duration;
	}
	[[self.itsScriptObjects lastObject] setParallel:NO];
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
	
	if (![self.itsScriptObjects isEqualToArray:self.shadowCopyOfScriptObjects]) {
		self.itsColors = nil;
		self.shadowCopyOfScriptObjects = [self.itsScriptObjects copy];
	}
	
	if (!self.itsColors) {
		NSMutableArray *outPutColors = [[NSMutableArray alloc]init];
		uint32_t compareMask = 0x00000001;
		for (unsigned int i = 0; i < NUM_OF_LED; i++) {  //i = 0 - 31
			NSUInteger j = self.itsScriptObjects.count;
			while (j--) {
				NWScriptEffectCommandObject *currentObj = [self.itsScriptObjects objectAtIndex:j];
				if ([currentObj respondsToSelector:@selector(address)]) {
					const uint32_t bitmask = (uint32_t)[currentObj performSelector:@selector(address)];
					if (bitmask & compareMask) {
						[outPutColors addObject:currentObj.colors[i]];
						break;
					}
				}
			}
			if (i >= outPutColors.count) {
				[outPutColors addObject:self.backgroundColor];
			}
			compareMask = compareMask << 1;
		}
		self.itsColors = [outPutColors copy];
	}
	return self.itsColors;
}


@end