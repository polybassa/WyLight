//
//  NWEditComplexScriptObjectViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWEditComplexScriptObjectViewController.h"
#import "NWScriptObjectView.h"
#import "NWSetFadeScriptCommandObject.h"
#import "NWSetGradientScriptCommandObject.h"
#import "NWEditFadeCommandViewController.h"
#import "NWEditGradientCommandViewController.h"
#import "WCWiflyControlWrapper.h"
#import "NWCollectionViewLayout.h"
#import "NWScriptObjectCollectionViewCell.h"
#import "NWAddCollectionViewCell.h"

@interface NWEditComplexScriptObjectViewController () <UIGestureRecognizerDelegate, UICollectionViewDataSource, UICollectionViewDelegateFlowLayout, NWCollectionViewLayoutDelegate, ALRadialMenuDelegate>
@property (nonatomic, strong) NWScriptObjectView *gradientPreviewView;
@property (nonatomic, strong) UISwitch *modeSwitch;
@property (nonatomic) BOOL isDeletionModeActive;
@property (nonatomic) NSUInteger indexOfObjectToAlter;
@property (nonatomic, strong) UICollectionView *collectionView;
@property (nonatomic, strong) ALRadialMenu *radialMenu;
@property (nonatomic, weak) NSIndexPath *indexPathOfLastCell;
@property (nonatomic, strong) UIImage *addFadeImage;
@property (nonatomic, strong) UIImage *addGradientImage;
@end

@implementation NWEditComplexScriptObjectViewController

- (void)viewWillLayoutSubviews {
	[super viewWillLayoutSubviews];
	[self fixLocations];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	self.modeSwitch.on = !self.command.isWaitCommand;
	[self updateGradientView];
}

- (void)updateGradientView {
	if (self.command.prev) {
		self.gradientPreviewView.startColors = self.command.prev.colors;
	} else {
		self.gradientPreviewView.startColors = nil;
	}

	if (self.command.isWaitCommand) {
		self.gradientPreviewView.endColors = self.gradientPreviewView.startColors;
	} else {
		self.gradientPreviewView.endColors = self.command.colors;
	}
	
	[self sendPreview];
}

- (void)fixLocations {
	CGFloat totalWidth = self.view.bounds.size.width - 40.0;
	CGFloat totalHeight = self.view.bounds.size.height - 40.0;
	CGFloat xOffset = 20;
	
	if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
		self.gradientPreviewView.frame = CGRectMake(xOffset, 80, totalWidth, self.command.colors.count * 5);
	
		self.modeSwitch.frame = CGRectMake(20, 20, 40, 20);

		self.collectionView.frame = CGRectMake(0, totalHeight / 2 + 100 , self.view.bounds.size.width, self.command.colors.count * 4);
	}
	else {
		self.gradientPreviewView.frame = CGRectMake(self.view.center.x, 20, self.view.bounds.size.width / 2 - 20, self.command.colors.count * 3);
		
		self.modeSwitch.frame = CGRectMake(20, 20, 40, 20);
		
		self.collectionView.frame = CGRectMake(0, self.view.center.y, self.view.bounds.size.width, self.command.colors.count * 4);
	}
}

- (void)viewDidLoad {
    [super viewDidLoad];
	[self setup];
}

- (void)setup {
	//self
	self.view.backgroundColor = [UIColor viewFlipsideBackgroundColor];
	
	//mode switch
	self.modeSwitch = [[UISwitch alloc]initWithFrame:CGRectZero];
	[self.modeSwitch addTarget:self action:@selector(switchChanged) forControlEvents:UIControlEventValueChanged];
	[self.view addSubview:self.modeSwitch];
	
	//gradient view
	self.gradientPreviewView = [[NWScriptObjectView alloc]initWithFrame:CGRectZero];
	self.gradientPreviewView.backgroundColor = [UIColor blackColor];
	[self.view addSubview:self.gradientPreviewView];
	
	//collection view
	NWCollectionViewLayout *layout = [[NWCollectionViewLayout alloc]init];
		
	self.collectionView = [[UICollectionView alloc]initWithFrame:CGRectZero collectionViewLayout:layout];
	self.collectionView.backgroundColor = [UIColor scrollViewTexturedBackgroundColor];
	self.collectionView.dataSource = self;
	self.collectionView.delegate = self;
	[self.collectionView registerClass:[NWScriptObjectCollectionViewCell class] forCellWithReuseIdentifier:@"SCRIPT"];
	[self.collectionView registerClass:[NWAddCollectionViewCell class] forCellWithReuseIdentifier:@"ADD"];
	[self.view addSubview:self.collectionView];
	
	UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(activateDeletionMode:)];
	longPress.delegate = self;
	[self.collectionView addGestureRecognizer:longPress];
	
	UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(endDeletionMode:)];
	tap.delegate = self;
	tap.numberOfTapsRequired = 1;
	[self.collectionView addGestureRecognizer:tap];
	
	//radial menue
	self.radialMenu = [[ALRadialMenu alloc] init];
	self.radialMenu.delegate = self;
}

