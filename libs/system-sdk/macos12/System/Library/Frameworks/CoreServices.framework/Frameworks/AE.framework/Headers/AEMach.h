/*
     File:       AE/AEMach.h
 
     Contains:   AppleEvent over mach_msg interfaces
 
    
 
     Copyright:  � 2000-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AEMACH__
#define __AEMACH__

#ifndef __CARBONCORE__
#include <CarbonCore/CarbonCore.h>
#endif

#ifndef __AEDATAMODEL__
#include <AE/AEDataModel.h>
#endif


#if TARGET_RT_MAC_MACHO
#ifdef __cplusplus
extern "C" {
#endif
#include <mach/message.h>
#ifdef __cplusplus
}
#endif
#endif

#include <os/availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*-
 * AE Mach API --
 *
 * AppleEvents on OS X are implemented in terms of mach messages.
 * To facilitate writing server processes that can send and receive
 * AppleEvents, the following APIs are provided.
 *
 * AppleEvents are directed to a well known port uniquely tied to a
 * process.  The AE framework will discover this port based on the
 * keyAddressAttr of the event (as specifed in AECreateAppleEvent by
 * the target parameter.)  If a port cannot be found,
 * procNotFound (-600) will be returned on AESend.
 *
 * Of note is a new attribute for an AppleEvent, keyReplyPortAttr.
 * This specifies the mach_port_t to which an AppleEvent reply
 * should be directed.  By default, replies are sent to the
 * processes' registered port where they are culled from the normal  
 * event stream if there is an outstanding AESend + kAEWaitReply.
 * But it may be desirable for a client to specify their own port to
 * receive queud replies.
 *
 * In the case of AESendMessage with kAEWaitReply specified, an 
 * anonymous port will be used to block until the reply is received.
 *
 * Not supplied is a convenience routine to block a server and
 * process AppleEvents.  This implementation will be detailed in a
 * tech note.
 *
 * In general, the AppleEvent APIs are thread safe, but the mechanism
 * of their delivery (AEProcessAppleEvent, AEResumeTheCurrentEvent)
 * are not.  If you're attemping to write a thread safe server, you
 * should avoid AppleEvent routines that don't explicitly state their
 * thread safety.
 *
 **/
CF_ENUM(AEKeyword) {
  keyReplyPortAttr              = 'repp'
};

/* typeReplyPortAttr was misnamed and is deprecated; use keyReplyPortAttr instead. */
CF_ENUM(DescType) {
  typeReplyPortAttr             = keyReplyPortAttr
};

/*
 *  AEGetRegisteredMachPort()
 *  
 *  Discussion:
 *    Return the mach_port_t that was registered by the AppleEvent
 *    framework for this process.  This port is considered public, and
 *    will be used by other applications to target your process.  You
 *    are free to use this mach_port_t to add to a port set, if and
 *    only if, you are not also using routines from HIToolbox.  In that
 *    case, HIToolbox retains control of this port and AppleEvents are
 *    dispatched through the main event loop.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern mach_port_t 
AEGetRegisteredMachPort(void)                                 API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEDecodeMessage()
 *  
 *  Discussion:
 *    Decode a mach_msg into an AppleEvent and its related reply.  (The
 *    reply is set up from fields of the event.)  You can call this
 *    routine if you wish to dispatch or handle the event yourself.  To
 *    return a reply to the sender, you should call:
 *     AESendMessage(reply, NULL, kAENoReply, kAENormalPriority,
 *    kAEDefaultTimeout);
 *    If this message is a reply, the 'reply' parameter will be
 *    initialized to { typeNull, 0 }, and the 'event' parameter will be
 *    the AppleEvent reply with a event class attribute of
 *    typeAppleEvent, class typeAppleEventReply:
 *    The contents of the header are invalid after this call.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    header:
 *      The incoming mach message to be dispatched
 *    
 *    event:
 *      The AppleEvent to decode the message in header into
 *    
 *    reply:
 *      The AppleEvent reply is decoded into reply
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEDecodeMessage(
  mach_msg_header_t *  header,
  AppleEvent *         event,
  AppleEvent *         reply)        /* can be NULL */        API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEProcessMessage()
 *  
 *  Discussion:
 *    Decodes and dispatches an event to an event handler.  Handles
 *    packaging and returning the reply to the sender.
 *    The contents of the header are invalid after this call.
 *  
 *  Mac OS X threading:
 *    Not thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    header:
 *      The incoming mach message to be dispatched.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEProcessMessage(mach_msg_header_t * header)                  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AESendMessage()
 *  
 *  Discussion:
 *    Send an AppleEvent to a target process.  If the target is the
 *    current process (as specified by using typeProcessSerialNumber of
 *    { 0, kCurrentProcess } it is dispatched directly to the
 *    appropriate event handler in your process and not serialized.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    event:
 *      The event to be sent
 *    
 *    reply:
 *      The reply for the event, if non-NULL
 *    
 *    sendMode:
 *      The mode to send the event
 *    
 *    timeOutInTicks:
 *      The timeout for sending the event, in ticks.  If 0, there is no
 *      timeout.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AESendMessage(
  const AppleEvent *  event,
  AppleEvent *        reply,                /* can be NULL */
  AESendMode          sendMode,
  long                timeOutInTicks)                         API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );




#ifdef __cplusplus
}
#endif

#endif /* __AEMACH__ */

