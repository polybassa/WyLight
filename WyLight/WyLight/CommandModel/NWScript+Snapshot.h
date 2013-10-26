//
//  NWScript+Snapshot.h
//  WyLight
//
//  Created by Nils Weiß on 25/10/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScript.h"
#import "NWScriptView.h"

@interface NWScript (Snapshot) //<NWScriptViewDataSource>

- (UIImage *)snapshotWithRect:(CGRect)rect;

@end
