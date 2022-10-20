/*
    NSCollectionView.h
    Application Kit
    Copyright (c) 2005-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSView.h>
#import <AppKit/NSViewController.h>
#import <AppKit/NSDragging.h>
#import <AppKit/AppKitDefines.h>
#import <Foundation/NSArray.h>

typedef NS_ENUM(NSInteger, NSCollectionViewDropOperation) {
    NSCollectionViewDropOn = 0,
    NSCollectionViewDropBefore = 1,
} API_AVAILABLE(macos(10.6));

typedef NS_ENUM(NSInteger, NSCollectionViewItemHighlightState) {
    NSCollectionViewItemHighlightNone = 0,
    NSCollectionViewItemHighlightForSelection = 1,
    NSCollectionViewItemHighlightForDeselection = 2,
    NSCollectionViewItemHighlightAsDropTarget = 3,
} API_AVAILABLE(macos(10.11));

/* Used with NSCollectionView's -selectItemsAtIndexPaths:scrollPosition: and -scrollToItemsAtIndexPaths:scrollPosition: to specify how the bounding box of the specified items should be positioned within the CollectionView's visibleRect.  NSCollectionView will try to satisfy this request as closely as feasible, while staying within the enclosing ClipView's scrollable range.
*/
typedef NS_OPTIONS(NSUInteger, NSCollectionViewScrollPosition) {
    NSCollectionViewScrollPositionNone                 = 0,
    
    /* The vertical positions are mutually exclusive to each other, but are bitwise or-able with the horizontal scroll positions.  Combining positions from the same grouping (horizontal or vertical) will result in an NSInvalidArgumentException.
    */
    NSCollectionViewScrollPositionTop                   = 1 << 0,
    NSCollectionViewScrollPositionCenteredVertically    = 1 << 1,
    NSCollectionViewScrollPositionBottom                = 1 << 2,
    NSCollectionViewScrollPositionNearestHorizontalEdge = 1 << 9, /* Nearer of Top,Bottom */
    
    /* Likewise, the horizontal positions are mutually exclusive to each other.
    */
    NSCollectionViewScrollPositionLeft                 = 1 << 3,
    NSCollectionViewScrollPositionCenteredHorizontally = 1 << 4,
    NSCollectionViewScrollPositionRight                = 1 << 5,
    NSCollectionViewScrollPositionLeadingEdge          = 1 << 6, /* Left if LTR, Right if RTL */
    NSCollectionViewScrollPositionTrailingEdge         = 1 << 7, /* Right if LTR, Left, if RTL */
    NSCollectionViewScrollPositionNearestVerticalEdge  = 1 << 8, /* Nearer of Leading,Trailing */
};

typedef NSString * NSCollectionViewSupplementaryElementKind NS_SWIFT_BRIDGED_TYPEDEF;

@class NSButton, NSCollectionView, NSCollectionViewLayout, NSCollectionViewLayoutAttributes, NSCollectionViewTransitionLayout, NSDraggingImageComponent, NSImageView, NSIndexSet, NSMutableIndexSet, NSNib, NSTextField;
@protocol NSCollectionViewDataSource, NSCollectionViewDelegate, NSCollectionViewPrefetching;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

/* NSCollectionViewElement declares functionality shared by (1) NSCollectionViewItems and (2) "supplementary" or "decoration" views that can be added to an NSCollectionView.  Each such reusable entity has the ability to be reset to its initial state by being sent -prepareForReuse, the ability to take size, position, and other state from an NSCollectionViewLayoutAttributes instance, and the ability to respond to transitions from one layout to another.  Note that, since these methods are all optional and invoked only when found to be present, all existing NSView classes can be considered conforming, and are therefore eligible to be used a supplementary and decorative views.  NSView and NSCollectionViewElement both conform to NSUserInterfaceItemIdentification, which provides an "identifier" property that CollectionView uses to track an item or supplementary/decoration view's kind.
 */
@protocol NSCollectionViewElement <NSObject, NSUserInterfaceItemIdentification>
@optional

/* Called by the NSCollectionView before the instance is returned from the reuse queue.  Subclassers must call super.  NSCollectionViewItem's implementation of this method sends -prepareForReuse to the ViewController's view.
 */
- (void)prepareForReuse API_AVAILABLE(macos(10.11));

/* Classes that want to support custom layout attributes specific to a given NSCollectionViewLayout subclass can apply them here.  This allows the view to work in conjunction with a layout class that returns a custom subclass of NSCollectionViewLayoutAttributes from -layoutAttributesForItem: or the corresponding layoutAttributesForHeader/Footer methods.  -applyLayoutAttributes: is then called after the view is added to the collection view and just before the view is returned from the reuse queue.  Note that -applyLayoutAttributes: is only called when attributes change, as defined by -isEqual:.
 */
- (void)applyLayoutAttributes:(NSCollectionViewLayoutAttributes *)layoutAttributes API_AVAILABLE(macos(10.11));

/* Override points for participating in layout transitions.  These messages are sent to a reusable part before and after the transition to a new layout occurs.
 */
- (void)willTransitionFromLayout:(NSCollectionViewLayout *)oldLayout toLayout:(NSCollectionViewLayout *)newLayout API_AVAILABLE(macos(10.11));
- (void)didTransitionFromLayout:(NSCollectionViewLayout *)oldLayout toLayout:(NSCollectionViewLayout *)newLayout API_AVAILABLE(macos(10.11));

/* Invoked when present to give a reusable entity the opportunity to make any desired final adjustments to its layout.
 */
- (NSCollectionViewLayoutAttributes *)preferredLayoutAttributesFittingAttributes:(NSCollectionViewLayoutAttributes *)layoutAttributes API_AVAILABLE(macos(10.11));

@end

/* Section header views can conform to this protocol, to enable a CollectionView to find the section collapse toggle button (when present).
*/
@protocol NSCollectionViewSectionHeaderView <NSCollectionViewElement>
@optional

/* If your header contains a button that's set up to toggle section collapse, wire this outlet to it.  This enables CollectionView to automatically show and hide the button, based on whether the section's items all fit.
*/
@property (nullable, weak) IBOutlet NSButton *sectionCollapseButton API_AVAILABLE(macos(10.12));

@end

/* An NSCollectionViewItem associates a visual representation (view subtree) with a representedObject of arbitrary type.  It also tracks whether the representedObject is part of the enclosing NSCollectionView's current selection.  Note that NSCollectionViewItem inherits some useful properties -- in particular, "representedObject" and "view" -- from NSViewController.
*/

API_AVAILABLE(macos(10.5))
@interface NSCollectionViewItem : NSViewController <NSCopying, NSCollectionViewElement>

/* Non-retained backlink to the containing CollectionView.
*/
@property (readonly, nullable, weak) NSCollectionView *collectionView;

/* Whether the item is part of its collectionView's current selection.
*/
@property (getter=isSelected) BOOL selected;

