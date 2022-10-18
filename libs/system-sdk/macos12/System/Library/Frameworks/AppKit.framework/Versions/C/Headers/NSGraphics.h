/*
	NSGraphics.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSGeometry.h> 
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor, NSView;

/* Operation types for composite operators */
typedef NS_ENUM(NSUInteger, NSCompositingOperation) {
    /* Porter-Duff compositing operations */
    /* https://www.w3.org/TR/compositing-1/#porterduffcompositingoperators */
    NSCompositingOperationClear,
    NSCompositingOperationCopy,
    NSCompositingOperationSourceOver,
    NSCompositingOperationSourceIn,
    NSCompositingOperationSourceOut,
    NSCompositingOperationSourceAtop,
    NSCompositingOperationDestinationOver,
    NSCompositingOperationDestinationIn,
    NSCompositingOperationDestinationOut,
    NSCompositingOperationDestinationAtop,
    NSCompositingOperationXOR,
    NSCompositingOperationPlusDarker,
    NSCompositingOperationHighlight API_DEPRECATED("Use NSCompositingOperationSourceOver instead", macos(10.0,10.0)),
    NSCompositingOperationPlusLighter,
    
    /* Separable blend-modes */
    /* https://www.w3.org/TR/compositing-1/#blendingseparable */
    NSCompositingOperationMultiply	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationScreen	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationOverlay	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationDarken	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationLighten	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationColorDodge	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationColorBurn	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationSoftLight	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationHardLight	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationDifference	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationExclusion	API_AVAILABLE(macos(10.10)),
    
    /* Non-separable blend-modes */
    /* https://www.w3.org/TR/compositing-1/#blendingnonseparable */
    NSCompositingOperationHue		API_AVAILABLE(macos(10.10)),
    NSCompositingOperationSaturation	API_AVAILABLE(macos(10.10)),
    NSCompositingOperationColor		API_AVAILABLE(macos(10.10)),
    NSCompositingOperationLuminosity	API_AVAILABLE(macos(10.10)),
};


static const NSCompositingOperation NSCompositeClear API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationClear", macos(10.0,10.12)) = NSCompositingOperationClear;
static const NSCompositingOperation NSCompositeCopy API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationCopy", macos(10.0,10.12)) = NSCompositingOperationCopy;
static const NSCompositingOperation NSCompositeSourceOver API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationSourceOver", macos(10.0,10.12)) = NSCompositingOperationSourceOver;
static const NSCompositingOperation NSCompositeSourceIn API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationSourceIn", macos(10.0,10.12)) = NSCompositingOperationSourceIn;
static const NSCompositingOperation NSCompositeSourceOut API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationSourceOut", macos(10.0,10.12)) = NSCompositingOperationSourceOut;
static const NSCompositingOperation NSCompositeSourceAtop API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationSourceAtop", macos(10.0,10.12)) = NSCompositingOperationSourceAtop;
static const NSCompositingOperation NSCompositeDestinationOver API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationDestinationOver", macos(10.0,10.12)) = NSCompositingOperationDestinationOver;
static const NSCompositingOperation NSCompositeDestinationIn API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationDestinationIn", macos(10.0,10.12)) = NSCompositingOperationDestinationIn;
static const NSCompositingOperation NSCompositeDestinationOut API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationDestinationOut", macos(10.0,10.12)) = NSCompositingOperationDestinationOut;
static const NSCompositingOperation NSCompositeDestinationAtop API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationDestinationAtop", macos(10.0,10.12)) = NSCompositingOperationDestinationAtop;
static const NSCompositingOperation NSCompositeXOR API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationXOR", macos(10.0,10.12)) = NSCompositingOperationXOR;
static const NSCompositingOperation NSCompositePlusDarker API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationPlusDarker", macos(10.0,10.12)) = NSCompositingOperationPlusDarker;
static const NSCompositingOperation NSCompositeHighlight API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationSourceOver", macos(10.0,10.0)) = NSCompositingOperationHighlight;
static const NSCompositingOperation NSCompositePlusLighter API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationPlusLighter", macos(10.0,10.12)) = NSCompositingOperationPlusLighter;
static const NSCompositingOperation NSCompositeMultiply	API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationMultiply", macos(10.0,10.12)) = NSCompositingOperationMultiply;
static const NSCompositingOperation NSCompositeScreen API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationScreen", macos(10.0,10.12)) = NSCompositingOperationScreen;
static const NSCompositingOperation NSCompositeOverlay API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationOverlay", macos(10.0,10.12)) = NSCompositingOperationOverlay;
static const NSCompositingOperation NSCompositeDarken API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationDarken", macos(10.0,10.12)) = NSCompositingOperationDarken;
static const NSCompositingOperation NSCompositeLighten API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationLighten", macos(10.0,10.12)) = NSCompositingOperationLighten;
static const NSCompositingOperation NSCompositeColorDodge API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationColorDodge", macos(10.0,10.12)) = NSCompositingOperationColorDodge;
static const NSCompositingOperation NSCompositeColorBurn API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationColorBurn", macos(10.0,10.12)) = NSCompositingOperationColorBurn;
static const NSCompositingOperation NSCompositeSoftLight API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationSoftLight", macos(10.0,10.12)) = NSCompositingOperationSoftLight;
static const NSCompositingOperation NSCompositeHardLight API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationHardLight", macos(10.0,10.12)) = NSCompositingOperationHardLight;
static const NSCompositingOperation NSCompositeDifference API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationDifference", macos(10.0,10.12)) = NSCompositingOperationDifference;
static const NSCompositingOperation NSCompositeExclusion API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationExclusion", macos(10.0,10.12)) = NSCompositingOperationExclusion;

