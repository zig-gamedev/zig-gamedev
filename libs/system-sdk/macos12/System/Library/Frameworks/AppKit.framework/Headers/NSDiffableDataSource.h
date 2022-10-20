/*
    NSDiffableDataSource.h
    Application Kit
    Copyright (c) 2019-2021, Apple Inc.
    All rights reserved.
*/

#import <TargetConditionals.h>

#if !TARGET_OS_IPHONE

#import <AppKit/NSCollectionView.h>

NS_ASSUME_NONNULL_BEGIN

/* An NSDiffableDataSourceSnapshot represents the complete state of a UI element (e.g. NSCollectionView)

   Snapshots can be created in two ways:
   1. From a connected data source: e.g. `[diffableDataSource snapshot]`
      This will contain the current state of the UI element.

   2. Constructing a new instance: e.g. `[[NSDiffableDataSourceSnapshot<Int,UUID> alloc] init]`
      This will construct an empty snapshot with no section or item identifiers.

   Notes:
     1. All identifiers must be unique. Section and Item identifiers do not overlap and may contain values that exist in the other (i.e. it is OK
        to have a section identifier == 1 and an item identifier == 1)

     2. If you pass duplicate values in an item or section array (e.g. `-appendItemsWithIdentifiers:`), the system will throw an exception.
*/

API_AVAILABLE(macos(10.15),ios(13.0),tvos(13.0),watchos(6.0))
@interface NSDiffableDataSourceSnapshot<SectionIdentifierType,ItemIdentifierType> : NSObject<NSCopying>

// structure

@property(nonatomic,readonly) NSInteger numberOfItems;
@property(nonatomic,readonly) NSInteger numberOfSections;
@property(nonatomic,readonly) NSArray<SectionIdentifierType> *sectionIdentifiers;
@property(nonatomic,readonly) NSArray<ItemIdentifierType> *itemIdentifiers;

- (NSInteger)numberOfItemsInSection:(SectionIdentifierType)sectionIdentifier;
- (NSArray<ItemIdentifierType> *)itemIdentifiersInSectionWithIdentifier:(SectionIdentifierType)sectionIdentifier;
- (nullable SectionIdentifierType)sectionIdentifierForSectionContainingItemIdentifier:(ItemIdentifierType)itemIdentifier;

- (NSInteger)indexOfItemIdentifier:(ItemIdentifierType)itemIdentifier;             // If item identifier is not present returns NSNotFound
- (NSInteger)indexOfSectionIdentifier:(SectionIdentifierType)sectionIdentifier;    // If section identifier is not present returns NSNotFound

// items operations

- (void)appendItemsWithIdentifiers:(NSArray<ItemIdentifierType> *)identifiers;     // convenience: appends into last section
- (void)appendItemsWithIdentifiers:(NSArray<ItemIdentifierType> *)identifiers intoSectionWithIdentifier:(SectionIdentifierType)sectionIdentifier;

- (void)insertItemsWithIdentifiers:(NSArray<ItemIdentifierType> *)identifiers beforeItemWithIdentifier:(ItemIdentifierType)itemIdentifier;
- (void)insertItemsWithIdentifiers:(NSArray<ItemIdentifierType> *)identifiers afterItemWithIdentifier:(ItemIdentifierType)itemIdentifier;

- (void)deleteItemsWithIdentifiers:(NSArray<ItemIdentifierType> *)identifiers;
- (void)deleteAllItems;

- (void)moveItemWithIdentifier:(ItemIdentifierType)fromIdentifier beforeItemWithIdentifier:(ItemIdentifierType)toIdentifier;
- (void)moveItemWithIdentifier:(ItemIdentifierType)fromIdentifier afterItemWithIdentifier:(ItemIdentifierType)toIdentifier;

- (void)reloadItemsWithIdentifiers:(NSArray<ItemIdentifierType>*)identifiers;

// section operations

