//
//  NWScriptFolderViewController.m
//  WyLight
//
//  Created by Nils Weiß on 10/18/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptFolderViewController.h"
#import "Command.h"
#import "iCarousel.h"
#import "ComplexEffect.h"
#import "UIView+Quivering.h"
#import "WCWiflyControlWrapper.h"
#import "NWScriptViewController.h"
#import "Script+defaultScripts.h"
#import "NWRenderableScriptImageView.h"
#import "NWEffectDrawer.h"
#import "Script+serialization.h"
#import <MessageUI/MFMailComposeViewController.h>

@interface NWScriptFolderViewController () <iCarouselDataSource, iCarouselDelegate, NWEffectDrawerDelegate, MFMailComposeViewControllerDelegate>

@property (nonatomic, assign) BOOL isDeletionModeActive;
@property (nonatomic, strong) UIBarButtonItem *addButton;
@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, strong) NSArray *scriptObjects;
@property (nonatomic, strong) iCarousel *carousel;
@property (nonatomic, strong) UIImageView *background;
@property (nonatomic, strong) UIButton *sendButton;
@property (nonatomic, strong) UILabel *scriptTitleLabel;
@property (nonatomic, strong) NWEffectDrawer *effectDrawer;

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
    if (!self.managedObjectContext) {
        [self useDocument];
    } else {
        [self refresh];
    }
    self.tabBarController.navigationItem.rightBarButtonItem = self.addButton;
    [self.carousel reloadData];
}

- (void)viewDidAppear:(BOOL)animated {
    [self updateView];
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
    self.tabBarController.navigationItem.rightBarButtonItem = nil;
    NSError *error;
    if (self.managedObjectContext && ![self.managedObjectContext save:&error]) {
        NSLog(@"Save failed");
    }
    [super viewWillDisappear:animated];
}

- (void)updateView {
    self.scriptTitleLabel.text = ((Script *)[self.scriptObjects objectAtIndex:self.carousel.currentItemIndex]).title;
}

- (void)fixLocations {
    if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
        [self.tabBarController.tabBar setHidden:NO];
        self.sendButton.frame = CGRectMake(0, self.view.frame.size.height - 100, self.view.frame.size.width, 44);
        self.scriptTitleLabel.frame = CGRectMake(20, 60, self.view.bounds.size.width - 40, 40);
        self.carousel.frame = CGRectMake(0, 80, self.view.bounds.size.width, self.view.bounds.size.height - 160);
    }
	else {
        [self.tabBarController.tabBar setHidden:YES];
        self.sendButton.frame = CGRectMake(0, self.view.frame.size.height - 44, self.view.frame.size.width, 44);
        self.scriptTitleLabel.frame = CGRectMake(20, 45, self.view.bounds.size.width - 40, 40);
        self.carousel.frame = CGRectMake(0, 100, self.view.bounds.size.width, self.view.bounds.size.height - 160);
    }
    self.background.frame = self.view.bounds;
}

