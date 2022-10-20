/*
     File:       CarbonCore/Resources.h
 
     Contains:   Resource Manager Interfaces.
                 The contents of this header file are deprecated.
                 Use Foundation or CoreFoundation bundles instead. 
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/
#ifndef __RESOURCES__
#define __RESOURCES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __FILES__
#include <CarbonCore/Files.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef SInt16                          ResID;
typedef SInt16                          ResAttributes;
typedef SInt16                          ResFileAttributes;
typedef SInt16                          ResourceCount;
typedef SInt16                          ResourceIndex;
typedef FSIORefNum                      ResFileRefNum;
/* Resource Attribute Bits */
enum {
  resSysRefBit                  = 7,    /*reference to system/local reference*/
  resSysHeapBit                 = 6,    /*In system/in application heap*/
  resPurgeableBit               = 5,    /*Purgeable/not purgeable*/
  resLockedBit                  = 4,    /*Locked/not locked*/
  resProtectedBit               = 3,    /*Protected/not protected*/
  resPreloadBit                 = 2,    /*Read in at OpenResource?*/
  resChangedBit                 = 1     /*Existing resource changed since last update*/
};

/* Resource Attribute Masks*/
enum {
  resSysHeap                    = 64,   /*System or application heap?*/
  resPurgeable                  = 32,   /*Purgeable resource?*/
  resLocked                     = 16,   /*Load it in locked?*/
  resProtected                  = 8,    /*Protected?*/
  resPreload                    = 4,    /*Load in on OpenResFile?*/
  resChanged                    = 2     /*Resource changed?*/
};

/* Resource Fork Attribute Bits*/
enum {
  mapReadOnlyBit                = 7,    /*is this file read-only?*/
  mapCompactBit                 = 6,    /*Is a compact necessary?*/
  mapChangedBit                 = 5     /*Is it necessary to write map?*/
};

/* Resource Fork Attribute Masks*/
enum {
  mapReadOnly                   = 128,  /*Resource file read-only*/
  mapCompact                    = 64,   /*Compact resource file*/
  mapChanged                    = 32    /*Write map out at update*/
};

/* Resource File Ref Num constants*/
enum {
  kResFileNotOpened             = -1,   /*ref num return as error when opening a resource file*/
  kSystemResFile                = 0     /*this is the default ref num to the system file*/
};


