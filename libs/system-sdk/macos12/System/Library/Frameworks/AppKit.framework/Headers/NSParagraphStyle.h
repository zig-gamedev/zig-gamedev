#if !__has_include(<UIFoundation/NSParagraphStyle.h>)
/*
        NSParagraphStyle.h
        Copyright (c) 1994-2021, Apple Inc.  All rights reserved.
 
	NSParagraphStyle and NSMutableParagraphStyle hold paragraph style information
	NSTextTab holds information about a single tab stop
 */

#import <Foundation/NSObject.h>
#import <AppKit/NSText.h>

@class NSTextBlock;
@class NSTextList;

NS_ASSUME_NONNULL_BEGIN

#if !TARGET_OS_IPHONE

typedef NS_ENUM(NSUInteger, NSLineBreakMode) {
    NSLineBreakByWordWrapping = 0,         // Wrap at word boundaries, default
    NSLineBreakByCharWrapping,        // Wrap at character boundaries
    NSLineBreakByClipping,        // Simply clip
    NSLineBreakByTruncatingHead,    // Truncate at head of line: "...wxyz"
    NSLineBreakByTruncatingTail,    // Truncate at tail of line: "abcd..."
    NSLineBreakByTruncatingMiddle    // Truncate middle of line:  "ab...yz"
} API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0));

// Line break strategy describes a collection of options that can affect where line breaks are placed in a paragraph.
// This is independent from line break mode, which describes what happens when text is too long to fit within its container.
// These options won't have any effect when used with line break modes that don't support multiple lines, like clipping or truncating middle.
typedef NS_OPTIONS(NSUInteger, NSLineBreakStrategy) {
    // Don't use any line break strategies
    NSLineBreakStrategyNone = 0,
    // Use the push out line break strategy.
    // This strategy allows the text system to "push out" individual lines by some number of words to avoid an orphan word on the last line of the paragraph.
    // The current implementation usually pushes out the last line by a single word.
    NSLineBreakStrategyPushOut API_AVAILABLE(macos(10.11), ios(9.0)) = 1 << 0,
    // When specified, it prohibits breaking between Hangul characters. It is the preferable typesetting strategy for the modern Korean documents suitable for UI strings.
    NSLineBreakStrategyHangulWordPriority API_AVAILABLE(macos(11.0), ios(14.0)) = 1 << 1,
    // Use the same configuration of line break strategies that the system uses for standard UI labels. This set of line break strategies is optimized for displaying shorter strings that are common in UI labels and may not be suitable for large amounts of text.
    NSLineBreakStrategyStandard API_AVAILABLE(macos(11.0), ios(14.0)) = 0xFFFF
};

// NSTextTab
typedef NSString * NSTextTabOptionKey NS_TYPED_ENUM;
APPKIT_EXTERN NSTextTabOptionKey  NSTabColumnTerminatorsAttributeName API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0)); // An attribute for NSTextTab options.  The value is NSCharacterSet.  The character set is used to determine the tab column terminating character.  The tab and newline characters are implied even if not included in the character set.

API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0)) @interface NSTextTab : NSObject <NSCopying, NSCoding, NSSecureCoding>

+ (NSCharacterSet *)columnTerminatorsForLocale:(nullable NSLocale *)aLocale API_AVAILABLE(macos(10.11), ios(7.0), watchos(2.0), tvos(9.0)); // Returns the column terminators for locale. Passing nil returns an instance corresponding to +[NSLocale systemLocale]. For matching user's formatting preferences, pass +[NSLocale currentLocale]. Can be used as the value for NSTabColumnTerminatorsAttributeName to make a decimal tab stop.

- (instancetype)initWithTextAlignment:(NSTextAlignment)alignment location:(CGFloat)loc options:(NSDictionary<NSTextTabOptionKey, id> *)options NS_DESIGNATED_INITIALIZER; // Initializes a text tab with the text alignment, location, and options.  The text alignment is used to determine the position of text inside the tab column.


