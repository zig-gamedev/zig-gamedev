/*
    NSCollectionViewLayout.h
    Application Kit
    Copyright (c) 2015-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSCollectionView.h>

/* The NSCollectionViewLayout class is provided as an abstract class for subclassing to define custom collection layouts.  Defining a custom layout is an advanced operation intended for applications with complex needs.
*/

typedef NS_ENUM(NSInteger, NSCollectionElementCategory) {
    NSCollectionElementCategoryItem,
    NSCollectionElementCategorySupplementaryView,
    NSCollectionElementCategoryDecorationView,
    NSCollectionElementCategoryInterItemGap
} API_AVAILABLE(macos(10.11));

@class NSIndexPath;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NSString * NSCollectionViewDecorationElementKind NS_SWIFT_BRIDGED_TYPEDEF;

/* The elementKind that NSCollectionView uses to identify an inter-item gap, when the proposedDropOperation is NSCollectionViewDropBefore.  A client can customize the default look of the inter-item gap drop target indicator by registering a supplementary view nib or class for this elementKind.  If your -collectionView:validateDrop:proposedIndexPath:dropOperation: method disallows NSCollectionViewDropBefore operations, the CollectionView won't show this indicator.
*/
APPKIT_EXTERN NSCollectionViewSupplementaryElementKind const NSCollectionElementKindInterItemGapIndicator API_AVAILABLE(macos(10.11));

@class NSCollectionViewLayoutAttributes;
@class NSCollectionView;
@class NSNib;

API_AVAILABLE(macos(10.11))
@interface NSCollectionViewLayoutAttributes : NSObject <NSCopying>

@property NSRect frame;
@property NSSize size;
@property CGFloat alpha;
@property NSInteger zIndex; // default is 0
@property (getter=isHidden) BOOL hidden; // As an optimization, NSCollectionView might not create a view for items whose hidden attribute is YES
@property (nullable, strong) NSIndexPath *indexPath;

@property (readonly) NSCollectionElementCategory representedElementCategory;
@property (nullable, readonly) NSString *representedElementKind; // nil when representedElementCategory is NSCollectionElementCategoryItem. NSCollectionViewSupplementaryElementKind when representedElementCategory is NSCollectionElementCategorySupplementaryView or NSCollectionElementCategoryInterItemGap. NSCollectionViewDecorationElementKind when representedElementCategory is NSCollectionElementCategoryDecorationView.

+ (instancetype)layoutAttributesForItemWithIndexPath:(NSIndexPath *)indexPath;
+ (instancetype)layoutAttributesForInterItemGapBeforeIndexPath:(NSIndexPath *)indexPath;
+ (instancetype)layoutAttributesForSupplementaryViewOfKind:(NSCollectionViewSupplementaryElementKind)elementKind withIndexPath:(NSIndexPath *)indexPath;
+ (instancetype)layoutAttributesForDecorationViewOfKind:(NSCollectionViewDecorationElementKind)decorationViewKind withIndexPath:(NSIndexPath*)indexPath;

@end

typedef NS_ENUM(NSInteger, NSCollectionUpdateAction) {
    NSCollectionUpdateActionInsert,
    NSCollectionUpdateActionDelete,
    NSCollectionUpdateActionReload,
    NSCollectionUpdateActionMove,
    NSCollectionUpdateActionNone
} API_AVAILABLE(macos(10.11));

API_AVAILABLE(macos(10.11))
@interface NSCollectionViewUpdateItem : NSObject

@property (nullable, readonly) NSIndexPath *indexPathBeforeUpdate; // nil for NSCollectionUpdateActionInsert
@property (nullable, readonly) NSIndexPath *indexPathAfterUpdate; // nil for NSCollectionUpdateActionDelete
@property (readonly) NSCollectionUpdateAction updateAction;

@end

API_AVAILABLE(macos(10.11))
@interface NSCollectionViewLayoutInvalidationContext : NSObject

@property (readonly) BOOL invalidateEverything; // set to YES when invalidation occurs because the collection view is sent -reloadData
@property (readonly) BOOL invalidateDataSourceCounts; // if YES, the layout should requery section and item counts from the collection view - set to YES when the collection view is sent -reloadData and when items are inserted or deleted

- (void)invalidateItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths;
- (void)invalidateSupplementaryElementsOfKind:(NSCollectionViewSupplementaryElementKind)elementKind atIndexPaths:(NSSet<NSIndexPath *> *)indexPaths;
- (void)invalidateDecorationElementsOfKind:(NSCollectionViewDecorationElementKind)elementKind atIndexPaths:(NSSet<NSIndexPath *> *)indexPaths;
@property (nullable, readonly) NSSet<NSIndexPath *> *invalidatedItemIndexPaths;
@property (nullable, readonly) NSDictionary<NSCollectionViewSupplementaryElementKind, NSSet<NSIndexPath *> *> *invalidatedSupplementaryIndexPaths; // keys are element kind strings - values are NSSet<NSIndexPath *>
@property (nullable, readonly) NSDictionary<NSCollectionViewDecorationElementKind, NSSet<NSIndexPath *> *> *invalidatedDecorationIndexPaths; // keys are element kind strings - values are NSSet<NSIndexPath *>

