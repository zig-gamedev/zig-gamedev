
#ifndef __IONDRVLIBRARIES__
#define __IONDRVLIBRARIES__

#include <libkern/OSByteOrder.h>
#include <libkern/OSAtomic.h>
#include <IOKit/ndrvsupport/IOMacOSTypes.h>
#include <IOKit/graphics/IOGraphicsTypes.h>


#ifdef __cplusplus
extern "C" {
#endif

/* NameRegistry error codes */
enum {
    nrLockedErr                         = -2536,
    nrNotEnoughMemoryErr                = -2537,
    nrInvalidNodeErr                    = -2538,
    nrNotFoundErr                       = -2539,
    nrNotCreatedErr                     = -2540,
    nrNameErr                           = -2541,
    nrNotSlotDeviceErr                  = -2542,
    nrDataTruncatedErr                  = -2543,
    nrPowerErr                          = -2544,
    nrPowerSwitchAbortErr               = -2545,
    nrTypeMismatchErr                   = -2546,
    nrNotModifiedErr                    = -2547,
    nrOverrunErr                        = -2548,
    nrResultCodeBase                    = -2549,
    nrPathNotFound                      = -2550,    /* a path component lookup failed */
    nrPathBufferTooSmall                = -2551,    /* buffer for path is too small */
    nrInvalidEntryIterationOp           = -2552,    /* invalid entry iteration operation */
    nrPropertyAlreadyExists             = -2553,    /* property already exists */
    nrIterationDone                     = -2554,    /* iteration operation is done */
    nrExitedIteratorScope               = -2555,    /* outer scope of iterator was exited */
    nrTransactionAborted                = -2556,        /* transaction was aborted */

    gestaltUndefSelectorErr             = -5551 /*undefined selector was passed to Gestalt*/
};

enum {
    kNVRAMProperty                      = 0x00000020,            // matches NR
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _IOKIT_IOSERVICE_H
typedef struct IOService IOService;
#endif

IOReturn _IONDRVLibrariesInitialize( IOService * provider );
IOReturn _IONDRVLibrariesFinalize( IOService * provider );

#ifndef kAAPLRegEntryIDKey
#define kAAPLRegEntryIDKey      "AAPL,RegEntryID"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*******************************************************************************
 * 
 * Foundation Types
 *
 */
/* Value of a property */
typedef void *                          RegPropertyValue;
/* Length of property value */
typedef UInt32                          RegPropertyValueSize;

/*******************************************************************************/

/*******************************************************************************
 *
 * Root Entry Name Definitions  (Applies to all Names in the RootNameSpace)
 *
 *  + Names are a colon-separated list of name components.  Name components
 *    may not themselves contain colons.  
 *  + Names are presented as null-terminated ASCII character strings.
 *  + Names follow similar parsing rules to Apple file system absolute
 *    and relative paths.  However the '::' parent directory syntax is
 *    not currently supported.
 */
/* Max length of Entry Name */
enum {
  kRegCStrMaxEntryNameLength    = 47
};

/* Entry Names are single byte ASCII */
typedef char                            RegCStrEntryName;
typedef char *                          RegCStrEntryNamePtr;
/* length of RegCStrEntryNameBuf =  kRegCStrMaxEntryNameLength+1*/
typedef char                            RegCStrEntryNameBuf[48];
typedef char                            RegCStrPathName;
typedef UInt32                          RegPathNameSize;
enum {
  kRegPathNameSeparator         = ':',  /* 0x3A */
  kRegEntryNameTerminator       = 0x00, /* '\0' */
  kRegPathNameTerminator        = 0x00  /* '\0' */
};

/*******************************************************************************
 *
 * Property Name and ID Definitions
 *  (Applies to all Properties Regardless of NameSpace)
 */
enum {
  kRegMaximumPropertyNameLength = 31,   /* Max length of Property Name */
  kRegPropertyNameTerminator    = 0x00  /* '\0' */
};

typedef char                            RegPropertyNameBuf[32];
typedef char                            RegPropertyName;
typedef char *                          RegPropertyNamePtr;
enum {
  kRegMaxPropertyNameLength     = kRegMaximumPropertyNameLength
};

/*******************************************************************************
 *
 * Iteration Operations
 *
 *  These specify direction when traversing the name relationships
 */
typedef UInt32                          RegIterationOp;
typedef RegIterationOp                  RegEntryIterationOp;
enum {
                                        /* Absolute locations*/
  kRegIterRoot                  = 0x00000002, /* "Upward" Relationships */
  kRegIterParents               = 0x00000003, /* include all  parent(s) of entry */
                                        /* "Downward" Relationships*/
  kRegIterChildren              = 0x00000004, /* include all children */
  kRegIterSubTrees              = 0x00000005, /* include all sub trees of entry */
  kRegIterDescendants           = 0x00000005, /* include all descendants of entry */
                                        /* "Horizontal" Relationships */
  kRegIterSibling               = 0x00000006, /* include all siblings */
                                        /* Keep doing the same thing*/
  kRegIterContinue              = 0x00000001
};

/*******************************************************************************
 *
 * Name Entry and Property Modifiers
 *
 *
 *
 * Modifiers describe special characteristics of names
 * and properties.  Modifiers might be supported for
 * some names and not others.
 * 
 * Device Drivers should not rely on functionality
 * specified as a modifier.
 */
typedef UInt32                          RegModifiers;
typedef RegModifiers                    RegEntryModifiers;
typedef RegModifiers                    RegPropertyModifiers;
enum {
  kRegNoModifiers               = 0x00000000, /* no entry modifiers in place */
  kRegUniversalModifierMask     = 0x0000FFFF, /* mods to all entries */
  kRegNameSpaceModifierMask     = 0x00FF0000, /* mods to all entries within namespace */
  kRegModifierMask              = (RegModifiers)0xFF000000 /* mods to just this entry */
};

/* Universal Property Modifiers */
enum {
  kRegPropertyValueIsSavedToNVRAM = 0x00000020, /* property is non-volatile (saved in NVRAM) */
  kRegPropertyValueIsSavedToDisk = 0x00000040 /* property is non-volatile (saved on disk) */
};

typedef size_t Size;

#ifndef _IOKIT_IOREGISTRYENTRY_H
typedef struct IORegistryIterator IORegistryIterator;
typedef struct OSIterator OSIterator;
#endif /* _IOKIT_IOREGISTRYENTRY_H */
typedef IORegistryIterator * RegEntryIter;
typedef OSIterator * RegPropertyIter;

OSStatus RegistryEntryIDCopy( const RegEntryID * entryID, RegEntryID * to );

OSStatus RegistryEntryIDInit( RegEntryID * entryID );

Boolean RegistryEntryIDCompare( const RegEntryID * id1, const RegEntryID * id2);

OSStatus RegistryPropertyGetSize(
  const RegEntryID *       entryID,
  const RegPropertyName *  propertyName,
  RegPropertyValueSize *   propertySize);

OSStatus RegistryPropertyGet(
  const RegEntryID *       entryID,
  const RegPropertyName *  propertyName,
  void *                   propertyValue,
  RegPropertyValueSize *   propertySize);

OSStatus RegistryPropertyCreate(
  const RegEntryID *       entryID,
  const RegPropertyName *  propertyName,
  const void *             propertyValue,
  RegPropertyValueSize     propertySize);

OSStatus RegistryPropertyDelete(
  const RegEntryID *       entryID,
  const RegPropertyName *  propertyName);

OSStatus RegistryPropertySet(
  const RegEntryID *       entryID,
  const RegPropertyName *  propertyName,
  const void *             propertyValue,
  RegPropertyValueSize     propertySize);

OSStatus RegistryPropertyGetMod(
  const RegEntryID *       entry,
  const RegPropertyName *  name,
  RegPropertyModifiers *   modifiers);

OSStatus RegistryPropertySetMod(
  const RegEntryID *       entry,
  const RegPropertyName *  name,
  RegPropertyModifiers     modifiers);

OSStatus RegistryPropertyIterateCreate(
  const RegEntryID *  entry,
  RegPropertyIter *   cookie);

OSStatus RegistryPropertyIterateDispose( RegPropertyIter * cookie);

OSStatus RegistryPropertyIterate(
  RegPropertyIter *  cookie,
  RegPropertyName *  foundProperty,
  Boolean *          done);

OSStatus RegistryEntryIterateCreate( RegEntryIter * cookie);

OSStatus RegistryEntryIterateDispose( RegEntryIter * cookie);

OSStatus RegistryEntryIterate( RegEntryIter *   cookie,
                        RegEntryIterationOp     relationship,
                        RegEntryID *    foundEntry,
                        Boolean *       done);

OSStatus RegistryCStrEntryToName( const RegEntryID *    entryID,
                            RegEntryID *                parentEntry,
                            RegCStrEntryName *          nameComponent,
                            Boolean *                   done );

OSStatus RegistryCStrEntryLookup(  const RegEntryID *   parentEntry,
                            const RegCStrPathName *     path,
                            RegEntryID *                newEntry);

OSStatus RegistryEntryIDDispose(RegEntryID * entryID);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum {
  paramErr                      = -50,  /*error in user parameter list*/
  noHardwareErr                 = -200, /*Sound Manager Error Returns*/
  notEnoughHardwareErr          = -201, /*Sound Manager Error Returns*/
  userCanceledErr               = -128,
  qErr                          = -1,   /*queue element not found during deletion*/
  vTypErr                       = -2,   /*invalid queue element*/
  corErr                        = -3,   /*core routine number out of range*/
  unimpErr                      = -4,   /*unimplemented core routine*/
  SlpTypeErr                    = -5,   /*invalid queue element*/
  seNoDB                        = -8,   /*no debugger installed to handle debugger command*/
  controlErr                    = -17,  /*I/O System Errors*/
  statusErr                     = -18,  /*I/O System Errors*/
  readErr                       = -19,  /*I/O System Errors*/
  writErr                       = -20,  /*I/O System Errors*/
  badUnitErr                    = -21,  /*I/O System Errors*/
  unitEmptyErr                  = -22,  /*I/O System Errors*/
  openErr                       = -23,  /*I/O System Errors*/
  closErr                       = -24,  /*I/O System Errors*/
  dRemovErr                     = -25,  /*tried to remove an open driver*/
  dInstErr                      = -26,   /*DrvrInstall couldn't find driver in resources*/

  badCksmErr                    = -69,  /*addr mark checksum didn't check*/
};
enum {
  durationMicrosecond           = -1,  /* Microseconds are negative*/
  durationMillisecond           = 1,   /* Milliseconds are positive*/
  durationSecond                = 1000, /* 1000 * durationMillisecond*/
  durationMinute                = 60000, /* 60 * durationSecond,*/
  durationHour                  = 3600000, /* 60 * durationMinute,*/
  durationDay                   = 86400000, /* 24 * durationHour,*/
  durationNoWait                = 0,   /* don't block*/
  durationForever               = 0x7FFFFFFF /* no time limit*/
};
#ifndef NULL
    #if !defined(__cplusplus) && (defined(__SC__) || defined(THINK_C))
        /* Symantec C compilers (but not C++) want NULL and nil to be (void*)0  */
        #define NULL ((void *) 0)
    #else
        /* in case int is 16-bits, make sure NULL is 32-bits */
        #define NULL 0L
    #endif
#endif

#ifndef nil
    #define nil NULL
#endif

enum {
  clutType                      = 0,    /*0 if lookup table*/
  fixedType                     = 1,    /*1 if fixed table*/
  directType                    = 2,    /*2 if direct values*/
  RGBDirect                     = 16   /* 16 & 32 bits/pixel pixelType value */
};
typedef UInt32 *                        UInt32Ptr;

typedef struct IOHardwareCursorDescriptor HardwareCursorDescriptorRec;
typedef HardwareCursorDescriptorRec *     HardwareCursorDescriptorPtr;
typedef struct IOHardwareCursorInfo       HardwareCursorInfoRec;
typedef HardwareCursorInfoRec *           HardwareCursorInfoPtr;

typedef ResType                         InterruptServiceType;
typedef struct _VSLService *            InterruptServiceIDType;
typedef InterruptServiceIDType *        InterruptServiceIDPtr;

enum {
  kVBLInterruptServiceType      = 'vbl ',
  kHBLInterruptServiceType      = 'hbl ',
  kFrameInterruptServiceType    = 'fram',
  kConnectInterruptServiceType  = 'dci ', /* Renamed -- Use kFBCheckInterruptServiceType*/
  kFBConnectInterruptServiceType = kConnectInterruptServiceType, /* Demand to check configuration (Hardware unchanged)*/
  kFBChangedInterruptServiceType = 'chng', /* Demand to rebuild (Hardware has reinitialized on dependent change)*/
  kFBOfflineInterruptServiceType = 'remv', /* Demand to remove framebuffer (Hardware not available on dependent change -- but must not buserror)*/
  kFBOnlineInterruptServiceType = 'add ' /* Notice that hardware is available (after being removed)*/
};

enum {
    kVSLClamshellStateGestaltType = 'clam',
};

OSErr
VSLNewInterruptService(
  RegEntryID *            serviceDevice,
  InterruptServiceType    serviceType,
  InterruptServiceIDPtr   serviceID);

OSErr
VSLDisposeInterruptService(InterruptServiceIDType serviceID);

OSErr
VSLDoInterruptService(InterruptServiceIDType serviceID);

Boolean
VSLPrepareCursorForHardwareCursor(
  void *                        cursorRef,
  IOHardwareCursorDescriptor *  hardwareDescriptor,
  IOHardwareCursorInfo *        hwCursorInfo);


enum {
                                        /* Version Release Stage Codes */
  developStage                  = 0x20,
  alphaStage                    = 0x40,
  betaStage                     = 0x60,
  finalStage                    = 0x80
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct OpaqueIOCommandID*       IOCommandID;

typedef UInt32 IOCommandKind;
typedef UInt32 IOCommandCode;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef __cplusplus
}
#endif

#endif /* __IONDRVLIBRARIES__ */

