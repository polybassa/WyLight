//
//  NWBroadcastsTVC.h
//  WyLightRemote
//
//  Created by Nils Weiß on 06.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NWBroadcastsTVC : UITableViewController

- (IBAction)unwindAtConnectionFatalErrorOccured:(UIStoryboardSegue *)segue;

- (IBAction)unwindAtConnectionHasDisconnected:(UIStoryboardSegue *)segue;

- (IBAction)cancelAddNewTargetSegue:(UIStoryboardSegue *)segue;

- (IBAction)noNewTargetFoundSegue:(UIStoryboardSegue *)segue;

- (IBAction)closeConnection:(UIStoryboardSegue *)sender;

@end
