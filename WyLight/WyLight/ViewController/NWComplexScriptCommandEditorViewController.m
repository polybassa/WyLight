//
//  NWComplexScriptCommandEditorViewController.m
//  WyLight
//
//  Created by Nils Weiß on 12.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWComplexScriptCommandEditorViewController.h"
#import "NWScriptObjectControl.h"
#import "iCarousel.h"
#import "ALRadialMenu.h"
#import "WCWiflyControlWrapper.h"
#import "NWTimeValueEditView.h"
#import "NWGradientEditView.h"
#import "NWSetFadeScriptCommandObject.h"
#import "NWSetGradientScriptCommandObject.h"

@interface NWComplexScriptCommandEditorViewController () <iCarouselDataSource, iCarouselDelegate, ALRadialMenuDelegate, NWTimeValueEditViewDelegate, NWGradientEditViewDelegate>

@property (nonatomic, strong) iCarousel *scriptSubCommandsCarousel;
@property (nonatomic, strong) iCarousel *toolKitCarousel;
@property (nonatomic, strong) NWTimeValueEditView *timeValueEditView;
@property (nonatomic, strong) NWGradientEditView *gradientEditView;
@property (nonatomic, strong) NSArray *addEffectImages;
@property (nonatomic, strong) ALRadialMenu *addEffectMenu;
@property (nonatomic, assign) NSUInteger currentItemIndex;
@property (nonatomic) BOOL sendInitialClearScript;

@end

@implementation NWComplexScriptCommandEditorViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	[self setup];
}

- (void)viewWillLayoutSubviews {
	[super viewWillLayoutSubviews];
	[self fixLocations];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	self.sendInitialClearScript = YES;
	self.currentItemIndex = self.scriptSubCommandsCarousel.currentItemView.tag;
	[self.scriptSubCommandsCarousel reloadItemAtIndex:self.scriptSubCommandsCarousel.currentItemIndex animated:YES];
	[super viewWillAppear:animated];
}

- (void)fixLocations {
	if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
		
		self.scriptSubCommandsCarousel.frame = CGRectMake(0, 60, self.view.bounds.size.width, self.view.bounds.size.height / 2);
		
		self.toolKitCarousel.frame = CGRectMake(0, self.view.bounds.size.height / 2 + 60, self.view.bounds.size.width, self.view.bounds.size.height / 2 - 60);
	}
	else {
	}
}

- (void)setup {
	self.view.superview.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	
	self.scriptSubCommandsCarousel = [[iCarousel alloc]initWithFrame:CGRectZero];
	self.scriptSubCommandsCarousel.dataSource = self;
	self.scriptSubCommandsCarousel.delegate = self;
	self.scriptSubCommandsCarousel.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	self.scriptSubCommandsCarousel.type = iCarouselTypeInvertedTimeMachine;
	self.scriptSubCommandsCarousel.bounces = NO;
	[self.view addSubview:self.scriptSubCommandsCarousel];
	
	self.addEffectMenu = [[ALRadialMenu alloc] init];
	self.addEffectMenu.delegate = self;
		
	self.toolKitCarousel = [[iCarousel alloc] initWithFrame:CGRectZero];
	self.toolKitCarousel.dataSource = self;
	self.toolKitCarousel.delegate = self;
	self.toolKitCarousel.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	self.toolKitCarousel.type = iCarouselTypeLinear;
	[self.view addSubview:self.toolKitCarousel];
}

- (void)updateView {
	if (self.command.isWaitCommand) {
		self.scriptSubCommandsCarousel.scrollEnabled = NO;
	} else {
		self.scriptSubCommandsCarousel.scrollEnabled = YES;
		
		id currentCommand = self.command.scriptObjects[self.currentItemIndex];
		if ([currentCommand isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
			NWSetFadeScriptCommandObject *currentFadeCommand = (NWSetFadeScriptCommandObject *)currentCommand;
		} else if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
			NWSetGradientScriptCommandObject *currentGradientCommand = (NWSetGradientScriptCommandObject *)currentCommand;
		}
	}
	[self sendPreview];
}

- (void)sendPreview {
	if (self.controlHandle) {
		if (self.sendInitialClearScript) {
			[self.controlHandle clearScript];
			self.sendInitialClearScript = NO;
		}
		[self.controlHandle setColorDirectWithColors:self.command.colors];
	}
}

#pragma mark - SETTER
- (void)setCurrentItemIndex:(NSUInteger)currentItemIndex {
	_currentItemIndex = currentItemIndex;
	self.gradientEditView.command = self.command.scriptObjects[currentItemIndex];
	[self updateView];
}

