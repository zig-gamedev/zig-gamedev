#if !__has_include(<UIFoundation/NSLayoutManager.h>)
/*
        NSLayoutManager.h
        Application Kit
        Copyright (c) 1994-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <AppKit/NSTextStorage.h>
#import <AppKit/NSFont.h>
#import <AppKit/NSGlyphGenerator.h>

@class NSTextContainer;
@class NSColor;
@class NSTypesetter;
@class NSTextView;
@class NSGraphicsContext;
@class NSAffineTransform;
@class NSBox;
@class NSRulerView;
@class NSRulerMarker;

@protocol NSLayoutManagerDelegate;

NS_ASSUME_NONNULL_BEGIN

#if !TARGET_OS_IPHONE

typedef NS_ENUM(NSInteger, NSTextLayoutOrientation) {
    NSTextLayoutOrientationHorizontal = 0, // Lines rendered horizontally, grow top to bottom
    NSTextLayoutOrientationVertical = 1, // Lines rendered vertically, grow right to left
} API_AVAILABLE(macos(10.0), ios(7.0), tvos(9.0));

typedef NS_OPTIONS(NSInteger, NSGlyphProperty) {
    NSGlyphPropertyNull = (1 << 0), // null glyph ignored for layout and display
    NSGlyphPropertyControlCharacter = (1 << 1), // control character such as tab, attachment, etc that has special behavior associated with
    NSGlyphPropertyElastic = (1 << 2), // glyphs with elastic glyph width behavior such as white spaces
    NSGlyphPropertyNonBaseCharacter = (1 << 3) // glyphs with combining properties. typically characters in Unicode Mn class.
} API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

typedef NS_OPTIONS(NSInteger, NSControlCharacterAction) {
    NSControlCharacterActionZeroAdvancement = (1 << 0), // glyphs with this action are filtered out from layout (notShownAttribute == YES)
    NSControlCharacterActionWhitespace = (1 << 1), // the width for glyphs with this action are determined by -layoutManager:boundingBoxForControlGlyphAtIndex:forTextContainer:proposedLineFragment:glyphPosition:characterIndex: if the method is implemented; otherwise, same as NSControlCharacterZeroAdvancementAction
    NSControlCharacterActionHorizontalTab = (1 << 2), // Treated as tab character
    NSControlCharacterActionLineBreak = (1 << 3), // Causes line break
    NSControlCharacterActionParagraphBreak = (1 << 4), // Causes paragraph break; firstLineIndent will be used for the following glyph
    NSControlCharacterActionContainerBreak = (1 << 5) // Causes container break
} API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

@protocol NSTextLayoutOrientationProvider
// A property describing the receiver's layout orientation.  This property defines the default value for the range of string laid out in the receiver in absence of explicit NSVerticalGlyphFormAttributeName attribute.  For example, when NSTextLayoutOrientationVertical, the default value for NSVerticalGlyphFormAttributeName is 1.  When rendering into the receiver, the Text System assumes the coordinate system is appropriately rotated.
@property (readonly) NSTextLayoutOrientation layoutOrientation API_AVAILABLE(macos(10.7), ios(7.0), tvos(9.0));
@end

typedef NS_ENUM(NSInteger, NSTypesetterBehavior) {
    NSTypesetterLatestBehavior                  = -1,
    NSTypesetterOriginalBehavior                = 0,    // Mac OS X versions 10.0 and 10.1 (uses NSSimpleHorizontalTypesetter)
    NSTypesetterBehavior_10_2_WithCompatibility = 1,    // 10.2 with backward compatibility layout (uses new ATS-based typesetter)
    NSTypesetterBehavior_10_2                   = 2,
    NSTypesetterBehavior_10_3                   = 3,
    NSTypesetterBehavior_10_4                   = 4
};

API_AVAILABLE(macos(10.7), ios(7.0), tvos(9.0)) @interface NSLayoutManager : NSObject <NSSecureCoding>

/**************************** Initialization ****************************/

// Designated Initializer.  Sets up this instance.  The NSLayoutManager starts off without an NSTextStorage.
- (instancetype)init NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;


/*************************** Text storage ***************************/

// Accessor for the NSTextStorage object owning the receiver.
// Avoid assigning a text storage directly through this property.  Adding a layout manager to a text storage through -[NSTextStorage addLayoutManager:] will use the property for assigning the new text storage.
@property (nullable, assign) NSTextStorage *textStorage;

// This method should be used instead of the primitive -setTextStorage: if you need to replace a NSLayoutManager's NSTextStorage with a new one, leaving all related objects intact.  This method deals with all the work of making sure the NSLayoutManager doesn't get deallocated and transferring all the NSLayoutManagers on the old NSTextStorage to the new one.
- (void)replaceTextStorage:(NSTextStorage *)newTextStorage;


/**************************** Text containers ****************************/

// NSTextContainer objects owner by the receiver.
@property (readonly) NSArray<NSTextContainer *> *textContainers;

// Add a container to the end of the array.  Must invalidate layout of all glyphs after the previous last container (i.e., glyphs that were not previously laid out because they would not fit anywhere).
- (void)addTextContainer:(NSTextContainer *)container;

// Insert a container into the array before the container at index.  Must invalidate layout of all glyphs in the containers from the one previously at index to the last container.
- (void)insertTextContainer:(NSTextContainer *)container atIndex:(NSUInteger)index;

// Removes the container at index from the array.  Must invalidate layout of all glyphs in the container being removed and any containers which come after it.
- (void)removeTextContainerAtIndex:(NSUInteger)index;

// Called by NSTextContainer whenever it changes size or shape.  Invalidates layout of all glyphs in container and all subsequent containers.
- (void)textContainerChangedGeometry:(NSTextContainer *)container;

// Called by NSTextContainer whenever its textView changes.  Used to keep notifications in sync.
- (void)textContainerChangedTextView:(NSTextContainer *)container;


/**************************** Delegate ****************************/

@property (nullable, weak) id <NSLayoutManagerDelegate> delegate;


/*********************** Global layout manager options ***********************/

// If YES, then whitespace and other "invisible" characters will be shown with special glyphs or other drawing.  The default is NO.
@property BOOL showsInvisibleCharacters;

// If YES, then control characters will be rendered visibly (usually like "^M").  The default is NO.
@property BOOL showsControlCharacters;

// When YES, NSLayoutManager will attempt to hyphenate when wrapping lines. May be overridden on a per-paragraph basis by the NSParagraphStyle's hyphenationFactor. The receiver makes the best effort to decide the exact logic including the hyphenation factor based on the context. The default value is NO. Can be overridden by the preference key @"NSUsesDefaultHyphenation".
@property BOOL usesDefaultHyphenation API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0));

// By default, a layout manager will use leading as specified by the font.  However, this is not appropriate for most UI text, for which a fixed leading is usually specified by UI layout guidelines.  These methods allow the use of the font's leading to be turned off.
@property BOOL usesFontLeading;

