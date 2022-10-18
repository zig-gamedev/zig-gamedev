/*
     File:       CarbonCore/Files.h
 
     Contains:   File Manager Interfaces.
                 The contents of this header file are deprecated.
                 Use Foundation, CoreFoundation, DiskArbitration, POSIX/BSD, etc. API instead.
 
     Copyright:  © 1985-2011 Apple, Inc. All rights reserved
*/
#ifndef __FILES__
#define __FILES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __OSUTILS__
#include <CarbonCore/OSUtils.h>
#endif

#ifndef __TEXTCOMMON__
#include <CarbonCore/TextCommon.h>
#endif

#ifndef __UTCUTILS__
#include <CarbonCore/UTCUtils.h>
#endif


/* Finder constants were moved to Finder.Å */
#ifndef __FINDER__
#include <CarbonCore/Finder.h>
#endif


#ifndef __CFURL__
#include <CoreFoundation/CFURL.h>
#endif

#ifndef __CFRUNLOOP__
#include <CoreFoundation/CFRunLoop.h>
#endif

#ifndef __CFUUID__
#include <CoreFoundation/CFUUID.h>
#endif


#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/acl.h>
#include <DiskArbitration/DADisk.h>
#include <hfs/hfs_unistr.h>

#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/*
// HFSUniStr255 is the Unicode equivalent of Str255. This struct definition is
// duplicated in the lower layer header <hfs/hfs_unistr>, so this copy of the
// struct definition has been commented out, but left here as documentation.

struct HFSUniStr255 {
  UInt16              length;                 // number of unicode characters
  UniChar             unicode[255];           // unicode characters
};
typedef struct HFSUniStr255             HFSUniStr255;
typedef const HFSUniStr255 *            ConstHFSUniStr255Param;
*/

#define kFSFileSecurityRemoveACL (acl_t) _FILESEC_REMOVE_ACL
/*
    File Permissions

    Do not mix and match the following two sets of File Manager permission
    constants (original model  and AFP model).
    See the Retired Technote FL 37 "You Want Permission To Do What?!!"
    <https://developer.apple.com/legacy/mac/library/#technotes/fl/fl_37.html#//apple_ref/doc/uid/DTS10002463>
    for a detailed discussion of the two separate models
    and how they are related.
*/
/*    Permissions for File Manager routines which follow the original model.
    These values can be passed in the permissions parameter to FSOpenFork,
    PBOpenForkSync, and PBOpenForkAsync.
 */
enum {
  fsCurPerm                     = 0x00,
  fsRdPerm                      = 0x01,
  fsWrPerm                      = 0x02,
  fsRdWrPerm                    = 0x03,
  fsRdWrShPerm                  = 0x04
};

/*    Permissions for deprecated File Manager routines which follow the AFP model
    that is, routines with "OpenDeny" in the name. (These values do not work with
    FSOpenFork, PBOpenForkSync, and PBOpenForkAsync).
    
    The most useful combinations of these are:
        fsRdAccessPerm -> one writer, multiple readers: the readers
        fsRdAccessPerm + fsWrAccessPerm + fsWrDenyPerm -> one writer, multiple readers: the writer
        fsWrDenyPerm + fsRdAccessPerm -> multiple readers, no writers
        fsRdAccessPerm + fsWrAccessPerm -> shared read/write access
        fsRdAccessPerm + fsWrAccessPerm + fsRdDenyPerm + fsWrDenyPerm -> exclusive access
*/
enum {
  fsRdAccessPerm                = 0x01,
  fsWrAccessPerm                = 0x02,
  fsRdDenyPerm                  = 0x10,
  fsWrDenyPerm                  = 0x20
};

enum {
  fsRtParID                     = 1,
  fsRtDirID                     = 2
};

enum {
  fsAtMark                      = 0,    /* positioning modes in ioPosMode */
  fsFromStart                   = 1,
  fsFromLEOF                    = 2,
  fsFromMark                    = 3
};

enum {
                                        /* positionMode (ioPosMode) flags */
  kFSAllowConcurrentAsyncIOBit  = 3,    /* allow concurrent execution of async calls */
  kFSAllowConcurrentAsyncIOMask = 0x0008,
  kFSPleaseCacheBit             = 4,    /* please cache this request */
  kFSPleaseCacheMask            = 0x0010,
  kFSNoCacheBit                 = 5,    /* please don't cache this request */
  kFSNoCacheMask                = 0x0020,
  kFSRdVerifyBit                = 6,    /* read verify mode */
  kFSRdVerifyMask               = 0x0040,
  kFSForceReadBit               = 6,
  kFSForceReadMask              = 0x0040,
  kFSNewLineBit                 = 7,    /* newline mode */
  kFSNewLineMask                = 0x0080,
  kFSNewLineCharMask            = 0xFF00 /* newline character */
};


enum {
                                        /* CatSearch Search bitmask Constants */
  fsSBPartialName               = 0x01,
  fsSBFullName                  = 0x02,
  fsSBFlAttrib                  = 0x04,
  fsSBFlFndrInfo                = 0x08,
  fsSBFlLgLen                   = 0x20,
  fsSBFlPyLen                   = 0x40,
  fsSBFlRLgLen                  = 0x80,
  fsSBFlRPyLen                  = 0x0100,
  fsSBFlCrDat                   = 0x0200,
  fsSBFlMdDat                   = 0x0400,
  fsSBFlBkDat                   = 0x0800,
  fsSBFlXFndrInfo               = 0x1000,
  fsSBFlParID                   = 0x2000,
  fsSBNegate                    = 0x4000,
  fsSBDrUsrWds                  = 0x08,
  fsSBDrNmFls                   = 0x10,
  fsSBDrCrDat                   = 0x0200,
  fsSBDrMdDat                   = 0x0400,
  fsSBDrBkDat                   = 0x0800,
  fsSBDrFndrInfo                = 0x1000,
  fsSBDrParID                   = 0x2000,
  fsSBNodeID                    = 0x8000,
  fsSBAttributeModDate          = 0x00010000,
  fsSBAccessDate                = 0x00020000,
  fsSBPermissions               = 0x00040000,
  fsSBSkipPackageContents       = 0x00080000,
  fsSBSkipHiddenItems           = 0x00100000,
  fsSBUserID                    = 0x00200000,
  fsSBGroupID                   = 0x00400000
};

enum {
                                        /* CatSearch Search bit value Constants */
  fsSBPartialNameBit            = 0,    /*ioFileName points to a substring*/
  fsSBFullNameBit               = 1,    /*ioFileName points to a match string*/
  fsSBFlAttribBit               = 2,    /*search includes file attributes*/
  fsSBFlFndrInfoBit             = 3,    /*search includes finder info*/
  fsSBFlLgLenBit                = 5,    /*search includes data logical length*/
  fsSBFlPyLenBit                = 6,    /*search includes data physical length*/
  fsSBFlRLgLenBit               = 7,    /*search includes resource logical length*/
  fsSBFlRPyLenBit               = 8,    /*search includes resource physical length*/
  fsSBFlCrDatBit                = 9,    /*search includes create date*/
  fsSBFlMdDatBit                = 10,   /*search includes modification date*/
  fsSBFlBkDatBit                = 11,   /*search includes backup date*/
  fsSBFlXFndrInfoBit            = 12,   /*search includes extended finder info*/
  fsSBFlParIDBit                = 13,   /*search includes file's parent ID*/
  fsSBNegateBit                 = 14,   /*return all non-matches*/
  fsSBDrUsrWdsBit               = 3,    /*search includes directory finder info*/
  fsSBDrNmFlsBit                = 4,    /*search includes directory valence*/
  fsSBDrCrDatBit                = 9,    /*directory-named version of fsSBFlCrDatBit*/
  fsSBDrMdDatBit                = 10,   /*directory-named version of fsSBFlMdDatBit*/
  fsSBDrBkDatBit                = 11,   /*directory-named version of fsSBFlBkDatBit*/
  fsSBDrFndrInfoBit             = 12,   /*directory-named version of fsSBFlXFndrInfoBit*/
  fsSBDrParIDBit                = 13,   /*directory-named version of fsSBFlParIDBit*/
  fsSBNodeIDBit                 = 15,   /* search by range of nodeID */
  fsSBAttributeModDateBit       = 16,   /* search by range of attributeModDate */
  fsSBAccessDateBit             = 17,   /* search by range of accessDate [CatalogSearch only] */
  fsSBPermissionsBit            = 18,   /* search by value/mask of permissions [CatalogSearch only] */
  fsSBSkipPackageContentsBit    = 19,   /*do not return items inside of packages*/
  fsSBSkipHiddenItemsBit        = 20,   /*do not return items with an invisible element in their path*/
  fsSBUserIDBit                 = 21,   /* search by userID in permissions field [CatalogSearch only] */
  fsSBGroupIDBit                = 22    /* search by groupID in permissions field [CatalogSearch only] */
};

enum {
                                        /* vMAttrib (GetVolParms) bit position constants */
  bLimitFCBs                    = 31,
  bLocalWList                   = 30,
  bNoMiniFndr                   = 29,
  bNoVNEdit                     = 28,
  bNoLclSync                    = 27,
  bTrshOffLine                  = 26,
  bNoSwitchTo                   = 25,
  bNoDeskItems                  = 20,
  bNoBootBlks                   = 19,
  bAccessCntl                   = 18,
  bNoSysDir                     = 17,
  bHasExtFSVol                  = 16,
  bHasOpenDeny                  = 15,
  bHasCopyFile                  = 14,
  bHasMoveRename                = 13,
  bHasDesktopMgr                = 12,
  bHasShortName                 = 11,
  bHasFolderLock                = 10,
  bHasPersonalAccessPrivileges  = 9,
  bHasUserGroupList             = 8,
  bHasCatSearch                 = 7,
  bHasFileIDs                   = 6,
  bHasBTreeMgr                  = 5,
  bHasBlankAccessPrivileges     = 4,
  bSupportsAsyncRequests        = 3,    /* asynchronous requests to this volume are handled correctly at any time*/
  bSupportsTrashVolumeCache     = 2
};

enum {
                                        /* vMAttrib (GetVolParms) bit position constants */
  bHasDirectIO                  = 1
};

enum {
                                        /* vMExtendedAttributes (GetVolParms) bit position constants */
  bIsEjectable                  = 0,    /* volume is in an ejectable disk drive */
  bSupportsHFSPlusAPIs          = 1,    /* volume supports HFS Plus APIs directly (not through compatibility layer) */
  bSupportsFSCatalogSearch      = 2,    /* volume supports FSCatalogSearch */
  bSupportsFSExchangeObjects    = 3,    /* volume supports FSExchangeObjects */
  bSupports2TBFiles             = 4,    /* volume supports supports 2 terabyte files */
  bSupportsLongNames            = 5,    /* volume supports file/directory/volume names longer than 31 characters */
  bSupportsMultiScriptNames     = 6,    /* volume supports file/directory/volume names with characters from multiple script systems */
  bSupportsNamedForks           = 7,    /* volume supports forks beyond the data and resource forks */
  bSupportsSubtreeIterators     = 8,    /* volume supports recursive iterators not at the volume root */
  bL2PCanMapFileBlocks          = 9,    /* volume supports Lg2Phys SPI correctly */
  bParentModDateChanges         = 10,   /* Changing a file or folder causes its parent's mod date to change */
  bAncestorModDateChanges       = 11,   /* Changing a file or folder causes all ancestor mod dates to change */
  bSupportsSymbolicLinks        = 13,   /* volume supports the creation and use of symbolic links (Mac OS X only) */
  bIsAutoMounted                = 14,   /* volume was mounted automatically (Mac OS X only) */
  bAllowCDiDataHandler          = 17,   /* allow QuickTime's CDi data handler to examine this volume */
  bSupportsExclusiveLocks       = 18,   /* volume supports exclusive opens for writing */
  bSupportsJournaling           = 19,   /* volume supports journal (journal may not be active) */
  bNoVolumeSizes                = 20,   /* volume is unable to report volume size or free space */
  bIsOnInternalBus              = 21,   /* device is on an internal bus - see note below */
  bIsCaseSensitive              = 22,   /* volume is case sensitive */
  bIsCasePreserving             = 23,   /* volume is case preserving */
  bDoNotDisplay                 = 24,   /* volume should not be displayed in UI */
  bIsRemovable                  = 25,   /* device is removable according to IOKit */
  bNoRootTimes                  = 26,   /* volume does not set reliable times for its root directory */
  bIsOnExternalBus              = 27,   /* device is on an external bus -- see note below */
  bSupportsExtendedFileSecurity = 28    /* volume supports FSFileSecurity objects */
};

/*    Note: A volume can return one of four states via the bIsInternal and bIsExternal bits.  A volume known
        to be on an internal bus will set bIsInternal and clear bIsExternal.  A volume known to
        be on an external bus will clear bIsInternal and set bIsExternal.  A volume on a bus that
        is indeterminate (could be either) will set both bits.  A volume not on a local bus will, such
        as a network volume, will leave both bits clear. */
enum {
                                        /* Large Volume Constants */
  kWidePosOffsetBit             = 8,
  kUseWidePositioning           = (1 << kWidePosOffsetBit),
  kMaximumBlocksIn4GB           = 0x007FFFFF
};

enum {
                                        /* Foreign Privilege Model Identifiers */
  fsUnixPriv                    = 1
};

enum {
                                        /* Authentication Constants */
  kNoUserAuthentication         = 1,
  kPassword                     = 2,
  kEncryptPassword              = 3,
  kTwoWayEncryptPassword        = 6
};


/* values of user IDs and group IDs */
enum {
  knoUser                       = 0,
  kadministratorUser            = 1
};

enum {
  knoGroup                      = 0
};


typedef SInt16                          FSVolumeRefNum;
#if __LP64__
typedef int                             FSIORefNum;
#else
typedef SInt16                          FSIORefNum;
#endif  /* __LP64__ */

enum {
  kFSInvalidVolumeRefNum        = 0
};

struct FSRef {
  UInt8               hidden[80];             /* private to File Manager; ¥¥ need symbolic constant */
};
typedef struct FSRef                    FSRef;
typedef FSRef *                         FSRefPtr;
typedef struct __FSFileSecurity*        FSFileSecurityRef;
/* Catalog position record */
struct CatPositionRec {
  SInt32              initialize;
  SInt16              priv[6];
};
typedef struct CatPositionRec           CatPositionRec;
#if __LP64__
struct FSSpec {
  UInt8               hidden[70];             /* FSSpecs are invalid for 64 bit, but defined in case they appear in shared structs*/
};
typedef struct FSSpec                   FSSpec;
#else
struct FSSpec {
  FSVolumeRefNum      vRefNum;
  SInt32              parID;
  StrFileName         name;                   /* a Str63 on MacOS*/
};
typedef struct FSSpec                   FSSpec;
#endif  /* __LP64__ */

typedef FSSpec *                        FSSpecPtr;
typedef FSSpecPtr *                     FSSpecHandle;
/* pointer to array of FSSpecs */
typedef FSSpecPtr                       FSSpecArrayPtr;
/* 
    The only difference between "const FSSpec*" and "ConstFSSpecPtr" is 
    that as a parameter, ConstFSSpecPtr is allowed to be NULL 
*/
typedef const FSSpec *                  ConstFSSpecPtr;
typedef union ParamBlockRec             ParamBlockRec;
#if __LP64__
typedef void *                          ParmBlkPtr;
#else
typedef ParamBlockRec *                 ParmBlkPtr;
#endif  /* __LP64__ */

typedef CALLBACK_API( void , IOCompletionProcPtr )(ParmBlkPtr paramBlock);
typedef STACK_UPP_TYPE(IOCompletionProcPtr)                     IOCompletionUPP;
#if !__LP64__
struct IOParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioRefNum;               /*refNum for I/O operation*/
  SInt8               ioVersNum;              /*version number*/
  SInt8               ioPermssn;              /*Open: permissions (byte)*/
  Ptr                 ioMisc;                 /*Rename: new name (GetEOF,SetEOF: logical end of file) (Open: optional ptr to buffer) (SetFileType: new type)*/
  Ptr                 ioBuffer;               /*data buffer Ptr*/
  SInt32              ioReqCount;             /*requested byte count; also = ioNewDirID*/
  SInt32              ioActCount;             /*actual byte count completed*/
  SInt16              ioPosMode;              /*initial file positioning*/
  SInt32              ioPosOffset;            /*file position offset*/
};
typedef struct IOParam                  IOParam;
typedef IOParam *                       IOParamPtr;
struct FileParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioFRefNum;              /*reference number*/
  SInt8               ioFVersNum;             /*version number*/
  SInt8               filler1;
  SInt16              ioFDirIndex;            /*GetFInfo directory index*/
  SInt8               ioFlAttrib;             /*GetFInfo: in-use bit=7, lock bit=0*/
  SInt8               ioFlVersNum;            /*file version number*/
  FInfo               ioFlFndrInfo;           /*user info*/
  UInt32              ioFlNum;                /*GetFInfo: file number; TF- ioDirID*/
  UInt16              ioFlStBlk;              /*start file block (0 if none)*/
  SInt32              ioFlLgLen;              /*logical length (EOF)*/
  SInt32              ioFlPyLen;              /*physical length*/
  UInt16              ioFlRStBlk;             /*start block rsrc fork*/
  SInt32              ioFlRLgLen;             /*file logical length rsrc fork*/
  SInt32              ioFlRPyLen;             /*file physical length rsrc fork*/
  UInt32              ioFlCrDat;              /*file creation date& time (32 bits in secs)*/
  UInt32              ioFlMdDat;              /*last modified date and time*/
};
typedef struct FileParam                FileParam;
typedef FileParam *                     FileParamPtr;
struct VolumeParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  UInt32              filler2;
  SInt16              ioVolIndex;             /*volume index number*/
  UInt32              ioVCrDate;              /*creation date and time*/
  UInt32              ioVLsBkUp;              /*last backup date and time*/
  UInt16              ioVAtrb;                /*volume attrib*/
  UInt16              ioVNmFls;               /*number of files in directory*/
  UInt16              ioVDirSt;               /*start block of file directory*/
  SInt16              ioVBlLn;                /*GetVolInfo: length of dir in blocks*/
  UInt16              ioVNmAlBlks;            /*for compatibilty ioVNmAlBlks * ioVAlBlkSiz <= 2 GB*/
  UInt32              ioVAlBlkSiz;            /*for compatibilty ioVAlBlkSiz is <= $0000FE00 (65,024)*/
  UInt32              ioVClpSiz;              /*GetVolInfo: bytes to allocate at a time*/
  UInt16              ioAlBlSt;               /*starting disk(512-byte) block in block map*/
  UInt32              ioVNxtFNum;             /*GetVolInfo: next free file number*/
  UInt16              ioVFrBlk;               /*GetVolInfo: # free alloc blks for this vol*/
};
typedef struct VolumeParam              VolumeParam;
typedef VolumeParam *                   VolumeParamPtr;
struct CntrlParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioCRefNum;              /*refNum for I/O operation*/
  SInt16              csCode;                 /*word for control status code*/
  SInt16              csParam[11];            /*operation-defined parameters*/
};
typedef struct CntrlParam               CntrlParam;
typedef CntrlParam *                    CntrlParamPtr;
struct SlotDevParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioSRefNum;
  SInt8               ioSVersNum;
  SInt8               ioSPermssn;
  Ptr                 ioSMix;
  SInt16              ioSFlags;
  SInt8               ioSlot;
  SInt8               ioID;
};
typedef struct SlotDevParam             SlotDevParam;
typedef SlotDevParam *                  SlotDevParamPtr;
struct MultiDevParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioMRefNum;
  SInt8               ioMVersNum;
  SInt8               ioMPermssn;
  Ptr                 ioMMix;
  SInt16              ioMFlags;
  Ptr                 ioSEBlkPtr;
};
typedef struct MultiDevParam            MultiDevParam;
typedef MultiDevParam *                 MultiDevParamPtr;
union ParamBlockRec {
  IOParam             ioParam;
  FileParam           fileParam;
  VolumeParam         volumeParam;
  CntrlParam          cntrlParam;
  SlotDevParam        slotDevParam;
  MultiDevParam       multiDevParam;
};

struct HFileInfo {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioFRefNum;
  SInt8               ioFVersNum;
  SInt8               filler1;
  SInt16              ioFDirIndex;
  SInt8               ioFlAttrib;
  SInt8               ioACUser;
  FInfo               ioFlFndrInfo;
  SInt32              ioDirID;
  UInt16              ioFlStBlk;
  SInt32              ioFlLgLen;
  SInt32              ioFlPyLen;
  UInt16              ioFlRStBlk;
  SInt32              ioFlRLgLen;
  SInt32              ioFlRPyLen;
  UInt32              ioFlCrDat;
  UInt32              ioFlMdDat;
  UInt32              ioFlBkDat;
  FXInfo              ioFlXFndrInfo;
  SInt32              ioFlParID;
  SInt32              ioFlClpSiz;
};
typedef struct HFileInfo                HFileInfo;
struct DirInfo {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioFRefNum;
  SInt8               ioFVersNum;
  SInt8               filler1;
  SInt16              ioFDirIndex;
  SInt8               ioFlAttrib;
  SInt8               ioACUser;
  DInfo               ioDrUsrWds;
  SInt32              ioDrDirID;
  UInt16              ioDrNmFls;
  SInt16              filler3[9];
  UInt32              ioDrCrDat;
  UInt32              ioDrMdDat;
  UInt32              ioDrBkDat;
  DXInfo              ioDrFndrInfo;
  SInt32              ioDrParID;
};
typedef struct DirInfo                  DirInfo;
union CInfoPBRec {
  HFileInfo           hFileInfo;
  DirInfo             dirInfo;
};
typedef union CInfoPBRec                CInfoPBRec;
typedef CInfoPBRec *                    CInfoPBPtr;
struct XCInfoPBRec {
  QElemPtr            qLink;
  SInt16              qType;
  SInt16              ioTrap;
  Ptr                 ioCmdAddr;
  ProcPtr             ioCompletion;           /* --> A pointer to a completion routine */
  volatile OSErr      ioResult;               /* --> The result code of the function */
  StringPtr           ioNamePtr;              /* --> Pointer to pathname to object */
  FSVolumeRefNum      ioVRefNum;              /* --> A volume specification */
  SInt32              filler1;
  StringPtr           ioShortNamePtr;         /* <-> A pointer to the short name string buffer - required! */
  SInt16              filler2;
  SInt16              ioPDType;               /* <-- The ProDOS file type */
  SInt32              ioPDAuxType;            /* <-- The ProDOS aux type */
  SInt32              filler3[2];
  SInt32              ioDirID;                /* --> A directory ID */
};
typedef struct XCInfoPBRec              XCInfoPBRec;
typedef XCInfoPBRec *                   XCInfoPBPtr;
struct DTPBRec {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioDTRefNum;             /* desktop refnum */
  SInt16              ioIndex;
  SInt32              ioTagInfo;
  Ptr                 ioDTBuffer;
  SInt32              ioDTReqCount;
  SInt32              ioDTActCount;
  SInt8               ioFiller1;
  UInt8               ioIconType;
  SInt16              ioFiller2;
  SInt32              ioDirID;
  OSType              ioFileCreator;
  OSType              ioFileType;
  SInt32              ioFiller3;
  SInt32              ioDTLgLen;
  SInt32              ioDTPyLen;
  SInt16              ioFiller4[14];
  SInt32              ioAPPLParID;
};
typedef struct DTPBRec                  DTPBRec;
typedef DTPBRec *                       DTPBPtr;

