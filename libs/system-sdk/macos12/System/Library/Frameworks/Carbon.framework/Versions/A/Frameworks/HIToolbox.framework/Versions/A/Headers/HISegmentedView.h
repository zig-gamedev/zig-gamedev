/*
     File:       HIToolbox/HISegmentedView.h
 
     Contains:   API and type definitions related to HISegmentedView.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HISEGMENTEDVIEW__
#define __HISEGMENTEDVIEW__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __HIVIEW__
#include <HIToolbox/HIView.h>
#endif


/*==============================================================================*/
/*  HISegmentedView                                                             */
/*                                                                              */
/*  HISegmentedView is a new view available in Mac OS X 10.3 and later.         */
/*  Examples of the segmented view are the Finder's icon/column/list view       */
/*  switcher, and the back/forward buttons in Open panels.                      */
/*  The segmented view operates as a group of buttons, each of which can be     */
/*  configured with different behaviors and content.                            */
/*                                                                              */
/*  Accessibility Notes: Those of you who wish to customize the accessibility   */
/*  information provided for individual segments of the segmented view -- by    */
/*  handling various kEventClassAccessibility Carbon Events, by calling         */
/*  HIObjectSetAuxiliaryAccessibilityAttribute, etc. -- need to know how to     */
/*  interpret and/or build AXUIElementRefs that represent individual segments.  */
/*  The AXUIElement representing an individual segment will/must be constructed */
/*  using the segmented view's HIViewRef and the UInt64 identifier of the       */
/*  one-based index of the segment the element refers to. As usual, a UInt64    */
/*  identifier of zero represents the segmented view as a whole. You must       */
/*  neither interpret nor create segmented view elements whose identifiers are  */
/*  greater than the count of segments in the segmented view.                   */
/*==============================================================================*/
/* The HIObject class ID for the HISegmentedView class. */

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define kHISegmentedViewClassID         CFSTR("com.apple.HISegmentedView")
/* The HIViewKind for the HISegmentedView class. */
enum {
  kHISegmentedViewKind          = 'sgmt'
};

#if !__LP64__
/*
 *  HISegmentedViewCreate()
 *  
 *  Summary:
 *    Creates a segmented view. This is the type of view you would use
 *    to implement the icon/column/list view switcher as seen in the
 *    Finder. 
 *    
 *    After creating a segmented view, you need to set the number of
 *    segments via HISegmentedViewSetSegmentCount. Each segment can be
 *    configured independently using the other HISegmentedView APIs.
 *    
 *    
 *    Changing the number of segments and configuring each segment will
 *    change the appearance of the segmented view. After you configure
 *    the view, you may want to call HIViewGetOptimalBounds on the view
 *    and resize it so the content will fit optimally. 
 *    
 *    The value of the whole segmented view corresponds to the index of
 *    the currently selected segment with the radio behavior. If you
 *    set the value of the whole segmented view to n via
 *    HIViewSetValue, every radio-behavior segment will have its value
 *    set to zero except for the segment at index n; if segment n also
 *    has the radio behavior, it will have its value set to one. When a
 *    radio-behavior segment is clicked, the value of the whole
 *    segmented view will be set to the segment's index. 
 *    
 *    The segmented view works in both compositing and non-compositing
 *    modes.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      The bounds of the view to be created. Can be NULL, in which
 *      case the view is created with CGRectZero bounds.
 *    
 *    outRef:
 *      A valid pointer to an HIViewRef. On successful completion of
 *      this routine, the destination HIViewRef will be filled with the
 *      HIViewRef of the newly created view. The view is initially
 *      invisible.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewCreate(
  const HIRect *  inBounds,       /* can be NULL */
  HIViewRef *     outRef)                                     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewSetSegmentCount()
 *  
 *  Summary:
 *    Sets the number of segments for the segmented view. Any previous
 *    segments beyond the new count will have their content released.
 *    All new segments beyond the previous count be initialized with
 *    the most basic settings possible: momentary, no attributes, zero
 *    value, enabled, no command, no label, no content, and
 *    auto-calculated content width. You should configure any new
 *    segments to match your needs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view for which the number of segments is to be
 *      set.
 *    
 *    inSegmentCount:
 *      A positive integer indicating how many segments the view is to
 *      have.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewSetSegmentCount(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentCount)                                 AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewGetSegmentCount()
 *  
 *  Summary:
 *    Gets the number of segments in the segmented view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view for which the number of segments is to be
 *      retrieved.
 *  
 *  Result:
 *    A UInt32 indicating the number of segments in the segmented view.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
