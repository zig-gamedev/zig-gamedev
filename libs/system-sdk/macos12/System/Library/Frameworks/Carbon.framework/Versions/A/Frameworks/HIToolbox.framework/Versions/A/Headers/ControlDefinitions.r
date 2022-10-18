/*
     File:       ControlDefinitions.r
 
     Contains:   Definitions of controls provided by the Control Manager
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __CONTROLDEFINITIONS_R__
#define __CONTROLDEFINITIONS_R__

#ifndef __CONTROLS_R__
#include <HIToolbox/Controls.r>
#endif

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define pushButProc 					0
#define checkBoxProc 					1
#define radioButProc 					2
#define scrollBarProc 					16
#define popupMenuProc 					1008

#define kControlLabelPart 				1
#define kControlMenuPart 				2
#define kControlTrianglePart 			4
#define kControlEditTextPart 			5					/*  Appearance 1.0 and later */
#define kControlPicturePart 			6					/*  Appearance 1.0 and later */
#define kControlIconPart 				7					/*  Appearance 1.0 and later */
#define kControlClockPart 				8					/*  Appearance 1.0 and later */
#define kControlListBoxPart 			24					/*  Appearance 1.0 and later */
#define kControlListBoxDoubleClickPart 	25					/*  Appearance 1.0 and later */
#define kControlImageWellPart 			26					/*  Appearance 1.0 and later */
#define kControlRadioGroupPart 			27					/*  Appearance 1.0.2 and later */
#define kControlButtonPart 				10
#define kControlCheckBoxPart 			11
#define kControlRadioButtonPart 		11
#define kControlUpButtonPart 			20
#define kControlDownButtonPart 			21
#define kControlPageUpPart 				22
#define kControlPageDownPart 			23
#define kControlClockHourDayPart 		9					/*  Appearance 1.1 and later */
#define kControlClockMinuteMonthPart 	10					/*  Appearance 1.1 and later */
#define kControlClockSecondYearPart 	11					/*  Appearance 1.1 and later */
#define kControlClockAMPMPart 			12					/*  Appearance 1.1 and later */
#define kControlDataBrowserPart 		24					/*  CarbonLib 1.0 and later */
#define kControlDataBrowserDraggedPart 	25					/*  CarbonLib 1.0 and later */

#define kControlEditTextProc 			272
#define kControlEditTextPasswordProc 	274

#define kControlEditTextInlineInputProc  276				/*  Can't combine with the other variants */
#define kControlKindEditText 			'etxt'
#define kControlPictureProc 			304
#define kControlPictureNoTrackProc 		305					/*  immediately returns kControlPicturePart */

#define kControlKindPicture 			'pict'
#define kControlPictureHandleTag 		'pich'				/*  PicHandle */
#define kControlListBoxProc 			352
#define kControlListBoxAutoSizeProc 	353

#define kControlKindListBox 			'lbox'
#define kControlListBoxListHandleTag 	'lhan'				/*  ListHandle */
#define kControlListBoxKeyFilterTag 	'fltr'				/*  ControlKeyFilterUPP */
#define kControlListBoxFontStyleTag 	'font'				/*  ControlFontStyleRec */

#define kControlListBoxDoubleClickTag 	'dblc'				/*  Boolean. Was last click a double-click? */
#define kControlListBoxLDEFTag 			'ldef'				/*  SInt16. ID of LDEF to use. */

#define kControlListDescResType 		'ldes'				/*  used for list box control (Appearance 1.0 and later) */
#define kControlScrollTextBoxProc 		432
#define kControlScrollTextBoxAutoScrollProc  433

#define kControlKindScrollingTextBox 	'stbx'
#define kControlScrollTextBoxDelayBeforeAutoScrollTag  'stdl' /*  UInt32 (ticks until autoscrolling starts) */
#define kControlScrollTextBoxDelayBetweenAutoScrollTag  'scdl' /*  UInt32 (ticks between scrolls) */
#define kControlScrollTextBoxAutoScrollAmountTag  'samt'	/*  UInt16 (pixels per scroll) -- defaults to 1 */
#define kControlScrollTextBoxContentsTag  'tres'			/*  SInt16 (resource ID of 'TEXT'/'styl') -- write only! */
#define kControlScrollTextBoxAnimatingTag  'anim'			/*  Boolean (whether the text box should auto-scroll) */


/*--------------------------ldes ¥ List Box Description Template------------------------*/
/*  Used in conjunction with the list box control.                                    */

type 'ldes'
{
    switch
 {
      case versionZero:
          key integer = 0;    /* version */

         integer;                                                /* Rows                 */
         integer;                                                /* Columns              */
         integer;                                                /* Cell Height          */
         integer;                                                /* Cell Width           */
         byte            noVertScroll, hasVertScroll;            /* Vert Scroll          */
         fill byte;                                              /* Filler Byte          */
         byte            noHorizScroll, hasHorizScroll;          /* Horiz Scroll         */
         fill byte;                                              /* Filler Byte          */
         integer;                                                /* LDEF Res ID          */
         byte            noGrowSpace, hasGrowSpace;              /* HasGrow?             */
         fill byte;
 };
};


#endif /* __CONTROLDEFINITIONS_R__ */

