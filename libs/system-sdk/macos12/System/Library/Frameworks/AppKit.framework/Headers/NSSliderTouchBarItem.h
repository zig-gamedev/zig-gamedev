/*
 NSSliderTouchBarItem.h
 Application Kit
 Copyright (c) 2016-2021, Apple Inc.
 All rights reserved.
 */

#import <AppKit/NSUserInterfaceCompression.h>
#import <AppKit/NSTouchBarItem.h>

NS_ASSUME_NONNULL_BEGIN

@class NSSliderAccessory, NSSlider;

typedef CGFloat NSSliderAccessoryWidth NS_TYPED_EXTENSIBLE_ENUM API_AVAILABLE(macos(10.12.2), ios(13.0));
/// The default width for slider accessories.
APPKIT_EXTERN const NSSliderAccessoryWidth NSSliderAccessoryWidthDefault API_AVAILABLE(macos(10.12.2), ios(13.0));
/// The standard "wide" width for slider accessories.
APPKIT_EXTERN const NSSliderAccessoryWidth NSSliderAccessoryWidthWide API_AVAILABLE(macos(10.12.2), ios(13.0));

API_AVAILABLE(macos(10.12.2), ios(13.0))
@interface NSSliderTouchBarItem : NSTouchBarItem

@property (readonly, nonnull) NSView<NSUserInterfaceCompression> *view API_UNAVAILABLE(ios);

/// The slider displayed by the bar item. It is automatically created, but can be set to a custom subclass. doubleValue, minValue, maxValue, etc can all be read and set through the slider.
@property (strong) NSSlider *slider API_UNAVAILABLE(ios);

/// The double value of the control
@property double doubleValue API_AVAILABLE(macos(10.15), ios(13.0));

/// The width boundaries of the slider track of this item.
/// The system defines the default minimum. The maximum defaults to MAXFLOAT
@property CGFloat minimumSliderWidth API_AVAILABLE(macos(10.15), ios(13.0));
@property CGFloat maximumSliderWidth API_AVAILABLE(macos(10.15), ios(13.0));

/// The text label displayed along with the slider. If set to nil, the label will not have space reserved in the item.
@property (nullable, copy) NSString *label;

/// The accessory that appears on the end of the slider with the minimum value
@property (strong, nullable) NSSliderAccessory *minimumValueAccessory;
/// The accessory that appears on the end of the slider with the maximum value
@property (strong, nullable) NSSliderAccessory *maximumValueAccessory;
/// The width of the value accessories. Defaults to `.default`, but can be set to `.wide` or a custom value.
@property NSSliderAccessoryWidth valueAccessoryWidth;

/// The target of the item, notified when the slider or accessories receive user interaction.
@property (weak, nullable) id target;
/// The action of the item, called when the slider or accessories receive user interaction.
@property (nullable) SEL action;

/// The localized string labelling this item during user customization. The default value is empty string.
@property (readwrite, copy, null_resettable) NSString *customizationLabel;

@end

NS_ASSUME_NONNULL_END
