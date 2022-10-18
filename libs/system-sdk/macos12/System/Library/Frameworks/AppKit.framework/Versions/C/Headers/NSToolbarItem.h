/*
	NSToolbarItem.h
	Application Kit
	Copyright (c) 2000-2021, Apple Inc.
	All rights reserved.
*/

#import <TargetConditionals.h>

#if !TARGET_OS_IPHONE
#import <AppKit/NSText.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSUserInterfaceValidation.h>
#endif
#import <AppKit/NSToolbar.h>
#import <Foundation/Foundation.h>
#import <Foundation/NSGeometry.h>

#if TARGET_OS_IPHONE
@class UIImage;
#endif

NS_ASSUME_NONNULL_BEGIN

@class NSMenuItem, NSView, NSImage, CKShare;

typedef NSInteger NSToolbarItemVisibilityPriority NS_TYPED_EXTENSIBLE_ENUM API_AVAILABLE(ios(13.0));
static const NSToolbarItemVisibilityPriority NSToolbarItemVisibilityPriorityStandard API_AVAILABLE(ios(13.0)) = 0;
static const NSToolbarItemVisibilityPriority NSToolbarItemVisibilityPriorityLow API_AVAILABLE(ios(13.0)) = -1000;
static const NSToolbarItemVisibilityPriority NSToolbarItemVisibilityPriorityHigh API_AVAILABLE(ios(13.0)) = 1000;
static const NSToolbarItemVisibilityPriority NSToolbarItemVisibilityPriorityUser API_AVAILABLE(ios(13.0)) = 2000;

API_AVAILABLE(ios(13.0)) NS_SWIFT_UI_ACTOR
@interface NSToolbarItem : NSObject <NSCopying>

/* Initialize the toolbar item with an identifier which is a development language string used by the toolbar and its delegate for identification purposes.  */
- (instancetype)initWithItemIdentifier:(NSToolbarItemIdentifier)itemIdentifier NS_DESIGNATED_INITIALIZER;
@property (readonly, copy) NSToolbarItemIdentifier itemIdentifier;

/* Use this to determine the toolbar in which an item is currently displayed. */
@property (readonly, nullable, weak) NSToolbar *toolbar;

// ----- Setters and Getters -----

/* Use this to set the item's label that appears in the toolbar.  The implication here is that the toolbar will draw the label for the item, and a redraw is triggered by this method.  The toolbar is in charge of the label area.  It is fine for an item to have no toolbar label.  Also, developers should make sure the length of the label is appropriate and not too long.  */
@property (copy) NSString *label;

/* Use this to set the item's label that appears when the item is in the customization palette.  All Items must have a palette label, and for most things it is reasonable to set them to the same string as the label used in the toolbar. */
@property (copy) NSString *paletteLabel;

/* Use this to set a tooltip to be used when the item is displayed in the toolbar.  (forwards to -view if it responds) */
@property (nullable, copy) NSString *toolTip;

/* The menu form of a toolbar item's purpose is twofold.  First, when the window is too small to display an item, it will be clipped but remain accessible from a "clipped items" menu containing the menu item returned here.  Second, in text only mode, the menu returned will be used to create the displayed items.  Singleton menu items will be clickable, while submenu items will be represented as a pull down.  For instance, say you want a button that allows you to switch between modes A, B, and C.  You could represent this as a menu by :  a menu item "mode" with three submenu items "A", "B", and "C".   By default, this method returns a singleton menu item with item label as the title.  For standard items, the target, action is set.  */
@property (nullable, strong) NSMenuItem *menuFormRepresentation API_UNAVAILABLE(ios);

/* Tag for your own custom purpose. (forwards to -view if it responds) */ 
@property NSInteger tag;

/* Set and get the action of an item. (forwards to -view if it responds) */
@property (nullable, weak) id target;

/* Set and get the action of an item. For custom views, this method will call setAction:/action on the view if it responds. (forwards to -view if it responds) */
@property (nullable) SEL action;

/* Set and get the enabled flag of an item.  For custom views, this method will call setEnabled:/isEnabled on the view if it responds. (forwards to -view if it responds) */
@property (getter=isEnabled) BOOL enabled;

/* Set and get the image of an item.  For custom views, this method will call setImage:/image on the view if it responds. (forwards to -view if it responds) */
#if !TARGET_OS_IPHONE
@property (nullable, strong) NSImage *image;
#else
@property (nullable, strong) UIImage *image API_AVAILABLE(ios(13.0));
#endif

/* Set and get the title of an item. For custom views, this method will call setTitle:/title on the view if it responds. (forwards to -view if it responds) */
@property (copy) NSString *title API_AVAILABLE(macos(10.15), ios(13.0));

/*
 When set on an item without a custom view, the button produced will have a bordered style.
 Defaults to NO.
 */
@property (getter=isBordered) BOOL bordered API_AVAILABLE(macos(10.15), ios(13.0));

/*
Whether or not the item behaves as a navigation item (i.e. back/forward) in the toolbar. Navigation items may be specially positioned by the system outside the normal list of items of the toolbar in the order specified by -toolbarDefaultItemIdentifiers:.
 */
@property (getter=isNavigational) BOOL navigational API_AVAILABLE(macos(11.0), ios(14.0));

/* Use setView: if you want your toolbar item to use something other than the standard.  Note that, by default, many of the set/get methods will be implemented by calls forwarded to the view you set, if it responds to it. */
@property (nullable, strong) NSView *view API_UNAVAILABLE(ios);

