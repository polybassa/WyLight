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

@interface NWTestViewController ()

@end

@implementation NWTestViewController

- (void)setup
{
	NWScriptObjectControl *controlView = [[NWScriptObjectControl alloc] init];
	NWScript *script = [[NWScript alloc]init];
	controlView.endColors = script.scriptArray.lastObject.colors;
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
