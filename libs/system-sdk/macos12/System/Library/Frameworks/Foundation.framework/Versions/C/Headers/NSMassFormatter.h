/*      NSMassFormatter.h
        Copyright (c) 2014-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSFormatter.h>

@class NSNumberFormatter;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSMassFormatterUnit) {
    NSMassFormatterUnitGram = 11,
    NSMassFormatterUnitKilogram = 14,
    NSMassFormatterUnitOunce = (6 << 8) + 1,
    NSMassFormatterUnitPound = (6 << 8) + 2,
    NSMassFormatterUnitStone = (6 << 8) + 3,
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
@interface NSMassFormatter : NSFormatter {
@private
    void *_formatter;
    BOOL _isForPersonMassUse;
    void *_reserved[2];
}

@property (null_resettable, copy) NSNumberFormatter *numberFormatter;    // default is NSNumberFormatter with NSNumberFormatterDecimalStyle
@property NSFormattingUnitStyle unitStyle;              // default is NSFormattingUnitStyleMedium
@property (getter = isForPersonMassUse) BOOL forPersonMassUse;  // default is NO; if it is set to YES, the number argument for -stringFromKilograms: and -unitStringFromKilograms: is considered as a person’s mass

// Format a combination of a number and an unit to a localized string.
- (NSString *)stringFromValue:(double)value unit:(NSMassFormatterUnit)unit;

// Format a number in kilograms to a localized string with the locale-appropriate unit and an appropriate scale (e.g. 1.2kg = 2.64lb in the US locale).
- (NSString *)stringFromKilograms:(double)numberInKilograms;

// Return a localized string of the given unit, and if the unit is singular or plural is based on the given number.
- (NSString *)unitStringFromValue:(double)value unit:(NSMassFormatterUnit)unit;

// Return the locale-appropriate unit, the same unit used by -stringFromKilograms:.
- (NSString *)unitStringFromKilograms:(double)numberInKilograms usedUnit:(nullable NSMassFormatterUnit *)unitp;

// No parsing is supported. This method will return NO.
- (BOOL)getObjectValue:(out id _Nullable * _Nullable)obj forString:(NSString *)string errorDescription:(out NSString * _Nullable * _Nullable)error;
@end

NS_ASSUME_NONNULL_END

