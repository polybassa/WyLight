//
//  NWScriptFolderViewController.m
//  WyLight
//
//  Created by Nils Weiß on 10/18/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptFolderViewController.h"
#import "NWScript.h"
#import "NWScriptCommandObject.h"
#import "iCarousel.h"
#import "NWScriptObjectControl.h"
#import "NWComplexScriptCommandObject.h"
#import "NWSetFadeScriptCommandObject.h"
#import "UIView+ImageEffect.h"

@interface NWScriptFolderViewController () <iCarouselDataSource, iCarouselDelegate>

@property (nonatomic) BOOL isDeletionModeActive;
@property (nonatomic, strong) NSMutableArray *scriptObjects;
@property (nonatomic, strong) iCarousel *carousel;
@property (nonatomic, strong) NWScriptObjectView *background;
@end

@implementation NWScriptFolderViewController

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    [self fixLocations];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setup];
}

- (void)fixLocations {
    if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
    }
	else {
    }
    self.background.frame = self.view.bounds;
    self.carousel.frame = CGRectMake(0, 60, self.view.bounds.size.width, self.view.bounds.size.height - 120);
}

- (void)setup {
    self.view.superview.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    
    self.background = [[NWScriptObjectView alloc] initWithFrame:CGRectZero];
    self.background.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    self.background.userInteractionEnabled = NO;
    self.background.opaque = YES;
    self.background.startColors = self.background.endColors = [NWScriptFolderViewController defaultCmd].colors;
    [self.view addSubview:self.background];
    
   
    UIToolbar* milkglassView = [[UIToolbar alloc] initWithFrame:self.view.frame];
    milkglassView.barStyle = UIBarStyleDefault;
    [self.view addSubview:milkglassView];
    
    self.carousel = [[iCarousel alloc] initWithFrame:CGRectZero];
    self.carousel.dataSource = self;
    self.carousel.delegate = self;
    self.carousel.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    self.carousel.type = iCarouselTypeCoverFlow;
    self.carousel.bounces = NO;
    self.carousel.pagingEnabled = YES;
    self.carousel.backgroundColor = [UIColor clearColor];
    self.carousel.opaque = NO;
    [self.view addSubview:self.carousel];
}
#pragma mark - default CMD
+ (NWSetFadeScriptCommandObject *)defaultCmd {
    NWSetFadeScriptCommandObject *cmd = [[NWSetFadeScriptCommandObject alloc]init];
    cmd.color = [UIColor colorWithRed:1 green:1 blue:1 alpha:1];
    cmd.address = 0xffffffff;
    cmd.duration = 5;
    return cmd;
}

#pragma mark - getter
- (NSMutableArray *)scriptObjects {
    if (!_scriptObjects) {
        _scriptObjects = [[NSMutableArray alloc] init];
        [_scriptObjects addObject:[[NWScript alloc] init]];
        [_scriptObjects addObject:[[NWScript alloc] init]];
        [_scriptObjects addObject:[[NWScript alloc] init]];
        [_scriptObjects addObject:[[NWScript alloc] init]];
        [_scriptObjects addObject:[[NWScript alloc] init]];
        [_scriptObjects addObject:[[NWScript alloc] init]];
    }
    return _scriptObjects;
}

- (void)setIsDeletionModeActive:(BOOL)isDeletionModeActive {
    _isDeletionModeActive = isDeletionModeActive;
}

#pragma mark - iCarouselDatasource
- (NSUInteger)numberOfItemsInCarousel:(iCarousel *)carousel {
    return self.scriptObjects.count;
}

- (UIView *)carousel:(iCarousel *)carousel viewForItemAtIndex:(NSUInteger)index reusingView:(UIView *)view {
    if (view == nil || ![view isKindOfClass:[NWScriptObjectControl class]])
    {
        view = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 200, 230)];
        view.contentMode = UIViewContentModeCenter;
    }
    
    if ([view isKindOfClass:[NWScriptObjectControl class]]) {
        NWScriptObjectControl* scriptView = [[NWScriptObjectControl alloc] initWithFrame:CGRectMake(0, 0, 200.0f, 230.0f)];;
        NWScript *script = [self.scriptObjects objectAtIndex:index];
        id<NWDrawableCommand> command = script.scriptArray.firstObject;
        [command setBackgroundColor:[UIColor clearColor]];
        scriptView.startColors = scriptView.endColors = [command colors];
        view.layer.cornerRadius = 5.0;
        
        UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPressOnScriptObjectControl:)];
        [view addGestureRecognizer:longPress];
        
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapOnScriptObjectControl:)];
        [view addGestureRecognizer:tap];
        
        UISwipeGestureRecognizer *swipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeUpOnScriptObjectControl:)];
        swipe.direction = UISwipeGestureRecognizerDirectionUp;
        [view addGestureRecognizer:swipe];
        
        ((UIImageView *)view).image = scriptView.blurredSnapshot;
        return view;
        
    }
    return nil;

}

#pragma mark - iCarouselDelegate

- (CGFloat)carousel:(iCarousel *)carousel valueForOption:(iCarouselOption)option withDefault:(CGFloat)value {
	switch (option)
	{
		case iCarouselOptionWrap:
		{
			//normally you would hard-code this to YES or NO
			return NO;
		}
		case iCarouselOptionSpacing:
		{
			//add a bit of spacing between the item views
			return value * 1.05f;
		}
		default:
		{
			return value;
		}
	}
}

- (void)carouselCurrentItemIndexDidChange:(iCarousel *)carousel {
    NWScript *currentScript = self.scriptObjects[carousel.currentItemIndex];
    NWComplexScriptCommandObject *cmd = currentScript.scriptArray.firstObject;
    [self.background setColorsAnimatedWithDuration:0.3 startColors:cmd.colors endColor:cmd.colors];
}

- (void)tapOnScriptObjectControl:(UITapGestureRecognizer *)gesture {
    
    NWScript *currentScript = self.scriptObjects[self.carousel.currentItemIndex];
    
    dispatch_async(dispatch_queue_create("animationQ", NULL), ^{
        for (NWComplexScriptCommandObject *cmd in currentScript.scriptArray) {
            if (cmd == currentScript.scriptArray.firstObject) {
                continue;
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.background setColorsAnimatedWithDuration:cmd.duration / 100 startColors:cmd.colors endColor:cmd.colors];
            });
            [NSThread sleepForTimeInterval:cmd.duration / 100];
        }
    });
    
    [UIView animateWithDuration:0.3 animations:^{
        gesture.view.alpha = 0.2;
    }];
    double delayInSeconds = (currentScript.totalDurationInTmms.doubleValue - ((NWComplexScriptCommandObject *)currentScript.scriptArray.firstObject).duration) / 100;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        [UIView animateWithDuration:0.5 animations:^{
            gesture.view.alpha = 1.0;
            [self.background setColorsAnimatedWithDuration:0.3
                                               startColors:((NWComplexScriptCommandObject *)currentScript.scriptArray.firstObject).colors
                                                  endColor:((NWComplexScriptCommandObject *)currentScript.scriptArray.firstObject).colors];
        }];
    });
    
}
@end
