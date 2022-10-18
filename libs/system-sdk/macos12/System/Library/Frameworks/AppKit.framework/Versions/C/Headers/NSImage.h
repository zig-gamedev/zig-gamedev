/*
	NSImage.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSBundle.h>
#import <AppKit/NSGraphics.h>
#import <AppKit/NSBitmapImageRep.h>
#import <AppKit/NSPasteboard.h>
#import <AppKit/NSLayoutConstraint.h>
#import <AppKit/NSFontDescriptor.h>
#import <AppKit/AppKitDefines.h>
#import <ApplicationServices/ApplicationServices.h>


#define NSIMAGE_UNAVAILABLE_MACCATALYST TARGET_OS_IPHONE


NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor, NSImageRep, NSGraphicsContext, NSURL, NSImageSymbolConfiguration;
@protocol NSImageDelegate;

typedef NSString * NSImageName NS_SWIFT_BRIDGED_TYPEDEF API_AVAILABLE(ios(13.0));

APPKIT_EXTERN NSImageHintKey const NSImageHintCTM API_AVAILABLE(macos(10.6)); // value is NSAffineTransform
APPKIT_EXTERN NSImageHintKey const NSImageHintInterpolation API_AVAILABLE(macos(10.6)); // value is NSNumber with NSImageInterpolation enum value
APPKIT_EXTERN NSImageHintKey const NSImageHintUserInterfaceLayoutDirection API_AVAILABLE(macos(10.12)); // value is NSNumber with NSUserInterfaceLayoutDirection enum value

typedef NS_ENUM(NSUInteger, NSImageLoadStatus) {
    NSImageLoadStatusCompleted,
    NSImageLoadStatusCancelled,
    NSImageLoadStatusInvalidData,
    NSImageLoadStatusUnexpectedEOF,
    NSImageLoadStatusReadError
};

typedef NS_ENUM(NSUInteger, NSImageCacheMode) {
    NSImageCacheDefault,    // unspecified. use image rep's default
    NSImageCacheAlways,     // always generate a cache when drawing
    NSImageCacheBySize,     // cache if cache size is smaller than original data
    NSImageCacheNever       // never cache, always draw direct
};

typedef NS_ENUM(NSInteger, NSImageResizingMode) {
#if !TARGET_ABI_USES_IOS_VALUES
    NSImageResizingModeStretch = 0,
    NSImageResizingModeTile = 1,
#else /* !TARGET_ABI_USES_IOS_VALUES */
    NSImageResizingModeTile = 0,
    NSImageResizingModeStretch = 1,
#endif /* !TARGET_ABI_USES_IOS_VALUES */
} API_AVAILABLE(macos(10.10));

API_AVAILABLE(ios(13.0))
#if NSIMAGE_UNAVAILABLE_MACCATALYST
__attribute__((objc_subclassing_restricted))
#endif /* NSIMAGE_UNAVAILABLE_MACCATALYST */
@interface NSImage : NSObject

#if NSIMAGE_UNAVAILABLE_MACCATALYST
- (instancetype)init API_UNAVAILABLE(ios);
+ (instancetype)new API_UNAVAILABLE(ios);
+ (instancetype)allocWithZone:(nullable NSZone *)zone API_UNAVAILABLE(ios);
+ (instancetype)alloc API_UNAVAILABLE(ios);
#endif /* NSIMAGE_UNAVAILABLE_MACCATALYST */

+ (nullable NSImage *)imageNamed:(NSImageName)name;	/* If this finds & creates the image, only name is saved when archived */

+ (nullable instancetype)imageWithSystemSymbolName:(NSString *)symbolName accessibilityDescription:(nullable NSString *)description API_AVAILABLE(macos(11.0));

- (instancetype)initWithSize:(NSSize)size NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)initWithData:(NSData *)data;			/* When archived, saves contents */
- (nullable instancetype)initWithContentsOfFile:(NSString *)fileName;	/* When archived, saves contents */
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url;               /* When archived, saves contents */
- (nullable instancetype)initByReferencingFile:(NSString *)fileName;	/* When archived, saves fileName */
- (instancetype)initByReferencingURL:(NSURL *)url;		/* When archived, saves url, supports progressive loading */
- (nullable instancetype)initWithPasteboard:(NSPasteboard *)pasteboard;

