/*
     File:       CarbonCore/Components.h
 
     Contains:   Component Manager Interfaces.
                 The contents of this header file are deprecated.
                 Use Foundation or CoreFoundation bundles and plugins instead.
 
     Copyright:  © 1991-2011 by Apple Inc. All rights reserved.
*/
#ifndef __COMPONENTS__
#define __COMPONENTS__

#ifndef __MACERRORS__
#include <CarbonCore/MacErrors.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __FILES__
#include <CarbonCore/Files.h>
#endif

#ifndef __RESOURCES__
#include <CarbonCore/Resources.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

enum {
  kAppleManufacturer            = 'appl', /* Apple supplied components */
  kComponentResourceType        = 'thng', /* a components resource type */
  kComponentAliasResourceType   = 'thga' /* component alias resource type */
};

enum {
  kAnyComponentType             = 0,
  kAnyComponentSubType          = 0,
  kAnyComponentManufacturer     = 0,
  kAnyComponentFlagsMask        = 0
};

enum {
  cmpThreadSafe                 = 1UL << 28, /* component is thread-safe */
  cmpIsMissing                  = 1UL << 29,
  cmpWantsRegisterMessage       = 1UL << 31
};

enum {
  kComponentOpenSelect          = -1,   /* ComponentInstance for this open */
  kComponentCloseSelect         = -2,   /* ComponentInstance for this close */
  kComponentCanDoSelect         = -3,   /* selector # being queried */
  kComponentVersionSelect       = -4,   /* no params */
  kComponentRegisterSelect      = -5,   /* no params */
  kComponentTargetSelect        = -6,   /* ComponentInstance for top of call chain */
  kComponentUnregisterSelect    = -7,   /* no params */
  kComponentGetMPWorkFunctionSelect = -8, /* some params */
  kComponentExecuteWiredActionSelect = -9, /* QTAtomContainer actionContainer, QTAtom actionAtom, QTCustomActionTargetPtr target, QTEventRecordPtr event */
  kComponentGetPublicResourceSelect = -10 /* OSType resourceType, short resourceId, Handle *resource */
};

/* Component Resource Extension flags */
enum {
  componentDoAutoVersion        = (1 << 0),
  componentWantsUnregister      = (1 << 1),
  componentAutoVersionIncludeFlags = (1 << 2),
  componentHasMultiplePlatforms = (1 << 3),
  componentLoadResident         = (1 << 4)
};



/* Set Default Component flags */
enum {
  defaultComponentIdentical     = 0,
  defaultComponentAnyFlags      = 1,
  defaultComponentAnyManufacturer = 2,
  defaultComponentAnySubType    = 4,
  defaultComponentAnyFlagsAnyManufacturer = (defaultComponentAnyFlags + defaultComponentAnyManufacturer),
  defaultComponentAnyFlagsAnyManufacturerAnySubType = (defaultComponentAnyFlags + defaultComponentAnyManufacturer + defaultComponentAnySubType)
};

/* RegisterComponentResource flags */
enum {
  registerComponentGlobal       = 1,
  registerComponentNoDuplicates = 2,
  registerComponentAfterExisting = 4,
  registerComponentAliasesOnly  = 8
};


struct ComponentDescription {
  OSType              componentType;          /* A unique 4-byte code indentifying the command set */
  OSType              componentSubType;       /* Particular flavor of this instance */
  OSType              componentManufacturer;  /* Vendor indentification */
  UInt32              componentFlags;         /* 8 each for Component,Type,SubType,Manuf/revision */
  UInt32              componentFlagsMask;     /* Mask for specifying which flags to consider in search, zero during registration */
};
typedef struct ComponentDescription     ComponentDescription;

