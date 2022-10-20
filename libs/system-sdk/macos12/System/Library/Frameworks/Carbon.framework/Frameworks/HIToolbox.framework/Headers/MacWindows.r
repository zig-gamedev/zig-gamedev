/*
     File:       MacWindows.r
 
     Contains:   Window Manager Interfaces
 
     Copyright:  © 1997-2008 by Apple Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __MACWINDOWS_R__
#define __MACWINDOWS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kAlertWindowClass 				1
#define kMovableAlertWindowClass 		2
#define kModalWindowClass 				3
#define kMovableModalWindowClass 		4
#define kFloatingWindowClass 			5
#define kDocumentWindowClass 			6
#define kUtilityWindowClass 			8
#define kHelpWindowClass 				10
#define kSheetWindowClass 				11
#define kToolbarWindowClass 			12
#define kPlainWindowClass 				13
#define kOverlayWindowClass 			14
#define kSheetAlertWindowClass 			15
#define kAltPlainWindowClass 			16
#define kSimpleWindowClass 				18
#define kDrawerWindowClass 				20
#define kAllWindowClasses 				0xFFFFFFFF

#define kWindowNoAttributes 			0
#define kWindowCloseBoxAttribute 		0x01
#define kWindowHorizontalZoomAttribute 	0x02
#define kWindowVerticalZoomAttribute 	0x04
#define kWindowFullZoomAttribute 		0x06
#define kWindowCollapseBoxAttribute 	0x08
#define kWindowResizableAttribute 		0x10
#define kWindowSideTitlebarAttribute 	0x20
#define kWindowToolbarButtonAttribute 	0x40
#define kWindowUnifiedTitleAndToolbarAttribute  0x80
#define kWindowMetalAttribute 			0x0100
#define kWindowNoTitleBarAttribute 		0x0200
#define kWindowTexturedSquareCornersAttribute  0x0400
#define kWindowMetalNoContentSeparatorAttribute  0x0800
#define kWindowHasRoundBottomBarCornersAttribute  0x1000
#define kWindowDoesNotCycleAttribute 	0x8000
#define kWindowNoUpdatesAttribute 		0x00010000
#define kWindowNoActivatesAttribute 	0x00020000
#define kWindowOpaqueForEventsAttribute  0x00040000
#define kWindowCompositingAttribute 	0x00080000
#define kWindowNoShadowAttribute 		0x00200000
#define kWindowCanBeVisibleWithoutLoginAttribute  0x00400000
#define kWindowHideOnSuspendAttribute 	0x01000000
#define kWindowAsyncDragAttribute 		0x00800000
#define kWindowStandardHandlerAttribute  0x02000000
#define kWindowHideOnFullScreenAttribute  0x04000000
#define kWindowInWindowMenuAttribute 	0x08000000
#define kWindowLiveResizeAttribute 		0x10000000
#define kWindowIgnoreClicksAttribute 	0x20000000
#define kWindowHighResolutionCapableAttribute  0x00100000
#define kWindowStandardDocumentAttributes  0x1F
#define kWindowStandardFloatingAttributes  0x09

#define kWindowFrameworkScaledAttribute  0x00100000
#define kWindowDefProcType 				'WDEF'
#define kStandardWindowDefinition 		0					/*  for document windows and dialogs */
#define kRoundWindowDefinition 			1					/*  old Desk Accessory-style window */
#define kFloatingWindowDefinition 		124					/*  for floating windows */

															/*  for use with kStandardWindowDefinition  */
#define kDocumentWindowVariantCode 		0
#define kModalDialogVariantCode 		1
#define kPlainDialogVariantCode 		2
#define kShadowDialogVariantCode 		3
#define kMovableModalDialogVariantCode 	5
#define kAlertVariantCode 				7
#define kMovableAlertVariantCode 		9					/*  for use with kFloatingWindowDefinition  */
#define kSideFloaterVariantCode 		8

															/*  Resource IDs for theme-savvy window defprocs  */