static const NSCompositingOperation NSCompositeHue API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationHue", macos(10.0,10.12)) = NSCompositingOperationHue;
static const NSCompositingOperation NSCompositeSaturation API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationSaturation", macos(10.0,10.12)) = NSCompositingOperationSaturation;
static const NSCompositingOperation NSCompositeColor API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationColor", macos(10.0,10.12)) = NSCompositingOperationColor;
static const NSCompositingOperation NSCompositeLuminosity API_DEPRECATED_WITH_REPLACEMENT("NSCompositingOperationLuminosity", macos(10.0,10.12)) = NSCompositingOperationLuminosity;




/* Types of window backing stores.
 */
typedef NS_ENUM(NSUInteger, NSBackingStoreType) {
    /* NSBackingStoreRetained and NSBackingStoreNonretained have effectively been synonyms of NSBackingStoreBuffered since OS X Mountain Lion.  Please switch to the equivalent NSBackingStoreBuffered.
     */
    NSBackingStoreRetained API_DEPRECATED_WITH_REPLACEMENT("NSBackingStoreBuffered", macos(10.0,10.13)) = 0,
    NSBackingStoreNonretained API_DEPRECATED_WITH_REPLACEMENT("NSBackingStoreBuffered", macos(10.0,10.13)) = 1,
    NSBackingStoreBuffered = 2,
};

/* ways to order windows */
typedef NS_ENUM(NSInteger, NSWindowOrderingMode) {
    NSWindowAbove		 =  1,
    NSWindowBelow		 = -1,
    NSWindowOut			 =  0
};

/* order in which to draw focus ring - above or below graphic or just draw ring */
typedef NS_ENUM(NSUInteger, NSFocusRingPlacement) {
    NSFocusRingOnly	 = 0,
    NSFocusRingBelow	 = 1,
    NSFocusRingAbove	 = 2
};

/* used by NSView and NSCell to configure if and how the control should draw its focus ring */
typedef NS_ENUM(NSUInteger, NSFocusRingType) {
    NSFocusRingTypeDefault = 0,
    NSFocusRingTypeNone = 1,
    NSFocusRingTypeExterior = 2
};


// values interchangeable with CGColorRenderingIntent values
typedef NS_ENUM(NSInteger, NSColorRenderingIntent) {
    NSColorRenderingIntentDefault, //  = kCGRenderingIntentDefault,
    NSColorRenderingIntentAbsoluteColorimetric, //  = kCGRenderingIntentAbsoluteColorimetric,
    NSColorRenderingIntentRelativeColorimetric, //  = kCGRenderingIntentRelativeColorimetric,
    NSColorRenderingIntentPerceptual, // = kCGRenderingIntentPerceptual,
    NSColorRenderingIntentSaturation // = kCGRenderingIntentSaturation
} API_AVAILABLE(macos(10.5));


