/*
	NSGradient.h
	Application Kit
	Copyright (c) 2006-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSBezierPath, NSColor, NSColorSpace;

typedef NS_OPTIONS(NSUInteger, NSGradientDrawingOptions) {
    NSGradientDrawsBeforeStartingLocation =   (1 << 0),
    NSGradientDrawsAfterEndingLocation =    (1 << 1),
};

/* An NSGradient defines a transition between colors.  The transition is defined over a range from 0.0 to 1.0 inclusive.  A gradient typically contains a color at location 0.0, and one at location 1.0 with additional colors assigned to locations between 0.0 and 1.0.

An NSGradient is a drawing primitive that can draw itself as a linear or radial gradient.  The color value at location 0.0 is considered the starting color, the color value at location 1.0 is considered the ending color.  A primitive drawing method is provided for both linear and radial gradients.  Each primitive drawing method provides flexibility in specifying the location of its respective gradient.  These primitive drawing methods perform no clipping before drawing.

In addition, convenience drawing methods take a rectangle or path, and automatically calculate staring and ending locations and perform clipping to provide a convenient means for drawing gradient fills.  Two of the convenience methods are demonstrated below:


The following code will fill a rectangle with a 45 degree linear gradient, from black to white:
	
	NSRect rect; // assume this exists
	NSGradient *gradient = [[NSGradient alloc] initWithStartingColor: [NSColor blackColor] endingColor: [NSColor whiteColor]];
	[gradient drawInRect: rect angle: 45.0];
	[gradient release];
	
The following code will fill an arbitrary bezier path with a radial gradient, from the center of the path's bounding box, that transitions through three colors, evenly distributed in the gradient from 0.0 to 1.0.

	NSBezierPath *path // assume this exists
	NSArray *colorArray = [NSArray arrayWithObjects: [NSColor blueColor], [NSColor yellowColor], [NSColor orangeColor], nil];
	NSGradient *gradient = [[NSGradient alloc] initWithColors: colorArray];
	[gradient drawInBezierPath: path relativeCenterPosition: NSZeroPoint];
	[gradient release];
	
An NSGradient has a color space.  When initialized, all colors provided are converted to that color space, and interpolation of colors occurs using the components of that color space.  The designated initializer takes a color space argument, all other initializers use the default generic RGB color space.

*/

API_AVAILABLE(macos(10.5))
@interface NSGradient : NSObject <NSCopying, NSSecureCoding>
/* Initializes a gradient with starting color at location 0.0 and ending color at location 1.0  The color space returned by [NSColorSpace genericRGBColorSpace] is used.
*/
- (nullable instancetype)initWithStartingColor:(NSColor *)startingColor endingColor:(NSColor *)endingColor;


/* Initializes a gradient with the first color in the array at 0.0, the last color in the array at 1.0, and intervening colors at equal intervals in between. The color space returned by [NSColorSpace genericRGBColorSpace] is used.
*/
- (nullable instancetype)initWithColors:(NSArray<NSColor *> *)colorArray;


/* This initializer takes the first color, then the first location as a CGFloat, then an alternating list of colors and CGFloats, terminated by nil.  If no color is provided for 0.0 or 1.0, the created color gradient will use the color provided at the locations closest to 0.0 and 1.0 for those values.  The color space returned by [NSColorSpace genericRGBColorSpace] is used.

    For example:
    NSGradient *gradient = [[NSGradient alloc] initWithColorsAndLocations: [NSColor blackColor], 0.0, [NSColor blueColor], 0.33, [NSColor cyanColor], 0.45, [NSColor orangeColor], 0.72, [NSColor redColor], 1.0, nil];

*/
- (nullable instancetype)initWithColorsAndLocations:(NSColor *)firstColor, ... NS_REQUIRES_NIL_TERMINATION;


/* Initializes a gradient by pairing the colors provided in the color array with the locations provided in the locations array.    Each location should be a CGFloat between 0.0 and 1.0.  The color array and location array should not be empty, and should contain the same number of items.  If no color is provided for 0.0 or 1.0, the created color gradient will use the color provided at the locations closest to 0.0 and 1.0 for those values.  This is the designated initializer.
*/
- (nullable instancetype)initWithColors:(NSArray<NSColor *> *)colorArray atLocations:(nullable const CGFloat *)locations colorSpace:(NSColorSpace *)colorSpace NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;