// not for general use, but useful for compatibility with old NSImage behavior.  Ignore exif orientation tags in JPEG and such.  See AppKit release notes.
- (nullable instancetype)initWithDataIgnoringOrientation:(NSData *)data API_AVAILABLE(macos(10.6));

// Note that the block passed to the below method may be invoked whenever and on whatever thread the image itself is drawn on. Care should be taken to ensure that all state accessed within the drawingHandler block is done so in a thread safe manner.
+ (instancetype)imageWithSize:(NSSize)size flipped:(BOOL)drawingHandlerShouldBeCalledWithFlippedContext drawingHandler:(BOOL (^)(NSRect dstRect))drawingHandler API_AVAILABLE(macos(10.8));

@property NSSize size;
- (BOOL)setName:(nullable NSImageName)string;
- (nullable NSImageName)name;
@property (copy) NSColor *backgroundColor;
@property BOOL usesEPSOnResolutionMismatch;
@property BOOL prefersColorMatch;
@property BOOL matchesOnMultipleResolution;
@property BOOL matchesOnlyOnBestFittingAxis API_AVAILABLE(macos(10.7)); // Available in MacOSX 10.7.4 // Available in MacOSX 10.7.4

- (void)drawAtPoint:(NSPoint)point fromRect:(NSRect)fromRect operation:(NSCompositingOperation)op fraction:(CGFloat)delta;
- (void)drawInRect:(NSRect)rect fromRect:(NSRect)fromRect operation:(NSCompositingOperation)op fraction:(CGFloat)delta;
- (void)drawInRect:(NSRect)dstSpacePortionRect fromRect:(NSRect)srcSpacePortionRect operation:(NSCompositingOperation)op fraction:(CGFloat)requestedAlpha respectFlipped:(BOOL)respectContextIsFlipped hints:(nullable NSDictionary<NSImageHintKey, id> *)hints API_AVAILABLE(macos(10.6));
- (BOOL)drawRepresentation:(NSImageRep *)imageRep inRect:(NSRect)rect;

/* This is exactly equivalent to calling -[image drawInRect:rect fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:1 respectFlipped:YES hints:nil].
 */
- (void)drawInRect:(NSRect)rect API_AVAILABLE(macos(10.9));

- (void)recache;
@property (nullable, readonly, strong) NSData *TIFFRepresentation;
- (nullable NSData *)TIFFRepresentationUsingCompression:(NSTIFFCompression)comp factor:(float)factor;

@property (readonly, copy) NSArray<NSImageRep *> *representations;
- (void)addRepresentations:(NSArray<NSImageRep *> *)imageReps;
- (void)addRepresentation:(NSImageRep *)imageRep;
- (void)removeRepresentation:(NSImageRep *)imageRep;

@property (getter=isValid, readonly) BOOL valid;
- (void)lockFocus;
- (void)lockFocusFlipped:(BOOL)flipped API_AVAILABLE(macos(10.6));
- (void)unlockFocus;

@property (nullable, weak) id<NSImageDelegate> delegate;

@property (class, readonly, copy) NSArray<NSString *> *imageTypes API_AVAILABLE(macos(10.5));
@property (class, readonly, copy) NSArray<NSString *> *imageUnfilteredTypes API_AVAILABLE(macos(10.5));

+ (BOOL)canInitWithPasteboard:(NSPasteboard *)pasteboard;

- (void)cancelIncrementalLoad;

@property NSImageCacheMode cacheMode;

/* The alignmentRect of an image is metadata that a client may use to help determine layout. The bottom of the rect gives the baseline of the image. The other edges give similar information in other directions.
 
 A 20x20 image of a phone icon with a glow might specify an alignmentRect of {{2,2},{16,16}} that excludes the glow. NSButtonCell can take advantage of the alignmentRect to place the image in the same visual location as an 16x16 phone icon without the glow. A 5x5 star that should render high when aligned with text might specify a rect of {{0,-7},{5,12}}.
 
 The alignmentRect of an image has no effect on methods such as drawInRect:fromRect:operation:Fraction: or drawAtPoint:fromRect:operation:fraction:. It is the client's responsibility to take the alignmentRect into account where applicable.
 
 The default alignmentRect of an image is {{0,0},imageSize}. The rect is adjusted when setSize: is called. 
 */
@property NSRect alignmentRect API_AVAILABLE(macos(10.5));

