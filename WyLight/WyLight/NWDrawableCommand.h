//
//  NWDrawableCommand.h
//  WyLightRemote
//
//  Created by Nils Weiß on 27.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol NWDrawableCommand <NSObject>

@property (nonatomic, readonly, strong) NSArray *colors;
@property (nonatomic, strong) UIColor *backgroundColor;

@end
