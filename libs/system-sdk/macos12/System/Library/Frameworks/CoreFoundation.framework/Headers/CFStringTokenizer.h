/*	CFStringTokenizer.h
	Copyright (c) 2006-2019, Apple Inc. All rights reserved.
*/

/*!
    @header CFStringTokenizer
    @abstract A CFStringTokenizer object provides the means by which you can
		tokenize a string. To find a token that includes the character
		specified by character index and set it as the current token, you call
		CFStringTokenizerGoToTokenAtIndex. To advance to the next token and set
		it as the current token, you call CFStringTokenizerAdvanceToNextToken.
		To get the range of current token, you call
		CFStringTokenizerGetCurrentTokenRange. You can use
		CFStringTokenizerCopyCurrentTokenAttribute to get the attribute of
		current token. If current token is a compound, you can call
		CFStringTokenizerGetCurrentSubTokens to retrieve the subtokens or derived
		subtokens contained in the compound token.
		To guess the language of a string, you call 
		CFStringTokenizerCopyBestStringLanguage.
*/

#if !defined(__COREFOUNDATION_CFSTRINGTOKENIZER__)
#define __COREFOUNDATION_CFSTRINGTOKENIZER__ 1

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFLocale.h>
#include <CoreFoundation/CFString.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN

/*
===================================================================
	Language Identifier
===================================================================
*/

/*!
	@function CFStringTokenizerCopyBestStringLanguage
	@abstract Guesses the language of a string and returns the BCP 47 string of the
		language.
	@param string The string whose language is to be guessed.
	@param range The range of characters in string whose language to be
		guessed. The specified range must not exceed the bounds of the string.
	@result A language represented in BCP 47 string. NULL is returned either if
		string is NULL, the location of range is negative, the length of range
		is 0, or the language of the string cannot be guessed.
	@discussion The result is not guaranteed to be accurate. Typically 200-400
		characters are required to reliably guess the language of a string.
*/
CF_EXPORT
CFStringRef CFStringTokenizerCopyBestStringLanguage(CFStringRef string, CFRange range) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));

/*
===================================================================
	Tokenization
===================================================================
*/

typedef struct CF_BRIDGED_MUTABLE_TYPE(id) __CFStringTokenizer * CFStringTokenizerRef;

/*!
	Tokenization options
*/
CF_ENUM(CFOptionFlags) {
    /*!
      Tokenization Unit
      Use one of tokenization unit options with CFStringTokenizerCreate to
      specify how the string should be tokenized. 
    */
    /* kCFStringTokenizerUnitWord doesn't return space between words as a token. */
    kCFStringTokenizerUnitWord                           = 0,
    kCFStringTokenizerUnitSentence                       = 1,
    kCFStringTokenizerUnitParagraph                      = 2,
    kCFStringTokenizerUnitLineBreak                      = 3,
    /* kCFStringTokenizerUnitWordBoundary can be used in double click detection
    and whole word search. It is locale sensitive. If the locale parameter of
    CFStringTokenizerCreate is NULL, default locale is used.
    kCFStringTokenizerUnitWordBoundary returns space between words as a token. */
    kCFStringTokenizerUnitWordBoundary                   = 4,

    /*!
      Attribute Specifier
      Use attribute specifier to tell tokenizer to prepare the specified attribute
      when it tokenizes the given string. The attribute value can be retrieved by
      calling CFStringTokenizerCopyCurrentTokenAttribute with one of the attribute
      option. 
    */
    /* Latin Transcription. Used with kCFStringTokenizerUnitWord or
       kCFStringTokenizerUnitWordBoundary */
    kCFStringTokenizerAttributeLatinTranscription        = 1UL << 16,
    /* Language in BCP 47 string. Used with kCFStringTokenizerUnitSentence
	   or kCFStringTokenizerUnitParagraph. */
    kCFStringTokenizerAttributeLanguage                  = 1UL << 17,
};