typedef NSString * NSColorSpaceName NS_TYPED_ENUM;
/* Predefined colorspace names.
*/
APPKIT_EXTERN NSColorSpaceName NSCalibratedWhiteColorSpace;	/* 1.0 == white */
APPKIT_EXTERN NSColorSpaceName NSCalibratedRGBColorSpace;
APPKIT_EXTERN NSColorSpaceName NSDeviceWhiteColorSpace;	/* 1.0 == white */
APPKIT_EXTERN NSColorSpaceName NSDeviceRGBColorSpace;
APPKIT_EXTERN NSColorSpaceName NSDeviceCMYKColorSpace;
APPKIT_EXTERN NSColorSpaceName NSNamedColorSpace;		/* Used for "catalog" colors */
APPKIT_EXTERN NSColorSpaceName NSPatternColorSpace;
APPKIT_EXTERN NSColorSpaceName NSCustomColorSpace;		/* Used to indicate a custom gstate in images */

/* Prefer colorspaces where 1.0 means white.
 */
APPKIT_EXTERN NSColorSpaceName NSCalibratedBlackColorSpace     /* 1.0 == black */ API_DEPRECATED("", macos(10.0,10.6));
APPKIT_EXTERN NSColorSpaceName NSDeviceBlackColorSpace 	/* 1.0 == black */ API_DEPRECATED("", macos(10.0,10.6));


/* NSWindowDepth defines the values used in setting window depth limits. "0" indicates default depth. Window depths should not be made persistent as they will not be the same across systems. Use the functions NSBitsPerSampleFromDepth(), NSColorSpaceFromDepth(), NSBitsPerPixelFromDepth(), and NSPlanarFromDepth() to extract info from an NSWindowDepth 

On Mac OS X 10.5 and earlier, use NSBestDepth() to compute window depths. NSBestDepth() will try to accomodate all the parameters (match or better); if there are multiple matches, it gives the closest, with matching colorSpace first, then bps, then planar, then bpp. bpp is "bits per pixel"; 0 indicates default (same as the number of bits per plane, either bps or bps * NSNumberOfColorComponents()); other values maybe used as hints to provide backing stores of different configuration; for instance, 8 bit color. exactMatch is optional and indicates whether all the parameters matched exactly.

On Mac OS X 10.6 and later, you can pass one of the explicit bit depths below to -[NSWindow setDepthLimit:].  NSWindowDepthTwentyfourBitRGB is the default.
*/
typedef NS_ENUM(int32_t, NSWindowDepth) {
    NSWindowDepthTwentyfourBitRGB = 0x208,
    NSWindowDepthSixtyfourBitRGB = 0x210,
    NSWindowDepthOnehundredtwentyeightBitRGB = 0x220
} API_AVAILABLE(macos(10.6));

APPKIT_EXTERN NSWindowDepth NSBestDepth (NSColorSpaceName colorSpace, NSInteger bps, NSInteger bpp, BOOL planar, BOOL * _Nullable exactMatch);
APPKIT_EXTERN BOOL NSPlanarFromDepth (NSWindowDepth depth);
APPKIT_EXTERN NSColorSpaceName _Nullable NSColorSpaceFromDepth (NSWindowDepth depth);
APPKIT_EXTERN NSInteger NSBitsPerSampleFromDepth(NSWindowDepth depth);
APPKIT_EXTERN NSInteger NSBitsPerPixelFromDepth(NSWindowDepth depth);
APPKIT_EXTERN NSInteger NSNumberOfColorComponents(NSColorSpaceName colorSpaceName);
APPKIT_EXTERN const NSWindowDepth *NSAvailableWindowDepths(void); /* returns pointer to 0-terminated array */


/* Standard gray values for the 2-bit deep grayscale colorspace.
*/
APPKIT_EXTERN const CGFloat NSWhite;
APPKIT_EXTERN const CGFloat NSLightGray;
APPKIT_EXTERN const CGFloat NSDarkGray;
APPKIT_EXTERN const CGFloat NSBlack;

