//
//  NWTimeValueEditView.m
//  WyLight
//
//  Created by Nils Weiß on 12.10.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "NWTimeValueEditView.h"
#import "ComplexEffect.h"

@interface NWTimeValueEditView () <UIPickerViewDelegate>

@property (nonatomic, strong) UIDatePicker *timePicker;
@property (nonatomic, strong) UIPickerView *pickerView;
@property (nonatomic, strong) NSMutableArray *minsArray;
@property (nonatomic, strong) NSMutableArray *secsArray;
@property (nonatomic, strong) NSMutableArray *millisecsArray;

@end

@implementation NWTimeValueEditView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleHeight;
		self.clipsToBounds = YES;

        self.pickerView = [[UIPickerView alloc] initWithFrame:CGRectZero];
        self.pickerView.delegate = self;
        [self addSubview:self.pickerView];
		
		[self setFrame:frame];
        // Initialization code
        
        self.minsArray = [[NSMutableArray alloc] init];
        self.secsArray = [[NSMutableArray alloc] init];
        self.millisecsArray = [[NSMutableArray alloc] init];
        
        for(int i=0; i<60; i++)
        {
            NSString *strMin = [NSString stringWithFormat:@"%dm", i];
            NSString *strSec = [NSString stringWithFormat:@"%ds", i];
            NSString *strMil = [NSString stringWithFormat:@"%dms", i * 100];
            
            if (i < 10)
            {
                [self.minsArray addObject:strMin];
            }
            [self.secsArray addObject:strSec];
            
            if (i < 10) {
                [self.millisecsArray addObject:strMil];
            }
        }
    }
    return self;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    [self calculateTimeFromPicker];
}

//Method to define how many columns/dials to show
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 3;
}

// Method to define the numberOfRows in a component using the array.
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent :(NSInteger)component
{
    if (component==0)
    {
        return [self.minsArray count];
    }
    else if (component==1)
    {
        return [self.secsArray count];
    }
    else
    {
        return [self.millisecsArray count];
    }
}


// Method to show the title of row for a component.
- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    switch (component)
    {
        case 0:
            return [self.minsArray objectAtIndex:row];
            break;
        case 1:
            return [self.secsArray objectAtIndex:row];
            break;
        case 2:
            return [self.millisecsArray objectAtIndex:row];
            break;
    }
    return nil;
}


- (void)calculateTimeFromPicker
{
    
    long long minsInt = [self.pickerView selectedRowInComponent:0] * 60 * 1000;
    long long secsInt = [self.pickerView selectedRowInComponent:1] * 1000;
    long long millisInt = [self.pickerView selectedRowInComponent:2] * 100;
    
    
    long long interval = (millisInt + secsInt + minsInt) / 10;
    
    [self.delegate TimeValueEditView:self timeValueChanged:@(interval)];
}

- (void)setFrame:(CGRect)frame {
	[super setFrame:frame];
	
    self.pickerView.frame = CGRectMake(0, (self.bounds.size.height - 162) / 2, self.bounds.size.width, 162);
}

- (void)setCornerRadius:(CGFloat)cornerRadius {
	self.layer.cornerRadius = cornerRadius;
	[self setNeedsDisplay];
}

- (void)setCommand:(ComplexEffect *)command {
	_command = command;
	[self reloadData];
}

- (void)reloadData {
    unsigned int milli, sec, min;
    long long total = self.command.duration.longLongValue;
    
    total = total / 10;
    milli = (int)total % 10;
    total = total / 10;
    sec = (int)total % 60;
    total = total / 60;
    min = (int)total;
    
    [self.pickerView selectRow:milli inComponent:2 animated:YES];
    [self.pickerView selectRow:sec inComponent:1 animated:YES];
    [self.pickerView selectRow:min inComponent:0 animated:YES];
}

@end
