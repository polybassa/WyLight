//
//  NWComplexScriptCommandEditorViewController.h
//  WyLight
//
//  Created by Nils Weiß on 12.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NWComplexScriptCommandObject.h"

@interface NWComplexScriptCommandEditorViewController : UIViewController

@property (nonatomic, weak) NWComplexScriptCommandObject *command;
@property (nonatomic, weak) WCWiflyControlWrapper *controlHandle;

@end
