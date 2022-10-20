/*
 NSTouchBarItem.h
 Application Kit
 Copyright (c) 2015-2021, Apple Inc.
 All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <Foundation/Foundation.h>

typedef NSString * NSTouchBarItemIdentifier NS_TYPED_EXTENSIBLE_ENUM API_AVAILABLE(ios(13.0));

@class NSTouchBar, NSViewController, NSView, NSImage, NSGestureRecognizer;
@class NSString;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef float NSTouchBarItemPriority NS_TYPED_EXTENSIBLE_ENUM API_AVAILABLE(macos(10.12.2), ios(13.0));

static const NSTouchBarItemPriority NSTouchBarItemPriorityHigh API_AVAILABLE(macos(10.12.2), ios(13.0)) = 1000;
static const NSTouchBarItemPriority NSTouchBarItemPriorityNormal API_AVAILABLE(macos(10.12.2), ios(13.0)) = 0;
static const NSTouchBarItemPriority NSTouchBarItemPriorityLow API_AVAILABLE(macos(10.12.2), ios(13.0)) = -1000;


API_AVAILABLE(macos(10.12.2), ios(13.0)) NS_SWIFT_UI_ACTOR
@interface NSTouchBarItem : NSObject <NSCoding>
/*
    The designated initializer. This instantiates a new NSTouchBarItem with the specified initializer.
*/
- (instancetype)initWithIdentifier:(NSTouchBarItemIdentifier)identifier NS_DESIGNATED_INITIALIZER API_AVAILABLE(ios(13.0));

/*
    Items can be archived and unarchived using NSCoder, as noted by conformance to the NSCoding protocol.
*/
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER API_AVAILABLE(ios(13.0));

/*
    Items must be instantiated with an identifier. Use -initWithIdentifier instead of -init.
*/
- (instancetype)init NS_UNAVAILABLE;

/*
    The identifier of this item. Apart from spaces, item identifiers should be globally unique.
*/
@property (readonly, copy) NSTouchBarItemIdentifier identifier API_AVAILABLE(ios(13.0));

/*
    If there are more items in the NSTouchBar than can be displayed, some will be hidden. Items with high visibility priority will be hidden after items with low visibility priority.
*/
@property NSTouchBarItemPriority visibilityPriority API_AVAILABLE(ios(13.0));

/*
    Intended for subclassing. By default, this method returns this item's view controller's view.
*/
@property (readonly, nullable) NSView *view;

/*
    Intended for subclassing. By default, this method returns nil.
*/
@property (readonly, nullable) NSViewController *viewController;

/*
    The user visible string identifying this item during customization. By default this method returns the empty string.
*/
@property (readonly, copy) NSString *customizationLabel API_AVAILABLE(ios(13.0));

/*
    When YES, this item is attached to a visible NSTouchBar, and is being displayed. Note that some types of items are never considered visible, for example spaces, other items proxys, and groups.
    This property is key value observable.
*/
@property (readonly, getter=isVisible) BOOL visible API_AVAILABLE(ios(13.0));
@end

/*
    The identifier of an item appropriate for use as a small space in the NSTouchBar. Generally, you can use this identifier in an NSTouchBar's itemIdentifiers array, and it will instantiate that space for you.
*/
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierFixedSpaceSmall API_AVAILABLE(macos(10.12.2), ios(13.0));

/*
    The identifier of an item appropriate for use as a large space in the NSTouchBar. Generally, you can use this identifier in an NSTouchBar's itemIdentifiers array, and it will instantiate that space for you.
*/
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierFixedSpaceLarge API_AVAILABLE(macos(10.12.2), ios(13.0));

/*
    The identifier of an item appropriate for use as a flexible space in the NSTouchBar. Generally, you can use this identifier in an NSTouchBar's itemIdentifiers array, and it will instantiate that space for you.
*/
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierFlexibleSpace API_AVAILABLE(macos(10.12.2), ios(13.0));

/*
    The identifier of the special "other items proxy." Generally, you can use this identifier in an NSTouchBar's itemIdentifiers array, and a special proxy item will be instantiated for you. When the NSTouchBar containing this item is visible, NSTouchBars provided by items closer to the first responder will be nested inside the space denoted for this item. Space items on either side of this item will be automatically massaged to handle cases where the NSTouchBar containing this identifier is itself the bar closest to the first responder (or closer bars are empty.)
    Note that an NSTouchBar lacking this item identifier will be replaced in its entirety by NSTouchBars closer to the first responder.
*/
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierOtherItemsProxy API_AVAILABLE(macos(10.12.2), ios(13.0));

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
