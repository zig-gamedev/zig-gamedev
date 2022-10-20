/*
     File:       SearchKit/SKSearch.h
 
     Contains:   SearchKit Interfaces.
 
     Version:    SearchKit-417.1
 
     Copyright:  2003-2019 by Apple, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SKSEARCH__
#define __SKSEARCH__

#ifndef __CFBASE__
#include <CoreFoundation/CFBase.h>
#endif

#ifndef __CFURL__
#include <CoreFoundation/CFURL.h>
#endif

#ifndef __CFDICTIONARY__
#include <CoreFoundation/CFDictionary.h>
#endif

#ifndef __CFDATE__
#include <CoreFoundation/CFDate.h>
#endif

#ifndef __SKINDEX__
#include <SearchKit/SKIndex.h>
#endif



/*
 * Asynchronous search
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
 *  SKSearchRef
 *  
 *  Summary:
 *    An opaque data type representing an asynchronous search.
 */
typedef struct __SKSearch*              SKSearchRef;
/*
 *  SKSearchGetTypeID()
 *  
 *  Summary:
 *    Returns the type identifier for the SKSearch type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
SKSearchGetTypeID(void)                                       API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );



/*
 *  SKSearchOptions
 *  
 *  Summary:
 *    The various search options you can use with
 *    <tt>SKSearchCreate</tt>.
 */
typedef UInt32                          SKSearchOptions;
enum {
  kSKSearchOptionDefault        = 0,
  kSKSearchOptionNoRelevanceScores = 1L << 0, /* Save time by not computing relevance scores. */
  kSKSearchOptionSpaceMeansOR   = 1L << 1, /* Space in a query means OR instead of AND. */
  kSKSearchOptionFindSimilar    = 1L << 2 /* Find documents similar to given text string */
};

/*
 *  SKSearchCreate()
 *  
 *  Summary:
 *    Create an asynchronous search request.
 *  
 *  Discussion:
 *    A call to this function must be balanced with a call at a later
 *    time to <tt>CFRelease</tt>.
 *  
 *  Parameters:
 *    
 *    inIndex:
 *      A reference to the index to be searched.
 *    
 *    inQuery:
 *      The query string to search for.
 *    
 *    inSearchOptions:
 *      The search options. See the <tt>SKSearchOptions</tt>
 *      enumeration for options.
 *  
 *  Result:
 *    SKSearchRef         A reference to a SKSearch opaque type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKSearchRef 
SKSearchCreate(
  SKIndexRef        inIndex,
  CFStringRef       inQuery,
  SKSearchOptions   inSearchOptions)                          API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSearchCancel()
 *  
 *  Summary:
 *    Cancel the search request.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SKSearchCancel(SKSearchRef inSearch)                          API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSearchFindMatches()
 *  
 *  Summary:
 *    Search for up to maximumTime seconds or until inMaximumCount (or
 *    all) items are found.
 *  
 *  Discussion:
 *    Returns TRUE if more to search, FALSE when the search is
 *    exhausted. Returns number of entries actually found in
 *    *outFoundCount. The maximumTime of 0 means return quickly, so may
 *    return TRUE, and 0 outFoundCount. The relevance score is not
 *    normalized, so it can be very large.
 *  
 *  Parameters:
 *    
 *    inSearch:
 *      A reference to the SKSearch opaque type.
 *    
 *    inMaximumCount:
 *      The maximum number of found items to return.
 *    
 *    outDocumentIDsArray:
 *      An array of found document IDs. Must be inMaximumCount in size.
 *    
 *    outScoresArray:
 *      An array of relevance scores for found items. May be
 *      <tt>NULL</tt>.
 *    
 *    maximumTime:
 *      The maximum seconds before return.
 *    
 *    outFoundCount:
 *      The number of items actually found.
 *  
 *  Result:
 *    Boolean             Returns TRUE if more to search, FALSE when
 *    the search is exhausted.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
SKSearchFindMatches(
  SKSearchRef      inSearch,
  CFIndex          inMaximumCount,
  SKDocumentID *   outDocumentIDsArray,
  float *          outScoresArray,
  CFTimeInterval   maximumTime,
  CFIndex *        outFoundCount)                             API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCopyInfoForDocumentIDs()
 *  
 *  Summary:
 *    Copies document names and parent ids by way of document IDs in an
 *    index.
 *  
 *  Parameters:
 *    
 *    inIndex:
 *      A reference to the index.
 *    
 *    inCount:
 *      The number of inDocumentIDsArray.
 *    
 *    inDocumentIDsArray:
 *      An array of document IDs.
 *    
 *    outNamesArray:
 *      An array of names for the specified document IDs. May be
 *      <tt>NULL</tt>.
 *    
 *    outParentIDsArray:
 *      An array of parent ids for the specified document IDs. May be
 *      <tt>NULL</tt>.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SKIndexCopyInfoForDocumentIDs(
  SKIndexRef      inIndex,
  CFIndex         inCount,
  SKDocumentID *  inDocumentIDsArray,
  CFStringRef *   outNamesArray,
  SKDocumentID *  outParentIDsArray)                          API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCopyDocumentRefsForDocumentIDs()
 *  
 *  Summary:
 *    Copies document references by way of document IDs in an index.
 *  
 *  Parameters:
 *    
 *    inIndex:
 *      A reference to the index.
 *    
 *    inCount:
 *      The number of inDocumentIDsArray.
 *    
 *    inDocumentIDsArray:
 *      An array of document IDs.
 *    
 *    outDocumentRefsArray:
 *      An array of document references for the specified document IDs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SKIndexCopyDocumentRefsForDocumentIDs(
  SKIndexRef       inIndex,
  CFIndex          inCount,
  SKDocumentID *   inDocumentIDsArray,
  SKDocumentRef *  outDocumentRefsArray)                      API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKIndexCopyDocumentURLsForDocumentIDs()
 *  
 *  Summary:
 *    Copies document URLs by way of document IDs in an index.
 *  
 *  Parameters:
 *    
 *    inIndex:
 *      A reference to the index.
 *    
 *    inCount:
 *      The number of inDocumentIDsArray.
 *    
 *    inDocumentIDsArray:
 *      An array of document IDs.
 *    
 *    outDocumentURLsArray:
 *      An array of CFURLs for the specified document IDs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SKIndexCopyDocumentURLsForDocumentIDs(
  SKIndexRef      inIndex,
  CFIndex         inCount,
  SKDocumentID *  inDocumentIDsArray,
  CFURLRef *      outDocumentURLsArray)                       API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 * Synchronous search
 */

