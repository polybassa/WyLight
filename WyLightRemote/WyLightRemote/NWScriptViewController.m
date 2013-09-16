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
#import "NWScriptObjectControl.h"
#import "NWEditComplexScriptObjectViewController.h"
#import "NWTimeInfoView.h"
#import "NWAddScriptObjectView.h"
#import "WCWiflyControlWrapper.h"

@interface NWScriptViewController () <UIGestureRecognizerDelegate, NWScriptViewDataSource, NWScriptObjectControlDelegate, UIScrollViewDelegate>

@property (strong, nonatomic) NWScript *script;
@property (strong, nonatomic) NWScriptView *scriptView;
@property (nonatomic) BOOL isDeletionModeActive;
@property (nonatomic) CGFloat timeScaleFactor;
@property (nonatomic) NSUInteger indexForObjectToEdit;
@property (nonatomic, strong) UIButton *sendButton;

@end

@implementation NWScriptViewController

#pragma mark - GETTER + SETTER
- (NWScript *)script {
	if (_script == nil) {
		_script = [[NWScript alloc]init];
	}
	return _script;
}

- (void)setIsDeletionModeActive:(BOOL)isDeletionModeActive {
	_isDeletionModeActive = isDeletionModeActive;
	if (isDeletionModeActive) {
		for (UIView *control in self.scriptView.subviews) {
			if ([control isKindOfClass:[NWScriptObjectControl class]]) {
				((NWScriptObjectControl *)control).showDeleteButton = YES;
			}
			if ([control isKindOfClass:[NWAddScriptObjectView class]]) {
				((NWAddScriptObjectView *)control).button.enabled = NO;
			}
		}
	} else {
		for (UIView *control in self.scriptView.subviews) {
			if ([control isKindOfClass:[NWScriptObjectControl class]]) {
				((NWScriptObjectControl *)control).showDeleteButton = NO;
			}
			if ([control isKindOfClass:[NWAddScriptObjectView class]]) {
				((NWAddScriptObjectView *)control).button.enabled = YES;
			}
		}
	}
}

#pragma mark - HANDLE ROTATION

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
	[super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
	
	if (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || toInterfaceOrientation == UIInterfaceOrientationLandscapeRight) {
		[self.tabBarController.tabBar setHidden:YES];
	}
	else {
		[self.tabBarController.tabBar setHidden:NO];
	}
}

#pragma mark - SETUP STUFF
#define SCRIPT_KEY @"script"
#define TIMESCALE_KEY @"timescalefactor"
- (void)fixLocations {
	if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
		
		//script view
		self.scriptView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height - 60);
		
		self.sendButton.frame = CGRectMake(0, self.view.frame.size.height - 40, self.view.frame.size.width, 40);
	}
	else {
		CGRect biggerFrame = self.tabBarController.view.frame;
		biggerFrame.size.height += self.tabBarController.tabBar.frame.size.height;
		self.tabBarController.view.frame = biggerFrame ;
		
		self.scriptView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height - 60);
		
		self.sendButton.frame = CGRectMake(0, self.view.frame.size.height - 40, self.view.frame.size.width, 40);

	}
	[self.scriptView fixLocationsOfSubviews];
}

- (void)setup {
	//user data
	self.timeScaleFactor = [[NSUserDefaults standardUserDefaults] floatForKey:TIMESCALE_KEY];
	if (self.timeScaleFactor == 0.0) {
		self.timeScaleFactor = 1.0;
	}
	
	NSData *data = [[NSUserDefaults standardUserDefaults] objectForKey:SCRIPT_KEY];
	if (data) {
		self.script = [NSKeyedUnarchiver unarchiveObjectWithData:data];
	}
	
	//script view
	self.scriptView = [[NWScriptView alloc] initWithFrame:CGRectZero];
	self.scriptView.dataSource = self;
	self.scriptView.backgroundColor = [UIColor clearColor];
	self.scriptView.delegate = self;
	[self.view addSubview:self.scriptView];

	UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchOnScriptView:)];
	[self.scriptView addGestureRecognizer:pinch];
	
	self.sendButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[self.sendButton setTitle:@"SEND" forState:UIControlStateNormal];
	[self.sendButton addTarget:self action:@selector(sendScript) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:self.sendButton];
}

- (void)saveUserData {
	NSData *data = [NSKeyedArchiver archivedDataWithRootObject:self.script];
	if (data) {
		[[NSUserDefaults standardUserDefaults] setObject:data forKey:SCRIPT_KEY];
	}
	
	[[NSUserDefaults standardUserDefaults] setFloat:self.timeScaleFactor forKey:TIMESCALE_KEY];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	[self setup];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self.scriptView reloadData];
}

