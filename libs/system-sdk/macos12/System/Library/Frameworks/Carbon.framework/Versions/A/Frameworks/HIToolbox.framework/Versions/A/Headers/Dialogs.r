/*
     File:       Dialogs.r
 
     Contains:   Dialog Manager interfaces.
 
     Copyright:  © 1985-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __DIALOGS_R__
#define __DIALOGS_R__

#ifndef __MACWINDOWS_R__
#include <HIToolbox/MacWindows.r>
#endif
#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

															/*  Alert types to pass into StandardAlert  */
#define kAlertStopAlert 				0
#define kAlertNoteAlert 				1
#define kAlertCautionAlert 				2
#define kAlertPlainAlert 				3

#define kAlertDefaultOKText 			(-1)				/*  "OK" */
#define kAlertDefaultCancelText 		(-1)				/*  "Cancel" */
#define kAlertDefaultOtherText 			(-1)				/*  "Don't Save" */

#define kAlertStdAlertOKButton 			1
#define kAlertStdAlertCancelButton 		2
#define kAlertStdAlertOtherButton 		3
#define kAlertStdAlertHelpButton 		4

#define kDialogFlagsUseThemeBackground 	0x01
#define kDialogFlagsUseControlHierarchy  0x02
#define kDialogFlagsHandleMovableModal 	0x04
#define kDialogFlagsUseThemeControls 	0x08
#define kDialogFlagsUseCompositing 		0x0100

															/*  Alert Flags for use in alrx resource  */
#define kAlertFlagsUseThemeBackground 	0x01
#define kAlertFlagsUseControlHierarchy 	0x02
#define kAlertFlagsAlertIsMovable 		0x04
#define kAlertFlagsUseThemeControls 	0x08
#define kAlertFlagsUseCompositing 		0x0100

#define kDialogFontNoFontStyle 			0
#define kDialogFontUseFontMask 			0x0001
#define kDialogFontUseFaceMask 			0x0002
#define kDialogFontUseSizeMask 			0x0004
#define kDialogFontUseForeColorMask 	0x0008
#define kDialogFontUseBackColorMask 	0x0010
#define kDialogFontUseModeMask 			0x0020
#define kDialogFontUseJustMask 			0x0040
#define kDialogFontUseAllMask 			0x00FF
#define kDialogFontAddFontSizeMask 		0x0100
#define kDialogFontUseFontNameMask 		0x0200
#define kDialogFontAddToMetaFontMask 	0x0400

#define kDialogFontUseThemeFontIDMask 	0x0080
#define kHICommandOther 				'othr'				/*  sent by standard sheet dialogs when the "other" button is pressed  */
#define kStdCFStringAlertVersionOne 	1
#define kStdCFStringAlertVersionTwo 	2

#define kStdAlertDoNotDisposeSheet 		0x01
#define kStdAlertDoNotAnimateOnDefault 	0x02
#define kStdAlertDoNotAnimateOnCancel 	0x04
#define kStdAlertDoNotAnimateOnOther 	0x08
#define kStdAlertDoNotCloseOnHelp 		0x10


/*----------------------------wctb ¥ Alert Color Lookup Table--------------------------*/
type 'actb' as 'wctb';


/*----------------------------ALRT ¥ Alert Template-------------------------------------*/
/*
 ALRT_RezTemplateVersion:
       0 - original
       1 - additional positioning info at end  <-- default
*/
#ifndef ALRT_RezTemplateVersion
 #define ALRT_RezTemplateVersion 1
#endif


