#if !__has_include(<UIFoundation/NSGlyphGenerator.h>)
/*
        NSGlyphGenerator.h
        Application Kit
        Copyright (c) 1993-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSAttributedString.h>
#import <AppKit/NSFont.h> // for NSGlyph

// NSGlyphGenerator is now deprecated. It will be formally deprecated in a future version of macOS. Use -layoutManager:shouldGenerateGlyphs:properties:characterIndexes:forGlyphRange: in order to customize the canonical glyph generation mapping

/* NSGlyphStorage layout options */
NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

enum {
    NSShowControlGlyphs = (1 << 0), // if set, generates displayable glyphs for control chars
    NSShowInvisibleGlyphs = (1 << 1), // if set, generates displayable glyphs for invisible chars
    NSWantsBidiLevels = (1 << 2) // if set, generates bidi levels
};

@protocol NSGlyphStorage
- (void)insertGlyphs:(const NSGlyph *)glyphs length:(NSUInteger)length forStartingGlyphAtIndex:(NSUInteger)glyphIndex characterIndex:(NSUInteger)charIndex;

// sets glyph attribute defined in NSLayoutManager.h
- (void)setIntAttribute:(NSInteger)attributeTag value:(NSInteger)val forGlyphAtIndex:(NSUInteger)glyphIndex;

- (NSAttributedString *)attributedString;
- (NSUInteger)layoutOptions;
@end

@interface NSGlyphGenerator : NSObject
- (void)generateGlyphsForGlyphStorage:(id <NSGlyphStorage>)glyphStorage desiredNumberOfCharacters:(NSUInteger)nChars glyphIndex:(nullable NSUInteger *)glyphIndex characterIndex:(nullable NSUInteger *)charIndex;

@property (class, readonly, strong) NSGlyphGenerator *sharedGlyphGenerator;
@end

#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSGlyphGenerator.h>
#endif
