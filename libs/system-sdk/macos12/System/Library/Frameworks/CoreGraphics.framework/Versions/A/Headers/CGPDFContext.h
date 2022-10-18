/* CoreGraphics - CGPDFContext.h
   Copyright (c) 2000-2011 Apple Inc.
   All rights reserved. */

#ifndef CGPDFCONTEXT_H_
#define CGPDFCONTEXT_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGContext.h>
#include <CoreGraphics/CGDataConsumer.h>
#include <CoreFoundation/CFDictionary.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN


/* Create a PDF context, using `consumer' for output. `mediaBox' is the
   default page media bounding box; if NULL, then a default page size is
   used. `auxiliaryInfo' specifies additional information used by the PDF
   context when generating the PDF file. The keys and values in
   `auxiliaryInfo' are described below. If `mediaBox' is non-NULL, then its
   value overrides the value of `kCGPDFContextMediaBox' if specified in the
   `auxiliaryInfo' dictionary. */

CG_EXTERN CGContextRef __nullable CGPDFContextCreate(CGDataConsumerRef cg_nullable consumer,
  const CGRect *__nullable mediaBox, CFDictionaryRef __nullable auxiliaryInfo)
  CG_AVAILABLE_STARTING(10.0, 2.0);

/* Create a PDF context for writing to `url'. This function behaves in the
   same manner as the above function, except that the output data will be
   written to `url'. */

CG_EXTERN CGContextRef __nullable CGPDFContextCreateWithURL(CFURLRef cg_nullable url,
  const CGRect * __nullable mediaBox, CFDictionaryRef __nullable auxiliaryInfo)
  CG_AVAILABLE_STARTING(10.0, 2.0);

/* Close a PDF context. After closing the context, all pending data is
   written to the context's destination, and the PDF file is completed. No
   additional data will be written to the context's destionation after
   closing. */

CG_EXTERN void CGPDFContextClose(CGContextRef cg_nullable context)
  CG_AVAILABLE_STARTING(10.5, 2.0);

/* Begin a new page in the PDF context `context'. */

CG_EXTERN void CGPDFContextBeginPage(CGContextRef cg_nullable context,
  CFDictionaryRef __nullable pageInfo) CG_AVAILABLE_STARTING(10.4, 2.0);

/* End the current page in the PDF context `context'. */

CG_EXTERN void CGPDFContextEndPage(CGContextRef cg_nullable context)
  CG_AVAILABLE_STARTING(10.4, 2.0);

/* Add the metadata stream specified by `metadata' to the document catalog
   of `context', as described in Table 3.25, "Entries in the catalog
   dictionary", of the PDF 1.7 specification. The contents of metadata must
   be XML formatted according to the Extensible Metadata Platform, as
   described in section 10.2.2, "Metadata Streams", of the PDF 1.7
   specification. */

CG_EXTERN void CGPDFContextAddDocumentMetadata(CGContextRef cg_nullable context,
  CFDataRef __nullable metadata) CG_AVAILABLE_STARTING(10.7, 4.0);

/* Set the URL associated with `rect' to `url' in the PDF context
   `context'. */

CG_EXTERN void CGPDFContextSetURLForRect(CGContextRef cg_nullable context, CFURLRef  url,
  CGRect rect) CG_AVAILABLE_STARTING(10.4, 2.0);

/* Create a PDF destination named `name' at `point' in the current page of
   the PDF context `context'. */

CG_EXTERN void CGPDFContextAddDestinationAtPoint(CGContextRef cg_nullable context,
  CFStringRef  name, CGPoint point) CG_AVAILABLE_STARTING(10.4, 2.0);

/* Specify a destination named `name' to jump to when clicking in `rect' of
   the current page of the PDF context `context'. */

CG_EXTERN void CGPDFContextSetDestinationForRect(CGContextRef cg_nullable context,
  CFStringRef  name, CGRect rect) CG_AVAILABLE_STARTING(10.4, 2.0);

/*** Keys for the auxiliary info dictionary or the page info dictionary. ***/ 

/* The media box for the document or for a given page. Optional; if present,
   the value of this key must be a CFData containing a CGRect (stored by
   value, not by reference). */

CG_EXTERN const CFStringRef  kCGPDFContextMediaBox
  CG_AVAILABLE_STARTING(10.4, 2.0);

/* The crop box for the document or for a given page. Optional; if present,
   the value of this key must be a CFData containing a CGRect (stored by
   value, not by reference). */

