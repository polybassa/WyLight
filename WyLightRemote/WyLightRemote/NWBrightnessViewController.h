//
//  NWBrightnessViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 07.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class WCWiflyControlWrapper;

@interface NWBrightnessViewController : UIViewController

@property (nonatomic, weak) WCWiflyControlWrapper *controlHandle;

@end
