/*
     File:       CarbonCore/MixedMode.h
 
     Contains:   Mixed Mode Manager Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1992-2011 by Apple Inc. All rights reserved.
*/
#ifndef __MIXEDMODE__
#define __MIXEDMODE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#pragma pack(push, 2)

/*
    ===========================================================================
    
    Notes on Mixed Mode and Mac OS X
    
    Mac OS X doesn't have mixed mode, nor the Mixed Mode Manager, because
    everything running is PowerPC code.  Therefore, there is little need for
    anything in this file unless the code still needs to run on Mac OS 9.x
    CarbonLib, and on Mac OS X.

    =========================================================================== 
*/
/* Mixed Mode constants */
/* Current Routine Descriptor Version */
enum {
  kRoutineDescriptorVersion     = 7
};

/* MixedModeMagic Magic Cookie/Trap number */
enum {
  _MixedModeMagic               = 0xAAFE
};

/* MixedModeState Version for CFM68K Mixed Mode */
enum {
  kCurrentMixedModeStateRecord  = 1
};

/* Calling Conventions */
typedef unsigned short                  CallingConventionType;
enum {
  kPascalStackBased             = 0,
  kCStackBased                  = 1,
  kRegisterBased                = 2,
  kD0DispatchedPascalStackBased = 8,
  kD1DispatchedPascalStackBased = 12,
  kD0DispatchedCStackBased      = 9,
  kStackDispatchedPascalStackBased = 14,
  kThinkCStackBased             = 5
};

/* ISA Types */
typedef SInt8                           ISAType;
enum {
  kM68kISA                      = 0,
  kPowerPCISA                   = 1
};

enum {
  kX86ISA                       = 2
};

/* RTA Types */
typedef SInt8                           RTAType;
enum {
  kOld68kRTA                    = 0 << 4,
  kPowerPCRTA                   = 0 << 4,
  kCFM68kRTA                    = 1 << 4
};

enum {
  kX86RTA                       = 2 << 4
};


#if TARGET_OS_MAC
 #if TARGET_CPU_PPC
     #define     GetCurrentISA()     ((ISAType) kPowerPCISA)
        #define     GetCurrentRTA()     ((RTAType) kPowerPCRTA)

   #elif TARGET_CPU_X86
       #define     GetCurrentISA()     ((ISAType) kX86ISA)
        #define     GetCurrentRTA()     ((RTAType) kX86RTA)

   #endif
 #define     GetCurrentArchitecture()    (GetCurrentISA() | GetCurrentRTA())
#else
  #define     GetCurrentArchitecture()    0
#endif

/* Constants for specifing 68k registers */
enum {
  kRegisterD0                   = 0,
  kRegisterD1                   = 1,
  kRegisterD2                   = 2,
  kRegisterD3                   = 3,
  kRegisterD4                   = 8,
  kRegisterD5                   = 9,
  kRegisterD6                   = 10,
  kRegisterD7                   = 11,
  kRegisterA0                   = 4,
  kRegisterA1                   = 5,
  kRegisterA2                   = 6,
  kRegisterA3                   = 7,
  kRegisterA4                   = 12,
  kRegisterA5                   = 13,
  kRegisterA6                   = 14,   /* A7 is the same as the PowerPC SP */
  kCCRegisterCBit               = 16,
  kCCRegisterVBit               = 17,
  kCCRegisterZBit               = 18,
  kCCRegisterNBit               = 19,
  kCCRegisterXBit               = 20
};

typedef unsigned short                  registerSelectorType;
/* SizeCodes we use everywhere */
enum {
  kNoByteCode                   = 0,
  kOneByteCode                  = 1,
  kTwoByteCode                  = 2,
  kFourByteCode                 = 3
};

/* Mixed Mode Routine Records */
typedef unsigned long                   ProcInfoType;
/* Routine Flag Bits */
typedef unsigned short                  RoutineFlagsType;
enum {
  kProcDescriptorIsAbsolute     = 0x00,
  kProcDescriptorIsRelative     = 0x01
};