struct HIOParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioRefNum;
  SInt8               ioVersNum;
  SInt8               ioPermssn;
  Ptr                 ioMisc;
  Ptr                 ioBuffer;
  SInt32              ioReqCount;
  SInt32              ioActCount;
  SInt16              ioPosMode;
  SInt32              ioPosOffset;
};
typedef struct HIOParam                 HIOParam;
typedef HIOParam *                      HIOParamPtr;
struct HFileParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioFRefNum;
  SInt8               ioFVersNum;
  SInt8               filler1;
  SInt16              ioFDirIndex;
  SInt8               ioFlAttrib;
  SInt8               ioFlVersNum;
  FInfo               ioFlFndrInfo;
  SInt32              ioDirID;
  UInt16              ioFlStBlk;
  SInt32              ioFlLgLen;
  SInt32              ioFlPyLen;
  UInt16              ioFlRStBlk;
  SInt32              ioFlRLgLen;
  SInt32              ioFlRPyLen;
  UInt32              ioFlCrDat;
  UInt32              ioFlMdDat;
};
typedef struct HFileParam               HFileParam;
typedef HFileParam *                    HFileParamPtr;
struct HVolumeParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  SInt32              filler2;
  SInt16              ioVolIndex;
  UInt32              ioVCrDate;
  UInt32              ioVLsMod;
  SInt16              ioVAtrb;
  UInt16              ioVNmFls;
  UInt16              ioVBitMap;
  UInt16              ioAllocPtr;
  UInt16              ioVNmAlBlks;
  UInt32              ioVAlBlkSiz;
  UInt32              ioVClpSiz;
  UInt16              ioAlBlSt;
  UInt32              ioVNxtCNID;
  UInt16              ioVFrBlk;
  UInt16              ioVSigWord;
  SInt16              ioVDrvInfo;
  FSIORefNum          ioVDRefNum;
  SInt16              ioVFSID;
  UInt32              ioVBkUp;
  SInt16              ioVSeqNum;
  UInt32              ioVWrCnt;
  UInt32              ioVFilCnt;
  UInt32              ioVDirCnt;
  SInt32              ioVFndrInfo[8];
};
typedef struct HVolumeParam             HVolumeParam;
typedef HVolumeParam *                  HVolumeParamPtr;
struct XIOParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioRefNum;
  SInt8               ioVersNum;
  SInt8               ioPermssn;
  Ptr                 ioMisc;
  Ptr                 ioBuffer;
  SInt32              ioReqCount;
  SInt32              ioActCount;
  SInt16              ioPosMode;              /* must have kUseWidePositioning bit set */
  wide                ioWPosOffset;           /* wide positioning offset */
};
typedef struct XIOParam                 XIOParam;
typedef XIOParam *                      XIOParamPtr;
struct XVolumeParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  UInt32              ioXVersion;             /* this XVolumeParam version (0) */
  SInt16              ioVolIndex;
  UInt32              ioVCrDate;
  UInt32              ioVLsMod;
  SInt16              ioVAtrb;
  UInt16              ioVNmFls;
  UInt16              ioVBitMap;
  UInt16              ioAllocPtr;
  UInt16              ioVNmAlBlks;
  UInt32              ioVAlBlkSiz;
  UInt32              ioVClpSiz;
  UInt16              ioAlBlSt;
  UInt32              ioVNxtCNID;
  UInt16              ioVFrBlk;
  UInt16              ioVSigWord;
  SInt16              ioVDrvInfo;
  SInt16              ioVDRefNum;
  SInt16              ioVFSID;
  UInt32              ioVBkUp;
  SInt16              ioVSeqNum;
  UInt32              ioVWrCnt;
  UInt32              ioVFilCnt;
  UInt32              ioVDirCnt;
  SInt32              ioVFndrInfo[8];
  UInt64              ioVTotalBytes;          /* total number of bytes on volume */
  UInt64              ioVFreeBytes;           /* number of free bytes on volume */
};
typedef struct XVolumeParam             XVolumeParam;
typedef XVolumeParam *                  XVolumeParamPtr;
struct AccessParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioRefNum;               /* <- , ioRefNum ( use to be filler3 ) */
  SInt16              ioDenyModes;            /*access rights data*/
  SInt16              filler4;
  SInt8               filler5;
  SInt8               ioACUser;               /*access rights for directory only*/
  SInt32              filler6;
  SInt32              ioACOwnerID;            /*owner ID*/
  SInt32              ioACGroupID;            /*group ID*/
  SInt32              ioACAccess;             /*access rights*/
  SInt32              ioDirID;
};
typedef struct AccessParam              AccessParam;
typedef AccessParam *                   AccessParamPtr;
struct ObjParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  SInt16              filler7;
  SInt16              ioObjType;              /*function code*/
  StringPtr           ioObjNamePtr;           /*ptr to returned creator/group name*/
  SInt32              ioObjID;                /*creator/group ID*/
};
typedef struct ObjParam                 ObjParam;
typedef ObjParam *                      ObjParamPtr;
struct CopyParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSVolumeRefNum      ioDstVRefNum;           /*destination vol identifier*/
  SInt16              filler8;
  StringPtr           ioNewName;              /*ptr to destination pathname*/
  StringPtr           ioCopyName;             /*ptr to optional name*/
  SInt32              ioNewDirID;             /*destination directory ID*/
  SInt32              filler14;
  SInt32              filler15;
  SInt32              ioDirID;
};
typedef struct CopyParam                CopyParam;
typedef CopyParam *                     CopyParamPtr;
struct WDParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  SInt16              ioWDCreated;
  SInt16              ioWDIndex;
  SInt32              ioWDProcID;
  FSVolumeRefNum      ioWDVRefNum;
  SInt16              filler10;
  SInt32              filler11;
  SInt32              filler12;
  SInt32              filler13;
  SInt32              ioWDDirID;
};
typedef struct WDParam                  WDParam;
typedef WDParam *                       WDParamPtr;
struct FIDParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  SInt32              filler14;
  StringPtr           ioDestNamePtr;          /* dest file name */
  SInt32              filler15;
  SInt32              ioDestDirID;            /* dest file's directory id */
  SInt32              filler16;
  SInt32              filler17;
  SInt32              ioSrcDirID;             /* source file's directory id */
  SInt16              filler18;
  SInt32              ioFileID;               /* file ID */
};
typedef struct FIDParam                 FIDParam;
typedef FIDParam *                      FIDParamPtr;
struct ForeignPrivParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  SInt32              ioFiller21;
  SInt32              ioFiller22;
  Ptr                 ioForeignPrivBuffer;
  SInt32              ioForeignPrivActCount;
  SInt32              ioForeignPrivReqCount;
  SInt32              ioFiller23;
  SInt32              ioForeignPrivDirID;
  SInt32              ioForeignPrivInfo1;
  SInt32              ioForeignPrivInfo2;
  SInt32              ioForeignPrivInfo3;
  SInt32              ioForeignPrivInfo4;
};
typedef struct ForeignPrivParam         ForeignPrivParam;
typedef ForeignPrivParam *              ForeignPrivParamPtr;
struct CSParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSSpecPtr           ioMatchPtr;             /* match array */
  SInt32              ioReqMatchCount;        /* maximum allowable matches */
  SInt32              ioActMatchCount;        /* actual match count */
  SInt32              ioSearchBits;           /* search criteria selector */
  CInfoPBPtr          ioSearchInfo1;          /* search values and range lower bounds */
  CInfoPBPtr          ioSearchInfo2;          /* search values and range upper bounds */
  SInt32              ioSearchTime;           /* length of time to run search */
  CatPositionRec      ioCatPosition;          /* current position in the catalog */
  Ptr                 ioOptBuffer;            /* optional performance enhancement buffer */
  SInt32              ioOptBufSize;           /* size of buffer pointed to by ioOptBuffer */
};
typedef struct CSParam                  CSParam;
typedef CSParam *                       CSParamPtr;
union HParamBlockRec {
  HIOParam            ioParam;
  HFileParam          fileParam;
  HVolumeParam        volumeParam;
  AccessParam         accessParam;
  ObjParam            objParam;
  CopyParam           copyParam;
  WDParam             wdParam;
  FIDParam            fidParam;
  CSParam             csParam;
  ForeignPrivParam    foreignPrivParam;
};
typedef union HParamBlockRec            HParamBlockRec;


typedef HParamBlockRec *                HParmBlkPtr;

struct CMovePBRec {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  SInt32              filler1;
  StringPtr           ioNewName;
  SInt32              filler2;
  SInt32              ioNewDirID;
  SInt32              filler3[2];
  SInt32              ioDirID;
};
typedef struct CMovePBRec               CMovePBRec;
typedef CMovePBRec *                    CMovePBPtr;
struct WDPBRec {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  SInt16              filler1;
  SInt16              ioWDIndex;
  SInt32              ioWDProcID;
  FSVolumeRefNum      ioWDVRefNum;
  SInt16              filler2[7];
  SInt32              ioWDDirID;
};
typedef struct WDPBRec                  WDPBRec;
typedef WDPBRec *                       WDPBPtr;
struct FCBPBRec {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/
  FSIORefNum          ioRefNum;
  SInt16              filler;
  SInt16              ioFCBIndx;
  SInt16              filler1;
  SInt32              ioFCBFlNm;
  SInt16              ioFCBFlags;
  UInt16              ioFCBStBlk;
  SInt32              ioFCBEOF;
  SInt32              ioFCBPLen;
  SInt32              ioFCBCrPs;
  FSVolumeRefNum      ioFCBVRefNum;
  SInt32              ioFCBClpSiz;
  SInt32              ioFCBParID;
};
typedef struct FCBPBRec                 FCBPBRec;
typedef FCBPBRec *                      FCBPBPtr;
struct VCB {
  QElemPtr            qLink;
  SInt16              qType;
  SInt16              vcbFlags;
  UInt16              vcbSigWord;
  UInt32              vcbCrDate;
  UInt32              vcbLsMod;
  SInt16              vcbAtrb;
  UInt16              vcbNmFls;
  SInt16              vcbVBMSt;
  SInt16              vcbAllocPtr;
  UInt16              vcbNmAlBlks;
  SInt32              vcbAlBlkSiz;
  SInt32              vcbClpSiz;
  SInt16              vcbAlBlSt;
  SInt32              vcbNxtCNID;
  UInt16              vcbFreeBks;
  Str27               vcbVN;
  SInt16              vcbDrvNum;
  FSIORefNum          vcbDRefNum;
  SInt16              vcbFSID;
  FSVolumeRefNum      vcbVRefNum;
  Ptr                 vcbMAdr;
  Ptr                 vcbBufAdr;
  SInt16              vcbMLen;
  SInt16              vcbDirIndex;
  SInt16              vcbDirBlk;
  UInt32              vcbVolBkUp;
  UInt16              vcbVSeqNum;
  SInt32              vcbWrCnt;
  SInt32              vcbXTClpSiz;
  SInt32              vcbCTClpSiz;
  UInt16              vcbNmRtDirs;
  SInt32              vcbFilCnt;
  SInt32              vcbDirCnt;
  SInt32              vcbFndrInfo[8];
  UInt16              vcbVCSize;
  UInt16              vcbVBMCSiz;
  UInt16              vcbCtlCSiz;
  UInt16              vcbXTAlBlks;
  UInt16              vcbCTAlBlks;
  SInt16              vcbXTRef;
  SInt16              vcbCTRef;
  Ptr                 vcbCtlBuf;
  SInt32              vcbDirIDM;
  SInt16              vcbOffsM;
};
typedef struct VCB                      VCB;
typedef VCB *                           VCBPtr;
struct DrvQEl {
  QElemPtr            qLink;
  SInt16              qType;
  SInt16              dQDrive;
  SInt16              dQRefNum;
  SInt16              dQFSID;
  UInt16              dQDrvSz;
  UInt16              dQDrvSz2;
};
typedef struct DrvQEl                   DrvQEl;
typedef DrvQEl *                        DrvQElPtr;
#endif  /* !__LP64__ */


/*
 *  FSPermissionInfo
 *  
 *  Discussion:
 *    This structure is used when kFSCatInfoPermissions is passed to
 *    the HFSPlus API. On return from GetCatalogInfo and
 *    GetCatalogInfoBulk, the userID, groupID, and mode fields are
 *    returned.  When passed to SetCatalogInfo, only the mode field is
 *    set.  See chmod(2) for details about the mode field. This is
 *    supported on Mac OS X only. NOTE: An FSFileSecurityRef retrieved
 *    via FSGetCatalogInfo is a copy and must be released using
 *    CFRelease() when no longer needed.
 */
struct FSPermissionInfo {
  UInt32              userID;
  UInt32              groupID;
  UInt8               reserved1;
  UInt8               userAccess;
  UInt16              mode;
  FSFileSecurityRef   fileSec;
};
typedef struct FSPermissionInfo         FSPermissionInfo;
/*  CatalogInfoBitmap describes which fields of the CatalogInfo you wish to get or set.*/

typedef UInt32                          FSCatalogInfoBitmap;
enum {
  kFSCatInfoNone                = 0x00000000,
  kFSCatInfoTextEncoding        = 0x00000001,
  kFSCatInfoNodeFlags           = 0x00000002, /* Locked (bit 0) and directory (bit 4) only */
  kFSCatInfoVolume              = 0x00000004,
  kFSCatInfoParentDirID         = 0x00000008,
  kFSCatInfoNodeID              = 0x00000010,
  kFSCatInfoCreateDate          = 0x00000020,
  kFSCatInfoContentMod          = 0x00000040,
  kFSCatInfoAttrMod             = 0x00000080,
  kFSCatInfoAccessDate          = 0x00000100, /* Note: although included in kFSCatInfoSettableInfo, attempts to set kFSCatInfoAccessDate will do nothing but will not cause an error */
  kFSCatInfoBackupDate          = 0x00000200,
  kFSCatInfoPermissions         = 0x00000400,
  kFSCatInfoFinderInfo          = 0x00000800,
  kFSCatInfoFinderXInfo         = 0x00001000,
  kFSCatInfoValence             = 0x00002000, /* Folders only, zero for files */
  kFSCatInfoDataSizes           = 0x00004000, /* Data fork logical and physical size */
  kFSCatInfoRsrcSizes           = 0x00008000, /* Resource fork logical and physical size */
  kFSCatInfoSharingFlags        = 0x00010000, /* sharingFlags: kioFlAttribMountedBit, kioFlAttribSharePointBit */
  kFSCatInfoUserPrivs           = 0x00020000, /* userPrivileges */
  kFSCatInfoUserAccess          = 0x00080000, /* (OS X only) */
  kFSCatInfoSetOwnership        = 0x00100000, /* (OS X only) */
  kFSCatInfoFSFileSecurityRef   = 0x00400000, /* FSFileSecurity Object, will show up in the permissions field independent of kFSCatInfoPermissions.  This is also not part of settable, gettable since it requires being released */
  kFSCatInfoAllDates            = 0x000003E0,
  kFSCatInfoGettableInfo        = 0x0003FFFF,
  kFSCatInfoSettableInfo        = 0x00001FE3, /* flags, dates, permissions, Finder info, text encoding */
  kFSCatInfoReserved            = (int)0xFFFC0000 /* bits that are currently reserved */
};

/*  Constants for nodeFlags field of FSCatalogInfo */
enum {
  kFSNodeLockedBit              = 0,    /* Set if file or directory is locked */
  kFSNodeLockedMask             = 0x0001,
  kFSNodeResOpenBit             = 2,    /* Set if the resource fork is open */
  kFSNodeResOpenMask            = 0x0004,
  kFSNodeDataOpenBit            = 3,    /* Set if the data fork is open */
  kFSNodeDataOpenMask           = 0x0008,
  kFSNodeIsDirectoryBit         = 4,    /* Set if the object is a directory */
  kFSNodeIsDirectoryMask        = 0x0010,
  kFSNodeCopyProtectBit         = 6,
  kFSNodeCopyProtectMask        = 0x0040,
  kFSNodeForkOpenBit            = 7,    /* Set if the file or directory has any open fork */
  kFSNodeForkOpenMask           = 0x0080,
  kFSNodeHardLinkBit            = 8,    /* Set if the file or directory has a link count > 1 */
  kFSNodeHardLinkMask           = 0x00000100
};

/*  Constants for sharingFlags field of FSCatalogInfo */
enum {
  kFSNodeInSharedBit            = 2,    /* Set if a directory is within a share point */
  kFSNodeInSharedMask           = 0x0004,
  kFSNodeIsMountedBit           = 3,    /* Set if a directory is a share point currently mounted by some user */
  kFSNodeIsMountedMask          = 0x0008,
  kFSNodeIsSharePointBit        = 5,    /* Set if a directory is a share point (exported volume) */
  kFSNodeIsSharePointMask       = 0x0020
};

#ifdef __LP64__

/*
 *  FSCatalogInfo
 *  
 *  Discussion:
 *    For each of the items in this structure, if the given bit is set
 *    in the whichInfo paramater to the FSGetCatalogInfo call then the
 *    field will be filled in on return.  Some fields which are not
 *    asked for my be returned as well, but do not depend on this
 *    behaviour.
 */
struct FSCatalogInfo {

  /*
   * kFSCatInfoNodeFlags / flag bits set if the file is locked, open,
   * is a directory, etc.
   */
  UInt16              nodeFlags;              /* node flags */

  /*
   * kFSCatInfoVolume / the volume reference of the returned item
   */
  FSVolumeRefNum      volume;                 /* object's volume ref */

  /*
   * kFSCatInfoParentDirID / the directory id of the parent of the
   * returned item
   */
  UInt32              parentDirID;            /* parent directory's ID */

  /*
   * kFSCatInfoNodeID / the file id of the returned item
   */
  UInt32              nodeID;                 /* file/directory ID */

  /*
   * kFSCatInfoSharingFlags / kioFlAttribMountedBit,
   * kioFlAttribSharePointBit
   */
  UInt8               sharingFlags;           /* kioFlAttribMountedBit and kioFlAttribSharePointBit */
  UInt8               userPrivileges;         /* user's effective AFP privileges (same as ioACUser) */
  UInt8               reserved1;

  /*
   * Unused
   */
  UInt8               reserved2;

  /*
   * kFSCatInfoCreateDate / date and time of creation
   */
  UTCDateTime         createDate;             /* date and time of creation */

  /*
   * kFSCatInfoContentMod / date and time of last modification of the
   * content of the returned item
   */
  UTCDateTime         contentModDate;         /* date and time of last fork modification */

  /*
   * kFSCatInfoAttrMod / date and time of the last modification of the
   * attributes of the returned item
   */
  UTCDateTime         attributeModDate;       /* date and time of last attribute modification */

  /*
   * kFSCatInfoAccessDate / date and time of the last access to the
   * returned item
   */
  UTCDateTime         accessDate;             /* date and time of last access (for Mac OS X) */

  /*
   * kFSCatInfoBackupDate / date and time of the last backup for the
   * returned item
   */
  UTCDateTime         backupDate;             /* date and time of last backup */

  /*
   * kFSCatInfoPermissions / Mac OS X only, file system permissions of
   * the returned item.  Coerce to a FSPermissionInfo to use.
   */
  FSPermissionInfo    permissions;            /* permissions (for Mac OS X), as FSPermissionInfo */

  /*
   * kFSCatInfoFinderInfo / file type, creator, flags, location. 
   * Coerce to a File/FolderInfo to use.
   */
  UInt8               finderInfo[16];         /* Finder information part 1, as FileInfo or FolderInfo  */

  /*
   * kFSCatInfoFinderXInfo / icon, script, et al.  Coerce to a
   * ExtendedFile/FolderInfo to use.
   */
  UInt8               extFinderInfo[16];      /* Finder information part 2, as ExtendedFileInfo or ExtendedFolderInfo */


  /*
   * kFSCatInfoDataSizes / the logical size of the data fork of the
   * returned item if a file
   */
  UInt64              dataLogicalSize;        /* files only */

  /*
   * kFSCatInfoDataSizes / the physical size of the data fork of the
   * returned item if a file
   */
  UInt64              dataPhysicalSize;       /* files only */

  /*
   * kFSCatInfoRsrcSizes / the logical size of the resource fork of the
   * returned item if a file
   */
  UInt64              rsrcLogicalSize;        /* files only */

  /*
   * kFSCatInfoRsrcSizes / the physical size of the resource fork of
   * the returned item if a file
   */
  UInt64              rsrcPhysicalSize;       /* files only */


  /*
   * kFSCatInfoValence / folders only, zero for files.
   */
  UInt32              valence;                /* folders only */

  /*
   * kFSCatInfoTextEncoding / the text encoding hint for the returned
   * item
   */
  TextEncoding        textEncodingHint;
};
typedef struct FSCatalogInfo            FSCatalogInfo;
#else
struct FSCatalogInfo {
  UInt16              nodeFlags;              /* node flags */
  FSVolumeRefNum      volume;                 /* object's volume ref */
  UInt32              parentDirID;            /* parent directory's ID */
  UInt32              nodeID;                 /* file/directory ID */
  UInt8               sharingFlags;           /* kioFlAttribMountedBit and kioFlAttribSharePointBit */
  UInt8               userPrivileges;         /* user's effective AFP privileges (same as ioACUser) */
  UInt8               reserved1;
  UInt8               reserved2;
  UTCDateTime         createDate;             /* date and time of creation */
  UTCDateTime         contentModDate;         /* date and time of last fork modification */
  UTCDateTime         attributeModDate;       /* date and time of last attribute modification */
  UTCDateTime         accessDate;             /* date and time of last access (for Mac OS X) */
  UTCDateTime         backupDate;             /* date and time of last backup */
  UInt32              permissions[4];         /* permissions (for Mac OS X), as FSPermissionInfo */
  UInt8               finderInfo[16];         /* Finder information part 1, as FileInfo or FolderInfo  */
  UInt8               extFinderInfo[16];      /* Finder information part 2, as ExtendedFileInfo or ExtendedFolderInfo */

  UInt64              dataLogicalSize;        /* files only */
  UInt64              dataPhysicalSize;       /* files only */
  UInt64              rsrcLogicalSize;        /* files only */
  UInt64              rsrcPhysicalSize;       /* files only */

  UInt32              valence;                /* folders only */
  TextEncoding        textEncodingHint;
};
typedef struct FSCatalogInfo            FSCatalogInfo;
#endif  /* defined(__LP64__) */

typedef FSCatalogInfo *                 FSCatalogInfoPtr;
struct FSRefParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  ConstStringPtr      ioNamePtr;              /*ptr to Vol:FileName string*/
  FSVolumeRefNum      ioVRefNum;              /*volume refnum (DrvNum for Eject and MountVol)*/

  SInt16              reserved1;              /* was ioRefNum */
  UInt8               reserved2;              /* was ioVersNum */
  UInt8               reserved3;              /* was ioPermssn */

  const FSRef *       ref;                    /* Input ref; the target of the call */
  FSCatalogInfoBitmap  whichInfo;
  FSCatalogInfo *     catInfo;
  UniCharCount        nameLength;             /* input name length for create/rename */
  const UniChar *     name;                   /* input name for create/rename */
  UInt32              ioDirID;
  FSSpecPtr           spec;
  FSRef *             parentRef;              /* ref of directory to move another ref to */
  FSRef *             newRef;                 /* Output ref */
  TextEncoding        textEncodingHint;       /* for Rename, MakeFSRefUnicode */
  HFSUniStr255 *      outName;                /* Output name for GetCatalogInfo */
};
typedef struct FSRefParam               FSRefParam;
typedef FSRefParam *                    FSRefParamPtr;
/* for use with PBCreateFileAndOpenFork*/
struct FSRefForkIOParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  const FSRef *       parentRef;              /* ref of directory to move another ref to */
  UniCharCount        nameLength;             /* input name length for create/rename */
  const UniChar *     name;                   /* input name for create/rename */
  FSCatalogInfoBitmap  whichInfo;
  const FSCatalogInfo * catInfo;
  UniCharCount        forkNameLength;         /* input; length of fork name */
  const UniChar *     forkName;               /* input; name of fork (NULL indicates data fork) */
  SInt8               permissions;            /* desired access to the fork */
  UInt8               reserved1;
  FSIORefNum          forkRefNum;             /* Output refNum of newly opened fork */
  FSRef *             newRef;                 /* Output ref */
};
typedef struct FSRefForkIOParam         FSRefForkIOParam;
typedef FSRefForkIOParam *              FSRefForkIOParamPtr;
typedef struct OpaqueFSIterator*        FSIterator;
enum {
  kFSIterateFlat                = 0,    /* Immediate children of container only */
  kFSIterateSubtree             = 1,    /* Entire subtree rooted at container */
  kFSIterateDelete              = 2,
  kFSIterateReserved            = (int)0xFFFFFFFC
};

typedef OptionBits                      FSIteratorFlags;
struct FSSearchParams {
  Duration            searchTime;             /* a Time Manager duration */
  OptionBits          searchBits;             /* which fields to search on */
  UniCharCount        searchNameLength;
  const UniChar *     searchName;
  FSCatalogInfo *     searchInfo1;            /* values and lower bounds */
  FSCatalogInfo *     searchInfo2;            /* masks and upper bounds */
};
typedef struct FSSearchParams           FSSearchParams;
typedef FSSearchParams *                FSSearchParamsPtr;
struct FSCatalogBulkParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  Boolean             containerChanged;       /* true if container changed since last iteration */
  UInt8               reserved;               /* make following fields 4-byte aligned */

  FSIteratorFlags     iteratorFlags;
  FSIterator          iterator;
  const FSRef *       container;              /* directory/volume to iterate */
  ItemCount           maximumItems;
  ItemCount           actualItems;
  FSCatalogInfoBitmap  whichInfo;
  FSCatalogInfo *     catalogInfo;            /* returns an array */
  FSRef *             refs;                   /* returns an array */
  FSSpecPtr           specs;                  /* returns an array */
  HFSUniStr255 *      names;                  /* returns an array */
  const FSSearchParams * searchParams;
};
typedef struct FSCatalogBulkParam       FSCatalogBulkParam;
typedef FSCatalogBulkParam *            FSCatalogBulkParamPtr;
typedef UInt16                          FSAllocationFlags;
enum {
  kFSAllocDefaultFlags          = 0x0000, /* as much as possible, not contiguous */
  kFSAllocAllOrNothingMask      = 0x0001, /* allocate all of the space, or nothing */
  kFSAllocContiguousMask        = 0x0002, /* new space must be one contiguous piece */
  kFSAllocNoRoundUpMask         = 0x0004, /* don't round up allocation to clump size */
  kFSAllocReservedMask          = 0xFFF8 /* these bits are reserved and must not be set */
};

struct FSForkIOParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  void *              reserved1;              /* was ioNamePtr */
  SInt16              reserved2;              /* was ioVRefNum */
  FSIORefNum          forkRefNum;             /* same as ioRefNum */
  UInt8               reserved3;              /* was ioVersNum */
  SInt8               permissions;            /* desired access to the fork */
  const FSRef *       ref;                    /* which object to open */


  Ptr                 buffer;                 /*data buffer Ptr*/
  UInt32              requestCount;           /*requested byte count*/
  UInt32              actualCount;            /*actual byte count completed*/
  UInt16              positionMode;           /*initial file positioning*/
  SInt64              positionOffset;         /*file position offset*/

  FSAllocationFlags   allocationFlags;
  UInt64              allocationAmount;

  UniCharCount        forkNameLength;         /* input; length of fork name */
  const UniChar *     forkName;               /* input; name of fork */

  CatPositionRec      forkIterator;
  HFSUniStr255 *      outForkName;            /* output; name of fork */
};
typedef struct FSForkIOParam            FSForkIOParam;
typedef FSForkIOParam *                 FSForkIOParamPtr;
typedef UInt8                           FSForkInfoFlags;
struct FSForkInfo {
  FSForkInfoFlags     flags;                  /* copy of FCB flags */
  SInt8               permissions;
  FSVolumeRefNum      volume;
  UInt32              reserved2;
  UInt32              nodeID;                 /* file or directory ID */
  UInt32              forkID;                 /* fork ID */
  UInt64              currentPosition;
  UInt64              logicalEOF;
  UInt64              physicalEOF;
  UInt64              process;                /* should be ProcessSerialNumber */
};
typedef struct FSForkInfo               FSForkInfo;
typedef FSForkInfo *                    FSForkInfoPtr;
struct FSForkCBInfoParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  FSIORefNum          desiredRefNum;          /* 0 to iterate, non-0 for specific refnum */
  FSVolumeRefNum      volumeRefNum;           /* volume to match, or 0 for all volumes */
  FSIORefNum          iterator;               /* 0 to start iteration */
  FSVolumeRefNum      actualRefNum;           /* actual refnum found */

  FSRef *             ref;
  FSForkInfo *        forkInfo;
  HFSUniStr255 *      forkName;
};
typedef struct FSForkCBInfoParam        FSForkCBInfoParam;
typedef FSForkCBInfoParam *             FSForkCBInfoParamPtr;
/* Parameter block for use with 64 bit range lock calls.*/
struct FSRangeLockParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  FSIORefNum          forkRefNum;             /* fork to operate on */
  UInt64              requestCount;           /*requested byte count*/
  UInt16              positionMode;           /*initial file positioning*/
  SInt64              positionOffset;         /*file position offset*/
  UInt64              rangeStart;             /* byte number of first byte (un)locked */

};
typedef struct FSRangeLockParam         FSRangeLockParam;
typedef FSRangeLockParam *              FSRangeLockParamPtr;
typedef UInt32                          FSVolumeInfoBitmap;
enum {
  kFSVolInfoNone                = 0x0000,
  kFSVolInfoCreateDate          = 0x0001,
  kFSVolInfoModDate             = 0x0002,
  kFSVolInfoBackupDate          = 0x0004,
  kFSVolInfoCheckedDate         = 0x0008,
  kFSVolInfoFileCount           = 0x0010,
  kFSVolInfoDirCount            = 0x0020,
  kFSVolInfoSizes               = 0x0040, /* totalBytes and freeBytes */
  kFSVolInfoBlocks              = 0x0080, /* blockSize, totalBlocks, freeBlocks */
  kFSVolInfoNextAlloc           = 0x0100,
  kFSVolInfoRsrcClump           = 0x0200,
  kFSVolInfoDataClump           = 0x0400,
  kFSVolInfoNextID              = 0x0800,
  kFSVolInfoFinderInfo          = 0x1000,
  kFSVolInfoFlags               = 0x2000,
  kFSVolInfoFSInfo              = 0x4000, /* filesystemID, signature */
  kFSVolInfoDriveInfo           = 0x8000, /* driveNumber, driverRefNum */
  kFSVolInfoGettableInfo        = 0xFFFF, /* This seems like it is here just for completeness */
  kFSVolInfoSettableInfo        = 0x3004 /* backup date, Finder info, flags */
};

/* FSVolumeInfo.flags bits.  These are the same as for ioVAtrb, but with nicer names. */
enum {
  kFSVolFlagDefaultVolumeBit    = 5,    /* Set if the volume is the default volume */
  kFSVolFlagDefaultVolumeMask   = 0x0020,
  kFSVolFlagFilesOpenBit        = 6,    /* Set if there are open files or iterators */
  kFSVolFlagFilesOpenMask       = 0x0040,
  kFSVolFlagHardwareLockedBit   = 7,    /* Set if volume is locked by a hardware setting */
  kFSVolFlagHardwareLockedMask  = 0x0080,
  kFSVolFlagJournalingActiveBit = 14,   /* Set if journaling is active on volume */
  kFSVolFlagJournalingActiveMask = 0x4000,
  kFSVolFlagSoftwareLockedBit   = 15,   /* Set if volume is locked by software */
  kFSVolFlagSoftwareLockedMask  = 0x8000
};


