/*
     File:       CarbonCore/Aliases.h
 
     Contains:   Alias Manager Interfaces.
                 The contents of this header file are deprecated.
                 Use Foundation or CoreFoundation URL Bookmarks instead.
 
     Copyright:  © 1989-2011 by Apple Inc. All rights reserved.
*/
#ifndef __ALIASES__
#define __ALIASES__

#ifndef __MACTYPES__
#include <MacTypes.h>
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

#pragma pack(push, 2)

typedef UInt32                          FSAliasInfoBitmap;
enum {
  kFSAliasInfoNone              = 0x00000000, /* no valid info*/
  kFSAliasInfoVolumeCreateDate  = 0x00000001, /* volume creation date is valid*/
  kFSAliasInfoTargetCreateDate  = 0x00000002, /* target creation date is valid*/
  kFSAliasInfoFinderInfo        = 0x00000004, /* file type and creator are valid*/
  kFSAliasInfoIsDirectory       = 0x00000008, /* isDirectory boolean is valid*/
  kFSAliasInfoIDs               = 0x00000010, /* parentDirID and nodeID are valid*/
  kFSAliasInfoFSInfo            = 0x00000020, /* filesystemID and signature are valid*/
  kFSAliasInfoVolumeFlags       = 0x00000040 /* volumeIsBootVolume, volumeIsAutomounted, volumeIsEjectable and volumeHasPersistentFileIDs are valid*/
};

enum {
  rAliasType                    = 'alis' /* Aliases are stored as resources of this type */
};

enum {
                                        /* define alias resolution action rules mask */
  kARMMountVol                  = 0x00000001, /* mount the volume automatically */
  kARMNoUI                      = 0x00000002, /* no user interface allowed during resolution */
  kARMMultVols                  = 0x00000008, /* search on multiple volumes */
  kARMSearch                    = 0x00000100, /* search quickly */
  kARMSearchMore                = 0x00000200, /* search further */
  kARMSearchRelFirst            = 0x00000400, /* search target on a relative path first */
  kARMTryFileIDFirst            = 0x00000800 /* search by file id before path */
};

enum {
                                        /* define alias record information types */
  asiZoneName                   = -3,   /* get zone name */
  asiServerName                 = -2,   /* get server name */
  asiVolumeName                 = -1,   /* get volume name */
  asiAliasName                  = 0,    /* get aliased file/folder/volume name */
  asiParentName                 = 1     /* get parent folder name */
};

/* ResolveAliasFileWithMountFlags options */
enum {
  kResolveAliasFileNoUI         = 0x00000001, /* no user interaction during resolution */
  kResolveAliasTryFileIDFirst   = 0x00000002 /* search by file id before path */
};

#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4
#define __AL_USE_OPAQUE_RECORD__ 1
#else
#define __AL_USE_OPAQUE_RECORD__ 0
#endif
/* define the alias record that will be the blackbox for the caller */
#if __AL_USE_OPAQUE_RECORD__
struct AliasRecord {
  UInt8               hidden[6];
};
typedef struct AliasRecord              AliasRecord;
#else
struct AliasRecord {
  OSType              userType;               /* appl stored type like creator type */
  unsigned short      aliasSize;              /* alias record size in bytes, for appl usage */
};
typedef struct AliasRecord              AliasRecord;
#endif  /* __AL_USE_OPAQUE_RECORD__ */

