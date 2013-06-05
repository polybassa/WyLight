//
//  WCBroadcastReceiverWrapper.m
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres, Nils Weiß. All rights reserved.
//

#import "WCBroadcastReceiverWrapper.h"

#include "BroadcastReceiver.h"
#include <iostream>
#include <sstream>
#include <thread>

@interface WCBroadcastReceiverWrapper () {
	std::stringstream *mStream;
	WyLight::BroadcastReceiver *receiver;
	std::thread *receiverThread;
}

#if !__has_feature(objc_arc)
@property (nonatomic, unsafe_unretained) NSThread* threadOfOwner;
#else
@property (nonatomic, weak) NSThread* threadOfOwner;
#endif

- (void)postNotification;

@end

@implementation WCBroadcastReceiverWrapper

NSString *const NewTargetAddedNotification = @"NewTargetAddedNotification";

- (id)init
{
    self = [super init];
    if (self)
    {
        // Start BroadcastReceiver
        mStream = new std::stringstream();
		self.threadOfOwner = [NSThread currentThread];
		
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
		
		NSMutableString *filePath = [[NSMutableString alloc] initWithString:basePath];
		[filePath appendString:@"/recv.txt"];

		receiver = new WyLight::BroadcastReceiver(55555,
												  [filePath cStringUsingEncoding:NSASCIIStringEncoding],
												  [=](const size_t index, const WyLight::Endpoint& endpoint)
													{
														NSLog(@"New: %zd : %d.%d.%d.%d, %d  %s",
															  index,
															  (endpoint.GetIp() >> 24) & 0xFF,
															  (endpoint.GetIp() >> 16) & 0xFF,
															  (endpoint.GetIp() >> 8) & 0xFF,
															  (endpoint.GetIp() & 0xFF),
															  endpoint.GetPort(),
															  endpoint.GetDeviceId().c_str());
		
														[self performSelector:@selector(postNotification) onThread:[self threadOfOwner] withObject:nil waitUntilDone:NO];
													}
												  );

		receiverThread = new std::thread(std::ref(*receiver));
		NSLog(@"start receiver");
	}
    return self;
}

- (void)stop
{
	if(receiver != NULL && receiverThread != NULL)
	{
		// Stop BroadcastReceiver
		receiver->Stop();
		receiverThread->join();
    }
	
    delete mStream;
    delete receiver;
    delete receiverThread;
	
	mStream = NULL;
	receiver = NULL;
	receiverThread = NULL;
}

- (void)dealloc
{
	[self stop];
#if !__has_feature(objc_arc)
    //Do manual memory management...
	[super dealloc];
#else
    //Usually do nothing...
#endif
	
}

- (size_t)numberOfTargets
{
    return receiver->NumRemotes();
}

- (uint32_t)ipAdressOfTarget:(size_t)index
{
    if(index >= [self numberOfTargets])
        return 0;
	
    WyLight::Endpoint mEndpoint = receiver->GetEndpoint(index);
    	
    return mEndpoint.GetIp();
}

- (void)increaseScoreOfIpAdress:(uint32_t)ipAdress
{
	for(size_t index = 0; index < [self numberOfTargets]; index++)
	{
		if((receiver->GetEndpoint(index)).GetIp() == ipAdress)
			++(receiver->GetEndpoint(index));
	}
}

- (void)SetScoreOfIpAdress:(uint32_t)ipAdress Score:(uint8_t)score
{
	for(size_t index = 0; index < [self numberOfTargets]; index++)
	{
		if(receiver->GetEndpoint(index).GetIp() == ipAdress)
			receiver->GetEndpoint(index).SetScore(score);
	}
}


- (uint16_t)portOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    WyLight::Endpoint mEndpoint = receiver->GetEndpoint(index);
    
    return mEndpoint.GetPort();
}

- (uint8_t)scoreOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    WyLight::Endpoint mEndpoint = receiver->GetEndpoint(index);
    
    return mEndpoint.GetScore();
}


- (NSString *)deviceNameOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    WyLight::Endpoint mEndpoint = receiver->GetEndpoint(index);
    
    std::string mStr = mEndpoint.GetDeviceId();
   
    return [NSString stringWithCString:mStr.c_str() encoding:NSASCIIStringEncoding];
}

- (void)saveTargets
{
	receiver->WriteRecentEndpoints();
}

- (void)postNotification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:NewTargetAddedNotification object:self];
}

@end
