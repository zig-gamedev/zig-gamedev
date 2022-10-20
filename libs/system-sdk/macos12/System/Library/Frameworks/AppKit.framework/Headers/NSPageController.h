/*
 NSPageController.h
 Application Kit
 Copyright (c) 2011-2021, Apple Inc.
 All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/NSAnimation.h>
#import <AppKit/NSViewController.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSMutableDictionary, NSView;
@protocol NSPageControllerDelegate;

typedef NSString * NSPageControllerObjectIdentifier NS_SWIFT_BRIDGED_TYPEDEF;

typedef NS_ENUM(NSInteger, NSPageControllerTransitionStyle) {
    NSPageControllerTransitionStyleStackHistory, // Pages are stacked on top of each other. Pages animate out to the right to reveal the previous page. Next pages animate in from the right. (See Safari as an example)
    NSPageControllerTransitionStyleStackBook,	 // Pages are stacked on top of each other. Pages animate out to the left to reveal the next page. Previous pages animate in from the left. (See Preview as an example)
    NSPageControllerTransitionStyleHorizontalStrip // Each page is laid out next to each other in one long horizontal strip
} API_AVAILABLE(macos(10.8));

API_AVAILABLE(macos(10.8))
@interface NSPageController : NSViewController <NSAnimatablePropertyContainer, NSCoding>

@property (nullable, weak) IBOutlet id<NSPageControllerDelegate> delegate;

/* The viewController associated with the selected arrangedObject. May be nil if delegate is not supplying viewControllers.
 */
@property (nullable, readonly, strong) NSViewController *selectedViewController;


/* The animation style used when transitioning from one page to another. The default value is NSPageControllerTransitionStyleStackHistory */
@property NSPageControllerTransitionStyle transitionStyle;

/* Array of all displayed objects (after sorting and potentially filtering by you). The delegate will be asked for snapshots as they are needed. Alternatively, you may never directly set this array and use the -navigateForwardToRepresentedObject API to create a history as the user navigates. This property is KVO compliant.
 */
@property (copy) NSArray *arrangedObjects;

/* The index into the arrangedObjects that is being displayed. This property is KVO compliant. To animate a transition to a new index, use NSPageController's animator object. */
@property NSInteger selectedIndex;


/* Clears the arrangedObjects array after the selected index, adds the argument to the arrangedObjects array, and sets the selectedIndex to the argument.
 */
- (void)navigateForwardToObject:(id)object;


/* Hide the transition view used for animation and show the selectedViewController.view. Generally, this is called during pageControllerDidEndLiveTransition: in the delegate when the new contents of view is ready to be displayed.
 */
- (void)completeTransition;

/* Supported IBActions - all IB actions are animated and delegate LiveTransition methods are called. */
- (IBAction)navigateBack:(nullable id)sender;
- (IBAction)navigateForward:(nullable id)sender;
- (IBAction)takeSelectedIndexFrom:(nullable id)sender; // sender must respond to -integerValue;
@end

@protocol NSPageControllerDelegate <NSObject>	
@required
@optional

/* HISTORY MODE - If you want NSPageController to completely manage the arrangedObjects and snapshots then do not implement the following 2 methods.
 CUSTOM MODE - If you manually set the arrangedObjects, then NSPageController does not have any chance to take snapshots of all the objects. In this case, or if you just need more control, you are required to implement the following 2 methods so that navigation can properly occur.
 */


/* Return the identifier of the view controller that owns a view to display the object. If NSPageController does not have an unused viewController for this identifier, then you will be asked to create one via pageController:viewControllerForIdentifier.
 */
- (NSPageControllerObjectIdentifier)pageController:(NSPageController *)pageController identifierForObject:(id)object;

/* NSPageController will cache as many viewControllers and views as necessary to maintain performance. This method is called whenever another instance is required. Note: The viewController may become the selectedViewController after a transition if necessary.
 */
- (NSViewController *)pageController:(NSPageController *)pageController viewControllerForIdentifier:(NSPageControllerObjectIdentifier)identifier;

/* NOTE: The following 2 methods are only useful if you also implement the above two methods.
 */

/* You only need to implement this if the view frame can differ between arrangedObjects. This method must return immediately. Avoid file, network or any potentially blocking or lengthy work to provide an answer. This method is called with a nil object to get the default frame size. If this method is not implemented, all arrangedObjects are assumed to have the same frame as the current selectedViewController.view or the bounds of view when selectedViewController is nil.
 */
- (NSRect)pageController:(NSPageController *)pageController frameForObject:(nullable id)object;

/* Prepare the viewController and view for drawing. Setup data sources and perform layout. Note: a nil object is passed for the purposes of caching a rendering of a default viewController. Note: this method is called on the main thread and should return immediately. The view will be asked to draw on a background thread and must support background drawing. If this method is not implemented, then viewController's representedObject is set to the representedObject.
 */
- (void)pageController:(NSPageController *)pageController prepareViewController:(NSViewController *)viewController withObject:(nullable id)object;

/* Note: You may find these useful regardless of which way you use NSPageController (History vs Custom).
 */

/* This message is sent when any page transition is completed. */
- (void)pageController:(NSPageController *)pageController didTransitionToObject:(id)object;

/* This message is sent when the user begins a transition wither via swipe gesture of one of the navigation IBAction methods. */
- (void)pageControllerWillStartLiveTransition:(NSPageController *)pageController;

/* This message is sent when a transition animation completes either via swipe gesture or one of the navigation IBAction methods. Your content view is still hidden and you must call -completeTransition; on pageController when your content is ready to show. If completed successfully, a pageController:didTransitionToRepresentedObject: will already have been sent.
 */
- (void)pageControllerDidEndLiveTransition:(NSPageController *)pageController;


@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
