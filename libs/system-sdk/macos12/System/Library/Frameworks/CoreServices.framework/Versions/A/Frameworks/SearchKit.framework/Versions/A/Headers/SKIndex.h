/*
     File:       SearchKit/SKIndex.h
 
     Contains:   SearchKit Interfaces.
 
     Version:    SearchKit-417.1
 
     Copyright:  2003-2019 by Apple, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SKINDEX__
#define __SKINDEX__

#ifndef __CFBASE__
#include <CoreFoundation/CFBase.h>
#endif

#ifndef __CFURL__
#include <CoreFoundation/CFURL.h>
#endif

#ifndef __CFDICTIONARY__
#include <CoreFoundation/CFDictionary.h>
#endif

#ifndef __SKDOCUMENT__
#include <SearchKit/SKDocument.h>
#endif



/*
 *  CFTypes for use with SearchKit
 */

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint on
#endif


/*
 *  SKIndexRef
 *  
 *  Summary:
 *    An opaque data type representing an index.
 */
typedef struct __SKIndex*               SKIndexRef;
/*
 *  SKIndexGetTypeID()
 *  
 *  Summary:
 *    Returns the type identifier for the index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
SKIndexGetTypeID(void)                                        API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );




/*
 *  SKIndexDocumentIteratorRef
 *  
 *  Summary:
 *    An opaque data type representing an index iterator.
 */
typedef struct __SKIndexDocumentIterator*  SKIndexDocumentIteratorRef;
/*
 *  SKIndexDocumentIteratorGetTypeID()
 *  
 *  Summary:
 *    Returns the type identifier of the index iterator.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
SKIndexDocumentIteratorGetTypeID(void)                        API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 * enumeration types
 */

/*
 *  SKIndexType
 *  
 *  Summary:
 *    The possible index types for SearchKit indexes.
 */
enum SKIndexType {

  /*
   * Unknown index type.
   */
  kSKIndexUnknown               = 0,

  /*
   * Inverted index, mapping terms to documents.
   */
  kSKIndexInverted              = 1,

  /*
   * Vector index, mapping documents to terms.
   */
  kSKIndexVector                = 2,

  /*
   * Index type with all the capabilities of an inverted and a vector
   * index.
   */
  kSKIndexInvertedVector        = 3
};
typedef enum SKIndexType SKIndexType;


/*
 *  SKDocumentIndexState
 *  
 *  Summary:
 *    The indexing state of a document.
 */
enum SKDocumentIndexState {

  /*
   * Document is not indexed.
   */
  kSKDocumentStateNotIndexed    = 0,

  /*
   * Document is indexed.
   */
  kSKDocumentStateIndexed       = 1,

  /*
   * Document is not in the index but will be added after the index is
   * flushed or closed.
   */
  kSKDocumentStateAddPending    = 2,

  /*
   * Document is in the index but will be deleted after the index is
   * flushed or closed.
   */
  kSKDocumentStateDeletePending = 3
};
typedef enum SKDocumentIndexState SKDocumentIndexState;


