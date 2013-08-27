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

- (WCBroadcastReceiverWrapper *)receiver
{
	if (!_receiver) {
		_receiver = [[WCBroadcastReceiverWrapper alloc] init];
	}
	return _receiver;
}

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
	[super viewDidDisappear:animated];
	[[NSNotificationCenter defaultCenter] removeObserver:self.tableView];
}

- (void)refresh {
	[self.refreshControl beginRefreshing];
	[self.receiver clearTargets];
}

#pragma mark - Table view data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	if ([self.refreshControl isRefreshing] && ([[self.receiver targets] count])) {
		[self.refreshControl endRefreshing];
	}
	
    return [[self.receiver targets] count];
};

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"Target";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];
    
    cell.textLabel.text = [[[self.receiver targets]objectAtIndex:indexPath.row] name];
	cell.detailTextLabel.text = [[[self.receiver targets]objectAtIndex:indexPath.row] adressString];
    return cell;
}

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
	[[[UIAlertView alloc]initWithTitle:@"Oh oh!" message:@"Connection lost\nPlease retry!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
	
	if ([segue.sourceViewController respondsToSelector:@selector(endpoint)]) {
		WCEndpoint *endpoint = [segue.sourceViewController performSelector:@selector(endpoint)];
		[self.receiver unsetWCEndpointAsFavorite:endpoint];
	}
}

// show a message
- (IBAction)unwindAtConnectionHasDisconnected:(UIStoryboardSegue *)segue {
	[[[UIAlertView alloc]initWithTitle:@"Configuration successfull!" message:@"Please change your WLAN-Network \nin your device settings!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
}

- (IBAction)cancelAddNewTargetSegue:(UIStoryboardSegue *)segue {
	
}

- (IBAction)noNewTargetFoundSegue:(UIStoryboardSegue *)segue {
	[[[UIAlertView alloc]initWithTitle:@"No new Target found!" message:@"Please retry!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil] show];

}

- (IBAction)closeConnection:(UIStoryboardSegue *)sender {
	
}

@end
