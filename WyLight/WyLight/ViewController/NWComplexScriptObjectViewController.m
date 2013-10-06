//
//  NWComplexScriptObjectViewController.m
//  WyLight
//
//  Created by Nils Weiß on 05.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWComplexScriptObjectViewController.h"
#import "NWScriptObjectView.h"
#import "iCarousel.h"
#import "ALRadialMenu.h"
#import "NWSetFadeScriptCommandObject.h"
#import "NWSetGradientScriptCommandObject.h"
#import "NWDefaultColorPickerViewController.h"

@interface NWComplexScriptObjectViewController () <iCarouselDataSource, iCarouselDelegate, ALRadialMenuDelegate>

@property (weak, nonatomic) IBOutlet iCarousel *carousel;
@property (nonatomic, strong) NSArray *colorImages;
@property (nonatomic, strong) ALRadialMenu *color1Menu;
@property (nonatomic, strong) ALRadialMenu *color2Menu;
@property (weak, nonatomic) IBOutlet UIButton *color1Button;
@property (weak, nonatomic) IBOutlet UIButton *color2Button;
@property (nonatomic, assign) NSUInteger currentItemIndex;
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
	self.currentItemIndex = self.carousel.currentItemView.tag;
}

- (void)fixLocations {
	if (self.view.bounds.size.height > self.view.bounds.size.width) {   //horizontal
	}
	else {
		CGRect biggerFrame = self.tabBarController.view.frame;
		biggerFrame.size.height += self.tabBarController.tabBar.frame.size.height;
		self.tabBarController.view.frame = biggerFrame ;
	}
}

- (void)setup {
	self.carousel.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	self.carousel.type = iCarouselTypeInvertedTimeMachine;
	self.carousel.bounces = NO;
		
	self.color1Menu = [[ALRadialMenu alloc]init];
	self.color2Menu = [[ALRadialMenu alloc]init];
	self.color1Menu.delegate = self;
	self.color2Menu.delegate = self;
}

- (void)updateView {
	id currentCommand = self.command.scriptObjects[self.currentItemIndex];
	if ([currentCommand isKindOfClass:[NWSetFadeScriptCommandObject class]]) {
		NWSetFadeScriptCommandObject *currentFadeCommand = (NWSetFadeScriptCommandObject *)currentCommand;
		
		[self.color2Menu itemsWillDisapearIntoButton:self.color2Button];
		[self.color2Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[[UIColor lightGrayColor]]] forState:UIControlStateDisabled];
		self.color2Button.enabled = NO;
		
		[self.color1Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[currentFadeCommand.color]] forState:UIControlStateNormal];
		
	} else if ([currentCommand isKindOfClass:[NWSetGradientScriptCommandObject class]]) {
		NWSetGradientScriptCommandObject *currentGradientCommand = (NWSetGradientScriptCommandObject *)currentCommand;
		
		self.color2Button.enabled = YES;
		[self.color2Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[currentGradientCommand.color2]] forState:UIControlStateNormal];
		[self.color1Button setBackgroundImage:[NWComplexScriptObjectViewController imageWithColors:@[currentGradientCommand.color1]] forState:UIControlStateNormal];
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
    if (view == nil || ![view isKindOfClass:[NWScriptObjectView class]])
    {
        view = [[NWScriptObjectView alloc] initWithFrame:CGRectMake(0, 0, 200.0f, 200.0f)];
        view.contentMode = UIViewContentModeCenter;
		view.backgroundColor = [UIColor colorWithRed:0.8 green:0.8 blue:1.0 alpha:0.1];
    }
    
    view.tag = self.command.scriptObjects.count - 1 - index;
	if ([view isKindOfClass:[NWScriptObjectView class]]) {
		NWScriptObjectView* scriptView = (NWScriptObjectView *)view;
		id<NWDrawableCommand> command = [self.command.scriptObjects objectAtIndex:view.tag];
		[command setBackgroundColor:[UIColor clearColor]];
		scriptView.endColors = [command colors];
		scriptView.cornerRadius = 5.0;
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

#pragma mark - RADIAL MENU DELEGATE
- (NSInteger)numberOfItemsInRadialMenu:(ALRadialMenu *)radialMenu {
	return self.colorImages.count;
}

- (NSInteger)arcStartForRadialMenu:(ALRadialMenu *)radialMenu {
	return 0;
}

- (NSInteger)arcSizeForRadialMenu:(ALRadialMenu *)radialMenu {
	return 360;
}

- (NSInteger)arcRadiusForRadialMenu:(ALRadialMenu *)radialMenu {
	return 70;
}

- (float)buttonSizeForRadialMenu:(ALRadialMenu *)radialMenu {
	return 44;
}

- (UIImage *)radialMenu:(ALRadialMenu *)radialMenu imageForIndex:(NSInteger)index {
	return self.colorImages[index - 1];
}

- (void)radialMenu:(ALRadialMenu *)radialMenu didSelectItemAtIndex:(NSInteger)index {
	[radialMenu itemsWillDisapearIntoButton:nil];
	index--;
	
	if (index >= [[NWComplexScriptObjectViewController allColors] count]) {
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
		[self updateView];
		[self.carousel reloadItemAtIndex:[self.carousel indexOfItemView:self.carousel.currentItemView] animated:YES];
	}
}

#pragma mark - SEGUES

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.destinationViewController respondsToSelector:@selector(setSelectedColor:)]) {
		[segue.destinationViewController performSelector:@selector(setSelectedColor:) withObject:[UIColor whiteColor]];
	}
	
}

#pragma mark - COLOR BUTTONS
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

- (IBAction)colorButton1TouchUpInside:(UIButton *)sender {
	[self.color1Menu buttonsWillAnimateFromButton:sender withFrame:sender.frame inView:self.view];
}

- (IBAction)colorButton2TouchUpInside:(UIButton *)sender {
	[self.color2Menu buttonsWillAnimateFromButton:sender withFrame:sender.frame inView:self.view];
}

+ (UIImage *)imageWithColors:(NSArray *)colors {
	NWScriptObjectView *view = [[NWScriptObjectView alloc]initWithFrame:CGRectMake(0, 0, 64, 64)];
	view.backgroundColor = [UIColor whiteColor];
	view.opaque = NO;
	view.endColors = colors;
	return [NWComplexScriptObjectViewController imageWithView:view];
}





@end