enum {
  kFragmentIsPrepared           = 0x00,
  kFragmentNeedsPreparing       = 0x02
};

enum {
  kUseCurrentISA                = 0x00,
  kUseNativeISA                 = 0x04
};

enum {
  kPassSelector                 = 0x00,
  kDontPassSelector             = 0x08
};

enum {
  kRoutineIsNotDispatchedDefaultRoutine = 0x00,
  kRoutineIsDispatchedDefaultRoutine = 0x10
};

enum {
  kProcDescriptorIsProcPtr      = 0x00,
  kProcDescriptorIsIndex        = 0x20
};

struct RoutineRecord {
  ProcInfoType        procInfo;               /* calling conventions */
  SInt8               reserved1;              /* Must be 0 */
  ISAType             ISA;                    /* Instruction Set Architecture */
  RoutineFlagsType    routineFlags;           /* Flags for each routine */
  ProcPtr             procDescriptor;         /* Where is the thing weÕre calling? */
  UInt32              reserved2;              /* Must be 0 */
  UInt32              selector;               /* For dispatched routines, the selector */
};
typedef struct RoutineRecord            RoutineRecord;
typedef RoutineRecord *                 RoutineRecordPtr;
typedef RoutineRecordPtr *              RoutineRecordHandle;
/* Mixed Mode Routine Descriptors */
/* Definitions of the Routine Descriptor Flag Bits */
typedef UInt8                           RDFlagsType;
enum {
  kSelectorsAreNotIndexable     = 0x00,
  kSelectorsAreIndexable        = 0x01
};

/* Routine Descriptor Structure */
struct RoutineDescriptor {
  UInt16              goMixedModeTrap;        /* Our A-Trap */
  SInt8               version;                /* Current Routine Descriptor version */
  RDFlagsType         routineDescriptorFlags; /* Routine Descriptor Flags */
  UInt32              reserved1;              /* Unused, must be zero */
  UInt8               reserved2;              /* Unused, must be zero */
  UInt8               selectorInfo;           /* If a dispatched routine, calling convention, else 0 */
  UInt16              routineCount;           /* Number of routines in this RD */
  RoutineRecord       routineRecords[1];      /* The individual routines */
};
typedef struct RoutineDescriptor        RoutineDescriptor;
typedef RoutineDescriptor *             RoutineDescriptorPtr;
typedef RoutineDescriptorPtr *          RoutineDescriptorHandle;
/* 68K MixedModeStateRecord */
struct MixedModeStateRecord {
  UInt32              state1;
  UInt32              state2;
  UInt32              state3;
  UInt32              state4;
};
typedef struct MixedModeStateRecord     MixedModeStateRecord;
/*
 *  NewRoutineDescriptor()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    This function is deprecated on Mac OS X and in CarbonLib because
 *    routine descriptors existed to allow 68k code and PowerPC code to
 *    call each other and get parameter marshalling and other OS
 *    services.  Mac OS X is entirely PowerPC native, and does not
 *    require the use of mixed mode.
 *    You should remove any calls to NewRoutineDescriptor() from your
 *    sources, and replace them with theProc parameter itself.
 *  
 *  Availability:
 *    Mac OS X:         not available but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  DisposeRoutineDescriptor()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    DisposeRoutineDescriptor() is deprecated on Mac OS X. 
 *    RoutineDescriptors are no longer used.
 *    You should delete any calls to DisposeRoutineDescriptor() from
 *    your sources.
 *  
 *  Availability:
 *    Mac OS X:         not available but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */



   #define NewRoutineDescriptor(theProc, procInfo, isa) ((UniversalProcPtr) theProc)
  #define DisposeRoutineDescriptor(upp)



