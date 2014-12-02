//
//  NWFadeEditView.h
//  WyLight
//
//  Created by Nils Weiß on 13.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NWFadeEditView, Effect;

@protocol NWFadeEditViewDelegate <NSObject>

- (void)NWFadeEditView:(NWFadeEditView *)view bitmaskChanged:(uint32_t)bitmask;

@end

@interface NWFadeEditView : UIView

@property (nonatomic, weak) Effect *command;
@property (nonatomic, weak) id<NWFadeEditViewDelegate> delegate;
@property (nonatomic) CGFloat cornerRadius;

- (void)reloadData;

@end
