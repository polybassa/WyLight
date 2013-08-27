//
//  NWScriptViewController.m
//  WyLightRemote
//
//  Created by Nils Weiß on 08.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptViewController.h"
#import "NWScriptObjectButton.h"
#import "NWScriptObject.h"
#import "NWScript.h"

@interface NWScriptViewController () <UIScrollViewDelegate>
@property (strong, nonatomic) UIScrollView *scrollView;
@property (strong, nonatomic) NWScript *script;
@property (strong, nonatomic) NSMutableArray *scriptButtons;
@property (weak, nonatomic) IBOutlet NWScriptObjectButton *button;
@end

@implementation NWScriptViewController

- (NSMutableArray *)scriptButtons
{
	if (!_scriptButtons) {
		_scriptButtons = [[NSMutableArray alloc]init];
	}
	return _scriptButtons;
}

- (void) viewDidLoad
{
	[super viewDidLoad];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
	if ([segue.identifier isEqualToString:@"edit:"])
	{
		if ([segue.destinationViewController respondsToSelector:@selector(setControlHandle:)])
		{
			[segue.destinationViewController performSelector:@selector(setControlHandle:) withObject:self.controlHandle];
		}
	}
}

- (void) viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	self.scrollView = [[UIScrollView alloc]initWithFrame:CGRectMake(0, 100, self.view.frame.size.width, self.view.frame.size.height)];
	[self.view addSubview:self.scrollView];
	self.scrollView.contentSize = CGSizeMake([self.script.totalDurationInTmms floatValue], self.view.bounds.size.height);
	self.scrollView.delegate = self;
	
	for (unsigned int i = 0; i < self.script.allVisibleScriptBlocks.count; i++) {
		NSArray* scriptObjects = [self.script.allVisibleScriptBlocks objectForKey:@(i)];
		NWScriptObjectButton *prevButton = nil;
		if (i > 0 && ([self.scriptButtons count] > (i - 1))) {
			prevButton = [self.scriptButtons objectAtIndex:i - 1];
		}
		NWScriptObjectButton *button = [[NWScriptObjectButton alloc]initWithFrame:
										CGRectMake(
												   (i == 0) ? 20 : prevButton.frame.origin.x + prevButton.frame.size.width + 1,
												   self.scrollView.frame.origin.y,
												   [[(NWScriptObject*)[scriptObjects lastObject] duration] floatValue],
												   96)];
		[self.scrollView addSubview:button];
		button.startColors = [[scriptObjects lastObject] startColors];
		button.endColors = [NWScriptObject mergedColorsFromArray:scriptObjects];
		[button addGestureRecognizer:[[UIPinchGestureRecognizer alloc]initWithTarget:button action:@selector(pinch:)]];
		[button addTarget:self action:@selector(wasDragged:withEvent:) forControlEvents:UIControlEventTouchDragInside];
		
		[self.scriptButtons addObject:button];
	}

	
	/*
	
	for (unsigned int i = 0; i < self.script.completeScript.count; i++) {
		NWScriptObject* scriptObj = [self.script.completeScript objectAtIndex:i];
		NWScriptObjectButton *prevButton = nil;
		if (i > 0 && ([self.scriptButtons count] > (i - 1))) {
			prevButton = [self.scriptButtons objectAtIndex:i - 1];
		}
		NWScriptObjectButton *button = [[NWScriptObjectButton alloc]initWithFrame:
										CGRectMake(
												   (i == 0) ? 20 : prevButton.frame.origin.x + prevButton.frame.size.width + 1,
												   self.scrollView.center.y,
												   [scriptObj.duration floatValue],
												   96)];
		[self.scrollView addSubview:button];
		button.startColors = scriptObj.startColors;
		button.endColors = scriptObj.endColors;
		[button addGestureRecognizer:[[UIPinchGestureRecognizer alloc]initWithTarget:button action:@selector(pinch:)]];
		[button addTarget:self action:@selector(wasDragged:withEvent:) forControlEvents:UIControlEventTouchDragInside];
		
		[self.scriptButtons addObject:button];
		
	}*/
}

- (void)wasDragged:(UIButton *)button withEvent:(UIEvent *)event
{
	// get the touch
	UITouch *touch = [[event touchesForView:button] anyObject];
	
	// get delta
	CGPoint previousLocation = [touch previousLocationInView:button];
	CGPoint location = [touch locationInView:button];
	CGFloat delta_x = location.x - previousLocation.x;
	CGFloat delta_y = location.y - previousLocation.y;
	
	// move button
	button.center = CGPointMake(button.center.x + delta_x,
								button.center.y + delta_y);
}

- (NWScript *)script
{
	if (!_script) {
		_script = [[NWScript alloc]init];
		for (int i = 0; i < 20; i++) {
			NWScriptObject *obj = [[NWScriptObject alloc]init];
			obj.command = [NWScriptObject randomCommand];
			[_script addObject:obj];
		}
	}
	return _script;
}
@end
