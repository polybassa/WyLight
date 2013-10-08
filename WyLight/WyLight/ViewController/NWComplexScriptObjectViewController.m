//
//  NWComplexScriptObjectViewController.m
//  WyLight
//
//  Created by Nils Weiß on 05.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWComplexScriptObjectViewController.h"
#import "NWScriptObjectControl.h"
#import "iCarousel.h"
#import "ALRadialMenu.h"
#import "NWSetFadeScriptCommandObject.h"
#import "NWSetGradientScriptCommandObject.h"
#import "NWDefaultColorPickerViewController.h"
#import "WCWiflyControlWrapper.h"
#import "TouchAndHoldButton.h"

enum EDITCOLORTARGET {
	fadeColor = 0,
	gradientColor1,
	gradientColor2
};

@interface NWComplexScriptObjectViewController () <iCarouselDataSource, iCarouselDelegate, ALRadialMenuDelegate, NWDefaultColorControllerDelegate>

@property (nonatomic, strong) iCarousel *carousel;
@property (nonatomic, strong) NSArray *colorImages;
@property (nonatomic, strong) NSArray *addEffectImages;
@property (nonatomic, strong) ALRadialMenu *color1Menu;
@property (nonatomic, strong) ALRadialMenu *color2Menu;
@property (nonatomic, strong) ALRadialMenu *addEffectMenu;
@property (nonatomic, strong) UIButton *color1Button;
@property (nonatomic, strong) UIButton *color2Button;
@property (nonatomic, strong) TouchAndHoldButton *gradientUpButton;
@property (nonatomic, strong) TouchAndHoldButton *gradientDownButton;
@property (nonatomic, strong) TouchAndHoldButton *gradientPlusButton;
@property (nonatomic, strong) TouchAndHoldButton *gradientMinusButton;
@property (nonatomic, strong) NWScriptObjectView *fadeEditViewPortrait;
@property (nonatomic, strong) NWScriptObjectView *fadeEditViewLandscape;
@property (nonatomic, assign) NSUInteger currentItemIndex;
@property (nonatomic, assign) enum EDITCOLORTARGET customColorTarget;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *addBarButton;
@property (nonatomic) BOOL sendInitialClearScript;
@property (nonatomic, assign) uint32_t addressMaskForFadeEditView;

@end

@implementation NWComplexScriptObjectViewController

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
	self.fadeEditViewPortrait.hidden = YES;
	self.fadeEditViewLandscape.hidden = YES;
	self.currentItemIndex = self.carousel.currentItemView.tag;
	[self.carousel reloadItemAtIndex:[self.carousel indexOfItemView:self.carousel.currentItemView] animated:YES];
	[super viewWillAppear:animated];
}

