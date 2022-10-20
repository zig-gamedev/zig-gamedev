/*
	NSGridView.h
	Application Kit
	Copyright (c) 2015-2021, Apple Inc.
	All rights reserved.
 */


#import <AppKit/NSView.h>
#import <AppKit/NSLayoutAnchor.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSLayoutConstraint, NSGridCell, NSGridColumn, NSGridRow;


typedef NS_ENUM(NSInteger, NSGridCellPlacement) {
    NSGridCellPlacementInherited = 0,
    NSGridCellPlacementNone,
    NSGridCellPlacementLeading,
    NSGridCellPlacementTop = NSGridCellPlacementLeading,
    
    NSGridCellPlacementTrailing,
    NSGridCellPlacementBottom = NSGridCellPlacementTrailing,
    
    NSGridCellPlacementCenter,
    NSGridCellPlacementFill
    
} API_AVAILABLE(macos(10.12));


typedef NS_ENUM(NSInteger, NSGridRowAlignment) {
    NSGridRowAlignmentInherited = 0,
    NSGridRowAlignmentNone,
    NSGridRowAlignmentFirstBaseline,
    NSGridRowAlignmentLastBaseline
} API_AVAILABLE(macos(10.12));

APPKIT_EXTERN const CGFloat NSGridViewSizeForContent API_AVAILABLE(macos(10.12)); // Default value for row & column size, indicating it should automatically fit the content views.


/*
 NSGridView is a layout container for aligning views in spreadsheet-like rows & columns, indexed from the top left starting at 0.  Rows and columns are sized to fit their largest content (unless an explicit size has been set), but all cells in a given column will be the same width and all cells in a row will be the same height.

 NSGridPlacement is used to specify the positioning of the contentView within the cell.  The content placement can be configured separately for the X & Y axes to align the content with either edge or to center it.  Placement properties on rows, columns, and cells default to 'inherited' so that the grid-level properties will effect all cells.  Content placement can be overridden for individual rows, columns, or cells by simply changing the appropriate property to a value other than 'inherited'.
 */

API_AVAILABLE(macos(10.12))
@interface NSGridView : NSView

- (instancetype)initWithFrame:(NSRect)frameRect NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

+ (instancetype)gridViewWithNumberOfColumns:(NSInteger)columnCount rows:(NSInteger)rowCount; // Convenience initializer for gridview with specific dimensions.
+ (instancetype)gridViewWithViews:(NSArray<NSArray<NSView *> *> *)rows; // Convenience initializer for an autoreleased GridView large enough to hold the provided rows. Each element in 'rows' is an array of views for that row.

@property (readonly) NSInteger numberOfRows;
@property (readonly) NSInteger numberOfColumns;


// Cells are arranged in rows and columns, which allow for configuration of content alignment on a row/column basis.
- (NSGridRow *)rowAtIndex:(NSInteger)index;
- (NSInteger)indexOfRow:(NSGridRow *)row; // Finds the index of the given row.  O(numberOfRows) time.
- (NSGridColumn *)columnAtIndex:(NSInteger)index;
- (NSInteger)indexOfColumn:(NSGridColumn *)column; // Finds the index of the given column.  O(numberOfColumns) time.
- (NSGridCell*)cellAtColumnIndex:(NSInteger)columnIndex rowIndex:(NSInteger)rowIndex;
- (nullable NSGridCell *)cellForView:(NSView*)view; // returns the cell from the receiver that contains the given view or one of its ancestors.

// Rows & columns can be inserted & removed dynamically.  The grid will be enlarged to hold the specified views, if needed.
- (NSGridRow *)addRowWithViews:(NSArray<NSView *> *)views;
- (NSGridRow *)insertRowAtIndex:(NSInteger)index withViews:(NSArray<NSView *> *)views;
- (void)moveRowAtIndex:(NSInteger)fromIndex toIndex:(NSInteger)toIndex;
- (void)removeRowAtIndex:(NSInteger)index;

- (NSGridColumn *)addColumnWithViews:(NSArray<NSView *> *)views;
- (NSGridColumn *)insertColumnAtIndex:(NSInteger)index withViews:(NSArray<NSView *> *)views;
- (void)moveColumnAtIndex:(NSInteger)fromIndex toIndex:(NSInteger)toIndex;
- (void)removeColumnAtIndex:(NSInteger)index;


// Grid-level layout properties will be used by cells that don't have the properties defined themselves or at the column/row level.  They default to bottom-leading placement, with no alignment.
@property NSGridCellPlacement xPlacement;
@property NSGridCellPlacement yPlacement;
@property NSGridRowAlignment rowAlignment;

// Grid-level property values for row & column spacing are added to the padding properties on rows & columns.  Defaults to 6pt.
@property CGFloat rowSpacing;
@property CGFloat columnSpacing;