typedef AliasRecord *                   AliasPtr;
typedef AliasPtr *                      AliasHandle;
/* info block to pass to FSCopyAliasInfo */
struct FSAliasInfo {
  UTCDateTime         volumeCreateDate;
  UTCDateTime         targetCreateDate;
  OSType              fileType;
  OSType              fileCreator;
  UInt32              parentDirID;
  UInt32              nodeID;
  UInt16              filesystemID;
  UInt16              signature;
  Boolean             volumeIsBootVolume;
  Boolean             volumeIsAutomounted;
  Boolean             volumeIsEjectable;
  Boolean             volumeHasPersistentFileIDs;
  Boolean             isDirectory;
};
typedef struct FSAliasInfo              FSAliasInfo;
typedef FSAliasInfo *                   FSAliasInfoPtr;
/* alias record information type */
typedef short                           AliasInfoType;
#if !__LP64__
typedef CALLBACK_API( Boolean , AliasFilterProcPtr )(CInfoPBPtr cpbPtr, Boolean *quitFlag, Ptr myDataPtr);
typedef STACK_UPP_TYPE(AliasFilterProcPtr)                      AliasFilterUPP;
/*
 *  NewAliasFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern AliasFilterUPP
NewAliasFilterUPP(AliasFilterProcPtr userRoutine)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  DisposeAliasFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeAliasFilterUPP(AliasFilterUPP userUPP)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeAliasFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeAliasFilterUPP(
  CInfoPBPtr      cpbPtr,
  Boolean *       quitFlag,
  Ptr             myDataPtr,
  AliasFilterUPP  userUPP)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline AliasFilterUPP                                       NewAliasFilterUPP(AliasFilterProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeAliasFilterUPP(AliasFilterUPP) { }
    inline Boolean                                              InvokeAliasFilterUPP(CInfoPBPtr cpbPtr, Boolean * quitFlag, Ptr myDataPtr, AliasFilterUPP userUPP) { return (*userUPP)(cpbPtr, quitFlag, myDataPtr); }
  #else
    #define NewAliasFilterUPP(userRoutine)                      ((AliasFilterUPP)userRoutine)
    #define DisposeAliasFilterUPP(userUPP)
    #define InvokeAliasFilterUPP(cpbPtr, quitFlag, myDataPtr, userUPP) (*userUPP)(cpbPtr, quitFlag, myDataPtr)
  #endif
#endif

#endif  /* !__LP64__ */

typedef CALLBACK_API_C( Boolean , FSAliasFilterProcPtr )(const FSRef *ref, Boolean *quitFlag, Ptr myDataPtr);
/*
 *  FSNewAlias()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSNewAlias(
  const FSRef *  fromFile,       /* can be NULL */
  const FSRef *  target,
  AliasHandle *  inAlias)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSNewAliasMinimal()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSNewAliasMinimal(
  const FSRef *  target,
  AliasHandle *  inAlias)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSIsAliasFile()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSIsAliasFile(
  const FSRef *  fileRef,
  Boolean *      aliasFileFlag,
  Boolean *      folderFlag)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSResolveAliasWithMountFlags()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSResolveAliasWithMountFlags(
  const FSRef *   fromFile,         /* can be NULL */
  AliasHandle     inAlias,
  FSRef *         target,
  Boolean *       wasChanged,
  unsigned long   mountFlags)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSResolveAlias()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSResolveAlias(
  const FSRef *  fromFile,         /* can be NULL */
  AliasHandle    alias,
  FSRef *        target,
  Boolean *      wasChanged)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSResolveAliasFileWithMountFlags()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSResolveAliasFileWithMountFlags(
  FSRef *         theRef,
  Boolean         resolveAliasChains,
  Boolean *       targetIsFolder,
  Boolean *       wasAliased,
  unsigned long   mountFlags)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSResolveAliasFile()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSResolveAliasFile(
  FSRef *    theRef,
  Boolean    resolveAliasChains,
  Boolean *  targetIsFolder,
  Boolean *  wasAliased)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSFollowFinderAlias()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSFollowFinderAlias(
  FSRef *       fromFile,         /* can be NULL */
  AliasHandle   alias,
  Boolean       logon,
  FSRef *       target,
  Boolean *     wasChanged)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSUpdateAlias()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
