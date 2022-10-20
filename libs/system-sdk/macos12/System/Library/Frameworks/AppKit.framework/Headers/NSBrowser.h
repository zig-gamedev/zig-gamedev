/*
    NSBrowser.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSControl.h>
#import <AppKit/NSDragging.h>
#import <AppKit/NSViewController.h>
#import <AppKit/NSApplication.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

static const NSAppKitVersion NSAppKitVersionNumberWithContinuousScrollingBrowser = 680.0;
static const NSAppKitVersion NSAppKitVersionNumberWithColumnResizingBrowser = 685.0;

typedef NSString * NSBrowserColumnsAutosaveName NS_SWIFT_BRIDGED_TYPEDEF;

@class NSMatrix, NSScroller, NSIndexSet;
@protocol NSBrowserDelegate;

typedef NS_ENUM(NSUInteger, NSBrowserColumnResizingType) {
/* Column sizes are fixed and set by developer.     
 */
    NSBrowserNoColumnResizing = 0,
    
/* No user resizing. Columns grow as window grows.  
 */
    NSBrowserAutoColumnResizing = 1,
    
/* Columns fixed as window grows.  User can resize. 
 */
    NSBrowserUserColumnResizing = 2,
};



/* In drag and drop, used to specify the drop operation from inside the delegate method browser:validateDrop:proposedRow:column:dropOperation. See the delegate method description for more information.
 */
typedef NS_ENUM(NSUInteger, NSBrowserDropOperation) { 
    NSBrowserDropOn,
    NSBrowserDropAbove,
} API_AVAILABLE(macos(10.5));


@interface NSBrowser : NSControl

@property (class, readonly) Class cellClass;

- (void)loadColumnZero;
@property (getter=isLoaded, readonly) BOOL loaded;

@property (nullable) SEL doubleAction;
- (void)setCellClass:(Class)factoryId;
@property (null_resettable, strong) id /* NSCell * */ cellPrototype;
@property (nullable, weak) id<NSBrowserDelegate> delegate;
@property BOOL reusesColumns;

@property BOOL hasHorizontalScroller;
@property BOOL autohidesScroller API_AVAILABLE(macos(10.6));
@property BOOL separatesColumns;
@property (getter=isTitled) BOOL titled;
@property CGFloat minColumnWidth;

@property NSInteger maxVisibleColumns;

@property BOOL allowsMultipleSelection;
@property BOOL allowsBranchSelection;
@property BOOL allowsEmptySelection;
@property BOOL takesTitleFromPreviousColumn;

@property BOOL sendsActionOnArrowKeys;

/* Returns the item at the given index path. This method can only be used if the delegate implements the item data source methods. The indexPath must be displayable in the browser.
 */
- (nullable id)itemAtIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.6));

/* Returns the item located at 'row' in 'column'.
 */
- (nullable id)itemAtRow:(NSInteger)row inColumn:(NSInteger)column API_AVAILABLE(macos(10.6));

/* Returns the index path of the item whose children are displayed in the given column. This method can only be used if the delegate implements the item data source methods.
 */
- (NSIndexPath *)indexPathForColumn:(NSInteger)column API_AVAILABLE(macos(10.6));

/* Returns whether the given item is a leaf item. This method can only be used if the delegate implements the item data source methods.
 */
- (BOOL)isLeafItem:(nullable id)item API_AVAILABLE(macos(10.6));

/* Updates a particular set of rows in 'column'. For rows that are visible, appropriate dataSource and delegate methods will be called and the row will be redrawn. For browsers that support variable row heights, the row height will not be re-queried from the delegate. This method can only be used if the delegate implements the item data source methods.
*/
- (void)reloadDataForRowIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column API_AVAILABLE(macos(10.6));

/* Returns the item that contains all children located in 'column'. In other words, it is the parent item for that column.
 */
- (nullable id)parentForItemsInColumn:(NSInteger)column API_AVAILABLE(macos(10.6));

/* Scrolls 'row' to be visible within 'column'. 'column' will not be scrolled visible. To scroll 'column' to visible, first call -[browser scrollColumnToVisible:column].
 */
