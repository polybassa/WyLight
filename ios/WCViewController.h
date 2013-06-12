//
//  WCViewController.h
//  WyLightExample
//
//  Created by Nils Weiß on 11.06.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WCWiflyControlWrapper.h"

@class WCEndpoint;

@interface WCViewController : UIViewController <WCWiflyControlDelegate>

@property (nonatomic, strong) NSMutableArray *targetEndpoints;
@property (nonatomic, strong) NSMutableArray *ledControllerArray;
@property (nonatomic, weak) id <WCWiflyControlDelegate> delegate;

@end
