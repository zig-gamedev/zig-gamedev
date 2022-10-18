/*
	NSDragging.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>
#import <limits.h>
#import <AppKit/NSPasteboard.h>
#import <AppKit/AppKitDefines.h>

@protocol NSPasteboardWriting;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSDraggingItem, NSDraggingSession, NSImage, NSPasteboard, NSView, NSWindow;
@class NSURL;

typedef NS_OPTIONS(NSUInteger, NSDragOperation) {
    NSDragOperationNone		= 0,
    NSDragOperationCopy		= 1,
    NSDragOperationLink		= 2,
    NSDragOperationGeneric	= 4,
    NSDragOperationPrivate	= 8,
    NSDragOperationMove		= 16,
    NSDragOperationDelete	= 32,
    NSDragOperationEvery	= NSUIntegerMax,
    
    NSDragOperationAll_Obsolete	API_DEPRECATED("", macos(10.0,10.10)) = 15, // Use NSDragOperationEvery
    NSDragOperationAll API_DEPRECATED("", macos(10.0,10.10)) = NSDragOperationAll_Obsolete, // Use NSDragOperationEvery
};

typedef NS_ENUM(NSInteger, NSDraggingFormation) {
    NSDraggingFormationDefault = 0, // System determined formation.
    NSDraggingFormationNone,  // Drag images maintain their set positions relative to each other
    NSDraggingFormationPile,  // Drag images are placed on top of each other with random rotations
    NSDraggingFormationList,  // Drag images are laid out vertically, non-overlapping with the left edges aligned
    NSDraggingFormationStack // Drag images are laid out overlapping diagonally
} API_AVAILABLE(macos(10.7));

typedef NS_ENUM(NSInteger, NSDraggingContext) {
    NSDraggingContextOutsideApplication = 0,
    NSDraggingContextWithinApplication
} API_AVAILABLE(macos(10.7));

typedef NS_OPTIONS(NSUInteger, NSDraggingItemEnumerationOptions) {
    NSDraggingItemEnumerationConcurrent			= NSEnumerationConcurrent,
    /* When the following option is set, AppKit will automatically set the imageComponentProvider to nil for all dragging items that do not meet the classes/searchOptions criteria. Effectively, this hides the drag image for non valid items for this destination. */
    NSDraggingItemEnumerationClearNonenumeratedImages 	= (1UL << 16),
} API_AVAILABLE(macos(10.7));

/* The spring loading highlight styles roughly correlate to {None: NotSpringLoadable, Light: SpringLoadable, Dark: SpringLoadingEngaged}. However, this not not strictly true as Drag & Drop may switch between highlight styles as an animated signal to the user. */
typedef NS_ENUM(NSInteger, NSSpringLoadingHighlight) {
    NSSpringLoadingHighlightNone = 0,
    NSSpringLoadingHighlightStandard,
    NSSpringLoadingHighlightEmphasized
} API_AVAILABLE(macos(10.11));

/* protocol for the sender argument of the messages sent to a drag destination.  The view or
   window that registered dragging types sends these messages as dragging is
   happening to find out details about that session of dragging.
 */
@protocol NSDraggingInfo <NSObject>
@required
@property (nullable, readonly) NSWindow *draggingDestinationWindow;
@property (readonly) NSDragOperation draggingSourceOperationMask;
@property (readonly) NSPoint draggingLocation;

/* Returns the current location of the current composited dragging image’s origin in NSDraggingFormationNone translated to the base coordinate system of the destination object’s window
*/
@property (readonly) NSPoint draggedImageLocation;
@property (nullable, readonly) NSImage *draggedImage API_DEPRECATED("Use NSDraggingItem objects instead", macos(10.0, 11.0));
@property (readonly) NSPasteboard *draggingPasteboard;
@property (nullable, readonly) id draggingSource;
@property (readonly) NSInteger draggingSequenceNumber;
- (void)slideDraggedImageTo:(NSPoint)screenPoint;
- (nullable NSArray<NSString *> *)namesOfPromisedFilesDroppedAtDestination:(NSURL *)dropDestination API_DEPRECATED("Use NSFilePromiseReceiver objects instead", macos(10.0,10.13));

/* Controls the dragging formation while the drag is over this destination. The default value is the current drag formation. */
@property NSDraggingFormation draggingFormation API_AVAILABLE(macos(10.7));

/* During the conclusion of an accepted drag, if this property is set to YES, the drag manager will animate each dragging image to their NSDraggingFormationNone locations. Otherwise, the drag images are removed without any animation. Note: This property is inspected between -prepareForDragOperation: and -performDragOperation:. If the final destination frames do not match the current NSDraggingFormationNone frames, then enumerate through the draggingItems during -performDragOperation: to set thier NSDraggingFormationNone frames to the correct destinations.
 */
@property BOOL animatesToDestination API_AVAILABLE(macos(10.7));

