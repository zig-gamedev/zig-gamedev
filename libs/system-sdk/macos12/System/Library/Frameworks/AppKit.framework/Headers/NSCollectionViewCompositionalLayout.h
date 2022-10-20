/*
    NSCollectionViewCompositionalLayout.h
    Application Kit
    Copyright (c) 2019-2021, Apple Inc.
    All rights reserved.
*/

#import <TargetConditionals.h>

#if !TARGET_OS_IPHONE

#import <AppKit/NSCollectionViewLayout.h>
#import <AppKit/NSCollectionViewFlowLayout.h> // for NSCollectionViewScrollDirection
#import <AppKit/AppKitDefines.h>

@protocol NSCollectionLayoutEnvironment;
@protocol NSCollectionLayoutContainer;
@protocol NSCollectionLayoutVisibleItem;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSCollectionLayoutSection;
@class NSCollectionLayoutGroup;
@class NSCollectionLayoutItem;
@class NSCollectionLayoutSupplementaryItem;
@class NSCollectionLayoutBoundarySupplementaryItem;
@class NSCollectionLayoutDecorationItem;

@class NSCollectionLayoutSize;
@class NSCollectionLayoutDimension;
@class NSCollectionLayoutSpacing;
@class NSCollectionLayoutEdgeSpacing;
@class NSCollectionLayoutAnchor;


/* These geometry type declarations are being relocated to Foundation/NSGeometry.h */

#ifndef FOUNDATION_HAS_DIRECTIONAL_GEOMETRY

/* Edge specification that should be interpreted according to the user interface layout direction.
 */
typedef NS_OPTIONS(NSUInteger, NSDirectionalRectEdge) {
    NSDirectionalRectEdgeNone       = 0,
    NSDirectionalRectEdgeTop        = 1 << 0,
    NSDirectionalRectEdgeLeading    = 1 << 1,
    NSDirectionalRectEdgeBottom     = 1 << 2,
    NSDirectionalRectEdgeTrailing   = 1 << 3,
    NSDirectionalRectEdgeAll        = NSDirectionalRectEdgeTop | NSDirectionalRectEdgeLeading | NSDirectionalRectEdgeBottom | NSDirectionalRectEdgeTrailing
} API_AVAILABLE(macos(10.15));

/* Specifies edge insets that should be interpreted according to the user interface layout direction.  Specify positive values to inset, negative values to outset.
 */
typedef struct __attribute__((objc_boxable)) NSDirectionalEdgeInsets {
    CGFloat top;
    CGFloat leading;
    CGFloat bottom;
    CGFloat trailing;
} NSDirectionalEdgeInsets API_AVAILABLE(macos(10.15));

APPKIT_EXTERN const NSDirectionalEdgeInsets NSDirectionalEdgeInsetsZero API_AVAILABLE(macos(10.15));

/* Alignment specification that should be interpreted according to the user interface layout direction.
 */
typedef NS_ENUM(NSInteger, NSRectAlignment) {
    NSRectAlignmentNone = 0,
    NSRectAlignmentTop,
    NSRectAlignmentTopLeading,
    NSRectAlignmentLeading,
    NSRectAlignmentBottomLeading,
    NSRectAlignmentBottom,
    NSRectAlignmentBottomTrailing,
    NSRectAlignmentTrailing,
    NSRectAlignmentTopTrailing,
} API_AVAILABLE(macos(10.15));

NS_INLINE NSDirectionalEdgeInsets NSDirectionalEdgeInsetsMake(CGFloat top, CGFloat leading, CGFloat bottom, CGFloat trailing) API_AVAILABLE(macos(10.15)) NS_SWIFT_UNAVAILABLE("Use 'NSDirectionalEdgeInsets()' instead") {
    NSDirectionalEdgeInsets insets = {top, leading, bottom, trailing};
    return insets;
}

#endif

/* End temporary declarations */


API_AVAILABLE(macos(10.15))
@interface NSCollectionViewCompositionalLayoutConfiguration : NSObject<NSCopying>
@property NSCollectionViewScrollDirection scrollDirection;
@property CGFloat interSectionSpacing;
@property(copy) NSArray<NSCollectionLayoutBoundarySupplementaryItem*> *boundarySupplementaryItems;
@end

