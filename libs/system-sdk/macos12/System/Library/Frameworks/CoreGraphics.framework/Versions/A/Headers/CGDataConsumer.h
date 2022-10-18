/* CoreGraphics - CGDataConsumer.h
 * Copyright (c) 1999-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGDATACONSUMER_H_
#define CGDATACONSUMER_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CF_BRIDGED_TYPE(id) CGDataConsumer *CGDataConsumerRef;

#include <CoreGraphics/CGBase.h>
#include <CoreFoundation/CFURL.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* This callback is called to copy `count' bytes from `buffer' to the data
   consumer. */

typedef size_t (*CGDataConsumerPutBytesCallback)(void * __nullable info,
    const void *  buffer, size_t count);

/* This callback is called to release the `info' pointer when the data
   provider is freed. */

typedef void (*CGDataConsumerReleaseInfoCallback)(void * __nullable info);

/* Callbacks for writing data.
   `putBytes' copies `count' bytes from `buffer' to the consumer, and
     returns the number of bytes copied. It should return 0 if no more data
     can be written to the consumer.
   `releaseConsumer', if non-NULL, is called when the consumer is freed. */

struct CGDataConsumerCallbacks {
    CGDataConsumerPutBytesCallback __nullable putBytes;
    CGDataConsumerReleaseInfoCallback __nullable releaseConsumer;
};
typedef struct CGDataConsumerCallbacks CGDataConsumerCallbacks;

/* Return the CFTypeID for CGDataConsumerRefs. */

CG_EXTERN CFTypeID CGDataConsumerGetTypeID(void)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create a data consumer using `callbacks' to handle the data. `info' is
   passed to each of the callback functions. */

CG_EXTERN CGDataConsumerRef __nullable CGDataConsumerCreate(
    void * __nullable info, const CGDataConsumerCallbacks * cg_nullable cbks)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Create a data consumer which writes data to `url'. */

CG_EXTERN CGDataConsumerRef __nullable CGDataConsumerCreateWithURL(
    CFURLRef cg_nullable url)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Create a data consumer which writes to `data'. */

CG_EXTERN CGDataConsumerRef __nullable CGDataConsumerCreateWithCFData(
    CFMutableDataRef cg_nullable data)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Equivalent to `CFRetain(consumer)'. */

CG_EXTERN CGDataConsumerRef cg_nullable CGDataConsumerRetain(
    CGDataConsumerRef cg_nullable consumer)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Equivalent to `CFRelease(consumer)'. */

CG_EXTERN void CGDataConsumerRelease(cg_nullable CGDataConsumerRef consumer)
    CG_AVAILABLE_STARTING(10.0, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif  /* CGDATACONSUMER_H_ */
