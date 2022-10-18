/*
    NSOutlineView.h
    Application Kit
    Copyright (c) 1997-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSTableView.h>
#import <AppKit/AppKitDefines.h>
#import <CoreFoundation/CFDictionary.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSButtonCell;
@class NSTableView;
@class NSTableHeaderView;
@class NSTableColumn;
@class NSTintConfiguration;
@class NSNotification;
@class NSString;
@protocol NSOutlineViewDelegate, NSOutlineViewDataSource;

/* NSOutlineViewDropOnItemIndex may be used as a valid childIndex of a drop target item. In this case, the drop will happen directly on the target item.
*/
enum { NSOutlineViewDropOnItemIndex = -1 };

@interface NSOutlineView : NSTableView <NSAccessibilityOutline>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-property-type"

@property (nullable, weak) id <NSOutlineViewDelegate> delegate;
@property (nullable, weak) id <NSOutlineViewDataSource> dataSource;

#pragma clang diagnostic pop

/* The 'outlineTableColumn' is the column that displays data in a hierarchical fashion, indented one identationlevel per level, decorated with indentation marker (disclosure triangle) on rows that are expandable. A nil 'outlineTableColumn' is silently ignored. On 10.5 and higher, this value is saved in encodeWithCoder: and restored in initWithCoder:.
*/
@property (nullable, assign) NSTableColumn *outlineTableColumn;

/* Returns YES if 'item' is expandable and can contain other items. May call out to the delegate/datasource, if required.
*/
- (BOOL)isExpandable:(nullable id)item;

/* Returns the number of children for a given parent item. This is mostly useful for static outline views where the data was encoded at design time; however, it will work for both static and non-static outline views. This may call out to the datasource, if required.
 */
- (NSInteger)numberOfChildrenOfItem:(nullable id)item API_AVAILABLE(macos(10.10));

/* Returns the child item for a given parent item. This is mostly useful for static outline views where the data was encoded at design time, however, it will work for both static and non-static outline views. This may call out to the datasource, if required.
 */
- (nullable id)child:(NSInteger)index ofItem:(nullable id)item API_AVAILABLE(macos(10.10));

/* Expands 'item', if not already expanded, and all children if 'expandChildren' is YES. On 10.5 and higher, passing 'nil' for 'item' will expand  each item under the root.
*/
- (void)expandItem:(nullable id)item expandChildren:(BOOL)expandChildren;

/* Calls expandItem:expandChildren with 'expandChildren == NO' 
*/
- (void)expandItem:(nullable id)item;

/* Collapses 'item' and all children if 'collapseChildren' is YES. On 10.5 and higher, passing 'nil' for 'item' will collapse each item under the root. 
*/
- (void)collapseItem:(nullable id)item collapseChildren:(BOOL)collapseChildren;

/* Calls collapseItem:collapseChildren with 'collapseChildren == NO' 
*/
- (void)collapseItem:(nullable id)item;

/* Reloads 'item' and all children if 'reloadChildren' is YES. On 10.5 and higher, passing 'nil' for 'item' will reload everything under the root item.
*/
- (void)reloadItem:(nullable id)item reloadChildren:(BOOL)reloadChildren;

/* Calls reloadItem:reloadChildren with 'reloadChildren == NO' 
*/
- (void)reloadItem:(nullable id)item;

/* Returns the parent for 'item', or nil, if the parent is the root.
*/
- (nullable id)parentForItem:(nullable id)item;

/* Returns the child index of 'item' within its parent. The performance of this method is O(1) at best and O(n) at worst.
 */
- (NSInteger)childIndexForItem:(id)item API_AVAILABLE(macos(10.11));

/* Item/Row translation
*/
- (nullable id)itemAtRow:(NSInteger)row;
- (NSInteger)rowForItem:(nullable id)item;

/* Indentation
*/
- (NSInteger)levelForItem:(nullable id)item;
- (NSInteger)levelForRow:(NSInteger)row;
- (BOOL)isItemExpanded:(nullable id)item;

