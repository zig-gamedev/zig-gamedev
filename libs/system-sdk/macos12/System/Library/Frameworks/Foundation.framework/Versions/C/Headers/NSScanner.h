/*	NSScanner.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString, NSCharacterSet, NSDictionary;

NS_ASSUME_NONNULL_BEGIN

// Some of these API are deprecated in Swift only. They remain available in Objective-C.
#if defined(__swift__)
#define _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT(...) API_DEPRECATED_WITH_REPLACEMENT(__VA_ARGS__)
#else
#define _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT(...)
#endif

@interface NSScanner : NSObject <NSCopying>

@property (readonly, copy) NSString *string;
@property NSUInteger scanLocation _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("currentIndex", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));
@property (nullable, copy) NSCharacterSet *charactersToBeSkipped;
@property BOOL caseSensitive;
@property (nullable, retain) id locale;

- (instancetype)initWithString:(NSString *)string NS_DESIGNATED_INITIALIZER;

@end

@interface NSScanner (NSExtendedScanner)

// On overflow, the below methods will return success and clamp
- (BOOL)scanInt:(nullable int *)result _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanInt()", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanInteger:(nullable NSInteger *)result API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanInt()", macos(10.5,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanLongLong:(nullable long long *)result;

- (BOOL)scanUnsignedLongLong:(nullable unsigned long long *)result API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanUnsignedLongLong()", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanFloat:(nullable float *)result _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanFloat()", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanDouble:(nullable double *)result _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanDouble()", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanHexInt:(nullable unsigned *)result                                                                                 // Optionally prefixed with "0x" or "0X"
    _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanHexInt()", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanHexLongLong:(nullable unsigned long long *)result API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0))    // Optionally prefixed with "0x" or "0X"
    _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanHexLongLong()", macos(10.5,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanHexFloat:(nullable float *)result API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0))                    // Corresponding to %a or %A formatting. Requires "0x" or "0X" prefix.
    _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanHexFloat()", macos(10.5,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanHexDouble:(nullable double *)result API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0))                  // Corresponding to %a or %A formatting. Requires "0x" or "0X" prefix.
    _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanHexDouble()", macos(10.5,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanString:(NSString *)string intoString:(NSString * _Nullable * _Nullable)result _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanString(_:)", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanCharactersFromSet:(NSCharacterSet *)set intoString:(NSString * _Nullable * _Nullable)result _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanCharacters(from:)", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanUpToString:(NSString *)string intoString:(NSString * _Nullable * _Nullable)result _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanUpTo(_:)", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

- (BOOL)scanUpToCharactersFromSet:(NSCharacterSet *)set intoString:(NSString * _Nullable * _Nullable)result _NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT("scanUpToCharacters(from:)", macos(10.0,10.15), ios(2.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

@property (getter=isAtEnd, readonly) BOOL atEnd;

+ (instancetype)scannerWithString:(NSString *)string;
+ (id)localizedScannerWithString:(NSString *)string;

@end

NS_ASSUME_NONNULL_END
