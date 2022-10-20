/*
     File:       CarbonCore/MacMemory.h
 
     Contains:   Memory Manager Interfaces.
                 The contents of this header file are deprecated.
                 Use malloc, free, etc instead.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/
#ifndef __MACMEMORY__
#define __MACMEMORY__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif


#include <strings.h>

#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

enum {
  kMacMemoryMaximumMemoryManagerBlockSize = 0x7FFFFFF0
};


// maxSize is a somewhat common name for a variable, and with this defined as an enum in the global scope any other usages would get warnings.
#if ! defined( MAC_OS_X_VERSION_10_7 ) || ! defined( MAC_OS_X_VERSION_MIN_REQUIRED ) || ( MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_7 )
enum {
 maxSize = kMacMemoryMaximumMemoryManagerBlockSize /*the largest block possible*/
};
#endif


/*
    If you define a macro named __MAC_OS_X_MEMORY_MANAGER_CLEAN__ with a non-zero value, then
    some Memory Manager APIs will become inlined, minimal implementations.  See the comments
    below for more information about this.
*/
#ifndef __MAC_OS_X_MEMORY_MANAGER_CLEAN__
#define __MAC_OS_X_MEMORY_MANAGER_CLEAN__ 0
#endif  /* !defined(__MAC_OS_X_MEMORY_MANAGER_CLEAN__) */

#if !__MAC_OS_X_MEMORY_MANAGER_CLEAN__
enum {
  defaultPhysicalEntryCount     = 8
};

enum {
                                        /* values returned from the GetPageState function */
  kPageInMemory                 = 0,
  kPageOnDisk                   = 1,
  kNotPaged                     = 2
};

enum {
                                        /* masks for Zone->heapType field */
  k32BitHeap                    = 1,    /* valid in all Memory Managers */
  kNewStyleHeap                 = 2,    /* true if new Heap Manager is present */
  kNewDebugHeap                 = 4     /* true if new Heap Manager is running in debug mode on this heap */
};


#endif  /* !__MAC_OS_X_MEMORY_MANAGER_CLEAN__ */

/* bits for use with HGetState/HSetState*/
enum {
  kHandleIsResourceBit          = 5,
  kHandlePurgeableBit           = 6,
  kHandleLockedBit              = 7
};

/* masks for use with HGetState/HSetState*/
enum {
  kHandleIsResourceMask         = 0x20,
  kHandlePurgeableMask          = 0x40,
  kHandleLockedMask             = 0x80
};

#if !__LP64__
typedef CALLBACK_API( long , GrowZoneProcPtr )(Size cbNeeded);
typedef CALLBACK_API( void , PurgeProcPtr )(Handle blockToPurge);
typedef CALLBACK_API( void , UserFnProcPtr )(void * parameter);
typedef STACK_UPP_TYPE(GrowZoneProcPtr)                         GrowZoneUPP;
typedef STACK_UPP_TYPE(PurgeProcPtr)                            PurgeUPP;
typedef STACK_UPP_TYPE(UserFnProcPtr)                           UserFnUPP;
struct Zone {
  Ptr                 bkLim;
  Ptr                 purgePtr;
  Ptr                 hFstFree;
  long                zcbFree;
  GrowZoneUPP         gzProc;
  short               moreMast;
  short               flags;
  short               cntRel;
  short               maxRel;
  short               cntNRel;
  SInt8               heapType;               /* previously "maxNRel", now holds flags (e.g. k32BitHeap)*/
  SInt8               unused;
  short               cntEmpty;
  short               cntHandles;
  long                minCBFree;
  PurgeUPP            purgeProc;
  Ptr                 sparePtr;
  Ptr                 allocPtr;
  short               heapData;
};
typedef struct Zone                     Zone;
typedef Zone *                          THz;
typedef THz *                           THzPtr;
#if !__MAC_OS_X_MEMORY_MANAGER_CLEAN__
struct MemoryBlock {
  void *              address;
  unsigned long       count;
};
typedef struct MemoryBlock              MemoryBlock;
struct LogicalToPhysicalTable {
  MemoryBlock         logical;
  MemoryBlock         physical[8];
};
typedef struct LogicalToPhysicalTable   LogicalToPhysicalTable;

typedef short                           PageState;
typedef short                           StatusRegisterContents;
enum {
  kVolumeVirtualMemoryInfoVersion1 = 1  /* first version of VolumeVirtualMemoryInfo*/
};

struct VolumeVirtualMemoryInfo {
  PBVersion           version;                /* Input: Version of the VolumeVirtualMemoryInfo structure*/
  SInt16              volumeRefNum;           /* Input: volume reference number*/
  Boolean             inUse;                  /* output: true if volume is currently used for file mapping*/
  UInt8               _fill;
  UInt32              vmOptions;              /* output: tells what volume can support (same as DriverGestaltVMOptionsResponse vmOptions bits in DriverGestalt)*/
                                              /* end of kVolumeVirtualMemoryInfoVersion1 structure*/
};
typedef struct VolumeVirtualMemoryInfo  VolumeVirtualMemoryInfo;
typedef VolumeVirtualMemoryInfo *       VolumeVirtualMemoryInfoPtr;
#endif  /* !__MAC_OS_X_MEMORY_MANAGER_CLEAN__ */

/*
 *  NewGrowZoneUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern GrowZoneUPP
NewGrowZoneUPP(GrowZoneProcPtr userRoutine)                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

/*
 *  NewPurgeUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern PurgeUPP
NewPurgeUPP(PurgeProcPtr userRoutine)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

/*
 *  NewUserFnUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern UserFnUPP
NewUserFnUPP(UserFnProcPtr userRoutine)                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeGrowZoneUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeGrowZoneUPP(GrowZoneUPP userUPP)                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposePurgeUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposePurgeUPP(PurgeUPP userUPP)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeUserFnUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeUserFnUPP(UserFnUPP userUPP)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeGrowZoneUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern long
InvokeGrowZoneUPP(
  Size         cbNeeded,
  GrowZoneUPP  userUPP)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokePurgeUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokePurgeUPP(
  Handle    blockToPurge,
  PurgeUPP  userUPP)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeUserFnUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeUserFnUPP(
  void *     parameter,
  UserFnUPP  userUPP)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline GrowZoneUPP                                          NewGrowZoneUPP(GrowZoneProcPtr userRoutine) { return userRoutine; }
    inline PurgeUPP                                             NewPurgeUPP(PurgeProcPtr userRoutine) { return userRoutine; }
    inline UserFnUPP                                            NewUserFnUPP(UserFnProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeGrowZoneUPP(GrowZoneUPP) { }
    inline void                                                 DisposePurgeUPP(PurgeUPP) { }
    inline void                                                 DisposeUserFnUPP(UserFnUPP) { }
    inline long                                                 InvokeGrowZoneUPP(Size cbNeeded, GrowZoneUPP userUPP) { return (*userUPP)(cbNeeded); }
    inline void                                                 InvokePurgeUPP(Handle blockToPurge, PurgeUPP userUPP) { (*userUPP)(blockToPurge); }
    inline void                                                 InvokeUserFnUPP(void * parameter, UserFnUPP userUPP) { (*userUPP)(parameter); }
  #else
    #define NewGrowZoneUPP(userRoutine)                         ((GrowZoneUPP)userRoutine)
    #define NewPurgeUPP(userRoutine)                            ((PurgeUPP)userRoutine)
    #define NewUserFnUPP(userRoutine)                           ((UserFnUPP)userRoutine)
    #define DisposeGrowZoneUPP(userUPP)
    #define DisposePurgeUPP(userUPP)
    #define DisposeUserFnUPP(userUPP)
    #define InvokeGrowZoneUPP(cbNeeded, userUPP)                (*userUPP)(cbNeeded)
    #define InvokePurgeUPP(blockToPurge, userUPP)               (*userUPP)(blockToPurge)
    #define InvokeUserFnUPP(parameter, userUPP)                 (*userUPP)(parameter)
  #endif
#endif

#endif  /* !__LP64__ */

