/*
    NSStepperTouchBarItem.h
    Application Kit
    Copyright (c) 1995-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSTouchBarItem.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.15), ios(13.0))
@interface NSStepperTouchBarItem : NSTouchBarItem

/// Creates an `NSStepperTouchBarItem` with a `formatter` to display the stepper's value as text
///
/// @param formatter A formatter used to display a textual representation of the stepper's value
+ (instancetype)stepperTouchBarItemWithIdentifier:(NSTouchBarItemIdentifier)identifier formatter:(NSFormatter *)formatter;

/// Creates an `NSStepperTouchBarItem` using the result of `drawingHandler` to display the stepper's value as an image
///
/// @param drawingHandler A block that draws a graphical representation of the stepper's value in the specified rectangle. The coordinates of this rectangle are specified in points.
+ (instancetype)stepperTouchBarItemWithIdentifier:(NSTouchBarItemIdentifier)identifier drawingHandler:(void (^)(NSRect rect, double value))drawingHandler;

/// The stepper's maximum value. The default is 59.0.
@property double maxValue;

/// The stepper's minimum value. The default is 0.0.
@property double minValue;

/// The stepper's increment value. The default is 1.0.
@property double increment;

/// The current value of the stepper.
@property double value;

/// The target object that receives action messages from the stepper.
@property (weak, nullable) id target;

/// The action-message selector associated with the stepper.
@property (nullable) SEL action;

/// The localized string labelling this item during user customization. The default value is empty string.
@property (copy, null_resettable) NSString *customizationLabel;

@end

NS_ASSUME_NONNULL_END