FSUpdateAlias(
  const FSRef *  fromFile,         /* can be NULL */
  const FSRef *  target,
  AliasHandle    alias,
  Boolean *      wasChanged)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSNewAliasUnicode()
 *  
 *  Summary:
 *    Creates an alias given a ref to the target's parent directory and
 *    the target's unicode name.  If the target does not exist fnfErr
 *    will be returned but the alias will still be created.  This
 *    allows the creation of aliases to targets that do not exist.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    fromFile:
 *      The starting point for a relative search.
 *    
 *    targetParentRef:
 *      An FSRef to the parent directory of the target.
 *    
 *    targetNameLength:
 *      Number of Unicode characters in the target's name.
 *    
 *    targetName:
 *      A pointer to the Unicode name.
 *    
 *    inAlias:
 *      A Handle to the newly created alias record.
 *    
 *    isDirectory:
 *      On input, if target does not exist, a flag to indicate whether
 *      or not the target is a directory.  On output, if the target did
 *      exist, a flag indicating if the target is a directory.  Pass
 *      NULL in the non-existant case if unsure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSNewAliasUnicode(
  const FSRef *    fromFile,               /* can be NULL */
  const FSRef *    targetParentRef,
  UniCharCount     targetNameLength,
  const UniChar *  targetName,
  AliasHandle *    inAlias,
  Boolean *        isDirectory)            /* can be NULL */  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSNewAliasMinimalUnicode()
 *  
 *  Summary:
 *    Creates a minimal alias given a ref to the target's parent
 *    directory and the target's unicode name.  If the target does not
 *    exist fnfErr will be returned but the alias will still be created.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    targetParentRef:
 *      An FSRef to the parent directory of the target.
 *    
 *    targetNameLength:
 *      Number of Unicode characters in the target's name.
 *    
 *    targetName:
 *      A pointer to the Unicode name.
 *    
 *    inAlias:
 *      A Handle to the newly created alias record.
 *    
 *    isDirectory:
 *      On input, if target does not exist, a flag to indicate whether
 *      or not the target is a directory.  On output, if the target did
 *      exist, a flag indicating if the target is a directory.  Pass
 *      NULL in the non-existant case if unsure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSNewAliasMinimalUnicode(
  const FSRef *    targetParentRef,
  UniCharCount     targetNameLength,
  const UniChar *  targetName,
  AliasHandle *    inAlias,
  Boolean *        isDirectory)            /* can be NULL */  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSNewAliasFromPath()
 *  
 *  Summary:
 *    Creates an alias given a POSIX style utf-8 path to the target. 
 *    If the target file does not exist but the path up to the leaf
 *    does then fnfErr will be returned but the alias will still be
 *    created.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    fromFilePath:
 *      The starting point for a relative search.
 *    
 *    targetPath:
 *      POSIX style UTF-8 path to target.
 *    
 *    flags:
 *      Options for future use.  Pass in 0.
 *    
 *    inAlias:
 *      A Handle to the newly created alias record.
 *    
 *    isDirectory:
 *      On input, if target does not exist, a flag to indicate whether
 *      or not the target is a directory.  On output, if the target did
 *      exist, a flag indicating if the target is a directory.  Pass
 *      NULL in the non-existant case if unsure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FSNewAliasFromPath(
  const char *   fromFilePath,       /* can be NULL */
  const char *   targetPath,
  OptionBits     flags,
  AliasHandle *  inAlias,
  Boolean *      isDirectory)        /* can be NULL */        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMatchAliasBulk()
 *  
 *  Summary:
 *    Given an alias handle and fromFile, match the alias and return
 *    FSRefs to the aliased file(s) and needsUpdate flag
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FSMatchAliasBulk(
  const FSRef *          fromFile,          /* can be NULL */
  unsigned long          rulesMask,
  AliasHandle            inAlias,
  short *                aliasCount,
  FSRef *                aliasList,
  Boolean *              needsUpdate,
  FSAliasFilterProcPtr   aliasFilter,       /* can be NULL */
  void *                 yourDataPtr)       /* can be NULL */ __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSCopyAliasInfo()
 *  
 *  Discussion:
 *    This routine will return the requested information from the
 *    passed in aliasHandle.  The information is gathered only from the
 *    alias record so it may not match what is on disk (no disk i/o is
 *    performed).  The whichInfo paramter is an output parameter that
 *    signifies which fields in the info record contain valid data.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inAlias:
 *      A handle to the alias record to get the information from.
 *    
 *    targetName:
 *      The name of the target item.
 *    
 *    volumeName:
 *      The name of the volume the target resides on.
 *    
 *    pathString:
 *      POSIX path to target.
 *    
 *    whichInfo:
 *      An indication of which fields in the info block contain valid
 *      data.
 *    
 *    info:
 *      Returned information about the alias.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FSCopyAliasInfo(
  AliasHandle          inAlias,
  HFSUniStr255 *       targetName,       /* can be NULL */
  HFSUniStr255 *       volumeName,       /* can be NULL */
  CFStringRef *        pathString,       /* can be NULL */
  FSAliasInfoBitmap *  whichInfo,        /* can be NULL */
  FSAliasInfo *        info)             /* can be NULL */    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetAliasSize()
 *  
 *  Discussion:
 *    This routine will return the size of the alias record referenced
 *    by the AliasHandle alias.  This will be smaller than the size
 *    returned by GetHandleSize if any custom data has been added (IM
 *    Files 4-13).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alias:
 *      A handle to the alias record to get the information from.
 *  
 *  Result:
 *    The size of the private section of the alias record.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern Size 
