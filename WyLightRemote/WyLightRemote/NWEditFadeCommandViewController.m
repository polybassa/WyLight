//
//  NWEditFadeCommandViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWEditFadeCommandViewController.h"
#import "NWScriptObjectView.h"
#import "KZColorPickerSwatchView.h"

@interface NWEditFadeCommandViewController ()
@property (strong, nonatomic) NWScriptObjectView *gradientPreviewView;
@property (strong, nonatomic) KZColorPickerSwatchView *colorButton1;
@property (nonatomic) uint32_t addressBeforPanBegan;

@end

@implementation NWEditFadeCommandViewController

- (void)setup {
	self.colorButton1 = [[KZColorPickerSwatchView alloc] initWithFrame:CGRectZero];
	[self.colorButton1 addTarget:self action:@selector(color1ButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
	[self.view addSubview:self.colorButton1];
	
	self.gradientPreviewView = [[NWScriptObjectView alloc]initWithFrame:CGRectZero];
	[self.view addSubview:self.gradientPreviewView];
	self.gradientPreviewView.backgroundColor = [UIColor blackColor];
	
	UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc]initWithTarget:self action:@selector(panOnGradientView:)];
	[self.gradientPreviewView addGestureRecognizer:pan];
	
	UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapOnGradientView:)];
	[self.gradientPreviewView addGestureRecognizer:tap];
}

- (void)viewWillLayoutSubviews
{
	[self fixLocations];
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	self.gradientPreviewView.endColors = self.command.colors;
	self.colorButton1.color = self.command.color;
}

- (void)fixLocations {

		CGFloat totalWidth = self.view.bounds.size.width - 40.0;
		CGFloat totalHeight = self.view.bounds.size.height - 40.0;
		
		CGFloat gradientViewHeight = totalHeight - ((NSUInteger)totalHeight % 32);
		
		self.gradientPreviewView.frame = CGRectMake(20, 20, totalWidth - 60, gradientViewHeight);
		
        CGFloat colorButtonCellWidth = 40;
		CGFloat colorButtonCellHeight = gradientViewHeight;
        
		self.colorButton1.frame = CGRectMake(totalWidth - 20, 20, colorButtonCellWidth, colorButtonCellHeight);
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	[self setup];
}

- (void)flipAddressBitsAtIndex:(NSUInteger)index
{
	uint32_t flipBitmask = 0x00000001 << index;
	uint32_t newAddressMask = self.command.address ^ flipBitmask;
	if ((flipBitmask & self.addressBeforPanBegan & self.command.address) || (flipBitmask & ~self.addressBeforPanBegan & ~self.command.address)) {
		self.command.address = newAddressMask;
		self.gradientPreviewView.endColors = self.command.colors;
	}
}

- (IBAction)tapOnGradientView:(UITapGestureRecognizer *)gesture {
	if (gesture.state == UIGestureRecognizerStateEnded) {
		self.addressBeforPanBegan = self.command.address;
		
		CGFloat heightOfOneGradientStrip = self.gradientPreviewView.frame.size.height / [self.gradientPreviewView.endColors count];
		CGPoint touchLocationInGradientView = [self.gradientPreviewView.latestTouchBegan locationInView:self.gradientPreviewView];
		
		[self flipAddressBitsAtIndex:(NSUInteger)(touchLocationInGradientView.y / heightOfOneGradientStrip)];
	}
}


- (IBAction)panOnGradientView:(UIPanGestureRecognizer *)gesture {
	CGFloat heightOfOneGradientStrip = self.gradientPreviewView.frame.size.height / [self.gradientPreviewView.endColors count];
	CGPoint translation = [gesture translationInView:self.gradientPreviewView];
	static CGPoint touchLocationInGradientView;
	
	if (gesture.state == UIGestureRecognizerStateBegan) {
		touchLocationInGradientView = [self.gradientPreviewView.latestTouchBegan locationInView:self.gradientPreviewView];
		self.addressBeforPanBegan = self.command.address;
	}
	if (gesture.state == UIGestureRecognizerStateChanged) {
		CGPoint currentPositionInGradientView = CGPointMake(translation.x + touchLocationInGradientView.x, translation.y + touchLocationInGradientView.y);
		CGFloat bitPosition = currentPositionInGradientView.y / heightOfOneGradientStrip;
		[self flipAddressBitsAtIndex:(NSUInteger)bitPosition];
		if (translation.y > 0 && bitPosition >= 1) {
			[self flipAddressBitsAtIndex:(NSUInteger)bitPosition - 1];
		} else if(translation.y < 0 && (NSUInteger)bitPosition < (self.command.colors.count - 1)) {
			[self flipAddressBitsAtIndex:(NSUInteger)bitPosition + 1];
		}
	}
}


- (IBAction)color1ButtonPressed:(id)sender {
	[self performSegueWithIdentifier:@"editColor:" sender:self];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.identifier isEqualToString:@"editColor:"]) {
		if ([segue.destinationViewController respondsToSelector:@selector(setSelectedColor:)]) {
				[segue.destinationViewController performSelector:@selector(setSelectedColor:) withObject:self.command.color];
		}
	}
}

- (void)unwindChangeColorCancel:(UIStoryboardSegue *)segue {
	
}

- (void)unwindChangeColorDone:(UIStoryboardSegue *)segue {
	if ([segue.sourceViewController respondsToSelector:@selector(selectedColor)]) {
		self.command.color = [segue.sourceViewController performSelector:@selector(selectedColor)];
	}
}

@end
