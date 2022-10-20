/*	NSPersonNameComponentsFormatter.h
	Copyright (c) 2015-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSPersonNameComponents.h>
#import <Foundation/NSFormatter.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSPersonNameComponentsFormatterStyle) {
    NSPersonNameComponentsFormatterStyleDefault = 0,

    /* Relies on user preferences and language defaults to display shortened form appropriate
      for display in space-constrained settings, e.g. C Darwin */
    NSPersonNameComponentsFormatterStyleShort,

    /* The minimally necessary features for differentiation in a casual setting , e.g. Charles Darwin */
    NSPersonNameComponentsFormatterStyleMedium,

    /* The fully-qualified name complete with all known components, e.g. Charles Robert Darwin, FRS */
    NSPersonNameComponentsFormatterStyleLong,

    /* The maximally-abbreviated form of a name suitable for monograms, e.g. CRD) */
    NSPersonNameComponentsFormatterStyleAbbreviated
} API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

typedef NS_OPTIONS (NSUInteger, NSPersonNameComponentsFormatterOptions) {
    /* Indicates that the formatter should format the component object's phoneticRepresentation components instead of its own components.
     The developer must have populated these manually. e.g.: Developer creates components object with the following properties:
     <family:"Family", given:"Given", phoneticRepresentation:<family:"FamilyPhonetic", given:"GivenPhonetic">>.
     If this option is specified, we perform formatting operations on <family "FamilyPhonetic", given "GivenPhonetic"> instead. */
    NSPersonNameComponentsFormatterPhonetic = (1UL << 1)
} API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));


API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0))
@interface NSPersonNameComponentsFormatter : NSFormatter {
@private
    id _private;
}

/* Specify the formatting style for the formatted string on an instance. ShortStyle will fall back to user preferences and language-specific defaults
 */
@property NSPersonNameComponentsFormatterStyle style;

/* Specify that the formatter should only format the components object's phoneticRepresentation
 */
@property (getter=isPhonetic) BOOL phonetic;


/* Specifies the locale to format names. Defaults to autoupdatingCurrentLocale. Also resets to autoupdatingCurrentLocale on assignment of nil.
 */
@property (null_resettable, copy) NSLocale *locale API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

/* Shortcut for converting an NSPersonNameComponents object into a string without explicitly creating an instance.
    Create an instance for greater customizability.
 */
+ (NSString *)localizedStringFromPersonNameComponents:(NSPersonNameComponents *)components
                                                style:(NSPersonNameComponentsFormatterStyle)nameFormatStyle
                                              options:(NSPersonNameComponentsFormatterOptions)nameOptions;

/* Convenience method on stringForObjectValue:. Returns a string containing the formatted value of the provided components object.
 */
- (NSString *)stringFromPersonNameComponents:(NSPersonNameComponents *)components;

/* Returns attributed string with annotations for each component. For each range, attributes can be obtained by querying
    dictionary key NSPersonNameComponentKey , using NSPersonNameComponent constant values.
 */
- (NSAttributedString *)annotatedStringFromPersonNameComponents:(NSPersonNameComponents *)components;


/* Convenience method on getObjectValue:forString:error:. Returns an NSPersonNameComponents object representing the name components found in the provided string.
 */
- (nullable NSPersonNameComponents *)personNameComponentsFromString:(nonnull NSString *)string API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

- (BOOL)getObjectValue:(out id _Nullable * _Nullable)obj forString:(NSString *)string errorDescription:(out NSString * _Nullable * _Nullable)error;

@end

// Attributed String identifier key string
FOUNDATION_EXPORT NSString * const NSPersonNameComponentKey API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

// Constants for attributed strings
FOUNDATION_EXPORT NSString * const NSPersonNameComponentGivenName API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSString * const NSPersonNameComponentFamilyName API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSString * const NSPersonNameComponentMiddleName API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSString * const NSPersonNameComponentPrefix API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSString * const NSPersonNameComponentSuffix API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSString * const NSPersonNameComponentNickname API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

/* The delimiter is the character or characters used to separate name components.
 For CJK languages there is no delimiter.
 */
FOUNDATION_EXPORT NSString * const NSPersonNameComponentDelimiter API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

NS_ASSUME_NONNULL_END