- (void)viewWillDisappear:(BOOL)animated {
	[self saveUserData];
	[super viewWillDisappear:animated];
}

- (void)viewWillLayoutSubviews {
	[super viewWillLayoutSubviews];
	[self fixLocations];
}

#pragma mark - GESTURE RECOGNIZER CALLBACKS
- (void)setObjectForEdit:(UITapGestureRecognizer *)gesture {
	if (self.isDeletionModeActive) {
		if ([gesture.view isKindOfClass:[NWScriptObjectControl class]]) {
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
		if ([gesture.view isKindOfClass:[NWScriptObjectControl class]] && self.script.scriptArray.count > 1) {
			self.isDeletionModeActive = YES;
		}
    }
}

- (void)pinchOnScriptView:(UIPinchGestureRecognizer *)gesture {
	if (gesture.state == UIGestureRecognizerStateChanged) {
		self.timeScaleFactor *= gesture.scale;
		self.scriptView.timeScaleFactor = self.timeScaleFactor;
		gesture.scale = 1;
	}
}

#pragma mark - BUTTON CALLBACKS
- (void)deleteScriptObject:(UIButton *)sender {
	if ([sender.superview isKindOfClass:[NWScriptObjectControl class]]) {
		if (self.script.scriptArray.count <= 1) {
			self.isDeletionModeActive = NO;
			return;
		}
		NWScriptObjectControl *objectToDelete = (NWScriptObjectControl *)sender.superview;
		
		NWTimeInfoView *timeViewToDelete;
		for (UIView *subview in self.scriptView.subviews) {
			if ((subview.tag == objectToDelete.tag) && [subview isKindOfClass:[NWTimeInfoView class]]) {
				timeViewToDelete = (NWTimeInfoView *)subview;
			}
		}
		//ausblenden
		[UIView animateWithDuration:0.5 delay:0.0 options:UIViewAnimationOptionCurveEaseIn animations:^{
			
			objectToDelete.alpha = 0.0;
			objectToDelete.deleteButton.alpha = 0.0;
			timeViewToDelete.alpha = 0.0;
			
		} completion:^(BOOL finished) {
			
			//tags anpassen um danach die Positionen richtig zu berechnen
			[self.script removeObjectAtIndex:objectToDelete.tag];
			[objectToDelete removeFromSuperview];
			[timeViewToDelete removeFromSuperview];
			for (NWScriptObjectControl *ctrl in self.scriptView.subviews) {
				if ([ctrl isKindOfClass:[NWScriptObjectControl class]]) {
					if (ctrl.tag > objectToDelete.tag) {
						ctrl.tag--;
					}
				}
				if ([ctrl isKindOfClass:[NWTimeInfoView class]]) {
					if (ctrl.tag > objectToDelete.tag) {
						ctrl.tag--;
					}
				}
			}
			//Positionen anpassen
			[UIView animateWithDuration:0.5 delay:0.0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
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
	UIView *viewToAdd = [self scriptView:self.scriptView objectViewForIndex:addButtonView.tag];
	viewToAdd.alpha = 0.0;
	[self.scriptView addSubview:viewToAdd];
	
	UIView *timeInfoViewToAdd = [self scriptView:self.scriptView timeInfoViewForIndex:addButtonView.tag];
	[self.scriptView addSubview:timeInfoViewToAdd];
	
	//change position of the addButton via tag;
	CGRect newFrame = CGRectMake(
								 addButtonView.frame.origin.x + commandToAdd.duration * self.timeScaleFactor + self.scriptView.scriptObjectSpacing,
								 addButtonView.frame.origin.y,
								 addButtonView.frame.size.width,
								 addButtonView.frame.size.height);
	
	[self.scriptView scrollRectToVisible:newFrame animated:YES];
	//Positionen anpassen
	[UIView animateWithDuration:0.5 delay:0.0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
		addButtonView.frame = newFrame;
	} completion:^(BOOL finished) {
		[self.scriptView fixLocationsOfSubviews];
		//Farben nachladen
		[UIView animateWithDuration:0.5 delay:0.0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
			viewToAdd.alpha = 1.0;
			[self.scriptView fixLocationOfTimeInfoView:viewToAdd.tag];
		} completion:^(BOOL finished) {
			[self.scriptView reloadData];
		}];
	}];

	
}

- (void)sendScript {
	dispatch_queue_t sendScriptQueue = dispatch_queue_create("sendScriptQueue", NULL);
	dispatch_async(sendScriptQueue, ^{
		[self.controlHandle clearScript];
		[self.controlHandle loopOn];
		for (NWComplexScriptCommandObject* command in self.script.scriptArray) {
			[command sendToWCWiflyControl:self.controlHandle];
			[NSThread sleepForTimeInterval:0.1];
		}
		[self.controlHandle loopOffAfterNumberOfRepeats:0];
	});
}

#pragma mark - SCRIPT VIEW DATASOURCE
- (CGFloat)scriptView:(NWScriptView *)view widthOfObjectAtIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		if (index < self.script.scriptArray.count) {
			return ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).duration * self.timeScaleFactor;
		}
	}
	return 0;
}

