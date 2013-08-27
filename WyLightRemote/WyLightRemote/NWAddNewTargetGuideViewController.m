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
#import "NWAddNewTargetConfigureViewController.h"

@interface NWAddNewTargetGuideViewController ()
@property (strong, nonatomic) IBOutlet NWAddNewTargetGuideView *guideView;

@end

@implementation NWAddNewTargetGuideViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	self.guideView.pageImageStrings = @[@"Guide1.png", @"Guide2.png"];
	self.guideView.currentPageIndex = 0;
}

#define SLEEP_TIME_INTERVAL 0.5
#define SCANNING_TIME_S 40
#define ENDPOINT_IP 16909060 // is equal to 1.2.3.4

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
			for (unsigned int i = 0; i < (SCANNING_TIME_S / SLEEP_TIME_INTERVAL); i++) {
				dispatch_async(dispatch_get_main_queue(), ^{
					progressView.progress = i / (SCANNING_TIME_S / SLEEP_TIME_INTERVAL);
				});
				for (WCEndpoint *endpoint in self.receiver.targets) {
					if (endpoint.ipAdress == ENDPOINT_IP) found = YES;
				}
				if (found) break;
				[NSThread sleepForTimeInterval:SLEEP_TIME_INTERVAL];
			}
			dispatch_async(dispatch_get_main_queue(), ^{
				[scanningAlertView dismissWithClickedButtonIndex:0 animated:YES];
				if (found) {
					//[self performSegueWithIdentifier:@"showSelectMode:" sender:self];
					[self showSelectionAlertView];
				} else {
					[self performSegueWithIdentifier:@"noNewTargetFoundSegue:" sender:self];
				}
			});
		});
	}
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
	if ([segue.destinationViewController isKindOfClass:[NWAddNewTargetConfigureViewController class]]) {
		
		NWAddNewTargetConfigureViewController *destViewCtrl = (NWAddNewTargetConfigureViewController*) segue.destinationViewController;
		
		if ([segue.identifier isEqualToString:@"configureAsSoftAP:"]) {
			destViewCtrl.configureTargetAsSoftAP = YES;
		} else {
			destViewCtrl.configureTargetAsSoftAP = NO;
		}
	}
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	if (buttonIndex == 1) {
		[self performSegueWithIdentifier:@"configureAsClient:" sender:self];
	} else if (buttonIndex == 0) {
		[self performSegueWithIdentifier:@"configureAsSoftAP:" sender:self];
	} else {
		[self performSegueWithIdentifier:@"cancelAddNewTargetSegue:" sender:self];
	}
}

- (void)showSelectionAlertView {
	[[[UIAlertView alloc] initWithTitle:@"Operation Mode" message:@"Please choose, in which mode you want to operate your WyLight." delegate:self cancelButtonTitle:nil otherButtonTitles:@"SoftAP", @"Client", nil] show];
}

- (IBAction)donePressed:(id)sender {
	[self nextPage];
}


@end
