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

@interface NWAddNewTargetGuideViewController () <UIAlertViewDelegate>

@property (nonatomic, strong) NWAddNewTargetGuideView *guideView;

@end

@implementation NWAddNewTargetGuideViewController

/*- (void)setup {
	
	NWAddNewTargetGuideView *guideView = [[NWAddNewTargetGuideView alloc] init];
	guideView.pageImageStrings = @[@"Guide1.png", @"Guide2.png"];
	guideView.currentPageIndex = 0;
	guideView.opaque = NO;
	guideView.backgroundColor = [UIColor whiteColor];
	
	UISwipeGestureRecognizer *swipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(nextPage)];
	swipe.direction = UISwipeGestureRecognizerDirectionRight | UISwipeGestureRecognizerDirectionLeft;
	[guideView addGestureRecognizer:swipe];
	self.guideView = guideView;
	//[self.view addSubview:guideView];
}

- (void)fixLocations {
	//horizontal
	if(self.view.bounds.size.width < self.view.bounds.size.height)
	{
		self.guideView.frame = CGRectMake(0, 64, self.view.bounds.size.width, self.view.bounds.size.height - 64);
	}
	else
	{
		CGFloat width = self.view.bounds.size.height;
		self.guideView.frame = CGRectMake(self.view.bounds.size.width / 2 - width / 2, 54, width, width - 54);
	}
}

- (void)viewDidLoad {
    [super viewDidLoad];
	[self setup];
}

- (void)viewWillLayoutSubviews {
	[super viewWillLayoutSubviews];
	[self fixLocations];
}*/

#define SLEEP_TIME_INTERVAL 0.5
#define SCANNING_TIME_S 40
#define ENDPOINT_IP 16909060 // is equal to 1.2.3.4

#define SCANNING_ALERT_VIEW_TITLE NSLocalizedStringFromTable(@"ScanningKey", @"ViewControllerLocalization", @"")
#define SELECTION_ALERT_VIEW_TITLE NSLocalizedStringFromTable(@"OperationModeKey", @"ViewControllerLocalization", @"")

- (void)nextPage {
	/*if ((self.guideView.currentPageIndex + 1 ) < [self.guideView.pageImageStrings count]) {
		[UIView transitionWithView:self.guideView
						  duration:0.7
						   options:UIViewAnimationOptionTransitionCurlUp
						animations:^{ self.guideView.currentPageIndex++; }
						completion:NULL];
	}
	else*/
    {
		//**** Alert view ****
		UIAlertView *scanningAlertView = [[UIAlertView alloc]initWithTitle:SCANNING_ALERT_VIEW_TITLE message:@" " delegate:self cancelButtonTitle:NSLocalizedStringFromTable(@"CancelKey", @"ViewControllerLocalization", @"") otherButtonTitles:nil];
		[scanningAlertView setDelegate:self];
		[scanningAlertView show];
		
		//**** dispatch queue scanning
		dispatch_async(dispatch_queue_create("Scanning for Broadcast of 1.2.3.4", NULL), ^{
			BOOL found = NO;
			//BOOL found = YES; //for DEBUG only !!!!!!!!!!!!!!!!!!!!!!!!!!!!
			[self.receiver clearTargets];
			for (unsigned int i = 0; i < (SCANNING_TIME_S / SLEEP_TIME_INTERVAL); i++) {
				for (WCEndpoint *endpoint in self.receiver.targets) {
					if (endpoint.ipAdress == ENDPOINT_IP) found = YES;
				}
				if (found) break;
				if (!scanningAlertView.isVisible) {
					return ;
				}
				[NSThread sleepForTimeInterval:SLEEP_TIME_INTERVAL];
			}
			dispatch_async(dispatch_get_main_queue(), ^{
				[scanningAlertView dismissWithClickedButtonIndex:0 animated:YES];
				if (found) {
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
	if ([alertView.title isEqualToString:SCANNING_ALERT_VIEW_TITLE]) {
		[self performSegueWithIdentifier:@"cancelAddNewTargetSegue:" sender:self];
	} else if ([alertView.title isEqualToString:SELECTION_ALERT_VIEW_TITLE]) {
		
		if (buttonIndex == 1) {
			[self performSegueWithIdentifier:@"configureAsClient:" sender:self];
		} else if (buttonIndex == 0) {
			[self performSegueWithIdentifier:@"configureAsSoftAP:" sender:self];
		} else {
			[self performSegueWithIdentifier:@"cancelAddNewTargetSegue:" sender:self];
		}
	}
}

- (void)showSelectionAlertView {
	[[[UIAlertView alloc] initWithTitle:SELECTION_ALERT_VIEW_TITLE message:NSLocalizedStringFromTable(@"OperationModeInfoKey", @"ViewControllerLocalization", @"") delegate:self cancelButtonTitle:nil otherButtonTitles:@"SoftAP", @"Client", nil] show];
}

- (IBAction)doneBarButtonPressed:(id)sender {
	[self nextPage];
}


@end