struct ResourceSpec {
  OSType              resType;                /* 4-byte code    */
  SInt16              resID;                  /*         */
};
typedef struct ResourceSpec             ResourceSpec;
struct ComponentResource {
  ComponentDescription  cd;                   /* Registration parameters */
  ResourceSpec        component;              /* resource where Component code is found */
  ResourceSpec        componentName;          /* name string resource */
  ResourceSpec        componentInfo;          /* info string resource */
  ResourceSpec        componentIcon;          /* icon resource */
};
typedef struct ComponentResource        ComponentResource;
typedef ComponentResource *             ComponentResourcePtr;
typedef ComponentResourcePtr *          ComponentResourceHandle;
struct ComponentPlatformInfo {
  SInt32              componentFlags;         /* flags of Component */
  ResourceSpec        component;              /* resource where Component code is found */
  SInt16              platformType;           /* gestaltSysArchitecture result */
};
typedef struct ComponentPlatformInfo    ComponentPlatformInfo;
struct ComponentResourceExtension {
  SInt32              componentVersion;       /* version of Component */
  SInt32              componentRegisterFlags; /* flags for registration */
  SInt16              componentIconFamily;    /* resource id of Icon Family */
};
typedef struct ComponentResourceExtension ComponentResourceExtension;
struct ComponentPlatformInfoArray {
  SInt32              count;
  ComponentPlatformInfo  platformArray[1];
};
typedef struct ComponentPlatformInfoArray ComponentPlatformInfoArray;
struct ExtComponentResource {
  ComponentDescription  cd;                   /* registration parameters */
  ResourceSpec        component;              /* resource where Component code is found */
  ResourceSpec        componentName;          /* name string resource */
  ResourceSpec        componentInfo;          /* info string resource */
  ResourceSpec        componentIcon;          /* icon resource */
  SInt32              componentVersion;       /* version of Component */
  SInt32              componentRegisterFlags; /* flags for registration */
  SInt16              componentIconFamily;    /* resource id of Icon Family */
  SInt32              count;                  /* elements in platformArray */
  ComponentPlatformInfo  platformArray[1];
};
typedef struct ExtComponentResource     ExtComponentResource;
typedef ExtComponentResource *          ExtComponentResourcePtr;
typedef ExtComponentResourcePtr *       ExtComponentResourceHandle;
struct ComponentAliasResource {
  ComponentResource   cr;                     /* Registration parameters */
  ComponentDescription  aliasCD;              /* component alias description */
};
typedef struct ComponentAliasResource   ComponentAliasResource;
/*  Structure received by Component:        */
struct ComponentParameters {
  UInt8               flags;                  /* call modifiers: sync/async, deferred, immed, etc */
  UInt8               paramSize;              /* size in bytes of actual parameters passed to this call */
  SInt16              what;                   /* routine selector, negative for Component management calls */
#if __LP64__

  UInt32              padding;
#endif

  long                params[1];              /* actual parameters for the indicated routine */
};
typedef struct ComponentParameters      ComponentParameters;
struct ComponentRecord {
  long                data[1];
};
typedef struct ComponentRecord          ComponentRecord;
typedef ComponentRecord *               Component;
struct ComponentInstanceRecord {
  long                data[1];
};
typedef struct ComponentInstanceRecord  ComponentInstanceRecord;
typedef ComponentInstanceRecord *       ComponentInstance;
struct RegisteredComponentRecord {
  long                data[1];
};
typedef struct RegisteredComponentRecord RegisteredComponentRecord;
typedef RegisteredComponentRecord *     RegisteredComponentRecordPtr;
struct RegisteredComponentInstanceRecord {
  long                data[1];
};
typedef struct RegisteredComponentInstanceRecord RegisteredComponentInstanceRecord;
typedef RegisteredComponentInstanceRecord * RegisteredComponentInstanceRecordPtr;
typedef SInt32                          ComponentResult;
enum {
  platform68k                   = 1,    /* platform type (response from gestaltComponentPlatform) */
  platformPowerPC               = 2,    /* (when gestaltComponentPlatform is not implemented, use */
  platformInterpreted           = 3,    /* gestaltSysArchitecture) */
  platformWin32                 = 4,
  platformPowerPCNativeEntryPoint = 5,
  platformIA32NativeEntryPoint  = 6,
  platformPowerPC64NativeEntryPoint = 7,
  platformX86_64NativeEntryPoint = 8,
  platformArm64NativeEntryPoint = 9
};