/* During draggingEntered: or draggingUpdated:, you are responsible for returning the drag operation. In some cases, you may accept some, but not all items on the dragging pasteboard. (For example, you only accept image files.) If you only accept some of the items, you should set this property so the drag manager can update the drag count badge. When -updateItems: is called, you should set the image of non valid dragging items to nil. If none of the drag items are valid then do not call this method. Simply return NSDragOperationNone from draggingEntered: and/or draggingUpdated: and do not modify any drag item properties.
 */
@property NSInteger numberOfValidItemsForDrop API_AVAILABLE(macos(10.7));

/* Use the following enumerate method to modify the properties of each dragging item. For example, change the drag image and size.
   Note: All changes made here are only in effect while the drag is over the destination. When the drag exits the destination all properties return to the values last set by the dragging session.
*/

/* Enumerate through each dragging item. Any changes made to the properties of the draggingItem are reflected in the drag and are automatically removed when the drag exits. Classes in the provided array must implement the NSPasteboardReading protocol. Cocoa classes that implement this protocol include NSImage, NSString, NSURL, NSColor, NSAttributedString, and NSPasteboardItem. For every item on the pasteboard, each class in the provided array will be queried for the types it can read using -readableTypesForPasteboard:. An instance will be created of the first class found in the provided array whose readable types match a conforming type contained in that pasteboard item. If an Instance is created from the pasteboard item data, it is placed into an NSDraggingItem along with the dragging properties of that item such as the dragging image. The NSDraggingItem is then passed as a parameter to the provided block. Additional search options, such as restricting the search to file URLs with particular content types, can be specified with a search options dictionary.  See the comments for the Pasteboard Reading Options keys in NSPasteboard.h for a full description. Note: all coordinate properties in the NSDraggingItem are in the coordinate system of view. If view is nil, the screen coordinate space is used.
*/
- (void)enumerateDraggingItemsWithOptions:(NSDraggingItemEnumerationOptions)enumOpts forView:(nullable  NSView *)view classes:(NSArray<Class> *)classArray searchOptions:(NSDictionary<NSPasteboardReadingOptionKey, id> *)searchOptions usingBlock:(void (^)(NSDraggingItem *draggingItem, NSInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.7));

@property (readonly) NSSpringLoadingHighlight springLoadingHighlight API_AVAILABLE(macos(10.11));

/* Used when the drag crosses two distinct but valid spring loading regions within the same destination. The hover timer is reset and if the user is currently in a force click, they must release and re-force click to highlight the new region. */
- (void)resetSpringLoading API_AVAILABLE(macos(10.11));

@end


/* Methods implemented by an object that receives dragged images.  The
   destination view or window is sent these messages during dragging if it
   responds to them.
 */
@protocol NSDraggingDestination <NSObject>
@optional
- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender;
- (NSDragOperation)draggingUpdated:(id <NSDraggingInfo>)sender; /* if the destination responded to draggingEntered: but not to draggingUpdated: the return value from draggingEntered: is used */
- (void)draggingExited:(nullable id <NSDraggingInfo>)sender;
- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender;
- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender;
- (void)concludeDragOperation:(nullable id <NSDraggingInfo>)sender;
/* draggingEnded: is implemented as of Mac OS 10.5 */
- (void)draggingEnded:(id<NSDraggingInfo>)sender;
/* the receiver of -wantsPeriodicDraggingUpdates should return NO if it does not require periodic -draggingUpdated messages (eg. not autoscrolling or otherwise dependent on draggingUpdated: sent while mouse is stationary) */
- (BOOL)wantsPeriodicDraggingUpdates;

/* While a destination may change the dragging images at any time, it is recommended to wait until this method is called before updating the dragging image. This allows the system to delay changing the dragging images until it is likely that the user will drop on this destination. Otherwise, the dragging images will change too often during the drag which would be distracting to the user. The destination may update the dragging images by calling one of the -enumerateDraggingItems methods on the sender.
*/
- (void)updateDraggingItemsForDrag:(nullable id <NSDraggingInfo>)sender API_AVAILABLE(macos(10.7));
@end


@protocol NSDraggingSource <NSObject>
/* Methods implemented by an object that initiates a drag session. The source application is sent these messages during dragging.  The first must be implemented, the others are sent if the source responds to them.
*/

@required 
/* Declares what types of operations the source allows to be performed. Apple may provide more specific "within" values in the future. To account for this, for unrecongized localities, return the operation mask for the most specific context that you are concerned with. For example:
    switch(context) {
        case NSDraggingContextOutsideApplication:
            return ...
            break;

        case NSDraggingContextWithinApplication:
        default:
            return ...
            break;
    }
*/
- (NSDragOperation)draggingSession:(NSDraggingSession *)session sourceOperationMaskForDraggingContext:(NSDraggingContext)context;