/* The 'template' property is metadata that allows clients to be smarter about image processing.  An image should be marked as a template if it is basic glpyh-like black and white art that is intended to be processed into derived images for use on screen.
 
 NSButtonCell applies effects to images based on the state of the button.  For example, images are shaded darker when the button is pressed.  If a template image is set on a cell, the cell can apply more sophisticated effects.  For example, it may be processed into an image that looks engraved when drawn into a cell whose interiorBackgroundStyle is NSBackgroundStyleRaised, like on a textured button.
 */
#if defined(__cplusplus)
- (BOOL)isTemplate API_AVAILABLE(macos(10.5));
- (void)setTemplate:(BOOL)isTemplate API_AVAILABLE(macos(10.5));
#else
@property (getter=isTemplate) BOOL template API_AVAILABLE(macos(10.5));
#endif


/* An accessibility description can be set on an image.  This description will be used automatically by interface elements that display images.  Like all accessibility descriptions, the string should be a short localized string that does not include the name of the interface element.  For instance, "delete" rather than "delete button". 
*/
@property (nullable, copy) NSString *accessibilityDescription API_AVAILABLE(macos(10.6));

/* Make an NSImage referencing a CGImage.  The client should not assume anything about the image, other than that drawing it is equivalent to drawing the CGImage.
 
 If size is NSZeroSize, the pixel dimensions of cgImage are the returned image's size.   
 
 This is not a designated initializer.
 
 Size of an NSImage is distinct from pixel dimensions.  If an NSImage is placed in an NSButton, it will be drawn in a rect with the provided size in the ambient coordinate system.
 */
- (instancetype)initWithCGImage:(CGImageRef)cgImage size:(NSSize)size API_AVAILABLE(macos(10.6));

/* Returns a CGImage capturing the drawing of the receiver.  This method returns an existing CGImage if one is available, or creates one if not.  It behaves the same as drawing the image with respect to caching and related behaviors.  This method is typically called, not overridden.  
 
 An NSImage is potentially resolution independent, and may have representations that allow it to draw well in many contexts.  A CGImage is more like a single pixel-based representation.   This method produces a snapshot of how the NSImage would draw if it was asked to draw in *proposedDestRect in the passed context.  Producing this snapshot may be more expensive than just drawing the NSImage, so prefer to use -[NSImage drawInRect:fromRect:operation:fraction:] unless you require a CGImage.
 
 The return value in *proposedDestRect tells the client where to draw the CGImage.  This rect may be outset from the requested rect, because a CGImage must have pixel-integral dimensions while an NSImage need not.
 
 All input parameters are optional.  They provide hints for how to choose among existing CGImages, or how to create one if there isn't already a CGImage available.  The parameters are _only_ hints.  Any CGImage is a valid return.
 
 If proposedDestRect is NULL, it defaults to the smallest pixel-integral rectangle containing {{0,0}, [self size]}.  The proposedDestRect is in user space in the reference context. 
 
 If referenceContext is nil, the method behaves as if a window context scaled by the default user space scaling factor was passed, though no context is actually created.  The properties of the context are used as hints for choosing the best representation and for creating a CGImage if creation is necessary.  It also provides the coordinate space in which the proposedDestRect is interpreted.  Only the snapshotted state of the context at the moment its passed to this method is relevant. Future changes to the context have no bearing on image behavior.
 
 The hints provide more context for selecting or generating a CGImage, and may override properties of the referenceContext.  Hints may be nil.  Any entries in a device description dictionary (see NSScreen) are valid, as are all CIContext creation options, plus a few extra hints defined below.  Unrecognized hints are ignored, but passed down to image reps (see -[NSImageRep CGImageForProposedRect:context:hints:]).  Explicit hints are particularly useful when it is not draw time and you don't have a context to pass in.  For example, if you want to pass a rect in pixels for proposedDestRect, you should pass a dictionary with the identity transform for NSImageHintCTM.  
  
 This method will always return a valid CGImage provided the NSImage is able to draw.  If the receiver is unable to draw for whatever reason, the error behavior is the same as when drawing the image.
 
 The CGImageRef returned is guaranteed to live as long as the current autorelease pool.  The caller should not release the CGImage.  This is the standard Cocoa convention, but people may not realize that it applies to CFTypes.
 */
