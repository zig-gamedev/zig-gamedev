/*
    NSPopover.h
    Application Kit
    Copyright (c) 2010-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSGeometry.h>
#import <Foundation/NSObject.h>

#import <AppKit/NSNibDeclarations.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSAppearance.h>
#import <AppKit/NSResponder.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSView, NSViewController, NSWindow, NSNotification, NSString;

#pragma mark -
#pragma mark Popovers

/*  A popover is a unit of content that is positioned relative to some other content on the screen.  An anchor is used to express the relation between these two units of content.  Each popover has an appearance that specifies its visual characteristics, as well as a behavior that determines which user interactions will cause the popover to close.  A transient popover is closed in response to most user interactions, whereas a semi-transient popovers is closed when the user interacts with the window containing the popover's positioning view.  Popovers with application-defined behavior are not usually closed on the developer's behalf.  AppKit automatically positions each popover relative to its positioning view and moves the popover whenever its positioning view moves.  A positioning rectangle within the positioning view can be specified for additional granularity.  Popovers can be detached to become a separate window when they are dragged by implementing the appropriate delegate method. 
 */

#pragma mark -
#pragma mark Enumerated Types

typedef NS_ENUM(NSInteger, NSPopoverAppearance) {
    ///  The popover will use the default, light content appearance.
    NSPopoverAppearanceMinimal API_DEPRECATED("", macos(10.7,10.10)) = 0,
    /// The popover will draw with a HUD appearance.
    NSPopoverAppearanceHUD API_DEPRECATED("", macos(10.7,10.10)) = 1
} API_DEPRECATED("", macos(10.7,10.10));

/*  AppKit supports transient, semi-transient, and application-defined behaviors. Please see the class description above for more information.  The default popover behavior is NSPopoverBehaviorApplicationDefined. 
 */
typedef NS_ENUM(NSInteger, NSPopoverBehavior) {
    /*  Your application assumes responsibility for closing the popover. AppKit will still close the popover in a limited number of circumstances. For instance, AppKit will attempt to close the popover when the window of its positioningView is closed.  The exact interactions in which AppKit will close the popover are not guaranteed.  You may consider implementing -cancel: to close the popover when the escape key is pressed. 
     */
    NSPopoverBehaviorApplicationDefined = 0,
    
    /*  AppKit will close the popover when the user interacts with a user interface element outside the popover.  Note that interacting with menus or panels that become key only when needed will not cause a transient popover to close.  The exact interactions that will cause transient popovers to close are not specified. 
     */
    NSPopoverBehaviorTransient = 1,
    
    /*  AppKit will close the popover when the user interacts with user interface elements in the window containing the popover's positioning view.  Semi-transient popovers cannot be shown relative to views in other popovers, nor can they be shown relative to views in child windows.  The exact interactions that cause semi-transient popovers to close are not specified. 
     */
    NSPopoverBehaviorSemitransient = 2
};

@protocol NSPopoverDelegate;

API_AVAILABLE(macos(10.7))
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_10
@interface NSPopover : NSResponder <NSAppearanceCustomization, NSAccessibilityElement, NSAccessibility>
#else
@interface NSPopover : NSResponder <NSAccessibilityElement, NSAccessibility>
#endif

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
    
#pragma mark -
#pragma mark Bindings

/*  NSPopover exposes the following bindings: 
         contentWidth    (CGFloat, readonly) 
         contentHeight   (CGFloat, readonly)
         positioningRect (NSRect, readonly) 
*/

#pragma mark -
#pragma mark Properties

/*  All properties of NSPopover are KVO compliant. 
 */

/*  The delegate of the popover. The delegate is not retained. 
 */
@property(nullable, weak) IBOutlet id <NSPopoverDelegate> delegate;

#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_10
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wavailability"

/*!
 * The appearance of the popover. The popover's contentView will inherit this appearance. The default effective appearance is the NSAppearanceNameVibrantLight appearance.
 * If nil is set, nil will be returned, and the effective appearance will return to the default.
 * To prevent conflicts with the previous appearance property, this is only available for apps that target 10.10 and higher.
 */
@property (nullable, strong) NSAppearance *appearance API_AVAILABLE(macos(10.10));

@property (readonly, strong) NSAppearance *effectiveAppearance API_AVAILABLE(macos(10.10));

#pragma clang diagnostic pop
#else

/*!
 * This NSPopoverAppearance-based property is deprecated as of 10.10 and should not be used if targeting 10.10 or higher.
 * A transition to the NSAppearance-based property should be made where possible.
 * The default is NSPopoverAppearanceMinimal.
 */
@property NSPopoverAppearance appearance API_DEPRECATED("", macos(10.7,10.10));

#endif



/*  The behavior of the popover.  The default behavior is NSPopoverBehaviorApplicationDefined. See the declaration of NSPopoverBehavior above for more information about popover behaviors. 
 */