/*
 *  MemError()
 *  
 *  Summary:
 *    Determines if an application’s last direct call to a Memory
 *    Manager function executed successfully.
 *  
 *  Discussion:
 *    MemError() yields the result code produced by the last Memory
 *    Manager function your application called directly, and resets
 *    MemError() to return noErr in the future. MemError() is useful
 *    during application debugging. You might also use MemError as one
 *    part of a memory-management scheme to identify instances in which
 *    the Memory Manager rejects overly large memory requests by
 *    returning the error code memFullErr.
 *    
 *    To view the result codes that MemError() can produce, see "Memory
 *    Manager Result Codes".
 *    
 *    Do not rely on MemError() as the only component of a
 *    memory-management scheme. For example, suppose you call NewHandle
 *    or NewPtr and receive the result code noErr, indicating that the
 *    Memory Manager was able to allocate sufficient memory. In this
 *    case, you have no guarantee that the allocation did not deplete
 *    your application’s memory reserves to levels so low that simple
 *    operations might cause your application to crash. Instead of
 *    relying on MemError(), check before making a memory request that
 *    there is enough memory both to fulfill the request and to support
 *    essential operations.
 *    
 *    On Mac OS X 10.3 and later, the value of MemError() is kept for
 *    each thread; prior to Mac OS X 10.3.  MemError() is global to the
 *    application.  Because of this, and other problems, the Memory
 *    Manager APIs are not thread safe before Mac OS X 10.3.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
MemError(void)                                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  LMGetMemErr()
 *  
 *  Summary:
 *    Returns the result of the last Memory Manager function, without
 *    clearing the value like MemError() does.
 *  
 *  Discussion:
 *    LMGetMemErr yields the result code produced by the last Memory
 *    Manager function your application called directly. Unlike
 *    MemError(), this function does not reset the stored value, so
 *    subsequent calls to LMGetMemErr() will still return this value
 *    until the next Memory Manager routine is called or until
 *    MemError() is called to reset the value. LMGetMemErr is useful
 *    during application debugging. You might also use this value as
 *    one part of a memory-management scheme to identify instances in
 *    which the Memory Manager rejects overly large memory requests by
 *    returning the error code memFullErr.
 *    
 *    To view the result codes that MemError() can produce, see "Memory
 *    Manager Result Codes".
 *    
 *    Do not rely on MemError() as the only component of a
 *    memory-management scheme. For example, suppose you call NewHandle
 *    or NewPtr and receive the result code noErr, indicating that the
 *    Memory Manager was able to allocate sufficient memory. In this
 *    case, you have no guarantee that the allocation did not deplete
 *    your application’s memory reserves to levels so low that simple
 *    operations might cause your application to crash. Instead of
 *    relying on MemError(), check before making a memory request that
 *    there is enough memory both to fulfill the request and to support
 *    essential operations.
 *    
 *    On Mac OS X 10.3 and later, the value of MemError() is kept for
 *    each thread; prior to Mac OS X 10.3 there was one global value of
 *    MemError() which all threads shared. Because of this, and other
 *    problems, the Memory Manager APIs are not thread safe before Mac
 *    OS X 10.3.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SInt16 
LMGetMemErr(void)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  LMSetMemErr()
 *  
 *  Summary:
 *    Set the value which will be returned by MemError()
 *  
 *  Discussion:
 *    User code shouldn't need to call this function, which is used to
 *    set the value which the next call to MemError() will return.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    value:
 *      the value which the next MemError() function call should return
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
LMSetMemErr(SInt16 value)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  NewHandle()
 *  
 *  Summary:
 *    Allocate a relocatable memory block of a specified size.
 *  
 *  Discussion:
 *    The NewHandle function attempts to allocate a new relocatable
 *    block in the current heap zone with a logical size of logicalSize
 *    bytes and then return a handle to the block. The new block is
 *    unlocked and unpurgeable. If NewHandle cannot allocate a block of
 *    the requested size, it returns NULL.  The memory block returned
 *    likely will contain garbage, and will be unlocked and
 *    non-purgeable.
 *    
 *    WARNING
 *    
 *    Do not try to manufacture your own handles without this function
 *    by simply assigning the address of a variable of type Ptr to a
 *    variable of type Handle. The resulting "fake handle" would not
 *    reference a relocatable block and could cause a system crash.
 *     If this function returns NIL, the error result can be determined
 *    by calling the function MemError().
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    byteCount:
 *      the size of the relocatable memory block to allocate.  If this
 *      value is < 0, NIL will be returned. If this value is 0, a
 *      handle to 0 byte block will be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
NewHandle(Size byteCount)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  NewHandleClear()
 *  
 *  Summary:
 *    Allocate a relocatable memory block of a specified size.
 *  
 *  Discussion:
 *    The NewHandle function attempts to allocate a new relocatable
 *    block in the current heap zone with a logical size of logicalSize
 *    bytes and then return a handle to the block. The new block is
 *    unlocked and unpurgeable. If NewHandle cannot allocate a block of
 *    the requested size, it returns NULL.  The memory block returned
 *    will be zeroed, and will be unlocked and non-purgeable.
 *    
 *    WARNING
 *    
 *    Do not try to manufacture your own handles without this function
 *    by simply assigning the address of a variable of type Ptr to a
 *    variable of type Handle. The resulting "fake handle" would not
 *    reference a relocatable block and could cause a system crash.
 *     If this function returns NIL, the error result can be determined
 *    by calling the function MemError().
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    byteCount:
 *      the size of the relocatable memory block to allocate.  If this
 *      value is < 0, NIL will be returned. If this value is 0, a
 *      handle to 0 byte block will be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
NewHandleClear(Size byteCount)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  RecoverHandle()
 *  
 *  Summary:
 *    Returns a handle to a relocatable block pointed to by a specified
 *    pointer.
 *  
 *  Discussion:
 *    The Memory Manager does not allow you to change relocatable
 *    blocks into nonrelocatable blocks, or vice-versa. However, if you
 *    no longer have access to a handle but still have access to its
 *    master pointer p, you can use the RecoverHandle function to
 *    recreate a handle to the relocatable block referenced by
 *    p.
 *    
 *    Call the function MemError() to get the result code. See "Memory
 *    Manager Result Codes".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    p:
 *      the master pointer to a relocatable block.
 *  
 *  Result:
 *    A handle to a relocatable block point to by p. If p does not
 *    point to a valid block, this function returns NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
RecoverHandle(Ptr p)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  NewPtr()
 *  
 *  Summary:
 *    Allocates a nonrelocatable block of memory of a specified size.
 *  
 *  Discussion:
 *    The NewPtr function attempts to reserve space for the new block.
 *    If it is able to reserve the requested amount of space, NewPtr
 *    allocates the nonrelocatable block.  Otherwise, NewPtr returns
 *    NULL and generates a memFullErr error. On Mac OS X, NewPtr will
 *    never fail because it is unable to allocate the pointer. Certain
 *    old versions of Mac OS X return a NULL pointer when asked to
 *    allocate a pointer of size 0.
 *    Call the function MemError to get the result code. See "Memory
 *    Manager Result Codes".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    byteCount:
 *      The requested size (in bytes) of the nonrelocatable block.  If
 *      you pass a value of zero, this function returns a valid zero
 *      length pointer.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Ptr 
NewPtr(Size byteCount)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  NewPtrClear()
 *  
 *  Summary:
 *    Allocates a nonrelocatable block of memory of a specified size
 *    with all its bytes set to 0.
 *  
 *  Discussion:
 *    The NewPtr function attempts to reserve space for the new block.
 *    If it is able to reserve the requested amount of space, NewPtr
 *    allocates the nonrelocatable block.  Otherwise, NewPtr returns
 *    NULL and generates a memFullErr error. On Mac OS X, NewPtr will
 *    never fail because it is unable to allocate the pointer. Certain
 *    old versions of Mac OS X return a NULL pointer when asked to
 *    allocate a pointer of size 0.
 *    Call the function MemError to get the result code. See "Memory
 *    Manager Result Codes".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    byteCount:
 *      The requested size (in bytes) of the nonrelocatable block.  If
 *      you pass a value of zero, this function returns a valid zero
 *      length pointer.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Ptr 
NewPtrClear(Size byteCount)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#if !__LP64__
/*
 *  MaxBlock()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the size of the largest block you could allocate in the
 *    current heap zone after compaction.
 *  
 *  Discussion:
 *    On Mac OS X, this function always returns a large value, because
 *    virtual memory is always available to fulfill any request for
 *    memory.  This function is deprecated on Mac OS X and later.  You
 *    can assume that any reasonable memory allocation will succeed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
MaxBlock(void)                                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  StackSpace()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Returns the amount of space unused on the current thread's stack.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
StackSpace(void)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  NewEmptyHandle()
 *  
 *  Summary:
 *    Initializes a new handle without allocating any memory for it to
 *    control.
 *  
 *  Discussion:
 *    When you want to allocate memory for the empty handle, use the
 *    ReallocateHandle function.
 *    Call the function MemError to get the result code. See "Memory
 *    Manager Result Codes".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
NewEmptyHandle(void)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  HLock()
 *  
 *  Summary:
 *    Lock a relocatable block so that it does not move in the heap
 *  
 *  Discussion:
 *    The HLock procedure locks the relocatable block to which h is a
 *    handle, preventing it from being moved within its heap zone. If
 *    the block is already locked,HLock does nothing.
 *    
 *    On Mac OS X, the behaviour of the Memory Manager and of heaps in
 *    general is different than on Mac OS 9.x and earlier. In
 *    particular, the heap on Mac OS X is never purged or compacted. 
 *    Therefore, an unlocked handle will never be relocated except as a
 *    result of a direct action by something calling SetHandleSize() or
 *    by using a function like PtrAndHand() which implicitly resizes
 *    the handle to append data to it.  Because of this, most locking
 *    and unlocking of handles is unnecessary on Mac OS X, and the use
 *    of HLock() and other functions is being deprecated.  If you
 *    define a macro named __MAC_OS_X_MEMORY_MANAGER_CLEAN__ to 1 in
 *    your sources before you include MacMemory.h, then HLock() and
 *    several other functions will become empty operations, removing
 *    the overhead of a function call.
 *    
 *    However, some applications are relying on the behavior that
 *    resizing a locked handle produces an error, or tracking the state
 *    of the locked bit for a give handle via the HGetState() function.
 *     Applications which rely on this can not use
 *    __MAC_OS_X_MEMORY_MANAGER_CLEAN__.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      the handle to lock.  If h is == NULL, then HLock() sets
 *      MemError() to noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HLock(Handle h)                                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  HLockHi()
 *  
 *  Summary:
 *    Lock a relocatable handle.
 *  
 *  Discussion:
 *    The HLockHi() function locks a handle in memory.  On versions of
 *    Mac OS before Mac OS X, it would first attempt to move the handle
 *    as high in memory as feasible.  However, on Mac OS X and later,
 *    there is no advantage to having handles high in memory, and so
 *    this function never moves a handle before locking it.
 *    See the discussion about handle locking above the function
 *    HLock().
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    h:
 *      the handle to lock.  If h is == NULL, then HLockHi() sets
 *      MemError() to noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HLockHi(Handle h)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  HUnlock()
 *  
 *  Summary:
 *    Unlock a relocatable block so that it does not move in the heap
 *  
 *  Discussion:
 *    The HUnlock procedure unlocks the relocatable block to which h is
 *    a handle, allowing it from being moved within its heap zone. If
 *    the block is already unlocked, HUnlock does nothing.
 *    
 *    See the discussion about handles and locking on Mac OS X above
 *    the HLock() function.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      the handle to unlock.  If h is == NULL, then HUnlock() sets
 *      MemError() to noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HUnlock(Handle h)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#if !__LP64__
/*
 *  HPurge()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Mark a relocatable block so that it does can be purged if a
 *    memory request cannot be fulfilled after compaction of the heap
 *  
 *  Discussion:
 *    The HPurge procedure makes the relocatable block to which h is a
 *    handle purgeable. If the block is already purgeable, HPurge does
 *    nothing.
 *    
 *    On Mac OS X, heaps are never purged.  Therefore, the use of
 *    HPurge() and its associated functios is deprecated. If you define
 *    a macro __MAC_OS_X_MEMORY_MANAGER_CLEAN__ in your sources before
 *    you include MacMemory.h, then any calls to HPurge() in your
 *    program will essentially be removed.
 *    
 *    However, some applications may set the handle as purgeable, and
 *    then later check the purgeBit for the handle via HGetState().  If
 *    your application depends on the purge bit being set for handles,
 *    you will not be able to take advantage of this macro.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      the handle to mark as purgeable.  If h is == NULL, then
 *      HPurge() just sets MemError() to noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HPurge(Handle h)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  HNoPurge()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Mark a relocatable block so that it can not be purged.
 *  
 *  Discussion:
 *    The HNoPurge procedure makes the relocatable block to which h is
 *    a handle unpurgeable. See the discussion about purgable handles
 *    above the HPurge() function.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      the handle to mark as nonpurgeable.  If h is == NULL, then
 *      HPurge() just sets MemError() to noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HNoPurge(Handle h)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  TempNewHandle()
 *  
 *  Summary:
 *    Allocate a relocatable memory block of a specified size.
 *  
 *  Discussion:
 *    The TempNewHandle function attempts to allocate a new relocatable
 *    block in the current heap zone with a logical size of logicalSize
 *    bytes and then return a handle to the block. The new block is
 *    unlocked and unpurgeable. If NewHandle cannot allocate a block of
 *    the requested size, it returns NULL.  The memory block returned
 *    likely will contain garbage.
 *    
 *    WARNING
 *    
 *    Do not try to manufacture your own handles without this function
 *    by simply assigning the address of a variable of type Ptr to a
 *    variable of type Handle. The resulting "fake handle" would not
 *    reference a relocatable block and could cause a system crash.
 *     If this function returns NIL, the error result can be determined
 *    by calling the function MemError().
 *    
 *    On Mac OS X, there is no temporary memory heap, and thus no
 *    difference between the handles returned by TempNewHandle() and
 *    those returned by NewHandle().  The only difference between the
 *    two is that TempNewHandle() also returns the error result of the
 *    call in resultCode.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    logicalSize:
 *      the size of the relocatable memory block to allocate.  If this
 *      value is < 0, NIL will be returned. If this value is 0, a
 *      handle to 0 byte block will be returned.
 *    
 *    resultCode:
 *      On exit, this will be set to the result of the operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
TempNewHandle(
  Size     logicalSize,
  OSErr *  resultCode)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#if !__LP64__
/*
 *  TempMaxMem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the maximum amount of temporary memory available
 *  
 *  Discussion:
 *    On Mac OS X, this function always returns a large value, because
 *    virtual memory is always available to fulfill any request for
 *    memory.  This function is deprecated on Mac OS X and later.  You
 *    can assume that any reasonable memory allocation will succeed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    grow:
 *      If != NULL, then this is filled in with the the value 0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Size 
TempMaxMem(Size * grow)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  TempFreeMem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the maximum amount of free memory in the temporary heap.
 *  
 *  Discussion:
 *    On Mac OS X, there is no separate temporary memory heap.  This
 *    function always returns a large value, because virtual memory is
 *    always available to fulfill any request for memory.  This
 *    function is deprecated on Mac OS X and later.  You can assume
 *    that any reasonable memory allocation will succeed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
TempFreeMem(void)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  CompactMem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Compact the heap by purging and moving blocks such that at least
 *    cbNeeded bytes are available, if possible.
 *  
 *  Discussion:
 *    On Mac OS X and later, blocks are never purged and memory heaps
 *    will grow as necessary, so compaction is never necessary nor
 *    performed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Size 
CompactMem(Size cbNeeded)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PurgeMem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Purge blocks from the heap until cbNeeded bytes are available, if
 *    possible.
 *  
 *  Discussion:
 *    On Mac OS X and later, blocks are never purged and memory heaps
 *    will grow as necessary, so purging of a heap is never necessary
 *    nor performed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
PurgeMem(Size cbNeeded)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  FreeMem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the maximum amount of free memory in the temporary heap.
 *  
 *  Discussion:
 *    On Mac OS X, this function always returns a large value, because
 *    virtual memory is always available to fulfill any request for
 *    memory.  This function is deprecated on Mac OS X and later.  You
 *    can assume that any reasonable memory allocation will succeed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
FreeMem(void)                                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  MaxMem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the maximum amount of free memory available
 *  
 *  Discussion:
 *    On Mac OS X, this function always returns a large value, because
 *    virtual memory is always available to fulfill any request for
 *    memory.  This function is deprecated on Mac OS X and later.  You
 *    can assume that any reasonable memory allocation will succeed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    grow:
 *      If != NULL, then this is filled in with the the value 0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Size 
MaxMem(Size * grow)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetGrowZone()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Set a function which is called when a heap is grown
 *  
 *  Discussion:
 *    On Mac OS X and later, heaps never grow, and so the function set
 *    by SetGrowZone() is never called.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    growZone:
 *      a upp for a function to call when a heap needs to be grown
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetGrowZone(GrowZoneUPP growZone)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetGrowZone()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Get the function which is called when a heap is grown
 *  
 *  Discussion:
 *    On Mac OS X and later, heaps never grow, and so this function (
 *    set by SetGrowZone() ) is never called.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern GrowZoneUPP 
GetGrowZone(void)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  MoveHHi()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Move a handle as high in memory as possible
 *  
 *  Discussion:
 *    On versions of Mac OS before Mac OS X, MoveHHi() would move the
 *    handle as high in memory as feasible. However, on Mac OS X and
 *    later, there is no advantage to having handles high in memory,
 *    and so this function never moves a handle before locking it.
 *     See the discussion about handle locking above the function
 *    HLock().
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    h:
 *      the handle to move
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
MoveHHi(Handle h)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  DisposePtr()
 *  
 *  Summary:
 *    Release memory occupied by a nonrelocatable block
 *  
 *  Discussion:
 *    When you no longer need a nonrelocatable block, call the
 *    DisposePtr function to free it for other uses.
 *    Call the function MemError to get the result code. See "Memory
 *    Manager Result Codes".
 *    After a call to DisposePtr, all pointers to the released block
 *    become invalid and should not be used again. Any subsequent use
 *    of a pointer to the released block might cause a system error. 
 *    You can pass the value NULL as the pointer to dispose.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    p:
 *      A pointer to the nonrelocatable block you want to dispose of
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DisposePtr(Ptr p)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetPtrSize()
 *  
 *  Summary:
 *    Returns the logical size of the nonrelocatable block
 *    corresponding to a pointer.
 *  
 *  Discussion:
 *    This function returns the number of bytes used for the given
 *    pointer.  Call the function MemError to get the result code. See
 *    "Memory Manager Result Codes".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    p:
 *      a pointer to a nonrelocatable block.
 *  
 *  Result:
 *    The logical size, in bytes, of the nonrelocatable block pointed
 *    to by p. In case of error, the function returns 0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Size 
GetPtrSize(Ptr p)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetPtrSize()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetPtrSize(
  Ptr    p,
  Size   newSize)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DisposeHandle()
 *  
 *  Summary:
 *    Releases memory occupied by a relocatable block.
 *  
 *  Discussion:
 *    The DisposeHandle function releases the memory occupied by the
 *    relocatable block whose handle is h. It also frees the handle’s
 *    master pointer for other uses.
 *    Do not use DisposeHandle to dispose of a handle obtained from the
 *    Resource Manager (for example, by a previous call to
 *    GetResource), useReleaseResource instead. If, however, you have
 *    called DetachResource on a resource handle, you should dispose of
 *    the storage by callingDisposeHandle.
 *    Call the function MemError to get the result code. See "Memory
 *    Manager Result Codes".
 *    After a call to DisposeHandle, all handles to the released block
 *    become invalid and should not be used again. Any subsequent calls
 *    to DisposeHandleusing an invalid handle might damage the master
 *    pointer list.  You can pass the value NULL as the handle to
 *    dispose.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      A handle to a relocatable block.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DisposeHandle(Handle h)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetHandleSize()
 *  
 *  Summary:
 *    Changes the logical size of the relocatable block corresponding
 *    to the specified handle.
 *  
 *  Discussion:
 *    Change the logical size of the relocatable block corresponding to
 *    the specified handle. SetHandleSize might need to move the
 *    relocatable block to obtain enough space for the resized block.
 *    Thus, for best results you should unlock a block before resizing
 *    it.
 *    
 *    An attempt to increase the size of a locked block might fail,
 *    because of blocks above and below it that are either
 *    nonrelocatable or locked. You should be prepared for this
 *    possibility.
 *    
 *    Instead of using the SetHandleSize function to set the size of a
 *    handle to 0, you can use the EmptyHandle function.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      a handle to a relocatable block.
 *    
 *    newSize:
 *      the desired new logical size, in bytes, of the relocatable
 *      block.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetHandleSize(
  Handle   h,
  Size     newSize)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetHandleSize()
 *  
 *  Summary:
 *    Returns the logical size of the relocatable block corresponding
 *    to a handle.
 *  
 *  Discussion:
 *    Returns the logical size of the relocatable block corresponding
 *    to a handle.  Call the function MemError to get the result code.
 *    See "Memory Manager Result Codes".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      a handle to a relocatable block.
 *  
 *  Result:
 *    The logical size, in bytes, of the relocatable block whose handle
 *    is h. In case of error, the function return 0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Size 
GetHandleSize(Handle h)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  ReallocateHandle()
 *  
 *  Summary:
 *    Allocates a new relocatable block of a specified size and sets a
 *    handle’s master pointer to point to the new block.
 *  
 *  Discussion:
 *    Usually you use ReallocateHandle to reallocate space for a block
 *    that you have emptied. If the handle references an existing
 *    block, ReallocateHandle releases that block before creating a new
 *    one.
 *    
 *    To reallocate space for a resource that has been purged, you
 *    should call LoadResource, not ReallocateHandle. To resize
 *    relocatable blocks, you should call the SetHandleSize
 *    function.
 *    
 *    Call the function MemError to get the result code. See "Memory
 *    Manager Result Codes".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      A handle to a relocatable block.
 *    
 *    byteCount:
 *      the desired new logical size (in bytes) of the relocatable
 *      block. The new block is unlocked and unpurgeable.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ReallocateHandle(
  Handle   h,
  Size     byteCount)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  EmptyHandle()
 *  
 *  Summary:
 *    Purges a relocatable block and sets the corresponding handle’s
 *    master pointer to NULL.
 *  
 *  Discussion:
 *    The EmptyHandle function purges the relocatable block whose
 *    handle is h and sets the handle’s master pointer to NULL. The
 *    EmptyHandle function allows you to free memory taken by a
 *    relocatable block without freeing the relocatable block’s master
 *    pointer for other uses. The block whose handle is h must be
 *    unlocked but need not be purgeable.
 *    
 *    Note that if there are multiple handles to the relocatable block,
 *    then calling the EmptyHandle function empties them all, because
 *    all of the handles share a common master pointer. When you later
 *    use ReallocateHandle to reallocate space for the block, the
 *    master pointer is updated, and all of the handles reference the
 *    new block correctly.
 *    
 *    To free the memory taken up by a relocatable block and release
 *    the block’s master pointer for other uses, use the DisposeHandle
 *    function.
 *    
 *    Call the function MemError to get the result code. See "Memory
 *    Manager Result Codes".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      a handle to a relocatable block.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
EmptyHandle(Handle h)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  HSetRBit()
 *  
 *  Summary:
 *    Set the "R" bit for the handle state.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HSetRBit(Handle h)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  HClrRBit()
 *  
 *  Summary:
 *    Clear the "R" bit for the handle state.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HClrRBit(Handle h)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  HGetState()
 *  
 *  Summary:
 *    Get the current state of the handle's locked, purgeable, and R
 *    bits
 *  
 *  Discussion:
 *    The HGetState function returns a signed byte (char) containing
 *    the flags of the master pointer for the given handle. You can
 *    save this byte, change the state of any of the flags using the
 *    functions described in this section, and then restore their
 *    original states by passing the byte to the HSetState
 *    function.
 *    
 *    You can use bit-manipulation functions on the returned signed
 *    byte to determine the value of a given attribute.
 *    Currently the following bits are used:
 *    kHandleIsResourceBit    - if set, handle is a resource
 *     kHandlePurgeableBit - if set, handle is purgeable
 *     kHandleLockedBit - if set, handle is locked
 *    On Mac OS X and later, heaps are never purged, so the purgeable
 *    bit is used but its setting is essentially ignored. Also, since
 *    heaps are never compacted, and therefore the only time a handle
 *    moves is when that handle is resized, the danger of using
 *    defererenced handles is lower and so handles likely do not need
 *    to be locked as often. Because of this, the state for a handle is
 *    less useful, and HGetState() and other functions is being
 *    deprecated.  If you define a macro named
 *    __MAC_OS_X_MEMORY_MANAGER_CLEAN__ in your sources before you
 *    include MacMemory.h, then HGetState() and several other functions
 *    will become empty operations, removing the overhead of a function
 *    call.
 *    
 *    However, some applications may depend on the state bits of a
 *    handle being correct or changing as functions like HLock(), etc.,
 *    are called.  Applications which rely on this can not use
 *    __MAC_OS_X_MEMORY_MANAGER_CLEAN__.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      the handle to get the state for
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SInt8 
HGetState(Handle h)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  HSetState()
 *  
 *  Summary:
 *    Set the current state of the handle's locked, purgeable, and R
 *    bits
 *  
 *  Discussion:
 *    See the discussion about handle state and Mac OS X above the
 *    function HGetState().
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    h:
 *      the handle to set the state for
 *    
 *    flags:
 *      the flags to set for the handle
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HSetState(
  Handle   h,
  SInt8    flags)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*****************************************************************************

    Beginning in Mac OS X Tiger, BlockMove, BlockMoveData, BlockMoveUncached,
    BlockMoveDataUncached, BlockZero, and BlockZeroUncached are inlined to a
    direct call to the posix memmove or bzero functions; this allows the 
    compiler to optimize in some cases.
    
    However, CarbonCore.framework still exports functions with these names,
    both so old code which linked them there will run and to support
    compilers which don't support inline function definitions.

    To use the exported version of BlockMove, #define NO_BLOCKMOVE_INLINE
    in your source code ( or prefix header file ) before including any headers
    which would include MacMemory.h.
    
*****************************************************************************/

