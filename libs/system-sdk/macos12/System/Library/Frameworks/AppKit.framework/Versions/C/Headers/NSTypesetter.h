#if !__has_include(<UIFoundation/NSTypesetter.h>)
/*
	NSTypesetter.h
	Copyright (c) 1994-2021, Apple Inc.  All rights reserved. 

	An abstract class to lay glyphs out in horizontal or vertical boxes	
*/

#import <CoreFoundation/CFCharacterSet.h> // for UTF32Char
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <AppKit/NSLayoutManager.h>
#import <AppKit/NSParagraphStyle.h>

NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

@interface NSTypesetter : NSObject

/* Primitive typesetting methods */
/* NSLayoutManager attributes */
/* Controls whether leading value specified by fonts affects line spacing
*/
@property BOOL usesFontLeading;

/* Controls various typesetting behavior for backward compatibility
*/
@property NSTypesetterBehavior typesetterBehavior;

/* Controls hyphenation factor. The value should be between 0.0 and 1.0.
*/
@property float hyphenationFactor;

/* NSTextContainer attributes */
/* Controls padding at both ends of line fragment.
*/
@property CGFloat lineFragmentPadding;

/* Screen/printer font mapping */
- (NSFont *)substituteFontForFont:(NSFont *)originalFont;

/* Tab stops */
/* Returns an NSTextTab instance for glyphLocation from the current text tab array. glyphLocation must be in the base typesetter coordinate (should not include line fragment rect origin and lineFragmentPadding). direction should be either NSWritingDirectionLeftToRight or NSWritingDirectionRightToLeft. maxLocation specifies the maximum location a glyph can be assigned in the base typesetter coordinate.
*/
- (nullable NSTextTab *)textTabForGlyphLocation:(CGFloat)glyphLocation writingDirection:(NSWritingDirection)direction maxLocation:(CGFloat)maxLocation;

/* Bidi control */
/* Controls whether to perform bi-directional processing.  You can disable the layout stage if you know the parapgraph does not need this stage (i.e. the backing-store is in the display order).
*/
@property BOOL bidiProcessingEnabled;

/* Accessors for required paragraph typesetting settings */ // Note this method does not retain attrString
@property (nullable, assign) NSAttributedString *attributedString;
- (void)setParagraphGlyphRange:(NSRange)paragraphRange separatorGlyphRange:(NSRange)paragraphSeparatorRange;
@property (readonly) NSRange paragraphGlyphRange; // does not include paragraphSeparatorGlyphRange
@property (readonly) NSRange paragraphSeparatorGlyphRange;
@property (readonly) NSRange paragraphCharacterRange;
@property (readonly) NSRange paragraphSeparatorCharacterRange;

/* layout primitive */
/* lineFragmentOrigin specifies the upper-left corner of line fragment rect.  On return, set to the next origin. The method returns the next glyph index. Usually the index right after paragraph separator but can be inside the paragraph range (i.e. text container exhaustion). Concrete subclasses must implement this method. A concrete implementation must invoke -beginParagraph, -endParagraph, -beginLineWithGlyphAtIndex: and -endLineWithGlyphRange:.
*/
- (NSUInteger)layoutParagraphAtPoint:(NSPointPointer)lineFragmentOrigin;

/* Layout parameter setup */
- (void)beginParagraph; // should be invoked at the beginning of -layoutParagraphAtPoint:
- (void)endParagraph; // should be invoked at the end of -layoutParagraphAtPoint:
- (void)beginLineWithGlyphAtIndex:(NSUInteger)glyphIndex; // should be invoked at the beginning of each line
- (void)endLineWithGlyphRange:(NSRange)lineGlyphRange; // should be invoked at the end of each line

/* Line/paragraph spacing */
- (CGFloat)lineSpacingAfterGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect;
- (CGFloat)paragraphSpacingBeforeGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect;
- (CGFloat)paragraphSpacingAfterGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect;

/* Empty paragraph handling */
/* This method calculates the line fragment rect/line fragment used rect for blank lines. theParagraphSeparatorGlyphRange with length == 0 indicates extra line fragment.
*/
- (void)getLineFragmentRect:(NSRectPointer)lineFragmentRect usedRect:(NSRectPointer)lineFragmentUsedRect forParagraphSeparatorGlyphRange:(NSRange)paragraphSeparatorGlyphRange atProposedOrigin:(NSPoint)lineOrigin;

