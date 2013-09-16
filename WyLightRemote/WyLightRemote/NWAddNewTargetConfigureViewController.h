//
//  NWAddNewTargetConfigureViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WCWiflyControlWrapper.h"

@interface NWAddNewTargetConfigureViewController : UIViewController <UITextFieldDelegate, WCWiflyControlDelegate>

@property (nonatomic, assign) BOOL configureTargetAsSoftAP;

@end
