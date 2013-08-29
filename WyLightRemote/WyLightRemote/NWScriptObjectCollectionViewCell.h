//
//  NWScriptObjectCollectionViewCell.h
//  WyLightRemote
//
//  Created by Nils Weiß on 29.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "NWScriptObjectView.h"

@interface NWScriptObjectCollectionViewCell : UICollectionViewCell

@property (nonatomic, strong) NWScriptObjectView *scriptObjectView;
@property (nonatomic, strong) UIButton *deleteButton;

@end
