//
//  NWSendableCommand.h
//  WyLightRemote
//
//  Created by Nils Weiß on 27.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@class WCWiflyControlWrapper;

@protocol NWSendableCommand <NSObject>

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control;

@end
