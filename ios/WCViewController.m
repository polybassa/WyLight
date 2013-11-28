//
//  WCViewController.m
//  WyLightExample
//
//  Created by Nils Weiß on 11.06.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "WCViewController.h"
#import "WCWiflyControlWrapper.h"
#import "WCEndpoint.h"

@interface WCViewController ()

@end

@implementation WCViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
	self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
	if(self) {}
	return self;
}

- (void)viewDidLoad
{
	[super viewDidLoad];
	// Do any additional setup after loading the view.
	[self setLedControllerArray:[[NSMutableArray alloc]init]];

	for(WCEndpoint *endpoint in [self targetEndpoints]) {
		WCWiflyControlWrapper *temporaryController = [[WCWiflyControlWrapper alloc]initWithIP:[endpoint ipAdress] port:[endpoint port]];
		[temporaryController setDelegate:self];
		[[self ledControllerArray] addObject:temporaryController];
	}
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
	// Dispose of any resources that can be recreated.
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];

	for(WCWiflyControlWrapper *controller in [self ledControllerArray]) {
		[controller disconnect];
	}
}

#pragma mark - DELEGATE METHODES

- (void) fatalErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber *)errorCode
{
	NSLog(@"FatalError: ErrorCode = %d\n", [errorCode unsignedIntValue]);
	[sender disconnect];
	[[self ledControllerArray]removeObject:sender];
	[[self delegate] fatalErrorOccured:nil errorCode:errorCode];
}

- (void) scriptFullErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber *)errorCode
{
	NSLog(@"ScriptFullError - Cleared Scriptbuffer automatically!\n");
	[sender clearScript];
	[[self delegate] scriptFullErrorOccured:sender errorCode:errorCode];
}

- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender
{
	NSLog(@"WiflyControlHasDisconnected\n");
	[sender disconnect];
	[[self ledControllerArray]removeObject:sender];
	[[self delegate] wiflyControlHasDisconnected:nil];

}


@end
