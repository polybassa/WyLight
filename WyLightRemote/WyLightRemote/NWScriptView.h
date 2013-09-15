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

- (UIView *)scriptView:(NWScriptView *)view objectViewForIndex:(NSUInteger)index;
- (UIView *)scriptView:(NWScriptView *)view timeInfoViewForIndex:(NSUInteger)index;
- (NSUInteger)numberOfObjectsInScriptView:(NWScriptView *)view;
- (CGFloat)scriptView:(NWScriptView *)view widthOfObjectAtIndex:(NSUInteger)index;

@end

@interface NWScriptView : UIScrollView

@property (nonatomic, weak) id<NWScriptViewDataSource> dataSource;
@property (nonatomic) CGFloat timeScaleFactor;

- (void)reloadData;
- (void)fixLocationsOfSubviews;
- (void)fixLocationOfTimeInfoView:(NSInteger)tag;
@end
