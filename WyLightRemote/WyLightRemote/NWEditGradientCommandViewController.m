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

@interface NWEditGradientCommandViewController ()
@property (weak, nonatomic) IBOutlet NWScriptObjectButton *gradientPreviewView;
@property (weak, nonatomic) IBOutlet UIStepper *durationStepper;
@property (weak, nonatomic) IBOutlet UILabel *currentDurationLabel;
@property (weak, nonatomic) IBOutlet UIButton *color1Button;
@property (weak, nonatomic) IBOutlet UIButton *color2Button;
@property (strong, nonatomic) NSString *colorToEdit;
@property (nonatomic) BOOL gestureLockForEditOffset;

@end

@implementation NWEditGradientCommandViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	//init command only for testing purpose
	self.command = [[NWSetGradientScriptCommandObject alloc]init];
	self.command.color1 = [[UIColor greenColor] copy];
	self.command.color2 = [[UIColor redColor] copy];
	self.command.duration = 100;
	self.command.offset = 0;
	self.command.numberOfLeds = 32;
	
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	self.command.backgroundColor = [UIColor blackColor];
	
	self.durationStepper.value = self.command.duration / 100;
	self.currentDurationLabel.text = [NSString stringWithFormat:@"Duration: %d s", self.command.duration / 100];
	
	self.color1Button.titleLabel.backgroundColor = self.command.color1;
	self.color2Button.titleLabel.backgroundColor = self.command.color2;
	self.gradientPreviewView.backgroundColor = [UIColor blackColor];
	self.gradientPreviewView.endColors = self.command.colors;
	
	UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchOnGradientView:)];
	[self.gradientPreviewView addGestureRecognizer:pinch];
	
	UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panOnGradientView:)];
	[self.gradientPreviewView addGestureRecognizer:pan];
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

- (IBAction)durationChanged:(UIStepper *)sender {
	
	self.command.duration = (unsigned int)(sender.value * 100);
	self.currentDurationLabel.text = [NSString stringWithFormat:@"Duration: %d s", self.command.duration / 100];
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
			self.command.color2 = [segue.sourceViewController performSelector:@selector(selectedColor)];
		}
	}
}

@end
