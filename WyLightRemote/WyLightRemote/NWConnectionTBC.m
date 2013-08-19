//
//  NWConnectionTBC.m
//  WyLightRemote
//
//  Created by Nils Weiß on 06.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWConnectionTBC.h"
#import "WCWiflyControlWrapper.h"
#import "WCEndpoint.h"
#import "NWConnectionViewController.h"

@interface NWConnectionTBC ()
@property (nonatomic, strong) WCWiflyControlWrapper* controlHandle;
@end

@implementation NWConnectionTBC

- (void)setEndpoint:(WCEndpoint *)endpoint
{
	__block UIAlertView *connectingView = [[UIAlertView alloc] initWithTitle:@"Connecting" message:nil delegate:self cancelButtonTitle:nil otherButtonTitles:nil, nil];
	[connectingView show];
	UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
	spinner.center = CGPointMake(connectingView.bounds.size.width / 2, connectingView.bounds.size.height - 50);
	[spinner startAnimating];
	[connectingView addSubview:spinner];
	
	dispatch_async(dispatch_queue_create("connecting to target Queue", NULL), ^{
		if (self.controlHandle) {
			[self.controlHandle disconnect];
			self.controlHandle = nil;
		}
		self.controlHandle = [[WCWiflyControlWrapper alloc] initWithWCEndpoint:endpoint establishConnection:YES];
		[self.controlHandle setDelegate:self];
		
		for (id obj in self.viewControllers) {
			if ([obj respondsToSelector:@selector(setControlHandle:)]) {
				[obj performSelector:@selector(setControlHandle:) withObject:self.controlHandle];
			}
			if ([obj respondsToSelector:@selector(setEndpoint:)]) {
				[obj performSelector:@selector(setEndpoint:) withObject:endpoint];
			}
		}
		dispatch_async(dispatch_get_main_queue(), ^{
			[connectingView dismissWithClickedButtonIndex:0 animated:YES];
			connectingView = nil;
		});
	});
}

- (void)viewDidLoad
{
	for (id obj in self.viewControllers) {
		if ([obj respondsToSelector:@selector(setDelegate:)]) {
			[obj performSelector:@selector(setDelegate:) withObject:self];
		}
	}
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
	if (self.controlHandle) {
		[self.controlHandle disconnect];
		self.controlHandle = nil;
	}
}


#pragma mark - DELEGATE METHODES

- (void)performUnwindSegue
{
	NSLog(@"A ViewController in TabbarController.viewControllers force to unwind\n");
	[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured" sender:self];
}

- (void) fatalErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber *)errorCode
{
	NSLog(@"FatalError: ErrorCode = %d\n", [errorCode unsignedIntValue]);
	[sender disconnect];
	[sender setDelegate:nil];
	[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured" sender:self];
}

- (void) scriptFullErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber*)errorCode
{
	NSLog(@"ScriptFullError - Cleared Scriptbuffer automatically!\n");
	[sender clearScript];
}

- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender
{
	NSLog(@"WiflyControlHasDisconnected\n");
	[sender disconnect];
	[sender setDelegate:nil];
	[self performSegueWithIdentifier:@"unwindAtConnectionHasDisconnected" sender:self];
}


@end
