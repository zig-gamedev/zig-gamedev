/*
     File:       CarbonCore/Collections.h
 
     Contains:   Collection Manager Interfaces
                 The contents of this header file are deprecated.
                 Use Foundation or CoreFoundation collection objects instead.
 
     Copyright:  © 1989-2011 by Apple Inc. All rights reserved.
*/
#ifndef __COLLECTIONS__
#define __COLLECTIONS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*************/
/* Constants */
/*************/
/* Convenience constants for functions which optionally return values */
enum {
  kCollectionDontWantTag        = 0,
  kCollectionDontWantId         = 0,
  kCollectionDontWantSize       = 0,
  kCollectionDontWantAttributes = 0,
  kCollectionDontWantIndex      = 0,
  kCollectionDontWantData       = 0
};


/* attributes bits */
enum {
  kCollectionNoAttributes       = 0x00000000, /* no attributes bits set */
  kCollectionAllAttributes      = (int)0xFFFFFFFF, /* all attributes bits set */
  kCollectionUserAttributes     = 0x0000FFFF, /* user attributes bits */
  kCollectionDefaultAttributes  = 0x40000000 /* default attributes - unlocked, persistent */
};


/* 
    Attribute bits 0 through 15 (entire low word) are reserved for use by the application.
    Attribute bits 16 through 31 (entire high word) are reserved for use by the Collection Manager.
    Only bits 31 (kCollectionLockBit) and 30 (kCollectionPersistenceBit) currently have meaning.
*/
enum {
  kCollectionUser0Bit           = 0,
  kCollectionUser1Bit           = 1,
  kCollectionUser2Bit           = 2,
  kCollectionUser3Bit           = 3,
  kCollectionUser4Bit           = 4,
  kCollectionUser5Bit           = 5,
  kCollectionUser6Bit           = 6,
  kCollectionUser7Bit           = 7,
  kCollectionUser8Bit           = 8,
  kCollectionUser9Bit           = 9,
  kCollectionUser10Bit          = 10,
  kCollectionUser11Bit          = 11,
  kCollectionUser12Bit          = 12,
  kCollectionUser13Bit          = 13,
  kCollectionUser14Bit          = 14,
  kCollectionUser15Bit          = 15,
  kCollectionReserved0Bit       = 16,
  kCollectionReserved1Bit       = 17,
  kCollectionReserved2Bit       = 18,
  kCollectionReserved3Bit       = 19,
  kCollectionReserved4Bit       = 20,
  kCollectionReserved5Bit       = 21,
  kCollectionReserved6Bit       = 22,
  kCollectionReserved7Bit       = 23,
  kCollectionReserved8Bit       = 24,
  kCollectionReserved9Bit       = 25,
  kCollectionReserved10Bit      = 26,
  kCollectionReserved11Bit      = 27,
  kCollectionReserved12Bit      = 28,
  kCollectionReserved13Bit      = 29,
  kCollectionPersistenceBit     = 30,
  kCollectionLockBit            = 31
};


