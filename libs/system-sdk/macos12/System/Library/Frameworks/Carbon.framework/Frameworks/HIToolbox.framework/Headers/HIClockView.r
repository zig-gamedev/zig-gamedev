/*
     File:       HIClockView.r
 
     Contains:   Definition of the clock view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HICLOCKVIEW_R__
#define __HICLOCKVIEW_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlClockTimeProc 			240
#define kControlClockTimeSecondsProc 	241
#define kControlClockDateProc 			242
#define kControlClockMonthYearProc 		243

#define kControlClockTypeHourMinute 	0
#define kControlClockTypeHourMinuteSecond  1
#define kControlClockTypeMonthDayYear 	2
#define kControlClockTypeMonthYear 		3

#define kControlClockFlagStandard 		0					/*  editable, non-live */
#define kControlClockNoFlags 			0
#define kControlClockFlagDisplayOnly 	1					/*  add this to become non-editable */
#define kControlClockIsDisplayOnly 		1
#define kControlClockFlagLive 			2					/*  automatically shows current time on idle. only valid with display only. */
#define kControlClockIsLive 			2

#define kControlKindClock 				'clck'
#define kControlClockLongDateTag 		'date'				/*  LongDateRec, 32-bit only */
#define kControlClockAbsoluteTimeTag 	'abst'				/*  CFAbsoluteTime; available in Leopard and later */
#define kControlClockFontStyleTag 		'font'				/*  ControlFontStyleRec */
#define kControlClockAnimatingTag 		'anim'				/*  Boolean */


#endif /* __HICLOCKVIEW_R__ */