@property NSPopoverBehavior behavior;

/*  Should the popover be animated when it shows, closes, or appears to transition to a detachable window.  This property also controls whether the popover animates when the content view or content size changes. AppKit does not guarantee which behaviors will be animated or that this property will be respected; it is regarded as a hint.  The default value is YES. 
 */
@property BOOL animates;

/*  The view controller that manages the content of the popover.  The default value is nil.  You must set the content view controller of the popover to a non-nil value before the popover is shown.  Changes to the popover's content view controller while the popover is shown will animate (provided animates is YES). 
 */
@property(nullable, retain) IBOutlet NSViewController *contentViewController;

/*  The content size of the popover.  The popover's content size is set to match the size of the content view when the content view controller is set.  Changes to the content size of the popover will animate while the popover is shown (provided animates is YES). 
 */
@property NSSize contentSize;

/*  YES if the popover is being shown, NO otherwise. The popover is considered to be shown from the point when -showRelativeToRect:ofView:preferredEdge: is invoked until the popover is closed in response to an invocation of either -close or -performClose:.
 */
@property(readonly, getter=isShown) BOOL shown;

/// Returns \c YES if the window is detached to an implicitly created detached window, \c NO otherwise. This method does not apply when the popover is detached to a window returned with \c -detachableWindowForPopover:.
@property (readonly, getter=isDetached) BOOL detached API_AVAILABLE(macos(10.10));


/*  Popovers are positioned relative to a positioning view and are automatically moved when the location or size of the positioning view changes.  Sometimes it is desirable to position popovers relative to a rectangle within the positioning view.  In this case, you must update the positioningRect binding whenever this rectangle changes, or use the positioningRect binding so AppKit can re-position the popover when appropriate. 
 */
@property NSRect positioningRect;

#pragma mark -
#pragma mark Show and Close

/*  Shows the popover positioned relative to positioningRect of positioningView (see the description of positioningRect above).  The common case is to pass [positioningView bounds] for positioningRect, in which case the popover will be placed adjacent to the positioningView and there is no need to update positioningRect (AppKit will detect the the bounds of the positioning view was specified and automatically update the positioningView).  preferredEdge is a hint to AppKit about the desired placement of the anchor of the popover towards the positioningRect, and is with respect to the -isFlipped state of the positioningView. Also, if positioningRect is an empty rect, the [view bounds] will automatically be used. The current (but not guaranteed) behavior is that AppKit will place the anchor towards the preferredEdge of the positioningRect unless such a placement would cause the popover not to fit on the screen of positioningView.  If the anchor cannot be placed towards the preferredEdge, AppKit will (in the current implementation) attempt to place the anchor on the opposite side of the positioningRect.  If that cannot be done, AppKit will attempt to place the anchor on a remaining sides of the popover, and failing that will center the popover on the screen, causing it to (at least temporarily) lose its anchor. The popover will animate onscreen and eventually animate offscreen when it is closed (unless the property animates is set to NO). This method will throw a NSInvalidArgumentException if view is nil or if view is not in a window, or if the popover's behavior is NSPopoverBehaviorSemitransient and the popover's positioningView is in a popover or child window. It will throw a NSInternalInconsistencyException if the popover's  content view controller (or the view controller's view) is nil. If the popover is already being shown, this method will update to be associated with the new view and positioningRect passed. If the positioning view is not visible, this method does nothing. 
 */
- (void)showRelativeToRect:(NSRect)positioningRect ofView:(NSView *)positioningView preferredEdge:(NSRectEdge)preferredEdge;

/*  Attempts to close the popover.  The popover will not be closed if it has a delegate and the delegate returns NO to -popoverShouldClose: (or if the popover's class implements -popoverShouldClose: to return NO). The operation will fail if it is displaying a nested popover, or if it has a child window.  A window will attempt to close its popovers when it receives a -performClose: message.  The popover will animate out when closed (unless the animates property is set to NO).
 */
- (IBAction)performClose:(nullable id)sender;

/*  Forces the popover to close without consulting its delegate.  Any popovers nested within the popovers will also receive a close message.  When a window is closed in response to the -close message being sent, all of its popovers will be closed.  The popover will animate out when closed (unless the animates property is set to NO).
 */
- (void)close;

@end

#pragma mark -
#pragma mark Notifications

/* Specifies the close reason.  Currently used only as the userInfo key for the NSPopoverWillCloseNotification.
 */
APPKIT_EXTERN NSString * const NSPopoverCloseReasonKey API_AVAILABLE(macos(10.7));

/* Possible values for NSPopoverCloseReasonKey */
typedef NSString * NSPopoverCloseReasonValue NS_TYPED_ENUM;
/* the popover is being closed in a standard way */
APPKIT_EXTERN NSPopoverCloseReasonValue const NSPopoverCloseReasonStandard API_AVAILABLE(macos(10.7));
/* The popover has been closed because it is being detached to a window */
APPKIT_EXTERN NSPopoverCloseReasonValue const NSPopoverCloseReasonDetachToWindow API_AVAILABLE(macos(10.7));