typedef NSCollectionLayoutSection * _Nullable (^NSCollectionViewCompositionalLayoutSectionProvider)(NSInteger section, id<NSCollectionLayoutEnvironment>);

API_AVAILABLE(macos(10.15))
@interface NSCollectionViewCompositionalLayout : NSCollectionViewLayout
- (instancetype)initWithSection:(NSCollectionLayoutSection*)section;
- (instancetype)initWithSection:(NSCollectionLayoutSection*)section configuration:(NSCollectionViewCompositionalLayoutConfiguration*)configuration;

- (instancetype)initWithSectionProvider:(NSCollectionViewCompositionalLayoutSectionProvider)sectionProvider;
- (instancetype)initWithSectionProvider:(NSCollectionViewCompositionalLayoutSectionProvider)sectionProvider configuration:(NSCollectionViewCompositionalLayoutConfiguration*)configuration;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

// Setting this property will invalidate the layout immediately to affect any changes
//    Note: any changes made to properites directly will have no effect.
@property(copy) NSCollectionViewCompositionalLayoutConfiguration *configuration;
@end

typedef NS_ENUM(NSInteger,NSCollectionLayoutSectionOrthogonalScrollingBehavior) {
    // default behavior. Section will layout along main layout axis (i.e. configuration.scrollDirection)
    NSCollectionLayoutSectionOrthogonalScrollingBehaviorNone,
    
    // NOTE: For each of the remaining cases, the section content will layout orthogonal to the main layout axis (e.g. main layout axis == .vertical, section will scroll in .horizontal axis)
    
    // Standard scroll view behavior
    NSCollectionLayoutSectionOrthogonalScrollingBehaviorContinuous,
    
    // Scrolling will come to rest on the leading edge of a group boundary
    NSCollectionLayoutSectionOrthogonalScrollingBehaviorContinuousGroupLeadingBoundary,
    
    // Standard scroll view paging behavior with page size == extent of the collection view's bounds
    NSCollectionLayoutSectionOrthogonalScrollingBehaviorPaging,
    
    // Fractional size paging behavior determined by the sections layout group's dimension
    NSCollectionLayoutSectionOrthogonalScrollingBehaviorGroupPaging,
    
    // Same of group paging with additional leading and trailing content insets to center each group's contents along the orthogonal axis
    NSCollectionLayoutSectionOrthogonalScrollingBehaviorGroupPagingCentered,
} API_AVAILABLE(macos(10.15));

typedef void (^NSCollectionLayoutSectionVisibleItemsInvalidationHandler)(NSArray<id<NSCollectionLayoutVisibleItem>> *visibleItems, NSPoint contentOffset, id<NSCollectionLayoutEnvironment> layoutEnvironment);

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutSection : NSObject<NSCopying>
+ (instancetype)sectionWithGroup:(NSCollectionLayoutGroup*)group;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property NSDirectionalEdgeInsets contentInsets;
@property CGFloat interGroupSpacing;

// default is .none
@property NSCollectionLayoutSectionOrthogonalScrollingBehavior orthogonalScrollingBehavior;

// Supplementaries associated with the boundary edges of the section
@property(copy) NSArray<NSCollectionLayoutBoundarySupplementaryItem*> *boundarySupplementaryItems;

// by default, section supplementaries will follow any section-specific contentInsets
@property BOOL supplementariesFollowContentInsets;

// Called for each layout pass to allow modification of item properties right before they are displayed.
@property(copy,nullable) NSCollectionLayoutSectionVisibleItemsInvalidationHandler visibleItemsInvalidationHandler;

// decoration views anchored to the section's geometry (e.g. background decoration view)
@property(copy) NSArray<NSCollectionLayoutDecorationItem*> *decorationItems;
@end


API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutItem : NSObject<NSCopying>
+ (instancetype)itemWithLayoutSize:(NSCollectionLayoutSize*)layoutSize;
+ (instancetype)itemWithLayoutSize:(NSCollectionLayoutSize*)layoutSize supplementaryItems:(NSArray<NSCollectionLayoutSupplementaryItem*>*)supplementaryItems;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

//                       +---------------------+
//   +-------------+<----|Specified layout size|
//   |             |     +---------------------+
//   |  +-------+  |     +--------------------------+
//   |  |~~~~~~~|  |     |Final size (after         |
//   |  |~~~~~~~|<-+-----|contentInsets are applied)|
//   |  +-------+  |     +--------------------------+
//   |             |
//   +-------------+
//
//  Use contentInsets on an item to adjust the final size of the item after layout is computed.
//    useful for grid style layouts to apply even spacing around each the edges of each item.
//
//  Note: contentInsets are ignored for items with .estimated dimension(s)

