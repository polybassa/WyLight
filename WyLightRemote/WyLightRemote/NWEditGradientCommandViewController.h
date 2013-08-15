//
//  NWEditGradientCommandViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NWCommandObject.h"

@class NWSetGradientScriptCommandObject;

@interface NWEditGradientCommandViewController : UIViewController

@property (nonatomic, weak) NWSetGradientScriptCommandObject *command;

- (IBAction)unwindChangeColorDone:(UIStoryboardSegue *)segue;

- (IBAction)unwindChangeColorCancel:(UIStoryboardSegue *)segue;


@end