#if ! __LP64__
    #if ! NO_BLOCKMOVE_INLINE && ( defined(__cplusplus) || ( defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L ) )
  static inline void BlockMove(const void *srcPtr,void *destPtr,Size byteCount) { if ( byteCount > 0 ) memmove( destPtr, srcPtr, (size_t)byteCount ); }
  #elif ! NO_BLOCKMOVE_INLINE && defined( __GCC__ ) 
 extern __inline__ void BlockMove(const void *srcPtr,void *destPtr,Size byteCount) { if ( byteCount > 0 ) memmove( destPtr, srcPtr, (size_t)byteCount ); }
  #else
  extern void BlockMove(const void *srcPtr,void *destPtr,Size byteCount);
    #endif
#endif // !__LP64__

#if ! __LP64__
    #if ! NO_BLOCKMOVE_INLINE && ( defined(__cplusplus) || ( defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L ) )
  static inline void BlockMoveData(const void *srcPtr,void *destPtr,Size byteCount) { if ( byteCount > 0 ) memmove( destPtr, srcPtr, (size_t)byteCount ); }
  #elif ! NO_BLOCKMOVE_INLINE && defined( __GCC__ ) 
 extern __inline__ void BlockMoveData(const void *srcPtr,void *destPtr,Size byteCount) { if ( byteCount > 0 ) memmove( destPtr, srcPtr, (size_t)byteCount ); }
  #else
  extern void BlockMoveData(const void *srcPtr,void *destPtr,Size byteCount);
    #endif
