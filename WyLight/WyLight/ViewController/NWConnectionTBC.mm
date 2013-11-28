//
//  NWConnectionTBC.m
//  WyLightRemote
//
//  Created by Nils Weiß on 06.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWConnectionTBC.h"
#import "WCEndpoint.h"
#include "StartupManager.h"

@interface NWConnectionTBC () <UIAlertViewDelegate>

@property (nonatomic, strong) WCWiflyControlWrapper* controlHandle;
@property (nonatomic, strong) UIAlertView* connectingView;
@property (nonatomic) BOOL didUpdate;

@end

@implementation NWConnectionTBC

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
	if (buttonIndex == 0 && self.didUpdate) {
		[self performSegueWithIdentifier:@"closeConnection" sender:self];
	}
}

//establishes connection to the endpoint automatically
- (void)setEndpoint:(WCEndpoint *)endpoint {
	_endpoint = endpoint;
	if (endpoint) {
		self.title = endpoint.name;
	}
}

- (UIAlertView *)connectingView {
	if (!_connectingView) {
		self.connectingView = [[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"ConnectingKey", @"ViewControllerLocalization", @"") message:nil delegate:nil cancelButtonTitle:nil otherButtonTitles:nil, nil];
	}
	
	return _connectingView;
}

- (void)connectToEndpoint {
	if (self.controlHandle != nil) {
		return;
	}
    if (![self.connectingView isVisible]) {
		[self.connectingView show];
	}
	dispatch_async(dispatch_queue_create("connecting to target Queue", NULL), ^{
		
		self.controlHandle = [[WCWiflyControlWrapper alloc] initWithWCEndpoint:self.endpoint establishConnection:NO doStartup:NO];
		[self.controlHandle setDelegate:self];

        NSInteger returnValue = [self.controlHandle connectWithStartup:YES];
        dispatch_async(dispatch_get_main_queue(), ^{
			[self.connectingView dismissWithClickedButtonIndex:0 animated:YES];
			if (returnValue != 0) {
				if (self.didUpdate) {
					[[[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"UpdateFailedKey", @"ViewControllerLocalization", @"") message:NSLocalizedStringFromTable(@"RetryKey", @"ViewControllerLocalization", @"") delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
				} else {
					[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured" sender:self];
				}
			}
		});
		if (returnValue != 0) {
			return;
		}
        
		// Finish connection Block
		for (id obj in self.viewControllers) {
			if ([obj respondsToSelector:@selector(setControlHandle:)]) {
				[obj performSelector:@selector(setControlHandle:) withObject:self.controlHandle];
			}
		}
	});
}

- (void)disconnectFromEndpoint {
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

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
	if (self.endpoint) {
		[self connectToEndpoint];
	}
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	if (self.controlHandle == nil) {
		[self.connectingView show];
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
	[super viewWillDisappear:animated];
}

- (void)handleEnteredBackground:(NSNotification *)notification {
	if ([notification.name isEqualToString:UIApplicationDidEnterBackgroundNotification]) {
		//[self performSegueWithIdentifier:@"closeConnection" sender:self];
	}
}

#pragma mark - DELEGATE METHODES

- (void)wiflyControl:(WCWiflyControlWrapper *)sender connectionStartupStateChanged:(NSNumber *)state {
    
    NSString* titel = NSLocalizedStringFromTable(@"ConnectingKey", @"ViewControllerLocalization", @"");
    NSString* message;
    switch (state.unsignedIntegerValue) {
        case WyLight::StartupManager::STARTUP_FAILURE:
            break;
            
        case WyLight::StartupManager::STARTUP_SUCCESSFUL:
            break;
            
        case WyLight::StartupManager::UPDATING:{
			[self setDidUpdate:YES];
			titel = NSLocalizedStringFromTable(@"UpdateRequiredKey", @"ViewControllerLocalization", @"");
			message = NSLocalizedStringFromTable(@"PleaseWaitKey", @"ViewControllerLocalization", @"");
		} break;

        default:
            break;
    }
    
    [self.connectingView setTitle:titel];
    [self.connectingView setMessage:message];
}

- (void)wiflyControl:(WCWiflyControlWrapper *)sender fatalErrorOccured:(NSNumber *)errorCode {
	NSLog(@"FatalError: ErrorCode = %d\n", [errorCode unsignedIntValue]);
	[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured" sender:self];
}

- (void)wiflyControl:(WCWiflyControlWrapper *)sender scriptBufferErrorOccured:(NSNumber *)errorCode {
	NSLog(@"ScriptFullError - Cleared Scriptbuffer automatically!\n");
	[sender clearScript];
}

- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender {
	NSLog(@"WiflyControlHasDisconnected\n");
	self.controlHandle.delegate = nil;
	self.controlHandle = nil;
	[self performSegueWithIdentifier:@"closeConnection" sender:self];
}


@end
