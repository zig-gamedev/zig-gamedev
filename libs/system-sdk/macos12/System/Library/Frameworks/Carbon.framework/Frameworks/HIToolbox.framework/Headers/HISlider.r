/*
     File:       HISlider.r
 
     Contains:   API and type definitions related to HISlider.
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HISLIDER_R__
#define __HISLIDER_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlSliderProc 				48
#define kControlSliderLiveFeedback 		0x01
#define kControlSliderHasTickMarks 		0x02
#define kControlSliderReverseDirection 	0x04
#define kControlSliderNonDirectional 	0x08

#define kControlSliderPointsDownOrRight  0
#define kControlSliderPointsUpOrLeft 	1
#define kControlSliderDoesNotPoint 		2

#define kControlKindSlider 				'sldr'

#endif /* __HISLIDER_R__ */

