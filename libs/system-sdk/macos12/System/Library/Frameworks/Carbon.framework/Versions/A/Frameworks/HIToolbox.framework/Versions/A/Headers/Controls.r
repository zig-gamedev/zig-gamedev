/*
     File:       Controls.r
 
     Contains:   Control Manager interfaces
 
     Copyright:  © 1985-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __CONTROLS_R__
#define __CONTROLS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlNoVariant 				0					/*  No variant */
#define kControlUsesOwningWindowsFontVariant  0x08			/*  Control uses owning windows font to display text */

#define kControlNoPart 					0
#define kControlIndicatorPart 			129
#define kControlDisabledPart 			254
#define kControlInactivePart 			255

#define kControlEntireControl 			0
#define kControlStructureMetaPart 		(-1)
#define kControlContentMetaPart 		(-2)
#define kControlOpaqueMetaPart 			(-3)
#define kControlClickableMetaPart 		(-4)

#define kControlFocusNoPart 			0					/*  tells control to clear its focus */
#define kControlFocusNextPart 			(-1)				/*  tells control to focus on the next part */
#define kControlFocusPrevPart 			(-2)				/*  tells control to focus on the previous part */

#define kControlCollectionTagBounds 	'boun'
#define kControlCollectionTagValue 		'valu'
#define kControlCollectionTagMinimum 	'min '
#define kControlCollectionTagMaximum 	'max '
#define kControlCollectionTagViewSize 	'view'
#define kControlCollectionTagVisibility  'visi'
#define kControlCollectionTagRefCon 	'refc'
#define kControlCollectionTagTitle 		'titl'
#define kControlCollectionTagUnicodeTitle  'uttl'
#define kControlCollectionTagIDSignature  'idsi'
#define kControlCollectionTagIDID 		'idid'
#define kControlCollectionTagCommand 	'cmd '
#define kControlCollectionTagVarCode 	'varc'

#define kControlContentTextOnly 		0
#define kControlNoContent 				0
#define kControlContentIconSuiteRes 	1
#define kControlContentCIconRes 		2
#define kControlContentPictRes 			3
#define kControlContentICONRes 			4
#define kControlContentAlertIconRes 	5
#define kControlContentIconSuiteHandle 	129
#define kControlContentCIconHandle 		130
#define kControlContentPictHandle 		131
#define kControlContentIconRef 			132
#define kControlContentICON 			133
#define kControlContentCGImageRef 		134

#define kControlKeyScriptBehaviorAllowAnyScript  'any '		/*  leaves the current keyboard alone and allows user to change the keyboard. */
#define kControlKeyScriptBehaviorPrefersRoman  'prmn'		/*  switches the keyboard to roman, but allows them to change it as desired. */
#define kControlKeyScriptBehaviorRequiresRoman  'rrmn'		/*  switches the keyboard to roman and prevents the user from changing it. */

#define kControlFontBigSystemFont 		(-1)				/*  force to big system font */
#define kControlFontSmallSystemFont 	(-2)				/*  force to small system font */
#define kControlFontSmallBoldSystemFont  (-3)				/*  force to small bold system font */
#define kControlFontViewSystemFont 		(-4)				/*  force to views system font (DataBrowser control only) */
#define kControlFontMiniSystemFont 		(-5)				/*  force to mini system font */

#define kControlUseFontMask 			0x0001
#define kControlUseFaceMask 			0x0002
#define kControlUseSizeMask 			0x0004
#define kControlUseForeColorMask 		0x0008
#define kControlUseBackColorMask 		0x0010
#define kControlUseModeMask 			0x0020
#define kControlUseJustMask 			0x0040
#define kControlUseAllMask 				0x00FF
#define kControlAddFontSizeMask 		0x0100

