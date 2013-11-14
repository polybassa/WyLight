//
//  Script+defaultScripts.m
//  WyLight
//
//  Created by Nils Weiß on 08/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Script+defaultScripts.h"
#import "ComplexEffect.h"
#import "Fade.h"
#import "Gradient.h"

@implementation Script (defaultScripts)

+ (Script *)defaultScriptFastColorChangeInContext:(NSManagedObjectContext *)context {
    Script *tempScript = [Script insertNewObjectIntoContext:context];
    tempScript.repeatsWhenFinished = @(YES);
    tempScript.title = NSLocalizedStringFromTable(@"FastColorKey", @"defaultScriptsLocalization", @"");
    for (int i = 0; i < 7; i++)
    {
        ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
        {
            Fade *obj = [Fade insertNewObjectIntoContext:context];
            obj.address = @(0xffffffff);
            switch (i) {
                case 0:
                    obj.color = [UIColor redColor];
                    break;
                case 1:
                    obj.color = [UIColor orangeColor];
                    break;
                case 2:
                    obj.color = [UIColor yellowColor];
                    break;
                case 3:
                    obj.color = [UIColor greenColor];
                    break;
                case 4:
                    obj.color = [UIColor cyanColor];
                    break;
                case 5:
                    obj.color = [UIColor blueColor];
                    break;
                case 6:
                    obj.color = [UIColor purpleColor];
                    break;
                    
                default:
                    break;
            }
            
            obj.complexEffect = comObj;
        }
        comObj.duration = @(100);
        comObj.script = tempScript;
    }
    return tempScript;
}

+ (Script *)defaultScriptSlowColorChangeInContext:(NSManagedObjectContext *)context {
    Script *tempScript = [Script insertNewObjectIntoContext:context];
    tempScript.repeatsWhenFinished = @(YES);
    tempScript.title = NSLocalizedStringFromTable(@"SlowColorKey", @"defaultScriptsLocalization", @"");
    for (int i = 0; i < 7; i++)
    {
        ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
        {
            Fade *obj = [Fade insertNewObjectIntoContext:context];
            obj.address = @(0xffffffff);
            switch (i) {
                case 0:
                    obj.color = [UIColor redColor];
                    break;
                case 1:
                    obj.color = [UIColor orangeColor];
                    break;
                case 2:
                    obj.color = [UIColor yellowColor];
                    break;
                case 3:
                    obj.color = [UIColor greenColor];
                    break;
                case 4:
                    obj.color = [UIColor cyanColor];
                    break;
                case 5:
                    obj.color = [UIColor blueColor];
                    break;
                case 6:
                    obj.color = [UIColor purpleColor];
                    break;
                    
                default:
                    break;
            }

            obj.complexEffect = comObj;
        }
        comObj.duration = @(1000);
        comObj.script = tempScript;
    }
    return tempScript;
}

+ (Script *)defaultScriptConzentrationLightInContext:(NSManagedObjectContext *)context {
    Script *tempScript = [Script insertNewObjectIntoContext:context];
    tempScript.repeatsWhenFinished = @(YES);
    tempScript.title = NSLocalizedStringFromTable(@"ConzentrationLightKey", @"defaultScriptsLocalization", @"");
    for (int i = 0; i < 6; i++)
    {
        ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
        {
            Fade *obj = [Fade insertNewObjectIntoContext:context];
            switch (i) {
                case 0:
                    obj.color = [UIColor colorWithRed:1 green:1 blue:0.8 alpha:1];
                    break;
                case 1:
                    obj.color = [UIColor colorWithRed:1 green:0.8 blue:0.8 alpha:1];
                    break;
                case 2:
                    obj.color = [UIColor colorWithRed:1 green:0.8 blue:1 alpha:1];
                    break;
                case 3:
                    obj.color = [UIColor colorWithRed:0.8 green:0.8 blue:1 alpha:1];
                    break;
                case 4:
                    obj.color = [UIColor colorWithRed:0.8 green:1 blue:1 alpha:1];
                    break;
                case 5:
                    obj.color = [UIColor colorWithRed:0.8 green:1 blue:0.8 alpha:1];
                    break;
                    
                default:
                    break;
            }
            obj.address = @(0xffffffff);
            obj.complexEffect = comObj;
        }
        comObj.duration = @(6000);
        comObj.script = tempScript;
    }
    return tempScript;
}

