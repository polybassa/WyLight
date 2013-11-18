//
//  NWEffectDrawer.m
//  WyLight
//
//  Created by Nils Weiß on 15/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWEffectDrawer.h"
#import "Script.h"
#import "UIImage+ImageEffects.h"

@interface NWEffectDrawer ()

@property (nonatomic, strong) NSMutableSet *currentJobs;

@end

@implementation NWEffectDrawer

- (NSMutableSet *)currentJobs {
    if (_currentJobs == nil) {
        _currentJobs = [[NSMutableSet alloc] init];
    }
    return _currentJobs;
}

- (BOOL)effectIsDrawing:(ComplexEffect *)effect {
    if (effect) {
        return [self.currentJobs containsObject:effect];
    }
    return self.currentJobs.count;
}

- (UIImage *)renderImageForEffect:(ComplexEffect *)effect {
    UIImage *resultUIImage;
    @autoreleasepool {
        CGRect rect = CGRectMake(0, 0, 200, 200);
        //Data collecting
        NSMutableArray *lineHeightArray = [[NSMutableArray alloc] init];
        
        const CGFloat heightFract = rect.size.height / effect.colors.count;
        for (int i = 0; i < effect.colors.count; i++) {
            CGFloat rectOriginY = floorf(i * heightFract);
            CGFloat nextRectOriginY = floorf((i + 1) * heightFract);
            [lineHeightArray addObject:@(nextRectOriginY - rectOriginY)];
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
            
            for (int i = 0; i < effect.colors.count; i++) {
                
                UIColor *endColor = effect.colors[i];
                NSUInteger positionInScript = [effect.script.effects indexOfObject:effect];
                UIColor *startColor;
                if (positionInScript == 0) {
                    if (effect.script.repeatsWhenFinished.boolValue) {
                        ComplexEffect *prev = [effect.script.effects lastObject];
                        if (prev.colors.count > i) {
                            startColor = prev.colors[i];
                        }
                    }
                    else {
                        startColor = effect.backgroundColor;
                    }
                } else {
                    if (effect.prev.colors.count > i) {
                        startColor = effect.prev.colors[i];
                    }
                }
                
                if ([endColor isEqual:[UIColor blackColor]]) {
                    endColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:1];
                }
                if ([startColor isEqual:[UIColor blackColor]]) {
                    startColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:1];
                }
                
                CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
                CGFloat locations[2] = {0.0f, 1.0f};
                CGFloat startRed, startGreen, startBlue, startAlpha;
                CGFloat endRed, endGreen, endBlue, endAlpha;
                
                [endColor getRed:&endRed green:&endGreen blue:&endBlue alpha:&endAlpha];
                [startColor getRed:&startRed green:&startGreen blue:&startBlue alpha:&startAlpha];
                
                CGFloat components[8] = {
                    startRed, startGreen, startBlue, startAlpha,
                    endRed, endGreen, endBlue, endAlpha
                };
                
                CGGradientRef gradient;
                gradient = CGGradientCreateWithColorComponents(colorSpace, components, locations, 2);
                
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
            
            // Get your image
            //
            resultImage = CGBitmapContextCreateImage(context);
            UIColor *tintColor = [UIColor clearColor];
            resultUIImage = [[UIImage imageWithCGImage:resultImage] applyBlurWithRadius:1 tintColor:tintColor saturationDeltaFactor:1.0 maskImage:nil];
            CGImageRelease(resultImage);
            CGColorSpaceRelease(colorSpace);
            CGContextRelease(context);
        }
    }
    return resultUIImage;
}

- (void)drawEffect:(ComplexEffect *)effect {
    if (effect.snapshot != nil || [self.currentJobs containsObject:effect]) {
        [self.delegate NWEffectDrawer:self finishedDrawing:effect];
        return;
    } else {
        NSLog(@"draw effect");
        [self.currentJobs addObject:effect];
       
        dispatch_async(dispatch_queue_create([[NSString stringWithFormat:@"drawing effect for %@", effect.description] cStringUsingEncoding:NSASCIIStringEncoding]
                                             , NULL), ^{
            UIImage *resultUIImage = [self renderImageForEffect:effect];
            
            dispatch_async(dispatch_get_main_queue(), ^{
                effect.snapshot = resultUIImage;
                [self.currentJobs removeObject:effect];
                [self.delegate NWEffectDrawer:self finishedDrawing:effect];
            });
        });
    }
}

- (void)drawScript:(Script *)script {
    if (script.snapshot != nil || [self.currentJobs containsObject:script]) {
        [self.delegate NWEffectDrawer:self finishedDrawing:script];
        return;
    } else {
        NSLog(@"script effect");
        [self.currentJobs addObject:script];
        
        dispatch_async(dispatch_queue_create([[NSString stringWithFormat:@"drawing effect for %@", script.description] cStringUsingEncoding:NSASCIIStringEncoding]
                                             , NULL), ^{
            UIImage *resultUIImage = [self renderImageForScript:script];
            
            dispatch_async(dispatch_get_main_queue(), ^{
                script.snapshot = resultUIImage;
                [self.currentJobs removeObject:script];
                [self.delegate NWEffectDrawer:self finishedDrawing:script];
            });
        });
    }

}

- (UIImage *)renderImageForScript:(Script *)script {
    CGSize imageSize = CGSizeMake(200, 200);
    
    CGFloat leftDuration = script.totalDurationInTmms.floatValue;
    CGFloat xPos = 0;
    UIGraphicsBeginImageContext(imageSize);
    
    for (ComplexEffect *effect in script.effects) {
        if (effect.snapshot == nil) {
            effect.snapshot = [self renderImageForEffect:effect];
        }
        xPos = floorf(xPos);
        CGFloat widthFract = (imageSize.width - xPos) / leftDuration;
        CGFloat nextXPos = xPos + floorf(widthFract * effect.duration.floatValue);
        CGRect drawArea = CGRectMake(xPos, 0, nextXPos - xPos, imageSize.height);
        [effect.snapshot drawInRect:drawArea];
        xPos = nextXPos;
        leftDuration -= effect.duration.floatValue;
    }
    UIImage *outputImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return outputImage;
}

@end
