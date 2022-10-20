/*
	NSColor.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

/* NSColors store colors. Often the only NSColor message you send is the "set" method, which makes the receiver the current color in the drawing context. There is usually no need to dive in and get the individual components (for instance, red, green, blue) that make up a color.

 There are three types of NSColors: Component based, pattern, and catalog.  Different types of NSColors provide different ways of getting at the values that define the colors:
 
     NSColorTypeComponentBased  Colors which have a color space (specified as NSColorSpace) and appropriate number of CGFloat components.
     NSColorTypePattern         Colors that draw a pattern, specified as an NSImage.
     NSColorTypeCatalog         Color with a catalog name and a color name. These colors can be dynamic and have multiple underlying color values.
                                Calling set will choose the right underlying implementation.

 It's illegal to ask a color for components or other information that are not valid for the color's type. If you need to ask a color for a certain set of components, use methods such as colorUsingType: or colorUsingColorSpace: to first convert the color, and assuming you do not get back nil, then ask for the appropriate component.  If the color is already the specified type or in the specified color space, you get the same color back; otherwise a conversion occurs.
 
 So, to get the colorSpace of an arbitrary color whose type you don't know:
 
    NSColorSpace *colorSpace = [color colorUsingType:NSColorTypeComponentBased].colorSpace;
 
 To get the sRGB red component of a color:
 
    NSColor *sRGBColor = color.colorUsingColorSpace(NSColorSpace.sRGBColorSpace);
    CGFloat red = sRGBColor ? sRGBColor.redComponent : someFallBackValue;
 
 From the above example it's important to note that the explicitly named component accessors such as redComponent, cyanComponent, whiteComponent don't work on all component based colors.  For example, only component-based colors whose color space has an RGB model will respond to red/blue/greenComponent, so hence the conversion above. Instead of doing a conversion, you could also validate the color you have with something like:
 
    if ((color.type == NSColorTypeComponentBased) && (color.colorSpace.colorSpaceModel == NSColorSpaceModelRGB)) red = color.redComponent;
 
 But very often doing a conversion is a better approach since it will let you extract the value you want from any color, as long as conversion is possible.
 
 All colors have an alphaComponent property, which defines opacity (1.0 == full opacity).  For some color types 1.0 is the only valid value.
 
 Note that prior to 10.13, NSColor used its colorSpaceName property to distinguish different types of colors, rather than type. The name colorSpaceName is a bit misleading since the value doesn't literally correspond to the name of an NSColorSpace. Like type, colors with different colorSpaceName values provide different ways of getting at the components:  NSNamedColorSpace corresponds to NSColorTypeCatalog, NSPatternColorSpace corresponds to NSColorTypePattern, and NSCustomColorSpace corresponds to NSColorTypeComponentBased. There are five other colorSpaceName values which all also correspond to color type of NSColorTypeComponentBased: NSDeviceWhiteColorSpace, NSCalibratedWhiteColorSpace, NSDeviceRGBColorSpace, NSCalibratedRGBColorSpace, NSDeviceCMYKColorSpace.  In new code where possible use type instead of colorSpaceName.

 Subclassers of NSColor need to implement the methods type, set, setFill, setStroke, the accessors appropriate for the color's type, and the NSCoding protocol. In code that needs to run prior to 10.13, colorSpaceName should also be implemented.  Some other methods such as colorWithAlphaComponent:, isEqual:, colorUsingType:, colorUsingColorSpaceName:, and CGColor may also be implemented if needed. If isEqual: is overridden, so should hash (because if [a isEqual:b] then [a hash] == [b hash]). Mutable subclassers (if any) should also implement copyWithZone: to a true copy.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSApplication.h>
#import <AppKit/NSCell.h>
#import <AppKit/NSColorList.h>
#import <AppKit/NSPasteboard.h>
#import <AppKit/AppKitDefines.h>
#import <CoreImage/CIColor.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSImage, NSColorSpace, NSAppearance;

static const NSAppKitVersion NSAppKitVersionNumberWithPatternColorLeakFix = 641.0;

/* Enum to distinguish types of colors. The color type of a color determines which properties are valid for that color; it's an exception-raising error to use properties that are not valid for a color.
 */
