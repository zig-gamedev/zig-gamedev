/*
     File:       HIServices/HIShape.h
 
     Contains:   Generic Abstract Shape API
 
     Copyright:  © 2001-2012 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HISHAPE__
#define __HISHAPE__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef __QUICKDRAW__
#include <QD/Quickdraw.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  HIShape
 *  
 *  Discussion:
 *    HIShape is an abstract shape object for use with some of the
 *    HIToolbox APIs. It is designed as a replacement for RgnHandles
 *    (though it is currently implemented in terms of them at the time
 *    of this writing). This abstraction will allow us to avoid using
 *    QD types in our APIs, particularly in HIView. 
 *    
 *    One of the biggest benefits of HIShape is that we have mutable
 *    and immutable variants. This means that immutable shapes can be
 *    created and passed around and 'copied' very quickly, since they
 *    are actually refcounted when copied. This avoids needing to do
 *    the handle-to-handle copies that occur right now with
 *    RgnHandle-based APIs. 
 *    
 *    Thread Safety Information 
 *    
 *    On Mac OS X 10.5 and later, all of the HIShape APIs are thread
 *    safe. However, only the APIs are thread safe; the HIShapeRefs
 *    themselves are not. It is safe to call HIShape APIs on multiple
 *    threads, so long as no two threads try to operate on the same
 *    HIShapeRef at the same time. 
 *    
 *    If you need multiple threads to operate on a single HIShapeRef at
 *    the same time, you must implement your own locking mechanism.
 */
typedef const struct __HIShape*         HIShapeRef;
typedef struct __HIShape*               HIMutableShapeRef;

/*
 *  Summary:
 *    Messages passed to an HIShapeEnumeratorProcPtr callback.
 */
enum {

  /*
   * The callback receives this message at the beginning of enumeration.
   */
  kHIShapeEnumerateInit         = 1,

  /*
   * The callback receives this message when it is passed a rectangular
   * portion of the shape.
   */
  kHIShapeEnumerateRect         = 2,

  /*
   * The callback receives this message at the end of enumeration.
   */
  kHIShapeEnumerateTerminate    = 3
};


/*
 *  Summary:
 *    Options for HIShapeEnumerate.
 */
enum {

  /*
   * Enumeration should begin at the top of the shape. This is the
   * default behavior.
   */
  kHIShapeParseFromTop          = 0,

  /*
   * Enumeration should begin at the bottom of the shape.
   */
  kHIShapeParseFromBottom       = (1 << 0),

  /*
   * Enumeration should begin at the left side of the shape. This is
   * the default behavior.
   */
  kHIShapeParseFromLeft         = 0,

  /*
   * Enumeration should begin at the right side of the shape.
   */
  kHIShapeParseFromRight        = (1 << 1),

  /*
   * Enumeration should begin at the top left corner of the shape. This
   * is the default behavior.
   */
  kHIShapeParseFromTopLeft      = kHIShapeParseFromTop | kHIShapeParseFromLeft,

  /*
   * Enumeration should begin at the bottom right corner of the shape.
   */
  kHIShapeParseFromBottomRight  = kHIShapeParseFromBottom | kHIShapeParseFromRight
};


/*
 *  HIShapeEnumerateProcPtr
 *  
 *  Summary:
 *    Callback function that receives rectangles parsed from an HIShape.
 *  
 *  Parameters:
 *    
 *    inMessage:
 *      One of kHIShapeEnumerateInit, kHIShapeEnumerateRect, or
 *      kHIShapeEnumerateTerminate.
 *    
 *    inShape:
 *      The shape being enumerated.
 *    
 *    inRect:
 *      If inMessage is kHIShapeEnumerateRect, this parameter is a
 *      rectangle that forms part of the shape. If inMessage is
 *      kHIShapeEnumerateInit or kHIShapeEnumerateTerminate, the value
 *      of this parameter is undefined.
 *    
 *    inRefcon:
 *      Client-provided data that was passed to HIShapeEnumerate.
 *  
 *  Result:
 *    The callback should return noErr to continue enumeration, or any
 *    other error code to stop enumeration. If the callback returns a
 *    value other than noErr in response to the Init message, then the
 *    callback will not be called with the Rect or Terminate messages.
 *    If the callback returns a value other than noErr in response to a
 *    Rect message, then the callback will still be called with a
 *    Terminate message. The first value other than noErr returned by
 *    the callback is also returned from HIShapeEnumerate.
 */