#define kWindowDocumentDefProcResID 	64
#define kWindowDialogDefProcResID 		65
#define kWindowUtilityDefProcResID 		66
#define kWindowUtilitySideTitleDefProcResID  67
#define kWindowSheetDefProcResID 		68
#define kWindowSimpleDefProcResID 		69
#define kWindowSheetAlertDefProcResID 	70

															/*  Proc IDs for theme-savvy windows  */
#define kWindowDocumentProc 			1024
#define kWindowGrowDocumentProc 		1025
#define kWindowVertZoomDocumentProc 	1026
#define kWindowVertZoomGrowDocumentProc  1027
#define kWindowHorizZoomDocumentProc 	1028
#define kWindowHorizZoomGrowDocumentProc  1029
#define kWindowFullZoomDocumentProc 	1030
#define kWindowFullZoomGrowDocumentProc  1031

															/*  Proc IDs for theme-savvy dialogs  */
#define kWindowPlainDialogProc 			1040
#define kWindowShadowDialogProc 		1041
#define kWindowModalDialogProc 			1042
#define kWindowMovableModalDialogProc 	1043
#define kWindowAlertProc 				1044
#define kWindowMovableAlertProc 		1045

															/*  procIDs available from Mac OS 8.1 (Appearance 1.0.1) forward  */
#define kWindowMovableModalGrowProc 	1046
															/*  Proc IDs for top title bar theme-savvy floating windows  */
#define kWindowFloatProc 				1057
#define kWindowFloatGrowProc 			1059
#define kWindowFloatVertZoomProc 		1061
#define kWindowFloatVertZoomGrowProc 	1063
#define kWindowFloatHorizZoomProc 		1065
#define kWindowFloatHorizZoomGrowProc 	1067
#define kWindowFloatFullZoomProc 		1069
#define kWindowFloatFullZoomGrowProc 	1071

															/*  Proc IDs for side title bar theme-savvy floating windows  */
#define kWindowFloatSideProc 			1073
#define kWindowFloatSideGrowProc 		1075
#define kWindowFloatSideVertZoomProc 	1077
#define kWindowFloatSideVertZoomGrowProc  1079
#define kWindowFloatSideHorizZoomProc 	1081
#define kWindowFloatSideHorizZoomGrowProc  1083
#define kWindowFloatSideFullZoomProc 	1085
#define kWindowFloatSideFullZoomGrowProc  1087

															/*  Proc IDs for sheet windows  */
#define kWindowSheetProc 				1088				/*  available in Mac OS X and CarbonLib 1.3  */
#define kWindowSheetAlertProc 			1120				/*  available in Mac OS X 10.1 and CarbonLib 1.3  */

															/*  Proc IDs for simple windows  */
#define kWindowSimpleProc 				1104
#define kWindowSimpleFrameProc 			1105

#define kWindowOverlayProc 				1136
#define kWindowNoPosition 				0x0000
#define kWindowDefaultPosition 			0x0000				/*  used by StandardAlert */
#define kWindowCenterMainScreen 		0x280A
#define kWindowAlertPositionMainScreen 	0x300A
#define kWindowStaggerMainScreen 		0x380A
#define kWindowCenterParentWindow 		0xA80A
#define kWindowAlertPositionParentWindow  0xB00A
#define kWindowStaggerParentWindow 		0xB80A
#define kWindowCenterParentWindowScreen  0x680A
#define kWindowAlertPositionParentWindowScreen  0x700A
#define kWindowStaggerParentWindowScreen  0x780A

#define kWindowCenterOnMainScreen 		1
#define kWindowCenterOnParentWindow 	2
#define kWindowCenterOnParentWindowScreen  3
#define kWindowCascadeOnMainScreen 		4
#define kWindowCascadeOnParentWindow 	5
#define kWindowCascadeOnParentWindowScreen  6
#define kWindowCascadeStartAtParentWindowScreen  10
#define kWindowAlertPositionOnMainScreen  7
#define kWindowAlertPositionOnParentWindow  8
#define kWindowAlertPositionOnParentWindowScreen  9