typedef NS_ENUM(NSInteger, NSColorType) {
    NSColorTypeComponentBased,  // Colors with colorSpace, and floating point color components
    NSColorTypePattern,         // Colors with patternImage
    NSColorTypeCatalog          // Colors with catalogNameComponent and colorNameComponent; these colors may have device and appearance specific representations
};

typedef NS_ENUM(NSInteger, NSColorSystemEffect) {
    NSColorSystemEffectNone,
    NSColorSystemEffectPressed,
    NSColorSystemEffectDeepPressed,
    NSColorSystemEffectDisabled,
    NSColorSystemEffectRollover,
} API_AVAILABLE(macos(10.14));

@interface NSColor : NSObject <NSCopying, NSSecureCoding, NSPasteboardReading, NSPasteboardWriting>

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;


/* Create component-based colors with arbitrary colorspace. The colorspace should be component-based, and the number of components in the provided array should match the number dictated by the specified colorspace, plus one for alpha (supply 1.0 for opaque colors); otherwise an exception will be raised.
*/
+ (NSColor *)colorWithColorSpace:(NSColorSpace *)space components:(const CGFloat *)components count:(NSInteger)numberOfComponents;


/* Create component-based colors in the sRGB, GenericGamma22Gray, or P3 colorspaces.
*/
+ (NSColor *)colorWithSRGBRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha  API_AVAILABLE(macos(10.7));
+ (NSColor *)colorWithGenericGamma22White:(CGFloat)white alpha:(CGFloat)alpha  API_AVAILABLE(macos(10.7));
+ (NSColor *)colorWithDisplayP3Red:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha API_AVAILABLE(macos(10.12));


/* Create component-based colors that are compatible with sRGB colorspace; these methods are effectively same as colorWithSRGBRed:... or colorWithGenericGamma22White:..., but provided for easier reuse of code that uses UIColor on iOS.  If red, green, blue, or saturation, brightness, or white values are outside of the 0..1 range, these will create colors in the extended sRGB (or for colorWithWhite:alpha:, extendedGenericGamma22GrayColorSpace) color spaces. This behavior is compatible with iOS UIColor.
 */
+ (NSColor *)colorWithWhite:(CGFloat)white alpha:(CGFloat)alpha API_AVAILABLE(macos(10.9));
+ (NSColor *)colorWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha API_AVAILABLE(macos(10.9));
+ (NSColor *)colorWithHue:(CGFloat)hue saturation:(CGFloat)saturation brightness:(CGFloat)brightness alpha:(CGFloat)alpha API_AVAILABLE(macos(10.9));


/* Create a RGB-based color using HSB component values. An exception will be raised if the color model of the provided color space is not RGB.
 */
+ (NSColor *)colorWithColorSpace:(NSColorSpace *)space hue:(CGFloat)hue saturation:(CGFloat)saturation brightness:(CGFloat)brightness alpha:(CGFloat)alpha API_AVAILABLE(macos(10.12));



/* Look up and return catalog colors from standard color catalogs, or from NSColorList whose name matches the specified catalog name.
 */
+ (nullable NSColor *)colorWithCatalogName:(NSColorListName)listName colorName:(NSColorName)colorName;

/* Create catalog colors from values stored with the given name in the Asset Catalog of the specified bundle.  To look in the main bundle, use the method without the bundle argument, or specify a nil bundle argument.
 */
+ (nullable NSColor *)colorNamed:(NSColorName)name bundle:(nullable NSBundle *)bundle API_AVAILABLE(macos(10.13));
+ (nullable NSColor *)colorNamed:(NSColorName)name API_AVAILABLE(macos(10.13));

/* Create a dynamic catalog color with a provider that is used to lazily resolve the exact color value.
 *
 * When methods are called on this color that need color component values, the provider is called with +NSAppearance.currentAppearance. The provider can use the appearance to return another color to be used for drawing. As much as possible, use the given appearance to make that decision, not other state.
 * The `colorName` should be universally unique and equates to the identity of the color, if nil is provided a unique name will be generated.
 * The name and the Aqua appearance color fallback will be encoded as part of the color. The decoded color is also able to 'join' to a color that gets registered with the same name and become dynamic again.
 */
+ (NSColor *)colorWithName:(nullable NSColorName)colorName dynamicProvider:(NSColor * (^)(NSAppearance *))dynamicProvider API_AVAILABLE(macos(10.15));