@property NSDirectionalEdgeInsets contentInsets;

//                     +--------+
//                     |  Top   |
//                     +--------+
//                          |
//                          |
//                  +-------+--------------------------+
//                  |       v                          |
//                  |    +------+                      |
//   +--------+     |    |~~~~~~|        +--------+    |
//   |Leading |-----+->  |~~~~~~| <------|Trailing|    |
//   +--------+     |    |~~~~~~|        +--------+    |
//                  |    +------+                      |
//                  |        ^                         |
//                  +--------+-------------------------+
//                           |
//                           |
//                      +--------+
//                      | Bottom |
//                      +--------+
//
//  Specifies additional space required surrounding and item when laying out.
//  Flexible spacing can be used to apportion remaining space after items are layed out to
//    evenly align items among available layout space.

@property(copy,nullable) NSCollectionLayoutEdgeSpacing *edgeSpacing;

@property(readonly) NSCollectionLayoutSize *layoutSize;
@property(readonly) NSArray<NSCollectionLayoutSupplementaryItem*> *supplementaryItems;
@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutGroupCustomItem : NSObject<NSCopying>
+ (instancetype)customItemWithFrame:(NSRect)frame;
+ (instancetype)customItemWithFrame:(NSRect)frame zIndex:(NSInteger)zIndex;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property(readonly) NSRect frame;
@property(readonly) NSInteger zIndex;
@end

typedef NSArray<NSCollectionLayoutGroupCustomItem*> * _Nonnull(^NSCollectionLayoutGroupCustomItemProvider)(id<NSCollectionLayoutEnvironment> layoutEnvironment);

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutGroup : NSCollectionLayoutItem<NSCopying>

// Specifies a group that will have N items equally sized along the horizontal axis. use interItemSpacing to insert space between items
//
//   +------+--+------+--+------+
//   |~~~~~~|  |~~~~~~|  |~~~~~~|
//   |~~~~~~|  |~~~~~~|  |~~~~~~|
//   |~~~~~~|  |~~~~~~|  |~~~~~~|
//   +------+--+------+--+------+
//            ^        ^
//            |        |
//    +-----------------------+
//    |  Inter Item Spacing   |
//    +-----------------------+
//
+ (instancetype)horizontalGroupWithLayoutSize:(NSCollectionLayoutSize*)layoutSize subitem:(NSCollectionLayoutItem*)subitem count:(NSInteger)count;

// Specifies a group that will repeat items until available horizontal space is exhausted.
//   note: any remaining space after laying out items can be apportioned among flexible interItemSpacing defintions
+ (instancetype)horizontalGroupWithLayoutSize:(NSCollectionLayoutSize*)layoutSize subitems:(NSArray<NSCollectionLayoutItem*>*)subitems;

// Specifies a group that will have N items equally sized along the vertical axis. use interItemSpacing to insert space between items
//   +------+
//   |~~~~~~|
//   |~~~~~~|
//   |~~~~~~|
//   +------+
//   |      |<--+
//   +------+   |
//   |~~~~~~|   |    +-----------------------+
//   |~~~~~~|   +----|  Inter Item Spacing   |
//   |~~~~~~|   |    +-----------------------+
//   +------+   |
//   |      |<--+
//   +------+
//   |~~~~~~|
//   |~~~~~~|
//   |~~~~~~|
//   +------+
//
+ (instancetype)verticalGroupWithLayoutSize:(NSCollectionLayoutSize*)layoutSize subitem:(NSCollectionLayoutItem*)subitem count:(NSInteger)count;

// Specifies a group that will repeat items until available vertical space is exhausted.
//   note: any remaining space after laying out items can be apportioned among flexible interItemSpacing defintions
+ (instancetype)verticalGroupWithLayoutSize:(NSCollectionLayoutSize*)layoutSize subitems:(NSArray<NSCollectionLayoutItem*>*)subitems;

