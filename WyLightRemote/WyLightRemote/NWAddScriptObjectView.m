//
//  NWAddScriptObjectView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 14.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAddScriptObjectView.h"
#import "NWScriptObjectView.h"

@interface NWAddScriptObjectView ()

@property (nonatomic, strong) NWScriptObjectView *scriptObjectView;

@end

@implementation NWAddScriptObjectView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
		
		self.scriptObjectView = [[NWScriptObjectView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
		self.scriptObjectView.backgroundColor = [UIColor blackColor];
		self.scriptObjectView.borderWidth = 1;
		self.scriptObjectView.cornerRadius = 10;

        [self addSubview:self.scriptObjectView];
		
		
		self.button = [UIButton buttonWithType:UIButtonTypeContactAdd];
		self.button.frame = CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
		self.button.enabled = YES;
        
        [self addSubview:self.button];
		
	}
    return self;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	self.scriptObjectView.frame = CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
	self.button.frame = CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
	[self setNeedsDisplay];
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
