/*
     File:       CarbonCore/CodeFragments.h
 
     Contains:   Public Code Fragment Manager Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1992-2011 by Apple Inc. All rights reserved.
*/
/*
   ===========================================================================================
   The Code Fragment Manager API
   =============================
*/


#ifndef __CODEFRAGMENTS__
#define __CODEFRAGMENTS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif


#ifndef __CFBUNDLE__
#include <CoreFoundation/CFBundle.h>
#endif

#ifndef __FILES__
#include <CarbonCore/Files.h>
#endif

#ifndef __MULTIPROCESSING__
#include <CarbonCore/Multiprocessing.h>
#endif




#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

#if !__LP64__
/*
   ¤
   ===========================================================================================
   General Types and Constants
   ===========================
*/

enum {
  kCFragResourceType            = 'cfrg',
  kCFragResourceID              = 0,
  kCFragLibraryFileType         = 'shlb',
  kCFragAllFileTypes            = (int)0xFFFFFFFF
};


typedef OSType                          CFragArchitecture;
enum {
                                        /* Values for type CFragArchitecture.*/
  kPowerPCCFragArch             = 'pwpc',
  kMotorola68KCFragArch         = 'm68k',
  kAnyCFragArch                 = 0x3F3F3F3F
};


#if TARGET_CPU_PPC
enum {
  kCompiledCFragArch            = kPowerPCCFragArch
};

#endif  /* TARGET_CPU_PPC */

#if TARGET_CPU_X86 || TARGET_CPU_X86_64
enum {
  kCompiledCFragArch            = 'none'
};

#endif  /* TARGET_CPU_X86 || TARGET_CPU_X86_64 */

typedef UInt32                          CFragVersionNumber;
enum {
  kNullCFragVersion             = 0,
  kWildcardCFragVersion         = (int)0xFFFFFFFF
};


typedef UInt8                           CFragUsage;
enum {
                                        /* Values for type CFragUsage.*/
  kImportLibraryCFrag           = 0,    /* Standard CFM import library.*/
  kApplicationCFrag             = 1,    /* MacOS application.*/
  kDropInAdditionCFrag          = 2,    /* Application or library private extension/plug-in*/
  kStubLibraryCFrag             = 3,    /* Import library used for linking only*/
  kWeakStubLibraryCFrag         = 4     /* Import library used for linking only and will be automatically weak linked*/
};


enum {
  kIsCompleteCFrag              = 0,    /* A "base" fragment, not an update.*/
  kFirstCFragUpdate             = 1     /* The first update, others are numbered 2, 3, ...*/
};


enum {
  kCFragGoesToEOF               = 0
};




typedef UInt8                           CFragLocatorKind;
enum {
                                        /* Values for type CFragLocatorKind.*/
  kMemoryCFragLocator           = 0,    /* Container is in memory.*/
  kDataForkCFragLocator         = 1,    /* Container is in a file's data fork.*/
  kResourceCFragLocator         = 2,    /* Container is in a file's resource fork.*/
  kNamedFragmentCFragLocator    = 4,    /* ! Reserved for possible future use!*/
  kCFBundleCFragLocator         = 5,    /* Container is in the executable of a CFBundle*/
  kCFBundlePreCFragLocator      = 6     /* passed to init routines in lieu of kCFBundleCFragLocator*/
};


/*
   --------------------------------------------------------------------------------------
   A 'cfrg' resource consists of a header followed by a sequence of variable length
   members.  The constant kDefaultCFragNameLen only provides for a legal ANSI declaration
   and for a reasonable display in a debugger.  The actual name field is cut to fit.
   There may be "extensions" after the name, the memberSize field includes them.  The
   general form of an extension is a 16 bit type code followed by a 16 bit size in bytes.
   Only one standard extension type is defined at present, it is used by SOM's searching
   mechanism.
*/


