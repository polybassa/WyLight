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
#import "NWTimeLineView.h"

@interface NWScriptViewController () <UIGestureRecognizerDelegate, NWScriptViewDataSource, NWScriptObjectControlDelegate, UIScrollViewDelegate>

@property (strong, nonatomic) NWScript *script;
@property (strong, nonatomic) NWScriptView *scriptView;
//@property (strong, nonatomic) NWTimeLineView *timeLineView;
@property (nonatomic) BOOL isDeletionModeActive;
@property (nonatomic) CGFloat timeScaleFactor;
@property (nonatomic) NSUInteger indexForObjectToEdit;

@end

@implementation NWScriptViewController

#pragma mark - GETTER + SETTER
- (NWScript *)script {
	if (_script == nil) {
		_script = [[NWScript alloc]init];
	}
	return _script;
}

- (void)setTimeScaleFactor:(CGFloat)timeScaleFactor {
	_timeScaleFactor = timeScaleFactor;
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

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	[super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
	
	if (UIInterfaceOrientationIsLandscape([[UIApplication sharedApplication] statusBarOrientation]))
	{
		CGRect biggerFrame = self.tabBarController.view.frame;
		biggerFrame.size.height += self.tabBarController.tabBar.frame.size.height;
		self.tabBarController.view.frame = biggerFrame ;
	}
}

#pragma mark - SETUP STUFF
- (void)fixLocations {
	if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
		
		//script view
		self.scriptView.frame = CGRectMake(0, 30, self.view.frame.size.width, self.view.frame.size.height - 60);
	}
	else {
		
		self.scriptView.frame = CGRectMake(0, 20, self.view.frame.size.width, self.view.frame.size.height - 40);
	}
	[self.scriptView fixLocationsOfSubviews];
}

- (void)setup {
	self.timeScaleFactor = 1;
		
		
	//script view
	self.scriptView = [[NWScriptView alloc] initWithFrame:CGRectZero];
	self.scriptView.dataSource = self;
	self.scriptView.backgroundColor = [UIColor clearColor];
	self.scriptView.delegate = self;
	[self.view addSubview:self.scriptView];

	UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchOnScriptView:)];
	[self.scriptView addGestureRecognizer:pinch];
	[self.view addGestureRecognizer:pinch];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	[self setup];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self fixLocations];
	[self.scriptView reloadData];

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
            for (NWScriptObjectControl *control in self.scriptView.subviews) {
				if ([control isKindOfClass:[NWScriptObjectControl class]]) {
					control.showDeleteButton = NO;
				}
			}
		}
	} else {
		self.indexForObjectToEdit = gesture.view.tag;
		[self performSegueWithIdentifier:@"edit:" sender:self];
	}
}

- (void)activateDeletionMode:(UILongPressGestureRecognizer *)gesture {
    if (gesture.state == UIGestureRecognizerStateBegan)
    {
		if ([gesture.view isKindOfClass:[NWScriptObjectControl class]]) {
			self.isDeletionModeActive = YES;
            for (NWScriptObjectControl *control in self.scriptView.subviews) {
				if ([control isKindOfClass:[NWScriptObjectControl class]]) {
					control.showDeleteButton = YES;
				}
			}
		}
    }
}

- (void)pinchOnScriptView:(UIPinchGestureRecognizer *)gesture {
	if (gesture.state == UIGestureRecognizerStateChanged) {
		self.timeScaleFactor *= gesture.scale;
		gesture.scale = 1;
	}
}

#pragma mark - BUTTON CALLBACKS
- (void)deleteScriptObject:(UIButton *)sender {
	if ([sender.superview isKindOfClass:[NWScriptObjectControl class]]) {
		NWScriptObjectControl *objectToDelete = (NWScriptObjectControl *)sender.superview;
		
		//ausblenden
		[UIView animateWithDuration:0.5 delay:0.0 options:UIViewAnimationOptionCurveEaseIn animations:^{
			
			objectToDelete.alpha = 0.0;
			objectToDelete.deleteButton.alpha = 0.0;
			
		} completion:^(BOOL finished) {
			
			if (finished) {
				
				[self.script removeObjectAtIndex:objectToDelete.tag];
				[objectToDelete removeFromSuperview];
				for (NWScriptObjectControl *ctrl in self.scriptView.subviews) {
					if ([ctrl isKindOfClass:[NWScriptObjectControl class]]) {
						if (ctrl.tag > objectToDelete.tag) {
							ctrl.tag--;
						}
					}
				}
				
				[UIView animateWithDuration:0.5 delay:0.0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
					
					[self.scriptView fixLocationsOfSubviews];
					
				} completion:^(BOOL finished) {
					
					[self.scriptView reloadData];
				}];
			}
		}];
	}
}

- (void)addScriptCommand:(UIBarButtonItem *)sender {
	NSLog(@"ADD");
	
}

#pragma mark - SCRIPT VIEW DATASOURCE
- (CGFloat)scriptView:(NWScriptView *)view widthOfObjectAtIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		return ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).duration * self.timeScaleFactor;
	}
	return 0;
}

- (NWScriptObjectControl *)scriptView:(NWScriptView *)view objectForIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		NWScriptObjectControl *tempView = [[NWScriptObjectControl alloc]initWithFrame:CGRectZero];
		tempView.endColors = ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).colors;
		if (((NWComplexScriptCommandObject *)self.script.scriptArray[index]).prev) {
			tempView.startColors = ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).prev.colors;
		}
		tempView.borderWidth = 1;
		tempView.cornerRadius = 10;
		tempView.delegate = self;
		tempView.showDeleteButton = self.isDeletionModeActive;
		tempView.backgroundColor = [UIColor blackColor];
		
		UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(setObjectForEdit:)];
		tap.numberOfTapsRequired = 1;
		[tempView addGestureRecognizer:tap];
		
		UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:tempView action:@selector(pinchWidth:)];
		[tempView addGestureRecognizer:pinch];
		
		UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(activateDeletionMode:)];
		[tempView addGestureRecognizer:longPress];
		
		[tempView.deleteButton addTarget:self action:@selector(deleteScriptObject:) forControlEvents:UIControlEventTouchUpInside];
		
		return tempView;
	}
	return nil;
}

- (NSUInteger)numberOfObjectsInScriptView:(NWScriptView *)view {
	if (view == self.scriptView) {
		return self.script.scriptArray.count;
	}
	return 0;
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView {
	//self.timeLineView.contentOffset = CGPointMake(scrollView.contentOffset.x + 20, scrollView.contentOffset.y);
}

#pragma mark - SCRIPTOBJECTCONTROL DELEGATE
- (void)scriptObjectView:(NWScriptObjectView *)view changedWidthTo:(CGFloat)width deltaOfChange:(CGFloat)delta {
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

/*
#pragma mark - gesture-recognition action methods
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch {
    CGPoint touchPoint = [touch locationInView:self.collectionView];
    NSIndexPath *indexPath = [self.collectionView indexPathForItemAtPoint:touchPoint];
    if (indexPath && [gestureRecognizer isKindOfClass:[UITapGestureRecognizer class]])
    {
        return NO;
    }
    return YES;
}
*/



@end
