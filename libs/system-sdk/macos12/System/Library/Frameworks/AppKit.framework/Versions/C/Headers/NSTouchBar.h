/*
 NSTouchBar.h
 Application Kit
 Copyright (c) 2015-2021, Apple Inc.
 All rights reserved.
*/

#import <AppKit/NSTouchBarItem.h>
#if !TARGET_OS_IPHONE
#import <AppKit/NSApplication.h>
#import <AppKit/NSResponder.h>
#endif

NS_ASSUME_NONNULL_BEGIN

typedef NSString * NSTouchBarCustomizationIdentifier NS_SWIFT_BRIDGED_TYPEDEF API_AVAILABLE(ios(13.0));

@protocol NSTouchBarDelegate, NSTouchBarProvider;

API_AVAILABLE(macos(10.12.2), ios(13.0)) NS_SWIFT_UI_ACTOR
@interface NSTouchBar : NSObject <NSCoding>

/* 
    -init is a designated initializer.
*/
- (instancetype)init NS_DESIGNATED_INITIALIZER;

/* 
    -initWithCoder: is also a designated initializer.
*/
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* 
    A string uniquely identifying this bar for customization purposes. All bars with this identifier will have their items coordinated automatically during customization or instantiation.

    NSTouchBars lacking a customizationIdentifier are not customizable.
    
    This property is archived.
*/
@property (copy, nullable) NSTouchBarCustomizationIdentifier customizationIdentifier;

/*
    The items that are presented in the customization palette for the user to add to the NSTouchBar. These items will be presented to the user in the order specified in this array.

    This property is archived.
*/
@property (copy) NSArray<NSTouchBarItemIdentifier> *customizationAllowedItemIdentifiers;

/*
    Some items are too important to be removed. The corresponding item identifiers should be listed here. During customization the user will be prevented from removing these items from the NSTouchBar.
    
    This property is archived.
*/
@property (copy) NSArray<NSTouchBarItemIdentifier> *customizationRequiredItemIdentifiers;

/*
    An array of identifiers specifying the items in this NSTouchBar. When constructing the instantiated items array, the identifiers in this array will be fed through the -itemForIdentifier: method.
    Item identifiers should be globally unique, excepting NSTouchBarItemIdentifierFixedSpaceSmall, NSTouchBarItemIdentifierFixedSpaceLarge, NSTouchBarItemIdentifierFlexibleSpace, and NSTouchBarItemIdentifierOtherItemsProxy.
    
    This array also corresponds to the item ordering for the receiver in the “default set” in the customization palette.
 
    This property is archived.
*/
@property (copy) NSArray<NSTouchBarItemIdentifier> *defaultItemIdentifiers;

/*
    The resolved array of item identifiers. If the bar has not been customized this will match the defaultItemIdentifiers.
    
    This property is not archived.
*/
@property (copy, readonly) NSArray<NSTouchBarItemIdentifier> *itemIdentifiers;

/*
    Specifying a principal item identifier communicates that the item with that identifier has special significance to this NSTouchBar. Currently, that item will be placed in the center of the resolved  row. Note that multiple visible bars may each specify a principal item identifier - but only one of them can have the request honored.
    
    This property is archived.
*/
@property (copy, nullable) NSTouchBarItemIdentifier principalItemIdentifier;

/*
    An item identifier designating an item that replaces the standard system escape key. The item may be located in the templateItems set, or provided by the delegate.
    This property is not archived.
*/
@property (copy, nullable) NSTouchBarItemIdentifier escapeKeyReplacementItemIdentifier;

/*
    Items in this set are the first step in resolving instantiated items from their identifiers. If an item identifier is specified in the itemIdentifiers array, and an item with that identifier is in this set, it will be added to the items array in the corresponding location.
 
    This property is archived.
*/
@property (copy) NSSet<NSTouchBarItem *> *templateItems;

/*
    The NSTouchBar delegate. The delegate can dynamically create items.
    
    This property is conditionally archived (see -encodeConditionalObject:forKey:.)
*/
@property (nullable, weak) id <NSTouchBarDelegate> delegate;

/*
 Returns an instantiated NSTouchBarItem for the given identifier. Items are resolved from the following locations, in order:
 * items already in the instantiated items array
 * items in the defaultTouchBarItems set
 * items returned from the delegate's -touchBar:makeItemForIdentifier: method
 * some special identifiers are handled automatically
 NSTouchBarItemIdentifierFixedSpaceSmall -> NSTouchBar will automatically create a standard small space
 NSTouchBarItemIdentifierFixedSpaceLarge -> NSTouchBar will automatically create a standard large space
 NSTouchBarItemIdentifierFlexibleSpace -> NSTouchBar will automatically create a standard flexible space
 NSTouchBarItemIdentifierOtherItemsProxy -> NSTouchBar will automatically create a special item that acts as a proxy for the items of NSTouchBar closer to the first responder.
 */
