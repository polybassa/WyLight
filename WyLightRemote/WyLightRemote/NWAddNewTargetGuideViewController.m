//
//  NWAddNewTargetGuideViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 07.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAddNewTargetGuideViewController.h"
#import "NWAddNewTargetGuideView.h"
#import "WCBroadcastReceiverWrapper.h"
#import "WCEndpoint.h"

@interface NWAddNewTargetGuideViewController ()
@property (strong, nonatomic) IBOutlet NWAddNewTargetGuideView *guideView;

@end

@implementation NWAddNewTargetGuideViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	self.guideView.pageImageStrings = @[@"Guide1.png", @"Guide2.png"];
	self.guideView.currentPageIndex = 0;
}

- (void)nextPage
{
	if ((self.guideView.currentPageIndex + 1 ) < [self.guideView.pageImageStrings count]) {
		[UIView transitionWithView:self.guideView
						  duration:0.5
						   options:UIViewAnimationOptionTransitionFlipFromRight
						animations:^{ self.guideView.currentPageIndex++; }
						completion:NULL];
	}
	else {
		//**** Alert view ****
		UIAlertView *scanningAlertView = [[UIAlertView alloc]initWithTitle:@"Scanning" message:@"Please wait!" delegate:self cancelButtonTitle:nil otherButtonTitles:nil];
		[scanningAlertView show];
		UIProgressView *progressView = [[UIProgressView alloc]initWithProgressViewStyle:UIProgressViewStyleDefault];
		progressView.center = CGPointMake(scanningAlertView.bounds.size.width / 2, scanningAlertView.bounds.size.height - 50);
		[scanningAlertView addSubview:progressView];
		
		//**** dispatch queue scanning
		dispatch_async(dispatch_queue_create("Scanning for Broadcast of 1.2.3.4", NULL), ^{
			BOOL found = NO;
			//BOOL found = YES; //for DEBUG only !!!!!!!!!!!!!!!!!!!!!!!!!!!!
			[self.receiver clearTargets];
			for (unsigned int i = 0; i < 80; i++) {
				dispatch_async(dispatch_get_main_queue(), ^{
					progressView.progress = i / 80.0;
				});
				for (WCEndpoint *endpoint in self.receiver.targets) {
					// 16909060 is equal to 1.2.3.4
					if (endpoint.ipAdress == 16909060 ) found = YES;
				}
				if (found) break;
				[NSThread sleepForTimeInterval:0.5];
			}
			dispatch_async(dispatch_get_main_queue(), ^{
				[scanningAlertView dismissWithClickedButtonIndex:0 animated:YES];
				if (found) {
					[self performSegueWithIdentifier:@"showSelectMode:" sender:self];
				} else {
					[self performSegueWithIdentifier:@"noNewTargetFoundSegue:" sender:self];
				}
			});
		});
	}
}
- (IBAction)donePressed:(id)sender {
	[self nextPage];
}


@end
