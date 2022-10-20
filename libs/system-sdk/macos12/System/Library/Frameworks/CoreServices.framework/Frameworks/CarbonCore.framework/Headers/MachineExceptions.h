/*
     File:       CarbonCore/MachineExceptions.h
 
     Contains:   Processor Exception Handling Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1993-2011 by Apple Inc. All rights reserved.
*/
#ifndef __MACHINEEXCEPTIONS__
#define __MACHINEEXCEPTIONS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif


#if TARGET_CPU_X86 || TARGET_CPU_X86_64
#include <emmintrin.h>
#endif


#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma options align=power

/* Some basic declarations used throughout the kernel */
typedef struct OpaqueAreaID*            AreaID;
/* Machine Dependent types for PowerPC: */

/* Because a number of sources do a #define CR 13 and this file contains a struct member named CR,
 * an obscure compilation error gets spit out.  Rename the field to CRRegister.  To build old code
 * which has the name CR in it, either update the code to use CRRegister or do a
 * #define __MACHINEEXCEPTIONS_USE_OLD_CR_FIELD_NAME__ 1
 * before #including <CoreServices/CoreServices.h> */
#if __MACHINEEXCEPTIONS_USE_OLD_CR_FIELD_NAME__
#define CRRegister   CR
#endif

struct MachineInformationPowerPC {
  UnsignedWide        CTR;
  UnsignedWide        LR;
  UnsignedWide        PC;
  unsigned long       CRRegister;             /*    changed from CR since some folks had a #define CR  13 in their source code*/
  unsigned long       XER;
  unsigned long       MSR;
  unsigned long       MQ;
  unsigned long       ExceptKind;
  unsigned long       DSISR;
  UnsignedWide        DAR;
  UnsignedWide        Reserved;
};
typedef struct MachineInformationPowerPC MachineInformationPowerPC;
struct RegisterInformationPowerPC {
  UnsignedWide        R0;
  UnsignedWide        R1;
  UnsignedWide        R2;
  UnsignedWide        R3;
  UnsignedWide        R4;
  UnsignedWide        R5;
  UnsignedWide        R6;
  UnsignedWide        R7;
  UnsignedWide        R8;
  UnsignedWide        R9;
  UnsignedWide        R10;
  UnsignedWide        R11;
  UnsignedWide        R12;
  UnsignedWide        R13;
  UnsignedWide        R14;
  UnsignedWide        R15;
  UnsignedWide        R16;
  UnsignedWide        R17;
  UnsignedWide        R18;
  UnsignedWide        R19;
  UnsignedWide        R20;
  UnsignedWide        R21;
  UnsignedWide        R22;
  UnsignedWide        R23;
  UnsignedWide        R24;
  UnsignedWide        R25;
  UnsignedWide        R26;
  UnsignedWide        R27;
  UnsignedWide        R28;
  UnsignedWide        R29;
  UnsignedWide        R30;
  UnsignedWide        R31;
};
typedef struct RegisterInformationPowerPC RegisterInformationPowerPC;
struct FPUInformationPowerPC {
  UnsignedWide        Registers[32];
  unsigned long       FPSCR;
  unsigned long       Reserved;
};
typedef struct FPUInformationPowerPC    FPUInformationPowerPC;
union Vector128 {
#ifdef __APPLE_ALTIVEC__
   vector unsigned int         v;
#endif
  unsigned long       l[4];
  unsigned short      s[8];
  unsigned char       c[16];
};
typedef union Vector128                 Vector128;
struct VectorInformationPowerPC {
  Vector128           Registers[32];
  Vector128           VSCR;
  UInt32              VRsave;
};
typedef struct VectorInformationPowerPC VectorInformationPowerPC;
/* Exception related declarations */
enum {
  kWriteReference               = 0,
  kReadReference                = 1,
  kFetchReference               = 2,
  writeReference                = kWriteReference, /* Obsolete name*/
  readReference                 = kReadReference, /* Obsolete name*/
  fetchReference                = kFetchReference /* Obsolete name*/
};