type 'ALRT' {
     rect;                                                   /* boundsRect           */
     integer;                                                /* 'DITL' ID            */

        /* NOTE: the stages are ordered 4, 3, 2, 1 */
      wide array [4] {
               boolean                 OK, Cancel;             /* Bold Outline         */
             boolean                 invisible, visible;     /* Draw Alert           */
             unsigned bitstring[2]   silent = 0
                             sound1, sound2, sound3;         /* Beeps                */
     };

#if ALRT_RezTemplateVersion == 1
   /*  The following are alert positioning options used by System 7.0 and later */
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

/* stages for ALRT */
#define silentStage OK, visible, silent
#define silentStages { silentStage; silentStage; silentStage; silentStage; }

#define beepStage OK, visible, sound1
#define beepStages { beepStage; beepStage; beepStage; beepStage; }


/*----------------------------dctb ¥ Dialog Color Lookup Table--------------------------*/
type 'dctb' as 'wctb';


/*----------------------------DITL ¥ Dialog Item List-----------------------------------*/
type 'DITL' {
      integer = $$CountOf(DITLarray) - 1;                     /* Array size           */
     wide array DITLarray {
             fill long;
             rect;                                           /* Item bounds          */
             switch {

              case HelpItem:                                  /* Help Mgr type item */
                       boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 1;                   /* this is a new type = 1 */

                      switch {
                           case HMScanhdlg:
                               byte = 4;                       /* sizeola              */
                             key int = 1;                    /* key value            */
                             integer;                        /* resource ID          */

                            case HMScanhrct:
                               byte = 4;                       /* sizeola              */
                             key int = 2;                    /* key value            */
                             integer;                        /* resource ID          */

                            case HMScanAppendhdlg:
                             byte = 6;                       /* sizeola              */
                             key int = 8;                    /* key value            */
                             integer;                        /* resource ID          */
                             integer;                        /* offset (zero based)  */
                     };


               case Button:
                       boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 4;
                      pstring;                                /* Title                */

                case CheckBox:
                     boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 5;
                      pstring;                                /* Title                */

                case RadioButton:
                      boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 6;
                      pstring;                                /* Title                */

                case Control:
                      boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 7;
                      byte = 2;
                      integer;                                /* 'CNTL' ID            */

                case StaticText:
                       boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 8;
                      pstring;                                /* Text                 */


               case EditText:
                     boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 16;
                     pstring;                                /* Text                 */
             case Icon:
                     boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 32;
                     byte = 2;
                      integer;                                /* 'ICON' ID            */

                case Picture:
                      boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 64;
                     byte = 2;
                      integer;                                /* 'PICT' ID            */

                case UserItem:
                     boolean         enabled,disabled;       /* Enable flag          */
                     key bitstring[7] = 0;
                      byte = 0;
              };
             align word;
        };
};


/*----------------------------DLOG ¥ Dialog Template------------------------------------*/
/*
 DLOG_RezTemplateVersion:
       0 - original
       1 - additional positioning info at end  <-- default
*/
#ifndef DLOG_RezTemplateVersion
 #define DLOG_RezTemplateVersion 1
#endif


type 'DLOG' {
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
     integer;                                                /* 'DITL' ID            */
     pstring;                                                /* title                */
#if DLOG_RezTemplateVersion == 1
    /*  The following are window positioning options used by System 7.0 and later */
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



/*----------------------------alrx ¥ Extended Alert Template---------------------------*/
type 'alrx'
{
  switch
 {
      case versionZero:
          key integer=0;

            unsigned longint;               /* flags */
            longint;                        /* refCon */
           byte        kUseClassicWindow,
                     kUseThemeWindow;    /* Window type */
          fill byte;                      /* filler */
           fill long;                      /* filler */
           fill long;                      /* filler */
           fill long;                      /* filler */
           fill long;                      /* filler */
           pstring;                        /* title (movable only) */
         
       case versionOne:
           key integer=1;

            unsigned longint;               /* flags */
            longint;                        /* refCon */
           byte        kUseClassicWindow,
                     kUseThemeWindow;    /* Window type */
          fill byte;                      /* filler */
           pstring;                        /* title (movable only) */
 };
};


/*----------------------------dlgx ¥ Extended Dialog Template---------------------------*/
type 'dlgx'
{
  switch
 {
      case versionZero:
          key integer=0;

            unsigned longint;               /* flags */
    };
};


/*-----------------------------dftb ¥ Dialog Control Font Table--------------------------*/
type 'dftb'
{
 switch
 {
      case versionZero:
          key integer = 0;    /* version */

         integer = $$Countof(FontStyle);
            array FontStyle
            {
              switch
             {
                  case skipItem:
                     key integer=0;
                     
                   case dataItem:
                     key integer=1;integer;                              /* Flags        */
                     integer;                                            /* Font ID      */
                     integer;                                            /* Font Size    */
                     integer;                                            /* Font Style   */
                     integer;                                            /* Text Mode    */
                     integer;                                            /* Justification */

                       unsigned integer;                                   /* ForeColor:   red     */
                     unsigned integer;                                   /*              green   */
                     unsigned integer;                                   /*              blue    */

                        unsigned integer;                                   /* BackColor:   red     */
                     unsigned integer;                                   /*              green   */
                     unsigned integer;                                   /*              blue    */

                        pstring;                                            /* Font Name */
                };
         };
 };
};


#endif /* __DIALOGS_R__ */