- (void)scrollRowToVisible:(NSInteger)row inColumn:(NSInteger)column API_AVAILABLE(macos(10.6));

- (void)setTitle:(NSString *)string ofColumn:(NSInteger)column;
- (nullable NSString *)titleOfColumn:(NSInteger)column;
@property (copy) NSString *pathSeparator;
- (BOOL)setPath:(NSString *)path;
- (NSString *)path;
- (NSString *)pathToColumn:(NSInteger)column;

/* Returns the column and row clicked on to display a context menu. These methods will return -1 when no menu is active.
 */
@property (readonly) NSInteger clickedColumn API_AVAILABLE(macos(10.6));
@property (readonly) NSInteger clickedRow API_AVAILABLE(macos(10.6));

@property (readonly) NSInteger selectedColumn;

/* For the item based browser, selectedCell returns the prepared cell at the selected row in the selected column.
 */
@property (nullable, readonly, strong) id selectedCell;
- (nullable id)selectedCellInColumn:(NSInteger)column;

/* For the item based browser, selectedCells returns a copy of all prepared cells in the selected row in the selected column
 */
@property (nullable, readonly, copy) NSArray<NSCell *> *selectedCells;

- (void)selectRow:(NSInteger)row inColumn:(NSInteger)column;

- (NSInteger)selectedRowInColumn:(NSInteger)column;

/* Returns the index path of the item selected in the browser, or nil if there is no selection. The setter sets the browser's selection to the item at path. Throws an exception if the path is invalid. This method can only be used if the delegate implements the item data source methods.
 */
@property (nullable, copy) NSIndexPath *selectionIndexPath API_AVAILABLE(macos(10.6));

/* Returns the index paths of all items selected in the browser. The setter sets the browser's selection to the specified index paths. Throws an exception if any of the paths are invalid. This method can only be used if the delegate implements the item data source methods.
 */
@property (copy) NSArray<NSIndexPath *> *selectionIndexPaths API_AVAILABLE(macos(10.6));

/* Sets the selected row 'indexes' in the matrix located at 'column'. 
 */
- (void)selectRowIndexes:(NSIndexSet *)indexes inColumn:(NSInteger)column API_AVAILABLE(macos(10.5));

/* Returns the selected cell indexes in the matrix located at 'column'. 
 */
- (nullable NSIndexSet *)selectedRowIndexesInColumn:(NSInteger)column API_AVAILABLE(macos(10.5));

- (void)reloadColumn:(NSInteger)column;
- (void)validateVisibleColumns;
- (void)scrollColumnsRightBy:(NSInteger)shiftAmount;
- (void)scrollColumnsLeftBy:(NSInteger)shiftAmount;
- (void)scrollColumnToVisible:(NSInteger)column;

/* Returns the last loaded column. This is equal to the total number of columns minus 1. When setting. 'column' must be equal to or less than -lastColumn. To add more columns, use -addColumn.
 */
@property NSInteger lastColumn;

/* Adds a column at the end. To remove columns, use -setLastColumn:
 */
- (void)addColumn;

@property (readonly) NSInteger numberOfVisibleColumns;
@property (readonly) NSInteger firstVisibleColumn;
@property (readonly) NSInteger lastVisibleColumn;


- (nullable id)loadedCellAtRow:(NSInteger)row column:(NSInteger)col;
- (void)selectAll:(nullable id)sender;
- (void)tile;
- (void)doClick:(nullable id)sender;
- (void)doDoubleClick:(nullable id)sender;
- (BOOL)sendAction;

- (NSRect)titleFrameOfColumn:(NSInteger)column;
- (void)drawTitleOfColumn:(NSInteger)column inRect:(NSRect)rect;
@property (readonly) CGFloat titleHeight;
- (NSRect)frameOfColumn:(NSInteger)column;
- (NSRect)frameOfInsideOfColumn:(NSInteger)column;

/* Returns the frame of the row at 'row' / 'column' including the area for the expandable arrow. The returned NSRect is in the NSBrowser coordinate space.
*/
- (NSRect)frameOfRow:(NSInteger)row inColumn:(NSInteger)column API_AVAILABLE(macos(10.6));

