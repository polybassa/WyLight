//
//  NWScriptObject.m
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObject.h"

@interface NWScriptObject ()

@property (nonatomic, readwrite, strong) NSArray *endColors;
@property (nonatomic, readwrite, strong) NSArray *startColors;

@end

@implementation NWScriptObject

@synthesize startColors = _startColors;


+ (struct led_cmd)randomCommand
{
	struct led_cmd mCMD;
	
	switch (arc4random() % 3) {
		case 0:
		{
			mCMD.cmd = SET_FADE;
			mCMD.data.set_fade.addr[0] = arc4random() % 255;
			mCMD.data.set_fade.addr[1] = arc4random() % 255;
			mCMD.data.set_fade.addr[2] = arc4random() % 255;
			mCMD.data.set_fade.addr[3] = arc4random() % 255;
			
			mCMD.data.set_fade.fadeTmms = arc4random() % 200 + 20;
			mCMD.data.set_fade.red = arc4random() % 255;
			mCMD.data.set_fade.green = arc4random() % 255;
			mCMD.data.set_fade.blue = arc4random() % 255;
			mCMD.data.set_fade.parallelFade =  arc4random() % 1;
			
		} break;
			
		case 1:
		{
			mCMD.cmd = SET_GRADIENT;
			mCMD.data.set_gradient.red_1 = arc4random() % 255;
			mCMD.data.set_gradient.green_1 = arc4random() % 255;
			mCMD.data.set_gradient.blue_1 = arc4random() % 255;
			
			mCMD.data.set_gradient.red_2 = arc4random() % 255;
			mCMD.data.set_gradient.green_2 = arc4random() % 255;
			mCMD.data.set_gradient.blue_2 = arc4random() % 255;
			
			mCMD.data.set_gradient.parallelAndOffset = 0x0;
			mCMD.data.set_gradient.numberOfLeds = NUM_OF_LED;
			mCMD.data.set_gradient.fadeTmms = arc4random() % 200 + 20;
		} break;
			
		case 2:
		{
			mCMD.cmd = WAIT;
			mCMD.data.wait.waitTmms = arc4random() % 200 + 20;
		}
		default:
			break;
	}
	return mCMD;
}

#pragma mark - SETTER