// If YES, then the layout manager may perform glyph generation and layout for a given portion of the text, without having glyphs or layout for preceding portions.  The default is NO.  Turning this setting on will significantly alter which portions of the text will have glyph generation or layout performed when a given generation-causing method is invoked.  It also gives significant performance benefits, especially for large documents.
@property BOOL allowsNonContiguousLayout API_AVAILABLE(macos(10.5), ios(7.0), tvos(9.0));

// Even if non-contiguous layout is allowed, it may not always be used, and there may not always be layout holes.  This method returns YES if there might currently be non-contiguous portions of the text laid out.
@property (readonly) BOOL hasNonContiguousLayout API_AVAILABLE(macos(10.5), ios(7.0), tvos(9.0));

// When YES, enables internal security analysis for malicious inputs and activates defensive behaviors. By enabling this functionality, it's possible certain text such as a very long paragraph might result in unexpected layout. NO by default.
@property BOOL limitsLayoutForSuspiciousContents API_AVAILABLE(macos(10.14), ios(12.0), tvos(12.0));

// These methods allow you to set/query whether the NSLayoutManager will lay out text in the background, i.e. on the main thread when it is idle.  The default is YES, but this should be set to NO whenever the layout manager is being accessed from other threads.
@property BOOL backgroundLayoutEnabled;


// Specifies the default behavior desired if an attachment image is too large to fit in a text container.  Note that attachment cells control their own size and drawing, so this setting can only be advisory for them, but kit-supplied attachment cells will respect it.  The default is NSImageScaleNone, meaning that images will clip rather than scaling.
@property NSImageScaling defaultAttachmentScaling;


/*********************** Typesetter ***********************/
// By default an NSLayoutManager uses the shared default typesetter.  Setting the typesetter invalidates all glyphs in the NSLayoutManager.  It can't just invalidate layout because the typesetter may have contributed to the actual glyphs as well (e.g. hyphenation).
@property (strong) __kindof NSTypesetter *typesetter;

// Specifies the typesetter behavior (compatibility setting) value for the layout manager.  The default is determined by the version of AppKit against which the application is linked.
@property NSTypesetterBehavior typesetterBehavior;


/************************** Invalidation **************************/

// This removes all glyphs for the old character range, adjusts the character indices of all the subsequent glyphs by the change in length, and invalidates the new character range.  If actualCharRange is non-NULL it will be set to the actual range invalidated after any necessary expansion.
- (void)invalidateGlyphsForCharacterRange:(NSRange)charRange changeInLength:(NSInteger)delta actualCharacterRange:(nullable NSRangePointer)actualCharRange;

// This method invalidates the layout information for the given range of characters.  If actualCharRange is non-NULL it will be set to the actual range invalidated after any necessary expansion.
- (void)invalidateLayoutForCharacterRange:(NSRange)charRange actualCharacterRange:(nullable NSRangePointer)actualCharRange API_AVAILABLE(macos(10.5), ios(7.0), tvos(9.0));

// These methods invalidate display for the glyph or character range given.  For the character range variant, unlaid parts of the range are remembered and will be redisplayed at some point later when the layout is available.  For the glyph range variant any part of the range that does not yet have glyphs generated is ignored.  Neither method actually causes layout.
- (void)invalidateDisplayForCharacterRange:(NSRange)charRange;
- (void)invalidateDisplayForGlyphRange:(NSRange)glyphRange;

// Sent from processEditing in NSTextStorage.  The newCharRange is the range in the final string which was explicitly edited.  The invalidatedRange includes portions that changed as a result of attribute fixing. invalidatedRange is either equal to newCharRange or larger.  Layout managers should not change the contents of the text storage during the execution of this message.
- (void)processEditingForTextStorage:(NSTextStorage *)textStorage edited:(NSTextStorageEditActions)editMask range:(NSRange)newCharRange changeInLength:(NSInteger)delta invalidatedRange:(NSRange)invalidatedCharRange API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));


/************************ Causing glyph generation and layout ************************/

// These methods allow clients to specify exactly the portions of the document for which they wish to have glyphs or layout.  This is particularly important if non-contiguous layout is enabled.  The layout manager still reserves the right to perform glyph generation or layout for larger ranges.  If non-contiguous layout is not enabled, then the range in question will always effectively be extended to start at the beginning of the text.
- (void)ensureGlyphsForCharacterRange:(NSRange)charRange;
- (void)ensureGlyphsForGlyphRange:(NSRange)glyphRange;
- (void)ensureLayoutForCharacterRange:(NSRange)charRange;
- (void)ensureLayoutForGlyphRange:(NSRange)glyphRange;
- (void)ensureLayoutForTextContainer:(NSTextContainer *)container;
- (void)ensureLayoutForBoundingRect:(NSRect)bounds inTextContainer:(NSTextContainer *)container;


/************************ Set glyphs and glyph properties ************************/

// Sets the initial glyphs and properties for a character range.  This method is invoked mainly from the glyph generation process.  Only place apps are allowed to directly call this method is from -layoutManager:shouldGenerateGlyphs:properties:characterIndexes:forGlyphRange:.  Each array has glyphRange.length items.  charIndexes must be contiguous (no skipped indexes).  It allows multiple items to have a same character index (a character index generating multiple glyph IDs).  Due to the font substitution, aFont passed into this method might not match the font in the attributes dictionary.  Calling this method for a range with the layout information invalidates the layout and display.
- (void)setGlyphs:(const CGGlyph *)glyphs properties:(const NSGlyphProperty *)props characterIndexes:(const NSUInteger *)charIndexes font:(NSFont *)aFont forGlyphRange:(NSRange)glyphRange API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));


/************************ Get glyphs and glyph properties ************************/

// Returns the total number of glyphs.  If non-contiguous layout is not enabled, this will force generation of glyphs for all characters.
@property (readonly) NSUInteger numberOfGlyphs;

// If non-contiguous layout is not enabled, these will cause generation of all glyphs up to and including glyphIndex.  The first CGGlyphAtIndex variant returns kCGFontIndexInvalid if the requested index is out of the range (0, numberOfGlyphs), and optionally returns a flag indicating whether the requested index is in range.  The second CGGlyphAtIndex variant raises a NSRangeError if the requested index is out of range.
- (CGGlyph)CGGlyphAtIndex:(NSUInteger)glyphIndex isValidIndex:(nullable BOOL *)isValidIndex API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));
- (CGGlyph)CGGlyphAtIndex:(NSUInteger)glyphIndex API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));
- (BOOL)isValidGlyphIndex:(NSUInteger)glyphIndex API_AVAILABLE(macos(10.0), ios(7.0), tvos(9.0));

