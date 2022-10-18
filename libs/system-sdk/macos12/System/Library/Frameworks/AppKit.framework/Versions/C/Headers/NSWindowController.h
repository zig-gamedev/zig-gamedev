/*
    NSWindowController.h
    Application Kit
    Copyright (c) 1997-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSResponder.h>
#import <AppKit/NSNib.h>
#import <AppKit/NSNibDeclarations.h>
#import <AppKit/NSStoryboardSegue.h>
#import <AppKit/NSWindow.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSArray, NSDocument, NSStoryboard, NSViewController, NSWindow;

@interface NSWindowController : NSResponder <NSSeguePerforming>

/* Designated Initializer.  window can be nil.  All other -init... methods call this, and then possibly do other setup.
 */
- (instancetype)initWithWindow:(nullable NSWindow *)window NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* Instances initialized with the "Name" methods will eventually locate their nib file in the file's owner's class' bundle or in the app's +mainBundle using standard NSBundle API.  Use the "Path" method if your nib file is at a fixed location (which is not inside one of those bundles).
 */
- (instancetype)initWithWindowNibName:(NSNibName)windowNibName;	// self is the owner
- (instancetype)initWithWindowNibName:(NSNibName)windowNibName owner:(id)owner; // The owner is NOT retained
- (instancetype)initWithWindowNibPath:(NSString *)windowNibPath owner:(id)owner;

// -----------------------------------------------------------------------------
// Basic properties
// -----------------------------------------------------------------------------

/* If -initWithWindowNibPath:owner: was used to initialize the instance, this gives the last path component with its extension stripped off.  If -initWithWindowNibName:[owner:] was used this just gives that name.
*/
@property(nullable, copy, readonly) NSNibName windowNibName;

/* The full path of the nib.  If -initWithWindowNibPath:owner: was used to initialize the instance, the path is just returned.  If -initWithWindowNibName:[owner:] was used this locates the nib in the file's owner's class' bundle or in the app's mainBundle and returns the full path (or nil if it cannot be located).  Subclasses can override this to augment the search behavior, but probably ought to call super first.
*/
@property(nullable, copy, readonly) NSString *windowNibPath;

/* The file's owner for this window controller's nib file.  Usually this is either the controller itself or the controller's document, but, in general, it is specified by the -init... methods. The owner is weakly referenced.
*/
@property(weak, readonly) id owner;

/* This allows setting and accessing the autosave name for the controller's window.  If the controller has a window frame autosave name, it will automatically make sure its window gets it set appropriately whenever it gets its window set.
*/
@property(copy) NSWindowFrameAutosaveName windowFrameAutosaveName;

/* If this is set to YES then new windows will be cascaded based on the original frame of the window.
*/
@property BOOL shouldCascadeWindows;


// -----------------------------------------------------------------------------
// Dealing with the document
// -----------------------------------------------------------------------------

/* Returns the document associated with the window controller (if any).
 You should not set the document property.  It is invoked automatically from NSDocument's -addWindowController:.  You can override it if you need to, but call super's implementation. The document is not retained by the NSWindowController.
 */
@property (nullable, assign) id /* __kindof NSDocument * */ document;

/* NSDocument calls this method for its window controllers whenever the document is made dirty or clean.  By default this calls -setDocumentEdited: on the controller's window (if any).  A controller calls this on itself when its document gets set or its window gets set.
*/
- (void)setDocumentEdited:(BOOL)dirtyFlag;

/* This indicates whether closing this controller should cause the associated document to close even if the document has other open window controllers.  Normally a document will close automatically only when its last window controller closes.  See the -close method below for more info.
*/
@property BOOL shouldCloseDocument;

// -----------------------------------------------------------------------------
// Dealing with the window
// -----------------------------------------------------------------------------

