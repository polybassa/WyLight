//
//  TouchAndHoldButton.h
//  WyLight
//
//  Created by Nils Weiß on 08.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TouchAndHoldButton : UIButton

- (void)addTarget:(id)target action:(SEL)action forTouchAndHoldControlEventWithTimeInterval:(NSTimeInterval)periodTime;

@end