// If non-contiguous layout is not enabled, this will cause generation of all glyphs up to and including glyphIndex.  It will return the glyph property associated with the glyph at the specified index.
- (NSGlyphProperty)propertyForGlyphAtIndex:(NSUInteger)glyphIndex API_AVAILABLE(macos(10.5), ios(7.0), tvos(9.0));

// If non-contiguous layout is not enabled, this will cause generation of all glyphs up to and including glyphIndex.  It will return the character index for the first character associated with the glyph at the specified index.
- (NSUInteger)characterIndexForGlyphAtIndex:(NSUInteger)glyphIndex;

// If non-contiguous layout is not enabled, this will cause generation of all glyphs up to and including those associated with the specified character.  It will return the glyph index for the first glyph associated with the character at the specified index.
- (NSUInteger)glyphIndexForCharacterAtIndex:(NSUInteger)charIndex;

// Fills a passed-in buffer with a sequence of CGGlyphs.  They will also optionally fill other passed-in buffers with the glyph properties, character indexes, and bidi levels corresponding to these glyphs.  Each pointer passed in should either be NULL, or else point to sufficient memory to hold glyphRange.length elements.  These methods return the number of glyphs filled in.
- (NSUInteger)getGlyphsInRange:(NSRange)glyphRange glyphs:(nullable CGGlyph *)glyphBuffer properties:(nullable NSGlyphProperty *)props characterIndexes:(nullable NSUInteger *)charIndexBuffer bidiLevels:(nullable unsigned char *)bidiLevelBuffer API_AVAILABLE(macos(10.5), ios(7.0), tvos(9.0));


/************************ Set layout information ************************/

// Associates the given container with the given range of glyphs.  This method should be called by the typesetter first, before setting line fragment rect or any of the layout bits, for each range of glyphs it lays out.  This method will reset several key layout attributes (like not shown and draws outside line fragment) to their default values.  In the course of layout, all glyphs should end up being included in a range passed to this method.  The range passed in is not expected to be the entire range of glyphs for that text container; usually, in fact, it will be the range for a given line fragment being laid out in that container.
- (void)setTextContainer:(NSTextContainer *)container forGlyphRange:(NSRange)glyphRange;

// Associates the given line fragment bounds with the given range of glyphs.  The typesetter should call this method second, after setting the line fragment rect and before setting the location or any of the layout bits.  In the course of layout, all glyphs should end up being included in a range passed to this method, but only glyphs which start a new line fragment should be at the start of such ranges.  Line fragment rects and line fragment used rects are always in container coordinates.
- (void)setLineFragmentRect:(NSRect)fragmentRect forGlyphRange:(NSRange)glyphRange usedRect:(NSRect)usedRect;

// Sets the bounds and container for the extra line fragment.  The extra line fragment is used when the text backing ends with a hard line break or when the text backing is totally empty, to define the extra line which needs to be displayed at the end of the text.  Line fragment rects and line fragment used rects are always in container coordinates.  This method should be invoked only when there is a non-empty extra line fragment.
- (void)setExtraLineFragmentRect:(NSRect)fragmentRect usedRect:(NSRect)usedRect textContainer:(NSTextContainer *)container;

// Sets the location for the first glyph of the given range.  Setting the location for a glyph range implies that its first glyph is not nominally spaced with respect to the previous glyph.  In the course of layout, all glyphs should end up being included in a range passed to this method, but only glyphs which start a new nominal range should be at the start of such ranges.  The first glyph in a line fragment should always start a new nominal range.  Glyph locations are given relative to their line fragment rect's origin.
- (void)setLocation:(NSPoint)location forStartOfGlyphRange:(NSRange)glyphRange;

// Some glyphs are not shown.  The typesetter decides which ones and sets this attribute in the layout manager to ensure that those glyphs will not be displayed.
- (void)setNotShownAttribute:(BOOL)flag forGlyphAtIndex:(NSUInteger)glyphIndex;

// Used to indicate that a particular glyph for some reason will draw outside of its line fragment rect.  This can commonly happen if a fixed line height is used (consider a 12 point line height and a 24 point glyph).  This information is important for determining whether additional lines need to be redrawn as a result of changes to any given line fragment.
- (void)setDrawsOutsideLineFragment:(BOOL)flag forGlyphAtIndex:(NSUInteger)glyphIndex;

// For a glyph corresponding to an attachment, this method should be called to set the size the attachment cell will occupy.
- (void)setAttachmentSize:(NSSize)attachmentSize forGlyphRange:(NSRange)glyphRange;


/************************ Get layout information ************************/

// Returns (by reference for the "get" method) the character index or glyph index or both of the first unlaid character/glyph in the layout manager at this time.
- (void)getFirstUnlaidCharacterIndex:(nullable NSUInteger *)charIndex glyphIndex:(nullable NSUInteger *)glyphIndex;
- (NSUInteger)firstUnlaidCharacterIndex;
- (NSUInteger)firstUnlaidGlyphIndex;

// Returns the container in which the given glyph is laid and (optionally) by reference the whole range of glyphs that are in that container.  This will cause glyph generation and layout for the line fragment containing the specified glyph, or if non-contiguous layout is not enabled, up to and including that line fragment; if non-contiguous layout is not enabled and effectiveGlyphRange is non-NULL, this will additionally cause glyph generation and layout for the entire text container containing the specified glyph.
- (nullable NSTextContainer *)textContainerForGlyphAtIndex:(NSUInteger)glyphIndex effectiveRange:(nullable NSRangePointer)effectiveGlyphRange;
- (nullable NSTextContainer *)textContainerForGlyphAtIndex:(NSUInteger)glyphIndex effectiveRange:(nullable NSRangePointer)effectiveGlyphRange withoutAdditionalLayout:(BOOL)flag API_AVAILABLE(macos(10.0), ios(9.0), tvos(9.0));


// Returns the container's currently used area.  This determines the size that the view would need to be in order to display all the glyphs that are currently laid into the container.  This causes neither glyph generation nor layout.  Used rects are always in container coordinates.
- (NSRect)usedRectForTextContainer:(NSTextContainer *)container;

// Returns the rect for the line fragment in which the given glyph is laid and (optionally) by reference the whole range of glyphs that are in that fragment.  This will cause glyph generation and layout for the line fragment containing the specified glyph, or if non-contiguous layout is not enabled, up to and including that line fragment.  Line fragment rects are always in container coordinates.
- (NSRect)lineFragmentRectForGlyphAtIndex:(NSUInteger)glyphIndex effectiveRange:(nullable NSRangePointer)effectiveGlyphRange;
- (NSRect)lineFragmentRectForGlyphAtIndex:(NSUInteger)glyphIndex effectiveRange:(nullable NSRangePointer)effectiveGlyphRange withoutAdditionalLayout:(BOOL)flag API_AVAILABLE(macos(10.0), ios(9.0), tvos(9.0));