- (UIView *)scriptView:(NWScriptView *)view objectViewForIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		if (index < self.script.scriptArray.count) {
			NWScriptObjectControl *tempView = [[NWScriptObjectControl alloc]initWithFrame:CGRectZero];
			tempView.endColors = ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).colors;
			if (((NWComplexScriptCommandObject *)self.script.scriptArray[index]).prev) {
				tempView.startColors = ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).prev.colors;
			}
			tempView.borderWidth = 1;
			tempView.cornerRadius = 5;
			tempView.delegate = self;
			tempView.showDeleteButton = self.isDeletionModeActive;
			tempView.backgroundColor = [UIColor blackColor];
			tempView.tag = index;
			
			UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(setObjectForEdit:)];
			tap.numberOfTapsRequired = 1;
			[tempView addGestureRecognizer:tap];
			
			UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:tempView action:@selector(pinchWidth:)];
			[tempView addGestureRecognizer:pinch];
			
			UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(activateDeletionMode:)];
			[tempView addGestureRecognizer:longPress];
			
			[tempView.deleteButton addTarget:self action:@selector(deleteScriptObject:) forControlEvents:UIControlEventTouchUpInside];
			
			return tempView;
		} else {
			NWAddScriptObjectView *tempView = [[NWAddScriptObjectView alloc]initWithFrame:CGRectZero];
			[tempView.button addTarget:self action:@selector(addScriptCommand:) forControlEvents:UIControlEventTouchUpInside];
			
			tempView.scriptObjectView.borderWidth = 1;
			tempView.scriptObjectView.cornerRadius = 5;
			tempView.scriptObjectView.backgroundColor = [UIColor blackColor];
			tempView.tag = index;
			tempView.button.enabled = !self.isDeletionModeActive;

			return tempView;
		}
	}
	return nil;
}

- (NSUInteger)numberOfObjectsInScriptView:(NWScriptView *)view {
	if (view == self.scriptView) {
		return self.script.scriptArray.count + 1;
	}
	return 0;
}

- (UIView *)scriptView:(NWScriptView *)view timeInfoViewForIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		NWTimeInfoView *timeInfoView = [[NWTimeInfoView alloc] initWithFrame:CGRectZero];
		timeInfoView.timeScaleFactor = self.timeScaleFactor;
		timeInfoView.tag = index;
		return timeInfoView;
	}
	return nil;
}

#pragma mark - SCRIPTOBJECTCONTROL DELEGATE
- (void)scriptObjectView:(NWScriptObjectView *)view changedWidthTo:(CGFloat)width deltaOfChange:(CGFloat)delta {
	[self.scriptView fixLocationOfTimeInfoView:view.tag];
	for (NWScriptObjectView *subview in self.scriptView.subviews) {
		if ([subview isKindOfClass:[NWScriptObjectView class]]) {
			if (subview.tag <= view.tag) {
				continue;
			}
			CGFloat x = subview.frame.origin.x + delta;
			CGFloat y = subview.frame.origin.y;
			CGFloat width = subview.frame.size.width;
			CGFloat height = subview.frame.size.height;
			
			subview.frame = CGRectMake(x, y, width, height);
			[self.scriptView fixLocationOfTimeInfoView:subview.tag];
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

- (void)scriptObjectView:(NWScriptObjectView *)view finishedWidthChange:(CGFloat)width {
	NSUInteger index = view.tag;
	uint16_t duration = width / self.timeScaleFactor;
	((NWComplexScriptCommandObject *)self.script.scriptArray[index]).duration = duration;
	[self.scriptView reloadData];
}

#pragma mark - SEGUE
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.identifier isEqualToString:@"edit:"])
	{
		if ([segue.destinationViewController respondsToSelector:@selector(setControlHandle:)])
		{
			[segue.destinationViewController performSelector:@selector(setControlHandle:) withObject:self.controlHandle];
		}
		
		if ([segue.destinationViewController isKindOfClass:[NWEditComplexScriptObjectViewController class]]) {
			NWEditComplexScriptObjectViewController *ctrl = (NWEditComplexScriptObjectViewController *)segue.destinationViewController;
			ctrl.command = [self.script.scriptArray objectAtIndex:self.indexForObjectToEdit];
		}
	}
}


@end
