//
//  NWEditComplexScriptObjectViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 15.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWEditComplexScriptObjectViewController.h"
#import "NWScriptObjectButton.h"
#import "NWSetFadeScriptCommandObject.h"
#import "NWSetGradientScriptCommandObject.h"

@interface NWEditComplexScriptObjectViewController ()
@property (nonatomic, strong) NWScriptObjectButton *gradientPreviewView;
@end

@implementation NWEditComplexScriptObjectViewController

- (void)setup {
	self.gradientPreviewView = [[NWScriptObjectButton alloc]initWithFrame:CGRectZero];
	[self.view addSubview:self.gradientPreviewView];
	self.gradientPreviewView.backgroundColor = [UIColor blackColor];
	
}

- (void)viewWillLayoutSubviews
{
	[self fixLocations];
}


- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	self.gradientPreviewView.endColors = self.command.colors;
}


- (void)fixLocations {
	//horizontal
	CGFloat totalWidth = self.view.bounds.size.width - 40.0;
	CGFloat totalHeight = self.view.bounds.size.height - 40.0;
	
	CGFloat gradientViewHeight = totalHeight - ((NSUInteger)totalHeight % 32);
	
	self.gradientPreviewView.frame = CGRectMake(20, 20, totalWidth - 60, gradientViewHeight);
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	//TESTCODE
	self.command = [[NWComplexScriptCommandObject alloc] init];
	
	{
		NWSetFadeScriptCommandObject *obj = [[NWSetFadeScriptCommandObject alloc] init];
		obj.address = 0xff0000ff;
		obj.color = [UIColor redColor];
	
		[self.command.itsScriptObjects addObject:obj];
	}
	NWSetGradientScriptCommandObject *obj = [[NWSetGradientScriptCommandObject alloc] init];
	obj.color1 = [UIColor blueColor];
	obj.color2 = [UIColor greenColor];
	
	obj.offset = 10;
	obj.numberOfLeds = 10;
	[self.command.itsScriptObjects addObject: obj];
	
	[self setup];
}


- (void)unwindEditScriptObjectCancel:(UIStoryboardSegue *)segue {
	
}

- (void)unwindEditScriptObjectDone:(UIStoryboardSegue *)segue {
/*	if ([segue.sourceViewController respondsToSelector:@selector(selectedColor)]) {
		if ([self.colorToEdit isEqualToString:@"color2"]) {
			self.command.color2 = [segue.sourceViewController performSelector:@selector(selectedColor)];
		}
		if ([self.colorToEdit isEqualToString:@"color1"]) {
			self.command.color1 = [segue.sourceViewController performSelector:@selector(selectedColor)];
		}
	}*/
}



@end
