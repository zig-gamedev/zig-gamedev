/*
 NSPickerTouchBarItem.h
 Application Kit
 Copyright (c) 2019-2021, Apple Inc.
 All rights reserved.
 */

#import <AppKit/NSTouchBarItem.h>

NS_ASSUME_NONNULL_BEGIN

#if !TARGET_OS_IPHONE
@class NSImage, NSColor;
#else
@class UIImage, UIColor;
#endif

typedef NS_ENUM(NSInteger, NSPickerTouchBarItemSelectionMode) {
    NSPickerTouchBarItemSelectionModeSelectOne = 0, // Only one option in the control can be selected at a time.
    NSPickerTouchBarItemSelectionModeSelectAny = 1, // One or more options in the control can be selected at a time.
    NSPickerTouchBarItemSelectionModeMomentary = 2  // An option is only selected when the user is engaging within the bounds of that option.
} API_AVAILABLE(macos(10.15), ios(13.0));

typedef NS_ENUM(NSInteger, NSPickerTouchBarItemControlRepresentation) {
    NSPickerTouchBarItemControlRepresentationAutomatic = 0, // representation will switch based on available space
    NSPickerTouchBarItemControlRepresentationExpanded = 1, // the control and all of its options will be visible to the user directly
    NSPickerTouchBarItemControlRepresentationCollapsed = 2 // the options of the control will be disclosed to the user through a popover
} API_AVAILABLE(macos(10.15), ios(13.0));

API_AVAILABLE(macos(10.15), ios(13.0))
@interface NSPickerTouchBarItem : NSTouchBarItem

/*
 Creates a standard item with an NSSegmentedControl appropriate for use in an NSTouchBar
 */
+ (instancetype)pickerTouchBarItemWithIdentifier:(NSTouchBarItemIdentifier)identifier labels:(NSArray<NSString *> *)labels selectionMode:(NSPickerTouchBarItemSelectionMode)selectionMode target:(nullable id)target action:(nullable SEL)action;

#if !TARGET_OS_IPHONE
+ (instancetype)pickerTouchBarItemWithIdentifier:(NSTouchBarItemIdentifier)identifier images:(NSArray<NSImage *> *)images selectionMode:(NSPickerTouchBarItemSelectionMode)selectionMode target:(nullable id)target action:(nullable SEL)action;
#else
+ (instancetype)pickerTouchBarItemWithIdentifier:(NSTouchBarItemIdentifier)identifier images:(NSArray<UIImage *> *)images selectionMode:(NSPickerTouchBarItemSelectionMode)selectionMode target:(nullable id)target action:(nullable SEL)action;
#endif

/*
 The style in which this item will be represented to the user.
 Defaults to NSPickerRepresentationAutomatic.
 */
@property NSPickerTouchBarItemControlRepresentation controlRepresentation;

/*
 The user facing label and or image visible when the control is in the collapsed representation.
 If the selectionMode is NSPickerSelectionModeSelectOne, the collapsed representation will automatically be populated with the associated label and or image from the seelctedIndex.
 Setting these values may be used to override any automatic behavior.
 */
@property (copy) NSString *collapsedRepresentationLabel;

#if !TARGET_OS_IPHONE
@property (nullable, strong) NSImage *collapsedRepresentationImage;
#else
@property (nullable, strong) UIImage *collapsedRepresentationImage;
#endif

/*
 The most recently selected index of the control, or -1 if nothing is selected.
 */
@property NSInteger selectedIndex;

#if !TARGET_OS_IPHONE
@property (nullable, copy) NSColor *selectionColor;
#else
@property (nullable, copy) UIColor *selectionColor;
#endif

/*
 Defaults to NSPickerTouchBarItemSelectionModeSelectOne
 */
@property NSPickerTouchBarItemSelectionMode selectionMode;

/*
 The number of options represented in the control.
 */
@property NSInteger numberOfOptions;

/*
 Get and set the image and labels of an option at the given index.
 */
#if !TARGET_OS_IPHONE
- (void)setImage:(nullable NSImage *)image atIndex:(NSInteger)index;
- (nullable NSImage *)imageAtIndex:(NSInteger)index;
#else
- (void)setImage:(nullable UIImage *)image atIndex:(NSInteger)index API_AVAILABLE(ios(13.0));
- (nullable UIImage *)imageAtIndex:(NSInteger)index API_AVAILABLE(ios(13.0));
#endif

- (void)setLabel:(NSString *)label atIndex:(NSInteger)index;
- (nullable NSString *)labelAtIndex:(NSInteger)index;

@property (nullable, weak) id target;
@property (nullable ) SEL action;

@property (getter=isEnabled) BOOL enabled;

- (void)setEnabled:(BOOL)enabled atIndex:(NSInteger)index;
- (BOOL)isEnabledAtIndex:(NSInteger)index;

/// The localized string labelling this item during user customization. The default value is empty string.
@property (readwrite, copy, null_resettable) NSString *customizationLabel;

@end

NS_ASSUME_NONNULL_END
