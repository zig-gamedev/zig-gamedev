/*
     File:       HIServices/TranslationServices.h
 
     Contains:   Translation Services Interfaces.
 
     Copyright:  � 2003-2012 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TRANSLATIONSERVICES__
#define __TRANSLATIONSERVICES__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif


#include <AvailabilityMacros.h>
#include <CoreServices/CoreServices.h> // FSRef

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  TranslationServices
 *  
 *  Discussion:
 *    Translation Services provides tools for conversion of data and
 *    file contents from one format to another. All information for
 *    performing a translation is contained within a TranslationRef.
 *    These include source and destination data types, flags indicating
 *    what type of translation can be performed and references to the
 *    system services which execute the translation. TranslationRefs
 *    are CFTypes which must be released after they are created. Source
 *    and destination formats as well as translation flags can be
 *    queried from the TranslationRef. TranslationRefs are generated
 *    either by requesting a specific translation via TranslationCreate
 *    or during discovery of all possible translations via
 *    TranslationCreateWithSourceArray. It is possible to request
 *    TranslationRefs which perform either data or file conversions or
 *    both by using TranslationFlags. When requesting a translation be
 *    executed via TranslationPerformForData, TranslationPerformForFile
 *    or TranslationPerformForURL it is important for the source and
 *    destination data formats to match those found in the
 *    TranslationRef. 
 *    
 *    It is possible to extend the set of system services which provide
 *    translations by creating what is called a filter service. Filter
 *    services are applications similar to those which provide the
 *    services available in the application menu but with a few
 *    modifications in the application plist. Filter services provide
 *    an "NSFilter" entry instead of "NSMessage". Filter Services must
 *    also provide an array of both "NSSendTypes" and "NSReturnTypes"
 *    containing Uniform Type Identifiers indicating from which formats
 *    a filter services translates to what format. Each filter service
 *    may translate multiple send types into a single return type.
 *    Finally, a filter service must indicate what type of translations
 *    it supports via the "NSSupportsDataTranslation" and
 *    "NSSupportsFileTranslation" entries. In the end, a filter
 *    service's plist may look like the following, 
 *    
 *    
 *    
 *            <key>NSServices</key> 
 *                <array> 
 *                    <dict> 
 *                        <key>NSFilter</key> 
 *                        <string>ExampleTranslation</string> 
 *                        <key>NSReturnTypes</key> 
 *                        <array> 
 *                            <string>com.example.returntype</string> 
 *                        </array> 
 *                        <key>NSSendTypes</key> 
 *                        <array> 
 *                            <string>com.example.sourcetype1</string> 
 *                            <string>com.example.sourcetype2</string> 
 *                        </array> 
 *                        <key>NSSupportsDataTranslation</key> 
 *                        <string></string> 
 *                        <key>NSSupportsFileTranslation</key> 
 *                        <string></string> 
 *                    </dict> 
            <array> 

 *    
 *    
 *    
 *    All filter services must handle the kEventServicePerform Carbon
 *    Event.  The filter service will be automatically launched when
 *    necessary and it will receive the kEventServicePerform event with
 *    the message indicated by the NSFilter tag in the plist as well as
 *    a Pasteboard Manager pasteboard containing flavors indicating
 *    what type of translation must be performed. If a filter service
 *    only supports data translations a flavor on the pasteboard will
 *    correspond to one of the type identifiers listed in your plist's
 *    send types.  Upon translation of the data, the filter service
 *    must clear the pasteboard, add the return identifier and
 *    translated data to the pasteboard, and return from the event. For
 *    a filter service which provides file translations,
 *    "public.file-url" and "com.apple.file-contents-type" will be
 *    available on the pasteboard indicating the file location and
 *    contents format from which to translate. Upon translation, the
 *    filter service should place a "public.file-url" flavor on the
 *    pasteboard indicating where the translated file has been placed,
 *    typically next to the orignal named "<filename> (converted)"
 *    (ala. Finder's "<filename> copy" behavior for duplicated files).
 */
