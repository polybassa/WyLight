//
//  NWScriptView.h
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NWScriptView, NWScriptObjectControl;

@protocol NWScriptViewDataSource <NSObject>

- (NWScriptObjectControl *)scriptView:(NWScriptView *)view objectForIndex:(NSUInteger)index;
- (NSUInteger)numberOfObjectsInScriptView:(NWScriptView *)view;
- (CGFloat)scriptView:(NWScriptView *)view widthOfObjectAtIndex:(NSUInteger)index;

@end

@interface NWScriptView : UIScrollView

@property (nonatomic, weak) id<NWScriptViewDataSource> dataSource;

- (void)reloadData;
- (void)fixLocationsOfSubviews;

@end
