//
//  NWScript+defaultScripts.h
//  WyLight
//
//  Created by Nils Weiß on 10/22/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScript.h"

@interface NWScript (defaultScripts)

+ (NWScript *)defaultScriptFastColorChange;

+ (NWScript *)defaultScriptSlowColorChange;

+ (NWScript *)defaultScriptConzentrationLight;

+ (NWScript *)defaultScriptMovingColors;

+ (NWScript *)defaultScriptRandomColors;

+ (NWScript *)defaultScriptRunLightWithColor:(UIColor *)color timeInterval:(uint16_t)time;
@end
