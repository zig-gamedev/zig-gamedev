/*
     File:       HIImageViews.r
 
     Contains:   API and type definitions related to views that display image content.
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HIIMAGEVIEWS_R__
#define __HIIMAGEVIEWS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlIconProc 				320
#define kControlIconNoTrackProc 		321					/*  immediately returns kControlIconPart */
#define kControlIconSuiteProc 			322
#define kControlIconSuiteNoTrackProc 	323					/*  immediately returns kControlIconPart */

															/*  icon ref controls may have either an icon, color icon, icon suite, or icon ref. */
															/*  for data other than icon, you must set the data by passing a */
															/*  ControlButtonContentInfo to SetControlData */
#define kControlIconRefProc 			324
#define kControlIconRefNoTrackProc 		325					/*  immediately returns kControlIconPart */

#define kControlKindIcon 				'icon'
#define kControlIconTransformTag 		'trfm'				/*  IconTransformType */
#define kControlIconAlignmentTag 		'algn'				/*  IconAlignmentType */

#define kControlIconResourceIDTag 		'ires'				/*  SInt16 resource ID of icon to use */
#define kControlIconContentTag 			'cont'				/*  ControlImageContentInfo */

#define kControlImageWellProc 			176
#define kControlKindImageWell 			'well'
#define kControlImageWellContentTag 	'cont'				/*  ControlImageContentInfo */
#define kControlImageWellTransformTag 	'tran'				/*  IconTransformType */
#define kControlImageWellIsDragDestinationTag  'drag'		/*  Boolean */


#endif /* __HIIMAGEVIEWS_R__ */

