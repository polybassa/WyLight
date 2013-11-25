//
//  NWScriptViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptViewController.h"
#import "Script.h"
#import "NWScriptView.h"
#import "ComplexEffect.h"
#import "NWComplexScriptCommandEditorViewController.h"
#import "NWScriptCellView.h"
#import "WCWiflyControlWrapper.h"
#import "TouchAndHoldButton.h"
#import "NWEffectDrawer.h"
#import "SimpelEffect.h"

@interface NWScriptViewController () <UITextFieldDelegate, NWScriptViewDataSource, NWScriptCellViewDelegate, UIScrollViewDelegate, NWEffectDrawerDelegate>

@property (strong, nonatomic) NWScriptView *scriptView;
@property (strong, nonatomic) TouchAndHoldButton *zoomInButton;
@property (strong, nonatomic) TouchAndHoldButton *zoomOutButton;
@property (nonatomic) BOOL isDeletionModeActive;
@property (nonatomic) CGFloat timeScaleFactor;
@property (nonatomic) NSUInteger indexForObjectToEdit;
@property (nonatomic, strong) UIButton *sendButton;
@property (weak, nonatomic) IBOutlet UITextField *titelTextField;
@property (nonatomic, strong) UIButton *repeatSwitch;
@property (nonatomic, strong) NWEffectDrawer *effectDrawer;

@end

@implementation NWScriptViewController

#pragma mark - GETTER + SETTER

- (NWEffectDrawer *)effectDrawer {
    if (_effectDrawer == nil) {
        _effectDrawer = [[NWEffectDrawer alloc] init];
        _effectDrawer.delegate = self;
    }
    return _effectDrawer;
}

- (void)resizeScriptToFit {
    if (self.script) {
        CGFloat availableWidth = self.view.frame.size.width - 20;
        CGFloat scriptDuration = self.script.totalDurationInTmms.floatValue;
    
        if (availableWidth != 0 && scriptDuration != 0) {
            self.timeScaleFactor = availableWidth / scriptDuration;
        }
    }
}

- (void)setScript:(Script *)script {
    _script = script;
    [self resizeScriptToFit];
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
					cell.scriptObjectImageView.quivering = YES;
					cell.scriptObjectImageView.downscale = YES;
				}];
			}
        }
	} else {
		for (UIView *control in self.scriptView.subviews) {
			if ([control isKindOfClass:[NWScriptCellView class]]) {
				NWScriptCellView *cell = (NWScriptCellView *)control;
				[UIView animateWithDuration:0.4 animations:^{
					cell.scriptObjectImageView.quivering = NO;
					cell.scriptObjectImageView.downscale = NO;
				}];
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
		
		self.sendButton.frame = CGRectMake(self.view.frame.size.width / 2, self.view.frame.size.height - 44, self.view.frame.size.width / 2 - 44, 44);
        
        self.repeatSwitch.frame = CGRectMake(45, self.view.frame.size.height - 44, self.view.frame.size.width / 2 - 44, 44);
		
		self.zoomOutButton.frame = CGRectMake(0, self.view.frame.size.height - 44, 44, 44);
		
		self.zoomInButton.frame = CGRectMake(self.view.frame.size.width - 44, self.view.frame.size.height - 44, 44, 44);
	}
	else {
		[self.tabBarController.tabBar setHidden:YES];

		//script view
		self.scriptView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height - 60);
		
		self.sendButton.frame = CGRectMake(self.view.frame.size.width / 2, self.view.frame.size.height - 44, self.view.frame.size.width / 2 - 44, 44);
        
         self.repeatSwitch.frame = CGRectMake(45, self.view.frame.size.height - 44, self.view.frame.size.width / 2 - 44, 44);
		
		self.zoomOutButton.frame = CGRectMake(0, self.view.frame.size.height - 44, 44, 44);
		
		self.zoomInButton.frame = CGRectMake(self.view.frame.size.width - 44, self.view.frame.size.height - 44, 44, 44);

	}
    [self resizeScriptToFit];
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
    self.scriptView.insets = UIEdgeInsetsMake(90, 20, 0, 0);
	self.scriptView.dataSource = self;
	self.scriptView.backgroundColor = [UIColor clearColor];
	self.scriptView.delegate = self;
	[self.view addSubview:self.scriptView];

	self.sendButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[self.sendButton setTitle:NSLocalizedStringFromTable(@"ScriptVCSendBtnKey", @"ViewControllerLocalization", @"") forState:UIControlStateNormal];
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
    
    self.repeatSwitch = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [self.repeatSwitch setImage:[UIImage imageNamed:@"Repeat_Icon"] forState:UIControlStateNormal];
    self.repeatSwitch.selected = self.script.repeatsWhenFinished.boolValue;
    [self.repeatSwitch addTarget:self action:@selector(repeatSwitchValueChanged) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:self.repeatSwitch];
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapOnBackground:)];
    [self.view addGestureRecognizer:tap];
}