- (void)fixLocations {
	if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
		
		self.carousel.frame = CGRectMake(0, 60, self.view.bounds.size.width, self.view.bounds.size.height / 2);
		
		self.color1Button.frame = CGRectMake(self.view.bounds.size.width / 2 - 80, self.view.bounds.size.height / 2 + 60, 60, 40);
		
		self.color2Button.frame = CGRectMake(self.view.bounds.size.width / 2 + 20, self.view.bounds.size.height / 2 + 60, 60, 40);
		
		self.gradientPlusButton.frame = CGRectMake(self.view.bounds.size.width - 80, self.view.bounds.size.height / 2 + 150, 80, 80);

		self.gradientMinusButton.frame = CGRectMake(self.view.bounds.size.width - 160, self.view.bounds.size.height / 2 + 150, 80, 80);
		
		self.gradientUpButton.frame = CGRectMake(10, self.view.bounds.size.height / 2 + 150, 80, 80);

		self.gradientDownButton.frame = CGRectMake(90, self.view.bounds.size.height / 2 + 150, 80, 80);
		
		self.fadeEditViewPortrait.frame = CGRectMake(0, self.view.bounds.size.height / 2 + 130, self.view.bounds.size.width, self.view.bounds.size.height - (self.view.bounds.size.height / 2 + 130) - 10);
	}
	else {
		CGRect biggerFrame = self.tabBarController.view.frame;
		biggerFrame.size.height += self.tabBarController.tabBar.frame.size.height;
		self.tabBarController.view.frame = biggerFrame ;
		
		self.carousel.frame = CGRectMake(0, 50, self.view.bounds.size.width / 2, self.view.bounds.size.height - 50);
		
		self.color1Button.frame = CGRectMake(self.view.bounds.size.width / 2 + 40, self.view.bounds.size.height / 2 - 40, 60, 40);
		
		self.color2Button.frame = CGRectMake(self.view.bounds.size.width / 2 + 40, self.view.bounds.size.height / 2 + 40, 60, 40);
		
		self.gradientPlusButton.frame = CGRectMake(self.view.bounds.size.width / 2 + 150, self.view.bounds.size.height / 2 - 120, 70, 70);
		
		self.gradientMinusButton.frame = CGRectMake(self.view.bounds.size.width / 2 + 150, self.view.bounds.size.height / 2 - 50, 70, 70);
		
		self.gradientUpButton.frame = CGRectMake(self.view.bounds.size.width / 2 + 150, self.view.bounds.size.height / 2 + 20, 70, 70);
		
		self.gradientDownButton.frame = CGRectMake(self.view.bounds.size.width / 2 + 150, self.view.bounds.size.height / 2 + 100, 70, 70);
				
		self.fadeEditViewLandscape.frame = CGRectMake(self.view.bounds.size.width / 2 + 140, 50, 100, self.view.bounds.size.height - 50);
	}
}

