//
//  NWConnectionViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 06.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol NWUnwindSegueOnConnectionLostDelegate

@optional

- (void) performUnwindSegue;

@end

@class WCEndpoint, WCWiflyControlWrapper;

@interface NWConnectionViewController : UIViewController 

@property (nonatomic, weak) WCWiflyControlWrapper *controlHandle;
@property (nonatomic, weak) id <NWUnwindSegueOnConnectionLostDelegate> delegate;

@end
