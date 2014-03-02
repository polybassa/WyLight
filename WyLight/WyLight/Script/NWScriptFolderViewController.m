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
#import "WCWiflyControlWrapper.h"
#import "NWScriptViewController.h"
#import "Script+defaultScripts.h"
#import "NWRenderableScriptImageView.h"
#import "NWEffectDrawer.h"
#import "Script+serialization.h"
#import <MessageUI/MFMailComposeViewController.h>

@interface NWScriptFolderViewController () <iCarouselDataSource, iCarouselDelegate, NWEffectDrawerDelegate, MFMailComposeViewControllerDelegate>

@property (nonatomic, strong) UIBarButtonItem *addButton;
@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, strong) NSArray *scriptObjects;
@property (nonatomic, strong) iCarousel *carousel;
@property (nonatomic, strong) UIButton *sendButton;
@property (nonatomic, strong) UILabel *scriptTitleLabel;
@property (nonatomic, strong) NWEffectDrawer *effectDrawer;
@property (atomic) BOOL waitingForProgrammaticallySegue;

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
		[self.carousel reloadData];
	}
    self.tabBarController.navigationItem.rightBarButtonItem = self.addButton;
	[self updateTitleLabel];
	self.waitingForProgrammaticallySegue = FALSE;
}

- (void)viewWillDisappear:(BOOL)animated {
    self.tabBarController.navigationItem.rightBarButtonItem = nil;
	[self.managedObjectContext processPendingChanges];
    NSError *error;
    if (self.managedObjectContext && ![self.managedObjectContext save:&error]) {
        NSLog(@"Save failed");
    }
	if (self.managedObjectContext.parentContext && ![self.managedObjectContext.parentContext save:&error]) {
		NSLog(@"Parent Save failed");
	}
    [super viewWillDisappear:animated];
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
}

- (void)setup {
    self.view.superview.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    
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
                [Script defaultScriptFastColorChangeInContext:document.managedObjectContext];
                [Script defaultScriptSlowColorChangeInContext:document.managedObjectContext];
                [Script defaultScriptRunLightWithColor:[UIColor redColor] timeInterval:100 inContext:document.managedObjectContext];
                [Script defaultScriptRandomColorsInContext:document.managedObjectContext];
                [Script defaultScriptMovingColorsInContext:document.managedObjectContext];
                [Script defaultScriptConzentrationLightInContext:document.managedObjectContext];
                [Script defaultScriptColorCrashWithTimeInterval:100 inContext:document.managedObjectContext];
                NSError *error;
                if (![document.managedObjectContext save:&error]) {
                    NSLog(@"save error");
                } else {
					self.managedObjectContext = document.managedObjectContext;
				}
            }
			else {
                NSLog(@"saveToURL failed");
            }
        }];
    } else if (document.documentState == UIDocumentStateClosed) {
        [document openWithCompletionHandler:^(BOOL success) {
            if (success) {
                self.managedObjectContext = document.managedObjectContext;
            }
        }];
    } else {
        self.managedObjectContext = document.managedObjectContext;
    }
}

- (NSArray *)scriptObjects {
	if (self.managedObjectContext) {
		if (_scriptObjects == nil || [self.managedObjectContext hasChanges]) {
			NSError *error;
			if (![self.managedObjectContext save:&error]) {
				NSLog(@"save error");
			}
        	NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:[Script entityName]];
        
        	NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"title" ascending:YES];
        	[request setSortDescriptors:@[sortDescriptor]];
        
        	_scriptObjects = [[NSMutableArray alloc] initWithArray:[self.managedObjectContext executeFetchRequest:request error:&error]];
        	return _scriptObjects;
		}
		return _scriptObjects;
	}
	else {
		return nil;
	}
}

#pragma mark - GETTER & SETTER

