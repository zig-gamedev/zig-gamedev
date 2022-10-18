/*
     File:       HIToolbox/HIImageViews.h
 
     Contains:   API and type definitions related to views that display image content.
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIIMAGEVIEWS__
#define __HIIMAGEVIEWS__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __HIVIEW__
#include <HIToolbox/HIView.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================*/
/*  HIImageView                                                                 */
/*                                                                              */
/* The image view is a new view starting in Mac OS 10.2. It can only be used    */
/* in a compositing window. Like all new HIFoo views, this view is initially    */
/* invisible. You must show the view after creation.                            */
/*==============================================================================*/
/* The HIObject class ID for the HIImageView class. */
#define kHIImageViewClassID             CFSTR("com.apple.HIImageView")

/*
 */
enum {

  /*
   * The control kind of the image view
   */
  kControlKindHIImageView       = 'imag'
};

#if !__LP64__
/*
 *  HIImageViewCreate()
 *  
 *  Discussion:
 *    Creates an image view. The view responds to the scrollable
 *    interface and can be used in a scrolling view. You can pass an
 *    image initially, or set one later.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inImage:
 *      An initial image, or NULL. You can set the image later via
 *      SetControlData.
 *    
 *    outView:
 *      The new image view.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIImageViewCreate(
  CGImageRef   inImage,       /* can be NULL */
  HIViewRef *  outView)                                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIImageViewSetOpaque()
 *  
 *  Discussion:
 *    Allows you to set whether an image view should be treated as
 *    opaque. If this is set to true, the image view can make certain
 *    optimizations for compositing and scrolling. The alpha-related
 *    image view APIs are rendered useless if opacity it set to true.
 *    An image view, when created, is transparent by default.
 *    
 *    NOTE: In Mac OS X 10.2, this view was documented as being opaque
 *    by default, but the implementation did not enforce that. So in
 *    Mac OS X 10.3 and beyond, the view is transparent by default, and
 *    you can make it opaque by calling HIImageViewSetOpaque.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to affect.
 *    
 *    inOpaque:
 *      The new opacity setting. Pass true to indicate you want the
 *      image to be treated as opaque.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIImageViewSetOpaque(
  HIViewRef   inView,
  Boolean     inOpaque)                                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIImageViewIsOpaque()
 *  
 *  Discussion:
 *    Allows you to determine whether an image view is opaque or not.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to query.
 *  
 *  Result:
 *    A boolean result, where true indicates the image view is opaque.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIImageViewIsOpaque(HIViewRef inView)                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIImageViewSetAlpha()
 *  
 *  Discussion:
 *    Allows you to set the alpha for an image, making it more or less
 *    transparent. An alpha of 1.0 is fully opaque, and 0.0 is fully
 *    transparent. The default alpha for an image is 1.0.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to affect.
 *    
 *    inAlpha:
 *      The new alpha value.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIImageViewSetAlpha(
  HIViewRef   inView,
  CGFloat     inAlpha)                                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIImageViewGetAlpha()
 *  
 *  Discussion:
 *    Allows you to get the alpha for an image. An alpha of 1.0 is
 *    fully opaque, and 0.0 is fully transparent.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to query.
 *  
 *  Result:
 *    A floating point number representing the alpha from 0.0 through
 *    1.0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern CGFloat 
HIImageViewGetAlpha(HIViewRef inView)                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIImageViewSetScaleToFit()
 *  
 *  Discussion:
 *    Normally an image view will clip to the view's bounds. Using this
 *    API, you can instead tell the image view to size the image to fit
 *    into the view bounds specified.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to affect.
 *    
 *    inScaleToFit:
 *      A boolean indicating whether the image should be scaled to fit
 *      the view bounds (true) or merely clip to the view bounds
 *      (false).
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIImageViewSetScaleToFit(
  HIViewRef   inView,
  Boolean     inScaleToFit)                                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIImageViewGetScaleToFit()
 *  
 *  Discussion:
 *    Returns whether or not an image view will scale the image it
 *    displays to the view bounds or merely clip to the view bounds. A
 *    true result means it scales.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to query.
 *  
 *  Result:
 *    A boolean result.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIImageViewGetScaleToFit(HIViewRef inView)                    AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIImageViewSetImage()
 *  
 *  Discussion:
 *    Sets the image to display in an image view. The image passed in
 *    is retained by the view, so you may release the image after
 *    calling this API if you no longer need to reference it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to affect.
 *    
 *    inImage:
 *      The image to set.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIImageViewSetImage(
  HIViewRef    inView,
  CGImageRef   inImage)       /* can be NULL */               AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIImageViewCopyImage()
 *  
 *  Discussion:
 *    Gets the image for an image view. If there is no image set on the
 *    view, or the view ref is invalid, NULL is returned. The image is
 *    retained, so you should take care to release it when you are
 *    finished with it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to query.
 *  
 *  Result:
 *    A CoreGraphics (Quartz) image ref.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern CGImageRef 