- (nullable CGImageRef)CGImageForProposedRect:(nullable NSRect *)proposedDestRect context:(nullable NSGraphicsContext *)referenceContext hints:(nullable NSDictionary<NSImageHintKey, id> *)hints API_AVAILABLE(macos(10.6)) CF_RETURNS_NOT_RETAINED;

/* Select best representation.  The parameters have the same meaning and behavior as in -CGImageForProposedRect:context:hints:.
 */
- (nullable NSImageRep *)bestRepresentationForRect:(NSRect)rect context:(nullable NSGraphicsContext *)referenceContext hints:(nullable NSDictionary<NSImageHintKey, id> *)hints API_AVAILABLE(macos(10.6));

/* Answers the question, "If you were to draw the image in the passed destination rect in the passed context respecting the passed flippedness with the passed hints, would the test rect in the context intersect a non-transparent portion of the image?"
 */
- (BOOL)hitTestRect:(NSRect)testRectDestSpace withImageDestinationRect:(NSRect)imageRectDestSpace context:(nullable NSGraphicsContext *)context hints:(nullable NSDictionary<NSImageHintKey, id> *)hints flipped:(BOOL)flipped API_AVAILABLE(macos(10.6));

- (CGFloat)recommendedLayerContentsScale:(CGFloat)preferredContentsScale API_AVAILABLE(macos(10.7));
- (id)layerContentsForContentsScale:(CGFloat)layerContentsScale API_AVAILABLE(macos(10.7));

@property NSEdgeInsets capInsets API_AVAILABLE(macos(10.10));
@property NSImageResizingMode resizingMode API_AVAILABLE(macos(10.10));

- (nullable NSImage *)imageWithSymbolConfiguration:(NSImageSymbolConfiguration *)configuration API_AVAILABLE(macos(11.0));

@property (readonly, copy) NSImageSymbolConfiguration *symbolConfiguration API_AVAILABLE(macos(12.0));

@end

#if !NSIMAGE_UNAVAILABLE_MACCATALYST
@interface NSImage () <NSCopying, NSSecureCoding, NSPasteboardReading, NSPasteboardWriting>
@end
#endif /* !NSIMAGE_UNAVAILABLE_MACCATALYST */

@protocol NSImageDelegate <NSObject>
@optional

- (nullable NSImage *)imageDidNotDraw:(NSImage *)sender inRect:(NSRect)rect;

- (void)image:(NSImage *)image willLoadRepresentation:(NSImageRep *)rep;
- (void)image:(NSImage *)image didLoadRepresentationHeader:(NSImageRep *)rep;
- (void)image:(NSImage *)image didLoadPartOfRepresentation:(NSImageRep *)rep withValidRows:(NSInteger)rows; 
- (void)image:(NSImage *)image didLoadRepresentation:(NSImageRep *)rep withStatus:(NSImageLoadStatus)status;
@end

@interface NSBundle(NSBundleImageExtension)
- (nullable NSImage *)imageForResource:(NSImageName)name API_AVAILABLE(macos(10.7)); /* May return nil if no file found */

/* Neither of the following methods can return images with multiple representations in different files (for example, MyImage.png and MyImage@2x.png.) The above method is generally prefered.
 */
- (nullable NSString *)pathForImageResource:(NSImageName)name;	/* May return nil if no file found */
- (nullable NSURL *)URLForImageResource:(NSImageName)name API_AVAILABLE(macos(10.6)); /* May return nil if no file found */
@end

#pragma mark - Deprecated declarations

@interface NSImage ()

- (null_unspecified NSImageRep *)bestRepresentationForDevice:(null_unspecified NSDictionary *)deviceDescription API_DEPRECATED("Use -[NSImage bestRepresentationForRect:context:hints:] instead.  Any deviceDescription dictionary is also a valid hints dictionary.", macos(10.0,10.6));

/* These return union of all the types registered with NSImageRep.
 */
+ (NSArray<NSString *> *)imageUnfilteredFileTypes API_DEPRECATED("Use +imageUnfilteredTypes instead", macos(10.0,10.10));
+ (NSArray<NSPasteboardType> *)imageUnfilteredPasteboardTypes API_DEPRECATED("Use +imageUnfilteredTypes instead", macos(10.0,10.10));
+ (NSArray<NSString *> *)imageFileTypes API_DEPRECATED("Use +imageTypes instead", macos(10.0,10.10));
+ (NSArray<NSPasteboardType> *)imagePasteboardTypes API_DEPRECATED("Use +imageTypes instead", macos(10.0,10.10));

