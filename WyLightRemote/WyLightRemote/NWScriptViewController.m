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
#import "NWScriptObjectView.h"
#import "NWEditComplexScriptObjectViewController.h"

@interface NWScriptViewController () <UIGestureRecognizerDelegate, NWScriptViewDataSource, NWScriptObjectViewDelegate>

@property (strong, nonatomic) NWScript *script;
@property (strong, nonatomic) NWScriptView *scriptView;
@property (nonatomic) BOOL isDeletionModeActive;
@property (nonatomic) CGFloat timeScaleFactor;
@property (nonatomic) NSUInteger indexForObjectToEdit;

@end

@implementation NWScriptViewController

- (NWScript *)script {
	if (_script == nil) {
		_script = [[NWScript alloc]init];
	}
	return _script;
}

- (void)fixLocations {
	if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
		//script view
		self.scriptView.frame = CGRectMake(0, 40, self.view.frame.size.width, self.view.frame.size.height - 60);
	}
	else {
		self.scriptView.frame = CGRectMake(0, 40, self.view.frame.size.width, self.view.frame.size.height - 60);
	}
}

- (void)setTimeScaleFactor:(CGFloat)timeScaleFactor {
	if (self.scriptView) {
		self.scriptView.timelineScaleFactor = timeScaleFactor;
	}
	_timeScaleFactor = timeScaleFactor;
}

- (void)setup {
	self.timeScaleFactor = 1;
	
	//script view
	self.scriptView = [[NWScriptView alloc] initWithFrame:CGRectZero];
	self.scriptView.dataSource = self;
	self.scriptView.timelineScaleFactor = self.timeScaleFactor;
	self.scriptView.backgroundColor = [UIColor clearColor];
	[self.view addSubview:self.scriptView];
	
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

- (CGFloat)scriptView:(NWScriptView *)view widthOfObjectAtIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		return ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).duration * self.timeScaleFactor;
	}
	return 0;
}

- (NWScriptObjectView *)scriptView:(NWScriptView *)view objectForIndex:(NSUInteger)index {
	if (view == self.scriptView) {
		NWScriptObjectView *tempView = [[NWScriptObjectView alloc]initWithFrame:CGRectZero];
		tempView.endColors = ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).colors;
		if (((NWComplexScriptCommandObject *)self.script.scriptArray[index]).prev) {
			tempView.startColors = ((NWComplexScriptCommandObject *)self.script.scriptArray[index]).prev.colors;
		}
		tempView.borderWidth = 1;
		tempView.cornerRadius = 10;
		tempView.delegate = self;
		
		UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(setObjectForEdit:)];
		tap.numberOfTapsRequired = 1;
		[tempView addGestureRecognizer:tap];
		
		UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:tempView action:@selector(pinchWidth:)];
		[tempView addGestureRecognizer:pinch];
		
		return tempView;
	}
	return nil;
}

- (void)setObjectForEdit:(UITapGestureRecognizer *)gesture {
	self.indexForObjectToEdit = gesture.view.tag;
	[self performSegueWithIdentifier:@"edit:" sender:self];
}

- (NSUInteger)numberOfObjectsInScriptView:(NWScriptView *)view {
	if (view == self.scriptView) {
		return self.script.scriptArray.count;
	}
	return 0;
}

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

- (void)activateDeletionMode:(UILongPressGestureRecognizer *)gr {
    if (gr.state == UIGestureRecognizerStateBegan)
    {
        NSIndexPath *indexPath = [self.collectionView indexPathForItemAtPoint:[gr locationInView:self.collectionView]];
        if (indexPath)
        {
            self.isDeletionModeActive = YES;
            NWCollectionViewLayout *layout = (NWCollectionViewLayout *)self.collectionView.collectionViewLayout;
            [layout invalidateLayout];
        }
    }
}

- (void)endDeletionMode:(UITapGestureRecognizer *)gr {
    if (self.isDeletionModeActive)
    {
        NSIndexPath *indexPath = [self.collectionView indexPathForItemAtPoint:[gr locationInView:self.collectionView]];
        //if (!indexPath)
        {
            self.isDeletionModeActive = NO;
            NWCollectionViewLayout *layout = (NWCollectionViewLayout *)self.collectionView.collectionViewLayout;
            [layout invalidateLayout];
        }
    }
}*/


@end
