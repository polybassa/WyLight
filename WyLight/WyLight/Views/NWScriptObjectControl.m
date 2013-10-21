//
//  NWScriptObjectControl.m
//  WyLightRemote
//
//  Created by Nils Weiß on 31.08.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptObjectControl.h"
#import <QuartzCore/QuartzCore.h>
#import "UIView+Quivering.h"

@implementation NWScriptObjectControl

- (void)setQuivering:(BOOL)quivering {
	
	if (_quivering == quivering) {
		return;
	} else {
		_quivering = quivering;
		if (quivering) {
			[self startQuivering];
		} else {
			[self stopQuivering];
		}
	}
}

#define DOWNSCALEFACTOR 0.8

- (void)setDownscale:(BOOL)downscale {
	if (_downscale == downscale) {
		//nothing to do
		return;
	} else {
		if (downscale) {
			self.layer.transform = CATransform3DMakeScale(DOWNSCALEFACTOR, DOWNSCALEFACTOR, 1.0);
		} else {
			self.layer.transform = CATransform3DMakeScale(1.0, 1.0, 1.0);
		}
		_downscale = downscale;
	}
}

- (void)setFrame:(CGRect)frame {
	if (self.downscale) {
		self.layer.transform = CATransform3DMakeScale(1.0, 1.0, 1.0);
		[super setFrame:frame];
		self.layer.transform = CATransform3DMakeScale(DOWNSCALEFACTOR, DOWNSCALEFACTOR, 1.0);
	} else {
		[super setFrame:frame];
	}
}

@end
