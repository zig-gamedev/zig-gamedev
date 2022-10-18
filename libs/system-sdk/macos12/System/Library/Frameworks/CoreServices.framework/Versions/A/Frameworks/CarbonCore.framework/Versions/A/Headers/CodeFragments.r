/*
     File:       CodeFragments.r
 
     Contains:   Public Code Fragment Manager Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1992-2011 by Apple Inc. All rights reserved.
*/

#ifndef __CODEFRAGMENTS_R__
#define __CODEFRAGMENTS_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif


/*
    The 'cfrg' resource serves to inform the Process Manager and Code Fragment Manager
 of code fragments present in this file.
    
   The 'cfrg' resource must have an ID of 0.  There can only be one 'cfrg' resource
   per file, however, the trailing part of a 'cfrg' resource is an array of entries.
  Each entry can be used to associate a name and architecture type to a specified 
   code fragment. The names are used when the system searches for code fragments as 
  named shared libraries.  There can be more than one cfrg array entry per code 
 fragment, this is used to give a single code fragment more than one name.

    For Applications
       The 'cfrg' is required to inform the Process Manager that there is a 
      code fragment available.  One of the entries which has the architecture
    type appropriate for the given machine (ie: pwpc, or m68k) should have the 
    kIsApp usage designation.  The Process Manager will choose this code fragment 
     over the traditional CODE 0.  The location of the code fragment (usually in 
       the data fork) is described within the cfrg array entry.  
     
       For applications, the name is important only when an application has by name 
      call backs, that is, exports for drop in extensions that import from the 
      application.  Generally speaking, the name of the application is used for 
     its 'cfrg' array entry.
    
    For Shared Libraries
      The 'cfrg' is required to allow the Code Fragment Manager to find shared 
      libraries. 
    
       Shared Libraries are libraries that satisfy link time imports, that are 
       usually placed in the extensions folder; they are automatically connected to 
      by the Code Fragment Manager at application launch time.  Shared Libraries
     must have the file type 'shlb'.  The usage field is set to kIsLib for shared 
      libraries.


   Plug-in extensions (code fragments packaged in various application specific ways)
  are that loaded programaticallly by the application do not require the file type 
  'shlb' and a cfrg resource, but may use some similar mechanism that is application
 specific and developer defined.


    There are two versions of the 'cfrg' resource template.

 The original version (cfrg_RezTemplateVersion = 0) is appropriate for 
 applications and most shared libraries.
    
   The second version (cfrg_RezTemplateVersion = 1) is needed only when the code 
 fragment will be found using the new CFM/SOM search mechanisms. It
 contains additional information.
   
   In order to mix both kinds of entries in one 'cfrg' resources, the second
  version of the template contains a switch statement for specifying which
   kind of entry is being generated.  The following example shows how to 
 convert a cfrg_RezTemplateVersion=0 format to a cfrg_RezTemplateVersion=1
  format 'cfrg' resource:         


   When cfrg_RezTemplateVersion=0 use:
    
               resource 'cfrg' (0) {
                  {
                      kPowerPCCFragArch, kIsCompleteCFrag, kNoVersionNum, kNoVersionNum,
                     kDefaultStackSize, kNoAppSubFolder,
                        kApplicationCFrag, kDataForkCFragLocator, kZeroOffset, kCFragGoesToEOF,
                        "My Application",
                  }
              };
             
   when cfrg_RezTemplateVersion=1 use:
                resource 'cfrg' (0) {
                  {
                      regularEntry {      // <--- add this line   
                           kPowerPCCFragArch, kIsCompleteCFrag, kNoVersionNum, kNoVersionNum,
                         kDefaultStackSize, kNoAppSubFolder,
                            kApplicationCFrag, kDataForkCFragLocator, kZeroOffset, kCFragGoesToEOF,
                            "My Application",
                      }                   // <--- and this line 
                 }
              };
             
   The compiled binary resource is identical.
 
   The extended form allows specification of an OSType and 4 Str255s that are 
    used as criterea in searching for libraries that satisfy some contstraints.
    For example, a SOM class library that is a particular sub-class of the 
    "ClassXYZ" base class, might be represented as :
               resource 'cfrg' (0) {
                  {
                      extendedEntry {
                            kPowerPCCFragArch, kIsCompleteCFrag, kNoVersionNum, kNoVersionNum,
                         kDefaultStackSize, kNoAppSubFolder,
                            kImportLibraryCFrag,kOnDiskFlat,kZeroOffset,kCFragGoesToEOF,
                           "MyLibName",    // standard internal name: used by CFM 
                            // start of extended info 
                         kFragSOMClassLibrary,
                          "ClassXYZ",
                            "",
                            "",
                            "My Name"       // external name: may be seen by user 
                     }
                  }
              };
 Using the new CFM/SOM search functions, one could search for all som
   class libraries that inherit from "ClassXYZ".
*/


/*---------------------------cfrg ¥ CodeFragments---------------------------*/
/*
 cfrg_RezTemplateVersion:
       0 - original                        <-- default
        1 - extended for SOM 
*/
#ifndef cfrg_RezTemplateVersion
   #ifdef UseExtendedCFRGTemplate                          /* grandfather in use of ÒUseExtendedCFRGTemplateÓ */
      #define cfrg_RezTemplateVersion 1
  #else
      #define cfrg_RezTemplateVersion 0
  #endif
