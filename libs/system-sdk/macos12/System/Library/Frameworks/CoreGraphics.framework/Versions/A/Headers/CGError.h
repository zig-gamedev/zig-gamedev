/* CoreGraphics - CGError.h
   Copyright (c) 2000-2011 Apple Inc.
   All rights reserved. */

#ifndef CGERROR_H_
#define CGERROR_H_

#include <CoreGraphics/CGBase.h>
#include <stdint.h>

#include <CoreFoundation/CFAvailability.h>


/* Types used for errors and error handlers. */

typedef CF_ENUM (int32_t, CGError) {
  kCGErrorSuccess = 0,
  kCGErrorFailure = 1000,
  kCGErrorIllegalArgument = 1001,
  kCGErrorInvalidConnection = 1002,
  kCGErrorInvalidContext = 1003,
  kCGErrorCannotComplete = 1004,
  kCGErrorNotImplemented = 1006,
  kCGErrorRangeCheck = 1007,
  kCGErrorTypeCheck = 1008,
  kCGErrorInvalidOperation = 1010,
  kCGErrorNoneAvailable = 1011,
};

typedef void (*CGErrorCallback)(void);

/* Set a callback for easier detection of error conditions
   causing CoreGraphics to raise an error.
   Pass NULL to reset the callback.*/

CG_EXTERN void CGErrorSetCallback(CGErrorCallback callback);

#endif /* CGERROR_H_ */