// Expands the cell at the top-leading corner of the given range to cover the entire area.  Other cells in the range become invalid: they will no longer maintain any layout, constraints, or content views.  Cell merging has no effect on the base cell coordinate system of the gridview, and cell references within a merged region will all refer to the single merged cell.  This is intended to be used to configure the grid geometry before installing views, but in the event that the cells being merged contain contentViews, only the top-leading will be kept.
- (void)mergeCellsInHorizontalRange:(NSRange)hRange verticalRange:(NSRange)vRange;

@end



// NSGridRow represents a row of cells in the grid view, and allows content placement to be specified on a per-row basis.
API_AVAILABLE(macos(10.12)) NS_SWIFT_UI_ACTOR
@interface NSGridRow : NSObject <NSCoding>

@property (readonly,weak) NSGridView *gridView;
@property (readonly) NSInteger numberOfCells;
- (NSGridCell *)cellAtIndex:(NSInteger)index;


// Row level placement properties will be used by cells whose Y-axis properties are set to 'inherited'.  These also default to 'inherited', falling back to the GridView level properties.
@property NSGridCellPlacement yPlacement;
@property NSGridRowAlignment rowAlignment;
@property CGFloat height; // Height of this row, or NSGridViewSizeForContent (the default) to fit content automatically.
@property CGFloat topPadding; // Padding is extra space between this row and an adjacent one.  Defaults to 0. Total inter-row-space is firstRow.bottomPadding + grid.rowSpacing + secondRow.topPadding
@property CGFloat bottomPadding;

@property (getter=isHidden) BOOL hidden; // Hidden rows/columns will collapse to 0 size and hide all their contentViews.
- (void)mergeCellsInRange:(NSRange)range;

@end



// NSGridColumn represents a column of cells in the grid view, and allows content placement to be specified on a per-column basis.
API_AVAILABLE(macos(10.12)) NS_SWIFT_UI_ACTOR
@interface NSGridColumn : NSObject <NSCoding>

@property (readonly,weak) NSGridView *gridView;
@property (readonly) NSInteger numberOfCells;
- (NSGridCell *)cellAtIndex:(NSInteger)index;

// Column level placement will be used by cells whose xPlacement is set to 'inherited'.  This also defaults to 'inherited', falling back to the NSGridView property.
@property NSGridCellPlacement xPlacement;
@property CGFloat width; // Width of this column, or NSGridViewSizeForContent (the default) to fit content automatically.
@property CGFloat leadingPadding; // Padding is extra space between this column and an adjacent one.  Defaults to 0.  Total inter-column-space is firstColumn.trailingPadding + grid.columnSpacing + secondColumn.leadingPadding
@property CGFloat trailingPadding;

@property (getter=isHidden) BOOL hidden; // Hidden rows/columns will collapse to 0 size and hide all their contentViews.
- (void)mergeCellsInRange:(NSRange)range;

@end


// NSGridCell represents a single cell in the grid.  The cell will maintain the necessary constraints for positioning out whichever contentView is set.
API_AVAILABLE(macos(10.12)) NS_SWIFT_UI_ACTOR
@interface NSGridCell : NSObject <NSCoding>


@property (strong,nullable) __kindof NSView *contentView; // The view whose placement will be managed by this cell.

@property (class, readonly, strong) NSView *emptyContentView; // This view is used as a marker in NSGridView's "...WithViews:" methods to indicate a cell whose contentView should be nil.

@property (readonly,weak) NSGridRow *row;
@property (readonly,weak) NSGridColumn *column;

// These properties control how the content view is placed in the cell.  Placement is configured independently for each axis.  Baseline alignment within a row is handled separately.  Properties set to the default value of 'inherited' will "fall back" to properties defined on the row/column, or to the GridView itself if needed.  Properties set to 'none' will cause the corresponding aspect of content layout to be left unmanaged by the grid view.  Handling of baseline alignment is special: all cells within a row that specify a rowAlignment (i.e., not "None") are considered to be 'aligned'.  The contentViews of such cells will be aligned by the specified baseline for each cell.  This creates the potential for unsatisfiable constraints in combination with the yPlacement properties on the aligned views.  Therefore, the entire baseline-aligned group is placed using the yPlacement of the first cell with a value other than NSGridCellPlaceNone.  The yPlacement properties of the remaining cells are overridden by their rowAlignment.

@property NSGridCellPlacement xPlacement;
@property NSGridCellPlacement yPlacement;
@property NSGridRowAlignment rowAlignment;

// Set these constraints to provide custom placement for the cell's content view.  NSGridView will activate the constraints when the cell is visible, and deactivate them when it is hidden.  Note that it is usually also necessary to set the xPlacement and/or yPlacement to 'Custom' in order to prevent NSGridView from adding its own placement constraints in one or both axes (which is likely to cause unsatisfiable constraints in combination with the custom ones).
@property (copy) NSArray<NSLayoutConstraint *> *customPlacementConstraints;

@end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
