/* CoreGraphics - CGPDFDictionary.h
 * Copyright (c) 2002-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGPDFDICTIONARY_H_
#define CGPDFDICTIONARY_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CGPDFDictionary *CGPDFDictionaryRef;

#include <CoreGraphics/CGPDFArray.h>
#include <CoreGraphics/CGPDFObject.h>
#include <CoreGraphics/CGPDFStream.h>
#include <CoreGraphics/CGPDFString.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Return the number of entries in `dictionary'. */

CG_EXTERN size_t CGPDFDictionaryGetCount(CGPDFDictionaryRef cg_nullable dict)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and return the result
   in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFDictionaryGetObject(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, CGPDFObjectRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and, if it's a
   boolean, return the result in `value'. Return true on success; false
   otherwise. */

CG_EXTERN bool CGPDFDictionaryGetBoolean(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, CGPDFBoolean * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and, if it's an
   integer, return the result in `value'. Return true on success; false
   otherwise. */

CG_EXTERN bool CGPDFDictionaryGetInteger(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, CGPDFInteger * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and, if it's a number
   (real or integer), return the result in `value'. Return true on success;
   false otherwise. */

CG_EXTERN bool CGPDFDictionaryGetNumber(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, CGPDFReal * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and, if it's a name,
   return the result in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFDictionaryGetName(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, const char * __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and, if it's a string,
   return the result in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFDictionaryGetString(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, CGPDFStringRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and, if it's an array,
   return the result in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFDictionaryGetArray(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, CGPDFArrayRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and, if it's a
   dictionary, return the result in `value'. Return true on success; false
   otherwise. */

CG_EXTERN bool CGPDFDictionaryGetDictionary(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, CGPDFDictionaryRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object associated with `key' in `dict' and, if it's a stream,
   return the result in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFDictionaryGetStream(CGPDFDictionaryRef cg_nullable dict,
    const char *  key, CGPDFStreamRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* The callback for `CGPDFDictionaryApplyFunction'. `key' is the current
   key, `value' is the value for `key', and `info' is the parameter passed
   to `CGPDFDictionaryApplyFunction'. */

typedef void (*CGPDFDictionaryApplierFunction)(const char *  key,
    CGPDFObjectRef  value, void * __nullable info);

/* Enumerate all of the keys in `dict', calling `function' once for each
   key/value pair. Passes the current key, the associated value, and `info'
   to `function'. */

CG_EXTERN void CGPDFDictionaryApplyFunction(CGPDFDictionaryRef cg_nullable dict,
    CGPDFDictionaryApplierFunction cg_nullable function, void * __nullable info)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* The block function for `CGPDFDictionaryApplyBlock'. `key' is the current
   key, `value' is the value for `key', and `info' is the parameter passed
   to `CGPDFDictionaryApplyBlock'. If the block returns true, the iteration
   of the dictionary will continue, otherwise if the block returns false,
   the iterator will stop and return early. */

typedef bool (^CGPDFDictionaryApplierBlock)(const char * key,
    CGPDFObjectRef value, void * __nullable info);

/* Enumerate all of the keys in `dict', calling the block-function `block' once
   for each key/value pair. Passes the current key, the associated value, and
   `info' to `block'. If `block' returns true, iteration will continue over all
   key/value pairs. If `block' returns false, iteration will stop and return
   early. */

CG_EXTERN void CGPDFDictionaryApplyBlock(CGPDFDictionaryRef cg_nullable dict,
    CGPDFDictionaryApplierBlock cg_nullable block, void * __nullable info)
    CG_AVAILABLE_STARTING(10.14, 12.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFDICTIONARY_H_ */