+ (Script *)defaultScriptMovingColorsInContext:(NSManagedObjectContext *)context {
    Script *tempScript = [Script insertNewObjectIntoContext:context];
    tempScript.repeatsWhenFinished = @(YES);
    tempScript.title = NSLocalizedStringFromTable(@"MovingColorsKey", @"defaultScriptsLocalization", @"");
    for (int i = 0; i < 7; i++) {
        ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
        {
            Gradient *obj = [Gradient insertNewObjectIntoContext:context];
            switch (i) {
                case 0:
                    obj.color1 = [UIColor redColor];
                    obj.color2 = [UIColor orangeColor];
                    break;
                case 1:
                    obj.color1 = [UIColor orangeColor];
                    obj.color2 = [UIColor yellowColor];
                    break;
                case 2:
                    obj.color1 = [UIColor yellowColor];
                    obj.color2 = [UIColor cyanColor];
                    break;
                case 3:
                    obj.color1 = [UIColor cyanColor];
                    obj.color2 = [UIColor blueColor];
                    break;
                case 4:
                    obj.color1 = [UIColor blueColor];
                    obj.color2 = [UIColor purpleColor];
                    break;
                case 5:
                    obj.color1 = [UIColor purpleColor];
                    obj.color2 = [UIColor greenColor];
                    break;
                case 6:
                    obj.color1 = [UIColor greenColor];
                    obj.color2 = [UIColor colorWithRed:1 green:1 blue:1 alpha:1];
                    break;
                    
                default:
                    break;
            }
            
            obj.offset = @(0);
            obj.numberOfLeds = @(32);
            obj.complexEffect = comObj;
        }
        comObj.duration = @(1000);
        comObj.script = tempScript;
    }
    return tempScript;
}

+ (Script *)defaultScriptRandomColorsInContext:(NSManagedObjectContext *)context {
    Script *tempScript = [Script insertNewObjectIntoContext:context];
    tempScript.repeatsWhenFinished = @(YES);
    tempScript.title = NSLocalizedStringFromTable(@"RandomColorsKey", @"defaultScriptsLocalization", @"");
    for (int i = 0; i < 7; i++)
    {
        ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
        {
            uint32_t adr1 = arc4random();
            uint32_t adr2 = arc4random();
            uint32_t adr3 = arc4random();
            uint32_t adr4 = arc4random();
            uint32_t adr5 = arc4random();
            uint32_t adr6 = arc4random();
            {
                Fade *obj = [Fade insertNewObjectIntoContext:context];
                obj.address = @((adr1 | adr2 | adr3 | adr4 | adr5 | adr6));
                obj.color = [UIColor colorWithRed:1 green:1 blue:1 alpha:1];
                obj.complexEffect = comObj;
            }
            {
                Fade *obj = [Fade insertNewObjectIntoContext:context];
                obj.address = @(adr1);
                obj.color = [UIColor redColor];
                obj.complexEffect = comObj;
            }
            {
                Fade *obj = [Fade insertNewObjectIntoContext:context];
                obj.address = @(adr2);
                obj.color = [UIColor orangeColor];
                obj.complexEffect = comObj;
            }
            {
                Fade *obj = [Fade insertNewObjectIntoContext:context];
                obj.address = @(adr3);
                obj.color = [UIColor blueColor];
                obj.complexEffect = comObj;
            }
            {
                Fade *obj = [Fade insertNewObjectIntoContext:context];
                obj.address = @(adr4);
                obj.color = [UIColor cyanColor];
                obj.complexEffect = comObj;
            }
            {
                Fade *obj = [Fade insertNewObjectIntoContext:context];
                obj.address = @(adr5);
                obj.color = [UIColor purpleColor];
                obj.complexEffect = comObj;
            }
            {
                Fade *obj = [Fade insertNewObjectIntoContext:context];
                obj.address = @(adr6);
                obj.color = [UIColor greenColor];
                obj.complexEffect = comObj;
            }
        }
        comObj.duration = @(100);
        comObj.script = tempScript;
    }
    return tempScript;
}