/*
 *  SKIndexCreateWithURL()
 *  
 *  Summary:
 *    Creates a named index in a file whose location is specified with
 *    a CFURL object.
 *  
 *  Discussion:
 *    When an index is created, the client must also specify the index
 *    type. The name can be <tt>NULL</tt>. A file can contain more than
 *    one index. If the analysis properties dictionary is not
 *    specified, the default dictionary is used. The various analysis
 *    properties are described in the SKAnalysis.h header file. Use the
 *    <tt>SKIndexGetAnalysisProperties</tt> function to get the
 *    analysis properties of an index. A call to this function must be
 *    balanced with a call at a later time to <tt>CFRelease</tt>.
 *  
 *  Parameters:
 *    
 *    inURL:
 *      A reference to a CFURL object containing the location of the
 *      index.
 *    
 *    inIndexName:
 *      A reference to a CFString object containing the name of the
 *      index. May be <tt>NULL</tt>.
 *    
 *    inIndexType:
 *      The index type.
 *    
 *    inAnalysisProperties:
 *      A reference to the analysis properties dictionary. May be
 *      <tt>NULL</tt>.
 *  
 *  Result:
 *    SKIndexRef  A reference to the index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKIndexRef 
SKIndexCreateWithURL(
  CFURLRef          inURL,
  CFStringRef       inIndexName,                /* can be NULL */
  SKIndexType       inIndexType,
  CFDictionaryRef   inAnalysisProperties)       /* can be NULL */ API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexOpenWithURL()
 *  
 *  Summary:
 *    Opens an existing, named index stored in a file whose location is
 *    specified with a CFURL object.
 *  
 *  Discussion:
 *    A call to this function must be balanced with a call at a later
 *    time to <tt>CFRelease</tt>.
 *  
 *  Parameters:
 *    
 *    inURL:
 *      A reference to a file CFURL object containing the location of
 *      the index.
 *    
 *    inIndexName:
 *      A reference to a CFString object containing the name of the
 *      index. May be <tt>NULL</tt>.
 *    
 *    inWriteAccess:
 *      A boolean value indicating whether the index is open for
 *      writing.
 *  
 *  Result:
 *    SKIndexRef  A reference to the index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKIndexRef 
SKIndexOpenWithURL(
  CFURLRef      inURL,
  CFStringRef   inIndexName,         /* can be NULL */
  Boolean       inWriteAccess)                                API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCreateWithMutableData()
 *  
 *  Summary:
 *    Creates a named index stored in a CFData object; the name can be
 *    <tt>NULL</tt>.
 *  
 *  Parameters:
 *    
 *    inData:
 *      A reference to a CFMutableData object containing the index to
 *      create.
 *    
 *    inIndexName:
 *      A reference to a CFString object containing the name of the
 *      index. May be <tt>NULL</tt>.
 *    
 *    inIndexType:
 *      A reference to the index type.
 *    
 *    inAnalysisProperties:
 *      A reference to the analysis properties dictionary. May be
 *      <tt>NULL</tt>.
 *  
 *  Result:
 *    SKIndexRef  A reference to the index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKIndexRef 
SKIndexCreateWithMutableData(
  CFMutableDataRef   inData,
  CFStringRef        inIndexName,                /* can be NULL */
  SKIndexType        inIndexType,
  CFDictionaryRef    inAnalysisProperties)       /* can be NULL */ API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexOpenWithData()
 *  
 *  Summary:
 *    Opens an existing, named index stored in a CFData object.
 *  
 *  Discussion:
 *    The index may be searched but not updated. To open the index for
 *    updating, use the <tt>SKIndexOpenWithMutableData</tt> function. A
 *    call to this function must be balanced with a call at a later
 *    time to <tt>CFRelease</tt>.
 *  
 *  Parameters:
 *    
 *    inData:
 *      A reference to a CFData object containing the index to open.
 *    
 *    inIndexName:
 *      A reference to a CFString object containing the name of the
 *      index. May be <tt>NULL</tt>.
 *  
 *  Result:
 *    SKIndexRef  A reference to the index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKIndexRef 
SKIndexOpenWithData(
  CFDataRef     inData,
  CFStringRef   inIndexName)       /* can be NULL */          API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexOpenWithMutableData()
 *  
 *  Summary:
 *    Opens an existing, named index stored in a CFData object.
 *  
 *  Discussion:
 *    The index may be searched or updated. To open the index for
 *    search only, use the <tt>SKIndexOpenWithData</tt> function. A
 *    call to this function must be balanced with a call at a later
 *    time to <tt>CFRelease</tt>.
 *  
 *  Parameters:
 *    
 *    inData:
 *      A reference to a CFMutableData object containing the index to
 *      open.
 *    
 *    inIndexName:
 *      A reference to a CFString object containing the name of the
 *      index. May be <tt>NULL</tt>.
 *  
 *  Result:
 *    SKIndexRef  A reference to the index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKIndexRef 