/* Mixed Mode ProcInfos */
enum {
                                        /* Calling Convention Offsets */
  kCallingConventionWidth       = 4,
  kCallingConventionPhase       = 0,
  kCallingConventionMask        = 0x0F, /* Result Offsets */
  kResultSizeWidth              = 2,
  kResultSizePhase              = kCallingConventionWidth,
  kResultSizeMask               = 0x30, /* Parameter offsets & widths */
  kStackParameterWidth          = 2,
  kStackParameterPhase          = (kCallingConventionWidth + kResultSizeWidth),
  kStackParameterMask           = (int)0xFFFFFFC0, /* Register Result Location offsets & widths */
  kRegisterResultLocationWidth  = 5,
  kRegisterResultLocationPhase  = (kCallingConventionWidth + kResultSizeWidth), /* Register Parameter offsets & widths */
  kRegisterParameterWidth       = 5,
  kRegisterParameterPhase       = (kCallingConventionWidth + kResultSizeWidth + kRegisterResultLocationWidth),
  kRegisterParameterMask        = 0x7FFFF800,
  kRegisterParameterSizePhase   = 0,
  kRegisterParameterSizeWidth   = 2,
  kRegisterParameterWhichPhase  = kRegisterParameterSizeWidth,
  kRegisterParameterWhichWidth  = 3,    /* Dispatched Stack Routine Selector offsets & widths */
  kDispatchedSelectorSizeWidth  = 2,
  kDispatchedSelectorSizePhase  = (kCallingConventionWidth + kResultSizeWidth), /* Dispatched Stack Routine Parameter offsets */
  kDispatchedParameterPhase     = (kCallingConventionWidth + kResultSizeWidth + kDispatchedSelectorSizeWidth), /* Special Case offsets & widths */
  kSpecialCaseSelectorWidth     = 6,
  kSpecialCaseSelectorPhase     = kCallingConventionWidth,
  kSpecialCaseSelectorMask      = 0x03F0
};

enum {
  kSpecialCase                  = 0x000F /* (CallingConventionType) */
};

enum {
                                        /* all of the special cases enumerated.  The selector field is 6 bits wide */
  kSpecialCaseHighHook          = 0,
  kSpecialCaseCaretHook         = 0,    /* same as kSpecialCaseHighHook */
  kSpecialCaseEOLHook           = 1,
  kSpecialCaseWidthHook         = 2,
  kSpecialCaseTextWidthHook     = 2,    /* same as kSpecialCaseWidthHook */
  kSpecialCaseNWidthHook        = 3,
  kSpecialCaseDrawHook          = 4,
  kSpecialCaseHitTestHook       = 5,
  kSpecialCaseTEFindWord        = 6,
  kSpecialCaseProtocolHandler   = 7,
  kSpecialCaseSocketListener    = 8,
  kSpecialCaseTERecalc          = 9,
  kSpecialCaseTEDoText          = 10,
  kSpecialCaseGNEFilterProc     = 11,
  kSpecialCaseMBarHook          = 12
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *  Macros for building ProcInfos.  Examples:
 *  
 *  
 *  uppModalFilterProcInfo = kPascalStackBased
 *       | RESULT_SIZE(SIZE_CODE(sizeof(Boolean)))
 *       | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(DialogRef)))
 *       | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EventRecord*)))
 *       | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(short*))),
 *
 *  uppDeskHookProcInfo = kRegisterBased
 *       | REGISTER_ROUTINE_PARAMETER(1, kRegisterD0, SIZE_CODE(sizeof(Boolean)))
 *       | REGISTER_ROUTINE_PARAMETER(2, kRegisterA0, SIZE_CODE(sizeof(EventRecord*)))
 *
 *  uppGXSpoolResourceProcInfo = kCStackBased
 *       | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
 *       | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(gxSpoolFile)))
 *       | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Handle)))
 *       | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ResType)))
 *       | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(long)))
 *
 *  uppTEFindWordProcInfo = SPECIAL_CASE_PROCINFO( 6 ),
 *
 */