#endif // !__LP64__

#if ! __LP64__
    #if ! NO_BLOCKMOVE_INLINE && ( defined(__cplusplus) || ( defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L ) )
  static inline void BlockMoveUncached(const void *srcPtr,void *destPtr,Size byteCount) { if ( byteCount > 0 ) memmove( destPtr, srcPtr, (size_t)byteCount ); }
  #elif ! NO_BLOCKMOVE_INLINE && defined( __GCC__ ) 
 extern __inline__ void BlockMoveUncached(const void *srcPtr,void *destPtr,Size byteCount) { if ( byteCount > 0 ) memmove( destPtr, srcPtr, (size_t)byteCount ); }
  #else
  extern void BlockMoveUncached(const void *srcPtr,void *destPtr,Size byteCount);
    #endif
#endif // !__LP64__

#if ! __LP64__
    #if ! NO_BLOCKMOVE_INLINE && ( defined(__cplusplus) || ( defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L ) )
  static inline void BlockMoveDataUncached(const void *srcPtr,void *destPtr,Size byteCount) { if ( byteCount > 0 ) memmove( destPtr, srcPtr, (size_t)byteCount ); }
  #elif ! NO_BLOCKMOVE_INLINE && defined( __GCC__ ) 
 extern __inline__ void BlockMoveDataUncached(const void *srcPtr,void *destPtr,Size byteCount) { if ( byteCount > 0 ) memmove( destPtr, srcPtr, (size_t)byteCount ); }
  #else
  extern void BlockMoveDataUncached(const void *srcPtr,void *destPtr,Size byteCount);
    #endif
