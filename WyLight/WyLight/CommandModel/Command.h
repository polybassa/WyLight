//
//  Command.h
//  WyLight
//
//  Created by Nils Weiß on 03/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class WCWiflyControlWrapper;

@interface Command : NSManagedObject

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control;

@end