/* DRAWING LINEAR GRADIENTS */

/* Draws a linear gradient from start point to end point.  The option flags control whether the gradient draws itself before the start point, or after the end point.  The gradient is drawn in the current graphics context without performing any additinal clipping.  This is the primitive method for drawing a linear gradient.
*/
- (void)drawFromPoint:(NSPoint)startingPoint toPoint:(NSPoint)endingPoint options:(NSGradientDrawingOptions)options;


/* Convenience method for drawing a linear gradient to fill a rectangle.  Draws a linear gradient clipped to the provided rect.  The start point and end point are calculated with respect to the provided rect so that the gradient is drawn at the provided angle in degrees.  The end points are located such that both the start and end color in the gradient are visible in the rect.
*/
- (void)drawInRect:(NSRect)rect angle:(CGFloat)angle;


/* Convenience method for drawing a linear gradient to fill a path.  Draws a linear gradient clipped by the provided path.  The start point and end point are calculated with respect to the provided path so that the gradient is drawn at the provided angle in degrees. The end points are located such that both the start and end color in the gradient are visible in the path.
*/
- (void)drawInBezierPath:(NSBezierPath *)path angle:(CGFloat)angle;


/* DRAWING RADIAL GRADIENTS */

/* Draws a radial gradient between two circles defined by the center point and radius of each circle.  The option flags control whether the gradient draws itself before the start point, or after the end point.  The gradient is drawn in the current graphics context without performing any additinal clipping.  This is the primitive method for drawing a radial gradient.
*/
- (void)drawFromCenter:(NSPoint)startCenter radius:(CGFloat)startRadius toCenter:(NSPoint)endCenter radius:(CGFloat)endRadius options:(NSGradientDrawingOptions)options;


/* Convenience method for drawing a radial gradient to fill a rect.  Draws a radial gradient clipped by the provided rect.  The starting circle is always a single point located at the center of the ending circle which encloses the drawn rect.  The radius of the ending circle is determined by the relative center position.

    The relative center position proportionally adjusts the center location of the radial gradient.  It maps the four corners of the rectangle to (-1.0, -1.0), (1.0, -1.0), (1.0, 1.0) and (-1.0, 1.0), with (0.0, 0.0) in the center of the rectangle.  Use NSZeroPoint to center the radial gradient in the rect.  The radius of the ending circle is the distance from the relative center to the opposite corner of the rect.
*/
- (void)drawInRect:(NSRect)rect relativeCenterPosition:(NSPoint)relativeCenterPosition;


/* Convenience method for drawing a radial gradient to fill a path.  Draws a radial gradient clipped by the provided path.  The starting circle is always a single point located at the center of the ending circle which encloses the drawn path.  The radius of the ending circle is determined by the relative center position.

    The relative center position proportionally adjusts the center location of the radial gradient.  It maps the four corners of the path bounding rect to (-1.0, -1.0), (1.0, -1.0), (1.0, 1.0) and (-1.0, 1.0), with (0.0, 0.0) in the center of path bounding rect.  Use NSZeroPoint to center the radial gradient in the path bounding rect.  The radius of the ending circle is the distance from the relative center to the opposite corner of the path bounding rect.
*/
- (void)drawInBezierPath:(NSBezierPath *)path relativeCenterPosition:(NSPoint)relativeCenterPosition;




/* These methods are primarily for the use of controls which manipulate the color gradient.
*/

/* Returns the color space of the gradient */
@property (readonly, strong) NSColorSpace *colorSpace;


/* The number of color stops in the color gradient */
@property (readonly) NSInteger numberOfColorStops;


/* Returns the color and location at a particular index in the color gradient */
- (void)getColor:(NSColor * _Nonnull * _Nullable)color location:(nullable CGFloat *)location atIndex:(NSInteger)index;


/* This method will return the interpolated gradient value at the given location.  For example, in a two color gradient with white at location 0.0 and black at location 1.0, the interpolated color at location 0.5 would be 50% gray.

You should not need to override this method, it reports the color value of the gradient at a particular location, and does not affect the drawn color values.
*/
- (NSColor *)interpolatedColorAtLocation:(CGFloat)location;


@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