struct FSVolumeInfo {
                                              /* Dates -- zero means "never" or "unknown" */
  UTCDateTime         createDate;
  UTCDateTime         modifyDate;
  UTCDateTime         backupDate;
  UTCDateTime         checkedDate;

                                              /* File/Folder counts -- return zero if unknown */
  UInt32              fileCount;              /* total files on volume */
  UInt32              folderCount;            /* total folders on volume */
                                              /* Note: no root directory counts */

  UInt64              totalBytes;             /* total number of bytes on volume */
  UInt64              freeBytes;              /* number of free bytes on volume */

                                              /* HFS and HFS Plus specific.  Set fields to zero if not appropriate */
  UInt32              blockSize;              /* size (in bytes) of allocation blocks */
  UInt32              totalBlocks;            /* number of allocation blocks in volume */
  UInt32              freeBlocks;             /* number of unused allocation blocks */
  UInt32              nextAllocation;         /* start of next allocation search */
  UInt32              rsrcClumpSize;          /* default resource fork clump size */
  UInt32              dataClumpSize;          /* default data fork clump size */
  UInt32              nextCatalogID;          /* next unused catalog node ID ¥¥¥ OYG ¥¥¥ need to make HFSVolumes.h work Should be HFSCatalogNodeID*/
  UInt8               finderInfo[32];         /* information used by Finder */

                                              /* Identifying information */
  UInt16              flags;                  /* ioVAtrb */
  UInt16              filesystemID;           /* ioVFSID */
  UInt16              signature;              /* ioVSigWord, unique within an FSID */
  UInt16              driveNumber;            /* ioVDrvInfo */
  FSIORefNum          driverRefNum;           /* ioVDRefNum */
};
typedef struct FSVolumeInfo             FSVolumeInfo;
typedef FSVolumeInfo *                  FSVolumeInfoPtr;
struct FSVolumeInfoParam {
  QElemPtr            qLink;                  /*queue link in header*/
  SInt16              qType;                  /*type byte for safety check*/
  SInt16              ioTrap;                 /*FS: the Trap*/
  Ptr                 ioCmdAddr;              /*FS: address to dispatch to*/
  IOCompletionUPP     ioCompletion;           /*completion routine addr (0 for synch calls)*/
  volatile OSErr      ioResult;               /*result code*/
  StringPtr           ioNamePtr;              /* unused */
  FSVolumeRefNum      ioVRefNum;              /* volume refnum */

  UInt32              volumeIndex;            /* index, or 0 to use ioVRefNum */
  FSVolumeInfoBitmap  whichInfo;              /* which volumeInfo fields to get/set */
  FSVolumeInfo *      volumeInfo;             /* information about the volume */
  HFSUniStr255 *      volumeName;             /* output; pointer to volume name */
  FSRef *             ref;                    /* volume's FSRef */
};
typedef struct FSVolumeInfoParam        FSVolumeInfoParam;
typedef FSVolumeInfoParam *             FSVolumeInfoParamPtr;
struct GetVolParmsInfoBuffer {
  SInt16              vMVersion;              /*version number*/
  SInt32              vMAttrib;               /*bit vector of attributes (see vMAttrib constants)*/
  Handle              vMLocalHand;            /*handle to private data*/
  SInt32              vMServerAdr;            /*AppleTalk server address or zero*/
                                              /*       vMVersion 1 GetVolParmsInfoBuffer ends here */
  SInt32              vMVolumeGrade;          /*approx. speed rating or zero if unrated*/
  SInt16              vMForeignPrivID;        /*foreign privilege model supported or zero if none*/
                                              /*       vMVersion 2 GetVolParmsInfoBuffer ends here */
  SInt32              vMExtendedAttributes;   /*extended attribute bits (see vMExtendedAttributes constants)*/
                                              /*       vMVersion 3 GetVolParmsInfoBuffer ends here */
  void *              vMDeviceID;             /* device id name for interoperability with IOKit */
                                              /*       vMVersion 4 GetVolParmsInfoBuffer ends here */
  UniCharCount        vMMaxNameLength;
                                              /*       vMVersion 5 GetVolParmsInfoBuffer ends here */
};
typedef struct GetVolParmsInfoBuffer    GetVolParmsInfoBuffer;
/* 
    The following are structures to be filled out with the _PBGetVolMountInfo call
    and passed back into the _PBVolumeMount call for external file system mounts. 
*/
/* the "signature" of the file system */

typedef OSType                          VolumeType;
enum {
                                        /* the signature for AppleShare */
  AppleShareMediaType           = 'afpm'
};

/*
    VolMount stuff was once in FSM.Å
*/
struct VolMountInfoHeader {
  SInt16              length;                 /* length of location data (including self) */
  VolumeType          media;                  /* type of media.  Variable length data follows */
};
typedef struct VolMountInfoHeader       VolMountInfoHeader;
typedef VolMountInfoHeader *            VolMountInfoPtr;
/* The new volume mount info record.  The old one is included for compatibility. 
    the new record allows access by foriegn filesystems writers to the flags 
    portion of the record. This portion is now public.  
*/
struct VolumeMountInfoHeader {
  SInt16              length;                 /* length of location data (including self) */
  VolumeType          media;                  /* type of media (must be registered with Apple) */
  SInt16              flags;                  /* volume mount flags. Variable length data follows */
};
typedef struct VolumeMountInfoHeader    VolumeMountInfoHeader;
typedef VolumeMountInfoHeader *         VolumeMountInfoHeaderPtr;
/* volume mount flags */
enum {
  volMountNoLoginMsgFlagBit     = 0,    /* Input to VolumeMount: If set, the file system */
  volMountNoLoginMsgFlagMask    = 0x0001, /*  should suppresss any log-in message/greeting dialog */
  volMountExtendedFlagsBit      = 7,    /* Input to VolumeMount: If set, the mount info is a */
  volMountExtendedFlagsMask     = 0x0080, /*  AFPXVolMountInfo record for 3.7 AppleShare Client */
  volMountInteractBit           = 15,   /* Input to VolumeMount: If set, it's OK for the file system */
  volMountInteractMask          = 0x8000, /*  to perform user interaction to mount the volume */
  volMountChangedBit            = 14,   /* Output from VoumeMount: If set, the volume was mounted, but */
  volMountChangedMask           = 0x4000, /*  the volume mounting information record needs to be updated. */
  volMountFSReservedMask        = 0x00FF, /* bits 0-7 are defined by each file system for its own use */
  volMountSysReservedMask       = 0xFF00 /* bits 8-15 are reserved for Apple system use */
};



struct AFPVolMountInfo {
  SInt16              length;                 /* length of location data (including self) */
  VolumeType          media;                  /* type of media */
  SInt16              flags;                  /* bits for no messages, no reconnect */
  SInt8               nbpInterval;            /* NBP Interval parameter (IM2, p.322) */
  SInt8               nbpCount;               /* NBP Interval parameter (IM2, p.322) */
  SInt16              uamType;                /* User Authentication Method */
  SInt16              zoneNameOffset;         /* short positive offset from start of struct to Zone Name */
  SInt16              serverNameOffset;       /* offset to pascal Server Name string */
  SInt16              volNameOffset;          /* offset to pascal Volume Name string */
  SInt16              userNameOffset;         /* offset to pascal User Name string */
  SInt16              userPasswordOffset;     /* offset to pascal User Password string */
  SInt16              volPasswordOffset;      /* offset to pascal Volume Password string */
  char                AFPData[144];           /* variable length data may follow */
};
typedef struct AFPVolMountInfo          AFPVolMountInfo;
typedef AFPVolMountInfo *               AFPVolMountInfoPtr;


/* AFPXVolMountInfo is the new AFP volume mount info record, requires the 3.7 AppleShare Client */
struct AFPXVolMountInfo {
  SInt16              length;                 /* length of location data (including self) */
  VolumeType          media;                  /* type of media */
  SInt16              flags;                  /* bits for no messages, no reconnect */
  SInt8               nbpInterval;            /* NBP Interval parameter (IM2, p.322) */
  SInt8               nbpCount;               /* NBP Interval parameter (IM2, p.322) */
  SInt16              uamType;                /* User Authentication Method type */
  SInt16              zoneNameOffset;         /* short positive offset from start of struct to Zone Name */
  SInt16              serverNameOffset;       /* offset to pascal Server Name string */
  SInt16              volNameOffset;          /* offset to pascal Volume Name string */
  SInt16              userNameOffset;         /* offset to pascal User Name string */
  SInt16              userPasswordOffset;     /* offset to pascal User Password string */
  SInt16              volPasswordOffset;      /* offset to pascal Volume Password string */
  SInt16              extendedFlags;          /* extended flags word */
  SInt16              uamNameOffset;          /* offset to a pascal UAM name string */
  SInt16              alternateAddressOffset; /* offset to Alternate Addresses in tagged format */
  char                AFPData[176];           /* variable length data may follow */
};
typedef struct AFPXVolMountInfo         AFPXVolMountInfo;
typedef AFPXVolMountInfo *              AFPXVolMountInfoPtr;
enum {
  kAFPExtendedFlagsAlternateAddressMask = 1 /*  bit in AFPXVolMountInfo.extendedFlags that means alternateAddressOffset is used*/
};


enum {
                                        /* constants for use in AFPTagData.fType field*/
  kAFPTagTypeIP                 = 0x01, /* 4 byte IP address (MSB first)            */
  kAFPTagTypeIPPort             = 0x02, /* 4 byte IP address, 2 byte port (MSB first)     */
  kAFPTagTypeDDP                = 0x03, /* Net,Node,Socket Sent by the server, currently unused by the client */
  kAFPTagTypeDNS                = 0x04  /* DNS name in  address:port format   (total length variable up to 254 chars of dns name)          */
};


enum {
                                        /* constants for use in AFPTagData.fLength field*/
  kAFPTagLengthIP               = 0x06,
  kAFPTagLengthIPPort           = 0x08,
  kAFPTagLengthDDP              = 0x06
};

struct AFPTagData {
  UInt8               fLength;                /* length of this data tag including the fLength field */
  UInt8               fType;
  UInt8               fData[1];               /* variable length data */
};
typedef struct AFPTagData               AFPTagData;
struct AFPAlternateAddress {
                                              /* ¥¥¥ÊNOTE: fVersion was missing in 3.2 Universal Interfaces*/
  UInt8               fVersion;               /* version of the structure (currently 0x00)*/
  UInt8               fAddressCount;
  UInt8               fAddressList[1];        /* actually variable length packed set of AFPTagData */
};
typedef struct AFPAlternateAddress      AFPAlternateAddress;
enum {
  kLargeIconSize                = 256,
  kLarge4BitIconSize            = 512,
  kLarge8BitIconSize            = 1024,
  kSmallIconSize                = 64,
  kSmall4BitIconSize            = 128,
  kSmall8BitIconSize            = 256
};

/*
 *  NewIOCompletionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern IOCompletionUPP
NewIOCompletionUPP(IOCompletionProcPtr userRoutine)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeIOCompletionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeIOCompletionUPP(IOCompletionUPP userUPP)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeIOCompletionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeIOCompletionUPP(
  ParmBlkPtr       paramBlock,
  IOCompletionUPP  userUPP)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline IOCompletionUPP                                      NewIOCompletionUPP(IOCompletionProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeIOCompletionUPP(IOCompletionUPP) { }
    inline void                                                 InvokeIOCompletionUPP(ParmBlkPtr paramBlock, IOCompletionUPP userUPP) { (*userUPP)(paramBlock); }
  #else
    #define NewIOCompletionUPP(userRoutine)                     ((IOCompletionUPP)userRoutine)
    #define DisposeIOCompletionUPP(userUPP)
    #define InvokeIOCompletionUPP(paramBlock, userUPP)          (*userUPP)(paramBlock)
  #endif
#endif


/*
    MakeFSRefUnicode
    Create an FSRef for an existing object specified by 
    Parent FSRef and Unicode name.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             A pointer to the parent directory FSRef
    ->  name            A pointer to Unicode name
    ->  nameLength      The length of the Unicode Name
    ->  textEncodingHint A suggested text encoding to use for the name
    <-  newRef          A pointer to an FSRef
*/
/*
 *  FSMakeFSRefUnicode()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSMakeFSRefUnicode(const FSRef *parentRef, UniCharCount nameLength, const UniChar *name, TextEncoding textEncodingHint, FSRef *newRef) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBMakeFSRefUnicodeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBMakeFSRefUnicodeSync(FSRefParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBMakeFSRefUnicodeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBMakeFSRefUnicodeAsync(FSRefParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    CompareFSRefs
    Test whether two FSRefs refer to the same file or directory.
    If they do, noErr is returned.  Otherwise, an appropriate error
    (such as errFSRefsDifferent) is returned.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             A pointer to the first FSRef
    ->  parentRef       A pointer to the second FSRef
*/
/*
 *  FSCompareFSRefs()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSCompareFSRefs(const FSRef *ref1, const FSRef *ref2)                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCompareFSRefsSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBCompareFSRefsSync(FSRefParam * paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCompareFSRefsAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBCompareFSRefsAsync(FSRefParam * paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    CreateFileUnicode
    Creates a new file.  The input filename is in Unicode.
    You can optionally set catalog info for the file.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The directory where the file is to be created
    ->  whichInfo       Which catalog info fields to set
    ->  catInfo         The values for catalog info fields to set; may be NULL
    ->  nameLength      Number of Unicode characters in the file's name
    ->  name            A pointer to the Unicode name
    <-  spec            A pointer to the FSSpec for the new directory; may be NULL.  Ignored on 64 bit.
    <-  newRef          A pointer to the FSRef for the new file; may be NULL
*/
/*
 *  FSCreateFileUnicode()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSCreateFileUnicode(const FSRef *parentRef, UniCharCount nameLength, const UniChar *name, FSCatalogInfoBitmap whichInfo, const FSCatalogInfo *catalogInfo, FSRef *newRef, FSSpecPtr newSpec) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCreateFileUnicodeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBCreateFileUnicodeSync(FSRefParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCreateFileUnicodeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBCreateFileUnicodeAsync(FSRefParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    CreateDirectoryUnicode
    Creates a new directory.  The input directory name is in Unicode.
    You can optionally set catalog info for the directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The parent directory where the directory is to be created
    ->  whichInfo       Which catalog info fields to set
    ->  catInfo         The values for catalog info fields to set; may be NULL
    ->  nameLength      Number of Unicode characters in the directory's name
    ->  name            A pointer to the Unicode name
    <-  ioDirID         The DirID of the new directory
    <-  newSpec         A pointer to the FSSpec for the new directory; may be NULL.  Ignored on 64 bit.
    <-  newRef          A pointer to the FSRef for the new directory; may be NULL
*/
/*
 *  FSCreateDirectoryUnicode()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSCreateDirectoryUnicode(const FSRef *parentRef, UniCharCount nameLength, const UniChar *name, FSCatalogInfoBitmap whichInfo, const FSCatalogInfo *catalogInfo, FSRef *newRef, FSSpecPtr newSpec, UInt32 *newDirID) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCreateDirectoryUnicodeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBCreateDirectoryUnicodeSync(FSRefParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCreateDirectoryUnicodeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBCreateDirectoryUnicodeAsync(FSRefParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    DeleteObject
    Deletes an existing file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory to be deleted
*/
/*
 *  FSDeleteObject()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSDeleteObject(const FSRef * ref)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDeleteObjectSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBDeleteObjectSync(FSRefParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDeleteObjectAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBDeleteObjectAsync(FSRefParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    UnlinkObject
    Unlinks an existing file or deletes an existing directory.  This call will succeed on an open file, unlike
    DeleteObject.  This call will unlink an archive directory link (where DeleteObject will treat the ADL as a directory).
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory to be deleted
*/
/*
 *  FSUnlinkObject()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSErr  FSUnlinkObject(const FSRef * ref)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBUnlinkObjectSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSErr  PBUnlinkObjectSync(FSRefParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBUnlinkObjectAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void  PBUnlinkObjectAsync(FSRefParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    MoveObject
    Move an existing file or directory into a different directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory to be moved
    ->  parentRef       The file or directory will be moved into this directory
    <-  newRef          A new FSRef for the file or directory in its new location;
                        optional, may be NULL
    NOTE: Moving an object may change its FSRef.  If you want to continue to
    refer to the object, you should pass a non-NULL pointer in newRef and use
    that returned FSRef to access the object after the move.  The FSRef passed
    in "ref" may or may not be usable to access the object after it is moved.
    "newRef" may point to the same storage as "parentRef" or "ref".
*/
/*
 *  FSMoveObject()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSMoveObject(const FSRef *ref, const FSRef *destDirectory, FSRef *newRef)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBMoveObjectSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBMoveObjectSync(FSRefParam * paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBMoveObjectAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBMoveObjectAsync(FSRefParam * paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    ExchangeObjects
    swap the contents of two files.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The first file 
    ->  parentRef       The second file 
*/
/*
 *  FSExchangeObjects()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSExchangeObjects(const FSRef *ref, const FSRef *destRef)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBExchangeObjectsSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBExchangeObjectsSync(FSRefParam * paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBExchangeObjectsAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBExchangeObjectsAsync(FSRefParam * paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    FSReplaceObject and FSPathReplaceObject

    You can use the FSReplaceObject and FSPathReplaceObject functions to replace
    one object, the original object, with another, the replacement object.
    FSReplaceObject and FSPathReplaceObject are provided to assist in properly
    preserving metadata during safe save operations. These functions allow files
    to replace files, directories to replace directories, directories to replace
    files and files to replace directories (so a package can replace a file, or
    a file can replace a package). Both objects must reside on the same volume.

    If FSReplaceObject or FSPathReplaceObject are successful, the result object
    will exist at the location of the original object. Nothing will exist at the
    location of the replacement object.  By default FSReplaceObject and  FSPathReplaceObject
    will test the object for write access, and fail if write access is not available.
    Write access in this context is ACL_WRITE_DATA for files and ACL_ADD_FILE for directories.
    This check can be bypassed by passing the kFSReplaceObjectDoNotCheckObjectWriteAccess flag.

    FSReplaceObject and FSPathReplaceObject may need to create a temporary object
    when one of the input objects is a directory, or when the
    kFSReplaceObjectSaveOriginalAsABackup option is specified. The temporary object
    is required since the rename system call does not handle non-empty directory
    replacement or file <-> directory replacement. The file replacing file case
    does not use a temporary object (unless the kFSReplaceObjectSaveOriginalAsABackup
    option is specified). If no temporary name is provided and a temporary object
    is required, then the functions will return an error. Providing a temporary
    name will also allow an application to recover the original object should a
    catastrophic failure (such as a power outage) occur during a replacement
    involving a directory. The temporary object will be created with the temporary
    name either in the temporary directory specified by the temporaryDirectory
    parameter, or in the preferred temporary directory (the preferred temporary
    directory can be obtained with the FSGetTemporaryDirectoryForReplaceObject
    or FSPathGetTemporaryDirectoryForReplaceObject functions). 

    If the kFSReplaceObjectSaveOriginalAsABackup option is specified, the original
    object will be saved in one of two locations: If the temporaryName parameter
    is provided, the original object will be saved with that temporary name in
    the temporary directory. If the temporaryName parameter is not provided and
    the newName parameter is provided, the original object will be saved with
    its original name in the temporary directory.

    The result object will have the data (or directory contents) from the
    replacement object, but will have the original object's creation date and
    backup date. By default, the result object will have extended attributes
    merged from the original object and the replacement object, using the extended
    attributes of the replacement object if the original object has extended
    attributes with the same name. If the kFSReplaceObjectReplaceMetadata option
    flag is passed, then the extended attributes of the original object will be
    completely overwritten by the extended attributes of the replacement object.
    
    The kFSReplaceObjectReplaceMetadata also controls how the label, type, creator,
    hidden extension bit, strong binding, custom icon and custom badge are handled.
    For the custom icon, custom badge, label (where a value of 0 is considered not
    present), and strong binding the replacement object's attributes will be
    used if present, otherwise the original object's attributes will be retained.  For
    the extension hidden bit, type, and creator the original object's attribute is
    preserved.  Setting the kFSReplaceObjectReplaceMetadata flag will use the replacement
    object's attributes for all cases.  Type and creator are only handled in the 
    file to file case.  Strong bindings are not applicable to plain directories.
    
    The default handling of ACL and mode info depends on the types of objects
    involved. If the original object and replacement object are of the same type
    (both files or both directories) then the default behavior is to use the ACL
    and mode of the original object. Setting the kFSReplaceObjectReplacePermissionInfo
    flag will use the ACL and mode from the replacementObject.

    If the objects are different types, then the ACL and mode will be based off
    the original object and will translated as follows:

        Mode:
        file r -> dir rx
        file w -> dir wx
        file x -> dir nothing

        dir r -> file r
        dir w -> file w
        dir x -> file nothing

        ACLs:

        ACL_SEARCH will be added to any allow ACE in the file to directory case.
        ACL_EXECUTE will be removed from any ACE in the file to directory case.
        
        ACL_SEARCH and ACL_DELETE_CHILD will be removed from any ACE in the directory to file case.

        File                Directory
        ACL_READ_DATA   <-> ACL_LIST_DIRECTORY
        ACL_WRITE_DATA  <-> ACL_ADD_FILE
        ACL_APPEND_DATA <-> ACL_ADD_SUBDIRECTORY

        ACL_DELETE , ACL_READ_ATTRIBUTES, ACL_WRITE_ATTRIBUTES, ACL_READ_EXTATTRIBUTES,
        ACL_WRITE_EXTATTRIBUTES, ACL_READ_SECURITY, ACL_WRITE_SECURITY and ACL_CHANGE_OWNER
        are the same for both types and will be left alone.
                        
    This translation behavior can be avoided by passing in either
    kFSReplaceObjectPreservePermissionInfo to use the original object ACL and
    mode info unchanged (except where the bits have different meanings for files
    and directories) or kFSReplaceObjectReplacePermissionInfo to use the ACL and
    mode info of the replacement object (except where the bits have different
    meanings for files and directories). Setting both of these flags is an
    error -- the call will fail with a paramErr. The permissions are only set on
    the top-level object -- the permissions inside a directory are unaffected.
    
    FSReplaceObject may not be atomic -- it may issue multiple system calls to
    accurately replace and preserve the metadata of a file system object.
    
    FSReplaceObject may fail if the source or destination files are open or
    the source or destination objects are directories which contain open files.
*/

/*
 *  FSReplaceObjectOptions
 *  
 *  Discussion:
 *    Options that can be passed to the FSReplaceObject and
 *    FSPathReplaceObject functions. These are mask values that can be
 *    ORed together to set more than one option.
 */
enum {

  /*
   * Use the default behavior (see the discussion of the
   * FSReplaceObject function)
   */
  kFSReplaceObjectDefaultOptions = 0,

  /*
   * The result object will only have the metadata from the replacement
   * object. The result object will have no metadata from the original
   * object.
   */
  kFSReplaceObjectReplaceMetadata = 0x01,

  /*
   * The original object will be saved with the temporary name in the
   * temporaryDirectory, or will be saved with the original name (if no
   * temporaryName is provided and a newName is provided) in the
   * temporaryDirectory.
   */
  kFSReplaceObjectSaveOriginalAsABackup = 0x02,

  /*
   * ACL and mode info will come from the replacement object.
   */
  kFSReplaceObjectReplacePermissionInfo = 0x04,

  /*
   * ACL and mode info will come from the original object
   */
  kFSReplaceObjectPreservePermissionInfo = 0x08,

  /*
   * FSReplaceObject does not actually require write permission to
   * perform the replace operation, since no actual writing is done to
   * the original object.  By default ReplaceObject will test the
   * object for write permissions and fail if they are not present
   * (tested using accessx_np).  Write permission in this context is
   * defined as ACL_WRITE_DATA for files and ACL_ADD_FILE for
   * directories.  Pass this flag to skip this check (which will allow
   * the replace to operate on "read only" objects).
   */
  kFSReplaceObjectDoNotCheckObjectWriteAccess = 0x10
};