#define kControlAddToMetaFontMask 		0x0200				/*  Available in Appearance 1.1 or later */
#define kControlUseThemeFontIDMask 		0x0080				/*  Available in Mac OS X or later */
#define kDoNotActivateAndIgnoreClick 	0					/*  probably never used. here for completeness. */
#define kDoNotActivateAndHandleClick 	1					/*  let the control handle the click while the window is still in the background. */
#define kActivateAndIgnoreClick 		2					/*  control doesn't want to respond directly to the click, but window should still be brought forward. */
#define kActivateAndHandleClick 		3					/*  control wants to respond to the click, but only after the window has been activated. */

#define kControlFontStyleTag 			'font'
#define kControlKeyFilterTag 			'fltr'
#define kControlKindTag 				'kind'
#define kControlSizeTag 				'size'
#define kControlContentTag 				'cont'
#define kControlThemeTextFontTag 		'thft'
#define kControlThemeTextHorizontalFlushTag  'thhf'
#define kControlThemeTextVerticalFlushTag  'thvf'
#define kControlThemeTextTruncationTag 	'thtt'
#define kControlThemeTextInfoTag 		'thti'

#define kControlSupportsGhosting 		0x01
#define kControlSupportsEmbedding 		0x02
#define kControlSupportsFocus 			0x04
#define kControlWantsIdle 				0x08
#define kControlWantsActivate 			0x10
#define kControlHandlesTracking 		0x20
#define kControlSupportsDataAccess 		0x40
#define kControlHasSpecialBackground 	0x80
#define kControlGetsFocusOnClick 		0x0100
#define kControlSupportsCalcBestRect 	0x0200
#define kControlSupportsLiveFeedback 	0x0400
#define kControlHasRadioBehavior 		0x0800				/*  Available in Appearance 1.0.1 or later */
#define kControlSupportsDragAndDrop 	0x1000				/*  Available in Carbon */
#define kControlAutoToggles 			0x4000				/*  Available in Appearance 1.1 or later */
#define kControlSupportsGetRegion 		0x00020000			/*  Available in Appearance 1.1 or later */
#define kControlSupportsFlattening 		0x00080000			/*  Available in Carbon */
#define kControlSupportsSetCursor 		0x00100000			/*  Available in Carbon */
#define kControlSupportsContextualMenus  0x00200000			/*  Available in Carbon */
#define kControlSupportsClickActivation  0x00400000			/*  Available in Carbon */
#define kControlIdlesWithTimer 			0x00800000			/*  Available in Carbon - this bit indicates that the control animates automatically */
#define kControlInvertsUpDownValueMeaning  0x01000000

#if !__LP64__
#define drawCntl 						0
#define testCntl 						1
#define calcCRgns 						2
#define initCntl 						3					/*  Param is Collection, result is OSStatus */
#define dispCntl 						4
#define posCntl 						5
#define thumbCntl 						6
#define dragCntl 						7
#define autoTrack 						8
#define calcCntlRgn 					10
#define calcThumbRgn 					11
#define drawThumbOutline 				12
#define kControlMsgDrawGhost 			13
#define kControlMsgCalcBestRect 		14					/*  Calculate best fitting rectangle for control */
#define kControlMsgHandleTracking 		15
#define kControlMsgFocus 				16					/*  param indicates action. */
#define kControlMsgKeyDown 				17
#define kControlMsgIdle 				18
#define kControlMsgGetFeatures 			19
#define kControlMsgSetData 				20
#define kControlMsgGetData 				21
#define kControlMsgActivate 			22
#define kControlMsgSetUpBackground 		23
#define kControlMsgCalcValueFromPos 	26
#define kControlMsgTestNewMsgSupport 	27					/*  See if this control supports new messaging */
#define kControlMsgSubValueChanged 		25					/*  Available in Appearance 1.0.1 or later */
#define kControlMsgSubControlAdded 		28					/*  Available in Appearance 1.0.1 or later */
#define kControlMsgSubControlRemoved 	29					/*  Available in Appearance 1.0.1 or later */
#define kControlMsgApplyTextColor 		30					/*  Available in Appearance 1.1 or later */
#define kControlMsgGetRegion 			31					/*  Available in Appearance 1.1 or later */
#define kControlMsgFlatten 				32					/*  Available in Carbon. Param is Collection. */
#define kControlMsgSetCursor 			33					/*  Available in Carbon. Param is ControlSetCursorRec */
#define kControlMsgDragEnter 			38					/*  Available in Carbon. Param is DragRef, result is boolean indicating acceptibility of drag. */
#define kControlMsgDragLeave 			39					/*  Available in Carbon. As above. */
#define kControlMsgDragWithin 			40					/*  Available in Carbon. As above. */
#define kControlMsgDragReceive 			41					/*  Available in Carbon. Param is DragRef, result is OSStatus indicating success/failure. */
#define kControlMsgDisplayDebugInfo 	46					/*  Available in Carbon on X. */
#define kControlMsgContextualMenuClick 	47					/*  Available in Carbon. Param is ControlContextualMenuClickRec */
#define kControlMsgGetClickActivation 	48					/*  Available in Carbon. Param is ControlClickActivationRec */