// Specifies a custom group with client-specified frames.
//   During layout, the itemProvider will be called with the group's current geometry provided via the NSCollectionLayoutEnvironment supplied.
//   The coordinate space for returned frames should be {0,0} relative to this group's geometry.
//   Custom groups can be nested arbitrarily inside other groups.
+ (instancetype)customGroupWithLayoutSize:(NSCollectionLayoutSize*)layoutSize itemProvider:(NSCollectionLayoutGroupCustomItemProvider)itemProvider;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

// Supplementary items are "anchored" to the group's geometry.
@property(copy) NSArray<NSCollectionLayoutSupplementaryItem*> *supplementaryItems;

// Supplies additional spacing between items along the layout axis of the group
@property(copy,nullable) NSCollectionLayoutSpacing *interItemSpacing;

@property(readonly) NSArray<NSCollectionLayoutItem*> *subitems;

// for visual debugging; will print an ASCII art rendering to console
- (NSString*)visualDescription;

@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutDimension : NSObject<NSCopying>
// dimension is computed as a fraction of the width of the containing group
+ (instancetype)fractionalWidthDimension:(CGFloat)fractionalWidth;

// dimension is computed as a fraction of the height of the containing group
+ (instancetype)fractionalHeightDimension:(CGFloat)fractionalHeight;

// dimension with an absolute point value
+ (instancetype)absoluteDimension:(CGFloat)absoluteDimension;

// dimension is estimated with a point value. Actual size will be determined when the content is rendered.
+ (instancetype)estimatedDimension:(CGFloat)estimatedDimension;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property(readonly) BOOL isFractionalWidth;
@property(readonly) BOOL isFractionalHeight;
@property(readonly) BOOL isAbsolute;
@property(readonly) BOOL isEstimated;
@property(readonly) CGFloat dimension;
@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutSize : NSObject<NSCopying>
+ (instancetype)sizeWithWidthDimension:(NSCollectionLayoutDimension*)width heightDimension:(NSCollectionLayoutDimension*)height;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property(readonly) NSCollectionLayoutDimension *widthDimension;
@property(readonly) NSCollectionLayoutDimension *heightDimension;
@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutSpacing : NSObject<NSCopying>
+ (instancetype)flexibleSpacing:(CGFloat)flexibleSpacing;  // i.e. >=
+ (instancetype)fixedSpacing:(CGFloat)fixedSpacing;        // i.e. ==

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property(readonly) CGFloat spacing;
@property(readonly) BOOL isFlexibleSpacing;
@property(readonly) BOOL isFixedSpacing;
@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutEdgeSpacing : NSObject<NSCopying>

// Edge spacing specifies additional outsets around items required when performing layout.
//   Edges may be omitted as-needed
//
//    +-----------------+--------+-----------------+------+-----------------+
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    |~~~~~~~~~~~~~~~~~|        |~~~~~~~~~~~~~~~~~|      |~~~~~~~~~~~~~~~~~|
//    +-----------------+        +-----------------+      +-----------------+
//    |                                                                     |
//    |        ^                         ^                          ^       |
//    +--------+-------------------------+--------------------------+-------+
//             |                         |                          |
//             |                         |                          |
//   +---------+-------------------------+--------------------------+--------------------------+
//   |NSCollectionLayoutEdgeSpacing(leading:nil, top: nil, bottom:.flexible(0.0), trailing:nil)|
//   |                                                                                         |
//   |*forces items to align to the top of their group's geometry                              |
//   +-----------------------------------------------------------------------------------------+

+ (instancetype)spacingForLeading:(nullable NSCollectionLayoutSpacing *)leading
                              top:(nullable NSCollectionLayoutSpacing *)top
                         trailing:(nullable NSCollectionLayoutSpacing *)trailing
                           bottom:(nullable NSCollectionLayoutSpacing *)bottom;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property(readonly,nullable) NSCollectionLayoutSpacing *leading;
@property(readonly,nullable) NSCollectionLayoutSpacing *top;
@property(readonly,nullable) NSCollectionLayoutSpacing *trailing;
@property(readonly,nullable) NSCollectionLayoutSpacing *bottom;
@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutSupplementaryItem : NSCollectionLayoutItem<NSCopying>

