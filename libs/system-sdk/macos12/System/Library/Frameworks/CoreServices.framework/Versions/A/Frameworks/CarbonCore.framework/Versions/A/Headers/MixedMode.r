/*
     File:       MixedMode.r
 
     Contains:   Mixed Mode Manager Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1992-2011 by Apple Inc. All rights reserved.
*/

#ifndef __MIXEDMODE_R__
#define __MIXEDMODE_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define kRoutineDescriptorVersion 		7
#define _MixedModeMagic 				0xAAFE
#define kCurrentMixedModeStateRecord 	1
#define kPascalStackBased 				0
#define kCStackBased 					1
#define kRegisterBased 					2
#define kD0DispatchedPascalStackBased 	8
#define kD1DispatchedPascalStackBased 	12
#define kD0DispatchedCStackBased 		9
#define kStackDispatchedPascalStackBased  14
#define kThinkCStackBased 				5

#define kM68kISA 						0
#define kPowerPCISA 					1

#define kX86ISA 						2
#define kOld68kRTA 						0x00
#define kPowerPCRTA 					0x00
#define kCFM68kRTA 						0x10

#define kX86RTA 						0x20
#define kRegisterD0 					0
#define kRegisterD1 					1
#define kRegisterD2 					2
#define kRegisterD3 					3
#define kRegisterD4 					8
#define kRegisterD5 					9
#define kRegisterD6 					10
#define kRegisterD7 					11
#define kRegisterA0 					4
#define kRegisterA1 					5
#define kRegisterA2 					6
#define kRegisterA3 					7
#define kRegisterA4 					12
#define kRegisterA5 					13
#define kRegisterA6 					14					/*  A7 is the same as the PowerPC SP  */
#define kCCRegisterCBit 				16
#define kCCRegisterVBit 				17
#define kCCRegisterZBit 				18
#define kCCRegisterNBit 				19
#define kCCRegisterXBit 				20

#define kNoByteCode 					0
#define kOneByteCode 					1
#define kTwoByteCode 					2
#define kFourByteCode 					3

#define kProcDescriptorIsAbsolute 		0x00
#define kProcDescriptorIsRelative 		0x01

#define kFragmentIsPrepared 			0x00
#define kFragmentNeedsPreparing 		0x02

#define kUseCurrentISA 					0x00
#define kUseNativeISA 					0x04

#define kPassSelector 					0x00
#define kDontPassSelector 				0x08

#define kRoutineIsNotDispatchedDefaultRoutine  0x00
#define kRoutineIsDispatchedDefaultRoutine  0x10

#define kProcDescriptorIsProcPtr 		0x00
#define kProcDescriptorIsIndex 			0x20

#define kSelectorsAreNotIndexable 		0x00
#define kSelectorsAreIndexable 			0x01

															/*  Calling Convention Offsets  */
#define kCallingConventionWidth 		4
#define kCallingConventionPhase 		0
#define kCallingConventionMask 			0x0F				/*  Result Offsets  */
#define kResultSizeWidth 				2
#define kResultSizePhase 				4
#define kResultSizeMask 				0x30				/*  Parameter offsets & widths  */
#define kStackParameterWidth 			2
#define kStackParameterPhase 			6
#define kStackParameterMask 			0xFFFFFFC0			/*  Register Result Location offsets & widths  */
#define kRegisterResultLocationWidth 	5
#define kRegisterResultLocationPhase 	6					/*  Register Parameter offsets & widths  */
#define kRegisterParameterWidth 		5
#define kRegisterParameterPhase 		11
#define kRegisterParameterMask 			0x7FFFF800
#define kRegisterParameterSizePhase 	0
#define kRegisterParameterSizeWidth 	2
#define kRegisterParameterWhichPhase 	2
#define kRegisterParameterWhichWidth 	3					/*  Dispatched Stack Routine Selector offsets & widths  */
#define kDispatchedSelectorSizeWidth 	2
#define kDispatchedSelectorSizePhase 	6					/*  Dispatched Stack Routine Parameter offsets  */
#define kDispatchedParameterPhase 		8					/*  Special Case offsets & widths  */
#define kSpecialCaseSelectorWidth 		6
#define kSpecialCaseSelectorPhase 		4
#define kSpecialCaseSelectorMask 		0x03F0