/*!
	Token type
	CFStringTokenizerGoToTokenAtIndex / CFStringTokenizerAdvanceToNextToken returns
	the type of current token.
*/
typedef CF_OPTIONS(CFOptionFlags, CFStringTokenizerTokenType) {
    /* Have no token. */
    kCFStringTokenizerTokenNone                                      = 0,
    
    /* Normal token */
    kCFStringTokenizerTokenNormal                                    = 1UL << 0,
    
    /*!
        Compound token which may contain subtokens but with no derived subtokens.
        Its subtokens can be obtained by calling CFStringTokenizerGetCurrentSubTokens.
    */
    kCFStringTokenizerTokenHasSubTokensMask                          = 1UL << 1,
    
    /*!
        Compound token which may contain derived subtokens.
        Its subtokens and derived subtokens can be obtained by calling
        CFStringTokenizerGetCurrentSubTokens.
    */
    kCFStringTokenizerTokenHasDerivedSubTokensMask                   = 1UL << 2,
    
    kCFStringTokenizerTokenHasHasNumbersMask                         = 1UL << 3,
    kCFStringTokenizerTokenHasNonLettersMask                         = 1UL << 4,
    kCFStringTokenizerTokenIsCJWordMask                              = 1UL << 5
};

/*!
	@function CFStringTokenizerGetTypeID
	@abstract Get the type identifier.
	@result the type identifier of all CFStringTokenizer instances.
*/
CF_EXPORT
CFTypeID CFStringTokenizerGetTypeID(void) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));
																				
/*!
	@function CFStringTokenizerCreate
	@abstract Creates a tokenizer instance.
	@param alloc The CFAllocator which should be used to allocate memory for the
		tokenizer and its storage for values. This parameter may be NULL in which 
		case the current default CFAllocator is used. 	
	@param string The string to tokenize.
	@param range The range of characters within the string to be tokenized. The
		specified range must not exceed the length of the string.
	@param options Use one of the Tokenization Unit options to specify how the 
		string should be tokenized. Optionally specify one or more attribute
		specifiers to tell the tokenizer to prepare specified attributes when it
		tokenizes the string.
	@param locale The locale to specify language or region specific behavior. Pass
               NULL if you want tokenizer to identify the locale automatically.
	@result A reference to the new CFStringTokenizer.
*/
CF_EXPORT
CFStringTokenizerRef CFStringTokenizerCreate(CFAllocatorRef alloc, CFStringRef string, CFRange range, CFOptionFlags options, CFLocaleRef locale) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));

/*!
	@function CFStringTokenizerSetString
	@abstract Set the string to tokenize.
	@param tokenizer The reference to CFStringTokenizer returned by
		CFStringTokenizerCreate.
	@param string The string to tokenize.
	@param range The range of characters within the string to be tokenized. The
		specified range must not exceed the length of the string.
*/
CF_EXPORT
void CFStringTokenizerSetString(CFStringTokenizerRef tokenizer, CFStringRef string, CFRange range) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));
																		
/*!
	@function CFStringTokenizerGoToTokenAtIndex
	@abstract Random access to a token. Find a token that includes the character specified
		by character index, and set it as the current token.
	@param tokenizer The reference to CFStringTokenizer returned by
		CFStringTokenizerCreate.
	@param index The index of the Unicode character in the CFString.
	@result Type of the token if succeeded in finding a token and setting it as
		current token. kCFStringTokenizerTokenNone if failed in finding a token.
	@discussion The range and attribute of the token can be obtained by calling
		CFStringTokenizerGetCurrentTokenRange and CFStringTokenizerCopyCurrentTokenAttribute.
		If the token is a compound (with type kCFStringTokenizerTokenHasSubTokensMask or
		kCFStringTokenizerTokenHasDerivedSubTokensMask), its subtokens and
		(or) derived subtokens can be obtained by calling CFStringTokenizerGetCurrentSubTokens. 
*/
CF_EXPORT
CFStringTokenizerTokenType CFStringTokenizerGoToTokenAtIndex(CFStringTokenizerRef tokenizer, CFIndex index) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));