typedef CALLBACK_API_C( OSStatus , HIShapeEnumerateProcPtr )(int inMessage, HIShapeRef inShape, const CGRect *inRect, void *inRefcon);
/*
 *  HIShapeGetTypeID()
 *  
 *  Discussion:
 *    Returns the CoreFoundation type ID for the HIShape class.
 *    
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Result:
 *    A CoreFoundation type ID.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
HIShapeGetTypeID(void)                                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;





/*======================================================================================*/
/*  IMMUTABLE FUNCTIONS                                                                 */
/*======================================================================================*/
/*
 *  HIShapeCreateEmpty()
 *  
 *  Discussion:
 *    Creates an immutable empty shape. Useful at times. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Result:
 *    An immutable, empty HIShape reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
HIShapeCreateEmpty(void)                                      AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIShapeCreateWithQDRgn()
 *  
 *  Discussion:
 *    Creates an immutable shape based on an existing Quickdraw region
 *    handle. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inRgn:
 *      The Quickdraw region from which to create the HIShape.
 *  
 *  Result:
 *    An immutable HIShape reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
HIShapeCreateWithQDRgn(RgnHandle inRgn)                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeCreateWithRect()
 *  
 *  Discussion:
 *    Creates an immutable, rectangular shape based on a given
 *    rectangle. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The CGRect from which to create the resulting shape.
 *  
 *  Result:
 *    An immutable HIShape reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
HIShapeCreateWithRect(const CGRect * inRect)                  AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeCreateCopy()
 *  
 *  Discussion:
 *    Creates an immutable copy of a mutable or immutable HIShape.
 *    
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      The existing HIShapeRef you wish to copy.
 *  
 *  Result:
 *    An immutable HIShape reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
HIShapeCreateCopy(HIShapeRef inShape)                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeCreateIntersection()
 *  
 *  Discussion:
 *    Creates a new immutable shape which is the intersection of two
 *    others. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape1:
 *      An existing HIShapeRef.
 *    
 *    inShape2:
 *      An existing HIShapeRef.
 *  
 *  Result:
 *    A new immutable HIShapeRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
HIShapeCreateIntersection(
  HIShapeRef   inShape1,
  HIShapeRef   inShape2)                                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeCreateDifference()
 *  
 *  Discussion:
 *    Creates a new immutable shape which is the difference of two
 *    others. The second shape is subtracted from the first. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape1:
 *      An existing HIShapeRef.
 *    
 *    inShape2:
 *      An existing HIShapeRef.
 *  
 *  Result:
 *    A new immutable HIShapeRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
HIShapeCreateDifference(
  HIShapeRef   inShape1,
  HIShapeRef   inShape2)                                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeCreateUnion()
 *  
 *  Discussion:
 *    Creates a new immutable shape which is the union of two others.
 *    
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape1:
 *      An existing HIShapeRef.
 *    
 *    inShape2:
 *      An existing HIShapeRef.
 *  
 *  Result:
 *    A new immutable HIShapeRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
HIShapeCreateUnion(
  HIShapeRef   inShape1,
  HIShapeRef   inShape2)                                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeCreateXor()
 *  
 *  Discussion:
 *    Creates a new immutable shape which is the difference between the
 *    union and the intersection of the two others. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape1:
 *      An existing HIShapeRef.
 *    
 *    inShape2:
 *      An existing HIShapeRef.
 *  
 *  Result:
 *    A new immutable HIShapeRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
HIShapeCreateXor(
  HIShapeRef   inShape1,
  HIShapeRef   inShape2)                                      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIShapeIsEmpty()
 *  
 *  Discussion:
 *    Returns true if the given HIShapeRef is empty, i.e. its area is
 *    empty. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      The existing HIShapeRef you wish to test.
 *  
 *  Result:
 *    A boolean result.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIShapeIsEmpty(HIShapeRef inShape)                            AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeIsRectangular()
 *  
 *  Discussion:
 *    Returns true if the given HIShapeRef is rectangular. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      The existing HIShapeRef you wish to test.
 *  
 *  Result:
 *    A boolean result.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIShapeIsRectangular(HIShapeRef inShape)                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeContainsPoint()
 *  
 *  Discussion:
 *    Returns true if the given HIShapeRef contains the point passed
 *    in. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      An existing HIShapeRef.
 *    
 *    inPoint:
 *      The point to check.
 *  
 *  Result:
 *    A boolean result.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIShapeContainsPoint(
  HIShapeRef       inShape,
  const CGPoint *  inPoint)                                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeIntersectsRect()
 *  
 *  Discussion:
 *    Returns true if the given HIShapeRef intersects the rect passed
 *    in. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      An existing HIShapeRef.
 *    
 *    inRect:
 *      The rectangle to check.
 *  
 *  Result:
 *    A boolean result.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIShapeIntersectsRect(
  HIShapeRef      inShape,
  const CGRect *  inRect)                                     AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIShapeGetBounds()
 *  
 *  Discussion:
 *    Returns the bounding rectangle of a given HIShapeRef. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      An existing HIShapeRef.
 *    
 *    outRect:
 *      Receives the bounding rectangle.
 *  
 *  Result:
 *    A pointer to the rectangle you passed in, for convenience.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern CGRect * 
HIShapeGetBounds(
  HIShapeRef   inShape,
  CGRect *     outRect)                                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeGetAsQDRgn()
 *  
 *  Discussion:
 *    Changes a given Quickdraw region handle to have the same shape as
 *    a given HIShapeRef. Essentially you are converting an HIShapeRef
 *    into a RgnHandle. This conversion may lose fidelity depending on
 *    how the shape was created originally. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      An existing HIShapeRef.
 *    
 *    outRgn:
 *      An existing region to change.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeGetAsQDRgn(
  HIShapeRef   inShape,
  RgnHandle    outRgn)                                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeReplacePathInCGContext()
 *  
 *  Discussion:
 *    Given an HIShapeRef and a CGContextRef, make the current path in
 *    the context represent the shape. You might use this to clip to a
 *    shape, for example. You could call this function and then
 *    immediately call CGContextClip. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      An existing HIShapeRef.
 *    
 *    inContext:
 *      The context to apply the shape to.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeReplacePathInCGContext(
  HIShapeRef     inShape,
  CGContextRef   inContext)                                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


#if !__LP64__
/*
 *  HIShapeSetQDClip()
 *  
 *  Discussion:
 *    Given an HIShapeRef and a Quickdraw port, set the current clip in
 *    the port to the shape. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      An existing HIShapeRef.
 *    
 *    inPort:
 *      The port to set the clip for.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeSetQDClip(
  HIShapeRef   inShape,
  CGrafPtr     inPort)                                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


#endif  /* !__LP64__ */

