//
//  NWAddNewTargetConfigureViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAddNewTargetConfigureViewController.h"
#import "WCEndpoint.h"
#import "WCWiflyControlWrapper.h"

@interface NWAddNewTargetConfigureViewController ()
@property (weak, nonatomic) IBOutlet UILabel *ssidLabel;
@property (weak, nonatomic) IBOutlet UITextField *ssidTextField;
@property (weak, nonatomic) IBOutlet UILabel *passLabel;
@property (weak, nonatomic) IBOutlet UITextField *passTextField;
@property (weak, nonatomic) IBOutlet UILabel *nameLabel;
@property (weak, nonatomic) IBOutlet UITextField *nameTextField;
@property (strong, nonatomic) UIAlertView *scanningAlertView;
@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;

@end

@implementation NWAddNewTargetConfigureViewController

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(keyboardWasShown:)
												 name:UIKeyboardDidShowNotification object:nil];
	
    [[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(keyboardWillBeHidden:)
												 name:UIKeyboardWillHideNotification object:nil];
	
	if (self.configureTargetAsSoftAP) {
		self.passLabel.hidden = YES;
		self.passTextField.hidden = YES;
		self.nameLabel.hidden = YES;
		self.nameTextField.hidden = YES;
	}
}

- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (BOOL)textFieldInputValid {
	if (self.configureTargetAsSoftAP) {
		if (![self.ssidTextField.text length]){
			[[[UIAlertView alloc] initWithTitle:@"Invalid Input" message:@"Please complete the input field" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
			return NO;
		}
	} else {
		if (![self.ssidTextField.text length] || ![self.passTextField.text length] || ![self.nameTextField.text length]) {
			[[[UIAlertView alloc] initWithTitle:@"Invalid Input" message:@"Please complete the input fields" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
			return NO;
		}
	}
	return YES;
}

- (IBAction)configButtonPressed:(id)sender {

	if (![self textFieldInputValid]) return;
	
	//**** Alert view ****
	self.scanningAlertView = [[UIAlertView alloc]initWithTitle:@"Configuration" message:@"Connecting to target!" delegate:self cancelButtonTitle:nil otherButtonTitles:nil];
	[self.scanningAlertView show];
	UIProgressView *progressView = [[UIProgressView alloc]initWithProgressViewStyle:UIProgressViewStyleDefault];
	progressView.center = CGPointMake(self.scanningAlertView.bounds.size.width / 2, self.scanningAlertView.bounds.size.height - 50);
	[self.scanningAlertView addSubview:progressView];
	
	__block WCWiflyControlWrapper *control = [[WCWiflyControlWrapper alloc] initWithWCEndpoint:[[WCEndpoint alloc] initWithIpAdress:16909060
																													   port:2000
																													   name:@"newTarget"
																													  score:0]
																		   establishConnection:YES];
	[control setDelegate:self];
	dispatch_queue_t configurationQ = dispatch_queue_create("Configure new Target", NULL);
	dispatch_async(configurationQ, ^{
		
		//Firmware update
		dispatch_async(dispatch_get_main_queue(), ^{
			progressView.progress = 0.2;
			self.scanningAlertView.message = @"Updating target firmware!";
		});
		[control programFlashAsync:NO];
						
		//Start Firmware
		dispatch_async(dispatch_get_main_queue(), ^{
			progressView.progress = 0.5;
			self.scanningAlertView.message = @"Terminate bootloader and start firmware!";
		});
		[control leaveBootloader];
		
		//Configure WLAN Modul
		dispatch_async(dispatch_get_main_queue(), ^{
			progressView.progress = 0.6;
			self.scanningAlertView.message = @"Configure wlan interface!";
		});
		if (self.configureTargetAsSoftAP) {
			[control configurateWlanModuleAsSoftAP:self.ssidTextField.text];
		} else {
			[control configurateWlanModuleAsClientForNetwork:self.ssidTextField.text password:self.passTextField.text name:self.nameTextField.text];
		}
	});
}

- (IBAction)tap:(id)sender {
	[self.ssidTextField resignFirstResponder];
	[self.passTextField resignFirstResponder];
	[self.nameTextField resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return NO;
}

- (void)keyboardWasShown:(NSNotification*)notification {
    NSDictionary* info = [notification userInfo];
    CGSize keyboardSize = [[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size;
	if(self.view.bounds.size.width < self.view.bounds.size.height) //horizontal
		self.scrollView.frame = CGRectMake(self.scrollView.frame.origin.x, self.scrollView.frame.origin.y, self.view.frame.size.width, self.scrollView.frame.size.height - keyboardSize.height);
}

- (void)keyboardWillBeHidden:(NSNotification*)notification {
	NSDictionary* info = [notification userInfo];
    CGSize keyboardSize = [[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size;
	if(self.view.bounds.size.width < self.view.bounds.size.height) //horizontal
		self.scrollView.frame = CGRectMake(self.scrollView.frame.origin.x, self.scrollView.frame.origin.y, self.view.frame.size.width, self.scrollView.frame.size.height + keyboardSize.height);
}

- (void) fatalErrorOccured:(WCWiflyControlWrapper *)sender errorCode:(NSNumber *)errorCode {
	NSLog(@"FatalError: ErrorCode = %d\n", [errorCode unsignedIntValue]);
	[sender disconnect];
	[sender setDelegate:nil];
	sender = nil;
	if (self.scanningAlertView) {
		[self.scanningAlertView dismissWithClickedButtonIndex:0 animated:YES];
		self.scanningAlertView = nil;
		[self performSegueWithIdentifier:@"unwindConfigurationAtFatalErrorOccured" sender:self];
	}
}

- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender {
	NSLog(@"WiflyControlHasDisconnected\n");
	[sender disconnect];
	[sender setDelegate:nil];
	sender = nil;
	if (self.scanningAlertView) {
		[self.scanningAlertView dismissWithClickedButtonIndex:0 animated:YES];
		self.scanningAlertView = nil;
		[self performSegueWithIdentifier:@"unwindConfigurationAtConnectionHasDisconnected" sender:self];
	}
}

@end