/* Whether the item should be shown as highlighted for proposed selection, deselection, as a drop target, or not highlighted.
*/
@property NSCollectionViewItemHighlightState highlightState API_AVAILABLE(macos(10.11));

/* Convenience outlet to the item's primary NSImageView (if any).
*/
@property (nullable, weak) IBOutlet NSImageView *imageView API_AVAILABLE(macos(10.7));

/* Convenience outlet to the item's primary NSTextField (if any).
*/
@property (nullable, weak) IBOutlet NSTextField *textField API_AVAILABLE(macos(10.7));

/* Multi-image drag and drop support. The default implementation will return an array of up to two NSDraggingImageComponent instances -- one for the imageView and another for the textField (if not nil). This methods can be subclassed and overridden to provide a custom set of NSDraggingImageComponents to create the drag image. Note: the component frames are relative to a coordinate system that has its origin at the bottom left, so you need to take into account the flippedness of your view when computing the component frames.
*/
@property (readonly) NSArray<NSDraggingImageComponent *> *draggingImageComponents API_AVAILABLE(macos(10.7));

@end

    
/* An NSCollectionView presents an ordered collection of items, with flexible, customizable appearance and layout.  The user may be permitted to select items, and/or drag items into and out of the CollectionView.
*/
API_AVAILABLE(macos(10.5))
@interface NSCollectionView : NSView <NSDraggingSource, NSDraggingDestination>

#pragma mark *** Data Source / Model Objects ***

/* A non-retained data source object, that the CollectionView will consult to obtain information about the model objects to be represented and the number of sections to divide them among, and to make (recycle or instantiate) items and supplementary views.  See the NSCollectionViewDataSource protocol, declared below, for the methods this delegate should implement.

Defaults to nil, which makes the CollectionView look to its "content" property or NSContentBinding for model object information.  The "content" property should only be used with the default "Grid" collectionViewLayout and legacy -newItemForRepresentedObject: / itemPrototype mechanisms for instantiating items.

To get the new capabilities and behaviors, and use the new NSCollectionView APIs added in OS X 10.11, you must specify a dataSource.  Setting dataSource != nil causes the CollectionView to empty its "content" array, and attempting to set "content" to a non-empty array while dataSource != nil is considered a programming error that will cause an exception to be raised.
*/
@property (nullable, weak) id<NSCollectionViewDataSource> dataSource API_AVAILABLE(macos(10.11));

/* An optional assistant to the CollectionView's 'dataSource' (potentially the same object), that receives notifications conducive to managing item data pre-fetching.
*/
@property (nullable, weak) id<NSCollectionViewPrefetching> prefetchDataSource API_AVAILABLE(macos(10.13));

/* The array of model objects that will be shown as "items" in this CollectionView.  The objects may be of any type(s).  If you prefer not to give the CollectionView a "dataSource", you can set its "content" property explicitly, or bind the CollectionView's NSContentBinding to an NSArrayController's arrangedObjects property, to provide the CollectionView with its model objects.  A CollectionView whose content is provided in this way cannot have more than one section.

Each corresponding "item" instantiated by the CollectionView will have its representedObject property wired to the corresponding model object from this "content" array.  Attempts to set "content" to nil are ignored on OS X 10.10 and earlier.  On OS X 10.11 and later, setting "content" to nil raises an exception.  (Set "content" to an empty NSArray instead, if you want to empty the CollectionView.)
*/
@property (copy) NSArray<id> *content;

/* Call this method to reload all of the items in the collection view.  This causes the collection view to discard any currently visible items and redisplay them.  For efficiency, the collection view only displays those items and supplementary views that are visible.  If the collection data shrinks as a result of the reload, the collection view adjusts its scroll position accordingly.  You should not call this method in the middle of animation blocks where items are being inserted or deleted.  Insertions and deletions automatically cause the table’s data to be updated appropriately.
*/
- (void)reloadData API_AVAILABLE(macos(10.11));


#pragma mark *** Delegate ***

/* An optional, non-retained delegate object, that will have the opportunity to influence the CollectionView's drag-and-drop, selection, highlighting, and layout transitioning behaviors.  See the NSCollectionViewDelegate protocol, declared below, for the methods this delegate may implement.  Defaults to nil, which leaves the CollectionView to determine its own behaviors.
*/
@property (nullable, weak) id<NSCollectionViewDelegate> delegate;


#pragma mark *** Decoration ***

/* An optional background view that's positioned underneath all of the CollectionView's content.  Defaults to nil.  The backgroundView's scrolling behavior and frame are determined by the "backgroundViewScrollsWithContent" property, as described below.  If "backgroundColors" are also specified for the CollectionView, backgroundColor[0] is drawn anywhere the backgroundView's content allows it to show through.
*/
@property (nullable, strong) NSView *backgroundView API_AVAILABLE(macos(10.11));

/* When YES, the CollectionView's backgroundView (if any) will match the CollectionView's frame and scroll with the CollectionView's items and other content.  When NO (the default, compatible with the behavior on OS X 10.11), the backgroundView is made to fill the CollectionView's visible area, and remains stationary when the CollectionView's content is scrolled.  Archived with the CollectionView's other persistent properties.
*/
@property BOOL backgroundViewScrollsWithContent API_AVAILABLE(macos(10.12));


#pragma mark *** Layout ***

/* The layout object that determines how the NSCollectionView's items will be sized and positioned.  Defaults to an instance of NSCollectionViewGridLayout whose maxNumberOfRows, maxNumberOfColumns, minItemSize, maxItemSize, and backgroundColors properties mirror the NSCollectionView's.  Replace this object, or change the values of its parameters, to change the layout.  collectionViewLayout may be set to nil for an NSCollectionView that is not using the dataSource-based API.

To get an animated transition to the new layout, use [[collectionView animator] setCollectionViewLayout:].  You can use NSAnimationContext's completionHandler provisions to notify you when the transition is complete.
*/
@property (nullable, strong) __kindof NSCollectionViewLayout *collectionViewLayout API_AVAILABLE(macos(10.11));

/* Returns the layout information for the item at the specified index path (or nil if no such item exists).  Use this method to retrieve the layout information for a particular item.  You should always use this method instead of querying the layout object directly.
*/
- (nullable NSCollectionViewLayoutAttributes *)layoutAttributesForItemAtIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Returns the layout information for the specified supplementary view.

Use this method to retrieve the layout information for a particular supplementary view. You should always use this method instead of querying the layout object directly.
*/
- (nullable NSCollectionViewLayoutAttributes *)layoutAttributesForSupplementaryElementOfKind:(NSCollectionViewSupplementaryElementKind)kind atIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Returns the frame calculated by the receiver where it intends to place the subview for the NSCollectionViewItem at the given index. You can use this method in the draggingImage methods to determine which views are in the visible portion of the enclosing scroll view. Overriding this method will have no effect on the receiver's subview layout. For apps that target OS X 10.11 and later, you may want to use -layoutAttributesForItemAtIndexPath: instead, which provides more than just the item's frame. This method raises an exception if invoked for a CollectionView that has more than one section. Use -layoutAttributesForItemAtIndexPath: instead.
*/
- (NSRect)frameForItemAtIndex:(NSUInteger)index API_AVAILABLE(macos(10.6));

