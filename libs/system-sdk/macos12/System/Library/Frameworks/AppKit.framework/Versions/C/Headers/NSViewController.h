/*
	NSViewController.h
	Application Kit
	Copyright (c) 2006-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSKeyValueBinding.h>
#import <AppKit/NSNib.h>
#import <AppKit/NSNibDeclarations.h>
#import <AppKit/NSResponder.h>
#import <AppKit/NSPopover.h>
#import <AppKit/NSStoryboard.h>
#import <AppKit/NSStoryboardSegue.h>
#import <AppKit/NSUserInterfaceItemIdentification.h>
#import <AppKit/AppKitDefines.h>

@protocol NSExtensionRequestHandling;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSBundle, NSPointerArray, NSView;
@protocol NSViewControllerPresentationAnimator;

typedef NS_OPTIONS(NSUInteger, NSViewControllerTransitionOptions) {
    /* No animation (the default).  Specifying any animation from the options below negates the "None" option.
     */
    NSViewControllerTransitionNone                  =    0x0,

    /* Basic animation type (optional).  This option can be combined with any Slide* option below.
     */
    NSViewControllerTransitionCrossfade             =    0x1,   // Fades the new view in and the old view out.

    /* Slide animation (optional).  Up and Down are disjoint and can't be combined.  Left and Right are disjoint and can't be combined.
     */
    NSViewControllerTransitionSlideUp               =   0x10,   // Animates by sliding the old view up while the new view comes from the bottom.
    NSViewControllerTransitionSlideDown             =   0x20,   // Animates by sliding the old view down while the new view comes from the top.
    NSViewControllerTransitionSlideLeft             =   0x40,   // Animates by sliding the old view to the left while the new view comes in from the right (both views move left).
    NSViewControllerTransitionSlideRight            =   0x80,   // Animates by sliding the old view to the right while the new view comes in from the left (both views move right).
    NSViewControllerTransitionSlideForward          =  0x140,   // Same as "Left", but automatically flips to be "Right" when NSApp.userInterfaceLayoutDirection is Right-to-Left.
    NSViewControllerTransitionSlideBackward         =  0x180,   // Same as "Right", but automatically flips to be "Left" when NSApp.userInterfaceLayoutDirection is Right-to-Left.

    NSViewControllerTransitionAllowUserInteraction  = 0x1000,   // Allow user interaction during the transaction; normally it is prevented for the parent view controller while the transition is happening.

} API_AVAILABLE(macos(10.10));

API_AVAILABLE(macos(10.5))
@interface NSViewController : NSResponder <NSEditor, NSSeguePerforming, NSUserInterfaceItemIdentification>

/* The designated initializer. The specified nib should typically have the class of the file's owner set to NSViewController, or a subclass of yours, with the "view" outlet connected to a view. If you pass in a nil nib name then -nibName will return nil. -loadView can be used to assign a view before -view is invoked. If you pass in a nil bundle then -nibBundle will return nil and -loadView will interpret it using the same rules as -[NSNib initWithNibNamed:bundle:].

On 10.10 and higher, a nil nibName can be used, and NSViewController will automatically attempt to load a view with the same class name. See loadView for more information.
*/
- (instancetype)initWithNibName:(nullable NSNibName)nibNameOrNil bundle:(nullable NSBundle *)nibBundleOrNil NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* Return the name of the nib to be loaded to instantiate the view. The default implementation returns whatever value was passed to the initializer.
*/
@property (nullable, copy, readonly) NSNibName nibName;

/* Return the bundle that the nib will be loaded from. The default implementation returns whatever value was passed to the initializer.
*/
@property (nullable, strong, readonly) NSBundle *nibBundle;

/* The object whose value is being presented in the view. The default implementation of -setRepresentedObject: doesn't copy the passed-in object, it retains it. (In another words, "representedObject" is a to-one relationship, not an attribute.) This class is key-value coding and key-value observing compliant for "representedObject" so when you use it as the file's owner of a view's nib you can bind controls to the file's owner using key paths that start with "representedObject."
*/
@property (nullable, strong) id representedObject;

/* The localized title of the view. This class doesn't actually do anything with the value of this property other than hold onto it, and be KVC and KVO compliant for "title." The default implementation of -setTitle: copies the passed-in object ("title" is an attribute). This property is here because so many anticipated uses of this class will involve letting the user choose among multiple named views using a pulldown menu or something like that.
*/
@property (nullable, copy) NSString *title;

/* The default implementation of the getter first invokes [self loadView] if the view hasn't been set yet. After -loadView is called, -viewDidLoad will be called. The setter can be used to assign a view that's created in a different manner than what -view's default implementation would do.
*/
@property (strong) IBOutlet NSView *view;