- (void)saveUserData {
	[[NSUserDefaults standardUserDefaults] setFloat:self.timeScaleFactor forKey:TIMESCALE_KEY];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	[self setup];
}

- (void)viewWillAppear:(BOOL)animated {
    [self resizeScriptToFit];
	[self.scriptView reloadData];
    self.titelTextField.text = self.script.title;
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
		if ([gesture.view isKindOfClass:[NWScriptCellView class]] && self.script.effects.count > 1) {
			self.isDeletionModeActive = YES;
			BOOL showDeletionModeInfo = ![[NSUserDefaults standardUserDefaults] boolForKey:DELETE_USER_INFO_KEY];
			if (showDeletionModeInfo) {
				[[NSUserDefaults standardUserDefaults] setBool:YES forKey:DELETE_USER_INFO_KEY];
				UIAlertView *infoView = [[UIAlertView alloc]initWithTitle:NSLocalizedStringFromTable(@"ScriptVCDeleteInfoKey1", @"ViewControllerLocalization", @"") message:NSLocalizedStringFromTable(@"ScriptVCDeleteInfoKey2", @"ViewControllerLocalization", @"") delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
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
- (IBAction)addBarButtonPressed:(UIBarButtonItem *)sender {
    [self addScriptCommand];
}

- (void)zoomOutButtonPressed {
	[UIView animateWithDuration:0.3 animations:^{
		self.timeScaleFactor *= ZOOM_OUT_STEP;
	}];
}

- (void)deleteScriptObject:(UIView *)object {
	if ([object isKindOfClass:[NWScriptCellView class]]) {
		if (self.script.effects.count <= 1) {
			self.isDeletionModeActive = NO;
			return;
		}
		NWScriptCellView *cellViewToDelete = (NWScriptCellView *)object;
		
		//Farbübergang zu den neuen Farben
		ComplexEffect *commandToDelete = [self.script.effects objectAtIndex:cellViewToDelete.tag];
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
			
			for (unsigned int i = 0; i < commandToDelete.colors.count; i++) {
				[prevEndColors addObject:[UIColor blackColor]];
			}
		}
		
        ComplexEffect *nextEffect;
        if (nextCell) {
            nextEffect = commandToDelete.next;
        } else {
            nextEffect = self.script.effects.firstObject;
        }
        nextEffect.snapshot = nil;
        for (SimpelEffect *subeffect in nextEffect.effects) {
            subeffect.colors = nil;
        }
        
        //ausblenden
		[UIView animateWithDuration:0.4 delay:0.0 options:UIViewAnimationOptionCurveEaseIn animations:^{
			cellViewToDelete.alpha = 0.0;
		} completion:^(BOOL finished) {
			
            //tags anpassen um danach die Positionen richtig zu berechnen
            [self.script.managedObjectContext deleteObject:commandToDelete];
            self.script.snapshot = nil;
            NSError *error;
            if (self.script.managedObjectContext && ![self.script.managedObjectContext save:&error]) {
                NSLog(@"Save failed");
            }
            
			[cellViewToDelete removeFromSuperview];
			for (NWScriptObjectControl *ctrl in self.scriptView.subviews) {
				if ([ctrl isKindOfClass:[NWScriptCellView class]]) {
					if (ctrl.tag > cellViewToDelete.tag) {
						ctrl.tag--;
					}
				}
			}
            if (nextEffect) {
                [self.effectDrawer drawEffect:nextEffect];
            }
			//Positionen anpassen
			[UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
				[self.scriptView fixLocationsOfSubviews];
            } completion:^(BOOL finished) {
			//Farben nachladen
				//[self.scriptView reloadData];
			}];
		}];
	}
}

- (void)addScriptCommand {
    
    if (self.isDeletionModeActive) {
        self.isDeletionModeActive = NO;
        return;
    }

	//insert new command in model
	ComplexEffect *commandToAdd = [((ComplexEffect *)self.script.effects.lastObject) copy];
    commandToAdd.script = self.script;
    
    NSError *error;
    if (self.script.managedObjectContext && ![self.script.managedObjectContext save:&error]) {
        NSLog(@"Save failed: %@", error.helpAnchor);
    }
    
	//get new view for the new command
	UIView *viewToAdd = [self scriptView:self.scriptView cellViewForIndex:self.script.effects.count - 1];
	viewToAdd.alpha = 0.0;

	[self.scriptView addSubview:viewToAdd];
    [self.scriptView fixLocationsOfSubviews];
    [self.scriptView scrollRectToVisible:((UIView *)self.scriptView.subviews.lastObject).frame animated:YES];

    [UIView animateWithDuration:0.8 delay:0.0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
        viewToAdd.alpha = 1.0;
	} completion:^(BOOL finished) {
		[self.scriptView reloadData];
    }];

}

- (void)repeatSwitchValueChanged {
        self.repeatSwitch.selected = !self.repeatSwitch.selected;
    self.script.repeatsWhenFinished = @(self.repeatSwitch.selected);
    ((ComplexEffect *)self.script.effects.firstObject).snapshot = nil;
    [self.effectDrawer drawEffect:self.script.effects.firstObject];
}

- (void)sendScript {
	dispatch_queue_t sendScriptQueue = dispatch_queue_create("sendScriptQueue", NULL);
	dispatch_async(sendScriptQueue, ^{
		[self.controlHandle setColorDirect:[UIColor blackColor]];
		[self.script sendToWCWiflyControl:self.controlHandle];
	});
}

- (void)tapOnBackground:(UITapGestureRecognizer *)gesture {
    if (self.isDeletionModeActive) {
        self.isDeletionModeActive = NO;
    }
}

#pragma mark - SCRIPT VIEW DATASOURCE
- (UIView *)scriptView:(NWScriptView *)view cellViewForIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		if (index < self.script.effects.count) {
			NWScriptCellView *tempView = [[NWScriptCellView alloc]initWithFrame:CGRectZero];
            ComplexEffect *currentEffect = self.script.effects[index];
			    
            //check the first view for update, in relation to the last view.
            if (index == 0 &&
                ((ComplexEffect *)self.script.effects.lastObject).snapshot == nil &&
                currentEffect.snapshot && self.script.repeatsWhenFinished.boolValue)
            {
                currentEffect.snapshot = nil;
            }
            
            if (currentEffect.snapshot == nil) {
                tempView.scriptObjectImageView.image = currentEffect.outdatedSnapshot;
            } else {
                tempView.scriptObjectImageView.image = currentEffect.snapshot;
            }
            
            tempView.scriptObjectImageView.animateSetImage = YES;
            tempView.scriptObjectImageView.cornerRadius = 5;
			tempView.delegate = self;
			tempView.tag = index;
			tempView.scriptObjectImageView.quivering = self.isDeletionModeActive;
			tempView.scriptObjectImageView.downscale = self.isDeletionModeActive;
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
            
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.effectDrawer drawEffect:currentEffect];
            });
			return tempView;
		}
	}
	return nil;

}

