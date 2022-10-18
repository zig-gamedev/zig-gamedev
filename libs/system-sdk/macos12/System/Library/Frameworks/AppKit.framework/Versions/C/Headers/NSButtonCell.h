/*
	NSButtonCell.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSActionCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSAttributedString, NSFont, NSImage, NSSound;

typedef NS_ENUM(NSUInteger, NSButtonType) {
    NSButtonTypeMomentaryLight    = 0,
    NSButtonTypePushOnPushOff     = 1,
    NSButtonTypeToggle            = 2,
    NSButtonTypeSwitch            = 3,
    NSButtonTypeRadio             = 4,
    NSButtonTypeMomentaryChange   = 5,
    NSButtonTypeOnOff             = 6,
    NSButtonTypeMomentaryPushIn   = 7,
    NSButtonTypeAccelerator API_AVAILABLE(macos(10.10.3)) = 8,
    NSButtonTypeMultiLevelAccelerator API_AVAILABLE(macos(10.10.3)) = 9,
};

typedef NS_ENUM(NSUInteger, NSBezelStyle) {
    NSBezelStyleRounded           = 1,
    NSBezelStyleRegularSquare     = 2,
    NSBezelStyleDisclosure        = 5,
    NSBezelStyleShadowlessSquare  = 6,
    NSBezelStyleCircular          = 7,
    NSBezelStyleTexturedSquare    = 8,
    NSBezelStyleHelpButton        = 9,
    NSBezelStyleSmallSquare       = 10,
    NSBezelStyleTexturedRounded   = 11,
    NSBezelStyleRoundRect         = 12,
    NSBezelStyleRecessed          = 13,
    NSBezelStyleRoundedDisclosure = 14,
    NSBezelStyleInline API_AVAILABLE(macos(10.7)) = 15,
};

@interface NSButtonCell : NSActionCell

- (instancetype)initTextCell:(NSString *)string NS_DESIGNATED_INITIALIZER;
- (instancetype)initImageCell:(nullable NSImage *)image NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property NSBezelStyle bezelStyle;

- (void)setButtonType:(NSButtonType)type;
@property NSCellStyleMask highlightsBy;
@property NSCellStyleMask showsStateBy;

@property (null_resettable, copy) NSString *title;
@property (copy) NSAttributedString *attributedTitle;

@property (copy) NSString *alternateTitle;
@property (copy) NSAttributedString *attributedAlternateTitle;

@property (nullable, strong) NSImage *alternateImage;
@property NSCellImagePosition imagePosition;
@property NSImageScaling imageScaling API_AVAILABLE(macos(10.5));

@property (copy) NSString *keyEquivalent;
@property NSEventModifierFlags keyEquivalentModifierMask;

@property (getter=isTransparent) BOOL transparent;
@property (getter=isOpaque, readonly) BOOL opaque;

// When disabled, the image and text of an NSButtonCell are normally dimmed with gray.
// Radio buttons and switches use (imageDimsWhenDisabled == NO) so only their text is dimmed.
@property BOOL imageDimsWhenDisabled;
@property BOOL showsBorderOnlyWhileMouseInside;

@property (nullable, strong) NSSound *sound;

@property (nullable, copy) NSColor *backgroundColor;

- (void)setPeriodicDelay:(float)delay interval:(float)interval;
- (void)getPeriodicDelay:(float *)delay interval:(float *)interval;
- (void)performClick:(nullable id)sender;
- (void)mouseEntered:(NSEvent *)event;
- (void)mouseExited:(NSEvent *)event;

- (void)drawBezelWithFrame:(NSRect)frame inView:(NSView *)controlView;
- (void)drawImage:(NSImage *)image withFrame:(NSRect)frame inView:(NSView *)controlView;
- (NSRect)drawTitle:(NSAttributedString *)title withFrame:(NSRect)frame inView:(NSView *)controlView;

@end

/* Deprecations */

typedef NS_ENUM(NSUInteger, NSGradientType) {
    NSGradientNone          = 0,
    NSGradientConcaveWeak   = 1,
    NSGradientConcaveStrong = 2,
    NSGradientConvexWeak    = 3,
    NSGradientConvexStrong  = 4
} API_DEPRECATED("", macos(10.0,10.12));

static const NSButtonType NSMomentaryLightButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeMomentaryLight", macos(10.0,10.14)) = NSButtonTypeMomentaryLight;
static const NSButtonType NSPushOnPushOffButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypePushOnPushOff", macos(10.0,10.14)) = NSButtonTypePushOnPushOff;
static const NSButtonType NSToggleButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeToggle", macos(10.0,10.14)) = NSButtonTypeToggle;
static const NSButtonType NSSwitchButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeSwitch", macos(10.0,10.14)) = NSButtonTypeSwitch;
static const NSButtonType NSRadioButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeRadio", macos(10.0,10.14)) = NSButtonTypeRadio;
static const NSButtonType NSMomentaryChangeButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeMomentaryChange", macos(10.0,10.14)) = NSButtonTypeMomentaryChange;
static const NSButtonType NSOnOffButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeOnOff", macos(10.0,10.14)) = NSButtonTypeOnOff;
static const NSButtonType NSMomentaryPushInButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeMomentaryPushIn", macos(10.0,10.14)) = NSButtonTypeMomentaryPushIn;
static const NSButtonType NSAcceleratorButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeAccelerator", macos(10.10.3,10.14)) = NSButtonTypeAccelerator;
static const NSButtonType NSMultiLevelAcceleratorButton API_DEPRECATED_WITH_REPLACEMENT("NSButtonTypeMultiLevelAccelerator", macos(10.10.3,10.14)) = NSButtonTypeMultiLevelAccelerator;