/* The default implementation of this method invokes [self nibName] and [self nibBundle] and then uses the NSNib class to load the nib with this object as the file's owner. If the "view" outlet of the file's owner in the nib is properly connected, the regular nib loading machinery will send this object a -setView: message. You can override this method to customize how nib loading is done, including merely adding new behavior immediately before or immediately after nib loading done by the default implementation. One can also use viewDidLoad to modify things after the view has been loaded. You should not invoke this method from other objects unless you take care to avoid redundant invocations; NSViewController's default implement can handle them but overrides in subclasses might not. (Typically other objects should instead invoke -view and let the view controller do whatever it takes to fulfill the request.) In general, you should not call this method, and let NSViewController call it when needed. If you need it called, simply call [viewController view].
 
Prior to 10.10, -loadView would not have well defined behavior if [self nibName] returned nil. On 10.10 and later, if nibName is nil, NSViewController will automatically try to load a nib with the same name as the classname. This allows a convenience of doing [[MyViewController alloc] init] (which has a nil nibName) and having it automatically load a nib with the name "MyViewController".
*/
- (void)loadView;

/* Conformance to KVB's NSEditor informal protocol. The default implementations of these methods pass on the message to each registered editor, which are typically controls in the nib that are bound to the nib file's owner. You can override these methods to customize what is done when your view's presentation to the user is about to end because, for example, the user has selected another of a set of views or hit a panel's OK button (committing time) or because the user has hit a panel's Cancel button (discarding time). This class also conforms to KVB's NSEditorRegistration protocol, but you're not encouraged to override those methods.
*/
- (void)commitEditingWithDelegate:(nullable id)delegate didCommitSelector:(nullable SEL)didCommitSelector contextInfo:(nullable void *)contextInfo;
- (BOOL)commitEditing;
- (void)discardEditing;


/* Called after the view has been loaded. For view controllers created in code, this is after -loadView. For view controllers unarchived from a nib, this is after the view is set. Default does nothing.
*/
- (void)viewDidLoad API_AVAILABLE(macos(10.10));

/* Returns whether the view has been loaded or not.
*/
@property (readonly, getter=isViewLoaded) BOOL viewLoaded API_AVAILABLE(macos(10.10));

/* Called when the view is about to appear, meaning it is going from a state of being "hidden" or having a hidden ancestor, and/or not being in a window or being in a window that is ordered out, to a state where none of these conditions is true. Default does nothing. In general, it is good practice to call [super viewWillAppear] in case a superclass implements this method. Causing a view to become hidden or removed from its window, or causing its window to be ordered out, in response to -viewWillAppear is not recommended and may result in an exception being raised. When such responses are necessary, defer them to a future runloop cycle.
*/
- (void)viewWillAppear API_AVAILABLE(macos(10.10));

/* Called when the view has been fully transitioned onto the screen, meaning it is not "hidden", does not have a hidden ancestor, and is in a window that's ordered in. Default does nothing. In general, it is good practice to call [super viewDidAppear] in case a superclass implements this method.
*/
- (void)viewDidAppear API_AVAILABLE(macos(10.10));

/* Called when the view is about to disappear, meaning it is going from a state of not being "hidden", nor having a hidden ancestor, nor having a nil window or being in a window that's ordered out, to a state where at least one of these conditions is true. Default does nothing. In general, it is good practice to call [super viewWillDisappear] in case a superclass implements this method. Causing a view to become unhidden, or causing its window to be ordered in, in response to -viewWillDisappear is not recommended and may result in an exception being raised. When such responses are necessary, defer them to a future runloop cycle.
*/
- (void)viewWillDisappear API_AVAILABLE(macos(10.10));

/* Called after the view has been fully transitioned off the screen, meaning it is now "hidden" or has a hidden ancestor, or it not in a window, or is in a window that is ordered out. Default does nothing. In general, it is good practice to call [super viewDidDisappear] in case a superclass implements this method.
*/
- (void)viewDidDisappear API_AVAILABLE(macos(10.10));

/* Expresses the view's desired size.  May be consulted by a parent ViewController when performing layout.
 */
@property NSSize preferredContentSize API_AVAILABLE(macos(10.10));

/*  The base implementation sends -updateConstraints to the view. When a view has a view controller, this message is sent to the view controller during the autolayout updateConstraints pass in lieu of sending updateConstraints directly to the view. You may override this method in a NSViewController subclass for updating custom constraints instead of subclassing your view and overriding -[NSView updateConstraints].  Overrides must call super or send -updateConstraints to the view. This method is only called for applications that link on 10.10 and higher.
*/
- (void)updateViewConstraints API_AVAILABLE(macos(10.10));

/* Called just before the view controller's view's layout method is invoked. Subclasses can implement as necessary. The default is a no-op.
*/
- (void)viewWillLayout API_AVAILABLE(macos(10.10));