typedef struct OpaqueTranslationRef*    TranslationRef;
/* Translation Services error codes*/
enum {
                                        /*invalidTranslationPathErr   = -3025*/
                                        /*couldNotParseSourceFileErr  = -3026*/
                                        /*noTranslationPathErr         = -3030          // no translation for source and destination provided*/
                                        /*badTranslationSpecErr      = -3031*/
                                        /*noPrefAppErr              = -3032*/
  badTranslationRefErr          = -3031 /* TranslationRef does not perform translation requested*/
};


/*
 *  TranslationFlags
 *  
 *  Summary:
 *    The following constants are used by the translation creation
 *    routines to indicate which types of translations are requested.
 *    The flags are cumulative (ie. when passing both
 *    kTranslationDataTranslation and kTranslationFileTranslation the
 *    client is requesting only those translations which support both
 *    data AND file translations).
 */
typedef OptionBits TranslationFlags;
enum {

  /*
   * Indicates that the client is interested in translations which
   * provide data translations.
   */
  kTranslationDataTranslation   = (1 << 0),

  /*
   * Indicates that the client is interested in translations which
   * provide file translations.
   */
  kTranslationFileTranslation   = (1 << 1)
};

/*
 *  TranslationGetTypeID()
 *  
 *  Summary:
 *    Returns the CFType identifier for a translation object.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    A CFTypeID unique to translation instances.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
TranslationGetTypeID(void)                                    AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  TranslationCreate()
 *  
 *  Summary:
 *    Creates a translation reference describing a system service
 *    providing translations of data from the source type to the
 *    destination type.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSourceType:
 *      A Uniform Type Identifier specifying the format of source data
 *      to be translated.
 *    
 *    inDestinationType:
 *      A Uniform Type Identifier specifying the destination format to
 *      which the source data should be translated.
 *    
 *    inTranslationFlags:
 *      A set of TranslationFlags indicating what type of translation
 *      is requested.
 *    
 *    outTranslation:
 *      A TranslationRef reference which receives the requested
 *      translation if a system service providing the translation
 *      exists.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TranslationCreate(
  CFStringRef        inSourceType,
  CFStringRef        inDestinationType,
  TranslationFlags   inTranslationFlags,
  TranslationRef *   outTranslation)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  TranslationCreateWithSourceArray()
 *  
 *  Summary:
 *    Creates a list of destination flavors translation references
 *    describing various system services providing translations of data
 *    from the source types to the destination types.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSourceTypes:
 *      An array of Uniform Type Identifiers specifying the formats of
 *      source data to be translated.
 *    
 *    inTranslationFlags:
 *      A set of TranslationFlags indicating what type of translations
 *      are requested.
 *    
 *    outDestinationTypes:
 *      A CFArrayRef reference which receives an array of Uniform Type
 *      Identifiers specifying what destination formats are available
 *      as translations of the provided source formats. Any destination
 *      formats already represented as a format in the source array are
 *      excluded from the returned list. The search for destination
 *      formats is performed in the order of source formats. This array
 *      must be released by the client.
 *    
 *    outTranslations:
 *      A CFDictionaryRef reference which receives a dictionary of
 *      TranslationRefs representing all translations provided by
 *      system services. The dictionary is keyed by destination flavor.
 *      Any translations with destination formats already represented
 *      as a format in the source array are excluded from the returned
 *      dictionary. This dictionary must be released by the client.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TranslationCreateWithSourceArray(
  CFArrayRef         inSourceTypes,
  TranslationFlags   inTranslationFlags,
  CFArrayRef *       outDestinationTypes,
  CFDictionaryRef *  outTranslations)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  TranslationPerformForData()
 *  
 *  Summary:
 *    Executes the translation of source data into destination data. 
 *    The formats of the source and destination data are contained
 *    within the TranslationRef.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTranslation:
 *      A TranslationRef containing information on the source and
 *      destination data formats and how to execute a translation from
 *      one to the other. The formats of the source and destination
 *      data must correspond to those indicated by the TranslationRef.
 *    
 *    inSourceData:
 *      A CFDataRef containing data to be translated.
 *    
 *    outDestinationData:
 *      A CFDataRef reference which receives the translated data.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TranslationPerformForData(
  TranslationRef   inTranslation,
  CFDataRef        inSourceData,
  CFDataRef *      outDestinationData)                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  TranslationPerformForFile()
 *  
 *  Summary:
 *    Executes the translation of source file contents to a destination
 *    file content format. The formats of the source and destination
 *    file contents are held within the TranslationRef.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTranslation:
 *      A TranslationRef containing information on the source and
 *      destination file content formats and how to execute a
 *      translation from one to the other. The formats of the source
 *      and destination file contents must correspond to those
 *      indicated by the TranslationRef.
 *    
 *    inSourceFile:
 *      A FSRef reference pointing to a file whose contents are to be
 *      translated.
 *    
 *    inDestinationDirectory:
 *      An optional FSRef reference pointing to the desired directory
 *      for the translation. By default the destination directory is
 *      the same as the source file.
 *    
 *    inDestinationName:
 *      An optional CFStringRef indicating the desired name for the
 *      translated file. By default the translated file's name will be
 *      "<filename> (converted)" (ala. Finder's "<filename> copy"
 *      behavior for duplicated files).
 *    
 *    outTranslatedFile:
 *      A FSRef reference which receives a new file with the translated
 *      contents. It is possible for the translated file to not have
 *      been created in the directory or with the name requested by the
 *      client due to disk space or translator limitations. It is
 *      important to rely only on the file reference returned in this
 *      parameter.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TranslationPerformForFile(
  TranslationRef   inTranslation,
  const FSRef *    inSourceFile,
  const FSRef *    inDestinationDirectory,       /* can be NULL */
  CFStringRef      inDestinationName,            /* can be NULL */
  FSRef *          outTranslatedFile)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  TranslationPerformForURL()
 *  
 *  Summary:
 *    Executes the translation of source data pointed to a URL to a
 *    destination format. The formats of the source and destination URL
 *    contents are held within the TranslationRef. Currently, only file
 *    URLs are accepted for URL translations.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTranslation:
 *      A TranslationRef containing information on the source and
 *      destination URL content formats and how to execute a
 *      translation from one to the other. The formats of the source
 *      and destination URL contents must correspond to those indicated
 *      by the TranslationRef.
 *    
 *    inSourceURL:
 *      A CFURLRef pointing to source data whose contents are to be
 *      translated.  Currently, only file URLs are accepted for URL
 *      translations.
 *    
 *    inDestinationURL:
 *      An optional CFURLRef indicating the desired location for the
 *      translated data. File URLs may either indicate the desired
 *      destination directory or directory and name for the translated
 *      file. By default for file URLs, the translated file's name will
 *      be "<filename> (converted)" (ala. Finder's "<filename> copy"
 *      behavior for duplicated files).
 *    
 *    outTranslatedURL:
 *      A FSRef reference which receives a new file with the translated
 *      contents. For file URLs, it is possible for the translated file
 *      to not have been created in the directory or with the name
 *      requested by the client due to disk space or translator
 *      limitations. It is important to rely only on the URL returned
 *      in this parameter.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TranslationPerformForURL(
  TranslationRef   inTranslation,
  CFURLRef         inSourceURL,
  CFURLRef         inDestinationURL,       /* can be NULL */
  CFURLRef *       outTranslatedURL)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  TranslationCopySourceType()
 *  
 *  Summary:
 *    Accesses a translation's source type.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTranslation:
 *      A TranslationRef containing the requested source type.
 *    
 *    outSourceType:
 *      A CFStringRef which receives the TranslationRef's source type.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TranslationCopySourceType(
  TranslationRef   inTranslation,
  CFStringRef *    outSourceType)                             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  TranslationCopyDestinationType()
 *  
 *  Summary:
 *    Accesses a translation's destination type.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTranslation:
 *      A TranslationRef containing the requested destination type.
 *    
 *    outDestinationType:
 *      A CFStringRef which receives the TranslationRef's destination
 *      type.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TranslationCopyDestinationType(
  TranslationRef   inTranslation,
  CFStringRef *    outDestinationType)                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  TranslationGetTranslationFlags()
 *  
 *  Summary:
 *    Accesses a translation's flags.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTranslation:
 *      A TranslationRef containing the requested flags.
 *    
 *    outTranslationFlags:
 *      A TranslationFlags which receives the TranslationRef's flags.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TranslationGetTranslationFlags(
  TranslationRef      inTranslation,
  TranslationFlags *  outTranslationFlags)                    AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



#ifdef __cplusplus
}
#endif

#endif /* __TRANSLATIONSERVICES__ */