typedef CALLBACK_API( void , ResErrProcPtr )(OSErr thErr);
typedef STACK_UPP_TYPE(ResErrProcPtr)                           ResErrUPP;
/*
 *  NewResErrUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ResErrUPP
NewResErrUPP(ResErrProcPtr userRoutine)                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeResErrUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeResErrUPP(ResErrUPP userUPP)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeResErrUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeResErrUPP(
  OSErr      thErr,
  ResErrUPP  userUPP)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline ResErrUPP                                            NewResErrUPP(ResErrProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeResErrUPP(ResErrUPP) { }
    inline void                                                 InvokeResErrUPP(OSErr thErr, ResErrUPP userUPP) { (*userUPP)(thErr); }
  #else
    #define NewResErrUPP(userRoutine)                           ((ResErrUPP)userRoutine)
    #define DisposeResErrUPP(userUPP)
    #define InvokeResErrUPP(thErr, userUPP)                     (*userUPP)(thErr)
  #endif
#endif

/* QuickTime 3.0*/
typedef CALLBACK_API( OSErr , ResourceEndianFilterPtr )(Handle theResource, Boolean currentlyNativeEndian);
/*
 *  CloseResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
CloseResFile(ResFileRefNum refNum)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  ResError()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
ResError(void)                                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CurResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResFileRefNum 
CurResFile(void)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  HomeResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResFileRefNum 
HomeResFile(Handle theResource)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  UseResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
UseResFile(ResFileRefNum refNum)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CountTypes()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResourceCount 
CountTypes(void)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Count1Types()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResourceCount 
Count1Types(void)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetIndType()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
GetIndType(
  ResType *       theType,
  ResourceIndex   itemIndex)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Get1IndType()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
Get1IndType(
  ResType *       theType,
  ResourceIndex   itemIndex)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetResLoad()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetResLoad(Boolean load)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CountResources()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResourceCount 
CountResources(ResType theType)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Count1Resources()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResourceCount 
Count1Resources(ResType theType)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetIndResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
GetIndResource(
  ResType         theType,
  ResourceIndex   itemIndex)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Get1IndResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
Get1IndResource(
  ResType         theType,
  ResourceIndex   itemIndex)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
GetResource(
  ResType   theType,
  ResID     theID)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Get1Resource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
Get1Resource(
  ResType   theType,
  ResID     theID)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetNamedResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
GetNamedResource(
  ResType            theType,
  ConstStr255Param   name)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Get1NamedResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
Get1NamedResource(
  ResType            theType,
  ConstStr255Param   name)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  [Mac]LoadResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
#if TARGET_OS_MAC
    #define MacLoadResource LoadResource
#endif
extern void 
MacLoadResource(Handle theResource)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  ReleaseResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ReleaseResource(Handle theResource)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DetachResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DetachResource(Handle theResource)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  UniqueID()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResID 
UniqueID(ResType theType)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Unique1ID()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResID 
Unique1ID(ResType theType)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetResAttrs()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResAttributes 
GetResAttrs(Handle theResource)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetResInfo()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
GetResInfo(
  Handle     theResource,
  ResID *    theID,
  ResType *  theType,
  Str255     name)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetResInfo()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetResInfo(
  Handle             theResource,
  ResID              theID,
  ConstStr255Param   name)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AddResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
AddResource(
  Handle             theData,
  ResType            theType,
  ResID              theID,
  ConstStr255Param   name)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetResourceSizeOnDisk()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
GetResourceSizeOnDisk(Handle theResource)                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetMaxResourceSize()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
GetMaxResourceSize(Handle theResource)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetResAttrs()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetResAttrs(
  Handle          theResource,
  ResAttributes   attrs)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  ChangedResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ChangedResource(Handle theResource)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  RemoveResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
RemoveResource(Handle theResource)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  UpdateResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
UpdateResFile(ResFileRefNum refNum)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WriteResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
WriteResource(Handle theResource)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetResPurge()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetResPurge(Boolean install)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetResFileAttrs()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResFileAttributes 
GetResFileAttrs(ResFileRefNum refNum)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetResFileAttrs()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetResFileAttrs(
  ResFileRefNum       refNum,
  ResFileAttributes   attrs)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  ReadPartialResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ReadPartialResource(
  Handle   theResource,
  long     offset,
  void *   buffer,
  long     count)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WritePartialResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
WritePartialResource(
  Handle        theResource,
  long          offset,
  const void *  buffer,
  long          count)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetResourceSize()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetResourceSize(
  Handle   theResource,
  long     newSize)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetNextFOND()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
GetNextFOND(Handle fondHandle)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* QuickTime 3.0*/
/*
 *  RegisterResourceEndianFilter()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */



/*
  _________________________________________________________________________________________________________
      
   ¥ RESOURCE CHAIN LOCATION - for use with the Resource Manager chain manipulation routines under Carbon.
  _________________________________________________________________________________________________________
*/

typedef SInt16                          RsrcChainLocation;
enum {
  kRsrcChainBelowSystemMap      = 0,    /* Below the system's resource map*/
  kRsrcChainBelowApplicationMap = 1,    /* Below the application's resource map*/
  kRsrcChainAboveApplicationMap = 2,    /* Above the application's resource map*/
  kRsrcChainAboveAllMaps        = 4     /* Above all resource maps*/
};

