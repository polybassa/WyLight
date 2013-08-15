//
//  NWAddNewTargetChooserViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAddNewTargetChooserViewController.h"
#import "NWAddNewTargetConfigureViewController.h"


@implementation NWAddNewTargetChooserViewController

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
	if ([segue.destinationViewController isKindOfClass:[NWAddNewTargetConfigureViewController class]]) {
		
		NWAddNewTargetConfigureViewController *destViewCtrl = (NWAddNewTargetConfigureViewController*) segue.destinationViewController;
		
		if ([segue.identifier isEqualToString:@"configureAsSoftAP:"]) {
			destViewCtrl.configureTargetAsSoftAP = YES;
		} else {
			destViewCtrl.configureTargetAsSoftAP = NO;
		}
	}
}

@end