/* Extra line fragment handling */
/* This method returns the attributes used to layout the extra line fragment. The default implementation tries to use -[NSTextView typingAttributes] if possible; otherwise, uses attributes for the last character.
*/
@property (readonly, copy) NSDictionary<NSAttributedStringKey, id> *attributesForExtraLineFragment;

/* Control/format character handling */
/* Cocoa Text System specific interface methods */
/* Friend class accessors */
@property (nullable, readonly, assign) NSLayoutManager *layoutManager;
@property (nullable, readonly, assign) NSArray<NSTextContainer *> *textContainers;
@property (nullable, readonly, assign) NSTextContainer *currentTextContainer;
@property (nullable, readonly, copy) NSParagraphStyle *currentParagraphStyle;

/* Forces NSLayoutManager to invalidate glyph cache in range when invalidating layout.
*/
- (void)setHardInvalidation:(BOOL)flag forGlyphRange:(NSRange)glyphRange;

/*  Actually lay out glyphs starting at the given index.  The given number of line fragments will be generated as long as the NSGlyphStorage doesn't run out of glyphs.  nextGlyph will be set to the index of the next glyph that needs to be laid out. New applications should use -layoutCharactersInRange:forLayoutManager:maximumNumberOfLineFragments: instead.
*/
- (void)layoutGlyphsInLayoutManager:(NSLayoutManager *)layoutManager startingAtGlyphIndex:(NSUInteger)startGlyphIndex maxNumberOfLineFragments:(NSUInteger)maxNumLines nextGlyphIndex:(NSUInteger *)nextGlyph;

/* Layouts characters in characterRange for layoutManager. The method returns the actual character range that the receiving NSTypesetter processed. The layout process can be interrupted when the number of line fragments exceeds maxNumLines. Specify NSUIntegerMax for unlimited number of line fragments.
*/
- (NSRange)layoutCharactersInRange:(NSRange)characterRange forLayoutManager:(NSLayoutManager *)layoutManager maximumNumberOfLineFragments:(NSUInteger)maxNumLines API_AVAILABLE(macos(10.5));

/* Returns the offset to be applied to the glyph in question when printing.
*/
+ (NSSize)printingAdjustmentInLayoutManager:(NSLayoutManager *)layoutMgr forNominallySpacedGlyphRange:(NSRange)nominallySpacedGlyphsRange packedGlyphs:(const unsigned char *)packedGlyphs count:(NSUInteger)packedGlyphsCount;

- (CGFloat)baselineOffsetInLayoutManager:(NSLayoutManager *)layoutMgr glyphIndex:(NSUInteger)glyphIndex;


/* Factory methods */
@property (class, readonly, strong) __kindof NSTypesetter *sharedSystemTypesetter;
+ (id)sharedSystemTypesetterForBehavior:(NSTypesetterBehavior)behavior;
@property (class, readonly) NSTypesetterBehavior defaultTypesetterBehavior;
@end

/* NSLayoutPhaseInterface declares various subclass override points that are invoked if implemented */
@interface NSTypesetter (NSLayoutPhaseInterface)
// Called right before setLineFragmentRect:forGlyphRange:usedRect:
- (void)willSetLineFragmentRect:(NSRectPointer)lineRect forGlyphRange:(NSRange)glyphRange usedRect:(NSRectPointer)usedRect baselineOffset:(CGFloat *)baselineOffset;

- (BOOL)shouldBreakLineByWordBeforeCharacterAtIndex:(NSUInteger)charIndex;
- (BOOL)shouldBreakLineByHyphenatingBeforeCharacterAtIndex:(NSUInteger)charIndex;

- (float)hyphenationFactorForGlyphAtIndex:(NSUInteger)glyphIndex;
- (UTF32Char)hyphenCharacterForGlyphAtIndex:(NSUInteger)glyphIndex;

- (NSRect)boundingBoxForControlGlyphAtIndex:(NSUInteger)glyphIndex forTextContainer:(NSTextContainer *)textContainer proposedLineFragment:(NSRect)proposedRect glyphPosition:(NSPoint)glyphPosition characterIndex:(NSUInteger)charIndex;
@end

/* NSGlyphStorageInterface declares all primitives interfacing to the glyph storage (usually NSLayoutManager). By overriding all the methods, you can implement an NSTypesetter subclass that interacts with custom glyph storage. */
@interface NSTypesetter (NSGlyphStorageInterface)
// Glyph/character range mappings
- (NSRange)characterRangeForGlyphRange:(NSRange)glyphRange actualGlyphRange:(nullable NSRangePointer)actualGlyphRange;
- (NSRange)glyphRangeForCharacterRange:(NSRange)charRange actualCharacterRange:(nullable NSRangePointer)actualCharRange;