/* attribute masks */
enum {
  kCollectionUser0Mask          = 1UL << kCollectionUser0Bit,
  kCollectionUser1Mask          = 1UL << kCollectionUser1Bit,
  kCollectionUser2Mask          = 1UL << kCollectionUser2Bit,
  kCollectionUser3Mask          = 1UL << kCollectionUser3Bit,
  kCollectionUser4Mask          = 1UL << kCollectionUser4Bit,
  kCollectionUser5Mask          = 1UL << kCollectionUser5Bit,
  kCollectionUser6Mask          = 1UL << kCollectionUser6Bit,
  kCollectionUser7Mask          = 1UL << kCollectionUser7Bit,
  kCollectionUser8Mask          = 1UL << kCollectionUser8Bit,
  kCollectionUser9Mask          = 1UL << kCollectionUser9Bit,
  kCollectionUser10Mask         = 1UL << kCollectionUser10Bit,
  kCollectionUser11Mask         = 1UL << kCollectionUser11Bit,
  kCollectionUser12Mask         = 1UL << kCollectionUser12Bit,
  kCollectionUser13Mask         = 1UL << kCollectionUser13Bit,
  kCollectionUser14Mask         = 1UL << kCollectionUser14Bit,
  kCollectionUser15Mask         = 1UL << kCollectionUser15Bit,
  kCollectionReserved0Mask      = 1UL << kCollectionReserved0Bit,
  kCollectionReserved1Mask      = 1UL << kCollectionReserved1Bit,
  kCollectionReserved2Mask      = 1UL << kCollectionReserved2Bit,
  kCollectionReserved3Mask      = 1UL << kCollectionReserved3Bit,
  kCollectionReserved4Mask      = 1UL << kCollectionReserved4Bit,
  kCollectionReserved5Mask      = 1UL << kCollectionReserved5Bit,
  kCollectionReserved6Mask      = 1UL << kCollectionReserved6Bit,
  kCollectionReserved7Mask      = 1UL << kCollectionReserved7Bit,
  kCollectionReserved8Mask      = 1UL << kCollectionReserved8Bit,
  kCollectionReserved9Mask      = 1UL << kCollectionReserved9Bit,
  kCollectionReserved10Mask     = 1UL << kCollectionReserved10Bit,
  kCollectionReserved11Mask     = 1UL << kCollectionReserved11Bit,
  kCollectionReserved12Mask     = 1UL << kCollectionReserved12Bit,
  kCollectionReserved13Mask     = 1UL << kCollectionReserved13Bit,
  kCollectionPersistenceMask    = 1UL << kCollectionPersistenceBit,
  kCollectionLockMask           = 1UL << kCollectionLockBit
};