/* This method is invoked automatically whenever the controller's document has changed that would affect the window title.  (For instance, when the fileName of the document changes, or when the controller gets a new document.)  This method uses the -displayName method of NSDocument to find out the display name.  It then passes the display name to -windowTitleForDocumentDisplayName: to give it a chance to change it.  Finally, if the document has a -fileName and the display name is the same as the -fileName's last component, it actually sets the title from the -fileName instead of the display name.  Otherwise it sets the title from the display name.  If there is a -fileName, that file is always set as the window's represented file  The window title is left alone if the controller has no document.  This method can be overridden if you need to do something else.
*/
- (void)synchronizeWindowTitleWithDocumentName;

/* This method can be overridden to modify or replace the display name computed by the document's -displayName.  If you return something other than the given display name, it will be used as-is for the window title.
*/
- (NSString *)windowTitleForDocumentDisplayName:(NSString *)displayName;


/* The view controller for the window's contentView. Tracks the window property of the same name.
 */
@property (nullable, strong) NSViewController *contentViewController API_AVAILABLE(macos(10.10));

/* The window getter will load the nib file (if there is one and it has not yet been loaded) and then return the window.  If it has to load the window, it will first call -windowWillLoad, then -loadWindow, then -windowDidLoad.  To affect nib loading or do something before or after it happens, you should always override those other methods.
 
    The window setter is used internally from -initWithWindow: or when a controller's nib file is loaded (as the "window" outlet gets connected).  You can also call it yourself if you want to create the window for a window controller lazily, but you aren't loading it from a nib.  This can also be used to set the window to nil for cases where your subclass might not want to keep the window it loaded from a nib, but rather only wants the contents of the window.  Setting the window to nil, after the nib has been loaded, does not reset the -isWindowLoaded state.  A window controller will only load its nib file once.  This method makes sure the window does not release when closed, and it sets the controller's -windowFrameAutosaveName onto the window and updates the window's dirty state to match the controller's document (if any).  It also calls -setWindowController: on the window.  You can override this if you need to know when the window gets set, but call super.
*/
@property (nullable, strong) NSWindow *window;

/* Returns YES if the receiver has loaded its -windowNibName, has itself been loaded from a nib, OR has no -windowNibName.  In other words, if there is no nib to load, this always returns YES.
*/
@property (getter=isWindowLoaded, readonly) BOOL windowLoaded;

/* These are invoked immediately before and after this controller loads its nib, if the the nib loading is triggered by an invocation of -window (which is typical). The default implementations of these methods do nothing. You can override them to add additional processing before and after nib loading. Overriding of -windowDidLoad in particular is useful because it's the first method that's invoked after the connecting of any IB outlets added in a subclass. Your application should not invoke these methods directly.
*/
- (void)windowWillLoad;
- (void)windowDidLoad;

/* This method can be overridden by advanced users.  It should not be called directly (doing so will cause the window controller to be improperly set up).  Call the -window method to force loading of the nib.
*/
- (void)loadWindow;

/* This will close the window controller.  If the controller belongs to a document and it is either the last controller for the document or -shouldCloseDocument is YES, then this will also -close the document.
*/
- (void)close;

/* This will show the controller's window, if any.  If the window is a panel and the panel becomes key only if needed, this method just orders it front.  Otherwise it makes the window key and orders it front.
*/
- (IBAction)showWindow:(nullable id)sender;

@end

/*!
 These methods are used to support using Storyboards with your app.
 */
@interface NSWindowController (NSWindowControllerStoryboardingMethods)

/* The Storyboard the WindowController was loaded from. Returns nil if the WindowController was not loaded from a Storyboard.
 */
@property(nullable, readonly, strong) NSStoryboard *storyboard API_AVAILABLE(macos(10.10));

@end

@interface NSWindowController (NSWindowControllerDismissing)

/* Dismisses the WindowController.  Does nothing if the receiver is not currently presented.
 */
- (IBAction)dismissController:(nullable id)sender API_AVAILABLE(macos(10.10));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END


