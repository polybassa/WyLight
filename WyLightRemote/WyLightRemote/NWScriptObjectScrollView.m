//
//  NWScriptObjectScrollView.m
//  WyLightRemote
//
//  Created by Nils Weiß on 28.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectScrollView.h"

@interface NWScriptObjectScrollView	()

@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, strong) NSMutableArray *objects;

@end

@implementation NWScriptObjectScrollView

const NSUInteger padding = 10;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self setup];
    }
    return self;
}

- (NSMutableArray *)objects {
	if (_objects == nil) {
		_objects = [[NSMutableArray alloc]init];
	}
	return _objects;
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	
	self.scrollView.frame = CGRectMake(0, 0, frame.size.width, frame.size.height);
	[self fixLocations];
}

- (void)fixLocations {
		
	self.scrollView.contentSize = CGSizeMake(self.objects.count * 100 + 2 * padding, self.frame.size.height - 2 * padding);

	[UIView animateWithDuration:0.4 animations:^{
		for (UIView* subview in self.scrollView.subviews) {
			if ([subview isKindOfClass:[NWScriptObjectButton class]]) {
				subview.frame = CGRectMake(padding + (((NWScriptObjectButton*)subview).position - 1) * 100, padding, 100 - 2 * padding, self.scrollView.frame.size.height - 2 * padding);
			}
		}
	}];
}

- (void)setup {
	self.scrollView = [[UIScrollView alloc] initWithFrame:CGRectZero];
	self.scrollView.backgroundColor = [UIColor clearColor];
	[self.scrollView setUserInteractionEnabled:YES];
	[self setUserInteractionEnabled:YES];
	[self addSubview:self.scrollView];
}

- (void)reloadDataFromArray:(NSArray *)array
{
	self.objects = nil;
	
	for (NSObject *obj in array) {
		if ([obj isKindOfClass:[NWScriptObjectButton class]]) {
			NWScriptObjectButton *button = (NWScriptObjectButton *)obj;
			[self.objects addObject:button];
			button.scrollParent = self.scrollView;
			button.mainView = self.mainView;
			button.delegate = self;
			button.tag = self.objects.count;
			button.position = self.objects.count;
			[self.scrollView addSubview:button];
		}
	}
	[self fixLocations];
}

- (BOOL)isInNewPosition:(NWScriptObjectButton *)button touching:(BOOL)finished
{
	NSUInteger newPos = [self calculatePositionInView:self.scrollView forButton:button];
	if (newPos != button.position) {
		NSUInteger oldPos = button.position;
		button.position = 0;
		
		for (UIView* subview in self.scrollView.subviews) {
			if ([subview isKindOfClass:[NWScriptObjectButton class]]) {
				if (((NWScriptObjectButton *)subview).position == newPos) {
					((NWScriptObjectButton *)subview).position = oldPos;
					break;
				}
			}
		}
		
		button.position = newPos;
		[self.delegate positionInScrollViewChanged:button];
		[self fixLocations];
		return YES;
	}
	
	return NO;
}

- (CGPoint)newOriginalPosition:(NWScriptObjectButton *)button {
	CGFloat xCenterOfButton = (button.frame.size.width / 2) + padding;
	return CGPointMake(xCenterOfButton + ((button.position - 1) * (button.frame.size.width + 2 * padding)), self.scrollView.center.y + self.scrollView.frame.origin.y);
}

- (NSUInteger)calculatePositionInView:(UIScrollView *)view forButton:(NWScriptObjectButton *)button
{
	CGFloat totalWidth = view.contentSize.width;
	CGFloat buttonsInView = totalWidth / (button.frame.size.width + padding);
	
	for (NSUInteger i = 0; i < buttonsInView; i++) {
		CGFloat lowerBorder = i * (button.frame.size.width + padding * 2);
		CGFloat upperBorder = (i + 1) * (button.frame.size.width + padding * 2);
		
		if (button.center.x > lowerBorder && button.center.x < upperBorder) {
			return i + 1;
		}
	}
	return buttonsInView;
}

@end