/* Returns the frame calculated by the receiver where it would place a subview for an NSCollectionViewItem at the given index in a collection with the given number of items. You should use this method when updating NSDraggingItem frames when your collection is a drag destination. You should not use -frameForItemAtIndex:, since a drag and drop operation may change the number of items in the collection, which affects the layout of the item views. This method raises an exception if invoked for a CollectionView that has more than one section. Use -layoutAttributesForItemAtIndexPath: instead.
*/
- (NSRect)frameForItemAtIndex:(NSUInteger)index withNumberOfItems:(NSUInteger)numberOfItems API_AVAILABLE(macos(10.7));

/* The first background color in the array is used as the background of the entire collection view. Passing nil resets the background colors back to the default.
*/
@property (null_resettable, copy) NSArray<NSColor *> *backgroundColors;

#pragma mark *** Section and Item Counts ***

/* The number of sections displayed by the collection view.
*/
@property(readonly) NSInteger numberOfSections API_AVAILABLE(macos(10.11));

/* Returns the number of items in the specified "section".
*/
- (NSInteger)numberOfItemsInSection:(NSInteger)section API_AVAILABLE(macos(10.11));

#pragma mark *** Selection State ***

/* Whether the CollectionView is its window's current firstResponder.  This bindable property is useful for determining appropriate appearance for selected items.
 */
@property (getter=isFirstResponder, readonly) BOOL firstResponder;

/* Whether the user can select items in the CollectionView.  Defaults to NO.  Changing this from YES to NO clears the CollectionView's current selection (if any), causing selectionIndexes to become an empty index set.
 */
@property (getter=isSelectable) BOOL selectable;

/* Whether the CollectionView allows its selection to become empty.  Defaults to YES.  When this is set to NO and "selectable" is YES, the CollectionView will attempt to always leave at least one item selected, when the CollectionView has at least one item.
 */
@property BOOL allowsEmptySelection API_AVAILABLE(macos(10.11));

/* Whether the user can simultaneously select multiple items in the CollectionView.  Defaults to NO, which means only one item at a time (or none) may be selected.  Changing this from YES to NO causes the CollectionView's selection to be reduced to the selected item with the lowest index in the section with the lowest index, if more than one item was selected.
 */
@property BOOL allowsMultipleSelection;

/* (Soft-Deprecated)  The indexes of the items that are currently "selected", indicating that user operations should target the "content" objects having these indexes.  Getting or setting this property raises an exception on 10.11 and later, if the CollectionView has more than one section.  Use "selectionIndexPaths" instead.
 */
@property (copy) NSIndexSet *selectionIndexes;

/* A set of of NSIndexPath values, that identify the currently selected items by (section,indexInSection).  When using the new NSCollectionView APIs on 10.11 and later, use this property instead of "selectionIndexes".  You can change the set of selected items by setting this property.  To request an animated transition of affected items between their selected and deselected appearances, you can set this property via the CollectionView's "animator" proxy: [[collectionView animator] setSelectionIndexPaths:newIndexPaths].  CollectionView will raise an exception if any of the given indexPaths is out-of-bounds.  This property is Key-Value Observable.
 */
@property (copy) NSSet<NSIndexPath *> *selectionIndexPaths API_AVAILABLE(macos(10.11));

/* Adds the specified items to the selection, and optionally scrolls their bounding box into view according to the value of "scrollPosition".  This method does not cause any selection-related delegate methods to be invoked.
 
 To animate the scroll (if any) and the view property changes that indicate selected state, use [[collectionView animator] selectItemsAtIndexPaths:scrollPosition:].
 */
- (void)selectItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths scrollPosition:(NSCollectionViewScrollPosition)scrollPosition API_AVAILABLE(macos(10.11));

/* Removes the specified items from the selection, potentially in an animated way.  This method does not cause any selection-related delegate methods to be invoked.
 
 To animate the view property changes that indicate deselected state, use [[collectionView animator] deselectItemsAtIndexPaths:].
 */
- (void)deselectItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Selects all of the CollectionView's items, if and only if it is "selectable" and it "allowsMultipleSelection", and notifies the CollectionView's delegate about any newly selected items.  If either "selectable" or "allowsMultipleSelection" is NO, this action silently does nothing (and its associated user interface item does not validate).
 */
- (IBAction)selectAll:(nullable id)sender;

/* Clears the CollectionView's selection (if any), if and only if it is "selectable", and notifies the CollectionView's delegate about any newly deselected items.  If "selectable" is NO, this action silently does nothing (and its associated user interface item does not validate).
 */
- (IBAction)deselectAll:(nullable id)sender;


/* For each item identifier that the collection view will use, register either a nib or class from which to instantiate an item, or provide a nib file or class whose name matches the identifier you plan to use.  If a nib is registered, it must contain exactly one top-level NSCollectionViewItem.  If a class is registered instead of a nib, it will be instantiated via alloc/init.
 */
- (void)registerClass:(nullable Class)itemClass forItemWithIdentifier:(NSUserInterfaceItemIdentifier)identifier API_AVAILABLE(macos(10.11));
- (void)registerNib:(nullable NSNib *)nib forItemWithIdentifier:(NSUserInterfaceItemIdentifier)identifier API_AVAILABLE(macos(10.11));

/* For each supplementary view identifier that the collection view will use, register either a nib or class from which to instantiate a view, or provide a nib file or class whose name matches the identifier you plan to use.  If a nib is registered, it must contain exactly one top-level view, that conforms to the NSCollectionViewElement protocol.  If a class is registered instead of a nib, it will be instantiated via alloc/initWithFrame:.
 */
- (void)registerClass:(nullable Class)viewClass forSupplementaryViewOfKind:(NSCollectionViewSupplementaryElementKind)kind withIdentifier:(NSUserInterfaceItemIdentifier)identifier API_AVAILABLE(macos(10.11));
- (void)registerNib:(nullable NSNib *)nib forSupplementaryViewOfKind:(NSCollectionViewSupplementaryElementKind)kind withIdentifier:(NSUserInterfaceItemIdentifier)identifier API_AVAILABLE(macos(10.11));

/* Call this method from your data source object when asked to provide a new item for the collection view.  This method dequeues an existing item if one is available or creates a new one based on the nib file or class you previously registered.  If you have not registered a nib file or class for the given identifier, CollectionView will try to load a nib file named identifier.nib, or (failing that) find and instantiate an NSCollectionViewItem subclass named "identifier".
 
 If a new item must be created from a class, this method initializes the item by invoking its -init method.  For nib-based items, this method loads the item from the provided nib file.  If an existing item was available for reuse, this method invokes the item's -prepareForReuse method instead.
 */
