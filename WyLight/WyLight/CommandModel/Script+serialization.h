//
//  Script+serialization.h
//  WyLight
//
//  Created by Nils Weiß on 29/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Script.h"

@interface Script (serialization)

- (NSString *) serializeToPath:(NSString *)path;
+ (Script *)deserializeScriptFromPath:(NSURL *)path inContext:(NSManagedObjectContext *)context;

@end