union CFragUsage1Union {                      /* ! Meaning differs depending on value of "usage".*/
  UInt32              appStackSize;           /* If the fragment is an application. (Not used by CFM!)*/
};
typedef union CFragUsage1Union          CFragUsage1Union;
union CFragUsage2Union {                      /* ! Meaning differs depending on value of "usage".*/
  SInt16              appSubdirID;            /* If the fragment is an application.*/
  UInt16              libFlags;               /* If the fragment is an import library.*/
};
typedef union CFragUsage2Union          CFragUsage2Union;
enum {
                                        /* Bit masks for the CFragUsage2Union libFlags variant.*/
  kCFragLibUsageMapPrivatelyMask = 0x0001 /* Put container in app heap if necessary.*/
};

union CFragWhere1Union {                      /* ! Meaning differs depending on value of "where".*/
  UInt32              spaceID;                /* If the fragment is in memory.  (Actually an AddressSpaceID.)*/
};
typedef union CFragWhere1Union          CFragWhere1Union;
union CFragWhere2Union {                      /* ! Meaning differs depending on value of "where".*/
  UInt16              reserved;
};
typedef union CFragWhere2Union          CFragWhere2Union;
enum {
  kDefaultCFragNameLen          = 16
};


struct CFragResourceMember {
  CFragArchitecture   architecture;
  UInt16              reservedA;              /* ! Must be zero!*/
  UInt8               reservedB;              /* ! Must be zero!*/
  UInt8               updateLevel;
  CFragVersionNumber  currentVersion;
  CFragVersionNumber  oldDefVersion;
  CFragUsage1Union    uUsage1;
  CFragUsage2Union    uUsage2;
  CFragUsage          usage;
  CFragLocatorKind    where;
  UInt32              offset;
  UInt32              length;
  CFragWhere1Union    uWhere1;
  CFragWhere2Union    uWhere2;
  UInt16              extensionCount;         /* The number of extensions beyond the name.*/
  UInt16              memberSize;             /* Size in bytes, includes all extensions.*/
  unsigned char       name[16];               /* ! Actually a sized PString.*/
};
typedef struct CFragResourceMember      CFragResourceMember;
typedef CFragResourceMember *           CFragResourceMemberPtr;
struct CFragResourceExtensionHeader {
  UInt16              extensionKind;
  UInt16              extensionSize;
};
typedef struct CFragResourceExtensionHeader CFragResourceExtensionHeader;
typedef CFragResourceExtensionHeader *  CFragResourceExtensionHeaderPtr;
struct CFragResourceSearchExtension {
  CFragResourceExtensionHeader  header;
  OSType              libKind;
  unsigned char       qualifiers[1];          /* ! Actually four PStrings.*/
};
typedef struct CFragResourceSearchExtension CFragResourceSearchExtension;
typedef CFragResourceSearchExtension *  CFragResourceSearchExtensionPtr;
enum {
  kCFragResourceSearchExtensionKind = 0x30EE
};


struct CFragResource {
  UInt32              reservedA;              /* ! Must be zero!*/
  UInt32              reservedB;              /* ! Must be zero!*/
  UInt16              reservedC;              /* ! Must be zero!*/
  UInt16              version;
  UInt32              reservedD;              /* ! Must be zero!*/
  UInt32              reservedE;              /* ! Must be zero!*/
  UInt32              reservedF;              /* ! Must be zero!*/
  UInt32              reservedG;              /* ! Must be zero!*/
  UInt16              reservedH;              /* ! Must be zero!*/
  UInt16              memberCount;
  CFragResourceMember  firstMember;
};
typedef struct CFragResource            CFragResource;
typedef CFragResource *                 CFragResourcePtr;
typedef CFragResourcePtr *              CFragResourceHandle;
enum {
  kCurrCFragResourceVersion     = 1
};


#define AlignToFour(aValue) (((aValue) + 3) & ~3)
#define CFMOffsetOf(structure,field) ((UInt32)&((structure *) 0)->field)
#define kBaseCFragResourceMemberSize    (CFMOffsetOf ( CFragResourceMember, name ) )
#define kBaseCFragResourceSize          (CFMOffsetOf ( CFragResource, firstMember.name ) )
#define NextCFragResourceMemberPtr(aMemberPtr)  \
        ((CFragResourceMemberPtr) ((BytePtr)aMemberPtr + aMemberPtr->memberSize))
#define FirstCFragResourceExtensionPtr(aMemberPtr)                                          \
        ((CFragResourceExtensionHeaderPtr) ((BytePtr)aMemberPtr +                           \
                                            AlignToFour ( kBaseCFragResourceMemberSize +    \
                                                          aMemberPtr->name[0] + 1 ) ))
