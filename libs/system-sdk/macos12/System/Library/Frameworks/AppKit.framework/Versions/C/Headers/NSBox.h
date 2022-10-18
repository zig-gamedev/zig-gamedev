/*
    NSBox.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSView.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSFont;

typedef NS_ENUM(NSUInteger, NSTitlePosition) {
    NSNoTitle     = 0,
    NSAboveTop    = 1,
    NSAtTop       = 2,
    NSBelowTop    = 3,
    NSAboveBottom = 4,
    NSAtBottom    = 5,
    NSBelowBottom = 6,
};

typedef NS_ENUM(NSUInteger, NSBoxType) {
    /* Group subviews with a standard look. This is the default style. */
    NSBoxPrimary = 0,

    /* Vertical or horizontal separtor line.  Not used with subviews. */
    NSBoxSeparator = 2,

    /* Custom boxes draw based entirely on user parameters; see the fillColor, borderColor, borderWidth, and cornerRadius properties for more information. */
    NSBoxCustom API_AVAILABLE(macos(10.5)) = 4,
};

@interface NSBox : NSView

@property NSBoxType boxType;
@property NSTitlePosition titlePosition;
@property (copy) NSString *title;
@property (strong) NSFont *titleFont;
@property (readonly) NSRect borderRect;
@property (readonly) NSRect titleRect;
@property (readonly, strong) id titleCell;
@property NSSize contentViewMargins;
- (void)sizeToFit;
- (void)setFrameFromContentFrame:(NSRect)contentFrame;

/* Get and set the content view for the box. Note that subviews added to the NSBox will be really added as subviews of the contentView. */
@property (nullable, strong) __kindof NSView *contentView;

/* Transparent boxes do not draw anything.  Subview drawing is unaffected.  The 'transparent' property corresponds to the binding 'NSTransparentBinding'. */
@property (getter=isTransparent) BOOL transparent API_AVAILABLE(macos(10.5));

/* For boxType == NSBoxCustom: Get and set the border width of the box. The default value is 1.0. */
@property CGFloat borderWidth API_AVAILABLE(macos(10.5));

/* For boxType == NSBoxCustom: Get and set the corner radius of the box. The default value is 0.0 (no corner radius). */
@property CGFloat cornerRadius API_AVAILABLE(macos(10.5));

/* For boxType == NSBoxCustom: Get and set the border color the box. The default value is +[NSColor secondaryLabelColor], and may vary release-to-release. */
@property (copy) NSColor *borderColor API_AVAILABLE(macos(10.5));

/* For boxType == NSBoxCustom: Get and set the fill (background) color the box. The default value is NSColor.clearColor. */
@property (copy) NSColor *fillColor API_AVAILABLE(macos(10.5));

@end

/* Deprecated Interfaces */

@interface NSBox (NSDeprecated)

@property NSBorderType borderType API_DEPRECATED("borderType is only applicable to NSBoxOldStyle, which is deprecated. To replace a borderType of NSNoBorder, use the `transparent` property.", macos(10.0, 10.15));

/* Mnemonics are deprecated in 10.8. Historically they have not done anything. For compatability, this method still calls setTitle: with the ampersand stripped from it. */
- (void)setTitleWithMnemonic:(null_unspecified NSString *)stringWithAmpersand API_DEPRECATED("", macos(10.0,10.8));
@end

/* The Secondary box type has been identical to Primary since Mac OS X 10.3. Use NSBoxPrimary instead. */
static const NSBoxType NSBoxSecondary API_DEPRECATED_WITH_REPLACEMENT("NSBoxPrimary", macos(10.0, 10.15)) = (NSBoxType)1;

/* Draws a border in the style of Mac OS X 10.2 and earlier. This style is not recommended. */
static const NSBoxType NSBoxOldStyle API_DEPRECATED("NSBoxOldStyle is discouraged in modern application design. It should be replaced with either NSBoxPrimary or NSBoxCustom.", macos(10.0, 10.15)) = (NSBoxType)3;




API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
