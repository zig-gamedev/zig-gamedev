/*
     File:       HITabbedView.r
 
     Contains:   Definition of the tab view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HITABBEDVIEW_R__
#define __HITABBEDVIEW_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlTabLargeProc 			128					/*  Large tab size, north facing    */
#define kControlTabSmallProc 			129					/*  Small tab size, north facing    */
#define kControlTabLargeNorthProc 		128					/*  Large tab size, north facing    */
#define kControlTabSmallNorthProc 		129					/*  Small tab size, north facing    */
#define kControlTabLargeSouthProc 		130					/*  Large tab size, south facing    */
#define kControlTabSmallSouthProc 		131					/*  Small tab size, south facing    */
#define kControlTabLargeEastProc 		132					/*  Large tab size, east facing     */
#define kControlTabSmallEastProc 		133					/*  Small tab size, east facing     */
#define kControlTabLargeWestProc 		134					/*  Large tab size, west facing     */
#define kControlTabSmallWestProc 		135					/*  Small tab size, west facing     */

#define kControlTabDirectionNorth 		0
#define kControlTabDirectionSouth 		1
#define kControlTabDirectionEast 		2
#define kControlTabDirectionWest 		3

#define kControlKindTabs 				'tabs'
#define kControlTabContentRectTag 		'rect'
#define kControlTabEnabledFlagTag 		'enab'
#define kControlTabFontStyleTag 		'font'
#define kControlTabInfoTag 				'tabi'
#define kControlTabImageContentTag 		'cont'

#define kControlTabInfoVersionZero 		0					/*  ControlTabInfoRec */
#define kControlTabInfoVersionOne 		1					/*  ControlTabInfoRecV1 */

#define kControlTabListResType 			'tab#'				/*  used for tab control (Appearance 1.0 and later) */

/*-------------------------------tab# ¥ Tab Control Template-----------------------------*/
type 'tab#'
{
 switch
 {
      case versionZero:
          key integer = 0;    /* version */

         integer = $$Countof(TabInfo);
          array TabInfo
          {
              integer;                                            /* Icon Suite ID        */
             pstring;                                            /* Tab Name             */
             fill long;                                          /* Reserved             */
             fill word;                                          /* Reserved             */
         };
 };
};


#endif /* __HITABBEDVIEW_R__ */

