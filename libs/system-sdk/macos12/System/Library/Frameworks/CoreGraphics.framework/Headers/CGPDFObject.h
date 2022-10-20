/* CoreGraphics - CGPDFObject.h
   Copyright (c) 2002-2011 Apple Inc.
   All rights reserved. */

#ifndef CGPDFOBJECT_H_
#define CGPDFOBJECT_H_

#include <CoreGraphics/CGBase.h>
#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* A type for boolean values. */

typedef unsigned char CGPDFBoolean;

/* A type for integer values. */

typedef long int CGPDFInteger;

/* A type for real values. */

typedef CGFloat CGPDFReal;

/* A type to hold any object. */

typedef struct CGPDFObject *CGPDFObjectRef;

/* An identifier to describe an object's type. */

typedef CF_ENUM (int32_t, CGPDFObjectType) {
  kCGPDFObjectTypeNull = 1,
  kCGPDFObjectTypeBoolean,
  kCGPDFObjectTypeInteger,
  kCGPDFObjectTypeReal,
  kCGPDFObjectTypeName,
  kCGPDFObjectTypeString,
  kCGPDFObjectTypeArray,
  kCGPDFObjectTypeDictionary,
  kCGPDFObjectTypeStream
};

/* Return the type of `object'. */

CG_EXTERN CGPDFObjectType CGPDFObjectGetType(CGPDFObjectRef cg_nullable object)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Get the value of `object'. If the type of `object' is equal to `type',
   then copy the value of `object' to `value' (if it's non-NULL) and return
   true. Otherwise, if the type of `object' is `kCGPDFObjectTypeInteger' and
   `type' is equal to `kCGPDFObjectTypeReal', then convert the value of
   `object' to floating point and copy the result to `value' (if it's
   non-NULL) and return true. Otherwise, return false. */

CG_EXTERN bool CGPDFObjectGetValue(CGPDFObjectRef cg_nullable object,
    CGPDFObjectType type, void * __nullable value)
    CG_AVAILABLE_STARTING(10.3, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFOBJECT_H_ */
