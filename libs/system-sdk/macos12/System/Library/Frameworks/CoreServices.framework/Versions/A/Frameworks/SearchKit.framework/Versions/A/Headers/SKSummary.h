/*
     File:       SearchKit/SKSummary.h
 
     Contains:   SearchKit Interfaces.
 
     Version:    SearchKit-417.1
 
     Copyright:  2004-2019 by Apple, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SKSUMMARY__
#define __SKSUMMARY__

#ifndef __CFBASE__
#include <CoreFoundation/CFBase.h>
#endif

#ifndef __CFSTRING__
#include <CoreFoundation/CFString.h>
#endif


/*
 * Summarization API
 */

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  SKSummaryRef
 *  
 *  Summary:
 *    An opaque data type representing summary information.
 *  
 *  Discussion:
 *    A summary reference contains summary information, from which
 *    summary text can be obtained.
 */
typedef struct __SKSummary*             SKSummaryRef;
/*
 *  SKSummaryGetTypeID()
 *  
 *  Summary:
 *    Returns the type identifier of the SKSummaryRef type.
 *  
 *  Result:
 *    Returns a CFTypeID object, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
SKSummaryGetTypeID(void)                                      API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryCreateWithString()
 *  
 *  Summary:
 *    Creates a summary reference with text string.
 *  
 *  Discussion:
 *    Creates a summary reference that pre-computes what is needed for
 *    fast summarization. This function must be balanced with a call at
 *    a later time to CFRelease.
 *  
 *  Parameters:
 *    
 *    inString:
 *      the text string.
 *  
 *  Result:
 *    Returns a summary reference, or <tt>NULL</tt> on failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SKSummaryRef 
SKSummaryCreateWithString(CFStringRef inString)               API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryGetSentenceCount()
 *  
 *  Summary:
 *    Gets the number of sentences available.
 *  
 *  Parameters:
 *    
 *    summary:
 *      A reference to the summary object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKSummaryGetSentenceCount(SKSummaryRef summary)               API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryGetParagraphCount()
 *  
 *  Summary:
 *    Gets the number of paragraphs available.
 *  
 *  Parameters:
 *    
 *    summary:
 *      A reference to the summary object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKSummaryGetParagraphCount(SKSummaryRef summary)              API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryCopySentenceAtIndex()
 *  
 *  Summary:
 *    Gets the ith sentence from the original text.
 *  
 *  Parameters:
 *    
 *    summary:
 *      A reference to the summary object
 *    
 *    i:
 *      zero-based index
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
SKSummaryCopySentenceAtIndex(
  SKSummaryRef   summary,
  CFIndex        i)                                           API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryCopyParagraphAtIndex()
 *  
 *  Summary:
 *    Gets the ith paragraph from the original text.
 *  
 *  Parameters:
 *    
 *    summary:
 *      A reference to the summary object
 *    
 *    i:
 *      zero-based index
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
SKSummaryCopyParagraphAtIndex(
  SKSummaryRef   summary,
  CFIndex        i)                                           API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryCopySentenceSummaryString()
 *  
 *  Summary:
 *    Gets a summary string that includes at most the requested number
 *    of sentences.
 *  
 *  Parameters:
 *    
 *    summary:
 *      A reference to the summary object
 *    
 *    numSentences:
 *      the number of sentences desired
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
SKSummaryCopySentenceSummaryString(
  SKSummaryRef   summary,
  CFIndex        numSentences)                                API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryCopyParagraphSummaryString()
 *  
 *  Summary:
 *    Gets a summary string that includes at most the requested number
 *    of paragraphs.
 *  
 *  Parameters:
 *    
 *    summary:
 *      A reference to the summary object
 *    
 *    numParagraphs:
 *      the number of paragraphs desired
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
SKSummaryCopyParagraphSummaryString(
  SKSummaryRef   summary,
  CFIndex        numParagraphs)                               API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryGetSentenceSummaryInfo()
 *  
 *  Summary:
 *    Get detailed information about a sentence-based summary. Useful
 *    for constructing your own summary string. Arrays must be of size
 *    numSentences or they can be nil. Return value is the number of
 *    sentences actually returned. Sentences are returned in text order
 *    via outSentenceIndexOfSentences.
 *  
 *  Parameters:
 *    
 *    summary:
 *      A reference to the summary object
 *    
 *    numSentencesInSummary:
 *      the number of sentences desired
 *    
 *    outRankOrderOfSentences:
 *      array for returning the rank of each sentence; most important
 *      sentence is rank 1
 *    
 *    outSentenceIndexOfSentences:
 *      array for returning the index of each sentence; use
 *      SKSummaryCopySentenceAtIndex to get sentence
 *    
 *    outParagraphIndexOfSentences:
 *      array for returning the index of the paragraph in which
 *      sentence occured
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKSummaryGetSentenceSummaryInfo(
  SKSummaryRef   summary,
  CFIndex        numSentencesInSummary,
  CFIndex *      outRankOrderOfSentences,
  CFIndex *      outSentenceIndexOfSentences,
  CFIndex *      outParagraphIndexOfSentences)                API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );


/*
 *  SKSummaryGetParagraphSummaryInfo()
 *  
 *  Summary:
 *    Get detailed information about a paragraph-based summary. Useful
 *    for constructing your own summary string. Arrays must be of size
 *    numParagraphs or they can be nil. Return value is the number of
 *    paragraphs actually returned. Paragraphs are returned in text
 *    order via outParagraphIndexOfParagraphs.
 *  
 *  Parameters:
 *    
 *    summary:
 *      A reference to the summary object
 *    
 *    numParagraphsInSummary:
 *      the number of sentences desired
 *    
 *    outRankOrderOfParagraphs:
 *      array for returning the rank of each paragraph; most important
 *      paragraph is rank 1
 *    
 *    outParagraphIndexOfParagraphs:
 *      array for returning the index of each paragraph; use
 *      SKSummaryCopyParagraphAtIndex to get paragraph
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFIndex 
SKSummaryGetParagraphSummaryInfo(
  SKSummaryRef   summary,
  CFIndex        numParagraphsInSummary,
  CFIndex *      outRankOrderOfParagraphs,
  CFIndex *      outParagraphIndexOfParagraphs)               API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( macCatalyst );



#ifdef __cplusplus
}
#endif

#endif /* __SKSUMMARY__ */

