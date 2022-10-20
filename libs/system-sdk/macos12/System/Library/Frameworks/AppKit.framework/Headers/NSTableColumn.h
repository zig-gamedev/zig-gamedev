/*
    NSTableColumn.h
    Application Kit
    Copyright (c) 1995-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/NSUserInterfaceItemIdentification.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSTableView, NSCell, NSImage, NSSortDescriptor, NSTableHeaderCell;

/* The resizing mask controls the resizability of a table column. Values can be combined together with a bitwise or. The default value is 'NSTableColumnUserResizingMask | NSTableColumnAutoresizingMask'. Compatability note: This method replaces setResizable:.
 */
typedef NS_OPTIONS(NSUInteger, NSTableColumnResizingOptions) {
    NSTableColumnNoResizing = 0, // Disallow any kind of resizing.
    NSTableColumnAutoresizingMask = ( 1 << 0 ), // This column can be resized as the table is resized.
    NSTableColumnUserResizingMask = ( 1 << 1 ), // The user can resize this column manually.
};

@interface NSTableColumn : NSObject <NSCoding, NSUserInterfaceItemIdentification>

/* Designated initializer for NSTableColumns. Prior to 10.7, the parameter type was 'id', but it is now an 'NSString *'. See also -setIdentifier: and -identifier, and NSUserInterfaceItemIdentification.
 */
- (instancetype)initWithIdentifier:(NSUserInterfaceItemIdentifier)identifier NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* Gets and sets the string identifier associated with the NSTableColumn. 'identifier' will be copied. Prior to 10.7, the type was 'id', but was changed to 'NSString *' for NSUserInterfaceItemIdentification.
 */
@property (copy) NSUserInterfaceItemIdentifier identifier;

@property (nullable, weak) NSTableView *tableView;

/* Gets and sets the current width of the NSTableColumn. The default for is 100.0. Calling setWidth: with a width that is less than the minWidth or larger than the maxWidth will automatically be constrained to minWidth or maxWidth as appropriate.
 */
@property CGFloat width;

/* Gets and sets the minimum width of the NSTableColumn. The default for is 10.0. If -[self width] is less than the 'minWidth' value when -setMinWidth: is called, the width will automatically be increased to the minWidth and [tableView tile] will be called.
   Note that if the NSTableColumn is the outlineTableColumn of an NSOutlineView, minWidth will return
    the maximum of the value set and NSOutlineView.indentationPerLevel.
 */
@property CGFloat minWidth;

/* Gets and sets the maximum width of the NSTableColumn. The default for is MAXFLOAT on Leopard and higher. If -[self width] is greater than the 'maxWidth' value when -setMaxWidth: is called, the width will automatically be decreased to the maxWidth and [tableView tile] will be called.
 */
@property CGFloat maxWidth;

/* Provides access to the title of this column header.
 */
@property (copy) NSString *title;

/* Gets and sets the headerCell associated with this NSTableColumn. 'cell' must be non nil, and should be a member of the NSTableHeaderCell class. The 'headerCell' is a strong reference, and will be retained.
 */
@property (strong) __kindof NSTableHeaderCell *headerCell;

/* Gets and sets the editability state of this NSTableColumn. The default value is YES. When an NSTableView wants to start editing a dataCell, the following happens: 1. Check [tableColumn isEditable]. If that returns YES, check the (optional) delegate method with tableView:shouldEditTableColumn:row:. If that returns YES, check the [dataCell isEnabled] and [isEnabled isSelectable] states before attempting to edit.
 */
@property (getter=isEditable) BOOL editable;

- (void)sizeToFit;

/* A column is considered sortable if it has a sortDescriptorPrototype. This prototype defines several things about the columns sorting. The prototype's ascending value defines the default sorting direction. Its key defines an arbitrary attribute which helps clients identify what to sort, while the selector defines how to sort. Note that, it is not required that the key be the same as the identifier. However, the key must be unique from the key used by other columns. The sortDescriptor is archived.
 */
@property (nullable, copy) NSSortDescriptor *sortDescriptorPrototype;

@property NSTableColumnResizingOptions resizingMask;

/* Get and set the Tool Tip for the NSTableColumn header that appears when hovering the mouse over the header for the NSTableColumn. The default value is 'nil', meaning there is no headerToolTip.
*/
@property (nullable, copy) NSString *headerToolTip API_AVAILABLE(macos(10.5));

/* Determines if the column is hidden or not. The isHidden value is stored out when the NSTableView automatically saves out NSTableColumn state. Note that columns which are hidden still exist in the the -[NSTableView tableColumns] array and -[NSTableView numberOfColumns] includes columns which are hidden. The default value is NO, meaning the NSTableColumn will not be hidden.
*/
@property (getter=isHidden) BOOL hidden API_AVAILABLE(macos(10.5));

@end

/*
 * Deprecated Methods
 */

@interface NSTableColumn(NSDeprecated)

/* Deprecated in Mac OS 10.4.  If flag is YES, calls setResizingMask:(NSTableColumnUserResizingMask | NSTableColumnAutoresizingMask).  If flag is NO, calls setResizingMask:(NSTableColumnNoResizing).
 */
- (void)setResizable:(BOOL)flag API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)isResizable API_DEPRECATED("", macos(10.0,10.4));

/* Cell-based NSTableViews are deprecated in Mac OS 10.10.  Use view-based NSTableViews instead.
 */
@property (strong) id /* NSCell * */ dataCell; // Use a view-based NSTableView, and see the -tableView:viewForTableColumn:row: delegate method and -viewAtColumn:row:makeIfNecessary:
- (id)dataCellForRow:(NSInteger)row; // Use a view-based NSTableView, and see -viewAtColumn:row:makeIfNecessary

@end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