/* Approximate color gamut for use by NSScreen and NSWindow
 */
typedef NS_ENUM(NSInteger, NSDisplayGamut) {
    NSDisplayGamutSRGB = 1,
    NSDisplayGamutP3 
} API_AVAILABLE(macos(10.12));

/* Keys for deviceDescription dictionaries.
*/
typedef NSString * NSDeviceDescriptionKey NS_TYPED_EXTENSIBLE_ENUM;
APPKIT_EXTERN NSDeviceDescriptionKey NSDeviceResolution;		/* NSValue containing NSSize, basically dpi */
APPKIT_EXTERN NSDeviceDescriptionKey NSDeviceColorSpaceName;	/* NSString */
APPKIT_EXTERN NSDeviceDescriptionKey NSDeviceBitsPerSample;		/* NSValue containing int */
APPKIT_EXTERN NSDeviceDescriptionKey NSDeviceIsScreen;		/* "YES" or not there */
APPKIT_EXTERN NSDeviceDescriptionKey NSDeviceIsPrinter;		/* "YES" or not there */
APPKIT_EXTERN NSDeviceDescriptionKey NSDeviceSize;			/* NSValue containing NSSize */


/* Graphics functions
*/
APPKIT_EXTERN void NSRectFill(NSRect rect);
APPKIT_EXTERN void NSRectFillList(const NSRect *rects, NSInteger count);
APPKIT_EXTERN void NSRectFillListWithGrays(const NSRect *rects, const CGFloat *grays, NSInteger num);
APPKIT_EXTERN void NSRectFillListWithColors(const NSRect *rects,  NSColor * const _Nonnull * _Nonnull colors, NSInteger num);
APPKIT_EXTERN void NSRectFillUsingOperation(NSRect rect, NSCompositingOperation op);
APPKIT_EXTERN void NSRectFillListUsingOperation(const NSRect *rects, NSInteger count, NSCompositingOperation op);
APPKIT_EXTERN void NSRectFillListWithColorsUsingOperation(const NSRect *rects, NSColor * const _Nonnull * _Nonnull colors, NSInteger num, NSCompositingOperation op);
APPKIT_EXTERN void NSFrameRect(NSRect rect);
APPKIT_EXTERN void NSFrameRectWithWidth(NSRect rect, CGFloat frameWidth);
APPKIT_EXTERN void NSFrameRectWithWidthUsingOperation(NSRect rect, CGFloat frameWidth, NSCompositingOperation op);
APPKIT_EXTERN void NSRectClip(NSRect rect);
APPKIT_EXTERN void NSRectClipList(const NSRect *rects, NSInteger count);
APPKIT_EXTERN NSRect NSDrawTiledRects(NSRect boundsRect, NSRect clipRect, const NSRectEdge *sides, const CGFloat *grays, NSInteger count);
APPKIT_EXTERN void NSDrawGrayBezel(NSRect rect, NSRect clipRect);
APPKIT_EXTERN void NSDrawGroove(NSRect rect, NSRect clipRect);
APPKIT_EXTERN void NSDrawWhiteBezel(NSRect rect, NSRect clipRect);
APPKIT_EXTERN void NSDrawButton(NSRect rect, NSRect clipRect);
APPKIT_EXTERN void NSEraseRect(NSRect rect);
APPKIT_EXTERN NSColor * _Nullable NSReadPixel(NSPoint passedPoint) API_DEPRECATED("Use -[NSBitmapImageRep colorAtX:y:] to interrogate pixel values.  If necessary, use -[NSView cacheDisplayInRect:toBitmapImageRep:] to snapshot a view hierarchy into an NSBitmapImageRep.", macos(10.0,10.14));
APPKIT_EXTERN void NSDrawBitmap(NSRect rect, NSInteger width, NSInteger height, NSInteger bps, NSInteger spp, NSInteger bpp, NSInteger bpr, BOOL isPlanar, BOOL hasAlpha, NSColorSpaceName colorSpaceName, const unsigned char *const _Nullable data[_Nonnull 5]);

