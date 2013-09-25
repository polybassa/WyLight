//
//  NWEditFadeCommandViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NWSetFadeScriptCommandObject.h"

@interface NWEditFadeCommandViewController : UIViewController

@property (nonatomic, strong) NWSetFadeScriptCommandObject *command;

- (IBAction)unwindChangeColorDone:(UIStoryboardSegue *)segue;

- (IBAction)unwindChangeColorCancel:(UIStoryboardSegue *)segue;

@end