SKIndexOpenWithMutableData(
  CFMutableDataRef   inData,
  CFStringRef        inIndexName)       /* can be NULL */     API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexFlush()
 *  
 *  Summary:
 *    Forces SearchKit to flush all caches associated with an index.
 *  
 *  Discussion:
 *    Index caches can become stale when clients add or remove
 *    documents. Before searching an index you need to call
 *    <tt>SKIndexFlush</tt>, even though the cache flushing may take
 *    some time.
 *  
 *  Result:
 *    Returns a Boolean value of <tt>true</tt> on success or
 *    <tt>false</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
SKIndexFlush(SKIndexRef inIndex)                              API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexSetMaximumBytesBeforeFlush()
 *  
 *  Summary:
 *    Sets the memory size limit for an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SKIndexSetMaximumBytesBeforeFlush(
  SKIndexRef   inIndex,
  CFIndex      inBytesForUpdate)                              API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetMaximumBytesBeforeFlush()
 *  
 *  Summary:
 *    Gets the memory size limit for an index, measured in bytes.
 *  
 *  Result:
 *    Returns a CFIndex object containing the memory size limit for an
 *    index. On failure, returns 0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKIndexGetMaximumBytesBeforeFlush(SKIndexRef inIndex)         API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCompact()
 *  
 *  Summary:
 *    Attempts to defragment and compact the index.
 *  
 *  Discussion:
 *    This function takes time. Call it only when the index is too
 *    fragmented. You can test for fragmentation be examining the ratio
 *    of the total document count, obtained with
 *    <tt>SKIndexGetDocumentCount</tt>, to the maximum document ID,
 *    obtained with <tt>SKIndexGetMaximumDocumentID</tt>.
 *  
 *  Result:
 *    Returns a Boolean value of <tt>true</tt> on success or
 *    <tt>false</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
SKIndexCompact(SKIndexRef inIndex)                            API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetIndexType()
 *  
 *  Summary:
 *    Gets the type of an index.
 *  
 *  Discussion:
 *    See the <tt>SKIndexType</tt> enumeration for a list of the
 *    various index types.
 *  
 *  Result:
 *    Returns the type of the index. On failure, returns a value of
 *    kSKIndexUnknown.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKIndexType 
SKIndexGetIndexType(SKIndexRef inIndex)                       API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetAnalysisProperties()
 *  
 *  Summary:
 *    Gets the text analysis properties of an index.
 *  
 *  Result:
 *    Returns a CFDictionary object containing the index's text
 *    analysis properties. On failure, returns <tt>NULL</tt>.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDictionaryRef 
SKIndexGetAnalysisProperties(SKIndexRef inIndex)              API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetDocumentCount()
 *  
 *  Summary:
 *    Gets the total number of documents represented in an index.
 *  
 *  Discussion:
 *    Indexed documents have an indexing state of
 *    kSKDocumentStateIndexed. See the <tt>SKDocumentIndexState</tt>
 *    enumeration.
 *  
 *  Result:
 *    Returns CFIndex object containing the number of documents in the
 *    index. On failure, returns 0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKIndexGetDocumentCount(SKIndexRef inIndex)                   API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexClose()
 *  
 *  Summary:
 *    Close the index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SKIndexClose(SKIndexRef inIndex)                              API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 * Managing documents inside an index
 */
