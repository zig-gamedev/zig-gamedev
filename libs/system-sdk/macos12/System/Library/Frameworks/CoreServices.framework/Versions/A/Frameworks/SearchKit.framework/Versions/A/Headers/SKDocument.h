/*
     File:       SearchKit/SKDocument.h
 
     Contains:   SearchKit Interfaces.
 
     Version:    SearchKit-417.1
 
     Copyright:  2003-2019 by Apple, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SKDOCUMENT__
#define __SKDOCUMENT__

#ifndef __CFBASE__
#include <CoreFoundation/CFBase.h>
#endif

#ifndef __CFURL__
#include <CoreFoundation/CFURL.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  SKDocumentRef
 *  
 *  Summary:
 *    An opaque data type representing a document.
 *  
 *  Discussion:
 *    A document reference is a generic descriptor to a document. It is
 *    built from a document scheme, a parent document, and a document
 *    name.
 */
typedef CFTypeRef                       SKDocumentRef;
/*
 *  SKDocumentGetTypeID()
 *  
 *  Summary:
 *    Returns the type identifier of the SKDocument type.
 *  
 *  Result:
 *    Returns a CFTypeID object, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
SKDocumentGetTypeID(void)                                     API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKDocumentCreateWithURL()
 *  
 *  Summary:
 *    Creates a reference to a document with a URL.
 *  
 *  Discussion:
 *    Use SKDocumentCreateWithURL to create a reference to a file or
 *    other URL. This function must be balanced with a call at a later
 *    time to CFRelease.
 *  
 *  Parameters:
 *    
 *    inURL:
 *      Only "file:" URLs can be used with the SKIndexAddDocument
 *      function, but the URL scheme may be anything you like if you
 *      use the SKIndexAddDocumentWithText function. The scheme of the
 *      document created is set to the scheme of the URL used.
 *  
 *  Result:
 *    Returns a reference to the document, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKDocumentRef 
SKDocumentCreateWithURL(CFURLRef inURL)                       API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKDocumentCopyURL()
 *  
 *  Summary:
 *    Builds a CFURL object from a document reference.
 *  
 *  Result:
 *    Returns a CFURL object, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFURLRef 
SKDocumentCopyURL(SKDocumentRef inDocument)                   API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKDocumentCreate()
 *  
 *  Summary:
 *    Create a document based on a scheme, parent, and name.
 *  
 *  Discussion:
 *    The parent can be <tt>NULL</tt>, but either a scheme or a parent
 *    must be specified. This function must be balanced with a call at
 *    a later time to CFRelease
 *  
 *  Parameters:
 *    
 *    inScheme:
 *      Analogous to the scheme of a URL. Documents with a "file"
 *      scheme can be read by the <tt>SKIndexAddDocument</tt> function
 *      (see SearchKit.h). The scheme may be anything you like if you
 *      use the SKIndexAddDocumentWithText function. If the scheme is
 *      <tt>NULL</tt>, it will be set to be the same as the parent.
 *    
 *    inParent:
 *      The reference to the document or container one step up in the
 *      document hierarchy.
 *    
 *    inName:
 *      The name of this document. For a "file" scheme, it is the name
 *      of the file or the container, not its path. The path can be
 *      constructed by following parent links.
 *  
 *  Result:
 *    Returns a reference to the document, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKDocumentRef 
SKDocumentCreate(
  CFStringRef     inScheme,
  SKDocumentRef   inParent,       /* can be NULL */
  CFStringRef     inName)                                     API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKDocumentGetSchemeName()
 *  
 *  Summary:
 *    Gets the scheme name of a document.
 *  
 *  Parameters:
 *    
 *    inDocument:
 *      The document whose scheme name you want to get.
 *  
 *  Result:
 *    Returns a CFString object, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
SKDocumentGetSchemeName(SKDocumentRef inDocument)             API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKDocumentGetName()
 *  
 *  Summary:
 *    Gets the name of a document.
 *  
 *  Parameters:
 *    
 *    inDocument:
 *      The document whose name you want to get.
 *  
 *  Result:
 *    Returns a CFString object, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
SKDocumentGetName(SKDocumentRef inDocument)                   API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKDocumentGetParent()
 *  
 *  Summary:
 *    Gets a reference to the parent document of a document.
 *  
 *  Parameters:
 *    
 *    inDocument:
 *      The document whose parent you want to get.
 *  
 *  Result:
 *    Returns a reference to the parent document, or <tt>NULL</tt> on
 *    failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKDocumentRef 
SKDocumentGetParent(SKDocumentRef inDocument)                 API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );



#ifdef __cplusplus
}
#endif

#endif /* __SKDOCUMENT__ */