enum {
  platformIRIXmips              = 1000,
  platformSunOSsparc            = 1100,
  platformSunOSintel            = 1101,
  platformLinuxppc              = 1200,
  platformLinuxintel            = 1201,
  platformAIXppc                = 1300,
  platformNeXTIntel             = 1400,
  platformNeXTppc               = 1401,
  platformNeXTsparc             = 1402,
  platformNeXT68k               = 1403,
  platformMacOSx86              = 1500
};

enum {
  mpWorkFlagDoWork              = (1 << 0),
  mpWorkFlagDoCompletion        = (1 << 1),
  mpWorkFlagCopyWorkBlock       = (1 << 2),
  mpWorkFlagDontBlock           = (1 << 3),
  mpWorkFlagGetProcessorCount   = (1 << 4),
  mpWorkFlagGetIsRunning        = (1 << 6)
};

enum {
  cmpAliasNoFlags               = 0,
  cmpAliasOnlyThisFile          = 1
};

typedef UInt32                          CSComponentsThreadMode;
enum {
  kCSAcceptAllComponentsMode    = 0,
  kCSAcceptThreadSafeComponentsOnlyMode = 1
};

/*
 *  CSSetComponentsThreadMode()
 *  
 *  Summary:
 *    Set whether or not using thread-unsafe components is allowed on
 *    the current thread.
 *  
 *  Discussion:
 *    When set to kCSAcceptThreadSafeComponentsOnlyMode, the current
 *    thread can only make thread-safe calls. Applications and other
 *    high-level code that wants to call QuickTime (and other) APIs
 *    from preemptive threads should call  SetComponentsThreadMode(
 *    kCSAcceptThreadSafeComponentsOnlyMode );  from their thread
 *    beforehand. The safeguard flag should only be left
 *    kCSAcceptAllComponentsMode for the main thread and other threads
 *    that participate in cooperative locking with it (such as the
 *    Carbon Thread Manager-style cooperative threads and application 
 *    threads that perform private locking).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    mode:
 *      The thread-safety mode in current thread.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSSetComponentsThreadMode(CSComponentsThreadMode mode)        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_3, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CSGetComponentsThreadMode()
 *  
 *  Summary:
 *    Get the current thread's thread-safety mode.
 *  
 *  Discussion:
 *    Returns kCSAcceptThreadSafeComponentsOnlyMode if only thread-safe
 *    components are allowed in current thread and
 *    kCSAcceptAllComponentsMode if all components are accepted
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSComponentsThreadMode 
CSGetComponentsThreadMode(void)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_3, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


struct ComponentMPWorkFunctionHeaderRecord {
  UInt32              headerSize;
  UInt32              recordSize;
  UInt32              workFlags;
  UInt16              processorCount;
  UInt8               unused;
  UInt8               isRunning;
};
typedef struct ComponentMPWorkFunctionHeaderRecord ComponentMPWorkFunctionHeaderRecord;
typedef ComponentMPWorkFunctionHeaderRecord * ComponentMPWorkFunctionHeaderRecordPtr;
typedef CALLBACK_API( ComponentResult , ComponentMPWorkFunctionProcPtr )(void *globalRefCon, ComponentMPWorkFunctionHeaderRecordPtr header);
typedef CALLBACK_API( ComponentResult , ComponentRoutineProcPtr )(ComponentParameters *cp, Handle componentStorage);
typedef CALLBACK_API( OSErr , GetMissingComponentResourceProcPtr )(Component c, OSType resType, SInt16 resID, void *refCon, Handle *resource);
typedef STACK_UPP_TYPE(ComponentMPWorkFunctionProcPtr)          ComponentMPWorkFunctionUPP;
typedef STACK_UPP_TYPE(ComponentRoutineProcPtr)                 ComponentRoutineUPP;
typedef STACK_UPP_TYPE(GetMissingComponentResourceProcPtr)      GetMissingComponentResourceUPP;
/*
    The parameter list for each ComponentFunction is unique. It is
    therefore up to users to create the appropriate procInfo for their
    own ComponentFunctions where necessary.
*/
typedef UniversalProcPtr                ComponentFunctionUPP;
/*
 *  NewComponentFunctionUPP()
 *  
 *  Discussion:
 *    For use in writing a Carbon compliant Component.  It is used to
 *    create a ComponentFunctionUPP needed to call
 *    CallComponentFunction in the Components dispatch routine.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ComponentFunctionUPP 
NewComponentFunctionUPP(
  ProcPtr        userRoutine,
  ProcInfoType   procInfo)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DisposeComponentFunctionUPP()
 *  
 *  Discussion:
 *    For use in writing a Carbon compliant Component.  It is used to
 *    dispose of a ComponentFunctionUPP created by
 *    NewComponentFunctionUPP.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.0
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void 
DisposeComponentFunctionUPP(ComponentFunctionUPP userUPP)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



#if TARGET_RT_MAC_CFM
/* 
    CallComponentUPP is a global variable exported from InterfaceLib.
    It is the ProcPtr passed to CallUniversalProc to manually call a component function.
*/
/*
 *  CallComponentUPP
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern UniversalProcPtr CallComponentUPP;
#endif

#define ComponentCallNow( callNumber, paramSize ) \
    FIVEWORDINLINE( 0x2F3C,paramSize,callNumber,0x7000,0xA82A )

/********************************************************
*                                                       *
*               APPLICATION LEVEL CALLS                 *
*                                                       *
********************************************************/
/********************************************************
* Component Database Add, Delete, and Query Routines
********************************************************/
/*
 *  RegisterComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern Component 
RegisterComponent(
  ComponentDescription *  cd,
  ComponentRoutineUPP     componentEntryPoint,
  SInt16                  global,
  Handle                  componentName,
  Handle                  componentInfo,
  Handle                  componentIcon)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  RegisterComponentResource()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern Component 
RegisterComponentResource(
  ComponentResourceHandle   cr,
  SInt16                    global)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  UnregisterComponent()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
UnregisterComponent(Component aComponent)                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FindNextComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern Component 
FindNextComponent(
  Component               aComponent,
  ComponentDescription *  looking)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CountComponents()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern long 
CountComponents(ComponentDescription * looking)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentInfo()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
GetComponentInfo(
  Component               aComponent,
  ComponentDescription *  cd,
  Handle                  componentName,
  Handle                  componentInfo,
  Handle                  componentIcon)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentListModSeed()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern SInt32 
GetComponentListModSeed(void)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentTypeModSeed()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern SInt32 
GetComponentTypeModSeed(OSType componentType)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/********************************************************
* Component Instance Allocation and dispatch routines
********************************************************/
/*
 *  OpenAComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
OpenAComponent(
  Component            aComponent,
  ComponentInstance *  ci)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  OpenComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentInstance 
OpenComponent(Component aComponent)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CloseComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
CloseComponent(ComponentInstance aComponentInstance)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentInstanceError()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
GetComponentInstanceError(ComponentInstance aComponentInstance) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/********************************************************
* Component aliases
********************************************************/
/*
 *  ResolveComponentAlias()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 3.0 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern Component 
ResolveComponentAlias(Component aComponent)                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/********************************************************
* Component public resources and public string lists
********************************************************/
/* Note: GetComponentPublicResource returns a Handle, not a resource.  The caller must dispose it with DisposeHandle. */
/*
 *  GetComponentPublicResource()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 4.0 and later
 */
