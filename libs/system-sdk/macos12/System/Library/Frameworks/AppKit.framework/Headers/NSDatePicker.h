/*
	NSDatePicker.h
	Application Kit
	Copyright (c) 2004-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/NSDatePickerCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSCalendar, NSLocale, NSTimeZone;

@interface NSDatePicker : NSControl
{
}

#pragma mark *** Appearance Control ***

@property NSDatePickerStyle datePickerStyle;

@property (getter=isBezeled) BOOL bezeled;

@property (getter=isBordered) BOOL bordered;

@property BOOL drawsBackground;

@property (copy) NSColor *backgroundColor;

@property (copy) NSColor *textColor;

#pragma mark *** Range Mode Control ***

@property NSDatePickerMode datePickerMode;

#pragma mark *** Date Presentation Control ***

@property NSDatePickerElementFlags datePickerElements;

@property (nullable, copy) NSCalendar *calendar;

@property (nullable, copy) NSLocale *locale;

@property (nullable, copy) NSTimeZone *timeZone;

#pragma mark *** Object Value Access ***

@property (copy) NSDate *dateValue;

@property NSTimeInterval timeInterval;

#pragma mark *** Constraints on Displayable/Selectable Range ***

@property (nullable, copy) NSDate *minDate;

@property (nullable, copy) NSDate *maxDate;

#pragma mark *** Calendar Overlay Support ***

@property BOOL presentsCalendarOverlay API_AVAILABLE(macos(10.15.4));

#pragma mark *** Delegate ***

@property (nullable, weak) id<NSDatePickerCellDelegate> delegate;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

