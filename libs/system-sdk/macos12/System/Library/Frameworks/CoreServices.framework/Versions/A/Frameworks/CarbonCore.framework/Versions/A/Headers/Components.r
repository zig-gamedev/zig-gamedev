/*
     File:       Components.r
 
     Contains:   Component Manager Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1991-2011 by Apple Inc. All rights reserved.
*/

#ifndef __COMPONENTS_R__
#define __COMPONENTS_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define cmpThreadSafe 					0x10000000			/*  component is thread-safe  */
#define cmpIsMissing 					0x20000000
#define cmpWantsRegisterMessage 		0x80000000

#define componentDoAutoVersion 			0x01
#define componentWantsUnregister 		0x02
#define componentAutoVersionIncludeFlags  0x04
#define componentHasMultiplePlatforms 	0x08
#define componentLoadResident 			0x10


/*----------------------------strn ¥ Pascal-Style String--------------------------------*/
    // ¥¥¥ used for component name resources
type 'strn' {
     pstring;                                                /* String               */
};
/*----------------------------stri ¥ Pascal-Style String--------------------------------*/
   // ¥¥¥ used for component info resources
type 'stri' {
     pstring;                                                /* String               */
};
/*----------------------------thng  ¥ Component Manager thing ------------------------*/
/*
  thng_RezTemplateVersion:
       0 - original 'thng' template    <-- default
        1 - extended 'thng' template
       2 - extended 'thng' template including resource map id
*/
#ifndef thng_RezTemplateVersion
  #ifdef UseExtendedThingResource         /* grandfather in use of ÒUseExtendedThingResourceÓ */
     #define thng_RezTemplateVersion 1
  #else
      #define thng_RezTemplateVersion 0
  #endif
#endif


type 'thng' {
     literal longint;                                        /* Type */
     literal longint;                                        /* Subtype */
      literal longint;                                        /* Manufacturer */
     unsigned hex longint;                                   /* component flags */
      unsigned hex longint    kAnyComponentFlagsMask = 0;     /* component flags Mask */
     literal longint;                                        /* Code Type */
        integer;                                                /* Code ID */
      literal longint;                                        /* Name Type */
        integer;                                                /* Name ID */
      literal longint;                                        /* Info Type */
        integer;                                                /* Info ID */
      literal longint;                                        /* Icon Type */
        integer;                                                /* Icon ID */
#if thng_RezTemplateVersion >= 1
     unsigned hex longint;                                   /* version of Component */
     longint;                                                /* registration flags */
       integer;                                                /* resource id of Icon Family */
       longint = $$CountOf(ComponentPlatformInfo);
        wide array ComponentPlatformInfo {
         unsigned hex longint;                               /* component flags */
          literal longint;                                    /* Code Type */
            integer;                                            /* Code ID */
          integer platform68k = 1,                            /* platform type (response from gestaltComponentPlatform if available, or else gestaltSysArchitecture) */
                  platformPowerPC = 2,
                   platformInterpreted = 3,
                   platformWin32 = 4,
                 platformPowerPCNativeEntryPoint = 5,
                   platformIA32NativeEntryPoint = 6,
                  platformPowerPC64NativeEntryPoint = 7,
                 platformX86_64NativeEntryPoint = 8,
                 platformArm64NativeEntryPoint = 9;
        };
#if thng_RezTemplateVersion >= 2
        literal longint;                                        /* resource map type */
        integer;                                                /* resource map id */
#endif
#endif
};


/*----------------------------thga ¥ Thing --------------------------------------------*/
type 'thga' {
       literal longint;                                        /* Type */
     literal longint;                                        /* Subtype */
      literal longint;                                        /* Manufacturer */
     unsigned hex longint;                                   /* component flags */
      unsigned hex longint    kAnyComponentFlagsMask = 0;     /* component flags Mask */
     literal longint;                                        /* Code Type */
        integer;                                                /* Code ID */
      literal longint;                                        /* Name Type */
        integer;                                                /* Name ID */
      literal longint;                                        /* Info Type */
        integer;                                                /* Info ID */
      literal longint;                                        /* Icon Type */
        integer;                                                /* Icon ID */

     literal longint;                                        /* Type */
     literal longint;                                        /* Subtype */
      literal longint;                                        /* Manufacturer */
     unsigned hex longint;                                   /* component flags */
      unsigned hex longint    kAnyComponentFlagsMask = 0;     /* component flags Mask */
#if thng_RezTemplateVersion >= 2
        literal longint;                                        /* resource map type */
        integer;                                                /* resource map id */
      integer cmpAliasNoFlags = 0, cmpAliasOnlyThisFile = 1;  /* thing alias flags */
#endif
};


/*----------------------------thn#  ¥ Component Manager thing load order dependency ----*/
type 'thn#' {
      array {
            literal longint;                                        /* Code Type */
            integer;                                                /* Code ID */
      };
};
/*----------------------------dlle ¥ dlle resource--------------------------------*/
// used for multi-platform things
type 'dlle' {
 cstring;
};

/*-----------------------thnr ¥ a Components public resource map ------------------*/
type 'thnr' {
       array {
            literal longint;                                        /* thing resource type */
          integer;                                                /* thing resource id */
            integer;                                                /* unused flags */

            literal longint;                                        /* Mac resource type */
            integer;                                                /* Mac resource id */
          integer cmpResourceNoFlags = 0, cmpResourceCallComponent = 1;   /* flags*/
     };
};

type 'thar' as 'thnr';


/*---------------------------- thnd ¥ Component dependency (used for missings)------*/
type 'thnd' {
      longint = $$CountOf(ComponentDependency);
      wide array ComponentDependency {
           literal longint;                                        /* Type */
         literal longint;                                        /* Subtype */
          literal longint;                                        /* Manufacturer */
         unsigned hex longint;                                   /* component flags */
          unsigned hex longint    kAnyComponentFlagsMask = 0;     /* component flags Mask */
     };
};


#endif /* __COMPONENTS_R__ */