#endif // !__LP64__

#if ! __LP64__
    #if ! NO_BLOCKMOVE_INLINE && ( defined(__cplusplus) || ( defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L ) )
  static inline void BlockZero(void *destPtr,Size byteCount) { if ( byteCount > 0 ) bzero( destPtr, (size_t)byteCount ); }
   #elif ! NO_BLOCKMOVE_INLINE && defined( __GCC__ ) 
 extern __inline__ void BlockZero(void *destPtr,Size byteCount) { if ( byteCount > 0 ) bzero( destPtr, (size_t)byteCount ); }
   #else
  extern void BlockZero(void *destPtr,Size byteCount);
   #endif
#endif // !__LP64__

#if ! __LP64__
    #if ! NO_BLOCKMOVE_INLINE && ( defined(__cplusplus) || ( defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L ) )
  static inline void BlockZeroUncached(void *destPtr,Size byteCount) { if ( byteCount > 0 ) bzero( destPtr, (size_t)byteCount ); }
   #elif ! NO_BLOCKMOVE_INLINE && defined( __GCC__ ) 
 extern __inline__ void BlockZeroUncached(void *destPtr,Size byteCount) { if ( byteCount > 0 ) bzero( destPtr, (size_t)byteCount ); }
   #else
  extern void BlockZeroUncached(void *destPtr,Size byteCount);
   #endif
