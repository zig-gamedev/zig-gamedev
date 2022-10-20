/*
     File:       HIToolbox/HIArchive.h
 
     Contains:   HIArchive Interfaces.
 
     Copyright:  © 2004-2008 by Apple Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIARCHIVE__
#define __HIARCHIVE__

#ifndef __HIOBJECT__
#include <HIToolbox/HIObject.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  HIArchive
 *  
 *  Discussion:
 *    HIArchive provides a standard, extensible mechanism to flatten
 *    objects for storage in memory or on disk for later retrieval or
 *    transfer to another application. The archive is encoded using the
 *    binary property list format. The binary plist can be converted to
 *    text XML with /usr/bin/plutil for development purposes. Details
 *    on how to create an object that supports the HIArchive protocol
 *    are provided in HIToolbox/HIObject.h. 
 *    
 *    When writing data out to an archive, the client must first use
 *    HIArchiveCreateForEncoding to generate the archive into which the
 *    data will be encoded. From there, data may be added to the
 *    archive by calling HIArchiveEncodeBoolean, HIArchiveEncodeNumber,
 *    and HIArchiveEncodeCFType. If HIArchiveEncodeCFType is being
 *    called on one of your custom HIObjects, HIToolbox will send it
 *    the kEventHIObjectEncode event (see HIObject.h). In order to
 *    receive this event your HIObject must first have set its
 *    archiving-ignored value to false via HIObjectSetArchivingIgnored.
 *    This lets HIToolbox know your object supports archiving. The
 *    kEventParamHIArchive parameter contains the HIArchiveRef into
 *    which it should encode all of its relevant state information. All
 *    information added to the archive is written with a key. This key
 *    is used later during the unarchiving process to pull the encoded
 *    data from the archive. System supplied HIObjects namespace their
 *    keys with an HI prefix. Subclasses of system supplied HIObjects
 *    should only use this namespace if explicitly overriding a value
 *    written to the archive by the superclass. Take care to mantain
 *    the same data format when overriding the default to avoid
 *    incompatibilities. When your archiving process is complete,
 *    HIArchiveCopyEncodedData will compress the data into the archive
 *    and return it in a CFDataRef. This CFDataRef can be sent to
 *    another application or written out to disk for later retrieval.
 *    Once the encoded data is compressed, no more data may be added to
 *    the archive. At this point, the HIArchiveRef must be released via
 *    CFRelease. 
 *    
 *    When retrieving data from an archive, the client must first use
 *    HIArchiveCreateForDecoding to create an archive reference capable
 *    of decoding the data from the provided CFDataRef. Given the
 *    HIArchiveRef, data may be pulled from the archive via
 *    HIArchiveDecodeBoolean, HIArchiveDecodeNumber, and
 *    HIArchiveCopyDecodedCFType. If HIArchiveCopyDecodedCFType is
 *    called on one of your custom HIObjects, HIToolbox will send it
 *    the kEventHIObjectInitialize event (see HIOject.h). The
 *    kEventParamHIArchive parameter contains the HIArchiveRef from
 *    which it should decode all of its relevant state information.
 *    Because these data values were written by key, they can be read
 *    in any order regardless of how they were written. This also means
 *    new keyed values can be added without breaking existing decoding
 *    routines. Once all data has been read from the archive, it may
 *    simply be released via CFRelease. 
 *    
 *    For those clients who wish to provide HIArchive editing features
 *    there are a few tricks necessary to achieve the desired behavior.
 *    A generic HIArchive editor will likely be used by clients to edit
 *    objects for which it has no direct knowledge (or which have not
 *    yet been designed). For instance, it may provide users with the
 *    ability to edit custom HIViews, including generic functionality
 *    to set the view's class identifier, title, frame, etc. In this
 *    case, it is necessary to instantiate the superclass
 *    ("com.apple.hiview") of the custom view object because the custom
 *    view class itself hasn't been registered within the editor.
 *    
 *    
 *    After the user has completed editing the object and desires to
 *    write out the archive, the editor must set the custom archive
 *    data to the object with HIObjectSetCustomArchiveData as a
 *    CFDictionary. Standard keys for initialization parameter types,
 *    names and values, class and superclass identifiers and CDEF
 *    procID are provided in HIToolbox/HIObject.h. Of particular
 *    importance are the object's class and superclass identifier.
 *    HIArchive uses these values to instantiate the proper object when
 *    loading the archive within the client's application. The
 *    parameter types, names and values are then automatically passed
 *    to the client object through its initialization event. 
 *    
 *    At this point, the object can simply be written into the archive
 *    with HIArchiveCreateForEncoding and HIArchiveEncodeCFType.
 *    HIArchive will handle writing the appropriate classID based on
 *    the custom data that was assigned earlier. 
 *    
 *    Generic HIArchive editing applications will also need to handle
 *    loading client archives. In this case, the archive is decoded in
 *    editing mode by passing the
 *    kHIArchiveDecodeSuperclassForUnregisteredObjects proxy bit in
 *    HIArchiveCreateForDecoding. When objects not registered in the
 *    current application are decoded, HIArchive will notice this and
 *    look into the custom data for the object's superclass identifier,
 *    instantiate an object of that type instead and attach the custom
 *    data to the newly created object. The editor can then look at the
 *    attached custom data with HIObjectCopyCustomArchiveData and
 *    provide it in the UI for the user to edit.
 */