typedef unsigned long                   MemoryReferenceKind;
struct MemoryExceptionInformation {
  AreaID              theArea;                /* The area related to the execption, same as MPAreaID.*/
  LogicalAddress      theAddress;             /* The 32-bit address of the exception.*/
  OSStatus            theError;               /* See enum below.*/
  MemoryReferenceKind  theReference;          /* read, write, instruction fetch.*/
};
typedef struct MemoryExceptionInformation MemoryExceptionInformation;
enum {
  kUnknownException             = 0,
  kIllegalInstructionException  = 1,
  kTrapException                = 2,
  kAccessException              = 3,
  kUnmappedMemoryException      = 4,
  kExcludedMemoryException      = 5,
  kReadOnlyMemoryException      = 6,
  kUnresolvablePageFaultException = 7,
  kPrivilegeViolationException  = 8,
  kTraceException               = 9,
  kInstructionBreakpointException = 10, /* Optional*/
  kDataBreakpointException      = 11,   /* Optional*/
  kIntegerException             = 12,
  kFloatingPointException       = 13,
  kStackOverflowException       = 14,   /* Optional, may be implemented as kAccessException on some systems.*/
  kTaskTerminationException     = 15,   /* Obsolete*/
  kTaskCreationException        = 16,   /* Obsolete*/
  kDataAlignmentException       = 17    /* May occur when a task is in little endian mode or created with kMPTaskTakesAllExceptions.*/
};

#if OLDROUTINENAMES
enum {
  unknownException              = kUnknownException, /* Obsolete name*/
  illegalInstructionException   = kIllegalInstructionException, /* Obsolete name*/
  trapException                 = kTrapException, /* Obsolete name*/
  accessException               = kAccessException, /* Obsolete name*/
  unmappedMemoryException       = kUnmappedMemoryException, /* Obsolete name*/
  excludedMemoryException       = kExcludedMemoryException, /* Obsolete name*/
  readOnlyMemoryException       = kReadOnlyMemoryException, /* Obsolete name*/
  unresolvablePageFaultException = kUnresolvablePageFaultException, /* Obsolete name*/
  privilegeViolationException   = kPrivilegeViolationException, /* Obsolete name*/
  traceException                = kTraceException, /* Obsolete name*/
  instructionBreakpointException = kInstructionBreakpointException, /* Obsolete name*/
  dataBreakpointException       = kDataBreakpointException, /* Obsolete name*/
  integerException              = kIntegerException, /* Obsolete name*/
  floatingPointException        = kFloatingPointException, /* Obsolete name*/
  stackOverflowException        = kStackOverflowException, /* Obsolete name*/
  terminationException          = kTaskTerminationException, /* Obsolete name*/
  kTerminationException         = kTaskTerminationException /* Obsolete name*/
};

#endif  /* OLDROUTINENAMES */


typedef unsigned long                   ExceptionKind;
union ExceptionInfo {
  MemoryExceptionInformation * memoryInfo;
};
typedef union ExceptionInfo             ExceptionInfo;
struct ExceptionInformationPowerPC {
  ExceptionKind       theKind;
  MachineInformationPowerPC * machineState;
  RegisterInformationPowerPC * registerImage;
  FPUInformationPowerPC * FPUImage;
  ExceptionInfo       info;
  VectorInformationPowerPC * vectorImage;
};
typedef struct ExceptionInformationPowerPC ExceptionInformationPowerPC;
#if TARGET_CPU_PPC || TARGET_CPU_PPC64
typedef ExceptionInformationPowerPC     ExceptionInformation;
typedef MachineInformationPowerPC       MachineInformation;
typedef RegisterInformationPowerPC      RegisterInformation;
typedef FPUInformationPowerPC           FPUInformation;
typedef VectorInformationPowerPC        VectorInformation;
#endif  /* TARGET_CPU_PPC || TARGET_CPU_PPC64 */

#if TARGET_CPU_X86 || TARGET_CPU_X86_64
union Vector128Intel {
  __m128              s;
  __m128i             si;
  __m128d             sd;
  unsigned char       c[16];
};
typedef union Vector128Intel            Vector128Intel;
#endif  /* TARGET_CPU_X86 || TARGET_CPU_X86_64 */