// Returns the usage rect for the line fragment in which the given glyph is laid and (optionally) by reference the whole range of glyphs that are in that fragment.  This will cause glyph generation and layout for the line fragment containing the specified glyph, or if non-contiguous layout is not enabled, up to and including that line fragment.  Line fragment used rects are always in container coordinates.
- (NSRect)lineFragmentUsedRectForGlyphAtIndex:(NSUInteger)glyphIndex effectiveRange:(nullable NSRangePointer)effectiveGlyphRange;
- (NSRect)lineFragmentUsedRectForGlyphAtIndex:(NSUInteger)glyphIndex effectiveRange:(nullable NSRangePointer)effectiveGlyphRange withoutAdditionalLayout:(BOOL)flag API_AVAILABLE(macos(10.0), ios(9.0), tvos(9.0));

// Return info about the extra line fragment.  The extra line fragment is used for displaying the line at the end of document when the last character in the document causes a line or paragraph break.  Since the extra line is not associated with any glyph inside the layout manager, the information is handed separately from other line fragment rects.  Typically the extra line fragment is placed in the last document content text container along with other normal line fragment rects.  Line fragment rects and line fragment used rects are always in container coordinates.
@property (readonly) NSRect extraLineFragmentRect;
@property (readonly) NSRect extraLineFragmentUsedRect;
@property (nullable, readonly) NSTextContainer *extraLineFragmentTextContainer;


// Returns the location for the given glyph within its line fragment.  If this glyph does not have an explicit location set for it (i.e., it is part of (but not first in) a sequence of nominally spaced characters), the location is calculated by glyph advancements from the location of the most recent preceding glyph with a location set.  Glyph locations are relative to their line fragment rect's origin.  This will cause glyph generation and layout for the line fragment containing the specified glyph, or if non-contiguous layout is not enabled, up to and including that line fragment.
- (NSPoint)locationForGlyphAtIndex:(NSUInteger)glyphIndex;

// Some glyphs are not shown.  This method returns whether the given glyph has been designated as not shown.  This will cause glyph generation and layout for the line fragment containing the specified glyph, or if non-contiguous layout is not enabled, up to and including that line fragment.
- (BOOL)notShownAttributeForGlyphAtIndex:(NSUInteger)glyphIndex;

// Returns whether the glyph will draw outside of its line fragment rect.  This will cause glyph generation and layout for the line fragment containing the specified glyph, or if non-contiguous layout is not enabled, up to and including that line fragment.
- (BOOL)drawsOutsideLineFragmentForGlyphAtIndex:(NSUInteger)glyphIndex;

// For a glyph corresponding to an attachment, this method returns the size the attachment cell will occupy.  Returns {-1,-1} if no attachment size has been set for the specified glyph.
- (NSSize)attachmentSizeForGlyphAtIndex:(NSUInteger)glyphIndex;


// Returns a range of truncated glyph range for a line fragment containing the specified index.  When there is no truncation for the line fragment, it returns {NSNotFound, 0}.
- (NSRange)truncatedGlyphRangeInLineFragmentForGlyphAtIndex:(NSUInteger)glyphIndex API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));


/************************ More sophisticated queries ************************/

// Returns the range of glyphs that are generated from the characters in the given charRange.  If actualCharRange is not NULL, it will return by reference the actual range of characters that fully define the glyph range returned.  This range may be identical to or slightly larger than the requested character range.  For example, if the text storage contains the characters "o" and (umlaut) and the glyph store contains the single precomposed glyph (o-umlaut), and if the character range given encloses only the first or second character, then actualCharRange will be set to enclose both characters.  If the length of charRange is zero, the resulting glyph range will be a zero-length range just after the glyph(s) corresponding to the preceding character, and actualCharRange will also be zero-length.  If non-contiguous layout is not enabled, this will force the generation of glyphs for all characters up to and including the end of the specified range.
- (NSRange)glyphRangeForCharacterRange:(NSRange)charRange actualCharacterRange:(nullable NSRangePointer)actualCharRange;

// Returns the range of characters that generated the glyphs in the given glyphRange.  If actualGlyphRange is not NULL, it will return by reference the full range of glyphs generated by the character range returned.  This range may be identical or slightly larger than the requested glyph range.  For example, if the text storage contains the character (o-umlaut) and the glyph store contains the two atomic glyphs "o" and (umlaut), and if the glyph range given encloses only the first or second glyph, then actualGlyphRange will be set to enclose both glyphs.  If the length of glyphRange is zero, the resulting character range will be a zero-length range just after the character(s) corresponding to the preceding glyph, and actualGlyphRange will also be zero-length.  If non-contiguous layout is not enabled, this will force the generation of glyphs for all characters up to and including the end of the returned range.
- (NSRange)characterRangeForGlyphRange:(NSRange)glyphRange actualGlyphRange:(nullable NSRangePointer)actualGlyphRange;

// Returns the range of characters which have been laid into the given container.  This is a less efficient method than the similar -textContainerForGlyphAtIndex:effectiveRange:.
- (NSRange)glyphRangeForTextContainer:(NSTextContainer *)container;

// Returns the range including the first glyph from glyphIndex on back that has a location set and up to, but not including the next glyph that has a location set.
- (NSRange)rangeOfNominallySpacedGlyphsContainingIndex:(NSUInteger)glyphIndex;

// Returns the smallest bounding rect which completely encloses the glyphs in the given glyphRange that are in the given container.  The range is intersected with the container's range before computing the bounding rect.  This method can be used to translate glyph ranges into display rectangles for invalidation.  Bounding rects are always in container coordinates.
- (NSRect)boundingRectForGlyphRange:(NSRange)glyphRange inTextContainer:(NSTextContainer *)container;

// Returns a contiguous glyph range containing all glyphs that would need to be displayed in order to draw all glyphs that fall (even partially) within the bounding rect given.  This range might include glyphs which do not fall into the rect at all.  At most this will return the glyph range for the whole container.  The WithoutAdditionalLayout variant will not generate glyphs or perform layout in attempting to answer, and thus may not be entirely correct.  Bounding rects are always in container coordinates.
- (NSRange)glyphRangeForBoundingRect:(NSRect)bounds inTextContainer:(NSTextContainer *)container;
- (NSRange)glyphRangeForBoundingRectWithoutAdditionalLayout:(NSRect)bounds inTextContainer:(NSTextContainer *)container;

// Returns the index of the glyph falling under the given point, expressed in the given container's coordinate system.  If no glyph is under the point, the nearest glyph is returned, where nearest is defined according to the requirements of selection by touch or mouse.  Clients who wish to determine whether the the point actually lies within the bounds of the glyph returned should follow this with a call to boundingRectForGlyphRange:inTextContainer: and test whether the point falls in the rect returned by that method.  If partialFraction is non-NULL, it will return by reference the fraction of the distance between the location of the glyph returned and the location of the next glyph. 
- (NSUInteger)glyphIndexForPoint:(NSPoint)point inTextContainer:(NSTextContainer *)container fractionOfDistanceThroughGlyph:(nullable CGFloat *)partialFraction;
- (NSUInteger)glyphIndexForPoint:(NSPoint)point inTextContainer:(NSTextContainer *)container;
- (CGFloat)fractionOfDistanceThroughGlyphForPoint:(NSPoint)point inTextContainer:(NSTextContainer *)container;

