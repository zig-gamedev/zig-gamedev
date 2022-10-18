/*
     File:       HIServices/Pasteboard.h
 
     Contains:   Pasteboard Manager Interfaces.
 
     Copyright:  � 2003-2012 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PASTEBOARD__
#define __PASTEBOARD__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef __CGGEOMETRY__
#include <CoreGraphics/CGGeometry.h>
#endif
#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
CF_ASSUME_NONNULL_BEGIN
CF_IMPLICIT_BRIDGING_ENABLED



/*
 *  Pasteboard Manager
 *  
 *  Discussion:
 *    Pasteboard Manager is the replacement of the Scrap and Drag
 *    flavor APIs. Like the Scrap Manager, it is a cross process data
 *    transport mechanism. Unlike the Drag Manager, it is not bound to
 *    particular high level user actions to initiate and resolve the
 *    data transport. This allows the Pasteboard Manager to be used as
 *    the data transport mechanism for copy and paste, drag and drop,
 *    services, as well as generic cross process communication.
 *    
 *    
 *    Like scraps, pasteboards are local references to global, cross
 *    process, resources. Although, where scraps exist forever,
 *    pasteboard are CFTypes and should be released at the end of their
 *    usefulness. Multiple pasteboard references in a single
 *    application may point to the same global pasteboard resource.
 *    Once the global pasteboard resource has been cleared, all
 *    pasteboard references to that global resource in the application
 *    share ownership allowing any of them to add data to the
 *    pasteboard. Of course, this is not a free for all. Once a flavor
 *    has been added it can not be overwritten by another local
 *    reference. Calling CFRelease() on any one of these references
 *    will implicitly call in promises, but only those made through the
 *    particular pasteboard reference being released. In this way,
 *    local pasteboard references in various components of the
 *    application can add data, make promises, keep those promises, and
 *    release their local pasteboard reference independently of the
 *    other components. 
 *    
 *    For long lived pasteboards, like the clipboard or find
 *    pasteboards, the suggested usage model is for a component to
 *    create a local reference to a pasteboard at the beginning of its
 *    life cycle, hold on to it, and release it at the end of its life
 *    cycle. As long as a pasteboard reference in any application still
 *    points to the global pasteboard resource, the global resource
 *    will not be released. When the final reference is released,
 *    promises will not be called in as the global pasteboard resource
 *    will be released. Only if you want to guarantee a global
 *    pasteboard resource continues to exist beyond the life of your
 *    application should you refrain from calling CFRelease(). In this
 *    case, be sure to call PasteboardResolvePromises() to make sure
 *    any promises are called in. This is not necessary for the
 *    clipboard and find pasteboards as they are retained by the system
 *    and never released. 
 *    
 *    Like the Drag Manager, the Pasteboard Manager allows the use of
 *    multiple items each with its own set of flavors. When using the
 *    clipboard or find pasteboards, it's traditionally been common to
 *    only use the first item, but now multiple items can be copied as
 *    is the case in the Finder. This also opens up the ability for
 *    services to act on multiple items. Another difference from both
 *    the Scrap and Drag Managers is the use of Uniform Type Identifier
 *    based flavor types rather than four character code OSTypes. These
 *    have several advantages. They allow compatibility with Cocoa's
 *    NSPasteboard, more accurately describe the data being
 *    transported, provide a type inheritance mechanism and allow
 *    namespacing with a reverse DNS scheme.
 */
typedef struct CF_BRIDGED_TYPE(id) OpaquePasteboardRef*     PasteboardRef;
typedef void *                          PasteboardItemID;
/* Pasteboard Manager error codes*/
enum {
  badPasteboardSyncErr          = -25130, /* pasteboard has been modified and must be synchronized*/
  badPasteboardIndexErr         = -25131, /* item index does not exist*/
  badPasteboardItemErr          = -25132, /* item reference does not exist*/
  badPasteboardFlavorErr        = -25133, /* item flavor does not exist*/
  duplicatePasteboardFlavorErr  = -25134, /* item flavor already exists*/
  notPasteboardOwnerErr         = -25135, /* client did not clear the pasteboard*/
  noPasteboardPromiseKeeperErr  = -25136 /* a promise is being added without a promise keeper*/
};


/*
 *  PasteboardSyncFlags
 *  
 *  Summary:
 *    The following constants are used by the PasteboardSynchronize()
 *    routine to indicate the status of the local pasteboard reference
 *    in relation to the global, cross process pasteboard resource.
 */