- (__kindof NSCollectionViewItem *)makeItemWithIdentifier:(NSUserInterfaceItemIdentifier)identifier forIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Call this method from your data source object when asked to provide a new supplementary view for the collection view.  This method dequeues an existing view if one is available or creates a new one based on the nib file or class you previously registered.  If you have not registered a nib file or class for the given identifier, CollectionView will try to load a nib file named identifier.nib, or (failing that) find and instantiate an NSView subclass named "identifier".
 
 If a new view must be created from a class, this method initializes the view by invoking its -initWithFrame: method. For nib-based views, this method loads the view from the provided nib file.  If an existing view was available for reuse, this method invokes the view's -prepareForReuse method instead.
 */
- (__kindof NSView *)makeSupplementaryViewOfKind:(NSCollectionViewSupplementaryElementKind)elementKind withIdentifier:(NSUserInterfaceItemIdentifier)identifier forIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

#pragma mark *** Finding Items ***

/* (Soft-Deprecated)  Returns the NSCollectionViewItem associated with the represented object at the given index.  Sending this to a CollectionView that has more than one section raises an exception.  Use -itemAtIndexPath: instead.
 */
- (nullable NSCollectionViewItem *)itemAtIndex:(NSUInteger)index API_AVAILABLE(macos(10.6));

/* Returns the NSCollectionViewItem (if any) associated with the represented object at the given indexPath.  This method returns nil if the CollectionView isn't currently maintaining an NSCollectionViewItem instance for the given indexPath, as may be the case if the specified item is outside the CollectionView's visibleRect.
*/
- (nullable NSCollectionViewItem *)itemAtIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Returns the NSCollectionViewItems that the CollectionView has instantiated and is managing as currently active. Each such item is associated with an item indexPath, and its view is part of the currently displayed view hierarchy. Note that this list may include items whose views fall outside the CollectionView's current visibleRect (for example, recently visible items that have been scrolled out of view, or items that the CollectionView anticipates may soon be visible). To determine which of these items may actually be visible to the user, test each item.view.frame for intersection with the CollectionView's visibleRect.
*/
- (NSArray<NSCollectionViewItem *> *)visibleItems API_AVAILABLE(macos(10.11));

/* Returns the index paths of the items that are currently displayed by the CollectionView. Note that these indexPaths correspond to the same items as "visibleItems", and thus may include items whose views fall outside the CollectionView's current "visibleRect".
*/
- (NSSet<NSIndexPath *> *)indexPathsForVisibleItems API_AVAILABLE(macos(10.11));

/* Returns the index path of the specified item (or nil if the specified item is not in the collection view).
*/
- (nullable NSIndexPath *)indexPathForItem:(NSCollectionViewItem *)item API_AVAILABLE(macos(10.11));

/* Returns the index path of the item at the specified point in the collection view.
 
 This method relies on the layout information provided by the associated layout object to determine which item contains the point.  It does not take opacity of the item's displayed content at that point into account.
*/
- (nullable NSIndexPath *)indexPathForItemAtPoint:(NSPoint)point API_AVAILABLE(macos(10.11));


#pragma mark *** Finding Supplementary Views ***

/* Returns the supplementary view (if any) of the given elementKind that's associated with the given indexPath.  Returns nil if no such supplementary view is currently instantiated.
 */
- (nullable NSView<NSCollectionViewElement> *)supplementaryViewForElementKind:(NSCollectionViewSupplementaryElementKind)elementKind atIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Returns the supplementary views that the CollectionView has instantiated and is managing as currently active.  Each such supplementary view is associated with an indexPath, and is part of the currently displayed view hierarchy.  Note that this list may include supplementary views that fall outside the CollectionView's current visibleRect (for example, recently visible section header or footer views that have been scrolled out of view, or section header or footer views that the CollectionView anticipates may soon be visible). To determine which of these views may actually be visible to the user, test each view.frame for intersection with the CollectionView's visibleRect.
 */
- (NSArray<NSView<NSCollectionViewElement> *> *)visibleSupplementaryViewsOfKind:(NSCollectionViewSupplementaryElementKind)elementKind API_AVAILABLE(macos(10.11));

/* Returns the index paths of the supplementary views of the given elementKind that are currently displayed by the CollectionView.  Note that these indexPaths correspond to the same supplementary views that "visibleSupplementaryViewsOfKind:" reports, and thus may include supplementary views that fall outside the CollectionView's current "visibleRect".
 */
- (NSSet<NSIndexPath *> *)indexPathsForVisibleSupplementaryElementsOfKind:(NSCollectionViewSupplementaryElementKind)elementKind API_AVAILABLE(macos(10.11));


#pragma mark *** Modifying Items and Sections ***

/* Use the following methods to communicate imminent model changes to a dataSource-based CollectionView.  Their function is similar to that of NSOutlineView's insert/move/remove methods.

When invoking any of these methods, you can request an animated instead of immediate layout update by messaging the CollectionView's animator.  For example: [[collectionView animator] insertItemsAtIndexPaths:indexPaths]
*/

/* Use this method to insert one or more sections into the collection view. This method adds the sections, and it is up to your data source to report the number of items in each section when asked for the information. The collection view then uses that information to get updated layout attributes for the newly inserted sections and items. If the insertions cause a change in the collection view’s visible content, those changes are animated into place.

You can also call this method from a block passed to the performBatchUpdates:completionHandler: method when you want to animate multiple separate changes into place at the same time. See the description of that method for more information.

Raises an exception if "sections" is nil.
*/
- (void)insertSections:(NSIndexSet *)sections API_AVAILABLE(macos(10.11));

/* Use this method to remove the sections and their items from the collection view. You might do this when you remove the sections from your data source object or in response to user interactions with the collection view. The collection view updates the layout of the remaining sections and items to account for the deletions, animating the remaining items into position as needed.

You can also call this method from a block passed to the performBatchUpdates:completionHandler: method when you want to animate multiple separate changes into place at the same time. See the description of that method for more information.

Raises an exception if "sections" is nil.
*/
- (void)deleteSections:(NSIndexSet *)sections API_AVAILABLE(macos(10.11));

/* Call this method to selectively reload only the objects in the specified sections. This causes the collection view to discard any items associated with those objects and redisplay them.

Raises an exception if "sections" is nil.
*/
- (void)reloadSections:(NSIndexSet *)sections API_AVAILABLE(macos(10.11));

/* Use this method to reorganize existing sections and their contained items. You might do this when you rearrange sections within your data source object or in response to user interactions with the collection view. The collection view updates the layout as needed to account for the move, animating new views into position as needed.

You can also call this method from a block passed to the performBatchUpdates:completionHandler: method when you want to animate multiple separate changes into place at the same time. See the description of that method for more information.
*/
- (void)moveSection:(NSInteger)section toSection:(NSInteger)newSection API_AVAILABLE(macos(10.11));