#define kSpecialCase 					0x000F				/*  (CallingConventionType)  */
															/*  all of the special cases enumerated.  The selector field is 6 bits wide  */
#define kSpecialCaseHighHook 			0
#define kSpecialCaseCaretHook 			0					/*  same as kSpecialCaseHighHook  */
#define kSpecialCaseEOLHook 			1
#define kSpecialCaseWidthHook 			2
#define kSpecialCaseTextWidthHook 		2					/*  same as kSpecialCaseWidthHook  */
#define kSpecialCaseNWidthHook 			3
#define kSpecialCaseDrawHook 			4
#define kSpecialCaseHitTestHook 		5
#define kSpecialCaseTEFindWord 			6
#define kSpecialCaseProtocolHandler 	7
#define kSpecialCaseSocketListener 		8
#define kSpecialCaseTERecalc 			9
#define kSpecialCaseTEDoText 			10
#define kSpecialCaseGNEFilterProc 		11
#define kSpecialCaseMBarHook 			12

#define GoMixedModeTrapType unsigned hex integer
#define VersionType byte
#define SelectorsAreIndexableType boolean
#define Reserved1Type fill long
#define Reserved2Type fill byte
#define SelectorInfoType hex byte
#define RoutineCountType integer
#define ProcInfoType binary longint
#define Reserved3Type fill byte
#define ISAType byte
#define ProcDescriptorIsRelativeType boolean
#define FragmentNeedsPreparingType boolean
#define UseNativeISAType boolean
#define DontPassSelectorType boolean
#define RoutineIsDispatchedDefaultType boolean
#define ProcDescriptorType longint
#define Reserved4Type fill long
#define SelectorType longint
/*
    Use the 'rdes' template to define a “native resource” which 
    starts with a routine descriptor. Such resources contain 
    just PowerPC code. 
    
    Note that such resources can only be executed on PowerPC
    machines. Executing them on a 68K machine will result 
    in a crash.

    To create a “native resource”, you should first build the
    executable code just as you normally do, placing the code
    in a PEF container in the data fork of a file.
    
    You would then use something like the following:
    
#include "MixedMode.r"

type 'BDef' as 'rdes';

resource 'BDef' (1) {
    $1,                                     // ProcInfo
    $$Read("MyCode.pef")                    // Specify name of PEF file
};

*/

type 'rdes' { 
Top:
    /* Routine Descriptor */
    GoMixedModeTrapType             = _MixedModeMagic;
    VersionType                     = kRoutineDescriptorVersion;
    fill bit [7];
    SelectorsAreIndexableType       = FALSE;
    Reserved1Type;
    Reserved2Type;
    SelectorInfoType                = 0;
    RoutineCountType                = 0;

    /* Routine Record */
    ProcInfoType;
    Reserved3Type;
    ISAType                         = kPowerPCISA;
    fill bit [11];
    RoutineIsDispatchedDefaultType  = FALSE;
    DontPassSelectorType            = FALSE;
    UseNativeISAType                = TRUE;
    FragmentNeedsPreparingType      = TRUE;
    ProcDescriptorIsRelativeType    = TRUE;
    ProcDescriptorType              = (BeginningOfPowerPCCode-Top) / 8;
    Reserved4Type;
    SelectorType                    = 0;
    Align LONG;

BeginningOfPowerPCCode:
    hex string;                     // The PEF container starts here
};


/*
    Use the 'fdes' template to define a “fat resource” which 
    starts with a routine descriptor and contains both 68K and 
    PowerPC code. 
    
    Note that such resources can only be executed on a machine
    with MixedMode installed. To create “safe fat resources”
    which will run on all machines, use the 'sdes' template
    defined below.

    To create a “fat resource”, you should first build the
    executable code just as you normally do, placing the PowerPC
    code in a PEF container in the data fork of a file. The 68k
    executable would be built as a stand-alone code resource, as
    has historically been done.
    
    You would then use something like the following:
    
#include "MixedMode.r"

type 'BDef' as 'fdes';

resource 'BDef' (1) {
    $1,                                         // 68K ProcInfo
    $1,                                         // PowerPC ProcInfo
    $$Resource("My68KCode.rsrc", 'oCod', 128),  // Specify file name, type and ID of resource
                                                //   containing 68k code
    $$Read("MyPPCCode.pef")                     // Specify file containing PPC code in PEF format
};

*/