// Glyph data

// NSTextContainer attribute
/* Calculates line fragment rect, line fragment used rect, and remaining rect for a line fragment starting at glyph index startingGlyphIndex with proposedRect. The height is determined using lineSpacing, paragraphSpacingBefore, and paragraphSpacingAfter as well as proposedRect. The width for lineFragmentUsedRect is set to the lineFragmentRect width. In the standard implementation, paragraph spacing is included in the line fragment rect but not the line fragment used rect; line spacing is included in both.
*/
- (void)getLineFragmentRect:(null_unspecified NSRectPointer)lineFragmentRect usedRect:(null_unspecified NSRectPointer)lineFragmentUsedRect remainingRect:(null_unspecified NSRectPointer)remainingRect forStartingGlyphAtIndex:(NSUInteger)startingGlyphIndex proposedRect:(NSRect)proposedRect lineSpacing:(CGFloat)lineSpacing paragraphSpacingBefore:(CGFloat)paragraphSpacingBefore paragraphSpacingAfter:(CGFloat)paragraphSpacingAfter;

// Layout storage
- (void)setLineFragmentRect:(NSRect)fragmentRect forGlyphRange:(NSRange)glyphRange usedRect:(NSRect)usedRect baselineOffset:(CGFloat)baselineOffset;
- (void)setNotShownAttribute:(BOOL)flag forGlyphRange:(NSRange)glyphRange;
- (void)setDrawsOutsideLineFragment:(BOOL)flag forGlyphRange:(NSRange)glyphRange;
- (void)setLocation:(NSPoint)location withAdvancements:(null_unspecified const CGFloat *)advancements forStartOfGlyphRange:(NSRange)glyphRange;
- (void)setAttachmentSize:(NSSize)attachmentSize forGlyphRange:(NSRange)glyphRange;
- (void)setBidiLevels:(null_unspecified const uint8_t *)levels forGlyphRange:(NSRange)glyphRange;
@end

typedef NS_OPTIONS(NSUInteger, NSTypesetterControlCharacterAction) {
    NSTypesetterZeroAdvancementAction = (1 << 0), // glyphs with this action are flitered out from layout (notShownAttribute == YES)
    NSTypesetterWhitespaceAction = (1 << 1), // the width for glyphs with this action are determined by -boundingBoxForControlGlyphAtIndex:forTextContainer:proposedLineFragment:glyphPosition:characterIndex: if the method is implemented; otherwise, same as NSTypesetterZeroAdvancementAction
    NSTypesetterHorizontalTabAction = (1 << 2), // Treated as tab character
    NSTypesetterLineBreakAction = (1 << 3), // Causes line break
    NSTypesetterParagraphBreakAction = (1 << 4), // Causes paragraph break; firstLineIndent will be used for the following glyph
    NSTypesetterContainerBreakAction = (1 << 5) // Causes container break
}; // Deprecated. Use NSControlCharacterAction instead


@interface NSTypesetter (NSTypesetter_Deprecated)
- (NSTypesetterControlCharacterAction)actionForControlCharacterAtIndex:(NSUInteger)charIndex; // Deprecated. Use -layoutManager:shouldUseAction:forControlCharacterAtIndex: instead
- (NSUInteger)getGlyphsInRange:(NSRange)glyphsRange glyphs:(null_unspecified NSGlyph *)glyphBuffer characterIndexes:(null_unspecified NSUInteger *)charIndexBuffer glyphInscriptions:(null_unspecified NSGlyphInscription *)inscribeBuffer elasticBits:(null_unspecified BOOL *)elasticBuffer bidiLevels:(null_unspecified unsigned char *)bidiLevelBuffer API_DEPRECATED("", macos(10.3,10.13));

- (void)substituteGlyphsInRange:(NSRange)glyphRange withGlyphs:(null_unspecified NSGlyph *)glyphs  API_DEPRECATED("", macos(10.3,10.13));
- (void)insertGlyph:(NSGlyph)glyph atGlyphIndex:(NSUInteger)glyphIndex characterIndex:(NSUInteger)characterIndex API_DEPRECATED("", macos(10.3,10.13));
- (void)deleteGlyphsInRange:(NSRange)glyphRange API_DEPRECATED("", macos(10.3,10.13));
@end



#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSTypesetter.h>
#endif