- (void)setup {
	self.carousel = [[iCarousel alloc]initWithFrame:CGRectZero];
	self.carousel.dataSource = self;
	self.carousel.delegate = self;
	self.carousel.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	self.carousel.type = iCarouselTypeInvertedTimeMachine;
	self.carousel.bounces = NO;
	[self.view addSubview:self.carousel];
		
	self.color1Menu = [[ALRadialMenu alloc]init];
	self.color2Menu = [[ALRadialMenu alloc]init];
	self.color1Menu.delegate = self;
	self.color2Menu.delegate = self;
	self.addEffectMenu = [[ALRadialMenu alloc] init];
	self.addEffectMenu.delegate = self;
	
	self.color1Button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[self.color1Button addTarget:self action:@selector(colorButton1TouchUpInside:) forControlEvents:UIControlEventTouchUpInside];
	[self.color1Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[[UIColor lightGrayColor]]] forState:UIControlStateDisabled];
	[self.view addSubview:self.color1Button];
	
	self.color2Button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	[self.color2Button addTarget:self action:@selector(colorButton2TouchUpInside:) forControlEvents:UIControlEventTouchUpInside];
	[self.color2Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[[UIColor lightGrayColor]]] forState:UIControlStateDisabled];
	[self.view addSubview:self.color2Button];
	
	self.gradientDownButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
	[self.gradientDownButton setImage:[UIImage imageNamed:@"Down_Icon"] forState:UIControlStateNormal];
	[self.gradientDownButton addTarget:self action:@selector(downButtonTouchUpInside:) forTouchAndHoldControlEventWithTimeInterval:0.3];
	[self.gradientDownButton addTarget:self action:@selector(downButtonTouchUpInside:) forControlEvents:UIControlEventTouchDown];
	[self.view addSubview:self.gradientDownButton];
	
	self.gradientUpButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
	[self.gradientUpButton setImage:[UIImage imageNamed:@"Up_Icon"] forState:UIControlStateNormal];
	[self.gradientUpButton addTarget:self action:@selector(upButtonTouchUpInside:) forTouchAndHoldControlEventWithTimeInterval:0.3];
	[self.gradientUpButton addTarget:self action:@selector(upButtonTouchUpInside:) forControlEvents:UIControlEventTouchDown];
	[self.view addSubview:self.gradientUpButton];
	
	self.gradientMinusButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
	[self.gradientMinusButton setImage:[UIImage imageNamed:@"Minus_Icon"] forState:UIControlStateNormal];
	[self.gradientMinusButton addTarget:self action:@selector(minusButtonTouchUpInside:) forTouchAndHoldControlEventWithTimeInterval:0.3];
	[self.gradientMinusButton addTarget:self action:@selector(minusButtonTouchUpInside:) forControlEvents:UIControlEventTouchDown];
	[self.view addSubview:self.gradientMinusButton];
	
	self.gradientPlusButton = [TouchAndHoldButton buttonWithType:UIButtonTypeRoundedRect];
	[self.gradientPlusButton setImage:[UIImage imageNamed:@"Plus_Icon"] forState:UIControlStateNormal];
	[self.gradientPlusButton addTarget:self action:@selector(plusButtonTouchUpInside:) forTouchAndHoldControlEventWithTimeInterval:0.3];
	[self.gradientPlusButton addTarget:self action:@selector(plusButtonTouchUpInside:) forControlEvents:UIControlEventTouchDown];
	[self.view addSubview:self.gradientPlusButton];
	
	UITapGestureRecognizer *tapPortrait = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapOnFadeEditViewPortrait:)];
	UITapGestureRecognizer *tapLandscape = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapOnFadeEditViewLandscape:)];
	
	self.fadeEditViewPortrait = [[NWScriptObjectView alloc] initWithFrame:CGRectZero];
	self.fadeEditViewPortrait.backgroundColor = [UIColor colorWithRed:0.4 green:0.4 blue:0.4 alpha:0.2];
	[self.fadeEditViewPortrait addGestureRecognizer:tapPortrait];
	self.fadeEditViewPortrait.orientation = 90 * M_PI / 180;
	[self.view addSubview:self.fadeEditViewPortrait];
	
	self.fadeEditViewLandscape = [[NWScriptObjectView alloc] initWithFrame:CGRectZero];
	self.fadeEditViewLandscape.backgroundColor = [UIColor colorWithRed:0.4 green:0.4 blue:0.4 alpha:0.2];
	[self.fadeEditViewLandscape addGestureRecognizer:tapLandscape];
	[self.view addSubview:self.fadeEditViewLandscape];
	
	[self.view bringSubviewToFront:self.carousel];
}