/* Finds the row and column located at 'point', returning YES if both can be found. If a row does not exist at 'point', then -1 is set for the row. If a column does not exist at 'point', then -1 is set for the column. 'point' is expected to be in the NSBrowser's coordinate system.
*/
- (BOOL)getRow:(nullable NSInteger *)row column:(nullable NSInteger *)column forPoint:(NSPoint)point API_AVAILABLE(macos(10.6));

/* These methods convert between column width (the column's scrollview), and the content width (the matrix in the scrollview).  For example, to guarantee that 16 pixels of your browser cell are always visible, call [browser setMinColumnWidth:[browser columnWidthForColumnContentWidth:16]] 
*/
- (CGFloat)columnWidthForColumnContentWidth:(CGFloat)columnContentWidth;
- (CGFloat)columnContentWidthForColumnWidth:(CGFloat)columnWidth;

/* Default is NSBrowserAutoColumnResizing.  This setting is persistent. 
 */
@property NSBrowserColumnResizingType columnResizingType;

/* Default is NO.  This setting is persistent.  This setting only applies to NSBrowserUserColumnResizing type browsers.  If YES, the browser defaults to resizing all columns simultaneously, otherwise it defaults to single column resizing. Holding down the option key while resizing switches the type of resizing used. 
 */
@property BOOL prefersAllColumnUserResizing;

/* setWidth:ofColumn: does nothing if columnResizingType is NSBrowserAutoColumnResizing.  Otherwise, Sets the width of the specified column.  Due to binary compatibility constraints, you may still set the default width for new columns by passing a columnIndex of -1; you are encouraged to use -setDefaultColumnWidth: instead.  NSBrowserColumnConfigurationDidChangeNotification will be posted (not immediately) if necessary.  The receiver will autosave its column configuration if necessary. 
 */
- (void)setWidth:(CGFloat)columnWidth ofColumn:(NSInteger)columnIndex;
- (CGFloat)widthOfColumn:(NSInteger)column;

/* Get and set the rowHeight. The value must be greater than 0. Calling -setRowHeight: with a non-pixel aligning (fractional) value will be forced to a pixel aligning (integral) value. For variable row height browsers (ones that have the delegate implement -browser:heightOfRow:column:), -rowHeight will be used to draw alternating rows past the last row in each browser column. The default value is 17.0. Note: The rowHeight methods are only valid when using the item delegate methods introduced in Mac OS 10.6. (see NSObject(NSBrowserDelegate)). An exception is thrown if using the older matrix delegate methods 
 */
@property CGFloat rowHeight API_AVAILABLE(macos(10.6));

/* If the delegate implements -browser:heightOfRow:inColumn:, this method immediately re-tiles the browser columns using row heights it provides.
*/
- (void)noteHeightOfRowsWithIndexesChanged:(NSIndexSet *)indexSet inColumn:(NSInteger)columnIndex API_AVAILABLE(macos(10.6));


/* Persistently sets the default width for new columns which don't otherwise have initial width from either defaults or the delegate. This method replaces -setWidth:ofColumn: with a columnIndex of -1. 
 */
- (void)setDefaultColumnWidth:(CGFloat)columnWidth API_AVAILABLE(macos(10.6));
- (CGFloat)defaultColumnWidth API_AVAILABLE(macos(10.6));

/* Sets the name used to automatically save the receivers column configuration.  This setting is persistent.  If name is different from the current name, this method also reads in the saved column configuration for the new name and applies the values to the browser.  Column configuration is defined as an array of column content widths.  One width is saved for each level the user has reached.  That is, browser saves column width based on depth, not based on unique paths.  To do more complex column persistence, you should register for NSBrowserColumnConfigurationDidChangeNotifications and handle persistence yourself. 
 */
@property (copy) NSBrowserColumnsAutosaveName columnsAutosaveName;

/* Removes the column data stored under name from the applications user defaults. 
 */
+ (void)removeSavedColumnsWithAutosaveName:(NSBrowserColumnsAutosaveName)name;