/*
   If the file is already in the resource chain, it is removed and re-inserted at the specified location
   If the file has been detached, it is added to the resource chain at the specified location
   Returns resFNotFound if it's not currently open.
*/
/*
 *  InsertResourceFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
InsertResourceFile(
  ResFileRefNum       refNum,
  RsrcChainLocation   where)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
   If the file is not currently in the resource chain, this returns resNotFound
   Otherwise, the resource file is removed from the resource chain.
*/
/*
 *  DetachResourceFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
DetachResourceFile(ResFileRefNum refNum)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
   GetTopResourceFile returns the refNum of the top most resource map in the current resource chain. If
   the resource chain is empty it returns resFNotFound.
*/
/*
 *  GetTopResourceFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
GetTopResourceFile(ResFileRefNum * refNum)                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
   GetNextResourceFile can be used to iterate over resource files in the resource chain. By passing a
   valid refNum in curRefNum it will return in nextRefNum the refNum of the next file in 
   the chain. If curRefNum is not found in the resource chain, GetNextResourceFile returns resFNotFound.
   When the end of the chain is reached GetNextResourceFile will return noErr and nextRefNum will be NIL.
*/
/*
 *  GetNextResourceFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
GetNextResourceFile(
  ResFileRefNum    curRefNum,
  ResFileRefNum *  nextRefNum)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSOpenResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern ResFileRefNum 
FSOpenResFile(
  const FSRef *  ref,
  SInt8          permission)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSCreateResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern void 
FSCreateResFile(
  const FSRef *          parentRef,
  UniCharCount           nameLength,
  const UniChar *        name,
  FSCatalogInfoBitmap    whichInfo,
  const FSCatalogInfo *  catalogInfo,       /* can be NULL */
  FSRef *                newRef,            /* can be NULL */
  FSSpecPtr              newSpec)           /* can be NULL */ __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
   Returns true if the resource file is already open and known by the Resource Manager (i.e., it is
   either in the current resource chain or it's a detached resource file.)  If it's in the resource 
   chain, the inChain Boolean is set to true on exit and true is returned.  If it's an open file, but
   the file is currently detached, inChain is set to false and true is returned.  If the file is open,
   the refNum to the file is returned.
*/
/*
 *  FSResourceFileAlreadyOpen()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern Boolean 
FSResourceFileAlreadyOpen(
  const FSRef *    resourceFileRef,
  Boolean *        inChain,
  ResFileRefNum *  refNum)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
   FSOpenOrphanResFile should be used to open a resource file that is persistent across all contexts,
   because using OpenResFile normally loads a map and all preloaded resources into the application
   context.  FSOpenOrphanResFile loads everything into the system context and detaches the file 
   from the context in which it was opened.  If the file is already in the resource chain and a new
   instance is not opened, FSOpenOrphanResFile will return a paramErr.
   Use with care, as can and will fail if the map is very large or a lot of preload
   resources exist.
*/
/*
 *  FSOpenOrphanResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSOpenOrphanResFile(
  const FSRef *    ref,
  SignedByte       permission,
  ResFileRefNum *  refNum)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSCreateResourceFile()
 *  
 *  Summary:
 *    Creates a new resource file.
 *  
 *  Discussion:
 *    This function creates a new file and initializes the specified
 *    named fork as an empty resource fork.  This function allows for
 *    the creation of data fork only files which can be used for
 *    storing resources.  Passing in a null name defaults to using the
 *    data fork.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    parentRef:
 *      The directory where the file is to be created
 *    
 *    nameLength:
 *      Number of Unicode characters in the file's name
 *    
 *    name:
 *      A pointer to the Unicode name
 *    
 *    whichInfo:
 *      Which catalog info fields to set
 *    
 *    catalogInfo:
 *      The values for catalog info fields to set; may be NULL
 *    
 *    forkNameLength:
 *      The length of the fork name (in Unicode characters)
 *    
 *    forkName:
 *      The name of the fork to initialize (in Unicode); may be NULL
 *    
 *    newRef:
 *      A pointer to the FSRef for the new file; may be NULL
 *    
 *    newSpec:
 *      A pointer to the FSSpec for the new directory; may be NULL. 
 *      Ignored on 64 bit.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSCreateResourceFile(
  const FSRef *          parentRef,
  UniCharCount           nameLength,
  const UniChar *        name,
  FSCatalogInfoBitmap    whichInfo,
  const FSCatalogInfo *  catalogInfo,          /* can be NULL */
  UniCharCount           forkNameLength,
  const UniChar *        forkName,             /* can be NULL */
  FSRef *                newRef,               /* can be NULL */
  FSSpecPtr              newSpec)              /* can be NULL */ __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSCreateResourceFork()
 *  
 *  Summary:
 *    Creates the named forked and initializes it as an empty resource
 *    fork.
 *  
 *  Discussion:
 *    This function allows a resource fork to be added to an existing
 *    file.  Passing in a null forkname will result in the data fork
 *    being used.  If the named fork already exists this function does
 *    nothing and returns errFSForkExists.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      The file to add the fork to
 *    
 *    forkNameLength:
 *      The length of the fork name (in Unicode characters)
 *    
 *    forkName:
 *      The name of the fork to open (in Unicode); may be NULL
 *    
 *    flags:
 *      Pass in zero
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSCreateResourceFork(
  const FSRef *    ref,
  UniCharCount     forkNameLength,
  const UniChar *  forkName,             /* can be NULL */
  UInt32           flags)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSOpenResourceFile()
 *  
 *  Summary:
 *    Opens the specified named fork as a resource fork.
 *  
 *  Discussion:
 *    This function allows any named fork of a file to be used for
 *    storing resources.  Passing in a null forkname will result in the
 *    data fork being used.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      The file containing the fork to open
 *    
 *    forkNameLength:
 *      The length of the fork name (in Unicode characters)
 *    
 *    forkName:
 *      The name of the fork to open (in Unicode); may be NULL
 *    
 *    permissions:
 *      The access (read and/or write) you want
 *    
 *    refNum:
 *      On exit the reference number for accessing the open fork
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSOpenResourceFile(
  const FSRef *    ref,
  UniCharCount     forkNameLength,
  const UniChar *  forkName,             /* can be NULL */
  SInt8            permissions,
  ResFileRefNum *  refNum)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/* Deprecated Functions -------------------------------------------------------*/