- (void)updateView {
	id currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
		NWSetFadeScriptCommandObject *currentFadeCommand = (NWSetFadeScriptCommandObject *)currentCommand;
		[self.color2Menu itemsWillDisapearIntoButton:self.color2Button];
		
		currentFadeCommand.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0];
		
		self.color2Button.hidden = YES;
		self.gradientUpButton.hidden = YES;
		self.gradientPlusButton.hidden = YES;
		self.gradientMinusButton.hidden = YES;
		self.gradientDownButton.hidden = YES;
		
		if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
			self.fadeEditViewPortrait.endColors = self.fadeEditViewPortrait.startColors = currentFadeCommand.colors;
			self.fadeEditViewLandscape.endColors = self.fadeEditViewLandscape.startColors = currentFadeCommand.colors;
			self.fadeEditViewPortrait.hidden = NO;
			self.fadeEditViewLandscape.hidden = YES;
		} else {
			self.fadeEditViewPortrait.endColors = self.fadeEditViewPortrait.startColors = currentFadeCommand.colors;
			self.fadeEditViewLandscape.endColors = self.fadeEditViewLandscape.startColors = currentFadeCommand.colors;
			self.fadeEditViewLandscape.hidden = NO;
			self.fadeEditViewPortrait.hidden = YES;
		}
		[self.color1Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[currentFadeCommand.color]] forState:UIControlStateNormal];
		
	} else if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		NWSetGradientScriptCommandObject *currentGradientCommand = (NWSetGradientScriptCommandObject *)currentCommand;
		
		self.fadeEditViewPortrait.hidden = YES;
		self.fadeEditViewLandscape.hidden = YES;
		
		self.gradientDownButton.hidden = NO;
		self.gradientMinusButton.hidden = NO;
		self.gradientPlusButton.hidden = NO;
		self.gradientUpButton.hidden = NO;
		
		if (currentGradientCommand.numberOfLeds <= 1) {
			self.gradientMinusButton.enabled = NO;
		} else {
			self.gradientMinusButton.enabled = YES;
		}
		
		if (currentGradientCommand.numberOfLeds >= [NWSetGradientScriptCommandObject maximalNumberOfLeds]) {
			self.gradientPlusButton.enabled = NO;
		} else {
			self.gradientPlusButton.enabled = YES;
		}
		
		if (currentGradientCommand.address & 0x80000000) {
			self.gradientDownButton.enabled = NO;
		} else {
			self.gradientDownButton.enabled = YES;
		}
		
		if (currentGradientCommand.address & 0x00000001) {
			self.gradientUpButton.enabled = NO;
		} else {
			self.gradientUpButton.enabled = YES;
		}
		
		self.color2Button.hidden = NO;
		[self.color2Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[currentGradientCommand.color2]] forState:UIControlStateNormal];
		[self.color1Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[currentGradientCommand.color1]] forState:UIControlStateNormal];
	}
	[self sendPreview];
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	[self updateView];
	[super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
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
	[self updateView];
}

#pragma mark - iCarousel methods

- (NSUInteger)numberOfItemsInCarousel:(iCarousel *)carousel {
	return self.command.scriptObjects.count;
}

- (UIView *)carousel:(iCarousel *)carousel viewForItemAtIndex:(NSUInteger)index reusingView:(UIView *)view {
	//create new view if no view is available for recycling
    if (view == nil || ![view isKindOfClass:[NWScriptObjectControl class]])
    {
        view = [[NWScriptObjectControl alloc] initWithFrame:CGRectMake(0, 0, 200.0f, 200.0f)];
        view.contentMode = UIViewContentModeCenter;
		view.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.9 alpha:0.2];
    }
    
    view.tag = self.command.scriptObjects.count - 1 - index;
	if ([view isKindOfClass:[NWScriptObjectControl class]]) {
		NWScriptObjectControl* scriptView = (NWScriptObjectControl *)view;
		id<NWDrawableCommand> command = [self.command.scriptObjects objectAtIndex:view.tag];
		[command setBackgroundColor:[UIColor clearColor]];
		scriptView.endColors = [command colors];
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
}

- (CGFloat)carousel:(iCarousel *)carousel valueForOption:(iCarouselOption)option withDefault:(CGFloat)value {
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
		case iCarouselOptionTilt:
		{
			//add a bit of spacing between the item views
            return value;
		}
		default:
        {
            return value;
        }
    }
}

- (void)carouselCurrentItemIndexDidChange:(iCarousel *)carousel {
	self.currentItemIndex = carousel.currentItemView.tag;
}

#pragma mark - GESTURE RECOGNIZER CALLBACKS
- (void)longPressOnScriptObjectControl:(UILongPressGestureRecognizer *)gesture {
	if ([gesture.view isKindOfClass:[NWScriptObjectControl class]] && gesture.view == self.carousel.currentItemView) {
		NWScriptObjectControl* scriptView = (NWScriptObjectControl *)gesture.view;
		scriptView.quivering = YES;
		self.carousel.scrollEnabled = NO;
	}
}

- (void)tapOnScriptObjectControl:(UITapGestureRecognizer *)gesture {
	if ([gesture.view isKindOfClass:[NWScriptObjectControl class]]
		&& gesture.view == self.carousel.currentItemView &&
		self.carousel.scrollEnabled == NO) {
		NWScriptObjectControl* scriptView = (NWScriptObjectControl *)gesture.view;
		scriptView.quivering = NO;
		self.carousel.scrollEnabled = YES;
	}
}

