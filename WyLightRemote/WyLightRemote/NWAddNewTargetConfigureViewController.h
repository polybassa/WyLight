//
//  NWAddNewTargetConfigureViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol WCWiflyControlDelegate;

@interface NWAddNewTargetConfigureViewController : UIViewController <UITextFieldDelegate, WCWiflyControlDelegate, UIScrollViewDelegate>

@property (nonatomic, assign) BOOL configureTargetAsSoftAP;

@end