#endif // !__LP64__
/*
 *  HandToHand()
 *  
 *  Summary:
 *    Copies all of the data from one relocatable block to a new
 *    relocatable block.
 *  
 *  Discussion:
 *    The HandToHand function attempts to copy the information in the
 *    relocatable block to which theHndl is a handle; if successful,
 *    HandToHand sets theHndlto a handle pointing to the new
 *    relocatable block.
 *    
 *    If successful in creating a new relocatable block, the HandToHand
 *    function does not duplicate the properties of the original block.
 *    The new block is unlocked, unpurgeable, and not a resource. Call
 *    HLock or HPurge to adjust the properties of the new
 *    block.
 *    
 *    To copy only part of a relocatable block into a new relocatable
 *    block, use the PtrToHand function. Before calling PtrToHand, lock
 *    and dereference the handle pointing to the relocatable block you
 *    want to copy.
 *    
 *    Because HandToHand replaces its parameter with the new handle,
 *    you should retain the original parameter value somewhere else,
 *    otherwise you will not be able to access it.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    theHndl:
 *      a handle to the relocatable block whose data HandToHand will
 *      copy.  On return, theHndl contains a handle to a new
 *      relocatable block whose data duplicates the original.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HandToHand(Handle * theHndl)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PtrToXHand()
 *  
 *  Summary:
 *    Copies data referenced by a pointer to an existing relocatable
 *    block.
 *  
 *  Discussion:
 *    The PtrToXHand function copies the specified number of bytes from
 *    the location specified by srcPtr to the handle specified by
 *    dstHndl.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    srcPtr:
 *      the address of the first byte to copy.
 *    
 *    dstHndl:
 *      a handle to an existing relocatable block.
 *    
 *    size:
 *      the number of bytes to copy.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PtrToXHand(
  const void *  srcPtr,
  Handle        dstHndl,
  long          size)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PtrToHand()
 *  
 *  Summary:
 *    Copies data referenced by a pointer to a new relocatable block.
 *  
 *  Discussion:
 *    If you dereference and lock a handle, the PtrToHand function can
 *    copy its data to a new handle. However, for copying data from one
 *    handle to another, the HandToHand function is more efficient.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    srcPtr:
 *      the address of the first byte to copy.
 *    
 *    dstHndl:
 *      a handle for which you have not yet allocated any memory. The
 *      PtrToHand function allocates memory for the handle and copies
 *      the specified number of bytes beginning at srcPtr into it. The
 *      dstHndl parameter is an output parameter that will hold the
 *      result. Its value on entry is ignored. If no error occurs, on
 *      exit it points to an unlocked, non-purgeable Handle of the
 *      requested size.
 *    
 *    size:
 *      the number of bytes to copy.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PtrToHand(
  const void *  srcPtr,
  Handle *      dstHndl,
  long          size)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  HandAndHand()
 *  
 *  Summary:
 *    Use the HandAndHand function to concatenate two relocatable
 *    blocks.
 *  
 *  Discussion:
 *    The HandAndHand function concatenates the information from the
 *    relocatable block to which aHndl is a handle onto the end of the
 *    relocatable block to which bHndl is a handle. The aHndl variable
 *    remains unchanged.
 *    
 *    WARNING
 *    
 *    The HandAndHand function dereferences the handle aHndl. You must
 *    call the HLock procedure to lock the block before calling
 *    HandAndHand. Afterward, you can call the HUnlock procedure to
 *    unlock it. Alternatively, you can save the block's original state
 *    by calling the HGetState function, lock the block by calling
 *    HLock, and then restore the original settings by calling
 *    HSetState.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    hand1:
 *      A handle to the first relocatable block, whose contents do not
 *      change but are concatenated to the end of the second
 *      relocatable block.
 *    
 *    hand2:
 *      A handle to the second relocatable block, whose size the Memory
 *      Manager expands so that it can concatenate the information from
 *      aHndl to the end of the contents of this block.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HandAndHand(
  Handle   hand1,
  Handle   hand2)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  PtrAndHand()
 *  
 *  Summary:
 *    Concatenates part or all of a memory block to the end of a
 *    relocatable block.
 *  
 *  Discussion:
 *    The PtrAndHand function takes the number of bytes specified by
 *    the size parameter, beginning at the location specified by ptr1,
 *    and concatenates them onto the end of the relocatable block to
 *    which hand2 is a handle. The contents of the source block remain
 *    unchanged.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    ptr1:
 *      a pointer to the beginning of the data that the Memory Manager
 *      is to concatenate onto the end of the relocatable block.
 *    
 *    hand2:
 *      a handle to the relocatable block, whose size the Memory
 *      Manager expands so that it can concatenate the information from
 *      ptr1 onto the end of this block.
 *    
 *    size:
 *      the number of bytes of the block referenced by ptr1 to copy.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PtrAndHand(
  const void *  ptr1,
  Handle        hand2,
  long          size)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#if !__LP64__
/*
 *  MoreMasters()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    On Mac OS X and later, master pointers do not need to be
 *    pre-allocated.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
MoreMasters(void)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  MoreMasterPointers()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    On Mac OS X and later, master pointers do not need to be
 *    pre-allocated.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inCount:
 *      the number of master pointers to preallocate
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
MoreMasterPointers(UInt32 inCount)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*  Temporary Memory routines renamed, but obsolete, in System 7.0 and later.  */
/*
 *  TempHLock()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    This function has been deprecated for many years; replace it with
 *    HLock()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
TempHLock(
  Handle   h,
  OSErr *  resultCode)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  TempHUnlock()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    This function has been deprecated for many years; replace it with
 *    HUnlock()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
TempHUnlock(
  Handle   h,
  OSErr *  resultCode)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  TempDisposeHandle()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    This function has been deprecated for many years; replace it with
 *    DisposeHandle()
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
TempDisposeHandle(
  Handle   h,
  OSErr *  resultCode)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  TempTopMem()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Mac OS X and later does not have a seperate temporary memory
 *    heap.  This function returns NULL.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Ptr 
TempTopMem(void)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  HoldMemory()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Mac OS X has never supported HoldMemory.  The functions in
 *    ./sys/mman.h may be useful for replacing usage of these
 *    functions, although Mac OS X does not allow the same level of
 *    control over whether pages are held in memory or resident as Mac
 *    OS 9.x did.
 *    If you define a macro __MAC_OS_X_MEMORY_MANAGER_CLEAN__ in your
 *    sources before you include MacMemory.h, then any calls to
 *    HoldMemory() in your program will essentially be removed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
HoldMemory(
  void *          address,
  unsigned long   count)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  UnholdMemory()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Mac OS X has never supported MakeMemoryResident.  See the comment
 *    above UnholdMemory for more information.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
UnholdMemory(
  void *          address,
  unsigned long   count)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  MakeMemoryResident()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Mac OS X has never supported MakeMemoryResident.  See the comment
 *    above UnholdMemory for more information.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    address:
 *      the address to make resident
 *    
 *    count:
 *      the count of pages to make make resident
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr 
MakeMemoryResident(
  void *          address,
  unsigned long   count)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  ReleaseMemoryData()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Mac OS X has never supported MakeMemoryResident.  See the comment
 *    above UnholdMemory for more information.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    address:
 *      the address to make release
 *    
 *    count:
 *      the count of pages to make make release
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr 
ReleaseMemoryData(
  void *          address,
  unsigned long   count)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  MakeMemoryNonResident()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Mac OS X has never supported MakeMemoryResident.  See the comment
 *    above UnholdMemory for more information.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    address:
 *      the address to make non-resident
 *    
 *    count:
 *      the count of pages to make make non-resident
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr 
MakeMemoryNonResident(
  void *          address,
  unsigned long   count)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  FlushMemory()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Mac OS X has never supported MakeMemoryResident.  See the comment
 *    above UnholdMemory for more information.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    address:
 *      the address to flush
 *    
 *    count:
 *      the count of pages to make flush
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern OSErr 
FlushMemory(
  void *          address,
  unsigned long   count)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/*
 *  GZSaveHnd()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X and always returns NULL.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
GZSaveHnd(void)                                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  TopMem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X and always returns NULL.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Ptr 
TopMem(void)                                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  ReserveMem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ReserveMem(Size cbNeeded)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PurgeSpace()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X.
 *  
 *  Discussion:
 *    On Mac OS X and later, heaps are never purged and therefore
 *    PurgeSpace will always return a large value for both the total
 *    space available and the largest block available.  You can assume
 *    that any reasonable memory allocation will succeed.
 *    
 *    If you define a macro __MAC_OS_X_MEMORY_MANAGER_CLEAN__ in your
 *    sources before you include MacMemory.h, then any calls to
 *    PurgeSpace() in your program will essentially be removed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
PurgeSpace(
  long *  total,
  long *  contig)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PurgeSpaceTotal()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X.
 *  
 *  Discussion:
 *    On Mac OS X and later, heaps are never purged and therefore
 *    PurgeSpaceTotal will always return a large value. You can assume
 *    that any reasonable memory allocation will succeed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern long 
PurgeSpaceTotal(void)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PurgeSpaceContiguous()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X.
 *  
 *  Discussion:
 *    On Mac OS X and later, heaps are never purged and therefore
 *    PurgeSpaceContiguous will always return a large value. You can
 *    assume that any reasonable memory allocation will succeed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern long 
PurgeSpaceContiguous(void)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/* Carbon routines to aid in debugging. */
/*
 *  CheckAllHeaps()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Check all known heaps for validity.  Deprecated on Mac OS X,
 *    since there really aren't heaps.  Use IsHeapValid() if you really
 *    want this functionality.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CheckAllHeaps(void)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  IsHeapValid()
 *  
 *  Summary:
 *    Check if the heap is valid.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsHeapValid(void)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* It is invalid to pass a NULL or an empty Handle to IsHandleValid */
