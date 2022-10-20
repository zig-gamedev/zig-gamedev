/* CoreAnimation - CATextLayer.h

   Copyright (c) 2006-2021, Apple Inc.
   All rights reserved. */

#import <QuartzCore/CALayer.h>

/* The text layer provides simple text layout and rendering of plain
 * or attributed strings. The first line is aligned to the top of the
 * layer. */

NS_ASSUME_NONNULL_BEGIN

typedef NSString * CATextLayerTruncationMode NS_TYPED_ENUM;
typedef NSString * CATextLayerAlignmentMode NS_TYPED_ENUM;

API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0))
@interface CATextLayer : CALayer
{
@private
  struct CATextLayerPrivate *_state;
}

/* The text to be rendered, should be either an NSString or an
 * NSAttributedString. Defaults to nil. */

@property(nullable, copy) id string;

/* The font to use, currently may be either a CTFontRef (toll-free
 * bridged from NSFont), a CGFontRef, or a string naming the font.
 * Defaults to the Helvetica font. Only used when the `string' property
 * is not an NSAttributedString. */

@property(nullable) CFTypeRef font;

/* The font size. Defaults to 36. Only used when the `string' property
 * is not an NSAttributedString. Animatable (Mac OS X 10.6 and later.) */

@property CGFloat fontSize;

/* The color object used to draw the text. Defaults to opaque white.
 * Only used when the `string' property is not an NSAttributedString.
 * Animatable (Mac OS X 10.6 and later.) */

@property(nullable) CGColorRef foregroundColor;

/* When true the string is wrapped to fit within the layer bounds.
 * Defaults to NO.*/

@property(getter=isWrapped) BOOL wrapped;

/* Describes how the string is truncated to fit within the layer
 * bounds. The possible options are `none', `start', `middle' and
 * `end'. Defaults to `none'. */

@property(copy) CATextLayerTruncationMode truncationMode;

/* Describes how individual lines of text are aligned within the layer
 * bounds. The possible options are `natural', `left', `right',
 * `center' and `justified'. Defaults to `natural'. */

@property(copy) CATextLayerAlignmentMode alignmentMode;

/* Sets allowsFontSubpixelQuantization parameter of CGContextRef
 * passed to the -drawInContext: method. Defaults to NO. */

@property BOOL allowsFontSubpixelQuantization;

@end

/* Truncation modes. */

CA_EXTERN CATextLayerTruncationMode const kCATruncationNone
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
CA_EXTERN CATextLayerTruncationMode const kCATruncationStart
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
CA_EXTERN CATextLayerTruncationMode const kCATruncationEnd
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
CA_EXTERN CATextLayerTruncationMode const kCATruncationMiddle
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/* Alignment modes. */

CA_EXTERN CATextLayerAlignmentMode const kCAAlignmentNatural
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
CA_EXTERN CATextLayerAlignmentMode const kCAAlignmentLeft
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
CA_EXTERN CATextLayerAlignmentMode const kCAAlignmentRight
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
CA_EXTERN CATextLayerAlignmentMode const kCAAlignmentCenter
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
CA_EXTERN CATextLayerAlignmentMode const kCAAlignmentJustified
    API_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

NS_ASSUME_NONNULL_END
