//
//  NWScriptObjectView.h
//  WyLightRemote
//
//  Created by Nils Weiß on 28.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NWScriptObjectView : UIView

@property (nonatomic, strong) NSArray *startColors;
@property (nonatomic, strong) NSArray *endColors;
@property (nonatomic, strong) UITouch *latestTouchBegan;
@property (nonatomic) CGFloat cornerRadius;
@property (nonatomic) CGFloat borderWidth;


@end
