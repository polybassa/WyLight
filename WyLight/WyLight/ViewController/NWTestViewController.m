//
//  NWTestViewController.m
//  WyLight
//
//  Created by Nils Weiß on 24.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWTestViewController.h"
#import "NWScript.h"
#import "NWScriptObjectControl.h"
#import "NWScriptEffectCommandObject.h"
#import "NWTimeInfoView.h"

@interface NWTestViewController ()

@property (nonatomic, strong) NWScriptObjectControl *controlView;
@property (nonatomic, strong) NWTimeInfoView *timeInfo;

@end

@implementation NWTestViewController

- (void)setup
{
	NWScriptObjectControl *controlView = [[NWScriptObjectControl alloc] init];
	NWScript *script = [[NWScript alloc]init];
	controlView.endColors = ((NWScriptEffectCommandObject*)script.scriptArray.lastObject).colors;
	self.controlView = controlView;
	//self.controlView.quivering = YES;
	[self.view addSubview:controlView];
	
	self.timeInfo = [[NWTimeInfoView alloc] init];
	[self.view addSubview:self.timeInfo];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	[self setup];
}

- (void)viewDidLayoutSubviews {
	[super viewDidLayoutSubviews];
	self.controlView.frame = CGRectInset(self.view.frame, 0, 70);
	self.timeInfo.frame = self.controlView.frame;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
