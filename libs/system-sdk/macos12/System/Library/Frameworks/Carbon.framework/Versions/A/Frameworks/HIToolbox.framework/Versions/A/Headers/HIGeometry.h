/*
     File:       HIToolbox/HIGeometry.h
 
     Contains:   HIToolbox interfaces for geometry
 
     Copyright:  © 1984-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIGEOMETRY__
#define __HIGEOMETRY__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)


/*
 *  HIPoint
 *  
 *  Discussion:
 *    HIPoint is a new, floating point-based type to help express
 *    coordinates in a much richer fashion than the classic QuickDraw
 *    points. It will, in time, be more heavily used throughout the
 *    Toolbox. For now, it is replacing our use of typeQDPoint in mouse
 *    events. This is to better support sub-pixel tablet coordinates.
 *    If you ask for a mouse location with typeQDPoint, and the point
 *    is actually stored as typeHIPoint, it will automatically be
 *    coerced to typeQDPoint for you, so this change should be largely
 *    transparent to applications. HIPoints are in screen space, i.e.
 *    the top left of the screen is 0, 0.
 */
typedef CGPoint                         HIPoint;

/*
 *  HISize
 *  
 *  Discussion:
 *    HISize is a floating point-based type to help express dimensions
 *    in a much richer fashion than the classic QuickDraw coordinates.
 */
typedef CGSize                          HISize;

/*
 *  HIRect
 *  
 *  Discussion:
 *    HIRect is a new, floating point-based type to help express
 *    rectangles in a much richer fashion than the classic QuickDraw
 *    rects. It will, in time, be more heavily used throughout the
 *    Toolbox. HIRects are in screen space, i.e. the top left of the
 *    screen is 0, 0.
 */
typedef CGRect                          HIRect;
#if !__LP64__
/*
 *  HIGetScaleFactor()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    This function is deprecated and should not be used by
 *    applications targeting Mac OS X 10.7 or later. Please use an
 *    appropriate AppKit API instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    This function always returns 1.0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only] but deprecated in 10.7
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern CGFloat 
HIGetScaleFactor(void)                                        AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;



#endif  /* !__LP64__ */


/*
 *  HICoordinateSpace
 *  
 *  Summary:
 *    Coordinate spaces for use with HIPointConvert, HIRectConvert, and
 *    HISizeConvert.
 */
typedef UInt32 HICoordinateSpace;
enum {

  /*
   * The coordinate space defined by the position and sizes of the
   * screen GDevices. More correctly, this is a 72 DPI coordinate space
   * covering the screens. When the scale factor is not 1.0, this is
   * the compatibility notion of global coordinates. When the scale
   * factor is 1.0, this and kHICoordSpaceScreenPixel are the same.
   */
  kHICoordSpace72DPIGlobal      = 1,

  /*
   * The coordinate space defined by physical screen pixels. When the
   * scale factor is 1.0, this and kHICoordSpace72DPIGlobal are the
   * same.
   */
  kHICoordSpaceScreenPixel      = 2,

  /*
   * The coordinate space of a specified WindowRef, with { 0, 0 } at
   * the top left of the window's structure. When this is passed to a
   * conversion routine as a source or destination coordinate space,
   * you must also pass a WindowRef as a source or destination object.
   */
  kHICoordSpaceWindow           = 3,

  /*
   * The coordinate space of a given HIViewRef, with { 0, 0 } at the
   * top left of the view unless changed by HIViewSetBoundsOrigin. When
   * this is passed to a conversion routine as a source or destination
   * coordinate space, you must also pass an HIViewRef as a source or
   * destination object.
   */
  kHICoordSpaceView             = 4
};

