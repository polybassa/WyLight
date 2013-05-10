//
//  WCBroadcastReceiverWrapper.m
//
//  Created by Bastian Kres on 16.04.13.
//  Copyright (c) 2013 Bastian Kres. All rights reserved.
//

#import "WCBroadcastReceiverWrapper.h"

#include "BroadcastReceiver.h"
#include <iostream>
#include <sstream>
#include <thread>

@interface WCBroadcastReceiverWrapper ()

@property (nonatomic) std::stringstream *mStream;
@property (nonatomic) WyLight::BroadcastReceiver *receiver;
@property (nonatomic) std::thread *receiverThread;

- (void)postNotification;

@end

void cNotification(WCBroadcastReceiverWrapper* receiver,NSThread* targetThread , const WyLight::Endpoint& endpoint)
{
	[receiver performSelector:@selector(postNotification) onThread:targetThread withObject:nil waitUntilDone:NO];
}

@implementation WCBroadcastReceiverWrapper

- (id)init
{
    self = [super init];
    if (self)
    {
        // Start BroadcastReceiver
        self.mStream = new std::stringstream();
        self.receiver = new WyLight::BroadcastReceiver(55555);
		self.receiver->SetCallbackAddedNewRemote(std::bind(&cNotification, self, [NSThread currentThread], std::placeholders::_1));
#ifndef DEBUG
        self.receiverThread = new std::thread(std::ref(*self.receiver), std::ref(*self.mStream));
#else
		self.receiverThread = new std::thread(std::ref(*self.receiver), std::ref(std::cout));
		NSLog(@"start receiver");
#endif
	}
    return self;
}

- (void)dealloc
{
    // Stop BroadcastReceiver
    (*self.receiver).Stop();
    (*self.receiverThread).join();
    
    delete self.mStream;
    delete self.receiver;
    delete self.receiverThread;
    
    self.mStream = NULL;
    self.receiver = NULL;
    self.receiverThread = NULL;
}

- (size_t)numberOfTargets
{
    return (*self.receiver).NumRemotes();
}

- (uint32_t)ipAdressOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    WyLight::Endpoint mEndpoint = (*self.receiver).GetEndpoint(index);
    
    return mEndpoint.GetIp();
}

- (uint16_t)portOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    WyLight::Endpoint mEndpoint = (*self.receiver).GetEndpoint(index);
    
    return mEndpoint.GetPort();
}

- (NSString *)deviceNameOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    WyLight::Endpoint mEndpoint = (*self.receiver).GetEndpoint(index);
    
    std::string mStr = mEndpoint.GetDeviceId();
   
    return [NSString stringWithCString:mStr.c_str() encoding:NSASCIIStringEncoding];
}

- (void)postNotification
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"NewTargetAddedNotification" object:self];
}

@end
