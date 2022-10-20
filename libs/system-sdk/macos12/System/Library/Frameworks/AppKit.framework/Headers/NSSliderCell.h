/*
	NSSliderCell.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSActionCell.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_ENUM(NSUInteger, NSTickMarkPosition) {
    NSTickMarkPositionBelow = 0,
    NSTickMarkPositionAbove = 1,
    NSTickMarkPositionLeading = NSTickMarkPositionAbove,
    NSTickMarkPositionTrailing = NSTickMarkPositionBelow
};
typedef NS_ENUM(NSUInteger, NSSliderType) {
    NSSliderTypeLinear = 0,
    NSSliderTypeCircular = 1,
};

@interface NSSliderCell : NSActionCell

@property (class, readonly) BOOL prefersTrackingUntilMouseUp;

@property double minValue;
@property double maxValue;
@property double altIncrementValue;

@property NSSliderType sliderType;
@property (readwrite, getter=isVertical) BOOL vertical API_AVAILABLE(macos(10.11));

@property (readonly) NSRect trackRect;

@property (readonly) CGFloat knobThickness;

- (NSRect)knobRectFlipped:(BOOL)flipped;
- (NSRect)barRectFlipped:(BOOL)flipped API_AVAILABLE(macos(10.9));

- (void)drawKnob:(NSRect)knobRect;
- (void)drawKnob;
- (void)drawBarInside:(NSRect)rect flipped:(BOOL)flipped;

@end

@interface NSSliderCell (NSSliderCellVerticalGetter)
@property (readonly, getter=isVertical) BOOL vertical API_AVAILABLE(macos(10.0));
@end

@interface NSSliderCell(NSTickMarkSupport)

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

- (void)drawTickMarks API_AVAILABLE(macos(10.9));

@end

@interface NSSliderCell (NSDeprecated)
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

static const NSTickMarkPosition NSTickMarkBelow API_DEPRECATED_WITH_REPLACEMENT("NSTickMarkPositionBelow", macos(10.0,10.12)) = NSTickMarkPositionBelow;
static const NSTickMarkPosition NSTickMarkAbove API_DEPRECATED_WITH_REPLACEMENT("NSTickMarkPositionAbove", macos(10.0,10.12)) = NSTickMarkPositionAbove;
static const NSTickMarkPosition NSTickMarkLeft API_DEPRECATED_WITH_REPLACEMENT("NSTickMarkPositionLeading", macos(10.0,10.12)) = NSTickMarkPositionLeading;
static const NSTickMarkPosition NSTickMarkRight API_DEPRECATED_WITH_REPLACEMENT("NSTickMarkPositionTrailing", macos(10.0,10.12)) = NSTickMarkPositionTrailing;

static const NSSliderType NSLinearSlider API_DEPRECATED_WITH_REPLACEMENT("NSSliderTypeLinear", macos(10.0,10.12)) = NSSliderTypeLinear;
static const NSSliderType NSCircularSlider API_DEPRECATED_WITH_REPLACEMENT("NSSliderTypeCircular", macos(10.0,10.12)) = NSSliderTypeCircular;




API_UNAVAILABLE_END
