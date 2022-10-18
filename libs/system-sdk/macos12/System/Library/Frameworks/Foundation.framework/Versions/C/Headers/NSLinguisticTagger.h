/*	NSLinguisticTagger.h
	Copyright (c) 2009-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>

@class NSArray<ObjectType>, NSOrthography, NSValue;

NS_ASSUME_NONNULL_BEGIN

/* NSLinguisticTagger is a class used to automatically segment natural-language text and tag the tokens with information such as language, script, lemma, and part of speech.  An instance of this class is assigned a string to tag, and clients can then obtain tags and ranges for tokens in that string appropriate to a given tag scheme and unit.
*/

/* Tag schemes */
typedef NSString *NSLinguisticTagScheme NS_TYPED_EXTENSIBLE_ENUM;

FOUNDATION_EXPORT NSLinguisticTagScheme const NSLinguisticTagSchemeTokenType API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));              /* This tag scheme classifies tokens according to their broad general type:  word, punctuation, whitespace, etc. */
FOUNDATION_EXPORT NSLinguisticTagScheme const NSLinguisticTagSchemeLexicalClass API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));              /* This tag scheme classifies tokens according to class:  part of speech for words, type of punctuation or whitespace, etc. */
FOUNDATION_EXPORT NSLinguisticTagScheme const NSLinguisticTagSchemeNameType API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));              /* This tag scheme classifies tokens as to whether they are part of named entities of various types or not. */
FOUNDATION_EXPORT NSLinguisticTagScheme const NSLinguisticTagSchemeNameTypeOrLexicalClass API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));  /* This tag scheme follows NSLinguisticTagSchemeNameType for names, NSLinguisticTagSchemeLexicalClass for all other tokens. */
FOUNDATION_EXPORT NSLinguisticTagScheme const NSLinguisticTagSchemeLemma API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));              /* This tag scheme supplies a stem form for each word token (if known). */
FOUNDATION_EXPORT NSLinguisticTagScheme const NSLinguisticTagSchemeLanguage API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));              /* This tag scheme tags tokens according to their most likely language (if known). */
FOUNDATION_EXPORT NSLinguisticTagScheme const NSLinguisticTagSchemeScript API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));              /* This tag scheme tags tokens according to their script. */

typedef NSString *NSLinguisticTag NS_TYPED_EXTENSIBLE_ENUM;

/* Tags for NSLinguisticTagSchemeTokenType */
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagWord API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));                                     /* Tokens considered to be words or word-like linguistic items. */
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagPunctuation API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));              /* Tokens made up of punctuation. */
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagWhitespace API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));              /* Tokens made up of whitespace of all sorts. */
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagOther API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));                                     /* Other tokens, including non-linguistic items such as symbols. */

/* Tags for NSLinguisticTagSchemeLexicalClass */
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagNoun API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagVerb API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagAdjective API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagAdverb API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagPronoun API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagDeterminer API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagParticle API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagPreposition API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagNumber API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagConjunction API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagInterjection API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagClassifier API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagIdiom API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagOtherWord API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagSentenceTerminator API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagOpenQuote API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagCloseQuote API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagOpenParenthesis API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagCloseParenthesis API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagWordJoiner API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagDash API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagOtherPunctuation API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagParagraphBreak API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagOtherWhitespace API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* Tags for NSLinguisticTagSchemeNameType */
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagPersonalName API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagPlaceName API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
FOUNDATION_EXPORT NSLinguisticTag const NSLinguisticTagOrganizationName API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* For NSLinguisticTagSchemeTokenType, NSLinguisticTagSchemeLexicalClass, NSLinguisticTagSchemeNameType, and NSLinguisticTagSchemeNameTypeOrLexicalClass, tags will be taken from the lists above (clients may use == comparison).  Tags for NSLinguisticTagSchemeLemma are lemmas from the language.  Tags for NSLinguisticTagSchemeLanguage are standard language abbreviations.  Tags for NSLinguisticTagSchemeScript are standard script abbreviations.
*/

/* NSLinguisticTaggerUnit specifes the size of units in a string to which tagging applies.  The tagging unit may be word, sentence, paragraph, or document.  Methods that do not specify a unit act at the word level.  Not all combinations of scheme and unit are supported; clients can use +availableTagSchemesForUnit:language: to determine which ones are.
*/
typedef NS_ENUM(NSInteger, NSLinguisticTaggerUnit) {
    NSLinguisticTaggerUnitWord,         /* Token units are at word or equivalent level */
    NSLinguisticTaggerUnitSentence,     /* Token units are at sentence level */
    NSLinguisticTaggerUnitParagraph,    /* Token units are at paragraph level */
    NSLinguisticTaggerUnitDocument      /* Token unit is the entire string */
};