- (void)appendSectionsWithIdentifiers:(NSArray*)sectionIdentifiers;

- (void)insertSectionsWithIdentifiers:(NSArray<SectionIdentifierType> *)sectionIdentifiers beforeSectionWithIdentifier:(SectionIdentifierType)toSectionIdentifier;
- (void)insertSectionsWithIdentifiers:(NSArray<SectionIdentifierType> *)sectionIdentifiers afterSectionWithIdentifier:(SectionIdentifierType)toSectionIdentifier;

- (void)deleteSectionsWithIdentifiers:(NSArray<SectionIdentifierType> *)sectionIdentifiers;

- (void)moveSectionWithIdentifier:(SectionIdentifierType)fromSectionIdentifier beforeSectionWithIdentifier:(SectionIdentifierType)toSectionIdentifier;
- (void)moveSectionWithIdentifier:(SectionIdentifierType)fromSectionIdentifier afterSectionWithIdentifier:(SectionIdentifierType)toSectionIdentifier;

- (void)reloadSectionsWithIdentifiers:(NSArray<SectionIdentifierType>*)sectionIdentifiers;

@end

/* NSCollectionViewDiffableDataSource offers an identifier-based interface for providing an NSCollectionView's content.  It automatically performs the inserts, deletes, and moves necessary to transition an NSCollectionView from one model-state snapshot to another.
*/
API_AVAILABLE(macos(10.15))
@interface NSCollectionViewDiffableDataSource<SectionIdentifierType,ItemIdentifierType> : NSObject<NSCollectionViewDataSource>

typedef NSCollectionViewItem * _Nullable (^NSCollectionViewDiffableDataSourceItemProvider)(NSCollectionView * _Nonnull, NSIndexPath * _Nonnull, ItemIdentifierType _Nonnull);

typedef NSView * _Nullable (^NSCollectionViewDiffableDataSourceSupplementaryViewProvider)(NSCollectionView* _Nonnull, NSString * _Nonnull, NSIndexPath * _Nonnull);


/* Creates a new NSCollectionViewDiffableDataSource that automatically wires itself up as the given `collectionView`'s `dataSource`.  The data source is initially empty.  To put items into the `collectionView`, you must create an NSDiffableDataSourceSnapshot, add the desired section and item identifiers to the snapshot, and apply the snapshot to the diffable data source.  The required `itemProvider` closure serves the same purpose as NSCollectionViewDataSource `-collectionView:itemForRepresentedObjectAtIndexPath:` method: Its job is to ask the `collectionView` to make an item of the desired type, configure its content and appearance to represent the model object specified by the given identifier and NSIndexPath, and return the configured item.
*/
- (instancetype)initWithCollectionView:(NSCollectionView*)collectionView itemProvider:(NSCollectionViewDiffableDataSourceItemProvider)itemProvider;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

// Create a mutable "snapshot" of the data source state.
//   This can be mutated via transactional updates which, when completed, can be "applied" back to a connected data source
- (NSDiffableDataSourceSnapshot<SectionIdentifierType,ItemIdentifierType>*)snapshot;

// Apply a snapshot to the connected data source.
//   To perform an animated diff of the changes: animatingDifferences == YES
//   For non-animated update (i.e. reloadData) without the diff overhead: animatingDifferences == NO
- (void)applySnapshot:(NSDiffableDataSourceSnapshot<SectionIdentifierType,ItemIdentifierType> *)snapshot animatingDifferences:(BOOL)animatingDifferences;

// IndexPath <-> item identifier conversion
- (nullable ItemIdentifierType)itemIdentifierForIndexPath:(NSIndexPath *)indexPath;
- (nullable NSIndexPath *)indexPathForItemIdentifier:(ItemIdentifierType)identifier;

@property(copy,nullable) NSCollectionViewDiffableDataSourceSupplementaryViewProvider supplementaryViewProvider;
@end

NS_ASSUME_NONNULL_END

#endif