HISegmentedViewGetSegmentCount(HIViewRef inSegmentedView)     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  Summary:
 *    HISegmentBehavior constants
 */
enum {

  /*
   * Pops back up after being pressed, just like a push button.
   */
  kHISegmentBehaviorMomentary   = 1,

  /*
   * Stays pressed until another segment with the radio behavior is
   * pressed. This makes the segment behave like a radio button. After
   * this segment is clicked, the segmented view's value will be
   * changed to this segment's one-based index.
   */
  kHISegmentBehaviorRadio       = 2,

  /*
   * Like a check box. When clicked, it toggles back and forth between
   * checked and unchecked states. This behavior is not implemented in
   * any version of Mac OS X, and this constant is deprecated.
   */
  kHISegmentBehaviorToggles     = 3,

  /*
   * After being pressed, this type of segment stays pressed until you
   * programatically unpress it. This behavior is not implemented in
   * any version of Mac OS X, and this constant is deprecated.
   */
  kHISegmentBehaviorSticky      = 4
};

typedef UInt32                          HISegmentBehavior;
#if !__LP64__
/*
 *  HISegmentedViewSetSegmentBehavior()
 *  
 *  Summary:
 *    Changes the behavior of an individual segment of a segmented
 *    view. By default, a segment has the kHISegmentBehaviorMomentary
 *    behavior.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment whose behavior you
 *      want to change.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose behavior you want to
 *      change. This must be a non-zero value that is no higher than
 *      the segmented view's current segment count.
 *    
 *    inBehavior:
 *      A HISegmentBehavior describing the behavior you wish the
 *      segment to have.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewSetSegmentBehavior(
  HIViewRef           inSegmentedView,
  UInt32              inSegmentIndexOneBased,
  HISegmentBehavior   inBehavior)                             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewGetSegmentBehavior()
 *  
 *  Summary:
 *    Returns the behavior of an individual segment of a segmented view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose behavior you desire.
 *      This must be a non-zero value that is no higher than the
 *      segmented view's current segment count.
 *  
 *  Result:
 *    A HISegmentBehavior describing the behavior of the given segment.
 *    If you pass an illegal segment index, the result is undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern HISegmentBehavior 
HISegmentedViewGetSegmentBehavior(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  Summary:
 *    HISegmentedView segment attributes
 *  
 *  Discussion:
 *    These attribute bits are for use with
 *    HISegmentedViewChangeSegmentAttributes and
 *    HISegmentedViewGetSegmentAttributes.
 */
enum {

  /*
   * Pass this to indicate no attributes at all.
   */
  kHISegmentNoAttributes        = 0,

  /*
   * If this attribute bit is set, the command that gets sent out when
   * the segment is clicked will be directed at the user focus instead
   * of up the segmented view's containment hierarchy.
   */
  kHISegmentSendCmdToUserFocus  = (1 << 0)
};

