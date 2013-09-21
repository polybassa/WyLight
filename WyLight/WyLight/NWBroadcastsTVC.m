//
//  NWBroadcastsTVC.m
//  WyLightRemote
//
//  Created by Nils Weiß on 06.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWBroadcastsTVC.h"
#import "WCEndpoint.h"
#import "WCBroadcastReceiverWrapper.h"

@interface NWBroadcastsTVC ()

@property (nonatomic, strong) WCBroadcastReceiverWrapper *receiver;

@end

@implementation NWBroadcastsTVC

- (void)refresh {
	[self.refreshControl beginRefreshing];
	[self.receiver clearTargets];
	[self.tableView reloadData];
	
	double delayInSeconds = 10.0;
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
	dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
		if ([self.refreshControl isRefreshing]) {
			[self.refreshControl endRefreshing];
		}
	});
}

#pragma mark - GETTER

- (WCBroadcastReceiverWrapper *)receiver {
	if (!_receiver) {
		_receiver = [[WCBroadcastReceiverWrapper alloc] init];
	}
	return _receiver;
}

#pragma mark - VIEW LIFECYCLE METHODES

- (void)viewDidLoad {
    [super viewDidLoad];
	[self.refreshControl addTarget:self action:@selector(refresh) forControlEvents:UIControlEventValueChanged];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[[NSNotificationCenter defaultCenter]addObserver:self.tableView selector:@selector(reloadData) name:TargetsChangedNotification object:nil];
	[self.receiver start];
}

- (void)viewDidDisappear:(BOOL)animated {
	[[NSNotificationCenter defaultCenter] removeObserver:self.tableView];
	[super viewDidDisappear:animated];
}

#pragma mark - HANDLE ROTATION

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
	[super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
	[self.refreshControl endRefreshing];
}

#pragma mark - Table view data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	size_t numberOfTargets = self.receiver.targets.count;
	if (self.refreshControl.refreshing && numberOfTargets > 0) {
		dispatch_async(dispatch_get_main_queue(), ^{
			[self.refreshControl endRefreshing];
		});
	}
    return numberOfTargets;
};

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"Target";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];
    
    cell.textLabel.text = [[[self.receiver targets]objectAtIndex:indexPath.row] name];
	cell.detailTextLabel.text = [[[self.receiver targets]objectAtIndex:indexPath.row] adressString];
    return cell;
}

#pragma mark - SEGUE's

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    NSIndexPath *indexPath = nil;
	
	if ([segue.identifier isEqualToString:@"addNewTarget:"]) {
		if ([segue.destinationViewController respondsToSelector:@selector(setReceiver:)]) {
			[segue.destinationViewController performSelector:@selector(setReceiver:) withObject:self.receiver];
		}
	}
    
    if ([sender isKindOfClass:[UITableViewCell class]]) {
        indexPath = [self.tableView indexPathForCell:sender];
    }
    
    if (indexPath) {
        if ([segue.identifier isEqualToString:@"setEndpoint:"]) {
			WCEndpoint *endpoint = [[self.receiver targets] objectAtIndex:indexPath.row];
			[self.receiver setWCEndpointAsFavorite:endpoint];
			[self.receiver saveTargets];
            if ([segue.destinationViewController respondsToSelector:@selector(setEndpoint:)]) {
                [segue.destinationViewController performSelector:@selector(setEndpoint:) withObject:endpoint];
            }
        }
    }
}

// if a fatal error occures I remove the endpoint from my favorites-list
- (IBAction)unwindAtConnectionFatalErrorOccured:(UIStoryboardSegue *)segue {
	dispatch_async(dispatch_get_main_queue(), ^{
		[[[UIAlertView alloc]initWithTitle:@"Connection lost!" message:@"Please retry!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
		
		if ([segue.sourceViewController respondsToSelector:@selector(endpoint)]) {
			[self.receiver unsetWCEndpointAsFavorite: [segue.sourceViewController performSelector:@selector(endpoint)]];
			[self.receiver saveTargets];
			[self.receiver clearTargets];
		}
	});
}

// show a message
- (IBAction)unwindAtConnectionHasDisconnected:(UIStoryboardSegue *)segue {
	dispatch_async(dispatch_get_main_queue(), ^{
		[[[UIAlertView alloc]initWithTitle:@"Configuration successfull!"
								   message:@"Please change your WLAN-Network \nin your device settings!"
								  delegate:self
						 cancelButtonTitle:@"OK"
						 otherButtonTitles:nil] show];
	});
	[self.receiver clearTargets];
}

- (IBAction)cancelAddNewTargetSegue:(UIStoryboardSegue *)segue {
	
}

- (IBAction)noNewTargetFoundSegue:(UIStoryboardSegue *)segue {
	dispatch_async(dispatch_get_main_queue(), ^{
		[[[UIAlertView alloc]initWithTitle:@"No new WyLight found!"
								   message:@"Please retry!"
								  delegate:self
						 cancelButtonTitle:@"OK"
						 otherButtonTitles:nil] show];
	});
}

- (IBAction)closeConnection:(UIStoryboardSegue *)sender {
	
}

@end