- (void)swipeUpOnScriptObjectControl:(UISwipeGestureRecognizer *)gesture {
	if ([gesture.view isKindOfClass:[NWScriptObjectControl class]]
		&& gesture.view == self.carousel.currentItemView &&
		self.carousel.scrollEnabled == NO) {
		
		if (self.command.scriptObjects.count > 1) {
			NSUInteger index = gesture.view.tag;
			[self.command.scriptObjects removeObjectAtIndex:index];
			[self.carousel removeItemAtIndex:self.carousel.currentItemIndex animated:YES];
		}
		else {
			NWScriptObjectControl* scriptView = (NWScriptObjectControl *)gesture.view;
			scriptView.quivering = NO;
		}
		self.carousel.scrollEnabled = YES;
		self.currentItemIndex = self.carousel.currentItemView.tag;
	}
}

#pragma mark - RADIAL MENU DELEGATE
- (NSInteger)numberOfItemsInRadialMenu:(ALRadialMenu *)radialMenu {
	if (radialMenu == self.addEffectMenu) {
		return self.addEffectImages.count;
	} else {
		return self.colorImages.count;
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
		return self.colorImages[index - 1];
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
	} else {
		
		[radialMenu itemsWillDisapearIntoButton:nil];
		index--;
		
		if (index >= [[NWComplexScriptObjectViewController allColors] count]) {
			if ([self.command.scriptObjects[self.currentItemIndex] isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
				self.customColorTarget = fadeColor;
			} else if ([self.command.scriptObjects[self.currentItemIndex] isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
				if (radialMenu == self.color1Menu) {
					self.customColorTarget = gradientColor1;
				} else {
					self.customColorTarget = gradientColor2;
				}
			}
				
			//SEGUE
			double delayInSeconds = 0.9;
			dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
			dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
				[self performSegueWithIdentifier:@"getCustomColor:" sender:self];
			});
		} else {
			id currentCommand = self.command.scriptObjects[self.currentItemIndex];
			UIColor *newColor = [[NWComplexScriptObjectViewController allColors] objectAtIndex:index];
			if ([currentCommand isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
				NWSetFadeScriptCommandObject *currentFadeCommand = (NWSetFadeScriptCommandObject *)currentCommand;
				currentFadeCommand.color = newColor;
			} else if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
				NWSetGradientScriptCommandObject *currentGradientCommand = (NWSetGradientScriptCommandObject *)currentCommand;
				if (radialMenu == self.color1Menu) {
					currentGradientCommand.color1 = newColor;
				} else {
					currentGradientCommand.color2 = newColor;
				}
			}
			[self.carousel reloadItemAtIndex:[self.carousel indexOfItemView:self.carousel.currentItemView] animated:YES];
			[self updateView];
		}
	}
}

#pragma mark - Add Commands
- (void)addFadeCommand {
	[self.command.scriptObjects addObject:[NWComplexScriptObjectViewController defaultFadeCommand]];
	[self.carousel reloadData];
	double delayInSeconds = 0.5;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
	dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
		self.currentItemIndex = self.carousel.currentItemView.tag;
	});
}

- (void)addGradientCommand {
	[self.command.scriptObjects addObject:[NWComplexScriptObjectViewController defaultGradientCommand]];
	[self.carousel reloadData];
	double delayInSeconds = 0.5;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
	dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
		self.currentItemIndex = self.carousel.currentItemView.tag;
	});}

