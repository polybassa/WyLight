//
//  NWScript+defaultScripts.m
//  WyLight
//
//  Created by Nils Weiß on 10/22/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScript+defaultScripts.h"
#import "NWSetFadeScriptCommandObject.h"
#import "NWSetGradientScriptCommandObject.h"
#import "NWComplexScriptCommandObject.h"

@implementation NWScript (defaultScripts)

+ (NWScript *)defaultScriptFastColorChange {
    NWScript *tempScript = [[NWScript alloc] init];
    tempScript.title = @"Fast Colors";
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor redColor];
		
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 100;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor orangeColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 100;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor yellowColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 100;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor greenColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 100;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor cyanColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 100;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor blueColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 100;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor purpleColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 100;
        [tempScript addObject:comObj];
    }
    return tempScript;
}

+ (NWScript *)defaultScriptSlowColorChange {
    NWScript *tempScript = [[NWScript alloc] init];
    tempScript.title = @"Slow Colors";
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor redColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor orangeColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor yellowColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor greenColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor cyanColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor blueColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor purpleColor];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    return tempScript;
}

+ (NWScript *)defaultScriptConzentrationLight {
    NWScript *tempScript = [[NWScript alloc] init];
    tempScript.title = @"Conzentration Light";
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor colorWithRed:1 green:1 blue:0.8 alpha:1];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 6000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor colorWithRed:1 green:0.8 blue:0.8 alpha:1];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 6000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor colorWithRed:1 green:0.8 blue:1 alpha:1];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 6000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor colorWithRed:0.8 green:0.8 blue:1 alpha:1];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 6000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor colorWithRed:0.8 green:1 blue:1 alpha:1];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 6000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
            obj.address = 0xffffffff;
            obj.color = [UIColor colorWithRed:0.8 green:1 blue:0.8 alpha:1];
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 6000;
        [tempScript addObject:comObj];
    }
    return tempScript;
}

+ (NWScript *)defaultScriptMovingColors {
    NWScript *tempScript = [[NWScript alloc] init];
    tempScript.title = @"Moving Colors";
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
            obj.color1 = [UIColor redColor];
            obj.color2 = [UIColor orangeColor];
            obj.offset = 0;
            obj.numberOfLeds = 32;
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
            obj.color1 = [UIColor orangeColor];
            obj.color2 = [UIColor yellowColor];
            obj.offset = 0;
            obj.numberOfLeds = 32;
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
            obj.color1 = [UIColor yellowColor];
            obj.color2 = [UIColor cyanColor];
            obj.offset = 0;
            obj.numberOfLeds = 32;
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
            obj.color1 = [UIColor cyanColor];
            obj.color2 = [UIColor blueColor];
            obj.offset = 0;
            obj.numberOfLeds = 32;
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
            obj.color1 = [UIColor blueColor];
            obj.color2 = [UIColor purpleColor];
            obj.offset = 0;
            obj.numberOfLeds = 32;
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
            obj.color1 = [UIColor purpleColor];
            obj.color2 = [UIColor greenColor];
            obj.offset = 0;
            obj.numberOfLeds = 32;
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
            obj.color1 = [UIColor greenColor];
            obj.color2 = [UIColor colorWithRed:1 green:1 blue:1 alpha:1];
            obj.offset = 0;
            obj.numberOfLeds = 32;
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
            obj.color1 = [UIColor colorWithRed:1 green:1 blue:1 alpha:1];
            obj.color2 = [UIColor redColor];
            obj.offset = 0;
            obj.numberOfLeds = 32;
            
            [comObj.scriptObjects addObject:obj];
        }
        comObj.duration = 1000;
        [tempScript addObject:comObj];
    }
    return tempScript;
}

+ (NWScript *)defaultScriptRandomColors {
    NWScript *tempScript = [[NWScript alloc] init];
    tempScript.title = @"Random Colors";
    for (int i = 0; i < 7; i++)
    {
        NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
        {
            uint32_t adr1 = arc4random();
            uint32_t adr2 = arc4random();
            uint32_t adr3 = arc4random();
            uint32_t adr4 = arc4random();
            uint32_t adr5 = arc4random();
            uint32_t adr6 = arc4random();
            {
                NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                obj.address = ~(adr1 | adr2 | adr3 | adr4 | adr5 | adr6);
                obj.color = [UIColor colorWithRed:1 green:1 blue:1 alpha:1];
                [comObj.scriptObjects addObject:obj];
            }
            {
                NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                obj.address = adr1;
                obj.color = [UIColor redColor];
                [comObj.scriptObjects addObject:obj];
            }
            {
                NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                obj.address = adr2;
                obj.color = [UIColor greenColor];
                [comObj.scriptObjects addObject:obj];
            }
            {
                NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                obj.address = adr3;
                obj.color = [UIColor blueColor];
                [comObj.scriptObjects addObject:obj];
            }
            {
                NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                obj.address = adr4;
                obj.color = [UIColor cyanColor];
                [comObj.scriptObjects addObject:obj];
            }
            {
                NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                obj.address = adr5;
                obj.color = [UIColor purpleColor];
                [comObj.scriptObjects addObject:obj];
            }
            {
                NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                obj.address = adr6;
                obj.color = [UIColor orangeColor];
                [comObj.scriptObjects addObject:obj];
            }
        }
        comObj.duration = 100;
        [tempScript addObject:comObj];
    }
    return tempScript;
}

+ (NWScript *)defaultScriptRunLightWithColor:(UIColor *)color timeInterval:(uint16_t)time {
    NWScript *tempScript = [[NWScript alloc] init];
    tempScript.title = @"Run Light";
    {
        for (int i = 0; i < 32; i = i + 2) {
            NWComplexScriptCommandObject *comObj = [[NWComplexScriptCommandObject alloc] init];
            {
                {
                    NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                    obj.address = 0xffffffff;
                    obj.color = [UIColor blackColor];
                    [comObj.scriptObjects addObject:obj];
                }
                {
                    NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
                    obj.address = 0x00000003 << i;
                    obj.color = color;
                    [comObj.scriptObjects addObject:obj];
                }
            }
            comObj.duration = time;
            [tempScript addObject:comObj];
        }
    }
    return tempScript;
}

@end
