/*
    NSCollectionViewFlowLayout.h
    Application Kit
    Copyright (c) 2015-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSCollectionView.h>
#import <AppKit/NSCollectionViewLayout.h>
#import <AppKit/AppKitDefines.h>

typedef NS_ENUM(NSInteger, NSCollectionViewScrollDirection) {
    NSCollectionViewScrollDirectionVertical,
    NSCollectionViewScrollDirectionHorizontal
} API_AVAILABLE(macos(10.11));

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

APPKIT_EXTERN NSCollectionViewSupplementaryElementKind const NSCollectionElementKindSectionHeader API_AVAILABLE(macos(10.11));
APPKIT_EXTERN NSCollectionViewSupplementaryElementKind const NSCollectionElementKindSectionFooter API_AVAILABLE(macos(10.11));

API_AVAILABLE(macos(10.11))
@interface NSCollectionViewFlowLayoutInvalidationContext : NSCollectionViewLayoutInvalidationContext

@property BOOL invalidateFlowLayoutDelegateMetrics; // if set to NO, flow layout will not requery the collection view delegate for size information etc.
@property BOOL invalidateFlowLayoutAttributes; // if set to NO, flow layout will keep all layout information, effectively not invalidating - useful for a subclass which invalidates only a piece of itself

@end

@protocol NSCollectionViewDelegateFlowLayout <NSCollectionViewDelegate>
@optional

- (NSSize)collectionView:(NSCollectionView *)collectionView layout:(NSCollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath;
- (NSEdgeInsets)collectionView:(NSCollectionView *)collectionView layout:(NSCollectionViewLayout*)collectionViewLayout insetForSectionAtIndex:(NSInteger)section;
- (CGFloat)collectionView:(NSCollectionView *)collectionView layout:(NSCollectionViewLayout*)collectionViewLayout minimumLineSpacingForSectionAtIndex:(NSInteger)section;
- (CGFloat)collectionView:(NSCollectionView *)collectionView layout:(NSCollectionViewLayout*)collectionViewLayout minimumInteritemSpacingForSectionAtIndex:(NSInteger)section;
- (NSSize)collectionView:(NSCollectionView *)collectionView layout:(NSCollectionViewLayout*)collectionViewLayout referenceSizeForHeaderInSection:(NSInteger)section;
- (NSSize)collectionView:(NSCollectionView *)collectionView layout:(NSCollectionViewLayout*)collectionViewLayout referenceSizeForFooterInSection:(NSInteger)section;

@end

API_AVAILABLE(macos(10.11))
@interface NSCollectionViewFlowLayout : NSCollectionViewLayout

@property CGFloat minimumLineSpacing;
@property CGFloat minimumInteritemSpacing;
@property NSSize itemSize;
@property NSSize estimatedItemSize; // defaults to NSZeroSize - setting a non-zero size enables cells that self-size via -preferredLayoutAttributesFittingAttributes:
@property NSCollectionViewScrollDirection scrollDirection; // default is NSCollectionViewScrollDirectionVertical
@property NSSize headerReferenceSize;
@property NSSize footerReferenceSize;
@property NSEdgeInsets sectionInset;

/* Set these properties to YES to get headers that pin to the top of the visible area and footers that pin to the bottom while scrolling.  Archived with the layout's other persistent properties.  Enabling this feature may affect the parenting of header and footer views.
*/
@property BOOL sectionHeadersPinToVisibleBounds API_AVAILABLE(macos(10.12));
@property BOOL sectionFootersPinToVisibleBounds API_AVAILABLE(macos(10.12));


#pragma mark *** Section Collapse ***

/* Returns YES if the specified section is currently collapsed; NO if not, or if there is no such section.  Defaults to NO.
*/
- (BOOL)sectionAtIndexIsCollapsed:(NSUInteger)sectionIndex API_AVAILABLE(macos(10.12));

/* Collapses the specified section to a single row, if it is not already collapsed.
*/
- (void)collapseSectionAtIndex:(NSUInteger)sectionIndex API_AVAILABLE(macos(10.12));

/* Un-collapses the specified section, if it is currently collapsed.
*/
- (void)expandSectionAtIndex:(NSUInteger)sectionIndex API_AVAILABLE(macos(10.12));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
