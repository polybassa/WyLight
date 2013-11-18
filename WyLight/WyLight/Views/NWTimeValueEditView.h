//
//  NWTimeValueEditView.h
//  WyLight
//
//  Created by Nils Weiß on 12.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ComplexEffect, NWTimeValueEditView;

@protocol NWTimeValueEditViewDelegate <NSObject>

- (void)TimeValueEditView:(NWTimeValueEditView *)view timeValueChanged:(NSNumber *)value;

@end

@interface NWTimeValueEditView : UIView

@property (nonatomic, weak) ComplexEffect *command;
@property (nonatomic, weak) id<NWTimeValueEditViewDelegate> delegate;
@property (nonatomic) CGFloat cornerRadius;

- (void)reloadData;

@end