typedef CFIndex                         SKDocumentID;
/*
 *  SKIndexAddDocumentWithText()
 *  
 *  Summary:
 *    Adds a document and its text to an index.
 *  
 *  Parameters:
 *    
 *    inIndex:
 *      A reference to the index to which you are adding the document.
 *    
 *    inDocument:
 *      A reference to the document to add.
 *    
 *    inDocumentText:
 *      A reference to the document text. May be <tt>NULL</tt>.
 *    
 *    inCanReplace:
 *      A boolean value indicating whether a document with the same
 *      descriptor can be overwritten.
 *  
 *  Result:
 *    Returns a Boolean value of <tt>true</tt> on success or
 *    <tt>false</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
SKIndexAddDocumentWithText(
  SKIndexRef      inIndex,
  SKDocumentRef   inDocument,
  CFStringRef     inDocumentText,       /* can be NULL */
  Boolean         inCanReplace)                               API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexAddDocument()
 *  
 *  Summary:
 *    Adds a document to an index.
 *  
 *  Discussion:
 *    This function uses the input document and the optional MIME type
 *    hint to get the document text using plug-in-based text
 *    extractors. Call <tt>SKLoadDefaultExtractorPlugIns</tt> to load
 *    the default text extractors.
 *  
 *  Parameters:
 *    
 *    inDocument:
 *      A reference to the document to add. The document scheme must be
 *      of type "file" to use this function. If it's not, call
 *      <tt>SKIndexAddDocumentWithText</tt> instead.
 *    
 *    inIndex:
 *      A reference to the index to which you are adding the document.
 *    
 *    inDocument:
 *      A reference to the document to add.
 *    
 *    inMIMETypeHint:
 *      The MIME type hint for the document. May be <tt>NULL</tt>.
 *    
 *    inCanReplace:
 *      A boolean value indicating whether a document with the same
 *      descriptor can be overwritten.
 *  
 *  Result:
 *    Returns a Boolean value of <tt>true</tt> on success or
 *    <tt>false</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
SKIndexAddDocument(
  SKIndexRef      inIndex,
  SKDocumentRef   inDocument,
  CFStringRef     inMIMETypeHint,       /* can be NULL */
  Boolean         inCanReplace)                               API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexRemoveDocument()
 *  
 *  Summary:
 *    Removes a document and its children, if any, from an index.
 *  
 *  Parameters:
 *    
 *    inIndex:
 *      A reference to the index from which you want to remove the
 *      document.
 *    
 *    inDocument:
 *      A reference to the document to remove.
 *  
 *  Result:
 *    Returns a Boolean value of <tt>true</tt> on success or
 *    <tt>false</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
SKIndexRemoveDocument(
  SKIndexRef      inIndex,
  SKDocumentRef   inDocument)                                 API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCopyDocumentProperties()
 *  
 *  Summary:
 *    Copies the user-defined properties of a document in an index to a
 *    dictionary.
 *  
 *  Result:
 *    Returns a CFDictionary object, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDictionaryRef 
SKIndexCopyDocumentProperties(
  SKIndexRef      inIndex,
  SKDocumentRef   inDocument)                                 API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexSetDocumentProperties()
 *  
 *  Summary:
 *    Sets the user-defined properties of a document in an index. A
 *    document's properties are persistently stored in the index. The
 *    existing properties dictionary will be replaced with the new one.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SKIndexSetDocumentProperties(
  SKIndexRef        inIndex,
  SKDocumentRef     inDocument,
  CFDictionaryRef   inProperties)                             API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetDocumentState()
 *  
 *  Summary:
 *    Gets the current indexing state of a document in an index.
 *  
 *  Discussion:
 *    A document can be in 1 of 4 states, as defined by the
 *    <tt>SKDocumentIndexState</tt> enumeration: not indexed, indexed,
 *    not in the index but will be added after the index is flushed or
 *    closed, and in the index but will be deleted after the index is
 *    flushed or closed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKDocumentIndexState 
SKIndexGetDocumentState(
  SKIndexRef      inIndex,
  SKDocumentRef   inDocument)                                 API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetDocumentID()
 *  
 *  Summary:
 *    Gets the ID of a document in an index.
 *  
 *  Discussion:
 *    The document ID is a persistent way to identify the document in
 *    an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKDocumentID 
SKIndexGetDocumentID(
  SKIndexRef      inIndex,
  SKDocumentRef   inDocument)                                 API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCopyDocumentForDocumentID()
 *  
 *  Summary:
 *    Copies a document reference by way of a document ID in an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKDocumentRef 
SKIndexCopyDocumentForDocumentID(
  SKIndexRef     inIndex,
  SKDocumentID   inDocumentID)                                API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexRenameDocument()
 *  
 *  Summary:
 *    Changes the name of a document in an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
SKIndexRenameDocument(
  SKIndexRef      inIndex,
  SKDocumentRef   inDocument,
  CFStringRef     inNewName)                                  API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexMoveDocument()
 *  
 *  Summary:
 *    Changes the parent of a document in an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
