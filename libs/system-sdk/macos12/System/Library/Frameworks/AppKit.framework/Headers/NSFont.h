/*
	NSFont.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h> // for NSStringEncoding
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSCell.h> // for NSControlSize
#import <AppKit/NSFontDescriptor.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSFontDescriptor, NSAffineTransform, NSGraphicsContext;

/********* Font Matrix *********/
/* This is a font matrix value representing [1 0 0 1 0 0].
*/
APPKIT_EXTERN const CGFloat * NSFontIdentityMatrix;

NS_AUTOMATED_REFCOUNT_WEAK_UNAVAILABLE
@interface NSFont : NSObject <NSCopying, NSSecureCoding>

/********* Factory *********/
+ (nullable NSFont *)fontWithName:(NSString *)fontName size:(CGFloat)fontSize;
+ (nullable NSFont *)fontWithName:(NSString *)fontName matrix:(const CGFloat *)fontMatrix;
/* Instantiates an NSFont object matching fontDescriptor. If fontSize is greater than 0.0, it has precedence over NSFontSizeAttribute in fontDescriptor.
*/
+ (nullable NSFont *)fontWithDescriptor:(NSFontDescriptor *)fontDescriptor size:(CGFloat)fontSize;

/* Instantiates an NSFont object matching fontDescriptor. If textTransform is non-nil, it has precedence over NSFontMatrixAttribute in fontDescriptor.
*/
+ (nullable NSFont *)fontWithDescriptor:(NSFontDescriptor *)fontDescriptor textTransform:(nullable NSAffineTransform *)textTransform;

/********* Meta Font *********/
/* User font settings
*/
+ (nullable NSFont *)userFontOfSize:(CGFloat)fontSize;	// Application font
+ (nullable NSFont *)userFixedPitchFontOfSize:(CGFloat)fontSize; // fixed-pitch font
+ (void)setUserFont:(nullable NSFont *)font;	// set preference for Application font.
+ (void)setUserFixedPitchFont:(nullable NSFont *)font; // set preference for fixed-pitch.

/* UI font settings
*/
+ (NSFont *)systemFontOfSize:(CGFloat)fontSize;	// System font with NSFontWeightRegular and proportional digits
+ (NSFont *)boldSystemFontOfSize:(CGFloat)fontSize; // System font with NSFontWeightBold and proportional digits
+ (NSFont *)labelFontOfSize:(CGFloat)fontSize; // Label font

+ (NSFont *)titleBarFontOfSize:(CGFloat)fontSize;
+ (NSFont *)menuFontOfSize:(CGFloat)fontSize;
+ (NSFont *)menuBarFontOfSize:(CGFloat)fontSize;
+ (NSFont *)messageFontOfSize:(CGFloat)fontSize;
+ (NSFont *)paletteFontOfSize:(CGFloat)fontSize;
+ (NSFont *)toolTipsFontOfSize:(CGFloat)fontSize;
+ (NSFont *)controlContentFontOfSize:(CGFloat)fontSize;

/* Following two factory methods return system font with NSFontWeightTrait. Returns the system font object corresponding to fontSize and weight. +monospacedDigitSystemFontOfSize:weight: always return a system font instance with monospaced digit glyphs. It's recommended to use the symbolic weight values declared in NSFontDescriptor.h. Due to sophisticated system font weight matching logic underneath, it's recommended to use this factory method all the time when getting non-standard weight system fonts instead of transforming existing instances via methods like -[NSFontManager convertFont:toSize]. When asked for a missing weight for the running version of OS X, this method returns the nearest heavier weight available if the application is linked against the current or older SDK; otherwise, it returns the nearest lighter weight available.
*/
+ (NSFont *)systemFontOfSize:(CGFloat)fontSize weight:(NSFontWeight)weight API_AVAILABLE(macos(10.11));
+ (NSFont *)monospacedDigitSystemFontOfSize:(CGFloat)fontSize weight:(NSFontWeight)weight API_AVAILABLE(macos(10.11));

