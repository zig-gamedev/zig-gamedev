/*
	NSButton.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/NSButtonCell.h>
#import <AppKit/NSUserInterfaceValidation.h>
#import <AppKit/NSUserInterfaceCompression.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSSound, NSImageSymbolConfiguration;

@interface NSButton : NSControl <NSUserInterfaceValidations, NSAccessibilityButton, NSUserInterfaceCompression>

#pragma mark Creating Standard Buttons

/*!
 Creates a standard push button with a title and image.
 @param title The localized title string that is displayed on the button.
 @param image The image that is displayed alongside the title. In left-to-right localizations, the image is displayed to the left of the title. In right-to-left localizations, it is displayed to the right.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized button object.
 */
+ (instancetype)buttonWithTitle:(NSString *)title image:(NSImage *)image target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

/*!
 Creates a standard push button with the provided title.
 @param title The localized title string that is displayed on the button.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized button object.
 */
+ (instancetype)buttonWithTitle:(NSString *)title target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

/*!
 Creates a standard push button with the provided image. Set the image's accessibilityDescription property to ensure accessibility for this control.
 @param image The image to display in the body of the button.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized button object.
 */
+ (instancetype)buttonWithImage:(NSImage *)image target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

/*!
 Creates a standard checkbox with the provided title.
 @param title The localized title string that is displayed alongside the checkbox.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized button object.
 */
+ (instancetype)checkboxWithTitle:(NSString *)title target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

/*!
 Creates a standard radio button with the provided title.
 @param title The localized title string that is displayed alongside the radio button.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized button object.
 */
+ (instancetype)radioButtonWithTitle:(NSString *)title target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

#pragma mark Configuring Buttons

/*! Sets the button’s type, which affects its user interface and behavior when clicked. See the NSButtonType enumeration for possible options and their behaviors. */
- (void)setButtonType:(NSButtonType)type;

/*! The title displayed on the button when it’s in an off state, or an empty string if the button does not display a title. By default, a button's title is "Button". */
@property (copy) NSString *title;

/*! The button's title, expressed as an attributed string. */
@property (copy) NSAttributedString *attributedTitle;

/*! The title that the button displays when the button is in an on state, or an empty string if there is no such title. Note that some button types do not display an alternate title. */
@property (copy) NSString *alternateTitle;

/*! The alternate title, expressed as an attributed string. */
@property (copy) NSAttributedString *attributedAlternateTitle;

/*! Indicates whether the button's action has a destructive effect on user data.  AppKit may guard a destructive-actioned button against accidental presses, and may give the button a special appearance in certain contexts to caution against unintentional use.  Defaults to NO. */
@property BOOL hasDestructiveAction API_AVAILABLE(macos(11.0));

/*! The sound that plays when the user clicks the button, or nil if the button should not play a sound. The default value is nil. */
@property (nullable, strong) NSSound *sound;

/*! Sends action on deep-press or extended hover while dragging. Defaults to NO. */
@property (getter=isSpringLoaded) BOOL springLoaded API_AVAILABLE(macos(10.10.3));

/*! Configures the maximum allowed level for an NSMultiLevelAcceleratorButton, allowed values range from [1,5]. Defaults to 2. */
@property NSInteger maxAcceleratorLevel API_AVAILABLE(macos(10.10.3));

/*! Sets the initial delay and repeat interval, in seconds, for repeated action messages sent when `continuous` is YES. */
- (void)setPeriodicDelay:(float)delay interval:(float)interval;

/*! Gets the initial delay and repeat interval, in seconds, for repeated action messages sent when `continuous` is YES. Both parameters to this method must not be NULL. */
- (void)getPeriodicDelay:(float *)delay interval:(float *)interval;

#pragma mark Configuring Button Images

/*! The bezel style of the button, which provides a set of bezel artwork, layout metrics, and content styling from a set of system-provided styles. See the NSBezelStyle enumeration for a list of available styles. The bezel style is not used if the `bordered` property is set to `NO`. */
@property NSBezelStyle bezelStyle;

/*! A Boolean value that determines whether the button draws a border. */
@property (getter=isBordered) BOOL bordered;

/*! A Boolean value that indicates whether the button is transparent. A transparent button never draws itself, but it receives mouse events, sends its action, and tracks the mouse properly. */
@property (getter=isTransparent) BOOL transparent;

/* A Boolean value that determines whether the button displays its border only when the pointer is over it. */
@property BOOL showsBorderOnlyWhileMouseInside;

/*! The image that appears on the button when it’s in an off state, or nil if there is no such image. */
@property (nullable, strong) NSImage *image;