#pragma mark - Default Color Picker Delegate
- (void)defaultColorController:(NWDefaultColorPickerViewController *)controller didChangeColor:(UIColor *)color {
	switch (self.customColorTarget) {
		case fadeColor:
			((NWSetFadeScriptCommandObject *)self.command.scriptObjects[self.currentItemIndex]).color = color;
			break;
		case gradientColor1:
			((NWSetGradientScriptCommandObject *)self.command.scriptObjects[self.currentItemIndex]).color1 = color;
			break;
		case gradientColor2:
			((NWSetGradientScriptCommandObject *)self.command.scriptObjects[self.currentItemIndex]).color2 = color;
			break;
		default:
			break;
	}
}

#pragma mark - SEGUES

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.destinationViewController respondsToSelector:@selector(setSelectedColor:)]) {
		[segue.destinationViewController performSelector:@selector(setSelectedColor:) withObject:[UIColor whiteColor]];
	}
	
	if ([segue.destinationViewController isKindOfClass:[NWDefaultColorPickerViewController class]]) {
		((NWDefaultColorPickerViewController *)segue.destinationViewController).colorPickerDelegate = self;
	}
}

#pragma mark - HELPERS
+ (UIImage *)imageWithView:(UIView *)view {
    UIGraphicsBeginImageContextWithOptions(view.bounds.size, view.opaque, 0.0);
    [view.layer renderInContext:UIGraphicsGetCurrentContext()];
	
    UIImage * img = UIGraphicsGetImageFromCurrentImageContext();
	
    UIGraphicsEndImageContext();
	
    return img;
}

+ (NSArray *)allColors {
	return @[[UIColor greenColor], [UIColor yellowColor], [UIColor orangeColor], [UIColor redColor], [UIColor magentaColor], [UIColor purpleColor], [UIColor cyanColor],  [UIColor blueColor], [UIColor colorWithRed:1 green:1 blue:1 alpha:1]];
}

- (NSArray *)colorImages {
	if (!_colorImages) {
		NSMutableArray *tempArray = [[NSMutableArray alloc]init];
		
		for (UIColor *color in [NWComplexScriptObjectViewController allColors]) {
			[tempArray addObject:[NWComplexScriptObjectViewController imageWithColors:@[color]]];
		}
		
		[tempArray addObject:[NWComplexScriptObjectViewController imageWithColors:[NWComplexScriptObjectViewController allColors]]];
		_colorImages = tempArray;
	}
	return _colorImages;
}

+ (UIImage *)imageWithColors:(NSArray *)colors {
	NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 64, 64)];
	view.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:0.8 alpha:0.7];
	view.opaque = NO;
	view.endColors = colors;
	return [NWComplexScriptObjectViewController imageWithView:view];
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
	view.endColors = [NWComplexScriptObjectViewController defaultFadeCommand].colors;
	view.opaque = NO;
	
	return [NWComplexScriptObjectViewController imageWithView:view];
}

- (UIImage *)addGradientImage {
	NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 80, 80)];
	//view.borderWidth = 1;
	view.backgroundColor = [UIColor blackColor];
	view.endColors = [NWComplexScriptObjectViewController defaultGradientCommand].colors;
	view.opaque = NO;
	
	return [NWComplexScriptObjectViewController imageWithView:view];
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
		
		_addEffectImages = @[[NWComplexScriptObjectViewController imageWithView:fadeButton], [NWComplexScriptObjectViewController imageWithView:gradientButton]];
	}
	return _addEffectImages;
}

#pragma mark - BUTTONS
- (IBAction)colorButton1TouchUpInside:(UIButton *)sender {
	[self.color1Menu buttonsWillAnimateFromButton:sender withFrame:sender.frame inView:self.view];
}

- (IBAction)colorButton2TouchUpInside:(UIButton *)sender {
	[self.color2Menu buttonsWillAnimateFromButton:sender withFrame:sender.frame inView:self.view];
}

- (IBAction)addEffect:(UIBarButtonItem *)sender {
	[self.addEffectMenu buttonsWillAnimateFromButton:sender withFrame:[self addBarButtonFrame]  inView:self.view];
}

