/*
 *  CTFontDescriptor.h
 *  CoreText
 *
 *  Copyright (c) 2006-2020 Apple Inc. All rights reserved.
 *
 */

/*!
    @header

    Thread Safety Information

    All functions in this header are thread safe unless otherwise specified.
*/

#ifndef __CTFONTDESCRIPTOR__
#define __CTFONTDESCRIPTOR__

#include <CoreText/CTFontTraits.h>

#include <CoreGraphics/CoreGraphics.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/*! --------------------------------------------------------------------------
    @group Descriptor Types
*///--------------------------------------------------------------------------

/*!
    @typedef    CTFontDescriptorRef
    @abstract   The Core Text Font Descriptor reference.
    @discussion This is a opaque reference to a font descriptor.
*/
#if TARGET_OS_IPHONE
typedef const struct CF_BRIDGED_TYPE(UIFontDescriptor) __CTFontDescriptor * CTFontDescriptorRef;
#else
typedef const struct CF_BRIDGED_TYPE(NSFontDescriptor) __CTFontDescriptor * CTFontDescriptorRef;
#endif

/*!
    @function   CTFontDescriptorGetTypeID
    @abstract   Returns the type identifier for Core Text font descriptor
                references.
    @result     The identifier for the opaque type CTFontDescriptorRef.
*/
CFTypeID CTFontDescriptorGetTypeID( void ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*! --------------------------------------------------------------------------
    @group Descriptor Constants
*///--------------------------------------------------------------------------

/*!
    @defined    kCTFontURLAttribute
    @abstract   The font URL.
    @discussion This is the key for accessing the font URL from the font descriptor. The value associated with this key is a CFURLRef.
*/
CT_EXPORT const CFStringRef kCTFontURLAttribute CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontNameAttribute
    @abstract   The PostScript name.
    @discussion This is the key for retrieving the PostScript name from the font descriptor. When matching, this is treated more generically: the system first tries to find fonts with this PostScript name. If none is found, the system tries to find fonts with this family name, and, finally, if still nothing, tries to find fonts with this display name. The value associated with this key is a CFStringRef. If unspecified, defaults to "Helvetica", if unavailable falls back to global font cascade list.
*/
CT_EXPORT const CFStringRef kCTFontNameAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontDisplayNameAttribute
    @abstract   The display name.
    @discussion This is the key for accessing the name used to display the font. Most commonly this is the full name. The value associated with this key is a CFStringRef.
*/
CT_EXPORT const CFStringRef kCTFontDisplayNameAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontFamilyNameAttribute
    @abstract   The family name.
    @discussion This is the key for accessing the family name from the font descriptor. The value associated with this key is a CFStringRef.
*/
CT_EXPORT const CFStringRef kCTFontFamilyNameAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontStyleNameAttribute
    @abstract   The style name.
    @discussion This is the key for accessing the style name of the font. This name represents the designer's description of the font's style. The value associated with this key is a CFStringRef.
*/
CT_EXPORT const CFStringRef kCTFontStyleNameAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontTraitsAttribute
    @abstract   The font traits dictionary.
    @discussion This is the key for accessing the dictionary of font traits for stylistic information. See CTFontTraits.h for the list of font traits. The value associated with this key is a CFDictionaryRef.
*/
CT_EXPORT const CFStringRef kCTFontTraitsAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontVariationAttribute
    @abstract   The font variation dictionary.
    @discussion This key is used to obtain the font variation instance as a CFDictionaryRef. If specified in a font descriptor, fonts with the specified axes will be primary match candidates, if no such fonts exist, this attribute will be ignored.
*/
CT_EXPORT const CFStringRef kCTFontVariationAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontVariationAxesAttribute
    @discussion An array of variation axis dictionaries or null if the font does not support variations. Each variation axis dictionary contains the five kCTFontVariationAxis* keys.
*/
CT_EXPORT const CFStringRef kCTFontVariationAxesAttribute CT_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));
/*!
    @defined    kCTFontSizeAttribute
    @abstract   The font point size.
    @discussion This key is used to obtain or specify the font point size. Creating a font with this unspecified will default to a point size of 12.0. The value for this key is represented as a CFNumberRef.
*/
CT_EXPORT const CFStringRef kCTFontSizeAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontMatrixAttribute
    @abstract   The font transformation matrix.
    @discussion This key is used to specify the font transformation matrix when creating a font. The default value is CGAffineTransformIdentity. The value for this key is a CFDataRef containing a CGAffineTransform, of which only the a, b, c, and d fields are used.