/*
 *  FSReplaceObject()
 *  
 *  Discussion:
 *    This function will replace the object specified by originalObject
 *    with the object specified by replacementObject. The result object
 *    will be in the same parent directory as the original object.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    originalObject:
 *      The original object to be replaced.
 *    
 *    replacementObject:
 *      The object which will replace the original object.
 *    
 *    newName:
 *      The new name for the result object. Pass NULL to use the
 *      original object's name.
 *    
 *    temporaryName:
 *      The name of a temporary object should the operation require a
 *      temporary object. The temporary object will be created in the
 *      preferred temporary directory or in the directory specified by
 *      the temporaryDirectory parameter.
 *    
 *    temporaryDirectory:
 *      The directory where the temporary object (if needed) will be
 *      created. The temporary directory must be on the same volume as
 *      the original object. If NULL is passed, then the preferred
 *      temporary directory is used (as per
 *      FSGetTemporaryDirectoryForReplaceObject).
 *    
 *    flags:
 *      A set of options to specify non-default behavior.
 *    
 *    resultObject:
 *      A new FSRef for the result object.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSReplaceObject(const FSRef *originalObject, const FSRef *replacementObject, CFStringRef newName, CFStringRef temporaryName, const FSRef *temporaryDirectory, OptionBits flags, FSRef *resultObject) __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA, "Use NSFileManager's -replaceItemAtURL:withItemAtURL:backupItemName:options:resultingItemURL:error: instead. WARNING: FSReplaceObject does not work correctly in sandboxed apps.");


/*
 *  FSPathReplaceObject()
 *  
 *  Discussion:
 *    This function will replace the object specified by
 *    originalObjectPath with the object specified by
 *    replacementObjectPath. The result object will be in the same
 *    parent directory as the original object.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    originalObjectPath:
 *      The path to the original object to be replaced.
 *    
 *    replacementObjectPath:
 *      The path to the object which will replace the original object.
 *    
 *    newName:
 *      The new name for the result object. Pass NULL to use the
 *      original object's name.
 *    
 *    temporaryName:
 *      The name of a temporary object should the operation require a
 *      temporary object. The temporary object will be created in the
 *      preferred temporary directory or in the directory specified by
 *      the temporaryDirectory parameter.
 *    
 *    temporaryDirectoryPath:
 *      The path to the directory where the temporary object (if
 *      needed) will be created. The temporary directory must be on the
 *      same volume as the original object. If NULL is passed, then the
 *      preferred temporary directory is used (as per
 *      FSPathGetTemporaryDirectoryForReplaceObject).
 *    
 *    flags:
 *      A set of options to specify non-default behavior.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
	extern OSStatus  FSPathReplaceObject(const char *originalObjectPath, const char *replacementObjectPath, CFStringRef newName, CFStringRef temporaryName, const char *temporaryDirectoryPath, OptionBits flags) __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA, "Use NSFileManager's -replaceItemAtURL:withItemAtURL:backupItemName:options:resultingItemURL:error: instead. WARNING: FSPathReplaceObject does not work correctly in sandboxed apps.");


/*
 *  FSGetTemporaryDirectoryForReplaceObject()
 *  
 *  Discussion:
 *    This function will return the preferred directory for use as the
 *    temporaryDirectory by FSReplaceObject. It will return an
 *    appropriate temporary location or the parent directory of the
 *    original object.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    originalObject:
 *      The original object to be replaced.
 *    
 *    temporaryDirectory:
 *      The preferred temporary directory.
 *    
 *    flags:
 *      A set of options to specify non-default behavior. Currently no
 *      flags are defined - pass in 0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetTemporaryDirectoryForReplaceObject(const FSRef *originalObject, FSRef *temporaryDirectory, OptionBits flags) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathGetTemporaryDirectoryForReplaceObject()
 *  
 *  Discussion:
 *    This function will return the preferred directory for use as the
 *    temporaryDirectory by FSPathReplaceObject. It will return an
 *    appropriate temporary location or the parent directory of the
 *    original object.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    originalObjectPath:
 *      The path to the original object to be replaced.
 *    
 *    temporaryDirectoryPath:
 *      The path to the preferred temporary directory.
 *    
 *    maxPathSize:
 *      The size of the buffer pointed to by the temporaryDirectoryPath
 *      parameter.
 *    
 *    flags:
 *      A set of options to specify non-default behavior. Currently no
 *      flags are defined - pass in 0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathGetTemporaryDirectoryForReplaceObject(const char *originalObjectPath, char *temporaryDirectoryPath, UInt32 maxPathSize, OptionBits flags) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    RenameUnicode
    Change the name of an existing file or directory.  The new name is in
    Unicode.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory to be moved
    ->  nameLength      Number of Unicode characters in the new name
    ->  name            A pointer to the new Unicode name
    ->  textEncodingHint A suggested text encoding to use for the name
    <-  newRef          A new FSRef for the file or directory; may be NULL
    NOTE: Renaming an object may change its FSRef.  If you want to continue to
    refer to the object, you should pass a non-NULL pointer in newRef and use
    that returned FSRef to access the object after the rename.  The FSRef passed
    in "ref" may or may not be usable to access the object after it is renamed.
    "newRef" may point to the same storage as "ref".
*/
/*
 *  FSRenameUnicode()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSRenameUnicode(const FSRef *ref, UniCharCount nameLength, const UniChar *name, TextEncoding textEncodingHint, FSRef *newRef) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBRenameUnicodeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBRenameUnicodeSync(FSRefParam * paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBRenameUnicodeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBRenameUnicodeAsync(FSRefParam * paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    GetCatalogInfo
    Returns various information about a given file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory whose information is to be returned
    ->  whichInfo       Which catalog info fields to get
    <-  catInfo         The returned values of catalog info fields; may be NULL
    <-  spec            A pointer to the FSSpec for the object; may be NULL.  Ignored for 64 bit.
    <-  parentRef       A pointer to the FSRef for the object's parent directory; may be NULL
    <-  outName         The Unicode name is returned here.  This pointer may be NULL.
    Note: All of the outputs are optional; if you don't want that particular output, just
    set its pointer to NULL.  This is the call to use to map from an FSRef to an FSSpec.
*/
/*
 *  FSGetCatalogInfo()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSGetCatalogInfo(const FSRef *ref, FSCatalogInfoBitmap whichInfo, FSCatalogInfo *catalogInfo, HFSUniStr255 *outName, FSSpecPtr fsSpec, FSRef *parentRef) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetCatalogInfoSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBGetCatalogInfoSync(FSRefParam * paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetCatalogInfoAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBGetCatalogInfoAsync(FSRefParam * paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSSetCatalogInfo()
 *  
 *  Summary:
 *    Set catalog information about a given file or directory.
 *  
 *  Discussion:
 *    Set the catalog information for the file or directory represented
 *    by ref.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Parameters:
 *    
 *    ref:
 *      A pointer to an FSRef specifying the file or directory whose
 *      information is to be changed
 *    
 *    whichInfo:
 *      A bitmap specifying which catalog information fields may be
 *      set.  The settable fields are given by the constant
 *      kFSCatInfoSettableInfo; no other bits may be set in whichInfo. 
 *      The one exception to this is that processes running as eid 0 or
 *      euid 0  can change the user ID and group ID of a file or
 *      directory by setting kFSCatInfoSetOwnership in whichInfo and
 *      setting the permissions field of catalogInfo.
 *    
 *    catalogInfo:
 *      A pointer to the structure containing the new catalog
 *      information. Only some of the catalog information fields may be
 *      set.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSSetCatalogInfo(const FSRef *ref, FSCatalogInfoBitmap whichInfo, const FSCatalogInfo *catalogInfo)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBSetCatalogInfoSync()
 *  
 *  Summary:
 *    Set catalog information about a given file or directory.
 *  
 *  Discussion:
 *    Set the catalog information for the file or directory represented
 *    by ref.  See the description in FSSetCatalogInfo for which items
 *    may be set.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBSetCatalogInfoSync(FSRefParam * paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBSetCatalogInfoAsync()
 *  
 *  Summary:
 *    Set catalog information about a given file or directory.
 *  
 *  Discussion:
 *    Set the catalog information for the file or directory represented
 *    by ref.  See the description in FSSetCatalogInfo for which items
 *    may be set.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBSetCatalogInfoAsync(FSRefParam * paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    OpenIterator
    Creates an FSIterator to iterate over a directory or subtree.  The
    iterator can then be passed to GetCatalogInfoBulk or CatalogSearch.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    <-  iterator        The returned FSIterator
    ->  iteratorFlags   Controls whether the iterator iterates over subtrees
                        or just the immediate children of the container.
    ->  container       An FSRef for the directory to iterate (or root of
                        the subtree to iterate).
*/
/*
 *  FSOpenIterator()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSOpenIterator(const FSRef *container, FSIteratorFlags iteratorFlags, FSIterator *iterator) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBOpenIteratorSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBOpenIteratorSync(FSCatalogBulkParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBOpenIteratorAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBOpenIteratorAsync(FSCatalogBulkParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    CloseIterator
    Invalidates and disposes an FSIterator.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  iterator        The returned FSIterator
*/
/*
 *  FSCloseIterator()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSCloseIterator(FSIterator iterator)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCloseIteratorSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBCloseIteratorSync(FSCatalogBulkParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCloseIteratorAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBCloseIteratorAsync(FSCatalogBulkParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    GetCatalogInfoBulk
    Iterates over catalog objects and returns information about them.
    For now, iterator must have been created with kFSIterateFlat option.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  iterator        The iterator
    ->  maximumItems    The maximum number of items to return
    <-  actualItems     The actual number of items returned
    <-  containerChanged Set to true if the container's contents changed
    ->  whichInfo       The catalog information fields to return for each item
    <-  catalogInfo     An array of catalog information; one for each returned item
    <-  refs            An array of FSRefs; one for each returned item
    <-  specs           An array of FSSpecs; one for each returned item.  Ignored on 64 bit.
    <-  names           An array of filenames; one for each returned item
    Note: The catalogInfo, refs, specs, names, and containerChanged are all optional outputs;
    if you don't want that particular output, set its pointer to NULL.
*/
/*
 *  FSGetCatalogInfoBulk()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSGetCatalogInfoBulk(FSIterator iterator, ItemCount maximumObjects, ItemCount *actualObjects, Boolean *containerChanged, FSCatalogInfoBitmap whichInfo, FSCatalogInfo *catalogInfos, FSRef *refs, FSSpecPtr specs, HFSUniStr255 *names) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetCatalogInfoBulkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBGetCatalogInfoBulkSync(FSCatalogBulkParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetCatalogInfoBulkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBGetCatalogInfoBulkAsync(FSCatalogBulkParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    CatalogSearch
    Iterates over catalog objects, searching for objects that match given
    search criteria.  Returns various information about matching objects.
    For now, iterator must have been created with kFSIterateSubtree option
    and the container must have been the root directory of a volume.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  iterator        The iterator
    ->  maximumItems    The maximum number of items to return
    <-  actualItems     The actual number of items returned
    <-  containerChanged Set to true if the container's contents changed
    ->  whichInfo       The catalog information fields to return for each item
    <-  catalogInfo     An array of catalog information; one for each returned item
    <-  refs            An array of FSRefs; one for each returned item
    <-  specs           An array of FSSpecs; one for each returned item.  Ignored on 64 bit.
    <-  names           An array of filenames; one for each returned item
    ->  searchParams    The criteria that controls the matching, including timeout, a bitmap
                        controlling the fields to compare, and the (Unicode) name to compare.
    Note: The catalogInfo, refs, specs, and names are all optional outputs; if you don't want
    that particular output, set its pointer to NULL.
*/
/*
 *  FSCatalogSearch()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSCatalogSearch(FSIterator iterator, const FSSearchParams *searchCriteria, ItemCount maximumObjects, ItemCount *actualObjects, Boolean *containerChanged, FSCatalogInfoBitmap whichInfo, FSCatalogInfo *catalogInfos, FSRef *refs, FSSpecPtr specs, HFSUniStr255 *names) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCatalogSearchSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBCatalogSearchSync(FSCatalogBulkParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCatalogSearchAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBCatalogSearchAsync(FSCatalogBulkParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    CreateFileAndOpenForkUnicode
    Creates a new file and opens the specified fork.  The input filename is in Unicode.
    You can optionally set catalog info for the file.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  parentRef       The directory where the file is to be created
    ->  whichInfo       Which catalog info fields to set
    ->  catalogInfo     The values for catalog info fields to set; may be NULL
    ->  nameLength      Number of Unicode characters in the file's name
    ->  name            A pointer to the Unicode name
    ->  forkNameLength  The length of the fork name (in Unicode characters).
    ->  forkName        The name of the fork to open (in Unicode).  Passing NULL will open the data fork.
    ->  permissions     The access (read and/or write) you want
    <-  forkRefNum      The reference number for accessing the open fork
    <-  newRef          A pointer to the FSRef for the new file; may be NULL
*/
/*
 *  FSCreateFileAndOpenForkUnicode()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSCreateFileAndOpenForkUnicode(const FSRef *parentRef, UniCharCount nameLength, const UniChar *name, FSCatalogInfoBitmap whichInfo, const FSCatalogInfo *catalogInfo, UniCharCount forkNameLength, const UniChar *forkName, SInt8 permissions, FSIORefNum *forkRefNum, FSRef *newRef) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCreateFileAndOpenForkUnicodeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBCreateFileAndOpenForkUnicodeSync(FSRefForkIOParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCreateFileAndOpenForkUnicodeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern void  PBCreateFileAndOpenForkUnicodeAsync(FSRefForkIOParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    CreateFork
    Create a named fork for a file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory
    ->  forkNameLength  The length of the fork name (in Unicode characters)
    ->  forkName        The name of the fork to open (in Unicode)
*/
/*
 *  FSCreateFork()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSCreateFork(const FSRef *ref, UniCharCount forkNameLength, const UniChar *forkName)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCreateForkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBCreateForkSync(FSForkIOParam * paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCreateForkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBCreateForkAsync(FSForkIOParam * paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    DeleteFork
    Delete a named fork of a file or directory.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory
    ->  forkNameLength  The length of the fork name (in Unicode characters)
    ->  forkName        The name of the fork to open (in Unicode)
*/
/*
 *  FSDeleteFork()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSDeleteFork(const FSRef *ref, UniCharCount forkNameLength, const UniChar *forkName)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDeleteForkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBDeleteForkSync(FSForkIOParam * paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDeleteForkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBDeleteForkAsync(FSForkIOParam * paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    IterateForks
    Return the names and sizes of the forks of a file or directory.
    One fork is returned per call.
    ->  ioCompletion    A pointer to a completion routine.
    <-  ioResult        The result code of the function.
    ->  ref             The file or directory containing the forks.
    <-  positionOffset  The length of the fork, in bytes.
    <-  allocationAmount The space allocated to the fork (physical length).
    <-  outForkName     The name of the fork in Unicode.
    <>  forkIterator    Maintains state between calls for a given FSRef.
                        Before the first call, set the initialize field to zero.
*/
/*
 *  FSIterateForks()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSIterateForks(const FSRef *ref, CatPositionRec *forkIterator, HFSUniStr255 *forkName, SInt64 *forkSize, UInt64 *forkPhysicalSize) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBIterateForksSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBIterateForksSync(FSForkIOParam * paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBIterateForksAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBIterateForksAsync(FSForkIOParam * paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    OpenFork
    Open a fork for reading and/or writing.  Allows the opened fork
    to grow beyond 2GB in size.  All volumes should support data and
    resource forks.  Other named forks may be supported by some
    volumes.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The file or directory containing the fork to open
    ->  forkNameLength  The length of the fork name (in Unicode characters)
    ->  forkName        The name of the fork to open (in Unicode)
    ->  permissions     The access (read and/or write) you want
    <-  forkRefNum      The reference number for accessing the open fork
*/
/*
 *  FSOpenFork()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSOpenFork(const FSRef *ref, UniCharCount forkNameLength, const UniChar *forkName, SInt8 permissions, FSIORefNum *forkRefNum) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBOpenForkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBOpenForkSync(FSForkIOParam * paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBOpenForkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBOpenForkAsync(FSForkIOParam * paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    ReadFork
    Read data from a fork opened via OpenFork.  The first byte to read is
    indicated by a combination of positionMode and positionOffset.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to read from
    <-  buffer          Pointer to buffer where data will be returned
    ->  requestCount    The number of bytes to read
    <-  actualCount     The number of bytes actually read
    ->  positionMode    The base location for start of read
    ->  positionOffset  The offset from base location for start of read
*/
/*
 *  FSReadFork()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSReadFork(FSIORefNum forkRefNum, UInt16 positionMode, SInt64 positionOffset, ByteCount requestCount, void *buffer, ByteCount *actualCount) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBReadForkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBReadForkSync(FSForkIOParam * paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBReadForkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBReadForkAsync(FSForkIOParam * paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    WriteFork
    Write data to a fork opened via OpenFork.  The first byte to write is
    indicated by a combination of positionMode and positionOffset.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to write to
    ->  buffer          Pointer to data to write
    ->  requestCount    The number of bytes to write
    <-  actualCount     The number of bytes actually written
    ->  positionMode    The base location for start of write
    ->  positionOffset  The offset from base location for start of write
*/
/*
 *  FSWriteFork()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSWriteFork(FSIORefNum forkRefNum, UInt16 positionMode, SInt64 positionOffset, ByteCount requestCount, const void *buffer, ByteCount *actualCount) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBWriteForkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBWriteForkSync(FSForkIOParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBWriteForkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBWriteForkAsync(FSForkIOParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    GetForkPosition
    Get the current (default) position of a fork that was
    opened via OpenFork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    <-  positionOffset  The current position of the fork
*/
/*
 *  FSGetForkPosition()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSGetForkPosition(FSIORefNum forkRefNum, SInt64 *position)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetForkPositionSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBGetForkPositionSync(FSForkIOParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetForkPositionAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBGetForkPositionAsync(FSForkIOParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    SetForkPosition
    Set the current (default) position of a fork that was
    opened via OpenFork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    ->  positionMode    The base location for the new position
    ->  positionOffset  The offset of the new position from the base
*/
/*
 *  FSSetForkPosition()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSSetForkPosition(FSIORefNum forkRefNum, UInt16 positionMode, SInt64 positionOffset) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBSetForkPositionSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBSetForkPositionSync(FSForkIOParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBSetForkPositionAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBSetForkPositionAsync(FSForkIOParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    GetForkSize
    Get the current logical size (end-of-file) of an open fork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    <-  positionOffset  The logical size of the fork, in bytes
*/
/*
 *  FSGetForkSize()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSGetForkSize(FSIORefNum forkRefNum, SInt64 *forkSize)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetForkSizeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBGetForkSizeSync(FSForkIOParam * paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetForkSizeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBGetForkSizeAsync(FSForkIOParam * paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    SetForkSize
    Set the logical size (end-of-file) of an open fork.  This
    may cause space to be allocated or deallocated.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    ->  positionMode    The base location for the new size
    ->  positionOffset  The offset of the new size from the base
*/
/*
 *  FSSetForkSize()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSSetForkSize(FSIORefNum forkRefNum, UInt16 positionMode, SInt64 positionOffset) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBSetForkSizeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBSetForkSizeSync(FSForkIOParam * paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBSetForkSizeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBSetForkSizeAsync(FSForkIOParam * paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    AllocateFork
    Allocate space to an open fork.  Typically, the space to be
    allocated is beyond the current size of the fork, to reserve
    space so the file will be able to grow later.  Some volume
    formats are unable to allocate space beyond the logical size
    of the fork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork
    ->  positionMode    The base location for start of allocation
    ->  positionOffset  The offset of the start of allocation
    ->  allocationFlags Zero or more of the following flags:
        kFSAllocContiguousMask
                Any newly allocated space must be one contiguous piece.
        kFSAllocAllOrNothingMask
                All of the request space must be available, or the call
                will fail.  (If not set, the call may succeed even though
                some of the requested space wasn't allocated.)
        kFSAllocNoRoundUpMask
                Do not allocate additional space.  (If not set, a volume
                may allocate additional space in order to reduce fragmentation.)
    <>  allocationAmount    The number of bytes to allocate
                            On output, the number of bytes actually added
*/
/*
 *  FSAllocateFork()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSAllocateFork(FSIORefNum forkRefNum, FSAllocationFlags flags, UInt16 positionMode, SInt64 positionOffset, UInt64 requestCount, UInt64 *actualCount) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBAllocateForkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBAllocateForkSync(FSForkIOParam * paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBAllocateForkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBAllocateForkAsync(FSForkIOParam * paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    FlushFork
    Flush a fork.  Any data written to this fork refnum is flushed to the device.
    The volume's control structures are also flushed to the device.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to flush
*/
/*
 *  FSFlushFork()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSFlushFork(FSIORefNum forkRefNum)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBFlushForkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBFlushForkSync(FSForkIOParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBFlushForkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBFlushForkAsync(FSForkIOParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    CloseFork
    Flush and close a fork.  Any data written to this fork refnum is flushed
    to the device.  The volume's control structures are also flushed to the device.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to close
*/
/*
 *  FSCloseFork()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSCloseFork(FSIORefNum forkRefNum)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCloseForkSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBCloseForkSync(FSForkIOParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBCloseForkAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBCloseForkAsync(FSForkIOParam * paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    GetForkCBInfo
    Return information about an open fork.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    <>  desiredRefNum   If non-zero on input, then get information for this refnum;
                        unchanged on output.  If zero on input, iterate over all open
                        forks (possibly limited to a single volume); on output, contains
                        the fork's refnum.
    ->  volumeRefNum    Used when desiredRefNum is zero on input.  Set to 0 to iterate over all
                        volumes, or set to a FSVolumeRefNum to limit iteration to that volume.
    <>  iterator        Used when desiredRefNum is zero on input.  Set to 0 before iterating.
                        Pass the iterator returned by the previous call to continue iterating.
    <-  actualRefNum    The refnum of the open fork.
    <-  ref             The FSRef for the file or directory that contains the fork.
    <-  forkInfo        Various information about the open fork.
    <-  outForkName     The name of the fork
    Note: the foundRefNum, ref, forkInfo, and fork name outputs are all optional; if you don't want
    a particular output, then set its pointer to NULL.  If forkName is NULL, then forkNameLength
    will be undefined.
    Note: Returning the forkInfo generally does not require a disk access.  Returning the
    ref or forkName may cause disk access for some volume formats.
*/
/*
 *  FSGetForkCBInfo()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSGetForkCBInfo(FSIORefNum desiredRefNum, FSVolumeRefNum volume, short *iterator, FSIORefNum *actualRefNum, FSForkInfo *forkInfo, FSRef *ref, HFSUniStr255 *outForkName) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetForkCBInfoSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBGetForkCBInfoSync(FSForkCBInfoParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetForkCBInfoAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBGetForkCBInfoAsync(FSForkCBInfoParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    PBXLockRange
    Lock a range of bytes of the file fork specified by forkRefNum.
    This is only supported on some volume formats. 
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to read from
    ->  requestCount    The number of bytes to lock
    ->  positionMode    The base location for start of the range
    ->  positionOffset  The offset from base location for start of the range
    <-  rangeStart      Number of the first byte locked
*/
/*
 *  FSLockRange()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSLockRange(FSIORefNum forkRefNum, UInt16 positionMode, SInt64 positionOffset, UInt64 requestCount, UInt64 *rangeStart) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBXLockRangeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBXLockRangeSync(FSRangeLockParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBXLockRangeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBXLockRangeAsync(FSRangeLockParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    PBXUnlockRange
    Unlock a range of bytes of the file fork specified by forkRefNum.
    This is only supported on some volume formats. 
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  forkRefNum      The reference number of the fork to read from
    ->  requestCount    The number of bytes to lock
    ->  positionMode    The base location for start of the range
    ->  positionOffset  The offset from base location for start of the range
    <-  rangeStart      Number of the first byte unlocked
*/
/*
 *  FSUnlockRange()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSUnlockRange(FSIORefNum forkRefNum, UInt16 positionMode, SInt64 positionOffset, UInt64 requestCount, UInt64 *rangeStart) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBXUnlockRangeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBXUnlockRangeSync(FSRangeLockParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBXUnlockRangeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBXUnlockRangeAsync(FSRangeLockParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    GetVolumeInfo
    Returns various information about a given volume, or indexing over all volumes.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    <>  ioVRefNum       On input, the volume reference number or drive number whose
                        information is to be returned (if volumeIndex is 0); same
                        as "volume" input to FSGetVolumeInfo.
                        On output, the actual volume reference number; same as
                        "actualVolume" output of FSGetVolumeInfo.
    ->  volumeIndex     The index of the desired volume, or 0 to use ioVRefNum
    ->  whichInfo       Which volInfo info fields to get
    <-  volumeInfo      The returned values of Volume info fields; may be NULL
    <-  name            The Unicode name is returned here.  This pointer may be NULL.
    Note: All of the outputs are optional; if you don't want that particular output, just
    set its pointer to NULL.
*/
/*
 *  FSGetVolumeInfo()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSGetVolumeInfo(FSVolumeRefNum volume, ItemCount volumeIndex, FSVolumeRefNum *actualVolume, FSVolumeInfoBitmap whichInfo, FSVolumeInfo *info, HFSUniStr255 *volumeName, FSRef *rootDirectory) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetVolumeInfoSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBGetVolumeInfoSync(FSVolumeInfoParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBGetVolumeInfoAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBGetVolumeInfoAsync(FSVolumeInfoParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    SetVolumeInfo
    Set information about a given volume.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ioVRefNum       The volume whose information is to be changed
    ->  whichInfo       Which catalog info fields to set
    ->  volumeInfo      The new values of volume info fields
    Note: Only some of the volume info fields may be set.  The settable fields
    are given by the constant kFSVolInfoSettableInfo; no other bits may be set in
    whichInfo.
*/
/*
 *  FSSetVolumeInfo()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSSetVolumeInfo(FSVolumeRefNum volume, FSVolumeInfoBitmap whichInfo, const FSVolumeInfo *info)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBSetVolumeInfoSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBSetVolumeInfoSync(FSVolumeInfoParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBSetVolumeInfoAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBSetVolumeInfoAsync(FSVolumeInfoParam * paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    FSGetDataForkName
    Returns the constant for the name of the data fork (the empty string)
*/
/*
 *  FSGetDataForkName()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSGetDataForkName(HFSUniStr255 * dataForkName)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    FSGetResourceForkName
    Returns the constant for the name of the resource fork
    (currently "RESOURCE_FORK").
*/
/*
 *  FSGetResourceForkName()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSGetResourceForkName(HFSUniStr255 * resourceForkName) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSRefMakePath()
 *  
 *  Summary:
 *    converts an FSRef to a POSIX path
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Parameters:
 *    
 *    ref:
 *      the file/dir to get the POSIX path for
 *    
 *    path:
 *      a pointer to a buffer which FSRefMakePath will fill with a UTF8
 *      encoded C string representing the path the the specified FSRef
 *    
 *    pathBufferSize:
 *      the size of the path buffer in bytes.  This size should include
 *      the NULL terminator for the returned path string.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSRefMakePath(const FSRef *ref, UInt8 *path, UInt32 pathBufferSize)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathMakeRef()
 *  
 *  Summary:
 *    converts a POSIX path to an FSRef
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Parameters:
 *    
 *    path:
 *      a pointer to a UTF8 encoded C String that is a POSIX path
 *    
 *    ref:
 *      a pointer to an FSRef to fill in
 *    
 *    isDirectory:
 *      an optional pointer to a Boolean that will be filled in with
 *      whether the specified path is a directory (vs. a file)
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathMakeRef(const UInt8 *path, FSRef *ref, Boolean *isDirectory)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSPathMakeRefOptions
 *  
 *  Discussion:
 *    Options that can be passed to the FSPathMakeRefWithOptions call.
 */
enum {
  kFSPathMakeRefDefaultOptions  = 0,
  kFSPathMakeRefDoNotFollowLeafSymlink = 0x01
};

/*
 *  FSPathMakeRefWithOptions()
 *  
 *  Summary:
 *    converts a POSIX path to an FSRef allowing for options other than
 *    the default
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    path:
 *      a pointer to a UTF8 encoded C String that is a POSIX path
 *    
 *    options:
 *      a set of FSPathMakeRef options
 *    
 *    ref:
 *      a pointer to an FSRef to fill in
 *    
 *    isDirectory:
 *      an optional pointer to a Boolean that will be filled in with
 *      whether the specified path is a directory (vs. a file)
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathMakeRefWithOptions(const UInt8 *path, OptionBits options, FSRef *ref, Boolean *isDirectory) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSIsFSRefValid()
 *  
 *  Summary:
 *    Returns true if ref refers to an existing valid file system
 *    object, false otherwise.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    ref:
 *      FSRef to test
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean  FSIsFSRefValid(const FSRef * ref)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FNMessage
 *  
 *  Discussion:
 *    Messages broadcast about a directory.  If system clients (such as
 *    the Finder) are interested in changes to a directory, they will
 *    receive notifications when application code broadcasts change
 *    messages about that directory.
 */
typedef UInt32 FNMessage;
enum {
  kFNDirectoryModifiedMessage   = 1
};

/*
 *  FNNotify()
 *  
 *  Summary:
 *    Broadcasts notification of changes to the specified directory.
 *  
 *  Discussion:
 *    FNNotify is used to notify system clients (such as the Finder) of
 *    modifications to the contents of a directory, specifically
 *    addition or removal of files or folders from the directory. The
 *    Finder and other system clients will refresh their views of the
 *    specified directory when they receive the change notification.
 *    FNNotify is not meant to notify the Finder of changes to a
 *    specific file (for example, changes to a file's type or creator);
 *    for that purpose, use a kAESync AppleEvent sent to the Finder.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Parameters:
 *    
 *    ref:
 *      The directory for which to broadcast the notification
 *    
 *    message:
 *      An indication of what happened to the target directory
 *    
 *    flags:
 *      Options about delivery of the notification (specify kNilOptions
 *      for default behaviour)
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FNNotify(const FSRef *ref, FNMessage message, OptionBits flags)                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FNNotifyByPath()
 *  
 *  Summary:
 *    Broadcasts notification of changes to the specified directory.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Parameters:
 *    
 *    path:
 *      Path to the directory for which to broadcast the notification
 *    
 *    message:
 *      An indication of what happened to the target directory
 *    
 *    flags:
 *      Options about delivery of the notification (specify kNilOptions
 *      for default behaviour)
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FNNotifyByPath(const UInt8 *path, FNMessage message, OptionBits flags)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FNNotifyAll()
 *  
 *  Discussion:
 *    Broadcasts notification of changes to the filesystem (should only
 *    be used by installers or programs which make lots of changes and
 *    only send one broadcast).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Parameters:
 *    
 *    message:
 *      An indication of what happened
 *    
 *    flags:
 *      Options about delivery of the notification (specify kNilOptions
 *      for default behaviour)
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FNNotifyAll(FNMessage message, OptionBits flags)                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);




/*
 *  FNSubscriptionRef
 *  
 *  Discussion:
 *    A token associated with a notification subscription.  The
 *    subscriber gets one back when they subscribe to notifications for
 *    a particular directory, and they supply it when the unsubscribe. 
 *    It is also delivered along with the notifications for that
 *    subscription.
 */
