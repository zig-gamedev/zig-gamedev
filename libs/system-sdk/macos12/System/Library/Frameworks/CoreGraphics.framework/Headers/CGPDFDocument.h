/* CoreGraphics - CGPDFDocument.h
   Copyright (c) 2000-2011 Apple Inc.
   All rights reserved. */

#ifndef CGPDFDOCUMENT_H_
#define CGPDFDOCUMENT_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CF_BRIDGED_TYPE(id) CGPDFDocument *CGPDFDocumentRef;

#include <CoreGraphics/CGDataProvider.h> 
#include <CoreGraphics/CGGeometry.h> 
#include <CoreGraphics/CGPDFPage.h> 
#include <CoreFoundation/CFURL.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

// To get access permissions from a CGPDFDocument, call CGPDFDocumentGetAccessPermissions. Setting permissions
// can only be done using the kCGPDFContextAccessPermissions property in the auxiliary info dictionary passed
// in to CGPDFContextCreate.
//
// Some PDF access permissions are non-strict supersets of other permissions. Granting access to a superset
// permission also grants access to permission(s) it is a superset of. These permissions are as follows:
//
// * Granting kCGPDFAllowsHighQualityPrinting also grants kCGPDFAllowsLowQualityPrinting
// * Granting kCGPDFAllowsDocumentChanges also grants kCGPDFAllowsCommenting and kCGPDFAllowsFormFieldEntry
// * Granting kCGPDFAllowsContentCopying also grants kCGPDFAllowsContentAccessibility
// * Granting kCGPDFAllowsCommenting also grants kCGPDFAllowsFormFieldEntry
//
// These relationships don't come from the PDF spec. There is nothing in the spec that says you always get
// low-quality printing if you've already got high-quality printing. They are just two different bits with
// no explicit dependencies. However, in practice, the software that most people use to edit PDF permissions
// only allows creation of PDFs that follow these conventions.
//
// In the interest of consistency with how these bits are used in practice, kCGPDFContextAccessPermissions
// enforces the superset model of PDF permissions. For example, if you try to grant high-quality printing but
// not low-quality printing, the low-quality bit will get turned on anyway. This does not apply to how permissions
// are read, however. It's very likely that there exist PDFs that don't follow these conventions. For that reason,
// when checking permissions, always check for the narrowest one that applies. For example, if you're interested
// in form field entry, always test for kCGPDFAllowsFormFieldEntry specifically. You can't assume that you have it
// just because you also have kCGPDFAllowsCommenting or kCGPDFAllowsDocumentChanges.

typedef CF_OPTIONS(uint32_t, CGPDFAccessPermissions) {
    kCGPDFAllowsLowQualityPrinting    = (1 << 0),   // Print at up to 150 DPI
    kCGPDFAllowsHighQualityPrinting   = (1 << 1),   // Print at any DPI
    kCGPDFAllowsDocumentChanges       = (1 << 2),   // Modify the document contents except for page management
    kCGPDFAllowsDocumentAssembly      = (1 << 3),   // Page management: insert, delete, and rotate pages
    kCGPDFAllowsContentCopying        = (1 << 4),   // Extract content (text, images, etc.)
    kCGPDFAllowsContentAccessibility  = (1 << 5),   // Extract content, but only for the purpose of accessibility
    kCGPDFAllowsCommenting            = (1 << 6),   // Create or modify annotations, including form field entries
    kCGPDFAllowsFormFieldEntry        = (1 << 7)    // Modify form field entries
};

/* CFDictionary keys for use with CGPDFDocumentGetOutline and CGPDFContextSetOutline. */

// Outlines don't contain a single root node, however we have to represent outlines as trees with a single root.
// The root CFDictionary will only have one key (kCGPDFOutlineChildren), and it is not considered part of the
// outline itself. All CFDictionaries below the root must have at least two keys: kCGPDFOutlineTitle and
// kCGPDFOutlineDestination. If they have any children, they must have kCGPDFOutlineChildren as well. Finally,
// kCGPDFOutlineDestinationRect can optionally be present to associate an area of the destination page.

/* Key:   kCGPDFOutlineTitle */
/* Value: CFString */
CG_EXTERN const CFStringRef kCGPDFOutlineTitle
    CG_AVAILABLE_STARTING(10.13, 11.0);

/* Key:   kCGPDFOutlineChildren */
/* Value: CFArray of CFDictionaries */
CG_EXTERN const CFStringRef kCGPDFOutlineChildren
    CG_AVAILABLE_STARTING(10.13, 11.0);

/* Key:   kCGPDFOutlineDestination */
/* Value: CFNumber (for a one-indexed page number) or CFURL */
CG_EXTERN const CFStringRef kCGPDFOutlineDestination
    CG_AVAILABLE_STARTING(10.13, 11.0);

/* Key:   kCGPDFOutlineDestinationRect */
/* Value: CFDictionary from CGRectCreateDictionaryRepresentation. Only valid when the destination is a page number. */
CG_EXTERN const CFStringRef kCGPDFOutlineDestinationRect
    CG_AVAILABLE_STARTING(10.13, 11.0);

/* Create a PDF document, using `provider' to obtain the document's data. */

