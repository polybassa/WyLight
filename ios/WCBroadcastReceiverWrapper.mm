//
//  WCBroadcastReceiverWrapper.m
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres, Nils Weiß. All rights reserved.
//

#import "WCBroadcastReceiverWrapper.h"
#import "WCEndpoint.h"

#include "BroadcastReceiver.h"
#include <iostream>
#include <memory>
#include <thread>

@interface WCBroadcastReceiverWrapper () {
	std::shared_ptr<WyLight::BroadcastReceiver> receiver;
	std::shared_ptr<std::thread> receiverThread;
}

@property (nonatomic, weak) NSThread *threadOfOwner;
@property (nonatomic, strong) NSMutableArray *arrayOfEndpoints;

- (void)postNotification;

@end

@implementation WCBroadcastReceiverWrapper

NSString *const TargetsChangedNotification = @"TargetsChangedNotification";

- (id)init
{
	self = [super init];
	if(self) {
		// Start BroadcastReceiver
		self.threadOfOwner = [NSThread currentThread];

		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *basePath = ([paths count] > 0) ?[paths objectAtIndex:0] : nil;

		NSMutableString *filePath = [[NSMutableString alloc] initWithString:basePath];
		[filePath appendString:@"/recv.txt"];

		receiver = std::make_shared<WyLight::BroadcastReceiver>
				(55555,
				filePath.UTF8String,
				[ = ](const size_t index, const WyLight::Endpoint& endpoint)
				{
					NSLog(@"New: %zd : %d.%d.%d.%d, %d  %s",
					      index,
					      (endpoint.GetIp() >> 24) & 0xFF,
					      (endpoint.GetIp() >> 16) & 0xFF,
					      (endpoint.GetIp() >> 8) & 0xFF,
					      (endpoint.GetIp() & 0xFF),
					      endpoint.GetPort(),
					      endpoint.GetDeviceId().c_str());

					WCEndpoint *endPoint = [[WCEndpoint alloc] initWithIpAdress:endpoint.GetIp()
								port:endpoint.GetPort()
								name:[NSString stringWithCString:endpoint.GetDeviceId().c_str() encoding:NSASCIIStringEncoding]
								score:endpoint.GetScore()];
					dispatch_async(dispatch_get_main_queue(), ^{
							       [self.arrayOfEndpoints insertObject:endPoint atIndex:index];
							       [self postNotification];
						       }
						       );
				}
				);
	}
	return self;
}

- (void)start
{
	if(receiver && !receiverThread) {
		receiverThread = std::make_shared<std::thread>(std::ref(*receiver));
		receiver->ReadRecentEndpoints();
		NSLog(@"start receiver");
		[self postNotification];
	} else {
		NSLog(@"receiver is already running");
	}
}

- (void)stop
{
	if(receiver && receiverThread) {
		// Stop BroadcastReceiver
		receiver->Stop();
		receiverThread->join();
	}
	receiverThread.reset();
}

- (void)dealloc
{
	[self stop];
	receiver.reset();
#if !__has_feature(objc_arc)
	//Do manual memory management...
	[super dealloc];
#else
	//Usually do nothing...
#endif

}

- (NSMutableArray *)arrayOfEndpoints {
	if(_arrayOfEndpoints == nil) {
		_arrayOfEndpoints = [[NSMutableArray alloc] init];
	}
	return _arrayOfEndpoints;
}

- (NSArray *)targets
{
	return self.arrayOfEndpoints;
}

- (void)setWCEndpointAsFavorite:(WCEndpoint *)endpoint
{
	for(size_t index = 0; index < receiver->NumRemotes(); index++) {
		if((receiver->GetEndpoint(index)).GetIp() == endpoint.ipAdress)
			++(receiver->GetEndpoint(index));
	}
	[self postNotification];
}

- (void)unsetWCEndpointAsFavorite:(WCEndpoint *)endpoint
{
	for(size_t index = 0; index < receiver->NumRemotes(); index++) {
		if((receiver->GetEndpoint(index)).GetIp() == endpoint.ipAdress) {
			receiver->GetEndpoint(index).SetScore(0);
		}
	}
}

- (void)saveTargets
{
	receiver->WriteRecentEndpoints();
}

- (void)postNotification
{
	[[NSNotificationCenter defaultCenter] postNotificationName:TargetsChangedNotification object:self];
}

- (void)clearTargets
{
	receiver->DeleteRecentEndpointFile();
	self.arrayOfEndpoints = nil;
}

@end