/*
 *  HIShapeEnumerate()
 *  
 *  Discussion:
 *    Parses a shape into its constituent rectangles and passes each
 *    rectangle to a callback function. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      The shape to enumerate.
 *    
 *    inOptions:
 *      Options controlling how to enumerate the shape.
 *    
 *    inProc:
 *      The callback function that will be called with each rectangle.
 *    
 *    inRefcon:
 *      Extra data that will be passed to the callback function.
 *  
 *  Result:
 *    The function result is the value returned by the callback
 *    function: noErr if the callback always returns noErr, or the
 *    first non-noErr value returned by the callback.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeEnumerate(
  HIShapeRef                inShape,
  OptionBits                inOptions,
  HIShapeEnumerateProcPtr   inProc,
  void *                    inRefcon)                         AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



/*======================================================================================*/
/*  MUTABLE FUNCTIONS                                                                   */
/*======================================================================================*/
/*
 *  HIShapeCreateMutable()
 *  
 *  Discussion:
 *    Creates a new, mutable, empty shape. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Result:
 *    A mutable shape reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIMutableShapeRef 
HIShapeCreateMutable(void)                                    AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeCreateMutableCopy()
 *  
 *  Discussion:
 *    Given an existing HIShapeRef, creates a new mutable copy.
 *    
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inOrig:
 *      The shape to copy.
 *  
 *  Result:
 *    A mutable shape reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIMutableShapeRef 
HIShapeCreateMutableCopy(HIShapeRef inOrig)                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeCreateMutableWithRect()
 *  
 *  Discussion:
 *    Creates a mutable HIShapeRef based on a given rectangle. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The rectangle from which to create the shape.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern HIMutableShapeRef 
HIShapeCreateMutableWithRect(const CGRect * inRect)           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIShapeSetEmpty()
 *  
 *  Discussion:
 *    Sets a mutable shape to be an empty shape. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      The shape to empty.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeSetEmpty(HIMutableShapeRef inShape)                    AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeSetWithShape()
 *  
 *  Discussion:
 *    Sets a mutable shape to have the same contents as another shape.
 *    
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inDestShape:
 *      The mutable destination shape.
 *    
 *    inSrcShape:
 *      The source shape. This shape's contents will be copied into
 *      inDestShape.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeSetWithShape(
  HIMutableShapeRef   inDestShape,
  HIShapeRef          inSrcShape)                             AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIShapeIntersect()
 *  
 *  Discussion:
 *    Takes two shapes and sets a third to be their intersection.
 *    
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape1:
 *      The first shape.
 *    
 *    inShape2:
 *      The second shape.
 *    
 *    outResult:
 *      The shape to receive the result of the intersection. This can
 *      be one of the source shapes. This shape must be mutable.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeIntersect(
  HIShapeRef          inShape1,
  HIShapeRef          inShape2,
  HIMutableShapeRef   outResult)                              AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeDifference()
 *  
 *  Discussion:
 *    Takes two shapes and sets a third to be their difference. The
 *    second shape is subtracted from the first. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape1:
 *      The first shape.
 *    
 *    inShape2:
 *      The second shape.
 *    
 *    outResult:
 *      The shape to receive the result of the intersection. This can
 *      be one of the source shapes. This shape must be mutable.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeDifference(
  HIShapeRef          inShape1,
  HIShapeRef          inShape2,
  HIMutableShapeRef   outResult)                              AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeUnion()
 *  
 *  Discussion:
 *    Takes two shapes and sets a third to be their union. 
 *    
 *    On Mac OS X 10.2 and 10.3, this API incorrectly required that the
 *    result shape be immutable. On Mac OS X 10.4 and later, this API
 *    correctly requires that the result shape be mutable. If you need
 *    to run on both 10.4 and earlier releases, you will need to
 *    account for this difference. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape1:
 *      The first shape.
 *    
 *    inShape2:
 *      The second shape.
 *    
 *    outResult:
 *      The shape to receive the result of the union. This can be one
 *      of the source shapes. On Mac OS X 10.2 and 10.3, this shape
 *      must be immutable, but it will be set to the unioned shape
 *      anyways. On Mac OS X 10.4 and later, this shape must be mutable.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeUnion(
  HIShapeRef          inShape1,
  HIShapeRef          inShape2,
  HIMutableShapeRef   outResult)                              AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeXor()
 *  
 *  Discussion:
 *    Takes two shapes and sets a third to be the difference between
 *    the union and the intersection of the two shapes. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape1:
 *      The first shape.
 *    
 *    inShape2:
 *      The second shape.
 *    
 *    outResult:
 *      The shape to receive the result of the xor operation. This can
 *      be one of the source shapes. This shape must be mutable.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeXor(
  HIShapeRef          inShape1,
  HIShapeRef          inShape2,
  HIMutableShapeRef   outResult)                              AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIShapeOffset()
 *  
 *  Discussion:
 *    Offsets a shape by some delta. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      The shape to offset.
 *    
 *    inDX:
 *      The delta to move the shape on the X axis.
 *    
 *    inDY:
 *      The delta to move the shape on the Y axis.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeOffset(
  HIMutableShapeRef   inShape,
  CGFloat             inDX,
  CGFloat             inDY)                                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIShapeInset()
 *  
 *  Discussion:
 *    Contracts or expands a shape by some delta. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      The shape to inset.
 *    
 *    inDX:
 *      The delta to contract the shape on the X axis. Can be negative
 *      to expand the shape.
 *    
 *    inDY:
 *      The delta to contract the shape on the Y axis. Can be negative
 *      to expand the shape.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeInset(
  HIMutableShapeRef   inShape,
  CGFloat             inDX,
  CGFloat             inDY)                                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIShapeUnionWithRect()
 *  
 *  Discussion:
 *    Unions a shape with a rectangle. 
 *    
 *    This API is thread safe only on Mac OS X 10.5 and later; see the
 *    Thread Safety Information section at the top of the header for
 *    other important details.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inShape:
 *      The shape to combine with the rectangle.
 *    
 *    inRect:
 *      The rectangle to combine with the shape.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIShapeUnionWithRect(
  HIMutableShapeRef   inShape,
  const CGRect *      inRect)                                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



#ifdef __cplusplus
}
#endif

#endif /* __HISHAPE__ */

