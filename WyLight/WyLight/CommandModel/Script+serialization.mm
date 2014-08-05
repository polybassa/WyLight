//
//  Script+serialization.m
//  WyLight
//
//  Created by Nils Weiß on 29/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Script+serialization.h"
#include "../../../library/Script.h"
#include "FwCommand.h"
#include "ComplexEffect.h"
#include "Fade.h"
#include "Gradient.h"
#import "UIColor+argbValue.h"
#import "Script+defaultScripts.h"
#include "wifly_cmd.h"
#include <sstream>

@implementation Script (serialization)

- (NSString *) serializeToPath:(NSString *)path {
	
	NSString *tempTitel = self.title;
	
	tempTitel = [[tempTitel componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] componentsJoinedByString:@"" ];
    tempTitel = [[tempTitel componentsSeparatedByCharactersInSet:[NSCharacterSet illegalCharacterSet]] componentsJoinedByString:@"" ];
    tempTitel = [[tempTitel componentsSeparatedByCharactersInSet:[NSCharacterSet symbolCharacterSet]] componentsJoinedByString:@"" ];
	tempTitel = [tempTitel stringByReplacingOccurrencesOfString:@"ä" withString:@"ae"];
	tempTitel = [tempTitel stringByReplacingOccurrencesOfString:@"ü" withString:@"ue"];
	tempTitel = [tempTitel stringByReplacingOccurrencesOfString:@"ö" withString:@"oe"];
	tempTitel = [tempTitel stringByReplacingOccurrencesOfString:@"Ä" withString:@"Ae"];
	tempTitel = [tempTitel stringByReplacingOccurrencesOfString:@"Ü" withString:@"Ue"];
	tempTitel = [tempTitel stringByReplacingOccurrencesOfString:@"Ö" withString:@"Oe"];

    	
	NSMutableString *filePath = [[NSMutableString alloc] initWithString:path];
	[filePath appendString:[NSString stringWithFormat:@"/%@.wyscript", tempTitel]];
	
	WyLight::Script mScript;
	const char* cStringScriptTitle = [self.title cStringUsingEncoding:NSASCIIStringEncoding];
	
	std::string ScriptTitle = " ";
	
	if (cStringScriptTitle == NULL) {
		ScriptTitle = std::string([tempTitel cStringUsingEncoding:NSASCIIStringEncoding]);
	} else {
		ScriptTitle = std::string(cStringScriptTitle);
	}
	mScript.setName(ScriptTitle);
	mScript.push_back(std::unique_ptr<WyLight::FwCmdScript>(new WyLight::FwCmdLoopOn));
	
	for (ComplexEffect *cmplx in self.effects) {
		[cmplx prepareForSendToWCWiflyControl];
		if (cmplx.waitCommand.boolValue) {
			mScript.push_back(std::unique_ptr<WyLight::FwCmdScript>(new WyLight::FwCmdWait(cmplx.duration.unsignedIntValue)));
		} else {
			for (Effect *effect in cmplx.effects) {
				if ([effect isKindOfClass:[Fade class]]) {
					Fade* castEffect = (Fade *)effect;
					mScript.push_back(std::unique_ptr<WyLight::FwCmdScript>(new WyLight::FwCmdSetFade([castEffect.color getARGB],
											  									castEffect.duration.unsignedIntValue,
											 	 								castEffect.address.unsignedIntValue,
																				castEffect.parallel.boolValue)));
				} else if ([effect isKindOfClass:[Gradient class]]) {
					Gradient* castEffect = (Gradient *)effect;
					mScript.push_back(std::unique_ptr<WyLight::FwCmdScript>(new WyLight::FwCmdSetGradient([castEffect.color1 getARGB],
																	[castEffect.color2 getARGB],
																	castEffect.duration.unsignedIntValue,
																	castEffect.parallel.boolValue,
																	castEffect.numberOfLeds.unsignedIntValue,
																	castEffect.offset.unsignedIntValue)));
				}
			}
		}
	}
	
	if (self.repeatsWhenFinished.boolValue) {
		mScript.push_back(std::unique_ptr<WyLight::FwCmdScript>(new WyLight::FwCmdLoopOff(0)));
	} else {
		mScript.push_back(std::unique_ptr<WyLight::FwCmdScript>(new WyLight::FwCmdLoopOff(1)));
	}
	
	const char* cStringFilePath = [filePath cStringUsingEncoding:NSASCIIStringEncoding];
	
	mScript.serialize(std::string(cStringFilePath), mScript);
	return filePath;
}

