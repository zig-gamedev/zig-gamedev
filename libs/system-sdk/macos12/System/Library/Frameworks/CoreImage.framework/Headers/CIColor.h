/* 
   CoreImage - CIColor.h

   Copyright (c) 2015 Apple, Inc.
   All rights reserved. 
*/

#import <CoreImage/CoreImageDefines.h>
#import <Foundation/Foundation.h>
#import <CoreImage/CIVector.h>

NS_ASSUME_NONNULL_BEGIN

NS_CLASS_AVAILABLE(10_4, 5_0)
@interface CIColor : NSObject <NSSecureCoding, NSCopying>
{
    void *_priv;
    void *_pad[3];
}

/* Create a new color object. */
+ (instancetype)colorWithCGColor:(CGColorRef)c;

/* Create a new color object in CI's default RGB colorspace
   which is kCGColorSpaceSRGB or, if running on OSX before 10.10,
   kCGColorSpaceGenericRGB. */
+ (instancetype)colorWithRed:(CGFloat)r green:(CGFloat)g blue:(CGFloat)b alpha:(CGFloat)a;
+ (instancetype)colorWithRed:(CGFloat)r green:(CGFloat)g blue:(CGFloat)b;

/* Create a new color object in a given colorspace. 
 * Will return null if the colorspace is not kCGColorSpaceModelRGB. */
+ (nullable instancetype)colorWithRed:(CGFloat)r green:(CGFloat)g blue:(CGFloat)b alpha:(CGFloat)a colorSpace:(CGColorSpaceRef)colorSpace NS_AVAILABLE(10_12, 10_0);
+ (nullable instancetype)colorWithRed:(CGFloat)r green:(CGFloat)g blue:(CGFloat)b colorSpace:(CGColorSpaceRef)colorSpace NS_AVAILABLE(10_12, 10_0);

/* Create a new color object, 'representation' should be a string in one of
 * the formats returned by the stringRepresentation method. */
+ (instancetype)colorWithString:(NSString *)representation;


/* Initialize a new color object. */
- (instancetype)initWithCGColor:(CGColorRef)c NS_DESIGNATED_INITIALIZER;

/* Initialize a new color object in CI's default RGB colorspace
   which is kCGColorSpaceSRGB or, if running on OSX before 10.10,
   kCGColorSpaceGenericRGB. */
- (instancetype)initWithRed:(CGFloat)r green:(CGFloat)g blue:(CGFloat)b alpha:(CGFloat)a;
- (instancetype)initWithRed:(CGFloat)r green:(CGFloat)g blue:(CGFloat)b NS_AVAILABLE(10_11, 9_0);

/* Initialize a new color object in a given colorspace.
 * Will return null if the colorspace is not kCGColorSpaceModelRGB. */
- (nullable instancetype)initWithRed:(CGFloat)r green:(CGFloat)g blue:(CGFloat)b alpha:(CGFloat)a colorSpace:(CGColorSpaceRef)colorSpace NS_AVAILABLE(10_12, 10_0);
- (nullable instancetype)initWithRed:(CGFloat)r green:(CGFloat)g blue:(CGFloat)b colorSpace:(CGColorSpaceRef)colorSpace NS_AVAILABLE(10_12, 10_0);

/* Return the number of color components (including alpha). */
@property (readonly) size_t numberOfComponents;

/* Return the color components (including alpha). */
@property (readonly) const CGFloat *components NS_RETURNS_INNER_POINTER;

/* Return the alpha value of the color. */
@property (readonly) CGFloat alpha;

/* Return the color space object associated with the color. */
@property (readonly) CGColorSpaceRef colorSpace CF_RETURNS_NOT_RETAINED;

/* Return the (unpremultiplied) red, green or blue components of the color. */
@property (readonly) CGFloat red;
@property (readonly) CGFloat green;
@property (readonly) CGFloat blue;

/* Returns a formatted string with the components of the color.
 * The string is suitable for passing to [CIColor colorWithString:].
 * This property is not KVO-safe because it returns a new NSString each time.
 * The value of the NSString will be the same each time it is called. */
@property (readonly) NSString *stringRepresentation;

/* Convenience constant CIColors in the sRGB colorspace. */
@property (class, strong, readonly) CIColor *blackColor   NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *whiteColor   NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *grayColor    NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *redColor     NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *greenColor   NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *blueColor    NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *cyanColor    NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *magentaColor NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *yellowColor  NS_AVAILABLE(10_12, 10_0);
@property (class, strong, readonly) CIColor *clearColor   NS_AVAILABLE(10_12, 10_0);

@end

NS_ASSUME_NONNULL_END