#pragma mark - RADIAL MENU DELEGATE 
- (NSInteger)numberOfItemsInRadialMenu:(ALRadialMenu *)radialMenu {
	return 2;
}

- (NSInteger)arcStartForRadialMenu:(ALRadialMenu *)radialMenu {
	return 145;
}

- (NSInteger)arcSizeForRadialMenu:(ALRadialMenu *)radialMenu {
	return 70;
}

- (NSInteger)arcRadiusForRadialMenu:(ALRadialMenu *)radialMenu {
	return 60;
}

- (float)buttonSizeForRadialMenu:(ALRadialMenu *)radialMenu {
	return 44;
}

- (UIImage *)radialMenu:(ALRadialMenu *)radialMenu imageForIndex:(NSInteger)index {
	UIImage *image = nil;
	
	if (index == 1) {
		image = self.addFadeImage;
	} else if (index == 2) {
		image = self.addGradientImage;
	}
	else return nil;
	return image;
}

- (void)radialMenu:(ALRadialMenu *)radialMenu didSelectItemAtIndex:(NSInteger)index {
	[self.radialMenu itemsWillDisapearIntoButton:nil];
	if (index == 1) {
		[self addFadeCommand];
	} else if (index == 2)
	{
		[self addGradientCommand];
	}
}

- (UIImage *)addFadeImage {
	if (!_addFadeImage) {
		NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 64, 64)];
		view.borderWidth = 1;
		view.backgroundColor = [UIColor blackColor];
		view.endColors = [NWEditComplexScriptObjectViewController defaultFadeCommand].colors;
		view.opaque = NO;
		
		_addFadeImage = [NWEditComplexScriptObjectViewController imageWithView:view];
	}
	return _addFadeImage;
}

- (UIImage *)addGradientImage {
	if (!_addGradientImage) {
		NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 64, 64)];
		view.borderWidth = 1;
		view.backgroundColor = [UIColor blackColor];
		view.endColors = [NWEditComplexScriptObjectViewController defaultGradientCommand].colors;
		view.opaque = NO;
		
		_addGradientImage = [NWEditComplexScriptObjectViewController imageWithView:view];
	}
	return _addGradientImage;
	
}

#pragma mark - ADD OBJECTS
+ (NWSetGradientScriptCommandObject *)defaultGradientCommand {
	NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
	obj.color1 = [UIColor blueColor];
	obj.color2 = [UIColor redColor];
	obj.offset = 0;
	obj.numberOfLeds = 32;
	obj.duration = 5;
	obj.parallel = YES;
	return obj;
}

+ (NWSetFadeScriptCommandObject *)defaultFadeCommand {
	NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
	obj.address = 0xffffffff;
	obj.color = [UIColor greenColor];
	obj.duration = 5;
	obj.parallel = YES;
	return obj;
}

- (void)addFadeCommand {
	[self.command.itsScriptObjects addObject:[NWEditComplexScriptObjectViewController defaultFadeCommand]];
	[self updateGradientView];
	[self.collectionView reloadData];
	if (self.indexPathOfLastCell) {
		[self.collectionView scrollToItemAtIndexPath:self.indexPathOfLastCell atScrollPosition:UICollectionViewScrollPositionCenteredHorizontally animated:YES];
	}
}

- (void)addGradientCommand {	
	[self.command.itsScriptObjects addObject: [NWEditComplexScriptObjectViewController defaultGradientCommand]];
	[self updateGradientView];
	[self.collectionView reloadData];
	if (self.indexPathOfLastCell) {
		[self.collectionView scrollToItemAtIndexPath:self.indexPathOfLastCell atScrollPosition:UICollectionViewScrollPositionCenteredHorizontally animated:YES];
	}
}

#pragma mark - USER ACTIONS

- (void)addCommandButtonPressed:(UIButton *)sender {
	if (!self.isDeletionModeActive) {

		CGRect frame = CGRectMake(0.0,0.0,sender.frame.size.width,sender.frame.size.height);
		frame.origin = [sender convertPoint:sender.frame.origin toView:self.view];
		[self.radialMenu buttonsWillAnimateFromButton:sender withFrame:frame inView:self.view];

	}
}

- (void)sendPreview {
	if (self.controlHandle) {
		[self.controlHandle clearScript];
		[self.controlHandle setColorDirectWithColors:self.gradientPreviewView.endColors];
	}
}