#define NextCFragResourceExtensionPtr(anExtensionPtr)                   \
        ((CFragResourceExtensionHeaderPtr) ((BytePtr)anExtensionPtr +   \
                                            ((CFragResourceExtensionHeaderPtr)anExtensionPtr)->extensionSize ))
#define FirstCFragResourceSearchQualifier(searchExtensionPtr)                   \
        ((StringPtr) ((BytePtr)searchExtensionPtr +                             \
                      CFMOffsetOf ( CFragResourceSearchExtension, qualifiers ) ))
#define NextCFragResourceSearchQualifier(searchQualifierPtr)    \
        ((StringPtr) ((BytePtr)searchQualifierPtr + searchQualifierPtr[0] + 1))

typedef MPProcessID                     CFragContextID;
typedef struct OpaqueCFragConnectionID*  CFragConnectionID;
typedef struct OpaqueCFragClosureID*    CFragClosureID;
typedef struct OpaqueCFragContainerID*  CFragContainerID;
typedef OptionBits                      CFragLoadOptions;
enum {
                                        /* Values for type CFragLoadOptions.*/
  kReferenceCFrag               = 0x0001, /* Try to use existing copy, increment reference counts.*/
  kFindCFrag                    = 0x0002, /* Try find an existing copy, do not increment reference counts.*/
  kPrivateCFragCopy             = 0x0005 /* Prepare a new private copy.  (kReferenceCFrag | 0x0004)*/
};


enum {
  kUnresolvedCFragSymbolAddress = 0
};


typedef UInt8                           CFragSymbolClass;
enum {
                                        /* Values for type CFragSymbolClass.*/
  kCodeCFragSymbol              = 0,
  kDataCFragSymbol              = 1,
  kTVectorCFragSymbol           = 2,
  kTOCCFragSymbol               = 3,
  kGlueCFragSymbol              = 4
};


/*
   ¤
   ===========================================================================================
   Macros and Functions
   ====================
*/


#define CFragHasFileLocation(where) \
        ( ((where) == kDataForkCFragLocator) || ((where) == kResourceCFragLocator) )
#if !__LP64__
/*
 *  GetSharedLibrary()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    The connID, mainAddr, and errMessage parameters may be NULL with
 *    MacOS 8.5 and later. Passing NULL as those parameters when
 *    running Mac OS 8.1 and earlier systems will corrupt low-memory.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CFragManager 1.0 and later
 */
extern OSErr 
GetSharedLibrary(
  ConstStr63Param      libName,
  CFragArchitecture    archType,
  CFragLoadOptions     options,
  CFragConnectionID *  connID,
  Ptr *                mainAddr,
  Str255               errMessage)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetDiskFragment()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CFragManager 1.0 and later
 */
extern OSErr 
GetDiskFragment(
  const FSSpec *       fileSpec,
  UInt32               offset,
  UInt32               length,
  ConstStr63Param      fragName,         /* can be NULL */
  CFragLoadOptions     options,
  CFragConnectionID *  connID,           /* can be NULL */
  Ptr *                mainAddr,         /* can be NULL */
  Str255               errMessage)       /* can be NULL */    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetMemFragment()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CFragManager 1.0 and later
 */
extern OSErr 
GetMemFragment(
  void *               memAddr,
  UInt32               length,
  ConstStr63Param      fragName,         /* can be NULL */
  CFragLoadOptions     options,
  CFragConnectionID *  connID,           /* can be NULL */
  Ptr *                mainAddr,         /* can be NULL */
  Str255               errMessage)       /* can be NULL */    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  CloseConnection()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CFragManager 1.0 and later
 */
extern OSErr 
CloseConnection(CFragConnectionID * connID)                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  FindSymbol()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CFragManager 1.0 and later
 */
extern OSErr 
FindSymbol(
  CFragConnectionID   connID,
  ConstStr255Param    symName,
  Ptr *               symAddr,        /* can be NULL */
  CFragSymbolClass *  symClass)       /* can be NULL */       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  CountSymbols()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CFragManager 1.0 and later
 */
