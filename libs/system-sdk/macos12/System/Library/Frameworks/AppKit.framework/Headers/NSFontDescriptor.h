/*
	NSFontDescriptor.h
	Application Kit
	Copyright (c) 2003-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSSet.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

/* Symbolic Font Traits */
typedef uint32_t NSFontSymbolicTraits; // Deprecated. Use NSFontDescriptorSymbolicTraits instead

typedef NS_OPTIONS(uint32_t, NSFontDescriptorSymbolicTraits) {
    NSFontDescriptorTraitItalic = 1u << 0,
    NSFontDescriptorTraitBold = 1u << 1,
    NSFontDescriptorTraitExpanded = 1u << 5,
    NSFontDescriptorTraitCondensed = 1u << 6,
    NSFontDescriptorTraitMonoSpace = 1u << 10,
    NSFontDescriptorTraitVertical = 1u << 11,
    NSFontDescriptorTraitUIOptimized = 1u << 12,
    NSFontDescriptorTraitTightLeading = 1u << 15,
    NSFontDescriptorTraitLooseLeading = 1u << 16,
    
    NSFontDescriptorTraitEmphasized API_UNAVAILABLE(macos) = NSFontDescriptorTraitBold,

    NSFontDescriptorClassMask = 0xF0000000,
    
    NSFontDescriptorClassUnknown = 0u << 28,
    NSFontDescriptorClassOldStyleSerifs = 1u << 28,
    NSFontDescriptorClassTransitionalSerifs = 2u << 28,
    NSFontDescriptorClassModernSerifs = 3u << 28,
    NSFontDescriptorClassClarendonSerifs = 4u << 28,
    NSFontDescriptorClassSlabSerifs = 5u << 28,
    NSFontDescriptorClassFreeformSerifs = 7u << 28,
    NSFontDescriptorClassSansSerif = 8u << 28,
    NSFontDescriptorClassOrnamentals = 9u << 28,
    NSFontDescriptorClassScripts = 10u << 28,
    NSFontDescriptorClassSymbolic = 12u << 28
};

@class NSAffineTransform;

typedef NSString * NSFontDescriptorAttributeName NS_TYPED_EXTENSIBLE_ENUM;
typedef NSString * NSFontDescriptorTraitKey NS_TYPED_ENUM;
typedef NSString * NSFontDescriptorVariationKey NS_TYPED_ENUM;
typedef NSString * NSFontDescriptorFeatureKey NS_TYPED_EXTENSIBLE_ENUM;
typedef CGFloat NSFontWeight NS_TYPED_EXTENSIBLE_ENUM;
typedef NSString * NSFontDescriptorSystemDesign NS_TYPED_ENUM;
typedef NSString * NSFontTextStyle NS_TYPED_ENUM API_AVAILABLE(macos(11.0));
typedef NSString * NSFontTextStyleOptionKey NS_TYPED_ENUM API_AVAILABLE(macos(11.0));

@interface NSFontDescriptor: NSObject <NSCopying, NSSecureCoding>

/* Core attribute access */
@property (nullable, readonly, copy) NSString *postscriptName;
@property (readonly) CGFloat pointSize;
@property (nullable, readonly, copy) NSAffineTransform *matrix;
@property (readonly) NSFontDescriptorSymbolicTraits symbolicTraits;
/* YES indicates that any fonts matching the descriptor needs to be downloaded prior to instantiating a font. To ensure that the matching fonts are available before use, use NSFontAssetRequest to download. NO indicates that the descriptor is not available for download, has already been downloaded, or is backed by an installed font.
 */
@property (readonly) BOOL requiresFontAssetRequest API_AVAILABLE(macos(10.13));

- (nullable id)objectForKey:(NSFontDescriptorAttributeName)attribute;

@property (readonly, copy) NSDictionary<NSFontDescriptorAttributeName, id> *fontAttributes;

/* Instantiation */
+ (NSFontDescriptor *)fontDescriptorWithFontAttributes:(nullable NSDictionary<NSFontDescriptorAttributeName, id> *)attributes;
+ (NSFontDescriptor *)fontDescriptorWithName:(NSString *)fontName size:(CGFloat)size;
+ (NSFontDescriptor *)fontDescriptorWithName:(NSString *)fontName matrix:(NSAffineTransform *)matrix;

- (instancetype)initWithFontAttributes:(nullable NSDictionary<NSFontDescriptorAttributeName, id> *)attributes;
/* Instance conversion */
/* Returns "normalized" font descriptors matching the receiver. mandatoryKeys is an NSSet instance containing keys that are required to be identical in order to be matched. mandatoryKeys can be nil.
*/
- (NSArray<NSFontDescriptor *> *)matchingFontDescriptorsWithMandatoryKeys:(nullable NSSet<NSFontDescriptorAttributeName> *)mandatoryKeys;

