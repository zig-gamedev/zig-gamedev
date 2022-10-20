/*	NSAttributedString.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/


#import <Foundation/NSString.h>
#import <Foundation/NSDictionary.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSString * NSAttributedStringKey NS_TYPED_EXTENSIBLE_ENUM;

API_AVAILABLE(macos(10.0), ios(3.2), watchos(2.0), tvos(9.0))
@interface NSAttributedString : NSObject <NSCopying, NSMutableCopying, NSSecureCoding>

@property (readonly, copy) NSString *string;
- (NSDictionary<NSAttributedStringKey, id> *)attributesAtIndex:(NSUInteger)location effectiveRange:(nullable NSRangePointer)range;

@end

@interface NSAttributedString (NSExtendedAttributedString)

@property (readonly) NSUInteger length;
- (nullable id)attribute:(NSAttributedStringKey)attrName atIndex:(NSUInteger)location effectiveRange:(nullable NSRangePointer)range;
- (NSAttributedString *)attributedSubstringFromRange:(NSRange)range;

- (NSDictionary<NSAttributedStringKey, id> *)attributesAtIndex:(NSUInteger)location longestEffectiveRange:(nullable NSRangePointer)range inRange:(NSRange)rangeLimit;
- (nullable id)attribute:(NSAttributedStringKey)attrName atIndex:(NSUInteger)location longestEffectiveRange:(nullable NSRangePointer)range inRange:(NSRange)rangeLimit;

- (BOOL)isEqualToAttributedString:(NSAttributedString *)other;

- (instancetype)initWithString:(NSString *)str;
- (instancetype)initWithString:(NSString *)str attributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attrs;
- (instancetype)initWithAttributedString:(NSAttributedString *)attrStr;

typedef NS_OPTIONS(NSUInteger, NSAttributedStringEnumerationOptions) {
  NSAttributedStringEnumerationReverse = (1UL << 1),
  NSAttributedStringEnumerationLongestEffectiveRangeNotRequired = (1UL << 20)
};

- (void)enumerateAttributesInRange:(NSRange)enumerationRange options:(NSAttributedStringEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(NSDictionary<NSAttributedStringKey, id> *attrs, NSRange range, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)enumerateAttribute:(NSAttributedStringKey)attrName inRange:(NSRange)enumerationRange options:(NSAttributedStringEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(id _Nullable value, NSRange range, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

API_AVAILABLE(macos(10.0), ios(3.2), watchos(2.0), tvos(9.0))
@interface NSMutableAttributedString : NSAttributedString

- (void)replaceCharactersInRange:(NSRange)range withString:(NSString *)str;
- (void)setAttributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attrs range:(NSRange)range;

@end

@interface NSMutableAttributedString (NSExtendedMutableAttributedString)

@property (readonly, retain) NSMutableString *mutableString;

- (void)addAttribute:(NSAttributedStringKey)name value:(id)value range:(NSRange)range;
- (void)addAttributes:(NSDictionary<NSAttributedStringKey, id> *)attrs range:(NSRange)range;
- (void)removeAttribute:(NSAttributedStringKey)name range:(NSRange)range;

- (void)replaceCharactersInRange:(NSRange)range withAttributedString:(NSAttributedString *)attrString;
- (void)insertAttributedString:(NSAttributedString *)attrString atIndex:(NSUInteger)loc;
- (void)appendAttributedString:(NSAttributedString *)attrString;
- (void)deleteCharactersInRange:(NSRange)range;
- (void)setAttributedString:(NSAttributedString *)attrString;

- (void)beginEditing;
- (void)endEditing;

@end

// Support for Markdown:

// Presentation intents correspond to the Markdown constructs applied to a certain range.
// The system may supply a default presentation for these intents in certain contexts.

// Inline presentation intents.
// For use with NSInlinePresentationAttributeName.
typedef NS_OPTIONS(NSUInteger, NSInlinePresentationIntent) {
    NSInlinePresentationIntentEmphasized                   = 1 << 0,
    NSInlinePresentationIntentStronglyEmphasized           = 1 << 1,
    NSInlinePresentationIntentCode                         = 1 << 2,
    NSInlinePresentationIntentStrikethrough                = 1 << 5,
    NSInlinePresentationIntentSoftBreak                    = 1 << 6,
    NSInlinePresentationIntentLineBreak                    = 1 << 7,
    NSInlinePresentationIntentInlineHTML                   = 1 << 8,
    NSInlinePresentationIntentBlockHTML                    = 1 << 9
} API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_SWIFT_NAME(InlinePresentationIntent);


// a NSNumber wrapping a value of type NSInlinePresentationIntent
FOUNDATION_EXTERN const NSAttributedStringKey NSInlinePresentationIntentAttributeName
 API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_SWIFT_NAME(inlinePresentationIntent);

// a NSString
FOUNDATION_EXTERN const NSAttributedStringKey NSAlternateDescriptionAttributeName
 API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_SWIFT_NAME(alternateDescription);

// a NSURL
FOUNDATION_EXTERN const NSAttributedStringKey NSImageURLAttributeName
 API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_SWIFT_NAME(imageURL);

// a NSString
FOUNDATION_EXTERN const NSAttributedStringKey NSLanguageIdentifierAttributeName
API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_SWIFT_NAME(languageIdentifier);


typedef NS_ENUM(NSInteger, NSAttributedStringMarkdownParsingFailurePolicy) {
    // If parsing fails, return an error from the appropriate constructor.
    NSAttributedStringMarkdownParsingFailureReturnError = 0,

    // If parsing fails, and if possible, return a partial string. It may contain unparsed markup.
    // Note that if it isn't possible, an error may be returned anyway.
    NSAttributedStringMarkdownParsingFailureReturnPartiallyParsedIfPossible = 1,
} API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_REFINED_FOR_SWIFT;

typedef NS_ENUM(NSInteger, NSAttributedStringMarkdownInterpretedSyntax) {
    // Interpret the full Markdown syntax and produce all relevant attributes
    NSAttributedStringMarkdownInterpretedSyntaxFull = 0,
    
    // Parse all Markdown text, but interpret only attributes that apply to inline spans. Attributes that differentiate blocks (e.g. NSPresentationIntentAttributeName) will not be applied. (Extended attributes apply to inline spans, if allowed, and will also be interpreted.)
    NSAttributedStringMarkdownInterpretedSyntaxInlineOnly = 1,
    
    // Like …InlineOnly, but do not interpret multiple consecutive instances of whitespace as a single separator space. All whitespace characters will appear in the result as they are specified in the source.
    NSAttributedStringMarkdownInterpretedSyntaxInlineOnlyPreservingWhitespace = 2
} API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_REFINED_FOR_SWIFT;


NS_REFINED_FOR_SWIFT
API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
@interface NSAttributedStringMarkdownParsingOptions : NSObject <NSCopying>

- (instancetype)init;

// Whether to allow parsing extensions to Markdown that specify extended attributes. Defaults to NO (only parse CommonMark syntax).
@property BOOL allowsExtendedAttributes;

// What subset of Markdown syntax will be interpreted to produce relevant attributes in the final result.
// Excluded syntax will still be parsed, and the text will be included in the final result. However, it will not have attributes applied to it.
@property NSAttributedStringMarkdownInterpretedSyntax interpretedSyntax;

// The policy to apply if the Markdown source triggers a parsing error.
// The default is NSAttributedStringMarkdownParsingFailureReturnError.
@property NSAttributedStringMarkdownParsingFailurePolicy failurePolicy;

// The BCP-47 language code for this document. If not nil, the NSLanguageAttributeName attribute will be applied to any range in the returned string that doesn't otherwise specify a language attribute.
// The default is nil, which applies no attributes.
@property (nullable, copy) NSString *languageCode;

@end


@interface NSAttributedString (NSAttributedStringCreateFromMarkdown)

// These constructors have a 'baseURL' parameter. If specified, links in the document will be relative to this URL, and images in the document will be looked for relative to this URL (if the other options allow image loading).
// Defaults to nil. If set to nil, paths will not be resolved.

- (nullable instancetype)initWithContentsOfMarkdownFileAtURL:(NSURL *)markdownFile
     options:(nullable NSAttributedStringMarkdownParsingOptions *)options
     baseURL:(nullable NSURL *)baseURL
     error:(NSError **)error
     API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
     NS_REFINED_FOR_SWIFT;

- (nullable instancetype)initWithMarkdown:(NSData *)markdown
     options:(nullable NSAttributedStringMarkdownParsingOptions *)options
     baseURL:(nullable NSURL *)baseURL
     error:(NSError **)error
     API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
     NS_REFINED_FOR_SWIFT;

- (nullable instancetype)initWithMarkdownString:(NSString *)markdownString
     options:(nullable NSAttributedStringMarkdownParsingOptions *)options
     baseURL:(nullable NSURL *)baseURL
     error:(NSError **)error
     API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
     NS_REFINED_FOR_SWIFT;

@end


// Formatting API:

typedef NS_OPTIONS(NSUInteger, NSAttributedStringFormattingOptions) {
    NSAttributedStringFormattingInsertArgumentAttributesWithoutMerging
      API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) = 1 << 0,
    NSAttributedStringFormattingApplyReplacementIndexAttribute
      API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) = 1 << 1,
} NS_REFINED_FOR_SWIFT;

@interface NSAttributedString (NSAttributedStringFormatting)

/// Formats the string using the specified locale (or the canonical one, if nil).
- (instancetype)initWithFormat:(NSAttributedString *)format
                       options:(NSAttributedStringFormattingOptions)options
                        locale:(nullable NSLocale *)locale, ...
    NS_REFINED_FOR_SWIFT
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

/// Formats the string using the arguments list and the specified locale (or the canonical one, if nil).
- (instancetype)initWithFormat:(NSAttributedString *)format
                       options:(NSAttributedStringFormattingOptions)options
                        locale:(nullable NSLocale *)locale
                     arguments:(va_list)arguments
    NS_REFINED_FOR_SWIFT
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

/// Formats the string using the current locale and default options.
+ (instancetype)localizedAttributedStringWithFormat:(NSAttributedString *)format, ...
    NS_REFINED_FOR_SWIFT
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

/// Formats the string using the current locale and the specified options.
+ (instancetype)localizedAttributedStringWithFormat:(NSAttributedString *)format
                                            options:(NSAttributedStringFormattingOptions)options, ...
    NS_REFINED_FOR_SWIFT
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

@end

@interface NSMutableAttributedString (NSMutableAttributedStringFormatting)

/// Formats the specified string and arguments with the current locale,
/// then appends the result to the receiver.
- (void)appendLocalizedFormat:(NSAttributedString *)format, ...
    NS_REFINED_FOR_SWIFT
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

@end

FOUNDATION_EXPORT NSAttributedStringKey const NSReplacementIndexAttributeName
    NS_SWIFT_NAME(replacementIndex)
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));


// -----

@interface NSAttributedString (NSMorphology)

/// If the string has portions tagged with NSInflectionRuleAttributeName
/// that have no format specifiers, create a new string with those portions inflected
/// by following the rule in the attribute.
- (NSAttributedString *)attributedStringByInflectingString
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

@end

FOUNDATION_EXPORT NSAttributedStringKey const NSMorphologyAttributeName
    NS_SWIFT_NAME(morphology)
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

FOUNDATION_EXPORT NSAttributedStringKey const NSInflectionRuleAttributeName
    NS_SWIFT_NAME(inflectionRule)
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

FOUNDATION_EXPORT NSAttributedStringKey const NSInflectionAlternativeAttributeName
    NS_SWIFT_NAME(inflectionAlternative)
    API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

FOUNDATION_EXTERN
const NSAttributedStringKey NSPresentationIntentAttributeName API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

typedef NS_ENUM(NSInteger, NSPresentationIntentKind) {
    NSPresentationIntentKindParagraph,
    NSPresentationIntentKindHeader,
    NSPresentationIntentKindOrderedList,
    NSPresentationIntentKindUnorderedList,
    NSPresentationIntentKindListItem,
    NSPresentationIntentKindCodeBlock,
    NSPresentationIntentKindBlockQuote,
    NSPresentationIntentKindThematicBreak,
    NSPresentationIntentKindTable,
    NSPresentationIntentKindTableHeaderRow,
    NSPresentationIntentKindTableRow,
    NSPresentationIntentKindTableCell,
} API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_REFINED_FOR_SWIFT;

typedef NS_ENUM(NSInteger, NSPresentationIntentTableColumnAlignment) {
    NSPresentationIntentTableColumnAlignmentLeft,
    NSPresentationIntentTableColumnAlignmentCenter,
    NSPresentationIntentTableColumnAlignmentRight,
} API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) NS_REFINED_FOR_SWIFT;

NS_REFINED_FOR_SWIFT
API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0))
@interface NSPresentationIntent: NSObject <NSCopying, NSSecureCoding>

@property (readonly) NSPresentationIntentKind intentKind;

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nullable, strong) NSPresentationIntent *parentIntent;

+ (NSPresentationIntent *)paragraphIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)headerIntentWithIdentity:(NSInteger)identity level:(NSInteger)level nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)codeBlockIntentWithIdentity:(NSInteger)identity languageHint:(nullable NSString *)languageHint nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)thematicBreakIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)orderedListIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)unorderedListIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)listItemIntentWithIdentity:(NSInteger)identity ordinal:(NSInteger)ordinal nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)blockQuoteIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)tableIntentWithIdentity:(NSInteger)identity columnCount:(NSInteger)columnCount alignments:(NSArray<NSNumber *> *)alignments nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)tableHeaderRowIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)tableRowIntentWithIdentity:(NSInteger)identity row:(NSInteger)row nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)tableCellIntentWithIdentity:(NSInteger)identity column:(NSInteger)column nestedInsideIntent:(nullable NSPresentationIntent *)parent;

/// An integer value which uniquely identifies this intent in the document. Identity disambiguates attributes which apply to contiguous text -- for example, two headers in a row with the same level. It can also be used to track the location in an attributed string of a particular part of a document, even after mutation.
@property (readonly) NSInteger identity;

/// If the intent is not a list, this value is 0.
@property (readonly) NSInteger ordinal;

/// If the intent is not a table, this value is `nil`.
@property (nullable, readonly) NSArray<NSNumber *> *columnAlignments;

/// If the intent is not a table, this value is 0.
@property (readonly) NSInteger columnCount;

/// If the intent is not a header, this value is 0.
@property (readonly) NSInteger headerLevel;

/// If the intent is not a code block, this value is `nil`.
@property (readonly, nullable, copy) NSString *languageHint;

/// The column to which this cell belongs (0-based). If the intent is not a cell, this value is 0.
@property (readonly) NSInteger column;

/// The row to which this cell belongs (0-based). If the intent is not a row, this value is 0. Header rows are always row 0. If the table has more rows, those start at row 1.
@property (readonly) NSInteger row;

/// The indentation level of this intent. Each nested list increases the indentation level by one; all elements within the same list (and not then nested into a child list intent) have the same indentation level.
/// Text outside list intents has an indentation level of 0.
@property (readonly) NSInteger indentationLevel;

/// Returns `YES` if this intent is equivalent to the other presentation intent. Equivalence is the same as equality except that identity is not taken into account.
- (BOOL)isEquivalentToPresentationIntent:(NSPresentationIntent *)other;

@end

NS_ASSUME_NONNULL_END