extern OSErr 
CountSymbols(
  CFragConnectionID   connID,
  long *              symCount)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetIndSymbol()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CFragManager 1.0 and later
 */
extern OSErr 
GetIndSymbol(
  CFragConnectionID   connID,
  long                symIndex,
  Str255              symName,        /* can be NULL */
  Ptr *               symAddr,        /* can be NULL */
  CFragSymbolClass *  symClass)       /* can be NULL */       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
   ¤
   ===========================================================================================
   Initialization & Termination Routines
   =====================================
*/


/*
   -----------------------------------------------------------------------------------------
   A fragment's initialization and termination routines are called when a new incarnation of
   the fragment is created or destroyed, respectively.  Exactly when this occurs depends on
   what kinds of section sharing the fragment has and how the fragment is prepared.  Import
   libraries have at most one incarnation per process.  Fragments prepared with option
   kPrivateCFragCopy may have many incarnations per process.
   The initialization function is passed a pointer to an initialization information structure
   and returns an OSErr.  If an initialization function returns a non-zero value the entire
   closure of which it is a part fails.  The C prototype for an initialization function is:
        OSErr   CFragInitFunction   ( const CFragInitBlock *    initBlock );
   The termination procedure takes no parameters and returns nothing.  The C prototype for a
   termination procedure is:
        void    CFragTermProcedure  ( void );
   Note that since the initialization and termination routines are themselves "CFM"-style
   routines whether or not they have the "pascal" keyword is irrelevant.
*/


/*
   -----------------------------------------------------------------------------------------
   ! Note:
   ! The "System7" portion of these type names was introduced during the evolution towards
   ! the now defunct Copland version of Mac OS.  Copland was to be called System 8 and there
   ! were slightly different types for System 7 and System 8.  The "generic" type names were
   ! conditionally defined for the desired target system.
   ! Always use the generic types, e.g. CFragInitBlock!  The "System7" names have been kept
   ! only to avoid perturbing code that (improperly) used the target specific type.
*/


#endif  /* !__LP64__ */

struct CFragSystem7MemoryLocator {
  LogicalAddress      address;
  UInt32              length;
  Boolean             inPlace;
  UInt8               reservedA;              /* ! Must be zero!*/
  UInt16              reservedB;              /* ! Must be zero!*/
};
typedef struct CFragSystem7MemoryLocator CFragSystem7MemoryLocator;
struct CFragSystem7DiskFlatLocator {
  FSSpec *            fileSpec;
  UInt32              offset;
  UInt32              length;
};
typedef struct CFragSystem7DiskFlatLocator CFragSystem7DiskFlatLocator;
/* ! This must have a file specification at the same offset as a disk flat locator!*/
struct CFragSystem7SegmentedLocator {
  FSSpec *            fileSpec;
  OSType              rsrcType;
  SInt16              rsrcID;
  UInt16              reservedA;              /* ! Must be zero!*/
};
typedef struct CFragSystem7SegmentedLocator CFragSystem7SegmentedLocator;
/*
   The offset and length for a "Bundle" locator refers to the offset in
   the CFM executable contained by the bundle.
*/
struct CFragCFBundleLocator {
  CFBundleRef         fragmentBundle;         /* Do not call CFRelease on this bundle!*/
  UInt32              offset;
  UInt32              length;
};
typedef struct CFragCFBundleLocator     CFragCFBundleLocator;
struct CFragSystem7Locator {
  SInt32              where;
  union {
    CFragSystem7DiskFlatLocator  onDisk;
    CFragSystem7MemoryLocator  inMem;
    CFragSystem7SegmentedLocator  inSegs;
    CFragCFBundleLocator  inBundle;
  }                       u;
};
typedef struct CFragSystem7Locator      CFragSystem7Locator;
typedef CFragSystem7Locator *           CFragSystem7LocatorPtr;
struct CFragSystem7InitBlock {
  CFragContextID      contextID;
  CFragClosureID      closureID;
  CFragConnectionID   connectionID;
  CFragSystem7Locator  fragLocator;
  StringPtr           libName;
  UInt32              reservedA;              /* ! Must be zero!*/
};
typedef struct CFragSystem7InitBlock    CFragSystem7InitBlock;
typedef CFragSystem7InitBlock *         CFragSystem7InitBlockPtr;
typedef CFragSystem7InitBlock           CFragInitBlock;
typedef CFragSystem7InitBlockPtr        CFragInitBlockPtr;
/* These init/term routine types are only of value to CFM itself.*/
typedef CALLBACK_API_C( OSErr , CFragInitFunction )(const CFragInitBlock * initBlock);
typedef CALLBACK_API_C( void , CFragTermProcedure )(void);
/* For use by init routines. If you get a BundlePreLocator, convert it to a CFBundleLocator with this*/
#if !__LP64__
/*
 *  ConvertBundlePreLocator()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
ConvertBundlePreLocator(CFragSystem7LocatorPtr initBlockLocator) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);




/*
   ¤
   ===========================================================================================
   Old Name Spellings
   ==================
*/