@property NSPoint contentOffsetAdjustment; // delta to be applied to the collection view's current contentOffset - default is NSZeroPoint
@property NSSize contentSizeAdjustment; // delta to be applied to the current content size - default is NSZeroSize

@end

/* The NSCollectionViewLayout class is provided as an abstract class for subclassing to define custom collection layouts.  Defining a custom layout is an advanced operation intended for applications with complex needs.
*/
API_AVAILABLE(macos(10.11))
@interface NSCollectionViewLayout : NSObject <NSCoding>

/* Methods in this class are meant to be overridden and will be called by its collection view to gather layout information.  To get the truth on the current state of the collection view, call methods on NSCollectionView rather than these.
*/

@property (nullable, readonly, weak) NSCollectionView *collectionView;

/* Call -invalidateLayout to indicate that the collection view needs to requery the layout information.  Subclasses must always call super if they override.
*/
- (void)invalidateLayout;
- (void)invalidateLayoutWithContext:(NSCollectionViewLayoutInvalidationContext *)context;

- (void)registerClass:(nullable Class)viewClass forDecorationViewOfKind:(NSCollectionViewDecorationElementKind)elementKind;
- (void)registerNib:(nullable NSNib *)nib forDecorationViewOfKind:(NSCollectionViewDecorationElementKind)elementKind;

@end

@interface NSCollectionViewLayout (NSSubclassingHooks)

@property (class, readonly) Class layoutAttributesClass; // override this method to provide a custom class to be used when instantiating instances of NSCollectionViewLayoutAttributes

@property (class, readonly) Class invalidationContextClass; // override this method to provide a custom class to be used for invalidation contexts

/* The collection view calls -prepareLayout once at its first layout as the first message to the layout instance.  The collection view calls -prepareLayout again after layout is invalidated and before requerying the layout information.  Subclasses should always call super if they override.
*/
- (void)prepareLayout;

/* NSCollectionView calls these four methods to determine the layout information.  Implement -layoutAttributesForElementsInRect: to return layout attributes for for supplementary or decoration views, or to perform layout in an as-needed-on-screen fashion. Additionally, all layout subclasses should implement -layoutAttributesForItemAtIndexPath: to return layout attributes instances on demand for specific index paths.  If the layout supports any supplementary or decoration view types, it should also implement the respective atIndexPath: methods for those types.
*/
- (NSArray<__kindof NSCollectionViewLayoutAttributes *> *)layoutAttributesForElementsInRect:(NSRect)rect; // return an array layout attributes instances for all the views in the given rect
- (nullable NSCollectionViewLayoutAttributes *)layoutAttributesForItemAtIndexPath:(NSIndexPath *)indexPath;
- (nullable NSCollectionViewLayoutAttributes *)layoutAttributesForSupplementaryViewOfKind:(NSCollectionViewSupplementaryElementKind)elementKind atIndexPath:(NSIndexPath *)indexPath;
- (nullable NSCollectionViewLayoutAttributes *)layoutAttributesForDecorationViewOfKind:(NSCollectionViewDecorationElementKind)elementKind atIndexPath:(NSIndexPath *)indexPath;

/* Returns an NSCollectionViewLayoutAttributes instance that identifies an appropriate proposed drop target from the given hover point.  If the point is between items, the returned layout attributes may describe an inter-item gap, which is distinguished by having a representedElementCategory of NSCollectionElementCategoryInterItemGap, and a representedElementKind of NSCollectionElementKindInterItemGapIndicator.  Its "frame" provides a bounding box for the gap, that will be used to position and size a drop target indicator, and its "indexPath" identifies the proposed "insert before" position.  The frame might not contain the given point, if the insertion indicator should lie elsewhere.  (For example, when a vertical Flow layout hit-tests a gap between rows, it may suggest an insertion gap to the left of the lower row's first item.)  Subclasses should override this method to return appropriate results based on their knowledge of the particular layout.  This method should return nil if no suitable drop target can be inferred from the given point.
*/
- (nullable NSCollectionViewLayoutAttributes *)layoutAttributesForDropTargetAtPoint:(NSPoint)pointInCollectionView;

/* Returns an NSCollectionViewLayoutAttributes instance that describes the inter-item gap preceding the given item indexPath.  You may pass an indexPath whose "item" index equals the number of items in the section, to request a description of the gap after the last item.  The result has a representedElementCategory of NSCollectionElementCategoryInterItemGap, and a representedElementKind of NSCollectionElementKindInterItemGapIndicator.  Subclasses should override this method to return appropriate results based on their knowledge of the particular layout.
*/
- (nullable NSCollectionViewLayoutAttributes *)layoutAttributesForInterItemGapBeforeIndexPath:(NSIndexPath *)indexPath;

