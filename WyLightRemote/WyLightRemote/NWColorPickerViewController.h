//
//  NWColorPickerViewController.h
//  WyLightRemote
//
//  Created by Nils Weiß on 07.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NWDefaultColorPickerViewController.h"

@class WCWiflyControlWrapper;


@interface NWColorPickerViewController : NWDefaultColorPickerViewController <NWDefaultColorControllerDelegate>

@property (nonatomic, weak) WCWiflyControlWrapper *controlHandle;

@end