/* Call this method to insert one or more new items into the collection view. You might do this when your data source object receives data for new items or in response to user interactions with the collection view. The collection view gets the layout information for the new items as part of calling this method. And if the layout information indicates that the items should appear onscreen, the collection view asks your data source to provide the appropriate views, animating them into position as needed.

You can also call this method from a block passed to the performBatchUpdates:completionHandler: method when you want to animate multiple separate changes into place at the same time. See the description of that method for more information.

Raises an exception if "indexPaths" is nil.
*/
- (void)insertItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Use this method to remove items from the collection view. You might do this when you remove the represented objects from your data source object or in response to user interactions with the collection view. The collection view updates the layout of the remaining items to account for the deletions, animating the remaining items into position as needed.

You can also call this method from a block passed to the performBatchUpdates:completionHandler: method when you want to animate multiple separate changes into place at the same time. See the description of that method for more information.

Raises an exception if "indexPaths" is nil.
*/
- (void)deleteItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Call this method to selectively reload only the specified objects. This causes the collection view to discard any items associated with those objects and redisplay them.

Raises an exception if "indexPaths" is nil.
*/
- (void)reloadItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Use this method to reorganize existing data items. You might do this when you rearrange the represented objects within your data source object or in response to user interactions with the collection view. You can move items between sections or within the same section. The collection view updates the layout as needed to account for the move, animating items into position as needed.

You can also call this method from a block passed to the performBatchUpdates:completionHandler: method when you want to animate multiple separate changes into place at the same time. See the description of that method for more information.

Raises an exception if "indexPath" or "newIndexPath" is nil.
*/
- (void)moveItemAtIndexPath:(NSIndexPath *)indexPath toIndexPath:(NSIndexPath *)newIndexPath API_AVAILABLE(macos(10.11));

/* Animates multiple insert, delete, reload, and move operations as a group.

You can use this method in cases where you want to make multiple changes to the collection view in one single animated operation, as opposed to in several separate animations. You might use this method to insert, delete, reload or move items or use it to change the layout parameters associated with one or more items. Use the block passed in the "updates" parameter to specify all of the operations you want to perform.

Deletes are processed before inserts in batch operations. This means the indexes for the insertions are processed relative to the indexes of the state before the batch operation, and the indexes for the deletions are processed relative to the indexes of the state after all the insertions in the batch operation.

Invocations of this method can be nested.
*/
- (void)performBatchUpdates:(void (NS_NOESCAPE ^_Nullable)(void))updates completionHandler:(void (^_Nullable)(BOOL finished))completionHandler API_AVAILABLE(macos(10.11));


#pragma mark *** Section Collapse ***

/* Toggles collapse of the CollectionView section that the sender resides in.  Typically you'll wire this action from a section header view's "sectionCollapse" button.  (See the NSCollectionViewSectionHeaderView protocol.)
 */
- (IBAction)toggleSectionCollapse:(id)sender API_AVAILABLE(macos(10.12));


#pragma mark *** Scrolling ***

/* Scrolls the collection view contents until the bounding box of the specified items is visible.  The final position of that bounding box within the scrollable area is determined by "scrollPosition".

To request an animated scroll, use [[collectionView animator] scrollToItemsAtIndexPaths:scrollPosition:].  You can use NSAnimationContext's completionHandler provisions to notify you when the animated scroll has finished.
*/
- (void)scrollToItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths scrollPosition:(NSCollectionViewScrollPosition)scrollPosition API_AVAILABLE(macos(10.11));


#pragma mark *** Drag and Drop ***

/* Configures the value returned from -draggingSourceOperationMaskForLocal:. An isLocal value of YES indicates that 'dragOperationMask' applies when the destination object is in the same application. By default, NSDragOperationEvery will be returned. An isLocal value of NO indicates that 'dragOperationMask' applies when the destination object is in an application outside the receiver's application. By default, NSDragOperationNone is returned. NSCollectionView will save the values you set for each isLocal setting. You typically will invoke this method, and not override it.
*/
- (void)setDraggingSourceOperationMask:(NSDragOperation)dragOperationMask forLocal:(BOOL)localDestination API_AVAILABLE(macos(10.6));

/* This method computes and returns an image to use for dragging. You can override this to return a custom drag image, or call it from the delegate method to get the default drag image. 'indexPaths' contains the NSIndexPaths of the items being dragged. 'event' is a reference to the mouse down event that began the drag. 'dragImageOffset' is an in/out parameter. This method will be called with dragImageOffset set to NSZeroPoint, but it can be modified to re-position the returned image. A dragImageOffset of NSZeroPoint will cause the image to be centered under the mouse. By default, an image will be created that contains a rendering of the visible portions of the views for each item. If the delegate implements the equivalent delegate method, it will be preferred over this method.
*/
- (NSImage *)draggingImageForItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths withEvent:(NSEvent *)event offset:(NSPointPointer)dragImageOffset API_AVAILABLE(macos(10.11));

/* Old Form (Single Section Only) */
/* This is an older variant of -draggingImageForItemsAtIndexPaths:withEvent:offset:.  It assumes all of the indexes are in section 0.  New code should use -draggingImageForItemsAtIndexPaths:withEvent:offset: instead.
*/
- (NSImage *)draggingImageForItemsAtIndexes:(NSIndexSet *)indexes withEvent:(NSEvent *)event offset:(NSPointPointer)dragImageOffset API_AVAILABLE(macos(10.6));

@end


@protocol NSCollectionViewDataSource <NSObject>
@required

/* Asks the data source for the number of items in the specified section.
 */
- (NSInteger)collectionView:(NSCollectionView *)collectionView numberOfItemsInSection:(NSInteger)section API_AVAILABLE(macos(10.11));

/* Asks the data source to provide an NSCollectionViewItem for the specified represented object.

Your implementation of this method is responsible for creating, configuring, and returning the appropriate item for the given represented object.  You do this by sending -makeItemWithIdentifier:forIndexPath: method to the collection view and passing the identifier that corresponds to the item type you want.  Upon receiving the item, you should set any properties that correspond to the data of the corresponding model object, perform any additional needed configuration, and return the item.

You do not need to set the location of the item's view inside the collection view’s bounds. The collection view sets the location of each item automatically using the layout attributes provided by its layout object.

This method must always return a valid item instance.
*/
- (NSCollectionViewItem *)collectionView:(NSCollectionView *)collectionView itemForRepresentedObjectAtIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

@optional

/* Asks the data source for the number of sections in the collection view.

If you do not implement this method, the collection view assumes it has only one section.
*/
- (NSInteger)numberOfSectionsInCollectionView:(NSCollectionView *)collectionView API_AVAILABLE(macos(10.11));

/* Asks the data source to provide a view for the specified supplementary element.

Your implementation of this method is responsible for creating, configuring, and returning an appropriate view.  You do this by sending -makeSupplementaryViewOfKind:withIdentifier:forIndexPath: to the collection view and passing the identifier that corresponds to the supplementary view type you want.  Upon receiving the view, you should set any desired appearance properties, perform any additional needed configuration, and return the item.

You do not need to set the location of the view inside the collection view’s bounds. The collection view sets the location of each supplementary view automatically using the layout attributes provided by its layout object.

This method must always return a valid view.
*/
- (NSView *)collectionView:(NSCollectionView *)collectionView viewForSupplementaryElementOfKind:(NSCollectionViewSupplementaryElementKind)kind atIndexPath:(NSIndexPath *)indexPath;

