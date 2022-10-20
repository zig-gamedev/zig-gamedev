/*
	NSMatrix.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSControl.h>
#import <AppKit/NSUserInterfaceValidation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor;
@protocol NSMatrixDelegate;

/* Matrix Constants */

typedef NS_ENUM(NSUInteger, NSMatrixMode) {
    NSRadioModeMatrix			= 0,
    NSHighlightModeMatrix		= 1,
    NSListModeMatrix			= 2,
    NSTrackModeMatrix			= 3
};

/*
    NSMatrix is deprecated and will be unsupported in future releases.  For radio button grouping, sibling radio buttons that share the same action will operate in an exclusive mode.
 */
@interface NSMatrix : NSControl <NSUserInterfaceValidations, NSViewToolTipOwner>


- (instancetype)initWithFrame:(NSRect)frameRect;
- (instancetype)initWithFrame:(NSRect)frameRect mode:(NSMatrixMode)mode prototype:(NSCell *)cell numberOfRows:(NSInteger)rowsHigh numberOfColumns:(NSInteger)colsWide;
- (instancetype)initWithFrame:(NSRect)frameRect mode:(NSMatrixMode)mode cellClass:(nullable Class)factoryId numberOfRows:(NSInteger)rowsHigh numberOfColumns:(NSInteger)colsWide;


@property (assign) Class cellClass;
@property (nullable, copy) __kindof NSCell *prototype;
- (NSCell *)makeCellAtRow:(NSInteger)row column:(NSInteger)col;
@property NSMatrixMode mode;
@property BOOL allowsEmptySelection;
- (void)sendAction:(SEL)selector to:(id)object forAllCells:(BOOL)flag;
@property (readonly, copy) NSArray<NSCell *> *cells;
- (void)sortUsingSelector:(SEL)comparator;
- (void)sortUsingFunction:(NSInteger (NS_NOESCAPE *)(id, id, void * _Nullable))compare context:(nullable void *)context;
@property (nullable, readonly, strong) __kindof NSCell *selectedCell;
@property (readonly, copy) NSArray<__kindof NSCell *> *selectedCells;
@property (readonly) NSInteger selectedRow;
@property (readonly) NSInteger selectedColumn;
@property (getter=isSelectionByRect) BOOL selectionByRect;
- (void)setSelectionFrom:(NSInteger)startPos to:(NSInteger)endPos anchor:(NSInteger)anchorPos highlight:(BOOL)lit;
- (void)deselectSelectedCell;
- (void)deselectAllCells;
- (void)selectCellAtRow:(NSInteger)row column:(NSInteger)col;
- (void)selectAll:(nullable id)sender;
- (BOOL)selectCellWithTag:(NSInteger)tag;
@property NSSize cellSize;
@property NSSize intercellSpacing;
- (void)setScrollable:(BOOL)flag;
@property (copy) NSColor *backgroundColor;
@property (nullable, copy) NSColor *cellBackgroundColor;
@property BOOL drawsCellBackground;
@property BOOL drawsBackground;
- (void)setState:(NSInteger)value atRow:(NSInteger)row column:(NSInteger)col;
- (void)getNumberOfRows:(nullable NSInteger *)rowCount columns:(nullable NSInteger *)colCount;
@property (readonly) NSInteger numberOfRows;
@property (readonly) NSInteger numberOfColumns;
- (nullable __kindof NSCell *)cellAtRow:(NSInteger)row column:(NSInteger)col;
- (NSRect)cellFrameAtRow:(NSInteger)row column:(NSInteger)col;
- (BOOL)getRow:(NSInteger *)row column:(NSInteger *)col ofCell:(NSCell *)cell;
- (BOOL)getRow:(NSInteger *)row column:(NSInteger *)col forPoint:(NSPoint)point;
- (void)renewRows:(NSInteger)newRows columns:(NSInteger)newCols;
- (void)putCell:(NSCell *)newCell atRow:(NSInteger)row column:(NSInteger)col;
- (void)addRow;
- (void)addRowWithCells:(NSArray<NSCell *> *)newCells;
- (void)insertRow:(NSInteger)row;
- (void)insertRow:(NSInteger)row withCells:(nullable NSArray<NSCell *> *)newCells;
- (void)removeRow:(NSInteger)row;
- (void)addColumn;
- (void)addColumnWithCells:(NSArray<NSCell *> *)newCells;
- (void)insertColumn:(NSInteger)column;
- (void)insertColumn:(NSInteger)column withCells:(nullable NSArray<NSCell *> *)newCells;
- (void)removeColumn:(NSInteger)col;
- (nullable __kindof NSCell *)cellWithTag:(NSInteger)tag;
@property (nullable) SEL doubleAction;
@property BOOL autosizesCells;
- (void)sizeToCells;
- (void)setValidateSize:(BOOL)flag;
- (void)drawCellAtRow:(NSInteger)row column:(NSInteger)col;
- (void)highlightCell:(BOOL)flag atRow:(NSInteger)row column:(NSInteger)col;

@property (getter=isAutoscroll) BOOL autoscroll;
- (void)scrollCellToVisibleAtRow:(NSInteger)row column:(NSInteger)col;
@property (readonly) NSInteger mouseDownFlags;
- (void)mouseDown:(NSEvent *)event;
- (BOOL)performKeyEquivalent:(NSEvent *)event;
- (BOOL)sendAction;
- (void)sendDoubleAction;
@property (nullable, weak) id<NSMatrixDelegate> delegate;
- (BOOL)textShouldBeginEditing:(NSText *)textObject;
- (BOOL)textShouldEndEditing:(NSText *)textObject;
- (void)textDidBeginEditing:(NSNotification *)notification;
- (void)textDidEndEditing:(NSNotification *)notification;
- (void)textDidChange:(NSNotification *)notification;
- (void)selectText:(nullable id)sender;
- (nullable __kindof NSCell *)selectTextAtRow:(NSInteger)row column:(NSInteger)col;
- (BOOL)acceptsFirstMouse:(nullable NSEvent *)event;
- (void)resetCursorRects;
- (void)setToolTip:(nullable NSString *)toolTipString forCell:(NSCell *)cell;
- (nullable NSString *)toolTipForCell:(NSCell *)cell;

/* Determine whether the receiver autorecalculates its cellSize. The default is NO. If set to YES, then the receiver will adjust its cellSize to accommodate its largest cell. Changing the cellSize does not directly affect the frame of the receiver; however it does affect the intrinsic content size, which may cause the receiver to resize under autolayout. 
 
   The default is NO. When using autolayout, you typically want this to be set to YES.
 */
@property BOOL autorecalculatesCellSize API_AVAILABLE(macos(10.8));

@end

@interface NSMatrix(NSKeyboardUI)
@property BOOL tabKeyTraversesCells;
@property (nullable, strong) __kindof NSCell *keyCell;
@end

@protocol NSMatrixDelegate <NSControlTextEditingDelegate> @end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

