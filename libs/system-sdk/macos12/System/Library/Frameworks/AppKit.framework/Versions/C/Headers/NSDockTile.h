/*
	NSDockTile.h
	Application Kit
	Copyright (c) 2006-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSGeometry.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>
#import <AppKit/NSApplication.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

static const NSAppKitVersion NSAppKitVersionNumberWithDockTilePlugInSupport = 1001.0;

@class NSView;

API_AVAILABLE(macos(10.5))
@interface NSDockTile : NSObject 
/* get the size of the dock tile, in screen coordinates
*/
@property (readonly) NSSize size;

/* set the content view to view.  view should be height and width resizable.  In order to initiate drawing in view, you must call -[NSDockTile display]. 
*/
@property (nullable, strong) NSView *contentView;

/* cause the dock tile to be redrawn.  The contentView and any subviews will be sent drawRect: messages.
*/
- (void)display;

/* setShowsApplicationBadge: sets whether or not the dock tile should be badged with the application icon.  Default is YES for NSWindow dock tiles, NO for the NSApplication dock tile.
*/
@property BOOL showsApplicationBadge;

/* Badge the dock icon with a localized string.  The badge appearance is system defined.  This is often used to show an unread count in the application dock icon.
*/
@property (nullable, copy) NSString *badgeLabel;

/* -owner will return NSApp for the application dock tile, or the NSWindow for a mini window dock tile.
*/
@property (readonly, weak) id owner;

@end



@class NSMenu;

/* An application may customize its dock tile when not running via a plugin whose principal class implements the NSDockTilePlugIn protocol.  The name of the plugin is indicated by a NSDockTilePlugIn key in the application's Info.plist file.  The plugin is loaded in a system process at login time or when the application tile is added to the Dock.  When the plugin is loaded, the principal class' implementation of -setDockTile: is invoked.  If the principal class implements -dockMenu, -dockMenu is invoked whenever the user causes the application's dock menu to be shown.  When the dock tile is no longer valid (eg. the application has been removed from the dock, -setDockTile: is invoked with a nil NSDockTile.
*/
@protocol NSDockTilePlugIn <NSObject>

@required
- (void)setDockTile:(nullable NSDockTile*)dockTile;

@optional
/* The plugin should return a menu whose contents will be inserted in front of the standard Dock tile contents. Note that the plugin must ensure that the menu remains valid until the user is done interacting with the menu, which generally requires that the menu be stored in a property with strong ownership. Specifically, the menu should not be autoreleased; that would allow the menu to be deallocated before the user can interact with it, and will cause menu item selections to be ignored.
 */
- (nullable NSMenu*)dockMenu;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