typedef CF_OPTIONS(OptionBits, PasteboardSyncFlags) {

  /*
   * Indicates that the global pasteboard resource has been modified
   * since the last time it was accessed via the local pasteboard
   * reference. The call to PasteboardSynchronize() has updated the
   * local pasteboard reference to sync it up with the global resource.
   * This is a good time to see what new information has been placed on
   * the pasteboard to determine whether any tasty flavors have been
   * added and possibly enable pasting.
   */
  kPasteboardModified           = (1 << 0),

  /*
   * Indicates that the global pasteboard resource was most recently
   * cleared by the this application. Any local pasteboard reference in
   * the client application may add data to the global pasteboard
   * resource.
   */
  kPasteboardClientIsOwner      = (1 << 1)
};

/*
 *  Pasteboard File Promising
 *  
 *  Summary:
 *    With the FSSpec type being deprecated and removed for 64 bit it is necessary
 *    to introduce a replacement for kDragFlavorTypePromiseHFS. The replacement comes
 *    in the form of two new Uniform Type Identifiers specifically for use with the
 *    pasteboard and promised files. Like the old HFS promise mechanism, the new UTI
 *    based method still requires a multistage handshake between sender and receiver
 *    but the process is somewhat simplified.
 *    
 *    Order of operations on copy or drag
 *    
 *    1) The sender promises kPasteboardTypeFileURLPromise for a file yet to be created.
 *    2) The sender adds kPasteboardTypeFilePromiseContent containing the UTI describing
 *          the file's content.
 *    
 *    Order of operations on paste or drop
 *    
 *    3) The receiver asks for kPasteboardTypeFilePromiseContent to decide if it wants the file.
 *    4) The receiver sets the paste location with PasteboardSetPasteLocation.
 *    5) The receiver asks for kPasteboardTypeFileURLPromise.
 *    6) The sender's promise callback for kPasteboardTypeFileURLPromise is called.
 *    7) The sender uses PasteboardCopyPasteLocation to retrieve the paste location, creates the file
 *          and keeps its kPasteboardTypeFileURLPromise promise.
 *
 *    Automatic translation support has been added so clients operating in the modern
 *    kPasteboardTypeFileURLPromise and kPasteboardTypeFilePromiseContent world can continue
 *    to communicate properly with clients using the traditional kDragFlavorTypePromiseHFS and
 *    kDragPromisedFlavor model.
 */

/*
 *  kPasteboardTypeFileURLPromise
 *  
 *  Discussion:
 *    A UTF-8 encoded promised file url on the pasteboard to a file
 *    which does not yet exist.
 */
#define kPasteboardTypeFileURLPromise   CFSTR("com.apple.pasteboard.promised-file-url")

/*
 *  kPasteboardTypeFilePromiseContent
 *  
 *  Discussion:
 *    A UTF-8 encoded UTI describing the type of data to be contained
 *    within the promised file.
 */
#define kPasteboardTypeFilePromiseContent  CFSTR("com.apple.pasteboard.promised-file-content-type")

/*
 *  PasteboardFlavorFlags
 *  
 *  Summary:
 *    Pasteboard Flavor Flags
 *  
 *  Discussion:
 *    The following constants are used to tag pasteboard item flavors
 *    with bits of useful information. The first five are settable by
 *    the client via PasteboardPutItemFlavor(). They may all be
 *    received via PasteboardGetItemFlavorFlags().
 */
