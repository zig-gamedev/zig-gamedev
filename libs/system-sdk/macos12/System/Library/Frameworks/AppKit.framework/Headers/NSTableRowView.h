/*
    NSTableRowView.h
    Application Kit
    Copyright (c) 2008-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <AppKit/NSView.h>
#import <AppKit/NSCell.h>
#import <AppKit/NSTableView.h>
#import <AppKit/AppKitDefines.h>

/* View Based TableView: The NSTableRowView is the view shown for a row in the table. It is responsible for drawing things associated with the row, including the selection highlight, and group row look. Properties can be changed on a row-by-row basis by using the table delegate method -tableView:didAddRowView:forRow:. Modifications of the properties are NOT reflected by the NSTableView instance; the NSTableRowView is simply a representation of the state. In other words, setting rowView.selected will NOT change the -selectedRowIndexes in NSTableView.
 */
NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSLayoutConstraint;

API_AVAILABLE(macos(10.7))
@interface NSTableRowView : NSView <NSAccessibilityRow>

/* The tableView sets the selectionHighlightStyle based on its current value. 
 */
@property NSTableViewSelectionHighlightStyle selectionHighlightStyle;

/* When emphasized is YES, the view should draw with the (typically) blue [NSColor alternateSelectedControlColor]. When NO, it should use the (typically) gray [NSColor secondarySelectedControlColor].
 */
@property(getter=isEmphasized) BOOL emphasized;

/* When groupRowStyle is set to YES, this row is a group row and will draw appropriately. 
 */
@property(getter=isGroupRowStyle) BOOL groupRowStyle;

/* Selection state. The selection will be drawn when selected and the selectionHighlightStyle is not NSTableViewSelectionHighlightStyleNone.
 */
@property(getter=isSelected) BOOL selected;

/* Next and previous row selection state. Allows subclassers to draw selection differently based on the previous next row being selected. State is automatically updated by NSTableView, however, the changing of the state does not invalidate the row view. If a row view depends on this state, it should override the particular setter and call [self setNeedsDisplay:YES] before or after calling super.
 */
@property(getter=isPreviousRowSelected) BOOL previousRowSelected API_AVAILABLE(macos(10.10));
@property(getter=isNextRowSelected) BOOL nextRowSelected;

/* Floating is a temporary attribute that is set when a particular group row is actually floating above other rows. The state may change dynamically based on the position of the group row. Drawing may be different for rows that are currently 'floating'. The TableView's delegate must implement tableView:isGroupRow: (or outlineView:isGroupItem:) to enable floating group rows.
 */
@property(getter=isFloating) BOOL floating;

/* Drag and drop state. When targetForDropOperation is set to YES, the NSTableRowView will draw a drop on indicator based on the current draggingDestinationFeedbackStyle. The indentationForDropOperation is set appropriately by NSOutlineView if the drop target row should be indented. Otherwise it is 0. On Mac OS 10.0.2, the indentation is always 0, and the feedback style is now drawn similar to selection.
*/ 
@property(getter=isTargetForDropOperation) BOOL targetForDropOperation;
@property NSTableViewDraggingDestinationFeedbackStyle draggingDestinationFeedbackStyle;
@property CGFloat indentationForDropOperation;

/* The interiorBackgroundStyle can be used as an indication of how the subviews should draw. This value is dynamically computed based on the set of properties set for the NSTableRowView. Subclassers can override this value when they draw differently based on the currently displayed properties. This method can also be called to determine what color a subview should use, or alternatively, NSControls can have the -backgroundStyle set on their cell to this value.
 */
@property(readonly) NSBackgroundStyle interiorBackgroundStyle;

/* The backgroundColor property defaults to the Table View's backgroundColor, unless usesAlternatingRowBackgroundColors is set to YES. In that case, the colors alternate, and are automatically updated as required by insertions and deletions. The value can be customized in the delegate method -tableView:didAddRowView:forRow:. The property is animatable.
 */
@property(copy) NSColor *backgroundColor;

/* NOTE: NSTableRowView overrides isFlipped and returns YES.
 */

#pragma mark -
#pragma mark ***** Drawing override points *****
/* 
 Row view drawing can be customized by overriding the following set of methods. -drawRect: calls them in the following deterministic order:
    -drawBackgroundInRect:
    -drawSelectionInRect: (only called if selection should be drawn (selected==YES, or alpha fading it in or out))
    -drawSeparatorInRect: (only called if the table has a grid mask set to include a horizontal separator).
    -drawDraggingDestinationFeedbackInRect: (only called if targetForDropOperation==YES)
*/

/* Override point to draw a custom background. 'dirtyRect' is the current dirty rect passed from -drawRect:. By default, this method draws the background color or "group row" style look. It will also draw the below look for a drop target if targetForDropOperation is YES. It is also recommended to override interiorBackgroundStyle when overriding this method.
 */
- (void)drawBackgroundInRect:(NSRect)dirtyRect;

/* Override point for drawing the selection. 'dirtyRect' is the current dirty rect passed from -drawRect:. This method is only called if the selection should be drawn. The selection will automatically be alpha-blended if the selection is animating in or out. The default selection drawn is dependent on the selectionHighlightStyle.  It is also recommended to override interiorBackgroundStyle when overriding this method.
 
   Note: For 10.10 drawSelectionInRect: will not be called when the selectionHighlightStyle is set to NSTableViewSelectionHighlightStyleSourceList.
 */
- (void)drawSelectionInRect:(NSRect)dirtyRect;

/* Override point for drawing the (horizontal) separator.'dirtyRect' is the current dirty rect passed from -drawRect:. By default, the separator is only drawn if the enclosing table's gridStyleMask is set to include a horizontal separator. The separator should be drawn at the bottom of the row view, indicating a separation from this row and the next.
 */
- (void)drawSeparatorInRect:(NSRect)dirtyRect;

/* Override point for drawing the dragging destination feedback when the entire row is a "drop target". 'dirtyRect' is the current dirty rect passed from -drawRect:. This method only is called if targetForDropOperation is YES, and will draw  based on the properties set (such as the group row style).
 */
- (void)drawDraggingDestinationFeedbackInRect:(NSRect)dirtyRect;

#pragma mark -

#pragma mark ***** Cell View Access *****

/* Provides access to the given view at a particular column. This is the only way to access cell views after the row view has been removed from the table. 
 */
- (nullable id)viewAtColumn:(NSInteger)column;

/* Provides access to the number of columns represented by views in this NSTableRowView. This may not be equal to the number of columns in the enclosing NSTableView, if this row view is a group style and has a single view that spans the entire width of the row.
 */
@property(readonly) NSInteger numberOfColumns;

#pragma mark -


@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