/***********/
/* Types   */
/***********/
/* abstract data type for a collection */
typedef struct OpaqueCollection*        Collection;
/* collection member 4 byte tag */
typedef FourCharCode                    CollectionTag;
typedef CALLBACK_API( OSErr , CollectionFlattenProcPtr )(SInt32 size, void *data, void *refCon);
typedef CALLBACK_API( OSErr , CollectionExceptionProcPtr )(Collection c, OSErr status);
typedef STACK_UPP_TYPE(CollectionFlattenProcPtr)                CollectionFlattenUPP;
typedef STACK_UPP_TYPE(CollectionExceptionProcPtr)              CollectionExceptionUPP;
/*
 *  NewCollectionFlattenUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern CollectionFlattenUPP
NewCollectionFlattenUPP(CollectionFlattenProcPtr userRoutine) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  NewCollectionExceptionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern CollectionExceptionUPP
NewCollectionExceptionUPP(CollectionExceptionProcPtr userRoutine) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeCollectionFlattenUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeCollectionFlattenUPP(CollectionFlattenUPP userUPP)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeCollectionExceptionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeCollectionExceptionUPP(CollectionExceptionUPP userUPP) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeCollectionFlattenUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeCollectionFlattenUPP(
  SInt32                size,
  void *                data,
  void *                refCon,
  CollectionFlattenUPP  userUPP)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeCollectionExceptionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeCollectionExceptionUPP(
  Collection              c,
  OSErr                   status,
  CollectionExceptionUPP  userUPP)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline CollectionFlattenUPP                                 NewCollectionFlattenUPP(CollectionFlattenProcPtr userRoutine) { return userRoutine; }
    inline CollectionExceptionUPP                               NewCollectionExceptionUPP(CollectionExceptionProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeCollectionFlattenUPP(CollectionFlattenUPP) { }
    inline void                                                 DisposeCollectionExceptionUPP(CollectionExceptionUPP) { }
    inline OSErr                                                InvokeCollectionFlattenUPP(SInt32 size, void * data, void * refCon, CollectionFlattenUPP userUPP) { return (*userUPP)(size, data, refCon); }
    inline OSErr                                                InvokeCollectionExceptionUPP(Collection c, OSErr status, CollectionExceptionUPP userUPP) { return (*userUPP)(c, status); }
  #else
    #define NewCollectionFlattenUPP(userRoutine)                ((CollectionFlattenUPP)userRoutine)
    #define NewCollectionExceptionUPP(userRoutine)              ((CollectionExceptionUPP)userRoutine)
    #define DisposeCollectionFlattenUPP(userUPP)
    #define DisposeCollectionExceptionUPP(userUPP)
    #define InvokeCollectionFlattenUPP(size, data, refCon, userUPP) (*userUPP)(size, data, refCon)
    #define InvokeCollectionExceptionUPP(c, status, userUPP)    (*userUPP)(c, status)
  #endif
#endif

/*********************************************/
/************* Public interfaces *************/
/*********************************************/
/*
 *  NewCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern Collection 
NewCollection(void)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  DisposeCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern void 
DisposeCollection(Collection c)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  CloneCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern Collection 
CloneCollection(Collection c)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  CountCollectionOwners()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern SInt32 
CountCollectionOwners(Collection c)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  RetainCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RetainCollection(Collection c)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  ReleaseCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ReleaseCollection(Collection c)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetCollectionRetainCount()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern ItemCount 
GetCollectionRetainCount(Collection c)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  CopyCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern Collection 
CopyCollection(
  Collection   srcCollection,
  Collection   dstCollection)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetCollectionDefaultAttributes()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern SInt32 
GetCollectionDefaultAttributes(Collection c)                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  SetCollectionDefaultAttributes()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern void 
SetCollectionDefaultAttributes(
  Collection   c,
  SInt32       whichAttributes,
  SInt32       newAttributes)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  CountCollectionItems()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern SInt32 
CountCollectionItems(Collection c)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  AddCollectionItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
AddCollectionItem(
  Collection      c,
  CollectionTag   tag,
  SInt32          id,
  SInt32          itemSize,
  const void *    itemData)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetCollectionItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetCollectionItem(
  Collection      c,
  CollectionTag   tag,
  SInt32          id,
  SInt32 *        itemSize,
  void *          itemData)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  RemoveCollectionItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
RemoveCollectionItem(
  Collection      c,
  CollectionTag   tag,
  SInt32          id)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  SetCollectionItemInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
SetCollectionItemInfo(
  Collection      c,
  CollectionTag   tag,
  SInt32          id,
  SInt32          whichAttributes,
  SInt32          newAttributes)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetCollectionItemInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetCollectionItemInfo(
  Collection      c,
  CollectionTag   tag,
  SInt32          id,
  SInt32 *        itemIndex,
  SInt32 *        itemSize,
  SInt32 *        attributes)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  ReplaceIndexedCollectionItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
ReplaceIndexedCollectionItem(
  Collection    c,
  SInt32        itemIndex,
  SInt32        itemSize,
  const void *  itemData)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetIndexedCollectionItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetIndexedCollectionItem(
  Collection   c,
  SInt32       itemIndex,
  SInt32 *     itemSize,
  void *       itemData)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  RemoveIndexedCollectionItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
RemoveIndexedCollectionItem(
  Collection   c,
  SInt32       itemIndex)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  SetIndexedCollectionItemInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
SetIndexedCollectionItemInfo(
  Collection   c,
  SInt32       itemIndex,
  SInt32       whichAttributes,
  SInt32       newAttributes)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetIndexedCollectionItemInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetIndexedCollectionItemInfo(
  Collection       c,
  SInt32           itemIndex,
  CollectionTag *  tag,
  SInt32 *         id,
  SInt32 *         itemSize,
  SInt32 *         attributes)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  CollectionTagExists()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern Boolean 
CollectionTagExists(
  Collection      c,
  CollectionTag   tag)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  CountCollectionTags()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern SInt32 
CountCollectionTags(Collection c)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetIndexedCollectionTag()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetIndexedCollectionTag(
  Collection       c,
  SInt32           tagIndex,
  CollectionTag *  tag)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  CountTaggedCollectionItems()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern SInt32 
CountTaggedCollectionItems(
  Collection      c,
  CollectionTag   tag)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetTaggedCollectionItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetTaggedCollectionItem(
  Collection      c,
  CollectionTag   tag,
  SInt32          whichItem,
  SInt32 *        itemSize,
  void *          itemData)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetTaggedCollectionItemInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetTaggedCollectionItemInfo(
  Collection      c,
  CollectionTag   tag,
  SInt32          whichItem,
  SInt32 *        id,
  SInt32 *        itemIndex,
  SInt32 *        itemSize,
  SInt32 *        attributes)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  PurgeCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern void 
PurgeCollection(
  Collection   c,
  SInt32       whichAttributes,
  SInt32       matchingAttributes)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  PurgeCollectionTag()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern void 
PurgeCollectionTag(
  Collection      c,
  CollectionTag   tag)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  EmptyCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern void 
EmptyCollection(Collection c)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FlattenCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
FlattenCollection(
  Collection             c,
  CollectionFlattenUPP   flattenProc,
  void *                 refCon)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FlattenPartialCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
FlattenPartialCollection(
  Collection             c,
  CollectionFlattenUPP   flattenProc,
  void *                 refCon,
  SInt32                 whichAttributes,
  SInt32                 matchingAttributes)                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  UnflattenCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
UnflattenCollection(
  Collection             c,
  CollectionFlattenUPP   flattenProc,
  void *                 refCon)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetCollectionExceptionProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern CollectionExceptionUPP 
GetCollectionExceptionProc(Collection c)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  SetCollectionExceptionProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern void 
SetCollectionExceptionProc(
  Collection               c,
  CollectionExceptionUPP   exceptionProc)                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetNewCollection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern Collection 
GetNewCollection(SInt16 collectionID)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/**********************************************************************/
/************** Utility routines for handle-based access **************/
/**********************************************************************/
/*
 *  AddCollectionItemHdl()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
AddCollectionItemHdl(
  Collection      aCollection,
  CollectionTag   tag,
  SInt32          id,
  Handle          itemData)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetCollectionItemHdl()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetCollectionItemHdl(
  Collection      aCollection,
  CollectionTag   tag,
  SInt32          id,
  Handle          itemData)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  ReplaceIndexedCollectionItemHdl()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
ReplaceIndexedCollectionItemHdl(
  Collection   aCollection,
  SInt32       itemIndex,
  Handle       itemData)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  GetIndexedCollectionItemHdl()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
GetIndexedCollectionItemHdl(
  Collection   aCollection,
  SInt32       itemIndex,
  Handle       itemData)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FlattenCollectionToHdl()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
FlattenCollectionToHdl(
  Collection   aCollection,
  Handle       flattened)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  UnflattenCollectionFromHdl()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CollectionsLib 1.0 and later
 */