typedef struct OpaqueFNSubscriptionRef*  FNSubscriptionRef;

/*
 *  Discussion:
 *    Options that can be specified at subscription time.
 */
enum {

  /*
   * Specify this option if you do not want to receive notifications on
   * this subscription when FNNotifyAll is called; by default any
   * subscription is also implicitly a subscription to wildcard
   * notifications
   */
  kFNNoImplicitAllSubscription  = (1 << 0),

  /*
   * Specify this option if you want to receive notifications on this
   * subscription when your application is in background.  By default
   * notifications will be coalesced and and delivered when your
   * application becomes foreground.
   */
  kFNNotifyInBackground         = (1 << 1)
};


/*
 *  FNSubscriptionProcPtr
 *  
 *  Discussion:
 *    Callback delivered for directory notifications.
 *  
 *  Parameters:
 *    
 *    message:
 *      An indication of what happened
 *    
 *    flags:
 *      Options about delivery of the notification (typically
 *      kNilOptions)
 *    
 *    refcon:
 *      User reference supplied with subscription
 *    
 *    subscription:
 *      Subscription corresponding to this notification
 */
typedef CALLBACK_API_C( void , FNSubscriptionProcPtr )(FNMessage message, OptionBits flags, void *refcon, FNSubscriptionRef subscription);
typedef STACK_UPP_TYPE(FNSubscriptionProcPtr)                   FNSubscriptionUPP;
/*
 *  NewFNSubscriptionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern FNSubscriptionUPP
NewFNSubscriptionUPP(FNSubscriptionProcPtr userRoutine)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeFNSubscriptionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeFNSubscriptionUPP(FNSubscriptionUPP userUPP)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeFNSubscriptionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeFNSubscriptionUPP(
  FNMessage          message,
  OptionBits         flags,
  void *             refcon,
  FNSubscriptionRef  subscription,
  FNSubscriptionUPP  userUPP)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline FNSubscriptionUPP                                    NewFNSubscriptionUPP(FNSubscriptionProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeFNSubscriptionUPP(FNSubscriptionUPP) { }
    inline void                                                 InvokeFNSubscriptionUPP(FNMessage message, OptionBits flags, void * refcon, FNSubscriptionRef subscription, FNSubscriptionUPP userUPP) { (*userUPP)(message, flags, refcon, subscription); }
  #else
    #define NewFNSubscriptionUPP(userRoutine)                   ((FNSubscriptionUPP)userRoutine)
    #define DisposeFNSubscriptionUPP(userUPP)
    #define InvokeFNSubscriptionUPP(message, flags, refcon, subscription, userUPP) (*userUPP)(message, flags, refcon, subscription)
  #endif
#endif

/*
 *  FNSubscribe()
 *  
 *  Summary:
 *    Subscribe to change notifications for the specified directory.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.1
 *  
 *  Parameters:
 *    
 *    directoryRef:
 *      Directory for which the caller wants notifications
 *    
 *    callback:
 *      Function to call back when a notification arrives
 *    
 *    refcon:
 *      User state carried with the subscription
 *    
 *    flags:
 *      Options for future use (specify kNilOptions, or one of the
 *      FNSubscriptionOptions)
 *    
 *    subscription:
 *      Subscription token for subsequent query or unsubscription
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FNSubscribe(const FSRef *directoryRef, FNSubscriptionUPP callback, void *refcon, OptionBits flags, FNSubscriptionRef *subscription) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FNSubscribeByPath()
 *  
 *  Summary:
 *    Subscribe to change notifications for the specified directory.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.1
 *  
 *  Parameters:
 *    
 *    directoryPath:
 *      Directory for which the caller wants notifications
 *    
 *    callback:
 *      Function to call back when a notification arrives
 *    
 *    refcon:
 *      User state carried with the subscription
 *    
 *    flags:
 *      Options for future use (specify kNilOptions, or one of the
 *      FNSubscriptionOptions)
 *    
 *    subscription:
 *      Subscription token for subsequent query or unsubscription
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FNSubscribeByPath(const UInt8 *directoryPath, FNSubscriptionUPP callback, void *refcon, OptionBits flags, FNSubscriptionRef *subscription) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FNUnsubscribe()
 *  
 *  Summary:
 *    Release a subscription which is no longer needed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.1
 *  
 *  Parameters:
 *    
 *    subscription:
 *      Subscription previously returned from FNSubscribe or
 *      FNSubscribeForPath
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FNUnsubscribe(FNSubscriptionRef subscription) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FNGetDirectoryForSubscription()
 *  
 *  Summary:
 *    Fetch the directory for which this subscription was originally
 *    entered. There is no path variant because paths are fragile, and
 *    the path may have changed.  If the caller does not care about
 *    this subtlety, she can call FSRefMakePath to get a path from the
 *    returned ref.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.1
 *  
 *  Parameters:
 *    
 *    subscription:
 *      Subscription previously returned from FNSubscribe or
 *      FNSubscribeForPath
 *    
 *    ref:
 *      Directory for which this subscription was created
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FNGetDirectoryForSubscription(FNSubscriptionRef subscription, FSRef *ref) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/* Async Volume Operation Status return values*/
enum {
  kAsyncMountInProgress         = 1,
  kAsyncMountComplete           = 2,
  kAsyncUnmountInProgress       = 3,
  kAsyncUnmountComplete         = 4,
  kAsyncEjectInProgress         = 5,
  kAsyncEjectComplete           = 6
};


typedef UInt32                          FSMountStatus;
typedef UInt32                          FSEjectStatus;
typedef UInt32                          FSUnmountStatus;
typedef struct OpaqueFSVolumeOperation*  FSVolumeOperation;
typedef CALLBACK_API_C( void , FSVolumeMountProcPtr )(FSVolumeOperation volumeOp, void *clientData, OSStatus err, FSVolumeRefNum mountedVolumeRefNum);
typedef CALLBACK_API_C( void , FSVolumeUnmountProcPtr )(FSVolumeOperation volumeOp, void *clientData, OSStatus err, FSVolumeRefNum volumeRefNum, pid_t dissenter);
typedef CALLBACK_API_C( void , FSVolumeEjectProcPtr )(FSVolumeOperation volumeOp, void *clientData, OSStatus err, FSVolumeRefNum volumeRefNum, pid_t dissenter);
typedef STACK_UPP_TYPE(FSVolumeMountProcPtr)                    FSVolumeMountUPP;
typedef STACK_UPP_TYPE(FSVolumeUnmountProcPtr)                  FSVolumeUnmountUPP;
typedef STACK_UPP_TYPE(FSVolumeEjectProcPtr)                    FSVolumeEjectUPP;
/*
 *  NewFSVolumeMountUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern FSVolumeMountUPP
NewFSVolumeMountUPP(FSVolumeMountProcPtr userRoutine)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  NewFSVolumeUnmountUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern FSVolumeUnmountUPP
NewFSVolumeUnmountUPP(FSVolumeUnmountProcPtr userRoutine)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  NewFSVolumeEjectUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern FSVolumeEjectUPP
NewFSVolumeEjectUPP(FSVolumeEjectProcPtr userRoutine)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeFSVolumeMountUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeFSVolumeMountUPP(FSVolumeMountUPP userUPP)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeFSVolumeUnmountUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeFSVolumeUnmountUPP(FSVolumeUnmountUPP userUPP)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeFSVolumeEjectUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeFSVolumeEjectUPP(FSVolumeEjectUPP userUPP)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeFSVolumeMountUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeFSVolumeMountUPP(
  FSVolumeOperation  volumeOp,
  void *             clientData,
  OSStatus           err,
  FSVolumeRefNum     mountedVolumeRefNum,
  FSVolumeMountUPP   userUPP)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeFSVolumeUnmountUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeFSVolumeUnmountUPP(
  FSVolumeOperation   volumeOp,
  void *              clientData,
  OSStatus            err,
  FSVolumeRefNum      volumeRefNum,
  pid_t               dissenter,
  FSVolumeUnmountUPP  userUPP)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeFSVolumeEjectUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeFSVolumeEjectUPP(
  FSVolumeOperation  volumeOp,
  void *             clientData,
  OSStatus           err,
  FSVolumeRefNum     volumeRefNum,
  pid_t              dissenter,
  FSVolumeEjectUPP   userUPP)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline FSVolumeMountUPP                                     NewFSVolumeMountUPP(FSVolumeMountProcPtr userRoutine) { return userRoutine; }
    inline FSVolumeUnmountUPP                                   NewFSVolumeUnmountUPP(FSVolumeUnmountProcPtr userRoutine) { return userRoutine; }
    inline FSVolumeEjectUPP                                     NewFSVolumeEjectUPP(FSVolumeEjectProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeFSVolumeMountUPP(FSVolumeMountUPP) { }
    inline void                                                 DisposeFSVolumeUnmountUPP(FSVolumeUnmountUPP) { }
    inline void                                                 DisposeFSVolumeEjectUPP(FSVolumeEjectUPP) { }
    inline void                                                 InvokeFSVolumeMountUPP(FSVolumeOperation volumeOp, void * clientData, OSStatus err, FSVolumeRefNum mountedVolumeRefNum, FSVolumeMountUPP userUPP) { (*userUPP)(volumeOp, clientData, err, mountedVolumeRefNum); }
    inline void                                                 InvokeFSVolumeUnmountUPP(FSVolumeOperation volumeOp, void * clientData, OSStatus err, FSVolumeRefNum volumeRefNum, pid_t dissenter, FSVolumeUnmountUPP userUPP) { (*userUPP)(volumeOp, clientData, err, volumeRefNum, dissenter); }
    inline void                                                 InvokeFSVolumeEjectUPP(FSVolumeOperation volumeOp, void * clientData, OSStatus err, FSVolumeRefNum volumeRefNum, pid_t dissenter, FSVolumeEjectUPP userUPP) { (*userUPP)(volumeOp, clientData, err, volumeRefNum, dissenter); }
  #else
    #define NewFSVolumeMountUPP(userRoutine)                    ((FSVolumeMountUPP)userRoutine)
    #define NewFSVolumeUnmountUPP(userRoutine)                  ((FSVolumeUnmountUPP)userRoutine)
    #define NewFSVolumeEjectUPP(userRoutine)                    ((FSVolumeEjectUPP)userRoutine)
    #define DisposeFSVolumeMountUPP(userUPP)
    #define DisposeFSVolumeUnmountUPP(userUPP)
    #define DisposeFSVolumeEjectUPP(userUPP)
    #define InvokeFSVolumeMountUPP(volumeOp, clientData, err, mountedVolumeRefNum, userUPP) (*userUPP)(volumeOp, clientData, err, mountedVolumeRefNum)
    #define InvokeFSVolumeUnmountUPP(volumeOp, clientData, err, volumeRefNum, dissenter, userUPP) (*userUPP)(volumeOp, clientData, err, volumeRefNum, dissenter)
    #define InvokeFSVolumeEjectUPP(volumeOp, clientData, err, volumeRefNum, dissenter, userUPP) (*userUPP)(volumeOp, clientData, err, volumeRefNum, dissenter)
  #endif
#endif


/*
 *  Discussion:
 *    Options that can be passed to the FSMountServerVolumeCalls. These
 *    options are not for use with the local volumes.
 */
enum {

  /*
   * Specify this option if you do want the volume displayed as a stand
   * along volume in the UI.
   */
  kFSMountServerMarkDoNotDisplay = (1 << 0),

  /*
   * Specify this option if you want the volume mounted on the mountdir
   * passed in instead of in it.
   */
  kFSMountServerMountOnMountDir = (1 << 2),

  /*
   * Specify this option if you want to suppress connection-time UI
   * when mounting the server volume.
   */
  kFSMountServerSuppressConnectionUI = (1 << 6)
};


/*
 *  Discussion:
 *    Deprecated options that can be passed to
 *    FSMountServerVolumeCalls.  The options will have no effect on the
 *    OS version they are noted as deprecated on or later.
 */
enum {

  /*
   * Specify this option if you do not want other processes notified
   * that this volume has been mounted.  Deprecated on Mac OS X 10.5. 
   * Use kFSMountServerMarkDoNotDisplay instead.
   */
  kFSMountServerMountWithoutNotification = (1 << 1)
};


/*
 *  Discussion:
 *    Options that can be passed to the FSEjectVolume calls.
 */
enum {

  /*
   * Specify this option if you want the volume forcibly ejected. 
   * Force ejecting a volume will very likely result in data loss since
   * the volume will be ejected even if there are open files on it or
   * other volumes that share the same device. This option should be
   * reserved for situations such as the backing store for a volume is
   * gone (so the data is lost regardless).
   */
  kFSEjectVolumeForceEject      = (1 << 0)
};


/*
 *  Discussion:
 *    Options that can be passed to the FSUnmountVolume calls.
 */
enum {

  /*
   * Specify this option if you want the volume forcibly unmounted. 
   * Force unmounting a volume will very likely result in data loss
   * since the volume will be ejected even if there are open files on
   * it. This option should be reserved for situations such as the
   * backing store for a volume is gone (so the data is lost
   * regardless).
   */
  kFSUnmountVolumeForceUnmount  = (1 << 0)
};

/*
 *  FSCreateVolumeOperation()
 *  
 *  Discussion:
 *    This routine will return an FSVolumeOperation which can be used
 *    for an async volume operation.  When the operation is completed
 *    the volumeOp should be disposed of to free the memory associated
 *    with the operation using FSDisposeVolumeOperation.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    volumeOp:
 *      The new FSVolumeOperation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSCreateVolumeOperation(FSVolumeOperation * volumeOp) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSDisposeVolumeOperation()
 *  
 *  Discussion:
 *    This routine will release the memory associated with the passed
 *    in volumeOp. It will return paramErr is the volumeOp is in use.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    volumeOp:
 *      The FSVolumeOperation to release.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSDisposeVolumeOperation(FSVolumeOperation volumeOp) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMountLocalVolumeSync()
 *  
 *  Discussion:
 *    This routine will mount the disk specified by diskID at mountDir
 *    (or the default location if mountDir is NULL).  This routine
 *    returns after the mount is complete.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    diskID:
 *      The disk to mount.
 *    
 *    mountDir:
 *      Pass in NULL (currently only NULL is supported).
 *    
 *    mountedVolumeRefNum:
 *      The volume ref num of the newly mounted volume.
 *    
 *    flags:
 *      Options for future use.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSMountLocalVolumeSync(CFStringRef diskID, CFURLRef mountDir, FSVolumeRefNum *mountedVolumeRefNum, OptionBits flags) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMountLocalVolumeAsync()
 *  
 *  Discussion:
 *    This routine will start the process to disk specified by diskID
 *    at mountDir (or the default location if mountDir is NULL).  If a
 *    callback is provided the provided function will be called when
 *    the mount operation is complete.  Once this routine returns noErr
 *    the status of the operation can be found using
 *    FSGetAsyncMountStatus.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    diskID:
 *      The disk to mount.
 *    
 *    mountDir:
 *      Pass in NULL (currently only NULL is supported).
 *    
 *    volumeOp:
 *      An FSVolumeOperation returned by FSCreateVolumeOperation
 *    
 *    clientData:
 *      client data associated with the operation.
 *    
 *    flags:
 *      Options for future use.
 *    
 *    callback:
 *      Function to call when mount is complete.
 *    
 *    runloop:
 *      Runloop to run on.
 *    
 *    runloopMode:
 *      Mode for runloop.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSMountLocalVolumeAsync(CFStringRef diskID, CFURLRef mountDir, FSVolumeOperation volumeOp, void *clientData, OptionBits flags, FSVolumeMountUPP callback, CFRunLoopRef runloop, CFStringRef runloopMode) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMountServerVolumeSync()
 *  
 *  Discussion:
 *    This routine will mount the server specified by url at mountDir
 *    (or the default location if mountDir is NULL).  An optional user
 *    and password can be passed in for authentication. If no user or
 *    password is provided then the underlying file system will handle
 *    authentication if required.  This routine returns after the mount
 *    is complete.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    url:
 *      The server to mount.
 *    
 *    mountDir:
 *      The directory to mount the server to (default if NULL).
 *    
 *    user:
 *      String to pass as user for authentication.
 *    
 *    password:
 *      String to pass as password for authenticated log in.
 *    
 *    mountedVolumeRefNum:
 *      The volume ref num of the newly mounted volume.
 *    
 *    flags:
 *      Options (such as kFSMountServerMarkDoNotDisplay and
 *      kFSMountServerMountOnMountDir).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSMountServerVolumeSync(CFURLRef url, CFURLRef mountDir, CFStringRef user, CFStringRef password, FSVolumeRefNum *mountedVolumeRefNum, OptionBits flags) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMountServerVolumeAsync()
 *  
 *  Discussion:
 *    This routine will start the process to mount the server specified
 *    by url at mountDir (or the default location if mountDir is NULL).
 *     An optional user and password can be passed in for
 *    authentication. If no user or password is provided then the
 *    underlying file system will handle authentication if required. 
 *    If a callback is provided the provided function will be called
 *    when the mount operation is complete.  Once this routine returns
 *    noErr the status of the operation can be found using
 *    FSGetAsyncMountStatus.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    url:
 *      The server to mount.
 *    
 *    mountDir:
 *      The directory to mount the server to (default if NULL).
 *    
 *    user:
 *      String to pass as user for authentication.
 *    
 *    password:
 *      String to pass as password for authenticated log in.
 *    
 *    volumeOp:
 *      An FSVolumeOperation returned by FSCreateVolumeOperation
 *    
 *    clientData:
 *      client data associated with the operation.
 *    
 *    flags:
 *      Options (such as kFSMountServerMarkDoNotDisplay and
 *      kFSMountServerMountOnMountDir).
 *    
 *    callback:
 *      Function to call when mount is complete.
 *    
 *    runloop:
 *      Runloop run on.
 *    
 *    runloopMode:
 *      Mode for runloop.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSMountServerVolumeAsync(CFURLRef url, CFURLRef mountDir, CFStringRef user, CFStringRef password, FSVolumeOperation volumeOp, void *clientData, OptionBits flags, FSVolumeMountUPP callback, CFRunLoopRef runloop, CFStringRef runloopMode) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSGetAsyncMountStatus()
 *  
 *  Discussion:
 *    This routine returns the current status of an asynchronous mount
 *    operation. A return value of noErr signifies that the status
 *    parameter has been filled with valid information.  If the status
 *    is kAsyncMountComplete then the rest of data returned is valid. 
 *    If the status is anything else then the volumeOpStatus and
 *    mountedVolumeRefNum parameters are invalid (The clientData will
 *    be ok).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    volumeOp:
 *      The async volume operation to get status about.
 *    
 *    status:
 *      The status of the operation.
 *    
 *    volumeOpStatus:
 *      If status is kAsyncMountComplete then this contains the
 *      OSStatus for the operation.
 *    
 *    mountedVolumeRefNum:
 *      If status is kAsyncMountComplete and volumeOpStatus is noErr
 *      then this is the volume ref num for the newly mounted volume.
 *    
 *    clientData:
 *      client data associated with the original
 *      FSMountServerVolumeAsync operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetAsyncMountStatus(FSVolumeOperation volumeOp, FSMountStatus *status, OSStatus *volumeOpStatus, FSVolumeRefNum *mountedVolumeRefNum, void **clientData) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  FSUnmountVolumeSync()
 *  
 *  Discussion:
 *    This routine unmounts the volume specified by vRefNum.  If the
 *    volume cannot be unmounted the pid of the process which denied
 *    the unmount will be returned in the dissenter parameter.  This
 *    routine returns after the unmount is complete.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    vRefNum:
 *      The volume reference number of the volume to unmount.
 *    
 *    flags:
 *      Options (such as kFSUnmountVolumeForceUnmount).
 *    
 *    dissenter:
 *      pid of the process which denied the unmount if the unmount is
 *      denied.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSUnmountVolumeSync(FSVolumeRefNum vRefNum, OptionBits flags, pid_t *dissenter) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSUnmountVolumeAsync()
 *  
 *  Discussion:
 *    This routine starts the process of unmounting the volume
 *    specified by vRefNum. If a callback is provided the provided
 *    function will be called when the unmount operation is complete. 
 *    Once this routine returns noErr the status of the operation can
 *    be found using FSGetAsyncUnmountStatus.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    vRefNum:
 *      The volume reference number of the volume to unmount.
 *    
 *    flags:
 *      Options (such as kFSUnmountVolumeForceUnmount).
 *    
 *    volumeOp:
 *      An FSVolumeOperation returned by FSCreateVolumeOperation
 *    
 *    clientData:
 *      client data associated with the operation.
 *    
 *    callback:
 *      Function to call when unmount is complete.
 *    
 *    runloop:
 *      Runloop to run on.
 *    
 *    runloopMode:
 *      Mode for runloop.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSUnmountVolumeAsync(FSVolumeRefNum vRefNum, OptionBits flags, FSVolumeOperation volumeOp, void *clientData, FSVolumeUnmountUPP callback, CFRunLoopRef runloop, CFStringRef runloopMode) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSGetAsyncUnmountStatus()
 *  
 *  Discussion:
 *    This routine returns the current status of an asynchronous
 *    unmount operation. A return value of noErr signifies that the
 *    status parameter has been filled with valid information.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    volumeOp:
 *      The async volume operation to get status about.
 *    
 *    status:
 *      The status of the operation.
 *    
 *    volumeOpStatus:
 *      If status is kAsyncUnmountComplete then this contains the
 *      OSStatus for the operation.
 *    
 *    volumeRefNum:
 *      volume reference number of volume being unmounted.
 *    
 *    dissenter:
 *      pid of the process which denied the unmount if the unmount is
 *      denied.
 *    
 *    clientData:
 *      client data associated with the original
 *      FSMountServerVolumeAsync operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetAsyncUnmountStatus(FSVolumeOperation volumeOp, FSUnmountStatus *status, OSStatus *volumeOpStatus, FSVolumeRefNum *volumeRefNum, pid_t *dissenter, void **clientData) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSCancelVolumeOperation()
 *  
 *  Discussion:
 *    This routine will cancel and outstanding asynchronous volume
 *    mounting operation. It currently is only supported for server
 *    mounts.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    volumeOp:
 *      The async volume operation to cancel.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSCancelVolumeOperation(FSVolumeOperation volumeOp) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSEjectVolumeSync()
 *  
 *  Discussion:
 *    This routine ejects the volume specified by vRefNum.  If the
 *    volume cannot be ejected the pid of the process which denied the
 *    unmount will be returned in the dissenter parameter.  This
 *    routine returns after the eject is complete.  Ejecting a volume
 *    will result in the unmounting of other volumes on the same device.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    vRefNum:
 *      The volume reference number of the volume to eject.
 *    
 *    flags:
 *      Options (such as kFSEjectVolumeForceEject).
 *    
 *    dissenter:
 *      pid of the process which denied the unmount if the eject is
 *      denied.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSEjectVolumeSync(FSVolumeRefNum vRefNum, OptionBits flags, pid_t *dissenter) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSEjectVolumeAsync()
 *  
 *  Discussion:
 *    This routine starts the process of ejecting the volume specified
 *    by vRefNum. If a callback is provided the provided function will
 *    be called when the eject operation is complete.  Once this
 *    routine returns noErr the status of the operation can be found
 *    using FSGetAsyncEjectStatus.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    vRefNum:
 *      The volume reference number of the volume to eject.
 *    
 *    flags:
 *      Options (such as kFSEjectVolumeForceEject).
 *    
 *    volumeOp:
 *      An FSVolumeOperation returned by FSCreateVolumeOperation
 *    
 *    clientData:
 *      client data associated with the operation.
 *    
 *    callback:
 *      Function to call when eject is complete.
 *    
 *    runloop:
 *      Runloop to run on.
 *    
 *    runloopMode:
 *      Mode for runloop.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSEjectVolumeAsync(FSVolumeRefNum vRefNum, OptionBits flags, FSVolumeOperation volumeOp, void *clientData, FSVolumeEjectUPP callback, CFRunLoopRef runloop, CFStringRef runloopMode) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSGetAsyncEjectStatus()
 *  
 *  Discussion:
 *    This routine returns the current status of an asynchronous eject
 *    operation. A return value of noErr signifies that the status
 *    parameter has been filled with valid information.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    volumeOp:
 *      The async volume operation to get status about.
 *    
 *    status:
 *      The status of the operation.
 *    
 *    volumeOpStatus:
 *      If status is kAsyncEjectComplete then this contains the
 *      OSStatus for the operation.
 *    
 *    volumeRefNum:
 *      volume reference number of volume being ejected.
 *    
 *    dissenter:
 *      pid of the process which denied the unmount if the eject is
 *      denied.
 *    
 *    clientData:
 *      client data associated with the original
 *      FSMountServerVolumeAsync operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetAsyncEjectStatus(FSVolumeOperation volumeOp, FSEjectStatus *status, OSStatus *volumeOpStatus, FSVolumeRefNum *volumeRefNum, pid_t *dissenter, void **clientData) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSCopyDiskIDForVolume()
 *  
 *  Discussion:
 *    This routine returns a copy of the diskID for the passed in
 *    volume.  The caller is responsible for releasing the CFString
 *    later.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    vRefNum:
 *      FSVolumeRefNum of the target volume.
 *    
 *    diskID:
 *      The diskID string associated with the target volume.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSCopyDiskIDForVolume(FSVolumeRefNum vRefNum, CFStringRef *diskID)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_2, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSCopyURLForVolume()
 *  
 *  Discussion:
 *    This routine returns a copy of the url for the passed in volume. 
 *    The caller is responsible for releasing the CFURL later.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    vRefNum:
 *      FSVolumeRefNum of the target volume.
 *    
 *    url:
 *      The url associated with the target volume.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSCopyURLForVolume(FSVolumeRefNum vRefNum, CFURLRef *url)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_3, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSGetVolumeForDiskID()
 *  
 *  Discussion:
 *    This routine returnes the FSVolumeRefNum for a given diskID.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    diskID:
 *      The diskID string associated with the target volume.
 *    
 *    vRefNum:
 *      FSVolumeRefNum of the volume which corresponds to the diskID.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetVolumeForDiskID(CFStringRef diskID, FSVolumeRefNum *vRefNum)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSCopyDADiskForVolume()
 *  
 *  Discussion:
 *    This routine returns a copy of the diskID for the passed in
 *    volume.  The caller is responsible for releasing the DADisk
 *    later.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    vRefNum:
 *      FSVolumeRefNum of the target volume.
 *    
 *    disk:
 *      The DADisk associated with the target volume.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSCopyDADiskForVolume(FSVolumeRefNum vRefNum, DADiskRef *disk)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSGetVolumeForDADisk()
 *  
 *  Discussion:
 *    This routine returns the FSVolumeRefNum associated with the
 *    volume referenced by the passed in DADiskRef.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    disk:
 *      The DADiskRef of the target volume.
 *    
 *    vRefNum:
 *      FSVolumeRefNum of the volume which corresponds to the DADiskRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetVolumeForDADisk(DADiskRef disk, FSVolumeRefNum *vRefNum)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* File Operation routines*/

