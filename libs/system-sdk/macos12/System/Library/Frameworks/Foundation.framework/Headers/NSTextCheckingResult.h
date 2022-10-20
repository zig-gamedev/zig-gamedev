/*	NSTextCheckingResult.h
	Copyright (c) 2008-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSDate.h>

@class NSString, NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSDate, NSTimeZone, NSOrthography, NSURL, NSRegularExpression;

NS_ASSUME_NONNULL_BEGIN

/* NSTextCheckingResult is a class used to describe items located by text checking.  Each of these objects represents something that has been found during checking--a misspelled word, a sentence with grammatical issues, a detected URL, a straight quote to be replaced with curly quotes, and so forth. */

typedef NS_OPTIONS(uint64_t, NSTextCheckingType) {    // a single type
    NSTextCheckingTypeOrthography           = 1ULL << 0,            // language identification
    NSTextCheckingTypeSpelling              = 1ULL << 1,            // spell checking
    NSTextCheckingTypeGrammar               = 1ULL << 2,            // grammar checking
    NSTextCheckingTypeDate                  = 1ULL << 3,            // date/time detection
    NSTextCheckingTypeAddress               = 1ULL << 4,            // address detection
    NSTextCheckingTypeLink                  = 1ULL << 5,            // link detection
    NSTextCheckingTypeQuote                 = 1ULL << 6,            // smart quotes
    NSTextCheckingTypeDash                  = 1ULL << 7,            // smart dashes
    NSTextCheckingTypeReplacement           = 1ULL << 8,            // fixed replacements, such as copyright symbol for (c)
    NSTextCheckingTypeCorrection            = 1ULL << 9,            // autocorrection
    NSTextCheckingTypeRegularExpression API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0))  = 1ULL << 10,           // regular expression matches
    NSTextCheckingTypePhoneNumber API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0))        = 1ULL << 11,           // phone number detection
    NSTextCheckingTypeTransitInformation API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0)) = 1ULL << 12            // transit (e.g. flight) info detection
};

typedef uint64_t NSTextCheckingTypes;   // a combination of types
NS_ENUM(NSTextCheckingTypes) {
    NSTextCheckingAllSystemTypes    = 0xffffffffULL,        // the first 32 types are reserved
    NSTextCheckingAllCustomTypes    = 0xffffffffULL << 32,  // clients may use the remainder for their own purposes
    NSTextCheckingAllTypes          = (NSTextCheckingAllSystemTypes | NSTextCheckingAllCustomTypes)
};

typedef NSString *NSTextCheckingKey NS_TYPED_EXTENSIBLE_ENUM;

API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0))
@interface NSTextCheckingResult : NSObject <NSCopying, NSSecureCoding>

/* Mandatory properties, used with all types of results. */
@property (readonly) NSTextCheckingType resultType;
@property (readonly) NSRange range;

@end

@interface NSTextCheckingResult (NSTextCheckingResultOptional)

/* Optional properties, used with certain types of results. */
@property (nullable, readonly, copy) NSOrthography *orthography;
@property (nullable, readonly, copy) NSArray<NSDictionary<NSString *, id> *> *grammarDetails;
@property (nullable, readonly, copy) NSDate *date;
@property (nullable, readonly, copy) NSTimeZone *timeZone;
@property (readonly) NSTimeInterval duration;
@property (nullable, readonly, copy) NSDictionary<NSTextCheckingKey, NSString *> *components API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSURL *URL;
@property (nullable, readonly, copy) NSString *replacementString;
@property (nullable, readonly, copy) NSArray<NSString *> *alternativeStrings API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSRegularExpression *regularExpression API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSString *phoneNumber API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));


/* A result must have at least one range, but may optionally have more (for example, to represent regular expression capture groups).  The range at index 0 always matches the range property.  Additional ranges, if any, will have indexes from 1 to numberOfRanges-1. rangeWithName: can be used with named regular expression capture groups. */
@property (readonly) NSUInteger numberOfRanges API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));
- (NSRange)rangeAtIndex:(NSUInteger)idx API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));
- (NSRange)rangeWithName:(NSString *)name API_AVAILABLE(macosx(10.13), ios(11.0), watchos(4.0), tvos(11.0));
- (NSTextCheckingResult *)resultByAdjustingRangesWithOffset:(NSInteger)offset API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

@property (nullable, readonly, copy) NSDictionary<NSTextCheckingKey, NSString *> *addressComponents;    // Deprecated in favor of components

@end


/* Keys for address components. */
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingNameKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingJobTitleKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingOrganizationKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingStreetKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingCityKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingStateKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingZIPKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingCountryKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingPhoneKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingAirlineKey API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSTextCheckingKey const NSTextCheckingFlightKey API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));


@interface NSTextCheckingResult (NSTextCheckingResultCreation)

/* Methods for creating instances of the various types of results. */
+ (NSTextCheckingResult *)orthographyCheckingResultWithRange:(NSRange)range orthography:(NSOrthography *)orthography;
+ (NSTextCheckingResult *)spellCheckingResultWithRange:(NSRange)range;
+ (NSTextCheckingResult *)grammarCheckingResultWithRange:(NSRange)range details:(NSArray<NSDictionary<NSString *, id> *> *)details;
+ (NSTextCheckingResult *)dateCheckingResultWithRange:(NSRange)range date:(NSDate *)date;
+ (NSTextCheckingResult *)dateCheckingResultWithRange:(NSRange)range date:(NSDate *)date timeZone:(NSTimeZone *)timeZone duration:(NSTimeInterval)duration;
+ (NSTextCheckingResult *)addressCheckingResultWithRange:(NSRange)range components:(NSDictionary<NSTextCheckingKey, NSString *> *)components;
+ (NSTextCheckingResult *)linkCheckingResultWithRange:(NSRange)range URL:(NSURL *)url;
+ (NSTextCheckingResult *)quoteCheckingResultWithRange:(NSRange)range replacementString:(NSString *)replacementString;
+ (NSTextCheckingResult *)dashCheckingResultWithRange:(NSRange)range replacementString:(NSString *)replacementString;
+ (NSTextCheckingResult *)replacementCheckingResultWithRange:(NSRange)range replacementString:(NSString *)replacementString;
+ (NSTextCheckingResult *)correctionCheckingResultWithRange:(NSRange)range replacementString:(NSString *)replacementString;
+ (NSTextCheckingResult *)correctionCheckingResultWithRange:(NSRange)range replacementString:(NSString *)replacementString alternativeStrings:(NSArray<NSString *> *)alternativeStrings     API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));
+ (NSTextCheckingResult *)regularExpressionCheckingResultWithRanges:(NSRangePointer)ranges count:(NSUInteger)count regularExpression:(NSRegularExpression *)regularExpression   API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));
+ (NSTextCheckingResult *)phoneNumberCheckingResultWithRange:(NSRange)range phoneNumber:(NSString *)phoneNumber             API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));
+ (NSTextCheckingResult *)transitInformationCheckingResultWithRange:(NSRange)range components:(NSDictionary<NSTextCheckingKey, NSString *> *)components    API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));

@end

NS_ASSUME_NONNULL_END