/* * * * * * * * * * * * * * 
 *   SIZE_CODE -     Return the size code for an object, given its size in bytes.
 *     size - size of an object in bytes
 */
#define SIZE_CODE(size) \
    (((size) == 4) ? kFourByteCode : (((size) == 2) ? kTwoByteCode : (((size) == 1) ? kOneByteCode : 0)))


/* * * * * * * * * * * * * * 
 *   RESULT_SIZE -   Return the result field of a ProcInfo, given the return objectÕs size.
 *                   This is the same for all ProcInfos
 *       sizeCode - size code
 */
#define RESULT_SIZE(sizeCode) \
   ((ProcInfoType)(sizeCode) << kResultSizePhase)


/* * * * * * * * * * * * * * 
 *   STACK_ROUTINE_PARAMETER -   Return a parameter field of a ProcInfo, for a simple,
 *                                non-dispatched, stack based routine.
 *     whichParam - which parameter
 *     sizeCode - size code
 */
#define STACK_ROUTINE_PARAMETER(whichParam, sizeCode) \
   ((ProcInfoType)(sizeCode) << (kStackParameterPhase + (((whichParam) - 1) * kStackParameterWidth)))


/* * * * * * * * * * * * * * 
 *   DISPATCHED_STACK_ROUTINE_PARAMETER -    Return a parameter field of a ProcInfo, for 
 *                                         a dispatched, stack based routine.  The same
 *                                         macro is used regardless of the type of
 *                                          dispatching.
 *     whichParam - which parameter
 *     sizeCode - size code
 */
#define DISPATCHED_STACK_ROUTINE_PARAMETER(whichParam, sizeCode) \
    ((ProcInfoType)(sizeCode) << (kDispatchedParameterPhase + (((whichParam) - 1) * kStackParameterWidth)))


/* * * * * * * * * * * * * * 
 *   DISPATCHED_STACK_ROUTINE_SELECTOR_SIZE -    Return a the selector size field of a ProcInfo
 *                                               for a dispatched, stack based routine.  The
 *                                              same macro is used regardless of the type of
 *                                             dispatching.
 *     sizeCode - size code 
 */
#define DISPATCHED_STACK_ROUTINE_SELECTOR_SIZE(sizeCode) \
   ((ProcInfoType)(sizeCode) << kDispatchedSelectorSizePhase)


/* * * * * * * * * * * * * * 
 *   REGISTER_RESULT_LOCATION -      Return the Result Location field of a ProcInfo, 
 *                                 given the location.
 *      whichReg - which register
 */
#define REGISTER_RESULT_LOCATION(whichReg) \
 ((ProcInfoType)(whichReg) << kRegisterResultLocationPhase)


/* * * * * * * * * * * * * * 
 *   REGISTER_ROUTINE_PARAMETER -    Return a parameter field of a ProcInfo for a 
 *                                    register based routine.
 */
#define REGISTER_ROUTINE_PARAMETER(whichParam, whichReg, sizeCode) \
   ((((ProcInfoType)(sizeCode) << kRegisterParameterSizePhase) | ((ProcInfoType)(whichReg) << kRegisterParameterWhichPhase)) << \
         (kRegisterParameterPhase + (((whichParam) - 1) * kRegisterParameterWidth)))