typedef CF_OPTIONS(OptionBits, PasteboardFlavorFlags)  {

  /*
   * No additional information exists for this flavor.
   */
  kPasteboardFlavorNoFlags      = 0,

  /*
   * Only the process which has added this flavor can see it. If the
   * process that owns the pasteboard places this flag on a flavor
   * you'll never see it as the receiver so there's no reason to test
   * for it.
   */
  kPasteboardFlavorSenderOnly   = (1 << 0),

  /*
   * The data for this flavor was translated in some manner by the
   * sender before adding it to the pasteboard. Flavors marked with
   * this flag are not stored by the Finder in clipping files.
   */
  kPasteboardFlavorSenderTranslated = (1 << 1),

  /*
   * Set by the sender if the flavor data should not be saved by the
   * receiver. The data contained may become stale after the end of the
   * drag. Flavors marked with this flag are not stored by the Finder
   * in clipping files.
   */
  kPasteboardFlavorNotSaved     = (1 << 2),

  /*
   * Like kPasteboardFlavorSenderOnly, when the sender adds this flag,
   * the presence of this flavor will not be made known when
   * PasteboardCopyItemFlavors() is called by the receiver. However,
   * the flavor flags and data can be accessed when explicitly
   * requested via PasteboardGetItemFlavorFlags() or
   * PasteboardCopyItemFlavorData(). This allows a suite of
   * applications to communicate without exporting their method of
   * communication.
   */
  kPasteboardFlavorRequestOnly  = (1 << 3),

  /*
   * The data for this flavor is provided by the Translation Manager as
   * a convenience to receivers. Flavors marked with this flag are not
   * stored by the Finder in clipping files. This flag can not be added
   * by clients. It is automatically added by the Pasteboard Manager
   * when appropriate.
   */
  kPasteboardFlavorSystemTranslated = (1 << 8),

  /*
   * The data for this flavor has not yet been added to the pasteboard
   * by the sender. This may have been done as a performance measure if
   * the data is expensive to generate. A request by the receiver to
   * obtain the data will cause the promise to be kept by the sender.
   * This flag can not be added by clients. It is automatically added
   * by the Pasteboard Manager when appropriate.
   */
  kPasteboardFlavorPromised     = (1 << 9)
};


/*
 *  PasteboardStandardLocation
 *  
 *  Summary:
 *    Pasteboard Standard Drop Locations
 *  
 *  Discussion:
 *    The following constants define common "meta" paste locations.
 */
typedef CF_ENUM(OSType, PasteboardStandardLocation) {

  /*
   * The paste or drop location was in the trash.  This is set when a
   * drag is dropped on the trash icon or a paste occurs within the
   * trash.  Setting this standard paste location sets the traditional
   * paste location to an alias to the trash folder automatically.
   */
  kPasteboardStandardLocationTrash = 'trsh',

  /*
   * The receiver did not specify a paste location. This is the default.
   */
  kPasteboardStandardLocationUnknown = 'unkn'
};

/*
 *  PasteboardGetTypeID()
 *  
 *  Summary:
 *    Returns the CFType identifier for a pasteboard object.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    A CFTypeID unique to pasteboard instances.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
PasteboardGetTypeID(void)                                     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#define kPasteboardClipboard            CFSTR("com.apple.pasteboard.clipboard")
#define kPasteboardFind                 CFSTR("com.apple.pasteboard.find")
#define kPasteboardUniqueName           (CFStringRef)NULL
/*
 *  PasteboardCreate()
 *  
 *  Summary:
 *    Creates a local pasteboard reference to the global pasteboard
 *    resource of the same name.
 *  
 *  Discussion:
 *    If the the global pasteboard resource doesn't yet exist,
 *    PasteboardCreate creates a new one with the provided name.
 *    Pasteboard names are typically represented in a reverse DNS
 *    scheme (i.e., com.apple.pasteboard.clipboard). Multiple local
 *    pasteboard references may point to the same global pasteboard
 *    resource within an application. An application, window, plug-in,
 *    or object may each hold separate references. These should be held
 *    and used for the life time of the objects. Pasteboard references
 *    are CFTypes. Their memory should be released by calling
 *    CFRelease(). CFRelease() automatically resolves all promises made
 *    to the global pasteboard resource through the reference being
 *    released.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inName:
 *      The name of the pasteboard to reference or create. Passing
 *      kPasteboardUniqueName, or NULL, will guarantee that a new
 *      global pasteboard resource is created. kPasteboardClipboard is
 *      the traditional copy and paste pasteboard. kPasteboardFind is
 *      compatible with Cocoa's global find pasteboard.
 *    
 *    outPasteboard:
 *      The created pasteboard reference.
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
PasteboardCreate(
  CFStringRef _Nullable     inName,              /* can be NULL */
  PasteboardRef _Nullable * _Nonnull outPasteboard CF_RETURNS_RETAINED)                             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardSynchronize()
 *  
 *  Summary:
 *    Compares a local pasteboard reference with the global pasteboard
 *    resource to which it refers, determining whether the global
 *    pasteboard resource has been modified. If so, it updates the
 *    local pasteboard reference to reflect the change. The pasteboard
 *    reference provided is always brought up to date. This routine is
 *    lightweight whether a synchronization is required or not.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *  
 *  Result:
 *    A set of pasteboard synchronization flags.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern PasteboardSyncFlags 