/*
 Unless you have already set your own custom view, you should not call these methods.
 The min size should be small enough to look nice in all display modes.
 If you do not set a min/max size, the view's size properties will be calculated using constraints. Apps linked before 10.14 will use the view's current size.
 In general, apps should rely on the automatic measurements and constraints to define min/max sizes rather than setting these properties since this will account for localizations.
 */
@property NSSize minSize API_DEPRECATED("This property is no longer recommended. Instead, let the system automatically measure the size of the view using constraints.", macos(10.0, 12.0)) API_UNAVAILABLE(ios);
@property NSSize maxSize API_DEPRECATED("This property is no longer recommended. Instead, let the system automatically measure the size of the view using constraints.", macos(10.0, 12.0)) API_UNAVAILABLE(ios);

/* When a toolbar does not have enough space to fit all its items, it must push some into the overflow menu.  Items with the highest visibility priority level are choosen last for the overflow menu.  The default visibilityPriority value is NSToolbarItemVisibilityPriorityStandard.  To suggest that an item always remain visible, give it a value greater than NSToolbarItemVisibilityPriorityStandard, but less than NSToolbarItemVisibilityPriorityUser.   In 10.7, users can no longer modify the toolbar item visibility priority. */
@property NSToolbarItemVisibilityPriority visibilityPriority;


// ----- Validation of the items -----

/* Typically you should not invoke this method.  This method is called by its toolbar during validation.  Standard items validate themselves by sending the validateToolbarItem: validate message to the current validator.  Since items with custom views don't always have meaningful target/actions, they do nothing.  So for your custom items it may be useful to override this method and invent your own validation. */
- (void)validate API_AVAILABLE(ios(13.0));


/* By default NSToolbar automatically invokes its items validate method on a regular basis.  To be in complete control of when the -validate method is invoked, you can disable automatic validation on a per-item basis.  In particular, if your validation code is slow, you may want to do this for performance reasons. */
@property BOOL autovalidates API_AVAILABLE(ios(13.0));



// ----- Controlling Duplicates In The Toolbar -----

/* Return YES to allow dragging duplicate items into the toolbar.  By default, if an item with the same identifier is already in the toolbar, dragging in will act as a move of this item.  However, for instance, the separator item drags in as a duplicate always. */
@property (readonly) BOOL allowsDuplicatesInToolbar API_UNAVAILABLE(ios);

@end

#if !TARGET_OS_IPHONE
@interface NSToolbarItem () <NSMenuItemValidation, NSValidatedUserInterfaceItem>
@end
#endif

#if !TARGET_OS_IPHONE

@protocol NSToolbarItemValidation <NSObject>

/* NSToolbarItemValidation extends the standard validation idea by introducing this new method which is sent to validators for each visible standard NSToolbarItem with a valid target/action pair.  Note: This message is sent from NSToolbarItem's validate method, however validate will not send this message for items that have custom views. */
- (BOOL)validateToolbarItem:(NSToolbarItem *)item;

@end

#if __swift__ < 40200
@interface NSObject (NSToolbarItemValidation)
- (BOOL)validateToolbarItem:(NSToolbarItem *)item API_DEPRECATED("This is now a method of the NSToolbarItemValidation protocol.", macos(10.0, 11.0));
@end
#endif

@protocol NSCloudSharingValidation <NSObject>

/* NSToolbarItems created with NSToolbarCloudSharingItemIdentifier use this method for further validation after sending `-validateToolbarItem:` or `-validateUserInterfaceItem:`. The validator for the item's action should return the current CKShare corresponding to the selected item, if any. The state of the item will be changed reflect the state of the CKShare. */
- (nullable CKShare *)cloudShareForUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)item;

@end

#endif

/* standard toolbar item identifiers */

APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarSeparatorItemIdentifier API_DEPRECATED("This item is no longer recommended and will be ignored on 10.7 and later.", macos(10.0, 11.0)) API_UNAVAILABLE(ios);
APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarSpaceItemIdentifier API_AVAILABLE(ios(13.0));
APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarFlexibleSpaceItemIdentifier API_AVAILABLE(ios(13.0));

APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarShowColorsItemIdentifier API_AVAILABLE(ios(13.0));        // Shows the color panel.
APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarShowFontsItemIdentifier API_AVAILABLE(ios(13.0));         // Shows the font panel.
APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarCustomizeToolbarItemIdentifier API_DEPRECATED("This item is no longer recommended and will be ignored on 10.7 and later.", macos(10.0, 11.0)) API_UNAVAILABLE(ios);
APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarPrintItemIdentifier API_AVAILABLE(ios(13.0));             // Sends printDocument: to firstResponder, but you can change this in toolbarWillAddItem: if you need to do so.
APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarToggleSidebarItemIdentifier API_AVAILABLE(macos(10.11), ios(13.0));  // A standard toolbar item identifier for sidebars. It sends -toggleSidebar: to the firstResponder.
APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarCloudSharingItemIdentifier API_AVAILABLE(macos(10.12)); // A standard toolbar item identifier for cloud sharing via NSSharingServiceNameCloudSharing. It validates itself and modifies its appearance by using the NSCloudSharingValidation protocol. It sends -performCloudSharing: to the firstResponder.

/*
Creates a new NSTrackingSeparatorToolbarItem and automatically configures it to track the divider of the sidebar if one is discovered.
*/
APPKIT_EXTERN NSToolbarItemIdentifier NSToolbarSidebarTrackingSeparatorItemIdentifier API_AVAILABLE(macos(11.0)) API_UNAVAILABLE(ios);

NS_ASSUME_NONNULL_END
