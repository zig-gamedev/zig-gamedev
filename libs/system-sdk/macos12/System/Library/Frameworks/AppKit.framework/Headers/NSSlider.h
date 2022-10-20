/*
	NSSlider.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/NSSliderCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSSlider : NSControl <NSAccessibilitySlider>

@property NSSliderType sliderType API_AVAILABLE(macos(10.10));

@property double minValue;
@property double maxValue;
@property double altIncrementValue;
@property (readonly) CGFloat knobThickness;
- (BOOL)acceptsFirstMouse:(nullable NSEvent *)event;
@property (readwrite, getter=isVertical) BOOL vertical API_AVAILABLE(macos(10.12));

@property (nullable, copy) NSColor *trackFillColor API_AVAILABLE(macos(10.12.2)); // The color of the filled portion of the track, in appearances that support it

@end

@interface NSSlider (NSSliderVerticalGetter)
@property (readonly, getter=isVertical) BOOL vertical API_AVAILABLE(macos(10.0));
@end

@interface NSSlider(NSTickMarkSupport)

// For setting and retrieving the number of tick marks on a slider.
//   0 indicates no tick marks are present, which is the default
@property NSInteger numberOfTickMarks;

// For setting and retrieving the position tick marks will be displayed in.
//   This has no effect if numberOfTickMarks is 0.
@property NSTickMarkPosition tickMarkPosition;

// For setting and retrieving whether values on the slider can be anything
//   the slider normally allows, or only values that correspond to a tick mark.
//   This has no effect if numberOfTickMarks is 0.
@property BOOL allowsTickMarkValuesOnly;

// Determine the slider value for a particular tick mark.
//   An NSRangeException will be raised if the index is invalid.
- (double)tickMarkValueAtIndex:(NSInteger)index;

// Determine the bounding rectangle of a particular tick mark.
//   An NSRangeException will be raised if the index is invalid.
- (NSRect)rectOfTickMarkAtIndex:(NSInteger)index;

// Determine whether a particular point is "near" a tick mark and return its
// index if so.  NSNotFound will be returned if no tick mark is close enough
// to qualify.
- (NSInteger)indexOfTickMarkAtPoint:(NSPoint)point;

- (double)closestTickMarkValueToValue:(double)value;

@end

@interface NSSlider(NSSliderConvenience)

/*!
 Creates a continuous horizontal slider over the range 0.0 to 1.0. The default value is 0.0.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized slider control.
 */
+ (instancetype)sliderWithTarget:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

/*!
 Creates a continuous horizontal slider that represents values over a specified range.
 @param value The initial value displayed by the control.
 @param minValue The minimum value represented by the control.
 @param maxValue The maximum value represented by the control.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized slider control.
 */
+ (instancetype)sliderWithValue:(double)value minValue:(double)minValue maxValue:(double)maxValue target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

@end

@interface NSSlider (NSSliderDeprecated)
- (void)setTitleCell:(null_unspecified NSCell *)cell API_DEPRECATED("-setTitleCell: had no effect since 10.0", macos(10.0,10.9));
- (null_unspecified id)titleCell API_DEPRECATED("-titleCell has returned nil since 10.0", macos(10.0,10.9));
- (void)setTitleColor:(null_unspecified NSColor *)newColor API_DEPRECATED("-setTitleColor: had no effect since 10.0", macos(10.0,10.9));
- (null_unspecified NSColor *)titleColor API_DEPRECATED("-titleColor has returned nil since 10.0", macos(10.0,10.9));
- (void)setTitleFont:(null_unspecified NSFont *)fontObj API_DEPRECATED("-setTitleFont: had no effect since 10.0", macos(10.0,10.9));
- (null_unspecified NSFont *)titleFont API_DEPRECATED("-titleFont has returned nil since 10.0", macos(10.0,10.9));
- (null_unspecified NSString *)title API_DEPRECATED("-title has returned nil since 10.0", macos(10.0,10.9));
- (void)setTitle:(null_unspecified NSString *)string API_DEPRECATED("-setTitle: had no effect since 10.0", macos(10.0,10.9));
- (void)setKnobThickness:(CGFloat)thickness API_DEPRECATED("-knobThickness has returned 0 since 10.0", macos(10.0,10.9));
- (void)setImage:(null_unspecified NSImage *)backgroundImage API_DEPRECATED("-setImage: had no effect since 10.0", macos(10.0,10.9));
- (null_unspecified NSImage *)image API_DEPRECATED("-image has returned nil since 10.0", macos(10.0,10.9));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