- (CGFloat)scriptView:(NWScriptView *)view widthOfObjectAtIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		if (index < self.script.effects.count) {
			return ((ComplexEffect *)self.script.effects[index]).duration.floatValue * self.timeScaleFactor;
		}
	}
	return 0;
}

- (NSUInteger)numberOfObjectsInScriptView:(NWScriptView *)view {
	if (view == self.scriptView) {
		return self.script.effects.count + 1;
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
	}
}

- (void)scriptCellView:(NWScriptCellView *)view finishedWidthChange:(CGFloat)width {
	NSUInteger index = view.tag;
	uint16_t duration = width / self.timeScaleFactor;
	((ComplexEffect *)self.script.effects[index]).duration = @(duration);
	[self.scriptView fixLocationsOfSubviews];
}

#pragma mark - Textfield Delegate

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    self.script.title = textField.text;
    [textField resignFirstResponder];
    return YES;
}

#pragma mark - NWEffectDrawerDelegate

- (void)NWEffectDrawer:(NWEffectDrawer *)drawer finishedDrawing:(id)effect {
    if (drawer == self.effectDrawer) {
        NSUInteger idx = [self.script.effects indexOfObject:effect];
        if (self.scriptView.subviews.count > idx && [effect isKindOfClass:[ComplexEffect class]]) {
            UIView *view = [self.scriptView viewWithTag:idx];
            
            if ([view isKindOfClass:[NWScriptCellView class]]) {
                ComplexEffect* complEffect = (ComplexEffect *)effect;
                NWScriptCellView *cell = (NWScriptCellView *)view;
                cell.scriptObjectImageView.image = complEffect.snapshot;
                cell.scriptObjectImageView.showActivityIndicator = NO;
            }
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
		
		if ([segue.destinationViewController isKindOfClass:[NWComplexScriptCommandEditorViewController class]]) {
			NWComplexScriptCommandEditorViewController *ctrl = (NWComplexScriptCommandEditorViewController *)segue.destinationViewController;
			ctrl.command = [self.script.effects objectAtIndex:self.indexForObjectToEdit];
		}
	}
}


@end