/*
 *  SKSearchGroupRef
 *  
 *  Summary:
 *    An opaque data type representing a search group.
 *  
 *  Discussion:
 *    A search group is a group of indexes to be searched.
 */
typedef struct __SKSearchGroup*         SKSearchGroupRef;
/*
 *  SKSearchGroupGetTypeID()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Returns the type identifier for the SKSearchGroup type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
SKSearchGroupGetTypeID(void)                                  API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );



/*
 *  SKSearchResultsRef
 *  
 *  Summary:
 *    An opaque data type representing search results.
 */
typedef struct __SKSearchResults*       SKSearchResultsRef;
/*
 *  SKSearchResultsGetTypeID()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Returns the type identifier for the SKSearchResults object.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
SKSearchResultsGetTypeID(void)                                API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );



/*
 *  SKSearchType
 *  
 *  Discussion:
 *    The various search types you can use with
 *    <tt>SKSearchResultsCreateWithQuery</tt>. Each of these specifies
 *    a set of ranked search hits. The kSKSearchRanked and
 *    kSKSearchPrefixRanked constants can be used for all index types.
 *    The kSKSearchBooleanRanked and kSKSearchRequiredRanked constants
 *    cannot be used for Vector indexes.
 */
enum SKSearchType {

  /*
   * Basic ranked search.
   */
  kSKSearchRanked               = 0,

  /*
   * The query can include boolean operators including '|', '&', '!',
   * '(', and ')'.
   */
  kSKSearchBooleanRanked        = 1,

  /*
   * The query can specify required ('+') or excluded ('-') terms.
   */
  kSKSearchRequiredRanked       = 2,

  /*
   * Prefix-based search.
   */
  kSKSearchPrefixRanked         = 3
};
typedef enum SKSearchType SKSearchType;



/*
 *  SKSearchResultsFilterCallBack
 *  
 *  Summary:
 *    A callback function for hit testing during searching.
 *  
 *  Discussion:
 *    Return <tt>true</tt> to keep this document in the results,
 *    <tt>false</tt> to filter it out.
 */
