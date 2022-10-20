/*
    NSPathCell.h
    Application Kit
    Copyright (c) 2005-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/NSActionCell.h>
#import <AppKit/NSSavePanel.h>
#import <AppKit/NSMenu.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString, NSImage, NSAnimation, NSPathComponentCell, NSURL, NSPopUpButtonCell, NSNotification, NSOpenPanel;
@protocol NSPathCellDelegate;

/* 
    NSPathCell
 
    This cell maintains a collection of NSPathComponentCells that represent a particular path to be displayed to the user. The path shown can be set by calling -setURL:. Doing so will remove all displayed NSPathComponentCells and automatically fill it up with NSPathComponentCells set to have the appropriate icons, display titles, and NSURL values for the particular path component they represent. One can manually fill up the control by setting the cell array or directly modifying existing cells. 
    
    Both an action and doubleAction can be set for the control. To find out what path component cell was clicked upon in the action, you can access the 'clickedPathComponentCell'. When the style is set to NSPathStylePopUp, the action is still sent, and the 'clickedPathComponentCell' for the represented menu item is correctly set. The 'clickedPathComponentCell' is only valid when the action is being sent. It will also be valid when the keyboard is used to invoke the action.
 
    Automatic animated expansion of partially hidden NSPathComponentCells will happen if you correctly call mouseEntered: and mouseExited: for each NSPathComponentCell in the NSPathCell. This is not required if the pathStyle is set to NSPathStylePopUp, or if you wish to not have the animation.
 
    The NSPathCell supports several path display styles. The NSPathStyleStandard has a light blue background with arrows indicating the path. The NSPathStylePopUp will look and work like an NSPopUpButton to display the full path, or select a new path, if the cell is editable.
 
    If the cell isEditable (the default is YES), one can drag and drop into the cell to change the value. You can constrain what can be dropped using UTIs (Uniform Type Identifier) in the allowedTypes, or the appropriate delegate methods on NSPathControl.
 
    If the cell isSelectable (the default is YES), the cell's contents can automatically be draged out. The proper UTI and filename and URL will be placed on the pasteboard. You can further control or limit this by using the appropriate delegate methods on NSPathControl.
 
    If the cell isEditable and has the pathStyle set to NSPathStylePopUp, an additional item in the pop up menu will allow selecting another location. By default, an NSOpenPanel will be configured based on the allowedTypes. The NSOpenPanel that is used can be customized with a delegate method.
*/

typedef NS_ENUM(NSInteger, NSPathStyle) {
/* The standard display style and behavior. All path component cells are displayed with an image/icon and path component name. If the path can not fully be displayed, the middle parts are truncated as required..
*/
    NSPathStyleStandard,
    
/* The pop up display style and behavior. Only the last path component is displayed with an image/icon and path component. The full path is shown when you click on the cell. If the cell isEditable, an option will be shown to select a different path.
*/
    NSPathStylePopUp = 2,
    
/* This style is deprecated as of Mac OS X 10.7. Please use one of the other NSPathStyle values.
 */
    NSPathStyleNavigationBar API_DEPRECATED("", macos(10.5,10.7))= 1
};


API_AVAILABLE(macos(10.5))
@interface NSPathCell : NSActionCell <NSMenuItemValidation, NSOpenSavePanelDelegate>
/* See NSPathControl for documentation on all the properties listed below. The NSPathControl directly calls the cell's methods.
 */
@property NSPathStyle pathStyle;

@property (nullable, copy) NSURL *URL;

/* If setObjectValue is called with an NSURL, setURL is automatically called. Calling objectValue will return the last set URL value. setObjectValue can also take a string value, with the items separated by the path separator. Any other value is a programming error and will cause an assertion.
*/
- (void)setObjectValue:(nullable id <NSCopying>)obj;

@property (nullable, copy) NSArray<NSString *> *allowedTypes;

@property (nullable, weak) id<NSPathCellDelegate> delegate;

/* Returns the class used to create pathComponentCells when automatically filling up the control. Subclassers can override this method to return a custom cell class that will automatically be used. By default, it will return [NSPathComponentCell class], or a specialized subclass thereof.
*/
@property (class, readonly) Class pathComponentCellClass;
    
@property (copy) NSArray<__kindof NSPathComponentCell *>  *pathComponentCells;

/* Returns the current rect being displayed for a particular NSPathComponentCell 'cell', with respect to the 'frame' in 'view'. NSZeroRect will be returned if 'cell' is not found, or it currently is not visible. 
*/
- (NSRect)rectOfPathComponentCell:(NSPathComponentCell *)cell withFrame:(NSRect)frame inView:(NSView *)view;

/* Returns the cell located at 'point' within 'frame' of 'view', or nil, if none exists at that location.
*/
- (nullable NSPathComponentCell *)pathComponentCellAtPoint:(NSPoint)point withFrame:(NSRect)frame inView:(NSView *)view;

/* Returns the clicked cell, or nil, if a no cell has been clicked. The clickedPathComponentCell is generally only valid when the action or doubleAction is being sent.
*/
@property (nullable, readonly, strong) NSPathComponentCell *clickedPathComponentCell;

/* The NSPathCell will dynamically animate to display the component that the mouse is hovering over by using mouseEntered and mouseExited events. The control should call these methods to correctly display the hovered component to the user. The control can acquire rects to track using rectOfPathComponentCell:withFrame:inView:.
*/
- (void)mouseEntered:(NSEvent *)event withFrame:(NSRect)frame inView:(NSView *)view;
- (void)mouseExited:(NSEvent *)event withFrame:(NSRect)frame inView:(NSView *)view;

/* Allows you to set the selector that will be called when the user double clicks on a particular NSPathComponentCell. 
*/
@property (nullable) SEL doubleAction;

/* The background color to be drawn. By default, it will be set to a light blue color for NSPathStyleStandard, and nil for everything else. You can use [NSColor clearColor] to make the background transparent. NSPathCell will return YES from isOpaque if the backgroundColor has an alphaComponent of 1.0, otherwise, it will return NO. When drawing with the background color, NSCompositeSourceOver is used for the compositing operation.
*/
@property (nullable, copy) NSColor *backgroundColor;

@property (nullable, copy) NSString *placeholderString;

@property (nullable, copy) NSAttributedString *placeholderAttributedString;

/* The NSPathCell properly respects the controlSize for the NSPathStyleStandard and NSPathStylePopUp styles. When the controlSize is set, the new size is propigated to the subcells. */

@end

@protocol NSPathCellDelegate <NSObject>

@optional

/* Called before the 'openPanel' is shown, but after the 'openPanel' has allowedFileTypes set to the cell's allowedTypes. At this time, you can further customize the 'openPanel' as required. This is only ever called when the style is set to NSPathStylePopUp.
*/
- (void)pathCell:(NSPathCell *)pathCell willDisplayOpenPanel:(NSOpenPanel *)openPanel;

/* Called before the 'menu' is shown. At this time, you can further customize the 'menu' as required, adding and removing items. This is only ever called when the style is set to NSPathStylePopUp.
*/
- (void)pathCell:(NSPathCell *)pathCell willPopUpMenu:(NSMenu *)menu;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END


