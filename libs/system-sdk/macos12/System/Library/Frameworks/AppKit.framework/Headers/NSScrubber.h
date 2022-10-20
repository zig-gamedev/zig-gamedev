/*
 NSScrubber.h
 Application Kit
 Copyright (c) 2016-2021, Apple Inc.
 All rights reserved.
 */

#import <AppKit/NSControl.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSScrubber, NSScrubberItemView, NSScrubberSelectionView, NSScrubberLayout, NSPanGestureRecognizer, NSPressGestureRecognizer, NSButton, NSNib;

#pragma mark Data Source & Delegate Protocols

@protocol NSScrubberDataSource <NSObject>
@required
- (NSInteger)numberOfItemsForScrubber:(NSScrubber *)scrubber API_AVAILABLE(macos(10.12.2));
- (__kindof NSScrubberItemView *)scrubber:(NSScrubber *)scrubber viewForItemAtIndex:(NSInteger)index API_AVAILABLE(macos(10.12.2));
@end

@protocol NSScrubberDelegate <NSObject>
@optional
- (void)scrubber:(NSScrubber *)scrubber didSelectItemAtIndex:(NSInteger)selectedIndex API_AVAILABLE(macos(10.12.2));
- (void)scrubber:(NSScrubber *)scrubber didHighlightItemAtIndex:(NSInteger)highlightedIndex API_AVAILABLE(macos(10.12.2));
- (void)scrubber:(NSScrubber *)scrubber didChangeVisibleRange:(NSRange)visibleRange API_AVAILABLE(macos(10.12.2));

- (void)didBeginInteractingWithScrubber:(NSScrubber *)scrubber API_AVAILABLE(macos(10.12.2));
- (void)didFinishInteractingWithScrubber:(NSScrubber *)scrubber API_AVAILABLE(macos(10.12.2));
- (void)didCancelInteractingWithScrubber:(NSScrubber *)scrubber API_AVAILABLE(macos(10.12.2));
@end

#pragma mark - Associated Types

/*!
 * @typedef NSScrubberMode
 * @discussion Determines the interaction mode for a NSScrubber control.
 *
 * @const NSScrubberModeFixed Panning over the control does not scroll, but instead highlights the element under the user’s finger. The highlighted element is selected the end of the gesture. If the gesture begins on top of the selected element, or if the @c continuous property is set to @c YES, the selection is changed immediately as the user pans.
 * @const NSScrubberModeFree  Panning over the control freely scrolls the scrubber content. Items are selected by tapping or pressing them without panning. If the @c continuous property is set to @c YES, the control automatically selects items as they scroll under the axis specified by the @c itemAlignment property; if @c itemAlignment is @c NSScrubberAlignmentNone, it is interpreted as @c NSScrubberAlignmentCenter for this purpose.
 */
typedef NS_ENUM(NSInteger, NSScrubberMode) {
    NSScrubberModeFixed = 0,
    NSScrubberModeFree
} API_AVAILABLE(macos(10.12.2));

/*!
 * @typedef NSScrubberAlignment
 * @discussion NSScrubberAlignment specifies the preferred alignment of elements within the control.
 *
 * @const NSScrubberAlignmentNone      Specifies no preference for item alignment.
 * @const NSScrubberAlignmentLeading   Specifies that an item will be leading-aligned within the control.
 * @const NSScrubberAlignmentTrailing  Specifies that an item will be trailing-aligned within the control.
 * @const NSScrubberAlignmentCenter    Specifies that an item will be center-aligned within the control.
 */
typedef NS_ENUM(NSInteger, NSScrubberAlignment) {
    NSScrubberAlignmentNone = 0,
    NSScrubberAlignmentLeading,
    NSScrubberAlignmentTrailing,
    NSScrubberAlignmentCenter
} API_AVAILABLE(macos(10.12.2));

/*!
 * @class NSScrubberSelectionStyle
 * @abstract @c NSScrubberSelectionStyle is an abstract class that provides decorative accessory views for selected and highlighted items within a NSScrubber control. Class properties provide convenient access to built-in styles. For a completely custom style, subclassers can override @c -makeSelectionView to create and configure arbitrary @c NSScrubberSelectionView subclasses.
 *
 */
