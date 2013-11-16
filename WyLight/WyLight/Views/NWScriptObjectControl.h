//
//  NWScriptObjectControl.h
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NWScriptObjectControl : UIView

@property (nonatomic, getter = isQuivering) BOOL quivering;
@property (nonatomic, getter = isDownscaled) BOOL downscale;

@end
