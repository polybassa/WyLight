//
//  NWEditGradientCommandViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWEditGradientCommandViewController.h"
#import "NWSetGradientScriptCommandObject.h"
#import "NWScriptObjectButton.h"
#import "KZColorPickerSwatchView.h"

@interface NWEditGradientCommandViewController ()
@property (strong, nonatomic) NWScriptObjectButton *gradientPreviewView;
@property (strong, nonatomic) KZColorPickerSwatchView *colorButton1;
@property (strong, nonatomic) KZColorPickerSwatchView *colorButton2;
@property (strong, nonatomic) NSString *colorToEdit;
@property (nonatomic) BOOL gestureLockForEditOffset;

@end

@implementation NWEditGradientCommandViewController

- (void)setup {	
	self.colorButton1 = [[KZColorPickerSwatchView alloc] initWithFrame:CGRectZero];
	[self.colorButton1 addTarget:self action:@selector(color1ButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:self.colorButton1];
	
	self.colorButton2 = [[KZColorPickerSwatchView alloc] initWithFrame:CGRectZero];
	[self.colorButton2 addTarget:self action:@selector(color2ButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:self.colorButton2];

	self.gradientPreviewView = [[NWScriptObjectButton alloc]initWithFrame:CGRectZero];
	[self.view addSubview:self.gradientPreviewView];
	self.gradientPreviewView.backgroundColor = [UIColor blackColor];
	
	UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchOnGradientView:)];
	[self.gradientPreviewView addGestureRecognizer:pinch];
	
	UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panOnGradientView:)];
	[self.gradientPreviewView addGestureRecognizer:pan];
}

- (void)viewWillLayoutSubviews
{
	[self fixLocations];
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	self.colorButton2.color = self.command.color2;
	self.gradientPreviewView.endColors = self.command.colors;
	self.colorButton1.color = self.command.color1;
}

- (void)fixLocations {
	//horizontal
		CGFloat totalWidth = self.view.bounds.size.width - 40.0;
		CGFloat totalHeight = self.view.bounds.size.height - 40.0;
		
		CGFloat gradientViewHeight = totalHeight - ((NSUInteger)totalHeight % 32);
		
		self.gradientPreviewView.frame = CGRectMake(20, 20, totalWidth - 60, gradientViewHeight);
		
        CGFloat colorButtonCellWidth = 40;
		CGFloat colorButtonCellHeight = gradientViewHeight / 2 - 20;
        
		self.colorButton1.frame = CGRectMake(totalWidth - 20, 20, colorButtonCellWidth, colorButtonCellHeight);
		self.colorButton2.frame = CGRectMake(totalWidth - 20, 20 + colorButtonCellHeight + 40, colorButtonCellWidth, colorButtonCellHeight);
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	self.command = [[NWSetGradientScriptCommandObject alloc] init];
	self.command.color1 = [UIColor blueColor];
	self.command.color2 = [UIColor greenColor];
	
	self.command.offset = 10;
	self.command.numberOfLeds = 10;

	[self setup];
}

- (IBAction)panOnGradientView:(UIPanGestureRecognizer *)gesture {
	
	CGFloat heightOfOneGradientStrip = self.gradientPreviewView.frame.size.height / [self.gradientPreviewView.endColors count];
	if (gesture.state == UIGestureRecognizerStateBegan) {
		//check if translation is in the current gradient area
		CGFloat gradientOffsetInView = self.command.offset * heightOfOneGradientStrip;
		CGFloat gradientEndHeightInView = (self.command.numberOfLeds + 1) * heightOfOneGradientStrip + gradientOffsetInView;
		CGPoint touchLocationInGradientView;
		if (self.gradientPreviewView.latestTouchBegan) {
			touchLocationInGradientView = [self.gradientPreviewView.latestTouchBegan locationInView:self.gradientPreviewView];
			
			if ((touchLocationInGradientView.y >= gradientOffsetInView) && (touchLocationInGradientView.y <= gradientEndHeightInView)) {
				self.gestureLockForEditOffset = YES;
			}
			else {
				self.gestureLockForEditOffset = NO;
			}
		}
	}
	if (gesture.state == UIGestureRecognizerStateChanged && self.gestureLockForEditOffset) {
		CGPoint translation = [gesture translationInView:self.gradientPreviewView];
		if (translation.y > 0 && translation.y > heightOfOneGradientStrip * 0.5) {
			self.command.offset = self.command.offset + 1;
			self.gradientPreviewView.endColors = self.command.colors;
			[gesture setTranslation:CGPointZero inView:self.gradientPreviewView];
		} else if (translation.y < 0 && translation.y < heightOfOneGradientStrip * -0.5) {
			self.command.offset = self.command.offset - 1;
			self.gradientPreviewView.endColors = self.command.colors;
			[gesture setTranslation:CGPointZero inView:self.gradientPreviewView];
		}

	}
	if (gesture.state == UIGestureRecognizerStateEnded || gesture.state == UIGestureRecognizerStateFailed || gesture.state == UIGestureRecognizerStatePossible || gesture.state == UIGestureRecognizerStateCancelled) {
		self.gestureLockForEditOffset = NO;
	}
}

- (IBAction)pinchOnGradientView:(UIPinchGestureRecognizer*)gesture {
	if ((gesture.state == UIGestureRecognizerStateChanged) ||
        (gesture.state == UIGestureRecognizerStateEnded)) {
		if (gesture.scale > 1) {
			self.command.numberOfLeds += 1;
			self.gradientPreviewView.endColors = self.command.colors;
		}
		if (gesture.scale < 1) {
			self.command.numberOfLeds -= 1;
			self.gradientPreviewView.endColors = self.command.colors;
		}
		gesture.scale = 1;
    }
}

- (IBAction)color2ButtonPressed:(id)sender {
	self.colorToEdit = @"color2";
	[self performSegueWithIdentifier:@"editColor:" sender:self];
}

- (IBAction)color1ButtonPressed:(id)sender {
	self.colorToEdit = @"color1";
	[self performSegueWithIdentifier:@"editColor:" sender:self];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.identifier isEqualToString:@"editColor:"]) {
		if ([segue.destinationViewController respondsToSelector:@selector(setSelectedColor:)]) {
			if ([self.colorToEdit isEqualToString:@"color2"]) {
				[segue.destinationViewController performSelector:@selector(setSelectedColor:) withObject:self.command.color2];
			}
			if ([self.colorToEdit isEqualToString:@"color1"]) {
				[segue.destinationViewController performSelector:@selector(setSelectedColor:) withObject:self.command.color1];
			}
		}
	}
}

- (void)unwindChangeColorCancel:(UIStoryboardSegue *)segue {
	
}

- (void)unwindChangeColorDone:(UIStoryboardSegue *)segue {
	if ([segue.sourceViewController respondsToSelector:@selector(selectedColor)]) {
		if ([self.colorToEdit isEqualToString:@"color2"]) {
			self.command.color2 = [segue.sourceViewController performSelector:@selector(selectedColor)];
		}
		if ([self.colorToEdit isEqualToString:@"color1"]) {
			self.command.color1 = [segue.sourceViewController performSelector:@selector(selectedColor)];
		}
	}
}

@end