SKIndexMoveDocument(
  SKIndexRef      inIndex,
  SKDocumentRef   inDocument,
  SKDocumentRef   inNewParent)                                API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexDocumentIteratorCreate()
 *  
 *  Summary:
 *    Creates an iterator for an index based on a starting document
 *    reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKIndexDocumentIteratorRef 
SKIndexDocumentIteratorCreate(
  SKIndexRef      inIndex,
  SKDocumentRef   inParentDocument)                           API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexDocumentIteratorCopyNext()
 *  
 *  Summary:
 *    Gets the next document reference from a document iterator.
 *  
 *  Discussion:
 *    This function returns <tt>NULL</tt> when there are no more
 *    documents. You must call <tt>CFRelease</tt> on all retrieved
 *    document references that are non-null.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKDocumentRef 
SKIndexDocumentIteratorCopyNext(SKIndexDocumentIteratorRef inIterator) API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 * Documents in Index
 */
/*
 *  SKIndexGetMaximumDocumentID()
 *  
 *  Summary:
 *    Gets the highest-numbered document ID in an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKDocumentID 
SKIndexGetMaximumDocumentID(SKIndexRef inIndex)               API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetDocumentTermCount()
 *  
 *  Summary:
 *    Gets the number of terms for a given document in an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKIndexGetDocumentTermCount(
  SKIndexRef     inIndex,
  SKDocumentID   inDocumentID)                                API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCopyTermIDArrayForDocumentID()
 *  
 *  Summary:
 *    Gets the IDs for the terms of a document in an index.
 *  
 *  Result:
 *    A reference to a CFArray object containing CFNumber objects.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
SKIndexCopyTermIDArrayForDocumentID(
  SKIndexRef     inIndex,
  SKDocumentID   inDocumentID)                                API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetDocumentTermFrequency()
 *  
 *  Summary:
 *    Gets the frequency of occurrences of a given term in a document.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKIndexGetDocumentTermFrequency(
  SKIndexRef     inIndex,
  SKDocumentID   inDocumentID,
  CFIndex        inTermID)                                    API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 * Terms in Index
 *
 * A term is a word from the content of a document.
 */
/*
 *  SKIndexGetMaximumTermID()
 *  
 *  Summary:
 *    Gets the highest-numbered term ID in an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKIndexGetMaximumTermID(SKIndexRef inIndex)                   API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetTermDocumentCount()
 *  
 *  Summary:
 *    Gets the number of documents containing a given term in an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKIndexGetTermDocumentCount(
  SKIndexRef   inIndex,
  CFIndex      inTermID)                                      API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCopyDocumentIDArrayForTermID()
 *  
 *  Summary:
 *    Gets the IDs of the documents containing a given term in an index.
 *  
 *  Result:
 *    A reference to a CFArray object containing CFNumber objects.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
SKIndexCopyDocumentIDArrayForTermID(
  SKIndexRef   inIndex,
  CFIndex      inTermID)                                      API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCopyTermStringForTermID()
 *  
 *  Summary:
 *    Gets the term specified by a term ID in an index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
SKIndexCopyTermStringForTermID(
  SKIndexRef   inIndex,
  CFIndex      inTermID)                                      API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexGetTermIDForTermString()
 *  
 *  Summary:
 *    Gets the term ID for a given term in an index.
 *  
 *  Discussion:
 *    If the term isn't found, return a value of kCFNotFound.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKIndexGetTermIDForTermString(
  SKIndexRef    inIndex,
  CFStringRef   inTermString)                                 API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );



/*
 *  SKLoadDefaultExtractorPlugIns()
 *  
 *  Summary:
 *    Loads the default text extractor plug-ins.
 *  
 *  Discussion:
 *    A text extractor knows the format of a specific kind of document.
 *    For example, various text extractors can return the text of a PDF
 *    document, strip the tags of an HTML document, and so on. Loading
 *    extractors allows the <tt>SKIndexAddDocument</tt> function to
 *    extract the text from supported documents, leaving the markup
 *    behind.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SKLoadDefaultExtractorPlugIns(void)                           API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( macCatalyst );



#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SKINDEX__ */