@end


@protocol NSCollectionViewPrefetching <NSObject>

@required

/* Notifies your 'prefetchDataSource' that items at the specified 'indexPaths' are likely to be instantiated and displayed soon.  The CollectionView sends this as early as possible, to give your 'prefetchDataSource' the opportunity to begin acquiring any resources such as thumbnail images or metadata that you might need in order to prepare the item's content for display.  The given 'indexPaths' are sorted by the order in which they are likely to be needed.
*/
- (void)collectionView:(NSCollectionView *)collectionView prefetchItemsAtIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;

@optional

/* Notifies your 'prefetchDataSource' that items at the specified 'indexPaths', for which the CollectionView previously sent -collectionView:prefetchItemsAtIndexPaths:, are no longer expected to be displayed.  This is a good opportunity to cancel any pending resource fetches you've initiated for the items, if possible and worthwhile.  This is only sent for items that don't end up being displayed; CollectionView doesn't cancel prefetching for items that it actually instantiates and shows.
*/
- (void)collectionView:(NSCollectionView *)collectionView cancelPrefetchingForItemsAtIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;

@end


@protocol NSCollectionViewDelegate <NSObject>
@optional

/* Drag and drop support */

/* To enable dealing with (section,item) NSIndexPaths now that NSCollectionView supports sections, many of the drag-and-drop delegate methods have been replaced with new versions that take NSIndexPath and array-of-NSIndexPath parameters.  In each such case, NSCollectionView will look for and invoke the newer method first.  If the delegate doesn't respond to the newer version, NSCollectionView will look for the old method, provided that the NSCollectionView has only a single section.  The old methods will not be invoked for a multi-section NSCollectionView.
*/

/* The return value indicates whether the collection view can attempt to initiate a drag for the given event and items. If the delegate does not implement this method, the collection view will act as if it returned YES.
*/
- (BOOL)collectionView:(NSCollectionView *)collectionView canDragItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths withEvent:(NSEvent *)event API_AVAILABLE(macos(10.11));

/* Old Form (Single Section Only) */
- (BOOL)collectionView:(NSCollectionView *)collectionView canDragItemsAtIndexes:(NSIndexSet *)indexes withEvent:(NSEvent *)event API_AVAILABLE(macos(10.6));

/* This method is called after it has been determined that a drag should begin, but before the drag has been started. To refuse the drag, return NO. To start the drag, declare the pasteboard types that you support with -[NSPasteboard declareTypes:owner:], place your data for the items at the given index paths on the pasteboard, and return YES from the method. The drag image and other drag related information will be set up and provided by the view once this call returns YES. You need to implement this method, or -collectionView:pasteboardWriterForItemAtIndexPath: (its more modern counterpart), for your collection view to be a drag source.  If you want to put file promises on the pasteboard, using the modern NSFilePromiseProvider API added in macOS 10.12, implement -collectionView:pasteboardWriterForItemAtIndexPath: instead of this method, and have it return an NSFilePromiseProvider.
*/
- (BOOL)collectionView:(NSCollectionView *)collectionView writeItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths toPasteboard:(NSPasteboard *)pasteboard API_DEPRECATED("Use -collectionView:pasteboardWriterForItemAtIndexPath: instead", macos(10.11, 10.15));

/* Old Form (Single Section Only) */
- (BOOL)collectionView:(NSCollectionView *)collectionView writeItemsAtIndexes:(NSIndexSet *)indexes toPasteboard:(NSPasteboard *)pasteboard API_DEPRECATED("Use -collectionView:pasteboardWriterForItemAtIndexPath: instead", macos(10.6, 10.15));

/* This is a legacy method for file promise dragging, that's invoked when the delegate has placed NSFilesPromisePboardType data on the dragging pasteboard (typically in -collectionView:writeItemsAtIndexPaths:toPasteboard:). When using file promises and targeting macOS 10.12 and later, you should instead implement -collectionView:pasteboardWriterForItemAtIndexPath: to return a fully configured NSFilePromiseProvider, whose delegate provides the file name resolution functionality that was previously delegated to this method.

 NSCollectionView implements -namesOfPromisedFilesDroppedAtDestination: to return the results of this delegate method. This method should return an array of filenames (not full paths) for the created files. `dropURL` specifies the drop location. You do not need to implement this method for your collection view to be a drag source.
 */
- (NSArray<NSString *> *)collectionView:(NSCollectionView *)collectionView namesOfPromisedFilesDroppedAtDestination:(NSURL *)dropURL forDraggedItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_DEPRECATED("Use NSFilePromiseReceiver objects instead", macos(10.11,10.13));

/* Old Form (Single Section Only) */
- (NSArray<NSString *> *)collectionView:(NSCollectionView *)collectionView namesOfPromisedFilesDroppedAtDestination:(NSURL *)dropURL forDraggedItemsAtIndexes:(NSIndexSet *)indexes API_DEPRECATED("Use NSFilePromiseReceiver objects instead", macos(10.6,10.13));

/* Allows the delegate to construct a custom dragging image for the items being dragged. 'indexPaths' contains the (section,item) identification of the items being dragged. 'event' is a reference to the  mouse down event that began the drag. 'dragImageOffset' is an in/out parameter. This method will be called with dragImageOffset set to NSZeroPoint, but it can be modified to re-position the returned image. A dragImageOffset of NSZeroPoint will cause the image to be centered under the mouse. You can safely call -[NSCollectionView draggingImageForItemsAtIndexPaths:withEvent:offset:] from within this method. You do not need to implement this method for your collection view to be a drag source.
*/
- (NSImage *)collectionView:(NSCollectionView *)collectionView draggingImageForItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths withEvent:(NSEvent *)event offset:(NSPointPointer)dragImageOffset API_AVAILABLE(macos(10.11));

/* Old Form (Single Section Only) */
- (NSImage *)collectionView:(NSCollectionView *)collectionView draggingImageForItemsAtIndexes:(NSIndexSet *)indexes withEvent:(NSEvent *)event offset:(NSPointPointer)dragImageOffset API_AVAILABLE(macos(10.6));

