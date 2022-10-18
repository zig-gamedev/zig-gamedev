/*
 NSMeasurementFormatter.h
 Copyright (c) 2015-2019, Apple Inc.
 All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSFormatter.h>
#import <Foundation/NSNumberFormatter.h>
#import <Foundation/NSMeasurement.h>
#import <Foundation/NSLocale.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSMeasurementFormatterUnitOptions) {
    NSMeasurementFormatterUnitOptionsProvidedUnit = (1UL << 0),              // e.g  This ensures the formatter uses this unit even if it is not the preferred unit of the set locale.
    NSMeasurementFormatterUnitOptionsNaturalScale = (1UL << 1),              // e.g. This would make the formatter show "12 kilometers" instead of "12000 meters".  Note that setting NSMeasurementFormatterUnitOptionsNaturalScale results in scaling within the unit system of the preferred unit of the locale.  To scale within the unit system of the provided unit, set NSMeasurementFormatterUnitOptionsNaturalScale | NSMeasurementFormatterUnitOptionsProvidedUnit.
    NSMeasurementFormatterUnitOptionsTemperatureWithoutUnit = (1UL << 2),    // e.g. This would display "90°" rather than "90°F" or "90°C"
} API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSMeasurementFormatter : NSFormatter <NSSecureCoding> {
@private
    void *_formatter;
}

/*
 This property can be set to ensure that the formatter behaves in a way the developer expects, even if it is not standard according to the preferences of the user's locale. If not specified, unitOptions defaults to localizing according to the preferences of the locale.
 
 Ex:
 
 By default, if unitOptions is set to the empty set, the formatter will do the following:
    - kilocalories may be formatted as "C" instead of "kcal" depending on the locale.
    - kilometersPerHour may be formatted as "miles per hour" for US and UK locales but "kilometers per hour" for other locales.
 
 However, if NSMeasurementFormatterUnitOptionsProvidedUnit is set, the formatter will do the following:
    - kilocalories would be formatted as "kcal" in the language of the locale, even if the locale prefers "C".
    - kilometersPerHour would be formatted as "kilometers per hour" for US and UK locales even though the preference is for "miles per hour."

 Note that NSMeasurementFormatter will handle converting measurement objects to the preferred units in a particular locale.  For instance, if provided a measurement object in kilometers and the set locale is en_US, the formatter will implicitly convert the measurement object to miles and return the formatted string as the equivalent measurement in miles.

 */
@property NSMeasurementFormatterUnitOptions unitOptions;

/*
 If not specified, unitStyle is set to NSFormattingUnitStyleMedium.
 */
@property NSFormattingUnitStyle unitStyle;

/*
 If not specified, locale is set to the user's current locale.
 */
@property (null_resettable, copy) NSLocale *locale;

/*
 If not specified, the number formatter is set up with NSNumberFormatterDecimalStyle.
 */
@property (null_resettable, copy) NSNumberFormatter *numberFormatter;

- (NSString *)stringFromMeasurement:(NSMeasurement *)measurement;

/*
 @param An NSUnit
 @return A formatted string representing the localized form of the unit without a value attached to it.  This method will return [unit symbol] if the provided unit cannot be localized.
 */
- (NSString *)stringFromUnit:(NSUnit *)unit;

@end

NS_ASSUME_NONNULL_END
