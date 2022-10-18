/*
        NSStepperCell.h
        Application Kit
        Copyright (c) 2000-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSActionCell.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSStepperCell : NSActionCell

@property double minValue;

@property double maxValue;

@property double increment;

@property BOOL valueWraps;

@property BOOL autorepeat;

@end

API_UNAVAILABLE_END