//  Supplementary items are positioned (i.e. anchored) to coordinate spaces throughout the layout
//    In this example, a supplementary is anchored to the top+trailing edge of a cell.
//    Supplementary items can be anchored to items (and groups, since a group is-a item)
//    Boundary supplementary items can be anchored to sections and the global layout
//
//                                +-----+       +------------------------------------------+
//                                |~~~~~|       | edges: [.top,.trailing]                  |
//   +----------------------------+~~~~~|<------+ fractionalOffset: NSPoint(x:0.5,y:-0.5)  |
//   |                            |~~~~~|       +------------------------------------------+
//   |                            +--+--+
//   |                               |
//   |                               |
//   |                               |
//   |                               |
//   +-------------------------------+
//
//  Container anchors are used to specify positioning of an item within the host geometry (e.g. item, group, section)

+ (instancetype)supplementaryItemWithLayoutSize:(NSCollectionLayoutSize*)layoutSize elementKind:(NSString*)elementKind containerAnchor:(NSCollectionLayoutAnchor*)containerAnchor;

//                                                     +----------------------------------------------------+
//                                     +-----+         |* Container Anchor                                  |
//                                     |~~~~~|         |edges: [.top,.trailing] offset: NSPoint(x:10,y:-10) |
//                                     |~~~~~|<--------|                                                    |
//                                     |~~~~~|         |* Item Anchor:                                      |
//                                     +-----+         |edges: [.bottom, .leading]                          |
//   +-------------------------------+                 +----------------------------------------------------+
//   |                               |
//   |                               |
//   |                               |
//   |                               |
//   |                               |
//   |                               |
//   +-------------------------------+
//
//   Combine a container anchor with an item anchor for fine-grained positioning.
//     Optionally add an offset for additional refinement.

+ (instancetype)supplementaryItemWithLayoutSize:(NSCollectionLayoutSize*)layoutSize elementKind:(NSString*)elementKind containerAnchor:(NSCollectionLayoutAnchor*)containerAnchor itemAnchor:(NSCollectionLayoutAnchor*)itemAnchor;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property NSInteger zIndex;

@property(readonly) NSString *elementKind;
@property(readonly) NSCollectionLayoutAnchor *containerAnchor;
@property(readonly,nullable) NSCollectionLayoutAnchor *itemAnchor;
@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutBoundarySupplementaryItem : NSCollectionLayoutSupplementaryItem<NSCopying>

// Specify the alignment of the supplementary relative the containing geometry's coordinate space to
// position the boundary supplementary item.
//
//                                            +------------------------------------------+
//   +----------------------+                 |Boundary Supplementary Item               |
//   |                      |<----------------|* alignment: [.top, .leading]             |
//   +----------------------+                 |* absoluteOffset: NSPoint(x:0.0, y:-10.0) |
//                                            +------------------------------------------+
//   +----------------------------------+
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |      +--------------------------------+
//   |                                  |<-----|        Section Geometry        |
//   |                                  |      +--------------------------------+
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   |                                  |
//   +----------------------------------+

+ (instancetype)boundarySupplementaryItemWithLayoutSize:(NSCollectionLayoutSize*)layoutSize elementKind:(NSString*)elementKind alignment:(NSRectAlignment)alignment;
+ (instancetype)boundarySupplementaryItemWithLayoutSize:(NSCollectionLayoutSize*)layoutSize elementKind:(NSString*)elementKind alignment:(NSRectAlignment)alignment absoluteOffset:(NSPoint)absoluteOffset;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

// Default is YES. This will automatically extend the content area of the host geometry (e.g. section)
//   For .estimated sized supplementary items, this allows automatic adjustment of the layout. (e.g. dynamic text)
@property BOOL extendsBoundary;

// Default is NO. Specify YES to keep the supplementary visible while any portion of the host geometry (e.g. section) is visible.
//   Occlusion disambiguation between other supplementaries will be managed automatically (e.g. section header + footer both pinned)
@property BOOL pinToVisibleBounds;

@property(readonly) NSRectAlignment alignment;
@property(readonly) NSPoint offset;
@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutDecorationItem : NSCollectionLayoutItem<NSCopying>

// Useful for setting a background decoration view behind a section's content area.
//
// contentInset can also be applied as-needed.
// Register the elementKind with the layout instance to associate with your custom view class/nib
//
//   +----------------------------------+
//   |                                  |
//   |  +----------------------------+  |      +--------------------------------+
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |      |Background Decoration Item      |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~<--+------|* contentInsets applied         |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |      +--------------------------------+
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |      +--------------------------------+
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |<-----|        Section Geometry        |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |      +--------------------------------+
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |
//   |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  |
//   |  +----------------------------+  |
//   |                                  |
//   +----------------------------------+