- (void)setCommand:(struct led_cmd)command
{
	_command = command;
	switch (command.cmd) {
		case SET_FADE:
		{
			NSMutableArray *tempEndColors = [[NSMutableArray alloc]init];
			uint32_t bitMask;
			memcpy(&bitMask, command.data.set_fade.addr, 4);
			uint32_t compareMask = 0x00000001;
			
			for (int i = 0; i < NUM_OF_LED; i++) {
				if (compareMask & bitMask) {
					[tempEndColors addObject:[UIColor colorWithRed:(float)command.data.set_fade.red / 255 green:(float)command.data.set_fade.green / 255 blue:(float)command.data.set_fade.blue / 255 alpha:1.0]];
				} else {
					[tempEndColors addObject:[UIColor colorWithRed:0 green:0 blue:0 alpha:0]];
				}
				compareMask = compareMask << 1;
			}
			
			self.endColors = tempEndColors;
			self.parallel = command.data.set_fade.parallelFade;
			self.duration = @(command.data.set_fade.fadeTmms);
		} break;
		case SET_GRADIENT:
		{
			NSMutableArray *tempEndColors = [[NSMutableArray alloc]init];
			float deltaRed, deltaGreen, deltaBlue;
			deltaRed = (float)command.data.set_gradient.red_2 - (float)command.data.set_gradient.red_1;
			deltaGreen = (float)command.data.set_gradient.green_2 - (float)command.data.set_gradient.green_1;
			deltaBlue =	(float)command.data.set_gradient.blue_2 - (float)command.data.set_gradient.blue_1;
			
			float startRed, startGreen, startBlue;
			startRed = (float)command.data.set_gradient.red_1;
			startGreen = (float)command.data.set_gradient.green_1;
			startBlue = (float)command.data.set_gradient.blue_1;
			
			uint8_t offset = command.data.set_gradient.parallelAndOffset & 0x7f;
			uint8_t numOfLeds = command.data.set_gradient.numberOfLeds - 1;
			offset *= 3;
			numOfLeds *= 3;
			
			const uint8_t endPosition = offset + numOfLeds;
			
			for (unsigned int i = 0; i < NUM_OF_LED; i++) {
				if (i > endPosition) {
					[tempEndColors addObject:[UIColor colorWithRed:0 green:0 blue:0 alpha:1]];
				} else if (i == endPosition ) {
					[tempEndColors addObject:[UIColor colorWithRed:(float)command.data.set_gradient.red_2 / 255
															 green:(float)command.data.set_gradient.green_2 / 255
															  blue:(float)command.data.set_gradient.blue_2 / 255
															 alpha:1.0]];
					
				} else if (i >= offset) {
					[tempEndColors addObject:[UIColor colorWithRed:startRed / 255 green:startGreen / 255 blue:startBlue / 255 alpha:1.0]];
					startRed += deltaRed / (numOfLeds / 3);
					startGreen += deltaGreen / (numOfLeds / 3);
					startBlue += deltaBlue / (numOfLeds / 3);
				} else {
					[tempEndColors addObject:[UIColor colorWithRed:0 green:0 blue:0 alpha:0]];
				}
			}
			
			self.endColors = tempEndColors;
			self.parallel = command.data.set_gradient.parallelAndOffset & 0x80;
			self.duration = @(command.data.set_gradient.fadeTmms);
		} break;
		case WAIT:
		{
			self.endColors = self.startColors;
			self.parallel = NO;
			self.duration = @(command.data.wait.waitTmms);
		} break;
		default:
			break;
	}
}

- (void)setNextScriptObject:(NWScriptObject *)nextScriptObject
{
	_nextScriptObject = nextScriptObject;
}

- (void)setPrevScriptObject:(NWScriptObject *)prevScriptObject
{
	_prevScriptObject = prevScriptObject;
	if (prevScriptObject) {
		self.startColors = prevScriptObject.endColors;
	}
}

- (void)setStartColors:(NSArray *)startColors
{
	_startColors = startColors;
	if (self.command.cmd == WAIT) {
		self.endColors = startColors;
	}
}

+ (NSArray *)mergedColorsFromArray:(NSArray *)array
{
	NSUInteger index = [array count] - 1;
	NWScriptObject *currentObj = [array objectAtIndex:index];
	NSMutableArray *colorArray = [[currentObj endColors] mutableCopy];
	while (index) {
		NWScriptObject *prevObj = [currentObj prevScriptObject];
		for (unsigned int i = 0; i < [colorArray count]; i++) {
			UIColor *color = [colorArray objectAtIndex:i];
			CGFloat alpha = 1;
			[color getRed:NULL green:NULL blue:NULL alpha:&alpha];
			if (alpha == 0) {
				[colorArray removeObjectAtIndex:i];
				[colorArray insertObject:[prevObj endColors][i] atIndex:i];
			}
		}
		currentObj = prevObj;
		index--;
	}
	return colorArray;
}

#pragma mark - GETTER

- (NSArray *)startColors
{
	if (!_startColors) {
		NSMutableArray *tempStartColors = [[NSMutableArray alloc] init];
		for (int i = 0; i < NUM_OF_LED; i++) {
			[tempStartColors addObject: [UIColor colorWithRed:0 green:0 blue:0 alpha:0]];
		}
		_startColors = tempStartColors;
	}
	return _startColors;
}

- (NSArray *)endColors
{
	if (!_endColors) {
		NSMutableArray *tempStartColors = [[NSMutableArray alloc] init];
		for (int i = 0; i < NUM_OF_LED; i++) {
			[tempStartColors addObject: [UIColor colorWithRed:0 green:0 blue:0 alpha:0]];
		}
		_endColors = tempStartColors;
	}
	return _endColors;
}


@end
