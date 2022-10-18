/*
        NSMenuItem.h
        Application Kit
        Copyright (c) 1996-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSUserInterfaceItemIdentification.h>
#import <AppKit/NSUserInterfaceValidation.h>
#import <AppKit/NSView.h>
#import <AppKit/NSCell.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSMenu;
@class NSImage, NSAttributedString, NSView;

@interface NSMenuItem : NSObject <NSCopying, NSCoding, NSValidatedUserInterfaceItem, NSUserInterfaceItemIdentification, NSAccessibilityElement, NSAccessibility>

@property (class) BOOL usesUserKeyEquivalents;

+ (NSMenuItem *)separatorItem;

- (instancetype)initWithTitle:(NSString *)string action:(nullable SEL)selector keyEquivalent:(NSString *)charCode NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* Never call the set method directly it is there only for subclassers.
 */
@property (nullable, assign) NSMenu *menu;


@property (readonly) BOOL hasSubmenu;
@property (nullable, strong) NSMenu *submenu;

/* Returns the NSMenuItem whose submenu contains the receiver, or nil if the receiver does not have a parent item.
*/
@property (nullable, readonly, assign) NSMenuItem *parentItem API_AVAILABLE(macos(10.6));

@property (copy) NSString *title;
@property (nullable, copy) NSAttributedString *attributedTitle;

@property (getter=isSeparatorItem, readonly) BOOL separatorItem;

@property (copy) NSString *keyEquivalent;
@property NSEventModifierFlags keyEquivalentModifierMask;

@property (readonly, copy) NSString *userKeyEquivalent;

/* By default, when a menu item is hidden, its key equivalent is ignored. By setting this property to YES, you allow a hidden item's key equivalent to be considered when searching for a menu item that matches a key event. This is useful to provide a keyboard shortcut when it's not necessary to have a visible menu item in the menubar. Note that Apple HI guidelines generally recommend that keyboard shortcuts should be clearly indicated in a menu, so this property should be used only rarely. */
@property BOOL allowsKeyEquivalentWhenHidden API_AVAILABLE(macos(10.13));

/* Suppose the system detects a given key equivalent that is not reachable in the current keyboard layout; it will localize the key equivalent to something reachable. By setting this property to NO, you will opt-out this menu item from the system-provided localization. YES by default for apps linked against 12.0 and later SDK. */
@property BOOL allowsAutomaticKeyEquivalentLocalization API_AVAILABLE(macos(12.0));

/* Suppose the system detects a given key equivalent with the following input string [ ] { } ( ) < > ← → in a right-to-left user interface environment (NSUserInterfaceLayoutDirectionRightToLeft); in that case, the system will automatically mirror the key equivalent. For example, a pair of menu items with key equivalents { and } will be localized to } and { in a right-to-left user interface. By setting this property to NO, you will opt-out this menu item of automatically mirroring in RTL. It would be best only to do this if your shortcut action will result in some sort of directional change in the UI, e.g. text alignment or a D-pad in a game. YES by default for apps linked against 12.0 and later SDK. */
@property BOOL allowsAutomaticKeyEquivalentMirroring API_AVAILABLE(macos(12.0));

@property (nullable, strong) NSImage *image;

@property NSControlStateValue state;
@property (null_resettable, strong) NSImage *onStateImage; // checkmark by default
@property (nullable, strong) NSImage *offStateImage; // none by default
@property (null_resettable, strong) NSImage *mixedStateImage; // horizontal line by default

@property (getter=isEnabled) BOOL enabled;

@property (getter=isAlternate) BOOL alternate;

@property NSInteger indentationLevel;

@property (nullable, weak) id target;
@property (nullable) SEL action;

@property NSInteger tag;

@property (nullable, strong) id representedObject;

/* Set (and get) the view for a menu item.  By default, a menu item has a nil view.
A menu item with a view does not draw its title, state, font, or other standard drawing attributes, and assigns drawing responsibility entirely to the view.  Keyboard equivalents and type-select continue to use the key equivalent and title as normal.
A menu item with a view sizes itself according to the view's frame, and the width of the other menu items.  The menu item will always be at least as wide as its view, but it may be wider.  If you want your view to auto-expand to fill the menu item, then make sure that its autoresizing mask has NSViewWidthSizable set; in that case, the view's width at the time setView: is called will be treated as the minimum width for the view.  A menu will resize itself as its containing views change frame size.  Changes to the view's frame during tracking are reflected immediately in the menu.
A view in a menu item will receive mouse and keyboard events normally.  During non-sticky menu tracking (manipulating menus with the mouse button held down), a view in a menu item will receive mouseDragged: events.
Animation is possible via the usual mechanism (set a timer to call setNeedsDisplay: or display), but because menu tracking occurs in the NSEventTrackingRunLoopMode, you must add the timer to the run loop in that mode.
When the menu is opened, the view is added to a window; when the menu is closed the view is removed from the window.  Override viewDidMoveToWindow in your view for a convenient place to start/stop animations, reset tracking rects, etc., but do not attempt to move or otherwise modify the window.
When a menu item is copied via NSCopying, any attached view is copied via archiving/unarchiving.  Menu item views are not supported in the Dock menu. */
@property (nullable, strong) NSView *view API_AVAILABLE(macos(10.5));

/* Indicates whether the menu item should be drawn highlighted or not. */
@property (getter=isHighlighted, readonly) BOOL highlighted API_AVAILABLE(macos(10.5));

/* Set (and get) the visibility of a menu item.  Hidden menu items (or items with a hidden superitem) do not appear in a menu and do not participate in command key matching.  isHiddenOrHasHiddenAncestor returns YES if the item is hidden or any of its superitems are hidden. */
@property (getter=isHidden) BOOL hidden API_AVAILABLE(macos(10.5));
@property (getter=isHiddenOrHasHiddenAncestor, readonly) BOOL hiddenOrHasHiddenAncestor API_AVAILABLE(macos(10.5));


@property (nullable, copy) NSString *toolTip;

@end

@interface NSView (NSViewEnclosingMenuItem)
/* Returns the menu item containing the receiver or any of its superviews in the view hierarchy, or nil if the receiver's view hierarchy is not in a menu item. */
@property (nullable, readonly, strong) NSMenuItem *enclosingMenuItem API_AVAILABLE(macos(10.5));
@end

APPKIT_EXTERN NSUserInterfaceItemIdentifier const NSMenuItemImportFromDeviceIdentifier API_AVAILABLE(macos(10.14)); // Continuity Camera menu item.

// The NSMenuItem protocol is deprecated.  Use the NSMenuItem class in your code.
#if defined(__GNUC__) && (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 2)
API_DEPRECATED("", macos(10.0,10.6))
#endif
@protocol NSMenuItem;

/* The following methods are deprecated.  They have never done anything useful in Mac OS X. */
@interface NSMenuItem (NSDeprecated)

- (void)setMnemonicLocation:(NSUInteger)location API_DEPRECATED("", macos(10.0,10.6));
- (NSUInteger)mnemonicLocation API_DEPRECATED("", macos(10.0,10.6));
- (null_unspecified NSString *)mnemonic API_DEPRECATED("", macos(10.0,10.6));
- (void)setTitleWithMnemonic:(NSString *)stringWithAmpersand API_DEPRECATED_WITH_REPLACEMENT("setTitle:", macos(10.0,10.13));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