+ (Script *)defaultScriptRunLightWithColor:(UIColor *)color timeInterval:(uint16_t)time inContext:(NSManagedObjectContext *)context {
    Script *tempScript = [Script insertNewObjectIntoContext:context];
    tempScript.repeatsWhenFinished = @(YES);
    tempScript.title = NSLocalizedStringFromTable(@"RunLightKey", @"defaultScriptsLocalization", @"");
    {
        for (int i = 0; i < 32; i = i + 2) {
            ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
            {
                {
                    Fade *obj = [Fade insertNewObjectIntoContext:context];
                    obj.address = @(0xffffffff);
                    obj.color = [UIColor colorWithRed:0 green:0 blue:0 alpha:1];
                    obj.complexEffect = comObj;
                }
                {
                    Fade *obj = [Fade insertNewObjectIntoContext:context];
                    obj.address = @(0x00000003 << i);
                    obj.color = color;
                    obj.complexEffect = comObj;
                }
            }
            comObj.duration = @(time);
            comObj.script = tempScript;
        }
    }
    return tempScript;
}

+ (Script *)defaultScriptColorCrashWithTimeInterval:(uint16_t)time inContext:(NSManagedObjectContext *)context {
    Script *tempScript = [Script insertNewObjectIntoContext:context];
    tempScript.repeatsWhenFinished = @(YES);
    tempScript.title = NSLocalizedStringFromTable(@"ColorCrashKey", @"defaultScriptsLocalization", @"");
    {
        for (int i = 0; i < 8; i++) {
            ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
            {
                {
                    Fade *obj = [Fade insertNewObjectIntoContext:context];
                    obj.address = @(0xffffffff);
                    obj.color = [UIColor colorWithRed:0 green:0 blue:0 alpha:1];
                    obj.complexEffect = comObj;
                }
                {
                    Fade *obj = [Fade insertNewObjectIntoContext:context];
                    obj.address = @(0x00000003 << i*2);
                    obj.color = [UIColor redColor];
                    obj.complexEffect = comObj;
                }
                {
                    Fade *obj = [Fade insertNewObjectIntoContext:context];
                    obj.address = @(0xC0000000 >> i*2);
                    obj.color = [UIColor greenColor];
                    obj.complexEffect = comObj;
                }
            }
            comObj.duration = @(time);
            comObj.script = tempScript;
            //[tempScript addEffectsObject:comObj];
        }
        for (int i = 0; i < 8; i++) {
            ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
            {
                {
                    Fade *obj = [Fade insertNewObjectIntoContext:context];
                    obj.address = @(0xffffffff);
                    obj.color = [UIColor colorWithRed:0 green:0 blue:0 alpha:1];
                    obj.complexEffect = comObj;
                }
                {
                    Fade *obj = [Fade insertNewObjectIntoContext:context];
                    obj.address = @(0x00030000 << i*2);
                    obj.color = [UIColor blueColor];
                    obj.complexEffect = comObj;
                }
                {
                    Fade *obj = [Fade insertNewObjectIntoContext:context];
                    obj.address = @(0x0000C000 >> i*2);
                    obj.color = [UIColor yellowColor];
                    obj.complexEffect = comObj;
                }
            }
            comObj.duration = @(time);
            comObj.script = tempScript;
        }
        
    }
    return tempScript;
}

+ (Script *)emptyScriptInContext:(NSManagedObjectContext *)context {
    Script *tempScript = [Script insertNewObjectIntoContext:context];
    tempScript.repeatsWhenFinished = @(YES);
    tempScript.title = NSLocalizedStringFromTable(@"NewScriptKey", @"defaultScriptsLocalization", @"");
    {
        ComplexEffect *comObj = [ComplexEffect insertNewObjectIntoContext:context];
        {
            Fade *obj = [Fade insertNewObjectIntoContext:context];
            obj.address = @(0xffffffff);
            obj.color = [UIColor colorWithRed:0 green:0 blue:0 alpha:1];
            obj.complexEffect = comObj;
        }
        comObj.duration = @(1000);
        comObj.script = tempScript;
    }
    return tempScript;
}

@end
