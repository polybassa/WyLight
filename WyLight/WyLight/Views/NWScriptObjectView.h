//
//  NWScriptObjectView.h
//  WyLightRemote
//
//  Created by Nils Weiß on 28.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectControl.h"

@interface NWScriptObjectView : NWScriptObjectControl

@property (nonatomic, strong) NSArray *startColors;
@property (nonatomic, strong) NSArray *endColors;
@property (nonatomic, readonly, strong) UITouch *latestTouchBegan;
@property (nonatomic) CGFloat cornerRadius;
@property (nonatomic) CGFloat orientation;

- (void)setColorsAnimatedWithDuration:(NSTimeInterval)duration startColors:(NSArray *)startColors endColor:(NSArray *)endColors;
- (void)setStartColors:(NSArray *)startColors withEndcolors:(NSArray *)endColors;
@end