HIImageViewCopyImage(HIViewRef inView)                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  HIImageViewAutoTransformOptions
 *  
 *  Summary:
 *    These are the available options for applying automatic transforms
 *    on image views and can be accessed with the
 *    HIImageViewSetAutoTransform and HIImageViewGetAutoTransform APIs.
 *  
 *  Discussion:
 *    Currently, the auto transforms for disabled or inactive image
 *    views is similar to the transform applied to the image of an
 *    image well or the image in a bevel button when those controls are
 *    disabled or inactive.
 */
typedef UInt32 HIImageViewAutoTransformOptions;
enum {

  /*
   * No transform is applied to the image.
   */
  kHIImageViewAutoTransformNone = 0,

  /*
   * Transforms are applied to the image view's image when the view is
   * disabled.
   */
  kHIImageViewAutoTransformOnDisable = 1 << 0,

  /*
   * Transforms are applied to the image view's image when the view is
   * deactivated.
   */
  kHIImageViewAutoTransformOnDeactivate = 1 << 1
};

#if !__LP64__
/*
 *  HIImageViewSetAutoTransform()
 *  
 *  Discussion:
 *    Sets how an image view will automatically transform its image.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view for which to set the auto transform options.
 *    
 *    inOptions:
 *      A set of HIImageViewAutoTransformOptions bits.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIImageViewSetAutoTransform(
  HIViewRef                         inView,
  HIImageViewAutoTransformOptions   inOptions)                AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIImageViewGetAutoTransform()
 *  
 *  Discussion:
 *    Gets how an image view is set to automatically transform its
 *    image.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The image view to query.
 *  
 *  Result:
 *    A value of HIImageViewAutoTransformOptions bits. The return value
 *    is undefined if the HIViewRef is invalid.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern HIImageViewAutoTransformOptions 
HIImageViewGetAutoTransform(HIViewRef inView)                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


#endif  /* !__LP64__ */


/*
 */
enum {

  /*
   * This tag is used with GetControlData and SetControlData to get or
   * set the CGImageRef associated with an HIImageView. This constant
   * is deprecated; use the HIImageViewCopyImage and
   * HIImageViewSetImage APIs to get or set the image for an image view.
   */
  kHIImageViewImageTag          = 'imag'
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ ICON CONTROL (CDEF 20)                                                            */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Value parameter should contain the ID of the ICON or cicn you wish to display when  */
/*  creating controls of this type. If you don't want the control tracked at all, use   */
/*  the 'no track' variant.                                                             */
/* Icon control proc IDs */
enum {
  kControlIconProc              = 320,
  kControlIconNoTrackProc       = 321,  /* immediately returns kControlIconPart*/
  kControlIconSuiteProc         = 322,
  kControlIconSuiteNoTrackProc  = 323   /* immediately returns kControlIconPart*/
};

enum {
                                        /* icon ref controls may have either an icon, color icon, icon suite, or icon ref.*/
                                        /* for data other than icon, you must set the data by passing a*/
                                        /* ControlButtonContentInfo to SetControlData*/
  kControlIconRefProc           = 324,
  kControlIconRefNoTrackProc    = 325   /* immediately returns kControlIconPart*/
};

/* Control Kind Tag */
enum {
  kControlKindIcon              = 'icon'
};

/* The HIObject class ID for the HIIconView class. */
#define kHIIconViewClassID              CFSTR("com.apple.HIIconView")
#if !__LP64__
/*
 *  CreateIconControl()
 *  
 *  Summary:
 *    Creates an Icon control at a specific position in the specified
 *    window.
 *  
 *  Discussion:
 *    Icon controls display an icon that (optionally) hilites when
 *    clicked on. On Mac OS X, a root control will be created for the
 *    window if one does not already exist. If a root control exists
 *    for the window, the Icon control will be embedded into it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The WindowRef into which the Icon control will be created. May
 *      be NULL on 10.3 and later.
 *    
 *    inBoundsRect:
 *      The desired position (in coordinates local to the window's
 *      port) for the Icon control.
 *    
 *    inIconContent:
 *      The descriptor for the icon you want the control to display.
 *      Mac OS X and CarbonLib 1.5 (and beyond) support all of the icon
 *      content types. Prior to CarbonLib 1.5, the only content types
 *      that are properly respected are kControlContentIconSuiteRes,
 *      kControlContentCIconRes, and kControlContentICONRes.
 *    
 *    inDontTrack:
 *      A Boolean value indicating whether the control should hilite
 *      when it is clicked on. False means hilite and track the mouse.
 *    
 *    outControl:
 *      On successful output, outControl will contain a reference to
 *      the Icon control.
 *  
 *  Result:
 *    An OSStatus code indicating success or failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateIconControl(
  WindowRef                         inWindow,            /* can be NULL */
  const Rect *                      inBoundsRect,
  const ControlButtonContentInfo *  inIconContent,
  Boolean                           inDontTrack,
  ControlRef *                      outControl)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by icon controls */