/* Create component-based colors in various device color spaces.
 */
+ (NSColor *)colorWithDeviceWhite:(CGFloat)white alpha:(CGFloat)alpha;
+ (NSColor *)colorWithDeviceRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha;
+ (NSColor *)colorWithDeviceHue:(CGFloat)hue saturation:(CGFloat)saturation brightness:(CGFloat)brightness alpha:(CGFloat)alpha;
+ (NSColor *)colorWithDeviceCyan:(CGFloat)cyan magenta:(CGFloat)magenta yellow:(CGFloat)yellow black:(CGFloat)black alpha:(CGFloat)alpha;


/* Create component-based colors in NSCalibratedWhiteColorSpace or NSCalibratedRGBColorSpace color spaces. In general it's best to avoid these methods, since sRGB or P3 colors are preferred over the calibrated (aka "generic") color spaces.
 */
+ (NSColor *)colorWithCalibratedWhite:(CGFloat)white alpha:(CGFloat)alpha;
+ (NSColor *)colorWithCalibratedRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha;
+ (NSColor *)colorWithCalibratedHue:(CGFloat)hue saturation:(CGFloat)saturation brightness:(CGFloat)brightness alpha:(CGFloat)alpha;


/* Create pattern colors that paint with the specified image.
 */
+ (NSColor *)colorWithPatternImage:(NSImage *)image;



/* Return the type of the color, which in turn determines which accessors are valid for this color. It's an exception-raising error to use properties that are not valid for a color; so, before using a given accessor on a color, call colorUsingType: to convert the color to the appropriate type. Or, if wanting to use colorSpace-specific accessors such as redComponent, convert to the desired colorSpace with colorUsingColorSpace:.
 
 Note that as of macOS 10.13, the new APIs type and colorUsingType: replace colorSpaceName and colorUsingColorSpaceName: as the funnel APIs for determining the type of color.  In code destined to run on 10.13 and newer systems, subclassers are encouraged to implement these two rather than the older colorSpaceName based methods.
 */
@property (readonly) NSColorType type API_AVAILABLE(macos(10.13));

/* Convert the color to a color of different type; return nil if conversion is not possible.
 */
- (nullable NSColor *)colorUsingType:(NSColorType)type API_AVAILABLE(macos(10.13));


/* colorUsingColorSpace: will convert existing color to a new color space, if the receiver can be represented as a component-based color.  This method will return the receiver if its colorspace is already the same as the one specified.  Will return nil if conversion is not possible.
 
 This method is highly recommended before using any specialized component accessors such as redComponent, cyanComponent, etc, since these will raise an exception if the color is not in an appropriate color space.
 */
- (nullable NSColor *)colorUsingColorSpace:(NSColorSpace *)space;



/* Some convenience methods to create standard colors, typically should not be used for display in user interface elements. Do not make any assumptions about the actual color spaces used: For applications built against the 10.13 SDK these return colors in the sRGB or genericGamma22GrayColorSpace color spaces, while in older apps they use calibrated color spaces.
 */
@property (class, strong, readonly) NSColor *blackColor;        /* 0.0 white */
@property (class, strong, readonly) NSColor *darkGrayColor;     /* 0.333 white */
@property (class, strong, readonly) NSColor *lightGrayColor;    /* 0.667 white */
@property (class, strong, readonly) NSColor *whiteColor;        /* 1.0 white */
@property (class, strong, readonly) NSColor *grayColor;         /* 0.5 white */
@property (class, strong, readonly) NSColor *redColor;          /* 1.0, 0.0, 0.0 RGB */
@property (class, strong, readonly) NSColor *greenColor;        /* 0.0, 1.0, 0.0 RGB */
@property (class, strong, readonly) NSColor *blueColor;         /* 0.0, 0.0, 1.0 RGB */
@property (class, strong, readonly) NSColor *cyanColor;         /* 0.0, 1.0, 1.0 RGB */
@property (class, strong, readonly) NSColor *yellowColor;       /* 1.0, 1.0, 0.0 RGB */
@property (class, strong, readonly) NSColor *magentaColor;      /* 1.0, 0.0, 1.0 RGB */
@property (class, strong, readonly) NSColor *orangeColor;       /* 1.0, 0.5, 0.0 RGB */
@property (class, strong, readonly) NSColor *purpleColor;       /* 0.5, 0.0, 0.5 RGB */
@property (class, strong, readonly) NSColor *brownColor;        /* 0.6, 0.4, 0.2 RGB */
@property (class, strong, readonly) NSColor *clearColor;        /* 0.0 white, 0.0 alpha */