/* Called just after the view controller's view's layout method is invoked. Subclasses can implement as necessary. The default is a no-op.
*/
- (void)viewDidLayout API_AVAILABLE(macos(10.10));

@end


@interface NSViewController (NSViewControllerPresentation)

/* Presents the viewController with a specific animator that handles both the presentation and dismissal of the viewController. The animator is held onto until dismissViewController: is called. This is the fundamental primitive for displaying view controllers that block interaction in some way. The method will assert if animator is nil. In general, you will not directly call this method unless you have a custom animator. Instead, you will use one of the standard cover methods that provide the animator: [NSViewController presentViewControllerAsSheet:], [NSViewController presentViewControllerAsModalWindow:], [NSViewController presentViewController:asPopoverRelativeToRect:...]
*/
- (void)presentViewController:(NSViewController *)viewController animator:(id <NSViewControllerPresentationAnimator>)animator API_AVAILABLE(macos(10.10));

/* Dismisses the viewController that was previously presented with the same animator that presented the viewController. This is the generic way to close a given viewController, no matter how it was presented.
*/
- (void)dismissViewController:(NSViewController *)viewController API_AVAILABLE(macos(10.10));

/* Dismisses the receiver.  If the receiver’s presenter is an NSViewController, it will be sent a -dismissViewController: message with this receiver as the parameter.  Does nothing if the receiver is not currently presented.
*/
- (IBAction)dismissController:(nullable id)sender API_AVAILABLE(macos(10.10));

/* The view controllers that were presented by this view controller. In other words, 'self' displayed each of the items in the array. This is a one-to-many relationship.
*/
@property (nullable, readonly) NSArray<__kindof NSViewController *> *presentedViewControllers API_AVAILABLE(macos(10.10));

/* The view controller that presented this view controller (or its farthest ancestor). In other words, 'presentingViewController' is the one that displayed 'self' to screen.
*/
@property (nullable, readonly, assign) NSViewController *presentingViewController API_AVAILABLE(macos(10.10));

@end

@interface NSViewController(NSViewControllerPresentationAndTransitionStyles)

/* Presents the viewController as a sheet. The viewController is made the delegate and contentViewController of the sheet while it is shown. This method calls [self presentViewController:viewController animator:] with an animator that controls the sheet animation. Call [presentingViewController dismissViewController:viewController] to close the viewController that was previously shown as a sheet.
*/
- (void)presentViewControllerAsSheet:(NSViewController *)viewController API_AVAILABLE(macos(10.10));

/* Presents the viewController as a modal window (also known as an alert). The viewController is made the delegate and contentViewController of the window while it is shown. NSWindow delegate methods can be used to prevent closing of the window (if needed). This method calls [self presentViewController:viewController animator:] with an animator that controls the displaying of the window. Call [presentingViewController dismissViewController:viewController] to close the viewController that was previously shown as a modal window.
*/
- (void)presentViewControllerAsModalWindow:(NSViewController *)viewController API_AVAILABLE(macos(10.10));

/* Presents the viewController as a popover. The viewController is made the delegate and contentViewController of the popover while it is shown. NSPopover delegate methods can be used to customize the popover. This method calls [self presentViewController:viewController animator:] with an animator that controls the displaying of the popover. Call [presentingViewController dismissViewController:viewController] to close the viewController that was previously shown as a popover.
*/
- (void)presentViewController:(NSViewController *)viewController asPopoverRelativeToRect:(NSRect)positioningRect ofView:(NSView *)positioningView preferredEdge:(NSRectEdge)preferredEdge behavior:(NSPopoverBehavior)behavior API_AVAILABLE(macos(10.10));

/* This method can be used to transition between sibling child view controllers. The receiver of this method is their common parent view controller. (Use [NSViewController addChildViewController:] to create the parent/child relationship.) This method will add the toViewController's view to the superview of the fromViewController's view. The fromViewController's view will be removed from the parent at the appropriate time. It is important to allow this method to add and remove the views. This method will throw an exception/assertion if the parent view controllers are not the same as the receiver, or if fromViewController.view does not have a superview.
*/
- (void)transitionFromViewController:(NSViewController *)fromViewController toViewController:(NSViewController *)toViewController options:(NSViewControllerTransitionOptions)options completionHandler:(void (^ _Nullable)(void))completion API_AVAILABLE(macos(10.10));

@end


@interface NSViewController(NSViewControllerContainer)

/* Returns the ancestor of this view controller. Can return nil if this is the contentViewController, or there is no parent for the given view controller.
*/
@property (nullable, readonly) NSViewController *parentViewController API_AVAILABLE(macos(10.10));

