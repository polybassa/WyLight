//
//  NWScriptViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptViewController.h"
#import "NWScript.h"
#import "NWScriptView.h"
#import "NWComplexScriptCommandObject.h"
#import "NWComplexScriptCommandEditorViewController.h"
#import "NWScriptCellView.h"
#import "NWAddScriptObjectView.h"
#import "WCWiflyControlWrapper.h"
#import "TouchAndHoldButton.h"

@interface NWScriptViewController () <NWScriptViewDataSource, NWScriptCellViewDelegate, UIScrollViewDelegate>

@property (strong, nonatomic) NWScriptView *scriptView;
@property (strong, nonatomic) TouchAndHoldButton *zoomInButton;
@property (strong, nonatomic) TouchAndHoldButton *zoomOutButton;
@property (nonatomic) BOOL isDeletionModeActive;
@property (nonatomic) CGFloat timeScaleFactor;
@property (nonatomic) NSUInteger indexForObjectToEdit;
@property (nonatomic, strong) UIButton *sendButton;

@end

@implementation NWScriptViewController

#pragma mark - GETTER + SETTER

- (void)setScript:(NWScript *)script {
    _script = script;
    CGFloat availableWidth = self.view.frame.size.width;
    CGFloat scriptDuration = script.totalDurationInTmms.floatValue;
    
    if (availableWidth != 0 && scriptDuration != 0) {
        self.timeScaleFactor = availableWidth / scriptDuration;
    }
    
    [self.scriptView reloadData];
}

- (void)setTimeScaleFactor:(CGFloat)timeScaleFactor {
	_timeScaleFactor = timeScaleFactor;
	for (NWScriptCellView *cell in self.scriptView.subviews) {
		if ([cell isKindOfClass:[NWScriptCellView class]]) {
			cell.timeInfoView.timeScaleFactor = _timeScaleFactor;
		}
	}
	[self.scriptView fixLocationsOfSubviews];
}

- (void)setIsDeletionModeActive:(BOOL)isDeletionModeActive {
	_isDeletionModeActive = isDeletionModeActive;
	if (isDeletionModeActive) {
		for (UIView *control in self.scriptView.subviews) {
			if ([control isKindOfClass:[NWScriptCellView class]]) {
				NWScriptCellView *cell = (NWScriptCellView *)control;
				[UIView animateWithDuration:0.4 animations:^{
					cell.scriptObjectView.quivering = YES;
					cell.scriptObjectView.downscale = YES;
				}];
			}
			if ([control isKindOfClass:[NWAddScriptObjectView class]]) {
				((NWAddScriptObjectView *)control).button.enabled = NO;
			}
		}
	} else {
		for (UIView *control in self.scriptView.subviews) {
			if ([control isKindOfClass:[NWScriptCellView class]]) {
				NWScriptCellView *cell = (NWScriptCellView *)control;
				[UIView animateWithDuration:0.4 animations:^{
					cell.scriptObjectView.quivering = NO;
					cell.scriptObjectView.downscale = NO;
				}];
			}
			if ([control isKindOfClass:[NWAddScriptObjectView class]]) {
				((NWAddScriptObjectView *)control).button.enabled = YES;
			}
		}
	}
}

#pragma mark - SETUP STUFF
#define TIMESCALE_KEY @"WyLightRemote.NWScriptViewController.timescalefactor"
#define DELETE_USER_INFO_KEY @"WyLightRemote.NWScriptViewController.showUserInfo"

- (void)fixLocations {
	if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
		[self.tabBarController.tabBar setHidden:NO];

		//script view
		self.scriptView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height - 64);
		
		self.sendButton.frame = CGRectMake(45, self.view.frame.size.height - 44, self.view.frame.size.width - 88, 44);
		
		self.zoomOutButton.frame = CGRectMake(0, self.view.frame.size.height - 44, 44, 44);
		
		self.zoomInButton.frame = CGRectMake(self.view.frame.size.width - 44, self.view.frame.size.height - 44, 44, 44);
	}
	else {
		[self.tabBarController.tabBar setHidden:YES];

		//script view
		self.scriptView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height - 60);
		
		self.sendButton.frame = CGRectMake(45, self.view.frame.size.height - 44, self.view.frame.size.width - 88, 44);
		
		self.zoomOutButton.frame = CGRectMake(0, self.view.frame.size.height - 44, 44, 44);
		
		self.zoomInButton.frame = CGRectMake(self.view.frame.size.width - 44, self.view.frame.size.height - 44, 44, 44);

	}
}

