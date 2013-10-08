//
//  NWEditComplexScriptObjectViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NWCollectionViewLayout.h"
#import "ALRadialMenu.h"
#import "NWComplexScriptCommandObject.h"
#import "WCWiflyControlWrapper.h"

@interface NWEditComplexScriptObjectViewController : UIViewController 

@property (nonatomic, weak) NWComplexScriptCommandObject *command; 
@property (nonatomic, weak) WCWiflyControlWrapper *controlHandle;

- (IBAction)unwindEditScriptObject:(UIStoryboardSegue *)segue;

@end
