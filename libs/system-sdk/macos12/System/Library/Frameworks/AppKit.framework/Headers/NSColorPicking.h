/*
    NSColorPicking.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/


#import <Foundation/NSObject.h>
#import <AppKit/NSColorPanel.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSButtonCell, NSColor, NSColorPanel, NSColorList, NSImage, NSView;

@protocol NSColorPickingDefault

/* 
    The following methods are implemented by the generic base class, NSColorPicker, and only need be implemented (overridden) by the CustomPicker developer if there is a need.
*/

/* 
    initWithPickerMask:colorPanel: allows you to return your custom color picker to the ColorPanel.

    "mask" is the mask (if any) passed to the ColorPanel by the +setPickerMask: method.  If your picker supports any of the bits in the mask, return "self", otherwise, return "nil" (default is "self" if not overridden, since if the "setPickerMask" method is never called, the ColorPanel will just start off with the normal picker modes, of which your custom mode will not be a part). This method can be used to turn off some (or all) of your subpickers, if you have any (like sliders). If this method returns "nil", the object will be freed.
 
    "owningColorPanel" is the id of the instantiating ColorPanel.  
 
    If this method is overridden, the base class' (NSColorPicker's) initFromPickerMask method should be called before any subclass processing. The instance variable "colorPanel" is set in this method.
 
    FOR PERFORMANCE, DO NOT LOAD .NIBS, ETC. HERE!! WAIT UNTIL provideNewView: in NSColorPickingCustom!
*/

- (nullable instancetype)initWithPickerMask:(NSUInteger)mask colorPanel:(NSColorPanel *)owningColorPanel;

/* Allows you to return a new image used in the toolbar. By default, you can simply have a .tiff file with the same name as your custom color picker class in the bundle, and will not need to implement this method. 
*/
- (NSImage *)provideNewButtonImage;
- (void)insertNewButtonImage:(NSImage *)newButtonImage in:(NSButtonCell *)buttonCell;
- (void)viewSizeChanged:(nullable id)sender;
- (void)alphaControlAddedOrRemoved:(nullable id)sender;
- (void)attachColorList:(NSColorList *)colorList;
- (void)detachColorList:(NSColorList *)colorList;
- (void)setMode:(NSColorPanelMode)mode;   


/* The tooltip to be used for the tool bar button.
*/
- (NSString *)buttonToolTip API_AVAILABLE(macos(10.5));

/* The minimum content size for your picker. The NSColorPanel will not allow resizing smaller than this size. By default, you will not have to do anything if you properly setup the Autosizing attributes in IB for your view. 
*/
- (NSSize)minContentSize API_AVAILABLE(macos(10.5));


@end

/* NSColorPickingCustom MUST be implemented by the CustomPicker, or an error will occur! 
*/
@protocol NSColorPickingCustom <NSColorPickingDefault>

/* Return NO if "mode" not supported. 
*/
- (BOOL)supportsMode:(NSColorPanelMode)mode;   

/* Return the current mode that your picker is in.
*/
- (NSColorPanelMode)currentMode;

/* Provide the view for your picker. initialRequest will be YES on very first call. At this point, you should load your nibs.
*/
- (NSView *)provideNewView:(BOOL)initialRequest;

/* Set your color picker's displayed color to newColor.
*/
- (void)setColor:(NSColor *)newColor;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