#if TARGET_CPU_X86
struct MachineInformationIntel {
  unsigned long       CS;
  unsigned long       DS;
  unsigned long       SS;
  unsigned long       ES;
  unsigned long       FS;
  unsigned long       GS;
  unsigned long       EFLAGS;
  unsigned long       EIP;
  unsigned long       ExceptTrap;
  unsigned long       ExceptErr;
  unsigned long       ExceptAddr;
};
typedef struct MachineInformationIntel  MachineInformationIntel;
struct RegisterInformationIntel {
  unsigned long       EAX;
  unsigned long       EBX;
  unsigned long       ECX;
  unsigned long       EDX;
  unsigned long       ESI;
  unsigned long       EDI;
  unsigned long       EBP;
  unsigned long       ESP;
};
typedef struct RegisterInformationIntel RegisterInformationIntel;

typedef unsigned char                   FPRegIntel[10];

   /* Because Xlib.h does a #define Status int, this struct fails to compile if Xlib.h is #include before CarbonCore.h
    So, hack around it for them. */

struct FPUInformationIntel {
FPRegIntel          Registers[8];
unsigned short      Control;
#ifdef Status
unsigned short      StatusReg;
#else
unsigned short      Status;
#endif
unsigned short      Tag;
unsigned short      Opcode;
unsigned int        EIP;
unsigned int        DP;
unsigned int        DS;
};
typedef struct FPUInformationIntel      FPUInformationIntel;

struct VectorInformationIntel {
  Vector128Intel      Registers[8];
};
typedef struct VectorInformationIntel   VectorInformationIntel;

typedef MachineInformationIntel         MachineInformation;
typedef RegisterInformationIntel        RegisterInformation;
typedef FPUInformationIntel             FPUInformation;
typedef VectorInformationIntel          VectorInformation;
#endif  /* TARGET_CPU_X86 */

#if TARGET_CPU_X86_64
struct MachineInformationIntel64 {
  unsigned long       CS;
  unsigned long       FS;
  unsigned long       GS;
  unsigned long       RFLAGS;
  unsigned long       RIP;
  unsigned long       ExceptTrap;
  unsigned long       ExceptErr;
  unsigned long       ExceptAddr;
};
typedef struct MachineInformationIntel64 MachineInformationIntel64;
struct RegisterInformationIntel64 {
  unsigned long       RAX;
  unsigned long       RBX;
  unsigned long       RCX;
  unsigned long       RDX;
  unsigned long       RDI;
  unsigned long       RSI;
  unsigned long       RBP;
  unsigned long       RSP;
  unsigned long       R8;
  unsigned long       R9;
  unsigned long       R10;
  unsigned long       R11;
  unsigned long       R12;
  unsigned long       R13;
  unsigned long       R14;
  unsigned long       R15;
};
typedef struct RegisterInformationIntel64 RegisterInformationIntel64;

typedef unsigned char                   FPRegIntel[10];
struct FPUInformationIntel64 {
  FPRegIntel          Registers[8];
  unsigned short      Control;
  unsigned short      Status;
  unsigned short      Tag;
  unsigned short      Opcode;
  unsigned int        IP;
  unsigned int        DP;
  unsigned int        DS;
};
typedef struct FPUInformationIntel64    FPUInformationIntel64;
struct VectorInformationIntel64 {
  Vector128Intel      Registers[16];
};
typedef struct VectorInformationIntel64 VectorInformationIntel64;

typedef MachineInformationIntel64       MachineInformation;
typedef RegisterInformationIntel64      RegisterInformation;
typedef FPUInformationIntel64           FPUInformation;
typedef VectorInformationIntel64        VectorInformation;
#endif  /* TARGET_CPU_X86_64 */

#if TARGET_CPU_X86 || TARGET_CPU_X86_64
struct ExceptionInformation {
  ExceptionKind       theKind;
  MachineInformation * machineState;
  RegisterInformation * registerImage;
  FPUInformation *    FPUImage;
  ExceptionInfo       info;
  VectorInformation * vectorImage;
};
typedef struct ExceptionInformation     ExceptionInformation;
#endif  /* TARGET_CPU_X86 || TARGET_CPU_X86_64 */