- (void)setup {	
	self.view.superview.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;

	//user data
	self.timeScaleFactor = [[NSUserDefaults standardUserDefaults] floatForKey:TIMESCALE_KEY];
	if (self.timeScaleFactor == 0.0) {
		self.timeScaleFactor = 1.0;
	}
	//script view
	self.scriptView = [[NWScriptView alloc] initWithFrame:CGRectZero];
	self.scriptView.dataSource = self;
	self.scriptView.backgroundColor = [UIColor clearColor];
	self.scriptView.delegate = self;
	[self.view addSubview:self.scriptView];

	self.sendButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[self.sendButton setTitle:@"SEND" forState:UIControlStateNormal];
	[self.sendButton addTarget:self action:@selector(sendScript) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:self.sendButton];

	self.zoomInButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
	[self.zoomInButton setImage:[UIImage imageNamed:@"Zoom_In_Icon"] forState:UIControlStateNormal];
	[self.zoomInButton addTarget:self action:@selector(zoomInButtonPressed) forTouchAndHoldControlEventWithTimeInterval:0.3];
	[self.zoomInButton addTarget:self action:@selector(zoomInButtonPressed) forControlEvents:UIControlEventTouchDown];
	[self.view addSubview:self.zoomInButton];
	
	self.zoomOutButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
	[self.zoomOutButton setImage:[UIImage imageNamed:@"Zoom_Out_Icon"] forState:UIControlStateNormal];
	[self.zoomOutButton addTarget:self action:@selector(zoomOutButtonPressed) forTouchAndHoldControlEventWithTimeInterval:0.3];
	[self.zoomOutButton addTarget:self action:@selector(zoomOutButtonPressed) forControlEvents:UIControlEventTouchDown];
	[self.view addSubview:self.zoomOutButton];
}

- (void)saveUserData {
	[[NSUserDefaults standardUserDefaults] setFloat:self.timeScaleFactor forKey:TIMESCALE_KEY];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	[self setup];
}

- (void)viewWillAppear:(BOOL)animated {
	[self.scriptView reloadData];
	[super viewWillAppear:animated];
}

- (void)viewWillLayoutSubviews {
	[super viewWillLayoutSubviews];
	[self fixLocations];
}

#pragma mark - GESTURE RECOGNIZER CALLBACKS
- (void)setObjectForEdit:(UITapGestureRecognizer *)gesture {
	if (self.isDeletionModeActive) {
		if ([gesture.view isKindOfClass:[NWScriptCellView class]]) {
			self.isDeletionModeActive = NO;
		}
	} else {
		self.indexForObjectToEdit = gesture.view.tag;
		[self performSegueWithIdentifier:@"edit:" sender:self];
	}
}

- (void)activateDeletionMode:(UILongPressGestureRecognizer *)gesture {
    if (gesture.state == UIGestureRecognizerStateBegan)
    {
		if ([gesture.view isKindOfClass:[NWScriptCellView class]] && self.script.scriptArray.count > 1) {
			self.isDeletionModeActive = YES;
			BOOL showDeletionModeInfo = ![[NSUserDefaults standardUserDefaults] boolForKey:DELETE_USER_INFO_KEY];
			if (showDeletionModeInfo) {
				[[NSUserDefaults standardUserDefaults] setBool:YES forKey:DELETE_USER_INFO_KEY];
				UIAlertView *infoView = [[UIAlertView alloc]initWithTitle:@"Swipe up" message:@"to delete an object" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
				[infoView show];
			}
		}
    }
}

- (void)swipeUpOnScriptObject:(UISwipeGestureRecognizer *)gesture {
	if (gesture.state == UIGestureRecognizerStateEnded && self.isDeletionModeActive) {
		[self deleteScriptObject:gesture.view];
	}
	
}

#pragma mark - BUTTON CALLBACKS

#define ZOOM_IN_STEP 1.1
#define ZOOM_OUT_STEP 0.9

- (void)zoomInButtonPressed {
	[UIView animateWithDuration:0.3 animations:^{
		self.timeScaleFactor *= ZOOM_IN_STEP;
	}];
}

- (void)zoomOutButtonPressed {
	[UIView animateWithDuration:0.3 animations:^{
		self.timeScaleFactor *= ZOOM_OUT_STEP;
	}];
}

- (void)deleteScriptObject:(UIView *)object {
	if ([object isKindOfClass:[NWScriptCellView class]]) {
		if (self.script.scriptArray.count <= 1) {
			self.isDeletionModeActive = NO;
			return;
		}
		NWScriptCellView *cellViewToDelete = (NWScriptCellView *)object;
		
		//Farbübergang zu den neuen Farben
		NWComplexScriptCommandObject *commandToDelete = [self.script.scriptArray objectAtIndex:cellViewToDelete.tag];
		NWScriptCellView *nextCell;
		for (NWScriptCellView *cell in self.scriptView.subviews) {
			if ([cell isKindOfClass:[NWScriptCellView class]]) {
				if (cell.tag == cellViewToDelete.tag + 1) {
					nextCell = cell;
					break;
				}
			}
		}
		NSMutableArray *prevEndColors = [commandToDelete.prev.colors mutableCopy];
		if (!prevEndColors) {
			prevEndColors = [[NSMutableArray alloc] init];
			
			for (unsigned int i = 0; i < nextCell.scriptObjectView.endColors.count; i++) {
				[prevEndColors addObject:[UIColor blackColor]];
			}
		}
		
		if (nextCell) {
			[nextCell.scriptObjectView setColorsAnimatedWithDuration:1.0 startColors:prevEndColors endColor:nextCell.scriptObjectView.endColors];
		}
		//ausblenden
		[UIView animateWithDuration:0.4 delay:0.0 options:UIViewAnimationOptionCurveEaseIn animations:^{
			cellViewToDelete.alpha = 0.0;
		} completion:^(BOOL finished) {
			
			//tags anpassen um danach die Positionen richtig zu berechnen
			[self.script removeObjectAtIndex:cellViewToDelete.tag];
			[cellViewToDelete removeFromSuperview];
			for (NWScriptObjectControl *ctrl in self.scriptView.subviews) {
				if ([ctrl isKindOfClass:[NWScriptCellView class]]) {
					if (ctrl.tag > cellViewToDelete.tag) {
						ctrl.tag--;
					}
				}
			}
			//Positionen anpassen
			[UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
				[self.scriptView fixLocationsOfSubviews];
			} completion:^(BOOL finished) {
			//Farben nachladen
				[self.scriptView reloadData];
			}];
		}];
	}
}