/* This method is used by the collection view to determine a valid drop target. Based on the mouse position, the collection view will suggest a proposed (section,item) index path and drop operation. These values are in/out parameters and can be changed by the delegate to retarget the drop operation. The collection view will propose NSCollectionViewDropOn when the dragging location is closer to the middle of the item than either of its edges. Otherwise, it will propose NSCollectionViewDropBefore. You may override this default behavior by changing proposedDropOperation or proposedDropIndexPath. This method must return a value that indicates which dragging operation the data source will perform. It must return something other than NSDragOperationNone to accept the drop.

Note: to receive drag messages, you must first send -registerForDraggedTypes: to the collection view with the drag types you want to support (typically this is done in -awakeFromNib). You must implement this method for your collection view to be a drag destination.

Multi-image drag and drop: You can set draggingFormation, animatesToDestination, numberOfValidItemsForDrop within this method.
*/
- (NSDragOperation)collectionView:(NSCollectionView *)collectionView validateDrop:(id <NSDraggingInfo>)draggingInfo proposedIndexPath:(NSIndexPath * _Nonnull * _Nonnull)proposedDropIndexPath dropOperation:(NSCollectionViewDropOperation *)proposedDropOperation API_AVAILABLE(macos(10.11));

/* Old Form (Single Section Only) */
- (NSDragOperation)collectionView:(NSCollectionView *)collectionView validateDrop:(id <NSDraggingInfo>)draggingInfo proposedIndex:(NSInteger *)proposedDropIndex dropOperation:(NSCollectionViewDropOperation *)proposedDropOperation API_AVAILABLE(macos(10.6));

/* This method is called when the mouse is released over a collection view that previously decided to allow a drop via the above validateDrop method. At this time, the delegate should incorporate the data from the dragging pasteboard and update the collection view's contents. You must implement this method for your collection view to be a drag destination.

Multi-image drag and drop: If draggingInfo.animatesToDestination is set to YES, you should enumerate and update the dragging items with the proper image components and frames so that they dragged images animate to the proper locations.
*/
- (BOOL)collectionView:(NSCollectionView *)collectionView acceptDrop:(id <NSDraggingInfo>)draggingInfo indexPath:(NSIndexPath *)indexPath dropOperation:(NSCollectionViewDropOperation)dropOperation API_AVAILABLE(macos(10.11));

/* Old Form (Single Section Only) */
- (BOOL)collectionView:(NSCollectionView *)collectionView acceptDrop:(id <NSDraggingInfo>)draggingInfo index:(NSInteger)index dropOperation:(NSCollectionViewDropOperation)dropOperation API_AVAILABLE(macos(10.6));


/* Multi-image drag and drop */

/* Dragging Source Support - Required for multi-image drag and drop. Return a custom object that implements NSPasteboardWriting (or simply use NSPasteboardItem), or nil to prevent dragging for the item. For each valid item returned, NSCollectionView will create an NSDraggingItem with the draggingFrame equal to the frame of the item view at the given index path and components from -[NSCollectionViewItem draggingItem]. If this method is implemented, then -collectionView:writeItemsAtIndexPaths:toPasteboard: and -collectionView:draggingImageForItemsAtIndexPaths:withEvent:offset: will not be called.
*/
- (nullable id <NSPasteboardWriting>)collectionView:(NSCollectionView *)collectionView pasteboardWriterForItemAtIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Old Form (Single Section Only) */
- (nullable id <NSPasteboardWriting>)collectionView:(NSCollectionView *)collectionView pasteboardWriterForItemAtIndex:(NSUInteger)index;

/* Dragging Source Support - Optional. Implement this method to know when the dragging session is about to begin and to potentially modify the dragging session.
*/
- (void)collectionView:(NSCollectionView *)collectionView draggingSession:(NSDraggingSession *)session willBeginAtPoint:(NSPoint)screenPoint forItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Old Form (Single Section Only) */
- (void)collectionView:(NSCollectionView *)collectionView draggingSession:(NSDraggingSession *)session willBeginAtPoint:(NSPoint)screenPoint forItemsAtIndexes:(NSIndexSet *)indexes;

/* Dragging Source Support - Optional. Implement this method to know when the dragging session has ended. This delegate method can be used to know when the dragging source operation ended at a specific location, such as the trash (by checking for an operation of NSDragOperationDelete).
*/
- (void)collectionView:(NSCollectionView *)collectionView draggingSession:(NSDraggingSession *)session endedAtPoint:(NSPoint)screenPoint dragOperation:(NSDragOperation)operation;

/* Dragging Destination Support - Required for multi-image drag and drop. Implement this method to update dragging items as they are dragged over the view. Typically this will involve calling [draggingInfo enumerateDraggingItemsWithOptions:forView:classes:searchOptions:usingBlock:] and setting the draggingItem's imageComponentsProvider to a proper image based on the NSDraggingItem's -item value.
*/
- (void)collectionView:(NSCollectionView *)collectionView updateDraggingItemsForDrag:(id <NSDraggingInfo>)draggingInfo;


#pragma mark *** Selection and Highlighting ***

/* Sent during interactive selection or dragging, to inform the delegate that the CollectionView would like to change the "highlightState" property of the items at the specified "indexPaths" to the specified "highlightState".  In addition to optionally reacting to the proposed change, you can approve it (by returning "indexPaths" as-is), or selectively refuse some or all of the proposed highlightState changes (by returning a modified autoreleased mutableCopy of indexPaths, or an empty indexPaths instance).  Refusing a proposed highlightState change for an item will suppress the associated action for that item (selection change or eligibility to be a drop target).
*/
- (NSSet<NSIndexPath *> *)collectionView:(NSCollectionView *)collectionView shouldChangeItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths toHighlightState:(NSCollectionViewItemHighlightState)highlightState API_AVAILABLE(macos(10.11));

/* Sent during interactive selection or dragging, to inform the delegate that the CollectionView has changed the "highlightState" property of the items at the specified "indexPaths" to the specified "highlightState". */
- (void)collectionView:(NSCollectionView *)collectionView didChangeItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths toHighlightState:(NSCollectionViewItemHighlightState)highlightState API_AVAILABLE(macos(10.11));

/* Sent during interactive selection, to inform the delegate that the CollectionView would like to select the items at the specified "indexPaths".  In addition to optionally reacting to the proposed change, you can approve it (by returning "indexPaths" as-is), or selectively refuse some or all of the proposed selection changes (by returning a modified autoreleased mutableCopy of indexPaths, or an empty indexPaths instance).
*/
- (NSSet<NSIndexPath *> *)collectionView:(NSCollectionView *)collectionView shouldSelectItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Sent during interactive selection, to inform the delegate that the CollectionView would like to de-select the items at the specified "indexPaths".  In addition to optionally reacting to the proposed change, you can approve it (by returning "indexPaths" as-is), or selectively refuse some or all of the proposed selection changes (by returning a modified autoreleased mutableCopy of indexPaths, or an empty indexPaths instance). */
- (NSSet<NSIndexPath *> *)collectionView:(NSCollectionView *)collectionView shouldDeselectItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Sent at the end of interactive selection, to inform the delegate that the CollectionView has selected the items at the specified "indexPaths".
*/
- (void)collectionView:(NSCollectionView *)collectionView didSelectItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Sent at the end of interactive selection, to inform the delegate that the CollectionView has de-selected the items at the specified "indexPaths".
*/
- (void)collectionView:(NSCollectionView *)collectionView didDeselectItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));


#pragma mark *** Display Notification ***

