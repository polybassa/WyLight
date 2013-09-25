//
//  NWAddCollectionViewCell.m
//  WyLightRemote
//
//  Created by Nils Weiß on 13.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAddCollectionViewCell.h"
#import "NWCollectionViewLayoutAttributes.h"
#import "NWScriptObjectView.h"

@implementation NWAddCollectionViewCell


- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
		UIView *insetView = [[UIView alloc] initWithFrame:CGRectInset(self.bounds, self.bounds.size.width/16, self.bounds.size.height/16)];
        [self.contentView addSubview:insetView];
		
		NWScriptObjectView *scriptObjectView = [[NWScriptObjectView alloc] initWithFrame:CGRectMake(0, 0, insetView.frame.size.width, insetView.frame.size.height)];
		scriptObjectView.backgroundColor = [UIColor blackColor];
        [insetView addSubview:scriptObjectView];

		
		self.button = [UIButton buttonWithType:UIButtonTypeContactAdd];
		self.button.frame = CGRectMake(0, 0, insetView.frame.size.width, insetView.frame.size.height);
		self.button.enabled = YES;
        
        [insetView addSubview:self.button];
		
		}
    return self;
}

- (void)applyLayoutAttributes:(NWCollectionViewLayoutAttributes *)layoutAttributes {
	self.button.enabled = layoutAttributes.isDeleteButtonHidden;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
