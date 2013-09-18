//
//  NWAddNewTargetConfigureViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAddNewTargetConfigureViewController.h"
#import "WCEndpoint.h"

@interface NWAddNewTargetConfigureViewController ()
@property (weak, nonatomic) IBOutlet UITextField *ssidTextField;
@property (weak, nonatomic) IBOutlet UITextField *passTextField;
@property (weak, nonatomic) IBOutlet UITextField *nameTextField;
@property (weak, nonatomic) IBOutlet UILabel *informationLabel;
@property (strong, nonatomic) UIAlertView *scanningAlertView;
@property (strong, nonatomic) WCWiflyControlWrapper *controlHandle;

@end

@implementation NWAddNewTargetConfigureViewController

- (void)viewWillDisappear:(BOOL)animated {
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[super viewWillDisappear:animated];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
		
	if (self.configureTargetAsSoftAP) {
		self.passTextField.hidden = YES;
		self.nameTextField.hidden = YES;
		self.informationLabel.text = @"Enter SSID for your WyLight";
	} else {
		self.informationLabel.text = @"Enter your AP informations";
	}
	
	[[NSNotificationCenter defaultCenter] addObserver: self
											 selector: @selector(handleEnteredBackground:)
												 name: UIApplicationDidEnterBackgroundNotification
											   object: nil];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	{
		UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 25)];
		label.text = @"SSID: ";
		label.backgroundColor = [UIColor clearColor];
		label.textColor = [UIColor darkGrayColor];
		label.shadowColor = [UIColor lightGrayColor];
		label.shadowOffset = CGSizeMake(1, 1);
		
		self.ssidTextField.leftViewMode = UITextFieldViewModeAlways;
		self.ssidTextField.leftView = label;
		[label sizeToFit];
	}
	{
		UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 25)];
		label.text = @"PASSWORD: ";
		label.backgroundColor = [UIColor clearColor];
		label.textColor = [UIColor darkGrayColor];
		label.shadowColor = [UIColor lightGrayColor];
		label.shadowOffset = CGSizeMake(1, 1);

		self.passTextField.leftViewMode = UITextFieldViewModeAlways;
		self.passTextField.leftView = label;
		[label sizeToFit];
	}
	{
		UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 25)];
		label.text = @"WyLight Name: ";
		label.backgroundColor = [UIColor clearColor];
		label.textColor = [UIColor darkGrayColor];
		label.shadowColor = [UIColor lightGrayColor];
		label.shadowOffset = CGSizeMake(1, 1);

		self.nameTextField.leftViewMode = UITextFieldViewModeAlways;
		self.nameTextField.leftView = label;
		[label sizeToFit];
	}
}

- (void)handleEnteredBackground:(NSNotification *)notification {
	if ([notification.name isEqualToString:UIApplicationDidEnterBackgroundNotification]) {
		if (self.controlHandle && self.controlHandle.delegate) {
			[self.controlHandle disconnect];
			self.controlHandle.delegate = nil;
			self.controlHandle = nil;
		}
		[self.navigationController popToRootViewControllerAnimated:YES];
	}
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
	
	 self.controlHandle = [[WCWiflyControlWrapper alloc] initWithWCEndpoint:[[WCEndpoint alloc] initWithIpAdress:16909060
																													   port:2000
																													   name:@"newTarget"
																													  score:0]
																		   establishConnection:YES];
	[self.controlHandle setDelegate:self];
	dispatch_queue_t configurationQ = dispatch_queue_create("Configure new Target", NULL);
	dispatch_async(configurationQ, ^{
		
		//Erase eeprom
		dispatch_async(dispatch_get_main_queue(), ^{
			progressView.progress = 0.2;
			self.scanningAlertView.message = @"Erase eeprom of target!";
		});
		[self.controlHandle eraseEeprom];
		[NSThread sleepForTimeInterval:1];
		//Firmware update
		dispatch_async(dispatch_get_main_queue(), ^{
			progressView.progress = 0.4;
			self.scanningAlertView.message = @"Updating target firmware!";
		});
		[self.controlHandle programFlashAsync:NO];
						
		//Start Firmware
		dispatch_async(dispatch_get_main_queue(), ^{
			progressView.progress = 0.7;
			self.scanningAlertView.message = @"Terminate bootloader and start firmware!";
		});
		[self.controlHandle leaveBootloader];
		[NSThread sleepForTimeInterval:0.4];
		
		//Configure WLAN Modul
		dispatch_async(dispatch_get_main_queue(), ^{
			progressView.progress = 0.8;
			self.scanningAlertView.message = @"Configure wlan interface!";
		});
		if (self.configureTargetAsSoftAP) {
			[self.controlHandle configurateWlanModuleAsSoftAP:self.ssidTextField.text];
		} else {
			[self.controlHandle configurateWlanModuleAsClientForNetwork:self.ssidTextField.text password:self.passTextField.text name:self.nameTextField.text];
		}
	});
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return NO;
}

#pragma mark - WCWiflyControlDelegate

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