/* Returns current default monospaced font for system UI. Clients of this API should be aware that the monospaced system font has a similar coverage of default system UI font, which includes Latin and common symbols used for displaying text like source code. For the characters it does not cover, the subtituted fonts are usually not the same width as the monospaced system font, they can be wider, narrower, or variable. To ensure fixed advances in text layout, clients can consider using string attributes like NSFontFixedAdvanceAttribute. */
+ (NSFont *)monospacedSystemFontOfSize:(CGFloat)fontSize weight:(NSFontWeight)weight API_AVAILABLE(macos(10.15));

// Use instead of -[NSFontManager convertFont:toSize:].
- (NSFont *)fontWithSize:(CGFloat)fontSize API_AVAILABLE(macos(10.15));

/* UI font size settings
*/
@property (class, readonly) CGFloat systemFontSize; // size of the standard System font.
@property (class, readonly) CGFloat smallSystemFontSize; // size of standard small System font.
@property (class, readonly) CGFloat labelFontSize; // size of the standard Label Font.

+ (CGFloat)systemFontSizeForControlSize:(NSControlSize)controlSize;

/********* Core font attribute *********/
@property (readonly, copy) NSString *fontName;
@property (readonly) CGFloat pointSize;
@property (readonly) const CGFloat *matrix NS_RETURNS_INNER_POINTER;
@property (nullable, readonly, copy) NSString *familyName;
@property (nullable, readonly, copy) NSString *displayName;
@property (readonly, strong) NSFontDescriptor *fontDescriptor;
@property (readonly, copy) NSAffineTransform *textTransform;

/********* Glyph coverage *********/
@property (readonly) NSUInteger numberOfGlyphs;
@property (readonly) NSStringEncoding mostCompatibleStringEncoding;
@property (readonly, strong) NSCharacterSet *coveredCharacterSet;

/********* Font instance-wide metrics *********/
/* These methods return scaled numbers.  If the font was created with a matrix, the matrix is applied automatically; otherwise the coordinates are multiplied by size.
*/
@property (readonly) NSRect boundingRectForFont;
@property (readonly) NSSize maximumAdvancement;

@property (readonly) CGFloat ascender;
@property (readonly) CGFloat descender;
@property (readonly) CGFloat leading;

@property (readonly) CGFloat underlinePosition;
@property (readonly) CGFloat underlineThickness;
@property (readonly) CGFloat italicAngle;
@property (readonly) CGFloat capHeight;
@property (readonly) CGFloat xHeight;
@property (getter=isFixedPitch, readonly) BOOL fixedPitch;

/********* Glyph metrics *********/
/********* Glyph metrics *********/
- (NSRect)boundingRectForCGGlyph:(CGGlyph)glyph API_AVAILABLE(macos(10.13));
- (NSSize)advancementForCGGlyph:(CGGlyph)glyph API_AVAILABLE(macos(10.13));

// bulk query
- (void)getBoundingRects:(NSRectArray)bounds forCGGlyphs:(const CGGlyph *)glyphs count:(NSUInteger)glyphCount API_AVAILABLE(macos(10.13));
- (void)getAdvancements:(NSSizeArray)advancements forCGGlyphs:(const CGGlyph *)glyphs count:(NSUInteger)glyphCount API_AVAILABLE(macos(10.13));

/********* NSGraphicsContext-related *********/
- (void)set;
- (void)setInContext:(NSGraphicsContext *)graphicsContext;

/********* Vertical mode *********/
/* Returns a vertical version of the receiver if such a configuration is supported.  Returns the receiver if no vertical variant available.  A vertical font applies appropriate rotation to the text matrix in -setInContext:, returns vertical metrics, and enables the vertical glyph substitution feature by default. */
@property (readonly, copy) NSFont *verticalFont API_AVAILABLE(macos(10.7));