extern OSErr 
GetComponentPublicResource(
  Component   aComponent,
  OSType      resourceType,
  SInt16      resourceID,
  Handle *    theResource)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentPublicResourceList()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 4.0 and later
 */
extern OSErr 
GetComponentPublicResourceList(
  OSType                           resourceType,
  SInt16                           resourceID,
  SInt32                           flags,
  ComponentDescription *           cd,
  GetMissingComponentResourceUPP   missingProc,
  void *                           refCon,
  void *                           atomContainerPtr)          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentPublicIndString()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 4.0 and later
 */
extern OSErr 
GetComponentPublicIndString(
  Component   aComponent,
  Str255      theString,
  SInt16      strListID,
  SInt16      index)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/********************************************************
*                                                       *
*                   CALLS MADE BY COMPONENTS            *
*                                                       *
********************************************************/
/********************************************************
* Component Management routines
********************************************************/
/*
 *  SetComponentInstanceError()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern void 
SetComponentInstanceError(
  ComponentInstance   aComponentInstance,
  OSErr               theError)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentRefcon()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern long 
GetComponentRefcon(Component aComponent)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetComponentRefcon()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern void 
SetComponentRefcon(
  Component   aComponent,
  long        theRefcon)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  OpenComponentResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ResFileRefNum 
OpenComponentResFile(Component aComponent)                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  OpenAComponentResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
OpenAComponentResFile(
  Component        aComponent,
  ResFileRefNum *  resRef)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CloseComponentResFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
CloseComponentResFile(ResFileRefNum refnum)                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* Note: GetComponentResource returns a Handle, not a resource.  The caller must dispose it with DisposeHandle. */
/*
 *  GetComponentResource()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 3.0 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
GetComponentResource(
  Component   aComponent,
  OSType      resType,
  SInt16      resID,
  Handle *    theResource)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentIndString()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 3.0 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
GetComponentIndString(
  Component   aComponent,
  Str255      theString,
  SInt16      strListID,
  SInt16      index)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/********************************************************
* Component Instance Management routines
********************************************************/
/*
 *  GetComponentInstanceStorage()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern Handle 
GetComponentInstanceStorage(ComponentInstance aComponentInstance) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetComponentInstanceStorage()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern void 
SetComponentInstanceStorage(
  ComponentInstance   aComponentInstance,
  Handle              theStorage)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CountComponentInstances()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern long 
CountComponentInstances(Component aComponent)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* useful helper routines for convenient method dispatching */
/*
 *  CallComponentFunction()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
CallComponentFunction(
  ComponentParameters *  params,
  ComponentFunctionUPP   func)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentFunctionWithStorage()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentFunctionWithStorage(
  Handle                 storage,
  ComponentParameters *  params,
  ComponentFunctionUPP   func)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentFunctionWithStorageProcInfo()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 */
