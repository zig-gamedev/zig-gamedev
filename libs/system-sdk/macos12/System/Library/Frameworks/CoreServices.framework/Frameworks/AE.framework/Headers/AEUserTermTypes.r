/*
     File:       AEUserTermTypes.r
 
     Contains:   AppleEvents AEUT resource format Interfaces.
 
    
 
     Copyright:  � 1991-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __AEUSERTERMTYPES_R__
#define __AEUSERTERMTYPES_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kAEUserTerminology 				'aeut'				/*   0x61657574   */
#define kAETerminologyExtension 		'aete'				/*   0x61657465   */
#define kAEScriptingSizeResource 		'scsz'				/*   0x7363737a   */
#define kAEOSAXSizeResource 			'osiz'


#include <AE/AEObjects.r>



//    "reserved" needs to be defined to be false for resources
//   but undef'ed for type definitions.  We preserve its state
//   and undef it here.

#undef reserved

/*-------------------aeut : Apple Event User Terminology--------------------------------*/
/*
    aeut_RezTemplateVersion:
       0 - original                            <-- default
        1 - wide version
*/
#ifndef aeut_RezTemplateVersion
    #define aeut_RezTemplateVersion 0
#endif


type 'aeut' {
     hex byte;                                           /* major version in BCD     */
     hex byte;                                           /* minor version in BCD     */
     integer     Language, english = 0, japanese = 11;   /* language code            */
     integer     Script, roman = 0;                      /* script code              */
     integer = $$Countof(Suites);
       array Suites {
             pstring;                                    /* suite name               */
             pstring;                                    /* suite description        */
             align word;                                 /* alignment                */
             literal longint;                            /* suite ID                 */
             integer;                                    /* suite level              */
             integer;                                    /* suite version            */
             integer = $$Countof(Events);
               array Events {
                 pstring;                                /* event name               */
                 pstring;                                /* event description        */
                 align word;                             /* alignment                */
                 literal longint;                        /* event class              */
                 literal longint;                        /* event ID                 */
                 literal longint     noReply = 'null';   /* reply type               */
                 pstring;                                /* reply description        */
                 align word;                             /* alignment                */
#if aeut_RezTemplateVersion == 1
                    wide array [1] {
#endif

                       boolean replyRequired,                  /* if the reply is          */
                             replyOptional;                  /*   required               */
                     boolean singleItem,                     /* if the reply must be a   */
                             listOfItems;                    /*   list                   */                                      
                       boolean notEnumerated,                  /* if the type is           */
                             enumerated;                     /*   enumerated             */
                     boolean notTightBindingFunction,        /* if the message has tight */
                             tightBindingFunction;           /*   binding precedence     */
                     boolean enumsAreConstants,              /* AppleScript 1.3:         */
                             enumsAreTypes;                  /*   reply can be one of these types */                 
                       boolean enumListCanRepeat,              /* AppleScript 1.3:         */
                             enumListIsExclusive;            /*   reply is a set of nonrepeating values */
                      boolean replyIsValue,                   /* AppleScript 1.3:         */
                             replyIsReference;               /*   reply is a reference, not a value */
                      boolean reserved;
                  /* the following bits are reserved for localization */
                     boolean reserved;
                      boolean reserved;
                      boolean reserved;
                      boolean reserved;
                      boolean verbEvent,                      /* for Japanese; nonVerb    */
                             nonVerbEvent;                   /*  is used as an expr v.s. */
                                                             /*  v.s. verb is a command  */
                         /* the following 3 bits are considered as a part of look up key */
                     boolean reserved;   
                       boolean reserved;
                      boolean reserved;
#if aeut_RezTemplateVersion == 1
                 };
#endif
                  literal longint     noDirectParam = 'null',
                                        noParams ='null';   /* direct param type        */
                 pstring;                                /* direct param description */
                 align word;                             /* alignment                */
#if aeut_RezTemplateVersion == 1
                    wide array [1] {
#endif

                       boolean directParamRequired,            /* if the direct param      */
                             directParamOptional;            /*   is required            */
                     boolean singleItem,                     /* if the param must be a   */
                             listOfItems;                    /*   list                   */                                      
                       boolean notEnumerated,                  /* if the type is           */
                             enumerated;                     /*   enumerated             */
                     boolean doesntChangeState,              /* if the event changes     */
                             changesState;                   /*   server's state         */
                     boolean enumsAreConstants,              /* AppleScript 1.3:         */
                             enumsAreTypes;                  /*   direct parameter can be one of these types */                  
                       boolean enumListCanRepeat,              /* AppleScript 1.3:         */
                             enumListIsExclusive;            /*   direct parameter is a set of nonrepeating values */
                       boolean directParamIsValue,             /* AppleScript 1.3:         */
                             directParamIsReference;         /*   direct parameter is a reference, not a value */
                       boolean directParamIsTarget,            /* AppleScript 1.3:         */
                             notDirectParamIsTarget;         /*   event should not be dispatched to this object */
                      boolean reserved;
                      boolean reserved;
                      boolean reserved;
                      boolean reserved;
                      boolean reserved;
                      boolean reserved;
                      boolean reserved;
                      boolean reserved;
#if aeut_RezTemplateVersion == 1
                 };
#endif
                  integer = $$Countof(OtherParams);
                  array OtherParams {
                        pstring;                            /* parameter name           */
                     align word;                         /* alignment                */
                     literal longint;                    /* parameter keyword        */
                     literal longint;                    /* parameter type           */
                     pstring;                            /* parameter description    */
                     align word;                         /* alignment                */
#if aeut_RezTemplateVersion == 1
                        wide array [1] {
#endif

                           boolean required,
                                  optional;                   /* if param is optional     */
                         boolean singleItem,                 /* if the param must be a   */
                                 listOfItems;                /*   list                   */                                      
                           boolean notEnumerated,              /* if the type is           */
                                 enumerated;                 /*   enumerated             */
                         boolean reserved;
                          boolean enumsAreConstants,          /* AppleScript 1.3:         */
                                 enumsAreTypes;              /*   reply can be one of these types */                 
                           boolean enumListCanRepeat,          /* AppleScript 1.3:         */
                                 enumListIsExclusive;        /*   parameter is a set of nonrepeating values */
                          boolean paramIsValue,               /* AppleScript 1.3:         */
                                 paramIsReference;           /*   parameter is a reference, not a value */
                          boolean notParamIsTarget,           /* AppleScript 1.3:         */
                                 paramIsTarget;              /*   event should be dispatched to this object */
                      /* the following bits are reserved for localization */
                         boolean reserved;
                          boolean reserved;
                          boolean reserved;
                          boolean reserved;
                          boolean prepositionParam,           /* for Japanese; labeled    */
                                 labeledParam;               /*  param name comes before */
                                                             /*  the param value         */
                             /* the following 3 bits are considered as a part of look up key */
                         boolean notFeminine,                /* feminine                 */
                                 feminine;
                          boolean notMasculine,               /* masculine                */  
                                   masculine;                  
                           boolean singular,
                                  plural;                     /* plural                   */
#if aeut_RezTemplateVersion == 1
                        };
#endif
                  };
             };
             integer = $$Countof(Classes);
              array Classes {
                    pstring;                                /* class name               */
                 align word;                             /* alignment                */
                 literal longint;                        /* class ID                 */
                 pstring;                                /* class description        */
                 align word;                             /* alignment                */
                 integer = $$Countof(Properties);


                   array Properties {
                     pstring;                            /* property name            */
                     align word;                         /* alignment                */
                     literal longint;                    /* property ID              */
                     literal longint;                    /* property class           */
                     pstring;                            /* property description     */
                     align word;                         /* alignment                */
#if aeut_RezTemplateVersion == 1
                        wide array [1] {
#endif
                            boolean reserved;                   /* reserved                 */
                         boolean singleItem,                 /* if the property must be  */
                                 listOfItems;                /*   a list                 */                                      
                           boolean notEnumerated,              /* if the type is           */
                                 enumerated;                 /*   enumerated             */
                         boolean readOnly,                   /* can only read it         */
                                 readWrite;                  /* can read or write it     */
                         boolean enumsAreConstants,          /* AppleScript 1.3:         */
                                 enumsAreTypes;              /*   property can be one of these types */                  
                           boolean enumListCanRepeat,          /* AppleScript 1.3:         */
                                 enumListIsExclusive;        /*   property is a set of nonrepeating values */
                           boolean propertyIsValue,            /* AppleScript 1.3:         */
                                 propertyIsReference;        /*   property is a reference, not a value */
                           boolean reserved;
                  /* the following bits are reserved for localization */
                         boolean reserved;
                          boolean reserved;
                          boolean reserved;
                          boolean reserved;
                          boolean noApostrophe,   /* This bit is special to the French dialect */
                                    apostrophe;     /* It indicates that the name begins */
                                                    /* with a vowel */
                             /* the following 3 bits are considered as a part of look up key */
                                         /* what if both feminine and masculine? */
                             /* the following 3 bits are considered as a part of look up key */
                         boolean notFeminine,                /* feminine                 */
                                 feminine;
                          boolean notMasculine,               /* masculine                */  
                                   masculine;                  
                           boolean singular,
                                  plural;                     /* plural                   */
#if aeut_RezTemplateVersion == 1
                        };
#endif

                   };
                 integer = $$Countof(Elements);
                 array Elements {
                       literal longint;                    /* element class            */
                     integer = $$Countof(KeyForms);
                     array KeyForms {                    /* list of key forms        */
                         literal longint;                /* key form ID              */
                     };
                 };
             };
             integer = $$Countof(ComparisonOps);
                array ComparisonOps {
                  pstring;                                /* comparison operator name */
                 align word;                             /* alignment                */
                 literal longint;                        /* comparison operator ID   */
                 pstring;                                /* comparison comment       */
                 align word;                             /* alignment                */
             };
             integer = $$Countof(Enumerations);
             array Enumerations {                        /* list of Enumerations     */
                 literal longint;                        /* Enumeration ID           */
                 integer = $$Countof(Enumerators);
                  array Enumerators {                     /* list of Enumerators      */
                     pstring;                            /* Enumerator name          */
                     align word;                         /* alignment                */
                     literal longint;                    /* Enumerator ID            */
                     pstring;                            /* Enumerator comment       */
                     align word;                         /* alignment                */
                 };
             };
         };
     };


