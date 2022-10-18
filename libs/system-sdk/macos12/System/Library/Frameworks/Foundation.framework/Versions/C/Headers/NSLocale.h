/*	NSLocale.h
	Copyright (c) 2003-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <CoreFoundation/CFLocale.h>
#import <Foundation/NSNotification.h>

@class NSCalendar;

typedef NSString * NSLocaleKey NS_TYPED_ENUM;

@class NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSString;

// Toll-free bridged with CFLocaleRef

NS_ASSUME_NONNULL_BEGIN

@interface NSLocale : NSObject <NSCopying, NSSecureCoding>

- (nullable id)objectForKey:(NSLocaleKey)key;

- (nullable NSString *)displayNameForKey:(NSLocaleKey)key value:(id)value;

- (instancetype)initWithLocaleIdentifier:(NSString *)string NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end

@interface NSLocale (NSExtendedLocale)

@property (readonly, copy) NSString *localeIdentifier;  // same as NSLocaleIdentifier
- (NSString *)localizedStringForLocaleIdentifier:(NSString *)localeIdentifier API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *languageCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));
- (nullable NSString *)localizedStringForLanguageCode:(NSString *)languageCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (nullable, readonly, copy) NSString *countryCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));
- (nullable NSString *)localizedStringForCountryCode:(NSString *)countryCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (nullable, readonly, copy) NSString *scriptCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));
- (nullable NSString *)localizedStringForScriptCode:(NSString *)scriptCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (nullable, readonly, copy) NSString *variantCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));
- (nullable NSString *)localizedStringForVariantCode:(NSString *)variantCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSCharacterSet *exemplarCharacterSet API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *calendarIdentifier API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));
- (nullable NSString *)localizedStringForCalendarIdentifier:(NSString *)calendarIdentifier API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (nullable, readonly, copy) NSString *collationIdentifier API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));
- (nullable NSString *)localizedStringForCollationIdentifier:(NSString *)collationIdentifier API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly) BOOL usesMetricSystem API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *decimalSeparator API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *groupingSeparator API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *currencySymbol API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (nullable, readonly, copy) NSString *currencyCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));
- (nullable NSString *)localizedStringForCurrencyCode:(NSString *)currencyCode API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *collatorIdentifier API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));
- (nullable NSString *)localizedStringForCollatorIdentifier:(NSString *)collatorIdentifier API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *quotationBeginDelimiter API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *quotationEndDelimiter API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *alternateQuotationBeginDelimiter API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@property (readonly, copy) NSString *alternateQuotationEndDelimiter API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

@end

@interface NSLocale (NSLocaleCreation)

@property (class, readonly, strong) NSLocale *autoupdatingCurrentLocale API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)); // generally you should use this property

@property (class, readonly, copy) NSLocale *currentLocale;	// an object representing the user's current locale
@property (class, readonly, copy) NSLocale *systemLocale;	// the default generic root locale with little localization

+ (instancetype)localeWithLocaleIdentifier:(NSString *)ident API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (instancetype)init API_UNAVAILABLE(macos, ios, watchos, tvos);     /* do not invoke; not a valid initializer for this class */

@end

@interface NSLocale (NSLocaleGeneralInfo)

@property (class, readonly, copy) NSArray<NSString *> *availableLocaleIdentifiers;
@property (class, readonly, copy) NSArray<NSString *> *ISOLanguageCodes;
@property (class, readonly, copy) NSArray<NSString *> *ISOCountryCodes;
@property (class, readonly, copy) NSArray<NSString *> *ISOCurrencyCodes;
@property (class, readonly, copy) NSArray<NSString *> *commonISOCurrencyCodes API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@property (class, readonly, copy) NSArray<NSString *> *preferredLanguages API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)); // note that this list does not indicate what language the app is actually running in; the NSBundle.mainBundle object determines that at launch and knows that information

+ (NSDictionary<NSString *, NSString *> *)componentsFromLocaleIdentifier:(NSString *)string;
+ (NSString *)localeIdentifierFromComponents:(NSDictionary<NSString *, NSString *> *)dict;

+ (NSString *)canonicalLocaleIdentifierFromString:(NSString *)string;
+ (NSString *)canonicalLanguageIdentifierFromString:(NSString *)string;