/*  Sent before the popover is shown. 
 */
APPKIT_EXTERN NSNotificationName const NSPopoverWillShowNotification API_AVAILABLE(macos(10.7));

/*  Sent after the popover has finished animating onscreen. 
 */
APPKIT_EXTERN NSNotificationName const NSPopoverDidShowNotification API_AVAILABLE(macos(10.7));

/*  Sent before the popover is closed. The userInfo key NSPopoverCloseReasonKey specifies the reason for closing.  It can currently be either NSPopoverCloseReasonStandard or NSPopoverCloseReasonDetachToWindow, although more reasons for closing may be added in the future. 
 */
APPKIT_EXTERN NSNotificationName const NSPopoverWillCloseNotification API_AVAILABLE(macos(10.7));

/*  Sent after the popover has finished animating offscreen.  This notification has the same user info keys as NSPopoverWillCloseNotification. 
 */
APPKIT_EXTERN NSNotificationName const NSPopoverDidCloseNotification API_AVAILABLE(macos(10.7));

#pragma mark -
#pragma mark Delegate Methods

@protocol NSPopoverDelegate <NSObject>
@optional

/*!
 * The popover invokes this method on its delegate whenever it is about to close to give the delegate a chance to veto the close. 
 * If the delegate does not implement this method, \c -popoverShouldClose: will also be invoked on the popover to allow the popover to veto the close.
 * \return Return YES if the popover should close, NO otherwise.
 */
- (BOOL)popoverShouldClose:(NSPopover *)popover;

/*!
 * Return \c YES to allow the popover to detach from its positioning view. Return \c NO if it should not. If this method is not implemented, the default behavior is \c NO.
 * If this method returns YES, and \c -detachableWindowForPopover: is not implemented or returns nil, a detachable window will be created with the popover’s \c contentViewController.
 * This implicit detached window will have the same appearance as the popover. If the \c contentViewController has a title, it will be bound to and displayed as the title of the detached window. Upon being released in a detached state, the popover will call \c -popoverDidDetach: on the delegate. Once the detached popover is closed, PopoverShould/Will/DidClose delegate calls and notifications will be sent with the reason \c NSPopoverCloseReasonStandard.
 * \param popover The popover that may be detached
 * \return YES if the popover should detach, whether to a custom window or the implicitly detached window. NO if not.
 */
- (BOOL)popoverShouldDetach:(NSPopover *)popover API_AVAILABLE(macos(10.10));

/*!
 * This is called when the popover has been released in an implicitly detached state, i.e. not when detached to a custom window returned from \c -detachableWindowForPopover:.
 * \param popover The popover that detached from its anchor view and is not closing.
 */
- (void)popoverDidDetach:(NSPopover *)popover API_AVAILABLE(macos(10.10));

/*!
 * Return a custom window to which the popover should be detached. This should be used when the content of the detached window is wanted to be different from the content of the popover. If the same content should be used in the detached window, only \c -popoverShouldDetach: needs to be implemented.
 * If implementing this method, you should not remove the popover's content view as part of your implementation of this method. The popover and the detachable window may be shown at the same time and therefore cannot share a content view (or a content view controller).  If the popover and the detachable window should have the same content, you should define the content in a separate nib file and use a view controller to instantiate separate copies of the content for the popover and the detachable window.  The popover will animate to appear as though it morphs into the detachable window (unless the animates property is set to NO.  The exact animation used is not guaranteed).  Subclasses of NSPopover may also implement this method, in which case the subclass method will be invoked only if the delegate does not implement the method.
 * Not implementing this method, and just returning YES to \c -popoverShouldDetach: is recommended over providing a custom window.
 * If \c -popoverShouldDetach: is not overridden or returns NO, this method will not be called and the popover will not be detachable.
 * \param popover The popover that is being detached
 * \return The custom window to detach to.
 */
- (nullable NSWindow *)detachableWindowForPopover:(NSPopover *)popover;

/*!
 * Invoked on the delegate when the NSPopoverWillShowNotification notification is sent.  This method will also be invoked on the popover.
 */
- (void)popoverWillShow:(NSNotification *)notification;

/*!
 * Invoked on the delegate when the NSPopoverDidShowNotification notification is sent.  This method will also be invoked on the popover.
 */
- (void)popoverDidShow:(NSNotification *)notification;

/*!
 * Invoked on the delegate when the NSPopoverWillCloseNotification notification is sent.  This method will also be invoked on the popover.
 */
- (void)popoverWillClose:(NSNotification *)notification;

/*!
 * Invoked on the delegate when the NSPopoverDidCloseNotification notification is sent.  This method will also be invoked on the popover.
 */
- (void)popoverDidClose:(NSNotification *)notification;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