CG_EXTERN const CFStringRef  kCGPDFContextCropBox CG_AVAILABLE_STARTING(10.4, 2.0);

/* The bleed box for the document or for a given page. Optional; if present,
   the value of this key must be a CFData containing a CGRect (stored by
   value, not by reference). */

CG_EXTERN const CFStringRef  kCGPDFContextBleedBox CG_AVAILABLE_STARTING(10.4, 2.0);

/* The trim box for the document or for a given page. Optional; if present,
   the value of this key must be a CFData containing a CGRect (stored by
   value, not by reference). */

CG_EXTERN const CFStringRef  kCGPDFContextTrimBox CG_AVAILABLE_STARTING(10.4, 2.0);

/* The art box for the document or for a given page. Optional; if present,
   the value of this key must be a CFData containing a CGRect (stored by
   value, not by reference). */

CG_EXTERN const CFStringRef  kCGPDFContextArtBox CG_AVAILABLE_STARTING(10.4, 2.0);

/*** Keys for auxiliary info dictionary. ***/

/* The document's title. Optional; if present, the value of this key must be
   a CFString. */

CG_EXTERN const CFStringRef  kCGPDFContextTitle CG_AVAILABLE_STARTING(10.4, 2.0);

/* The name of the person who created this document. Optional; if present,
   the value of this key must be a CFString. */

CG_EXTERN const CFStringRef  kCGPDFContextAuthor CG_AVAILABLE_STARTING(10.4, 2.0);

/* The subject of a document. Optional; if present, the value of this key
   must be a CFString. */

CG_EXTERN const CFStringRef  kCGPDFContextSubject CG_AVAILABLE_STARTING(10.5, 2.0);

/* The keywords for this document. This key is optional. If the value of
   this key is a CFString, the /Keywords entry will be the specified string.
   If the value of this key is a CFArray, then it must be an array of
   CFStrings. The /Keywords entry will in this case be the concatenation of
   the specified strings separated by commas (","). In addition, an entry
   with the key "/AAPL:Keywords" will be stored in the document information
   dictionary; its value is an array consisting of each of the specified
   strings. The value of this key must be in one of the above forms;
   otherwise, this key is ignored. */

CG_EXTERN const CFStringRef  kCGPDFContextKeywords CG_AVAILABLE_STARTING(10.5, 2.0);

/* The name of the application that created the original data used to create
   this document. Optional; if present, the value of this key must be a
   CFString. */

CG_EXTERN const CFStringRef  kCGPDFContextCreator CG_AVAILABLE_STARTING(10.4, 2.0);

/* The "owner password" of the PDF document. If this key is specified, the
   document will be encrypted using the value as the owner password;
   otherwise, the document will not be encrypted. The value of this key must
   be a CFStringRef which can be represented in ASCII encoding; only the
   first 32 bytes will be used for the password. There is no default value
   for this key.

   If the value of this key cannot be represented in ASCII, the document
   will not be created and the creation function will return NULL. */

CG_EXTERN const CFStringRef  kCGPDFContextOwnerPassword CG_AVAILABLE_STARTING(10.4, 2.0);

/* The "user password" of the PDF document. If the document is encrypted,
   then the value of this key will be the user password for the document; if
   unspecified, the user password will be the empty string. The value of
   this key must be a CFStringRef which can be represented in ASCII
   encoding; only the first 32 bytes will be used for the password.

   If the value of this key cannot be represented in ASCII, the document
   will not be created and the creation function will return NULL. */

CG_EXTERN const CFStringRef  kCGPDFContextUserPassword CG_AVAILABLE_STARTING(10.4, 2.0);

/* Specifies the encryption key length in bits; see Table 3.18 "Entries
   common to all encryption dictionaries", PDF Reference: Adobe PDF version
   1.5 (4th ed.) for more info. Optional; if present, the value of this key
   must be a CFNumber with value which is a multiple of 8 between 40 and
   128, inclusive. If this key is absent or invalid, the encryption key
   length defaults to 40 bits. */

CG_EXTERN const CFStringRef  kCGPDFContextEncryptionKeyLength CG_AVAILABLE_STARTING(10.5, 2.0);

/* Used to specify whether the document allows printing when unlocked with
   the user password. The value of this key must be a CFBooleanRef. The
   default value of this key is "kCFBooleanTrue". */

CG_EXTERN const CFStringRef  kCGPDFContextAllowsPrinting CG_AVAILABLE_STARTING(10.4, 2.0);