typedef struct __FSFileOperation*       FSFileOperationRef;
typedef UInt32                          FSFileOperationStage;

/*
 *  FSFileOperationClientContext
 *  
 *  Discussion:
 *    Structure containing the user-defined data and callbacks for
 *    FSFileOperation client contextual data.
 */
struct FSFileOperationClientContext {

  /*
   * The version number of the structure type being passed in as a
   * parameter to FSCopyObjectAsync or FSMoveObjectAsync function.
   * Valid version number is currently 0.
   */
  CFIndex             version;

  /*
   * An arbitrary pointer to client-defined data, which can be
   * associated with the operation and is passed to the callbacks.
   */
  void *              info;

  /*
   * The callback used to add a retain for the operation on the info
   * pointer for the life of the operation, and may be used for
   * temporary references the operation needs to take. This callback
   * returns the actual info pointer to store in the operation, almost
   * always just the pointer passed as the parameter.
   */
  CFAllocatorRetainCallBack  retain;

  /*
   * The callback used to remove a retain previously added for the
   * operation on the info pointer.
   */
  CFAllocatorReleaseCallBack  release;

  /*
   * The callback used to create a descriptive string representation of
   * the info pointer (or the data pointed to by the info pointer) for
   * debugging purposes. This is used by the CFCopyDescription()
   * function.
   */
  CFAllocatorCopyDescriptionCallBack  copyDescription;
};
typedef struct FSFileOperationClientContext FSFileOperationClientContext;

/*
 *  FSFileOperationStatusProcPtr
 *  
 *  Discussion:
 *    Callback function which is called to return status of an
 *    operation.  It will be called when an operation changes stages
 *    (including failing due to an error) or as updated information is
 *    available limited by the statusChangeInterval of the operation.
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      Operation receiving the callback.
 *    
 *    currentItem:
 *      FSRef to item operation is currently processing.  If the
 *      operation is complete then currentItem refers to the target
 *      item (the new item corresponding to the source item in the
 *      destination directory).
 *    
 *    stage:
 *      Current stage of the operation.
 *    
 *    error:
 *      Either noErr or an error value which caused the operation to
 *      fail.
 *    
 *    statusDictionary:
 *      A CFDictionary with more detailed status information.  The
 *      caller should not release this item.  If the item is needed
 *      beyond the scope of the callback then it needs to be copied.
 *    
 *    info:
 *      The info pointer passed in by the client.
 */
typedef CALLBACK_API_C( void , FSFileOperationStatusProcPtr )(FSFileOperationRef fileOp, const FSRef *currentItem, FSFileOperationStage stage, OSStatus error, CFDictionaryRef statusDictionary, void *info);

/*
 *  FSPathFileOperationStatusProcPtr
 *  
 *  Discussion:
 *    Callback function which is called to return status of an
 *    operation.  It will be called when an operation changes stages
 *    (including failing due to an error) or as updated information is
 *    available limited by the statusChangeInterval of the operation.
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      Operation receiving the callback.
 *    
 *    currentItem:
 *      The UTF-8 path for the item operation is currently processing. 
 *      The caller should not release this item.  If the item is needed
 *      beyond the scope of the callback then it needs to be copied. 
 *      If the operation is complete then currentItem refers to the
 *      target item (the new item corresponding to the source item in
 *      the destination directory).
 *    
 *    stage:
 *      Current stage of the operation.
 *    
 *    error:
 *      Either noErr or an error value which caused the operation to
 *      fail.
 *    
 *    statusDictionary:
 *      A CFDictionary with more detailed status information.  The
 *      caller should not release this item.  If the item is needed
 *      beyond the scope of the callback then it needs to be copied.
 *    
 *    info:
 *      The info pointer passed in by the client.
 */
typedef CALLBACK_API_C( void , FSPathFileOperationStatusProcPtr )(FSFileOperationRef fileOp, const char *currentItem, FSFileOperationStage stage, OSStatus error, CFDictionaryRef statusDictionary, void *info);

/*
 *  FSFileOperationOptions
 *  
 *  Discussion:
 *    Set of flags that can be passed into an FSMoveObject(A)sync or
 *    FSCopyObject(A)sync call.
 */
enum {

  /*
   * Use the default options - no overwrite, fail if any source item
   * cannot be read, cross volume moves OK.
   */
  kFSFileOperationDefaultOptions = 0,

  /*
   * Replace an item in the destDir that has the same name as an item
   * being moved/copied there.
   */
  kFSFileOperationOverwrite     = 0x01,

  /*
   * Skip items that cannot be read and continue copying/moving instead
   * of failing the operation.
   */
  kFSFileOperationSkipSourcePermissionErrors = 0x02,

  /*
   * Do not perform a copy/delete to move an item across volume
   * boundries - fail the operation instead.
   */
  kFSFileOperationDoNotMoveAcrossVolumes = 0x04,

  /*
   * Skip the preflight for a directory move/copy.  This will limit the
   * status information that can be returned since the totals will not
   * be calculated.
   */
  kFSFileOperationSkipPreflight = 0x08
};


/*
 *  FSFileOperationStage
 *  
 *  Discussion:
 *    The stage corresponds to the state of an asynchronous File
 *    Operation.  The stage is reported in the operation's status
 *    callback or as part of an FSFileOperationCopyStatus call.
 */
enum {

  /*
   * Operation has not started yet.
   */
  kFSOperationStageUndefined    = 0,

  /*
   * Operation is calulating sizes and number of items involved in the
   * operation.
   */
  kFSOperationStagePreflighting = 1,

  /*
   * Operation is in progress.
   */
  kFSOperationStageRunning      = 2,

  /*
   * Operation is done.
   */
  kFSOperationStageComplete     = 3
};

/* FSFileOperation status dictionary keys*/
/*
 *  kFSOperationTotalBytesKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the total number of bytes that will be moved/copied by
 *    this operation as a CFNumber.  This value is not available for a
 *    directory operation if kFSFileOperationSkipPreflight was
 *    specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationTotalBytesKey                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationBytesCompleteKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the number of bytes that have been moved/copied by this
 *    operation at the time the status call was made as a CFNumber. 
 *    During the preflight stage this value represents the currently
 *    known number of bytes that will be copied/moved.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationBytesCompleteKey                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationBytesRemainingKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the number of bytes that remain to be moved/copied by
 *    this operation at the time the status call was made as a
 *    CFNumber.  This value is not available for a directory operation
 *    if kFSFileOperationSkipPreflight was specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationBytesRemainingKey               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationTotalObjectsKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the total number of objects that will be moved/copied by
 *    this operation as a CFNumber.  This value is not available for a
 *    directory operation if kFSFileOperationSkipPreflight was
 *    specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationTotalObjectsKey                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationObjectsCompleteKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the number of objects that have been moved/copied by this
 *    operation at the time the status call was made as a CFNumber. 
 *    During the preflight stage this value represents the currently
 *    known number of objects that will be copied/moved.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationObjectsCompleteKey              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationObjectsRemainingKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the number of objects that remain to be moved/copied by
 *    this operation at the time the status call was made as a
 *    CFNumber.  This value is not available for a directory operation
 *    if kFSFileOperationSkipPreflight was specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationObjectsRemainingKey             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationTotalUserVisibleObjectsKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the total number of user visibleobjects that will be
 *    moved/copied by this operation as a CFNumber.  This value is not
 *    available for a directory operation if
 *    kFSFileOperationSkipPreflight was specified.  A packaged
 *    application is one user visible object even though it is made up
 *    of multiple files and directories.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationTotalUserVisibleObjectsKey      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationUserVisibleObjectsCompleteKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the number of user visible objects that have been
 *    moved/copied by this operation at the time the status call was
 *    made as a CFNumber.  During the preflight stage this value
 *    represents the currently known number of objects that will be
 *    copied/moved.  A packaged application is one user visible object
 *    even though it is made up of multiple files and directories.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationUserVisibleObjectsCompleteKey   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationUserVisibleObjectsRemainingKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the number of user visible objects that remain to be
 *    moved/copied by this operation at the time the status call was
 *    made as a CFNumber.  This value is not available for a directory
 *    operation if kFSFileOperationSkipPreflight was specified.  A
 *    packaged application is one user visible object even though it is
 *    made up of multiple files and directories.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationUserVisibleObjectsRemainingKey  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  kFSOperationThroughputKey
 *  
 *  Discussion:
 *    This value associated with this key in a status dictionary
 *    returns the current throughput for the operation in bytes per
 *    second as a CFNumber.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFSOperationThroughputKey                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/*
 *  FSCopyObjectSync()
 *  
 *  Discussion:
 *    This routine will copy the source object into the destination
 *    directory.  The source object can be a file or directory.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    source:
 *      The source object to copy.
 *    
 *    destDir:
 *      The destination directory for the copy.
 *    
 *    destName:
 *      The name for the new object in the destination directory.  Pass
 *      NULL to use the source object name.
 *    
 *    target:
 *      Upon successful completion a ref to the newly created object. 
 *      If source is a directory then target will be the corresponding
 *      object in the destination directory.
 *    
 *    options:
 *      One or more FSFileOperation flags
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSCopyObjectSync(const FSRef *source, const FSRef *destDir, CFStringRef destName, FSRef *target, OptionBits options) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMoveObjectSync()
 *  
 *  Discussion:
 *    This routine will move the source object into the destination
 *    directory.  The source object can be a file or directory.  If a
 *    destName is provided then the object will be renamed as well as
 *    moved.  By default a move across volumes will result in a copy
 *    and deletion of the original source.  The
 *    kFSFileOperationDoNotMoveAcrossVolumes flag will cause cross
 *    volume moves to do nothing and return an error.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    source:
 *      The source object to move.
 *    
 *    destDir:
 *      The destination directory for the move.
 *    
 *    destName:
 *      The name for the object in the destination directory.  Pass
 *      NULL to use the source object name.
 *    
 *    target:
 *      Upon successful completion a ref to the object in its new
 *      location.  If source is a directory then target will be the
 *      corresponding object in the destination directory.
 *    
 *    options:
 *      One or more FSFileOperation flags
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSMoveObjectSync(const FSRef *source, const FSRef *destDir, CFStringRef destName, FSRef *target, OptionBits options) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMoveObjectToTrashSync()
 *  
 *  Discussion:
 *    This routine will move the source object into the trash.  The
 *    source object can be a file or directory.  If the volume the
 *    source object resides on does not support a trash folder then
 *    this call will return an error (this is the same circumstance
 *    that triggers the delete immediately behavior in the Finder).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    source:
 *      The source object to move to the trash.
 *    
 *    target:
 *      Upon successful completion a ref the object in the trash.  If
 *      source is a directory then target will be the corresponding
 *      object in the destination directory.
 *    
 *    options:
 *      One or more FSFileOperation flags
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSMoveObjectToTrashSync(const FSRef *source, FSRef *target, OptionBits options) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathCopyObjectSync()
 *  
 *  Discussion:
 *    This routine will copy the source object into the destination
 *    directory.  The source object can be a file or directory.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    sourcePath:
 *      The UTF-8 path string of the source object to copy.
 *    
 *    destDirPath:
 *      The UTF-8 path of the destination directory for the copy.
 *    
 *    destName:
 *      The name for the new object in the destination directory.  Pass
 *      NULL to use the source object name.
 *    
 *    targetPath:
 *      A pointer to a char * to allow returning the path to the newly
 *      created object.  The path is allocated using malloc and it is
 *      the caller's responsibility to free.  The pointer will be set
 *      to NULL if the copy failed.
 *    
 *    options:
 *      One or more FSFileOperation flags
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathCopyObjectSync(const char *sourcePath, const char *destDirPath, CFStringRef destName, char **targetPath, OptionBits options) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathMoveObjectSync()
 *  
 *  Discussion:
 *    This routine will move the source object into the destination
 *    directory.  The source object can be a file or directory.  If a
 *    destName is provided then the object will be renamed as well as
 *    moved.  By default a move across volumes will result in a copy
 *    and deletion of the original source.  The
 *    kFSFileOperationDoNotMoveAcrossVolumes flag will cause cross
 *    volume moves to do nothing and return an error.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    sourcePath:
 *      The UTF-8 path string of the source object to move.
 *    
 *    destDirPath:
 *      The UTF-8 path of the destination directory for the move.
 *    
 *    destName:
 *      The name for the moved object in the destination directory. 
 *      Pass NULL to use the source object name.
 *    
 *    targetPath:
 *      A pointer to a char * to allow returning the path to the newly
 *      created object.  The path is allocated using malloc and it is
 *      the caller's responsibility to free.  The pointer will be set
 *      to NULL if the move failed.
 *    
 *    options:
 *      One or more FSFileOperation flags
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathMoveObjectSync(const char *sourcePath, const char *destDirPath, CFStringRef destName, char **targetPath, OptionBits options) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathMoveObjectToTrashSync()
 *  
 *  Discussion:
 *    This routine will move the source object into the trash.  The
 *    source object can be a file or directory.  If the volume the
 *    source object resides on does not support a trash folder then
 *    this call will return an error (this is the same circumstance
 *    that triggers the delete immediately behavior in the Finder).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    sourcePath:
 *      The UTF-8 path string of the source object to move.
 *    
 *    targetPath:
 *      A pointer to a char * to allow returning the path to the newly
 *      created object.  The path is allocated using malloc and it is
 *      the caller's responsibility to free.  The pointer will be set
 *      to NULL if the move failed.
 *    
 *    options:
 *      One or more FSFileOperation flags
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathMoveObjectToTrashSync(const char *sourcePath, char **targetPath, OptionBits options) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileOperationGetTypeID()
 *  
 *  Discussion:
 *    This routine will return the CFTypeID for the FSFileOpeation type.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Result:
 *    the CFTypeID for the FSFileOperation type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID  FSFileOperationGetTypeID(void)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileOperationCreate()
 *  
 *  Discussion:
 *    This routine will create an FSFileOperation for use with either
 *    FSCopyObjectAsync or FSMoveObjectAsync.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alloc:
 *      The CFAllocator to use.  Pass NULL for the default allocator.
 *  
 *  Result:
 *    A reference to the newly created object or NULL if the creation
 *    failed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern FSFileOperationRef  FSFileOperationCreate(CFAllocatorRef alloc) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileOperationScheduleWithRunLoop()
 *  
 *  Discussion:
 *    Schedule the given operation on the specified runloop and mode. A
 *    FSFileOperation can be scheduled on multiple runloop/mode
 *    combinations. An opertion must be scheduled on at least one
 *    runloop.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation to schedule.
 *    
 *    runLoop:
 *      The runLoop on which to schedule the operation.  Must be
 *      non-NULL.
 *    
 *    runLoopMode:
 *      The mode on which to schedule the operation.  Must be non-NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileOperationScheduleWithRunLoop(FSFileOperationRef fileOp, CFRunLoopRef runLoop, CFStringRef runLoopMode) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileOperationUnscheduleFromRunLoop()
 *  
 *  Discussion:
 *    Unschedule the given operation from the specified runloop and
 *    mode.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation to unschedule.
 *    
 *    runLoop:
 *      The runLoop from which to unschedule the operation.  Must be
 *      non-NULL.
 *    
 *    runLoopMode:
 *      The mode from which to unschedule the operation.  Must be
 *      non-NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileOperationUnscheduleFromRunLoop(FSFileOperationRef fileOp, CFRunLoopRef runLoop, CFStringRef runLoopMode) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSCopyObjectAsync()
 *  
 *  Discussion:
 *    This routine will start an asynchronous copy of the object
 *    specified by source to the directory specified by destDir.  If
 *    destName is provided then the new object will be renamed to
 *    destName.  If destName is not provided then the name of the
 *    source object will be used.  Status callbacks will occur on one
 *    of the runloop/mode combinations that the operation was scheduled
 *    on (and is running).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation object created for this operation.
 *    
 *    source:
 *      The source object to copy.
 *    
 *    destDir:
 *      The destination directory for the copy.
 *    
 *    destName:
 *      The name for the new object in the destination directory.  Pass
 *      NULL to use the source object name.
 *    
 *    flags:
 *      One or more FSFileOperation flags
 *    
 *    callback:
 *      An optional FSFileOperationStatusProcPtr which will be called
 *      with status updates as the copy proceeds.
 *    
 *    statusChangeInterval:
 *      The minimum time between callbacks within a single stage of an
 *      operation.
 *    
 *    clientContext:
 *      Client contextual information to associate with this operation.
 *       The info pointer will be passed to status callbacks.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSCopyObjectAsync(FSFileOperationRef fileOp, const FSRef *source, const FSRef *destDir, CFStringRef destName, OptionBits flags, FSFileOperationStatusProcPtr callback, CFTimeInterval statusChangeInterval, FSFileOperationClientContext *clientContext) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMoveObjectAsync()
 *  
 *  Discussion:
 *    This routine will start an asynchronous move of the object
 *    specified by source to the directory specified by destDir.  If
 *    destName is provided then the new object will be renamed to
 *    destName.  If destName is not provided then the name of the
 *    source object will be used.  Status callbacks will occur on one
 *    of the runloop/mode combinations that the operation was scheduled
 *    on (and is running).  By default a move across volumes will
 *    result in a copy and deletion of the original source. The
 *    kFSFileOperationDoNotMoveAcrossVolumes flag will cause cross
 *    volume moves to do nothing and return an error.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation object created for this operation.
 *    
 *    source:
 *      The source object to move.
 *    
 *    destDir:
 *      The destination directory for the move.
 *    
 *    destName:
 *      The name for the object in the destination directory.  Pass
 *      NULL to leave the name unchanged.
 *    
 *    flags:
 *      One or more FSFileOperation flags
 *    
 *    callback:
 *      An optional FSFileOperationStatusProcPtr which will be called
 *      with status updates as the copy proceeds.
 *    
 *    statusChangeInterval:
 *      The minimum time between callbacks within a single stage of an
 *      operation.
 *    
 *    clientContext:
 *      Client contextual information to associate with this operation.
 *       The info pointer will be passed to status callbacks.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSMoveObjectAsync(FSFileOperationRef fileOp, const FSRef *source, const FSRef *destDir, CFStringRef destName, OptionBits flags, FSFileOperationStatusProcPtr callback, CFTimeInterval statusChangeInterval, FSFileOperationClientContext *clientContext) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSMoveObjectToTrashAsync()
 *  
 *  Discussion:
 *    This routine will start an asynchronous move of the object
 *    specified by source to the trash.  If the volume the source
 *    object resides on does not support a trash folder then the
 *    operation will return an error (this is the same circumstance
 *    that triggers the delete immediately behavior in the Finder).
 *    Status callbacks will occur on one of the runloop/mode
 *    combinations that the operation was scheduled on (and is
 *    running).   Upon successul complettion of the operation the last
 *    currentItem (from either the last status callback or retrieved by
 *    FSFileOperationCopyStatus) will be object in the trash.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation object created for this operation.
 *    
 *    source:
 *      The source object to move.
 *    
 *    flags:
 *      One or more FSFileOperation flags
 *    
 *    callback:
 *      An optional FSFileOperationStatusProcPtr which will be called
 *      with status updates as the move proceeds.
 *    
 *    statusChangeInterval:
 *      The minimum time between callbacks within a single stage of an
 *      operation.
 *    
 *    clientContext:
 *      Client contextual information to associate with this operation.
 *       The info pointer will be passed to status callbacks.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSMoveObjectToTrashAsync(FSFileOperationRef fileOp, const FSRef *source, OptionBits flags, FSFileOperationStatusProcPtr callback, CFTimeInterval statusChangeInterval, FSFileOperationClientContext *clientContext) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathCopyObjectAsync()
 *  
 *  Discussion:
 *    This routine will start an asynchronous copy of the object
 *    specified by source to the directory specified by destDir.  If
 *    destName is provided then the new object will be renamed to
 *    destName.  If destName is not provided then the name of the
 *    source object will be used.  Status callbacks will occur on one
 *    of the runloop/mode combinations that the operation was scheduled
 *    on (and is running).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation object created for this operation.
 *    
 *    sourcePath:
 *      The UTF-8 path string of the source object to copy.
 *    
 *    destDirPath:
 *      The UTF-8 path of the destination directory for the copy.
 *    
 *    destName:
 *      The name for the new object in the destination directory.  Pass
 *      NULL to use the source object name.
 *    
 *    flags:
 *      One or more FSFileOperation flags
 *    
 *    callback:
 *      An optional FSPathFileOperationStatusProcPtr which will be
 *      called with status updates as the copy proceeds.
 *    
 *    statusChangeInterval:
 *      The minimum time between callbacks within a single stage of an
 *      operation.
 *    
 *    clientContext:
 *      Client contextual information to associate with this operation.
 *       The info pointer will be passed to status callbacks.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathCopyObjectAsync(FSFileOperationRef fileOp, const char *sourcePath, const char *destDirPath, CFStringRef destName, OptionBits flags, FSPathFileOperationStatusProcPtr callback, CFTimeInterval statusChangeInterval, FSFileOperationClientContext *clientContext) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathMoveObjectAsync()
 *  
 *  Discussion:
 *    This routine will start an asynchronous move of the object
 *    specified by source to the directory specified by destDir.  If
 *    destName is provided then the new object will be renamed to
 *    destName.  If destName is not provided then the name of the
 *    source object will be used.  Status callbacks will occur on one
 *    of the runloop/mode combinations that the operation was scheduled
 *    on (and is running).  By default a move across volumes will
 *    result in a copy and deletion of the original source. The
 *    kFSFileOperationDoNotMoveAcrossVolumes flag will cause cross
 *    volume moves to do nothing and return an error.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation object created for this operation.
 *    
 *    sourcePath:
 *      The UTF-8 path string of the source object to move.
 *    
 *    destDirPath:
 *      The UTF-8 path of the destination directory for the move.
 *    
 *    destName:
 *      The name for the object in the destination directory.  Pass
 *      NULL to leave the name unchanged.
 *    
 *    flags:
 *      One or more FSFileOperation flags
 *    
 *    callback:
 *      An optional FSPathFileOperationStatusProcPtr which will be
 *      called with status updates as the move proceeds.
 *    
 *    statusChangeInterval:
 *      The minimum time between callbacks within a single stage of an
 *      operation.
 *    
 *    clientContext:
 *      Client contextual information to associate with this operation.
 *       The info pointer will be passed to status callbacks.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathMoveObjectAsync(FSFileOperationRef fileOp, const char *sourcePath, const char *destDirPath, CFStringRef destName, OptionBits flags, FSPathFileOperationStatusProcPtr callback, CFTimeInterval statusChangeInterval, FSFileOperationClientContext *clientContext) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathMoveObjectToTrashAsync()
 *  
 *  Discussion:
 *    This routine will start an asynchronous move of the object
 *    specified by source to the trash.  If the volume the source
 *    object resides on does not support a trash folder then this call
 *    will return an error (this is the same circumstance that triggers
 *    the delete immediately behavior in the Finder). Status callbacks
 *    will occur on one of the runloop/mode combinations that the
 *    operation was scheduled on (and is running).  Note that the
 *    object may be renamed when placed in the trash.  Upon successful
 *    completion of the operation the last currentItem (from either the
 *    last status callback or retrieved by
 *    FSPathFileOperationCopyStatus) will be object in the trash.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation object created for this operation.
 *    
 *    sourcePath:
 *      The UTF-8 path string of the source object to move.
 *    
 *    flags:
 *      One or more FSFileOperation flags
 *    
 *    callback:
 *      An optional FSPathFileOperationStatusProcPtr which will be
 *      called with status updates as the move proceeds.
 *    
 *    statusChangeInterval:
 *      The minimum time between callbacks within a single stage of an
 *      operation.
 *    
 *    clientContext:
 *      Client contextual information to associate with this operation.
 *       The info pointer will be passed to status callbacks.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathMoveObjectToTrashAsync(FSFileOperationRef fileOp, const char *sourcePath, OptionBits flags, FSPathFileOperationStatusProcPtr callback, CFTimeInterval statusChangeInterval, FSFileOperationClientContext *clientContext) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileOperationCancel()
 *  
 *  Discussion:
 *    Cancels the specified FSFileOperation. This makes the operation
 *    ineligible to run on any runloop.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation to cancel.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileOperationCancel(FSFileOperationRef fileOp) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileOperationCopyStatus()
 *  
 *  Discussion:
 *    This routine returns the current status of an FSFileOperation. 
 *    The status dictionary must be released by the caller.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation from which to retrieve status information.
 *    
 *    currentItem:
 *      FSRef to item operation is currently processing.  If the
 *      operation is complete then currentItem refers to the target
 *      item (the new item corresponding to the source item in the
 *      destination directory).
 *    
 *    stage:
 *      current stage of the operation.
 *    
 *    error:
 *      Either noErr or an error value which caused the operation to
 *      fail.
 *    
 *    statusDictionary:
 *      A CFDictionary with more detailed status information. The
 *      caller is responsible for releasing the object when done with
 *      it.
 *    
 *    info:
 *      The info pointer passed in by the client.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileOperationCopyStatus(FSFileOperationRef fileOp, FSRef *currentItem, FSFileOperationStage *stage, OSStatus *error, CFDictionaryRef *statusDictionary, void **info) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSPathFileOperationCopyStatus()
 *  
 *  Discussion:
 *    This routine returns the current status of an FSFileOperation. 
 *    The status dictionary must be released by the caller.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileOp:
 *      The FSFileOperation from which to retrieve status information.
 *    
 *    currentItem:
 *      A path to the item operation is currently processing.  The
 *      caller is responsible for calling free to dispose of the path
 *      string.  If the operation is complete then currentItem refers
 *      to the target item (the new item corresponding to the source
 *      item in the destination directory).
 *    
 *    stage:
 *      current stage of the operation.
 *    
 *    error:
 *      Either noErr or an error value which caused the operation to
 *      fail.
 *    
 *    statusDictionary:
 *      A CFDictionary with more detailed status information.  The
 *      caller is responsible for releasing the object when done with
 *      it.
 *    
 *    info:
 *      The info pointer passed in by the client.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSPathFileOperationCopyStatus(FSFileOperationRef fileOp, char **currentItem, FSFileOperationStage *stage, OSStatus *error, CFDictionaryRef *statusDictionary, void **info) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* String conversion functions*/
/*
 *  FSCreateStringFromHFSUniStr()
 *  
 *  Discussion:
 *    Creates a CFString from a HFSUniStr255.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alloc:
 *      The CFAllocator to use.  Pass NULL for the default allocator.
 *    
 *    uniStr:
 *      A HFSUniStr255 to use as the source value for the CFString.
 *  
 *  Result:
 *    A CFStringRef created from the HFSUniStr255 or NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef  FSCreateStringFromHFSUniStr(CFAllocatorRef alloc, const HFSUniStr255 *uniStr) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSGetHFSUniStrFromString()
 *  
 *  Discussion:
 *    Convert a CFString into a HFSUniStr255.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    theString:
 *      The CFString to convert to a HFSUniStr255.
 *    
 *    uniStr:
 *      A pointer to a HFSUniStr255 which will be filled in using the
 *      value of theString.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetHFSUniStrFromString(CFStringRef theString, HFSUniStr255 *uniStr)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* ACL support*/