/* * * * * * * * * * * * * * 
 *
 *    SPECIAL_CASE_PROCINFO -     Returns the procInfo constant for the following special cases:
 *   
 *     High Hook & Caret Hook -  (see I-379)
 *            C calling conventions, Rect on stack, pointer in A3, no return value
 *     EOL Hook - (see VI-15-26)
 *            Register-based; inputs in D0, A3, A4; 
 *           output is Z flag of status register
 *      Width Hook - (see VI-15-27)
 *          Register-based; inputs in D0, D1, A0, A3, A4; output in D1 
 *      NWidth Hook - (see VI-15-27)
 *         Register-based; inputs in D0, D1, D2, A0, A2, A3, A4; output in D1 
 *      TextWidthHook - (see VI-15-28)
 *           Register-based; inputs in D0, D1, A0, A3, A4; output in D1 
 *      DrawHook - (see VI-15-28)
 *            Register-based; inputs in D0, D1, A0, A3, A4; no output
 *      HitTestHook - (See VI-15-29)
 *         Register-based; inputs in D0, D1, D2, A0, A3, A4; outputs in D0, D1, D2
 *      FindWord -  (see VI-15-30)
 *           Register-based; inputs in D0, D2, A3, A4; outputs in D0, D1
 *      ADBRoutines - (see V-371)
 *            Register-based; inputs in A0, A1, A2, D0; no outputs
 *     ProtocolHandler - (see II-326)
 *           Register-based; inputs in A0, A1, A2, A3, A4, D1.w; output in Z 
 *     SocketListener - (see II-329)
 *            Register-based; inputs in A0, A1, A2, A3, A4, D0.b, D1.w; output in Z 
 *       Reclac - (see I-391)
 *         Register-based; inputs in A3, D7; outputs in D2, D3, D4
 *      DoText - (see I-391)
 *         Register-based; inputs in A3, D3, D4, D7; outputs in A0, D0
 *      GNEFilterProc - (see tech note 85)
 *           Register & Stack Based; inputs in A1, D0 & on the stack; outputs on the stack
 *        MenuBarHook - (see I-356)
 *            Register & Stack Based; input on the stack; output in D0
*/
#define SPECIAL_CASE_PROCINFO(specialCaseCode)          \
      (kSpecialCase | ((ProcInfoType)(specialCaseCode) << 4))


/* * * * * * * * * * * * * * * 
 * STACK_UPP_TYPE    - used in typedefs to create ÅUPP type
 * REGISTER_UPP_TYPE - used in typedefs to create ÅUPP type
 * TVECTOR_UPP_TYPE  - used in typedefs to create ÅUPP type
 *
 *  Example:
 *
 *      typedef STACK_UPP_TYPE(ModalFilterProcPtr)      ModalFilterUPP;
 *      typedef REGISTER_UPP_TYPE(IOCompletionProcPtr)  IOCompletionUPP;
 *
 */
#if TARGET_OS_MAC && TARGET_RT_MAC_CFM
 /* PowerPC and CFM68K runtime */
   #if OPAQUE_UPP_TYPES
       #define STACK_UPP_TYPE(name)    struct Opaque##name##*
     #define REGISTER_UPP_TYPE(name) struct Opaque##name##*
     #define TVECTOR_UPP_TYPE(name)  struct Opaque##name##*
 #else
      #define STACK_UPP_TYPE(name)    UniversalProcPtr
       #define REGISTER_UPP_TYPE(name) UniversalProcPtr
       #define TVECTOR_UPP_TYPE(name)  name
   #endif
#elif TARGET_OS_MAC && TARGET_RT_MAC_MACHO
  /* Mac OS X runtime */
 #define STACK_UPP_TYPE(name)    name
   #define REGISTER_UPP_TYPE(name) name
   #define TVECTOR_UPP_TYPE(name)  name
#else
 /* other runtimes */
   #define STACK_UPP_TYPE(name)    name
   #define REGISTER_UPP_TYPE(name) name
   #define TVECTOR_UPP_TYPE(name)  name
#endif


/* * * * * * * * * * * * * * * 
 * CALL_Å_PARAMETER_UPP  - used in CallÅProc macros
 *
 *  Example:
 *
 *      #define CallIOCompletionProc(userRoutine, paramBlock)   \
 *                CALL_TWO_PARAMETER_UPP((userRoutine), uppIOCompletionProcInfo, (paramBlock))
 *
 */