/* An array of children view controllers. Assignment of the array filters all additions and removals through the insert and remove API below.
*/
@property (copy) NSArray<__kindof NSViewController *> *childViewControllers API_AVAILABLE(macos(10.10));

/* A convenience method for adding a child view controller; this simply calls insertChildViewController:atIndex: at the end of the array.
*/
- (void)addChildViewController:(NSViewController *)childViewController API_AVAILABLE(macos(10.10));

/* Removes the child controller from this parent controller. This method simply calls [self.parentViewController removeChildViewControllerAtIndex:index] with the appropriate index in the array.
*/
- (void)removeFromParentViewController API_AVAILABLE(macos(10.10));

/* Primitive method to insert a child view controller in the childViewControllers array. If the child controller has a different parent controller, it will first be removed from its current parent by calling [childViewController removeFromParentViewController]. If this method is overridden then the super implementation should be called. This is the primitive to override for performing insert operations in a container class, and has little value in itself if the parent doesn't do something with the children.
*/
- (void)insertChildViewController:(NSViewController *)childViewController atIndex:(NSInteger)index API_AVAILABLE(macos(10.10));

/* Primitive method to remove a child view controller in the childViewControllers array. This method can be overridden to do additional work when a child is removed. If it is overridden, then the super implementation should be called. This is the primitive to override for performing insert operations in a container class, and has little value in itself if the parent doesn't do something with the children. */
- (void)removeChildViewControllerAtIndex:(NSInteger)index API_AVAILABLE(macos(10.10));

/* Sent to a view controller when the `preferredContentSize` property of one of its child or presented view controllers changes.
*/
- (void)preferredContentSizeDidChangeForViewController:(NSViewController *)viewController API_AVAILABLE(macos(10.10));

/* Sent to a service view controller when its view is about to be resized.  Override this, if desired, to perform relayout in response to the resize, potentially in an animated way.
*/
- (void)viewWillTransitionToSize:(NSSize)newSize API_AVAILABLE(macos(10.10));

@end


/* A presentation animator is responsible for both presenting and dismissing a view controller's view. It can be presented in any way the animator wishes. Normally you do not need to implement this protocol, unless you want to have a custom presentation.
*/
@protocol NSViewControllerPresentationAnimator <NSObject>

@required

/* Called when the view controller is going to be presented. Implement presentation in this method when it is called.
*/
- (void)animatePresentationOfViewController:(NSViewController *)viewController fromViewController:(NSViewController *)fromViewController API_AVAILABLE(macos(10.10));

/* Called to dismiss a previously shown view controller.
*/
- (void)animateDismissalOfViewController:(NSViewController *)viewController fromViewController:(NSViewController *)fromViewController API_AVAILABLE(macos(10.10));

@end


/* These methods are used to support using Storyboards with your app.
*/
@interface NSViewController (NSViewControllerStoryboardingMethods)

/* The Storyboard the ViewController was loaded from. Returns nil if the ViewController was not loaded from a Storyboard.
*/
@property(nullable, readonly, strong) NSStoryboard *storyboard API_AVAILABLE(macos(10.10));

@end

@interface NSViewController(NSExtensionAdditions) <NSExtensionRequestHandling>

/* Returns the extension context. Also acts as a convenience method for a view controller to check if it participating in an extension request.
*/
@property (nullable, readonly,retain) NSExtensionContext *extensionContext API_AVAILABLE(macos(10.10));

/* For services that vend UI.  Identifies the view (if any) in the service’s UI that displays the source image.  Defaults to nil.  When your service loads its UI (or in your service UI's xib file), you can set this to point to an image view, or some containing border view, that’s a descendant of the ViewController’s view.  Doing so helps the system to position and animate the service’s UI with respect to the source item representation.  If your service UI doesn't display the source item at its original screen position and size, leave this set to nil.
*/
@property(nullable, strong) IBOutlet NSView *sourceItemView API_AVAILABLE(macos(10.10));

/* For services that vend UI.  Set this to position the service UI's lower-left corner in screen space.  (Use the `preferredContentSize` property to specify the service UI's desired size, in screen units.)
*/
@property NSPoint preferredScreenOrigin API_AVAILABLE(macos(10.10));

/* For services that vend UI.  Expresses the smallest allowable size for the service’s root view, in screen units.  A service should return the minimum dimensions its root view can accommodate, based on the items the service has been sent.  This defaults to a small but non-empty size.
*/
@property(readonly) NSSize preferredMinimumSize API_AVAILABLE(macos(10.10));

/* For services that vend UI.  Expresses the largest allowable size for the service’s root view, in screen units.  A service should return the maximum dimensions that are potentially useful for its root view, based on the items the service has been sent.  This defaults to a large or infinite size.
*/
@property(readonly) NSSize preferredMaximumSize API_AVAILABLE(macos(10.10));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