PasteboardSynchronize(PasteboardRef inPasteboard)             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardClear()
 *  
 *  Summary:
 *    Clears all data from the global pasteboard resource associated
 *    with the pasteboard reference provided. The application now owns
 *    the pasteboard and data may be placed onto the global pasteboard
 *    resource from all local pasteboards in the client application
 *    which reference it. PasteboardClear must be called before the
 *    pasteboard can be modified.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
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
PasteboardClear(PasteboardRef inPasteboard)                   AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardCopyName()
 *  
 *  Summary:
 *    Copies the name of the given pasteboard. Useful for discovering
 *    the name of a uniquely named pasteboard so other processes may
 *    access it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    outName:
 *      On return, a CFString reference to the pasteboard's name. This
 *      string must be released by the client.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
PasteboardCopyName(
  PasteboardRef   inPasteboard,
  CFStringRef _Nullable * _Nonnull outName CF_RETURNS_RETAINED)                                    AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  PasteboardGetItemCount()
 *  
 *  Summary:
 *    Returns the number of items on the pasteboard.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    outItemCount:
 *      An ItemCount reference which receives number of pasteboard
 *      items.
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
PasteboardGetItemCount(
  PasteboardRef   inPasteboard,
  ItemCount *     outItemCount)                               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardGetItemIdentifier()
 *  
 *  Summary:
 *    Returns the item identifier for the nth pasteboard item.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    inIndex:
 *      A 1-based CFIndex requesting the nth pasteboard item reference.
 *    
 *    outItem:
 *      A PasteboardItemID which receives the nth pasteboard item
 *      reference.
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
PasteboardGetItemIdentifier(
  PasteboardRef       inPasteboard,
  CFIndex             inIndex,
  PasteboardItemID _Nullable *  _Nonnull outItem)                                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardCopyItemFlavors()
 *  
 *  Summary:
 *    Returns the array of flavors for the provided pasteboard
 *    reference.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    inItem:
 *      A pasteboard item identifier containing the flavors of interest.
 *    
 *    outFlavorTypes:
 *      A CFArrayRef reference which receives the array of Uniform Type
 *      Identifier based flavor types.  It is the client's
 *      responsibility to release the flavor array via CFRelease().
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
PasteboardCopyItemFlavors(
  PasteboardRef      inPasteboard,
  PasteboardItemID   inItem,
  CFArrayRef _Nullable *  _Nonnull outFlavorTypes CF_RETURNS_RETAINED)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardGetItemFlavorFlags()
 *  
 *  Summary:
 *    Returns the array of flags for the provided flavor, including
 *    implicit translations included by the system automatically.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    inItem:
 *      A pasteboard item identifier containing the flavor of interest.
 *    
 *    inFlavorType:
 *      A Uniform Type Identifier based flavor type whose flags are
 *      being interrogated.
 *    
 *    outFlags:
 *      A PasteboardFlavorFlags reference which receives the flavor
 *      flags.
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
PasteboardGetItemFlavorFlags(
  PasteboardRef            inPasteboard,
  PasteboardItemID         inItem,
  CFStringRef              inFlavorType,
  PasteboardFlavorFlags *  outFlags)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardCopyItemFlavorData()
 *  
 *  Summary:
 *    Returns the data for the provided flavor.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    inItem:
 *      A pasteboard item identifier containing the flavor of interest.
 *    
 *    inFlavorType:
 *      A Uniform Type Identifier-based flavor type whose data is being
 *      retrieved.
 *    
 *    outData:
 *      A CFDataRef reference which receives the flavor data. It is the
 *      client's responsibility to release the data via CFRelease().
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
PasteboardCopyItemFlavorData(
  PasteboardRef         inPasteboard,
  PasteboardItemID      inItem,
  CFStringRef           inFlavorType,
  CFDataRef _Nullable *  _Nonnull outData CF_RETURNS_RETAINED)                                 AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#define kPasteboardPromisedData             (CFDataRef)NULL