// Returns the index of the character falling under the given point, expressed in the given container's coordinate system.  If no character is under the point, the nearest character is returned, where nearest is defined according to the requirements of selection by touch or mouse.  This is not simply equivalent to taking the result of the corresponding glyph index method and converting it to a character index, because in some cases a single glyph represents more than one selectable character, for example an fi ligature glyph.  In that case, there will be an insertion point within the glyph, and this method will return one character or the other, depending on whether the specified point lies to the left or the right of that insertion point.  In general, this method will return only character indexes for which there is an insertion point (see next method).  The partial fraction is a fraction of the distance from the insertion point logically before the given character to the next one, which may be either to the right or to the left depending on directionality.
- (NSUInteger)characterIndexForPoint:(NSPoint)point inTextContainer:(NSTextContainer *)container fractionOfDistanceBetweenInsertionPoints:(nullable CGFloat *)partialFraction;

// Allows clients to obtain all insertion points for a line fragment in one call.  The caller specifies the line fragment by supplying one character index within it, and can choose whether to obtain primary or alternate insertion points, and whether they should be in logical or in display order.  The return value is the number of insertion points returned.  Each pointer passed in should either be NULL, or else point to sufficient memory to hold as many elements as there are insertion points in the line fragment (which cannot be more than the number of characters + 1).  The positions buffer passed in will be filled in with the positions of the insertion points, in the order specified, and the charIndexes buffer passed in will be filled in with the corresponding character indexes.  Positions indicate a transverse offset relative to the line fragment rect's origin.  Internal caching is used to ensure that repeated calls to this method for the same line fragment (possibly with differing values for other arguments) will not be significantly more expensive than a single call.
- (NSUInteger)getLineFragmentInsertionPointsForCharacterAtIndex:(NSUInteger)charIndex alternatePositions:(BOOL)aFlag inDisplayOrder:(BOOL)dFlag positions:(nullable CGFloat *)positions characterIndexes:(nullable NSUInteger *)charIndexes;

// Enumerates line fragments intersecting with glyphRange.
- (void)enumerateLineFragmentsForGlyphRange:(NSRange)glyphRange usingBlock:(void (^)(NSRect rect, NSRect usedRect, NSTextContainer *textContainer, NSRange glyphRange, BOOL *stop))block API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Enumerates enclosing rects for glyphRange in textContainer.  If a selected range is given in the second argument, the rectangles returned will be correct for drawing the selection.  Selection rectangles are generally more complicated than enclosing rectangles and supplying a selected range is the clue these methods use to determine whether to go to the trouble of doing this special work.  If the caller is interested in this more from an enclosing point of view rather than a selection point of view, pass {NSNotFound, 0} as the selected range.  This method will do the minimum amount of work required to answer the question.
- (void)enumerateEnclosingRectsForGlyphRange:(NSRange)glyphRange withinSelectedGlyphRange:(NSRange)selectedRange inTextContainer:(NSTextContainer *)textContainer usingBlock:(void (^)(NSRect rect, BOOL *stop))block API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));


/************************ Drawing support ************************/
// These methods are primitives for drawing.  You can override these to perform additional drawing, or to replace text drawing entirely, but not to change layout.  You can call them if you want, but focus must already be locked on the destination view or image.  -drawBackgroundForGlyphRange:atPoint: draws the background color and selection and marked range aspects of the text display, along with block decoration such as table backgrounds and borders.  -drawGlyphsForGlyphRange:atPoint: draws the actual glyphs, including attachments, as well as any underlines or strikethroughs.  In either case all of the specified glyphs must lie in a single container.
- (void)drawBackgroundForGlyphRange:(NSRange)glyphsToShow atPoint:(NSPoint)origin;
- (void)drawGlyphsForGlyphRange:(NSRange)glyphsToShow atPoint:(NSPoint)origin;

// This is the glyph rendering primitive method.  Renders glyphs at positions into the CGContext.  The positions are in the user space coordinate system.  CGContext that is passed in is already configured according to the text attributes arguments: font, textMatrix, and attributes.  The font argument represents the font applied to the graphics state.  The value can be different from the NSFontAttributeName value in the attributes argument because of various font substitutions that the system automatically executes.  The textMatrix is the affine transform mapping the text space coordinate system to the user space coordinate system.  The tx and ty components of textMatrix are ignored since Quartz overrides them with the glyph positions.
- (void)showCGGlyphs:(const CGGlyph *)glyphs positions:(const CGPoint *)positions count:(NSInteger)glyphCount font:(NSFont *)font textMatrix:(CGAffineTransform)textMatrix attributes:(NSDictionary<NSAttributedStringKey, id> *)attributes inContext:(CGContextRef)CGContext API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0));

// This is the primitive used by -drawBackgroundForGlyphRange:atPoint: for actually filling rects with a particular background color, whether due to a background color attribute, a selected or marked range highlight, a block decoration, or any other rect fill needed by that method.  As with -showCGGlyphs:..., the character range and color are merely for informational purposes; the color will already be set in the graphics state.  If for any reason you modify it, you must restore it before returning from this method.  You should never call this method, but you might override it.  The default implementation will simply fill the specified rect array.
- (void)fillBackgroundRectArray:(const NSRect *)rectArray count:(NSUInteger)rectCount forCharacterRange:(NSRange)charRange color:(NSColor *)color API_AVAILABLE(macos(10.6));

// The first of these methods actually draws an appropriate underline for the glyph range given.  The second method potentially breaks the range it is given up into subranges and calls drawUnderline... for ranges that should actually have the underline drawn.  As examples of why there are two methods, consider two situations.  First, in all cases you don't want to underline the leading and trailing whitespace on a line.  The -underlineGlyphRange... method is passed glyph ranges that have underlining turned on, but it will then look for this leading and trailing white space and only pass the ranges that should actually be underlined to -drawUnderline...  Second, if the underlineType: indicates that only words, (i.e., no whitespace), should be underlined, then -underlineGlyphRange... will carve the range it is passed up into words and only pass word ranges to -drawUnderline.
- (void)drawUnderlineForGlyphRange:(NSRange)glyphRange underlineType:(NSUnderlineStyle)underlineVal baselineOffset:(CGFloat)baselineOffset lineFragmentRect:(NSRect)lineRect lineFragmentGlyphRange:(NSRange)lineGlyphRange containerOrigin:(NSPoint)containerOrigin;
- (void)underlineGlyphRange:(NSRange)glyphRange underlineType:(NSUnderlineStyle)underlineVal lineFragmentRect:(NSRect)lineRect lineFragmentGlyphRange:(NSRange)lineGlyphRange containerOrigin:(NSPoint)containerOrigin;