- (void)addScriptCommand:(UIButton *)sender {

	//insert new command in model
	NWComplexScriptCommandObject *commandToAdd = [[self.script.scriptArray lastObject] copy];
	[self.script addObject:commandToAdd];
	
	//get last scriptviewobject, it's the addButton
	NWAddScriptObjectView *addButtonView;
	for (UIView *view in self.scriptView.subviews) {
		if ([view isKindOfClass:[NWAddScriptObjectView class]]) {
			addButtonView = (NWAddScriptObjectView *)view;
		}
	}
	
	//get new view for the new command
	UIView *viewToAdd = [self scriptView:self.scriptView cellViewForIndex:addButtonView.tag];
	viewToAdd.alpha = 0.0;
	viewToAdd.frame = CGRectMake(
								 addButtonView.frame.origin.x,
								 addButtonView.frame.origin.y,
								 [self scriptView:self.scriptView widthOfObjectAtIndex:viewToAdd.tag],
								 addButtonView.frame.size.height);

	[self.scriptView addSubview:viewToAdd];
		
	//change position of the addButton via tag;
	CGRect newFrame = CGRectMake(
								 addButtonView.frame.origin.x + commandToAdd.duration * self.timeScaleFactor + self.scriptView.scriptObjectSpacing,
								 addButtonView.frame.origin.y,
								 addButtonView.frame.size.width,
								 addButtonView.frame.size.height);
	
	[self.scriptView scrollRectToVisible:newFrame animated:YES];
	//Positionen anpassen
	[UIView animateWithDuration:0.3 delay:0.0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
		addButtonView.frame = newFrame;
	} completion:^(BOOL finished) {
		[self.scriptView fixLocationsOfSubviews];
		//Farben nachladen
		[UIView animateWithDuration:0.4 delay:0.0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
			viewToAdd.alpha = 1.0;
		} completion:^(BOOL finished) {
			[self.scriptView reloadData];
		}];
	}];
}

- (void)sendScript {
	dispatch_queue_t sendScriptQueue = dispatch_queue_create("sendScriptQueue", NULL);
	dispatch_async(sendScriptQueue, ^{
		[self.controlHandle clearScript];
		[self.controlHandle setColorDirect:[UIColor blackColor]];
		[self.controlHandle loopOn];
		for (NWComplexScriptCommandObject* command in self.script.scriptArray) {
			[command sendToWCWiflyControl:self.controlHandle];
			[NSThread sleepForTimeInterval:0.1];
		}
		[self.controlHandle loopOffAfterNumberOfRepeats:0];
	});
}

