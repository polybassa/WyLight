//
//  NWEffectDrawer.h
//  WyLight
//
//  Created by Nils Weiß on 15/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ComplexEffect.h"

@class NWEffectDrawer;

@protocol NWEffectDrawerDelegate <NSObject>

- (void)NWEffectDrawer:(NWEffectDrawer *)drawer finishedDrawing:(id)effect;

@end

@interface NWEffectDrawer : NSObject

- (void)drawEffect:(ComplexEffect *)effect;
- (BOOL)effectIsDrawing:(id)effect;

- (void)drawScript:(Script *)script;

- (UIImage *)renderImageForEffect:(ComplexEffect *)effect;
- (UIImage *)renderImageForScript:(Script *)script;

@property (nonatomic, weak) id<NWEffectDrawerDelegate> delegate;

@end