/* The following are semantic system colors that are used for drawing user interface elements. These colors are implemented as dynamic named colors whose values may vary between releases and between different appearances (such as Aqua vs DarkAqua vs TouchBar). Do not make assumptions about the color spaces or actual colors used.
 
 If you do need to extract a CGColor (to use with a layer, say), always do it as lazily/late as possible, and update it whenever needed.  If you do need to extract color components, always convert to the desired color space first as described at the top of this file.
 */

/* Foreground Colors */
@property (class, strong, readonly) NSColor *labelColor API_AVAILABLE(macos(10.10));           // Foreground color for static text and related elements
@property (class, strong, readonly) NSColor *secondaryLabelColor API_AVAILABLE(macos(10.10));  // Foreground color for secondary static text and related elements
@property (class, strong, readonly) NSColor *tertiaryLabelColor API_AVAILABLE(macos(10.10));   // Foreground color for disabled static text and related elements
@property (class, strong, readonly) NSColor *quaternaryLabelColor API_AVAILABLE(macos(10.10)); // Foreground color for large secondary or disabled static text, separators, large glyphs/icons, etc

@property (class, strong, readonly) NSColor *linkColor API_AVAILABLE(macos(10.10));            // Foreground color for standard system links
@property (class, strong, readonly) NSColor *placeholderTextColor API_AVAILABLE(macos(10.10)); // Foreground color for placeholder text in controls or text views
@property (class, strong, readonly) NSColor *windowFrameTextColor;                         // Foreground color on window chrome
@property (class, strong, readonly) NSColor *selectedMenuItemTextColor;                    // Foreground color inside selected menu items. Equivalent to +labelColor in a NSBackgroundStyleEmphasized context.
@property (class, strong, readonly) NSColor *alternateSelectedControlTextColor;            // Foreground color inside emphasized and selected content: table views rows, collection views, etc. Equivalent to +labelColor in a NSBackgroundStyleEmphasized context.
@property (class, strong, readonly) NSColor *headerTextColor;                              // Foreground color for header cells in Table/OutlineView
@property (class, strong, readonly) NSColor *separatorColor API_AVAILABLE(macos(10.14));       // Foreground used for separators between sections of content. Not appropriate for use as split view or window chrome dividers.
@property (class, strong, readonly) NSColor *gridColor;                                    // Grids in controls


/* Background Colors */
@property (class, strong, readonly) NSColor *windowBackgroundColor;                        // Background for windows. This should not be used for drawing, and NSVisualEffectMaterialWindowBackground should be used instead.
@property (class, strong, readonly) NSColor *underPageBackgroundColor API_AVAILABLE(macos(10.8)); // Background areas revealed behind documents. This should not be used for drawing, and NSVisualEffectMaterialUnderPageBackground should be used instead.
@property (class, strong, readonly) NSColor *controlBackgroundColor;                       // Background for content areas: scroll views, table views, collection views. This should not be used for drawing, and NSVisualEffectMaterialContentBackground should be used instead.
@property (class, strong, readonly) NSColor *selectedContentBackgroundColor API_AVAILABLE(macos(10.14)); // The background color of selected and emphasized (focused) content: table views rows, collection views, etc. Alias for +alternateSelectedControlColor
@property (class, strong, readonly) NSColor *unemphasizedSelectedContentBackgroundColor API_AVAILABLE(macos(10.14)); // The background color of selected and unemphasized content: table views rows, collection views, etc. Alias for +secondarySelectedControlColor
@property (class, strong, readonly) NSArray<NSColor *> *alternatingContentBackgroundColors API_AVAILABLE(macos(10.14)); // The background colors for alternating content items: such as table view rows, collection view items. Alias for +controlAlternatingRowBackgroundColors
@property (class, strong, readonly) NSColor *findHighlightColor API_AVAILABLE(macos(10.13));   // Background color of find indicators: the bubbles that show inline search result hits