- (IBAction)upButtonTouchUpInside:(id)sender {
	NWSetGradientScriptCommandObject *currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		currentCommand.offset -= 1;
		[self.carousel reloadItemAtIndex:[self.carousel indexOfItemView:self.carousel.currentItemView] animated:YES];
		[self updateView];
	}
}

- (IBAction)downButtonTouchUpInside:(id)sender {
	NWSetGradientScriptCommandObject *currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		currentCommand.offset += 1;
		[self.carousel reloadItemAtIndex:[self.carousel indexOfItemView:self.carousel.currentItemView] animated:YES];
		[self updateView];
	}
}

- (IBAction)plusButtonTouchUpInside:(id)sender {
	NWSetGradientScriptCommandObject *currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		currentCommand.numberOfLeds += 1;
		[self.carousel reloadItemAtIndex:[self.carousel indexOfItemView:self.carousel.currentItemView] animated:YES];
		[self updateView];
	}
}

- (IBAction)minusButtonTouchUpInside:(id)sender {
	NWSetGradientScriptCommandObject *currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		currentCommand.numberOfLeds -= 1;
		[self.carousel reloadItemAtIndex:[self.carousel indexOfItemView:self.carousel.currentItemView] animated:YES];
		[self updateView];
	}
}

#pragma mark - FADE EDIT
- (IBAction)tapOnFadeEditViewPortrait:(UITapGestureRecognizer *)sender {
	if (sender.state == UIGestureRecognizerStateEnded) {
		id currentCommand = self.command.scriptObjects[self.currentItemIndex];
		
		if ([currentCommand isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
			NWSetFadeScriptCommandObject *currentFadeCommand = (NWSetFadeScriptCommandObject *)currentCommand;
			self.addressMaskForFadeEditView = currentFadeCommand.address;
		}
		CGFloat heigthOfOneGradientStrip = self.fadeEditViewPortrait.bounds.size.height / 8;
		CGPoint touchLocationInGradientView = [self.fadeEditViewPortrait.latestTouchBegan locationInView:self.fadeEditViewPortrait];
		[self flipAddressBitsAtIndex:(NSUInteger)(touchLocationInGradientView.y / heigthOfOneGradientStrip)];
	}
}

- (IBAction)tapOnFadeEditViewLandscape:(UITapGestureRecognizer *)sender {
	if (sender.state == UIGestureRecognizerStateEnded) {
		id currentCommand = self.command.scriptObjects[self.currentItemIndex];
		
		if ([currentCommand isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
			NWSetFadeScriptCommandObject *currentFadeCommand = (NWSetFadeScriptCommandObject *)currentCommand;
			self.addressMaskForFadeEditView = currentFadeCommand.address;
		}
		CGFloat heigthOfOneGradientStrip = self.fadeEditViewLandscape.bounds.size.height / 8;
		CGPoint touchLocationInGradientView = [self.fadeEditViewLandscape.latestTouchBegan locationInView:self.fadeEditViewLandscape];
		[self flipAddressBitsAtIndex:(NSUInteger)(touchLocationInGradientView.y / heigthOfOneGradientStrip)];
	}
}

- (void)flipAddressBitsAtIndex:(NSUInteger)index {
	id currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
		NWSetFadeScriptCommandObject *currentFadeCommand = (NWSetFadeScriptCommandObject *)currentCommand;

		uint32_t flipBitmask = 0x0000000f << index * 4;
		uint32_t newAddressMask = currentFadeCommand.address ^ flipBitmask;
		
		if ((flipBitmask & self.addressMaskForFadeEditView & currentFadeCommand.address) || (flipBitmask & ~self.addressMaskForFadeEditView & ~currentFadeCommand.address)) {
			currentFadeCommand.address = newAddressMask;
			[self.carousel reloadItemAtIndex:[self.carousel indexOfItemView:self.carousel.currentItemView] animated:YES];
			[self updateView];
		}
	}
}

@end