// These two methods parallel the two corresponding underline methods, but draw strikethroughs instead of underlines.
- (void)drawStrikethroughForGlyphRange:(NSRange)glyphRange strikethroughType:(NSUnderlineStyle)strikethroughVal baselineOffset:(CGFloat)baselineOffset lineFragmentRect:(NSRect)lineRect lineFragmentGlyphRange:(NSRange)lineGlyphRange containerOrigin:(NSPoint)containerOrigin;
- (void)strikethroughGlyphRange:(NSRange)glyphRange strikethroughType:(NSUnderlineStyle)strikethroughVal lineFragmentRect:(NSRect)lineRect lineFragmentGlyphRange:(NSRange)lineGlyphRange containerOrigin:(NSPoint)containerOrigin;

// This is the primitive for actually drawing an attachment cell.  The attachment should be drawn within the given rect.  The character index is provided for those cells that alter their appearance based on their location.
- (void)showAttachmentCell:(NSCell *)cell inRect:(NSRect)rect characterIndex:(NSUInteger)attachmentIndex;


/************************** Block information **************************/
// Used to set and get information about layout of text blocks.  These methods cause glyph generation but not layout.  The latter two will return NSZeroRect if no rect has been set for the specified block and range since the last invalidation.  Block layout rects and bounds rects are always in container coordinates.
- (void)setLayoutRect:(NSRect)rect forTextBlock:(NSTextBlock *)block glyphRange:(NSRange)glyphRange;
- (void)setBoundsRect:(NSRect)rect forTextBlock:(NSTextBlock *)block glyphRange:(NSRange)glyphRange;
- (NSRect)layoutRectForTextBlock:(NSTextBlock *)block glyphRange:(NSRange)glyphRange;
- (NSRect)boundsRectForTextBlock:(NSTextBlock *)block glyphRange:(NSRange)glyphRange;

// Similar to the above, but they can be passed any index in the range and will return the range by reference if effectiveGlyphRange is not NULL.
- (NSRect)layoutRectForTextBlock:(NSTextBlock *)block atIndex:(NSUInteger)glyphIndex effectiveRange:(nullable NSRangePointer)effectiveGlyphRange;
- (NSRect)boundsRectForTextBlock:(NSTextBlock *)block atIndex:(NSUInteger)glyphIndex effectiveRange:(nullable NSRangePointer)effectiveGlyphRange;


/************************ Temporary attribute support ************************/
// Temporary attributes provide a way to override attributes for drawing on a per-layout manager basis, without affecting the underlying stored text.  Clients may set any attributes they wish, but the only attributes that the layout manager will recognize for drawing are those that do not affect layout (color, underline, etc.).
- (NSDictionary<NSAttributedStringKey, id> *)temporaryAttributesAtCharacterIndex:(NSUInteger)charIndex effectiveRange:(nullable NSRangePointer)effectiveCharRange;
- (void)setTemporaryAttributes:(NSDictionary<NSAttributedStringKey, id> *)attrs forCharacterRange:(NSRange)charRange;
- (void)addTemporaryAttributes:(NSDictionary<NSAttributedStringKey, id> *)attrs forCharacterRange:(NSRange)charRange;
- (void)removeTemporaryAttribute:(NSAttributedStringKey)attrName forCharacterRange:(NSRange)charRange;

- (nullable id)temporaryAttribute:(NSAttributedStringKey)attrName atCharacterIndex:(NSUInteger)location effectiveRange:(nullable NSRangePointer)range API_AVAILABLE(macos(10.5));
- (nullable id)temporaryAttribute:(NSAttributedStringKey)attrName atCharacterIndex:(NSUInteger)location longestEffectiveRange:(nullable NSRangePointer)range inRange:(NSRange)rangeLimit API_AVAILABLE(macos(10.5));
- (NSDictionary<NSAttributedStringKey, id> *)temporaryAttributesAtCharacterIndex:(NSUInteger)location longestEffectiveRange:(nullable NSRangePointer)range inRange:(NSRange)rangeLimit API_AVAILABLE(macos(10.5));
- (void)addTemporaryAttribute:(NSAttributedStringKey)attrName value:(id)value forCharacterRange:(NSRange)charRange API_AVAILABLE(macos(10.5));


/******************************* Font metrics ******************************/
// Returns the default line height specified by the layout manager's typesetter behavior for the given font.
- (CGFloat)defaultLineHeightForFont:(NSFont *)theFont;
// Returns the default baseline offset specified by the layout manager's typesetter behavior for the given font.
- (CGFloat)defaultBaselineOffsetForFont:(NSFont *)theFont;
@end

@interface NSLayoutManager (NSTextViewSupport)
/***************************** Ruler support *****************************/
// These return, respectively, an array of text ruler objects for the current selection, and the accessory view that the text system uses for its ruler.  If you have turned off automatic ruler updating through the use of setUsesRulers: so that you can do more complex things, but you still want to display the appropriate text ruler objects and/or accessory view, you can use these methods.
- (NSArray<NSRulerMarker *> *)rulerMarkersForTextView:(NSTextView *)view paragraphStyle:(NSParagraphStyle *)style ruler:(NSRulerView *)ruler;
- (nullable __kindof NSView *)rulerAccessoryViewForTextView:(NSTextView *)view paragraphStyle:(NSParagraphStyle *)style ruler:(NSRulerView *)ruler enabled:(BOOL)isEnabled;

/************************ First responder support ************************/
// Returns YES if the firstResponder of the given window is one of the NSTextViews attached to this NSLayoutManager.
- (BOOL)layoutManagerOwnsFirstResponderInWindow:(NSWindow *)window;

@property (nullable, readonly, assign) NSTextView *firstTextView;

// This method is special in that it won't cause layout if the beginning of the selected range is not yet laid out.  Other than that this method could be done through other API.
@property (nullable, readonly, assign) NSTextView *textViewForBeginningOfSelection;
@end

@protocol NSLayoutManagerDelegate <NSObject>
@optional

/************************ Glyph generation ************************/
// This is sent whenever layoutManager is about to store the initial glyph information via -setGlyphs:properties:characterIndexes:forGlyphRange:.  This method allows customizing the initial glyph generation process.  It can invoke -setGlyphs:properties:characterIndexes:forGlyphRange: with modified glyph information.  The return value specifies the actual glyph range stored in this method.  By returning 0, it can indicate layoutManager to do the default processing. Note that querying glyph information surrounding glyphRange could lead to recursion since the data might not be available, yet.
- (NSUInteger)layoutManager:(NSLayoutManager *)layoutManager shouldGenerateGlyphs:(const CGGlyph *)glyphs properties:(const NSGlyphProperty *)props characterIndexes:(const NSUInteger *)charIndexes font:(NSFont *)aFont forGlyphRange:(NSRange)glyphRange API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));


