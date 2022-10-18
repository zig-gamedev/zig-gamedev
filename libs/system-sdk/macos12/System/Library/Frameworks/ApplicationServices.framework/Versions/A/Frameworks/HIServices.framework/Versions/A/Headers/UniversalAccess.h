/*
     File:       HIServices/UniversalAccess.h
 
     Contains:   Universal Access Interfaces.
 
     Copyright:  � 2005-2012 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __UNIVERSALACCESS__
#define __UNIVERSALACCESS__

#ifndef __COREGRAPHICS__
#include <CoreGraphics/CoreGraphics.h>
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  Universal Access
 *  
 *  Discussion:
 *    Universal Access provides apps the ability to control the zoom
 *    focus. The following functions are provided so that applications
 *    can tell Universal Access what part of the UI needs focus.
 */

/*!
	@header UniversalAccess.h
	@discussion This header file contains functions that give applications the ability to control the zoom focus. Using these functions,
	an application can tell the Mac OS X Universal Access zoom feature what part of its user interface needs focus.
	@indexgroup Universal Access
*/

/*
 *  UAZoomChangeFocusType
 *  
 *  Summary:
 *    Universal Access Zoom Change Focus Types
 *  
 *  Discussion:
 *    The following constants are used to tell Universal Access Zoom
 *    the type of event that is driving the change in the zoom focus.
 */
 
/*!
	@typedef UAZoomChangeFocusType
	@abstract Defines the Universal Access zoom change focus type.
*/
typedef UInt32 UAZoomChangeFocusType;

/*!
	@enum UAZoomFocusTypes
	@abstract Values that tell the Universal Access zoom feature what type of event is causing the change in zoom focus.
*/

enum {
  /*!
   * An event is requesting focus.
   */
  kUAZoomFocusTypeOther         = 0,
  /*!
   * The text insertion point has moved.
   */
  kUAZoomFocusTypeInsertionPoint = 1
};


/*
 *  UAZoomEnabled()
 *  
 *  Summary:
 *    Determine if Universal Access Zoom is enabled.
 *  
 *  Discussion:
 *    This queries the state of Universal Access Zoom
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Result:
 *    TRUE if Universal Access Zoom is on, FALSE if Zoom is off or the
 *    user has zoomed all the way out.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
 
/*!
	@function UAZoomEnabled
	@abstract Determines if the Universal Access zoom feature is enabled.
	@result Returns <code>true</code> if the Universal Access zoom feature is on, <code>false</code> if the zoom feature is off or if the user
	has zoomed all the way out.
	@availability Available in Mac OS X v10.4 and later (not available in CarbonLib 1.x and not available for nonCarbon CFM).
*/
extern Boolean 
UAZoomEnabled(void)                                           AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;



/*
 *  UAZoomChangeFocus()
 *  
 *  Summary:
 *    Tell Universal Access where Zoom should focus.
 *  
 *  Discussion:
 *    Tells Universal Access the frame of the element in focus and the
 *    part of the element that should be in focus.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      - The frame of the element in focus in global 72dpi coordinates.
 *    
 *    inHighlightRect:
 *      - The frame of highlighted part of the element in focus in
 *      global 72dpi coordinates.  If the whole element is in focus,
 *      and not just a smaller part of it, pass the inRect parameter
 *      and pass NULL for inHighlightRect.
 *    
 *    inType:
 *      - Universal Access Zoom change focus type.
 *  
 *  Result:
 *    OSStatus - noErr if there were no problems or Universal Access
 *    Zoom is off or zoomed all the way out.  paramErr if inRect is
 *    NULL or inType is out of range.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
 
/*!
	@function UAZoomChangeFocus
	@abstract Tells the Universal Access zoom feature where it should focus.
	@discussion This function tells Universal Access the frame of the element in focus and the
	part of the element that should be in focus.
	@param inRect The frame of the element in focus, in global 72-dot-per-inch (dpi) coordinates.
	@param inHighlightRect The frame of the highlighted part of the element in focus, in global 72 dpi coordinates.  If the whole element is in focus,
	and not just a smaller part of it, pass the <code>inRect</code> parameter and pass <code>NULL</code> for <code>inHighlightRect</code>.
	@param inType A value of type @link UAZoomChangeFocusType UAZoomChangeFocusType@/link.
	@result Returns <code>noErr</code> if there were no problems, if Universal Access Zoom is zoomed all the way out, or if the feature is off;
	 returns <code>paramErr</code> if <code>inRect</code> is <code>NULL</code> or if <code>inType</code> is out of range. 
	@availability Available in Mac OS X v10.4 and later (not available in CarbonLib 1.x and not available for nonCarbon CFM).
*/ 
extern OSStatus 
UAZoomChangeFocus(
  const CGRect *          inRect,
  const CGRect *          inHighlightRect,
  UAZoomChangeFocusType   inType)                             AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;



#ifdef __cplusplus
}
#endif

#endif /* __UNIVERSALACCESS__ */

