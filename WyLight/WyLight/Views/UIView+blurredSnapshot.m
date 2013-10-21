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

-(UIImage *)blurredSnapshot
{
    // Create the image context
    UIGraphicsBeginImageContextWithOptions(self.bounds.size, self.opaque, 0.0);
    
    // There he is! The new API method
    //[self drawViewHierarchyInRect:self.frame afterScreenUpdates:YES];
    [self.layer renderInContext:UIGraphicsGetCurrentContext()];
    // Get the snapshot
    UIImage *snapshotImage = UIGraphicsGetImageFromCurrentImageContext();
    
    UIColor *tintColor = [UIColor colorWithWhite:1.0 alpha:0.3];
    UIImage *blurredSnapshotImage = [snapshotImage applyBlurWithRadius:5 tintColor:tintColor saturationDeltaFactor:1.0 maskImage:nil];
    
    // Be nice and clean your mess up
    UIGraphicsEndImageContext();
    
    return blurredSnapshotImage;
}


@end