#pragma mark -
#pragma mark **** Drag and Drop Support ****

/* The return value indicates whether the receiver can attempt to initiate a drag for the given event. You can override this method and return NO to disallow initiating drags at the location in the given event. By default, the method will return YES if rowIndexes contains at least one item, and each item is enabled. If the equivalent delegate method is present, it will be preferred over this method, and you can safely call this method from your delegate. 
 */
- (BOOL)canDragRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column withEvent:(NSEvent *)event API_AVAILABLE(macos(10.5));

/* This method computes and returns an image to use for dragging. You can override this to return a custom drag image, or call it to get the default drag image from the delegate method. 'rowIndexes' are the indexes of the cells being dragged in matrix 'column'. 'event' is a reference to the mouse down event that began the drag. 'dragImageOffset' is an in/out parameter. This method will be called with dragImageOffset set to NSZeroPoint, but it can be modified to re-position the returned image. A dragImageOffset of NSZeroPoint will cause the image to be centered under the mouse. By default, an image will be created that contain the visible cells within 'rowIndexes'. If the equivalent delegate method is present, it will be preferred over this method, and you can safely call this method from your delegate. 
 */
- (nullable NSImage *)draggingImageForRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column withEvent:(NSEvent *)event offset:(nullable NSPointPointer)dragImageOffset API_AVAILABLE(macos(10.5));

/* Configures the value returned from -draggingSourceOperationMaskForLocal:. An isLocal value of YES indicates that 'mask' applies when the destination object is in the same application. By default, NSDragOperationEvery will be returned. An isLocal value of NO indicates that 'mask' applies when the destination object in an application outside the receiver's application. By default, NSDragOperationNone is returned. NSBrowser will save the values you set for each isLocal setting. You typically will invoke this method, and not override it. 
 */
- (void)setDraggingSourceOperationMask:(NSDragOperation)mask forLocal:(BOOL)isLocal API_AVAILABLE(macos(10.5));

#pragma mark -

/* Allow type selection in this NSBrowser. The default for 'allowsTypeSelect' is YES.
 */
@property BOOL allowsTypeSelect API_AVAILABLE(macos(10.5));

/* The background color to be drawn. By default, it will be set [NSColor whiteColor]. You can use [NSColor clearColor] to make the background transparent. NSBrowser will return YES from isOpaque if the backgroundColor has an alphaComponent of 1.0 and it doesn't have a title, otherwise, it will return NO. Calling setBackgroundColor: will cause all NSMatrix instances have setDrawsBackground:NO be called in order for the NSBrowser's background color to show through. When drawing with the background color, NSCompositeSourceOver is used for the compositing operation.
 */
@property (strong) NSColor *backgroundColor API_AVAILABLE(macos(10.5));

/* Begins editing the item at the specified path. event may be nil if programatically editing. The cell's contents will be selected if select is YES. Overriding this method will not affect the editing behavior of the browser.
 */
- (void)editItemAtIndexPath:(NSIndexPath *)indexPath withEvent:(nullable NSEvent *)event select:(BOOL)select API_AVAILABLE(macos(10.6));

@end

#pragma mark -
#pragma mark **** Notifications ****

/* The -object in the NSNotification is the browser whose column sizes need to be persisted. There is no userInfo.
 */
APPKIT_EXTERN NSNotificationName NSBrowserColumnConfigurationDidChangeNotification;

#pragma mark -
#pragma mark **** Delegate methods ****

@protocol NSBrowserDelegate <NSObject>
@optional

/* As of Mac OS X 10.6, browser has two different mechanisms for populating columns. You may implement either the matrix or item delegate methods listed below. Many newer features of the browser are only available if you implement the item delegate methods. */

/* Implement one of the following two methods to populate the browser's columns with instances of NSMatrix.
   Note: the matrix based NSBrowser is deprecated in Mac OS 10.10.  New code should use the item based interface.
 */
/* Called by the browser to determine the number of rows in the given column. The delegate will be called with -browser:willDisplayCell:atRow:column: before displaying each cell, giving it a chance to fill in the NSBrowserCell properties.
 */