/************************ Line layout ************************/
// These methods are invoked while each line is laid out.  They allow NSLayoutManager delegate to customize the shape of line.

// Returns the spacing after the line ending with glyphIndex.
- (CGFloat)layoutManager:(NSLayoutManager *)layoutManager lineSpacingAfterGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Returns the paragraph spacing before the line starting with glyphIndex.
- (CGFloat)layoutManager:(NSLayoutManager *)layoutManager paragraphSpacingBeforeGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Returns the paragraph spacing after the line ending with glyphIndex.
- (CGFloat)layoutManager:(NSLayoutManager *)layoutManager paragraphSpacingAfterGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Returns the control character action for the control character at charIndex.
- (NSControlCharacterAction)layoutManager:(NSLayoutManager *)layoutManager shouldUseAction:(NSControlCharacterAction)action forControlCharacterAtIndex:(NSUInteger)charIndex API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Invoked while determining the soft line break point.  When NO, NSLayoutManager tries to find the next line break opportunity before charIndex
- (BOOL)layoutManager:(NSLayoutManager *)layoutManager shouldBreakLineByWordBeforeCharacterAtIndex:(NSUInteger)charIndex API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Invoked while determining the hyphenation point.  When NO, NSLayoutManager tries to find the next hyphenation opportunity before charIndex
- (BOOL)layoutManager:(NSLayoutManager *)layoutManager shouldBreakLineByHyphenatingBeforeCharacterAtIndex:(NSUInteger)charIndex API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Invoked for resolving the glyph metrics for NSControlCharacterWhitespaceAction control character.
- (NSRect)layoutManager:(NSLayoutManager *)layoutManager boundingBoxForControlGlyphAtIndex:(NSUInteger)glyphIndex forTextContainer:(NSTextContainer *)textContainer proposedLineFragment:(NSRect)proposedRect glyphPosition:(NSPoint)glyphPosition characterIndex:(NSUInteger)charIndex API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Allows NSLayoutManagerDelegate to customize the line fragment geometry before committing to the layout cache. The implementation of this method should make sure that the modified fragments are still valid inside the text container coordinate. When it returns YES, the layout manager uses the modified rects. Otherwise, it ignores the rects returned from this method.
- (BOOL)layoutManager:(NSLayoutManager *)layoutManager shouldSetLineFragmentRect:(inout NSRect *)lineFragmentRect lineFragmentUsedRect:(inout NSRect *)lineFragmentUsedRect baselineOffset:(inout CGFloat *)baselineOffset inTextContainer:(NSTextContainer *)textContainer forGlyphRange:(NSRange)glyphRange API_AVAILABLE(macos(10.11), ios(9.0), tvos(9.0));


/************************ Layout processing ************************/
// This is sent whenever layout or glyphs become invalidated in a layout manager which previously had all layout complete.
- (void)layoutManagerDidInvalidateLayout:(NSLayoutManager *)sender API_AVAILABLE(macos(10.0), ios(7.0), tvos(9.0));

// This is sent whenever a container has been filled.  This method can be useful for paginating.  The textContainer might be nil if we have completed all layout and not all of it fit into the existing containers.  The atEnd flag indicates whether all layout is complete.
- (void)layoutManager:(NSLayoutManager *)layoutManager didCompleteLayoutForTextContainer:(nullable NSTextContainer *)textContainer atEnd:(BOOL)layoutFinishedFlag API_AVAILABLE(macos(10.0), ios(7.0), tvos(9.0));

// This is sent right before layoutManager invalidates the layout due to textContainer changing geometry.  The receiver of this method can react to the geometry change and perform adjustments such as recreating the exclusion path.
- (void)layoutManager:(NSLayoutManager *)layoutManager textContainer:(NSTextContainer *)textContainer didChangeGeometryFromSize:(NSSize)oldSize API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// This is sent when the layout manager is drawing and needs to decide whether to use temporary attributes or not.  The delegate returns a dictionary of temporary attributes to be used, or nil to suppress the use of temporary attributes altogether.  The effectiveCharRange argument is both an in and out by-reference effective range for those attributes.  The default behavior if this method is not implemented is to use temporary attributes only when drawing to the screen, so an implementation to match that behavior would return attrs if toScreen is YES and nil otherwise, without changing effectiveCharRange.
- (nullable NSDictionary<NSAttributedStringKey, id> *)layoutManager:(NSLayoutManager *)layoutManager shouldUseTemporaryAttributes:(NSDictionary<NSAttributedStringKey, id> *)attrs forDrawingToScreen:(BOOL)toScreen atCharacterIndex:(NSUInteger)charIndex effectiveRange:(nullable NSRangePointer)effectiveCharRange API_AVAILABLE(macos(10.5));
@end


/************************ Deprecated ************************/
enum {
    NSGlyphAttributeSoft API_DEPRECATED("", macos(10.0,10.11)) = 0,
    NSGlyphAttributeElastic API_DEPRECATED("", macos(10.0,10.11)) = 1,
    NSGlyphAttributeBidiLevel API_DEPRECATED("", macos(10.0,10.11)) = 2,
    NSGlyphAttributeInscribe API_DEPRECATED("", macos(10.0,10.11)) = 5
} API_DEPRECATED("Use NSGlyphProperty instead", macos(10.0,10.11));

typedef NS_ENUM(NSUInteger, NSGlyphInscription) {
    NSGlyphInscribeBase API_DEPRECATED("", macos(10.0,10.11)) = 0,
    NSGlyphInscribeBelow API_DEPRECATED("", macos(10.0,10.11)) = 1,
    NSGlyphInscribeAbove API_DEPRECATED("", macos(10.0,10.11)) = 2,
    NSGlyphInscribeOverstrike API_DEPRECATED("", macos(10.0,10.11)) = 3,
    NSGlyphInscribeOverBelow API_DEPRECATED("", macos(10.0,10.11)) = 4
} API_DEPRECATED("Use NSGlyphProperty instead", macos(10.0,10.11));

@interface NSLayoutManager (NSLayoutManagerDeprecated)
// These two methods are soft deprecated starting with OS X 10.11. It will be officially deprecated in a future release
- (NSGlyph)glyphAtIndex:(NSUInteger)glyphIndex isValidIndex:(nullable BOOL *)isValidIndex; // Use -CGGlyphAtIndex:isValidIndex: instead
- (NSGlyph)glyphAtIndex:(NSUInteger)glyphIndex; // Use -CGGlyphAtIndex: instead

