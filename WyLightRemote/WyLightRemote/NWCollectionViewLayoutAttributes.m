//
//  NWCollectionViewLayoutAttributes.m
//  WyLightRemote
//
//  Created by Nils Weiß on 29.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWCollectionViewLayoutAttributes.h"

@implementation NWCollectionViewLayoutAttributes

- (id)copyWithZone:(NSZone *)zone {
    NWCollectionViewLayoutAttributes *attributes = [super copyWithZone:zone];
    attributes.deleteButtonHidden = _deleteButtonHidden;
    return attributes;
}

@end