/* Controls the amount of indentation per level. Negative values are ignored, and only integral values are accepted. The default value is 16.0. An indentationPerLevel of 0 can be used to eliminate all indentation and make an NSOutlineView appear more like an NSTableView.
*/
@property CGFloat indentationPerLevel;

/* The indentation marker is the visual indicator that shows an item is expandable (i.e. disclosure triangle). The default value is YES.
*/
@property BOOL indentationMarkerFollowsCell;

/* If autoresizesOutlineColumn is YES, then the outlineTableColumn will automatically resize when there is a new expanded child at a particular depth level. The default value is YES.
*/
@property BOOL autoresizesOutlineColumn;

/* Returns the frame of the outline cell for a particular row, considering the current indentation and indentationMarkerFollowsCell value. If 'row' is not an expandable row, it will return NSZeroRect. This method can be overridden by subclassers to return a custom frame for the outline button cell. If an empty rect is returned, no outline cell will be drawn for that row.
*/
- (NSRect)frameOfOutlineCellAtRow:(NSInteger)row API_AVAILABLE(macos(10.5)); 

/* Drag and Drop
*/

/* To be used from validateDrop: in order to "re-target" the proposed drop.  To specify a drop on an item I, one would specify item=I, and index=NSOutlineViewDropOnItemIndex.  To specify a drop between child 2 and 3 of an item I, on would specify item=I, and index=3 (children are zero-base indexed).  To specify a drop on an un-expandable item I, one would specify item=I, and index=NSOutlineViewDropOnItemIndex.
*/
- (void)setDropItem:(nullable id)item dropChildIndex:(NSInteger)index;

/* This method returns YES to indicate that auto expanded items should return to their original collapsed state.  Override this method to provide custom behavior.  'deposited' tells wether or not the drop terminated due to a successful drop (as indicated by the return value from acceptDrop:).  Note that exiting the view will be treated the same as a failed drop.
*/
- (BOOL)shouldCollapseAutoExpandedItemsForDeposited:(BOOL)deposited;

/* Persistence. The value for autosaveExpandedItems is saved out in the nib file on Mac OS 10.5 or higher. The default value is NO. Calling setAutosaveExpandedItems:YES requires you to implement outlineView:itemForPersistentObject: and outlineView:persistentObjectForItem:.
*/
@property BOOL autosaveExpandedItems;

#pragma mark -
#pragma mark ***** Animated Insert / Remove / Move Support *****

/* This method parallels TableView's -insertRowIndexes:withAnimation:, and is used in a way similar to NSMutableArray's -insertObjects:atIndexes: This method inserts a new item at 'indexes' in 'parent'. 'parent' can be nil to insert items into the root of the OutlineView. The operation will not do anything if 'parent' is not expanded. The actual item values are determined by the dataSource methods -outlineView:child:ofItem: (note that this will only be called after an -endUpdates happens to ensure dataSource integrity). Calling this method multiple times within the same beginUpdates/endUpdates block is allowed; new insertions will move previously inserted new items, just like modifying an array. Inserting an index beyond what is available will throw an exception.  The "Cell Based TableView" must first call -beginUpdates before calling this method. This method can also be used when "usesStaticContents=YES".
 */
- (void)insertItemsAtIndexes:(NSIndexSet *)indexes inParent:(nullable id)parent withAnimation:(NSTableViewAnimationOptions)animationOptions API_AVAILABLE(macos(10.7));

/* This method parallels TableView's -removeRowsAtIndexes:withAnimation:, and is used similar to NSMutableArray's -removeObjectsAtIndexes:. The items at the given indexes will be removed from the parent. The operation will not do anything if 'parent' is not expanded. If one of the children items is expanded, then all of its children rows will also be removed. Calling this method multiple times within the same beginUpdates/endUpdates block is allowed; changes work just like modifying an array. Deleting an index beyond what is available will throw an exception.  The "Cell Based TableView" must first call -beginUpdates before calling this method. This method can also be used when "usesStaticContents=YES".
 */
