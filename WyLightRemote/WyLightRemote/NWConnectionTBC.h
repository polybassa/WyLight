//
//  NWConnectionTBC.h
//  WyLightRemote
//
//  Created by Nils Weiß on 06.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class WCEndpoint;

@protocol WCWiflyControlDelegate;

@interface NWConnectionTBC : UITabBarController <WCWiflyControlDelegate>

@property (nonatomic, strong) WCEndpoint* endpoint;

@end
