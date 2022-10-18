/*
	NSSpeechSynthesizer.h
	Application Kit
	Copyright (c) 2003-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSRange.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString, NSURL, NSError;
@protocol NSSpeechSynthesizerDelegate;

typedef NSString * NSSpeechSynthesizerVoiceName NS_TYPED_ENUM;

typedef NSString * NSVoiceAttributeKey NS_TYPED_ENUM;
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceName;
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceIdentifier;
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceAge;
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceGender;
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceDemoText;
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceLocaleIdentifier API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceSupportedCharacters API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceIndividuallySpokenCharacters API_AVAILABLE(macos(10.5));

// Use with addSpeechDictionary:
typedef NSString * NSSpeechDictionaryKey NS_TYPED_ENUM;
APPKIT_EXTERN NSSpeechDictionaryKey const NSSpeechDictionaryLocaleIdentifier API_AVAILABLE(macos(10.5));  // NSString
APPKIT_EXTERN NSSpeechDictionaryKey const NSSpeechDictionaryModificationDate API_AVAILABLE(macos(10.5));  // NSString
APPKIT_EXTERN NSSpeechDictionaryKey const NSSpeechDictionaryPronunciations API_AVAILABLE(macos(10.5));  // NSArray
APPKIT_EXTERN NSSpeechDictionaryKey const NSSpeechDictionaryAbbreviations API_AVAILABLE(macos(10.5));  // NSArray
APPKIT_EXTERN NSSpeechDictionaryKey const NSSpeechDictionaryEntrySpelling API_AVAILABLE(macos(10.5));  // NSString
APPKIT_EXTERN NSSpeechDictionaryKey const NSSpeechDictionaryEntryPhonemes API_AVAILABLE(macos(10.5));  // NSString

// Values for NSVoiceGender voice attribute
typedef NSString * NSVoiceGenderName NS_TYPED_ENUM;
APPKIT_EXTERN NSVoiceGenderName const NSVoiceGenderNeuter;
APPKIT_EXTERN NSVoiceGenderName const NSVoiceGenderMale;
APPKIT_EXTERN NSVoiceGenderName const NSVoiceGenderFemale;

// Synthesizer Properties (including object type)
typedef NSString * NSSpeechPropertyKey NS_TYPED_ENUM;
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechStatusProperty API_AVAILABLE(macos(10.5));  // NSDictionary, see keys below
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechErrorsProperty API_AVAILABLE(macos(10.5));  // NSDictionary, see keys below
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechInputModeProperty API_AVAILABLE(macos(10.5));  // NSString: NSSpeechModeTextProperty or NSSpeechModePhonemeProperty
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechCharacterModeProperty API_AVAILABLE(macos(10.5)); // NSString: NSSpeechModeNormalProperty or NSSpeechModeLiteralProperty
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechNumberModeProperty API_AVAILABLE(macos(10.5)); // NSString: NSSpeechModeNormalProperty or NSSpeechModeLiteralProperty
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechRateProperty API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechPitchBaseProperty API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechPitchModProperty API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechVolumeProperty API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechSynthesizerInfoProperty API_AVAILABLE(macos(10.5));  // NSDictionary, see keys below
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechRecentSyncProperty API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechPhonemeSymbolsProperty API_AVAILABLE(macos(10.5));  // NSDictionary, see keys below
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechCurrentVoiceProperty API_AVAILABLE(macos(10.5));  // NSString
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechCommandDelimiterProperty API_AVAILABLE(macos(10.5));  // NSDictionary, see keys below
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechResetProperty API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSSpeechPropertyKey const NSSpeechOutputToFileURLProperty API_AVAILABLE(macos(10.5));  // NSURL


// NSVoiceLanguage has been replaced by NSVoiceLocaleIdentifier
APPKIT_EXTERN NSVoiceAttributeKey const NSVoiceLanguage API_DEPRECATED("", macos(10.0,10.5));

// Values for NSSpeechBoundary
typedef NS_ENUM(NSUInteger, NSSpeechBoundary) {
    NSSpeechImmediateBoundary =  0,
    NSSpeechWordBoundary,
    NSSpeechSentenceBoundary
} API_AVAILABLE(macos(10.5));


@interface NSSpeechSynthesizer : NSObject

- (nullable instancetype)initWithVoice:(nullable NSSpeechSynthesizerVoiceName)voice;

- (BOOL)startSpeakingString:(NSString *)string;
- (BOOL)startSpeakingString:(NSString *)string toURL:(NSURL *)url;

@property (getter=isSpeaking, readonly) BOOL speaking;
- (void)stopSpeaking;
- (void)stopSpeakingAtBoundary:(NSSpeechBoundary)boundary API_AVAILABLE(macos(10.5));
- (void)pauseSpeakingAtBoundary:(NSSpeechBoundary)boundary API_AVAILABLE(macos(10.5));
- (void)continueSpeaking API_AVAILABLE(macos(10.5));

@property (nullable, weak) id<NSSpeechSynthesizerDelegate> delegate;
- (nullable NSSpeechSynthesizerVoiceName)voice;
- (BOOL)setVoice:(nullable NSSpeechSynthesizerVoiceName)voice;
@property float rate API_AVAILABLE(macos(10.5));
@property float volume API_AVAILABLE(macos(10.5));
@property BOOL usesFeedbackWindow;

- (void)addSpeechDictionary:(NSDictionary<NSSpeechDictionaryKey, id> *)speechDictionary API_AVAILABLE(macos(10.5));
- (NSString *)phonemesFromText:(NSString *)text API_AVAILABLE(macos(10.5));

- (nullable id)objectForProperty:(NSSpeechPropertyKey)property error:(NSError **)outError API_AVAILABLE(macos(10.5));
- (BOOL)setObject:(nullable id)object forProperty:(NSSpeechPropertyKey)property error:(NSError **)outError API_AVAILABLE(macos(10.5));

@property (class, readonly, getter=isAnyApplicationSpeaking) BOOL anyApplicationSpeaking;
@property (class, readonly, copy) NSSpeechSynthesizerVoiceName defaultVoice;
@property (class, readonly, copy) NSArray<NSSpeechSynthesizerVoiceName> *availableVoices;
+ (NSDictionary<NSVoiceAttributeKey, id> *)attributesForVoice:(NSSpeechSynthesizerVoiceName)voice;

@end

@protocol NSSpeechSynthesizerDelegate <NSObject>
@optional
- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender didFinishSpeaking:(BOOL)finishedSpeaking;
- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender willSpeakWord:(NSRange)characterRange ofString:(NSString *)string;
- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender willSpeakPhoneme:(short)phonemeOpcode;
- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender didEncounterErrorAtIndex:(NSUInteger)characterIndex ofString:(NSString *)string message:(NSString *)message API_AVAILABLE(macos(10.5));
- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender didEncounterSyncMessage:(NSString *)message API_AVAILABLE(macos(10.5));
@end

typedef NSString * NSSpeechMode NS_TYPED_ENUM;
// Speaking Modes for NSSpeechInputModeProperty
APPKIT_EXTERN NSSpeechMode const NSSpeechModeText API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSSpeechMode const NSSpeechModePhoneme API_AVAILABLE(macos(10.5));
// Speaking Modes for NSSpeechInputModeProperty and NSSpeechNumberModeProperty
APPKIT_EXTERN NSSpeechMode const NSSpeechModeNormal API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSSpeechMode const NSSpeechModeLiteral API_AVAILABLE(macos(10.5));

// Dictionary keys returned by NSSpeechStatusProperty
typedef NSString * NSSpeechStatusKey NS_TYPED_ENUM;
APPKIT_EXTERN NSSpeechStatusKey const NSSpeechStatusOutputBusy API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechStatusKey const NSSpeechStatusOutputPaused API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechStatusKey const NSSpeechStatusNumberOfCharactersLeft API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechStatusKey const NSSpeechStatusPhonemeCode API_AVAILABLE(macos(10.5));  // NSNumber

// Dictionary keys returned by NSSpeechErrorProperty
typedef NSString * NSSpeechErrorKey NS_TYPED_ENUM;
APPKIT_EXTERN NSSpeechErrorKey const NSSpeechErrorCount API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechErrorKey const NSSpeechErrorOldestCode API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechErrorKey const NSSpeechErrorOldestCharacterOffset API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechErrorKey const NSSpeechErrorNewestCode API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechErrorKey const NSSpeechErrorNewestCharacterOffset API_AVAILABLE(macos(10.5));  // NSNumber

// Dictionary keys returned by NSSpeechSynthesizerInfoProperty
typedef NSString * NSSpeechSynthesizerInfoKey NS_TYPED_ENUM;
APPKIT_EXTERN NSSpeechSynthesizerInfoKey const NSSpeechSynthesizerInfoIdentifier API_AVAILABLE(macos(10.5));  // NSString
APPKIT_EXTERN NSSpeechSynthesizerInfoKey const NSSpeechSynthesizerInfoVersion API_AVAILABLE(macos(10.5));  // NSString

// Dictionary keys returned by NSSpeechPhonemeSymbolsProperty
typedef NSString * NSSpeechPhonemeInfoKey NS_TYPED_ENUM;
APPKIT_EXTERN NSSpeechPhonemeInfoKey const NSSpeechPhonemeInfoOpcode API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechPhonemeInfoKey const NSSpeechPhonemeInfoSymbol API_AVAILABLE(macos(10.5));  // NSString
APPKIT_EXTERN NSSpeechPhonemeInfoKey const NSSpeechPhonemeInfoExample API_AVAILABLE(macos(10.5));  // NSString
APPKIT_EXTERN NSSpeechPhonemeInfoKey const NSSpeechPhonemeInfoHiliteStart API_AVAILABLE(macos(10.5));  // NSNumber
APPKIT_EXTERN NSSpeechPhonemeInfoKey const NSSpeechPhonemeInfoHiliteEnd API_AVAILABLE(macos(10.5));  // NSNumber

// Dictionary keys returned by NSSpeechCommandDelimiterProperty
typedef NSString * NSSpeechCommandDelimiterKey NS_TYPED_ENUM;
APPKIT_EXTERN NSSpeechCommandDelimiterKey const NSSpeechCommandPrefix API_AVAILABLE(macos(10.5));  // NSString
APPKIT_EXTERN NSSpeechCommandDelimiterKey const NSSpeechCommandSuffix API_AVAILABLE(macos(10.5));  // NSString

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END