/* Text Colors */
@property (class, strong, readonly) NSColor *textColor;                                    // Document text
@property (class, strong, readonly) NSColor *textBackgroundColor;                          // Document text background

@property (class, strong, readonly) NSColor *selectedTextColor;                            // Selected document text
@property (class, strong, readonly) NSColor *selectedTextBackgroundColor;                  // Selected document text background
@property (class, strong, readonly) NSColor *unemphasizedSelectedTextBackgroundColor API_AVAILABLE(macos(10.14)); // The background for unemphasized text selection (e.g. when the associated control/view/window is not key)
@property (class, strong, readonly) NSColor *unemphasizedSelectedTextColor API_AVAILABLE(macos(10.14)); // The foreground for unemphasized text selection (e.g. when the associated control/view/window is not key)

/* Control Colors */
@property (class, strong, readonly) NSColor *controlColor;                                 // Approximate background for the color of control faces (such as buttons).
@property (class, strong, readonly) NSColor *controlTextColor;                             // Text on controls
@property (class, strong, readonly) NSColor *selectedControlColor;                         // Control face for selected controls
@property (class, strong, readonly) NSColor *selectedControlTextColor;                     // Text on selected controls
@property (class, strong, readonly) NSColor *disabledControlTextColor;                     // Text on disabled controls
@property (class, strong, readonly) NSColor *keyboardFocusIndicatorColor;                  // Keyboard focus ring around controls

@property (class, strong, readonly) NSColor *scrubberTexturedBackgroundColor API_AVAILABLE(macos(10.12.2)); // Patterned background color for use in NSScrubber


/* Some colors that are used by system elements and applications. Like the above dynamic system colors, these return named colors whose values may vary between different appearances and releases.  Do not make assumptions about the color spaces or actual colors used.
 */
@property (class, strong, readonly) NSColor *systemRedColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemGreenColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemBlueColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemOrangeColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemYellowColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemBrownColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemPinkColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemPurpleColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemGrayColor API_AVAILABLE(macos(10.10));
@property (class, strong, readonly) NSColor *systemTealColor API_AVAILABLE(macos(10.12));
@property (class, strong, readonly) NSColor *systemIndigoColor API_AVAILABLE(macos(10.15));
@property (class, strong, readonly) NSColor *systemMintColor API_AVAILABLE(macos(10.12));
@property (class, strong, readonly) NSColor *systemCyanColor API_AVAILABLE(macos(12.0));


/*! A dynamic color that reflects the user's current preferred accent color. This color automatically updates when the accent color preference changes. Do not make assumptions about the color space of this color, which may change across releases. */
@property (class, strong, readonly) NSColor *controlAccentColor API_AVAILABLE(macos(10.14));

@property (class, readonly) NSControlTint currentControlTint;   // returns current system control tint
+ (NSColor *)colorForControlTint:(NSControlTint)controlTint API_DEPRECATED("NSControlTint does not describe the full range of available control accent colors. Use +[NSColor controlAccentColor] instead.", macos(10.0, 11.0));

@property (class, strong, readonly) NSColor *highlightColor; // Highlight color for UI elements (this is abstract and defines the color all highlights tend toward)
@property (class, strong, readonly) NSColor *shadowColor;    // Shadow color for UI elements (this is abstract and defines the color all shadows tend toward)
- (nullable NSColor *)highlightWithLevel:(CGFloat)val;       // val = 0 => receiver, val = 1 => highlightColor
- (nullable NSColor *)shadowWithLevel:(CGFloat)val;          // val = 0 => receiver, val = 1 => shadowColor

/// Returns a color representing the base color with a system defined effect applied to it. This color is safe to create before draw time, as the resolution of the final color only happens when being `-set`, retrieving its `CGColor`, resolving with `-colorWithType:`, etc. The return color type is `.named`.
- (NSColor *)colorWithSystemEffect:(NSColorSystemEffect)systemEffect API_AVAILABLE(macos(10.14));


/* Set the color: Sets the fill and stroke colors in the current drawing context. If the color doesn't know about alpha, it's set to 1.0. Should be implemented by subclassers.
*/
- (void)set;

/* Set the fill or stroke colors individually. These should be implemented by subclassers.
*/
- (void)setFill;
- (void)setStroke;