APPKIT_EXTERN void NSHighlightRect(NSRect rect) API_DEPRECATED("", macos(10.0,10.0));
APPKIT_EXTERN void NSBeep(void);

/* gets performance stats about window server memory usage */
APPKIT_EXTERN NSInteger NSGetWindowServerMemory(NSInteger context, NSInteger *virtualMemory, NSInteger *windowBackingMemory, NSString * _Nonnull * _Nonnull windowDumpString) API_DEPRECATED("Doesn't return anything useful since 10.0", macos(10.0,10.14));

APPKIT_EXTERN NSRect NSDrawColorTiledRects(NSRect boundsRect, NSRect clipRect, const NSRectEdge *sides, NSColor * _Nonnull * _Nonnull colors, NSInteger count);
APPKIT_EXTERN void NSDrawDarkBezel(NSRect rect, NSRect clipRect);
APPKIT_EXTERN void NSDrawLightBezel(NSRect rect, NSRect clipRect);
APPKIT_EXTERN void NSDottedFrameRect(NSRect rect);

APPKIT_EXTERN void NSDrawWindowBackground(NSRect rect);
APPKIT_EXTERN void NSSetFocusRingStyle(NSFocusRingPlacement placement);

APPKIT_EXTERN void NSDisableScreenUpdates(void) API_DEPRECATED("As of 10.11 it is not generally necessary to take explicit action to achieve visual atomicity. +[NSAnimationContext runAnimationGroup:] and other similar methods can be used when a stronger than normal need for visual atomicity is required. The NSAnimationContext methods do not suffer from the same performance problems as NSDisableScreenUpdates.", macos(10.0,10.11));

APPKIT_EXTERN void NSEnableScreenUpdates(void) API_DEPRECATED("As of 10.11 it is not generally necessary to take explicit action to achieve visual atomicity. +[NSAnimationContext runAnimationGroup:] and other similar methods can be used when a stronger than normal need for visual atomicity is required. The NSAnimationContext methods do not suffer from the same performance problems as NSEnableScreenUpdates.", macos(10.0,10.11));

/* Runs one of the standard system animation effects (display and sound).
** 'centerLocation' represents the center, in screen coordinates, to show the effect.
** 'size' specifies how big the effect should be.  Use NSZeroSize to get the default size.
** 'animationDelegate' is optionally, an object that wants to know when the effect has completed.
** 'didEndSelector' will be invoked in the animationDelegate when the animation has completed.
**  
** The didEndSelector should have the following signature:
** 	- (void)animationEffectDidEnd:(void *)contextInfo;
*/

typedef NS_ENUM(NSUInteger, NSAnimationEffect) {
	// The default effect used to indicate removal of an item from a collection, 
	// such as toolbar (indicates removal, without destroying the underlying data).
	NSAnimationEffectDisappearingItemDefault = 0,

	// An effect showing a puff of smoke.
	NSAnimationEffectPoof = 10
};

APPKIT_EXTERN void NSShowAnimationEffect(NSAnimationEffect animationEffect, NSPoint centerLocation, NSSize size, _Nullable id animationDelegate, _Nullable SEL didEndSelector, void * _Nullable contextInfo);

APPKIT_EXTERN void NSCountWindows(NSInteger *count) API_DEPRECATED("Use +[NSWindow windowNumbersWithOptions:] instead", macos(10.0,10.6));
APPKIT_EXTERN void NSWindowList(NSInteger size, NSInteger list[_Nonnull]) API_DEPRECATED("Use +[NSWindow windowNumbersWithOptions:] instead", macos(10.0,10.6));
APPKIT_EXTERN void NSCountWindowsForContext(NSInteger context, NSInteger *count) API_DEPRECATED("Use +[NSWindow windowNumbersWithOptions:] instead", macos(10.0,10.6));
APPKIT_EXTERN void NSWindowListForContext(NSInteger context, NSInteger size, NSInteger list[_Nonnull]) API_DEPRECATED("Use +[NSWindow windowNumbersWithOptions:] instead", macos(10.0,10.6));
APPKIT_EXTERN void NSCopyBits(NSInteger srcGState, NSRect srcRect, NSPoint destPoint) API_DEPRECATED("", macos(10.0,10.10));

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