GetAliasSize(AliasHandle alias)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetAliasUserType()
 *  
 *  Discussion:
 *    This routine will return the usertype associated with the alias
 *    record referenced by the AliasHandle alias.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alias:
 *      A handle to the alias record to get the userType from.
 *  
 *  Result:
 *    The userType associated with the alias as an OSType
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSType 
GetAliasUserType(AliasHandle alias)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetAliasUserType()
 *  
 *  Discussion:
 *    This routine will set the userType associated with an alias
 *    record.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alias:
 *      A handle to the alias record to set the userType for.
 *    
 *    userType:
 *      The OSType to set the userType to.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
SetAliasUserType(
  AliasHandle   alias,
  OSType        userType)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetAliasSizeFromPtr()
 *  
 *  Discussion:
 *    This routine will return the size of the alias record referenced
 *    by a pointer to the AliasRecord.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alias:
 *      A pointer to the alias record to get the information from.
 *  
 *  Result:
 *    The size of the private section of the alias record.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Size 
GetAliasSizeFromPtr(const AliasRecord * alias)                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetAliasUserTypeFromPtr()
 *  
 *  Discussion:
 *    This routine will return the usertype associated withthe alias
 *    record pointed to by alias.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alias:
 *      A pointer to the alias record to get the userType from.
 *  
 *  Result:
 *    The userType associated with the alias as an OSType
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSType 
GetAliasUserTypeFromPtr(const AliasRecord * alias)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetAliasUserTypeWithPtr()
 *  
 *  Discussion:
 *    This routine will set the userType associated with an alias
 *    record.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alias:
 *      A pointer to the alias record to set the userType for.
 *    
 *    userType:
 *      The OSType to set the userType to.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
SetAliasUserTypeWithPtr(
  AliasPtr   alias,
  OSType     userType)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* Functions beyond this point are deprecated*/

