//
//  NWTestViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 16.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWTestViewController.h"
#import "NWScript.h"
#import "NWScriptObjectView.h"
#import "NWScriptCommandObject.h"
#import "WCWiflyControlWrapper.h"

@interface NWTestViewController ()

@property (nonatomic, strong) NWScriptObjectView *gradientView;
@property (nonatomic, strong) NWScript *script;
@property (nonatomic, strong) UISlider *slider;

@end

@implementation NWTestViewController

#define SCRIPT_KEY @"WyLightRemote.NWScriptViewController.script"

- (void)setup {
	self.gradientView = [[NWScriptObjectView alloc] initWithFrame:CGRectMake(10, 10, 250, 300)];
	[self.view addSubview:self.gradientView];
	[self setupGradientView];
	
	self.slider = [[UISlider alloc] initWithFrame:CGRectMake(10, 310, 250, 20)];
	self.slider.minimumValue = 0.1;
	self.slider.maximumValue = 1.0;
	self.slider.value = 0.5;
	
	[self.view addSubview:self.slider];
	
	UIButton *pressMe = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	pressMe.frame = CGRectMake(50, 250, 150, 44);
	[pressMe setTitle:@"PRESS ME" forState:UIControlStateNormal];
	[self.view addSubview:pressMe];
	[pressMe addTarget:self action:@selector(buttonPressed) forControlEvents:UIControlEventTouchUpInside];
}

- (void)setupGradientView {
	NSData *data = [[NSUserDefaults standardUserDefaults] objectForKey:SCRIPT_KEY];
	if (data) {
		self.script = [NSKeyedUnarchiver unarchiveObjectWithData:data];
	}
	
	self.gradientView.startColors = ((NWScriptCommandObject *)[self.script.scriptArray objectAtIndex:0]).colors;
	self.gradientView.endColors = self.gradientView.startColors;
}

- (void)buttonPressed {
	
	dispatch_queue_t scanQ = dispatch_queue_create("Scan Wifi Q", NULL);
	dispatch_async(scanQ, ^{
		if (self.controlHandle) {
			[self.controlHandle configurateWlanModuleChannelAsync:NO];
		}
	});
	
	/*
	static BOOL animationIsRunning;
	if (!animationIsRunning) {
		dispatch_queue_t animationQ = dispatch_queue_create("animationQ", NULL);
		dispatch_async(animationQ, ^{
			animationIsRunning = YES;
			
			for (NWScriptCommandObject *cmd in self.script.scriptArray) {
				dispatch_async(dispatch_get_main_queue(), ^{
					[self.gradientView setColorsAnimatedWithDuration:(cmd.duration / 100) * self.slider.value startColors:cmd.colors endColor:cmd.colors];
				});
				[NSThread sleepForTimeInterval:(cmd.duration / 100) * self.slider.value];
			}
			dispatch_async(dispatch_get_main_queue(), ^{
				[self.gradientView setColorsAnimatedWithDuration:(((NWScriptCommandObject *)[self.script.scriptArray objectAtIndex:0]).duration / 100) * self.slider.value
													 startColors:((NWScriptCommandObject *)[self.script.scriptArray objectAtIndex:0]).colors
														endColor:((NWScriptCommandObject *)[self.script.scriptArray objectAtIndex:0]).colors];
			});
			[NSThread sleepForTimeInterval:(((NWScriptCommandObject *)[self.script.scriptArray objectAtIndex:0]).duration / 100) * self.slider.value];
			animationIsRunning = NO;
		});
	}*/
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self setupGradientView];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	[self setup];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