#pragma mark - iCarousel methods
- (NSUInteger)numberOfItemsInCarousel:(iCarousel *)carousel {
	if (carousel == self.scriptSubCommandsCarousel) {
		return self.command.scriptObjects.count;
	} else {
		return 5;
	}
}

- (UIView *)carousel:(iCarousel *)carousel viewForItemAtIndex:(NSUInteger)index reusingView:(UIView *)view {
	if (carousel == self.scriptSubCommandsCarousel) {
		//create new view if no view is available for recycling
		if (view == nil || ![view isKindOfClass:[NWScriptObjectControl class]])
		{
			view = [[NWScriptObjectControl alloc] initWithFrame:CGRectMake(0, 0, 200.0f, 200.0f)];
			view.contentMode = UIViewContentModeCenter;
		}
		
		view.tag = self.command.scriptObjects.count - 1 - index;
		if ([view isKindOfClass:[NWScriptObjectControl class]]) {
			NWScriptObjectControl* scriptView = (NWScriptObjectControl *)view;
			id<NWDrawableCommand> command = [self.command.scriptObjects objectAtIndex:view.tag];
			[command setBackgroundColor:[UIColor clearColor]];
			if (self.command.waitCommand) {
				scriptView.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.9 alpha:0.9];
				scriptView.endColors = self.command.prev.colors;
			} else {
				scriptView.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.9 alpha:0.2];
				scriptView.endColors = [command colors];
			}
			scriptView.cornerRadius = 5.0;
			scriptView.quivering = NO;
			scriptView.downscale = NO;
			
			UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPressOnScriptObjectControl:)];
			[scriptView addGestureRecognizer:longPress];
			
			UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapOnScriptObjectControl:)];
			[scriptView addGestureRecognizer:tap];
			
			UISwipeGestureRecognizer *swipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeUpOnScriptObjectControl:)];
			swipe.direction = UISwipeGestureRecognizerDirectionUp;
			[scriptView addGestureRecognizer:swipe];
		}
		return view;
	} else {
		CGRect standardFrame = CGRectMake(0, 0, 250, 150);
		switch (index) {
			case 0: {
				self.timeValueEditView = [[NWTimeValueEditView alloc] initWithFrame:standardFrame];
				self.timeValueEditView.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.9 alpha:0.2];
				self.timeValueEditView.cornerRadius = 5.0;
				self.timeValueEditView.contentMode = UIViewContentModeCenter;
				self.timeValueEditView.command = self.command;
				self.timeValueEditView.delegate = self;
				return self.timeValueEditView;
			}
				
			case 1: {
				self.gradientEditView = [[NWGradientEditView alloc] initWithFrame:standardFrame];
				self.gradientEditView.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.9 alpha:0.2];
				self.gradientEditView.cornerRadius = 0.5;
				self.gradientEditView.contentMode = UIViewContentModeCenter;
				self.gradientEditView.command = self.command.scriptObjects[self.currentItemIndex];
				self.gradientEditView.delegate = self;
				return self.gradientEditView;
			}
				
			default: {
				NWScriptObjectControl* scriptView = [[NWScriptObjectControl alloc] initWithFrame:CGRectMake(0, 0, 250.0f, 150)];
				scriptView.contentMode = UIViewContentModeCenter;
				scriptView.tag = self.command.scriptObjects.count - 1 - index;
				scriptView.cornerRadius = 5.0;
				scriptView.quivering = NO;
				scriptView.downscale = NO;
				
				scriptView.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.9 alpha:0.2];
				return scriptView;
			}
		}
		return nil;
	}
}

- (CGFloat)carousel:(iCarousel *)carousel valueForOption:(iCarouselOption)option withDefault:(CGFloat)value {
	if (carousel == self.scriptSubCommandsCarousel) {
		//customize carousel display
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
	} else {
		
		switch (option)
		{
			case iCarouselOptionSpacing:
			{
				return value * 1.05f;
			}
			default:
			{
				return value;
			}
		}
	}
}

- (void)carouselCurrentItemIndexDidChange:(iCarousel *)carousel {
	if (carousel == self.scriptSubCommandsCarousel) {
		self.currentItemIndex = carousel.currentItemView.tag;
	}
}

#pragma mark - GESTURE RECOGNIZER CALLBACKS
- (void)longPressOnScriptObjectControl:(UILongPressGestureRecognizer *)gesture {
	if ([gesture.view isKindOfClass:[NWScriptObjectControl class]] && gesture.view == self.scriptSubCommandsCarousel.currentItemView) {
		NWScriptObjectControl* scriptView = (NWScriptObjectControl *)gesture.view;
		scriptView.quivering = YES;
		self.scriptSubCommandsCarousel.scrollEnabled = NO;
	}
}

