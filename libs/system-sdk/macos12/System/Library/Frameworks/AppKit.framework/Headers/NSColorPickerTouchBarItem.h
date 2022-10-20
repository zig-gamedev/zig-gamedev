/*
 NSColorPickerTouchBarItem.h
 Application Kit
 Copyright (c) 2016-2021, Apple Inc.
 All rights reserved.
*/

#import <AppKit/NSTouchBarItem.h>

#if TARGET_OS_IPHONE
@class UIImage, UIColor;
#endif

NS_ASSUME_NONNULL_BEGIN

@class NSViewController, NSColor, NSImage, NSString, NSColorList, NSColorSpace;

API_AVAILABLE(macos(10.12.2), ios(13.0))
@interface NSColorPickerTouchBarItem : NSTouchBarItem

/// Creates a bar item containing a button with the standard color picker icon that invokes the color picker.
+ (instancetype)colorPickerWithIdentifier:(NSTouchBarItemIdentifier)identifier;
/// Creates a bar item containing a button with the standard text color picker icon that invokes the color picker. Should be used when the item is used for picking text colors.
+ (instancetype)textColorPickerWithIdentifier:(NSTouchBarItemIdentifier)identifier;
/// Creates a bar item containing a button with the standard stroke color picker icon that invokes the color picker. Should be used when the item is used for picking stroke colors.
+ (instancetype)strokeColorPickerWithIdentifier:(NSTouchBarItemIdentifier)identifier;

/// Creates a bar item containing a button with the provided image that invokes the color picker.
#if !TARGET_OS_IPHONE
+ (instancetype)colorPickerWithIdentifier:(NSTouchBarItemIdentifier)identifier buttonImage:(NSImage *)image;
#else
+ (instancetype)colorPickerWithIdentifier:(NSTouchBarItemIdentifier)identifier buttonImage:(UIImage *)image API_AVAILABLE(ios(13.0));
#endif

/// The selected color of the picker.
#if !TARGET_OS_IPHONE
@property (copy) NSColor *color;
#else
@property (copy) UIColor *color API_AVAILABLE(ios(13.0));
#endif

/// Whether or not the picker should allow picking a color with non-1.0 alpha. Defaults to `!NSColor.ignoresAlpha`.
@property BOOL showsAlpha;

/// Controls the color spaces that the receiver is able to produce. If a color outside of the allowed spaces are displayed or selected, it will first be converted to the first color space in the array. `nil` signifies any color space is allowed. Empty array is an invalid value and will raise an exception if set. Defaults to `nil`.
@property (copy, nullable) NSArray<NSColorSpace *> *allowedColorSpaces API_AVAILABLE(macos(10.13)) API_UNAVAILABLE(ios);

/// The color list displayed in the list color picker. Defaults to the standard system color list. Setting a custom color list will disable the additional tints/shades that appear on long-press.
@property (strong, null_resettable) NSColorList *colorList API_UNAVAILABLE(ios);

/// The localized string labelling this item during user customization. The default value is the localized string of "Color Picker".
@property (readwrite, copy, null_resettable) NSString *customizationLabel;

@property (weak, nullable) id target;
@property (nullable) SEL action;

/// Enables or disabled the color picker. If it is currently being shown in a popover, it will be dismissed.
@property (getter=isEnabled) BOOL enabled;

@end

NS_ASSUME_NONNULL_END