*/
CT_EXPORT const CFStringRef kCTFontMatrixAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontCascadeListAttribute
    @abstract   The font cascade list.
    @discussion This key is used to specify or obtain the cascade list used for a font reference. The cascade list is a CFArrayRef containing CTFontDescriptorRefs. If unspecified, the global cascade list is used. This list is not consulted for private-use characters on OS X 10.10, iOS 8, or earlier.
*/
CT_EXPORT const CFStringRef kCTFontCascadeListAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontCharacterSetAttribute
    @abstract   The font Unicode character coverage set.
    @discussion The value for this key is a CFCharacterSetRef. Creating a font with this attribute will restrict the font to a subset of its actual character set.
*/
CT_EXPORT const CFStringRef kCTFontCharacterSetAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontLanguagesAttribute
    @abstract   The list of supported languages.
    @discussion The value for this key is a CFArrayRef of CFStringRef language identifiers conforming to UTS #35. It can be requested from any font. If present in a descriptor used for matching, only fonts supporting the specified languages will be returned.
*/
CT_EXPORT const CFStringRef kCTFontLanguagesAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontBaselineAdjustAttribute
    @abstract   The baseline adjustment to apply to font metrics.
    @discussion The value for this key is a floating-point CFNumberRef. This is primarily used when defining font descriptors for a cascade list to keep the baseline of all fonts even.
*/
CT_EXPORT const CFStringRef kCTFontBaselineAdjustAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontMacintoshEncodingsAttribute
    @abstract   The Macintosh encodings (legacy script codes).
    @discussion The value associated with this key is a CFNumberRef containing a bitfield of the script codes in <CoreText/SFNTTypes.h>; bit 0 corresponds to kFontRomanScript, and so on. This attribute is provided for legacy compatibility.
*/
CT_EXPORT const CFStringRef kCTFontMacintoshEncodingsAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontFeaturesAttribute
    @abstract   The array of font features.
    @discussion This key is used to specify or obtain the font features for a font reference. The value associated with this key is a CFArrayRef of font feature dictionaries. This features list contains the feature information from the 'feat' table of the font. See the CTFontCopyFeatures() API in   CTFont.h.
*/
CT_EXPORT const CFStringRef kCTFontFeaturesAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontFeatureSettingsAttribute
    @abstract   The array of typographic feature settings.
    @discussion This key is used to specify an array of zero or more feature settings. Each setting dictionary indicates which setting should be applied. In the case of duplicate or conflicting settings the last setting in the list will take precedence. In the case of AAT settings, it is the caller's responsibility to handle exclusive and non-exclusive settings as necessary.
                An AAT setting dictionary contains a tuple of a kCTFontFeatureTypeIdentifierKey key-value pair and a kCTFontFeatureSelectorIdentifierKey key-value pair.
                An OpenType setting dictionary contains a tuple of a kCTFontOpenTypeFeatureTag key-value pair and a kCTFontOpenTypeFeatureValue key-value pair.

                Starting with OS X 10.10 and iOS 8.0, settings are also accepted (but not returned) in the following simplified forms:
                An OpenType setting can be either an array pair of tag string and value number, or a tag string on its own. For example: @[ @"c2sc", @1 ] or simply @"c2sc". An unspecified value enables the feature and a value of zero disables it.
                An AAT setting can be specified as an array pair of type and selector numbers. For example: @[ @(kUpperCaseType), @(kUpperCaseSmallCapsSelector) ].
*/
CT_EXPORT const CFStringRef kCTFontFeatureSettingsAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontFixedAdvanceAttribute
    @abstract   Specifies advance width.
    @discussion This key is used to specify a constant advance width, which affects the glyph metrics of any font instance created with this key; it overrides font values and the font transformation matrix, if any. The value associated with this key must be a CFNumberRef.

                Starting with macOS 10.14 and iOS 12.0, this only affects glyph advances that have non-zero width when this attribute is not present.
*/
CT_EXPORT const CFStringRef kCTFontFixedAdvanceAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontOrientationAttribute
    @abstract   The orientation attribute.
    @discussion This key is used to specify a particular orientation for the glyphs of the font. The value associated with this key is a int as a CFNumberRef. If you want to receive vertical metrics from a font for vertical rendering, specify kCTFontVerticalOrientation. If unspecified, the font will use its native orientation.
*/
CT_EXPORT const CFStringRef kCTFontOrientationAttribute CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @enum       CTFontOrientation
    @abstract   Specifies the intended rendering orientation of the font for obtaining glyph metrics.
