/*
 NSPopoverTouchBarItem.h
 Application Kit
 Copyright (c) 2015-2021, Apple Inc.
 All rights reserved.
*/

#import <AppKit/NSTouchBarItem.h>

#if TARGET_OS_IPHONE
@class UIImage;
#endif

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.12.2), ios(13.0))
@interface NSPopoverTouchBarItem : NSTouchBarItem

/*
    The NSTouchBar displayed when this item is "popped." By default this is an empty bar that cannot be customized. This property is archived.
*/
@property (strong) NSTouchBar *popoverTouchBar;

/*
    The localized string labelling this item during user customization. The default value is the empty string. This property is archived.
*/
@property (readwrite, copy, null_resettable) NSString *customizationLabel;

/*
    The view displayed when the item is in its hosted NSTouchBar. By default, this is an NSButton whose target is this popover item, whose action is showPopover:, and whose image and title are bound to this item's collapsedRepresentationImage and collapsedRepresentationLabel respectively. This property is archived.
*/
@property (strong) __kindof NSView *collapsedRepresentation API_UNAVAILABLE(ios);

/*
    The image displayed by the button used by default for the default collapsed representation. If the collapsedRepresentation button has been replaced by a different view, this property may not have any effect. This property is archived.
*/
#if !TARGET_OS_IPHONE
@property (strong, nullable) NSImage *collapsedRepresentationImage;
#else
@property (strong, nullable) UIImage *collapsedRepresentationImage API_AVAILABLE(ios(13.0));
#endif

/*
    The localized string displayed by the button used by default for the default collapsed representation. If the collapsedRepresentation button has been replaced by a different view, this property may not have any effect. This property is archived.
*/
@property (strong) NSString *collapsedRepresentationLabel;

/*
    An NSTouchBar to be used exclusively for press-and-hold popovers. This NSTouchBar can be the same as the one used for "popoverTouchBar" property, but does not have to be. When non-nil this NSTouchBar will be displayed while the user holds their finger down on the collapsed representation and released when the user raises their finger. This tracking behavior is automatic, but popovers with custom collapsed representations will still need to send -showPopover: to start tracking.

    This property is archived.
*/
@property (strong, nullable) NSTouchBar *pressAndHoldTouchBar;

/*
    When YES, automatically displays a close button in the popover. When NO it is the responsibility of the client to dismiss the popover.
*/
@property BOOL showsCloseButton;

/*
    Replaces the main NSTouchBar with this item's popover NSTouchBar. If this item is not visible, this method will have no effect. If this item ceases to be visible, the popover NSTouchBar will automatically be ordered out.
*/
- (void)showPopover:(nullable id)sender;

/*
    This method will restore the previously visible main NSTouchBar. This method can be invoked explicitly to order out a popover if interacting with an item inside it should close it.
*/
- (void)dismissPopover:(nullable id)sender;

/* 
    Returns a new gesture recognizer, already wired up to send this popover -showPopover:. It is the callers responsibility to attach this GR to a custom collapsedRepresentation view 
*/
- (NSGestureRecognizer *)makeStandardActivatePopoverGestureRecognizer API_UNAVAILABLE(ios);
@end

NS_ASSUME_NONNULL_END
