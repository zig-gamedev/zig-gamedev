/* CoreGraphics - CGPDFArray.h
 * Copyright (c) 2002-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGPDFARRAY_H_
#define CGPDFARRAY_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CGPDFArray *CGPDFArrayRef;

#include <CoreGraphics/CGPDFDictionary.h>
#include <CoreGraphics/CGPDFObject.h>
#include <CoreGraphics/CGPDFStream.h>
#include <CoreGraphics/CGPDFString.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

#/* Return the number of items in `array'. */

CG_EXTERN size_t CGPDFArrayGetCount(CGPDFArrayRef cg_nullable array)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and return the result in
   `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetObject(CGPDFArrayRef cg_nullable array, size_t index,
    CGPDFObjectRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's a null, return
   true; otherwise, return false. */

CG_EXTERN bool CGPDFArrayGetNull(CGPDFArrayRef cg_nullable array, size_t index)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's a boolean, return
   the result in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetBoolean(CGPDFArrayRef cg_nullable array,
    size_t index, CGPDFBoolean * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's an integer, return
   the result in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetInteger(CGPDFArrayRef cg_nullable array,
    size_t index, CGPDFInteger * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's a number (real or
   integer), return the result in `value'. Return true on success; false
   otherwise. */

CG_EXTERN bool CGPDFArrayGetNumber(CGPDFArrayRef cg_nullable array,
    size_t index, CGPDFReal * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's a name, return the
   result in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetName(CGPDFArrayRef cg_nullable array,
    size_t index, const char * __nullable *  __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's a string, return
   the result in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetString(CGPDFArrayRef cg_nullable array,
    size_t index, CGPDFStringRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's an array, return it
   in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetArray(CGPDFArrayRef cg_nullable array,
    size_t index, CGPDFArrayRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's a dictionary,
   return it in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetDictionary(CGPDFArrayRef cg_nullable array,
    size_t index, CGPDFDictionaryRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Look up the object at `index' in `array' and, if it's a stream, return it
   in `value'. Return true on success; false otherwise. */

CG_EXTERN bool CGPDFArrayGetStream(CGPDFArrayRef cg_nullable array,
    size_t index, CGPDFStreamRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* The block function for `CGPDFArrayApplyBlock'. `index' is the current
   iterator index, `value' is the CGPDFObject at this index. If the block
   returns true, the iteration of the array will continue, otherwise if
   the block returns false, the iterator will stop and return early. */

typedef bool (^CGPDFArrayApplierBlock)(size_t index,
    CGPDFObjectRef value, void * __nullable info);

/* Enumerate all of the objects in `array', calling the block-function `block' once
   for each value in the array. Passes the current iterator index, the current
   value, and `info' to `block'. If `block' returns true, iteration will continue
   over all values. If `block' returns false, iteration will stop and return
   early. */

CG_EXTERN void CGPDFArrayApplyBlock(CGPDFArrayRef cg_nullable array,
    CGPDFArrayApplierBlock cg_nullable block, void * __nullable info)
    CG_AVAILABLE_STARTING(10.14, 12.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFARRAY_H_ */