- (void)tapOnScriptObjectControl:(UITapGestureRecognizer *)gesture {
	if ([gesture.view isKindOfClass:[NWScriptObjectControl class]]
		&& gesture.view == self.scriptSubCommandsCarousel.currentItemView &&
		self.scriptSubCommandsCarousel.scrollEnabled == NO) {
		NWScriptObjectControl* scriptView = (NWScriptObjectControl *)gesture.view;
		scriptView.quivering = NO;
		self.scriptSubCommandsCarousel.scrollEnabled = YES;
	}
}

- (void)swipeUpOnScriptObjectControl:(UISwipeGestureRecognizer *)gesture {
	if ([gesture.view isKindOfClass:[NWScriptObjectControl class]]
		&& gesture.view == self.scriptSubCommandsCarousel.currentItemView &&
		self.scriptSubCommandsCarousel.scrollEnabled == NO) {
		
		if (self.command.scriptObjects.count > 1) {
			NSUInteger index = gesture.view.tag;
			[self.command.scriptObjects removeObjectAtIndex:index];
			[self.scriptSubCommandsCarousel removeItemAtIndex:self.scriptSubCommandsCarousel.currentItemIndex animated:YES];
		}
		else {
			NWScriptObjectControl* scriptView = (NWScriptObjectControl *)gesture.view;
			scriptView.quivering = NO;
		}
		self.scriptSubCommandsCarousel.scrollEnabled = YES;
		self.currentItemIndex = self.scriptSubCommandsCarousel.currentItemView.tag;
	}
}

#pragma mark - RADIAL MENU DELEGATE
- (NSInteger)numberOfItemsInRadialMenu:(ALRadialMenu *)radialMenu {
	if (radialMenu == self.addEffectMenu) {
		return self.addEffectImages.count;
	} else {
		return 0;
	}
}

- (NSInteger)arcStartForRadialMenu:(ALRadialMenu *)radialMenu {
	if (radialMenu == self.addEffectMenu) {
		return 90;
	} else {
		return 180;
	}
}

- (NSInteger)arcSizeForRadialMenu:(ALRadialMenu *)radialMenu {
	if (radialMenu == self.addEffectMenu) {
		return 45;
	} else {
		return 360;
	}
}

- (NSInteger)arcRadiusForRadialMenu:(ALRadialMenu *)radialMenu {
	if (radialMenu == self.addEffectMenu) {
		return 80;
	} else {
		return 70;
	}
}

- (float)buttonSizeForRadialMenu:(ALRadialMenu *)radialMenu {
	return 44;
}

- (UIImage *)radialMenu:(ALRadialMenu *)radialMenu imageForIndex:(NSInteger)index {
	if (radialMenu == self.addEffectMenu) {
		return self.addEffectImages[index - 1];
	} else {
		return nil;
	}
}

- (void)radialMenu:(ALRadialMenu *)radialMenu didSelectItemAtIndex:(NSInteger)index {
	if (radialMenu == self.addEffectMenu) {
		[radialMenu itemsWillDisapearIntoButton:nil];
		if (index == 1) {
			[self addFadeCommand];
		} else if (index == 2) {
			[self addGradientCommand];
		}
	}
}

#pragma mark - 
#pragma mark TOOLKIT DELEGATES
#pragma mark TimeEditViewDelegate
- (void)TimeValueEditView:(NWTimeValueEditView *)view sliderValueChanged:(CGFloat)value {
	if (view == self.timeValueEditView) {
		self.command.duration = (uint16_t)(value);
		[view reloadData];
	}
}

- (void)TimeValueEditView:(NWTimeValueEditView *)view switchValueChanged:(BOOL)on {
	if (view == self.timeValueEditView) {
		self.command.waitCommand = !on;
		[view reloadData];
		[self.scriptSubCommandsCarousel reloadData];
	}
	[self updateView];
}

#pragma mark GradientEditViewDelegate

- (void)NWGradientEditViewDelegateDownButtonPressed:(NWGradientEditView *)view {
	NWSetGradientScriptCommandObject *currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		currentCommand.offset += 1;
		[self.scriptSubCommandsCarousel reloadItemAtIndex:[self.scriptSubCommandsCarousel indexOfItemView:self.scriptSubCommandsCarousel.currentItemView] animated:YES];
		[view reloadData];
		[self sendPreview];
	}
}

- (void)NWGradientEditViewDelegateMinusButtonPressed:(NWGradientEditView *)view {
	NWSetGradientScriptCommandObject *currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		currentCommand.numberOfLeds -= 1;
		[self.scriptSubCommandsCarousel reloadItemAtIndex:[self.scriptSubCommandsCarousel indexOfItemView:self.scriptSubCommandsCarousel.currentItemView] animated:YES];
		[view reloadData];
		[self sendPreview];
	}
}

