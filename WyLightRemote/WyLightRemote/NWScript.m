//
//  NWScript.m
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScript.h"
#import "NWScriptObject.h"

@interface NWScript()

@property (nonatomic, strong) NSMutableArray *script;

@end

@implementation NWScript

- (NSArray *)completeScript
{
	return self.script;
}

- (NSDictionary *)allVisibleScriptBlocks
{
	NSMutableDictionary* mutDict = [[NSMutableDictionary alloc]init];
	NSNumber *key = @(0);
	NSMutableArray *valueArray = [[NSMutableArray alloc]init];
	for (unsigned int i = 0; i < self.script.count;i++) {
		
		[valueArray addObject:[self.completeScript objectAtIndex:i]];
		if (![[self.completeScript objectAtIndex:i] parallel]) {
			[mutDict addEntriesFromDictionary:@{key: valueArray}];
			key = @([key intValue] + 1);
			valueArray = [[NSMutableArray alloc]init];
		}
	}
	return mutDict;
}

- (NSMutableArray *)script
{
	if (!_script)
	{
		_script = [[NSMutableArray alloc]init];
	}
	return _script;
}

- (void)linkScriptObject:(NWScriptObject *)scriptObject
{
	NSUInteger index = [self.script indexOfObject:scriptObject];
	
	if (index) {
		NWScriptObject *prevScriptObject = [self.script objectAtIndex:index - 1];
		scriptObject.prevScriptObject = prevScriptObject;
		prevScriptObject.nextScriptObject = scriptObject;
	}
	if ((index + 1) < [self.script count]) {
		NWScriptObject *nextScriptObject = [self.script objectAtIndex:index + 1];
		nextScriptObject.prevScriptObject = scriptObject;
		scriptObject.nextScriptObject = nextScriptObject;
	}
}

- (void)unlinkScriptObject:(NWScriptObject *)scriptObject
{
	NSUInteger index = [self.script indexOfObject:scriptObject];
	NWScriptObject *prevScriptObject = nil;
	NWScriptObject *nextScriptObject = nil;
	
	if (index) {
		prevScriptObject = [self.script objectAtIndex:index - 1];
	}
	if ((index + 1) < [self.script count]) {
		nextScriptObject = [self.script objectAtIndex:index + 1];
	}
	
	if (prevScriptObject) {
		prevScriptObject.nextScriptObject = nextScriptObject;
	}
	if (nextScriptObject) {
		nextScriptObject.prevScriptObject = prevScriptObject;
	}
}

- (void) addObject:(id)anObject
{
	if ([anObject isKindOfClass:[NWScriptObject class]]) {
		[self.script addObject:anObject];
		NWScriptObject *scriptObject = (NWScriptObject *)anObject;
		[self linkScriptObject:scriptObject];
	}
}

- (void)insertObject:(id)anObject atIndex:(NSUInteger)index
{
	if ([anObject isKindOfClass:[NWScriptObject class]]) {
		[self.script addObject:anObject];
		NWScriptObject *scriptObject = (NWScriptObject *)anObject;
		[self linkScriptObject:scriptObject];
	}
}
- (void)removeObject:(id)anObject
{
	if ([anObject isKindOfClass:[NWScriptObject class]]) {
		NWScriptObject *scriptObject = (NWScriptObject *)anObject;
		[self unlinkScriptObject:scriptObject];
		[self.script removeObject:anObject];
	}
}

- (void)removeObjectAtIndex:(NSUInteger)index
{
	if (index < [self.script count]) {
		NWScriptObject *scriptObject = [self.script objectAtIndex:index];
		[self unlinkScriptObject:scriptObject];
		[self.script removeObjectAtIndex:index];
	}
}

- (NSNumber *)totalDurationInTmms
{
	NSUInteger totalDuration = 0;
	for (NWScriptObject *obj in self.script) {
		if (!obj.parallel) {
			totalDuration = totalDuration + [obj.duration unsignedIntegerValue];
		}
	}
	return @(totalDuration);
}

@end
