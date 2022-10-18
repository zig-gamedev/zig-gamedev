/*    NSMorphology.h
      Copyright (c) 2021, Apple Inc. All rights reserved.
 */

#import <Foundation/NSObject.h>

@class NSMorphology;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
NS_REFINED_FOR_SWIFT
@interface NSInflectionRule: NSObject <NSCopying, NSSecureCoding>

- (id)init NS_UNAVAILABLE; // This class is abstract. Use +automaticRule, or instantiate a subclass instead.

@property (class, readonly) NSInflectionRule *automaticRule;

@end

API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
NS_REFINED_FOR_SWIFT
@interface NSInflectionRuleExplicit: NSInflectionRule

- (instancetype)initWithMorphology:(NSMorphology *)morphology NS_DESIGNATED_INITIALIZER;
@property (readonly, copy) NSMorphology *morphology;

@end

// -----

@interface NSInflectionRule (NSInflectionAvailability)

// Whether inflection will work in the specified language code.
+ (BOOL)canInflectLanguage:(NSString *)language
 API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

// Whether inflection will work in the language of the main bundle's first preferred localization.
@property (class, readonly) BOOL canInflectPreferredLocalization
 API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));
 
@end

NS_ASSUME_NONNULL_END