/* Blend using the NSCalibratedRGB color space. Both colors are converted into the calibrated RGB color space, and they are blended by taking fraction of color and 1 - fraction of the receiver. The result is in the calibrated RGB color space. If the colors cannot be converted into the calibrated RGB color space the blending fails and nil is returned.
*/
- (nullable NSColor *)blendedColorWithFraction:(CGFloat)fraction ofColor:(NSColor *)color;


/* Returns a color in the same color space as the receiver with the specified alpha component. The abstract implementation of this method returns the receiver if alpha is 1.0, otherwise it returns nil; subclassers who have explicit opacity components override this method to actually return a color with the specified alpha.
*/
- (NSColor *)colorWithAlphaComponent:(CGFloat)alpha;


/*** Methods to get various components of colors. Not all of the methods apply to all colors; if called, they raise. ***/

/* Get the catalog and color name of standard colors from catalogs. Valid only on colors whose type is NSColorTypeCatalog. These colors are special colors which can have different device or appearance specific underlying colors.
*/
@property (readonly, copy) NSColorListName catalogNameComponent;
@property (readonly, copy) NSColorName colorNameComponent;

/* Return localized versions of the above.
*/
@property (readonly, copy) NSString *localizedCatalogNameComponent;
@property (readonly, copy) NSString *localizedColorNameComponent;

/* Get the red, green, or blue components of NSColorTypeComponentBased colors whose colorSpace model is RGB.  Raise for other types of colors.  To call these safely on unknown colors, you can use code like:
 
    NSColor *sRGBColor = color.colorUsingColorSpace(NSColorSpace.sRGBColorSpace);
    CGFloat red = sRGBColor ? sRGBColor.redComponent : someFallBackValue;
 */
@property (readonly) CGFloat redComponent;          // Valid only on component based colors whose colorSpace model is NSColorSpaceModelRGB.
@property (readonly) CGFloat greenComponent;        // Valid only on component based colors whose colorSpace model is NSColorSpaceModelRGB.
@property (readonly) CGFloat blueComponent;         // Valid only on component based colors whose colorSpace model is NSColorSpaceModelRGB.
- (void)getRed:(nullable CGFloat *)red green:(nullable CGFloat *)green blue:(nullable CGFloat *)blue alpha:(nullable CGFloat *)alpha;           // Valid only on component based colors whose colorSpace model is NSColorSpaceModelRGB.

/* Get the components of NSCalibratedRGB or NSDeviceRGB colors as hue, saturation, or brightness.  Raise for other types of colors.
*/
@property (readonly) CGFloat hueComponent;          // Valid only on component based colors whose colorSpace model is NSColorSpaceModelRGB.
@property (readonly) CGFloat saturationComponent;   // Valid only on component based colors whose colorSpace model is NSColorSpaceModelRGB.
@property (readonly) CGFloat brightnessComponent;   // Valid only on component based colors whose colorSpace model is NSColorSpaceModelRGB.
- (void)getHue:(nullable CGFloat *)hue saturation:(nullable CGFloat *)saturation brightness:(nullable CGFloat *)brightness alpha:(nullable CGFloat *)alpha;     // Valid only on component based colors whose colorSpace model is NSColorSpaceModelRGB.


/* Get the white component of NSCalibratedWhite or NSDeviceWhite colors, or NSCustomColorSpace colors with color spaces with grayscale model.  Raise for other types of colors.
*/
@property (readonly) CGFloat whiteComponent;        // Valid only on component based colors whose colorSpace model is NSColorSpaceModelGray.
- (void)getWhite:(nullable CGFloat *)white alpha:(nullable CGFloat *)alpha;     // Valid only on component based colors whose colorSpace model is NSColorSpaceModelGray.


/* Get the CMYK components of NSDeviceCMYK colors, or NSCustomColorSpace colors with color spaces with CMYK model.   Raise for other types of colors.
*/
@property (readonly) CGFloat cyanComponent;         // Valid only on component based colors whose colorSpace model is NSColorSpaceModelCMYK.
@property (readonly) CGFloat magentaComponent;      // Valid only on component based colors whose colorSpace model is NSColorSpaceModelCMYK.
@property (readonly) CGFloat yellowComponent;       // Valid only on component based colors whose colorSpace model is NSColorSpaceModelCMYK.
@property (readonly) CGFloat blackComponent;        // Valid only on component based colors whose colorSpace model is NSColorSpaceModelCMYK.
- (void)getCyan:(nullable CGFloat *)cyan magenta:(nullable CGFloat *)magenta yellow:(nullable CGFloat *)yellow black:(nullable CGFloat *)black alpha:(nullable CGFloat *)alpha;         // Valid only on component based colors whose colorSpace model is NSColorSpaceModelCMYK.