/*!
	@function CFStringTokenizerAdvanceToNextToken
	@abstract Token enumerator.
	@param tokenizer The reference to CFStringTokenizer returned by
		CFStringTokenizerCreate.
	@result Type of the token if succeeded in finding a token and setting it as
		current token. kCFStringTokenizerTokenNone if failed in finding a token.
	@discussion If there is no preceding call to CFStringTokenizerGoToTokenAtIndex 
		or CFStringTokenizerAdvanceToNextToken, it finds the first token in the range
		specified to CFStringTokenizerCreate. If there is a current token after successful
		call to CFStringTokenizerGoToTokenAtIndex or CFStringTokenizerAdvanceToNextToken,
		it proceeds to the next token. If succeeded in finding a token, set it as current 
		token and return its token type. Otherwise invalidate current token and return
		kCFStringTokenizerTokenNone.
		The range and attribute of the token can be obtained by calling
		CFStringTokenizerGetCurrentTokenRange and 
        CFStringTokenizerCopyCurrentTokenAttribute. If the token is a compound
		(with type kCFStringTokenizerTokenHasSubTokensMask or
		kCFStringTokenizerTokenHasDerivedSubTokensMask), its subtokens and
		(or) derived subtokens can be obtained by calling CFStringTokenizerGetCurrentSubTokens. 
*/
CF_EXPORT
CFStringTokenizerTokenType CFStringTokenizerAdvanceToNextToken(CFStringTokenizerRef tokenizer) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));

/*!
	@function CFStringTokenizerGetCurrentTokenRange
	@abstract Returns the range of current token.
	@param tokenizer The reference to CFStringTokenizer returned by
		CFStringTokenizerCreate.
	@result Range of current token, or {kCFNotFound,0} if there is no current token.
*/
CF_EXPORT 
CFRange CFStringTokenizerGetCurrentTokenRange(CFStringTokenizerRef tokenizer) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));
																				
/*!
	@function CFStringTokenizerCopyCurrentTokenAttribute
	@abstract Copies the specified attribute of current token.
	@param tokenizer The reference to CFStringTokenizer returned by
		CFStringTokenizerCreate.
	@param attribute Specify a token attribute you want to obtain. The value is
		one of kCFStringTokenizerAttributeLatinTranscription or
		kCFStringTokenizerAttributeLanguage.
	@result Token attribute, or NULL if current token does not have the specified
		attribute or if there is no current token.
*/
CF_EXPORT
CFTypeRef CFStringTokenizerCopyCurrentTokenAttribute(CFStringTokenizerRef tokenizer, CFOptionFlags attribute) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));

/*!
	@function CFStringTokenizerGetCurrentSubTokens
	@abstract Retrieves the subtokens or derived subtokens contained in the compound token.
	@param tokenizer The reference to CFStringTokenizer returned by CFStringTokenizerCreate.
	@param ranges An array of CFRange to fill in with the ranges of subtokens. The filled in 
		ranges are relative to the string specified to CFStringTokenizerCreate. This parameter
		can be NULL.
	@param maxRangeLength The maximum number of ranges to return.
	@param derivedSubTokens An array of CFMutableArray to which the derived subtokens are to
		be added. This parameter can be NULL.
	@result number of subtokens.
	@discussion If token type is kCFStringTokenizerTokenNone, the ranges array and 
		derivedSubTokens array are untouched and the return value is 0.
        If token type is kCFStringTokenizerTokenNormal, the ranges array has one item
        filled in with the entire range of the token (if maxRangeLength >= 1) and a string
		taken from the entire token range is added to the derivedSubTokens array and the 
		return value is 1.
		If token type is kCFStringTokenizerTokenHasSubTokensMask or
        kCFStringTokenizerTokenHasDerivedSubTokensMask, the ranges array is filled
        in with as many items as there are subtokens (up to a limit of maxRangeLength).
		The derivedSubTokens array will have sub tokens added even when the sub token is a 
		substring of the token. If token type is kCFStringTokenizerTokenHasSubTokensMask,
		the ordinary non-derived subtokens are added to the derivedSubTokens array. 
*/
CF_EXPORT
CFIndex CFStringTokenizerGetCurrentSubTokens(CFStringTokenizerRef tokenizer, CFRange *ranges, CFIndex maxRangeLength, CFMutableArrayRef derivedSubTokens) API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));

CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif /* ! __COREFOUNDATION_CFSTRINGTOKENIZER__ */
