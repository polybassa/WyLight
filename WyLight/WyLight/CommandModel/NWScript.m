//
//  NWScript.m
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScript.h"
#import "NWComplexScriptCommandObject.h"
#import "NWSetFadeScriptCommandObject.h" //remove when finished
#import "NWSetGradientScriptCommandObject.h" //remove when finished

@interface NWScript()

@property (nonatomic, readwrite, strong) NSMutableArray *scriptArray;

@end

@implementation NWScript

- (id)init {
	self = [super init];
	
	if (self) {
		[self fillWithTestData];
	}
	return self;
}

#define SCRIPTARRAY_KEY @"WyLightRemote.NWScript.scriptarray"

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [self init];
	if (self) {
		_scriptArray = [aDecoder decodeObjectForKey:SCRIPTARRAY_KEY];
	}
	return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
	[aCoder encodeObject:_scriptArray forKey:SCRIPTARRAY_KEY];
}

- (void)fillWithTestData {
	//TESTCODE
	NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
	//self.command.backgroundColor = [UIColor blackColor];
	
	{
		NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
		obj.address = 0xffffffff;
		obj.color = [UIColor redColor];
		
		[comObj.scriptObjects addObject:obj];
	}
	{
		NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
		obj.address = 0x000000ff;
		obj.color = [UIColor yellowColor];
		
		[comObj.scriptObjects addObject:obj];
	}
	{
		NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
		obj.color1 = [UIColor blueColor];
		obj.color2 = [UIColor greenColor];
		
		obj.offset = 10;
		obj.numberOfLeds = 10;
		[comObj.scriptObjects addObject: obj];
	}
	{
		NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
		obj.color1 = [UIColor blueColor];
		obj.color2 = [UIColor greenColor];
		
		obj.offset = 20;
		obj.numberOfLeds = 5;
		[comObj.scriptObjects addObject: obj];
	}
	comObj.duration = 200;
	[self addObject:comObj];
	//TESTCODE
	comObj = [[NWComplexScriptCommandObject alloc] init];
	//self.command.backgroundColor = [UIColor blackColor];
	
	{
		NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
		obj.address = 0xffffffff;
		obj.color = [UIColor greenColor];
		
		[comObj.scriptObjects addObject:obj];
	}
	comObj.duration = 50;
	[self addObject:comObj];
	//TESTCODE
	comObj = [[NWComplexScriptCommandObject alloc] init];
	//self.command.backgroundColor = [UIColor blackColor];
	{
		NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
		obj.color1 = [UIColor blueColor];
		obj.color2 = [UIColor greenColor];
		
		obj.offset = 10;
		obj.numberOfLeds = 10;
		[comObj.scriptObjects addObject: obj];
	}
	{
		NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
		obj.color1 = [UIColor blueColor];
		obj.color2 = [UIColor greenColor];
		
		obj.offset = 0;
		obj.numberOfLeds = 32;
		[comObj.scriptObjects addObject: obj];
	}
	comObj.duration = 100;
	[self addObject:comObj];
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
}

@end
