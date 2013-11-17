//
//  NWRenderableScriptImageView.h
//  WyLight
//
//  Created by Nils Weiß on 30/10/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NWRenderableScriptImageView : UIImageView

@property (nonatomic, getter = isQuivering) BOOL quivering;
@property (nonatomic, getter = isDownscaled) BOOL downscale;

@property (nonatomic) BOOL animateSetImage;

@property (nonatomic, assign) BOOL showActivityIndicator;
@property (nonatomic) CGFloat cornerRadius;

@end
