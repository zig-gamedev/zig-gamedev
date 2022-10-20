/*
    NSImageView.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/NSImageCell.h>
#import <AppKit/NSMenu.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSImageSymbolConfiguration;

@interface NSImageView : NSControl <NSAccessibilityImage, NSMenuItemValidation>

/*!
 Creates a non-editable image view containing the provided image. The image is scaled proportionally down to fit the view, and is centered within the view.
 @param image The image to display within the view.
 @return An initialized image view.
 */
+ (instancetype)imageViewWithImage:(NSImage *)image API_AVAILABLE(macos(10.12));

@property (nullable, strong) NSImage *image;

@property (getter=isEditable) BOOL editable;

@property NSImageAlignment imageAlignment;
@property NSImageScaling imageScaling;
@property NSImageFrameStyle imageFrameStyle;

/*!
 Specifies a combination of point size, weight, and scale to use when sizing and displaying symbol images. If a symbol configuration isn't provided, the image view uses a default size, weight, and scale provided by the system. The default value is `nil`.
 */
@property (nullable, copy) NSImageSymbolConfiguration *symbolConfiguration API_AVAILABLE(macos(11));

/*!
 A tint color to be used when rendering template image content. This color may be combined with other effects to produce a theme-appropriate rendition of the template image. A nil value indicates the standard set of effects without color modification. The default value is nil.
 */
@property (nullable, copy) NSColor *contentTintColor API_AVAILABLE(macos(10.14));

@property BOOL animates;

@property BOOL allowsCutCopyPaste;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END


