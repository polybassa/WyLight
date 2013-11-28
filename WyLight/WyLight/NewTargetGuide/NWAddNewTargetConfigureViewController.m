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
@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;

@end

@implementation NWAddNewTargetConfigureViewController

#pragma mark - ViewController Lifecycle Stuff

- (void)setup {
	{
		UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 25)];
		label.text = @"SSID: ";
		label.backgroundColor = [UIColor clearColor];
		label.textColor = [UIColor darkGrayColor];
				
		self.ssidTextField.leftViewMode = UITextFieldViewModeAlways;
		self.ssidTextField.leftView = label;
		[label sizeToFit];
	}
	{
		UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 25)];
		label.text = NSLocalizedStringFromTable(@"PasswordKey", @"ViewControllerLocalization", @"");
		label.backgroundColor = [UIColor clearColor];
		label.textColor = [UIColor darkGrayColor];
		
		self.passTextField.secureTextEntry = YES;
		self.passTextField.leftViewMode = UITextFieldViewModeAlways;
		self.passTextField.leftView = label;
		[label sizeToFit];
	}
	{
		UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 25)];
		label.text = @"WyLight Name: ";
		label.backgroundColor = [UIColor clearColor];
		label.textColor = [UIColor darkGrayColor];
				
		self.nameTextField.leftViewMode = UITextFieldViewModeAlways;
		self.nameTextField.leftView = label;
		[label sizeToFit];
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
	
	if (self.scanningAlertView) {
		[self.scanningAlertView dismissWithClickedButtonIndex:0 animated:YES];
		self.scanningAlertView = nil;
	}
	
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[super viewWillDisappear:animated];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
		
	if (self.configureTargetAsSoftAP) {
		self.passTextField.hidden = YES;
		self.nameTextField.hidden = YES;
		self.informationLabel.text = NSLocalizedStringFromTable(@"SSIDInfoKey", @"ViewControllerLocalization", @"");
	} else {
		self.informationLabel.text = NSLocalizedStringFromTable(@"APInfoKey", @"ViewControllerLocalization", @"");
	}
	[self.informationLabel sizeToFit];
	
	[[NSNotificationCenter defaultCenter] addObserver: self
											 selector: @selector(handleEnteredBackground:)
												 name: UIApplicationDidEnterBackgroundNotification
											   object: nil];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	[self setup];
}

- (void)handleEnteredBackground:(NSNotification *)notification {
	if ([notification.name isEqualToString:UIApplicationDidEnterBackgroundNotification]) {
		[self performSegueWithIdentifier:@"cancelAddNewTargetSegue:" sender:self];
	}
}

#pragma mark - TextField Stuff

- (BOOL)textFieldInputValid {
    NSCharacterSet * set = [[NSCharacterSet characterSetWithCharactersInString:@"-_/:.,;+*#<>()=?abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLKMNOPQRSTUVWXYZ0123456789"] invertedSet];
	if (self.configureTargetAsSoftAP) {
		if (![self.ssidTextField.text length]){
            [[[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"InvalidInputKey", @"ViewControllerLocalization", @"")
                                        message:NSLocalizedStringFromTable(@"CompleteInputKey", @"ViewControllerLocalization", @"")
                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
			return NO;
		}
        /*if ([self.ssidTextField.text rangeOfCharacterFromSet:set].location != NSNotFound) {
            [[[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"InvalidInputKey", @"ViewControllerLocalization", @"")
                                        message:NSLocalizedStringFromTable(@"InvalidInputKey2", @"ViewControllerLocalization", @"")
                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
            return NO;
        }*/
	} else {
		if (![self.ssidTextField.text length] || ![self.passTextField.text length] || ![self.nameTextField.text length]) {
			[[[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"InvalidInputKey", @"ViewControllerLocalization", @"")
                                        message:NSLocalizedStringFromTable(@"CompleteInputKey", @"ViewControllerLocalization", @"")
                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
			return NO;
		}/*
        if ([self.ssidTextField.text rangeOfCharacterFromSet:set].location != NSNotFound) {
            [[[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"InvalidInputKey", @"ViewControllerLocalization", @"")
                                        message:NSLocalizedStringFromTable(@"InvalidInputKey2", @"ViewControllerLocalization", @"")
                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
            return NO;
        }*/
	}
	return YES;
}

- (IBAction)configButtonPressed:(id)sender {

	if (![self textFieldInputValid]) return;
		
		//**** Alert view ****
		self.scanningAlertView = [[UIAlertView alloc]initWithTitle:NSLocalizedStringFromTable(@"ConfigurationKey", @"ViewControllerLocalization", @"")
                                                           message:NSLocalizedStringFromTable(@"ConnectingKey", @"ViewControllerLocalization", @"")
                                                          delegate:self cancelButtonTitle:nil otherButtonTitles:nil];
		[self.scanningAlertView show];
			
		dispatch_queue_t configurationQ = dispatch_queue_create("Configure new Target", NULL);
		dispatch_async(configurationQ, ^{
			
			self.controlHandle = [[WCWiflyControlWrapper alloc] initWithWCEndpoint:[[WCEndpoint alloc] initWithIpAdress:16909060
																													   port:2000
																													   name:@"newTarget"
																													  score:0]
																   establishConnection:YES doStartup:NO];
			if (self.controlHandle == nil) {
				dispatch_async(dispatch_get_main_queue(), ^{
					[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured:" sender:self];
					return;
				});
				return;
			}
			[self.controlHandle setDelegate:self];
						
			//Configure WLAN Modul
			dispatch_async(dispatch_get_main_queue(), ^{
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

- (IBAction)didEnterInTextField:(id)sender
{
    [sender becomeFirstResponder];
    // Resize the scroll view to reduce the keyboard height
    CGRect scrollViewFrame = self.scrollView.frame;
    if (scrollViewFrame.size.height > 300) {
        scrollViewFrame.size.height -= 216;
        self.scrollView.frame = scrollViewFrame;
    }
	
    // Scroll the view to see the text field
    UITextField *selectedTextField = (UITextField *)sender;
    float yPosition = selectedTextField.frame.origin.y - 60;
    float bottomPositon = self.scrollView.contentSize.height - self.scrollView.bounds.size.height;
    if (yPosition > bottomPositon) {
        yPosition = bottomPositon;
    }
    [self.scrollView setContentOffset:CGPointMake(0, yPosition) animated:YES];
}

#pragma mark - ControlHandle

- (void)disconnectFromEndpoint {
	if (self.controlHandle && self.controlHandle.delegate) {
		self.controlHandle.delegate = nil;
		[self.controlHandle disconnect];
		self.controlHandle = nil;
	}
}

#pragma mark - WCWiflyControlDelegate

- (void)wiflyControl:(WCWiflyControlWrapper *)sender fatalErrorOccured:(NSNumber *)errorCode {
	NSLog(@"FatalError: ErrorCode = %d\n", [errorCode unsignedIntValue]);
	[self performSegueWithIdentifier:@"unwindAtConnectionFatalErrorOccured:" sender:self];
}

- (void) wiflyControlHasDisconnected:(WCWiflyControlWrapper *)sender {
	NSLog(@"WiflyControlHasDisconnected\n");
	[self performSegueWithIdentifier:@"unwindAtConnectionHasDisconnected:" sender:self];
}

@end