API_AVAILABLE(macos(10.12.2)) NS_SWIFT_UI_ACTOR
@interface NSScrubberSelectionStyle : NSObject <NSCoding>

#pragma mark Built-in Styles

@property (class, strong, readonly) NSScrubberSelectionStyle *outlineOverlayStyle;
@property (class, strong, readonly) NSScrubberSelectionStyle *roundedBackgroundStyle;

#pragma mark Initialization & View Creation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (nullable __kindof NSScrubberSelectionView *)makeSelectionView;

@end

#pragma mark - Scrubber Control

/*!
 * @class NSScrubber
 * @abstract @c NSScrubber is a control designed for the NSTouchBar environment. 
 *
 * @c NSScrubber arranges a finite number of "items" (represented by views of type @c NSScrubberItemView ) according to a layout object (see @c NSScrubberLayout ), and provides several methods for navigating and selecting those items.
 *
 * Clients provide data to @c NSScrubber via a data source object (see the @c NSScrubberDataSource protocol) and react to user interaction via a delegate object (see the @c NSScrubberDelegate protocol).
 *
 */
API_AVAILABLE(macos(10.12.2))
@interface NSScrubber : NSView

#pragma mark Properties

@property (weak, nullable) id<NSScrubberDataSource> dataSource;
@property (weak, nullable) id<NSScrubberDelegate> delegate;
@property (strong) __kindof NSScrubberLayout *scrubberLayout;

/// Returns the number of items represented by the scrubber control.
@property (readonly) NSInteger numberOfItems;

/// The index of the currently highlighted item within the control. If there is no highlighted item, the value of this property is (-1).
@property (readonly) NSInteger highlightedIndex;

/// The index of the selected item within the control. If there is no selected item, the value of this property is (-1). Setting this property through the animator proxy will animate the selection change. Programmatic selection changes do not trigger delegate callbacks.
@property NSInteger selectedIndex;

/// Describes the interaction mode for the scrubber control. See the @c NSScrubberMode enumeration for a list of possible values. The default value is @c NSScrubberModeFixed.
@property NSScrubberMode mode;

/// If the value of @c itemAlignment is not @c NSScrubberAlignmentNone, the scrubber will ensure that some item rests at the preferred alignment within the control following a scrolling or paging interaction. The default value is @c NSScrubberAlignmentNone.
@property NSScrubberAlignment itemAlignment;

/// When @c continuous is @c YES, panning over the control in @c NSScrubberModeFixed will immediately select the item under the user's finger, and scrolling in @c NSScrubberModeFree will continuously select items as they pass through the current @c itemAlignment. The default is @c NO.
@property (getter=isContinuous) BOOL continuous;

/// When @c floatsSelectionViews is @c YES, the selection decorations provided by @c selectionBackgroundStyle and @c selectionOverlayStyle will smoothly float between selected items, rather than animating their entrance/exit in-place. The default is @c NO.
@property BOOL floatsSelectionViews;

/// Specifies a style of decoration to place behind items that are selected and/or highlighted. The default value is @c nil, indicating no built-in background decoration.
@property (strong, nullable) NSScrubberSelectionStyle *selectionBackgroundStyle;

/// Specifies a style of decoration to place above items that are selected and/or highlighted. The default value is @c nil, indicating no built-in overlay decoration.
@property (strong, nullable) NSScrubberSelectionStyle *selectionOverlayStyle;

/// If @c showsArrowButtons is @c YES, the control provides leading and trailing arrow buttons. Tapping an arrow button moves the selection index by one element; pressing and holding repeatedly moves the selection. The default is @c NO.
@property BOOL showsArrowButtons;

/// If @c showsAdditionalContentIndicators is @c YES, the control will draw a fade effect to indicate that there is additional unscrolled content. The default is @c NO.
@property BOOL showsAdditionalContentIndicators;