/*
 *  FSFileSecurityGetTypeID()
 *  
 *  Discussion:
 *    This routine will return the CFTypeID for the FSFileSecurity type.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Result:
 *    the CFTypeID for the FSFilSecurity type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID  FSFileSecurityGetTypeID(void)                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecurityCreate()
 *  
 *  Discussion:
 *    This routine will create an FSFileSecurity object.  The object
 *    should be released using CFRelease when it is no longer needed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    alloc:
 *      The CFAllocator to use.  Pass NULL for the default allocator.
 *  
 *  Result:
 *    A reference to the newly created object or NULL if the creation
 *    failed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern FSFileSecurityRef  FSFileSecurityCreate(CFAllocatorRef alloc) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecurityCreateWithFSPermissionInfo()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern FSFileSecurityRef  FSFileSecurityCreateWithFSPermissionInfo(CFAllocatorRef alloc, const FSPermissionInfo *permissions) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecurityRefCreateCopy()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern FSFileSecurityRef  FSFileSecurityRefCreateCopy(CFAllocatorRef alloc, FSFileSecurityRef fileSec) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* get and set properties*/

/*
 *  FSFileSecurityGetOwnerUUID()
 *  
 *  Discussion:
 *    This routine will get the owner UUID associated with the passed
 *    in FSFileSecurityRef.  In there is no owner UUID property
 *    associated with the FSFileSecurity object then
 *    errFSPropertyNotValid will be returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to get the owner UUID
 *      from.
 *    
 *    owner:
 *      A pointer to storage for the owner UUID associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecurityGetOwnerUUID(FSFileSecurityRef fileSec, CFUUIDBytes *owner)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecuritySetOwnerUUID()
 *  
 *  Discussion:
 *    This routine will set the owner UUID associated with the passed
 *    in FSFileSecurityRef.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to set the owner UUID
 *      for.
 *    
 *    owner:
 *      The UUID to set as the owner UUID associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecuritySetOwnerUUID(FSFileSecurityRef fileSec, const CFUUIDBytes *owner)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecurityGetGroupUUID()
 *  
 *  Discussion:
 *    This routine will get the group UUID associated with the passed
 *    in FSFileSecurityRef.  In there is no group UUID property
 *    associated with the FSFileSecurity object then
 *    errFSPropertyNotValid will be returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to get the group UUID
 *      from.
 *    
 *    group:
 *      A pointer to storage for the owner UUID associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecurityGetGroupUUID(FSFileSecurityRef fileSec, CFUUIDBytes *group)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecuritySetGroupUUID()
 *  
 *  Discussion:
 *    This routine will set the group UUID associated with the passed
 *    in FSFileSecurityRef.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to set the group UUID
 *      for.
 *    
 *    group:
 *      The UUID to set as the group UUID associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecuritySetGroupUUID(FSFileSecurityRef fileSec, const CFUUIDBytes *group)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecurityCopyAccessControlList()
 *  
 *  Discussion:
 *    This routine will copy the acl_t associated with the passed in
 *    FSFileSecurityRef.  The accessControlList returned by this
 *    routine is a copy and must be released using acl_free.  The
 *    accessControlList is an acl_t and is meant to be manipulated
 *    using the acl calls defined in <sys/acl.h>.   If there is no acl
 *    property associated with the FSFileSecurity object then
 *    errFSPropertyNotValid will be returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to set the group for.
 *    
 *    accessControlList:
 *      A pointer to storage for the acl_t associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecurityCopyAccessControlList(FSFileSecurityRef fileSec, acl_t *accessControlList) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecuritySetAccessControlList()
 *  
 *  Discussion:
 *    This routine will set the acl associated with the passed in
 *    FSFileSecurityRef. To request removal of an ACL from a filesystem
 *    object pass in kFSFileSecurityRemoveACL as the accessControlList
 *    and set the fileSec on the target object using FSSetCatalogInfo. 
 *    Setting the accessControlList to NULL will result in the property
 *    being unset.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to set the group for.
 *    
 *    accessControlList:
 *      The acl_t to set as the acl associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecuritySetAccessControlList(FSFileSecurityRef fileSec, acl_t accessControlList) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecurityGetOwner()
 *  
 *  Discussion:
 *    This routine will get the owner uid associated with the passed in
 *    FSFileSecurityRef.  In there is no owner property associated with
 *    the FSFileSecurity object then errFSPropertyNotValid will be
 *    returned.  Note that this value is not the owner UUID which is
 *    returned by FSFileSecurityGetOwnerUUID.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to get the owner from.
 *    
 *    owner:
 *      A pointer to storage for the owner uid associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecurityGetOwner(FSFileSecurityRef fileSec, UInt32 *owner)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecuritySetOwner()
 *  
 *  Discussion:
 *    This routine will set the owner uid associated with the passed in
 *    FSFileSecurityRef.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to set the owner for.
 *    
 *    owner:
 *      The uid to set as the owner associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecuritySetOwner(FSFileSecurityRef fileSec, UInt32 owner)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecurityGetGroup()
 *  
 *  Discussion:
 *    This routine will get the group gid associated with the passed in
 *    FSFileSecurityRef.  In there is no group property associated with
 *    the FSFileSecurity object then errFSPropertyNotValid will be
 *    returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to get the owner from.
 *    
 *    group:
 *      A pointer to storage for the group gid associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecurityGetGroup(FSFileSecurityRef fileSec, UInt32 *group)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecuritySetGroup()
 *  
 *  Discussion:
 *    This routine will set the group gid associated with the passed in
 *    FSFileSecurityRef.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to set the group for.
 *    
 *    group:
 *      The gid to set as the group associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecuritySetGroup(FSFileSecurityRef fileSec, UInt32 group)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecurityGetMode()
 *  
 *  Discussion:
 *    This routine will get the mode associated with the passed in
 *    FSFileSecurityRef.  In there is no mode property associated with
 *    the FSFileSecurity object then errFSPropertyNotValid will be
 *    returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to set the group for.
 *    
 *    mode:
 *      A pointer to storage for the mode associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecurityGetMode(FSFileSecurityRef fileSec, UInt16 *mode)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSFileSecuritySetMode()
 *  
 *  Discussion:
 *    This routine will set the mode associated with the passed in
 *    FSFileSecurityRef.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    fileSec:
 *      A reference to the FSFileSecurity object to set the group for.
 *    
 *    mode:
 *      The mode to set as the mode associated with fileSec.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFileSecuritySetMode(FSFileSecurityRef fileSec, UInt16 mode)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



enum {
                                        /* old names for ioPosMode flags */
  pleaseCacheBit                = 4,    /* old name of kFSPleaseCacheBit */
  pleaseCacheMask               = 0x0010,
  noCacheBit                    = 5,    /* old name of kFSNoCacheBit  */
  noCacheMask                   = 0x0020,
  rdVerifyBit                   = 6,    /* old name of kFSRdVerifyBit */
  rdVerifyMask                  = 0x0040,
  rdVerify                      = 64,   /* old name of rdVerifyMask */
  forceReadBit                  = 6,    /* old name of kFSForceReadBit */
  forceReadMask                 = 0x0040,
  newLineBit                    = 7,    /* old name of kFSNewLineBit */
  newLineMask                   = 0x0080,
  newLineCharMask               = 0xFF00 /* old name of kFSNewLineCharMask */
};


/* mapping codes (ioObjType) for MapName & MapID */
enum {
  kOwnerID2Name                 = 1,
  kGroupID2Name                 = 2,
  kOwnerName2ID                 = 3,
  kGroupName2ID                 = 4,    /* types of oj object to be returned (ioObjType) for _GetUGEntry */
  kReturnNextUser               = 1,
  kReturnNextGroup              = 2,
  kReturnNextUG                 = 3
};

/* vcbFlags bits */
enum {
  kVCBFlagsIdleFlushBit         = 3,    /* Set if volume should be flushed at idle time */
  kVCBFlagsIdleFlushMask        = 0x0008,
  kVCBFlagsHFSPlusAPIsBit       = 4,    /* Set if volume implements HFS Plus APIs itself (not via emulation) */
  kVCBFlagsHFSPlusAPIsMask      = 0x0010,
  kVCBFlagsHardwareGoneBit      = 5,    /* Set if disk driver returned a hardwareGoneErr to Read or Write */
  kVCBFlagsHardwareGoneMask     = 0x0020,
  kVCBFlagsVolumeDirtyBit       = 15,   /* Set if volume information has changed since the last FlushVol */
  kVCBFlagsVolumeDirtyMask      = 0x8000
};

/* ioVAtrb bits returned by PBHGetVInfo and PBXGetVolInfo */
enum {
  kioVAtrbDefaultVolumeBit      = 5,    /* Set if the volume is the default volume */
  kioVAtrbDefaultVolumeMask     = 0x0020,
  kioVAtrbFilesOpenBit          = 6,    /* Set if there are open files or iterators */
  kioVAtrbFilesOpenMask         = 0x0040,
  kioVAtrbHardwareLockedBit     = 7,    /* Set if volume is locked by a hardware setting */
  kioVAtrbHardwareLockedMask    = 0x0080,
  kioVAtrbSoftwareLockedBit     = 15,   /* Set if volume is locked by software */
  kioVAtrbSoftwareLockedMask    = 0x8000
};

/* ioFlAttrib bits returned by PBGetCatInfo */
enum {
                                        /* file and directory attributes in ioFlAttrib */
  kioFlAttribLockedBit          = 0,    /* Set if file or directory is locked */
  kioFlAttribLockedMask         = 0x01,
  kioFlAttribResOpenBit         = 2,    /* Set if resource fork is open */
  kioFlAttribResOpenMask        = 0x04,
  kioFlAttribDataOpenBit        = 3,    /* Set if data fork is open */
  kioFlAttribDataOpenMask       = 0x08,
  kioFlAttribDirBit             = 4,    /* Set if this is a directory */
  kioFlAttribDirMask            = 0x10,
  ioDirFlg                      = 4,    /* Set if this is a directory (old name) */
  ioDirMask                     = 0x10,
  kioFlAttribCopyProtBit        = 6,    /* Set if AppleShare server "copy-protects" the file */
  kioFlAttribCopyProtMask       = 0x40,
  kioFlAttribFileOpenBit        = 7,    /* Set if file (either fork) is open */
  kioFlAttribFileOpenMask       = 0x80, /* ioFlAttrib for directories only */
  kioFlAttribInSharedBit        = 2,    /* Set if the directory is within a shared area of the directory hierarchy */
  kioFlAttribInSharedMask       = 0x04,
  kioFlAttribMountedBit         = 3,    /* Set if the directory is a share point that is mounted by some user */
  kioFlAttribMountedMask        = 0x08,
  kioFlAttribSharePointBit      = 5,    /* Set if the directory is a share point */
  kioFlAttribSharePointMask     = 0x20
};

/* ioFCBFlags bits returned by PBGetFCBInfo */
/* IMPORTANT: These ioFCBFlags bit constants are for the SInt16 FCBPBRec.ioFCBFlags field returned */
/* by PBGetFCBInfoSync and PBGetFCBInfoAsync. Do not use them with the FSForkInfo.flags */
/* field returned by the FSGetForkCBInfo, PBGetForkCBInfoSync and PBGetForkCBInfoAsyn functions. */
enum {
  kioFCBWriteBit                = 8,    /* Data can be written to this file */
  kioFCBWriteMask               = 0x0100,
  kioFCBResourceBit             = 9,    /* This file is a resource fork */
  kioFCBResourceMask            = 0x0200,
  kioFCBWriteLockedBit          = 10,   /* File has a locked byte range */
  kioFCBWriteLockedMask         = 0x0400,
  kioFCBLargeFileBit            = 11,   /* File may grow beyond 2GB; cache uses file blocks, not bytes */
  kioFCBLargeFileMask           = 0x0800,
  kioFCBSharedWriteBit          = 12,   /* File is open for shared write access */
  kioFCBSharedWriteMask         = 0x1000,
  kioFCBFileLockedBit           = 13,   /* File is locked (write-protected) */
  kioFCBFileLockedMask          = 0x2000,
  kioFCBOwnClumpBit             = 14,   /* File has clump size specified in FCB */
  kioFCBOwnClumpMask            = 0x4000,
  kioFCBModifiedBit             = 15,   /* File has changed since it was last flushed */
  kioFCBModifiedMask            = 0x8000
};

/* IMPORTANT: These FSForkInfoFlags constants are for use with the FSForkInfo.flags */
/* field returned by the FSGetForkCBInfo, PBGetForkCBInfoSync and PBGetForkCBInfoAsyn functions. */
/* Do not use them with the FCBPBRec.ioFCBFlags field returned by PBGetFCBInfoSync and PBGetFCBInfoAsync. */
enum {
  kForkInfoFlagsWriteBit        = (kioFCBWriteBit - 8), /* Data can be written to this file */
  kForkInfoFlagsWriteMask       = (1 << kForkInfoFlagsWriteBit),
  kForkInfoFlagsResourceBit     = (kioFCBResourceBit - 8), /* This file is a resource fork */
  kForkInfoFlagsResourceMask    = (1 << kForkInfoFlagsResourceBit),
  kForkInfoFlagsWriteLockedBit  = (kioFCBWriteLockedBit - 8), /* File has a locked byte range */
  kForkInfoFlagsWriteLockedMask = (1 << kForkInfoFlagsWriteLockedBit),
  kForkInfoFlagsLargeFileBit    = (kioFCBLargeFileBit - 8), /* File may grow beyond 2GB; cache uses file blocks, not bytes */
  kForkInfoFlagsLargeFileMask   = (1 << kForkInfoFlagsLargeFileBit),
  kForkInfoFlagsSharedWriteBit  = (kioFCBSharedWriteBit - 8), /* File is open for shared write access */
  kForkInfoFlagsSharedWriteMask = (1 << kForkInfoFlagsSharedWriteBit),
  kForkInfoFlagsFileLockedBit   = (kioFCBFileLockedBit - 8), /* File is locked (write-protected) */
  kForkInfoFlagsFileLockedMask  = (1 << kForkInfoFlagsFileLockedBit),
  kForkInfoFlagsOwnClumpBit     = (kioFCBOwnClumpBit - 8), /* File has clump size specified in FCB */
  kForkInfoFlagsOwnClumpMask    = (1 << kForkInfoFlagsOwnClumpBit),
  kForkInfoFlagsModifiedBit     = (kioFCBModifiedBit - 8), /* File has changed since it was last flushed */
  kForkInfoFlagsModifiedMask    = (1 << kForkInfoFlagsModifiedBit)
};

/* ioACUser bits returned by PBGetCatInfo */
/* Note: you must clear ioACUser before calling PBGetCatInfo because some file systems do not use this field */
enum {
  kioACUserNoSeeFolderBit       = 0,    /* Set if user does not have See Folder privileges */
  kioACUserNoSeeFolderMask      = 0x01,
  kioACUserNoSeeFilesBit        = 1,    /* Set if user does not have See Files privileges */
  kioACUserNoSeeFilesMask       = 0x02,
  kioACUserNoMakeChangesBit     = 2,    /* Set if user does not have Make Changes privileges */
  kioACUserNoMakeChangesMask    = 0x04,
  kioACUserNotOwnerBit          = 7,    /* Set if user is not owner of the directory */
  kioACUserNotOwnerMask         = 0x80
};

/* Folder and File values of access privileges in ioACAccess */
enum {
  kioACAccessOwnerBit           = 31,   /* User is owner of directory */
  kioACAccessOwnerMask          = (int)0x80000000,
  kioACAccessBlankAccessBit     = 28,   /* Directory has blank access privileges */
  kioACAccessBlankAccessMask    = 0x10000000,
  kioACAccessUserWriteBit       = 26,   /* User has write privileges */
  kioACAccessUserWriteMask      = 0x04000000,
  kioACAccessUserReadBit        = 25,   /* User has read privileges */
  kioACAccessUserReadMask       = 0x02000000,
  kioACAccessUserSearchBit      = 24,   /* User has search privileges */
  kioACAccessUserSearchMask     = 0x01000000,
  kioACAccessEveryoneWriteBit   = 18,   /* Everyone has write privileges */
  kioACAccessEveryoneWriteMask  = 0x00040000,
  kioACAccessEveryoneReadBit    = 17,   /* Everyone has read privileges */
  kioACAccessEveryoneReadMask   = 0x00020000,
  kioACAccessEveryoneSearchBit  = 16,   /* Everyone has search privileges */
  kioACAccessEveryoneSearchMask = 0x00010000,
  kioACAccessGroupWriteBit      = 10,   /* Group has write privileges */
  kioACAccessGroupWriteMask     = 0x00000400,
  kioACAccessGroupReadBit       = 9,    /* Group has read privileges */
  kioACAccessGroupReadMask      = 0x00000200,
  kioACAccessGroupSearchBit     = 8,    /* Group has search privileges */
  kioACAccessGroupSearchMask    = 0x00000100,
  kioACAccessOwnerWriteBit      = 2,    /* Owner has write privileges */
  kioACAccessOwnerWriteMask     = 0x00000004,
  kioACAccessOwnerReadBit       = 1,    /* Owner has read privileges */
  kioACAccessOwnerReadMask      = 0x00000002,
  kioACAccessOwnerSearchBit     = 0,    /* Owner has search privileges */
  kioACAccessOwnerSearchMask    = 0x00000001,
  kfullPrivileges               = 0x00070007, /* all privileges for everybody and owner*/
  kownerPrivileges              = 0x00000007 /* all privileges for owner only*/
};


/*  Volume Characteristics */
/*
 *  FSGetVolumeParms()
 *  
 *  Discussion:
 *    Returns information about the specified volume in the passed in
 *    GetVolParmsInfoBuffer.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    volume:
 *      The volume to get the information about.
 *    
 *    buffer:
 *      A GetVolParmsInfoBuffer to fill out
 *    
 *    bufferSize:
 *      the size of the passed in buffer
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetVolumeParms(FSVolumeRefNum volume, GetVolParmsInfoBuffer *buffer, ByteCount bufferSize) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*  VolumeMount */
/*
 *  FSGetVolumeMountInfoSize()
 *  
 *  Discussion:
 *    Returns the size of the MountInfo block associated with the
 *    specified volume.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    volume:
 *      The volume to get the MountInfo size for.
 *    
 *    size:
 *      The size of the buffer required to store the MountInfo data
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetVolumeMountInfoSize(FSVolumeRefNum volume, ByteCount *size)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSGetVolumeMountInfo()
 *  
 *  Discussion:
 *    Returns the MountInfo data for the specified volume.  If the
 *    buffer passed in is too small then errFSBadBuffer is returned and
 *    actualSize will contain the required buffer size to hold the
 *    MountInfo data.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    volume:
 *      The volume to get the MountInfo for.
 *    
 *    buffer:
 *      A pointer to the buffer to copy the MountInfo data into
 *    
 *    bufferSize:
 *      The size of the buffer passed in
 *    
 *    actualSize:
 *      The actual size of the MountInfo data
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSGetVolumeMountInfo(FSVolumeRefNum volume, BytePtr buffer, ByteCount bufferSize, ByteCount *actualSize) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FSVolumeMount()
 *  
 *  Discussion:
 *    Will attempt to remount a volume using the passed in MountInfo
 *    buffer
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    buffer:
 *      A pointer to the buffer with the MountInfo data.
 *    
 *    mountedVolume:
 *      the FSVolumeRefNum of the volume that was mounted
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSVolumeMount(BytePtr buffer, FSVolumeRefNum *mountedVolume)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*  Volume flushing */
/*
 *  FSFlushVolume()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSFlushVolume(FSVolumeRefNum vRefNum)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBFlushVolumeSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBFlushVolumeSync(FSRefParamPtr paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBFlushVolumeAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBFlushVolumeAsync(FSRefParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* CopyFile */
/*
    PBFSCopyFile
    Copies a file on a volume that supports the CopyFile system call (which can be
    determined from the bHasCopyFile GetVolParms bit).
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ref             The source item to copy
    ->  parentRef       The destination to copy to
    ->  nameLength      Number of Unicode characters in the optional new name.  Pass in 0 to use the source name
    ->  name            A pointer to the optional new Unicode name; Pass in NULL to use the source name
    <-  newRef          A pointer to the FSRef for the new object; may be NULL
*/
/*
 *  PBFSCopyFileSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBFSCopyFileSync(FSRefParamPtr paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBFSCopyFileAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBFSCopyFileAsync(FSRefParamPtr paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* Node ID Resolution */
/*
 *  FSResolveNodeID()
 *  
 *  Discussion:
 *    Returns an FSRef to the item on volume with the specified node id.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    volume:
 *      The FSVolumeRefNum of the volume the item is on
 *    
 *    nodeID:
 *      The node id to resolve
 *    
 *    newRef:
 *      A pointer to storage for the FSRef of the item with the
 *      corresponding node id
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  FSResolveNodeID(FSVolumeRefNum volume, UInt32 nodeID, FSRefPtr newRef)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ioVRefNum       The FSVolumeRefNum of the volume the item is on
    ->  ioDirID         The node id to resolve (can be file as well as a directory node id)
    <-  newRef          A pointer to storage for the FSRef of the item with the corresponding node id
*/
/*
 *  PBFSResolveNodeIDSync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBFSResolveNodeIDSync(FSRefParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBFSResolveNodeIDAsync()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus  PBFSResolveNodeIDAsync(FSRefParamPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* Calls beyond this point are deprecated*/

/*
    MakeFSRef
    Create an FSRef for an existing object specified by a combination
    of volume refnum, parent directory, and pathname.
    ->  ioCompletion    A pointer to a completion routine
    <-  ioResult        The result code of the function
    ->  ioNamePtr       A pointer to a pathname
    ->  ioVRefNum       A volume specification
    ->  ioDirID         A directory ID
    <-  newRef          A pointer to an FSRef
   This function is deprecated in Mac OS X 10.5. Use FSMakeFSRefUnicode instead.
*/
#if !__LP64__
/*
 *  FSpMakeFSRef()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  FSpMakeFSRef(const FSSpec *source, FSRef *newRef)                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBMakeFSRefUnicodeSync instead.*/
/*
 *  PBMakeFSRefSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBMakeFSRefSync(FSRefParam * paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBMakeFSRefUnicodeAsync instead.*/
/*
 *  PBMakeFSRefAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern void  PBMakeFSRefAsync(FSRefParam * paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBCloseForkSync instead.*/
/*
 *  PBCloseSync()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBCloseSync(ParmBlkPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBCloseForkAsync instead.*/
/*
 *  PBCloseAsync()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBCloseAsync(ParmBlkPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBReadForkSync instead.*/
/*
 *  PBReadSync()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBReadSync(ParmBlkPtr paramBlock)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBReadForkAsync instead.*/
/*
 *  PBReadAsync()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBReadAsync(ParmBlkPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBWriteForkSync instead.*/
/*
 *  PBWriteSync()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBWriteSync(ParmBlkPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBWriteForkAsync instead.*/
/*
 *  PBWriteAsync()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBWriteAsync(ParmBlkPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. It was never implemented on OS X.*/
/*
 *  PBWaitIOComplete()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.0 and later
 */
extern OSErr  PBWaitIOComplete(ParmBlkPtr paramBlock, Duration timeout)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*  Volume Characteristics */
/* This function is deprecated in Mac OS X 10.5. Use FSGetVolumeParms instead.*/
/*
 *  PBHGetVolParmsSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetVolParmsSync(HParmBlkPtr paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSGetVolumeParms instead.*/
/*
 *  PBHGetVolParmsAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetVolParmsAsync(HParmBlkPtr paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*  VolumeMount */
/* This function is deprecated in Mac OS X 10.5. Use FSGetVolumeMountInfoSize instead.*/
/*
 *  PBGetVolMountInfoSize()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetVolMountInfoSize(ParmBlkPtr paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSGetVolumeMountInfo instead.*/
/*
 *  PBGetVolMountInfo()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetVolMountInfo(ParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSVolumeMount instead.*/
/*
 *  PBVolumeMount()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBVolumeMount(ParmBlkPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*  Volume flushing */
/* This function is deprecated in Mac OS X 10.5. Use FSFlushVolume instead.*/
/*
 *  FlushVol()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  FlushVol(ConstStr63Param volName, FSVolumeRefNum vRefNum)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBFlushVolumeSync instead.*/
/*
 *  PBFlushVolSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBFlushVolSync(ParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBFlushVolumeAsync instead.*/
/*
 *  PBFlushVolAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBFlushVolAsync(ParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5.*/
/*
 *  PBHOpenDenySync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenDenySync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5.*/
/*
 *  PBHOpenDenyAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenDenyAsync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5.*/
/*
 *  PBHOpenRFDenySync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenRFDenySync(HParmBlkPtr paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5.*/
/*
 *  PBHOpenRFDenyAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenRFDenyAsync(HParmBlkPtr paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSGetCatalogInfo instead.*/
/*
 *  PBHGetDirAccessSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetDirAccessSync(HParmBlkPtr paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSGetCatalogInfo instead.*/
/*
 *  PBHGetDirAccessAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetDirAccessAsync(HParmBlkPtr paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSSetCatalogInfo instead.*/
/*
 *  PBHSetDirAccessSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHSetDirAccessSync(HParmBlkPtr paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSSetCatalogInfo instead.*/
/*
 *  PBHSetDirAccessAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHSetDirAccessAsync(HParmBlkPtr paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. See getpwnam() and getpwuid().*/
/*
 *  PBHMapIDSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHMapIDSync(HParmBlkPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. See getpwnam() and getpwuid().*/
/*
 *  PBHMapIDAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHMapIDAsync(HParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. See getpwnam() and getpwuid().*/
/*
 *  PBHMapNameSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHMapNameSync(HParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. See getpwnam() and getpwuid().*/
/*
 *  PBHMapNameAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHMapNameAsync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBFSCopyFileSync instead.*/
/*
 *  PBHCopyFileSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHCopyFileSync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use PBFSCopyFileAsync instead.*/
/*
 *  PBHCopyFileAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHCopyFileAsync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSGetCatalogInfo to get the node id.*/
/*
 *  PBCreateFileIDRefSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBCreateFileIDRefSync(HParmBlkPtr paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSGetCatalogInfo to get the node id.*/
/*
 *  PBCreateFileIDRefAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBCreateFileIDRefAsync(HParmBlkPtr paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSGetCatalogInfo to get the node id.*/
/*
 *  PBResolveFileIDRefSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBResolveFileIDRefSync(HParmBlkPtr paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. Use FSGetCatalogInfo to get the node id.*/
/*
 *  PBResolveFileIDRefAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBResolveFileIDRefAsync(HParmBlkPtr paramBlock) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. It is obsolete and has no replacement.*/
/*
 *  PBDeleteFileIDRefSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDeleteFileIDRefSync(HParmBlkPtr paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.5. It is obsolete and has no replacement.*/
/*
 *  PBDeleteFileIDRefAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDeleteFileIDRefAsync(HParmBlkPtr paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSGetVolumeInfo instead.*/
/*
 *  PBXGetVolInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr  PBXGetVolInfoSync(XVolumeParamPtr paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSGetVolumeInfo instead.*/
/*
 *  PBXGetVolInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr  PBXGetVolInfoAsync(XVolumeParamPtr paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBAllocateForkSync instead.*/