- (NSInteger)browser:(NSBrowser *)sender numberOfRowsInColumn:(NSInteger)column;

/* Called by the browser to fill in the matrix with populated NSBrowserCells.
 */
- (void)browser:(NSBrowser *)sender createRowsForColumn:(NSInteger)column inMatrix:(NSMatrix *)matrix;

#pragma mark -

/* Alternatively, implement all of the following methods, patterned after NSOutlineView's data source methods. Note that browsers with delegates implementing these methods do not use NSMatrix to populate columns; the implementation is private. The item parameter passed to each of these methods will be nil if the browser is querying about the root of the tree, unless -rootItemForBrowser: is implemented.
*/

/* Return the number of children of the given item. */
- (NSInteger)browser:(NSBrowser *)browser numberOfChildrenOfItem:(nullable id)item API_AVAILABLE(macos(10.6));

/* Return the indexth child of item. You may expect that index is never equal to or greater to the number of children of item as reported by -browser:numberOfChildrenOfItem:.
 */
- (id)browser:(NSBrowser *)browser child:(NSInteger)index ofItem:(nullable id)item API_AVAILABLE(macos(10.6));

/* Return whether item should be shown as a leaf item; that is, an item that can not be expanded into another column. Returning NO does not prevent you from returning 0 from -browser:numberOfChildrenOfItem:.
 */
- (BOOL)browser:(NSBrowser *)browser isLeafItem:(nullable id)item API_AVAILABLE(macos(10.6));

/* Return the object value passed to the cell displaying item.
 */
- (nullable id)browser:(NSBrowser *)browser objectValueForItem:(nullable id)item API_AVAILABLE(macos(10.6));

/* Optional - Variable Row Heights
    Implement this method to support varying row heights per column.  The height returned by this method should not include intercell spacing and must be greater than zero.  NSBrowser may cache the values this method returns.  So if you would like to change a row's height make sure to invalidate the row height by calling -noteHeightOfRowsWithIndexesChanged:inColumn:.
*/
- (CGFloat)browser:(NSBrowser *)browser heightOfRow:(NSInteger)row inColumn:(NSInteger)columnIndex API_AVAILABLE(macos(10.6));

#pragma mark -

/* Optional - Alternate root item. 
 By default, NSBrowser uses 'nil' to identify the root item. It can optionally use a different root item provided by this delegate method. To reload the rootItem that was previously set, call -loadColumnZero, and NSBrowser will call -rootItemForBrowser: again.
 */
- (nullable id)rootItemForBrowser:(NSBrowser *)browser API_AVAILABLE(macos(10.6));

/* Optional - editing support.
 Implement this method to support editing of browser items. The browser will pass back the object value from the cell displaying item.
 */
- (void)browser:(NSBrowser *)browser setObjectValue:(nullable id)object forItem:(nullable id)item API_AVAILABLE(macos(10.6));

/* Optional - editing support.
 Implement this method to control whether the browser may start an editing session for item.
 */
- (BOOL)browser:(NSBrowser *)browser shouldEditItem:(nullable id)item API_AVAILABLE(macos(10.6));

#pragma mark -

/* Optional - display support.
 Called by the browser before displaying a cell at the given location.
 */
- (void)browser:(NSBrowser *)sender willDisplayCell:(id)cell atRow:(NSInteger)row column:(NSInteger)column;

- (nullable NSString *)browser:(NSBrowser *)sender titleOfColumn:(NSInteger)column;

/* Called by the browser when selecting cells by title, either from -setPath:, or when recomputing the selection after -reloadColumn:. Return NO if a matching cell could not be found.
 */
- (BOOL)browser:(NSBrowser *)sender selectCellWithString:(NSString *)title inColumn:(NSInteger)column;

/* Called by the browser when selecting a cell using -selectRow:inColumn:. Return NO if a matching cell could not be found.
 */
- (BOOL)browser:(NSBrowser *)sender selectRow:(NSInteger)row inColumn:(NSInteger)column;

/* Called by the browser from -validateVisibleColumns to determine whether the currently-displayed columns are valid. Returning NO will prompt the browser to call -reloadColumn:.
 */