/*  Fat Routines  */
type 'fdes' { 
Top:
    /* Routine Descriptor */
    GoMixedModeTrapType             = _MixedModeMagic;
    VersionType                     = kRoutineDescriptorVersion;
    fill bit [7];
    SelectorsAreIndexableType       = FALSE;
    Reserved1Type;
    Reserved2Type;
    SelectorInfoType                = 0;
    RoutineCountType                = 1;

    /* 68k Routine Record */
    ProcInfoType;
    Reserved3Type;
    ISAType                         = kM68kISA;
    fill bit [11];
    RoutineIsDispatchedDefaultType  = FALSE;
    DontPassSelectorType            = FALSE;
    UseNativeISAType                = TRUE;
    FragmentNeedsPreparingType      = FALSE;
    ProcDescriptorIsRelativeType    = TRUE;
    ProcDescriptorType              = (BeginningOf68KCode-Top) / 8;
    Reserved4Type;
    SelectorType                    = 0;

    /* PowerPC Routine Record 1 */
    ProcInfoType;
    Reserved3Type;
    ISAType                         = kPowerPCISA;
    fill bit [11];
    RoutineIsDispatchedDefaultType  = FALSE;
    DontPassSelectorType            = FALSE;
    UseNativeISAType                = TRUE;
    FragmentNeedsPreparingType      = TRUE;
    ProcDescriptorIsRelativeType    = TRUE;
    ProcDescriptorType              = (BeginningOfPowerPCCode-Top) / 8;
    Reserved4Type;
    SelectorType                    = 0;
    Align LONG;

BeginningOf68kCode:
    hex string;             // The code starts here
    
    Align LONG;

BeginningOfPowerPCCode:
    hex string;             // The PEF container starts here
};


/*
    Use the 'sdes' template to define a “safe fat resource” which 
    contains both 68K and PowerPC code. A safe fat resource starts
    with 68K code which is executed the first time the resource
    is called. This code determines if MixedMode is present. If
    so, a routine descriptor is moved to the beginning of the
    resource. If not, a branch instruction to the 68K portion
    of the code is placed at the beginning of the resource. 
    Therefore, the first time the resource is executed, there 
    is some overhead incurred. However, subsequent calls 
    will be fast.
    
    Note: This template cannot currently be used for resources
    containing code with register-based calling conventions
    because the 68K code at the beginning of the resource
    uses D0, A0, and A1.
    
    To create a “safe fat resource”, you should first build the
    executable code just as you normally do, placing the PowerPC
    code in a PEF container in the data fork of a file. The 68k
    executable would be built as a stand-alone code resource, as
    has historically been done.
    
    You would then use something like the following:
    
#include "MixedMode.r"

type 'BDef' as 'sdes';

resource 'BDef' (1) {
    $1,                                         // 68K ProcInfo
    $1,                                         // PowerPC ProcInfo
    $$Resource("My68KCode.rsrc", 'oCod', 128),  // Specify file name, type and ID of resource
                                                //   containing 68k code
    $$Read("MyPPCCode.pef")                     // Specify file containing PPC code in PEF format
};

*/