#if TARGET_OS_MAC && TARGET_RT_MAC_CFM
    #define CALL_ZERO_PARAMETER_UPP(    upp, procInfo)      CallUniversalProc(upp, procInfo)
    #define CALL_ONE_PARAMETER_UPP(     upp, procInfo, p1)      CallUniversalProc(upp, procInfo, (p1))
    #define CALL_TWO_PARAMETER_UPP(     upp, procInfo, p1, p2)      CallUniversalProc(upp, procInfo, (p1), (p2))
    #define CALL_THREE_PARAMETER_UPP(   upp, procInfo, p1, p2, p3)      CallUniversalProc(upp, procInfo, (p1), (p2), (p3))
    #define CALL_FOUR_PARAMETER_UPP(    upp, procInfo, p1, p2, p3, p4)      CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4))
    #define CALL_FIVE_PARAMETER_UPP(    upp, procInfo, p1, p2, p3, p4, p5)      CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5))
    #define CALL_SIX_PARAMETER_UPP(     upp, procInfo, p1, p2, p3, p4, p5, p6)      CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5), (p6))
    #define CALL_SEVEN_PARAMETER_UPP(   upp, procInfo, p1, p2, p3, p4, p5, p6, p7)      CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5), (p6), (p7))
    #define CALL_EIGHT_PARAMETER_UPP(   upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8)      CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8))
    #define CALL_NINE_PARAMETER_UPP(    upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9)      CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9))
    #define CALL_TEN_PARAMETER_UPP(     upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)     CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9), (p10))
    #define CALL_ELEVEN_PARAMETER_UPP(  upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)    CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9), (p10), (p11))
    #define CALL_TWELVE_PARAMETER_UPP(  upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)   CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9), (p10), (p11), (p12))
    #define CALL_THIRTEEN_PARAMETER_UPP(upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)  CallUniversalProc(upp, procInfo, (p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9), (p10), (p11), (p12), (p13))
#else
    #define CALL_ZERO_PARAMETER_UPP(    upp, procInfo)      (*(upp))()
    #define CALL_ONE_PARAMETER_UPP(     upp, procInfo, p1)      (*(upp))((p1))
    #define CALL_TWO_PARAMETER_UPP(     upp, procInfo, p1, p2)      (*(upp))((p1), (p2))
    #define CALL_THREE_PARAMETER_UPP(   upp, procInfo, p1, p2, p3)      (*(upp))((p1), (p2), (p3))
    #define CALL_FOUR_PARAMETER_UPP(    upp, procInfo, p1, p2, p3, p4)      (*(upp))((p1), (p2), (p3), (p4))
    #define CALL_FIVE_PARAMETER_UPP(    upp, procInfo, p1, p2, p3, p4, p5)      (*(upp))((p1), (p2), (p3), (p4), (p5))
    #define CALL_SIX_PARAMETER_UPP(     upp, procInfo, p1, p2, p3, p4, p5, p6)      (*(upp))((p1), (p2), (p3), (p4), (p5), (p6))
    #define CALL_SEVEN_PARAMETER_UPP(   upp, procInfo, p1, p2, p3, p4, p5, p6, p7)      (*(upp))((p1), (p2), (p3), (p4), (p5), (p6), (p7))
    #define CALL_EIGHT_PARAMETER_UPP(   upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8)      (*(upp))((p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8))
    #define CALL_NINE_PARAMETER_UPP(    upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9)      (*(upp))((p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9))
    #define CALL_TEN_PARAMETER_UPP(     upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)     (*(upp))((p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9), (p10))
    #define CALL_ELEVEN_PARAMETER_UPP(  upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)    (*(upp))((p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9), (p10), (p11))
    #define CALL_TWELVE_PARAMETER_UPP(  upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)   (*(upp))((p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9), (p10), (p11), (p12))
    #define CALL_THIRTEEN_PARAMETER_UPP(upp, procInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)  (*(upp))((p1), (p2), (p3), (p4), (p5), (p6), (p7), (p8), (p9), (p10), (p11), (p12), (p13))
#endif





#pragma pack(pop)


#endif /* __MIXEDMODE__ */