/* Returns a "normalized" font descriptor matching the receiver. It is the first element returned from -matchingFontDescriptorsWithMandatoryKeys:. mandatoryKeys is an NSSet instance containing keys that are required to be identical in order to be matched. mandatoryKeys can be nil.
 */
- (nullable NSFontDescriptor *)matchingFontDescriptorWithMandatoryKeys:(nullable NSSet<NSFontDescriptorAttributeName> *)mandatoryKeys API_AVAILABLE(macos(10.5));

- (NSFontDescriptor *)fontDescriptorByAddingAttributes:(NSDictionary<NSFontDescriptorAttributeName, id> *)attributes; // the new attributes take precedence over the existing ones in the receiver
- (NSFontDescriptor *)fontDescriptorWithSymbolicTraits:(NSFontDescriptorSymbolicTraits)symbolicTraits;
- (NSFontDescriptor *)fontDescriptorWithSize:(CGFloat)newPointSize;
- (NSFontDescriptor *)fontDescriptorWithMatrix:(NSAffineTransform *)matrix;
- (NSFontDescriptor *)fontDescriptorWithFace:(NSString *)newFace;
- (NSFontDescriptor *)fontDescriptorWithFamily:(NSString *)newFamily;

- (nullable instancetype)fontDescriptorWithDesign:(NSFontDescriptorSystemDesign)design API_AVAILABLE(macos(10.15));
@end

/* Predefined font attributes not defined in NSAttributedString.h */

APPKIT_EXTERN NSFontDescriptorAttributeName NSFontFamilyAttribute;
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontNameAttribute;
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontFaceAttribute;
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontSizeAttribute;
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontVisibleNameAttribute;

APPKIT_EXTERN NSFontDescriptorAttributeName NSFontMatrixAttribute                  ; // An NSAffineTransform instance. (default: identity matrix)
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontVariationAttribute               ; // An NSDictionary instance describing variation axis (default: supplied by font)
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontCharacterSetAttribute            ; // An NSCharacterSet instance representing a set of Unicode characters covered by the font. (default: supplied by font)
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontCascadeListAttribute             ; // An NSArray instance. Each member of the array is a sub-descriptor. (default: the system default cascading list for user's locale)
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontTraitsAttribute                  ; // An NSDictionary instance fully describing font traits. (default: supplied by font)
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontFixedAdvanceAttribute            ; // A float represented as an NSNumber. The value overrides glyph advancement specified by the font. (default: 0.0)
APPKIT_EXTERN NSFontDescriptorAttributeName NSFontFeatureSettingsAttribute          API_AVAILABLE(macos(10.5)); // An array of dictionaries representing non-default font feature settings. Each dictionary contains NSFontFeatureTypeIdentifierKey and NSFontFeatureSelectorIdentifierKey.

/* Font traits keys */
/* This key is used with a trait dictionary to get the symbolic traits value as an NSNumber.
*/
APPKIT_EXTERN NSFontDescriptorTraitKey NSFontSymbolicTrait;

/* This key is used with a trait dictionary to get the normalized weight value as an NSNumber. The valid value range is from -1.0 to 1.0. The value of 0.0 corresponds to the regular or medium font weight.
*/
APPKIT_EXTERN NSFontDescriptorTraitKey NSFontWeightTrait;

/* This key is used with a trait dictionary to get the relative inter-glyph spacing value as an NSNumber. The valid value range is from -1.0 to 1.0. The value of 0.0 corresponds to the regular glyph spacing.
*/
APPKIT_EXTERN NSFontDescriptorTraitKey NSFontWidthTrait;

/* This key is used with a trait dictionary to get the relative slant angle value as an NSNumber. The valid value range is from -1.0 to 1.0. The value or 0.0 corresponds to 0 degree clockwise rotation from the vertical and 1.0 corresponds to 30 degrees clockwise rotation.
*/
APPKIT_EXTERN NSFontDescriptorTraitKey NSFontSlantTrait;

/* Font variation keys */
/* This key is used with a variation axis dictionary to get the axis identifier value as an NSNumber.
*/
APPKIT_EXTERN NSFontDescriptorVariationKey NSFontVariationAxisIdentifierKey;

/* This key is used with a variation axis dictionary to get the  minimum axis value as an NSNumber.
*/
APPKIT_EXTERN NSFontDescriptorVariationKey NSFontVariationAxisMinimumValueKey;

/* This key is used with a variation axis dictionary to get the maximum axis value as an NSNumber.
*/
APPKIT_EXTERN NSFontDescriptorVariationKey NSFontVariationAxisMaximumValueKey;

/* This key is used with a variation axis dictionary to get the default axis value as an NSNumber.
*/
APPKIT_EXTERN NSFontDescriptorVariationKey NSFontVariationAxisDefaultValueKey;

