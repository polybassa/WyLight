//
//  NWAddScriptObjectView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 14.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAddScriptObjectView.h"

@implementation NWAddScriptObjectView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
		self.scriptObjectView = [[NWScriptObjectView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
		self.scriptObjectView.backgroundColor = [UIColor colorWithRed:0.9 green:0.9 blue:0.9 alpha:0.6];
		
		[self addSubview:self.scriptObjectView];
		self.button = [UIButton buttonWithType:UIButtonTypeContactAdd];
		self.button.frame = CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
        
        [self addSubview:self.button];
		
	}
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	self.scriptObjectView.frame = CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
	self.button.frame = CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
}

@end
