/*
     File:       QD/ATSUnicodeGlyphs.h
 
     Contains:   ATSUI glyph handling functions.
 
     Version:    Quickdraw-291~1
 
     Copyright:  � 2003-2008 by Apple Inc. all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATSUNICODEGLYPHS__
#define __ATSUNICODEGLYPHS__

#ifndef __ATSUNICODETYPES__
#include <ATSUI/ATSUnicodeTypes.h>
#endif


/* ---------------------------------------------------------------------------- */
/* ATSUI glyph metrics                                                          */
/* ---------------------------------------------------------------------------- */

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ATSQuadraticNewPathProcPtr
 *  
 *  Discussion:
 *    A pointer to a client supplied callback function for handling
 *    glyph curve drawing operations. This callback handles operations
 *    to start a new drawing path.
 *  
 *  Parameters:
 *    
 *    callBackDataPtr:
 *      A pointer to any application specific data that may have been
 *      passed to the callbacks through the iCallbackDataPtr parameter
 *      of the ATSUGlyphGetQuadraticPaths function.
 *  
 *  Result:
 *    Return status. Pass any errors you wish to propagate back to the
 *    original caller of ATSUGlyphGetQuadraticPaths through this return
 *    value. Note that any nonzero result from this callback will halt
 *    the curve drawing process.
 */
typedef CALLBACK_API( OSStatus , ATSQuadraticNewPathProcPtr )(void * callBackDataPtr);
typedef STACK_UPP_TYPE(ATSQuadraticNewPathProcPtr)              ATSQuadraticNewPathUPP;


/*
 *  ATSQuadraticLineProcPtr
 *  
 *  Discussion:
 *    A pointer to a client supplied callback function for handling
 *    glyph curve drawing operations. This callback handles operations
 *    to draw straight lines.
 *  
 *  Parameters:
 *    
 *    pt1:
 *      The starting point of the line.
 *    
 *    pt2:
 *      The end point of the line.
 *    
 *    callBackDataPtr:
 *      A pointer to any application specific data that may have been
 *      passed to the callbacks through the iCallbackDataPtr parameter
 *      of the ATSUGlyphGetQuadraticPaths function.
 *  
 *  Result:
 *    Return status. Pass any errors you wish to propagate back to the
 *    original caller of ATSUGlyphGetQuadraticPaths through this return
 *    value. Note that any nonzero result from this callback will halt
 *    the curve drawing process.
 */
typedef CALLBACK_API( OSStatus , ATSQuadraticLineProcPtr )(const Float32Point *pt1, const Float32Point *pt2, void *callBackDataPtr);
typedef STACK_UPP_TYPE(ATSQuadraticLineProcPtr)                 ATSQuadraticLineUPP;


/*
 *  ATSQuadraticCurveProcPtr
 *  
 *  Discussion:
 *    A pointer to a client supplied callback function for handling
 *    glyph curve drawing operations. This callback handles operations
 *    to draw curves. The curve is a quadratic patch specified by a
 *    start point (pt1), and end point (pt2), and a single control
 *    point (controlPt).
 *  
 *  Parameters:
 *    
 *    pt1:
 *      The starting point of the curve.
 *    
 *    controlPt:
 *      The off-curve control point.
 *    
 *    pt2:
 *      The end point of the curve.
 *    
 *    callBackDataPtr:
 *      A pointer to any application specific data that may have been
 *      passed to the callbacks through the iCallbackDataPtr parameter
 *      of the ATSUGlyphGetQuadraticPaths function.
 *  
 *  Result:
 *    Return status. Pass any errors you wish to propagate back to the
 *    original caller of ATSUGlyphGetQuadraticPaths through this return
 *    value. Note that any nonzero result from this callback will halt
 *    the curve drawing process.
 */
typedef CALLBACK_API( OSStatus , ATSQuadraticCurveProcPtr )(const Float32Point *pt1, const Float32Point *controlPt, const Float32Point *pt2, void *callBackDataPtr);
typedef STACK_UPP_TYPE(ATSQuadraticCurveProcPtr)                ATSQuadraticCurveUPP;

/*
 *  ATSQuadraticClosePathProcPtr
 *  
 *  Discussion:
 *    A pointer to a client supplied callback function for handling
 *    glyph curve drawing operations. This callback handles operations
 *    to close the current drawing path.
 *  
 *  Parameters:
 *    
 *    callBackDataPtr:
 *      A pointer to any application specific data that may have been
 *      passed to the callbacks through the iCallbackDataPtr parameter
 *      of the ATSUGlyphGetQuadraticPaths function.
 *  
 *  Result:
 *    Return status. Pass any errors you wish to propagate back to the
 *    original caller of ATSUGlyphGetQuadraticPaths through this return
 *    value. Note that any nonzero result from this callback will halt
 *    the curve drawing process.
 */
