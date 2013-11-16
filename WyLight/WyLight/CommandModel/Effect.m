//
//  Effect.m
//  WyLight
//
//  Created by Nils Weiß on 16/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "Effect.h"


@implementation Effect

@dynamic backgroundColor;
@dynamic duration;
@dynamic colors;
@dynamic snapshot;

- (void)awakeFromInsert {
    [super awakeFromInsert];
    [self setBackgroundColor:[UIColor blackColor]];
}

- (void)sendToWCWiflyControl:(WCWiflyControlWrapper *)control {
    [[[NSException alloc] initWithName:@"Function not implemented" reason:@"Abstract Object:" userInfo:nil] raise];
}

@end
