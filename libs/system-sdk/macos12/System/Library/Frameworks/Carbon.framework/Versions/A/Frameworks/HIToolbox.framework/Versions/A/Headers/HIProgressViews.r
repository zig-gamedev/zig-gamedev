/*
     File:       HIProgressViews.r
 
     Contains:   Definition of the progress-display views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HIPROGRESSVIEWS_R__
#define __HIPROGRESSVIEWS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlProgressBarProc 		80
#define kControlRelevanceBarProc 		81

#define kControlKindProgressBar 		'prgb'
#define kControlKindRelevanceBar 		'relb'

#define kControlProgressBarIndeterminateTag  'inde'			/*  Boolean */
#define kControlProgressBarAnimatingTag  'anim'				/*  Boolean */

#define kControlChasingArrowsProc 		112
#define kControlKindChasingArrows 		'carr'
#define kControlChasingArrowsAnimatingTag  'anim'			/*  Boolean */

#endif /* __HIPROGRESSVIEWS_R__ */