- (void)removeItemsAtIndexes:(NSIndexSet *)indexes inParent:(nullable id)parent withAnimation:(NSTableViewAnimationOptions)animationOptions API_AVAILABLE(macos(10.7));

/* This method parallels TableView's -moveRowAtIndex:toIndex:. The item located at 'fromIndex' is moved from the existing 'oldParent' to the given index in 'newParent'. 'newParent' can be the same as 'oldParent' to reorder an item within the same parent. This method can be called multiple times within the same beginUpdates/endUpdates block. Moving from an invalid index, or to an invalid index will throw an exception.  The "Cell Based TableView" must first call -beginUpdates before calling this method. This method can also be used when "usesStaticContents=YES".
 */
- (void)moveItemAtIndex:(NSInteger)fromIndex inParent:(nullable id)oldParent toIndex:(NSInteger)toIndex inParent:(nullable id)newParent API_AVAILABLE(macos(10.7));

/* Modifications to an OutlineView should be done with the insertItems:/deleteItems:/moveItem: methods, and not the NSTableView primitives. Internally, these methods are used in the implementation, and while they are unavailable for callers they can be overridden by subclasses.
 */
- (void)insertRowsAtIndexes:(NSIndexSet *)indexes withAnimation:(NSTableViewAnimationOptions)animationOptions UNAVAILABLE_ATTRIBUTE;
- (void)removeRowsAtIndexes:(NSIndexSet *)indexes withAnimation:(NSTableViewAnimationOptions)animationOptions UNAVAILABLE_ATTRIBUTE;
- (void)moveRowAtIndex:(NSInteger)oldIndex toIndex:(NSInteger)newIndex UNAVAILABLE_ATTRIBUTE;

#pragma mark -

/* Get and set the user interface layout direction. When set to NSUserInterfaceLayoutDirectionRightToLeft, the Outline View will show the disclosure triangle to the right of the cell instead of the left. This method is available for NSOutlineView on 10.7 and higher.
 */
@property NSUserInterfaceLayoutDirection userInterfaceLayoutDirection API_AVAILABLE(macos(10.7));

/* When YES, the NSOutlineView will retain and release the objects returned to it from the dataSource (outlineView:child:ofItem:). When NO, it only treats the objects as opaque items and assume the client has a retain on them. The default value is YES for applications linked on 10.12 and later, and NO for previous applications. This value is not encoded in the nib, and must be explicitly set to NO in code if one requires the legacy behavior and is linking on 10.12 and later. In general, this is required if the items themselves create a retain cycle.
 */
@property BOOL stronglyReferencesItems API_AVAILABLE(macos(10.12));

@end

#pragma mark -

/* Data Source Note: Specifying nil as the item will refer to the "root" item(s).
*/
@protocol NSOutlineViewDataSource <NSObject>
@optional

#pragma mark -
#pragma mark ***** Required Methods (unless bindings is used) *****

/* NOTE: it is not acceptable to call reloadData or reloadItem from the implementation of any of the following four methods, and doing so can cause corruption in NSOutlineViews internal structures.
 */

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(nullable id)item;
- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(nullable id)item;
- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item;

/* NOTE: this method is optional for the View Based OutlineView.
 */