+ (Script *)deserializeScriptFromPath:(NSURL *)path inContext:(NSManagedObjectContext *)context {
	NSString *str = [NSString stringWithContentsOfURL:path encoding:NSUTF8StringEncoding error:nil];
	
	Script *tempScript = [Script deserializeScriptFromString:str inContext:context];
	return tempScript;
}

+ (Script *)deserializeScriptFromString:(NSString *)string inContext:(NSManagedObjectContext *)context {
	
	WyLight::Script mScript;
	const char* cStringDataString = [string cStringUsingEncoding:NSASCIIStringEncoding];
	if (cStringDataString == NULL) {
		return [Script emptyScriptInContext:context];
	}
	std::stringstream mStream(cStringDataString);
	mScript.deserialize(mStream, mScript);
	
	Script *tempScript = [Script insertNewObjectIntoContext:context];
	tempScript.title = [NSString stringWithCString:mScript.getName().c_str() encoding:NSASCIIStringEncoding];
	
	for (const auto& cmd : mScript) {
		const led_cmd * const data = (led_cmd *)cmd->GetData();
		if (data->cmd == LOOP_OFF) {
			tempScript.repeatsWhenFinished = data->data.loopEnd.numLoops == 0 ? @(YES) : @(NO);
			break;
		}
	}
		
	{
		ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
		comObj.script = tempScript;
		for (const auto& cmd : mScript) {
			const led_cmd * const data = (led_cmd *)cmd->GetData();
			if (data->cmd == SET_FADE) {
				
				Fade *obj = [Fade insertNewObjectIntoContext:context];
				obj.address = @(((uint32_t)data->data.set_fade.addr[0]) |
								((uint32_t)data->data.set_fade.addr[1] << 8) |
								((uint32_t)data->data.set_fade.addr[2] << 16) |
								((uint32_t)data->data.set_fade.addr[3] << 24));
				obj.color = [UIColor colorWithRed:((CGFloat)data->data.set_fade.red) / 255
											green:((CGFloat)data->data.set_fade.green) / 255
											 blue:((CGFloat)data->data.set_fade.blue) / 255
											alpha:1.0];
				obj.duration = @(data->data.set_fade.fadeTmms);
				obj.complexEffect = comObj;
				if (data->data.set_fade.parallelFade == 0) {
					comObj.duration = @(ntohs(data->data.set_fade.fadeTmms));
					comObj = [ComplexEffect insertNewObjectIntoContext:context];
					comObj.script = tempScript;
				}
			} else if (data->cmd == SET_GRADIENT) {
								
				Gradient *obj = [Gradient insertNewObjectIntoContext:context];
				obj.numberOfLeds = @(data->data.set_gradient.numberOfLeds);
				obj.offset = @(data->data.set_gradient.parallelAndOffset & 0x7f);
				obj.color1 = [UIColor colorWithRed:((CGFloat)data->data.set_gradient.red_1) / 255
											green:((CGFloat)data->data.set_gradient.green_1) / 255
											 blue:((CGFloat)data->data.set_gradient.blue_1) / 255
											alpha:1.0];
				obj.color2 = [UIColor colorWithRed:((CGFloat)data->data.set_gradient.red_2) / 255
											 green:((CGFloat)data->data.set_gradient.green_2) / 255
											  blue:((CGFloat)data->data.set_gradient.blue_2) / 255
											 alpha:1.0];
				obj.duration = @(data->data.set_gradient.fadeTmms);
				obj.complexEffect = comObj;
				if ((data->data.set_gradient.parallelAndOffset & 0x80) == 0) {
					comObj.duration = @(ntohs(data->data.set_gradient.fadeTmms));
					comObj = [ComplexEffect insertNewObjectIntoContext:context];
					comObj.script = tempScript;
				}
			} else if (data->cmd == WAIT) {
								
				comObj.duration = @(ntohs(data->data.wait.waitTmms));
				comObj = [ComplexEffect insertNewObjectIntoContext:context];
				comObj.script = tempScript;
				
			}
		}
	}
	return tempScript;
}


@end