/* This key is used with a variation axis dictionary to get the localized variation axis name.
*/
APPKIT_EXTERN NSFontDescriptorVariationKey NSFontVariationAxisNameKey;

/* Font feature keys */
/* A number object specifying font feature type such as ligature, character shape, etc. Refer to ATS/SFNTLayoutTypes.h for predefined feature types.
 */
APPKIT_EXTERN NSFontDescriptorFeatureKey NSFontFeatureTypeIdentifierKey API_AVAILABLE(macos(10.5));

/* A number object specifying font feature selector such as common ligature off, traditional character shape, etc. Refer to ATS/SFNTLayoutTypes.h for predefined feature selectors.
 */
APPKIT_EXTERN NSFontDescriptorFeatureKey NSFontFeatureSelectorIdentifierKey API_AVAILABLE(macos(10.5));

/* Font weight trait */
/* Predefined symbolic weight values used for NSFontWeightTrait and +[NSFont systemFontOfSize:weight:].
 */
APPKIT_EXTERN const NSFontWeight NSFontWeightUltraLight API_AVAILABLE(macos(10.11));
APPKIT_EXTERN const NSFontWeight NSFontWeightThin API_AVAILABLE(macos(10.11));
APPKIT_EXTERN const NSFontWeight NSFontWeightLight API_AVAILABLE(macos(10.11));
APPKIT_EXTERN const NSFontWeight NSFontWeightRegular API_AVAILABLE(macos(10.11));
APPKIT_EXTERN const NSFontWeight NSFontWeightMedium API_AVAILABLE(macos(10.11));
APPKIT_EXTERN const NSFontWeight NSFontWeightSemibold API_AVAILABLE(macos(10.11));
APPKIT_EXTERN const NSFontWeight NSFontWeightBold API_AVAILABLE(macos(10.11));
APPKIT_EXTERN const NSFontWeight NSFontWeightHeavy API_AVAILABLE(macos(10.11));
APPKIT_EXTERN const NSFontWeight NSFontWeightBlack API_AVAILABLE(macos(10.11));

APPKIT_EXTERN NSFontDescriptorSystemDesign const NSFontDescriptorSystemDesignDefault API_AVAILABLE(macos(10.15));
APPKIT_EXTERN NSFontDescriptorSystemDesign const NSFontDescriptorSystemDesignSerif API_AVAILABLE(macos(10.15));
APPKIT_EXTERN NSFontDescriptorSystemDesign const NSFontDescriptorSystemDesignMonospaced API_AVAILABLE(macos(10.15));
APPKIT_EXTERN NSFontDescriptorSystemDesign const NSFontDescriptorSystemDesignRounded API_AVAILABLE(macos(10.15));

APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleLargeTitle API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleTitle1 API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleTitle2 API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleTitle3 API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleHeadline API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleSubheadline API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleBody API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleCallout API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleFootnote API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleCaption1 API_AVAILABLE(macos(11.0));
APPKIT_EXTERN NSFontTextStyle const NSFontTextStyleCaption2 API_AVAILABLE(macos(11.0));

// Deprecated
typedef uint32_t NSFontFamilyClass; // Deprecated. Use NSFontDescriptorSymbolicTraits instead

// The following enum items are deprecated. Use NSFontDescriptorSymbolicTraits instead
enum {
    NSFontUnknownClass = 0 << 28,
    NSFontOldStyleSerifsClass = 1 << 28,
    NSFontTransitionalSerifsClass = 2 << 28,
    NSFontModernSerifsClass = 3 << 28,
    NSFontClarendonSerifsClass = 4 << 28,
    NSFontSlabSerifsClass = 5 << 28,
    NSFontFreeformSerifsClass = 7 << 28,
    NSFontSansSerifClass = 8 << 28,
    NSFontOrnamentalsClass = 9 << 28,
    NSFontScriptsClass = 10 << 28,
    NSFontSymbolicClass = 12 << 28
};

enum {
    NSFontFamilyClassMask = 0xF0000000
};

enum {
    NSFontItalicTrait = (1 << 0),
    NSFontBoldTrait = (1 << 1),
    NSFontExpandedTrait = (1 << 5),
    NSFontCondensedTrait = (1 << 6),
    NSFontMonoSpaceTrait = (1 << 10),
    NSFontVerticalTrait = (1 << 11),
    NSFontUIOptimizedTrait = (1 << 12)
};

APPKIT_EXTERN NSString * NSFontColorAttribute 			API_DEPRECATED("", macos(10.3,10.4)); // This attribute is obsolete. Use NSForegroundColorAttributeName instead.

@interface NSFontDescriptor (NSFontDescriptor_TextStyles)
// Returns a font descriptor containing the text style.
+ (NSFontDescriptor *)preferredFontDescriptorForTextStyle:(NSFontTextStyle)style options:(NSDictionary<NSFontTextStyleOptionKey, id> *)options API_AVAILABLE(macos(11.0));
@end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