// These two methods are soft deprecated starting with OS X 10.11. It will be officially deprecated in a future release. Use -enumerateEnclosingRectsForGlyphRange:withinSelectedGlyphRange:inTextContainer:usingBlock: instead
- (nullable NSRectArray)rectArrayForCharacterRange:(NSRange)charRange withinSelectedCharacterRange:(NSRange)selCharRange inTextContainer:(NSTextContainer *)container rectCount:(NSUInteger *)rectCount NS_RETURNS_INNER_POINTER;
- (nullable NSRectArray)rectArrayForGlyphRange:(NSRange)glyphRange withinSelectedGlyphRange:(NSRange)selGlyphRange inTextContainer:(NSTextContainer *)container rectCount:(NSUInteger *)rectCount NS_RETURNS_INNER_POINTER;

@property BOOL usesScreenFonts API_DEPRECATED("", macos(10.0,10.11));
- (NSFont *)substituteFontForFont:(NSFont *)originalFont API_DEPRECATED("", macos(10.0,10.11));


- (void)insertGlyphs:(const NSGlyph *)glyphs length:(NSUInteger)length forStartingGlyphAtIndex:(NSUInteger)glyphIndex characterIndex:(NSUInteger)charIndex API_DEPRECATED("Use -setGlyphs:properties:characterIndexes:font:forGlyphRange instead", macos(10.0,10.11));
- (void)insertGlyph:(NSGlyph)glyph atGlyphIndex:(NSUInteger)glyphIndex characterIndex:(NSUInteger)charIndex API_DEPRECATED("Use -setGlyphs:properties:characterIndexes:font:forGlyphRange instead", macos(10.0,10.11));
- (void)replaceGlyphAtIndex:(NSUInteger)glyphIndex withGlyph:(NSGlyph)newGlyph API_DEPRECATED("Use -setGlyphs:properties:characterIndexes:font:forGlyphRange instead", macos(10.0,10.11));
- (void)deleteGlyphsInRange:(NSRange)glyphRange API_DEPRECATED("Use -setGlyphs:properties:characterIndexes:font:forGlyphRange instead", macos(10.0,10.11));
- (void)setCharacterIndex:(NSUInteger)charIndex forGlyphAtIndex:(NSUInteger)glyphIndex API_DEPRECATED("Use -setGlyphs:properties:characterIndexes:font:forGlyphRange instead", macos(10.0,10.11));
- (void)setIntAttribute:(NSInteger)attributeTag value:(NSInteger)val forGlyphAtIndex:(NSUInteger)glyphIndex API_DEPRECATED("Use -setGlyphs:properties:characterIndexes:font:forGlyphRange instead", macos(10.0,10.11));
- (void)invalidateGlyphsOnLayoutInvalidationForGlyphRange:(NSRange)glyphRange API_DEPRECATED("Use -setGlyphs:properties:characterIndexes:font:forGlyphRange instead", macos(10.5,10.11));

- (NSInteger)intAttribute:(NSInteger)attributeTag forGlyphAtIndex:(NSUInteger)glyphIndex API_DEPRECATED("Use -getGlyphsInRange:glyphs:properties:characterIndexes:bidiLevels: instead", macos(10.0,10.11));
- (NSUInteger)getGlyphsInRange:(NSRange)glyphRange glyphs:(nullable NSGlyph *)glyphBuffer characterIndexes:(nullable NSUInteger *)charIndexBuffer glyphInscriptions:(nullable NSGlyphInscription *)inscribeBuffer elasticBits:(nullable BOOL *)elasticBuffer API_DEPRECATED("Use -getGlyphsInRange:glyphs:properties:characterIndexes:bidiLevels: instead", macos(10.0,10.11));
- (NSUInteger)getGlyphsInRange:(NSRange)glyphRange glyphs:(nullable NSGlyph *)glyphBuffer characterIndexes:(nullable NSUInteger *)charIndexBuffer glyphInscriptions:(nullable NSGlyphInscription *)inscribeBuffer elasticBits:(nullable BOOL *)elasticBuffer bidiLevels:(nullable unsigned char *)bidiLevelBuffer API_DEPRECATED("Use -getGlyphsInRange:glyphs:properties:characterIndexes:bidiLevels: instead", macos(10.0,10.11));
- (NSUInteger)getGlyphs:(nullable NSGlyph *)glyphArray range:(NSRange)glyphRange API_DEPRECATED("Use -getGlyphsInRange:glyphs:properties:characterIndexes:bidiLevels: instead", macos(10.0,10.11));

- (void)invalidateLayoutForCharacterRange:(NSRange)charRange isSoft:(BOOL)flag actualCharacterRange:(nullable NSRangePointer)actualCharRange API_DEPRECATED("Use -invalidateLayoutForCharacterRange:actualCharacterRange: instead", macos(10.0,10.11));
- (void)textStorage:(NSTextStorage *)str edited:(NSTextStorageEditedOptions)editedMask range:(NSRange)newCharRange changeInLength:(NSInteger)delta invalidatedRange:(NSRange)invalidatedCharRange API_DEPRECATED("Use -processEditingForTextStorage:edited:range:changeInLength:invalidatedRange: instead", macos(10.0,10.11));

- (void)setLocations:(NSPointArray)locations startingGlyphIndexes:(NSUInteger *)glyphIndexes count:(NSUInteger)count forGlyphRange:(NSRange)glyphRange API_DEPRECATED("Use -setLocation:forStartOfGlyphRange: instead", macos(10.5,10.11));

- (void)showPackedGlyphs:(char *)glyphs length:(NSUInteger)glyphLen glyphRange:(NSRange)glyphRange atPoint:(NSPoint)point font:(NSFont *)font color:(NSColor *)color printingAdjustment:(NSSize)printingAdjustment API_DEPRECATED("Use -showCGGlyphs:positions:count:font:matrix:attributes:inContext: instead", macos(10.0,10.7));

- (void)showCGGlyphs:(const CGGlyph *)glyphs positions:(const NSPoint *)positions count:(NSUInteger)glyphCount font:(NSFont *)font matrix:(NSAffineTransform *)textMatrix attributes:(NSDictionary<NSAttributedStringKey, id> *)attributes inContext:(NSGraphicsContext *)graphicsContext API_DEPRECATED_WITH_REPLACEMENT("showCGGlyphs:positions:count:font:textMatrix:attributes:inContext:", macos(10.7,10.15), ios(7.0,13.0), tvos(9.0,13.0)) API_UNAVAILABLE(macCatalyst);

@property float hyphenationFactor API_DEPRECATED("Please use usesDefaultHyphenation or -[NSParagraphStyle hyphenationFactor] instead.", macos(10.0,10.15), ios(7.0,13.0), tvos(9.0,13.0)) API_UNAVAILABLE(macCatalyst);
@end

// NSGlyph-based API is now deprecated. Use -layoutManager:shouldGenerateGlyphs:properties:characterIndexes:forGlyphRange: instead
@interface NSLayoutManager (NSGlyphGeneration) <NSGlyphStorage>
@property (strong) NSGlyphGenerator *glyphGenerator;
@end



#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSLayoutManager.h>
#endif