/* Returns YES if a vertical variant */
@property (getter=isVertical, readonly) BOOL vertical API_AVAILABLE(macos(10.7));
@end

/********* Notifications *********/
/* This notification is posted when the antialias threshold is changed by the user.
*/
APPKIT_EXTERN NSNotificationName NSAntialiasThresholdChangedNotification;

/* This notification is posted when the available font set is modified as a result of activation/deactivation.
*/
APPKIT_EXTERN NSNotificationName NSFontSetChangedNotification;


/********* Deprecated API *********/
// NSGlyph and related API are soft deprecated. They will be formally deprecated in a future version. Use CGGlyph-based TextKit API instead
typedef unsigned int NSGlyph; // Use CGGlyph instead

enum {
    NSControlGlyph = 0x00FFFFFF, // Deprecated. Use NSGlyphPropertyControlCharacter instead
    NSNullGlyph = 0x0 // Deprecated. Use NSGlyphPropertyNull instead
};

// NSFontRenderingMode-related API is now deprecated.
/********* Screen Font Rendering Mode *********/
typedef NS_ENUM(NSUInteger, NSFontRenderingMode) {
    NSFontDefaultRenderingMode = 0, // Determines the actual mode based on the user preference settings
    NSFontAntialiasedRenderingMode = 1, // Antialiased, floating-point advancements rendering mode (synonym to printerFont)
    NSFontIntegerAdvancementsRenderingMode = 2, // integer advancements rendering mode
    NSFontAntialiasedIntegerAdvancementsRenderingMode = 3 // Antialiased, integer advancements rendering mode
};

typedef NS_ENUM(NSUInteger, NSMultibyteGlyphPacking) {
    NSNativeShortGlyphPacking API_DEPRECATED("", macos(10.0,10.13)) = 5
} API_DEPRECATED("", macos(10.0,10.13));

APPKIT_EXTERN NSInteger NSConvertGlyphsToPackedGlyphs(NSGlyph * _Nonnull glBuf, NSInteger count, NSMultibyteGlyphPacking packing, char * _Nonnull packedGlyphs) API_DEPRECATED("", macos(10.0,10.13));

@interface NSFont (NSFont_Deprecated)
- (NSGlyph)glyphWithName:(NSString *)name;
- (NSRect)boundingRectForGlyph:(NSGlyph)glyph; // Deprecated. Use -boundingRectForCGGlyph: instead
- (NSSize)advancementForGlyph:(NSGlyph)glyph; // Deprecated. Use -advancementForCGGlyph: instead

// bulk query
- (void)getBoundingRects:(NSRectArray)bounds forGlyphs:(const NSGlyph *)glyphs count:(NSUInteger)glyphCount; // Deprecated. Use -getBoundingRects:forCGGlyphs:count: instead
- (void)getAdvancements:(NSSizeArray)advancements forGlyphs:(const NSGlyph *)glyphs count:(NSUInteger)glyphCount; // Deprecated. Use -getAdvancements:forCGGlyphs:count: insteda
- (void)getAdvancements:(NSSizeArray)advancements forPackedGlyphs:(const void *)packedGlyphs length:(NSUInteger)length; // Deprecated. Use -getAdvancements:forCGGlyphs:count: instead

/********* Rendering mode *********/
@property (readonly, copy) NSFont *printerFont;
@property (readonly, copy) NSFont *screenFont;
- (NSFont *)screenFontWithRenderingMode:(NSFontRenderingMode)renderingMode;
@property (readonly) NSFontRenderingMode renderingMode;
@end

@interface NSFont (NSFont_TextStyles)
// Returns an instance of the font associated with the text style. See NSFontDescriptor.h for the complete list.
+ (NSFont *)preferredFontForTextStyle:(NSFontTextStyle)style options:(NSDictionary<NSFontTextStyleOptionKey, id> *)options API_AVAILABLE(macos(11.0));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
