//
//  WCBroadcastReceiverWrapper.m
//  WiflyCommander
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
@property (nonatomic) BroadcastReceiver *receiver;
@property (nonatomic) std::thread *receiverThread;

@end

@implementation WCBroadcastReceiverWrapper

- (id)init
{
    self = [super init];
    if (self)
    {
        // Start BroadcastReceiver
        self.mStream = new std::stringstream();
        self.receiver = new BroadcastReceiver(55555);
        //self.receiverThread = new std::thread(std::ref(*self.receiver), std::ref(*self.mStream));
        self.receiverThread = new std::thread(std::ref(*self.receiver), std::ref(std::cout));

        NSLog(@"start receiver");
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

/*
 - (NSString *)ipInfoStream
 {
 if((*self.receiver).NumRemotes() == 0)
 return @"No device online";
 
 char temp[256];
 (*self.mStream).getline(temp, sizeof(temp));
 
 return [NSString stringWithCString:temp encoding:NSASCIIStringEncoding];
 }
 */

- (uint32_t)ipAdressOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    Endpoint mEndpoint = (*self.receiver).GetEndpoint(index);
    
    return mEndpoint.GetIp();
}

- (uint16_t)portOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    Endpoint mEndpoint = (*self.receiver).GetEndpoint(index);
    
    return mEndpoint.GetPort();
}

- (NSString *)deviceNameOfTarget:(size_t)index
{
    if(index > [self numberOfTargets])
        return 0;
    Endpoint mEndpoint = (*self.receiver).GetEndpoint(index);
    
    std::string mStr = mEndpoint.GetDeviceId();
    //const char *temp = mStr.c_str();
    
    return [NSString stringWithCString:mStr.c_str() encoding:NSASCIIStringEncoding];
}

@end