typedef struct OpaqueHIArchiveRef*      HIArchiveRef;

/*
 *  Discussion:
 *    HIArchive errors
 */
enum {

  /*
   * The archive was created specifically for encoding or decoding but
   * passed into a non-corresponding routine.
   */
  hiArchiveTypeMismatchErr      = -6780,

  /*
   * The keyed value requested does not exist in the archive.
   */
  hiArchiveKeyNotAvailableErr   = -6781,

  /*
   * HIArchiveCopyEncodedData has been called and no more data may be
   * encoded.
   */
  hiArchiveEncodingCompleteErr  = -6782,

  /*
   * The HIObject does not support the archiving protocol.
   */
  hiArchiveHIObjectIgnoresArchivingErr = -6783
};


/*
 *  Discussion:
 *    HIArchiveCreateForDecoding options
 */
enum {

  /*
   * kDecodeSuperclassForUnregisteredObjects is passed to
   * HIArchiveCreateForDecoding indicating that if an HIObject's class
   * has not been registered before it is pulled from the archive,
   * HIArchiveCopyDecodedCFType will automatically instantiate the
   * unarchived object as its superclass if it exists. For instance, a
   * custom HIView of class "com.myco.customview" being unarchived will
   * be instantiated as class "com.apple.hiview" if your app has not
   * yet registered "com.myco.customview". This is useful for archive
   * editors that do not implement all objects contained in a client
   * archive. Note that only data written to the archive by the
   * superclass will be decoded. All data unique to the unregistered
   * subclass will be ignored. This option also signals the HIObject to
   * load its custom archive data so it can be accessed via
   * HIObjectCopyCustomArchiveData. HIArchive is unable to instantiate
   * unregistered objects whose superclasses are also unregistered.
   */
  kHIArchiveDecodeSuperclassForUnregisteredObjects = (1 << 0),

  /*
   * Indicates that an archive is being decoded by an archive editor.
   * This information is passed to the object being decoded via the
   * kEventParamDecodingForEditor parameter in the
   * kEventHIObjectInitialize and kEventHIObjectCreatedFromArchive
   * event. This option may be used in Mac OS X 10.5 and later.
   */
  kHIArchiveDecodingForEditor   = (1 << 1)
};

