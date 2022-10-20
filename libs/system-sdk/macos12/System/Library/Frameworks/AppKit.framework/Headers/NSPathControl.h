/*
    NSPathControl.h
    Application Kit
    Copyright (c) 2005-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/NSControl.h>
#import <AppKit/NSPathCell.h>
#import <AppKit/NSDragging.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSPathComponentCell, NSOpenPanel, NSPathControlItem;
@protocol NSPathControlDelegate;

/* 
    NSPathControl

 This control maintains a collection of NSPathControlItems that represent a particular path to be displayed to the user. The path shown can be set by calling -setURL:. Doing so will remove all displayed NSPathControlItems and automatically fill it up with items set to have the appropriate icons, display titles, and NSURL values for the particular path component they represent. One can manually fill up the control by setting the array or directly modifying existing item objects.
 
 Both an action and doubleAction can be set for the control. To find out what path item was clicked upon in the action, you can access the 'clickedPathItem'. When the style is set to NSPathStylePopUp, the action is still sent, and the 'clickedPathItem' for the represented menu item is correctly set. The 'clickedPathItem' is only valid when the action is being sent. It will also be valid when the keyboard is used to invoke the action.
 
 To accept drag and drop, the NSPathControl automatically calls registerForDraggedTypes: with NSFilenamesPboardType and NSURLPboardType.
 
 When the URL value in the NSPathControl changes from an automatic drag and drop operation, or from the user selecting a new path via the open panel, the action is sent, but the clickedPathItem will be nil.
 
 The NSPathControl supports several path display styles. The NSPathStyleStandard has a light blue background with arrows indicating the path. The NSPathStylePopUp will look and work like an NSPopUpButton to display the full path, or select a new path, if the control is editable.
 
 If the control isEditable (the default is YES), one can drag and drop into the control to change the value. You can constrain what can be dropped using UTIs (Uniform Type Identifier) in the allowedTypes, or the appropriate delegate methods on NSPathControl.
 
 If the control isSelectable (the default is YES), the control's contents can automatically be dragged out. The proper UTI and filename and URL will be placed on the pasteboard. You can further control or limit this by using the appropriate delegate methods on NSPathControl.
 
 If the control isEditable and has the pathStyle set to NSPathStylePopUp, an additional choice in the pop up menu will allow selecting another location. By default, an NSOpenPanel will be configured based on the allowedTypes. The NSOpenPanel that is used can be customized with a delegate method.

 */
API_AVAILABLE(macos(10.5))
@interface NSPathControl : NSControl

@property(getter=isEditable) BOOL editable API_AVAILABLE(macos(10.10));

/* Specifies the allowed types when the control isEditable. The allowedTypes can contain a file extension (without the period that begins the extension) or UTI (Uniform Type Identifier). To allow folders, include the UTI 'public.folder'. To allow all types, use 'nil'. If allowedTypes is an empty array, nothing will be allowed. The default value is 'nil', allowing all types.
 */
@property(nullable, copy) NSArray<NSString *> *allowedTypes API_AVAILABLE(macos(10.10));


/* If there are no components in the path, the placeholder is drawn instead (if non-nil).  We first try to draw the placeholderAttributedString. If it's nil, then the placeholderString will be drawn with correct default attributes.
 */
@property(nullable, copy) NSString *placeholderString API_AVAILABLE(macos(10.10));
@property(nullable, copy) NSAttributedString *placeholderAttributedString API_AVAILABLE(macos(10.10));

/* Gets and sets the path value displayed. When setting, an array of NSPathControlItems will automatically be set based on the path in the 'url'. If the 'url' is a file URL (returns YES from isFileURL), the images will automatically be filled up with file icons, if the path exists.  The URL value itself is stored in the objectValue of the control.
 */
@property (nullable, copy) NSURL *URL;

/* The selector that will be called when the user double clicks on a NSPathControlItem.
 */
@property (nullable) SEL doubleAction;

/* The style/mode of the NSPathControl. Defaults to NSPathStyleStandard.
 */
@property NSPathStyle pathStyle;

/* The clicked NSPathControlItem, or nil, if no item has been clicked. The clickedPathItem is generally only valid while the action or doubleAction is being sent.
 */
@property (nullable, readonly) NSPathControlItem *clickedPathItem API_AVAILABLE(macos(10.10));

/* The array of NSPathControlItems currently being displayed.
    Each item must be an NSPathControlItem.  Do not subclass NSPathControlItem. You cannot set this value to nil, but should instead set it to an empty array.
 */