extern OSErr 
UnflattenCollectionFromHdl(
  Collection   aCollection,
  Handle       flattened)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



#if OLDROUTINENAMES
enum {
  dontWantTag                   = kCollectionDontWantTag,
  dontWantId                    = kCollectionDontWantId,
  dontWantSize                  = kCollectionDontWantSize,
  dontWantAttributes            = kCollectionDontWantAttributes,
  dontWantIndex                 = kCollectionDontWantIndex,
  dontWantData                  = kCollectionDontWantData
};

enum {
  noCollectionAttributes        = kCollectionNoAttributes,
  allCollectionAttributes       = kCollectionAllAttributes,
  userCollectionAttributes      = kCollectionUserAttributes,
  defaultCollectionAttributes   = kCollectionDefaultAttributes
};

enum {
  collectionUser0Bit            = kCollectionUser0Bit,
  collectionUser1Bit            = kCollectionUser1Bit,
  collectionUser2Bit            = kCollectionUser2Bit,
  collectionUser3Bit            = kCollectionUser3Bit,
  collectionUser4Bit            = kCollectionUser4Bit,
  collectionUser5Bit            = kCollectionUser5Bit,
  collectionUser6Bit            = kCollectionUser6Bit,
  collectionUser7Bit            = kCollectionUser7Bit,
  collectionUser8Bit            = kCollectionUser8Bit,
  collectionUser9Bit            = kCollectionUser9Bit,
  collectionUser10Bit           = kCollectionUser10Bit,
  collectionUser11Bit           = kCollectionUser11Bit,
  collectionUser12Bit           = kCollectionUser12Bit,
  collectionUser13Bit           = kCollectionUser13Bit,
  collectionUser14Bit           = kCollectionUser14Bit,
  collectionUser15Bit           = kCollectionUser15Bit,
  collectionReserved0Bit        = kCollectionReserved0Bit,
  collectionReserved1Bit        = kCollectionReserved1Bit,
  collectionReserved2Bit        = kCollectionReserved2Bit,
  collectionReserved3Bit        = kCollectionReserved3Bit,
  collectionReserved4Bit        = kCollectionReserved4Bit,
  collectionReserved5Bit        = kCollectionReserved5Bit,
  collectionReserved6Bit        = kCollectionReserved6Bit,
  collectionReserved7Bit        = kCollectionReserved7Bit,
  collectionReserved8Bit        = kCollectionReserved8Bit,
  collectionReserved9Bit        = kCollectionReserved9Bit,
  collectionReserved10Bit       = kCollectionReserved10Bit,
  collectionReserved11Bit       = kCollectionReserved11Bit,
  collectionReserved12Bit       = kCollectionReserved12Bit,
  collectionReserved13Bit       = kCollectionReserved13Bit,
  collectionPersistenceBit      = kCollectionPersistenceBit,
  collectionLockBit             = kCollectionLockBit
};

