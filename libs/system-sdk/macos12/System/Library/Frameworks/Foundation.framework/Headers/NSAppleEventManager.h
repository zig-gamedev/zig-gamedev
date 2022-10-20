/*
	NSAppleEventManager.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <CoreServices/CoreServices.h>
#import <Foundation/NSNotification.h>

@class NSAppleEventDescriptor;

NS_ASSUME_NONNULL_BEGIN

typedef const struct __NSAppleEventManagerSuspension* NSAppleEventManagerSuspensionID;

extern const double NSAppleEventTimeOutDefault;
extern const double NSAppleEventTimeOutNone;

extern NSNotificationName NSAppleEventManagerWillProcessFirstEventNotification;

@interface NSAppleEventManager : NSObject {
    @private
    BOOL _isPreparedForDispatch;
    char _padding[3];
}

// Get the pointer to the program's single NSAppleEventManager.
+ (NSAppleEventManager *)sharedAppleEventManager;

// Set or remove a handler for a specific kind of Apple Event.  The handler method should have the same signature as:
// - (void)handleAppleEvent:(NSAppleEventDescriptor *)event withReplyEvent:(NSAppleEventDescriptor *)replyEvent;
// When it is invoked, the value of the first parameter will be the event to be handled.  The value of the second parameter will be the reply event to fill in.  A reply event object will always be passed in (replyEvent will never be nil), but it should not be touched if the event sender has not requested a reply, which is indicated by [replyEvent descriptorType]==typeNull.
- (void)setEventHandler:(id)handler andSelector:(SEL)handleEventSelector forEventClass:(AEEventClass)eventClass andEventID:(AEEventID)eventID;
- (void)removeEventHandlerForEventClass:(AEEventClass)eventClass andEventID:(AEEventID)eventID;

// Given an event, reply event, and refCon of the sort passed into Apple event handler functions that can be registered with AEInstallEventHandler(), dispatch the event to a handler that has been registered with -setEventHandler:andSelector:forEventClass:andEventID:.
// This method is primarily meant for Cocoa's internal use.  It does not send events to other applications!
- (OSErr)dispatchRawAppleEvent:(const AppleEvent *)theAppleEvent withRawReply:(AppleEvent *)theReply handlerRefCon:(SRefCon)handlerRefCon;

// If an Apple event is being handled on the current thread (i.e., a handler that was registered with -setEventHandler:andSelector:forEventClass:andEventID: is being messaged at this instant or -setCurrentAppleEventAndReplyEventWithSuspensionID: has just been invoked), return the descriptor for the event.  Return nil otherwise.  The effects of mutating or retaining the returned descriptor are undefined, though it may be copied.
@property (nullable, readonly, retain) NSAppleEventDescriptor *currentAppleEvent;

// If an Apple event is being handled on the current thread (i.e., -currentAppleEvent would not return nil), return the corresponding reply event descriptor.  Return nil otherwise.  This descriptor, including any mutatations, will be returned to the sender of the current event when all handling of the event has been completed, if the sender has requested a reply.  The effects of retaining the descriptor are undefined; it may be copied, but mutations of the copy will not be returned to the sender of the current event.
@property (nullable, readonly, retain) NSAppleEventDescriptor *currentReplyAppleEvent;

// If an Apple event is being handled on the current thread (i.e., -currentAppleEvent would not return nil), suspend the handling of the event, returning an ID that must be used to resume the handling of the event.  Return zero otherwise.  The suspended event will no longer be the current event after this method has returned.
- (nullable NSAppleEventManagerSuspensionID)suspendCurrentAppleEvent NS_RETURNS_INNER_POINTER;

// Given a nonzero suspension ID returned by an invocation of -suspendCurrentAppleEvent, return the descriptor for the event whose handling was suspended.  The effects of mutating or retaining the returned descriptor are undefined, though it may be copied.  This method may be invoked in any thread, not just the one in which the corresponding invocation of -suspendCurrentAppleEvent occurred.
- (NSAppleEventDescriptor *)appleEventForSuspensionID:(NSAppleEventManagerSuspensionID)suspensionID;

// Given a nonzero suspension ID returned by an invocation of -suspendCurrentAppleEvent, return the corresponding reply event descriptor.  This descriptor, including any mutatations, will be returned to the sender of the suspended event when handling of the event is resumed, if the sender has requested a reply.  The effects of retaining the descriptor are undefined; it may be copied, but mutations of the copy will not be returned to the sender of the suspended event.  This method may be invoked in any thread, not just the one in which the corresponding invocation of -suspendCurrentAppleEvent occurred.
- (NSAppleEventDescriptor *)replyAppleEventForSuspensionID:(NSAppleEventManagerSuspensionID)suspensionID;

// Given a nonzero suspension ID returned by an invocation of -suspendCurrentAppleEvent, set the values that will be returned by subsequent invocations of -currentAppleEvent and -currentReplyAppleEvent to be the event whose handling was suspended and its corresponding reply event, respectively.  Redundant invocations of this method will be ignored.
- (void)setCurrentAppleEventAndReplyEventWithSuspensionID:(NSAppleEventManagerSuspensionID)suspensionID;

// Given a nonzero suspension ID returned by an invocation of -suspendCurrentAppleEvent, signal that handling of the suspended event may now continue.  This may result in the immediate sending of the reply event to the sender of the suspended event, if the sender has requested a reply.  If the suspension ID has been used in a previous invocation of -setCurrentAppleEventAndReplyEventWithSuspensionID: the effects of that invocation will be completely undone.  Subsequent invocations of other NSAppleEventManager methods using the same suspension ID are invalid.  This method may be invoked in any thread, not just the one in which the corresponding invocation of -suspendCurrentAppleEvent occurred.
- (void)resumeWithSuspensionID:(NSAppleEventManagerSuspensionID)suspensionID;

@end

NS_ASSUME_NONNULL_END