- (BOOL)browser:(NSBrowser *)sender isColumnValid:(NSInteger)column;

- (void)browserWillScroll:(NSBrowser *)sender;
- (void)browserDidScroll:(NSBrowser *)sender;

/*
 * Delegate methods used by resizable column browsers.
 */

/* Optional for browsers with resize type NSBrowserNoColumnResizing, and NSBrowserUserColumnResizing.
 This method is used for both constraining column resize, and determining a columns initial size.  If 'forUserResize' is NO, this method should return the initial width for a newly added column.  If 'forUserResize' is YES, this method can be used to constrain resizing on a per-column basis.  (Currently per-column constraining is only implemented for single column resize).
 */
- (CGFloat)browser:(NSBrowser *)browser shouldSizeColumn:(NSInteger)columnIndex forUserResize:(BOOL)forUserResize toWidth:(CGFloat)suggestedWidth;

/* Optional for browsers with resize type NSBrowserUserColumnResizing only.
 This method returns the "ideal" width for a column.  This method is used when performing a "right-size-each" or "right-size-one" operation.  If columnIndex is -1, the result is used for a "right-size-all-simultaneous" operation.  In this case, you should return a best uniform right size for all column (every column will be set to this size).  You can opt out on a per column basis by returning -1 for that column. It is assumed that the implementation may be expensive, so it will be called only when necessary.  (See documentation for definitions of right-size one/each/all). 
 */
- (CGFloat)browser:(NSBrowser *)browser sizeToFitWidthOfColumn:(NSInteger)columnIndex;

/* Optional and used for browsers with resize type NSBrowserUserColumnResizing only.
 This method is intended to be used by clients wishing to implement their own column width persistence.  It is called when the width of any browser columns has changed.  User column resizing will cause a single notification to be posted when the user is finished resizing.  (See NSBrowserColumnConfigurationDidChangeNotification for more information.)  
 */
- (void)browserColumnConfigurationDidChange:(NSNotification *)notification;

/* Optional - Expansion ToolTip support.
    Implement this method and return NO to prevent an expansion tooltip from appearing for a particular cell at 'row' in 'column'. See NSCell.h for more information on expansion tool tips. 
 */
- (BOOL)browser:(NSBrowser *)browser shouldShowCellExpansionForRow:(NSInteger)row column:(NSInteger)column API_AVAILABLE(macos(10.5));


#pragma mark -
#pragma mark **** Optional Drag and Drop Support Methods ****

#pragma mark ** Dragging Source Methods **

/* This method is called after it has been determined that a drag should begin, but before the drag has been started.  To refuse the drag, return NO. To start a drag, declared the pasteboard types that you support with [pasteboard declareTypes:owner:], place your data on the pasteboard, and return YES from the method. The drag image and other drag related information will be set up and provided by the view once this call returns with YES. You need to implement this method for your browser to be a drag source. 
 */
- (BOOL)browser:(NSBrowser *)browser writeRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column toPasteboard:(NSPasteboard *)pasteboard API_AVAILABLE(macos(10.5));

/* The delegate can support file promise drags by adding NSFilesPromisePboardType to the pasteboard in browser:writeRowsWithIndexes:inColumn:toPasteboard:. NSBrowser implements -namesOfPromisedFilesDroppedAtDestination: to return the results of this data source method.  This method should returns an array of filenames for the created files (filenames only, not full paths).  The URL represents the drop location.  For more information on file promise dragging, see documentation on the NSDraggingSource protocol and -namesOfPromisedFilesDroppedAtDestination:. You do not need to implement this method for your browser to be a drag source.
 */
- (NSArray<NSString *> *)browser:(NSBrowser *)browser namesOfPromisedFilesDroppedAtDestination:(NSURL *)dropDestination forDraggedRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column API_DEPRECATED("Use NSFilePromiseReceiver objects instead", macos(10.5,10.13));

/* The delegate can control if some particular rows can be dragged or not for a particular event. You do not need to implement this method for your browser to be a drag source. 
 */