- (BOOL)shouldInvalidateLayoutForBoundsChange:(NSRect)newBounds; // return YES to cause the collection view to requery the layout for geometry information
- (NSCollectionViewLayoutInvalidationContext *)invalidationContextForBoundsChange:(NSRect)newBounds;

- (BOOL)shouldInvalidateLayoutForPreferredLayoutAttributes:(NSCollectionViewLayoutAttributes *)preferredAttributes withOriginalAttributes:(NSCollectionViewLayoutAttributes *)originalAttributes;
- (NSCollectionViewLayoutInvalidationContext *)invalidationContextForPreferredLayoutAttributes:(NSCollectionViewLayoutAttributes *)preferredAttributes withOriginalAttributes:(NSCollectionViewLayoutAttributes *)originalAttributes;

- (NSPoint)targetContentOffsetForProposedContentOffset:(NSPoint)proposedContentOffset withScrollingVelocity:(NSPoint)velocity; // return a point at which to rest after scrolling - for layouts that want snap-to-point scrolling behavior
- (NSPoint)targetContentOffsetForProposedContentOffset:(NSPoint)proposedContentOffset; // a layout can return the content offset to be applied during transition or update animations

@property(readonly) NSSize collectionViewContentSize; // Subclasses must implement a collectionViewContentSize getter that returns the width and height of the collection view’s content. These values represent the width and height of all the content, not just the content that is currently visible. The collection view uses this information to configure its own content size to facilitate scrolling.

@end

@interface NSCollectionViewLayout (NSUpdateSupportHooks)

// This method is called when there is an update with deletes/inserts to the collection view.
// It will be called prior to calling the initial/final layout attribute methods below to give the layout an opportunity to do batch computations for the insertion and deletion layout attributes.
// The updateItems parameter is an array of NSCollectionViewUpdateItem instances for each element that is moving to a new index path.
- (void)prepareForCollectionViewUpdates:(NSArray<NSCollectionViewUpdateItem *> *)updateItems;
- (void)finalizeCollectionViewUpdates; // called inside an animation block after the update

- (void)prepareForAnimatedBoundsChange:(NSRect)oldBounds; // NSCollectionView calls this when its bounds have changed inside an animation block before displaying items in its new bounds
- (void)finalizeAnimatedBoundsChange; // also called inside the animation block

// NSCollectionView calls this when prior the layout transition animation on the incoming and outgoing layout
- (void)prepareForTransitionToLayout:(NSCollectionViewLayout *)newLayout;
- (void)prepareForTransitionFromLayout:(NSCollectionViewLayout *)oldLayout;
- (void)finalizeLayoutTransition;  // called inside an animation block after the transition

// This set of methods is called when the collection view undergoes an animated transition such as a batch update block or an animated bounds change.
// For each element on screen before the invalidation, finalLayoutAttributesForDisappearingXXX will be called and an animation setup from what is on screen to those final attributes.
// For each element on screen after the invalidation, initialLayoutAttributesForAppearingXXX will be called an an animation setup from those initial attributes to what ends up on screen.
- (nullable NSCollectionViewLayoutAttributes *)initialLayoutAttributesForAppearingItemAtIndexPath:(NSIndexPath *)itemIndexPath;
- (nullable NSCollectionViewLayoutAttributes *)finalLayoutAttributesForDisappearingItemAtIndexPath:(NSIndexPath *)itemIndexPath;
- (nullable NSCollectionViewLayoutAttributes *)initialLayoutAttributesForAppearingSupplementaryElementOfKind:(NSCollectionViewSupplementaryElementKind)elementKind atIndexPath:(NSIndexPath *)elementIndexPath;
- (nullable NSCollectionViewLayoutAttributes *)finalLayoutAttributesForDisappearingSupplementaryElementOfKind:(NSCollectionViewSupplementaryElementKind)elementKind atIndexPath:(NSIndexPath *)elementIndexPath;
- (nullable NSCollectionViewLayoutAttributes *)initialLayoutAttributesForAppearingDecorationElementOfKind:(NSCollectionViewDecorationElementKind)elementKind atIndexPath:(NSIndexPath *)decorationIndexPath;
- (nullable NSCollectionViewLayoutAttributes *)finalLayoutAttributesForDisappearingDecorationElementOfKind:(NSCollectionViewDecorationElementKind)elementKind atIndexPath:(NSIndexPath *)decorationIndexPath;

// These methods are called by collection view during an update block.
// Return an array of index paths to indicate views that the layout is deleting or inserting in response to the update.
- (NSSet<NSIndexPath *> *)indexPathsToDeleteForSupplementaryViewOfKind:(NSCollectionViewSupplementaryElementKind)elementKind;
- (NSSet<NSIndexPath *> *)indexPathsToDeleteForDecorationViewOfKind:(NSCollectionViewDecorationElementKind)elementKind;
- (NSSet<NSIndexPath *> *)indexPathsToInsertForSupplementaryViewOfKind:(NSCollectionViewSupplementaryElementKind)elementKind;
- (NSSet<NSIndexPath *> *)indexPathsToInsertForDecorationViewOfKind:(NSCollectionViewDecorationElementKind)elementKind;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