extern ComponentResult 
CallComponentFunctionWithStorageProcInfo(
  Handle                 storage,
  ComponentParameters *  params,
  ProcPtr                func,
  ProcInfoType           funcProcInfo)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DelegateComponentCall()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
DelegateComponentCall(
  ComponentParameters *  originalParams,
  ComponentInstance      ci)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetDefaultComponent()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
SetDefaultComponent(
  Component   aComponent,
  SInt16      flags)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  OpenDefaultComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentInstance 
OpenDefaultComponent(
  OSType   componentType,
  OSType   componentSubType)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  OpenADefaultComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
OpenADefaultComponent(
  OSType               componentType,
  OSType               componentSubType,
  ComponentInstance *  ci)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CaptureComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern Component 
CaptureComponent(
  Component   capturedComponent,
  Component   capturingComponent)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  UncaptureComponent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
UncaptureComponent(Component aComponent)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  RegisterComponentResourceFile()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern SInt32 
RegisterComponentResourceFile(
  SInt16   resRefNum,
  SInt16   global)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* This call is deprecated. Please use GetIconRefFromComponent() instead.*/
#if !__LP64__
/*
 *  GetComponentIconSuite()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern OSErr 
GetComponentIconSuite(
  Component   aComponent,
  Handle *    iconSuite)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 * These calls allow you to register a file system entity.  The
 * Component Manager will "do the right thing" with the entity,
 * whether it is a standard resource fork based CFM component, CFM
 * bundle, mach-o bundle, or packaged bundle.  
 *
 * The *Entries calls allow you to specify a component description
 * which will be used to register selective components.  (Passing
 * NULL, 0 means to register all components.  
 */