#if TARGET_OS_OSX
- (instancetype)initWithIconRef:(IconRef)iconRef API_DEPRECATED("Use -[NSWorkspace iconForFile:], -[NSWorkspace iconForFiles:], -[NSWorkspace iconForFileType:], or +[NSImage imageNamed:] instead.", macos(10.5, 11.0));
#endif // TARGET_OS_OSX

- (void)setFlipped:(BOOL)flag API_DEPRECATED("The concept of flippedness for NSImage is deprecated.  Please see the AppKit 10.6 release notes for a discussion of why and for how to replace existing usage.", macos(10.0,10.6));
- (BOOL)isFlipped API_DEPRECATED("The concept of flippedness for NSImage is deprecated.  Please see the AppKit 10.6 release notes for a discussion of why and for how to replace existing usage.", macos(10.0,10.6));

// These methods have surprising semantics.  Prefer to use the 'draw' methods (and note the new draw method taking respectContextIsFlipped as a parameter).  Please see the AppKit 10.6 release notes for exactly what's going on.
- (void)dissolveToPoint:(NSPoint)point fraction:(CGFloat)fraction API_DEPRECATED("Use -drawAtPoint:... or -drawInRect:... methods instead", macos(10.0,10.6));
- (void)dissolveToPoint:(NSPoint)point fromRect:(NSRect)rect fraction:(CGFloat)fraction API_DEPRECATED("Use -drawAtPoint:... or -drawInRect:... methods instead", macos(10.0,10.6));
- (void)compositeToPoint:(NSPoint)point operation:(NSCompositingOperation)op API_DEPRECATED("Use -drawAtPoint:... or -drawInRect:... methods instead", macos(10.0,10.6));
- (void)compositeToPoint:(NSPoint)point fromRect:(NSRect)rect operation:(NSCompositingOperation)op API_DEPRECATED("Use -drawAtPoint:... or -drawInRect:... methods instead", macos(10.0,10.6));
- (void)compositeToPoint:(NSPoint)point operation:(NSCompositingOperation)op fraction:(CGFloat)delta API_DEPRECATED("Use -drawAtPoint:... or -drawInRect:... methods instead", macos(10.0,10.6));
- (void)compositeToPoint:(NSPoint)point fromRect:(NSRect)rect operation:(NSCompositingOperation)op fraction:(CGFloat)delta API_DEPRECATED("Use -drawAtPoint:... or -drawInRect:... methods instead", macos(10.0,10.6));

// This method doesn't do what people expect.  See AppKit 10.6 release notes.  Briefly, you can replace invocation of this method with code that locks focus on the image and then draws the rep in the image.
- (void)lockFocusOnRepresentation:(null_unspecified NSImageRep *)imageRepresentation API_DEPRECATED("Use -lockFocus followed by -[NSImageRep drawInRect:] instead. See documentation for more info.", macos(10.0,10.6));

- (void)setScalesWhenResized:(BOOL)flag API_DEPRECATED("You should be able to remove use of this method without any replacement.  See 10.6 AppKit release notes for details.", macos(10.0,10.6));
- (BOOL)scalesWhenResized API_DEPRECATED("You should be able to remove use of this method without any replacement.  See 10.6 AppKit release notes for details.", macos(10.0,10.6));
- (void)setDataRetained:(BOOL)flag API_DEPRECATED("You should be able to remove use of this method without any replacement.  See 10.6 AppKit release notes for details.", macos(10.0,10.6));
- (BOOL)isDataRetained API_DEPRECATED("You should be able to remove use of this method without any replacement.  See 10.6 AppKit release notes for details.", macos(10.0,10.6));
- (void)setCachedSeparately:(BOOL)flag API_DEPRECATED("You should be able to remove use of this method without any replacement.  See 10.6 AppKit release notes for details.", macos(10.0,10.6));
- (BOOL)isCachedSeparately API_DEPRECATED("You should be able to remove use of this method without any replacement.  See 10.6 AppKit release notes for details.", macos(10.0,10.6));
- (void)setCacheDepthMatchesImageDepth:(BOOL)flag API_DEPRECATED("You should be able to remove use of this method without any replacement.  See 10.6 AppKit release notes for details.", macos(10.0,10.6));
- (BOOL)cacheDepthMatchesImageDepth API_DEPRECATED("You should be able to remove use of this method without any replacement.  See 10.6 AppKit release notes for details.", macos(10.0,10.6));