/*
 *  IsHandleValid()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsHandleValid(Handle h)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* It is invalid to pass a NULL Pointer to IsPointerValid */
/*
 *  IsPointerValid()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsPointerValid(Ptr p)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#if !__LP64__
#if OLDROUTINENAMES
#define ApplicZone() ApplicationZone()
#define MFTempNewHandle(logicalSize, resultCode) TempNewHandle(logicalSize, resultCode)
#define MFMaxMem(grow) TempMaxMem(grow)
#define MFFreeMem() TempFreeMem()
#define MFTempHLock(h, resultCode) TempHLock(h, resultCode)
#define MFTempHUnlock(h, resultCode) TempHUnlock(h, resultCode)
#define MFTempDisposHandle(h, resultCode) TempDisposeHandle(h, resultCode)
#define MFTopMem() TempTopMem()
#define ResrvMem(cbNeeded) ReserveMem(cbNeeded)
#define DisposPtr(p) DisposePtr(p)
#define DisposHandle(h) DisposeHandle(h)
#define ReallocHandle(h, byteCount) ReallocateHandle(h, byteCount)
#endif  /* OLDROUTINENAMES */

#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  LMGetSysZone()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern THz 
LMGetSysZone(void)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  LMSetSysZone()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
LMSetSysZone(THz value)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  LMGetApplZone()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern THz 
LMGetApplZone(void)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  LMSetApplZone()   *** DEPRECATED ***
 *  
 *  Summary:
 *    This function is deprecated on Mac OS X.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