enum {
  collectionUser0Mask           = kCollectionUser0Mask,
  collectionUser1Mask           = kCollectionUser1Mask,
  collectionUser2Mask           = kCollectionUser2Mask,
  collectionUser3Mask           = kCollectionUser3Mask,
  collectionUser4Mask           = kCollectionUser4Mask,
  collectionUser5Mask           = kCollectionUser5Mask,
  collectionUser6Mask           = kCollectionUser6Mask,
  collectionUser7Mask           = kCollectionUser7Mask,
  collectionUser8Mask           = kCollectionUser8Mask,
  collectionUser9Mask           = kCollectionUser9Mask,
  collectionUser10Mask          = kCollectionUser10Mask,
  collectionUser11Mask          = kCollectionUser11Mask,
  collectionUser12Mask          = kCollectionUser12Mask,
  collectionUser13Mask          = kCollectionUser13Mask,
  collectionUser14Mask          = kCollectionUser14Mask,
  collectionUser15Mask          = kCollectionUser15Mask,
  collectionReserved0Mask       = kCollectionReserved0Mask,
  collectionReserved1Mask       = kCollectionReserved1Mask,
  collectionReserved2Mask       = kCollectionReserved2Mask,
  collectionReserved3Mask       = kCollectionReserved3Mask,
  collectionReserved4Mask       = kCollectionReserved4Mask,
  collectionReserved5Mask       = kCollectionReserved5Mask,
  collectionReserved6Mask       = kCollectionReserved6Mask,
  collectionReserved7Mask       = kCollectionReserved7Mask,
  collectionReserved8Mask       = kCollectionReserved8Mask,
  collectionReserved9Mask       = kCollectionReserved9Mask,
  collectionReserved10Mask      = kCollectionReserved10Mask,
  collectionReserved11Mask      = kCollectionReserved11Mask,
  collectionReserved12Mask      = kCollectionReserved12Mask,
  collectionReserved13Mask      = kCollectionReserved13Mask,
  collectionPersistenceMask     = kCollectionPersistenceMask,
  collectionLockMask            = kCollectionLockMask
};

#endif  /* OLDROUTINENAMES */



#ifdef __cplusplus
}
#endif

#endif /* __COLLECTIONS__ */

