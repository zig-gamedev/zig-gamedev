/* CoreGraphics - CGPDFPage.h
   Copyright (c) 2001-2011 Apple Inc.
   All rights reserved. */

#ifndef CGPDFPAGE_H_
#define CGPDFPAGE_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CF_BRIDGED_TYPE(id) CGPDFPage *CGPDFPageRef;

#include <CoreGraphics/CGAffineTransform.h> 
#include <CoreGraphics/CGPDFDictionary.h> 
#include <CoreGraphics/CGPDFDocument.h> 

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

typedef CF_ENUM (int32_t, CGPDFBox) {
  kCGPDFMediaBox = 0,
  kCGPDFCropBox = 1,
  kCGPDFBleedBox = 2,
  kCGPDFTrimBox = 3,
  kCGPDFArtBox = 4
};

/* Equivalent to `CFRetain(page)', except it doesn't crash (as CFRetain
   does) if `page' is NULL. */

CG_EXTERN CGPDFPageRef __nullable CGPDFPageRetain(CGPDFPageRef cg_nullable page)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Equivalent to `CFRelease(page)', except it doesn't crash (as CFRelease
   does) if `page' is NULL. */

CG_EXTERN void CGPDFPageRelease(CGPDFPageRef cg_nullable page)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the document of `page'. */

CG_EXTERN CGPDFDocumentRef __nullable CGPDFPageGetDocument(
    CGPDFPageRef cg_nullable page)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the page number of `page'. */

CG_EXTERN size_t CGPDFPageGetPageNumber(CGPDFPageRef cg_nullable page)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the rectangle associated with `box' in `page'. This is the value
   of the corresponding entry (such as /MediaBox, /ArtBox, and so on) in the
   page's dictionary. Return CGRectNull if `page' is not a valid CGPDFPageRef
   or `box' is not a valid CGPDFBox. */

CG_EXTERN CGRect CGPDFPageGetBoxRect(CGPDFPageRef cg_nullable page, CGPDFBox box)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the rotation angle (in degrees) of `page'. This is the value of
   the /Rotate entry in the page's dictionary. Return 0 if `page' is not a valid
   CGPDFPageRef. */

CG_EXTERN int CGPDFPageGetRotationAngle(CGPDFPageRef cg_nullable page)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return a transform mapping the box specified by `box' to `rect' as
   follows:
     - Compute the effective rect by intersecting the rect associated with
       `box' and the /MediaBox entry of the page.
     - Rotate the effective rect according to the page's /Rotate entry.
     - Center the resulting rect on `rect'. If `rotation' is non-zero, then
       the rect will rotated clockwise by `rotation' degrees. `rotation'
       must be a multiple of 90.
     - Scale the rect down, if necessary, so that it coincides with the
       edges of `rect'. If `preserveAspectRatio' is true, then the final
       rect will coincide with the edges of `rect' only in the more
       restrictive dimension. */

CG_EXTERN CGAffineTransform CGPDFPageGetDrawingTransform(
    CGPDFPageRef cg_nullable page, CGPDFBox box, CGRect rect, int rotate,
    bool preserveAspectRatio)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the dictionary of `page'. */

CG_EXTERN CGPDFDictionaryRef __nullable CGPDFPageGetDictionary(
    CGPDFPageRef cg_nullable page)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the CFTypeID for CGPDFPageRefs. */

CG_EXTERN CFTypeID CGPDFPageGetTypeID(void)
    CG_AVAILABLE_STARTING(10.3, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFPAGE_H_ */
