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
#import "UIView+blurredSnapshot.h"
#import <QuartzCore/QuartzCore.h>
#import "UIView+Quivering.h"
#import "WCWiflyControlWrapper.h"
#import "NWScriptViewController.h"
#import "NWScript+defaultScripts.h"
#import "NWScript+Snapshot.h"

@interface NWScriptFolderViewController () <iCarouselDataSource, iCarouselDelegate>

@property (nonatomic, assign) BOOL isDeletionModeActive;
@property (nonatomic, strong) NSMutableArray *scriptObjects;
@property (nonatomic, strong) iCarousel *carousel;
@property (nonatomic, strong) UIImageView *background;
@property (nonatomic, strong) UIButton *sendButton;
@property (nonatomic, strong) UILabel *scriptTitleLabel;
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

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.carousel reloadData];
}

- (void)viewDidAppear:(BOOL)animated {
    if ([self.carousel.currentItemView isKindOfClass:[UIImageView class]]) {
        [UIView animateWithDuration:0.3 animations:^{
             self.background.image = ((UIImageView *)[self.carousel currentItemView]).image;
        }];
    }
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
    [self saveUserData];
    [super viewWillDisappear:animated];
}

- (void)fixLocations {
    if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
        [self.tabBarController.tabBar setHidden:NO];
        self.sendButton.frame = CGRectMake(0, self.view.frame.size.height - 100, self.view.frame.size.width, 44);
    }
	else {
        [self.tabBarController.tabBar setHidden:YES];
        self.sendButton.frame = CGRectMake(0, self.view.frame.size.height - 44, self.view.frame.size.width, 44);
    }
    self.background.frame = self.view.bounds;
    self.carousel.frame = CGRectMake(0, 80, self.view.bounds.size.width, self.view.bounds.size.height - 160);
}

#define SCRIPTARRAY_KEY @"WyLightRemote.NWScript.ScriptArray"

