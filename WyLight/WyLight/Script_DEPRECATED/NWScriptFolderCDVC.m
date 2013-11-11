//
//  NWScriptFolderCDVC.m
//  WyLight
//
//  Created by Nils Weiß on 08/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWScriptFolderCDVC.h"
#import "Script+defaultScripts.h"
#import "ComplexEffect.h"

@interface NWScriptFolderCDVC ()

@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, strong) NSArray *scriptObjects;

@end

@implementation NWScriptFolderCDVC

- (void) viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    if (!self.managedObjectContext) {
        [self useDocument];
    }
}

- (void) viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [self refresh];
}

- (void)useDocument {
    NSURL *url = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
    url = [url URLByAppendingPathComponent:@"ScriptDocument"];
    UIManagedDocument *document = [[UIManagedDocument alloc] initWithFileURL:url];
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:[url path]]) {
        [document saveToURL:url forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success) {
            if (success) {
                self.managedObjectContext = document.managedObjectContext;
                [Script defaultScriptFastColorChangeInContext:self.managedObjectContext];
                [Script defaultScriptSlowColorChangeInContext:self.managedObjectContext];
                [Script defaultScriptRunLightWithColor:[UIColor redColor] timeInterval:100 inContext:self.managedObjectContext];
                [Script defaultScriptRandomColorsInContext:self.managedObjectContext];
                [Script defaultScriptMovingColorsInContext:self.managedObjectContext];
                [Script defaultScriptConzentrationLightInContext:self.managedObjectContext];
                [Script defaultScriptColorCrashWithTimeInterval:100 inContext:self.managedObjectContext];
                 NSError *error;
                [self.managedObjectContext save:&error];
                [self refresh];
            } else {
                NSLog(@"saveToURL failed");
            }
        }];
    } else if (document.documentState == UIDocumentStateClosed) {
        [document openWithCompletionHandler:^(BOOL success) {
            if (success) {
                self.managedObjectContext = document.managedObjectContext;
                [self refresh];
            }
        }];
    } else {
        self.managedObjectContext = document.managedObjectContext;
    }
}

- (void)refresh {
    if (self.managedObjectContext) {
        NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:[Script entityName]];
        
        NSError *error;
        self.scriptObjects = [self.managedObjectContext executeFetchRequest:request error:&error];
        if (self.scriptObjects == nil) {
            NSLog(@"ERROR FETCH");
        }
        
        Script *tempScriptObj = self.scriptObjects.lastObject;
        NSLog(@"%@", tempScriptObj.title);
        for (ComplexEffect *cmpl in tempScriptObj.effects) {
            NSLog(@"%@ - %d", cmpl.effects.lastObject, cmpl.duration.unsignedIntegerValue);
        }
    }

}

@end