/// If set, @c backgroundColor is displayed behind the scrubber content. The background color is suppressed if the scrubber is assigned a non-nil @c backgroundView. The default value is @c nil.
@property (copy, nullable) NSColor *backgroundColor;

/// If non-nil, the @c backgroundView is displayed below the scrubber content. The view's layout is managed by @c NSScrubber to match the content area. If this property is non-nil, the @c backgroundColor property has no effect. The default value is @c nil.
@property (strong, nullable) NSView *backgroundView;

#pragma mark Initialization

- (instancetype)initWithFrame:(NSRect)frameRect NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

#pragma mark Data Manipulation

/// Invalidate all data within the scrubber control, triggering a reload of all content, and clearing the current selection.
- (void)reloadData;

/// Updates inside the @c performSequentialBatchUpdates block are processed and displayed all at once, including insertion, removal, moving, reloading items, and changing the selected index. Changes are performed iteratively using the same semantics as @c NSMutableArray. NSScrubber expects its dataSource to reflect the changes made inside @c -performSequentialBatchUpdates: immediately after the @c updateBlock finishes executing.
- (void)performSequentialBatchUpdates:(void(NS_NOESCAPE ^)(void))updateBlock;

/// Inserts new items at the specified indexes. NSScrubber will request views for each new index from the @c dataSource. This method uses the same semantics as @c NSMutableArray; each index in the set specifies the destination index after all previous insertions have occurred. Therefore, an NSIndexSet of [1,2,3] will result in three new contiguous items.
- (void)insertItemsAtIndexes:(NSIndexSet *)indexes;

/// Removes the items at the specified indexes. This method uses the same semantics as @c NSMutableArray.
- (void)removeItemsAtIndexes:(NSIndexSet *)indexes;

/// Reloads the items at the specified indexes. NSScrubber will request new views for each item and smoothly crossfade between them before discarding the original views.
- (void)reloadItemsAtIndexes:(NSIndexSet *)indexes;

/// Moves an item from one index to another. @c oldIndex refers to the item's index prior to the movement, whereas @c newIndex refers to the item's final location.
- (void)moveItemAtIndex:(NSInteger)oldIndex toIndex:(NSInteger)newIndex;

#pragma mark Interaction

/// Scrolls an item to a given alignment within the control. If @c NSScrubberAlignmentNone is provided, then the control scrolls the minimum amount necessary to make the item visible. Scrolling is animated if called on the animator proxy.
- (void)scrollItemAtIndex:(NSInteger)index toAlignment:(NSScrubberAlignment)alignment;

/// Returns the @c NSScrubberItemView for the given index, if one currently exists; returns @c nil otherwise.
- (nullable __kindof NSScrubberItemView *)itemViewForItemAtIndex:(NSInteger)index;

#pragma mark Item Reuse Queue

/// Registers a @c NSScrubberItemView class to be instantiated for the given @c itemIdentifier. Raises an exception if @c itemViewClass is not a subclass of @c NSScrubberItemView. Passing @c nil for @c itemViewClass removes a previous registration. Registrations made through this method do not persist through NSCoding.
- (void)registerClass:(nullable Class)itemViewClass forItemIdentifier:(NSUserInterfaceItemIdentifier)itemIdentifier;

/// Register a nib to be instantiated for the given @c itemIdentifier. The nib must contain a top-level object which is a subclass of NSScrubberItemView; otherwise, @c -makeItemWithIdentifier: may return @c nil for this identifier. Passing @c nil for @c nib removes a previous registration.
- (void)registerNib:(nullable NSNib *)nib forItemIdentifier:(NSUserInterfaceItemIdentifier)itemIdentifier;

/// Creates or reuses a @c NSScrubberItemView corresponding to the provided @c itemIdentifier. @c NSScrubber searches, in order: the reuse queue, the list of registered classes, and then the list of registered nibs. If the reuse queue is empty, and there is no Class or Interface Builder archive registered for the @c itemIdentifier, this method returns @c nil.
- (nullable __kindof NSScrubberItemView *)makeItemWithIdentifier:(NSUserInterfaceItemIdentifier)itemIdentifier owner:(nullable id)owner;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
