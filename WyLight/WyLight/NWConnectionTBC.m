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
	_endpoint = endpoint;
	if (endpoint) {
		self.title = endpoint.name;
	}
}

- (void)connectToEndpoint {
	UIAlertView *connectingView = [[UIAlertView alloc] initWithTitle:@"Connecting" message:nil delegate:self cancelButtonTitle:nil otherButtonTitles:nil, nil];
	[connectingView show];
	
	dispatch_async(dispatch_queue_create("connecting to target Queue", NULL), ^{
		
		self.controlHandle = [[WCWiflyControlWrapper alloc] initWithWCEndpoint:self.endpoint establishConnection:YES];
		if (self.controlHandle == nil) {
			dispatch_async(dispatch_get_main_queue(), ^{
				[connectingView dismissWithClickedButtonIndex:0 animated:YES];
				[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured" sender:self];
				return;
			});
			return;
		}
		
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

- (void)disconnectFromEndpoint {
	if (self.controlHandle && self.controlHandle.delegate) {
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

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[[NSNotificationCenter defaultCenter] addObserver: self
											 selector: @selector(handleEnteredBackground:)
												 name: UIApplicationDidEnterBackgroundNotification
											   object: nil];
	if (self.endpoint) {
		[self connectToEndpoint];
	}
}

- (void)viewWillDisappear:(BOOL)animated {
	NSArray *viewControllers = self.navigationController.viewControllers;
	if (viewControllers.count > 1 && [viewControllers objectAtIndex:viewControllers.count - 2] == self) {
		// View is disappearing because a new view controller was pushed onto the stack
	} else if ([viewControllers indexOfObject:self] == NSNotFound) {
		// View is disappearing because it was popped from the stack
		[self disconnectFromEndpoint];
	}
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[super viewWillDisappear:animated];
}

- (void)handleEnteredBackground:(NSNotification *)notification {
	if ([notification.name isEqualToString:UIApplicationDidEnterBackgroundNotification]) {
		[self performSegueWithIdentifier:@"closeConnection" sender:self];
	}
}

#pragma mark - DELEGATE METHODES

- (void) fatalErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber *)errorCode {
	NSLog(@"FatalError: ErrorCode = %d\n", [errorCode unsignedIntValue]);
	[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured" sender:self];
}

- (void) scriptFullErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber*)errorCode {
	NSLog(@"ScriptFullError - Cleared Scriptbuffer automatically!\n");
	[sender clearScript];
}

- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender {
	NSLog(@"WiflyControlHasDisconnected\n");
	[self performSegueWithIdentifier:@"unwindAtConnectionHasDisconnected" sender:self];
}


@end