#pragma mark - SCRIPT VIEW DATASOURCE
- (UIView *)scriptView:(NWScriptView *)view cellViewForIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		if (index < self.script.scriptArray.count) {
			NWScriptCellView *tempView = [[NWScriptCellView alloc]initWithFrame:CGRectZero];
			tempView.scriptObjectView.endColors = ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).colors;
			if (((NWComplexScriptCommandObject *)self.script.scriptArray[index]).prev) {
				tempView.scriptObjectView.startColors = ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).prev.colors;
			}
			if (index == 0) {
				tempView.scriptObjectView.startColors = ((NWComplexScriptCommandObject *)self.script.scriptArray.lastObject).colors;
			}
			tempView.scriptObjectView.cornerRadius = 5;
			tempView.delegate = self;
			tempView.tag = index;
			tempView.scriptObjectView.quivering = self.isDeletionModeActive;
			tempView.scriptObjectView.downscale = self.isDeletionModeActive;
			tempView.timeInfoView.timeScaleFactor = self.timeScaleFactor;
						
			UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(setObjectForEdit:)];
			tap.numberOfTapsRequired = 1;
			[tempView addGestureRecognizer:tap];
			
			UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:tempView action:@selector(pinchWidth:)];
			[tempView addGestureRecognizer:pinch];
			
			UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(activateDeletionMode:)];
			[tempView addGestureRecognizer:longPress];
			
			UISwipeGestureRecognizer *swipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeUpOnScriptObject:)];
			swipe.direction = UISwipeGestureRecognizerDirectionUp;
			[tempView addGestureRecognizer:swipe];
			return tempView;
		} else {
			NWAddScriptObjectView *tempView = [[NWAddScriptObjectView alloc]initWithFrame:CGRectZero];
			[tempView.button addTarget:self action:@selector(addScriptCommand:) forControlEvents:UIControlEventTouchUpInside];
			
			tempView.scriptObjectView.cornerRadius = 5;
			tempView.tag = index;
			tempView.button.enabled = !self.isDeletionModeActive;
			
			return tempView;
		}
	}
	return nil;

}

- (CGFloat)scriptView:(NWScriptView *)view widthOfObjectAtIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		if (index < self.script.scriptArray.count) {
			return ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).duration * self.timeScaleFactor;
		}
	}
	return 0;
}

- (NSUInteger)numberOfObjectsInScriptView:(NWScriptView *)view {
	if (view == self.scriptView) {
		return self.script.scriptArray.count + 1;
	}
	return 0;
}

#pragma mark - SCRIPTOBJECTCONTROL DELEGATE
- (void)scriptCellView:(NWScriptCellView *)view changedWidthTo:(CGFloat)width deltaOfChange:(CGFloat)delta {
	for (NWScriptCellView *subview in self.scriptView.subviews) {
		if ([subview isKindOfClass:[NWScriptCellView class]]) {
			if (subview.tag <= view.tag) {
				continue;
			}
			CGFloat x = subview.frame.origin.x + delta;
			CGFloat y = subview.frame.origin.y;
			CGFloat width = subview.frame.size.width;
			CGFloat height = subview.frame.size.height;
			
			subview.frame = CGRectMake(x, y, width, height);
		}
		if ([subview isKindOfClass:[NWAddScriptObjectView class]]) {
			if (subview.tag <= view.tag) {
				continue;
			}
			CGFloat x = subview.frame.origin.x + delta;
			CGFloat y = subview.frame.origin.y;
			CGFloat width = subview.frame.size.width;
			CGFloat height = subview.frame.size.height;
			
			subview.frame = CGRectMake(x, y, width, height);
		}
	}
}

- (void)scriptCellView:(NWScriptCellView *)view finishedWidthChange:(CGFloat)width {
	NSUInteger index = view.tag;
	uint16_t duration = width / self.timeScaleFactor;
	((NWComplexScriptCommandObject *)self.script.scriptArray[index]).duration = duration;
	[self.scriptView fixLocationsOfSubviews];
}

#pragma mark - SEGUE
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.identifier isEqualToString:@"edit:"])
	{
		if ([segue.destinationViewController respondsToSelector:@selector(setControlHandle:)])
		{
			[segue.destinationViewController performSelector:@selector(setControlHandle:) withObject:self.controlHandle];
		}
		
		if ([segue.destinationViewController isKindOfClass:[NWComplexScriptCommandEditorViewController class]]) {
			NWComplexScriptCommandEditorViewController *ctrl = (NWComplexScriptCommandEditorViewController *)segue.destinationViewController;
			ctrl.command = [self.script.scriptArray objectAtIndex:self.indexForObjectToEdit];
		}
	}
}


@end
