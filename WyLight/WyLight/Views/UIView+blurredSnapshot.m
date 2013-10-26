//
//  UIView+blurredSnapshot.m
//  WyLight
//
//  Created by Nils Weiß on 10/21/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "UIView+blurredSnapshot.h"
#import "UIImage+ImageEffects.h"

@implementation UIView (blurredSnapshot)

static BOOL _supportDrawViewHierarchyInRect;

+ (void)load {
    if ([self instancesRespondToSelector:@selector(drawViewHierarchyInRect:afterScreenUpdates:)]) {
        _supportDrawViewHierarchyInRect = YES;
    } else {
        _supportDrawViewHierarchyInRect = NO;
    }
}

-(UIImage *)blurredSnapshot
{
    UIGraphicsBeginImageContextWithOptions(self.bounds.size, YES, 0.0);
    
    [self.layer renderInContext:UIGraphicsGetCurrentContext()];
   
    UIImage *snapshotImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    UIColor *tintColor = [UIColor colorWithWhite:1.0 alpha:0.3];
    UIImage *blurredSnapshotImage = [snapshotImage applyBlurWithRadius:5 tintColor:tintColor saturationDeltaFactor:1.0 maskImage:nil];
    
    return blurredSnapshotImage;
}


@end
