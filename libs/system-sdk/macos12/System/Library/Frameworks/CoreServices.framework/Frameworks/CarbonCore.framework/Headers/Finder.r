/*
     File:       Finder.r
 
     Contains:   Finder flags and container types.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1990-2011 by Apple Inc. All rights reserved.
*/

#ifndef __FINDER_R__
#define __FINDER_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define kCustomIconResource 			(-16455)			/*  Custom icon family resource ID  */
#define kCustomBadgeResourceType 		'badg'
#define kCustomBadgeResourceID 			(-16455)
#define kCustomBadgeResourceVersion 	0


/*----------------------------badg ¥ Custom badge---------------------------------------*/
type 'badg' {
  integer = 0;            /* version */
  integer;                /* customBadgeResourceID */
    literal longint;        /* customBadgeType */
  literal longint;        /* customBadgeCreator */
   literal longint;        /* windowBadgeType */
  literal longint;        /* windowBadgeCreator */
   literal longint;        /* overrideType */
 literal longint;        /* overrideCreator */
};

#define kRoutingResourceType 			'rout'
#define kRoutingResourceID 				0


/*----------------------------rout ¥ Folder routing-------------------------------------*/
type 'rout' {
  array {
        literal longint;        /* creator */
      literal longint;        /* fileType */
     literal longint;        /* targetFolder */
     literal longint;        /* destinationFolder */
        longint = 0;            /* Reserved (set to 0) */
  };
};

#define kContainerFolderAliasType 		'fdrp'				/*  type for folder aliases  */
#define kContainerTrashAliasType 		'trsh'				/*  type for trash folder aliases  */
#define kContainerHardDiskAliasType 	'hdsk'				/*  type for hard disk aliases  */
#define kContainerFloppyAliasType 		'flpy'				/*  type for floppy aliases  */
#define kContainerServerAliasType 		'srvr'				/*  type for server aliases  */
#define kApplicationAliasType 			'adrp'				/*  type for application aliases  */
#define kContainerAliasType 			'drop'				/*  type for all other containers  */
#define kDesktopPrinterAliasType 		'dtpa'				/*  type for Desktop Printer alias  */
#define kContainerCDROMAliasType 		'cddr'				/*  type for CD-ROM alias  */
#define kApplicationCPAliasType 		'acdp'				/*  type for application control panel alias  */
#define kApplicationDAAliasType 		'addp'				/*  type for application DA alias  */
#define kPackageAliasType 				'fpka'				/*  type for plain package alias  */
#define kAppPackageAliasType 			'fapa'				/*  type for application package alias  */

#define kSystemFolderAliasType 			'fasy'
#define kAppleMenuFolderAliasType 		'faam'
#define kStartupFolderAliasType 		'fast'
#define kPrintMonitorDocsFolderAliasType  'fapn'
#define kPreferencesFolderAliasType 	'fapf'
#define kControlPanelFolderAliasType 	'fact'
#define kExtensionFolderAliasType 		'faex'

#define kExportedFolderAliasType 		'faet'
#define kDropFolderAliasType 			'fadr'
#define kSharedFolderAliasType 			'fash'
#define kMountedFolderAliasType 		'famn'


/*----------------------------BNDL ¥ Bundle---------------------------------------------*/
type 'BNDL' {
        literal longint;                                        /* Signature            */
        integer;                                                /* Version ID           */
        integer = $$CountOf(TypeArray) - 1;
        array TypeArray {
                literal longint;                                /* Type                 */
                integer = $$CountOf(IDArray) - 1;
                wide array IDArray {
                        integer;                                /* Local ID             */
                        integer;                                /* Actual ID            */
                };
        };
};

/*----------------------------open ¥ FileTypes an app can open--------------------------*/
type 'open' {
        literal longint;                                        /* app signature        */
        integer = 0;
        integer = $$CountOf(typeArray);                         /* Array size           */
        wide array typeArray {                                  /* FileTypes that app can open */
                literal longint;                                /* FileType             */
        };
};

/*----------------------------FREF ¥ File Reference-------------------------------------*/
type 'FREF' {
        literal longint;                                        /* File Type            */
        integer;                                                /* Icon ID              */
        pstring;                                                /* Filename             */
};

/*----------------------------kind ¥ Custom kind strings --------------------------------*/
type 'kind' {
        literal longint;                                        /* app signature         */
        integer;                                                /* region code of kind string localizations */
        integer = 0;
        integer = $$CountOf(kindArray);                         /* Array size           */
        wide array kindArray {
                literal longint;                                /* File Type            */
                pstring;                                        /* custom kind strings  */
                align   word;
        };
};
/*----------------------------mach ¥ cdev filtering-------------------------------------*/
type 'mach' {
        unsigned hex integer;                                               /* Softmask */
        unsigned hex integer;                                               /* Hardmask */
};
/*----------------------------nrct ¥ Rectangle List-------------------------------------*/
type 'nrct' {
        integer = $$CountOf(RectArray);                         /* Number of rectangles */
        array RectArray {
            rect;
        };
};


#endif /* __FINDER_R__ */

