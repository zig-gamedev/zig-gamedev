/* CoreGraphics - CGSession.h
   Copyright (c) 2003-2008 Apple Inc.
   All rights reserved. */

#ifndef CGSESSION_H_
#define CGSESSION_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

#include <CoreGraphics/CGBase.h>
#include <CoreFoundation/CoreFoundation.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Return a window server session dictionary, or NULL if the caller is not
   running within a Quartz GUI session or the window server is disabled. You
   should release the dictionary when you are finished using it. */

CG_EXTERN CFDictionaryRef __nullable CGSessionCopyCurrentDictionary(void)
  CG_AVAILABLE_STARTING(10.3);

/* Keys for window server session dictionaries. */

/* The user ID for the session's current user, specified as a CFNumber
   32-bit unsigned integer value. */

#define kCGSessionUserIDKey		CFSTR("kCGSSessionUserIDKey")

/* The session's short user name as set by the login operation, specified as
   a CFString value. */

#define kCGSessionUserNameKey		CFSTR("kCGSSessionUserNameKey")

/* The set of hardware composing a console, specified as a CFNumber 32-bit
   unsigned integer value. */

#define kCGSessionConsoleSetKey		CFSTR("kCGSSessionConsoleSetKey")

/* An indication of whether the session is on a console, specified as a
   CFBoolean value. */

#define kCGSessionOnConsoleKey		CFSTR("kCGSSessionOnConsoleKey")

/* An indication of whether the login operation has been done, specified as
   a CFBoolean value. */

#define kCGSessionLoginDoneKey		CFSTR("kCGSessionLoginDoneKey")

/* When the GUI session on a console changes, this notification is posted
   via `notify_post'. */

#define kCGNotifyGUIConsoleSessionChanged				\
  "com.apple.coregraphics.GUIConsoleSessionChanged"

/* When a user logs in or out of a session, this notification is posted via
   `notify_post'. */

#define kCGNotifyGUISessionUserChanged					\
  "com.apple.coregraphics.GUISessionUserChanged"

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGSESSION_H_ */