- (BOOL)browser:(NSBrowser *)browser canDragRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column withEvent:(NSEvent *)event API_AVAILABLE(macos(10.5));

/* Allows the delegate to compute a dragging image for the particular cells being dragged. 'rowIndexes' are the indexes of the cells being dragged in the matrix in 'column'. 'event' is a reference to the mouse down event that began the drag. 'dragImageOffset' is an in/out parameter. This method will be called with dragImageOffset set to NSZeroPoint, but it can be modified to re-position the returned image.  A dragImageOffset of NSZeroPoint will cause the image to be centered under the mouse. You can safely call [browser dragImageForRowsWithIndexes:inColumn:withEvent:offset:] from inside this method. You do not need to implement this method for your browser to be a drag source. You can safely call the corresponding NSBrowser method.
 */
- (nullable NSImage *)browser:(NSBrowser *)browser draggingImageForRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column withEvent:(NSEvent *)event offset:(NSPointPointer)dragImageOffset API_AVAILABLE(macos(10.5));

#pragma mark ** Dragging Destination Methods **

/* This method is used by the browser to determine a valid drop target. Based on the mouse position, the browser will suggest a proposed row, column and dropOperation. These values are in/out parameters and can be changed by the delegate to retarget the drop operation. The dropOperation suggested will be NSBrowserDropOn if the drag location is closer to the middle of a row. If it is closer to being between two rows, it will be NSBrowserDropAbove, indicating a drop above 'row' and below 'row - 1'. This method must return a value that indicates which dragging operation the data source will perform. It must return something other than NSDragOperationNone to accept the drop.

    To retarget a drop and specify a drop on row 2 (with the first visual row being row 0), one would set *row=2, and set *dropOperation=NSBrowserDropOn. To specify a drop below the last row, one would specify *row=[browser numberOfRows], and *dropOperation=NSBrowserDropAbove. To specify a drop on the entire column, you will specify *row=-1, and *dropOperation=NSBrowserDropOn. To specify a drop between rows 2 and 3, one would set *row=3 and *dropOperation=NSBrowserDropAbove.

    Note: to receive drag messages, you must first call [NSBrowser registerForDraggedTypes:] for the drag types you want to support (typically this is done in awakeFromNib). You must implement this method for your browser to be a drag destination. 
*/
- (NSDragOperation)browser:(NSBrowser *)browser validateDrop:(id <NSDraggingInfo>)info proposedRow:(NSInteger *)row column:(NSInteger *)column  dropOperation:(NSBrowserDropOperation *)dropOperation API_AVAILABLE(macos(10.5));

/* This method is called when the mouse is released over a browser that previously decided to allow a drop via the above validateDrop method.  The delegate should incorporate the data from the dragging pasteboard at this time. You must implement this method for your browser to be a drag destination.
*/
- (BOOL)browser:(NSBrowser *)browser acceptDrop:(id <NSDraggingInfo>)info atRow:(NSInteger)row column:(NSInteger)column dropOperation:(NSBrowserDropOperation)dropOperation API_AVAILABLE(macos(10.5));

#pragma mark -

#pragma mark **** Type Select Support ****

/* Optional - Type select support
    Implement this method if you want to control the string that is used for type selection. You may want to change what is searched for based on what is displayed, or simply return an empty string for that row and/or column to not be searched. You can also return 'nil' if the cell does not contain any text. By default, all cells with text in them are searched. The default value when this delegate method is not implemented is the stringValue for the cell at that location.
 */
- (nullable NSString *)browser:(NSBrowser *)browser typeSelectStringForRow:(NSInteger)row inColumn:(NSInteger)column API_AVAILABLE(macos(10.5));

/* Optional - Type select support
    Implement this method if you would like to prevent a type select from happening based on the current event and current search string. Generally, this will be called from keyDown: and the event will be a key event. The search string will be nil if no type select has began. 
 */
- (BOOL)browser:(NSBrowser *)browser shouldTypeSelectForEvent:(NSEvent *)event withCurrentSearchString:(nullable NSString *)searchString API_AVAILABLE(macos(10.5));

