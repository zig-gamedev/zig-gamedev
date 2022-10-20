/*
     File:       HIToolbox/HIMenuView.h
 
     Contains:   Definition of the base and standard menu views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIMENUVIEW__
#define __HIMENUVIEW__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __HIVIEW__
#include <HIToolbox/HIView.h>
#endif

#ifndef __MENUS__
#include <HIToolbox/Menus.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  HIMenuView.h
 *  
 *  Discussion:
 *    API definitions for the base and standard menu views.
 */
/*==============================================================================*/
/*  HIMenuView                                                                  */
/*                                                                              */
/*  HIMenuView and HIStandardMenuView are new views available in Mac OS X 10.3. */
/*  HIMenuView is intended for use as a base class for custom menu item views;  */
/*  it does not draw or handle events itself, but provides useful functionality */
/*  that all menu views need to implement. HIStandardMenuView is the standard   */
/*  HIView used by the Menu Manager to draw menu item content, beginning with   */
/*  Mac OS X 10.3. It can also be subclassed by custom menu item views.         */
/*  Both of these views are meant to be used only in compositing windows.       */
/*                                                                              */
/*  Because HIMenuView and HIStandardMenuView are not typically created         */
/*  directly by applications, no API is provided to create instances of these   */
/*  views. If you need to create an instance of either view, you can use        */
/*  HIObjectCreate.                                                             */
/*==============================================================================*/
/* the HIObject class ID for the HIMenuView class*/
#define kHIMenuViewClassID              CFSTR("com.apple.HIMenuView")
/* the HIObject class ID for the standard menu HIView class*/
#define kHIStandardMenuViewClassID      CFSTR("com.apple.HIStandardMenuView")
/* Control Kinds (only used in Mac OS X 10.4 and later)*/
enum {
  kControlKindHIMenuView        = 'menu',
  kControlKindHIStandardMenuView = 'smnu'
};

/*
    The kEventHIObjectInitialize event for HIMenuView and HIStandardMenuView is expected to contain
    the following parameters. Be sure to include these parameters in the init event if you create an
    instance of these views with HIObjectCreate.
    
    --> kEventParamMenuRef (in, typeMenuRef)
            The menu that the view should draw.
*/
/*
 *  kHIViewMenuContentID
 *  
 *  Summary:
 *    The HIViewID for the menu content view. The Menu Manager assigns
 *    this view ID to all menu content views.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewMenuContentID                           AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;
#if !__LP64__
/*
 *  HIMenuViewGetMenu()
 *  
 *  Summary:
 *    Returns the MenuRef that is associated with an HIView that is a
 *    subclass of HIMenuView.
 *  
 *  Discussion:
 *    An HIMenuView subclass might use this API to determine the menu
 *    that it should draw. Prior to Mac OS X 10.5, this API returns
 *    NULL if passed an instance of the standard menu view. In Mac OS X
 *    10.5 and later, this API correctly returns the owning menu of an
 *    instance of the standard menu view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The view whose menu to return.
 *  
 *  Result:
 *    The MenuRef associated with the HIView, or NULL if an HIView is
 *    passed that is not a subclass of HIMenuView.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern MenuRef 
HIMenuViewGetMenu(HIViewRef inView)                           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HIMENUVIEW__ */