/* Options arguments of type NSLinguisticTaggerOptions may include the following flags, which allow clients interested only in certain general types of tokens to specify that tokens of other types should be omitted from the returned results. */
typedef NS_OPTIONS(NSUInteger, NSLinguisticTaggerOptions) {   /* Any combination of options from the enumeration. */
    NSLinguisticTaggerOmitWords         = 1 << 0,       /* Omit tokens of type NSLinguisticTagWord. */
    NSLinguisticTaggerOmitPunctuation   = 1 << 1,       /* Omit tokens of type NSLinguisticTagPunctuation. */
    NSLinguisticTaggerOmitWhitespace    = 1 << 2,       /* Omit tokens of type NSLinguisticTagWhitespace. */
    NSLinguisticTaggerOmitOther         = 1 << 3,       /* Omit tokens of type NSLinguisticTagOther. */
    NSLinguisticTaggerJoinNames         = 1 << 4        /* Join tokens of tag scheme NSLinguisticTagSchemeNameType. */
};


API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED))
@interface NSLinguisticTagger : NSObject {
@private
    NSArray *_schemes;
    NSUInteger _options;
    NSString *_string;
    id _orthographyArray;
    id _tokenArray;
    void *_reserved;
}

/* An instance of NSLinguisticTagger is created with an array of tag schemes.  The tagger will be able to supply tags corresponding to any of the schemes in this array.
*/
- (instancetype)initWithTagSchemes:(NSArray<NSLinguisticTagScheme> *)tagSchemes options:(NSUInteger)opts NS_DESIGNATED_INITIALIZER API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

@property (readonly, copy) NSArray<NSLinguisticTagScheme> *tagSchemes API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
@property (nullable, retain) NSString *string API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* Clients wishing to know the tag schemes supported in NSLinguisticTagger for a particular unit and language may query them with this method.  The language should be specified using a standard BCP-47 language tag as with NSOrthography.
*/
+ (NSArray<NSLinguisticTagScheme> *)availableTagSchemesForUnit:(NSLinguisticTaggerUnit)unit language:(NSString *)language API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));

/* Clients wishing to know the tag schemes supported in NSLinguisticTagger for a particular language at the word level may query them with this method.  The language should be specified using a standard abbreviation as with NSOrthography.
*/
+ (NSArray<NSLinguisticTagScheme> *)availableTagSchemesForLanguage:(NSString *)language API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* If clients know the orthography for a given portion of the string, they may supply it to the tagger.  Otherwise, the tagger will infer the language from the contents of the text.  In each case, the charIndex or range passed in must not extend beyond the end of the tagger's string, or the methods will raise an exception.
*/
- (void)setOrthography:(nullable NSOrthography *)orthography range:(NSRange)range API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (nullable NSOrthography *)orthographyAtIndex:(NSUInteger)charIndex effectiveRange:(nullable NSRangePointer)effectiveRange API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* If the string attached to the tagger is mutable, this method must be called to inform the tagger whenever the string changes.  The newRange is the range in the final string which was explicitly edited, and delta is the change in length from the previous version to the current version of the string.  Alternatively, the client may call setString: again to reset all information about the string, but this has the disadvantage of not preserving information about portions of the string that have not changed.
*/
- (void)stringEditedInRange:(NSRange)newRange changeInLength:(NSInteger)delta API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* Returns the range corresponding to the token for the given unit that contains the given character index.
*/
- (NSRange)tokenRangeAtIndex:(NSUInteger)charIndex unit:(NSLinguisticTaggerUnit)unit API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));

/* Returns a range covering all sentences intersecting the given range.
*/
- (NSRange)sentenceRangeForRange:(NSRange)range API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* The tagger will segment the string as needed into tokens for the given unit, and return those ranges along with a tag for any scheme in its array of tag schemes.  The fundamental tagging method on NSLinguisticTagger is a block iterator, that iterates over all tokens intersecting a given range, supplying tags and ranges.  There are several additional convenience methods, for obtaining a sentence range, information about a single token, or for obtaining information about all tokens intersecting a given range at once, in arrays.  In each case, the charIndex or range passed in must not extend beyond the end of the tagger's string, or the methods will raise an exception.  Note that a given instance of NSLinguisticTagger should not be used from more than one thread simultaneously.
*/
- (void)enumerateTagsInRange:(NSRange)range unit:(NSLinguisticTaggerUnit)unit scheme:(NSLinguisticTagScheme)scheme options:(NSLinguisticTaggerOptions)options usingBlock:(void (NS_NOESCAPE ^)(NSLinguisticTag _Nullable tag, NSRange tokenRange, BOOL *stop))block API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));
- (nullable NSLinguisticTag)tagAtIndex:(NSUInteger)charIndex unit:(NSLinguisticTaggerUnit)unit scheme:(NSLinguisticTagScheme)scheme tokenRange:(nullable NSRangePointer)tokenRange API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));
- (NSArray<NSLinguisticTag> *)tagsInRange:(NSRange)range unit:(NSLinguisticTaggerUnit)unit scheme:(NSLinguisticTagScheme)scheme options:(NSLinguisticTaggerOptions)options tokenRanges:(NSArray<NSValue *> * _Nullable * _Nullable)tokenRanges API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));