/* Optional - Type select support
    Implement this method if you want to control how type selection works. Return the first row that matches searchString from within the range of startRow to endRow. It is possible for endRow to be less than startRow if the search will wrap. Return -1 when there is no match. Include startRow as a possible match, but do not include endRow. It is not necessary to implement this method in order to support type select.
 */
- (NSInteger)browser:(NSBrowser *)browser nextTypeSelectMatchFromRow:(NSInteger)startRow toRow:(NSInteger)endRow inColumn:(NSInteger)column forString:(nullable NSString *)searchString API_AVAILABLE(macos(10.5));

#pragma mark -

/* Optional - Preview column support
    Implement this method to provide a preview column for leaf items. Return nil to suppress the preview column. The controller's representedObject will be set to the browser's selected leaf item. This method is only called if the delegate implements the item data source methods.
 */
- (nullable NSViewController *)browser:(NSBrowser *)browser previewViewControllerForLeafItem:(id)item API_AVAILABLE(macos(10.6));

/* Optional - Column header support
    Implement this method to provide a header view for columns. Return nil to omit the header view. The controller's representedObject will be set to the column's item. This method is only called if the delegate implements the item data source methods.
 */
- (nullable NSViewController *)browser:(NSBrowser *)browser headerViewControllerForItem:(nullable id)item API_AVAILABLE(macos(10.6));

/* Optional - Notification when the lastColumn changes.
 */
- (void)browser:(NSBrowser *)browser didChangeLastColumn:(NSInteger)oldLastColumn toColumn:(NSInteger)column;

/* Optional - Return a set of new indexes to select when the user changes the selection with the keyboard or mouse. This method may be called multiple times with one new index added to the existing selection to find out if a particular index can be selected when the user is extending the selection with the keyboard or mouse. Note that 'proposedSelectionIndexes' will contain the entire newly suggested selection, and you can return the exsiting selection to avoid changing the selection. This method only works for item-based NSBrowsers.
 */
- (NSIndexSet *)browser:(NSBrowser *)browser selectionIndexesForProposedSelection:(NSIndexSet *)proposedSelectionIndexes inColumn:(NSInteger)column API_AVAILABLE(macos(10.6));

@end

#pragma mark -
#pragma mark **** Deprecated Methods ****

@interface NSBrowser(NSDeprecated)

/* This property has been non-functional since before 10.0.
 */
- (void)setAcceptsArrowKeys:(BOOL)flag API_DEPRECATED("", macos(10.0,10.6));
- (BOOL)acceptsArrowKeys API_DEPRECATED("", macos(10.0,10.6));

/* Use of -displayColumn: is deprecated in 10.3.  Use setNeedsDisplayInRect: instead.
 */
- (void)displayColumn:(NSInteger)column API_DEPRECATED("", macos(10.0,10.3));

/* Use of -displayAllColumns; is deprecated in 10.3.  Use setNeedsDisplay/InRect: instead.
 */
- (void)displayAllColumns API_DEPRECATED("", macos(10.0,10.3));

/* Use of -scrollViaScroller: is deprecated in 10.3.  Continuous scrolling no longer requires this functionality.
 */
- (void)scrollViaScroller:(null_unspecified NSScroller *)sender API_DEPRECATED("", macos(10.0,10.3));

/* Use of -updateScroller is deprecated in 10.3.  Continuous scrolling no longer requires this functionality.
 */
- (void)updateScroller API_DEPRECATED("", macos(10.0,10.3));

/* Matrix based NSBrowser is deprecated in 10.10.
 */
- (void)setMatrixClass:(Class)factoryId  API_DEPRECATED("Use the item based NSBrowser instead", macos(10.0,10.10));
- (Class)matrixClass  API_DEPRECATED("Use the item based NSBrowser instead", macos(10.0,10.10));
- (NSInteger)columnOfMatrix:(NSMatrix *)matrix  API_DEPRECATED("Use the item based NSBrowser instead", macos(10.0,10.10));
- (nullable NSMatrix *)matrixInColumn:(NSInteger)column  API_DEPRECATED("Use the item based NSBrowser instead", macos(10.0,10.10));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
