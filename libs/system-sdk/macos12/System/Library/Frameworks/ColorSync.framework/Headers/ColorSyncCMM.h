/*
 * ColorSync - ColorSyncCMM.h
 * Copyright (c)  2008 Apple Inc.
 * All rights reserved.
 */

#ifndef __COLORSYNCCMM__
#define __COLORSYNCCMM__

#if !defined(__swift__)

#ifdef __cplusplus
extern "C" {
#endif

#include <ColorSync/ColorSyncProfile.h>
#include <ColorSync/ColorSyncTransform.h>

/*
 * Notes:
 *  - Color conversions are performed by a Color Management Module (CMM) which is a plugin to ColorSync.
 *  - ColorSync contains Apple CMM, which is not replaceable, but third parties can install their own CMMs
 *  - ColorSync provides access to installed CMMs as well as those that can be part of the application bundle.
 *  - CMM can be selected and specified as a preferred CMM per color transform created by the application 
 *  - if the third party CMM fails to perform a task, Apple CMM will take it over
 *  - ColorSyncCMMRef is a light weight wrapper of CFBundleRef
 *  - See /Developer/Examples/ColorSync/DemoCMM
 */

typedef struct CF_BRIDGED_TYPE(id) ColorSyncCMM* ColorSyncCMMRef;

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

CSEXTERN_DESKTOP CFTypeID ColorSyncCMMGetTypeID(void) CS_AVAILABLE_PUBLIC_DESKTOP(10.4);
   /*
    * returns the CFTypeID for ColorSyncCMMs.
    */

CSEXTERN_DESKTOP ColorSyncCMMRef __nullable ColorSyncCMMCreate(CFBundleRef cmmBundle) CS_AVAILABLE_PUBLIC_DESKTOP(10.4);

CSEXTERN_DESKTOP CFBundleRef __nullable ColorSyncCMMGetBundle(ColorSyncCMMRef) CS_AVAILABLE_PUBLIC_DESKTOP(10.4); // will return NULL for built-in Apple CMM

CSEXTERN_DESKTOP CFStringRef __nullable ColorSyncCMMCopyLocalizedName(ColorSyncCMMRef) CS_AVAILABLE_PUBLIC_DESKTOP(10.4); // needed to get the name of built-in CMM

CSEXTERN_DESKTOP CFStringRef __nullable ColorSyncCMMCopyCMMIdentifier(ColorSyncCMMRef) CS_AVAILABLE_PUBLIC_DESKTOP(10.4); // needed to get the identifier of built-in CMM

typedef bool (*ColorSyncCMMIterateCallback)(ColorSyncCMMRef cmm, void* userInfo) CS_AVAILABLE_PUBLIC_DESKTOP(10.4);
   /*
    * Note:  If the ColorSyncCMMIterateCallback returns false, the iteration stops
    *
    */

CSEXTERN_DESKTOP void ColorSyncIterateInstalledCMMs (ColorSyncCMMIterateCallback callBack, void* __nullable userInfo) CS_AVAILABLE_PUBLIC_DESKTOP(10.4);
   /*
    *  callBack   - pointer to a client provided function
    *  user Info  - (optional) pointer to the userIndo to be passed to the callback
    *
    */


/*
* ==========================================================================================
* This part defines the interface for developers of third party CMMs for ColorSync.
* ==========================================================================================
*/

typedef bool (*CMMInitializeLinkProfileProc) (ColorSyncMutableProfileRef, CFArrayRef profileInfo, CFDictionaryRef __nullable options);

typedef bool (*CMMInitializeTransformProc) (ColorSyncTransformRef, CFArrayRef profileInfo, CFDictionaryRef __nullable options);

typedef bool (*CMMApplyTransformProc)(ColorSyncTransformRef transform, size_t width, size_t height,
                                      size_t dstPlanes, void* __nonnull  * __nonnull  dst, ColorSyncDataDepth dstDepth,
                                      ColorSyncDataLayout dstFormat, size_t dstBytesPerRow,
                                      size_t srcPlanes, const void* __nonnull* __nonnull src, ColorSyncDataDepth srcDepth,
                                      ColorSyncDataLayout srcFormat, size_t srcBytesPerRow,
                                      CFDictionaryRef __nullable options);

typedef CFTypeRef __nullable (*CMMCreateTransformPropertyProc)(ColorSyncTransformRef __nullable transform, CFTypeRef key, CFDictionaryRef __nullable options);

CSEXTERN_DESKTOP CFStringRef kCMMInitializeLinkProfileProcName CS_AVAILABLE_PUBLIC_DESKTOP(10.4);     /* CMMInitializeLinkProfileProcName   */
CSEXTERN_DESKTOP CFStringRef kCMMInitializeTransformProcName CS_AVAILABLE_PUBLIC_DESKTOP(10.4);       /* CMMInitializeTransformProcName     */
CSEXTERN_DESKTOP CFStringRef kCMMApplyTransformProcName CS_AVAILABLE_PUBLIC_DESKTOP(10.4);            /* CMMApplyTransformProcName          */
CSEXTERN_DESKTOP CFStringRef kCMMCreateTransformPropertyProcName CS_AVAILABLE_PUBLIC_DESKTOP(10.4);   /* CMMCreateTransformPropertyProcName */

/*
* Following keys are expected to be present in the CMM bundle info dictionary:
*
* Standard Mac OS X bundle keys:
*              kCFBundleExecutableKey
*              kCFBundleIdentifierKey
*              kCFBundleVersionKey
*              kCFBundleNameKey
*
* CMM specific keys:
*              kCMMInitializeLinkProfileProcName  -  CFStringRef of the name of a CMMInitializeLinkProfile
*                                                    function implemented in the CMM bundle executable.
*
*              kCMMInitializeTransformProcName    -  CFStringRef of the name of a CMMInitializeTransform
*                                                    function implemented in the CMM bundle executable.
*
*              kCMMApplyTransformProcName         -  CFStringRef of the name of a CMMApplyTransform function
*                                                    implemented in the CMM bundle executable.
*
*              kCMMCreateTransformPropertyProcName - CFStringRef of the name of a CMMCreateTransformProperty
*                                                    function implemented in the CMM bundle executable.
*                                                    Optional.
*/

#ifdef __cplusplus
}
#endif

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#else /* #if !defined(__swift__) */