#if !__LP64__
/*
 *  FSMatchAlias()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSMatchAliasBulk instead
 *  
 *  Summary:
 *    Given an alias handle and fromFile, match the alias and return
 *    FSRefs to the aliased file(s) and needsUpdate flag
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSMatchAlias(
  const FSRef *    fromFile,          /* can be NULL */
  unsigned long    rulesMask,
  AliasHandle      inAlias,
  short *          aliasCount,
  FSRef *          aliasList,
  Boolean *        needsUpdate,
  AliasFilterUPP   aliasFilter,
  void *           yourDataPtr)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMatchAliasNoUI()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSMatchAliasBulk with the kARMNoUI flag instead
 *  
 *  Summary:
 *    variation on FSMatchAlias that does not prompt user with a dialog
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
FSMatchAliasNoUI(
  const FSRef *    fromFile,          /* can be NULL */
  unsigned long    rulesMask,
  AliasHandle      inAlias,
  short *          aliasCount,
  FSRef *          aliasList,
  Boolean *        needsUpdate,
  AliasFilterUPP   aliasFilter,
  void *           yourDataPtr)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  NewAlias()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSNewAlias
 *  
 *  Summary:
 *    create a new alias between fromFile and target, returns alias
 *    record handle
 *  
 *  Discussion:
 *    Create an alias betwen fromFile and target, and return it in an
 *    AliasHandle. This function is deprecated in Mac OS X 10.4;
 *    instead, you should use FSNewAliasUnicode() because NewAlias()
 *    has problems creating aliases to certain files, including those
 *    which are impossible to represent in an FSSpec.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
NewAlias(
  const FSSpec *  fromFile,       /* can be NULL */
  const FSSpec *  target,
  AliasHandle *   alias)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  NewAliasMinimal()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSNewAliasMinimalUnicode
 *  
 *  Summary:
 *    create a minimal new alias for a target and return alias record
 *    handle
 *  
 *  Discussion:
 *    Create a minimal alias for a target, and return it in an
 *    AliasHandle. This function is deprecated in Mac OS X 10.4;
 *    instead, you should use FSNewAliasMinimalUnicode() because
 *    NewAliasMinimalAlias() has problems creating aliases to certain
 *    files, including those which are impossible to represent in an
 *    FSSpec.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
NewAliasMinimal(
  const FSSpec *  target,
  AliasHandle *   alias)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  NewAliasMinimalFromFullPath()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSNewAliasMinimalUnicode
 *  
 *  Summary:
 *    create a minimal new alias from a target fullpath (optional zone
 *    and server name) and return alias record handle
 *  
 *  Discussion:
 *    Create a minimal alias for a target fullpath, and return it in an
 *    AliasHandle. This function is deprecated in Mac OS X 10.4;
 *    instead, you should use FSNewAliasMinimalUnicode() because
 *    NewAliasMinimalFromFullPath() has problems creating aliases to
 *    certain files, including those which are impossible to represent
 *    in an FSSpec.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
NewAliasMinimalFromFullPath(
  short             fullPathLength,
  const void *      fullPath,
  ConstStr32Param   zoneName,
  ConstStr31Param   serverName,
  AliasHandle *     alias)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  ResolveAlias()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSResolveAlias() or FSResolveAliasWithMountFlags() instead.
 *  
 *  Summary:
 *    given an alias handle and fromFile, resolve the alias, update the
 *    alias record and return aliased filename and wasChanged flag.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
ResolveAlias(
  const FSSpec *  fromFile,         /* can be NULL */
  AliasHandle     alias,
  FSSpec *        target,
  Boolean *       wasChanged)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetAliasInfo()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSCopyAliasInfo instead.
 *  
 *  Summary:
 *    This call does not work on all aliases. Given an alias handle and
 *    an index specifying requested alias information type, return the
 *    information from alias record as a string. An empty string is
 *    returned when the index is greater than the number of levels
 *    between the target and root.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetAliasInfo(
  AliasHandle     alias,
  AliasInfoType   itemIndex,
  Str63           theString)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);