/* use FSOpenResourceFile instead*/
#if !__LP64__
/*
 *  OpenRFPerm()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResFileRefNum 
OpenRFPerm(
  ConstStr255Param   fileName,
  FSVolumeRefNum     vRefNum,
  SInt8              permission)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* use FSOpenResourceFile instead*/
/*
 *  HOpenResFile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResFileRefNum 
HOpenResFile(
  FSVolumeRefNum     vRefNum,
  long               dirID,
  ConstStr255Param   fileName,
  SInt8              permission)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* use FSCreateResourceFile instead*/
/*
 *  HCreateResFile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HCreateResFile(
  FSVolumeRefNum     vRefNum,
  long               dirID,
  ConstStr255Param   fileName)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* use FSOpenResourceFile instead*/
/*
 *  FSpOpenResFile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ResFileRefNum 
FSpOpenResFile(
  const FSSpec *  spec,
  SignedByte      permission)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* use FSCreateResourceFile instead*/
/*
 *  FSpCreateResFile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
FSpCreateResFile(
  const FSSpec *  spec,
  OSType          creator,
  OSType          fileType,
  ScriptCode      scriptTag)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* use FSResourceFileAlreadyOpen instead*/
/*
 *  FSpResourceFileAlreadyOpen()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern Boolean 
FSpResourceFileAlreadyOpen(
  const FSSpec *   resourceFile,
  Boolean *        inChain,
  ResFileRefNum *  refNum)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* use FSOpenOrphanResFile instead*/
/*
 *  FSpOpenOrphanResFile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSpOpenOrphanResFile(
  const FSSpec *   spec,
  SignedByte       permission,
  ResFileRefNum *  refNum)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
#if OLDROUTINENAMES
#define SizeResource(theResource) GetResourceSizeOnDisk(theResource)
#define MaxSizeRsrc(theResource) GetMaxResourceSize(theResource)
#define RmveResource(theResource) RemoveResource(theResource)
#endif  /* OLDROUTINENAMES */

#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __RESOURCES__ */

