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

@end

@implementation NWAppInfoViewController

- (void)viewWillAppear:(BOOL)animated {
	self.appVersionLabel.text = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
}


@end
