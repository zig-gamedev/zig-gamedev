/* CoreGraphics - CGDataProvider.h
   Copyright (c) 1999-2011 Apple Inc.
   All rights reserved. */

#ifndef CGDATAPROVIDER_H_
#define CGDATAPROVIDER_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CF_BRIDGED_TYPE(id) CGDataProvider *CGDataProviderRef;

#include <CoreGraphics/CGBase.h>
#include <CoreFoundation/CFURL.h>
#include <sys/types.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* This callback is called to copy `count' bytes from the sequential data
   stream to `buffer'. */

typedef size_t (*CGDataProviderGetBytesCallback)(void * __nullable info,
    void *  buffer, size_t count);

/* This callback is called to skip `count' bytes forward in the sequential
   data stream. It should return the number of bytes that were actually
   skipped. */

typedef off_t (*CGDataProviderSkipForwardCallback)(void * __nullable info,
    off_t count);

/* This callback is called to rewind to the beginning of sequential data
   stream. */

typedef void (*CGDataProviderRewindCallback)(void * __nullable info);

/* This callback is called to release the `info' pointer when the data
   provider is freed. */

typedef void (*CGDataProviderReleaseInfoCallback)(void * __nullable info);

/* Callbacks for sequentially accessing data.
   `version' is the version of this structure. It should be set to 0.
   `getBytes' is called to copy `count' bytes from the sequential data
     stream to `buffer'. It should return the number of bytes copied, or 0
     if there's no more data.
   `skipForward' is called to skip ahead in the sequential data stream by
     `count' bytes.
   `rewind' is called to rewind the sequential data stream to the beginning
     of the data.
   `releaseInfo', if non-NULL, is called to release the `info' pointer when
     the provider is freed. */

struct CGDataProviderSequentialCallbacks {
    unsigned int version;
    CGDataProviderGetBytesCallback __nullable getBytes;
    CGDataProviderSkipForwardCallback __nullable skipForward;
    CGDataProviderRewindCallback __nullable rewind;
    CGDataProviderReleaseInfoCallback __nullable releaseInfo;
};
typedef struct CGDataProviderSequentialCallbacks
    CGDataProviderSequentialCallbacks;

/*  This callback is called to get a pointer to the entire block of data. */

typedef const void * __nullable(*CGDataProviderGetBytePointerCallback)(
    void * __nullable info);

/* This callback is called to release the pointer to entire block of
   data. */

typedef void (*CGDataProviderReleaseBytePointerCallback)(
    void * __nullable info, const void *  pointer);

/* This callback is called to copy `count' bytes at byte offset `position'
   into `buffer'. */

typedef size_t (*CGDataProviderGetBytesAtPositionCallback)(
    void * __nullable info, void *  buffer, off_t pos, size_t cnt);

/* Callbacks for directly accessing data.
   `version' is the version of this structure. It should be set to 0.
   `getBytePointer', if non-NULL, is called to return a pointer to the
     provider's entire block of data. This callback may be called multiple
     times in proper sequence with `releaseBytePointer'. Data block does not
     need to be available until this callback is invoked.
   `releaseBytePointer', if non-NULL, is called to release a pointer to the
     provider's entire block of data. This callback may be called multiple
     times in proper sequence with `getBytePointer'. If possible, data should
     be purged with this callback and refilled with `getBytePointer'.
   `getBytesAtPosition', if non-NULL, is called to copy `count' bytes at
     offset `position' from the provider's data to `buffer'. It should
     return the number of bytes copied, or 0 if there's no more data.
   `releaseInfo', if non-NULL, is called to release the `info' pointer when
     the provider is freed.

   At least one of `getBytePointer' or `getBytesAtPosition' must be
   non-NULL. 
 
   If both `getBytePointer' are present `getBytesAtPosition', the latter one 
   may be ignored. */

struct CGDataProviderDirectCallbacks {
    unsigned int version;
    CGDataProviderGetBytePointerCallback __nullable getBytePointer;
    CGDataProviderReleaseBytePointerCallback __nullable releaseBytePointer;
    CGDataProviderGetBytesAtPositionCallback __nullable getBytesAtPosition;
    CGDataProviderReleaseInfoCallback __nullable releaseInfo;
};
typedef struct CGDataProviderDirectCallbacks CGDataProviderDirectCallbacks;

/* Return the CFTypeID for CGDataProviderRefs. */

CG_EXTERN CFTypeID CGDataProviderGetTypeID(void)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create a sequential-access data provider using `callbacks' to provide the
   data. `info' is passed to each of the callback functions. */

CG_EXTERN CGDataProviderRef __nullable CGDataProviderCreateSequential(
    void * __nullable info,
    const CGDataProviderSequentialCallbacks * cg_nullable callbacks)
    CG_AVAILABLE_STARTING(10.5, 2.0);

/* Create a direct-access data provider using `callbacks' to supply `size'
   bytes of data. `info' is passed to each of the callback functions.
   The underlying data must not change for the life of the data provider. */

CG_EXTERN CGDataProviderRef __nullable CGDataProviderCreateDirect(
    void * __nullable info, off_t size,
    const CGDataProviderDirectCallbacks * cg_nullable callbacks)
    CG_AVAILABLE_STARTING(10.5, 2.0);

/* The callback used by `CGDataProviderCreateWithData'. */

typedef void (*CGDataProviderReleaseDataCallback)(void * __nullable info,
    const void *  data, size_t size);

/* Create a direct-access data provider using `data', an array of `size'
   bytes. `releaseData' is called when the data provider is freed, and is
   passed `info' as its first argument. */

CG_EXTERN CGDataProviderRef __nullable CGDataProviderCreateWithData(
    void * __nullable info, const void * cg_nullable data, size_t size,
    CGDataProviderReleaseDataCallback cg_nullable releaseData)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Create a direct-access data provider which reads from `data'. */

CG_EXTERN CGDataProviderRef __nullable CGDataProviderCreateWithCFData(
    CFDataRef cg_nullable data)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Create a data provider reading from `url'. */

CG_EXTERN CGDataProviderRef __nullable CGDataProviderCreateWithURL(
    CFURLRef cg_nullable url)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Create a data provider reading from `filename'. */

CG_EXTERN CGDataProviderRef __nullable CGDataProviderCreateWithFilename(
    const char * cg_nullable filename)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Equivalent to `CFRetain(provider)', but doesn't crash (as CFRetain does)
   if `provider' is NULL. */

CG_EXTERN CGDataProviderRef __nullable CGDataProviderRetain(
    CGDataProviderRef cg_nullable provider)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Equivalent to `CFRelease(provider)', but doesn't crash (as CFRelease
   does) if `provider' is NULL. */

CG_EXTERN void CGDataProviderRelease(CGDataProviderRef cg_nullable provider)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return a copy of the data specified by provider. Returns NULL if a
   complete copy of the data can't be obtained (for example, if the
   underlying data is too large to fit in memory). */

CG_EXTERN CFDataRef __nullable CGDataProviderCopyData(
    CGDataProviderRef cg_nullable provider)
    CG_AVAILABLE_STARTING(10.3, 2.0);

CG_EXTERN void* __nullable CGDataProviderGetInfo(CGDataProviderRef cg_nullable provider)
    CG_AVAILABLE_STARTING(10.13, 11.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGDATAPROVIDER_H_ */