@property (readonly) NSTextAlignment alignment;  // Defines the alignment of tab column contents. NSTextAlignmentNatural and NSTextAlignmentJustified are resolved either NSTextAlignmentLeft or NSTextAlignmentRight based on the user's preferred language.
@property (readonly) CGFloat location; // Location of the tab stop inside the line fragment rect coordinate system
@property (readonly) NSDictionary<NSTextTabOptionKey, id> *options; // Optional configuration attributes
@end


// NSParagraphStyle

API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0)) @interface NSParagraphStyle : NSObject <NSCopying, NSMutableCopying, NSSecureCoding>

@property (class, readonly, copy) NSParagraphStyle *defaultParagraphStyle; // This class property returns a shared and cached NSParagraphStyle instance with the default style settings, with same value as the result of [[NSParagraphStyle alloc] init].

+ (NSWritingDirection)defaultWritingDirectionForLanguage:(nullable NSString *)languageName;  // languageName is in ISO lang region format

@property (readonly) CGFloat lineSpacing; // "Leading": distance between the bottom of one line fragment and top of next (applied between lines in the same container). This value is included in the line fragment heights in layout manager.
@property (readonly) CGFloat paragraphSpacing; // Distance between the bottom of this paragraph and top of next (or the beginning of its paragraphSpacingBefore, if any).
@property (readonly) NSTextAlignment alignment;

// The following values are relative to the appropriate margin (depending on the paragraph direction)

@property (readonly) CGFloat headIndent; // Distance from margin to front edge of paragraph
@property (readonly) CGFloat tailIndent; // Distance from margin to back edge of paragraph; if negative or 0, from other margin
@property (readonly) CGFloat firstLineHeadIndent; // Distance from margin to edge appropriate for text direction

@property (readonly) CGFloat minimumLineHeight; // Line height is the distance from bottom of descenders to top of ascenders; basically the line fragment height. Does not include lineSpacing (which is added after this computation).
@property (readonly) CGFloat maximumLineHeight; // 0 implies no maximum.

@property (readonly) NSLineBreakMode lineBreakMode;

@property (readonly) NSWritingDirection baseWritingDirection;

@property (readonly) CGFloat lineHeightMultiple; // Natural line height is multiplied by this factor (if positive) before being constrained by minimum and maximum line height.
@property (readonly) CGFloat paragraphSpacingBefore; // Distance between the bottom of the previous paragraph (or the end of its paragraphSpacing, if any) and the top of this paragraph.

// Specifies the threshold for hyphenation.  Valid values lie between 0.0 and 1.0 inclusive.  Hyphenation will be attempted when the ratio of the text width as broken without hyphenation to the width of the line fragment is less than the hyphenation factor.  When this takes on its default value of 0.0, the layout manager's hyphenation factor is used instead.  When both are 0.0, hyphenation is disabled.
@property (readonly) float hyphenationFactor;

// A property controlling the hyphenation behavior for the paragraph associated with the paragraph style. The exact hyphenation logic is dynamically determined by the layout context such as language, platform, etc. When YES, it affects the return value from -hyphenationFactor when the property is set to 0.0.
@property (readonly) BOOL usesDefaultHyphenation API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));

@property (readonly,copy) NSArray<NSTextTab *> *tabStops API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0)); // An array of NSTextTabs. Contents should be ordered by location. The default value is an array of 12 left-aligned tabs at 28pt interval
@property (readonly) CGFloat defaultTabInterval API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0)); // The default tab interval used for locations beyond the last element in tabStops

@property (readonly) BOOL allowsDefaultTighteningForTruncation API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)); // Tightens inter-character spacing in attempt to fit lines wider than the available space if the line break mode is one of the truncation modes before starting to truncate. YES by default for apps linked against 10.11 and later SDK. The maximum amount of tightening performed is determined by the system based on contexts such as font, line width, etc.

