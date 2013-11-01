//
//  NWRenderableScript+defaultScripts.h
//  WyLight
//
//  Created by Nils Weiß on 10/22/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWRenderableScript.h"

@interface NWRenderableScript (defaultScripts)

+ (NWRenderableScript *)defaultScriptFastColorChange;

+ (NWRenderableScript *)defaultScriptSlowColorChange;

+ (NWRenderableScript *)defaultScriptConzentrationLight;

+ (NWRenderableScript *)defaultScriptMovingColors;

+ (NWRenderableScript *)defaultScriptRandomColors;

+ (NWRenderableScript*)defaultScriptRunLightWithColor:(UIColor *)color timeInterval:(uint16_t)time;

+ (NWRenderableScript *)defaultScriptColorCrashWithTimeInterval:(uint16_t)time;

+ (NWRenderableScript *)emptyScript;
@end