@property (copy) NSArray<NSPathControlItem *> *pathItems API_AVAILABLE(macos(10.10));

/* The background color to be drawn.  By default, it will be set to a light blue color for NSPathStyleStandard, and nil for everything else. You can use [NSColor clearColor] to make the background transparent.
 */
@property (nullable, copy) NSColor *backgroundColor;

/* The delegate for this control. The delegate is weakly referenced for zeroing-weak compatible objects in apps linked on 10.10 or later. Otherwise the behavior of this property is 'assign'.
*/
@property (nullable, weak) id <NSPathControlDelegate> delegate;

/* Configures the default value returned from -draggingSourceOperationMaskForLocal:. An 'isLocal' value of YES indicates that 'mask' applies when the destination object is in the same application. An 'isLocal' value of NO indicates that 'mask' applies when the destination object in a destination object outside the receiver's application. By default, -draggingSourceOperationMaskForLocal: returns NSDragOperationEvery when 'isLocal' is YES, and NSDragOperationNone when 'isLocal' is NO.
*/
- (void)setDraggingSourceOperationMask:(NSDragOperation)mask forLocal:(BOOL)isLocal;

/* The menu used when the syle is NSPathStylePopUp.
 */
@property (nullable, strong) NSMenu *menu;

@end


@protocol NSPathControlDelegate <NSObject>

@optional

/* Optional dragging source support. These methods are called when a drag is about to begin. One of these two methods must be implemented to allow dragging from the control. You can refuse to allow the drag to happen by returning NO, and allow it by returning YES. By default, 'pasteboard' automatically has the following types on it: NSStringPboardType, NSURLPboardType (if there is a URL value for the item being dragged), and NSFilenamesPboardType (if the URL value returns YES from -isFileURL). You can customize the types placed on the pasteboard at this time, if desired.
*/
- (BOOL)pathControl:(NSPathControl *)pathControl shouldDragItem:(NSPathControlItem *)pathItem withPasteboard:(NSPasteboard *)pasteboard;
- (BOOL)pathControl:(NSPathControl *)pathControl shouldDragPathComponentCell:(NSPathComponentCell *)pathComponentCell withPasteboard:(NSPasteboard *)pasteboard;

/* Optional dragging destination support. This method is called when something is dragged over the control. This method can be implemented to allow dragging onto the control. The delegate method will be called even for instances that are "disabled" (ie: return NO from -isEnabled). Return NSDragOperationNone to refuse the drop, or anything else to accept it.

    If not implemented, and the control isEnabled and isEditable, the drop will be accepted if the pasteboard contains NSURLPboardType or NSFilenamesPboardType, and if the type of the referenced file conforms to the allowedTypes.
*/
- (NSDragOperation)pathControl:(NSPathControl *)pathControl validateDrop:(id <NSDraggingInfo>)info;

/* Optional dragging destination support. In order to accept the dropped contents previously accepted from validateDrop:, you must implement this method. This method is called from -performDragOperation:. You should change the URL value based on the dragged information.

    If not implemented, and the control isEditable, the drop will be accepted if the pasteboard contains NSURLPboardType or NSFilenamesPboardType, and if the type of the referenced file conforms to the allowedTypes. The control's URL value will automatically be changed, and the action will be invoked.

*/
- (BOOL)pathControl:(NSPathControl *)pathControl acceptDrop:(id <NSDraggingInfo>)info;

/* Called before the 'openPanel' is shown, but after the 'openPanel' has allowedFileTypes set to the control's allowedTypes. At this time, you can further customize the 'openPanel' as required. This is only ever called when the style is set to NSPathStylePopUp.
*/
- (void)pathControl:(NSPathControl *)pathControl willDisplayOpenPanel:(NSOpenPanel *)openPanel;

/* Called before the 'menu' is shown. At this time, you can further customize the 'menu' as required, adding and removing items. This is only ever called when the style is set to NSPathStylePopUp.
*/
- (void)pathControl:(NSPathControl *)pathControl willPopUpMenu:(NSMenu *)menu;

@end


@interface NSPathControl (NSDeprecated)

- (nullable NSPathComponentCell *)clickedPathComponentCell API_DEPRECATED("Use -clickedPathItem instead", macos(10.0,10.14));
- (NSArray<NSPathComponentCell *> *)pathComponentCells API_DEPRECATED("Use -pathItems instead", macos(10.0,10.14));
- (void)setPathComponentCells:(NSArray<NSPathComponentCell *> *)cells API_DEPRECATED("Use -setPathItems: instead", macos(10.0,10.14));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
