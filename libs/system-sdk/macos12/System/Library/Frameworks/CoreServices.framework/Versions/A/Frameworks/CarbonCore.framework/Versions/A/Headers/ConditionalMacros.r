/*
     File:       ConditionalMacros.r
 
     Contains:   Set up for compiler independent conditionals
                 The contents of this header file are deprecated.
 
     Copyright:  © 1993-2011 by Apple Inc. All rights reserved.
*/

#ifndef __CONDITIONALMACROS_R__
#define __CONDITIONALMACROS_R__

/****************************************************************************************************
    UNIVERSAL_INTERFACES_VERSION
    
        0x0400 --> version 4.0 (Mac OS X only)
        0x0335 --> version 3.4 
        0x0331 --> version 3.3.1
        0x0330 --> version 3.3
        0x0320 --> version 3.2
        0x0310 --> version 3.1
        0x0301 --> version 3.0.1
        0x0300 --> version 3.0
        0x0210 --> version 2.1
        This conditional did not exist prior to version 2.1
****************************************************************************************************/
#define UNIVERSAL_INTERFACES_VERSION 0x0400
/****************************************************************************************************

    TARGET_CPU_Å    
    These conditionals specify which microprocessor instruction set is being
    generated.  At most one of these is true, the rest are false.

        TARGET_CPU_PPC          - Compiler is generating PowerPC instructions
        TARGET_CPU_68K          - Compiler is generating 680x0 instructions
        TARGET_CPU_X86          - Compiler is generating x86 instructions
        TARGET_CPU_MIPS         - Compiler is generating MIPS instructions
        TARGET_CPU_SPARC        - Compiler is generating Sparc instructions
        TARGET_CPU_ALPHA        - Compiler is generating Dec Alpha instructions


    TARGET_OS_Å 
    These conditionals specify in which Operating System the generated code will
    run. At most one of the these is true, the rest are false.

        TARGET_OS_MAC           - Generate code will run under Mac OS
        TARGET_OS_WIN32         - Generate code will run under 32-bit Windows
        TARGET_OS_UNIX          - Generate code will run under some unix 


    TARGET_RT_Å 
    Rez is a data description language, and it doesn't make sense to try to say which
    architecture will eventually look at the data.  Therefore, the TARGET_RT_ macros will
    not be defined for Rez builds.
    
    TARGET__API_Å_Å 
    These conditionals are used to differentiate between sets of API's on the same
    processor under the same OS.  The first section after _API_ is the OS.  The
    second section is the API set.  Unlike TARGET_OS_ and TARGET_CPU_, these
    conditionals are not mutally exclusive. This file will attempt to auto-configure
    all TARGET_API_Å_Å values, but will often need a TARGET_API_Å_Å value predefined
    in order to disambiguate.
    
        TARGET_API_MAC_OS8      - Code is being compiled to run on System 7 through Mac OS 8.x
        TARGET_API_MAC_CARBON   - Code is being compiled to run on Mac OS 8 and Mac OS X via CarbonLib
        TARGET_API_MAC_OSX      - Code is being compiled to run on Mac OS X

****************************************************************************************************/

#define PRAGMA_ONCE    0

#if defined(Environ_OS_Win32) || defined(Environ_OS_Unix) || defined(Environ_OS_Mac) || defined(environ_os_mac)

    #if Environ_OS_Win32
       /*
         Rez.exe resource compiler for Win32 from QuickTime 3.0, Apple Inc.    
       */
     #define TARGET_OS_MAC               0
      #define TARGET_OS_WIN32             1
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     1
      #define TARGET_RT_BIG_ENDIAN        0

     #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0
      #define TARGET_CPU_X86              1
      #define TARGET_CPU_PPC              0

    
   #elif Environ_OS_Unix
      /*
         Rez resource compiler for unix from QuickTime 3.0, Apple Inc. 
       */
     #define TARGET_OS_MAC               1
      #define TARGET_OS_WIN32             0
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     0
      #define TARGET_RT_BIG_ENDIAN        1

     #define TARGET_CPU_MIPS             Environ_CPU_MIPS
       #define TARGET_CPU_SPARC            Environ_CPU_SPARC
      #define TARGET_CPU_X86              Environ_CPU_X86
        #define TARGET_CPU_PPC              Environ_CPU_PPC

    
   #else
      /*
         Rez resource compiler for MacOS from QuickTime 3.0, Apple Inc.    
       */
     #define TARGET_OS_MAC               1
      #define TARGET_OS_WIN32             0
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     0
      #define TARGET_RT_BIG_ENDIAN        1

     #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_PPC              1
  #endif

    
#else
 /*
     Rez resource compiler for MacOS, Apple Inc.   
   */
 #define TARGET_OS_MAC               1
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
#endif

    
#if !defined(TARGET_REZ_MAC_68K)
    #define TARGET_REZ_MAC_68K      0
#endif
#if !defined(TARGET_REZ_MAC_PPC)
    #define TARGET_REZ_MAC_PPC      0
#endif
#if !defined(TARGET_REZ_CARBON_CFM)
   #define TARGET_REZ_CARBON_CFM   0
#endif
#if !defined(TARGET_REZ_CARBON_MACHO)
 #define TARGET_REZ_CARBON_MACHO 0
#endif

