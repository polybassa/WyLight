//
//  NWCollectionViewLayout.m
//  WyLightRemote
//
//  Created by Nils Weiß on 29.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWCollectionViewLayout.h"
#import "NWCollectionViewLayoutAttributes.h"

@implementation NWCollectionViewLayout

- (id)init {
    if (self = [super init])
    {
        self.itemSize = CGSizeMake(500, 500);
        self.minimumInteritemSpacing = 2;
        self.minimumLineSpacing = 2;
        self.scrollDirection = UICollectionViewScrollDirectionHorizontal;
        self.sectionInset = UIEdgeInsetsMake(10, 10, 10, 10);
    }
    return self;
}

- (BOOL)isDeletionModeOn {
    if ([[self.collectionView.delegate class] conformsToProtocol:@protocol(NWCollectionViewLayoutDelegate)])
    {
        return [(id)self.collectionView.delegate isDeletionModeActiveForCollectionView:self.collectionView layout:self];
    }
    return NO;
}

+ (Class)layoutAttributesClass {
    return [NWCollectionViewLayoutAttributes class];
}

- (NWCollectionViewLayoutAttributes *)layoutAttributesForItemAtIndexPath:(NSIndexPath *)indexPath {
    NWCollectionViewLayoutAttributes *attributes = (NWCollectionViewLayoutAttributes *)[super layoutAttributesForItemAtIndexPath:indexPath];
    if ([self isDeletionModeOn])
        attributes.deleteButtonHidden = NO;
	else
		attributes.deleteButtonHidden = YES;
	return attributes;
}

- (NSArray *)layoutAttributesForElementsInRect:(CGRect)rect {
    NSArray *attributesArrayInRect = [super layoutAttributesForElementsInRect:rect];
    for (NWCollectionViewLayoutAttributes *attribs in attributesArrayInRect)
    {
        if ([self isDeletionModeOn]) attribs.deleteButtonHidden = NO;
        else attribs.deleteButtonHidden = YES;
    }
    return attributesArrayInRect;
}

@end
