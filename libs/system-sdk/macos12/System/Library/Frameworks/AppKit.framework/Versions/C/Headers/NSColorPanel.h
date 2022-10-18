/*
	NSColorPanel.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSPanel.h>
#import <AppKit/NSApplication.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColorList, NSMutableArray;

typedef NS_ENUM(NSInteger, NSColorPanelMode) {
    /* If the color panel is not displaying a mode, the NSColorPanelModeNone will be returned */
    NSColorPanelModeNone API_AVAILABLE(macos(10.5)) = -1,
    NSColorPanelModeGray            = 0,
    NSColorPanelModeRGB             = 1,
    NSColorPanelModeCMYK            = 2,
    NSColorPanelModeHSB             = 3,
    NSColorPanelModeCustomPalette   = 4,
    NSColorPanelModeColorList       = 5,
    NSColorPanelModeWheel           = 6,
    NSColorPanelModeCrayon          = 7
};

typedef NS_OPTIONS(NSUInteger, NSColorPanelOptions) {
    NSColorPanelGrayModeMask		= 0x00000001,
    NSColorPanelRGBModeMask		= 0x00000002,
    NSColorPanelCMYKModeMask		= 0x00000004,
    NSColorPanelHSBModeMask		= 0x00000008,
    NSColorPanelCustomPaletteModeMask	= 0x00000010,
    NSColorPanelColorListModeMask	= 0x00000020,
    NSColorPanelWheelModeMask		= 0x00000040,
    NSColorPanelCrayonModeMask		= 0x00000080,
    NSColorPanelAllModesMask		= 0x0000ffff
};
    
    

@interface NSColorPanel : NSPanel

@property (class, readonly, strong) NSColorPanel *sharedColorPanel;
@property (class, readonly) BOOL sharedColorPanelExists;
+ (BOOL)dragColor:(NSColor *)color withEvent:(NSEvent *)event fromView:(NSView *)sourceView;
+ (void)setPickerMask:(NSColorPanelOptions)mask;
+ (void)setPickerMode:(NSColorPanelMode)mode;

@property (nullable, strong) NSView *accessoryView;
@property (getter=isContinuous) BOOL continuous;
@property BOOL showsAlpha;
@property NSColorPanelMode mode;
@property (copy) NSColor *color;
@property (readonly) CGFloat alpha;
- (void)setAction:(nullable SEL)selector;
- (void)setTarget:(nullable id)target;
- (void)attachColorList:(NSColorList *)colorList;
- (void)detachColorList:(NSColorList *)colorList;
@end

@interface NSApplication(NSColorPanel)
- (void)orderFrontColorPanel:(nullable id)sender;
@end

@protocol NSColorChanging <NSObject>
- (void)changeColor:(nullable NSColorPanel *)sender;
@end

#if __swift__ < 40200
@interface NSObject(NSColorPanelResponderMethod)
- (void)changeColor:(nullable id)sender API_DEPRECATED("This is now a method of the NSColorChanging protocol.", macos(10.0, 11.0));
@end
#endif

/* Notifications */
APPKIT_EXTERN NSNotificationName NSColorPanelColorDidChangeNotification;


static const NSColorPanelMode NSNoModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeNone", macos(10.5,10.14)) = NSColorPanelModeNone;
static const NSColorPanelMode NSGrayModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeGray", macos(10.0,10.14)) = NSColorPanelModeGray;
static const NSColorPanelMode NSRGBModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeRGB", macos(10.0,10.14)) = NSColorPanelModeRGB;
static const NSColorPanelMode NSCMYKModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeCMYK", macos(10.0,10.14)) = NSColorPanelModeCMYK;
static const NSColorPanelMode NSHSBModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeHSB", macos(10.0,10.14)) = NSColorPanelModeHSB;
static const NSColorPanelMode NSCustomPaletteModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeCustomPalette", macos(10.0,10.14)) = NSColorPanelModeCustomPalette;
static const NSColorPanelMode NSColorListModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeColorList", macos(10.0,10.14)) = NSColorPanelModeColorList;
static const NSColorPanelMode NSWheelModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeWheel", macos(10.0,10.14)) = NSColorPanelModeWheel;
static const NSColorPanelMode NSCrayonModeColorPanel API_DEPRECATED_WITH_REPLACEMENT("NSColorPanelModeCrayon", macos(10.0,10.14)) = NSColorPanelModeCrayon;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