#if !__LP64__
/*
 *  RegisterComponentFile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
RegisterComponentFile(
  const FSSpec *  spec,
  short           global)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  RegisterComponentFileEntries()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
RegisterComponentFileEntries(
  const FSSpec *                spec,
  short                         global,
  const ComponentDescription *  toRegister,          /* can be NULL */
  UInt32                        registerCount)                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  RegisterComponentFileRef()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
RegisterComponentFileRef(
  const FSRef *  ref,
  SInt16         global)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  RegisterComponentFileRefEntries()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
RegisterComponentFileRefEntries(
  const FSRef *                 ref,
  SInt16                        global,
  const ComponentDescription *  toRegister,          /* can be NULL */
  UInt32                        registerCount)                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/********************************************************
*                                                       *
*           Direct calls to the Components              *
*                                                       *
********************************************************/
/* Old style names*/

#if !__LP64__
/*
 *  ComponentFunctionImplemented()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
ComponentFunctionImplemented(
  ComponentInstance   ci,
  SInt16              ftnNumber)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetComponentVersion()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
GetComponentVersion(ComponentInstance ci)                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  ComponentSetTarget()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
ComponentSetTarget(
  ComponentInstance   ci,
  ComponentInstance   target)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/* New style names*/

#endif  /* !__LP64__ */

/*
 *  CallComponentOpen()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentOpen(
  ComponentInstance   ci,
  ComponentInstance   self)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentClose()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentClose(
  ComponentInstance   ci,
  ComponentInstance   self)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentCanDo()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentCanDo(
  ComponentInstance   ci,
  SInt16              ftnNumber)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentVersion()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentVersion(ComponentInstance ci)                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentRegister()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentRegister(ComponentInstance ci)                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentTarget()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentTarget(
  ComponentInstance   ci,
  ComponentInstance   target)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentUnregister()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentUnregister(ComponentInstance ci)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentGetMPWorkFunction()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 2.5 and later
 *    Windows:          in qtmlClient.lib 3.0 and later
 */
extern ComponentResult 
CallComponentGetMPWorkFunction(
  ComponentInstance             ci,
  ComponentMPWorkFunctionUPP *  workFunction,
  void **                       refCon)                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CallComponentGetPublicResource()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib via QuickTime 4.0 and later
 */
extern ComponentResult 
CallComponentGetPublicResource(
  ComponentInstance   ci,
  OSType              resourceType,
  SInt16              resourceID,
  Handle *            resource)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
    CallComponentDispatch is a CarbonLib routine that replaces CallComponent inline glue
    to call a component function.
 */