- (void)setup {
    self.view.superview.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    
    self.background = [[UIImageView alloc] initWithFrame:self.view.frame];
    self.background.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    self.background.contentMode = UIViewContentModeScaleToFill;
    self.background.userInteractionEnabled = NO;
    self.background.opaque = YES;
    [self.view addSubview:self.background];
    
    self.sendButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[self.sendButton setTitle:NSLocalizedStringFromTable(@"ScriptVCSendBtnKey", @"ViewControllerLocalization", @"") forState:UIControlStateNormal];
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
    
    self.scriptTitleLabel = [[UILabel alloc] initWithFrame:CGRectZero];
    self.scriptTitleLabel.textAlignment = NSTextAlignmentCenter;
    [self.view addSubview:self.scriptTitleLabel];
    
    self.addButton = [[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(addBarButtonPressed:)];
}

#pragma mark - CORE DATA STUFF

- (void)useDocument {
    NSURL *url = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
    url = [url URLByAppendingPathComponent:@"ScriptDocument"];
    UIManagedDocument *document = [[UIManagedDocument alloc] initWithFileURL:url];
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:[url path]]) {
        [document saveToURL:url forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success) {
            if (success) {
                self.managedObjectContext = document.managedObjectContext;
                [Script defaultScriptFastColorChangeInContext:self.managedObjectContext];
                [Script defaultScriptSlowColorChangeInContext:self.managedObjectContext];
                [Script defaultScriptRunLightWithColor:[UIColor redColor] timeInterval:100 inContext:self.managedObjectContext];
                [Script defaultScriptRandomColorsInContext:self.managedObjectContext];
                [Script defaultScriptMovingColorsInContext:self.managedObjectContext];
                [Script defaultScriptConzentrationLightInContext:self.managedObjectContext];
                [Script defaultScriptColorCrashWithTimeInterval:100 inContext:self.managedObjectContext];
                NSError *error;
                if (![self.managedObjectContext save:&error]) {
                    NSLog(@"save error");
                }
                [self refresh];
            } else {
                NSLog(@"saveToURL failed");
            }
        }];
    } else if (document.documentState == UIDocumentStateClosed) {
        [document openWithCompletionHandler:^(BOOL success) {
            if (success) {
                self.managedObjectContext = document.managedObjectContext;
                [self refresh];
            }
        }];
    } else {
        self.managedObjectContext = document.managedObjectContext;
    }
}

- (void)refresh {
    if (self.managedObjectContext) {
        NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:[Script entityName]];
        
        NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"title" ascending:YES];
        [request setSortDescriptors:@[sortDescriptor]];
        
        NSError *error;
        self.scriptObjects = [self.managedObjectContext executeFetchRequest:request error:&error];
        if (self.scriptObjects == nil) {
            NSLog(@"ERROR FETCH");
        }
        [self updateView];
        [self.carousel reloadData];
    }
}

#pragma mark - GETTER & SETTER
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
		[self.controlHandle setColorDirect:[UIColor blackColor]];
        Script *currentScript = self.scriptObjects[self.carousel.currentItemIndex];
		[currentScript sendToWCWiflyControl:self.controlHandle];
	});
}

- (NWEffectDrawer *)effectDrawer {
    if (_effectDrawer == nil) {
        _effectDrawer = [[NWEffectDrawer alloc] init];
        _effectDrawer.delegate = self;
    }
    return _effectDrawer;
}

#pragma mark - iCarouselDatasource
- (NSUInteger)numberOfItemsInCarousel:(iCarousel *)carousel {
    return self.scriptObjects.count;
}

- (UIView *)carousel:(iCarousel *)carousel viewForItemAtIndex:(NSUInteger)index reusingView:(UIView *)view {
    if (view == nil || ![view isKindOfClass:[UIImageView class]])
    {
        view = [[NWRenderableScriptImageView alloc] initWithFrame:CGRectMake(0, 0, 200, 200)];
        view.contentMode = UIViewContentModeCenter;
        view.backgroundColor = [UIColor grayColor];
        view.tag = index + 1;
    }
    
    if ([view isKindOfClass:[UIImageView class]]) {
        
        Script *currentScript = [self.scriptObjects objectAtIndex:index];
        
        if (currentScript.snapshot) {
            ((NWRenderableScriptImageView *)view).image = currentScript.snapshot;
        } else {
            ((NWRenderableScriptImageView *)view).showActivityIndicator = YES;
            ((NWRenderableScriptImageView *)view).image = currentScript.outdatedSnapshot;
            double delayInSeconds = 0.01;
            dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
            dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
                [self.effectDrawer drawScript:currentScript];
            });
        }
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
    [self updateView];
}

- (void)tapOnScriptObjectControl:(UITapGestureRecognizer *)gesture {
    if ([self.effectDrawer effectIsDrawing:nil]) {
        return;
    }
    if (self.isDeletionModeActive) {
        self.isDeletionModeActive = NO;
    } else {
        double delayInSeconds = 0.1;
        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
        dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
            [self performSegueWithIdentifier:@"edit:" sender:self];
        });
    }
}

- (void)longPressOnScriptObjectControl:(UILongPressGestureRecognizer *)gesture {
    if ([self.effectDrawer effectIsDrawing:nil]) {
        return;
    }
    self.isDeletionModeActive = YES;
}

