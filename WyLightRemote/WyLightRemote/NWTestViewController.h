//
//  NWTestViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 16.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class WCWiflyControlWrapper;

@interface NWTestViewController : UIViewController

@property (nonatomic, weak) WCWiflyControlWrapper *controlHandle;

@end
