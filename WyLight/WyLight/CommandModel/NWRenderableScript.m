//
//  NWRenderableScript.m
//  WyLight
//
//  Created by Nils Weiß on 26/10/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWRenderableScript.h"
#import "NWComplexScriptCommandObject.h"
#import "UIImage+ImageEffects.h"

@interface NWRenderableScript ()

@property (atomic, assign, readwrite) BOOL rendering;

@end

@implementation NWRenderableScript

#define SNAPSHOT_KEY @"WyLightRemote.NWRenderableScript.snapshot"

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
        _snapshot = [aDecoder decodeObjectForKey:SNAPSHOT_KEY];
    }
    return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
    [super encodeWithCoder:aCoder];
    [aCoder encodeObject:_snapshot forKey:SNAPSHOT_KEY];
}

- (UIImage *)snapshot {
    if (self.rendering) {
        return nil;
    }
    return _snapshot;
}

- (BOOL)needsRendering {
    BOOL update = self.needsUpdate;
    for (NWComplexScriptCommandObject *cmplxCmd in self.scriptArray) {
        if (!update) {
            if (cmplxCmd.needsUpdate) {
                update = YES;
                cmplxCmd.needsUpdate = NO;
            }
        } else {
            cmplxCmd.needsUpdate = NO;
        }
        for (NWScriptCommandObject *cmd in cmplxCmd.scriptObjects) {
            if (!update) {
                if (cmd.needsUpdate) {
                    update = YES;
                    cmd.needsUpdate = NO;
                }
            } else {
                cmd.needsUpdate = NO;
            }
        }
    }
    if (self.needsUpdate) {
        self.needsUpdate = NO;
    }
    return update || (_snapshot == nil);
}

- (void)startRenderingWithRect:(CGRect)rect {
    if (self.rendering) {
        return;
    }
    {
        self.rendering = YES;
        @autoreleasepool {
            //Data collecting
            NSMutableArray *lineArray = [[NSMutableArray alloc] init];
            NSMutableArray *locationsArray = [[NSMutableArray alloc] init];
            NSMutableArray *lineHeightArray = [[NSMutableArray alloc] init];
            
            CGFloat position = 0;
            CGFloat red, green, blue;
            [locationsArray addObject:@(position)];
            
            const CGFloat heightFract = rect.size.height / ((NWComplexScriptCommandObject*)self.scriptArray.firstObject).colors.count;
            NSUInteger totalDuration = self.totalDurationInTmms.unsignedIntegerValue;
            for (int i = 0; i < ((NWComplexScriptCommandObject*)self.scriptArray.firstObject).colors.count; i++) {
                @autoreleasepool {
                    NSMutableArray *colorComponentsArray = [[NSMutableArray alloc] init];
                    for (NWComplexScriptCommandObject* cmd in self.scriptArray) {
                        if (i == 0) {
                            CGFloat width = (CGFloat)cmd.duration / (CGFloat)totalDuration;
                            position += width;
                            [locationsArray addObject:@(position)];
                        }
                        if (cmd == self.scriptArray.firstObject) {
                            red = 0; green = 0; blue = 0;
                            NSArray *colors = ((NWComplexScriptCommandObject*)self.scriptArray.lastObject).colors;
                            if (self.repeatWhenFinished) {
                                [((UIColor *)[colors objectAtIndex:i]) getRed:&red green:&green blue:&blue alpha:NULL];
                            }
                            [colorComponentsArray addObjectsFromArray:@[@(red), @(green), @(blue), @(1)]];
                            
                            CGFloat rectOriginY = floorf(i * heightFract);
                            CGFloat nextRectOriginY = floorf((i + 1) * heightFract);
                            [lineHeightArray addObject:@(nextRectOriginY - rectOriginY)];
                        }
                        red = 0; green = 0; blue = 0;
                        NSArray *colors = cmd.colors;
                        UIColor* color = [colors objectAtIndex:i];
                        [color getRed:&red green:&green blue:&blue alpha:NULL];
                        [colorComponentsArray addObjectsFromArray:@[@(red), @(green), @(blue), @(1)]];
                    }
                    [lineArray addObject:colorComponentsArray];
                }
            }
            @autoreleasepool {
                //Drawing
                CGImageRef			resultImage;
                
                CGContextRef		context;
                CGColorSpaceRef		colorSpace;
                CGSize				canvasSize;
                
                //Initialize the canvas size!
                canvasSize = rect.size;
                
                //Create the color space
                colorSpace = CGColorSpaceCreateDeviceRGB();
                
                context = CGBitmapContextCreate(NULL, canvasSize.width, canvasSize.height, 8, 0, colorSpace, kCGImageAlphaPremultipliedLast);
                
                CGContextTranslateCTM(context, 0.0f, rect.size.height);
                CGContextScaleCTM(context, 1.0f, -1.0f);
                
                
                CGFloat yPosition = 0;
                
                CGFloat *components = (CGFloat *) malloc(sizeof(CGFloat) * ((NSArray *)lineArray.firstObject).count);
                CGFloat *locations = (CGFloat *) malloc(sizeof(CGFloat) * locationsArray.count);
                for (int i = 0; i < lineArray.count; i++) {
                    @autoreleasepool {
                        NSArray *colors = [lineArray objectAtIndex:i];
                        [colors enumerateObjectsWithOptions:NSEnumerationConcurrent
                                                 usingBlock:^(id obj, NSUInteger idx, BOOL *stop)
                         {
                             components[idx] = [(NSNumber *)obj floatValue];
                         }];
                        [locationsArray enumerateObjectsWithOptions:NSEnumerationConcurrent usingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
                            locations[idx] = [(NSNumber *)obj floatValue];
                        }];
                        
                        CGGradientRef gradient;
                        gradient = CGGradientCreateWithColorComponents(colorSpace, components, locations, locationsArray.count);
                        
                        CGFloat currentHeight = (CGFloat)((NSNumber *)lineHeightArray[i]).floatValue;
                        CGPoint startPoint = CGPointMake(rect.origin.x, yPosition + currentHeight / 2 );
                        CGPoint endPoint = CGPointMake(rect.origin.x + rect.size.width, yPosition + currentHeight / 2);
                        
                        
                        CGContextSaveGState(context);
                        CGContextClipToRect(context, CGRectMake(rect.origin.x, yPosition, rect.size.width, currentHeight));
                        CGContextDrawLinearGradient(context, gradient, startPoint, endPoint, 0);
                        CGContextRestoreGState(context);
                        CGGradientRelease(gradient);
                        
                        yPosition += currentHeight;
                    }
                }
                free(components);
                free(locations);
                
                // Get your image
                //
                resultImage = CGBitmapContextCreateImage(context);
                UIColor *tintColor = [UIColor colorWithWhite:1.0 alpha:0.3];
                self.snapshot = [[UIImage imageWithCGImage:resultImage] applyBlurWithRadius:5 tintColor:tintColor saturationDeltaFactor:1.0 maskImage:nil];
                CGImageRelease(resultImage);
                CGColorSpaceRelease(colorSpace);
                CGContextRelease(context);
            }
        }
        self.rendering = NO;
        self.needsUpdate = NO;
        if (self.delegate) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.delegate NWRenderableScriptFinishedRendering:self];
            });
        }
    }
    return;
}

@end