/* Methods that do not specify a unit act at the word level.
*/
- (void)enumerateTagsInRange:(NSRange)range scheme:(NSLinguisticTagScheme)tagScheme options:(NSLinguisticTaggerOptions)opts usingBlock:(void (NS_NOESCAPE ^)(NSLinguisticTag _Nullable tag, NSRange tokenRange, NSRange sentenceRange, BOOL *stop))block API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (nullable NSLinguisticTag)tagAtIndex:(NSUInteger)charIndex scheme:(NSLinguisticTagScheme)scheme tokenRange:(nullable NSRangePointer)tokenRange sentenceRange:(nullable NSRangePointer)sentenceRange API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (NSArray<NSString *> *)tagsInRange:(NSRange)range scheme:(NSString *)tagScheme options:(NSLinguisticTaggerOptions)opts tokenRanges:(NSArray<NSValue *> * _Nullable * _Nullable)tokenRanges API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* Returns the top identified language (if any) for the entire string.  Convenience for tagAtIndex: with NSLinguisticTagSchemeLanguage and NSLinguisticTaggerUnitDocument.
*/
@property (nullable, readonly, copy) NSString *dominantLanguage API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));

/* The following class methods are conveniences for clients who wish to perform a single analysis on a string without having to create an instance of NSLinguisticTagger.  If more than one tagging operation is needed on a given string, it is more efficient to use an explicit NSLinguisticTagger instance.
*/
+ (nullable NSString *)dominantLanguageForString:(NSString *)string API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));
+ (nullable NSLinguisticTag)tagForString:(NSString *)string atIndex:(NSUInteger)charIndex unit:(NSLinguisticTaggerUnit)unit scheme:(NSLinguisticTagScheme)scheme orthography:(nullable NSOrthography *)orthography tokenRange:(nullable NSRangePointer)tokenRange API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));
+ (NSArray<NSLinguisticTag> *)tagsForString:(NSString *)string range:(NSRange)range unit:(NSLinguisticTaggerUnit)unit scheme:(NSLinguisticTagScheme)scheme options:(NSLinguisticTaggerOptions)options orthography:(nullable NSOrthography *)orthography tokenRanges:(NSArray<NSValue *> * _Nullable * _Nullable)tokenRanges API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));
+ (void)enumerateTagsForString:(NSString *)string range:(NSRange)range unit:(NSLinguisticTaggerUnit)unit scheme:(NSLinguisticTagScheme)scheme options:(NSLinguisticTaggerOptions)options orthography:(nullable NSOrthography *)orthography usingBlock:(void (NS_NOESCAPE ^)(NSLinguisticTag _Nullable tag, NSRange tokenRange, BOOL *stop))block API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), watchos(4.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED));

/* Deprecated method for obtaining a list of possible tags for the token at a given index.
*/
- (nullable NSArray<NSString *> *)possibleTagsAtIndex:(NSUInteger)charIndex scheme:(NSString *)tagScheme tokenRange:(nullable NSRangePointer)tokenRange sentenceRange:(nullable NSRangePointer)sentenceRange scores:(NSArray<NSValue *> * _Nullable * _Nullable)scores API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

@end

@interface NSString (NSLinguisticAnalysis)

/* Clients wishing to analyze a given string once may use these NSString APIs without having to create an instance of NSLinguisticTagger.  If more than one tagging operation is needed on a given string, it is more efficient to use an explicit NSLinguisticTagger instance.
*/
- (NSArray<NSLinguisticTag> *)linguisticTagsInRange:(NSRange)range scheme:(NSLinguisticTagScheme)scheme options:(NSLinguisticTaggerOptions)options orthography:(nullable NSOrthography *)orthography tokenRanges:(NSArray<NSValue *> * _Nullable * _Nullable)tokenRanges API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (void)enumerateLinguisticTagsInRange:(NSRange)range scheme:(NSLinguisticTagScheme)scheme options:(NSLinguisticTaggerOptions)options orthography:(nullable NSOrthography *)orthography usingBlock:(void (NS_NOESCAPE ^)(NSLinguisticTag _Nullable tag, NSRange tokenRange, NSRange sentenceRange, BOOL *stop))block API_DEPRECATED("All NSLinguisticTagger API should be replaced with NaturalLanguage.framework API", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

@end

NS_ASSUME_NONNULL_END
