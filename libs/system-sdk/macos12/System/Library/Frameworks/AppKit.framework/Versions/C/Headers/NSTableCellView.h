/*
    NSTableCellView.h
    Application Kit
    Copyright (c) 2009-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSNibDeclarations.h>
#import <AppKit/NSView.h>
#import <AppKit/NSCell.h>
#import <AppKit/NSTableView.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSTextField, NSImageView, NSDraggingImageComponent;

/* View Based NSTableView: The NSTableCellView is a reusable container view shown for a particular cell in the table. See NSTableView.h for more information. 
 */
API_AVAILABLE(macos(10.7))
@interface NSTableCellView : NSView

/* The 'objectValue' is automatically set by the table when using bindings, or the result from the dataSource method -tableView:objectValueForTableColumn:row:. Key Value Observing (KVO) compliant so user interface elements can be bound to the 'objectValue'. 
 */
@property (nullable, strong) id objectValue;

/* The following two properties are provided for the developer to hook up as outlets. These outlets can then be accessed in -tableView:viewForTableColumn:row: and modified. Additional outlets can be added by subclassing NSTableCellView, adding appropriate ivars and properties, and hooking it up in Interface Builder.
 */
@property (nullable, assign) IBOutlet NSTextField *textField;
@property (nullable, assign) IBOutlet NSImageView *imageView;

/* The backgroundStyle property is automatically set by the enclosing NSTableRowView to let this view know what its background looks like. For instance, when the -backgroundStyle is NSBackgroundStyleEmphasized, the view should use a contrasting text color. The system label colors (labelColor, secondaryLabelColor, tertiaryLabelColor, and quaternaryLabelColor) automatically adapt when displayed with an emphasized backgroundStyle. Upon setting, the default implementation automatically forwards calls to all subviews that implement -setBackgroundStyle: or are an NSControl (which have NSCells that respond to -setBackgroundStyle:).
 */
@property NSBackgroundStyle backgroundStyle;

/* The rowSizeStyle is set by the NSTableView to its -effectiveRowSizeStyle. The cell view will layout the textField and imageView based on the rowSizeStyle. A value of NSTableViewRowSizeStyleDefault should never be set on the cell view, as it is an appropriate value only for the table. See NSTableView.h for more information. 
 */
@property NSTableViewRowSizeStyle rowSizeStyle;

/* Multi-Item Drag Support. The default implementation will return an array of up to two NSDraggingImageComponent instances -- one for the imageView and another for the textField (if not nil). These methods can be subclassed and overridden to provide a custom set of NSDraggingImageComponents to create the drag image from this view.
 */
@property (readonly, strong) NSArray<NSDraggingImageComponent *> *draggingImageComponents;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
