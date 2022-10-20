/*    NSMorphology.h
      Copyright (c) 2021, Apple Inc. All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSError.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSGrammaticalGender) {
    NSGrammaticalGenderNotSet = 0,
    NSGrammaticalGenderFeminine,
    NSGrammaticalGenderMasculine,
    NSGrammaticalGenderNeuter,
} API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

typedef NS_ENUM(NSInteger, NSGrammaticalPartOfSpeech) {
    NSGrammaticalPartOfSpeechNotSet = 0,
    NSGrammaticalPartOfSpeechDeterminer,
    NSGrammaticalPartOfSpeechPronoun,
    NSGrammaticalPartOfSpeechLetter,
    NSGrammaticalPartOfSpeechAdverb,
    NSGrammaticalPartOfSpeechParticle,
    NSGrammaticalPartOfSpeechAdjective,
    NSGrammaticalPartOfSpeechAdposition,
    NSGrammaticalPartOfSpeechVerb,
    NSGrammaticalPartOfSpeechNoun,
    NSGrammaticalPartOfSpeechConjunction,
    NSGrammaticalPartOfSpeechNumeral,
    NSGrammaticalPartOfSpeechInterjection,
    NSGrammaticalPartOfSpeechPreposition,
    NSGrammaticalPartOfSpeechAbbreviation,
} API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

typedef NS_ENUM(NSInteger, NSGrammaticalNumber) {
    NSGrammaticalNumberNotSet = 0,
    NSGrammaticalNumberSingular,
    NSGrammaticalNumberZero,
    NSGrammaticalNumberPlural,
    NSGrammaticalNumberPluralTwo,
    NSGrammaticalNumberPluralFew,
    NSGrammaticalNumberPluralMany,
} API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));


API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
NS_REFINED_FOR_SWIFT
@interface NSMorphology: NSObject <NSCopying, NSSecureCoding>

@property (nonatomic) NSGrammaticalGender grammaticalGender;
@property (nonatomic) NSGrammaticalPartOfSpeech partOfSpeech;
@property (nonatomic) NSGrammaticalNumber number;

@end


// Per-language attribute support:

@class NSMorphologyCustomPronoun;

API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
@interface NSMorphology (NSCustomPronouns)

- (nullable NSMorphologyCustomPronoun *)customPronounForLanguage:(NSString *)language;
- (BOOL)setCustomPronoun:(nullable NSMorphologyCustomPronoun *)features forLanguage:(NSString *)language error:(NSError **)error;

@end

API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
NS_REFINED_FOR_SWIFT
@interface NSMorphologyCustomPronoun: NSObject <NSCopying, NSSecureCoding>

+ (BOOL)isSupportedForLanguage:(NSString *)language;
+ (NSArray<NSString *> *)requiredKeysForLanguage:(NSString *)language;

@property(nullable, copy, nonatomic) NSString *subjectForm;
@property(nullable, copy, nonatomic) NSString *objectForm;
@property(nullable, copy, nonatomic) NSString *possessiveForm;
@property(nullable, copy, nonatomic) NSString *possessiveAdjectiveForm;
@property(nullable, copy, nonatomic) NSString *reflexiveForm;

@end


// User settings access:

@interface NSMorphology (NSMorphologyUserSettings)

// Equivalent to the above.
@property (readonly, getter=isUnspecified) BOOL unspecified
 API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

@property (class, readonly) NSMorphology *userMorphology
 API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

@end

NS_ASSUME_NONNULL_END
