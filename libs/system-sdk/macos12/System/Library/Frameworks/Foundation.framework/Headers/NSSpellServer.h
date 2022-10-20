/*	NSSpellServer.h
	Copyright (c) 1990-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSTextCheckingResult.h>

@class NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSOrthography;
@protocol NSSpellServerDelegate;

/*
The server just handles all the checking in and IAC and delegates the real work to its delegate.  A single server can handle more than one language.  Services is used to rendezvous applications with servers.
*/


NS_ASSUME_NONNULL_BEGIN

@interface NSSpellServer : NSObject {

  /*All instance variables are private*/

  @private
    // All instance variables are private and subject to future change.  Do not access them.
    id _delegate;
    NSInteger _caseSensitive;
    id _spellServerConnection;
    id _dictionaries;
    NSArray *_learnedDictionaries;
    
    struct __ssFlags {
        unsigned int delegateLearnsWords:1;
        unsigned int delegateForgetsWords:1;
        unsigned int busy:1;
        unsigned int _reserved:29;
    } _ssFlags;

    id _checker;
    void *_reservedSpellServer;
}

@property (nullable, assign) id<NSSpellServerDelegate> delegate;

/* Used to check in */
- (BOOL)registerLanguage:(nullable NSString *)language byVendor:(nullable NSString *)vendor;

/* Way to query user's private dictionary(ies). */
- (BOOL)isWordInUserDictionaries:(NSString *)word caseSensitive:(BOOL)flag;

/* Waits for spell-check request to come in. */
- (void)run;

@end

/*
These are the methods the object in the server that does the real work should respond to.  Only the first method is required.  The first method checks spelling and the second suggests proper corrections.  The third and fourth allow the delegate to be notified when new words are learned or forgotten.  

The argument wordCount is an out parameter.  It should return the number of words found in the document from startPosition until the misspelled word was found (or the end of the stream)  If your spell server can't count words (though it is hard to imagine why that might be), return -1 in wordCount.  countOnly of Yes means forget about checking the spelling of words, just count them until you get to the end of the stream.

The fifth method suggests potential completions, in the order in which they should be presented to the user.  The elements of the suggested completions array should be complete words that the user might be trying to type when starting by typing the partial word at the given range in the given string.  

The sixth method permits those delegates that support it to provide grammar checking.  The return value should be the range of the next sentence or other similar grammatical unit that contains sections to be flagged for grammatical issues; the details array optionally points out subranges of that range with specific issues.  The elements of the details array should be dictionaries with some or all of the keys listed below.  

The value for the NSGrammarRange key should be an NSValue containing an NSRange, a subrange of the sentence range used as the return value, whose location should be an offset from the beginning of the sentence--so, for example, an NSGrammarRange for the first four characters of the overall sentence range should be {0, 4}.  The value for the NSGrammarUserDescription key should be an NSString containing descriptive text about that range, to be presented directly to the user; it is intended that the user description should provide enough information to allow the user to correct the problem.  A value may also be provided for the NSGrammarCorrections key, consisting of an NSArray of NSStrings representing potential substitutions to correct the problem, but it is expected that this may not be available in all cases.  It is recommended that NSGrammarUserDescription be supplied in all cases; in any event, either NSGrammarUserDescription or NSGrammarCorrections must be supplied in order for something to be presented to the user.  If NSGrammarRange is not present, it will be assumed to be equal to the overall sentence range.  Additional keys may be added in future releases.

The seventh method is optional, but if implemented it will be called during the course of unified text checking via the NSSpellChecker checkString:... or requestCheckingOfString:... methods.  This allows spelling and grammar checking to be performed simultaneously, which can be significantly more efficient, and allows the delegate to return autocorrection results as well.  If this method is not implemented, then unified text checking will call the separate spelling and grammar checking methods described above instead.  

The result should be an array of NSTextCheckingResult objects, of the spelling, grammar, or correction types, depending on the checkingTypes requested.  This method may be called repeatedly with strings representing different subranges of the string that was originally requested to be checked; the offset argument represents the offset of the portion passed in to this method within that original string, and should be added to the origin of the range in any NSTextCheckingResult returned.  The options dictionary corresponds to the options supplied to the NSSpellChecker checkString:... or requestCheckingOfString:... methods, and the orthography argument represents the identified orthography of the text being passed in.
*/

@protocol NSSpellServerDelegate <NSObject>
@optional

- (NSRange)spellServer:(NSSpellServer *)sender findMisspelledWordInString:(NSString *)stringToCheck language:(NSString *)language wordCount:(NSInteger *)wordCount countOnly:(BOOL)countOnly;

- (nullable NSArray<NSString *> *)spellServer:(NSSpellServer *)sender suggestGuessesForWord:(NSString *)word inLanguage:(NSString *)language;

- (void)spellServer:(NSSpellServer *)sender didLearnWord:(NSString *)word inLanguage:(NSString *)language;

- (void)spellServer:(NSSpellServer *)sender didForgetWord:(NSString *)word inLanguage:(NSString *)language;

- (nullable NSArray<NSString *> *)spellServer:(NSSpellServer *)sender suggestCompletionsForPartialWordRange:(NSRange)range inString:(NSString *)string language:(NSString *)language;

- (NSRange)spellServer:(NSSpellServer *)sender checkGrammarInString:(NSString *)stringToCheck language:(nullable NSString *)language details:(NSArray<NSDictionary<NSString *, id> *> * _Nullable * _Nullable)details API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

/* Keys for the dictionaries in the details array. */
FOUNDATION_EXPORT NSString *const NSGrammarRange API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString *const NSGrammarUserDescription API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString *const NSGrammarCorrections API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

- (nullable NSArray<NSTextCheckingResult *> *)spellServer:(NSSpellServer *)sender checkString:(NSString *)stringToCheck offset:(NSUInteger)offset types:(NSTextCheckingTypes)checkingTypes options:(nullable NSDictionary<NSString *, id> *)options orthography:(nullable NSOrthography *)orthography wordCount:(NSInteger *)wordCount API_AVAILABLE(macos(10.6)) API_UNAVAILABLE(ios, watchos, tvos);

- (void)spellServer:(NSSpellServer *)sender recordResponse:(NSUInteger)response toCorrection:(NSString *)correction forWord:(NSString *)word language:(NSString *)language API_AVAILABLE(macos(10.7)) API_UNAVAILABLE(ios, watchos, tvos);

@end

NS_ASSUME_NONNULL_END