/* Description of the Apple Event Terminology Extension resource       */
/* This resource is provided by your application                     */

type 'aete' as 'aeut';

type 'scsz' {
  boolean             dontReadExtensionTerms, readExtensionTerms, dontLaunchToGetTerminology = 0, launchToGetTerminology = 1;
    boolean             findAppBySignature,     dontFindAppBySignature;
    boolean             dontAlwaysSendSubject,  alwaysSendSubject;
 boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  boolean             reserved;
  /* Memory sizes are in bytes. Zero means use default. */
   unsigned longint    minStackSize;
  unsigned longint    preferredStackSize;
    unsigned longint    maxStackSize;
  unsigned longint    minHeapSize;
   unsigned longint    preferredHeapSize;
 unsigned longint    maxHeapSize;    
};


/* Description of Scripting Additions size resource                    */
/* From AppleScript 1.1 SDK                                          */

type 'osiz' {
      boolean                 openResourceFile,
                              dontOpenResourceFile;
      boolean                 acceptRemoteEvents, 
                               dontAcceptRemoteEvents;
        boolean                 openWithReadPermission,
                                openWithReadWritePermission;
       boolean                 reserved;
      boolean                 osaxCodeInResources,                /* AppleScript 1.3 new for shared library osaxen */
                                osaxCodeInSharedLibraries;
     boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
      boolean                 reserved;
};

#define reserved false


#endif /* __AEUSERTERMTYPES_R__ */