typedef CALLBACK_API_C( Boolean , SKSearchResultsFilterCallBack )(SKIndexRef inIndex, SKDocumentRef inDocument, void *inContext);
/*
 *  SKSearchGroupCreate()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Creates a search group as an array of references to indexes.
 *  
 *  Discussion:
 *    A search group is used to search one or more indexes.
 *  
 *  Parameters:
 *    
 *    inArrayOfInIndexes:
 *      A CFArray object containing SKIndex objects.
 *  
 *  Result:
 *    SKSearchGroupRef    A reference to an SKSearchGroup opaque type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKSearchGroupRef 
SKSearchGroupCreate(CFArrayRef inArrayOfInIndexes)            API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSearchGroupCopyIndexes()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Gets the indexes for a search group.
 *  
 *  Result:
 *    A CFArray object containing SKIndex objects.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
SKSearchGroupCopyIndexes(SKSearchGroupRef inSearchGroup)      API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSearchResultsCreateWithQuery()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Queries the indexes in a search group. This call has been
 *    deprecated in favor of <tt>SKSearchCreate</tt>.
 *  
 *  Discussion:
 *    A call to this function must be balanced with a call at a later
 *    time to <tt>CFRelease</tt>.
 *  
 *  Parameters:
 *    
 *    inSearchGroup:
 *      A reference to the search group.
 *    
 *    inQuery:
 *      The query string to search for.
 *    
 *    inSearchType:
 *      The type of search to perform. See the <tt>SKSearchType</tt>
 *      enumeration for options.
 *    
 *    inMaxFoundDocuments:
 *      The maximum number of found items to return. Your client must
 *      specify a positive value.
 *    
 *    inContext:
 *      A client-specified context. May be <tt>NULL</tt>.
 *    
 *    inFilterCallBack:
 *      A callback function for hit testing during searching. May be
 *      <tt>NULL</tt>.
 *  
 *  Result:
 *    SKSearchResultsRef  A reference to an SKSearchResults opaque type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKSearchResultsRef 
SKSearchResultsCreateWithQuery(
  SKSearchGroupRef                inSearchGroup,
  CFStringRef                     inQuery,
  SKSearchType                    inSearchType,
  CFIndex                         inMaxFoundDocuments,
  void *                          inContext,
  SKSearchResultsFilterCallBack   inFilterCallBack)           API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSearchResultsCreateWithDocuments()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Finds documents similar to given example documents by searching
 *    the indexes in a search group. This call has been deprecated in
 *    favor of <tt>SKSearchCreate</tt>.
 *  
 *  Discussion:
 *    A call to SKSearchResultsCreateWithDocuments must be balanced
 *    with a call at a later time to <tt>CFRelease</tt>.
 *  
 *  Parameters:
 *    
 *    inSearchGroup:
 *      A reference to the search group.
 *    
 *    inExampleDocuments:
 *      An array of example documents. The documents must previously
 *      have been indexed.
 *    
 *    inMaxFoundDocuments:
 *      The maximum number of found items to return. Your client must
 *      specify a positive value.
 *    
 *    inContext:
 *      A client-specified context. May be <tt>NULL</tt>.
 *    
 *    inFilterCallBack:
 *      A callback function for hit testing during searching. May be
 *      <tt>NULL</tt>.
 *  
 *  Result:
 *    SKSearchResultsRef  A reference to an SKSearchResults opaque type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKSearchResultsRef 
SKSearchResultsCreateWithDocuments(
  SKSearchGroupRef                inSearchGroup,
  CFArrayRef                      inExampleDocuments,
  CFIndex                         inMaxFoundDocuments,
  void *                          inContext,
  SKSearchResultsFilterCallBack   inFilterCallBack)           API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSearchResultsGetCount()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Gets the total number of found items in a search.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKSearchResultsGetCount(SKSearchResultsRef inSearchResults)   API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSearchResultsGetInfoInRange()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Fills in requested results, returns number of items that were
 *    returned.
 *  
 *  Discussion:
 *    Search results are returned in descending order of relevance
 *    score.
 *  
 *  Parameters:
 *    
 *    inSearchResults:
 *      A reference to the search results.
 *    
 *    inRange:
 *      A CFRange value pair, specified as (location, length). The
 *      location value specifies the starting item by ranking. The
 *      length value specifies the total number of items. Examples:
 *      (0,1) means the first item, which is also the highest ranking
 *      item. (1,1) means the second item, which is also the
 *      second-highest ranking item. (0,5) means to get the first 5
 *      items.
 *    
 *    outDocumentsArray:
 *      An array of found documents.
 *    
 *    outIndexesArray:
 *      An array of indexes in which the found docouments reside. May
 *      be <tt>NULL</tt> provided that the client does not care.
 *    
 *    outScoresArray:
 *      An array of correspondence scores for found items. May be
 *      <tt>NULL</tt>.
 *  
 *  Result:
 *    The number of items returned -- usually the same number as
 *    specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKSearchResultsGetInfoInRange(
  SKSearchResultsRef   inSearchResults,
  CFRange              inRange,
  SKDocumentRef *      outDocumentsArray,
  SKIndexRef *         outIndexesArray,
  float *              outScoresArray)                        API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSearchResultsCopyMatchingTerms()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Gets the matching terms for the specified search result item
 *    index.
 *  
 *  Parameters:
 *    
 *    inSearchResults:
 *      A reference to the search results.
 *    
 *    inItem:
 *      The search result item index, starting from 1.
 *  
 *  Result:
 *    A reference to a CFArray object of term IDs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
SKSearchResultsCopyMatchingTerms(
  SKSearchResultsRef   inSearchResults,
  CFIndex              inItem)                                API_DEPRECATED("No longer supported", macos(10.3, 10.4)) API_UNAVAILABLE( macCatalyst );



#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SKSEARCH__ */