#endif  /* !__LP64__ */

enum {
  kControlIconTransformTag      = 'trfm', /* IconTransformType*/
  kControlIconAlignmentTag      = 'algn' /* IconAlignmentType*/
};

/* Tags available with appearance 1.1 or later */
enum {
  kControlIconResourceIDTag     = 'ires', /* SInt16 resource ID of icon to use*/
  kControlIconContentTag        = kControlContentTag /* ControlImageContentInfo*/
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ IMAGE WELL (CDEF 11)                                                              */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Image Wells allow you to control the content type (pict/icon/etc.) shown in the     */
/*  well.                                                                               */
/*                                                                                      */
/*  This is made possible by overloading the Min and Value parameters for the control.  */
/*                                                                                      */
/*  Parameter                   What Goes Here                                          */
/*  ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ         ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ      */
/*  Min                         content type (see constants for bevel buttons)          */
/*  Value                       Resource ID of content type, if resource-based.         */
/*                                                                                      */
/*  Handle-based Content                                                                */
/*  ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ                                                                */
/*  You can create your control and then set the content to an existing handle to an    */
/*  icon suite, etc. using the macros below. Please keep in mind that resource-based    */
/*  content is owned by the control, handle-based content is owned by you. The CDEF will*/
/*  not try to dispose of handle-based content. If you are changing the content type of */
/*  the button on the fly, you must make sure that if you are replacing a handle-       */
/*  based content with a resource-based content to properly dispose of the handle,      */
/*  else a memory leak will ensue.                                                      */
/*                                                                                      */
/* Image Well proc IDs */
enum {
  kControlImageWellProc         = 176
};

/* Control Kind Tag */
enum {
  kControlKindImageWell         = 'well'
};

/* The HIObject class ID for the HIImageWell class. */
#define kHIImageWellClassID             CFSTR("com.apple.HIImageWell")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateImageWellControl()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateImageWellControl(
  WindowRef                         window,
  const Rect *                      boundsRect,
  const ControlButtonContentInfo *  info,
  ControlRef *                      outControl)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by image wells */
#endif  /* !__LP64__ */

enum {
  kControlImageWellContentTag   = kControlContentTag, /* ControlImageContentInfo*/
  kControlImageWellTransformTag = 'tran', /* IconTransformType*/
  kControlImageWellIsDragDestinationTag = 'drag' /* Boolean*/
};

/* Helper routines are available only thru the shared library/glue. */
#if !__LP64__
/*
 *  GetImageWellContentInfo()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
GetImageWellContentInfo(
  ControlRef                    inButton,
  ControlButtonContentInfoPtr   outContent)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetImageWellContentInfo()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
SetImageWellContentInfo(
  ControlRef                    inButton,
  ControlButtonContentInfoPtr   inContent)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetImageWellTransform()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
SetImageWellTransform(
  ControlRef          inButton,
  IconTransformType   inTransform)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HIIMAGEVIEWS__ */