// Specifies the threshold for using tightening as an alternative to truncation when -allowsDefaultTighteningForTruncation=NO.  When the line break mode specifies truncation, the text system will attempt to tighten inter-character spacing as an alternative to truncation, provided that the ratio of the text width to the line fragment width does not exceed 1.0 + tighteningFactorForTruncation.  Otherwise the text will be truncated at a location determined by the line break mode.  The default value is 0.0 for apps linked against 10.11 and later SDK. This property is ignored when -allowsDefaultTighteningForTruncation=YES. Explicitly setting this property to 0.0 has a side effect of also setting -allowsDefaultTighteningForTruncation to NO.
@property (readonly) float tighteningFactorForTruncation;

@property (readonly, copy) NSArray<__kindof NSTextBlock *> *textBlocks;    // Array to specify the text blocks containing the paragraph, nested from outermost to innermost.
@property (readonly, copy) NSArray<NSTextList *> *textLists;     // Array to specify the text lists containing the paragraph, nested from outermost to innermost.

// Specifies whether the paragraph is to be treated as a header for purposes of HTML generation.  Should be set to 0 (the default value) if the paragraph is not a header, or from 1 through 6 if the paragraph is to be treated as a header.
@property (readonly) NSInteger headerLevel;

// Specifies the line break strategies that may be used for laying out the paragraph.  The default value is NSLineBreakStrategyNone.
@property (readonly) NSLineBreakStrategy lineBreakStrategy API_AVAILABLE(macos(10.11), ios(9.0));
@end


API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0)) @interface NSMutableParagraphStyle : NSParagraphStyle

@property CGFloat lineSpacing;
@property CGFloat paragraphSpacing;
@property NSTextAlignment alignment;
@property CGFloat firstLineHeadIndent;
@property CGFloat headIndent;
@property CGFloat tailIndent;
@property NSLineBreakMode lineBreakMode;
@property CGFloat minimumLineHeight;
@property CGFloat maximumLineHeight;
@property NSWritingDirection baseWritingDirection;
@property CGFloat lineHeightMultiple;
@property CGFloat paragraphSpacingBefore;
@property float hyphenationFactor;
@property (readwrite) BOOL usesDefaultHyphenation API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));
@property (null_resettable, copy) NSArray<NSTextTab *> *tabStops API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0));
@property CGFloat defaultTabInterval API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0));
@property BOOL allowsDefaultTighteningForTruncation API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

- (void)addTabStop:(NSTextTab *)anObject API_AVAILABLE(macos(10.0), ios(9.0), watchos(2.0), tvos(9.0));
- (void)removeTabStop:(NSTextTab *)anObject API_AVAILABLE(macos(10.0), ios(9.0), watchos(2.0), tvos(9.0));

- (void)setParagraphStyle:(NSParagraphStyle *)obj API_AVAILABLE(macos(10.0), ios(9.0), watchos(2.0), tvos(9.0));

@property float tighteningFactorForTruncation;
@property (copy) NSArray<__kindof NSTextBlock *> *textBlocks;
@property (copy) NSArray<NSTextList *> *textLists;
@property NSInteger headerLevel;
@property NSLineBreakStrategy lineBreakStrategy API_AVAILABLE(macos(10.11), ios(9.0));
@end

/************************ Deprecated ************************/
// NSTextTabType and NSTextTab methods using the type are soft deprecated starting with OS X 10.11. It will be officially deprecated in a future release. Use NSTextAlignment-based API instead
typedef NS_ENUM(NSUInteger, NSTextTabType) {
    NSLeftTabStopType = 0,
    NSRightTabStopType,
    NSCenterTabStopType,
    NSDecimalTabStopType
};

@interface NSTextTab (NSTextTabDeprecated)
- (instancetype)initWithType:(NSTextTabType)type location:(CGFloat)loc; // Use -initWithTextAlignment:location:options:
@property (readonly) NSTextTabType tabStopType; // Use -alignment and -options
@end



#endif // !TARGET_OS_IPHONE

NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSParagraphStyle.h>
#endif