#if !__LP64__
/*
 *  HIArchiveGetTypeID()
 *  
 *  Summary:
 *    Returns the CFType identifier for an HIArchive object.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    A CFTypeID unique to HIArchive instances.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
HIArchiveGetTypeID(void)                                      AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveCreateForEncoding()
 *  
 *  Summary:
 *    Creates an HIArchive for use in encoding object information.
 *  
 *  Discussion:
 *    The created HIArchiveRef is a CFType and must be released via
 *    CFRelease.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outEncoder:
 *      An HIArchive reference which receives the created HIArchive on
 *      return.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveCreateForEncoding(HIArchiveRef * outEncoder)         AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveEncodeBoolean()
 *  
 *  Summary:
 *    Adds a keyed boolean value to the provided archive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEncoder:
 *      An HIArchiveRef to which the boolean value is added.
 *    
 *    inKey:
 *      The key associated with the boolean value used for later
 *      decoding.
 *    
 *    inBoolean:
 *      The boolean value to be encoded.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveEncodeBoolean(
  HIArchiveRef   inEncoder,
  CFStringRef    inKey,
  Boolean        inBoolean)                                   AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveEncodeNumber()
 *  
 *  Summary:
 *    Adds a keyed number value to the provided archive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEncoder:
 *      An HIArchiveRef to which the number value is added.
 *    
 *    inKey:
 *      The key associated with the number value used for later
 *      decoding.
 *    
 *    inNumberType:
 *      A CFNumberType describing the type of number value being
 *      encoded.
 *    
 *    inNumberValue:
 *      The number value to be encoded.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveEncodeNumber(
  HIArchiveRef   inEncoder,
  CFStringRef    inKey,
  CFNumberType   inNumberType,
  const void *   inNumberValue)                               AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveEncodeCFType()
 *  
 *  Summary:
 *    Adds a keyed CFType to the provided archive.
 *  
 *  Discussion:
 *    Encodes basic and property list based CFTypes including HIObjects
 *    supporting the archiving protocol. If the HIObject supports the
 *    archiving protocol, it will receive the kEventHIObjectEncode
 *    event during which it should encode all relevant state
 *    information. The kEventParamHIArchive parameter contains the
 *    archive into which data should be added.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEncoder:
 *      An HIArchiveRef to which the CFType is added.
 *    
 *    inKey:
 *      The key associated with the CFType used for later decoding.
 *    
 *    inCFType:
 *      The CFType to be encoded.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveEncodeCFType(
  HIArchiveRef   inEncoder,
  CFStringRef    inKey,
  CFTypeRef      inCFType)                                    AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveCopyEncodedData()
 *  
 *  Summary:
 *    Returns the encoded archive as a CFDataRef.
 *  
 *  Discussion:
 *    Compresses the archived data for storage and returns it as a
 *    CFDataRef. After the archived data is compressed, no further
 *    information may be encoded. Do not call this routine until the
 *    encoding process is complete.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inEncoder:
 *      An HIArchiveRef into which the archived data was compiled.
 *    
 *    outData:
 *      A CFData reference which receives the compressed archive data
 *      on return.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveCopyEncodedData(
  HIArchiveRef   inEncoder,
  CFDataRef *    outData)                                     AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveCreateForDecoding()
 *  
 *  Summary:
 *    Creates an HIArchive for use in decoding the object information
 *    contained in the provided CFData reference.
 *  
 *  Discussion:
 *    The created HIArchiveRef is a CFType and must be released via
 *    CFRelease.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inData:
 *      A CFDataRef containing a previously encoded archive.
 *    
 *    inOptions:
 *      The only option supported by this routine at present is
 *      kHIArchiveDecodeSuperclassForUnregisteredObjects. You may also
 *      pass zero for this parameter to get the default behavior.
 *    
 *    outDecoder:
 *      An HIArchive reference which receives the created HIArchive on
 *      return.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveCreateForDecoding(
  CFDataRef       inData,
  OptionBits      inOptions,
  HIArchiveRef *  outDecoder)                                 AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveDecodeBoolean()
 *  
 *  Summary:
 *    Pulls a keyed boolean value from the provided archive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDecoder:
 *      An HIArchiveRef from which the boolean value is pulled.
 *    
 *    inKey:
 *      The key associated with the boolean value used while encoding.
 *    
 *    outBoolean:
 *      The boolean value to be decoded.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveDecodeBoolean(
  HIArchiveRef   inDecoder,
  CFStringRef    inKey,
  Boolean *      outBoolean)                                  AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveDecodeNumber()
 *  
 *  Summary:
 *    Pulls a keyed number value from the provided archive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDecoder:
 *      An HIArchiveRef from which the number value is pulled.
 *    
 *    inKey:
 *      The key associated with the number value used while encoding.
 *    
 *    inNumberType:
 *      A CFNumberType describing the type of number value being
 *      encoded.
 *    
 *    outNumberValue:
 *      The number value to be decoded.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveDecodeNumber(
  HIArchiveRef   inDecoder,
  CFStringRef    inKey,
  CFNumberType   inNumberType,
  void *         outNumberValue)                              AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIArchiveCopyDecodedCFType()
 *  
 *  Summary:
 *    Pulls a keyed CFType from the provided archive.
 *  
 *  Discussion:
 *    Decodes basic and property list based CFTypes and HIObjects. If
 *    the CFType is an HIObject, it will be constructed and receive the
 *    kEventHIObjectInitialize event. The kEventParamHIArchive
 *    parameter contains the archive from which data should be
 *    retrieved.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDecoder:
 *      An HIArchiveRef from which the CFType value is pulled.
 *    
 *    inKey:
 *      The key associated with the CFType used while encoding.
 *    
 *    outCFType:
 *      The CFType to be decoded.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIArchiveCopyDecodedCFType(
  HIArchiveRef   inDecoder,
  CFStringRef    inKey,
  CFTypeRef *    outCFType)                                   AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HIARCHIVE__ */

