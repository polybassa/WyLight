//
//  NWEditComplexScriptObjectViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NWComplexScriptCommandObject.h"
#import "NWCollectionViewLayout.h"
#import "ALRadialMenu.h"

@interface NWEditComplexScriptObjectViewController : UIViewController <UIGestureRecognizerDelegate, UICollectionViewDataSource, UICollectionViewDelegateFlowLayout, NWCollectionViewLayoutDelegate, ALRadialMenuDelegate>

@property (nonatomic, strong) NWComplexScriptCommandObject *command; //turn to weak later

@property (nonatomic, weak) WCWiflyControlWrapper *controlHandle;

- (IBAction)unwindEditScriptObject:(UIStoryboardSegue *)segue;

@end