- (void)swipeUpOnScriptObjectControl:(UISwipeGestureRecognizer *)gesture {
    if ([self.effectDrawer effectIsDrawing:nil]) {
        return;
    }
    if (self.isDeletionModeActive && self.scriptObjects.count > 1) {
        NSUInteger indexOfObjectToRemove = self.carousel.currentItemIndex;
        [self.carousel removeItemAtIndex:indexOfObjectToRemove animated:YES];
        [self.managedObjectContext deleteObject:[self.scriptObjects objectAtIndex:indexOfObjectToRemove]];
        NSError *error;
        if (self.managedObjectContext && ![self.managedObjectContext save:&error]) {
            NSLog(@"Save failed: %@", error.helpAnchor);
        }
        NSMutableArray *mutScripts = [self.scriptObjects mutableCopy];
        [mutScripts removeObjectAtIndex:indexOfObjectToRemove];
        self.scriptObjects = mutScripts;
        [self updateView];
    } else if (!self.isDeletionModeActive) {
		[self shareScript];
	}
    self.isDeletionModeActive = NO;
}

- (IBAction)addBarButtonPressed:(UIBarButtonItem *)sender {
    Script *script = [Script emptyScriptInContext:self.managedObjectContext];
    NSError *error;
    if (self.managedObjectContext && ![self.managedObjectContext save:&error]) {
        NSLog(@"Save failed: %@", error.helpAnchor);
    }
    NSMutableArray *mutScripts = [self.scriptObjects mutableCopy];
    [mutScripts addObject:script];
    self.scriptObjects = mutScripts;
    [self.carousel insertItemAtIndex:(self.scriptObjects.count - 1) animated:YES];
    [self.carousel scrollToItemAtIndex:(self.scriptObjects.count - 1) animated:YES];
    [self updateView];
    double delayInSeconds = 0.8;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        [self performSegueWithIdentifier:@"edit:" sender:self];
    });
}

- (void)shareScript {
	MFMailComposeViewController *picker = [[MFMailComposeViewController alloc] init];
    picker.mailComposeDelegate = self;
    [picker setSubject:@"Check out this script!"];
	
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *basePath = ([paths count] > 0) ?[paths objectAtIndex:0] : nil;
	Script *script = self.scriptObjects[[self.carousel currentItemIndex]];
	NSString *filePath = [script serializeToPath:basePath];
	
	NSString *fileName = [filePath componentsSeparatedByString:@"/"].lastObject;
	 
	NSData *myData = [NSData dataWithContentsOfFile:filePath];
    [picker addAttachmentData:myData mimeType:@"text/wylightScript" fileName:fileName];
	
    // Fill out the email body text
    NSString *emailBody = @"My cool WyLight script is attached";
    [picker setMessageBody:emailBody isHTML:NO];
    [self presentModalViewController:picker animated:YES];
}

- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error
{
    // Notifies users about errors associated with the interface
    switch (result)
    {
        case MFMailComposeResultCancelled:
            NSLog(@"Result: canceled");
            break;
        case MFMailComposeResultSaved:
            NSLog(@"Result: saved");
            break;
        case MFMailComposeResultSent:
            NSLog(@"Result: sent");
            break;
        case MFMailComposeResultFailed:
            NSLog(@"Result: failed");
            break;
        default:
            NSLog(@"Result: not sent");
            break;
    }
    [self dismissModalViewControllerAnimated:YES];
}

#pragma mark - EffectDrawerDelegate
- (void)NWEffectDrawer:(NWEffectDrawer *)drawer finishedDrawing:(id)effect {
    if ([effect isKindOfClass:[Script class]]) {
        NSUInteger index = [self.scriptObjects indexOfObject:effect];
        UIView *view = [self.carousel viewWithTag:index + 1];
        if ([view isKindOfClass:[NWRenderableScriptImageView class]]) {
            ((NWRenderableScriptImageView *)view).image = ((Script *)effect).snapshot;
            ((NWRenderableScriptImageView *)view).showActivityIndicator = NO;
        }
    }
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