- (nullable __kindof NSTouchBarItem *)itemForIdentifier:(NSTouchBarItemIdentifier)identifier;

/*
    When YES, the NSTouchBar is attached to an eligible NSTouchBar provider, and its items are displayable, assuming adequate space.
    This property is key value observable.
*/
@property (readonly, getter=isVisible) BOOL visible;

/*
 Convenience method for the NSApp's automaticCustomizeTouchBarMenuItemEnabled property.
 */
@property (class, getter=isAutomaticCustomizeTouchBarMenuItemEnabled) BOOL automaticCustomizeTouchBarMenuItemEnabled API_AVAILABLE(macos(10.15), ios(13.0));

@end

API_AVAILABLE(ios(13.0))
@protocol NSTouchBarDelegate <NSObject>
@optional
/*
    When constructing the items array, this delegate method will be invoked to construct an NSTouchBarItem if that item cannot be found in the `templateItems` set.
*/
- (nullable NSTouchBarItem *)touchBar:(NSTouchBar *)touchBar makeItemForIdentifier:(NSTouchBarItemIdentifier)identifier;
@end

/*
    Instances of NSTouchBar are discovered by searching certain well known components of the application for objects that conform to the NSTouchBarProvider protocol.
    
    Some specific objects in a process are searched to discover NSTouchBar providers. In order, these objects are:
    * the application delegate
    * the application object itself
    * the main window’s window controller
    * the main window’s delegate
    * the main window itself
    * the main window’s first responder
    * the key window’s window controller
    * the key window’s delegate
    * the key window itself
    * the key window’s first responder

    If any of these objects are an NSResponder or a subclass of NSResponder, the responder chain anchored at that object is also searched.

    For example in a complicated, but otherwise standard application, that chain might look like this.

    * Application delegate
    * Application
    * key window controller
    * key window delegate
    * key window
    * view controller (closest to root of window)
    * view (closest to root of window)
    * intermediate view controllers and views
    * key window’s first responder’s view controller
    * key window’s first responder

    NSTouchBars can be nested by including the special NSTouchBarItemIdentifierOtherItemsProxy item identifier. This allows more broadly scoped NSTouchBars (closer to the app delegate) to also display the contents of more narrowly scoped NSTouchBars (closer to the first responder.) If an NSTouchBar omits the special NSTouchBarItemIdentifierOtherItemsProxy item identifier, it will be hidden if a more narrowly scoped NSTouchBar is provided.
*/
API_AVAILABLE(ios(13.0))
@protocol NSTouchBarProvider <NSObject>
@required
/*
    The basic method for providing an NSTouchBar. AppKit will key value observe this property, if for some reason you wish to replace a live NSTouchBar wholesale.
    Note that many subclasses of NSResponder already implement this method and conform to this protocol.
*/
@property (strong, readonly, nullable) NSTouchBar *touchBar API_AVAILABLE(macos(10.12.2));
@end

#if !TARGET_OS_IPHONE
@interface NSResponder (NSTouchBarProvider) <NSTouchBarProvider>
/*
    The NSTouchBar object associated with this responder. If no NSTouchBar is explicitly set, NSResponder will send -makeTouchBar to itself to create the default NSTouchBar for this responder. This property is archived.
*/
@property (strong, readwrite, nullable) NSTouchBar *touchBar API_AVAILABLE(macos(10.12.2));

/*
    Subclasses should over-ride this method to create and configure the default NSTouchBar for this responder.
*/
- (nullable NSTouchBar *)makeTouchBar API_AVAILABLE(macos(10.12.2));
@end

@interface NSApplication (NSTouchBarCustomization)
/// Whether or not a menu item to customize the NSTouchBar can be automatically added to the main menu. It will only actually be added when Touch Bar hardware or simulator is present. Defaults to NO. Setting this property to YES is the recommended way to add the customization menu item. But if non-standard placement of the menu item is needed, creating a menu item with an action of `toggleTouchBarCustomizationPalette:` can be used instead.
@property (getter=isAutomaticCustomizeTouchBarMenuItemEnabled) BOOL automaticCustomizeTouchBarMenuItemEnabled API_AVAILABLE(macos(10.12.2));

/// Show or dismiss the customization palette for the currently displayed NSTouchBars. NSApplication validates this selector against whether the current NSTouchBars are customizable and, if configured on a menu item, will standardize and localize the title. If the current system does not have Touch Bar support, the menu item will be automatically hidden.
- (IBAction)toggleTouchBarCustomizationPalette:(nullable id)sender API_AVAILABLE(macos(10.12.2));
@end
#endif

NS_ASSUME_NONNULL_END