/* Sent to notify the delegate that the CollectionView is about to add an NSCollectionViewItem.  The indexPath identifies the object that the item represents.
*/
- (void)collectionView:(NSCollectionView *)collectionView willDisplayItem:(NSCollectionViewItem *)item forRepresentedObjectAtIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Sent to notify the delegate that the CollectionView is about to add a supplementary view (e.g. a section header or footer view).  Each NSCollectionViewLayout class defines its own possible values and associated meanings for "elementKind".  (For example, NSCollectionViewFlowLayout declares NSCollectionElementKindSectionHeader and NSCollectionElementKindSectionFooter.)
*/
- (void)collectionView:(NSCollectionView *)collectionView willDisplaySupplementaryView:(NSView *)view forElementKind:(NSCollectionViewSupplementaryElementKind)elementKind atIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Sent to notify the delegate that the CollectionView is no longer displaying the given NSCollectionViewItem instance.  This happens when the model changes, or when an item is scrolled out of view.  You should perform any actions necessary to help decommission the item (such as releasing expensive resources).  The CollectionView may retain the item instance and later reuse it to represent the same or a different model object.
*/
- (void)collectionView:(NSCollectionView *)collectionView didEndDisplayingItem:(NSCollectionViewItem *)item forRepresentedObjectAtIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Sent to notify the delegate that the CollectionView is no longer displaying the given supplementary view. This happens when the model changes, or when a supplementary view is scrolled out of view. You should perform any actions necessary to help decommission the view (such as releasing expensive resources). The CollectionView may retain the view and later reuse it. */
- (void)collectionView:(NSCollectionView *)collectionView didEndDisplayingSupplementaryView:(NSView *)view forElementOfKind:(NSCollectionViewSupplementaryElementKind)elementKind atIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));


#pragma mark *** Layout Transition Support ***

/* Sent when the CollectionView switches to a different layout, to allow the delegate to provide a custom transition if desired.
*/
- (NSCollectionViewTransitionLayout *)collectionView:(NSCollectionView *)collectionView transitionLayoutForOldLayout:(NSCollectionViewLayout *)fromLayout newLayout:(NSCollectionViewLayout *)toLayout API_AVAILABLE(macos(10.11));

@end

#if !TARGET_OS_IPHONE
@interface NSIndexPath (NSCollectionViewAdditions)

+ (NSIndexPath *)indexPathForItem:(NSInteger)item inSection:(NSInteger)section API_AVAILABLE(macos(10.11));

@property (readonly) NSInteger item API_AVAILABLE(macos(10.11));
@property (readonly) NSInteger section API_AVAILABLE(macos(10.11));

@end
#endif // !TARGET_OS_IPHONE

@interface NSSet (NSCollectionViewAdditions)

/* Creates and returns a set that contains a single given NSIndexPath.
 */
+ (instancetype)setWithCollectionViewIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.11));

/* Creates and returns a set containing a uniqued collection of the NSIndexPaths contained in a given array.
 */
+ (instancetype)setWithCollectionViewIndexPaths:(NSArray<NSIndexPath *> *)indexPaths API_AVAILABLE(macos(10.11));

/* Executes the given block for each NSIndexPath in the set.  The index paths are enumerated in the order defined by NSIndexPath's -compare: method.  For CollectionView item index paths, this means all index paths in section 0, in ascending order, followed by all index paths in section 1, and so on.  You may pass the NSEnumerationReverse option to enumerate in the reverse order.  Set *stop = YES if desired, to halt the enumeration early.  Note that the given indexPath is not guaranteed to survive return from the block invocation, so if you wish to pass it outside the block, you must make a -copy (and later -release or -autorelease, if not using ARC).
 */
- (void)enumerateIndexPathsWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(NSIndexPath *indexPath, BOOL *stop))block API_AVAILABLE(macos(10.11));

@end

@interface NSCollectionView (NSDeprecated)

/* Invoked by a dataSource-less NSCollectionView when it needs a new item to represent the given model "object" (which is presumed to be a member of the CollectionView's "content" array).  Instantiates and returns a non-autoreleased item whose "representedObject" has been set to point to the given "object".  NSCollectionView's implementation of this method makes a copy of the CollectionView's itemPrototype (raising an exception if itemPrototype is nil).  You can override this method to customize the returned item or its view subtree however you wish, calling up to super first to obtain it.  Or, you can replace super's implementation entirely, to instantiate whatever kind of item you wish, potentially based on the type or properties of the given model "object".  The item returned from this factory method should NOT be autoreleased.

   It is considered a programming error to send this message to an NSCollectionView that has a dataSource; an exception will be raised.  Use -makeItemWithIdentifier:forIndexPath: instead, from the dataSource's -collectionView:itemForRepresentedObjectAtIndexPath: method.
 */
- (NSCollectionViewItem *)newItemForRepresentedObject:(id)object API_DEPRECATED("Use -[NSCollectionViewDataSource collectionView:itemForRepresentedObjectAtIndexPath:] instead", macos(10.5,10.14));

/* A prototype NSCollectionViewItem that a dataSource-less NSCollectionView should clone to produce new items.  When not using a dataSource, you must either specify an itemPrototype, or override -newItemForRepresentedObject:, for the CollectionView to be able to create and display items.

   It is considered a programming error to send this message to an NSCollectionView that has a dataSource; an exception will be raised.
 */
@property (nullable, strong) NSCollectionViewItem *itemPrototype API_DEPRECATED("Use -registerNib:forItemWithIdentifier: or -registerClass:forItemWithIdentifier: instead.", macos(10.5,10.14));

/* The following properties pertain only to NSCollectionViewGridLayout.  When you set collectionViewLayout to point to an instance of this class (or a subclass), these NSCollectionView properties take on the corresponding property values of the NSCollectionViewGridLayout.  When collectionViewLayout points to any other kind of layout object, these properties remember the values they are given, but they don't affect item layout or display.  When targeting OS X 10.11 and later, it's recommended that you reference the corresponding NSCollectionViewGridLayout properties instead.
 */
@property NSUInteger maxNumberOfRows API_DEPRECATED("Use NSCollectionViewGridLayout as the receiver's collectionViewLayout, setting its maximumNumberOfRows instead", macos(10.5,10.14)); // default: 0, which means no limit
@property NSUInteger maxNumberOfColumns API_DEPRECATED("Use NSCollectionViewGridLayout as the receiver's collectionViewLayout, setting its maximumNumberOfColumns instead", macos(10.5,10.14)); // default: 0, which means no limit
@property NSSize minItemSize API_DEPRECATED("Use NSCollectionViewGridLayout as the receiver's collectionViewLayout, setting its minimumItemSize instead", macos(10.5,10.14)); // default: (0; 0)
@property NSSize maxItemSize API_DEPRECATED("Use NSCollectionViewGridLayout as the receiver's collectionViewLayout, setting its maximumItemSize instead", macos(10.5,10.14)); // default: (0; 0), which means no limit

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
