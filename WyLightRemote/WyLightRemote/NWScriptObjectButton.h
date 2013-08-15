//
//  NWScriptObjectButton.h
//  WyLightRemote
//
//  Created by Nils Weiß on 09.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NWScriptObjectButton : UIButton

@property (nonatomic, strong) NSArray *startColors;
@property (nonatomic, strong) NSArray *endColors;

@property (nonatomic, strong) UITouch *latestTouchBegan;

- (void)pinch:(UIPinchGestureRecognizer *)gesture;
@end