/* Used to specify whether the document allows copying when unlocked with
   the user password. The value of this key must be a CFBooleanRef. The
   default value of this key is "kCFBooleanTrue". */

CG_EXTERN const CFStringRef  kCGPDFContextAllowsCopying CG_AVAILABLE_STARTING(10.4, 2.0);

/* The document's PDF/X output intent. Optional; if present, the value of
   this key must be a CFDictionaryRef. The dictionary is added to the
   /OutputIntents entry in the PDF file's document catalog. The keys and
   values contained in the dictionary must match those specified in section
   9.10.4 of the PDF 1.4 specification, ISO/DIS 15930-3 document published
   by ISO/TC 130, and Adobe Technical Note #5413. */

CG_EXTERN const CFStringRef  kCGPDFContextOutputIntent CG_AVAILABLE_STARTING(10.4, 14.0);

/* The following keys are supported in the output intent dictionary:

   kCGPDFXOutputIntentSubtype ("S"): The output intent subtype. This key is
   required; the value of this key must be a CFString equal to "GTS_PDFX";
   otherwise, the dictionary is ignored. */

CG_EXTERN const CFStringRef  kCGPDFXOutputIntentSubtype CG_AVAILABLE_STARTING(10.4, 14.0);

/* kCGPDFXOutputConditionIdentifier ("OutputConditionIdentifier"): A string
   identifying the intended output device or production condition in a
   human- or machine-readable form. This key is required; the value of this
   key must be a CFString. For best results, the string should be
   representable losslessly in ASCII encoding. */

CG_EXTERN const CFStringRef  kCGPDFXOutputConditionIdentifier CG_AVAILABLE_STARTING(10.4, 14.0);

/* kCGPDFXOutputCondition ("OutputCondition"): A text string identifying the
   intended output device or production condition in a human-readable form.
   This key is optional; if present, the value of this key must be a
   CFString. */

CG_EXTERN const CFStringRef  kCGPDFXOutputCondition CG_AVAILABLE_STARTING(10.4, 14.0);

/* kCGPDFXRegistryName ("RegistryName"): A string identifying the registry
   in which the condition designated by `kCGPDFXOutputConditionIdentifier'
   is defined. This key is optional; if present, the value of this key must
   be a CFString. For best results, the string should be representable
   losslessly in ASCII encoding. */

CG_EXTERN const CFStringRef  kCGPDFXRegistryName CG_AVAILABLE_STARTING(10.4, 14.0);

/* kCGPDFXInfo ("Info"): A human-readable text string containing additional
   information about the intended target device or production condition.
   This key is required if the value of `kCGPDFXOutputConditionIdentifier'
   does not specify a standard production condition; it is optional
   otherwise. If present, the value of this key must be a CFString. */

CG_EXTERN const CFStringRef  kCGPDFXInfo CG_AVAILABLE_STARTING(10.4, 14.0);

/* kCGPDFXDestinationOutputProfile ("DestOutputProfile"): An ICC profile
   stream defining the transformation from the PDF document's source colors
   to output device colorants. This key is required if the value of
   `kCGPDFXOutputConditionIdentifier' does not specify a standard production
   condition; it is optional otherwise. If present, the value of this key
   must be a ICC-based CGColorSpaceRef. */

CG_EXTERN const CFStringRef  kCGPDFXDestinationOutputProfile CG_AVAILABLE_STARTING(10.4, 14.0);

/* The document's output intents. Optional; if present, the value must be a
   CFArrayRef containing one or more CFDictionaryRefs. The array is added to
   the PDF document in the /OutputIntents entry in the PDF file's document
   catalog. Each dictionary in the array must be of form specified above for
   the `kCGPDFContextOutputIntent' key, except that only the first
   dictionary in the array may contain the `kCGPDFXOutputIntentSubtype'
   ("S") key with a value of "GTS_PDFX". If both `kCGPDFContextOutputIntent'
   and `kCGPDFContextOutputIntents' keys are specified, the former is
   ignored. */

CG_EXTERN const CFStringRef  kCGPDFContextOutputIntents CG_AVAILABLE_STARTING(10.4, 14.0);

/* The document's access permissions, expressed as a CFNumber. The number is
   defined by ORing together the desired CGPDFAccessPermissions values. */

CG_EXTERN const CFStringRef  kCGPDFContextAccessPermissions CG_AVAILABLE_STARTING(10.13, 11.0);

