//
//  NWTimeLineView.h
//  WyLightRemote
//
//  Created by Nils Weiß on 01.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NWTimeLineView : UIView

@property (nonatomic) CGPoint contentOffset;
@property (nonatomic) CGFloat strokeWidth;
@property (nonatomic) CGFloat timeLineWidth;
@property (nonatomic) CGFloat timeLineScaleFactor;
@property (nonatomic) CGFloat scaleResolution;
@property (nonatomic) CGRect contentFrame;

@end