*/
typedef CF_ENUM(uint32_t, CTFontOrientation) {
    kCTFontOrientationDefault    = 0,
    kCTFontOrientationHorizontal = 1,
    kCTFontOrientationVertical   = 2,

    kCTFontDefaultOrientation CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTFontOrientationDefault,
    kCTFontHorizontalOrientation CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTFontOrientationHorizontal,
    kCTFontVerticalOrientation CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTFontOrientationVertical
};

/*!
    @defined    kCTFontFormatAttribute
    @abstract   Specifies the recognized format of the font.
    @discussion The attribute is used to specify or obtain the format of the font. The returned value is a CFNumber containing one of the constants defined below.
*/
CT_EXPORT const CFStringRef kCTFontFormatAttribute CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @constant   kCTFontFormatUnrecognized
                The font is not a recognized format
    @constant   kCTFontFormatOpenTypePostScript
                The font is an OpenType format containing PostScript data
    @constant   kCTFontFormatOpenTypeTrueType
                The font is an OpenType format containing TrueType data
    @constant   kCTFontFormatTrueType
                The font is a recognized TrueType format
    @constant   kCTFontFormatPostScript
                The font is a recognized PostScript format
    @constant   kCTFontFormatBitmap
                The font is a bitmap only format
*/
typedef CF_ENUM(uint32_t, CTFontFormat) {
    kCTFontFormatUnrecognized       = 0,
    kCTFontFormatOpenTypePostScript = 1,
    kCTFontFormatOpenTypeTrueType   = 2,
    kCTFontFormatTrueType           = 3,
    kCTFontFormatPostScript         = 4,
    kCTFontFormatBitmap             = 5
};

/*!
    @defined    kCTFontRegistrationScopeAttribute
    @abstract   Specifies the font descriptor's registration scope.
    @discussion The attribute is used to specify or obtain the font registration scope. The value returned is a CFNumberRef containing one of the CTFontManagerScope enumerated values. A value of NULL can be returned for font descriptors that are not registered.
*/
CT_EXPORT const CFStringRef kCTFontRegistrationScopeAttribute CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontPriorityAttribute
    @abstract   The font descriptors priority when resolving duplicates and sorting match results.
    @discussion This key is used to obtain or specify the font priority. The value returned is a CFNumberRef containing an integer value as defined below. The higher the value, the higher the priority of the font. Only registered fonts will have a priority. Unregistered font descriptors will return NULL.