/* Set the outline in the PDF created by a CGPDFContext (no effect in other types of CGContexts). */

CG_EXTERN void CGPDFContextSetOutline(CGContextRef context, __nullable CFDictionaryRef outline)
  CG_AVAILABLE_STARTING(10.13, 11.0);

/* Output a linearized PDF */

CG_EXTERN const CFStringRef kCGPDFContextCreateLinearizedPDF
  CG_AVAILABLE_STARTING(11.0, 14.0);

/* Output a PDF that conforms to PDF/A-2u (ISO 19005-2). The value of this key
   must be a CFBooleanRef. The default value of this key is "kCFBooleanFalse".  */

CG_EXTERN const CFStringRef  kCGPDFContextCreatePDFA CG_AVAILABLE_STARTING(11.0, 14.0);

/* Tagged PDF Authoring */

/* Tagged PDF Authoring is based on the Marked Content and Tagged PDF features defined
   in the Adobe Portable Document Format Version 1.7 November 2006. Link:
   http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 
   These features are described in Chapter 10, "Document Interchange", starting on page 841.
 
 - Marked-content operators (Section 10.5) identify portions of a content streams with
   additional properties, typically a tagged structure declaring accessibility information.
 
 - Logical structure facilities (Section 10.6) declares a hierarchical organization of the
   tags used by marked-content operators. This reflects the logical structure that tags
   should be read by an application presenting a PDF document.
 
 - Tagged PDF (Section 10.7) defines the properties of tags, and focuses on the accessibility
   features they provide. */

/* All CGPDFTagType reflect official "Role Types" defined in the Adobe Portable Document
   Format Version 1.7 November 2006. The enums below are defined between pages 899 to 912. */
typedef CF_ENUM (int32_t, CGPDFTagType) {
    
    /* Page 899 - 901, TABLE 10.20 Standard structure types for grouping elements */
    CGPDFTagTypeDocument = 100,
    CGPDFTagTypePart,
    CGPDFTagTypeArt,
    CGPDFTagTypeSection,
    CGPDFTagTypeDiv,
    CGPDFTagTypeBlockQuote,
    CGPDFTagTypeCaption,
    CGPDFTagTypeTOC,
    CGPDFTagTypeTOCI,
    CGPDFTagTypeIndex,
    CGPDFTagTypeNonStructure,
    CGPDFTagTypePrivate,
    
    /* Page 901, TABLE 10.21 Block-level structure elements. Long form descriptions on page 902 - 903 */
    
    /* Paragraph like elements */
    CGPDFTagTypeParagraph = 200,
    CGPDFTagTypeHeader,
    CGPDFTagTypeHeader1,
    CGPDFTagTypeHeader2,
    CGPDFTagTypeHeader3,
    CGPDFTagTypeHeader4,
    CGPDFTagTypeHeader5,
    CGPDFTagTypeHeader6,
    
    /* List elements */
    CGPDFTagTypeList = 300,
    CGPDFTagTypeListItem,
    CGPDFTagTypeLabel,
    CGPDFTagTypeListBody,
    
    /* Table element */
    CGPDFTagTypeTable = 400,
    CGPDFTagTypeTableRow,
    CGPDFTagTypeTableHeaderCell,
    CGPDFTagTypeTableDataCell,
    CGPDFTagTypeTableHeader,
    CGPDFTagTypeTableBody,
    CGPDFTagTypeTableFooter,
    
    /* Page 905, TABLE 10.25 Standard structure types for inline-level structure elements */
    CGPDFTagTypeSpan = 500,
    CGPDFTagTypeQuote,
    CGPDFTagTypeNote,
    CGPDFTagTypeReference,
    CGPDFTagTypeBibliography,
    CGPDFTagTypeCode,
    CGPDFTagTypeLink,
    CGPDFTagTypeAnnotation,
    
    /* Page 911, TABLE 10.26 Standard structure types for Ruby and Warichu elements (PDF 1.5) */
    CGPDFTagTypeRuby = 600,
    CGPDFTagTypeRubyBaseText,
    CGPDFTagTypeRubyAnnotationText,
    CGPDFTagTypeRubyPunctuation,
    CGPDFTagTypeWarichu,
    CGPDFTagTypeWarichuText,
    CGPDFTagTypeWarichuPunctiation,
    
    /* Page 912, TABLE 10.27 Standard structure types for illustration elements */
    CGPDFTagTypeFigure = 700,
    CGPDFTagTypeFormula,
    CGPDFTagTypeForm,
} CG_AVAILABLE_STARTING(10.15, 13.0);