- (void)NWGradientEditViewDelegatePlusButtonPressed:(NWGradientEditView *)view {
	NWSetGradientScriptCommandObject *currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		currentCommand.numberOfLeds += 1;
		[self.scriptSubCommandsCarousel reloadItemAtIndex:[self.scriptSubCommandsCarousel indexOfItemView:self.scriptSubCommandsCarousel.currentItemView] animated:YES];
		[view reloadData];
		[self sendPreview];
	}

}

- (void)NWGradientEditViewDelegateUpButtonPressed:(NWGradientEditView *)view {
	NWSetGradientScriptCommandObject *currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		currentCommand.offset -= 1;
		[self.scriptSubCommandsCarousel reloadItemAtIndex:[self.scriptSubCommandsCarousel indexOfItemView:self.scriptSubCommandsCarousel.currentItemView] animated:YES];
		[view reloadData];
		[self sendPreview];
	}
}

#pragma mark - Add Commands
- (void)addFadeCommand {
	[self.command.scriptObjects addObject:[NWComplexScriptCommandEditorViewController defaultFadeCommand]];
	[self.scriptSubCommandsCarousel reloadData];
	double delayInSeconds = 0.5;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
	dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
		self.currentItemIndex = self.scriptSubCommandsCarousel.currentItemView.tag;
	});
}

- (void)addGradientCommand {
	[self.command.scriptObjects addObject:[NWComplexScriptCommandEditorViewController defaultGradientCommand]];
	[self.scriptSubCommandsCarousel reloadData];
	double delayInSeconds = 0.5;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
	dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
		self.currentItemIndex = self.scriptSubCommandsCarousel.currentItemView.tag;
	});
}

#pragma mark - HELPERS
+ (UIImage *)imageWithView:(UIView *)view {
    UIGraphicsBeginImageContextWithOptions(view.bounds.size, view.opaque, 0.0);
    [view.layer renderInContext:UIGraphicsGetCurrentContext()];
	
    UIImage * img = UIGraphicsGetImageFromCurrentImageContext();
	
    UIGraphicsEndImageContext();
	
    return img;
}

+ (UIImage *)imageWithColors:(NSArray *)colors {
	NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 64, 64)];
	view.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.8 alpha:0.7];
	view.opaque = NO;
	view.endColors = colors;
	return [NWComplexScriptCommandEditorViewController imageWithView:view];
}

- (CGRect)addBarButtonFrame {
	NSMutableArray* buttons = [[NSMutableArray alloc] init];
	for (UIControl* btn in self.navigationController.navigationBar.subviews)
		if ([btn isKindOfClass:[UIControl class]])
			[buttons addObject:btn];
	UIView* view = [buttons lastObject];
	if (view) {
		return [view convertRect:view.bounds toView:self.view];
	}
	return CGRectZero;
}

- (UIImage *)addFadeImage {
	NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 80, 80)];
	//view.borderWidth = 1;
	view.backgroundColor = [UIColor blackColor];
	view.endColors = [NWComplexScriptCommandEditorViewController defaultFadeCommand].colors;
	view.opaque = NO;
	
	return [NWComplexScriptCommandEditorViewController imageWithView:view];
}

- (UIImage *)addGradientImage {
	NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 80, 80)];
	//view.borderWidth = 1;
	view.backgroundColor = [UIColor blackColor];
	view.endColors = [NWComplexScriptCommandEditorViewController defaultGradientCommand].colors;
	view.opaque = NO;
	
	return [NWComplexScriptCommandEditorViewController imageWithView:view];
}

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
	obj.color = [UIColor orangeColor];
	obj.duration = 5;
	obj.parallel = YES;
	return obj;
}

- (NSArray *)addEffectImages {
	if (!_addEffectImages) {
		UIButton *fadeButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		fadeButton.frame = CGRectMake(0, 0, 80, 80);
		[fadeButton setBackgroundImage:[self addFadeImage] forState:UIControlStateNormal];
		[fadeButton setTitle:@"Fade" forState:UIControlStateNormal];
		
		UIButton *gradientButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
		gradientButton.frame = CGRectMake(0, 0, 80, 80);
		[gradientButton setBackgroundImage:[self addGradientImage] forState:UIControlStateNormal];
		[gradientButton setTitle:@"Gradient" forState:UIControlStateNormal];
		
		_addEffectImages = @[[NWComplexScriptCommandEditorViewController imageWithView:fadeButton], [NWComplexScriptCommandEditorViewController imageWithView:gradientButton]];
	}
	return _addEffectImages;
}

#pragma mark - BUTTONS
- (IBAction)addEffect:(UIBarButtonItem *)sender {
	[self.addEffectMenu buttonsWillAnimateFromButton:sender withFrame:[self addBarButtonFrame]  inView:self.view];
}



@end