- (void)sendScript {
	dispatch_queue_t sendScriptQueue = dispatch_get_main_queue();//dispatch_queue_create("sendScriptQueue", NULL);
	dispatch_async(sendScriptQueue, ^{
		[self.controlHandle clearScript];
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

- (void)setManagedObjectContext:(NSManagedObjectContext *)managedObjectContext {
	_managedObjectContext = managedObjectContext;
	dispatch_async(dispatch_get_main_queue(), ^(void){
		[self.carousel reloadData];
		[self updateTitleLabel];
	});
}

- (void)updateTitleLabel {
	if (self.scriptObjects && self.carousel && self.scriptObjects.count > self.carousel.currentItemIndex) {
		self.scriptTitleLabel.text = ((Script *)[self.scriptObjects objectAtIndex:self.carousel.currentItemIndex]).title;
	}
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
	[self updateTitleLabel];
}

#pragma mark - UIMenuController callback
- (void)delete:(id)sender {
	NSUInteger indexOfObjectToRemove = self.carousel.currentItemIndex;
	[self.carousel removeItemAtIndex:indexOfObjectToRemove animated:YES];
	NSManagedObject *objToRemove = [self.scriptObjects objectAtIndex:indexOfObjectToRemove];
	[self.managedObjectContext deleteObject:objToRemove];
	[self updateTitleLabel];
}
#define WYLIGHT_UTI @"com.WyLightRemote.customUTIHandler.wyscript"
- (void)copy:(id)sender {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *basePath = ([paths count] > 0) ?[paths objectAtIndex:0] : nil;
	Script *script = self.scriptObjects[[self.carousel currentItemIndex]];
	NSString *filePath = [script serializeToPath:basePath];
	NSData *myData = [NSData dataWithContentsOfFile:filePath];
	
	UIPasteboard* pasteboard = [UIPasteboard generalPasteboard];
	[pasteboard setData:myData forPasteboardType:WYLIGHT_UTI];
}

- (void)cut:(id)sender {
	[self copy:sender];
	[self delete:sender];
}

- (void)paste:(id)sender {
	NSData *scriptData = [[UIPasteboard generalPasteboard] dataForPasteboardType:WYLIGHT_UTI];
	Script *script = [Script deserializeScriptFromString:[[NSString alloc] initWithData:scriptData encoding:NSASCIIStringEncoding] inContext:self.managedObjectContext];
	[self.carousel insertItemAtIndex:[self.scriptObjects indexOfObject:script] animated:YES];
	[self.carousel scrollToItemAtIndex:[self.scriptObjects indexOfObject:script] animated:YES];
	[self updateTitleLabel];
}

#pragma mark - Gesture callbacks
- (void)tapOnScriptObjectControl:(UITapGestureRecognizer *)gesture {
    if ([self.effectDrawer effectIsDrawing:nil]) {
        return;
    }
	if (!self.waitingForProgrammaticallySegue) {
		self.waitingForProgrammaticallySegue = TRUE;
		[self performSegueWithIdentifier:@"edit:" sender:self];
	}
}

- (BOOL)canBecomeFirstResponder {
	return YES;
}

- (BOOL)canPerformAction:(SEL)action withSender:(id)sender {
    BOOL retValue = NO;
    if (action == @selector(paste:))
        retValue = [[UIPasteboard generalPasteboard] containsPasteboardTypes:@[WYLIGHT_UTI]];
    else if ( action == @selector(copy:) )
        retValue = YES;
    else if (action == @selector(delete:) || action == @selector(cut:))
		retValue = ![self.effectDrawer effectIsDrawing:nil] && (self.scriptObjects.count > 1);
	else if (action == @selector(shareScript))
		retValue = YES;
	else
        retValue = [super canPerformAction:action withSender:sender];
    return retValue;
}

- (void)longPressOnScriptObjectControl:(UILongPressGestureRecognizer *)gesture {
    if ([self.effectDrawer effectIsDrawing:nil]) {
        return;
    }
	if (gesture.state == UIGestureRecognizerStateBegan) {
        UIMenuController *menuController = [UIMenuController sharedMenuController];
		UIMenuItem *shareItem = [[UIMenuItem alloc] initWithTitle:NSLocalizedStringFromTable(@"ScriptVCShareKey", @"ViewControllerLocalization", @"") action:@selector(shareScript)];
		
		NSAssert([self becomeFirstResponder], @"Sorry, UIMenuController will not work with %@ since it cannot become first responder", self);
        [menuController setMenuItems:[NSArray arrayWithObject:shareItem]];
        [menuController setTargetRect:[self.carousel.currentItemView convertRect:self.carousel.currentItemView.frame toView:self.view] inView:self.view];
        [menuController setMenuVisible:YES animated:YES];
	}
}

- (IBAction)addBarButtonPressed:(UIBarButtonItem *)sender {
	if (!self.waitingForProgrammaticallySegue) {
		self.waitingForProgrammaticallySegue = TRUE;
		NSManagedObjectContext *currentContext = self.managedObjectContext;
		Script *script = [Script emptyScriptInContext:currentContext];
		[self.carousel insertItemAtIndex:[self.scriptObjects indexOfObject:script] animated:YES];
		[self.carousel scrollToItemAtIndex:[self.scriptObjects indexOfObject:script] animated:YES];
		[self updateTitleLabel];
		double delayInSeconds = 0.5;
		dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
		dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
			[self performSegueWithIdentifier:@"edit:" sender:self];
			
		});
	}
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
    [picker addAttachmentData:myData mimeType:@"text/wyscript" fileName:fileName];
	
    // Fill out the email body text
    NSString *emailBody = @"My cool WyLight script is attached";
    [picker setMessageBody:emailBody isHTML:NO];
	[self presentViewController:picker animated:YES completion:NULL];
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
	[self dismissViewControllerAnimated:YES completion:NULL];
}

#pragma mark - EffectDrawerDelegate
- (void)NWEffectDrawer:(NWEffectDrawer *)drawer finishedDrawing:(id)effect {
    if ([effect isKindOfClass:[Script class]]) {
        NSUInteger index = [self.scriptObjects indexOfObject:effect];
        UIView *view = [self.carousel itemViewAtIndex:index];
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
