//
//  NWConnectionTBC.m
//  WyLightRemote
//
//  Created by Nils Weiß on 06.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWConnectionTBC.h"
#import "WCEndpoint.h"

@interface NWConnectionTBC ()

@property (nonatomic, strong) WCWiflyControlWrapper* controlHandle;

@end

@implementation NWConnectionTBC

//establishes connection to the endpoint automatically
- (void)setEndpoint:(WCEndpoint *)endpoint {
	self.title = endpoint.name;
	
	UIAlertView *connectingView = [[UIAlertView alloc] initWithTitle:@"Connecting" message:nil delegate:self cancelButtonTitle:nil otherButtonTitles:nil, nil];
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
		}
		dispatch_async(dispatch_get_main_queue(), ^{
			[connectingView dismissWithClickedButtonIndex:0 animated:YES];
		});
	});
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[[NSNotificationCenter defaultCenter] addObserver: self
											 selector: @selector(handleEnteredBackground:)
												 name: UIApplicationDidEnterBackgroundNotification
											   object: nil];
}

- (void)viewWillDisappear:(BOOL)animated {
	NSArray *viewControllers = self.navigationController.viewControllers;
	if (viewControllers.count > 1 && [viewControllers objectAtIndex:viewControllers.count - 2] == self) {
		// View is disappearing because a new view controller was pushed onto the stack
		NSLog(@"New view controller was pushed");
	} else if ([viewControllers indexOfObject:self] == NSNotFound) {
		// View is disappearing because it was popped from the stack
		NSLog(@"View controller was popped");
		
		if (self.controlHandle) {
			self.controlHandle.delegate = nil;
			[self.controlHandle disconnect];
			self.controlHandle = nil;
			
			for (id obj in self.viewControllers) {
				if ([obj respondsToSelector:@selector(setControlHandle:)]) {
					[obj performSelector:@selector(setControlHandle:) withObject:nil];
				}
			}
		}
	}
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	[super viewWillDisappear:animated];
}

- (void)handleEnteredBackground:(NSNotification *)notification {
	if ([notification.name isEqualToString:UIApplicationDidEnterBackgroundNotification]) {
		NSLog(@"background");
		/*
		NSArray *viewControllers = self.navigationController.viewControllers;
		if (viewControllers.count > 1 && [viewControllers objectAtIndex:viewControllers.count-2] == self) {
			// View is disappearing because a new view controller was pushed onto the stack
			NSLog(@"New view controller was pushed");
		} else if ([viewControllers indexOfObject:self] == NSNotFound) {
			// View is disappearing because it was popped from the stack
			NSLog(@"View controller was popped");
		}*/

	}
}

#pragma mark - DELEGATE METHODES

- (void) fatalErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber *)errorCode {
	NSLog(@"FatalError: ErrorCode = %d\n", [errorCode unsignedIntValue]);
	[sender disconnect];
	[sender setDelegate:nil];
	[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured" sender:self];
}

- (void) scriptFullErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber*)errorCode {
	NSLog(@"ScriptFullError - Cleared Scriptbuffer automatically!\n");
	[sender clearScript];
}

- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender {
	NSLog(@"WiflyControlHasDisconnected\n");
	[sender disconnect];
	[sender setDelegate:nil];
	[self performSegueWithIdentifier:@"unwindAtConnectionHasDisconnected" sender:self];
}


@end
