//
//  NWScriptViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptViewController.h"
#import "NWScript.h"
#import "NWEditComplexScriptObjectViewController.h"
#import "NWScriptObjectCollectionViewCell.h"

@interface NWScriptViewController () <UIGestureRecognizerDelegate>

@property (strong, nonatomic) NWScript *script;
@property (strong, nonatomic) UIScrollView *scrollView;
@property (nonatomic) BOOL isDeletionModeActive;
@property (nonatomic) CGFloat timeScaleFactor;

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
		//scroll view
		self.scrollView.frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
	
	}
	else {
		
		self.scrollView.frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
		
	}
}

- (void)setup {
	self.timeScaleFactor = 1;
	
	//scroll view
	self.scrollView = [[UIScrollView alloc] initWithFrame:CGRectZero];
	self.scrollView.backgroundColor = [UIColor lightGrayColor];
	[self.view addSubview:self.scrollView];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	[self setup];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self fixLocations];
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
			ctrl.command = [self.script.scriptArray lastObject];
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