/*
 *  PBAllocateSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBAllocateSync(ParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBAllocateForkAsync instead.*/
/*
 *  PBAllocateAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBAllocateAsync(ParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetForkSizeSync instead.*/
/*
 *  PBGetEOFSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetEOFSync(ParmBlkPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetForkSizeAsync instead.*/
/*
 *  PBGetEOFAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetEOFAsync(ParmBlkPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetForkSizeSync instead.*/
/*
 *  PBSetEOFSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetEOFSync(ParmBlkPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetForkSizeAsync instead.*/
/*
 *  PBSetEOFAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetEOFAsync(ParmBlkPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetForkPositionSync instead.*/
/*
 *  PBGetFPosSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetFPosSync(ParmBlkPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetForkPositionAsync instead.*/
/*
 *  PBGetFPosAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetFPosAsync(ParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetForkPositionSync instead.*/
/*
 *  PBSetFPosSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetFPosSync(ParmBlkPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetForkPositionAsync instead.*/
/*
 *  PBSetFPosAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetFPosAsync(ParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBFlushForkSync instead.*/
/*
 *  PBFlushFileSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBFlushFileSync(ParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBFlushForkAsync instead.*/
/*
 *  PBFlushFileAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBFlushFileAsync(ParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSEjectVolumeSync instead.*/
/*
 *  PBUnmountVol()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBUnmountVol(ParmBlkPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBCatalogSearchSync instead.*/
/*
 *  PBCatSearchSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBCatSearchSync(CSParamPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBCatalogSearchAsync instead.*/
/*
 *  PBCatSearchAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBCatSearchAsync(CSParamPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSEjectVolumeSync instead.*/
/*
 *  UnmountVol()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
UnmountVol(
  ConstStr63Param   volName,       /* can be NULL */
  FSVolumeRefNum    vRefNum)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. The routines which use the default volume concept have been deprecated.*/
/*
 *  HSetVol()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HSetVol(
  ConstStr63Param   volName,       /* can be NULL */
  FSVolumeRefNum    vRefNum,
  SInt32            dirID)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* AddDrive() was moved to Devices.h*/

/* This function is deprecated in Mac OS X 10.4. Use FSCloseFork instead.*/
/*
 *  FSClose()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSClose(FSIORefNum refNum)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSReadFork instead.*/
/*
 *  FSRead()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSRead(
  FSIORefNum   refNum,
  SInt32 *     count,
  void *       buffPtr)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSWriteFork instead.*/
/*
 *  FSWrite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSWrite(
  FSIORefNum    refNum,
  SInt32 *      count,
  const void *  buffPtr)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSAllocateFork instead.*/
/*
 *  Allocate()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
Allocate(
  FSIORefNum   refNum,
  SInt32 *     count)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSGetForkSize instead.*/
/*
 *  GetEOF()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetEOF(
  FSIORefNum   refNum,
  SInt32 *     logEOF)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSSetForkSize instead.*/
/*
 *  SetEOF()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetEOF(
  FSIORefNum   refNum,
  SInt32       logEOF)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSGetForkPosition instead.*/
/*
 *  GetFPos()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetFPos(
  FSIORefNum   refNum,
  SInt32 *     filePos)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSSetForkPosition instead.*/
/*
 *  SetFPos()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetFPos(
  FSIORefNum   refNum,
  SInt16       posMode,
  SInt32       posOff)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSGetCatalogInfo instead.*/
/*
 *  GetVRefNum()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetVRefNum(
  FSIORefNum        fileRefNum,
  FSVolumeRefNum *  vRefNum)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBXLockRangeSync or FSLockRange instead.*/
/*
 *  PBLockRangeSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PBLockRangeSync(ParmBlkPtr paramBlock)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBXLockRangeAsync instead.*/
/*
 *  PBLockRangeAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PBLockRangeAsync(ParmBlkPtr paramBlock)                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBXUnlockRangeSync or FSUnlockRange instead.*/
/*
 *  PBUnlockRangeSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PBUnlockRangeSync(ParmBlkPtr paramBlock)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBXUnlockRangeAsync instead.*/
/*
 *  PBUnlockRangeAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PBUnlockRangeAsync(ParmBlkPtr paramBlock)                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. The routines which use the default volume concept have been deprecated.*/
/*
 *  PBHSetVolSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHSetVolSync(WDPBPtr paramBlock)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. The routines which use the default volume concept have been deprecated.*/
/*
 *  PBHSetVolAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHSetVolAsync(WDPBPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. The routines which use the default volume concept have been deprecated.*/
/*
 *  PBHGetVolSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetVolSync(WDPBPtr paramBlock)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. The routines which use the default volume concept have been deprecated.*/
/*
 *  PBHGetVolAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetVolAsync(WDPBPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBMoveObjectSync instead.*/
/*
 *  PBCatMoveSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBCatMoveSync(CMovePBPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBMoveObjectAsync instead.*/
/*
 *  PBCatMoveAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBCatMoveAsync(CMovePBPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBCreateDirectoryUnicodeSync instead.*/
/*
 *  PBDirCreateSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDirCreateSync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBCreateDirectoryUnicodeAsync instead.*/
/*
 *  PBDirCreateAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDirCreateAsync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetForkCBInfoSync instead.*/
/*
 *  PBGetFCBInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetFCBInfoSync(FCBPBPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetForkCBInfoAsync instead.*/
/*
 *  PBGetFCBInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetFCBInfoAsync(FCBPBPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetCatalogInfoSync instead.*/
/*
 *  PBGetCatInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetCatInfoSync(CInfoPBPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetCatalogInfoAsync instead.*/
/*
 *  PBGetCatInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetCatInfoAsync(CInfoPBPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetCatalogInfoSync instead.*/
/*
 *  PBSetCatInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetCatInfoSync(CInfoPBPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetCatalogInfoAsync instead.*/
/*
 *  PBSetCatInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetCatInfoAsync(CInfoPBPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBAllocateForkSync instead.*/
/*
 *  PBAllocContigSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBAllocContigSync(ParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBAllocateForkAsync instead.*/
/*
 *  PBAllocContigAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBAllocContigAsync(ParmBlkPtr paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetVolumeInfoSync instead.*/
/*
 *  PBSetVInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetVInfoSync(HParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetVolumeInfoAsync instead.*/
/*
 *  PBSetVInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetVInfoAsync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetVolumeInfoSync instead.*/
/*
 *  PBHGetVInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetVInfoSync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetVolumeInfoAsync instead.*/
/*
 *  PBHGetVInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetVInfoAsync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBOpenForkSync instead.*/
/*
 *  PBHOpenSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenSync(HParmBlkPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBOpenForkAsync instead.*/
/*
 *  PBHOpenAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenAsync(HParmBlkPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBOpenForkSync instead.*/
/*
 *  PBHOpenRFSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenRFSync(HParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBOpenForkAsync instead.*/
/*
 *  PBHOpenRFAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenRFAsync(HParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBOpenForkSync instead.*/
/*
 *  PBHOpenDFSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenDFSync(HParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBOpenForkAsync instead.*/
/*
 *  PBHOpenDFAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHOpenDFAsync(HParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBCreateFileUnicodeSync instead.*/
/*
 *  PBHCreateSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHCreateSync(HParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBCreateFileUnicodeAsync instead.*/
/*
 *  PBHCreateAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHCreateAsync(HParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBDeleteObjectSync instead.*/
/*
 *  PBHDeleteSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHDeleteSync(HParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBDeleteObjectAsync instead.*/
/*
 *  PBHDeleteAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHDeleteAsync(HParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBRenameUnicodeSync instead.*/
/*
 *  PBHRenameSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHRenameSync(HParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBRenameUnicodeAsync instead.*/
/*
 *  PBHRenameAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHRenameAsync(HParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetCatalogInfoSync instead.*/
/*
 *  PBHRstFLockSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHRstFLockSync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetCatalogInfoAsync instead.*/
/*
 *  PBHRstFLockAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHRstFLockAsync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetCatalogInfoSync instead.*/
/*
 *  PBHSetFLockSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHSetFLockSync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetCatalogInfoAsync instead.*/
/*
 *  PBHSetFLockAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHSetFLockAsync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetCatalogInfoSync instead.*/
/*
 *  PBHGetFInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetFInfoSync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBGetCatalogInfoAsync instead.*/
/*
 *  PBHGetFInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetFInfoAsync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetCatalogInfoSync instead.*/
/*
 *  PBHSetFInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHSetFInfoSync(HParmBlkPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBSetCatalogInfoAsync instead.*/
/*
 *  PBHSetFInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHSetFInfoAsync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBMakeFSRefUnicodeSync instead.*/
/*
 *  PBMakeFSSpecSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBMakeFSSpecSync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBMakeFSRefUnicodeAsync instead.*/
/*
 *  PBMakeFSSpecAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBMakeFSSpecAsync(HParmBlkPtr paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. The routines which use the default volume concept have been deprecated.*/
/*
 *  HGetVol()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HGetVol(
  StringPtr         volName,
  FSVolumeRefNum *  vRefNum,
  SInt32 *          dirID)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSOpenFork instead.*/
/*
 *  HOpen()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HOpen(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName,
  SInt8              permission,
  FSIORefNum *       refNum)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSOpenFork instead.*/
/*
 *  HOpenDF()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HOpenDF(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName,
  SInt8              permission,
  FSIORefNum *       refNum)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSOpenFork instead.*/
/*
 *  HOpenRF()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HOpenRF(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName,
  SInt8              permission,
  FSIORefNum *       refNum)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSAllocateFork instead.*/
/*
 *  AllocContig()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AllocContig(
  FSVolumeRefNum   refNum,
  SInt32 *         count)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSCreateFileUnicode instead.*/
/*
 *  HCreate()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HCreate(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName,
  OSType             creator,
  OSType             fileType)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSCreateDirectoryUnicode instead.*/
/*
 *  DirCreate()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
DirCreate(
  FSVolumeRefNum     vRefNum,
  SInt32             parentDirID,
  ConstStr255Param   directoryName,
  SInt32 *           createdDirID)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSDeleteObject instead.*/
/*
 *  HDelete()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HDelete(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSGetCatalogInfo instead.*/
/*
 *  HGetFInfo()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HGetFInfo(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName,
  FInfo *            fndrInfo)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSSetCatalogInfo instead.*/
/*
 *  HSetFInfo()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HSetFInfo(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName,
  const FInfo *      fndrInfo)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSSetCatalogInfo instead.*/
/*
 *  HSetFLock()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HSetFLock(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSSetCatalogInfo instead.*/
/*
 *  HRstFLock()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HRstFLock(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSRenameUnicode instead.*/
/*
 *  HRename()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HRename(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   oldName,
  ConstStr255Param   newName)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSMoveObject instead.*/
/*
 *  CatMove()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
CatMove(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   oldName,
  SInt32             newDirID,
  ConstStr255Param   newName)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBHGetLogInInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetLogInInfoSync(HParmBlkPtr paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBHGetLogInInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHGetLogInInfoAsync(HParmBlkPtr paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
   This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.
   Use FSMoveObjectSync instead.
*/
/*
 *  PBHMoveRenameSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHMoveRenameSync(HParmBlkPtr paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
   This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.
   Use FSMoveObjectAsync instead.
*/
/*
 *  PBHMoveRenameAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBHMoveRenameAsync(HParmBlkPtr paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBGetXCatInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr  PBGetXCatInfoSync(XCInfoPBPtr paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBGetXCatInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr  PBGetXCatInfoAsync(XCInfoPBPtr paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBExchangeObjectsSync instead.*/
/*
 *  PBExchangeFilesSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBExchangeFilesSync(HParmBlkPtr paramBlock)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use PBExchangeObjectsAsync instead.*/
/*
 *  PBExchangeFilesAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBExchangeFilesAsync(HParmBlkPtr paramBlock)    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBGetForeignPrivsSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetForeignPrivsSync(HParmBlkPtr paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBGetForeignPrivsAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetForeignPrivsAsync(HParmBlkPtr paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBSetForeignPrivsSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetForeignPrivsSync(HParmBlkPtr paramBlock)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBSetForeignPrivsAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBSetForeignPrivsAsync(HParmBlkPtr paramBlock)  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*  Desktop Manager  */
#endif  /* !__LP64__ */

#if !__LP64__
enum {
                                        /* Desktop Database, ffsGetIconMessage and fsmGetFSIconMessage icon type and size Constants */
  kLargeIcon                    = 1,
  kLarge4BitIcon                = 2,
  kLarge8BitIcon                = 3,
  kSmallIcon                    = 4,
  kSmall4BitIcon                = 5,
  kSmall8BitIcon                = 6,
  kicnsIconFamily               = 239   /* Note: The 'icns' icon family record is variable sized. */
};

#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  PBDTGetPath()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetPath(DTPBPtr paramBlock)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTCloseDown()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTCloseDown(DTPBPtr paramBlock)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTAddIconSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTAddIconSync(DTPBPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTAddIconAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTAddIconAsync(DTPBPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetIconSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetIconSync(DTPBPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetIconAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetIconAsync(DTPBPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetIconInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetIconInfoSync(DTPBPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetIconInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetIconInfoAsync(DTPBPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTAddAPPLSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTAddAPPLSync(DTPBPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTAddAPPLAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTAddAPPLAsync(DTPBPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTRemoveAPPLSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTRemoveAPPLSync(DTPBPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTRemoveAPPLAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTRemoveAPPLAsync(DTPBPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetAPPLSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetAPPLSync(DTPBPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetAPPLAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetAPPLAsync(DTPBPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTSetCommentSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTSetCommentSync(DTPBPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTSetCommentAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTSetCommentAsync(DTPBPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTRemoveCommentSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTRemoveCommentSync(DTPBPtr paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTRemoveCommentAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTRemoveCommentAsync(DTPBPtr paramBlock)      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetCommentSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetCommentSync(DTPBPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetCommentAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetCommentAsync(DTPBPtr paramBlock)         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTFlushSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTFlushSync(DTPBPtr paramBlock)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTFlushAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTFlushAsync(DTPBPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTResetSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTResetSync(DTPBPtr paramBlock)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTResetAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTResetAsync(DTPBPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetInfoSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetInfoSync(DTPBPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTGetInfoAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTGetInfoAsync(DTPBPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTOpenInform()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTOpenInform(DTPBPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTDeleteSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTDeleteSync(DTPBPtr paramBlock)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PBDTDeleteAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBDTDeleteAsync(DTPBPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*  FSp traps  */
/* This function is deprecated in Mac OS X 10.4. Use FSMakeFSRefUnicode instead.*/
/*
 *  FSMakeFSSpec()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSMakeFSSpec(
  FSVolumeRefNum     vRefNum,
  SInt32             dirID,
  ConstStr255Param   fileName,
  FSSpec *           spec)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSOpenFork instead.*/
/*
 *  FSpOpenDF()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpOpenDF(
  const FSSpec *  spec,
  SInt8           permission,
  FSIORefNum *    refNum)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSOpenFork instead.*/
/*
 *  FSpOpenRF()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpOpenRF(
  const FSSpec *  spec,
  SInt8           permission,
  FSIORefNum *    refNum)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSCreateFileUnicode instead.*/
/*
 *  FSpCreate()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpCreate(
  const FSSpec *  spec,
  OSType          creator,
  OSType          fileType,
  ScriptCode      scriptTag)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSCreateDirectoryUnicode instead.*/
/*
 *  FSpDirCreate()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpDirCreate(
  const FSSpec *  spec,
  ScriptCode      scriptTag,
  SInt32 *        createdDirID)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSDeleteObject instead.*/
/*
 *  FSpDelete()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpDelete(const FSSpec * spec)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSGetCatalogInfo instead.*/
/*
 *  FSpGetFInfo()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpGetFInfo(
  const FSSpec *  spec,
  FInfo *         fndrInfo)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSSetCatalogInfo instead.*/
/*
 *  FSpSetFInfo()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpSetFInfo(
  const FSSpec *  spec,
  const FInfo *   fndrInfo)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSSetCatalogInfo instead.*/
/*
 *  FSpSetFLock()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpSetFLock(const FSSpec * spec)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSSetCatalogInfo instead.*/
/*
 *  FSpRstFLock()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpRstFLock(const FSSpec * spec)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSRenameUnicode instead.*/
/*
 *  FSpRename()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpRename(
  const FSSpec *     spec,
  ConstStr255Param   newName)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSMoveObject instead.*/
/*
 *  FSpCatMove()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpCatMove(
  const FSSpec *  source,
  const FSSpec *  dest)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. Use FSExchangeObjects instead.*/
/*
 *  FSpExchangeFiles()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
FSpExchangeFiles(
  const FSSpec *  source,
  const FSSpec *  dest)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBShareSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBShareSync(HParmBlkPtr paramBlock)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBShareAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBShareAsync(HParmBlkPtr paramBlock)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBUnshareSync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBUnshareSync(HParmBlkPtr paramBlock)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBUnshareAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBUnshareAsync(HParmBlkPtr paramBlock)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBGetUGEntrySync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetUGEntrySync(HParmBlkPtr paramBlock)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* This function is deprecated in Mac OS X 10.4. It was never implemented on OS X.*/
/*
 *  PBGetUGEntryAsync()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr  PBGetUGEntryAsync(HParmBlkPtr paramBlock)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
    The PBxxx() routines are obsolete.  
    
    Use the PBxxxSync() or PBxxxAsync() version instead.
*/
#define PBGetVInfo(pb, async) ((async) ? PBGetVInfoAsync(pb) : PBGetVInfoSync(pb))
#define PBXGetVolInfo(pb, async) ((async) ? PBXGetVolInfoAsync(pb) : PBXGetVolInfoSync(pb))
#define PBGetVol(pb, async) ((async) ? PBGetVolAsync(pb) : PBGetVolSync(pb))
#define PBSetVol(pb, async) ((async) ? PBSetVolAsync(pb) : PBSetVolSync(pb))
#define PBFlushVol(pb, async) ((async) ? PBFlushVolAsync(pb) : PBFlushVolSync(pb))
#define PBCreate(pb, async) ((async) ? PBCreateAsync(pb) : PBCreateSync(pb))
#define PBDelete(pb, async) ((async) ? PBDeleteAsync(pb) : PBDeleteSync(pb))
#define PBOpenDF(pb, async) ((async) ? PBOpenDFAsync(pb) : PBOpenDFSync(pb))
#define PBOpenRF(pb, async) ((async) ? PBOpenRFAsync(pb) : PBOpenRFSync(pb))
#define PBRename(pb, async) ((async) ? PBRenameAsync(pb) : PBRenameSync(pb))
#define PBGetFInfo(pb, async) ((async) ? PBGetFInfoAsync(pb) : PBGetFInfoSync(pb))
#define PBSetFInfo(pb, async) ((async) ? PBSetFInfoAsync(pb) : PBSetFInfoSync(pb))
#define PBSetFLock(pb, async) ((async) ? PBSetFLockAsync(pb) : PBSetFLockSync(pb))
#define PBRstFLock(pb, async) ((async) ? PBRstFLockAsync(pb) : PBRstFLockSync(pb))
#define PBSetFVers(pb, async) ((async) ? PBSetFVersAsync(pb) : PBSetFVersSync(pb))
#define PBAllocate(pb, async) ((async) ? PBAllocateAsync(pb) : PBAllocateSync(pb))
#define PBGetEOF(pb, async) ((async) ? PBGetEOFAsync(pb) : PBGetEOFSync(pb))
#define PBSetEOF(pb, async) ((async) ? PBSetEOFAsync(pb) : PBSetEOFSync(pb))
#define PBGetFPos(pb, async) ((async) ? PBGetFPosAsync(pb) : PBGetFPosSync(pb))
#define PBSetFPos(pb, async) ((async) ? PBSetFPosAsync(pb) : PBSetFPosSync(pb))
#define PBFlushFile(pb, async) ((async) ? PBFlushFileAsync(pb) : PBFlushFileSync(pb))
#define PBCatSearch(pb, async) ((async) ? PBCatSearchAsync(pb) : PBCatSearchSync(pb))
#define PBOpenWD(pb, async) ((async) ? PBOpenWDAsync(pb) : PBOpenWDSync(pb))
#define PBCloseWD(pb, async) ((async) ? PBCloseWDAsync(pb) : PBCloseWDSync(pb))
#define PBHSetVol(pb, async) ((async) ? PBHSetVolAsync(pb) : PBHSetVolSync(pb))
#define PBHGetVol(pb, async) ((async) ? PBHGetVolAsync(pb) : PBHGetVolSync(pb))
#define PBCatMove(pb, async) ((async) ? PBCatMoveAsync(pb) : PBCatMoveSync(pb))
#define PBDirCreate(pb, async) ((async) ? PBDirCreateAsync(pb) : PBDirCreateSync(pb))
#define PBGetWDInfo(pb, async) ((async) ? PBGetWDInfoAsync(pb) : PBGetWDInfoSync(pb))
#define PBGetFCBInfo(pb, async) ((async) ? PBGetFCBInfoAsync(pb) : PBGetFCBInfoSync(pb))
#define PBGetCatInfo(pb, async) ((async) ? PBGetCatInfoAsync(pb) : PBGetCatInfoSync(pb))
#define PBSetCatInfo(pb, async) ((async) ? PBSetCatInfoAsync(pb) : PBSetCatInfoSync(pb))
#define PBAllocContig(pb, async) ((async) ? PBAllocContigAsync(pb) : PBAllocContigSync(pb))
#define PBLockRange(pb, async) ((async) ? PBLockRangeAsync(pb) : PBLockRangeSync(pb))
#define PBUnlockRange(pb, async) ((async) ? PBUnlockRangeAsync(pb) : PBUnlockRangeSync(pb))
#define PBSetVInfo(pb, async) ((async) ? PBSetVInfoAsync(pb) : PBSetVInfoSync(pb))
#define PBHGetVInfo(pb, async) ((async) ? PBHGetVInfoAsync(pb) : PBHGetVInfoSync(pb))
#define PBHOpen(pb, async) ((async) ? PBHOpenAsync(pb) : PBHOpenSync(pb))
#define PBHOpenRF(pb, async) ((async) ? PBHOpenRFAsync(pb) : PBHOpenRFSync(pb))
#define PBHOpenDF(pb, async) ((async) ? PBHOpenDFAsync(pb) : PBHOpenDFSync(pb))
#define PBHCreate(pb, async) ((async) ? PBHCreateAsync(pb) : PBHCreateSync(pb))
#define PBHDelete(pb, async) ((async) ? PBHDeleteAsync(pb) : PBHDeleteSync(pb))
#define PBHRename(pb, async) ((async) ? PBHRenameAsync(pb) : PBHRenameSync(pb))
#define PBHRstFLock(pb, async) ((async) ? PBHRstFLockAsync(pb) : PBHRstFLockSync(pb))
#define PBHSetFLock(pb, async) ((async) ? PBHSetFLockAsync(pb) : PBHSetFLockSync(pb))
#define PBHGetFInfo(pb, async) ((async) ? PBHGetFInfoAsync(pb) : PBHGetFInfoSync(pb))
#define PBHSetFInfo(pb, async) ((async) ? PBHSetFInfoAsync(pb) : PBHSetFInfoSync(pb))
#define PBMakeFSSpec(pb, async) ((async) ? PBMakeFSSpecAsync(pb) : PBMakeFSSpecSync(pb))
#define PBHGetVolParms(pb, async) ((async) ? PBHGetVolParmsAsync(pb) : PBHGetVolParmsSync(pb))
#define PBHGetLogInInfo(pb, async) ((async) ? PBHGetLogInInfoAsync(pb) : PBHGetLogInInfoSync(pb))
#define PBHGetDirAccess(pb, async) ((async) ? PBHGetDirAccessAsync(pb) : PBHGetDirAccessSync(pb))
#define PBHSetDirAccess(pb, async) ((async) ? PBHSetDirAccessAsync(pb) : PBHSetDirAccessSync(pb))
#define PBHMapID(pb, async) ((async) ? PBHMapIDAsync(pb) : PBHMapIDSync(pb))
#define PBHMapName(pb, async) ((async) ? PBHMapNameAsync(pb) : PBHMapNameSync(pb))
#define PBHCopyFile(pb, async) ((async) ? PBHCopyFileAsync(pb) : PBHCopyFileSync(pb))
#define PBHMoveRename(pb, async) ((async) ? PBHMoveRenameAsync(pb) : PBHMoveRenameSync(pb))
#define PBHOpenDeny(pb, async) ((async) ? PBHOpenDenyAsync(pb) : PBHOpenDenySync(pb))
#define PBHOpenRFDeny(pb, async) ((async) ? PBHOpenRFDenyAsync(pb) : PBHOpenRFDenySync(pb))
#define PBExchangeFiles(pb, async) ((async) ? PBExchangeFilesAsync(pb) : PBExchangeFilesSync(pb))
#define PBCreateFileIDRef(pb, async) ((async) ? PBCreateFileIDRefAsync(pb) : PBCreateFileIDRefSync(pb))
#define PBResolveFileIDRef(pb, async) ((async) ? PBResolveFileIDRefAsync(pb) : PBResolveFileIDRefSync(pb))
#define PBDeleteFileIDRef(pb, async) ((async) ? PBDeleteFileIDRefAsync(pb) : PBDeleteFileIDRefSync(pb))
#define PBGetForeignPrivs(pb, async) ((async) ? PBGetForeignPrivsAsync(pb) : PBGetForeignPrivsSync(pb))
#define PBSetForeignPrivs(pb, async) ((async) ? PBSetForeignPrivsAsync(pb) : PBSetForeignPrivsSync(pb))
#define PBDTAddIcon(pb, async) ((async) ? PBDTAddIconAsync(pb) : PBDTAddIconSync(pb))
#define PBDTGetIcon(pb, async) ((async) ? PBDTGetIconAsync(pb) : PBDTGetIconSync(pb))
#define PBDTGetIconInfo(pb, async) ((async) ? PBDTGetIconInfoAsync(pb) : PBDTGetIconInfoSync(pb))
#define PBDTAddAPPL(pb, async) ((async) ? PBDTAddAPPLAsync(pb) : PBDTAddAPPLSync(pb))
#define PBDTRemoveAPPL(pb, async) ((async) ? PBDTRemoveAPPLAsync(pb) : PBDTRemoveAPPLSync(pb))
#define PBDTGetAPPL(pb, async) ((async) ? PBDTGetAPPLAsync(pb) : PBDTGetAPPLSync(pb))
#define PBDTSetComment(pb, async) ((async) ? PBDTSetCommentAsync(pb) : PBDTSetCommentSync(pb))
#define PBDTRemoveComment(pb, async) ((async) ? PBDTRemoveCommentAsync(pb) : PBDTRemoveCommentSync(pb))
#define PBDTGetComment(pb, async) ((async) ? PBDTGetCommentAsync(pb) : PBDTGetCommentSync(pb))
#define PBDTFlush(pb, async) ((async) ? PBDTFlushAsync(pb) : PBDTFlushSync(pb))
#define PBDTReset(pb, async) ((async) ? PBDTResetAsync(pb) : PBDTResetSync(pb))
#define PBDTGetInfo(pb, async) ((async) ? PBDTGetInfoAsync(pb) : PBDTGetInfoSync(pb))
#define PBDTDelete(pb, async) ((async) ? PBDTDeleteAsync(pb) : PBDTDeleteSync(pb))
#endif  /* !__LP64__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __FILES__ */