#endif


/* grandfather in old names for fields */
#define kIsLib               kImportLibraryCFrag
#define kIsApp              kApplicationCFrag
#define kIsDropIn             kDropInAdditionCFrag
#define kInMem                 kMemoryCFragLocator
#define kOnDiskFlat         kDataForkCFragLocator
#define kOnDiskSegmented  kResourceCFragLocator
#define kPowerPC          kPowerPCCFragArch
#define kMotorola         kMotorola68KCFragArch
#define kFullLib          kIsCompleteCFrag
#define kUpdateLib         kFirstCFragUpdate
#define kWholeFork            kCFragGoesToEOF


type 'cfrg' {
   longint = 0;            /* reserved - in use */
   longint = 0;            /* reserved - in use */
   longint = 1;            /* cfrgVersion       */
   longint = 0;            /* reserved - in use */
   longint = 0;            /* reserved - in use */
   longint = 0;            /* reserved - free   */
   longint = 0;            /* reserved - free   */
   longint = $$CountOf (memberArray);
   Array memberArray {
     memberStart:
      align long;
     
#if cfrg_RezTemplateVersion == 1
      switch{
      case regularEntry:
#endif   
       literal longint archType, kPowerPCCFragArch = 'pwpc', kMotorola68KCFragArch = 'm68k';
      longint         updateLevel, kIsCompleteCFrag = 0, kFirstCFragUpdate = 1;
      hex longint     currentVersion, kNoVersionNum = 0;
     hex longint     oldDefVersion, kNoVersionNum = 0;
      longint         appStackSize, kDefaultStackSize = 0;
       integer         appSubFolderID, kNoAppSubFolder = 0;
       byte            usage, kImportLibraryCFrag = 0, kApplicationCFrag = 1, 
                            kDropInAdditionCFrag = 2, kStubLibraryCFrag = 3, 
                          kWeakStubLibraryCFrag = 4;
     byte            where, kMemoryCFragLocator = 0, kDataForkCFragLocator = 1, 
                            kResourceCFragLocator = 2;
     longint         offset, kZeroOffset = 0, kRSEG = 'rseg';
       longint         length, kCFragGoesToEOF = 0, kSegIDZero = 0;
       longint         = 0;      /* reserved - free */
#if cfrg_RezTemplateVersion == 1
       integer         = 0;      /* reserved - free */
        key integer     = 0;      /* no cfrg extensions */
#else
       longint         = 0;      /* reserved - free   */
#endif
       integer         = (memberEnd[$$ArrayIndex(memberArray)] - memberStart[$$ArrayIndex(memberArray)]) / 8;
     pstring;        /* member name */


#if cfrg_RezTemplateVersion == 1
      case extendedEntry:
        literal longint archType, kPowerPCCFragArch = 'pwpc', kMotorola68KCFragArch = 'm68k';
      longint         updateLevel, kIsCompleteCFrag = 0, kFirstCFragUpdate = 1;
      hex longint     currentVersion, kNoVersionNum = 0;
     hex longint     oldDefVersion, kNoVersionNum = 0;
      longint         appStackSize, kDefaultStackSize = 0;
       integer         appSubFolderID, kNoAppSubFolder = 0;
       byte            usage, kImportLibraryCFrag = 0, kApplicationCFrag = 1, 
                            kDropInAdditionCFrag = 2, kStubLibraryCFrag = 3, 
                          kWeakStubLibraryCFrag = 4;
     byte            where, kMemoryCFragLocator = 0, kDataForkCFragLocator = 1, 
                                kResourceCFragLocator = 2;
     longint         offset, kZeroOffset = 0, kRSEG = 'rseg';
       longint         length, kCFragGoesToEOF = 0, kSegIDZero = 0;
       longint         = 0;      /* reserved - free */
        integer         = 0;      /* reserved - free */
        key integer     = 1;      /* one extension   */
        integer         = (memberEnd[$$ArrayIndex(memberArray)] - memberStart[$$ArrayIndex(memberArray)]) / 8;
     pstring;        /* member name */
      align long;     /* match size to C structure size */
       extensionStart: 
       integer       = 0x30ee;     /* magic # signifies this extended cfrg entry format */
        integer         = (extensionEnd[$$ArrayIndex(memberArray)] - extensionStart[$$ArrayIndex(memberArray)]) / 8;
       literal longint libKind, 
                  kFragDocumentPartHandler    = 'part',
                  kFragSOMClassLibrary        = 'clas',
                  kFragInterfaceDefinition    = 'libr',
                  kFragComponentMgrComponent  = 'comp';
                  /* others allowed, need to be a DTS registered OSType */
       pstring;          /* qualifier 1: 
                         'part' : part handler type
                         'clas' : base class name
                           'libr' : interface definition name
                         'comp' : component kind
                      */


       pstring;          /* qualifier 2:
                          'part' : part handler sub type ?
                           'clas' : not used
                          'libr' : not used
                          'comp' : component sub kind
                      */
       pstring;          /* infoStr, optional information depending on libKind */
     pstring;          /* intlName, an internationalizable string that can
                       be displayed to the user on the screen */
     align long;
       extensionEnd:
     };
#endif
     align long;
     memberEnd:
   };
};


#endif /* __CODEFRAGMENTS_R__ */