- (void)setup {
    self.view.superview.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    
    NSData *data = [[NSUserDefaults standardUserDefaults] objectForKey:SCRIPTARRAY_KEY];
     if (data) {
     self.scriptObjects = [NSKeyedUnarchiver unarchiveObjectWithData:data];
     } else {
         self.scriptObjects = [[NSMutableArray alloc] init];
         [self.scriptObjects addObject:[NWScript defaultScriptFastColorChange]];
         [self.scriptObjects addObject:[NWScript defaultScriptSlowColorChange]];
         [self.scriptObjects addObject:[NWScript defaultScriptConzentrationLight]];
         [self.scriptObjects addObject:[NWScript defaultScriptMovingColors]];
         [self.scriptObjects addObject:[NWScript defaultScriptRandomColors]];
         [self.scriptObjects addObject:[NWScript defaultScriptRunLightWithColor:[UIColor greenColor] timeInterval:100]];
         [self.scriptObjects addObject:[NWScript defaultScriptRunLightWithColor:[UIColor redColor] timeInterval:100]];
         [self.scriptObjects addObject:[NWScript defaultScriptRunLightWithColor:[UIColor blueColor] timeInterval:100]];
     }
   
    self.background = [[UIImageView alloc] initWithFrame:self.view.frame];
    self.background.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    self.background.contentMode = UIViewContentModeScaleToFill;
    self.background.userInteractionEnabled = NO;
    self.background.opaque = YES;
    [self.view addSubview:self.background];
    
   
    UIView* milkglassView = [[UIView alloc] initWithFrame:self.view.frame];
    milkglassView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    milkglassView.opaque = NO;
    milkglassView.backgroundColor = [UIColor colorWithRed:1 green:1 blue:1 alpha:0.5];
    [self.view addSubview:milkglassView];
    
    self.sendButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[self.sendButton setTitle:@"SEND" forState:UIControlStateNormal];
	[self.sendButton addTarget:self action:@selector(sendScript) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:self.sendButton];
    
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

- (void)saveUserData {
	NSData *data = [NSKeyedArchiver archivedDataWithRootObject:self.scriptObjects];
	if (data) {
		[[NSUserDefaults standardUserDefaults] setObject:data forKey:SCRIPTARRAY_KEY];
	}
}

#pragma mark - GETTER & SETTER
- (NSMutableArray *)scriptObjects {
    if (!_scriptObjects) {
        _scriptObjects = [[NSMutableArray alloc] init];
        [_scriptObjects addObject:[[NWScript alloc] init]];
    }
    return _scriptObjects;
}

- (void)setIsDeletionModeActive:(BOOL)isDeletionModeActive {
    if (_isDeletionModeActive != isDeletionModeActive) {
        _isDeletionModeActive = isDeletionModeActive;
        if (_isDeletionModeActive) {
            [self.carousel.currentItemView startQuivering];
            self.carousel.scrollEnabled = NO;
        } else {
            [self.carousel.currentItemView stopQuivering];
            self.carousel.scrollEnabled = YES;
        }
    }
}

- (void)sendScript {
	dispatch_queue_t sendScriptQueue = dispatch_queue_create("sendScriptQueue", NULL);
	dispatch_async(sendScriptQueue, ^{
		[self.controlHandle clearScript];
		[self.controlHandle setColorDirect:[UIColor blackColor]];
		[self.controlHandle loopOn];
        NWScript *currentScript = self.scriptObjects[self.carousel.currentItemIndex];
		for (NWComplexScriptCommandObject* command in currentScript.scriptArray) {
			[command sendToWCWiflyControl:self.controlHandle];
			[NSThread sleepForTimeInterval:0.1];
		}
		[self.controlHandle loopOffAfterNumberOfRepeats:0];
	});
}

#pragma mark - iCarouselDatasource
- (NSUInteger)numberOfItemsInCarousel:(iCarousel *)carousel {
    return self.scriptObjects.count;
}

- (UIView *)carousel:(iCarousel *)carousel viewForItemAtIndex:(NSUInteger)index reusingView:(UIView *)view {
    if (view == nil || ![view isKindOfClass:[UIImageView class]])
    {
        view = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 200, 200)];
        view.contentMode = UIViewContentModeCenter;
    }
    
    if ([view isKindOfClass:[UIImageView class]]) {
        ((UIImageView *)view).image = [((NWScript *)self.scriptObjects[index]) snapshotWithRect:view.frame];
        
        view.layer.cornerRadius = 5.0;
        view.clipsToBounds = YES;
        
        UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPressOnScriptObjectControl:)];
        [view addGestureRecognizer:longPress];
        
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapOnScriptObjectControl:)];
        [view addGestureRecognizer:tap];
        
        UISwipeGestureRecognizer *swipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeUpOnScriptObjectControl:)];
        swipe.direction = UISwipeGestureRecognizerDirectionUp;
        [view addGestureRecognizer:swipe];
        
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
    if ([self.carousel.currentItemView isKindOfClass:[UIImageView class]]) {
        self.background.image = ((UIImageView *)[self.carousel currentItemView]).image;
    }
}

- (void)tapOnScriptObjectControl:(UITapGestureRecognizer *)gesture {
    if (self.isDeletionModeActive) {
        self.isDeletionModeActive = NO;
    } else {
        [self performSegueWithIdentifier:@"edit:" sender:self];
    }
}

- (void)longPressOnScriptObjectControl:(UILongPressGestureRecognizer *)gesture {
    self.isDeletionModeActive = YES;
}

- (void)swipeUpOnScriptObjectControl:(UISwipeGestureRecognizer *)gesture {
    if (self.isDeletionModeActive && self.scriptObjects.count > 1) {
        [self.scriptObjects removeObjectAtIndex:self.carousel.currentItemIndex];
        [self.carousel removeItemAtIndex:self.carousel.currentItemIndex animated:YES];
    }
    self.isDeletionModeActive = NO;
}

- (IBAction)addBarButtonPressed:(UIBarButtonItem *)sender {
}

#pragma mark - SEGUE
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.identifier isEqualToString:@"edit:"])
	{
		if ([segue.destinationViewController respondsToSelector:@selector(setControlHandle:)])
		{
			[segue.destinationViewController performSelector:@selector(setControlHandle:) withObject:self.controlHandle];
		}
		
		if ([segue.destinationViewController isKindOfClass:[NWScriptViewController class]]) {
			NWScriptViewController *ctrl = (NWScriptViewController *)segue.destinationViewController;
			ctrl.script = [self.scriptObjects objectAtIndex:self.carousel.currentItemIndex];
		}
	}
}



@end