/* For a given CGPDFTagType, return a C-string that matches the names defined in section 10.7.3: Standard Structure Types.
   These are defined on pages 899 - 912. Returns NULL for an unknown value. */
CG_EXTERN const char* cg_nullable CGPDFTagTypeGetName(CGPDFTagType tagType) CG_AVAILABLE_STARTING(10.15, 13.0);

/* The following CGPDFTagProperty keys are to be paired with CFStringRef values in
   CGPDFContextBeginTag(...)'s optional tagProperties dictionary. These key-value pairs
   encode accessibility information for the defined tag. */

typedef CFStringRef CGPDFTagProperty CF_TYPED_ENUM;

/* Actual text, defined on page 860:
   "Text that is an exact replacement for the structure element and its children..."
   "...useful when extracting the document’s contents in support of accessibility..."
   This can be used to more precisely control what string is extracted by a user when
   they copy and paste from a document. */
CG_EXTERN const CGPDFTagProperty _Nonnull kCGPDFTagPropertyActualText CG_AVAILABLE_STARTING(10.15, 13.0);

/* Alternative text, defined on page 860:
   "An alternate description of the structure element and its children in human-readable form".
   This is typically used for graphical content, like an image. */
CG_EXTERN const CGPDFTagProperty _Nonnull kCGPDFTagPropertyAlternativeText CG_AVAILABLE_STARTING(10.15, 13.0);

/* Title, defined on page 859:
   Title of the node in a human-readable form. This should *not* be used for accessibility,
   but can be useful when presenting the structure of a tagged node tree. */
CG_EXTERN const CGPDFTagProperty _Nonnull kCGPDFTagPropertyTitleText CG_AVAILABLE_STARTING(10.15, 13.0);

/* Language of text content, defined on page 860:
   Typically you should use the document's catalog to get its language, but if a section
   of text is not the same language as the document, this may be set and allow you to
   look at what language it is hinting at. */
CG_EXTERN const CGPDFTagProperty _Nonnull kCGPDFTagPropertyLanguageText CG_AVAILABLE_STARTING(10.15, 13.0);

/* General usage of CGPDFContextBeginTag(...) and CGPDFContextEndTag(...):
 
 - Create a CG PDF context using CGPDFContextCreate(...).
 - Begin and end pages as you normally would, through CGPDFContextBeginPage(...), CGPDFContextEndPage(...).
 - When drawing content for a page, you may tag any series of draw commands with balanced series of push / pops,
   which will mark your content with the given tag type and optional tag properties.
 
 - Pushing a tag saves the current tag to a tag-stack, assigning the given tag as the "current tag".
 - Popping removes the top-most element of the tag-stack. The remaining top-most element becomes the "current tag".
 - Any pushed tag is a child of the "current tag". This allows you to push children to parent-tags, and to build
   a tag structure tree.
 
 - When saving the context to a file, the file will be authored with marked content sequences in the page's content-
   stream, as well as the implicitly built tagged structure there. */

/* Sets the current tag to the given tagType with an associated tagProperties dictionary. The previous tag will be pushed
   to a tag-stack. The previous tag can be restored through CGPDFContextEndTag(...). This new tag will record any future
   drawing commands to the given context as part of itself, unless another tag is pushed or the tag is popped. When the
   PDF context is serialized to a PDF file, these tags will create marked content sequences around the associated draw commands
   around the page's content stream. Note that tagProperties is an optional dictionary that allows you to specificy additional
   properties of the marked content: content may want to re-declare how text should be extracted, any alt text for figures,
   or explicitly declare what language the text is in. All child-tags (tags pushed on top of other tags) will inherit their
   parent tag's properties. */
CG_EXTERN void CGPDFContextBeginTag(CGContextRef _Nonnull context, CGPDFTagType tagType, CFDictionaryRef cg_nullable tagProperties)
  CG_AVAILABLE_STARTING(10.15, 13.0);

/* Pop the current tag. Sets the current tag to the previous tag on the tag-stack. If there was no previous tag, then the
   current tag will be set to the root document tag (of type CGPDFTagTypeDocument). */
CG_EXTERN void CGPDFContextEndTag(CGContextRef _Nonnull context) CG_AVAILABLE_STARTING(10.15, 13.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFCONTEXT_H_ */