#define kWindowDefinitionVersionOne 	1
#define kWindowDefinitionVersionTwo 	2

#define kWindowIsCollapsedState 		0x01
#define kStoredWindowSystemTag 			'appl'				/*  Only Apple collection items will be of this tag */
#define kStoredBasicWindowDescriptionID  'sbas'				/*  BasicWindowDescription */
#define kStoredWindowPascalTitleID 		's255'				/*  pascal title string */
#define kStoredWindowTitleCFStringID 	'cfst'				/*  CFString title string */



/*--------------------------wind ¥ Extensible Window Resource Format------------------*/
/* The extensible window resource format is a flattened collection containing items    */
/* stored by StoreWindowIntoCollection, plus possible application-specific items.     */

type 'wind' as 'flac';


/*--------------------------wctb ¥ Window Color Lookup Table--------------------------*/
/*
   wctb_RezTemplateVersion:
       0 - original 
      1 - more color parts and implicit header    <-- default
*/
#ifndef wctb_RezTemplateVersion
 #ifdef oldTemp                          /* grandfather in use of ÒoldTempÓ */
      #define wctb_RezTemplateVersion 0
  #else
      #define wctb_RezTemplateVersion 1
  #endif
#endif


type 'wctb' {
#if wctb_RezTemplateVersion == 0
            unsigned hex longint;                                   /* ctSeed               */
         integer;                                                /* ctFlags              */
#elif wctb_RezTemplateVersion == 1
          unsigned hex longint = 0;                               /* ctSeed               */
         integer = 0;                                            /* ctFlags              */
#endif
          integer = $$Countof(ColorSpec) - 1;                     /* ctSize               */
         wide array ColorSpec {
                 integer     wContentColor,                      /* value                */
                             wFrameColor,
                               wTextColor,
                                wHiliteColor,
                              wTitleBarColor,
                                wHiliteLight,
                              wHiliteDark,
                               wTitleBarLight,
                                wTitleBarDark,
                             wDialogLight,
                              wDialogDark,
                               wTingeLight,
                               wTingeDark;
                    unsigned integer;                               /* RGB: red             */
                 unsigned integer;                               /*      green           */
                 unsigned integer;                               /*      blue            */
         };
 };


/*----------------------------WIND ¥ Window Template------------------------------------*/
/*
 WIND_RezTemplateVersion:
       0 - original 
      1 - additional positioning info at end  <-- default 
*/
#ifndef WIND_RezTemplateVersion
    #define WIND_RezTemplateVersion 1
#endif


type 'WIND' {
     rect;                                                   /* boundsRect           */
     integer         documentProc,                           /* procID               */
                     dBoxProc,
                      plainDBox,
                     altDBoxProc,
                       noGrowDocProc,
                     movableDBoxProc,
                       zoomDocProc = 8,
                       zoomNoGrow = 12;
       byte            invisible, visible;                     /* visible              */
     fill byte;
     byte            noGoAway, goAway;                       /* goAway               */
     fill byte;
     unsigned hex longint;                                   /* refCon               */
     pstring         Untitled = "Untitled";              /* title                */
     
#if WIND_RezTemplateVersion == 1
  /*  The following are window positioning options used from Mac OS 7.0 forward */
       align word;
        unsigned integer                noAutoCenter = 0x0000,
                                     centerMainScreen = 0x280a,
                                     alertPositionMainScreen = 0x300a,
                                      staggerMainScreen = 0x380a,
                                        centerParentWindow = 0xa80a,
                                       alertPositionParentWindow = 0xb00a,
                                        staggerParentWindow = 0xb80a,
                                      centerParentWindowScreen = 0x680a,
                                     alertPositionParentWindowScreen = 0x700a,
                                      staggerParentWindowScreen = 0x780a;
#endif
};


#endif /* __MACWINDOWS_R__ */