*/
CT_EXPORT const CFStringRef kCTFontPriorityAttribute CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @constant   kCTFontPrioritySystem
                Priority of system fonts (located in /System/Library/Fonts).
    @constant   kCTFontPriorityNetwork
                Priority of network fonts (located in /Network/Library/Fonts).
    @constant   kCTFontPriorityComputer
                Priority of computer local fonts (located in /Library/Fonts).
    @constant   kCTFontPriorityUser
                Priority of local fonts (located in user's Library/Fonts).
    @constant   kCTFontPriorityDynamic
                Priority of fonts registered dynamically, not located in a standard location (either kCTFontManagerScopeUser, or kCTFontManagerScopeSession).
    @constant   kCTFontPriorityProcess
                Priority of fonts registered for the process (kCTFontManagerScopeProcess).
*/
enum {
    kCTFontPrioritySystem       =  10000,
    kCTFontPriorityNetwork      =  20000,
    kCTFontPriorityComputer     =  30000,
    kCTFontPriorityUser         =  40000,
    kCTFontPriorityDynamic      =  50000,
    kCTFontPriorityProcess      =  60000
};
typedef uint32_t CTFontPriority;

/*!
    @defined    kCTFontEnabledAttribute
    @abstract   The font enabled state.
    @discussion The value associated with this key is a CFBoolean. Unregistered font descriptors will return NULL, which is equivalent to false.
*/
CT_EXPORT const CFStringRef kCTFontEnabledAttribute CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @defined    kCTFontDownloadableAttribute
    @abstract   The font downloadable state.
    @discussion The value associated with this key is a CFBoolean. If it is true, CoreText attempts to download a font if necessary when matching a descriptor.
*/
CT_EXPORT const CFStringRef kCTFontDownloadableAttribute CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/*!
    @defined    kCTFontDownloadedAttribute
    @abstract   The download state.
    @discussion The value associated with this key is a CFBoolean. If it is true, corresponding FontAsset has been downloaded. (but still it may be necessary to call appropriate API in order to use the font in the FontAsset.)
*/
CT_EXPORT const CFStringRef kCTFontDownloadedAttribute CT_AVAILABLE(macos(10.12), ios(7.0), watchos(2.0), tvos(9.0));

/*!
    @defined    kCTFontOpticalSizeAttribute
    @abstract   The point size at which this font is intended to be used.
    @discussion The value is a CFNumber used to activate size-specific (not linearly scaled) metrics. Starting with macOS 10.14 and iOS 12.0, the CFString "auto" can be used instead to request an optical size matching the point size. Starting with macOS 10.15 and iOS 13.0, the CFString "none" can be used instead to explicitly disable automatic optical sizing enabled by the font.
*/
CT_EXPORT const CFStringRef kCTFontOpticalSizeAttribute CT_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/*! --------------------------------------------------------------------------
    @group Descriptor Creation
*///--------------------------------------------------------------------------

/*!
    @function   CTFontDescriptorCreateWithNameAndSize
    @abstract   Creates a new font descriptor with the provided PostScript name and size.

    @param      name
                The PostScript name to be used for the font descriptor as a CFStringRef. Any font name beginning with a "." is reserved for the system and should not be used here.

    @param      size
                The point size. If 0.0, the kCTFontSizeAttribute will be omitted from the font descriptor.

    @result     This function creates a new font descriptor reference with the given PostScript name and point size.

    @discussion If you are trying to create a system UI font descriptor (with name beginning with a "."), you should create a font with CTFontCreateUIFontForLanguage() or appropriate AppKit/UIKit APIs instead, then use CTFontCopyFontDescriptor() to get its font descriptor.
*/
CTFontDescriptorRef CTFontDescriptorCreateWithNameAndSize(
    CFStringRef         name,
    CGFloat             size ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontDescriptorCreateWithAttributes
    @abstract   Creates a new font descriptor reference from a dictionary of attributes.

    @param      attributes
                A CFDictionaryRef of arbitrary attributes.

    @result     This function creates a new font descriptor with the attributes specified. This dictionary can contain arbitrary attributes that will be preserved, however unrecognized attributes will be ignored on font creation and and may not be preserved over the round trip (descriptor -> font -> descriptor).
*/
CTFontDescriptorRef CTFontDescriptorCreateWithAttributes(
    CFDictionaryRef     attributes ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontDescriptorCreateCopyWithAttributes
    @abstract   Creates a copy of the original font descriptor with new attributes.

    @param      original
                The original font descriptor reference.

    @param      attributes
                A CFDictionaryRef of arbitrary attributes.

    @result     This function creates a new copy of the original font descriptor with attributes augmented by those specified. If there are conflicts between attributes, the new attributes will replace existing ones, except for kCTFontVariationAttribute and kCTFontFeatureSettingsAttribute which will be merged.

                Starting with macOS 10.12 and iOS 10.0, setting the value of kCTFontFeatureSettingsAttribute to kCFNull will clear the feature settings of the original font descriptor. Setting the value of any individual feature settings pair in the kCTFontFeatureSettingsAttribute value array to kCFNull will clear that feature setting alone. For example, an element like @{ (id)kCTFontFeatureTypeIdentifierKey: @(kLigaturesType), (id)kCTFontFeatureSelectorIdentifierKey: (id)kCFNull } means clear the kLigatureType feature set in the original font descriptor. An element like @[ @"liga", (id)kCFNull ] will have the same effect.
*/
CTFontDescriptorRef CTFontDescriptorCreateCopyWithAttributes(
    CTFontDescriptorRef     original,
    CFDictionaryRef         attributes ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontCreateCopyWithFamily
    @abstract   Returns a new font descriptor in the specified family based on the traits of the original descriptor.

    @param      original
                The original font descriptor reference.

    @param      family
                The name of the desired family.

    @result     Returns a new font reference with the original traits in the given family, or NULL if none found in the system.
*/
CTFontDescriptorRef _Nullable CTFontDescriptorCreateCopyWithFamily(
    CTFontDescriptorRef     original,
    CFStringRef             family ) CT_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontDescriptorCreateCopyWithSymbolicTraits
    @abstract   Returns a new font descriptor based on the original descriptor having the specified symbolic traits.

    @param      original
                The original font descriptor reference.

    @param      symTraitValue
                The value of the symbolic traits. This bitfield is used to indicate the desired value for the traits specified by the symTraitMask parameter. Used in conjunction, they can allow for trait removal as well as addition.

    @param      symTraitMask
                The mask bits of the symbolic traits. This bitfield is used to indicate the traits that should be changed.

    @result     Returns a new font descriptor reference in the same family with the given symbolic traits, or NULL if none found in the system.
*/
CTFontDescriptorRef _Nullable CTFontDescriptorCreateCopyWithSymbolicTraits(
    CTFontDescriptorRef     original,
    CTFontSymbolicTraits    symTraitValue,
    CTFontSymbolicTraits    symTraitMask ) CT_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontDescriptorCreateCopyWithVariation
    @abstract   Creates a copy of the original font descriptor with a new variation instance.

    @param      original
                The original font descriptor reference.

    @param      variationIdentifier
                The variation axis identifier. This is the four character code of the variation axis as a CFNumberRef.

    @param      variationValue
                The value corresponding with the variation instance.

    @result     This function returns a copy of the original font descriptor with a new variation instance. This is a convenience method for easily creating new variation font instances.
*/
CTFontDescriptorRef CTFontDescriptorCreateCopyWithVariation(
    CTFontDescriptorRef     original,
    CFNumberRef             variationIdentifier,
    CGFloat                 variationValue ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontDescriptorCreateCopyWithFeature
    @abstract   Copies a font descriptor with new feature setting.

    @discussion This is a convenience method to more easily toggle the state of individual features.

    @param      original
                The original font descriptor reference.

    @param      featureTypeIdentifier
                The feature type identifier.

    @param      featureSelectorIdentifier
                The feature selector identifier.

    @result     A copy of the original font descriptor modified with the given feature settings.
*/
CTFontDescriptorRef CTFontDescriptorCreateCopyWithFeature(
    CTFontDescriptorRef     original,
    CFNumberRef             featureTypeIdentifier,
    CFNumberRef             featureSelectorIdentifier ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontDescriptorCreateMatchingFontDescriptors
    @abstract   Returns an array of font normalized font descriptors matching the provided descriptor.

    @param      descriptor
                The font descriptor reference.

    @param      mandatoryAttributes
                A set of attribute keys which are required to be identically matched in any returned font descriptors. Optional.

    @result     This function returns a retained array of normalized font descriptors matching the attributes present in descriptor. If descriptor itself is normalized then the array will contain only one item, the original descriptor.
*/
CFArrayRef _Nullable CTFontDescriptorCreateMatchingFontDescriptors(
    CTFontDescriptorRef     descriptor,
    CFSetRef _Nullable      mandatoryAttributes ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontDescriptorCreateMatchingFontDescriptor
    @abstract   Returns an the single preferred matching font descriptor based on the original descriptor and system precedence.

    @param      descriptor
                The font descriptor reference.

    @param      mandatoryAttributes
                A set of attribute keys which are required to be identically matched in any returned font descriptors. Optional.

    @result     This function returns a retained normalized font descriptor matching the attributes present in descriptor. The original descriptor may be returned in normalized form.
*/
CTFontDescriptorRef _Nullable CTFontDescriptorCreateMatchingFontDescriptor(
    CTFontDescriptorRef     descriptor,
    CFSetRef _Nullable      mandatoryAttributes ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

    
/*!
    Progress state
 */

typedef CF_ENUM(uint32_t, CTFontDescriptorMatchingState) {
    kCTFontDescriptorMatchingDidBegin,              // called once at the beginning.
    kCTFontDescriptorMatchingDidFinish,             // called once at the end.
    
    kCTFontDescriptorMatchingWillBeginQuerying,     // called once before talking to the server.  Skipped if not necessary.
    kCTFontDescriptorMatchingStalled,               // called when stalled. (e.g. while waiting for server response.)
    
    // Downloading and activating are repeated for each descriptor.
    kCTFontDescriptorMatchingWillBeginDownloading,  // Downloading part may be skipped if all the assets are already downloaded
    kCTFontDescriptorMatchingDownloading,
    kCTFontDescriptorMatchingDidFinishDownloading,
    kCTFontDescriptorMatchingDidMatch,              // called when font descriptor is matched.
    
    kCTFontDescriptorMatchingDidFailWithError       // called when an error occurred.  (may be called multiple times.)
};

/*!
    keys for progressParameter dictionary.
 */

/* CTFontDescriptorRef; The current font descriptor.   Valid when state is kCTFontDescriptorMatchingDidMatch. */
CT_EXPORT const CFStringRef kCTFontDescriptorMatchingSourceDescriptor CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* CFArray; Array of descriptors to be queried.   Valid while downloading or when state is kCTFontDescriptorMatchingWillBeginQuerying. */
CT_EXPORT const CFStringRef kCTFontDescriptorMatchingDescriptors CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* CFArray; Array of matched font descriptors.   Valid when state is kCTFontDescriptorMatchingDidMatch or CTFontDescriptorMatchingEnd. */
CT_EXPORT const CFStringRef kCTFontDescriptorMatchingResult CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* CFNumber; Download progress in 0 - 100.   Valid during Downloading state. */
CT_EXPORT const CFStringRef kCTFontDescriptorMatchingPercentage CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* CFNumber; Byte size to download for the current descriptor.   Valid during Downloading state. */
CT_EXPORT const CFStringRef kCTFontDescriptorMatchingCurrentAssetSize CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* CFNumber; Total downloaded byte size.   Valid during Downloading state. */
CT_EXPORT const CFStringRef kCTFontDescriptorMatchingTotalDownloadedSize CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* CFNumber; Total byte size to download.   Always valid, but may be Zero when information is not available. */
CT_EXPORT const CFStringRef kCTFontDescriptorMatchingTotalAssetSize CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* CFError; Valid when state kCTFontDescriptorMatchingDidFailWithError. */
CT_EXPORT const CFStringRef kCTFontDescriptorMatchingError CT_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

#if defined(__BLOCKS__)

/* Progress callback type */
typedef bool (^CTFontDescriptorProgressHandler)(CTFontDescriptorMatchingState state, CFDictionaryRef progressParameter);

/*
    @function   CTFontDescriptorMatchFontDescriptorsWithProgressHandler
                This function returns immediately, but can potentially take long time to process.  The progress is notified via progressBlock.
     
    @param      descriptors
                An array of descriptors to process.
     
    @param      mandatoryAttributes
     
    @param      progressBlock
                Callback block to indicate the progress.
                Return true to continue, and return false to cancel the process.
                This block is called on a private serial queue on OS X 10.15, iOS 13, and later.
     
    @result     false if it couldn't start the work.
*/
    
bool CTFontDescriptorMatchFontDescriptorsWithProgressHandler(
    CFArrayRef                          descriptors,
    CFSetRef _Nullable                  mandatoryAttributes,
    CTFontDescriptorProgressHandler     progressBlock) CT_AVAILABLE(macos(10.9), ios(6.0), watchos(2.0), tvos(9.0));

#endif // defined(__BLOCKS__)

/*! --------------------------------------------------------------------------
    @group Descriptor Accessors
*///--------------------------------------------------------------------------

/*!
    @function   CTFontDescriptorCopyAttributes
    @abstract   Returns the attributes dictionary of the font descriptor.

    @param      descriptor
                The font descriptor reference.

    @result     A retained reference to the font descriptor attributes dictionary. This dictionary will contain the minimum number of attributes to fully specify this particular font descriptor.
*/
CFDictionaryRef CTFontDescriptorCopyAttributes(
    CTFontDescriptorRef     descriptor ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @function   CTFontDescriptorCopyAttribute
    @abstract   Returns the value associated with an arbitrary attribute.

    @param      descriptor
                The font descriptor.

    @param      attribute
                The requested attribute.

    @result     A retained reference to the requested attribute, or NULL if the requested attribute is not present. Refer to the attribute definitions for documentation as to how each attribute is packaged as a CFType.
*/
CFTypeRef _Nullable CTFontDescriptorCopyAttribute(
    CTFontDescriptorRef     descriptor,
    CFStringRef             attribute ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    function    CTFontDescriptorCopyLocalizedAttribute
    @abstract   Returns a localized value for the requested attribute if available.

    @discussion This function returns a localized attribute based on the global language list. If localization is not possible for the attribute the behavior matches CTFontDescriptorCopyAttribute(). Generally, localization of attributes is only applicable to name attributes of a normalized font descriptor.

    @param      descriptor
                The font descriptor reference.

    @param      attribute
                The requested font attribute.

    @param      language
                If non-NULL, this will be receive a retained reference to the matched language. The language identifier will conform to UTS #35.
                If CoreText can supply its own localized string where the font cannot, this value will be NULL.

    @result     A retained reference to the requested attribute, or NULL if the requested attribute is not present. Refer to the attribute definitions for documentation as to how each attribute is packaged as a CFType.
*/
CFTypeRef _Nullable CTFontDescriptorCopyLocalizedAttribute(
    CTFontDescriptorRef     descriptor,
    CFStringRef             attribute,
    CFStringRef _Nullable * _Nullable language ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

CF_ASSUME_NONNULL_END
CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif
