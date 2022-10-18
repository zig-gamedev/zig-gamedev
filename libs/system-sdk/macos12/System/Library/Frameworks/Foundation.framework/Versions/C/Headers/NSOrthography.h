/*	NSOrthography.h
	Copyright (c) 2008-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString, NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>;

NS_ASSUME_NONNULL_BEGIN

/* NSOrthography is a class used to describe the linguistic content of a piece of text, especially for the purposes of spelling and grammar checking.  It describes (a) which scripts the text contains, (b) a dominant language and possibly other languages for each of these scripts, and (c) a dominant script and language for the text as a whole.  Scripts are uniformly described by standard four-letter tags (Latn, Grek, Cyrl, etc.) with the supertags Jpan and Kore typically used for Japanese and Korean text, Hans and Hant for Chinese text; the tag Zyyy is used if a specific script cannot be identified.  Languages are uniformly described by BCP-47 tags, preferably in canonical form; the tag und is used if a specific language cannot be determined. */

API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0))
@interface NSOrthography : NSObject <NSCopying, NSSecureCoding>

/* These are the primitive properties which a subclass must implement.  The dominantScript should be a script tag (such as Latn, Cyrl, and so forth) and the languageMap should be a dictionary whose keys are script tags and whose values are arrays of language tags (such as en, fr, de, and so forth). */
@property (readonly, copy) NSString *dominantScript;
@property (readonly, copy) NSDictionary<NSString *, NSArray<NSString *> *> *languageMap;
- (instancetype)initWithDominantScript:(NSString *)script languageMap:(NSDictionary<NSString *, NSArray<NSString *> *> *)map API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end

@interface NSOrthography (NSOrthographyExtended)

/* languagesForScript: returns the list of languages for the specified script, and dominantLanguageForScript: returns the first item on that list. */
- (nullable NSArray<NSString *> *)languagesForScript:(NSString *)script API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (nullable NSString *)dominantLanguageForScript:(NSString *)script API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/*  The dominantLanguage is the first in the list of languages for the dominant script, allScripts includes the dominant script and all others appearing as keys in the language map, and allLanguages includes all languages appearing in the values of the language map. */
@property (readonly, copy) NSString *dominantLanguage API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (readonly, copy) NSArray<NSString *> *allScripts API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (readonly, copy) NSArray<NSString *> *allLanguages API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

+ (instancetype)defaultOrthographyForLanguage:(NSString *)language API_AVAILABLE(macosx(10.13), ios(11.0), watchos(4.0), tvos(11.0));

@end

@interface NSOrthography (NSOrthographyCreation)

+ (instancetype)orthographyWithDominantScript:(NSString *)script languageMap:(NSDictionary<NSString *, NSArray<NSString *> *> *)map API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

NS_ASSUME_NONNULL_END
