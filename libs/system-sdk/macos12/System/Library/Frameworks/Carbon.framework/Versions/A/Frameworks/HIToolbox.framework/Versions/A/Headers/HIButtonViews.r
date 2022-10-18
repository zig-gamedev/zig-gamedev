/*
     File:       HIButtonViews.r
 
     Contains:   Definitions of the button views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HIBUTTONVIEWS_R__
#define __HIBUTTONVIEWS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlPushButtonProc 			368
#define kControlPushButLeftIconProc 	374					/*  Standard pushbutton with left-side icon */
#define kControlPushButRightIconProc 	375					/*  Standard pushbutton with right-side icon */

#define kControlPushButtonIconOnLeft 	6
#define kControlPushButtonIconOnRight 	7

#define kControlKindPushButton 			'push'
#define kControlKindPushIconButton 		'picn'

#define kControlPushButtonDefaultTag 	'dflt'
#define kControlPushButtonCancelTag 	'cncl'
#define kControlPushButtonContentTag 	'cont'
#define kControlPushButtonIconAlignmentTag  'cpia'
#define kControlPushButtonAnimatingTag 	'anim'

#define kControlPushButtonIsTexturedTag  'metl'
#define kControlCheckBoxProc 			369
#define kControlCheckBoxAutoToggleProc 	371
#define kControlKindCheckBox 			'cbox'
#define kControlCheckBoxUncheckedValue 	0
#define kControlCheckBoxCheckedValue 	1
#define kControlCheckBoxMixedValue 		2

#define kControlRadioButtonProc 		370
#define kControlRadioButtonAutoToggleProc  372
#define kControlRadioButtonUncheckedValue  0
#define kControlRadioButtonCheckedValue  1
#define kControlRadioButtonMixedValue 	2

#define kControlRadioGroupProc 			416
#define kControlKindRadioGroup 			'rgrp'
#define kControlBevelButtonSmallBevelProc  32
#define kControlBevelButtonNormalBevelProc  33
#define kControlBevelButtonLargeBevelProc  34

#define kControlBevelButtonSmallBevelVariant  0
#define kControlBevelButtonNormalBevelVariant  0x01
#define kControlBevelButtonLargeBevelVariant  0x02
#define kControlBevelButtonMenuOnRightVariant  0x04

#define kControlBevelButtonSmallBevel 	0
#define kControlBevelButtonNormalBevel 	1
#define kControlBevelButtonLargeBevel 	2

#define kControlBehaviorPushbutton 		0
#define kControlBehaviorToggles 		0x0100
#define kControlBehaviorSticky 			0x0200
#define kControlBehaviorOffsetContents 	0x8000
#define kControlBehaviorSingleValueMenu  0
#define kControlBehaviorMultiValueMenu 	0x4000				/*  only makes sense when a menu is attached. */

#define kControlBehaviorCommandMenu 	0x2000				/*  menu holds commands, not choices. Overrides multi-value bit. */
#define kControlBevelButtonMenuOnBottom  0
#define kControlBevelButtonMenuOnRight 	0x04

#define kControlKindBevelButton 		'bevl'
#define kControlBevelButtonAlignSysDirection  (-1)			/*  only left or right */
#define kControlBevelButtonAlignCenter 	0
#define kControlBevelButtonAlignLeft 	1
#define kControlBevelButtonAlignRight 	2
#define kControlBevelButtonAlignTop 	3
#define kControlBevelButtonAlignBottom 	4
#define kControlBevelButtonAlignTopLeft  5
#define kControlBevelButtonAlignBottomLeft  6
#define kControlBevelButtonAlignTopRight  7
#define kControlBevelButtonAlignBottomRight  8

#define kControlBevelButtonAlignTextSysDirection  0
#define kControlBevelButtonAlignTextCenter  1
#define kControlBevelButtonAlignTextFlushRight  (-1)
#define kControlBevelButtonAlignTextFlushLeft  (-2)

#define kControlBevelButtonPlaceSysDirection  (-1)			/*  if graphic on right, then on left */
#define kControlBevelButtonPlaceNormally  0
#define kControlBevelButtonPlaceToRightOfGraphic  1
#define kControlBevelButtonPlaceToLeftOfGraphic  2
#define kControlBevelButtonPlaceBelowGraphic  3
#define kControlBevelButtonPlaceAboveGraphic  4

#define kControlBevelButtonContentTag 	'cont'				/*  ControlImageContentInfo */
#define kControlBevelButtonTransformTag  'tran'				/*  IconTransformType */
#define kControlBevelButtonTextAlignTag  'tali'				/*  ButtonTextAlignment */
#define kControlBevelButtonTextOffsetTag  'toff'			/*  SInt16 */
#define kControlBevelButtonGraphicAlignTag  'gali'			/*  ButtonGraphicAlignment */
#define kControlBevelButtonGraphicOffsetTag  'goff'			/*  Point */
#define kControlBevelButtonTextPlaceTag  'tplc'				/*  ButtonTextPlacement */
#define kControlBevelButtonMenuValueTag  'mval'				/*  SInt16 */
#define kControlBevelButtonMenuHandleTag  'mhnd'			/*  MenuRef */
#define kControlBevelButtonMenuRefTag 	'mhnd'				/*  MenuRef */
#define kControlBevelButtonCenterPopupGlyphTag  'pglc'		/*  Boolean: true = center, false = bottom right */

#define kControlBevelButtonLastMenuTag 	'lmnu'				/*  SInt16: menuID of last menu item selected from */
#define kControlBevelButtonMenuDelayTag  'mdly'				/*  SInt32: ticks to delay before menu appears */

															/*  Boolean: True = if an icon of the ideal size for */
															/*  the button isn't available, scale a larger or */
															/*  smaller icon to the ideal size. False = don't */
															/*  scale; draw a smaller icon or clip a larger icon. */
															/*  Default is false. Only applies to IconSuites and */
#define kControlBevelButtonScaleIconTag  'scal'				/*  IconRefs. */
#define kControlBevelButtonOwnedMenuRefTag  'omrf'			/*  MenuRef (control will dispose) */
#define kControlBevelButtonKindTag 		'bebk'				/*  ThemeButtonKind ( kTheme[Small,Medium,Large,Rounded]BevelButton ) */

#define kControlBevelButtonIsMultiValueMenuTag  'mult'
#define kControlRoundButtonNormalSize 	0
#define kControlRoundButtonLargeSize 	2

#define kControlRoundButtonContentTag 	'cont'				/*  ControlImageContentInfo */
#define kControlRoundButtonSizeTag 		'size'				/*  ControlRoundButtonSize */

#define kControlKindRoundButton 		'rndb'

#endif /* __HIBUTTONVIEWS_R__ */

