//
//  NWAppInfoViewController.m
//  WyLight
//
//  Created by Nils Weiß on 08.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAppInfoViewController.h"

@interface NWAppInfoViewController ()
@property (weak, nonatomic) IBOutlet UILabel *appVersionLabel;
@property (weak, nonatomic) IBOutlet UILabel *fwVersionLabel;
@property (weak, nonatomic) IBOutlet UILabel *hexVersionLabel;

@end

@implementation NWAppInfoViewController

- (void)viewWillAppear:(BOOL)animated {
	self.appVersionLabel.text = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
	self.fwVersionLabel.text = @"0.0";
	self.hexVersionLabel.text = @"0.0";
}


@end
