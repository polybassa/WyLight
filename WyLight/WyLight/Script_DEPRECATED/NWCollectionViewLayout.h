//
//  NWCollectionViewLayout.h
//  WyLightRemote
//
//  Created by Nils Weiß on 29.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol NWCollectionViewLayoutDelegate

- (BOOL)isDeletionModeActiveForCollectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout;

@end

@interface NWCollectionViewLayout : UICollectionViewFlowLayout

@end
