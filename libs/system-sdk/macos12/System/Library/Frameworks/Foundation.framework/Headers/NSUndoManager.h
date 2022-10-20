/*	NSUndoManager.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/


//
// NSUndoManager is a general-purpose undo stack where clients can register
// callbacks to be invoked should an undo be requested.
//

#import <Foundation/NSObject.h>
#include <stdint.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSRunLoop.h>

@class NSArray<ObjectType>;
@class NSString;

NS_ASSUME_NONNULL_BEGIN

// used with NSRunLoop's performSelector:target:argument:order:modes:
static const NSUInteger NSUndoCloseGroupingRunLoopOrdering = 350000;

API_AVAILABLE(macos(10.0), ios(3.0), watchos(2.0), tvos(9.0))
@interface NSUndoManager : NSObject {
    @private
    id 			_undoStack;
    id 			_redoStack;
    NSArray 		*_runLoopModes;
    uint64_t		_NSUndoManagerPrivate1;
    id 			_target;
    id			_proxy;
    void		*_NSUndoManagerPrivate2;
    void		*_NSUndoManagerPrivate3;
}

        /* Begin/End Grouping */

- (void)beginUndoGrouping;
- (void)endUndoGrouping;
    // These nest.

@property (readonly) NSInteger groupingLevel;
    // Zero means no open group.

        /* Enable/Disable registration */

- (void)disableUndoRegistration;
- (void)enableUndoRegistration;
@property (readonly, getter=isUndoRegistrationEnabled) BOOL undoRegistrationEnabled;

        /* Groups By Event */

@property BOOL groupsByEvent;
    // If groupsByEvent is enabled, the undoManager automatically groups
    // all undos registered during a single NSRunLoop event together in
    // a single top-level group. This featured is enabled by default.

        /* Undo levels */

@property NSUInteger levelsOfUndo;
    // Sets the number of complete groups (not operations) that should
    // be kept my the manager.  When limit is reached, oldest undos are
    // thrown away.  0 means no limit !

        /* Run Loop Modes */

@property (copy) NSArray<NSRunLoopMode> *runLoopModes;

        /* Undo/Redo */

- (void)undo;
    // Undo until a matching begin. It terminates a top level undo if
    // necesary. Useful for undoing when groupByEvents is on (default is
    // on)
- (void)redo;
    // Will redo last top-level undo.

- (void)undoNestedGroup;
    // Undoes a nested grouping without first trying to close a top level
    // undo group.

@property (readonly) BOOL canUndo;
@property (readonly) BOOL canRedo;
    // returns whether or not the UndoManager has anything to undo or redo

@property (readonly, getter=isUndoing) BOOL undoing;
@property (readonly, getter=isRedoing) BOOL redoing;
    // returns whether or not the undo manager is currently in the process
    // of invoking undo or redo operations.

        /* remove */

- (void)removeAllActions;

- (void)removeAllActionsWithTarget:(id)target;

        /* Object based Undo */

- (void)registerUndoWithTarget:(id)target selector:(SEL)selector object:(nullable id)anObject;

        /* Invocation based undo */

- (id)prepareWithInvocationTarget:(id)target;
   // called as:
   // [[undoManager prepareWithInvocationTarget:self] setFont:oldFont color:oldColor]
   // When undo is called, the specified target will be called with
   // [target setFont:oldFont color:oldColor]

/*! @abstract records single undo operation for the specified target
    @param target non-nil target of the undo operation
    @param undoHandler non-nil block to be executed for the undo operation
    @discussion
      As with other undo operations, this does not strongly retain target. Care should be taken to avoid introducing retain cycles by other references captured by the block.
 */
- (void)registerUndoWithTarget:(id)target handler:(void (^)(id target))undoHandler API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) NS_REFINED_FOR_SWIFT;

- (void)setActionIsDiscardable:(BOOL)discardable API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
   // Set the latest undo action to discardable if it may be safely discarded when a document can not be saved for any reason. An example might be an undo action that changes the viewable area of a document. To find out if an undo group contains only discardable actions, look for the NSUndoManagerGroupIsDiscardableKey in the userInfo dictionary of the NSUndoManagerDidCloseUndoGroupNotification.

// This key is set on the user info dictionary of the NSUndoManagerDidCloseUndoGroupNotification, with a NSNumber boolean value of YES, if the undo group as a whole is discardable.
FOUNDATION_EXPORT NSString * const NSUndoManagerGroupIsDiscardableKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

@property (readonly) BOOL undoActionIsDiscardable API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
@property (readonly) BOOL redoActionIsDiscardable API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
   // Call to see if the next undo or redo action is discardable.

    	/* Undo/Redo action name */

@property (readonly, copy) NSString *undoActionName;
@property (readonly, copy) NSString *redoActionName;
    // Call undoActionName or redoActionName to get the name of the next action to be undone or redone.
    // Returns @"" if there is nothing to undo/redo or no action names were registered.

- (void)setActionName:(NSString *)actionName;
    // Call setActionName: to set the name of an action.
    // The actionName parameter can not be nil

    	/* Undo/Redo menu item title */

@property (readonly, copy) NSString *undoMenuItemTitle;
@property (readonly, copy) NSString *redoMenuItemTitle;
    // Call undoMenuItemTitle or redoMenuItemTitle to get the string for the undo or redo menu item.
    // In English they will return "Undo <action name>"/"Redo <action name>" or "Undo"/"Redo" if there is
    // nothing to undo/redo or no action names were set.

    	/* localization hooks */

- (NSString *)undoMenuTitleForUndoActionName:(NSString *)actionName;
- (NSString *)redoMenuTitleForUndoActionName:(NSString *)actionName;
    // The localization of the pattern is usually done by localizing the string patterns in
    // undo.strings. But undo/redoMenuTitleForUndoActionName can also be overridden if
    // localizing the pattern happens to not be sufficient.

@end

FOUNDATION_EXPORT NSNotificationName const NSUndoManagerCheckpointNotification API_AVAILABLE(macos(10.0), ios(3.0), watchos(2.0), tvos(9.0));
    // This is called before an undo group is begun or ended so any
    // clients that need to lazily register undos can do so in the
    // correct group.

FOUNDATION_EXPORT NSNotificationName const NSUndoManagerWillUndoChangeNotification API_AVAILABLE(macos(10.0), ios(3.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSNotificationName const NSUndoManagerWillRedoChangeNotification API_AVAILABLE(macos(10.0), ios(3.0), watchos(2.0), tvos(9.0));

FOUNDATION_EXPORT NSNotificationName const NSUndoManagerDidUndoChangeNotification API_AVAILABLE(macos(10.0), ios(3.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSNotificationName const NSUndoManagerDidRedoChangeNotification API_AVAILABLE(macos(10.0), ios(3.0), watchos(2.0), tvos(9.0));

FOUNDATION_EXPORT NSNotificationName const NSUndoManagerDidOpenUndoGroupNotification API_AVAILABLE(macos(10.0), ios(3.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSNotificationName const NSUndoManagerWillCloseUndoGroupNotification API_AVAILABLE(macos(10.0), ios(3.0), watchos(2.0), tvos(9.0));

// This notification is sent after an undo group closes. It should be safe to undo at this time.
FOUNDATION_EXPORT NSNotificationName const NSUndoManagerDidCloseUndoGroupNotification API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

NS_ASSUME_NONNULL_END