/*
   -------------------------------------------------------------------------------------------
   We've tried to reduce the risk of name collisions in the future by introducing the phrase
   "CFrag" into constant and type names.  The old names are defined below in terms of the new.
*/


#endif  /* !__LP64__ */

enum {
  kLoadCFrag                    = kReferenceCFrag
};


#if OLDROUTINENAMES
#define IsFileLocation      CFragHasFileLocation
typedef CFragConnectionID               ConnectionID;
typedef CFragLoadOptions                LoadFlags;
typedef CFragSymbolClass                SymClass;
typedef CFragInitBlock                  InitBlock;
typedef CFragInitBlockPtr               InitBlockPtr;
typedef CFragSystem7MemoryLocator       MemFragment;
typedef CFragSystem7DiskFlatLocator     DiskFragment;
typedef CFragSystem7SegmentedLocator    SegmentedFragment;
typedef CFragSystem7Locator             FragmentLocator;
typedef CFragSystem7LocatorPtr          FragmentLocatorPtr;
typedef CFragSystem7MemoryLocator       CFragHFSMemoryLocator;
typedef CFragSystem7DiskFlatLocator     CFragHFSDiskFlatLocator;
typedef CFragSystem7SegmentedLocator    CFragHFSSegmentedLocator;
typedef CFragSystem7Locator             CFragHFSLocator;
typedef CFragSystem7LocatorPtr          CFragHFSLocatorPtr;
enum {
  kPowerPCArch                  = kPowerPCCFragArch,
  kMotorola68KArch              = kMotorola68KCFragArch,
  kAnyArchType                  = kAnyCFragArch,
  kNoLibName                    = 0,
  kNoConnectionID               = 0,
  kLoadLib                      = kLoadCFrag,
  kFindLib                      = kFindCFrag,
  kNewCFragCopy                 = kPrivateCFragCopy,
  kLoadNewCopy                  = kPrivateCFragCopy,
  kUseInPlace                   = 0x80,
  kCodeSym                      = kCodeCFragSymbol,
  kDataSym                      = kDataCFragSymbol,
  kTVectSym                     = kTVectorCFragSymbol,
  kTOCSym                       = kTOCCFragSymbol,
  kGlueSym                      = kGlueCFragSymbol,
  kInMem                        = kMemoryCFragLocator,
  kOnDiskFlat                   = kDataForkCFragLocator,
  kOnDiskSegmented              = kResourceCFragLocator,
  kIsLib                        = kImportLibraryCFrag,
  kIsApp                        = kApplicationCFrag,
  kIsDropIn                     = kDropInAdditionCFrag,
  kFullLib                      = kIsCompleteCFrag,
  kUpdateLib                    = kFirstCFragUpdate,
  kWholeFork                    = kCFragGoesToEOF,
  kCFMRsrcType                  = kCFragResourceType,
  kCFMRsrcID                    = kCFragResourceID,
  kSHLBFileType                 = kCFragLibraryFileType,
  kUnresolvedSymbolAddress      = kUnresolvedCFragSymbolAddress
};

enum {
  kPowerPC                      = kPowerPCCFragArch,
  kMotorola68K                  = kMotorola68KCFragArch
};

#endif  /* OLDROUTINENAMES */

#endif  /* !__LP64__ */






#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __CODEFRAGMENTS__ */