/*
 *  PasteboardPutItemFlavor()
 *  
 *  Summary:
 *    Adds flavor data or a promise to the global pasteboard resource.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    inItem:
 *      A pasteboard item identifier in which to add the data or
 *      promise. Pasteboard item identifiers are unique values created
 *      by the owning application when adding pasteboard item flavor
 *      information to a drag. Additional flavors may be added to an
 *      existing item by using the same item identifier value.
 *      Depending on the application, it might be easier to use
 *      internal memory addresses as item identifiers (as long as each
 *      item being added has a unique item reference number).
 *      Pasteboard item identifiers should only be interpreted by the
 *      owning application.
 *    
 *    inFlavorType:
 *      A Uniform Type Identifier based flavor type associated with the
 *      data. If multiple flavors are to be added to an item, the
 *      owning application should add them in order of preference or
 *      richness as determined by the owing application.  The ordering
 *      will be retained when viewed by the receiving application.
 *    
 *    inData:
 *      A CFDataRef reference which receives the flavor data. Passing
 *      kPasteboardPromisedData, or NULL, indicates the data is
 *      promised. This is useful if the data is expensive to generate.
 *      Making promises requires the sender to also implement a promise
 *      keeper which must be set before the promise is made.
 *    
 *    inFlags:
 *      A PasteboardFlavorFlags set of flags to attach to the data.
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
PasteboardPutItemFlavor(
  PasteboardRef           inPasteboard,
  PasteboardItemID        inItem,
  CFStringRef             inFlavorType,
  CFDataRef  _Nullable    inData,             /* can be NULL */
  PasteboardFlavorFlags   inFlags)                            AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardCopyPasteLocation()
 *  
 *  Summary:
 *    Called by the owner of a pasteboard while providing promised data
 *    to determine the paste location set by the pasteboard receiver.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    outPasteLocation:
 *      A CFURL reference describing the paste location. It is the
 *      client's responsibility to release the data via CFRelease().
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
PasteboardCopyPasteLocation(
  PasteboardRef         inPasteboard,
  CFURLRef _Nullable *  _Nonnull  outPasteLocation CF_RETURNS_RETAINED)                           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PasteboardSetPasteLocation()
 *  
 *  Summary:
 *    Called by the receiver of a pasteboard before requesting any item
 *    flavor data via PasteboardCopyItemFlavorData.  When a sending
 *    application's pasteboard promise keeper is called to provide data
 *    to the receiver, PasteboardGetDropLocation can be called to
 *    determine the paste location while providing data.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      A local pasteboard reference.
 *    
 *    inPasteLocation:
 *      A CFURL describing the paste location.
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
PasteboardSetPasteLocation(
  PasteboardRef   inPasteboard,
  CFURLRef        inPasteLocation)                            AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



/*
 *  PasteboardPromiseKeeperProcPtr
 *  
 *  Summary:
 *    Callback for providing data previously promised on the pasteboard.
 *  
 *  Parameters:
 *    
 *    pasteboard:
 *      The local pasteboard reference on which the promise was made.
 *    
 *    item:
 *      The pasteboard item identifier containing the promised flavor.
 *    
 *    flavorType:
 *      The Uniform Type Identifier based flavor type for which the
 *      promised data is being requested.
 *    
 *    context:
 *      The value passed as the context in PasteboardSetPromiseKeeper().
 *  
 *  Result:
 *    An operating system result code.
 */
typedef CALLBACK_API_C( OSStatus , PasteboardPromiseKeeperProcPtr )(PasteboardRef pasteboard, PasteboardItemID item, CFStringRef flavorType, void * _Nullable context);
/*
 *  PasteboardSetPromiseKeeper()
 *  
 *  Summary:
 *    Associates a promise keeper callback with a local pasteboard
 *    reference. The promise keeper must be set before a promise is
 *    made.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      The local pasteboard reference on which promises will be made.
 *    
 *    inPromiseKeeper:
 *      A PasteboardPromiseKeeperProcPtr promise keeper proc.
 *    
 *    inContext:
 *      The value passed in this parameter is passed on to your promise
 *      keeper proc when it is called.
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
PasteboardSetPromiseKeeper(
  PasteboardRef                    inPasteboard,
  PasteboardPromiseKeeperProcPtr   inPromiseKeeper,
  void *  _Nullable                inContext)                 AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#define kPasteboardResolveAllPromises           (PasteboardRef)NULL
/*
 *  PasteboardResolvePromises()
 *  
 *  Summary:
 *    Resolves promises on the provided local pasteboard reference. If
 *    multiple local pasteboard references to the same global
 *    pasteboard resource exist, only those promises made through the
 *    provided reference are resolved.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inPasteboard:
 *      The local pasteboard reference for which promises will be
 *      resolved. Passing kPasteboardResolveAllPromises, or NULL, will
 *      cause all promises on all global pasteboard resources currently
 *      owned by this application to be resolved.
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
PasteboardResolvePromises(PasteboardRef inPasteboard)         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


CF_IMPLICIT_BRIDGING_DISABLED
CF_ASSUME_NONNULL_END


#ifdef __cplusplus
}
#endif

#endif /* __PASTEBOARD__ */

