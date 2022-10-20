/*
     File:       HIContainerViews.r
 
     Contains:   Definition of the container views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HICONTAINERVIEWS_R__
#define __HICONTAINERVIEWS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlGroupBoxTextTitleProc 	160
#define kControlGroupBoxCheckBoxProc 	161
#define kControlGroupBoxPopupButtonProc  162
#define kControlGroupBoxSecondaryTextTitleProc  164
#define kControlGroupBoxSecondaryCheckBoxProc  165
#define kControlGroupBoxSecondaryPopupButtonProc  166

#define kControlKindGroupBox 			'grpb'
#define kControlKindCheckGroupBox 		'cgrp'
#define kControlKindPopupGroupBox 		'pgrp'

#define kControlGroupBoxMenuHandleTag 	'mhan'				/*  MenuRef (popup title only) */
#define kControlGroupBoxMenuRefTag 		'mhan'				/*  MenuRef (popup title only) */
#define kControlGroupBoxFontStyleTag 	'font'				/*  ControlFontStyleRec */

#define kControlGroupBoxTitleRectTag 	'trec'				/*  Rect. Rectangle that the title text/control is drawn in. (get only) */
#define kControlGroupBoxFrameRectTag 	'frec'
#define kControlPlacardProc 			224
#define kControlKindPlacard 			'plac'
#define kControlWindowHeaderProc 		336					/*  normal header */
#define kControlWindowListViewHeaderProc  337				/*  variant for list views - no bottom line */

#define kControlKindWindowHeader 		'whed'
#define kControlWindowHeaderIsListHeaderTag  'islh'
#define kControlUserPaneProc 			256
#define kControlKindUserPane 			'upan'
#define kControlUserItemDrawProcTag 	'uidp'				/*  UserItemUPP */
#define kControlUserPaneDrawProcTag 	'draw'				/*  ControlUserPaneDrawUPP */
#define kControlUserPaneHitTestProcTag 	'hitt'				/*  ControlUserPaneHitTestUPP */
#define kControlUserPaneTrackingProcTag  'trak'				/*  ControlUserPaneTrackingUPP */
#define kControlUserPaneIdleProcTag 	'idle'				/*  ControlUserPaneIdleUPP */
#define kControlUserPaneKeyDownProcTag 	'keyd'				/*  ControlUserPaneKeyDownUPP */
#define kControlUserPaneActivateProcTag  'acti'				/*  ControlUserPaneActivateUPP */
#define kControlUserPaneFocusProcTag 	'foci'				/*  ControlUserPaneFocusUPP */
#define kControlUserPaneBackgroundProcTag  'back'			/*  ControlUserPaneBackgroundUPP (32-bit only) */


#endif /* __HICONTAINERVIEWS_R__ */