typedef CALLBACK_API( OSStatus , ATSQuadraticClosePathProcPtr )(void * callBackDataPtr);
typedef STACK_UPP_TYPE(ATSQuadraticClosePathProcPtr)            ATSQuadraticClosePathUPP;

/*
 *  ATSCubicMoveToProcPtr
 *  
 *  Discussion:
 *    A pointer to a client supplied callback function for handling
 *    glyph curve drawing operations. This callback handles operations
 *    to move the current pen location.
 *  
 *  Parameters:
 *    
 *    pt:
 *      The point to which to move the current pen location.
 *    
 *    callBackDataPtr:
 *      A pointer to any application specific data that may have been
 *      passed to the callbacks through the iCallbackDataPtr parameter
 *      of the ATSUGlyphGetCubicPaths function.
 *  
 *  Result:
 *    Return status. Pass any errors you wish to propagate back to the
 *    original caller of ATSUGlyphGetCubicPaths through this return
 *    value. Note that any nonzero result from this callback will halt
 *    the curve drawing process.
 */
typedef CALLBACK_API( OSStatus , ATSCubicMoveToProcPtr )(const Float32Point *pt, void *callBackDataPtr);
typedef STACK_UPP_TYPE(ATSCubicMoveToProcPtr)                   ATSCubicMoveToUPP;


/*
 *  ATSCubicLineToProcPtr
 *  
 *  Discussion:
 *    A pointer to a client supplied callback function for handling
 *    glyph curve drawing operations. This callback handles operations
 *    to draw straight lines.
 *  
 *  Parameters:
 *    
 *    pt:
 *      The end point of the line to be drawn. The starting point is
 *      whatever the current pen position is.
 *    
 *    callBackDataPtr:
 *      A pointer to any application specific data that may have been
 *      passed to the callbacks through the iCallbackDataPtr parameter
 *      of the ATSUGlyphGetCubicPaths function.
 *  
 *  Result:
 *    Return status. Pass any errors you wish to propagate back to the
 *    original caller of ATSUGlyphGetCubicPaths through this return
 *    value. Note that any nonzero result from this callback will halt
 *    the curve drawing process.
 */
typedef CALLBACK_API( OSStatus , ATSCubicLineToProcPtr )(const Float32Point *pt, void *callBackDataPtr);
typedef STACK_UPP_TYPE(ATSCubicLineToProcPtr)                   ATSCubicLineToUPP;


/*
 *  ATSCubicCurveToProcPtr
 *  
 *  Discussion:
 *    A pointer to a client supplied callback function for handling
 *    glyph curve drawing operations. This callback handles operations
 *    to draw a curve. The curve is a Bezier patch defined by two
 *    off-curve control points (pt1 and pt2), and an endpoint (pt3).
 *    The starting point is whatever the current pen position is.
 *  
 *  Parameters:
 *    
 *    pt1:
 *      The first off-curve control point.
 *    
 *    pt2:
 *      The second off-curve control point.
 *    
 *    pt3:
 *      The end point of the curve.
 *    
 *    callBackDataPtr:
 *      A pointer to any application specific data that may have been
 *      passed to the callbacks through the iCallbackDataPtr parameter
 *      of the ATSUGlyphGetCubicPaths function.
 *  
 *  Result:
 *    Return status. Pass any errors you wish to propagate back to the
 *    original caller of ATSUGlyphGetCubicPaths through this return
 *    value. Note that any nonzero result from this callback will halt
 *    the curve drawing process.
 */
typedef CALLBACK_API( OSStatus , ATSCubicCurveToProcPtr )(const Float32Point *pt1, const Float32Point *pt2, const Float32Point *pt3, void *callBackDataPtr);
typedef STACK_UPP_TYPE(ATSCubicCurveToProcPtr)                  ATSCubicCurveToUPP;


/*
 *  ATSCubicClosePathProcPtr
 *  
 *  Discussion:
 *    A pointer to a client supplied callback function for handling
 *    glyph curve drawing operations. This callback handles operations
 *    to close the current drawing path.
 *  
 *  Parameters:
 *    
 *    callBackDataPtr:
 *      A pointer to any application specific data that may have been
 *      passed to the callbacks through the iCallbackDataPtr parameter
 *      of the ATSUGlyphGetCubicPaths function.
 *  
 *  Result:
 *    Return status. Pass any errors you wish to propagate back to the
 *    original caller of ATSUGlyphGetCubicPaths through this return
 *    value. Note that any nonzero result from this callback will halt
 *    the curve drawing process.
 */
typedef CALLBACK_API( OSStatus , ATSCubicClosePathProcPtr )(void * callBackDataPtr);
typedef STACK_UPP_TYPE(ATSCubicClosePathProcPtr)                ATSCubicClosePathUPP;

#ifdef __cplusplus
}
#endif

#endif /* __ATSUNICODEGLYPHS__ */