/*
 *  HIPointConvert()
 *  
 *  Discussion:
 *    This routine converts an HIPoint from one coordinate space to
 *    another. It takes into account the resolution-independent display
 *    scale factor as appropriate.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    ioPoint:
 *      The HIPoint to convert.
 *    
 *    inSourceSpace:
 *      The HICoordinateSpace constant specifying the source coordinate
 *      space that the point is to be converted from. Some coordinate
 *      spaces require the caller to pass extra information in the
 *      inSourceObject parameter.
 *    
 *    inSourceObject:
 *      An specific object defining the source coordinate space that
 *      the point is to be converted from. You might pass a WindowRef
 *      or an HIViewRef. If no object is necessary, you must pass NULL.
 *      See the HICoordinateSpace documentation for details on which
 *      HICoordinateSpaces require objects.
 *    
 *    inDestinationSpace:
 *      The HICoordinateSpace constant specifying the destination
 *      coordinate space that the point is to be converted to. Some
 *      coordinate spaces require the caller to pass extra information
 *      in the inDestinationObject parameter.
 *    
 *    inDestinationObject:
 *      An specific object defining the destination coordinate space
 *      that the point is to be converted to. You might pass a
 *      WindowRef or an HIViewRef. If no object is necessary, you must
 *      pass NULL. See the HICoordinateSpace documentation for details
 *      on which HICoordinateSpaces require objects.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
HIPointConvert(
  HIPoint *           ioPoint,
  HICoordinateSpace   inSourceSpace,
  void *              inSourceObject,            /* can be NULL */
  HICoordinateSpace   inDestinationSpace,
  void *              inDestinationObject)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIRectConvert()
 *  
 *  Discussion:
 *    This routine converts an HIRect from one coordinate space to
 *    another. It takes into account the resolution-independent display
 *    scale factor as appropriate.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    ioRect:
 *      The HIRect to convert.
 *    
 *    inSourceSpace:
 *      The HICoordinateSpace constant specifying the source coordinate
 *      space that the rect is to be converted from. Some coordinate
 *      spaces require the caller to pass extra information in the
 *      inSourceObject parameter.
 *    
 *    inSourceObject:
 *      An specific object defining the source coordinate space that
 *      the rect is to be converted from. You might pass a WindowRef or
 *      an HIViewRef. If no object is necessary, you must pass NULL.
 *      See the HICoordinateSpace documentation for details on which
 *      HICoordinateSpaces require objects.
 *    
 *    inDestinationSpace:
 *      The HICoordinateSpace constant specifying the destination
 *      coordinate space that the rect is to be converted to. Some
 *      coordinate spaces require the caller to pass extra information
 *      in the inDestinationObject parameter.
 *    
 *    inDestinationObject:
 *      An specific object defining the destination coordinate space
 *      that the rect is to be converted to. You might pass a WindowRef
 *      or an HIViewRef. If no object is necessary, you must pass NULL.
 *      See the HICoordinateSpace documentation for details on which
 *      HICoordinateSpaces require objects.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
HIRectConvert(
  HIRect *            ioRect,
  HICoordinateSpace   inSourceSpace,
  void *              inSourceObject,            /* can be NULL */
  HICoordinateSpace   inDestinationSpace,
  void *              inDestinationObject)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HISizeConvert()
 *  
 *  Discussion:
 *    This routine converts an HISize from one coordinate space to
 *    another. It takes into account the resolution-independent display
 *    scale factor as appropriate.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    ioSize:
 *      The HISize to convert.
 *    
 *    inSourceSpace:
 *      The HICoordinateSpace constant specifying the source coordinate
 *      space that the size is to be converted from. Some coordinate
 *      spaces require the caller to pass extra information in the
 *      inSourceObject parameter.
 *    
 *    inSourceObject:
 *      An specific object defining the source coordinate space that
 *      the size is to be converted from. You might pass a WindowRef or
 *      an HIViewRef. If no object is necessary, you must pass NULL.
 *      See the HICoordinateSpace documentation for details on which
 *      HICoordinateSpaces require objects.
 *    
 *    inDestinationSpace:
 *      The HICoordinateSpace constant specifying the destination
 *      coordinate space that the size is to be converted to. Some
 *      coordinate spaces require the caller to pass extra information
 *      in the inDestinationObject parameter.
 *    
 *    inDestinationObject:
 *      An specific object defining the destination coordinate space
 *      that the size is to be converted to. You might pass a WindowRef
 *      or an HIViewRef. If no object is necessary, you must pass NULL.
 *      See the HICoordinateSpace documentation for details on which
 *      HICoordinateSpaces require objects.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
HISizeConvert(
  HISize *            ioSize,
  HICoordinateSpace   inSourceSpace,
  void *              inSourceObject,            /* can be NULL */
  HICoordinateSpace   inDestinationSpace,
  void *              inDestinationObject)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __HIGEOMETRY__ */