@end

#pragma mark - Standard images

/* Standard images.
 
 Most images are named by a specific function or situation where they are intended to be used.  In some cases, the artwork may be more generic than the name.  For example, the image for NSImageNameInvalidDataFreestandingTemplate is an arrow in 10.5.  Please do not use an image outside of the function for which it is intended - the artwork can change between releases.  The invalid data image could change to a yellow exclamation-point-in-triangle icon.  If there is no image available for the situation you're interested in, please file a bug report, and use your own custom art in the meantime.
 
 The size of an image is also not guaranteed to be the same (or maintain the same aspect ratio) between releases, so you should explcitly size the image appropriately for your use.

 Constants that end in the word "Template" name black and clear images that return YES for isTemplate.  These images can be processed into variants appropriate for different situations.  For example, these images can invert in a selected table view row.  See -[NSImage setTemplate:] for more comments.  These images are inappropriate for display without further processing, but NSCell and its subclasses will perform the processing.
 
 Some images also contain the word "Freestanding".  This indicates that an image is appropriate for use as a borderless button - it doesn't need any extra bezel artwork behind it.  For example, Safari uses NSImageNameStopProgressTemplate as the stop button in a button on its toolbar, while it uses NSImageNameStopProgressFreestandingTemplate in the downloads window where it appears inline with a progress indicator.
  
 The string value of each symbol is typically the same as the constant name without the "ImageName" part.  For example, NSImageNameBonjour is @"NSBonjour".  This is documented so that images can be used by name in Interface Builder.
 */