- (nullable id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(nullable NSTableColumn *)tableColumn byItem:(nullable id)item;

#pragma mark -
#pragma mark ***** Optional Methods *****

/* View Based OutlineView: This method is not applicable.
 */
- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(nullable id)object forTableColumn:(nullable NSTableColumn *)tableColumn byItem:(nullable id)item;

/* NOTE: Returning nil indicates the item no longer exists, and won't be re-expanded.
 */
- (nullable id)outlineView:(NSOutlineView *)outlineView itemForPersistentObject:(id)object;

/* NOTE: Returning nil indicates that the item's state will not be persisted.
 */
- (nullable id)outlineView:(NSOutlineView *)outlineView persistentObjectForItem:(nullable id)item;

/* Optional - Sorting Support
    This is the indication that sorting needs to be done. Typically the data source will sort its data, reload, and adjust selections.
*/
- (void)outlineView:(NSOutlineView *)outlineView sortDescriptorsDidChange:(NSArray<NSSortDescriptor *> *)oldDescriptors;

/* Optional - Drag and Drop support
*/

/* Dragging Source Support - Required for multi-image dragging. Implement this method to allow the table to be an NSDraggingSource that supports multiple item dragging. Return a custom object that implements NSPasteboardWriting (or simply use NSPasteboardItem). Return nil to prevent a particular item from being dragged. If this method is implemented, then outlineView:writeItems:toPasteboard: will not be called.
 */
- (nullable id <NSPasteboardWriting>)outlineView:(NSOutlineView *)outlineView pasteboardWriterForItem:(id)item API_AVAILABLE(macos(10.7));

/* Dragging Source Support - Optional. Implement this method know when the dragging session is about to begin and to potentially modify the dragging session. 'draggedItems' is an array of items that we dragged, excluding items that were not dragged due to outlineView:pasteboardWriterForItem: returning nil. This array will directly match the pasteboard writer array used to begin the dragging session with [NSView beginDraggingSessionWithItems:event:source]. Hence, the order is deterministic, and can be used in -outlineView:acceptDrop:item:childIndex: when enumerating the NSDraggingInfo's pasteboard classes. 
 */
- (void)outlineView:(NSOutlineView *)outlineView draggingSession:(NSDraggingSession *)session willBeginAtPoint:(NSPoint)screenPoint forItems:(NSArray *)draggedItems API_AVAILABLE(macos(10.7));

/* Dragging Source Support - Optional. Implement this method know when the dragging session has ended. This delegate method can be used to know when the dragging source operation ended at a specific location, such as the trash (by checking for an operation of NSDragOperationDelete).
 */
- (void)outlineView:(NSOutlineView *)outlineView draggingSession:(NSDraggingSession *)session endedAtPoint:(NSPoint)screenPoint operation:(NSDragOperation)operation API_AVAILABLE(macos(10.7));

/* Dragging Source Support - Optional for single-image dragging. This method is called after it has been determined that a drag should begin, but before the drag has been started.  To refuse the drag, return NO.  To start a drag, return YES and place the drag data onto the pasteboard (data, owner, etc...).  The drag image and other drag related information will be set up and provided by the outline view once this call returns with YES.  The items array is the list of items that will be participating in the drag.
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView writeItems:(NSArray *)items toPasteboard:(NSPasteboard *)pasteboard API_DEPRECATED("Use -outlineView:pasteboardWriterForItem: instead", macos(10.0, 10.15));

/* Dragging Destination Support - Required for multi-image dragging. Implement this method to allow the table to update dragging items as they are dragged over the view. Typically this will involve calling [draggingInfo enumerateDraggingItemsWithOptions:forView:classes:searchOptions:usingBlock:] and setting the draggingItem's imageComponentsProvider to a proper image based on the content. For View Based TableViews, one can use NSTableCellView's -draggingImageComponents and -draggingImageFrame.
 */
- (void)outlineView:(NSOutlineView *)outlineView updateDraggingItemsForDrag:(id <NSDraggingInfo>)draggingInfo API_AVAILABLE(macos(10.7));

/* Dragging Destination Support - This method is used by NSOutlineView to determine a valid drop target. Based on the mouse position, the outline view will suggest a proposed child 'index' for the drop to happen as a child of 'item'. This method must return a value that indicates which NSDragOperation the data source will perform. The data source may "re-target" a drop, if desired, by calling setDropItem:dropChildIndex: and returning something other than NSDragOperationNone. One may choose to re-target for various reasons (eg. for better visual feedback when inserting into a sorted position). On Leopard linked applications, this method is called only when the drag position changes or the dragOperation changes (ie: a modifier key is pressed). Prior to Leopard, it would be called constantly in a timer, regardless of attribute changes.
*/
- (NSDragOperation)outlineView:(NSOutlineView *)outlineView validateDrop:(id <NSDraggingInfo>)info proposedItem:(nullable id)item proposedChildIndex:(NSInteger)index;

/* Dragging Destination Support - This method is called when the mouse is released over an outline view that previously decided to allow a drop via the validateDrop method. The data source should incorporate the data from the dragging pasteboard at this time. 'index' is the location to insert the data as a child of 'item', and are the values previously set in the validateDrop: method.
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView acceptDrop:(id <NSDraggingInfo>)info item:(nullable id)item childIndex:(NSInteger)index;

/* Dragging Destination Support - NSOutlineView data source objects can support file promised drags via by adding  NSFilesPromisePboardType to the pasteboard in outlineView:writeItems:toPasteboard:.  NSOutlineView implements -namesOfPromisedFilesDroppedAtDestination: to return the results of this data source method.  This method should returns an array of filenames for the created files (filenames only, not full paths).  The URL represents the drop location.  For more information on file promise dragging, see documentation on the NSDraggingSource protocol and -namesOfPromisedFilesDroppedAtDestination:.
*/
- (NSArray<NSString *> *)outlineView:(NSOutlineView *)outlineView namesOfPromisedFilesDroppedAtDestination:(NSURL *)dropDestination forDraggedItems:(NSArray *)items API_DEPRECATED("Use NSFilePromiseReceiver objects instead", macos(10.0,10.13));

@end

#pragma mark -

@protocol NSOutlineViewDelegate <NSControlTextEditingDelegate>
@optional

#pragma mark -
#pragma mark ***** View Based TableView/OutlineView Support *****

/* View Based OutlineView: See the delegate method -tableView:viewForTableColumn:row: in NSTableView.
 */
- (nullable NSView *)outlineView:(NSOutlineView *)outlineView viewForTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item API_AVAILABLE(macos(10.7));

/* View Based OutlineView: See the delegate method -tableView:rowViewForRow: in NSTableView.
 */
- (nullable NSTableRowView *)outlineView:(NSOutlineView *)outlineView rowViewForItem:(id)item API_AVAILABLE(macos(10.7));

/* View Based OutlineView: This delegate method can be used to know when a new 'rowView' has been added to the table. At this point, you can choose to add in extra views, or modify any properties on 'rowView'.
 */
- (void)outlineView:(NSOutlineView *)outlineView didAddRowView:(NSTableRowView *)rowView forRow:(NSInteger)row API_AVAILABLE(macos(10.7));

/* View Based OutlineView: This delegate method can be used to know when 'rowView' has been removed from the table. The removed 'rowView' may be reused by the table so any additionally inserted views should be removed at this point. A 'row' parameter is included. 'row' will be '-1' for rows that are being deleted from the table and no longer have a valid row, otherwise it will be the valid row that is being removed due to it being moved off screen.
 */
- (void)outlineView:(NSOutlineView *)outlineView didRemoveRowView:(NSTableRowView *)rowView forRow:(NSInteger)row API_AVAILABLE(macos(10.7));

#pragma mark -
#pragma mark ***** Regular Delegate Methods *****

/* NSOutlineView replacements for NSTableView delegate methods.
*/
- (void)outlineView:(NSOutlineView *)outlineView willDisplayCell:(id)cell forTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item;
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldEditTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item;
- (BOOL)selectionShouldChangeInOutlineView:(NSOutlineView *)outlineView;

/* Optional - Return YES if 'item' should be selected and 'NO' if it should not. For better performance, and greater control, it is recommended that you use outlineView:selectionIndexesForProposedSelection:. 
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldSelectItem:(id)item;

/* Optional - Return a set of new indexes to select when the user changes the selection with the keyboard or mouse. If implemented, this method will be called instead of outlineView:shouldSelectItem:. This method may be called multiple times with one new index added to the existing selection to find out if a particular index can be selected when the user is extending the selection with the keyboard or mouse. Note that 'proposedSelectionIndexes' will contain the entire newly suggested selection, and you can return the existing selection to avoid changing the selection.
*/
- (NSIndexSet *)outlineView:(NSOutlineView *)outlineView selectionIndexesForProposedSelection:(NSIndexSet *)proposedSelectionIndexes API_AVAILABLE(macos(10.5));

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldSelectTableColumn:(nullable NSTableColumn *)tableColumn;

- (void)outlineView:(NSOutlineView *)outlineView mouseDownInHeaderOfTableColumn:(NSTableColumn *)tableColumn;
- (void)outlineView:(NSOutlineView *)outlineView didClickTableColumn:(NSTableColumn *)tableColumn;
- (void)outlineView:(NSOutlineView *)outlineView didDragTableColumn:(NSTableColumn *)tableColumn;

/* Optional - Tool Tip support
    When the user pauses over a cell, the value returned from this method will be displayed in a tooltip.  'point' represents the current mouse location in view coordinates.  If you don't want a tooltip at that location, return an empty string.  On entry, 'rect' represents the proposed active area of the tooltip.  By default, rect is computed as [cell drawingRectForBounds:cellFrame].  To control the default active area, you can modify the 'rect' parameter.
*/
- (NSString *)outlineView:(NSOutlineView *)outlineView toolTipForCell:(NSCell *)cell rect:(NSRectPointer)rect tableColumn:(nullable NSTableColumn *)tableColumn item:(id)item mouseLocation:(NSPoint)mouseLocation;

/* Optional - Variable Row Heights
    Implement this method to support a table with varying row heights. The height returned by this method should not include intercell spacing. Returning a height of -1 will default to the rowHeight of the tableView for normal rows, and the system defined height for group rows. Performance Considerations: For large tables in particular, you should make sure that this method is efficient. NSTableView may cache the values this method returns, but this should NOT be depended on, as all values may not be cached. To signal a row height change, call -noteHeightOfRowsWithIndexesChanged:. For a given row, the same row height should always be returned until -noteHeightOfRowsWithIndexesChanged: is called, otherwise unpredicable results will happen. NSTableView automatically invalidates its entire row height cache in -reloadData, and -noteNumberOfRowsChanged.
 */
- (CGFloat)outlineView:(NSOutlineView *)outlineView heightOfRowByItem:(id)item;

/* View Based OutlineView: Optional - Item tinting customization
    Implement this method to customize an item's tinting behavior. This is typically used for sidebars which may want custom colors. Return a non-nil `NSTintConfiguration` to specify a particular tinting behavior for the item's row, or return nil to inherit the tinting behavior from the item's parent. Items at the root of the outline inherit the default tint configuration.
*/
- (nullable NSTintConfiguration *)outlineView:(NSOutlineView *)outlineView tintConfigurationForItem:(id)item API_AVAILABLE(macos(11.0));


/* Optional - Type select support
    Implement this method if you want to control the string that is used for type selection. You may want to change what is searched for based on what is displayed, or simply return nil for that row and/or column to not be searched. By default, all cells with text in them are searched. The default value when this delegate method is not implemented is [[outlineView preparedCellForColumn:tableColumn row:[outlineView rowForItem:item]] stringValue], and this value can be returned from the delegate method if desired.
*/
- (nullable NSString *)outlineView:(NSOutlineView *)outlineView typeSelectStringForTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item API_AVAILABLE(macos(10.5));

/* Optional - Type select support
    Implement this method if you want to control how type selection works. Return the first item that matches the searchString from within the range of startItem to endItem. It is possible for endItem to be less than startItem if the search will wrap. Return nil when there is no match. Include startItem as a possible match, but do not include endItem. It is not necessary to implement this method in order to support type select.
*/
- (nullable id)outlineView:(NSOutlineView *)outlineView nextTypeSelectMatchFromItem:(id)startItem toItem:(id)endItem forString:(NSString *)searchString API_AVAILABLE(macos(10.5));

/* Optional - Type select support
    Implement this method if you would like to prevent a type select from happening based on the current event and current search string. Generally, this will be called from keyDown: and the event will be a key event. The search string will be nil if no type select has began. 
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldTypeSelectForEvent:(NSEvent *)event withCurrentSearchString:(nullable NSString *)searchString API_AVAILABLE(macos(10.5));

/* Optional - Expansion ToolTip support
    Implement this method and return NO to prevent an expansion tooltip from appearing for a particular cell at 'item' in 'tableColumn'. See NSCell.h for more information on expansion tool tips. 
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldShowCellExpansionForTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item API_AVAILABLE(macos(10.5));

/*  Optional - Custom tracking support
    It is possible to control the ability to track a cell or not. Normally, only selectable or selected cells can be tracked. If you implement this method, cells which are not selectable or selected can be tracked, and vice-versa. For instance, this allows you to have an NSButtonCell in a table which does not change the selection, but can still be clicked on and tracked.
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldTrackCell:(NSCell *)cell forTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item API_AVAILABLE(macos(10.5));

/*  Optional - Different cells for each row
    A different data cell can be returned for any particular tableColumn and item, or a cell that will be used for the entire row (a full width cell). The returned cell should properly implement copyWithZone:, since the cell may be copied by NSTableView. If the tableColumn is non-nil, you should return a cell, and generally you will want to default to returning the result from [tableColumn dataCellForRow:row].

    When each row (identified by the item) is being drawn, this method will first be called with a nil tableColumn. At this time, you can return a cell that will be used to draw the entire row, acting like a group. If you do return a cell for the 'nil' tableColumn, be prepared to have the other corresponding datasource and delegate methods to be called with a 'nil' tableColumn value. If don't return a cell, the method will be called once for each tableColumn in the tableView, as usual.
*/
- (nullable NSCell *)outlineView:(NSOutlineView *)outlineView dataCellForTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item API_AVAILABLE(macos(10.5));

/* Optional - Group rows. 
    Implement this method and return YES to indicate a particular row should have the "group row" style drawn for that row. If the cell in that row is an NSTextFieldCell and contains only a stringValue, the "group row" style attributes will automatically be applied for that cell. 
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView isGroupItem:(id)item API_AVAILABLE(macos(10.5));

/* Optional - Controlling expanding/collapsing of items.
    Called when the outlineView is about to expand 'item'. Implementations of this method should be fast. This method may be called multiple times if a given 'item' has children that are also being expanded. If NO is returned, 'item' will not be expanded, nor will its children (even if -[outlineView expandItem:item expandChildren:YES] is called).
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldExpandItem:(id)item;

/* Optional - Controlling expanding/collapsing of items.
    Called when the outlineView is about to collapse 'item'. Implementations of this method should be fast. If NO is returned, 'item' will not be collapsed, nor will its children (even if -[outlineView collapseItem:item collapseChildren:YES] is called).
*/
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldCollapseItem:(id)item;

/* Optional - OutlineCell (disclosure triangle button cell)
    Implement this method to customize the "outline cell" used for the disclosure triangle button. customization of the "outline cell" used for the disclosure triangle button.
*/
- (void)outlineView:(NSOutlineView *)outlineView willDisplayOutlineCell:(id)cell forTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item;

/* Optional - Autosizing table columns
 Implement this method if you want to control how wide a column is made when the user double clicks on the resize divider. By default, NSTableView iterates every row in the table, accesses a cell via preparedCellAtRow:column:, and requests the "cellSize" to find the appropriate largest width to use. For large row counts, a monte carlo simulation is done instead of interating every row. For accurate performance, it is recommended that this method is implemented when using large tables. 
*/

- (CGFloat)outlineView:(NSOutlineView *)outlineView sizeToFitWidthOfColumn:(NSInteger)column API_AVAILABLE(macos(10.6));

/*  Optional - Control of column reordering.
 Specifies if the column can be reordered to a new location, or not. 'columnIndex' is the column that is being dragged. The actual NSTableColumn instance can be retrieved from the [tableView tableColumns] array. 'newColumnIndex' is the new proposed target location for 'columnIndex'. When a column is initially dragged by the user, the delegate is first called with a 'newColumnIndex' of -1. Returning NO will disallow that column from being reordered at all. Returning YES allows it to be reordered, and the delegate will be called again when the column reaches a new location. If this method is not implemented, all columns are considered reorderable. 
 */
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldReorderColumn:(NSInteger)columnIndex toColumn:(NSInteger)newColumnIndex API_AVAILABLE(macos(10.6));

/* Optional - Hiding the outline cell (disclosure triangle)
 Allows the delegate to decide if the outline cell (disclosure triangle) for 'item' should be displayed or not. This method will only be called for expandable rows. If 'NO' is returned,  -[outlineView frameOfOutlineCellAtRow:] will return NSZeroRect, causing the outline cell to be hidden. In addition, if 'NO' is returned, the row will not be collapsable by keyboard shortcuts.
 */
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldShowOutlineCellForItem:(id)item API_AVAILABLE(macos(10.6));

/* Notifications - see comments below for more information about the @"NSObject" parameter in the userInfo dictionary.
 */
- (void)outlineViewSelectionDidChange:(NSNotification *)notification;
- (void)outlineViewColumnDidMove:(NSNotification *)notification;
- (void)outlineViewColumnDidResize:(NSNotification *)notification;
- (void)outlineViewSelectionIsChanging:(NSNotification *)notification;
- (void)outlineViewItemWillExpand:(NSNotification *)notification;
- (void)outlineViewItemDidExpand:(NSNotification *)notification;
- (void)outlineViewItemWillCollapse:(NSNotification *)notification;
- (void)outlineViewItemDidCollapse:(NSNotification *)notification;

@end


/* The following NSOutlineView*Keys are used by the View Based NSOutlineView to create the "disclosure button" used to collapse and expand items. The NSOutlineView creates these buttons by calling [self makeViewWithIdentifier:owner:] passing in the key as the identifier and the delegate as the owner. Custom NSButtons (or subclasses thereof) can be provided for NSOutlineView to use in the following two ways:
 1. makeViewWithIdentifier:owner: can be overridden, and if the identifier is (for instance) NSOutlineViewDisclosureButtonKey, a custom NSButton can be configured and returned. Be sure to set the button.identifier to be NSOutlineViewDisclosureButtonKey.
 2. At design time, a button can be added to the outlineview which has this identifier, and it will be unarchived and used as needed.
 
 When a custom button is used, it is important to properly set up the target/action to do something (probably expand or collapse the rowForView: that the sender is located in). Or, one can call super to get the default button, and copy its target/action to get the normal default behavior.
 
 NOTE: These keys are backwards compatible to 10.7, however, the symbol is not exported prior to 10.9 and the regular string value must be used (i.e.: @"NSOutlineViewDisclosureButtonKey").
 */
APPKIT_EXTERN NSUserInterfaceItemIdentifier const NSOutlineViewDisclosureButtonKey API_AVAILABLE(macos(10.9)); // The normal triangle disclosure button
APPKIT_EXTERN NSUserInterfaceItemIdentifier const NSOutlineViewShowHideButtonKey API_AVAILABLE(macos(10.9)); // The show/hide button used in "Source Lists"

/* Notifications
*/
APPKIT_EXTERN NSNotificationName NSOutlineViewSelectionDidChangeNotification;
APPKIT_EXTERN NSNotificationName NSOutlineViewColumnDidMoveNotification;                                // @"NSOldColumn", @"NSNewColumn"
APPKIT_EXTERN NSNotificationName NSOutlineViewColumnDidResizeNotification;                // @"NSTableColumn", @"NSOldWidth"
APPKIT_EXTERN NSNotificationName NSOutlineViewSelectionIsChangingNotification;


/* Note for the following NSOutlineViewItem*Notifications:
   The 'userInfo' dictionary in the notification will have an @"NSObject" key where the value is the changed (id)item.
*/
APPKIT_EXTERN NSNotificationName NSOutlineViewItemWillExpandNotification;
APPKIT_EXTERN NSNotificationName NSOutlineViewItemDidExpandNotification;
APPKIT_EXTERN NSNotificationName NSOutlineViewItemWillCollapseNotification;
APPKIT_EXTERN NSNotificationName NSOutlineViewItemDidCollapseNotification;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

