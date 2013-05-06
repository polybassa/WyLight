//
//  NWAppDelegate.h
//  WiflyControlObjCWrapper
//
//  Created by Nils Weiß on 17.04.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WCBroadcastReceiverWrapper.h"
#import "WCWiflyControlWrapper.h"

@interface NWAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) WCBroadcastReceiverWrapper *broadcastreceiver;
@property (strong, nonatomic) WCWiflyControlWrapper *control;

@end