/* In colors with type == NSColorTypeComponentBased, return the colorspace and individual floating point components, including alpha.  Like other colorspace-specific methods these will raise exceptions on other color types.
*/
@property (readonly, strong) NSColorSpace *colorSpace;
@property (readonly) NSInteger numberOfComponents;
- (void)getComponents:(CGFloat *)components;


/* Return the image used for the pattern color. Will raise exception for colors other than type == NSColorTypePattern.
 */
@property (readonly, strong) NSImage *patternImage;


/* Get the alpha component. Valid on all colors; for colors which do not have alpha components, this will return 1.0 (opaque).
*/
@property (readonly) CGFloat alphaComponent;

/* Pasteboard methods
*/
+ (nullable NSColor *)colorFromPasteboard:(NSPasteboard *)pasteBoard;
- (void)writeToPasteboard:(NSPasteboard *)pasteBoard;


/* Draws the color and adorns it to indicate the type of color. Used by colorwells, swatches, and other UI objects that need to display colors. Implementation in NSColor simply draws the color (with an indication of transparency if the color isn't fully opaque); subclassers can draw more stuff as they see fit.
*/
- (void)drawSwatchInRect:(NSRect)rect;


/* Convert to and from CGColorRef.
*/
+ (nullable NSColor *)colorWithCGColor:(CGColorRef)cgColor API_AVAILABLE(macos(10.8));          // May return nil
@property (readonly) CGColorRef CGColor NS_RETURNS_INNER_POINTER API_AVAILABLE(macos(10.8));    // Returns an autoreleased CGColor. This will never be NULL, although the return value may be an approximation in some cases, so there isn't guaranteed round-trip fidelity.


/* Global flag for determining whether an application supports alpha.  This flag is consulted when an application imports alpha (through color dragging, for instance). The value of this flag also determines whether the color panel has an opacity slider. This value is YES by default, indicating that the opacity components of imported colors will be set to 1.0. If an application wants alpha, it can either set the "NSIgnoreAlpha" default to NO or call the set method below.

This method provides a global approach to removing alpha which might not always be appropriate. Applications which need to import alpha sometimes should set this flag to NO and explicitly make colors opaque in cases where it matters to them.
*/
@property (class) BOOL ignoresAlpha;

@end

@interface NSColor (NSDeprecated)

/// Historically used as the inner border highlight color for beveled buttons. No longer used.
@property (class, strong, readonly) NSColor *controlHighlightColor API_DEPRECATED("Use a color that matches the semantics being used, such as `separatorColor`", macos(10.0, 11.0));
/// Historically used as the outer border highlight color for beveled buttons. No longer used.
@property (class, strong, readonly) NSColor *controlLightHighlightColor API_DEPRECATED("Use a color that matches the semantics being used, such as `separatorColor`", macos(10.0, 11.0));
/// Historically used as the inner border shadow color for beveled buttons. No longer used.
@property (class, strong, readonly) NSColor *controlShadowColor API_DEPRECATED("Use a color that matches the semantics being used, such as `separatorColor`", macos(10.0, 11.0));
/// Historically used as the outer border shadow color for beveled buttons. No longer used.
@property (class, strong, readonly) NSColor *controlDarkShadowColor API_DEPRECATED("Use a color that matches the semantics being used, such as `separatorColor`", macos(10.0, 11.0));

/// Historically used as the color of scroll bars. No longer used.
@property (class, strong, readonly) NSColor *scrollBarColor API_DEPRECATED("Use NSScroller instead", macos(10.0, 11.0));
/// Historically used as the color of scroll bar knobs. No longer used.
@property (class, strong, readonly) NSColor *knobColor API_DEPRECATED("Use NSScroller instead", macos(10.0, 11.0));
/// Historically used as the color of scroll bar knobs being dragged. No longer used.
@property (class, strong, readonly) NSColor *selectedKnobColor API_DEPRECATED("Use NSScroller instead", macos(10.0, 11.0));