/*
 *  IsAliasFile()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSIsAliasFile() instead.
 *  
 *  Summary:
 *    Return true if the file pointed to by fileFSSpec is an alias file.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr 
IsAliasFile(
  const FSSpec *  fileFSSpec,
  Boolean *       aliasFileFlag,
  Boolean *       folderFlag)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  ResolveAliasWithMountFlags()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSResolveAliasWithMountFlags() instead.
 *  
 *  Summary:
 *    Given an AliasHandle, return target file spec. It resolves the
 *    entire alias chain or one step of the chain.  It returns info
 *    about whether the target is a folder or file; and whether the
 *    input file spec was an alias or not.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr 
ResolveAliasWithMountFlags(
  const FSSpec *  fromFile,         /* can be NULL */
  AliasHandle     alias,
  FSSpec *        target,
  Boolean *       wasChanged,
  unsigned long   mountFlags)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  ResolveAliasFile()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSResolveAliasFile
 *  
 *  Summary:
 *    Given a file spec, return target file spec if input file spec is
 *    an alias. It resolves the entire alias chain or one step of the
 *    chain.  It returns info about whether the target is a folder or
 *    file; and whether the input file spec was an alias or not.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
ResolveAliasFile(
  FSSpec *   theSpec,
  Boolean    resolveAliasChains,
  Boolean *  targetIsFolder,
  Boolean *  wasAliased)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* Deprecated: Use FSResolveAliasFileWithMountFlags instead*/
/*
 *  ResolveAliasFileWithMountFlags()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr 
ResolveAliasFileWithMountFlags(
  FSSpec *        theSpec,
  Boolean         resolveAliasChains,
  Boolean *       targetIsFolder,
  Boolean *       wasAliased,
  unsigned long   mountFlags)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* Deprecated:  Use FSFollowFinderAlias instead*/
/*
 *  FollowFinderAlias()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr 
FollowFinderAlias(
  const FSSpec *  fromFile,         /* can be NULL */
  AliasHandle     alias,
  Boolean         logon,
  FSSpec *        target,
  Boolean *       wasChanged)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* 
   Low Level Routines 
*/
/*
 *  UpdateAlias()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    UseFSUpdateAlias
 *  
 *  Summary:
 *    given a fromFile-target pair and an alias handle, update the
 *    alias record pointed to by alias handle to represent target as
 *    the new alias.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
UpdateAlias(
  const FSSpec *  fromFile,         /* can be NULL */
  const FSSpec *  target,
  AliasHandle     alias,
  Boolean *       wasChanged)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/*
 *  MatchAlias()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSMatchAliasBulk instead
 *  
 *  Summary:
 *    Given an alias handle and fromFile, match the alias and return
 *    FSSpecs to the aliased file(s) and needsUpdate flag
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
MatchAlias(
  const FSSpec *   fromFile,          /* can be NULL */
  unsigned long    rulesMask,
  AliasHandle      alias,
  short *          aliasCount,
  FSSpecArrayPtr   aliasList,
  Boolean *        needsUpdate,
  AliasFilterUPP   aliasFilter,
  void *           yourDataPtr)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);




/*
 *  ResolveAliasFileWithMountFlagsNoUI()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSResolveAliasFileWithMountFlags passing in the
 *    kResolveAliasFileNoUI flag
 *  
 *  Summary:
 *    variation on ResolveAliasFile that does not prompt user with a
 *    dialog
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
ResolveAliasFileWithMountFlagsNoUI(
  FSSpec *        theSpec,
  Boolean         resolveAliasChains,
  Boolean *       targetIsFolder,
  Boolean *       wasAliased,
  unsigned long   mountFlags)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  MatchAliasNoUI()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use FSMatchAliasBulk with the kARMNoUI flag instead
 *  
 *  Summary:
 *    variation on MatchAlias that does not prompt user with a dialog
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
MatchAliasNoUI(
  const FSSpec *   fromFile,          /* can be NULL */
  unsigned long    rulesMask,
  AliasHandle      alias,
  short *          aliasCount,
  FSSpecArrayPtr   aliasList,
  Boolean *        needsUpdate,
  AliasFilterUPP   aliasFilter,
  void *           yourDataPtr)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);



#endif  /* !__LP64__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __ALIASES__ */

