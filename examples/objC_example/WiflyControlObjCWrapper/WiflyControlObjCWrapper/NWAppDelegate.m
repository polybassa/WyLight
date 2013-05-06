//
//  NWAppDelegate.m
//  WiflyControlObjCWrapper
//
//  Created by Nils Weiß on 17.04.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAppDelegate.h"

@implementation NWAppDelegate

- (void)dealloc
{
	[_window release];
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Override point for customization after application launch.
	self.broadcastreceiver = [[WCBroadcastReceiverWrapper alloc] init];
	self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)createConnectionToWiflyLight
{
	size_t numberOfAvailableTargets = [self.broadcastreceiver numberOfTargets];
	if(numberOfAvailableTargets > 0)
	{
		uint32_t ipOfFirstAvailableTarget = [self.broadcastreceiver ipAdressOfTarget:0];
		uint16_t portOfFirstAvailableTarget = [self.broadcastreceiver portOfTarget:0];
		NSString *nameOfFirstAvailableTarget = [self.broadcastreceiver deviceNameOfTarget:0];
		
		// Now we have all informations to create a WiflyControl object. A connection to our selected target will be create automatically by initialisation of our object.
		
		self.control = [[WCWiflyControlWrapper alloc]initWithIP:ipOfFirstAvailableTarget port:portOfFirstAvailableTarget];
		NSLog(@"Connetion to %@ : %d %d created!\n", nameOfFirstAvailableTarget, ipOfFirstAvailableTarget, portOfFirstAvailableTarget);
		
		// Now we can play around with our control object
		
		// clear scriptcontroller to execute the next command immediately
		[self.control clearScript];
		
		// fade to red in 10 seconds
		[self.control setFade:0xffff0000 time:1000];
		
		// fade to green in 500 seconds
		[self.control setFade:0xff00ff00 time:50000];
		
		// fade half ledstrip to red in parallel with the next command
		[self.control setFade:0xffff0000 time:3000 address:0xffff0000 parallelFade:true];
		
		// the other side of the ledstripe is fade to white with the same timevalue of the above command
		[self.control setFade:0xffffffff time:3000 address:0x0000ffff parallelFade:false];
		
		// now we want to create a small script for our lamp
		// tell our lamp that we want to program a script. With loopOn we mark the top of our script
		[self.control loopOn];
		
		[self.control setGradientWithColor:0xffff0000 colorTwo:0xff00ff00 time:5000];
		[self.control setFade:0xffffff00 time:5000];
		[self.control setGradientWithColor:0xff00ff00 colorTwo:0xffff0000 time:5000];
		[self.control setFade:0xffffff00 time:5000];

		// enough now... Now we want to execute this loop 5 times. If you want to execute a loop as infinite loop, we have to choose 0 as numberOfRepeats f.e. [self.control loopOffWithNumberOfRepeats:0];
		[self.control loopOffAfterNumberOfRepeats:5];
		
		// every function of the WCWiflyControlWrapper has a returnvalue. See documentation for further informations. 0 means no error
		// if we want to react to communication errors or other failures we can check the return value
		
		uint32_t returnValue = [self.control clearScript];
		if(returnValue)
		{
			NSLog(@"Error %d occured.", returnValue);
		}
		
		// dealloc object to close connection. The connection to our target is closed automatically if we throw away our WCWiflyControlWrapper object.
		[self.control dealloc];
		
		self.control = nil;
	}
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