/*! An alternate image that appears on the button when the button is in an on state, or nil if there is no such image. Note that some button types do not display an alternate image. */
@property (nullable, strong) NSImage *alternateImage;

/*! The position of the button's image relative to its title. See the NSCellImagePosition enumeration for possible values. */
@property NSCellImagePosition imagePosition;

/*! The scaling mode applied to make the button's image fit within its bounds. */
@property NSImageScaling imageScaling API_AVAILABLE(macos(10.5));

/*! A Boolean value that determines how the button's image and title are positioned together within the button bezel. If false, the image is positioned according to the imagePosition property at the edge of the button bezel, and the title is positioned within the remaining space. If true, the button’s image is positioned directly adjacent to the title based on the imagePosition property, and the image and title are positioned within the button bezel as a single unit. */
@property BOOL imageHugsTitle API_AVAILABLE(macos(10.12));

/*!
 Specifies a combination of point size, weight, and scale to use when sizing and displaying symbol images. If a symbol configuration isn't provided, the symbol is matched to the button's `font` property. The default value is nil.
 */
@property (nullable, copy) NSImageSymbolConfiguration *symbolConfiguration API_AVAILABLE(macos(11));

/*! Applies a custom color to the button's bezel, in appearances that support it. A nil value indicates an unmodified button appearance. The default value is nil. */
@property (nullable, copy) NSColor *bezelColor API_AVAILABLE(macos(10.12.2));

/*! Applies a tint color to template image and text content, in combination with other theme-appropriate effects. Only applicable to borderless buttons. A nil value indicates the standard set of effects without color modification. The default value is nil. Non-template images and attributed string values are not affected by the contentTintColor. */
@property (nullable, copy) NSColor *contentTintColor API_AVAILABLE(macos(10.14));

#pragma mark Managing Button State

/*! The button's state. Buttons support the off and on states, and an additional mixed state depending on the value of the `allowsMixedState` property. */
@property NSControlStateValue state;

/*! A Boolean value that indicates whether the button allows a mixed state. If NO, the button has two states (on and off), and if YES, the button has three states (on, off, and mixed). The mixed state is commonly used with checkboxes and radio buttons to indicate a value which is partially on. */
@property BOOL allowsMixedState;

/*! Sets the button to its next eligible state. If the button allows mixed state, this cycles through the states in the order: on, off, mixed, on, etc. If the button does not allow mixed state, it toggles between off and on. */
- (void)setNextState;

/*! Highlights, or un-highlights, the button. Highlighting makes the button appear "pressed", which may include showing an illuminated bezel, or showing the alternate image or title, depending on the type of button. */
- (void)highlight:(BOOL)flag;

#pragma mark Handling Keyboard Events

/*! This property contains the button's key equivalent, or the empty string if no equivalent has been defined. Buttons don’t have a default key equivalent. Setting the key equivalent to the Return character causes it to act as the default button for its window. */
@property (copy) NSString *keyEquivalent;

/*! A bitmask specifying the modifier keys that are applied to the button's key equivalent. Mask bits are defined by the NSEventModifierFlags option set. The only mask bits relevant in button key-equivalent modifier masks are NSEventModifierFlagControl, NSEventModifierFlagOption, and NSEventModifierFlagCommand. */
@property NSEventModifierFlags keyEquivalentModifierMask;

/*! If the event parameter matches the button's key equivalent, the button briefly highlights and performs its action, and then returns YES. Otherwise, returns NO. */
- (BOOL)performKeyEquivalent:(NSEvent *)key;

#pragma mark NSUserInterfaceCompression

- (void)compressWithPrioritizedCompressionOptions:(NSArray<NSUserInterfaceCompressionOptions *> *)prioritizedOptions API_AVAILABLE(macos(10.13));
- (NSSize)minimumSizeWithPrioritizedCompressionOptions:(NSArray<NSUserInterfaceCompressionOptions *> *)prioritizedOptions API_AVAILABLE(macos(10.13));
@property (readonly, copy) NSUserInterfaceCompressionOptions *activeCompressionOptions API_AVAILABLE(macos(10.13));

@end

@interface NSButton (NSButtonDeprecated)

/* Mnemonics, which are underlined characters in the button title that can be used as a keyboard shortcut, are not used on macOS. The -setTitleWithMnemonic: method passes its input (minus the ampersand, which was used to denote the mnemonic) to -setTitle:, but otherwise has no effect. */
- (void)setTitleWithMnemonic:(null_unspecified NSString *)stringWithAmpersand API_DEPRECATED("Mnemonics are not used on macOS. Set the title property directly instead.", macos(10.0,10.8));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