#ifdef __cplusplus
extern "C" {
#endif
    
#include <ColorSync/ColorSyncProfile.h>
#include <ColorSync/ColorSyncTransform.h>
    
    /*
     * Notes:
     *  - Color conversions are performed by a Color Management Module (CMM) which is a plugin to ColorSync.
     *  - ColorSync contains Apple CMM, which is not replaceable, but third parties can install their own CMMs
     *  - ColorSync provides access to installed CMMs as well as those that can be part of the application bundle.
     *  - CMM can be selected and specified as a preferred CMM per color transform created by the application
     *  - if the third party CMM fails to perform a task, Apple CMM will take it over
     *  - ColorSyncCMMRef is a light weight wrapper of CFBundleRef
     *  - See /Developer/Examples/ColorSync/DemoCMM
     */
    
    typedef struct CF_BRIDGED_TYPE(id) ColorSyncCMM* ColorSyncCMMRef;
    
    CSEXTERN CFTypeID ColorSyncCMMGetTypeID(void);
    /*
     * returns the CFTypeID for ColorSyncCMMs.
     */
    
    CSEXTERN ColorSyncCMMRef ColorSyncCMMCreate(CFBundleRef cmmBundle);
    
    CSEXTERN CFBundleRef ColorSyncCMMGetBundle(ColorSyncCMMRef); // will return NULL for built-in Apple CMM
    
    CSEXTERN CFStringRef ColorSyncCMMCopyLocalizedName(ColorSyncCMMRef); // needed to get the name of built-in CMM
    
    CSEXTERN CFStringRef ColorSyncCMMCopyCMMIdentifier(ColorSyncCMMRef); // needed to get the identifier of built-in CMM
    
    typedef bool (*ColorSyncCMMIterateCallback)(ColorSyncCMMRef cmm, void* userInfo);
    /*
     * Note:  If the ColorSyncCMMIterateCallback returns false, the iteration stops
     *
     */
    
    CSEXTERN void ColorSyncIterateInstalledCMMs (ColorSyncCMMIterateCallback callBack, void* userInfo);
    /*
     *  callBack   - pointer to a client provided function
     *  user Info  - (optional) pointer to the userIndo to be passed to the callback
     *
     */
    
    
    /*
     * ==========================================================================================
     * This part defines the interface for developers of third party CMMs for ColorSync.
     * ==========================================================================================
     */
    
    typedef bool (*CMMInitializeLinkProfileProc) (ColorSyncMutableProfileRef, CFArrayRef profileInfo, CFDictionaryRef options);
    
    typedef bool (*CMMInitializeTransformProc) (ColorSyncTransformRef, CFArrayRef profileInfo, CFDictionaryRef options);
    
    typedef bool (*CMMApplyTransformProc)(ColorSyncTransformRef transform, size_t width, size_t height,
                                          size_t dstPlanes, void* dst[], ColorSyncDataDepth dstDepth,
                                          ColorSyncDataLayout dstFormat, size_t dstBytesPerRow,
                                          size_t srcPlanes, const void* src[], ColorSyncDataDepth srcDepth,
                                          ColorSyncDataLayout srcFormat, size_t srcBytesPerRow,
                                          CFDictionaryRef options);
    
    typedef CFTypeRef (*CMMCreateTransformPropertyProc)(ColorSyncTransformRef transform, CFTypeRef key, CFDictionaryRef options);
    
    CSEXTERN CFStringRef kCMMInitializeLinkProfileProcName;     /* CMMInitializeLinkProfileProcName   */
    CSEXTERN CFStringRef kCMMInitializeTransformProcName;       /* CMMInitializeTransformProcName     */
    CSEXTERN CFStringRef kCMMApplyTransformProcName;            /* CMMApplyTransformProcName          */
    CSEXTERN CFStringRef kCMMCreateTransformPropertyProcName;   /* CMMCreateTransformPropertyProcName */
    
    /*
     * Following keys are expected to be present in the CMM bundle info dictionary:
     *
     * Standard Mac OS X bundle keys:
     *              kCFBundleExecutableKey
     *              kCFBundleIdentifierKey
     *              kCFBundleVersionKey
     *              kCFBundleNameKey
     *
     * CMM specific keys:
     *              kCMMInitializeLinkProfileProcName  -  CFStringRef of the name of a CMMInitializeLinkProfile
     *                                                    function implemented in the CMM bundle executable.
     *
     *              kCMMInitializeTransformProcName    -  CFStringRef of the name of a CMMInitializeTransform
     *                                                    function implemented in the CMM bundle executable.
     *
     *              kCMMApplyTransformProcName         -  CFStringRef of the name of a CMMApplyTransform function
     *                                                    implemented in the CMM bundle executable.
     *
     *              kCMMCreateTransformPropertyProcName - CFStringRef of the name of a CMMCreateTransformProperty
     *                                                    function implemented in the CMM bundle executable.
     *                                                    Optional.
     */
    
#ifdef __cplusplus
}
#endif

#endif /* #if !defined(__swift__) */


#endif /* __COLORSYNCCMM__ */
