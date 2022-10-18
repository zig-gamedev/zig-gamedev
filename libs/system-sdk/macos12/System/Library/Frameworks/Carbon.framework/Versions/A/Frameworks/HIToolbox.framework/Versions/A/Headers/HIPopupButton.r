/*
     File:       HIPopupButton.r
 
     Contains:   Definitions of the popup button and popup arrow views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HIPOPUPBUTTON_R__
#define __HIPOPUPBUTTON_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlPopupButtonProc 		400
#define kControlPopupFixedWidthVariant 	0x01
#define kControlPopupVariableWidthVariant  0x02
#define kControlPopupUseAddResMenuVariant  0x04
#define kControlPopupUseWFontVariant 	0x08

#define kControlKindPopupButton 		'popb'
#define kControlPopupButtonMenuHandleTag  'mhan'			/*  MenuRef */
#define kControlPopupButtonMenuRefTag 	'mhan'				/*  MenuRef */
#define kControlPopupButtonMenuIDTag 	'mnid'				/*  SInt16 */

#define kControlPopupButtonExtraHeightTag  'exht'			/*  SInt16 - extra vertical whitespace within the button */
#define kControlPopupButtonOwnedMenuRefTag  'omrf'			/*  MenuRef */

#define kControlPopupButtonCheckCurrentTag  'chck'			/*  Boolean    - whether the popup puts a checkmark next to the current item (defaults to true) */
#define kControlPopupArrowEastProc 		192
#define kControlPopupArrowWestProc 		193
#define kControlPopupArrowNorthProc 	194
#define kControlPopupArrowSouthProc 	195
#define kControlPopupArrowSmallEastProc  196
#define kControlPopupArrowSmallWestProc  197
#define kControlPopupArrowSmallNorthProc  198
#define kControlPopupArrowSmallSouthProc  199

#define kControlPopupArrowOrientationEast  0
#define kControlPopupArrowOrientationWest  1
#define kControlPopupArrowOrientationNorth  2
#define kControlPopupArrowOrientationSouth  3

#define kControlPopupArrowSizeNormal 	0
#define kControlPopupArrowSizeSmall 	1

#define kControlKindPopupArrow 			'parr'
#define popupFixedWidth 				0x01
#define popupVariableWidth 				0x02
#define popupUseAddResMenu 				0x04
#define popupUseWFont 					0x08

#define popupTitleBold 					0x0100
#define popupTitleItalic 				0x0200
#define popupTitleUnderline 			0x0400
#define popupTitleOutline 				0x0800
#define popupTitleShadow 				0x1000
#define popupTitleCondense 				0x2000
#define popupTitleExtend 				0x4000
#define popupTitleNoStyle 				0x8000

#define popupTitleLeftJust 				0x00000000
#define popupTitleCenterJust 			0x00000001
#define popupTitleRightJust 			0x000000FF


#endif /* __HIPOPUPBUTTON_R__ */

