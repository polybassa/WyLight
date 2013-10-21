//
//  NWScriptViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class WCWiflyControlWrapper, NWScript;

@interface NWScriptViewController : UIViewController

@property (nonatomic, weak) WCWiflyControlWrapper *controlHandle;
@property (weak, nonatomic) NWScript *script;


@end