/// Historically used as the color of the window chrome, which is no longer able to be represented by a color. No longer used.
@property (class, strong, readonly) NSColor *windowFrameColor API_DEPRECATED("Use NSVisualEffectMaterialTitlebar", macos(10.0, 11.0));
/// Historically used as the color of selected menu items, which is no longer a color but a tinted blur effect. No longer used.
@property (class, strong, readonly) NSColor *selectedMenuItemColor API_DEPRECATED("Use NSVisualEffectMaterialSelection", macos(10.0, 11.0));
/// Historically used as the color of table headers, which is no longer a color but a tinted blur effect.
@property (class, strong, readonly) NSColor *headerColor API_DEPRECATED("Use NSVisualEffectMaterialHeaderView", macos(10.0, 11.0));

/// The background color of selected content or text that is unemphasized. Older alias for +unemphasizedSelectedContentBackgroundColor and +unemphasizedSelectedTextBackgroundColor
@property (class, strong, readonly) NSColor *secondarySelectedControlColor API_DEPRECATED_WITH_REPLACEMENT("unemphasizedSelectedContentBackgroundColor", macos(10.1, 11.0));
/// The background color of selected and emphasized (focused) content: table views rows, collection views, etc. Older alias for +selectedContentBackgroundColor
@property (class, strong, readonly) NSColor *alternateSelectedControlColor API_DEPRECATED_WITH_REPLACEMENT("selectedContentBackgroundColor", macos(10.2, 11.0));
/// The background colors for alternating content items: such as table view rows, collection view items. Older alias for +alternatingContentBackgroundColors
@property (class, strong, readonly) NSArray<NSColor *> *controlAlternatingRowBackgroundColors API_DEPRECATED_WITH_REPLACEMENT("alternatingContentBackgroundColors", macos(10.3, 11.0));

/* Get the color space name of the color. Note that here "color space name" doesn't refer to the name of an NSColorSpace, but rather an older way to distinguish the color type of different types of colors, as described at the top of this file. Should be implemented by subclassers for code destined to run on 10.12 or earlier. Newer code can choose to implement type instead.
 */
@property (readonly, copy) NSColorSpaceName colorSpaceName API_DEPRECATED("Use -type and NSColorType instead", macos(10.0,10.14));


/* Convert the color to a color of another color space name. This may return nil if the specified conversion cannot be done. The abstract implementation of this method returns the receiver if the specified name matches that of the receiver; otherwise it returns nil. Subclassers who can convert themselves to other color space names override this method to do something better.

 The device description argument allows the color to specialize itself for the given device.  Device descriptions can be obtained from windows, screens, and printers with the "deviceDescription" method.

 If device is nil then the current device (as obtained from the currently lockFocus'ed view's window or, if printing, the current printer) is used. The method without the device: argument passes nil for the device.

 If colorSpaceName is nil, then the most appropriate color space is used.
 */
- (nullable NSColor *)colorUsingColorSpaceName:(nullable NSColorSpaceName)name device:(nullable NSDictionary<NSDeviceDescriptionKey, id> *)deviceDescription API_DEPRECATED("Use -colorUsingType: or -colorUsingColorSpace: instead", macos(10.0,10.14));
- (nullable NSColor *)colorUsingColorSpaceName:(NSColorSpaceName)name API_DEPRECATED("Use -colorUsingType: or -colorUsingColorSpace: instead", macos(10.0,10.14));


@end


@interface NSColor (NSQuartzCoreAdditions)
+ (NSColor *)colorWithCIColor:(CIColor *)color;
@end

@interface CIColor (NSAppKitAdditions)
- (nullable instancetype)initWithColor:(NSColor *)color;
@end

@interface NSCoder(NSAppKitColorExtensions)

/* To decode NXColors... Will return nil if the archived color was "invalid" (written out by the kit in a few instances). Otherwise returns autoreleased NSColor. Can't write NSColors as NXColors, so we have no corresponding encode method.
*/
- (null_unspecified NSColor *)decodeNXColor API_DEPRECATED("", macos(10.0,10.9));

@end

APPKIT_EXTERN NSNotificationName NSSystemColorsDidChangeNotification;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