#if TARGET_CPU_ARM64
typedef struct {
	const void*		__unusedMachineInformationField;
} MachineInformation;
typedef struct {
	const void*		__unusedRegisterInformationField;
} RegisterInformation;
typedef struct {
	const void*		__unusedFPUInformationField;
} FPUInformation;
typedef struct {
	const void*		__unusedVectorInformationField;
} VectorInformation;

struct ExceptionInformation {
  ExceptionKind       theKind;
  /* XXX: Not implemented */
  MachineInformation * machineState;
  RegisterInformation * registerImage;
  FPUInformation *    FPUImage;
  ExceptionInfo       info;
  VectorInformation * vectorImage;
};
typedef struct ExceptionInformation     ExceptionInformation;
#endif  /* TARGET_CPU_ARM64 */

/* 
    Note:   An ExceptionHandler is NOT a UniversalProcPtr, except in Carbon.
            It must be a PowerPC function pointer with NO routine descriptor, 
            except on Carbon, where it must be a UniversalProcPtr (TPP actually)
            to allow the interface to work from both CFM and Mach-O.
*/
typedef CALLBACK_API_C( OSStatus , ExceptionHandlerProcPtr )(ExceptionInformation * theException);
typedef STACK_UPP_TYPE(ExceptionHandlerProcPtr)                 ExceptionHandlerUPP;
/*
 *  NewExceptionHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ExceptionHandlerUPP
NewExceptionHandlerUPP(ExceptionHandlerProcPtr userRoutine) __API_DEPRECATED("No longer supported", macos(10.0,10.8)) __API_UNAVAILABLE(ios,watchos,tvos);

/*
 *  DisposeExceptionHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeExceptionHandlerUPP(ExceptionHandlerUPP userUPP) __API_DEPRECATED("No longer supported", macos(10.0,10.8)) __API_UNAVAILABLE(ios,watchos,tvos);

/*
 *  InvokeExceptionHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSStatus
InvokeExceptionHandlerUPP(
						  ExceptionInformation *  theException,
						  ExceptionHandlerUPP     userUPP) __API_DEPRECATED("No longer supported", macos(10.0,10.8)) __API_UNAVAILABLE(ios,watchos,tvos);

#if __MACH__
  #ifdef __cplusplus
    inline ExceptionHandlerUPP                                  NewExceptionHandlerUPP(ExceptionHandlerProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeExceptionHandlerUPP(ExceptionHandlerUPP) { }
    inline OSStatus                                             InvokeExceptionHandlerUPP(ExceptionInformation * theException, ExceptionHandlerUPP userUPP) { return (*userUPP)(theException); }
  #else
    #define NewExceptionHandlerUPP(userRoutine)                 ((ExceptionHandlerUPP)userRoutine)
    #define DisposeExceptionHandlerUPP(userUPP)
    #define InvokeExceptionHandlerUPP(theException, userUPP)    (*userUPP)(theException)
  #endif
#endif

/*
   ExceptionHandler function pointers (TPP):
   on classic PowerPC, use raw function pointers
   on classic PowerPC with OPAQUE_UPP_TYPES=1, use UPP's
   on Carbon, use UPP's
*/
/* use UPP's*/

typedef ExceptionHandlerUPP             ExceptionHandlerTPP;
typedef ExceptionHandlerTPP             ExceptionHandler;
/* Routine for installing per-process exception handlers */
/*!
 *  InstallExceptionHandler()
 *
 *  As of macOS 10.16, this call always returns kMPInvalidIDErr and otherwise does nothing.  It has been deprecated since
 *  macOS 10.8 and non-functional since macOS10.13.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ExceptionHandlerTPP  InstallExceptionHandler(ExceptionHandlerTPP theHandler) __API_DEPRECATED("No longer support", macos(10.0,10.8)) __API_UNAVAILABLE(ios,watchos,tvos);




#pragma options align=reset

#ifdef __cplusplus
}
#endif

#endif /* __MACHINEEXCEPTIONS__ */