CG_EXTERN CGPDFDocumentRef __nullable CGPDFDocumentCreateWithProvider(
    CGDataProviderRef cg_nullable provider)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Create a PDF document from `url'. */

CG_EXTERN CGPDFDocumentRef __nullable CGPDFDocumentCreateWithURL(
    CFURLRef cg_nullable url)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Equivalent to `CFRetain(document)', except it doesn't crash (as CFRetain
   does) if `document' is NULL. */

CG_EXTERN CGPDFDocumentRef cg_nullable CGPDFDocumentRetain(
    CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Equivalent to `CFRelease(document)', except it doesn't crash (as
   CFRelease does) if `document' is NULL. */

CG_EXTERN void CGPDFDocumentRelease(CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the major and minor version numbers of `document'. */

CG_EXTERN void CGPDFDocumentGetVersion(CGPDFDocumentRef cg_nullable document,
  int *  majorVersion, int *  minorVersion)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return true if the PDF file associated with `document' is encrypted;
   false otherwise. If the PDF file is encrypted, then a password must be
   supplied before certain operations are enabled; different passwords may
   enable different operations. */

CG_EXTERN bool CGPDFDocumentIsEncrypted(CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Use `password' to decrypt `document' and grant permission for certain
   operations. Returns true if `password' is a valid password; false
   otherwise. */

CG_EXTERN bool CGPDFDocumentUnlockWithPassword(
    CGPDFDocumentRef cg_nullable document, const char *  password)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return true if `document' is unlocked; false otherwise. A document is
   unlocked if it isn't encrypted, or if it is encrypted and a valid
   password was specified with `CGPDFDocumentUnlockWithPassword'. */

CG_EXTERN bool CGPDFDocumentIsUnlocked(CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return true if `document' allows printing; false otherwise. Typically,
   this function returns false only if the document is encrypted and the
   document's current password doesn't grant permission to perform
   printing. */

CG_EXTERN bool CGPDFDocumentAllowsPrinting(CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return true if `document' allows copying; false otherwise. Typically,
   this function returns false only if the document is encrypted and the
   document's current password doesn't grant permission to perform
   copying. */

CG_EXTERN bool CGPDFDocumentAllowsCopying(CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the number of pages in `document'. */

CG_EXTERN size_t CGPDFDocumentGetNumberOfPages(
    CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the page corresponding to `pageNumber', or NULL if no such page
   exists in the document. Pages are numbered starting at 1. */

CG_EXTERN CGPDFPageRef __nullable CGPDFDocumentGetPage(
    CGPDFDocumentRef cg_nullable document, size_t pageNumber)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the document catalog of `document'. */

CG_EXTERN CGPDFDictionaryRef __nullable CGPDFDocumentGetCatalog(
    CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the info dictionary of `document'. */

CG_EXTERN CGPDFDictionaryRef __nullable CGPDFDocumentGetInfo(
    CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the "file identifier" array of `document'. */

CG_EXTERN CGPDFArrayRef __nullable CGPDFDocumentGetID(
    CGPDFDocumentRef cg_nullable document)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the CFTypeID for CGPDFDocumentRefs. */

CG_EXTERN CFTypeID CGPDFDocumentGetTypeID(void)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the document outline in the form of a CFDictionary tree containing the kCGPDFOutline keys. */

CG_EXTERN __nullable CFDictionaryRef CGPDFDocumentGetOutline(CGPDFDocumentRef document)
    CG_AVAILABLE_STARTING(10.13, 11.0);

/* Return the document access permissions. */

CG_EXTERN CGPDFAccessPermissions CGPDFDocumentGetAccessPermissions(CGPDFDocumentRef document)
    CG_AVAILABLE_STARTING(10.13, 11.0);

/* The following functions are deprecated in favor of the CGPDFPage API. */

/* DEPRECATED; return the media box of page number `page' in `document'.
   CGRectNull is returned if cannot find `page' in `document'. */

CG_EXTERN CGRect CGPDFDocumentGetMediaBox(CGPDFDocumentRef cg_nullable document,
    int page)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.5) CG_UNAVAILABLE_EMBEDDED;

/* DEPRECATED; return the crop box of page number `page' in `document'.
   CGRectNull is returned if cannot find `page' in `document'. */


CG_EXTERN CGRect CGPDFDocumentGetCropBox(CGPDFDocumentRef cg_nullable document,
    int page)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.5) CG_UNAVAILABLE_EMBEDDED;

/* DEPRECATED; return the bleed box of page number `page' in `document'.
   CGRectNull is returned if cannot find `page' in `document'. */

CG_EXTERN CGRect CGPDFDocumentGetBleedBox(CGPDFDocumentRef cg_nullable document,
    int page)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.5) CG_UNAVAILABLE_EMBEDDED;

/* DEPRECATED; return the trim box of page number `page' in `document'.
   CGRectNull is returned if cannot find `page' in `document'. */

CG_EXTERN CGRect CGPDFDocumentGetTrimBox(CGPDFDocumentRef cg_nullable document,
    int page)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.5) CG_UNAVAILABLE_EMBEDDED;

/* DEPRECATED; return the art box of page number `page' in `document'.
  CGRectNull is returned if cannot find `page' in `document'. */

CG_EXTERN CGRect CGPDFDocumentGetArtBox(CGPDFDocumentRef cg_nullable document,
    int page)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.5) CG_UNAVAILABLE_EMBEDDED;

/* DEPRECATED; return the rotation angle (in degrees) of page number `page'
   in `document'. 0 if if cannot find `page' in `document'.*/

CG_EXTERN int CGPDFDocumentGetRotationAngle(CGPDFDocumentRef cg_nullable document,
    int page)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.5) CG_UNAVAILABLE_EMBEDDED;

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFDOCUMENT_H_ */