- (void)switchChanged {
	self.command.waitCommand = !self.modeSwitch.on;
	[self updateGradientView];
}

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
		//enter deletion mode only if
		// indexPath is available
		// I have more than 1 scriptobject, because i don't want an empty model
		// gesture is fired on a scriptObject, not on the add Object
		// and the radial menu is not shown
		
        if (indexPath && self.command.itsScriptObjects.count > 1 && indexPath.row < self.command.itsScriptObjects.count && self.radialMenu.itemIndex == 0)
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
		self.isDeletionModeActive = NO;
		NWCollectionViewLayout *layout = (NWCollectionViewLayout *)self.collectionView.collectionViewLayout;
		[layout invalidateLayout];
    }
	else {
		NSIndexPath *indexPath = [self.collectionView indexPathForItemAtPoint:[gr locationInView:self.collectionView]];
		if (indexPath) {
			self.indexOfObjectToAlter = indexPath.row;
			NWScriptCommandObject *obj = [self.command.itsScriptObjects objectAtIndex:self.indexOfObjectToAlter];
			
			if ([obj isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
				[self performSegueWithIdentifier:@"editFade:" sender:self];
			} else if ([obj isKindOfClass:[NWSetGradientScriptCommandObject class]])
			{
				[self performSegueWithIdentifier:@"editGradient:" sender:self];
			}
		}
	}
}

#pragma mark - COLLECTION VIEW STUFF
- (BOOL)isDeletionModeActiveForCollectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout {
	return self.isDeletionModeActive;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
	return self.command.itsScriptObjects.count + 1;
}

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
	return 1;
}

- (BOOL)collectionView:(UICollectionView *)collectionView shouldSelectItemAtIndexPath:(NSIndexPath *)indexPath {
	if (self.isDeletionModeActive) {
		return NO;
	}
	else return YES;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
	if (indexPath.row < self.command.itsScriptObjects.count) {
		NWScriptObjectCollectionViewCell *tempCell = [collectionView dequeueReusableCellWithReuseIdentifier:@"SCRIPT" forIndexPath:indexPath];
		tempCell.scriptObjectView.endColors = [self.command.itsScriptObjects[indexPath.row] colors];
	
		UITapGestureRecognizer *gesture = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(endDeletionMode:)];
		gesture.numberOfTapsRequired = 1;
		[tempCell addGestureRecognizer:gesture];
	
		[tempCell.deleteButton addTarget:self action:@selector(delete:) forControlEvents:UIControlEventTouchUpInside];
		self.indexPathOfLastCell = indexPath;
		return tempCell;
	}
	else {
		NWAddCollectionViewCell *tempCell = [collectionView dequeueReusableCellWithReuseIdentifier:@"ADD" forIndexPath:indexPath];
		[tempCell.button addTarget:self action:@selector(addCommandButtonPressed:) forControlEvents:UIControlEventTouchUpInside];

		return tempCell;
	}
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
	return CGSizeMake(122, self.collectionView.frame.size.height - 20);
}

#pragma mark - delete for button
- (void)delete:(UIButton *)sender {
	if (self.command.itsScriptObjects.count > 1) {
		NSIndexPath *indexPath = [self.collectionView indexPathForCell:(NWScriptObjectCollectionViewCell *)sender.superview.superview];
		[self.command.itsScriptObjects removeObjectAtIndex:indexPath.row];
		[self.collectionView deleteItemsAtIndexPaths:[NSArray arrayWithObject:indexPath]];
		[self updateGradientView];
	}
	else if (self.isDeletionModeActive) {
		[self endDeletionMode:nil];
	}
}

#pragma mark - SEGUES
- (IBAction)unwindEditScriptObject:(UIStoryboardSegue *)segue {
	if ([segue.sourceViewController respondsToSelector:@selector(command)]) {
		NWScriptCommandObject *cmdObj = (NWScriptCommandObject*)[segue.sourceViewController command];
		[self.command.itsScriptObjects replaceObjectAtIndex:self.indexOfObjectToAlter withObject:cmdObj];
		[self.collectionView reloadData];
	}
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.identifier isEqualToString:@"editFade:"]) {
		if ([segue.destinationViewController isKindOfClass:[NWEditFadeCommandViewController class]]) {
			NWEditFadeCommandViewController *dest = (NWEditFadeCommandViewController*)segue.destinationViewController;
			dest.command = [[self.command.itsScriptObjects objectAtIndex:self.indexOfObjectToAlter] copy];
		}
	} else if ([segue.identifier isEqualToString:@"editGradient:"]) {
		if ([segue.destinationViewController isKindOfClass:[NWEditGradientCommandViewController class]]) {
			NWEditGradientCommandViewController *dest = (NWEditGradientCommandViewController*)segue.destinationViewController;
			dest.command = [[self.command.itsScriptObjects objectAtIndex:self.indexOfObjectToAlter] copy];
		}
	}
}

+ (UIImage *) imageWithView:(UIView *)view {
    UIGraphicsBeginImageContextWithOptions(view.bounds.size, view.opaque, 0.0);
    [view.layer renderInContext:UIGraphicsGetCurrentContext()];
	
    UIImage * img = UIGraphicsGetImageFromCurrentImageContext();
	
    UIGraphicsEndImageContext();
	
    return img;
}



@end