@optional
- (void)draggingSession:(NSDraggingSession *)session willBeginAtPoint:(NSPoint)screenPoint;
- (void)draggingSession:(NSDraggingSession *)session movedToPoint:(NSPoint)screenPoint;
- (void)draggingSession:(NSDraggingSession *)session endedAtPoint:(NSPoint)screenPoint operation:(NSDragOperation)operation;

- (BOOL)ignoreModifierKeysForDraggingSession:(NSDraggingSession *)session;

@end


typedef NS_OPTIONS(NSUInteger, NSSpringLoadingOptions) {
    NSSpringLoadingDisabled	= 0,		// Spring loading disabled
    NSSpringLoadingEnabled	= 1UL << 0,	// Spring loading allowed (via Force Click release and hover timeout depending on user preferences)
    
    /* Activate spring loading when user enters Force Click as opposed to release from Force Click. Spring loading is de-activated when Force Click is released. If hover is enabled, spring loading is activated on hover timeout and deactivates when the drag exits the target. This option should be used sparingly. */
    NSSpringLoadingContinuousActivation    = 1UL << 1,
    
    /* Disable spring loading activation via hover. This option should be used sparingly, but there are some rare cases where spring loading via hover results in too many false positives, but is otherwise very useful with a Force Click. These are generally cases when the destination view is very large, such as a canvas. */
    NSSpringLoadingNoHover 	= 1UL << 3
} API_AVAILABLE(macos(10.11));

@protocol NSSpringLoadingDestination <NSObject>
@required
/* Perform the spring loading action (For example, the button's action, or select the tab). Normally, spring loading is a discrete action that only activates after the user completes the spring loading input. When the NSSpringLoadingContinuousActivation option set, spring loading become a continuous action that activates (YES) when the user starts spring loading and then deactivates (NO) when the user releases spring loading. See NSSpringLoadingContinuousActivation for more information.
 */
- (void)springLoadingActivated:(BOOL)activated draggingInfo:(id <NSDraggingInfo>)draggingInfo API_AVAILABLE(macos(10.11));

/* Called when the spring loading highlight changes */
- (void)springLoadingHighlightChanged:(id <NSDraggingInfo>)draggingInfo API_AVAILABLE(macos(10.11));

/* Note: You must also implement either -springLoadingEntered: or -springLoadingUpdated: */

@optional
/* Called when a drag enters the spring loading destination. Return NSSpringLoadingEnabled to enable spring loading. A view is not considered valid spring loading drag destination if neither this method nor springLoadingUpdated: is implemented */
- (NSSpringLoadingOptions)springLoadingEntered:(id <NSDraggingInfo>)draggingInfo API_AVAILABLE(macos(10.11));

/* Called when a drag moves or the drag info changes. If this method is not implemented, then the value from -springLoadingEntered: is used. */
- (NSSpringLoadingOptions)springLoadingUpdated:(id <NSDraggingInfo>)draggingInfo API_AVAILABLE(macos(10.11));

/* Called when a drag exits the spring loading destination */
- (void)springLoadingExited:(id <NSDraggingInfo>)draggingInfo API_AVAILABLE(macos(10.11));

/* The drag & drop operation has ended. Un-spring if needed. Note: If this obejct is both an NSSpringLoadingDestination and NSDraggingDestination, draggingEnded: will only be called once. */
- (void)draggingEnded:(id <NSDraggingInfo>)draggingInfo API_AVAILABLE(macos(10.11));
@end


@interface NSObject(NSDraggingSourceDeprecated)
- (nullable NSArray<NSString *> *)namesOfPromisedFilesDroppedAtDestination:(NSURL *)dropDestination API_DEPRECATED("Use NSFilePromiseProvider objects instead", macos(10.0,10.13));
- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)flag API_DEPRECATED("Use -draggingSession:sourceOperationMaskForDraggingContext: instead", macos(10.0,10.7));
- (void)draggedImage:(null_unspecified NSImage *)image beganAt:(NSPoint)screenPoint API_DEPRECATED("Use -draggingSession:willBeginAtPoint: instead", macos(10.0,10.7));
- (void)draggedImage:(null_unspecified NSImage *)image endedAt:(NSPoint)screenPoint operation:(NSDragOperation)operation API_DEPRECATED("Use -draggingSession:endedAtPoint:operation: instead", macos(10.0,10.7));
- (void)draggedImage:(null_unspecified NSImage *)image movedTo:(NSPoint)screenPoint API_DEPRECATED("Use -draggingSession:movedToPoint: instead", macos(10.0,10.7));
- (BOOL)ignoreModifierKeysWhileDragging API_DEPRECATED("Use -ignoreModifierKeysForDraggingSession: instead", macos(10.0,10.7));

- (void)draggedImage:(null_unspecified NSImage *)image endedAt:(NSPoint)screenPoint deposited:(BOOL)flag DEPRECATED_IN_MAC_OS_X_VERSION_10_1_AND_LATER;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