#endif  /* !__LP64__ */

#define kControlSizeNormal 				0
#define kControlSizeSmall 				1
#define kControlSizeLarge 				2
#define kControlSizeMini 				3
#define kControlSizeAuto 				0xFFFF

#define kDrawControlEntireControl 		0
#define kDrawControlIndicatorOnly 		129

#if !__LP64__
#define kDragControlEntireControl 		0
#define kDragControlIndicator 			1

#endif  /* !__LP64__ */

#if !__LP64__
#define kControlSupportsNewMessages 	' ok '
#endif  /* !__LP64__ */

#define kControlKeyFilterBlockKey 		0
#define kControlKeyFilterPassKey 		1

#define kControlKindSignatureApple 		'appl'
#if !__LP64__

/*--------------------------cctb ¥ Control Color old Lookup Table----------------------*/
#ifdef oldTemp
  type 'cctb' {
          unsigned hex longint;                                   /* CCSeed               */
         integer;                                                /* ccReserved           */
         integer = $$Countof(ColorSpec) - 1;                     /* ctSize               */
         wide array ColorSpec {
                 integer     cFrameColor,                        /* partcode             */
                             cBodyColor,
                                cTextColor,
                                cElevatorColor;
                    unsigned integer;                               /* RGB: red             */
                 unsigned integer;                               /*      green           */
                 unsigned integer;                               /*      blue            */
         };
 };
#else
   type 'cctb' {
          unsigned hex longint = 0;                               /* CCSeed               */
         integer = 0;                                            /* ccReserved           */
         integer = $$Countof(ColorSpec) - 1;                     /* ctSize               */
         wide array ColorSpec {
                 integer     cFrameColor,                        /* partcode             */
                             cBodyColor,
                                cTextColor,
                                cElevatorColor,
                                cFillPatColor,
                             cArrowsLight,
                              cArrowsDark,
                               cThumbLight,
                               cThumbDark,
                                cHiliteLight,
                              cHiliteDark,
                               cTitleBarLight,
                                cTitleBarDark,
                             cTingeLight,
                               cTingeDark;
                    unsigned integer;                               /* RGB: red             */
                 unsigned integer;                               /*      green           */
                 unsigned integer;                               /*      blue            */
         };
 };
#endif


/*----------------------------CNTL ¥ Control Template-----------------------------------*/
type 'CNTL' {
      rect;                                                   /* Bounds               */
     integer;                                                /* Value                */
     byte            invisible, visible;                     /* visible              */
     fill byte;
     integer;                                                /* Max                  */
     integer;                                                /* Min                  */
     integer         pushButProc,                            /* ProcID               */
                     checkBoxProc,
                      radioButProc,
                      pushButProcUseWFont = 8,
                       checkBoxProcUseWFont,
                      radioButProcUseWFont,
                      scrollBarProc = 16;
        longint;                                                /* RefCon               */
     pstring;                                                /* Title                */
};

#define  popupMenuCDEFproc       1008                            /* ProcID 1008 = 16 * 63        */

#endif  /* !__LP64__ */


#endif /* __CONTROLS_R__ */

