/*
	NSDatePickerCell.h
	Application Kit
	Copyright (c) 2004-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSDate.h>
#import <AppKit/NSActionCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_ENUM(NSUInteger, NSDatePickerStyle) {
    NSDatePickerStyleTextFieldAndStepper    = 0,
    NSDatePickerStyleClockAndCalendar       = 1,
    NSDatePickerStyleTextField              = 2
};

typedef NS_ENUM(NSUInteger, NSDatePickerMode) {
    NSDatePickerModeSingle = 0,
    NSDatePickerModeRange = 1
};

typedef NS_OPTIONS(NSUInteger, NSDatePickerElementFlags) {
    /* Time Elements */
    NSDatePickerElementFlagHourMinute       = 0x000c,
    NSDatePickerElementFlagHourMinuteSecond = 0x000e,
    NSDatePickerElementFlagTimeZone	    = 0x0010,

    /* Date Elements */
    NSDatePickerElementFlagYearMonth	    = 0x00c0,
    NSDatePickerElementFlagYearMonthDay	    = 0x00e0,
    NSDatePickerElementFlagEra		    = 0x0100,
};

@class NSCalendar, NSLocale, NSTimeZone;
@protocol NSDatePickerCellDelegate;

@interface NSDatePickerCell : NSActionCell

- (instancetype)initTextCell:(NSString *)string NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (instancetype)initImageCell:(nullable NSImage *)image NS_UNAVAILABLE;

#pragma mark *** Appearance Control ***

@property NSDatePickerStyle datePickerStyle;

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

#pragma mark *** Delegate ***

@property (nullable, weak) id<NSDatePickerCellDelegate> delegate;

@end

@protocol NSDatePickerCellDelegate <NSObject>
@optional
- (void)datePickerCell:(NSDatePickerCell *)datePickerCell validateProposedDateValue:(NSDate * _Nonnull *_Nonnull)proposedDateValue timeInterval:(nullable NSTimeInterval *)proposedTimeInterval;
@end

/* Deprecated legacy date picker style constants. Prefer to use NSDatePickerStyle values instead.
*/
static const NSDatePickerStyle NSTextFieldAndStepperDatePickerStyle API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerStyleTextFieldAndStepper", macos(10.0, 11.0)) = NSDatePickerStyleTextFieldAndStepper;
static const NSDatePickerStyle NSClockAndCalendarDatePickerStyle API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerStyleClockAndCalendar", macos(10.0, 11.0)) = NSDatePickerStyleClockAndCalendar;
static const NSDatePickerStyle NSTextFieldDatePickerStyle API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerStyleTextField", macos(10.0, 11.0)) = NSDatePickerStyleTextField;

/* Deprecated legacy date picker mode constants. Prefer to use NSDatePickerMode values instead.
*/
static const NSDatePickerMode NSSingleDateMode API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerModeSingle", macos(10.0, 11.0)) = NSDatePickerModeSingle;
static const NSDatePickerMode NSRangeDateMode API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerModeRange", macos(10.0, 11.0)) = NSDatePickerModeRange;

/* Deprecated legacy date picker element flag constants. Prefer to use NSDatePickerElementFlags values instead.
*/
static const NSDatePickerElementFlags NSHourMinuteDatePickerElementFlag API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerElementFlagHourMinute", macos(10.0, 11.0)) = NSDatePickerElementFlagHourMinute;
static const NSDatePickerElementFlags NSHourMinuteSecondDatePickerElementFlag API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerElementFlagHourMinuteSecond", macos(10.0, 11.0)) = NSDatePickerElementFlagHourMinuteSecond;
static const NSDatePickerElementFlags NSTimeZoneDatePickerElementFlag API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerElementFlagTimeZone", macos(10.0, 11.0)) = NSDatePickerElementFlagTimeZone;
static const NSDatePickerElementFlags NSYearMonthDatePickerElementFlag API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerElementFlagYearMonth", macos(10.0, 11.0)) = NSDatePickerElementFlagYearMonth;
static const NSDatePickerElementFlags NSYearMonthDayDatePickerElementFlag API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerElementFlagYearMonthDay", macos(10.0, 11.0)) = NSDatePickerElementFlagYearMonthDay;
static const NSDatePickerElementFlags NSEraDatePickerElementFlag API_DEPRECATED_WITH_REPLACEMENT("NSDatePickerElementFlagEra", macos(10.0, 11.0)) = NSDatePickerElementFlagEra;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

