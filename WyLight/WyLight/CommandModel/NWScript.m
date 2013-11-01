//
//  NWScript.m
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScript.h"
#import "NWComplexScriptCommandObject.h"
#import "WCWiflyControlWrapper.h"

@interface NWScript()

@property (nonatomic, readwrite, strong) NSMutableArray *scriptArray;

@end

@implementation NWScript

#define SCRIPTARRAY_KEY @"WyLightRemote.NWScript.scriptarray"
#define TITLE_KEY @"WyLightRemote.NWScript.title"
#define REPEAT_KEY @"WyLightRemote.NWScript.repeat"

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [self init];
	if (self) {
		_scriptArray = [aDecoder decodeObjectForKey:SCRIPTARRAY_KEY];
        _title = [aDecoder decodeObjectForKey:TITLE_KEY];
        _repeatWhenFinished = [aDecoder decodeBoolForKey:REPEAT_KEY];
	}
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
	[aCoder encodeObject:_scriptArray forKey:SCRIPTARRAY_KEY];
    [aCoder encodeObject:_title forKey:TITLE_KEY];
    [aCoder encodeBool:_repeatWhenFinished forKey:REPEAT_KEY];
}

- (NSMutableArray *)scriptArray {
	if (!_scriptArray)
	{
		_scriptArray = [[NSMutableArray alloc]init];
	}
	return _scriptArray;
}

- (NSNumber *)totalDurationInTmms {
	NSUInteger totalDuration = 0;
	for (NWComplexScriptCommandObject *obj in self.scriptArray) {
		totalDuration = totalDuration + obj.duration;
	}
	return @(totalDuration);
}

- (void)addObject:(NWComplexScriptCommandObject *)anObject {
	NWComplexScriptCommandObject *lastObj = self.scriptArray.lastObject;
	[self.scriptArray addObject:anObject];
	if (lastObj) {
		lastObj.next = anObject;
		anObject.prev = lastObj;
		anObject.next = nil;
	}
    self.needsUpdate = YES;
}

- (void)setRepeatWhenFinished:(BOOL)repeatWhenFinished {
    _repeatWhenFinished = repeatWhenFinished;
    self.needsUpdate = YES;
}

- (void)removeObjectAtIndex:(NSUInteger)index {
	NWComplexScriptCommandObject *tempObj = [self.scriptArray objectAtIndex:index];
	if (tempObj.prev) {
		tempObj.prev.next = tempObj.next;
	}
	if (tempObj.next) {
		tempObj.next.prev = tempObj.prev;
	}
	[self.scriptArray removeObjectAtIndex:index];
    self.needsUpdate = YES;
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
    [control clearScript];
    [control loopOn];
    for (NWComplexScriptCommandObject* command in self.scriptArray) {
        [command sendToWCWiflyControl:control];
        [NSThread sleepForTimeInterval:0.1];
    }
    if (self.repeatWhenFinished) {
        [control loopOffAfterNumberOfRepeats:0];
    } else {
        [control loopOffAfterNumberOfRepeats:1];
    }

    
}

@end