/*
 *  CallComponentDispatch()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern ComponentResult 
CallComponentDispatch(ComponentParameters * cp)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/* UPP call backs */
/*
 *  NewComponentMPWorkFunctionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ComponentMPWorkFunctionUPP
NewComponentMPWorkFunctionUPP(ComponentMPWorkFunctionProcPtr userRoutine) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  NewComponentRoutineUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ComponentRoutineUPP
NewComponentRoutineUPP(ComponentRoutineProcPtr userRoutine)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  NewGetMissingComponentResourceUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern GetMissingComponentResourceUPP
NewGetMissingComponentResourceUPP(GetMissingComponentResourceProcPtr userRoutine) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeComponentMPWorkFunctionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeComponentMPWorkFunctionUPP(ComponentMPWorkFunctionUPP userUPP) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeComponentRoutineUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeComponentRoutineUPP(ComponentRoutineUPP userUPP)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeGetMissingComponentResourceUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeGetMissingComponentResourceUPP(GetMissingComponentResourceUPP userUPP) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeComponentMPWorkFunctionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ComponentResult
InvokeComponentMPWorkFunctionUPP(
  void *                                  globalRefCon,
  ComponentMPWorkFunctionHeaderRecordPtr  header,
  ComponentMPWorkFunctionUPP              userUPP)            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeComponentRoutineUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ComponentResult
InvokeComponentRoutineUPP(
  ComponentParameters *  cp,
  Handle                 componentStorage,
  ComponentRoutineUPP    userUPP)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeGetMissingComponentResourceUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeGetMissingComponentResourceUPP(
  Component                       c,
  OSType                          resType,
  SInt16                          resID,
  void *                          refCon,
  Handle *                        resource,
  GetMissingComponentResourceUPP  userUPP)                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline ComponentMPWorkFunctionUPP                           NewComponentMPWorkFunctionUPP(ComponentMPWorkFunctionProcPtr userRoutine) { return userRoutine; }
    inline ComponentRoutineUPP                                  NewComponentRoutineUPP(ComponentRoutineProcPtr userRoutine) { return userRoutine; }
    inline GetMissingComponentResourceUPP                       NewGetMissingComponentResourceUPP(GetMissingComponentResourceProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeComponentMPWorkFunctionUPP(ComponentMPWorkFunctionUPP) { }
    inline void                                                 DisposeComponentRoutineUPP(ComponentRoutineUPP) { }
    inline void                                                 DisposeGetMissingComponentResourceUPP(GetMissingComponentResourceUPP) { }
    inline ComponentResult                                      InvokeComponentMPWorkFunctionUPP(void * globalRefCon, ComponentMPWorkFunctionHeaderRecordPtr header, ComponentMPWorkFunctionUPP userUPP) { return (*userUPP)(globalRefCon, header); }
    inline ComponentResult                                      InvokeComponentRoutineUPP(ComponentParameters * cp, Handle componentStorage, ComponentRoutineUPP userUPP) { return (*userUPP)(cp, componentStorage); }
    inline OSErr                                                InvokeGetMissingComponentResourceUPP(Component c, OSType resType, SInt16 resID, void * refCon, Handle * resource, GetMissingComponentResourceUPP userUPP) { return (*userUPP)(c, resType, resID, refCon, resource); }
  #else
    #define NewComponentMPWorkFunctionUPP(userRoutine)          ((ComponentMPWorkFunctionUPP)userRoutine)
    #define NewComponentRoutineUPP(userRoutine)                 ((ComponentRoutineUPP)userRoutine)
    #define NewGetMissingComponentResourceUPP(userRoutine)      ((GetMissingComponentResourceUPP)userRoutine)
    #define DisposeComponentMPWorkFunctionUPP(userUPP)
    #define DisposeComponentRoutineUPP(userUPP)
    #define DisposeGetMissingComponentResourceUPP(userUPP)
    #define InvokeComponentMPWorkFunctionUPP(globalRefCon, header, userUPP) (*userUPP)(globalRefCon, header)
    #define InvokeComponentRoutineUPP(cp, componentStorage, userUPP) (*userUPP)(cp, componentStorage)
    #define InvokeGetMissingComponentResourceUPP(c, resType, resID, refCon, resource, userUPP) (*userUPP)(c, resType, resID, refCon, resource)
  #endif
#endif

/* ProcInfos */

/* MixedMode ProcInfo constants for component calls */
enum {
    uppComponentFunctionImplementedProcInfo    = 0x000002F0,
    uppGetComponentVersionProcInfo             = 0x000000F0,
    uppComponentSetTargetProcInfo              = 0x000003F0,
    uppCallComponentOpenProcInfo               = 0x000003F0,
    uppCallComponentCloseProcInfo              = 0x000003F0,
    uppCallComponentCanDoProcInfo              = 0x000002F0,
    uppCallComponentVersionProcInfo            = 0x000000F0,
    uppCallComponentRegisterProcInfo           = 0x000000F0,
    uppCallComponentTargetProcInfo             = 0x000003F0,
    uppCallComponentUnregisterProcInfo         = 0x000000F0,
    uppCallComponentGetMPWorkFunctionProcInfo  = 0x00000FF0,
    uppCallComponentGetPublicResourceProcInfo  = 0x00003BF0
};







#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __COMPONENTS__ */