+ (nullable NSString *)localeIdentifierFromWindowsLocaleCode:(uint32_t)lcid API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
+ (uint32_t)windowsLocaleCodeFromLocaleIdentifier:(NSString *)localeIdentifier API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

typedef NS_ENUM(NSUInteger, NSLocaleLanguageDirection) {
    NSLocaleLanguageDirectionUnknown = kCFLocaleLanguageDirectionUnknown,
    NSLocaleLanguageDirectionLeftToRight = kCFLocaleLanguageDirectionLeftToRight,
    NSLocaleLanguageDirectionRightToLeft = kCFLocaleLanguageDirectionRightToLeft,
    NSLocaleLanguageDirectionTopToBottom = kCFLocaleLanguageDirectionTopToBottom,
    NSLocaleLanguageDirectionBottomToTop = kCFLocaleLanguageDirectionBottomToTop
};

+ (NSLocaleLanguageDirection)characterDirectionForLanguage:(NSString *)isoLangCode API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
+ (NSLocaleLanguageDirection)lineDirectionForLanguage:(NSString *)isoLangCode API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end


FOUNDATION_EXPORT NSNotificationName const NSCurrentLocaleDidChangeNotification API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));


FOUNDATION_EXPORT NSLocaleKey const NSLocaleIdentifier;		// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleLanguageCode;	// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleCountryCode;		// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleScriptCode;		// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleVariantCode;		// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleExemplarCharacterSet;// NSCharacterSet
FOUNDATION_EXPORT NSLocaleKey const NSLocaleCalendar;		// NSCalendar
FOUNDATION_EXPORT NSLocaleKey const NSLocaleCollationIdentifier; // NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleUsesMetricSystem;	// NSNumber boolean
FOUNDATION_EXPORT NSLocaleKey const NSLocaleMeasurementSystem;	// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleDecimalSeparator;	// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleGroupingSeparator;	// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleCurrencySymbol;      // NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleCurrencyCode;	// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleCollatorIdentifier API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));  // NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleQuotationBeginDelimiterKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));	// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleQuotationEndDelimiterKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));	// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleAlternateQuotationBeginDelimiterKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));	// NSString
FOUNDATION_EXPORT NSLocaleKey const NSLocaleAlternateQuotationEndDelimiterKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));	// NSString


#if !defined(NS_CALENDAR_ENUM_DEPRECATED)
#if 1 || NS_ENABLE_CALENDAR_DEPRECATIONS
#define NS_CALENDAR_ENUM_DEPRECATED(A, B, C, D, ...) NS_ENUM_DEPRECATED(A, B, C, D, __VA_ARGS__)
#define NS_CALENDAR_DEPRECATED(A, B, C, D, ...) NS_DEPRECATED(A, B, C, D, __VA_ARGS__)
#define NS_CALENDAR_DEPRECATED_MAC(A, B, ...) NS_DEPRECATED_MAC(A, B, __VA_ARGS__)
#else
#define NS_CALENDAR_ENUM_DEPRECATED(A, B, C, D, ...) NS_ENUM_AVAILABLE(A, C)
#define NS_CALENDAR_DEPRECATED(A, B, C, D, ...) NS_AVAILABLE(A, C)
#define NS_CALENDAR_DEPRECATED_MAC(A, B, ...) NS_AVAILABLE_MAC(A)
#endif
#endif

// Values for NSCalendar identifiers (not the NSLocaleCalendar property key)
FOUNDATION_EXPORT NSString * const NSGregorianCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierGregorian", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSBuddhistCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierBuddhist", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSChineseCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierChinese", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSHebrewCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierHebrew", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSIslamicCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierIslamic", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSIslamicCivilCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierIslamicCivil", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSJapaneseCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierJapanese", macos(10.4, 10.10), ios(2.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSRepublicOfChinaCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierRepublicOfChina", macos(10.6, 10.10), ios(4.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSPersianCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierPersian", macos(10.6, 10.10), ios(4.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSIndianCalendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierIndian", macos(10.6, 10.10), ios(4.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
FOUNDATION_EXPORT NSString * const NSISO8601Calendar API_DEPRECATED_WITH_REPLACEMENT("NSCalendarIdentifierISO8601", macos(10.6, 10.10), ios(4.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));

NS_ASSUME_NONNULL_END
