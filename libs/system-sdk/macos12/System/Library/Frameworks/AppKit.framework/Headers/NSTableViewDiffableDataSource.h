/*
    NSTableViewDiffableDataSource.h
    Application Kit
    Copyright (c) 2019-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSTableView.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSTableRowView;
@class NSTableCellView;
@class NSDiffableDataSourceSnapshot<SectionIdentifierType,ItemIdentifierType>;

API_AVAILABLE(macos(11.0))
@interface NSTableViewDiffableDataSource<SectionIdentifierType,ItemIdentifierType> : NSObject<NSTableViewDataSource>

typedef NSView * _Nonnull (^NSTableViewDiffableDataSourceCellProvider)(NSTableView * _Nonnull tableView, NSTableColumn * _Nonnull column, NSInteger row, id _Nonnull itemId);

- (instancetype)initWithTableView:(NSTableView*)tableView cellProvider:(NSTableViewDiffableDataSourceCellProvider)cellProvider;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

// Create a mutable "snapshot" of the data source state.
//   This can be mutated via transactional updates which, when completed, can be "applied" back to a connected data source
- (NSDiffableDataSourceSnapshot<SectionIdentifierType,ItemIdentifierType>*)snapshot;

// Apply a snapshot to the connected data source.
//   To perform an animated diff of the changes: animatingDifferences == YES
//   For non-animated update (i.e. reloadData) without the diff overhead: animatingDifferences == NO
//   If the (optional) completion block is specified, it will be called on the main queue when the animations are completed.
- (void)applySnapshot:(NSDiffableDataSourceSnapshot<SectionIdentifierType,ItemIdentifierType> *)snapshot animatingDifferences:(BOOL)animatingDifferences;

- (void)applySnapshot:(NSDiffableDataSourceSnapshot<SectionIdentifierType,ItemIdentifierType>*)snapshot animatingDifferences:(BOOL)animatingDifferences completion:(void(^ _Nullable)(void))completion;


// Row <-> item identifier conversion
- (nullable ItemIdentifierType)itemIdentifierForRow:(NSInteger)row;
- (NSInteger)rowForItemIdentifier:(ItemIdentifierType)identifier;

// Row <-> section identifier conversion
- (nullable SectionIdentifierType)sectionIdentifierForRow:(NSInteger)row;
- (NSInteger)rowForSectionIdentifier:(SectionIdentifierType)identifier;

typedef NSTableRowView * _Nonnull (^NSTableViewDiffableDataSourceRowProvider)(NSTableView * _Nonnull tableView, NSInteger row, id _Nonnull identifier);

@property (copy,nullable) NSTableViewDiffableDataSourceRowProvider rowViewProvider;

// FIXME: GroupRow or SectionHeader?
typedef NSView * _Nonnull (^NSTableViewDiffableDataSourceSectionHeaderViewProvider)(NSTableView * _Nonnull tableView, NSInteger row, id _Nonnull sectionId);

@property (copy,nullable) NSTableViewDiffableDataSourceSectionHeaderViewProvider sectionHeaderViewProvider;

// Default value is NSTableViewAnimationEffectFade. Change takes effect on the next update.
@property NSTableViewAnimationOptions defaultRowAnimation;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