/****************************************************************************************************
    
    Set up TARGET_API_Å_Å values

****************************************************************************************************/
/* Rez compiler for OS X */
#if !defined(TARGET_API_MAC_OS8) && !defined(TARGET_API_MAC_OSX) && !defined(TARGET_API_MAC_CARBON)
#ifdef environ_os_mac
#define TARGET_API_MAC_OS8 0
#define TARGET_API_MAC_CARBON 1
#define TARGET_API_MAC_OSX 1
#else
/* Rez compiler for OS 8/9 */
#define TARGET_API_MAC_OS8 1
#define TARGET_API_MAC_CARBON 0
#define TARGET_API_MAC_OSX 0
#endif  /* defined(environ_os_mac) */

#else
#ifndef TARGET_API_MAC_OS8
#define TARGET_API_MAC_OS8 0
#endif  /* !defined(TARGET_API_MAC_OS8) */

#ifndef TARGET_API_MAC_CARBON
#define TARGET_API_MAC_CARBON 0
#endif  /* !defined(TARGET_API_MAC_CARBON) */

#ifndef TARGET_API_MAC_OSX
#define TARGET_API_MAC_OSX 0
#endif  /* !defined(TARGET_API_MAC_OSX) */

#endif  /* !defined(TARGET_API_MAC_OS8) && !defined(TARGET_API_MAC_OSX) && !defined(TARGET_API_MAC_CARBON) */



/****************************************************************************************************

    OLDROUTINENAMES         - "Old" names for Macintosh system calls are allowed in source code.
                              (e.g. DisposPtr instead of DisposePtr). The names of system routine
                              are now more sensitive to change because CFM binds by name.  In the 
                              past, system routine names were compiled out to just an A-Trap.  
                              Macros have been added that each map an old name to its new name.  
                              This allows old routine names to be used in existing source files,
                              but the macros only work if OLDROUTINENAMES is true.  This support
                              will be removed in the near future.  Thus, all source code should 
                              be changed to use the new names! You can set OLDROUTINENAMES to false
                              to see if your code has any old names left in it.
    
****************************************************************************************************/
#ifndef OLDROUTINENAMES
#define OLDROUTINENAMES 0
#endif  /* !defined(OLDROUTINENAMES) */



/****************************************************************************************************

    TARGET_CARBON                   - default: false. Switches all of the above as described.  Overrides all others
                                    - NOTE: If you set TARGET_CARBON to 1, then the other switches will be setup by
                                            ConditionalMacros, and should not be set manually.

    If you wish to do development for pre-Carbon Systems, you can set the following:

    OPAQUE_TOOLBOX_STRUCTS          - default: false. True for Carbon builds, hides struct fields.
    OPAQUE_UPP_TYPES                - default: false. True for Carbon builds, UPP types are unique and opaque.
    ACCESSOR_CALLS_ARE_FUNCTIONS    - default: false. True for Carbon builds, enables accessor functions.
    CALL_NOT_IN_CARBON              - default: true.  False for Carbon builds, hides calls not supported in Carbon.
    
    Specifically, if you are building a non-Carbon application (one that links against InterfaceLib)
    but you wish to use some of the accessor functions, you can set ACCESSOR_CALLS_ARE_FUNCTIONS to 1
    and link with CarbonAccessors.o, which implements just the accessor functions. This will help you
    preserve source compatibility between your Carbon and non-Carbon application targets.
    
    MIXEDMODE_CALLS_ARE_FUNCTIONS   - deprecated.

****************************************************************************************************/
#if TARGET_API_MAC_CARBON && !TARGET_API_MAC_OS8
#ifndef OPAQUE_TOOLBOX_STRUCTS
#define OPAQUE_TOOLBOX_STRUCTS 1
#endif  /* !defined(OPAQUE_TOOLBOX_STRUCTS) */

#ifndef ACCESSOR_CALLS_ARE_FUNCTIONS
#define ACCESSOR_CALLS_ARE_FUNCTIONS 1
#endif  /* !defined(ACCESSOR_CALLS_ARE_FUNCTIONS) */

#ifndef CALL_NOT_IN_CARBON
#define CALL_NOT_IN_CARBON 0
#endif  /* !defined(CALL_NOT_IN_CARBON) */

#ifndef MIXEDMODE_CALLS_ARE_FUNCTIONS
#define MIXEDMODE_CALLS_ARE_FUNCTIONS 1
#endif  /* !defined(MIXEDMODE_CALLS_ARE_FUNCTIONS) */

#else
#ifndef OPAQUE_TOOLBOX_STRUCTS
#define OPAQUE_TOOLBOX_STRUCTS 0
#endif  /* !defined(OPAQUE_TOOLBOX_STRUCTS) */

#ifndef ACCESSOR_CALLS_ARE_FUNCTIONS
#define ACCESSOR_CALLS_ARE_FUNCTIONS 0
#endif  /* !defined(ACCESSOR_CALLS_ARE_FUNCTIONS) */

/*
     * It's possible to have ACCESSOR_CALLS_ARE_FUNCTIONS set to true and OPAQUE_TOOLBOX_STRUCTS
     * set to false, but not the other way around, so make sure the defines are not set this way.
     */
#ifndef CALL_NOT_IN_CARBON
#define CALL_NOT_IN_CARBON 1
#endif  /* !defined(CALL_NOT_IN_CARBON) */

#ifndef MIXEDMODE_CALLS_ARE_FUNCTIONS
#define MIXEDMODE_CALLS_ARE_FUNCTIONS 0
#endif  /* !defined(MIXEDMODE_CALLS_ARE_FUNCTIONS) */

#endif  /* TARGET_API_MAC_CARBON && !TARGET_API_MAC_OS8 */




#endif /* __CONDITIONALMACROS_R__ */