/*  Safe Fat Resources  */
type 'sdes' { 
Top:
    hex string  = 
        $"4E56 FFF0"      // SafeFatRsrc    LINK        A6, #-sysEnv1Size       ; Allocate a sysEnvRec
        $"41EE FFF0"      //                LEA         -sysEnv1Size(A6), A0
        $"7001"           //                MOVEQ       #1, D0                  ; On 6.X, Gestalt not be implemented...
        $"A090"           //                _SysEnvirons    
        $"4A40"           //                TST.W       D0
        $"6640"           //                BNE.S       Install68K_60           ; if call fails, load up the 68K without FlushCache
        $"0C68 0700 0004" //                CMPI        #$0700,systemVersion(A0)
        $"6D38"           //                BLT.S       Install68K_60           ; if pre- 7.0, assume no cache      
        $"303C A89F"      //                MOVE.W      #$A89F, D0              ; We have larger trap tables.  Is MixedMode installed?
        $"A746"           //                _GetToolBoxTrapAddress              ; Leave _Unimplemented on the top of the stack...
        $"2F08"           //                MOVE.L      A0, -(SP)               ; Unlk will clean this up
        $"303C AAFE"      //                MOVE.W      #$AAFE, D0 
        $"A746"           //                _GetToolBoxTrapAddress
        $"B1D7"           //                CMPA.L      (SP), A0                
        $"663E"           //                BNE.S       InstallPPCCode
        $"41FA FFD4"      // Install68K_70  LEA         SafeFatRsrc, A0
        $"30FC 6000"      //                MOVE.W      #$6000, (A0)+           ; Generate a BRA instruction
        $"43FA 0044"      //                LEA         FatRD, A1
        $"2029 0014"      //                MOVE.L      20(A1), D0              ; Get 68K code offset
        $"5580"           //                SUBQ.L      #2, D0
        $"3080"           //                MOVE.W      D0, (A0)                ; Fill in the second word of the BRA
        $"303C A198"      //                MOVE.W      #$A198, D0              ; Is _HWPriv implemented?
        $"A346"           //                _GetOSTrapAddress
        $"B1D7"           //                CMPA.L      (SP), A0
        $"4E5E"           //                UNLK        A6
        $"67B6"           //                BEQ.S       SafeFatRsrc
        $"7001 A198"      //                _FlushInstructionCache
        $"60B0"           //                BRA.S       SafeFatRsrc
        $"4E5E"           // Install68K_60  UNLK        A6                      ; Old machine, FlushCache not supported
        $"41FA FFAC"      //                LEA         SafeFatRsrc, A0                     
        $"30FC 6000"      //                MOVE.W      #$6000, (A0)+           ; Generate a BRA instruction
        $"43FA 001C"      //                LEA         FatRD, A1
        $"2029 0014"      //                MOVE.L      20(A1), D0              ; Get 68K code offset
        $"5580"           //                SUBQ.L      #2, D0
        $"3080"           //                MOVE.W      D0, (A0)                ; Fill in the second word of the BRA
        $"6098"           //                BRA.S       SafeFatRsrc
        $"4E5E"           // InstallPPCCode UNLK        A6
        $"43FA FF94"      //                LEA         SafeFatRsrc, A1
        $"41FA 0008"      //                LEA         FatRD, A0
        $"7034"           //                MOVE.L      #52, D0
        $"A02E"           //                _BlockMove                          ; Move R.D. to top of rsrc
        $"6088";          //                BRA.S       SafeFatRsrc
                          // FatRD  

    /* Routine Descriptor */
    GoMixedModeTrapType             = _MixedModeMagic;
    VersionType                     = kRoutineDescriptorVersion;
    fill bit [7];
    SelectorsAreIndexableType       = FALSE;
    Reserved1Type;
    Reserved2Type;
    SelectorInfoType                = 0;
    RoutineCountType                = 1;

    /* Routine Record */
    ProcInfoType;
    Reserved3Type;
    ISAType                         = kM68kISA;
    fill bit [11];
    RoutineIsDispatchedDefaultType  = FALSE;
    DontPassSelectorType            = FALSE;
    UseNativeISAType                = TRUE;
    FragmentNeedsPreparingType      = FALSE;
    ProcDescriptorIsRelativeType    = TRUE;
    ProcDescriptorType              = (BeginningOf68KCode-Top) / 8;
    Reserved4Type;
    SelectorType                    = 0;

    /* PowerPC Routine Record 1 */
    ProcInfoType;
    Reserved3Type;
    ISAType = kPowerPCISA;
    fill bit [11];
    RoutineIsDispatchedDefaultType  = FALSE;
    DontPassSelectorType            = FALSE;
    UseNativeISAType                = TRUE;
    FragmentNeedsPreparingType      = TRUE;
    ProcDescriptorIsRelativeType    = TRUE;
    ProcDescriptorType              = (BeginningOfPowerPCCode-Top) / 8;
    Reserved4Type;
    SelectorType                    = 0;
    Align LONG;

BeginningOf68KCode:
    hex string;     // The 68k code starts here

    Align LONG;

BeginningOfPowerPCCode:
    hex string;     // The PEF container starts here
};

#endif /* __MIXEDMODE_R__ */

