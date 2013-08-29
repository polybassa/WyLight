//
//  NWScriptObjectScrollView.h
//  WyLightRemote
//
//  Created by Nils Weiß on 28.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NWScriptCommandObject.h"
#import "NWScriptObjectButton.h"

@protocol NWScriptObjectScrollViewDelegate

- (void)positionInScrollViewChanged:(NWScriptObjectButton *)button;

@end

@interface NWScriptObjectScrollView : UIView <NWScriptObjectButtonDelegate>

@property (nonatomic, weak) UIView *mainView;

- (void) reloadDataFromArray:(NSArray *)array;

@property (nonatomic, weak) id<NWScriptObjectScrollViewDelegate> delegate;

@end