LMSetApplZone(THz value)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



#endif  /* !__LP64__ */

#if __MAC_OS_X_MEMORY_MANAGER_CLEAN__

#ifdef __cplusplus
    inline void HLock(Handle ) { LMSetMemErr(0); }
    inline void HLockHi(Handle ) { LMSetMemErr(0); }
    inline void HUnlock(Handle ) { LMSetMemErr(0); }
    inline void HSetRBit(Handle ) { LMSetMemErr(0); }
    inline void HClrRBit(Handle ) { LMSetMemErr(0); }
    inline SInt8 HGetState(Handle ) { LMSetMemErr(0); return 0; }
    inline void HSetState(Handle , SInt8 ) { LMSetMemErr(0); }
#if !__LP64__
    inline void HPurge(Handle ) { LMSetMemErr(0); }
    inline void HNoPurge(Handle ) { LMSetMemErr(0); }
    inline void MoveHHi(Handle ) { LMSetMemErr(0); }
    inline void TempHLock(Handle , OSErr *resultCode) { LMSetMemErr(0), *resultCode = 0; }
    inline void TempHUnlock(Handle , OSErr *resultCode) { LMSetMemErr(0), *resultCode = 0; }

    inline OSErr HoldMemory(void *, unsigned long ) { return noErr; }
    inline OSErr UnholdMemory(void *, unsigned long ) { return noErr; }
    inline OSErr MakeMemoryResident(void *, unsigned long ) { return noErr; }
    inline OSErr ReleaseMemoryData(void *, unsigned long ) { return noErr; }
    inline OSErr MakeMemoryNonResident(void *, unsigned long ) { return noErr; }
    inline OSErr FlushMemory(void *, unsigned long ) { return noErr; }
    inline void ReserveMem(Size ) { LMSetMemErr(0); }
    inline void PurgeSpace(long *, long *) { LMSetMemErr(0); }
    inline long PurgeSpaceTotal() { LMSetMemErr(0); }
    inline long PurgeSpaceContiguous() { LMSetMemErr(0); }
#endif


#else
    #define HLock(h) { LMSetMemErr(0); }
    #define HLockHi(h) { LMSetMemErr(0); }
    #define HUnlock(h) { LMSetMemErr(0); }
    #define HSetRBit(h) { LMSetMemErr(0); }
    #define HClrRBit(h) { LMSetMemErr(0); }
    #define HGetState(h) ( LMSetMemErr(0), 0 )
    #define HSetState(h, flags) { LMSetMemErr(0); }
#if !__LP64__
    #define HPurge(h) { LMSetMemErr(0); }
    #define HNoPurge(h) { LMSetMemErr(0); }
    #define MoveHHi(h) { LMSetMemErr(0); }
    #define TempHLock(h, resultCode) { LMSetMemErr(0) ; *resultCode = 0; }
    #define TempHUnlock(h, resultCode) { LMSetMemErr(0) ; *resultCode = 0; }

    #define HoldMemory(address, count) ( address, count, noErr)
    #define UnholdMemory(address, count) ( address, count, noErr)
    #define MakeMemoryResident(address, count) ( address, count, noErr)
    #define ReleaseMemoryData(address, count) ( address, count, noErr)
    #define MakeMemoryNonResident(address, count) ( address, count, noErr)
    #define FlushMemory(address, count) ( address, count, noErr)

    #define ReserveMem(cbNeeded) { LMSetMemErr(0) }
    #define PurgeSpace(total, contig) { LMSetMemErr(0); }
    #define PurgeSpaceTotal() ( LMSetMemErr(0), 0 )
    #define    PurgeSpaceContiguous()  ( LMSetMemErr(0), 0 )
#endif
#endif

#endif  /* __MAC_OS_X_MEMORY_MANAGER_CLEAN__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __MACMEMORY__ */

