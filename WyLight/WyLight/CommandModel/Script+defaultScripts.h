//
//  Script+defaultScripts.h
//  WyLight
//
//  Created by Nils Weiß on 08/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Script.h"

@interface Script (defaultScripts)

+ (Script *)defaultScriptFastColorChangeInContext:(NSManagedObjectContext *)context;

+ (Script *)defaultScriptSlowColorChangeInContext:(NSManagedObjectContext *)context;

+ (Script *)defaultScriptConzentrationLightInContext:(NSManagedObjectContext *)context;

+ (Script *)defaultScriptMovingColorsInContext:(NSManagedObjectContext *)context;

+ (Script *)defaultScriptRandomColorsInContext:(NSManagedObjectContext *)context;

+ (Script*)defaultScriptRunLightWithColor:(UIColor *)color timeInterval:(uint16_t)time inContext:(NSManagedObjectContext *)context;

+ (Script *)defaultScriptColorCrashWithTimeInterval:(uint16_t)time inContext:(NSManagedObjectContext *)context;

+ (Script *)emptyScriptInContext:(NSManagedObjectContext *)context;

@end