/* These constants were accidentally reversed so that NSMomentaryPushButton lit and NSMomentaryLight pushed. These names are now deprecated */
static const NSButtonType NSMomentaryPushButton API_DEPRECATED("This constant is misnamed and has the same effect as NSButtonTypeMomentaryLight. Use that name instead, or switch to NSButtonTypeMomentaryPushIn.", macos(10.0,10.9)) = NSButtonTypeMomentaryLight;
static const NSButtonType NSMomentaryLight API_DEPRECATED("This constant is misnamed and has the same effect as NSButtonTypeMomentaryPushIn. Use that name instead, or switch to NSButtonTypeMomentaryLight.", macos(10.0,10.9)) = NSButtonTypeMomentaryPushIn;

static const NSBezelStyle NSRoundedBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleRounded", macos(10.0,10.14)) = NSBezelStyleRounded;
static const NSBezelStyle NSRegularSquareBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleRegularSquare", macos(10.0,10.14)) = NSBezelStyleRegularSquare;
static const NSBezelStyle NSDisclosureBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleDisclosure", macos(10.0,10.14)) = NSBezelStyleDisclosure;
static const NSBezelStyle NSShadowlessSquareBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleShadowlessSquare", macos(10.0,10.14)) = NSBezelStyleShadowlessSquare;
static const NSBezelStyle NSCircularBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleCircular", macos(10.0,10.14)) = NSBezelStyleCircular;
static const NSBezelStyle NSTexturedSquareBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleTexturedSquare", macos(10.0,10.14)) = NSBezelStyleTexturedSquare;
static const NSBezelStyle NSHelpButtonBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleHelpButton", macos(10.0,10.14)) = NSBezelStyleHelpButton;
static const NSBezelStyle NSSmallSquareBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleSmallSquare", macos(10.0,10.14)) = NSBezelStyleSmallSquare;
static const NSBezelStyle NSTexturedRoundedBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleTexturedRounded", macos(10.0,10.14)) = NSBezelStyleTexturedRounded;
static const NSBezelStyle NSRoundRectBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleRoundRect", macos(10.0,10.14)) = NSBezelStyleRoundRect;
static const NSBezelStyle NSRecessedBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleRecessed", macos(10.0,10.14)) = NSBezelStyleRecessed;
static const NSBezelStyle NSRoundedDisclosureBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleRoundedDisclosure", macos(10.0,10.14)) = NSBezelStyleRoundedDisclosure;
static const NSBezelStyle NSInlineBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleInline", macos(10.0,10.14)) = NSBezelStyleInline;

static const NSBezelStyle NSSmallIconButtonBezelStyle API_DEPRECATED("This bezel style is obsolete and should not be used.", macos(10.0,10.0)) = (NSBezelStyle)2;
static const NSBezelStyle NSThickSquareBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleRegularSquare", macos(10.0,10.12)) = (NSBezelStyle)3;
static const NSBezelStyle NSThickerSquareBezelStyle API_DEPRECATED_WITH_REPLACEMENT("NSBezelStyleRegularSquare", macos(10.0,10.12)) = (NSBezelStyle)4;

@interface NSButtonCell(NSDeprecated)

/* The NSGradientType enumeration and corresponding gradientType property are not used on macOS. */
@property NSGradientType gradientType API_DEPRECATED("The gradientType property is unused, and setting it has no effect.", macos(10.0,10.12));

/* Mnemonics, which are underlined characters in the button title that can be used as a keyboard shortcut, are not used on macOS. The related APIs for setting and querying the mnemonic should not be used. */
- (void)setTitleWithMnemonic:(null_unspecified NSString *)stringWithAmpersand API_DEPRECATED("Mnemonics are not used on macOS. Set the title property directly instead.", macos(10.0,10.8));
- (void)setAlternateTitleWithMnemonic:(null_unspecified NSString *)stringWithAmpersand API_DEPRECATED("Mnemonics are not used on macOS. Set the alternateTitle property directly instead.", macos(10.0,10.8));
- (void)setAlternateMnemonicLocation:(NSUInteger)location API_DEPRECATED("Mnemonics are not used on macOS. Calling this method has no effect.", macos(10.0,10.8));
- (NSUInteger)alternateMnemonicLocation API_DEPRECATED("Mnemonics are not used on macOS. This property always returns NSNotFound.", macos(10.0,10.8));
- (null_unspecified NSString *)alternateMnemonic API_DEPRECATED("Mnemonics are not used on macOS. This property always returns an empty string.", macos(10.0,10.8));

/* Buttons on macOS do not draw their key equivalents. The methods for specifying the key equivalent font are obsolete. */
@property (nullable, strong) NSFont *keyEquivalentFont API_DEPRECATED("The keyEquivalentFont property is no longer used. It always returns the NSButtonCell's font, and setting it has no effect.", macos(10.0,10.15));
- (void)setKeyEquivalentFont:(NSString *)fontName size:(CGFloat)fontSize API_DEPRECATED("The keyEquivalentFont property is no longer used. Setting it has no effect.", macos(10.0,10.15));

@end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