APPKIT_EXTERN NSImageName const NSImageNameAddTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameBluetoothTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameBonjour API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameBookmarksTemplate API_AVAILABLE(macos(10.6), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameCaution API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSImageName const NSImageNameComputer API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameEnterFullScreenTemplate API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameExitFullScreenTemplate API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameFolder API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSImageName const NSImageNameFolderBurnable API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameFolderSmart API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameFollowLinkFreestandingTemplate API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameHomeTemplate API_AVAILABLE(macos(10.6), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameIChatTheaterTemplate API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameLockLockedTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameLockUnlockedTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameNetwork API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNamePathTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameQuickLookTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameRefreshFreestandingTemplate API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameRefreshTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameRemoveTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameRevealFreestandingTemplate API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameShareTemplate API_AVAILABLE(macos(10.8), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameSlideshowTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameStatusAvailable API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSImageName const NSImageNameStatusNone API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSImageName const NSImageNameStatusPartiallyAvailable API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSImageName const NSImageNameStatusUnavailable API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSImageName const NSImageNameStopProgressFreestandingTemplate API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameStopProgressTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTrashEmpty API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSImageName const NSImageNameTrashFull API_AVAILABLE(macos(10.6));

/* This image is appropriate on an 'action' button.  An action button is a popup that has the same contents as the contextual menu for a related control.
 */
APPKIT_EXTERN NSImageName const NSImageNameActionTemplate API_AVAILABLE(macos(10.5), ios(13.0));

/* This image can be used as a badge for a 'smart' item.  In 10.5, this and the 'action' image are both gears.  Please avoid using a gear for other situations, and if you do, use custom art.
 */
APPKIT_EXTERN NSImageName const NSImageNameSmartBadgeTemplate API_AVAILABLE(macos(10.5));

/* These images are intended for use in a segmented control for switching view interfaces for another part of the window.
 */
APPKIT_EXTERN NSImageName const NSImageNameIconViewTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameListViewTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameColumnViewTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameFlowViewTemplate API_AVAILABLE(macos(10.5));

/* Place this image to the right of invalid data.  For example, use it if the user tries to commit a form when it's missing a required name field.
 */
APPKIT_EXTERN NSImageName const NSImageNameInvalidDataFreestandingTemplate API_AVAILABLE(macos(10.5));

/* Use these images for "go forward" or "go back" functions, as seen in Safari's toolbar.  These images will automatically mirror when the user interface layout direction is right to left.
 */
APPKIT_EXTERN NSImageName const NSImageNameGoForwardTemplate API_AVAILABLE(macos(10.12), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameGoBackTemplate API_AVAILABLE(macos(10.12), ios(13.0));

/* These images are like GoForward and GoBack except that they always point in the specified direction regardless of layout direction.  See also the right and left facing triangle images.
 */
APPKIT_EXTERN NSImageName const NSImageNameGoRightTemplate API_AVAILABLE(macos(10.5), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameGoLeftTemplate API_AVAILABLE(macos(10.5), ios(13.0));

/* Prefer the GoForward and GoBack or GoLeft and GoRight images for situations where they apply.  These generic triangles aren't endorsed for any particular use, but you can use them if you don't have any better art.
 */
APPKIT_EXTERN NSImageName const NSImageNameRightFacingTriangleTemplate API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameLeftFacingTriangleTemplate API_AVAILABLE(macos(10.5));

/* NSImageNameDotMac will continue to work for the forseeable future, and will return the same image as NSImageNameMobileMe.
 */
APPKIT_EXTERN NSImageName const NSImageNameDotMac API_DEPRECATED_WITH_REPLACEMENT("NSImageNameMobileMe", macos(10.5,10.7));
APPKIT_EXTERN NSImageName const NSImageNameMobileMe API_AVAILABLE(macos(10.6));

/* This image is appropriate as a drag image for multiple items.
 */
APPKIT_EXTERN NSImageName const NSImageNameMultipleDocuments API_AVAILABLE(macos(10.5));

/* These images are intended for use in toolbars in preference windows.
 */
APPKIT_EXTERN NSImageName const NSImageNameUserAccounts API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNamePreferencesGeneral API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameAdvanced API_AVAILABLE(macos(10.5));

/* These images are intended for use in other toolbars.
 */
APPKIT_EXTERN NSImageName const NSImageNameInfo API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameFontPanel API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameColorPanel API_AVAILABLE(macos(10.5));

/* These images are appropriate for use in sharing or permissions interfaces.
 */
APPKIT_EXTERN NSImageName const NSImageNameUser API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameUserGroup API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSImageName const NSImageNameEveryone API_AVAILABLE(macos(10.5));  
APPKIT_EXTERN NSImageName const NSImageNameUserGuest API_AVAILABLE(macos(10.6));

/* These images are the default state images used by NSMenuItem.  Drawing these outside of menus is discouraged.
*/
APPKIT_EXTERN NSImageName const NSImageNameMenuOnStateTemplate API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSImageName const NSImageNameMenuMixedStateTemplate API_AVAILABLE(macos(10.6));

/* The name @"NSApplicationIcon" has been available since Mac OS X 10.0.  The symbol NSImageNameApplicationIcon is new in 10.6.
 */
APPKIT_EXTERN NSImageName const NSImageNameApplicationIcon API_AVAILABLE(macos(10.6));

#pragma mark - NSTouchBar images

/* These images are appropriate for use only in NSTouchBar.
 */
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAddDetailTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAddTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAlarmTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAudioInputMuteTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAudioInputTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAudioOutputMuteTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAudioOutputVolumeHighTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAudioOutputVolumeLowTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAudioOutputVolumeMediumTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarAudioOutputVolumeOffTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarBookmarksTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarColorPickerFill API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarColorPickerFont API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarColorPickerStroke API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarCommunicationAudioTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarCommunicationVideoTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarComposeTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarDeleteTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarDownloadTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarEnterFullScreenTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarExitFullScreenTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarFastForwardTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarFolderCopyToTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarFolderMoveToTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarFolderTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarGetInfoTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarGoBackTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarGoDownTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarGoForwardTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarGoUpTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarHistoryTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarIconViewTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarListViewTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarMailTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarNewFolderTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarNewMessageTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarOpenInBrowserTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarPauseTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarPlayPauseTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarPlayTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarQuickLookTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarRecordStartTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarRecordStopTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarRefreshTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarRemoveTemplate API_AVAILABLE(macos(10.13), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarRewindTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarRotateLeftTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarRotateRightTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSearchTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarShareTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSidebarTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSkipAhead15SecondsTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSkipAhead30SecondsTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSkipAheadTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSkipBack15SecondsTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSkipBack30SecondsTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSkipBackTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSkipToEndTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSkipToStartTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarSlideshowTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTagIconTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextBoldTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextBoxTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextCenterAlignTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextItalicTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextJustifiedAlignTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextLeftAlignTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextListTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextRightAlignTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextStrikethroughTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarTextUnderlineTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarUserAddTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarUserGroupTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarUserTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));

/* If you have a volume indicator, use NSImageNameTouchBarAudioOutputVolume{Off,Low,Medium,High}Template, which align the speaker correctly.  For volume controls, use NSImageNameTouchBarVolume{Down,Up}Template.
 */
APPKIT_EXTERN NSImageName const NSImageNameTouchBarVolumeDownTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));
APPKIT_EXTERN NSImageName const NSImageNameTouchBarVolumeUpTemplate API_AVAILABLE(macos(10.12.2), ios(13.0));

/* If you have an NSTouchBarItem with a seekable media control, NSImageNameTouchBarPlayheadTemplate is suitable for use in displaying the playhead.
 */
APPKIT_EXTERN NSImageName const NSImageNameTouchBarPlayheadTemplate API_AVAILABLE(macos(10.12.2));

typedef NS_ENUM(NSInteger, NSImageSymbolScale) {
    NSImageSymbolScaleSmall = 1,
    NSImageSymbolScaleMedium = 2,
    NSImageSymbolScaleLarge = 3,
} NS_SWIFT_NAME(NSImage.SymbolScale) API_AVAILABLE(macos(11.0));

API_AVAILABLE(macos(11.0)) NS_SWIFT_NAME(NSImage.SymbolConfiguration)
@interface NSImageSymbolConfiguration : NSObject <NSCopying, NSSecureCoding>

+ (instancetype)configurationWithPointSize:(CGFloat)pointSize weight:(NSFontWeight)weight scale:(NSImageSymbolScale)scale;
+ (instancetype)configurationWithPointSize:(CGFloat)pointSize weight:(NSFontWeight)weight;
+ (instancetype)configurationWithTextStyle:(NSFontTextStyle)style scale:(NSImageSymbolScale)scale;
+ (instancetype)configurationWithTextStyle:(NSFontTextStyle)style;
+ (instancetype)configurationWithScale:(NSImageSymbolScale)scale;

/*
 Create a color configuration with a palette derived from one color.

 A color scheme will be created based on the provided color, deriving
 secondary and tertiary colors by reducing the intensity of the base color.
 This is typically (but not only) accomplished by reducing opacity of the
 primary color.

 When combined with another configuration creating a palette, the
 last specified configuration will win, overwriting the other color
 configuration.

 If the symbol doesn't have a palette-based variant, the configuration will
 have no effect and the result will be a monochrome (templated) symbol.
 */
+ (instancetype)configurationWithHierarchicalColor:(NSColor *)hierarchicalColor API_AVAILABLE(macos(12.0));

/*
 Create a color configuration by specifying a palette of colors.
 The colors are used sequentially per layer: the first color for the first
 layer, the second color for the second layer etc. This is independent of
 the hierarchy level of the layer.

 When combined with another configuration creating a palette, the
 last specified configuration will win, overwriting the other color
 configuration.

 If the symbol doesn't have a palette-based variant, the configuration will
 have no effect and the result will be a monochrome (templated) symbol.
 */
+ (instancetype)configurationWithPaletteColors:(NSArray<NSColor *> *)paletteColors API_AVAILABLE(macos(12.0));

/*
 Create a configuration that specifies that the symbol should prefer
 its multicolor variant if one exists.

 This configuration can be combined with one of the palette-based
 configurations; in that case, the symbol will use the multicolor
 variant if one exists, or the palette variant otherwise.

 If the symbol supports neither, the result will be a monochrome
 (templated) symbol.
 */
+ (instancetype)configurationPreferringMulticolor NS_SWIFT_NAME(preferringMulticolor()) API_AVAILABLE(macos(12.0));

/*
 Returns a new configuration object whose values are defined by
 applying values from the provided configuration and the receiver.

 Values defined by both configurations will use the provided
 configuration's values.
 */
- (instancetype)configurationByApplyingConfiguration:(NSImageSymbolConfiguration *)configuration API_AVAILABLE(macos(12.0));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