#if !__LP64__
/*
 *  HISegmentedViewChangeSegmentAttributes()
 *  
 *  Summary:
 *    Changes the attributes of an individual segment of a segmented
 *    view. By default, a segment has no attribute bits set.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment whose attributes you
 *      want to change.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose attributes you want to
 *      change. This must be a non-zero value that is no higher than
 *      the segmented view's current segment count.
 *    
 *    inAttributesToSet:
 *      The attribute bits you wish to set for the segment.
 *    
 *    inAttributesToClear:
 *      The attribute bits you wish to clear for the segment.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewChangeSegmentAttributes(
  HIViewRef    inSegmentedView,
  UInt32       inSegmentIndexOneBased,
  OptionBits   inAttributesToSet,
  OptionBits   inAttributesToClear)                           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewGetSegmentAttributes()
 *  
 *  Summary:
 *    Returns the attributes of an individual segment of a segmented
 *    view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose attributes you desire.
 *      This must be a non-zero value that is no higher than the
 *      segmented view's current segment count.
 *  
 *  Result:
 *    The attribute bits that are set/enabled for the given segment. If
 *    you pass an illegal segment index, the result is undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OptionBits 
HISegmentedViewGetSegmentAttributes(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewSetSegmentValue()
 *  
 *  Summary:
 *    Changes the value of an individual segment of a segmented view.
 *    This is only meaningful for segments with the sticky, toggles, or
 *    radio behaviors. If you set the value of momentary segments to
 *    something other than zero, the behavior is undefined.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment whose value you want
 *      to change.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose value you want to
 *      change. This must be a non-zero value that is no higher than
 *      the segmented view's current segment count.
 *    
 *    inValue:
 *      The value you wish the segment to have. Zero means
 *      unpressed/unselected and one means pressed/selected. Other
 *      values will result in undefined behavior.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewSetSegmentValue(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased,
  SInt32      inValue)                                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewGetSegmentValue()
 *  
 *  Summary:
 *    Determines the value of an individual segment of a segmented
 *    view. This is only meaningful for segments with the sticky,
 *    toggles, or radio behaviors. The value of a momentary segment is
 *    undefined.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose value you desire. This
 *      must be a non-zero value that is no higher than the segmented
 *      view's current segment count.
 *  
 *  Result:
 *    A SInt32 indicating the value of the given segment. If you pass
 *    an illegal segment index, the result is undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern SInt32 
HISegmentedViewGetSegmentValue(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewSetSegmentEnabled()
 *  
 *  Summary:
 *    Enables or disables an individual segment of a segmented view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment to enable or disable.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment to disable or enable. This
 *      must be a non-zero value that is no higher than the segmented
 *      view's current segment count.
 *    
 *    inEnabled:
 *      A Boolean indicating whether the segment is to become enabled
 *      or disabled.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewSetSegmentEnabled(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased,
  Boolean     inEnabled)                                      AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewIsSegmentEnabled()
 *  
 *  Summary:
 *    Tests an individual segment of a segmented view to see if it is
 *    enabled or disabled.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment to test. This must be a
 *      non-zero value that is no higher than the segmented view's
 *      current segment count.
 *  
 *  Result:
 *    True if the segment is enabled or false if the segment is
 *    disabled. If you pass an illegal segment index, the result is
 *    undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HISegmentedViewIsSegmentEnabled(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewSetSegmentCommand()
 *  
 *  Summary:
 *    Sets the command ID for the given segment. By default, the
 *    command is zero. If you set any non-zero command ID, the
 *    segmented view will send an HICommand whenever the segment is
 *    clicked. By default, the command is sent to the segmented view
 *    and up the containment hierarchy. You can request that the
 *    command start at the user focus instead by turning on the
 *    kHISegmentSendCmdToUserFocus attribute for the segment.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment whose command you wish
 *      to set.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose command you wish to
 *      set. This must be a non-zero value that is no higher than the
 *      segmented view's current segment count.
 *    
 *    inCommand:
 *      The command ID you wish to associate with the segment. A value
 *      of zero signifies "no command".
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewSetSegmentCommand(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased,
  UInt32      inCommand)                                      AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewGetSegmentCommand()
 *  
 *  Summary:
 *    Gets the command ID associated with the given segment.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment to query. This must be a
 *      non-zero value that is no higher than the segmented view's
 *      current segment count.
 *  
 *  Result:
 *    Returns the command ID associated with the segment. If you pass
 *    an illegal segment index, the result is undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern UInt32 
HISegmentedViewGetSegmentCommand(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewSetSegmentLabel()
 *  
 *  Summary:
 *    Sets the label string for the given segment. By default, a
 *    segment has no label string.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment whose label you wish
 *      to set.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose label you wish to set.
 *      This must be a non-zero value that is no higher than the
 *      segmented view's current segment count.
 *    
 *    inLabel:
 *      A CFStringRef with the text of the label. The segmented view
 *      will copy the string passed in. You may pass NULL or an empty
 *      CFStringRef if you wish to eliminate the label from the segment.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewSetSegmentLabel(
  HIViewRef     inSegmentedView,
  UInt32        inSegmentIndexOneBased,
  CFStringRef   inLabel)                                      AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewCopySegmentLabel()
 *  
 *  Summary:
 *    Gets the label associated with the given segment.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment to query. This must be a
 *      non-zero value that is no higher than the segmented view's
 *      current segment count.
 *    
 *    outLabel:
 *      On exit, outLabel will be a copy of the label associated with
 *      the segment; you must release this string. If there is no label
 *      associated with the segment, outLabel will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewCopySegmentLabel(
  HIViewRef      inSegmentedView,
  UInt32         inSegmentIndexOneBased,
  CFStringRef *  outLabel)                                    AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewSetSegmentContentWidth()
 *  
 *  Summary:
 *    Sets whether you want the segment to automatically calculate its
 *    own width or whether you want to determine the segment's width
 *    manually. The content width is the horizontal area taken up by a
 *    segment's label and/or image.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment whose content width
 *      you wish to set.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose content width you wish
 *      to set. This must be a non-zero value that is no higher than
 *      the segmented view's current segment count.
 *    
 *    inAutoCalculateWidth:
 *      A Boolean indicating whether you want the segment to calculate
 *      its own width. If you pass true, the inWidth parameter is
 *      ignored.
 *    
 *    inWidth:
 *      If you passed false in inAutoCalculateWidth, this parameter
 *      specifies the width you want to manually associate with the
 *      segment. If you pass a negative width, the behavior is
 *      undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewSetSegmentContentWidth(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased,
  Boolean     inAutoCalculateWidth,
  CGFloat     inWidth)                                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewGetSegmentContentWidth()
 *  
 *  Summary:
 *    Gets the content width of the given segment. This also optionally
 *    passes back a Boolean indicating whether the width was
 *    automatically calculated. The content width is the horizontal
 *    area taken up by a segment's label and/or image.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment to query. This must be a
 *      non-zero value that is no higher than the segmented view's
 *      current segment count.
 *    
 *    outAutoCalculated:
 *      On exit, this is a Boolean indicating whether the width was
 *      automatically calculated. You may pass NULL if you don't need
 *      this information.
 *  
 *  Result:
 *    Returns the width of the content for the given segment. If you
 *    pass an illegal segment index, the result is undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern CGFloat 
HISegmentedViewGetSegmentContentWidth(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased,
  Boolean *   outAutoCalculated)                              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewSetSegmentImage()
 *  
 *  Summary:
 *    Sets or clears the image associated with a given segment.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment whose image you wish
 *      to set.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment whose image you wish to set.
 *      This must be a non-zero value that is no higher than the
 *      segmented view's current segment count.
 *    
 *    inImage:
 *      An HIViewImageContentInfo structure with the image information
 *      for the given segment. Prior to Mac OS X 10.5, segments only
 *      support three types of image content: kHIViewContentNone (no
 *      image), kHIViewContentIconRef, and kHIViewContentCGImageRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewSetSegmentImage(
  HIViewRef                       inSegmentedView,
  UInt32                          inSegmentIndexOneBased,
  const HIViewImageContentInfo *  inImage)                    AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewGetSegmentImageContentType()
 *  
 *  Summary:
 *    Gets the type of image content drawn by the given segment. You
 *    will need to call this before calling
 *    HISegmentedViewCopySegmentImage so you know what type of image
 *    content to request from the latter API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment to query. This must be a
 *      non-zero value that is no higher than the segmented view's
 *      current segment count.
 *  
 *  Result:
 *    Returns the image content type of the image drawn by the given
 *    segment. If you pass an illegal segment index, the result is
 *    undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern HIViewImageContentType 
HISegmentedViewGetSegmentImageContentType(
  HIViewRef   inSegmentedView,
  UInt32      inSegmentIndexOneBased)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISegmentedViewCopySegmentImage()
 *  
 *  Summary:
 *    Gives you a copy of the image (if any) drawn by the given
 *    segment. You are responsible for releasing any image passed back
 *    by this function. You request the image by asking for a
 *    particular type of image. If the segment isn't using the
 *    requested type of image, an error will be returned. If you wish
 *    to know the actual type of image displayed by the segment, you
 *    can call HISegmentedViewGetSegmentImageContentType.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSegmentedView:
 *      The segmented view that owns the segment being queried.
 *    
 *    inSegmentIndexOneBased:
 *      The one-based index of the segment to query. This must be a
 *      non-zero value that is no higher than the segmented view's
 *      current segment count.
 *    
 *    ioImage:
 *      On entry, you must fill out the contentType field of this
 *      structure with the type of image you desire. On exit, if that
 *      type of image is used by the segment, the appropriate field of
 *      the union will be filled in with a copy of the image. You are
 *      responsible for releasing the image.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISegmentedViewCopySegmentImage(
  HIViewRef                 inSegmentedView,
  UInt32                    inSegmentIndexOneBased,
  HIViewImageContentInfo *  ioImage)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HISEGMENTEDVIEW__ */