+ (instancetype)backgroundDecorationItemWithElementKind:(NSString*)elementKind;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property NSInteger zIndex;      // default is 0; all other section items will be automatically be promoted to zIndex=1

@property(readonly) NSString *elementKind;
@end

API_AVAILABLE(macos(10.15))
@interface NSCollectionLayoutAnchor : NSObject<NSCopying>

//                       +------------------+  +------+   +------------------+
//                       | [.top, .leading] |  |[.top]|   | [.top,.trailing] |
//                       +--+---------------+  +---+--+   +---------------+--+
//                          |                      |                      |
//                          v                      v                      v
//                       +-----+----------------+-----+----------------+-----+
//                       |~~~~~|                |~~~~~|                |~~~~~|
//                       |~~~~~|                |~~~~~|                |~~~~~|
//                       +-----+                +-----+                +-----+
//                       |                                                   |
//                       +-----+                                       +-----+
//   +--------------+    |~~~~~|                                       |~~~~~|    +-------------+
//   |  [.leading]  |--->|~~~~~|                                       |~~~~~|<---| [.trailing] |
//   +--------------+    +-----+                                       +-----+    +-------------+
//                       |                                                   |
//                       +-----+                +-----+                +-----+
//                       |~~~~~|                |~~~~~|                |~~~~~|
//                       |~~~~~|                |~~~~~|                |~~~~~|
//                       +-----+----------------+-----+----------------+-----+
//                          ^                      ^                      ^
//                          |                      |                      |
//                      +---+---------------+ +----+----+  +--------------+----+
//                      |[.bottom, .leading]| |[.bottom]|  |[.bottom,.trailing]|
//                      +-------------------+ +---------+  +-------------------+
//
// Edges are specified as shown above.

+ (instancetype)layoutAnchorWithEdges:(NSDirectionalRectEdge)edges;

//                                +-----+       +------------------------------------+
//                                |~~~~~|       |      edges: [.top,.trailing]       |
//   +----------------------------+~~~~~|<------+ unitOffset: NSPoint(x:0.5,y:-0.5)  |
//   |                            |~~~~~|       +------------------------------------+
//   |                            +--+--+
//   |                               |
//   |                               |
//   |                               |
//   |                               |
//   +-------------------------------+
//
// To specify additional offsets, combine edges with absoluteOffset or unitOffset.

+ (instancetype)layoutAnchorWithEdges:(NSDirectionalRectEdge)edges absoluteOffset:(NSPoint)absoluteOffset;
+ (instancetype)layoutAnchorWithEdges:(NSDirectionalRectEdge)edges fractionalOffset:(NSPoint)fractionalOffset;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property(readonly) NSDirectionalRectEdge edges;
@property(readonly) NSPoint offset;
@property(readonly) BOOL isAbsoluteOffset;
@property(readonly) BOOL isFractionalOffset;
@end

API_AVAILABLE(macos(10.15))
@protocol NSCollectionLayoutContainer<NSObject>
@property(readonly) NSSize contentSize;                               // resolved size of container (before any insets are applied)
@property(readonly) NSSize effectiveContentSize;                      // after insets are applied
@property(readonly) NSDirectionalEdgeInsets contentInsets;            // values < 1.0 are interpreted as fractional values (e.g. leading:0.15 == 15% width)
@property(readonly) NSDirectionalEdgeInsets effectiveContentInsets;   // resolved value after resolving any unit values
@end

API_AVAILABLE(macos(10.15))
@protocol NSCollectionLayoutEnvironment<NSObject>
@property(readonly) id<NSCollectionLayoutContainer> container;
@end

API_AVAILABLE(macos(10.15))
@protocol NSCollectionLayoutVisibleItem<NSObject>
@property CGFloat alpha;
@property NSInteger zIndex;
@property(getter=isHidden) BOOL hidden;
@property NSPoint center;

@property(readonly) NSString *name;
@property(readonly) NSIndexPath *indexPath;
@property(readonly) NSRect frame;
@property(readonly) NSRect bounds;
@property(readonly) NSCollectionElementCategory representedElementCategory;
@property(readonly,nullable) NSString *representedElementKind;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

#endif
