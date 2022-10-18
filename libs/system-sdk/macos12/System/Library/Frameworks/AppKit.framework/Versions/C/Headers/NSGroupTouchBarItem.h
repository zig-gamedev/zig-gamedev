/*
 NSGroupTouchBarItem.h
 Application Kit
 Copyright (c) 2015-2021, Apple Inc.
 All rights reserved.
*/

#import <AppKit/NSTouchBarItem.h>
#import <AppKit/NSUserInterfaceLayout.h>
#import <AppKit/NSUserInterfaceCompression.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.12.2), ios(13.0))
@interface NSGroupTouchBarItem : NSTouchBarItem

/*
    Returns an autoreleased NSGroupTouchBarItem whose groupTouchBar is built from the given items array.
    Customization is not enabled by default when creating an NSGroupTouchBarItem this way.
*/
+ (instancetype)groupItemWithIdentifier:(NSTouchBarItemIdentifier)identifier items:(NSArray<NSTouchBarItem *> *)items;

/*
    Creates a NSGroupTouchBarItem with the allowed compression options provided in the order AppKit defines.
    Default order: BreakEqualWidths, ReduceMetrics, HideText, HideImages.
    This should only be used with standard options provided by AppKit. Custom options need to be added using the prioritizedCompressionOptions property.
 */
+ (instancetype)groupItemWithIdentifier:(NSTouchBarItemIdentifier)identifier items:(NSArray<NSTouchBarItem *> *)items allowedCompressionOptions:(NSUserInterfaceCompressionOptions *)allowedCompressionOptions API_AVAILABLE(macos(10.13)) API_UNAVAILABLE(ios);

/*
    Creates a NSGroupTouchBarItem with the appropriate settings to match system style alerts in NSTouchBar.
    Spacing between items is under control of the client, but in general NSTouchBarItemIdentifierFixedSpaceLarge is recommended anywhere additional spacing is required.
    The groupUserInterfaceLayoutDirection will be set to [NSApp userInterfaceLayoutDirection].
 */
+ (instancetype)alertStyleGroupItemWithIdentifier:(NSTouchBarItemIdentifier)identifier API_AVAILABLE(macos(10.13));

/*
    An NSTouchBar, presented seamlessly as part of the NSTouchBar this item is hosted in.
    The NSTouchBar may have its own principal item, and can be customized (or not) per the normal customization rules.
    By default this is an empty bar that cannot be customized. This property is archived.
*/
@property (strong) NSTouchBar *groupTouchBar;

/*
    The localized string labelling this item during user customization. The default value is the empty string. This property is archived.
*/
@property (readwrite, copy, null_resettable) NSString *customizationLabel;

/*
    Get and set the user interface layout direction of the groupTouchBar, which defines the order in which the items should be laid out.
    By default, this value is NSUserInterfaceLayoutDirectionLeftToRight.
    To flip the layout direction of the group to the user’s preferred layout, this should be set to [NSApp userInterfaceLayoutDirection].
 */
@property NSUserInterfaceLayoutDirection groupUserInterfaceLayoutDirection API_AVAILABLE(macos(10.13)) API_UNAVAILABLE(ios);

/*
    When set, items in the associated groupTouchBar will be sized to have equal widths when possible.
    Defaults to NO.
 */
@property BOOL prefersEqualWidths API_AVAILABLE(macos(10.13));

/*
    The preferred width for items in the group when prefersEqualWidths is YES. This is the width that items will be set to if they fit, and the item doesn't clip.
    Optional; ignored if <0. Defaults to -1.
 */
@property CGFloat preferredItemWidth API_AVAILABLE(macos(10.13));

/*
    The currently active NSUserInterfaceCompressionOptions on the group.
 */
@property (readonly) NSUserInterfaceCompressionOptions *effectiveCompressionOptions API_AVAILABLE(macos(10.13)) API_UNAVAILABLE(ios);

/*
    The allowed compression options in the order they will be exercised.
    Client defined options should be added to specify the order in which they will be exercised.
    Options not included will be ignored.
    Defaults to an array containing all standard AppKit options in the order AppKit defines.
 */
@property (copy) NSArray<NSUserInterfaceCompressionOptions *> *prioritizedCompressionOptions API_AVAILABLE(macos(10.13)) API_UNAVAILABLE(ios);

@end

NS_ASSUME_NONNULL_END
