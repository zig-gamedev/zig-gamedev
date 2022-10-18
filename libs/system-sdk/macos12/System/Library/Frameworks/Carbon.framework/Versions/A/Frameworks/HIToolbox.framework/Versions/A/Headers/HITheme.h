/*
     File:       HIToolbox/HITheme.h
 
     Contains:   HIToolbox HITheme interfaces.
 
     Copyright:  © 1994-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HITHEME__
#define __HITHEME__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __APPEARANCE__
#include <HIToolbox/Appearance.h>
#endif

#ifndef __CARBONEVENTS__
#include <HIToolbox/CarbonEvents.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/* -------------------------------------------------------------------------- */
/*  HIThemeOrientation information                                            */
/* -------------------------------------------------------------------------- */

/*
 */
enum {

  /*
   * The passed context has an origin at the top left. This is the
   * default of a context passed to you by HIToolbox.
   */
  kHIThemeOrientationNormal     = 0,

  /*
   * The passed context has an origin at the bottom left. This is the
   * default for a context you create.
   */
  kHIThemeOrientationInverted   = 1
};

typedef UInt32                          HIThemeOrientation;
/* -------------------------------------------------------------------------- */
/*  Splitter types                                                            */
/* -------------------------------------------------------------------------- */

/*
 */
enum {

  /*
   * Draw the splitter with its normal appearance.
   */
  kHIThemeSplitterAdornmentNone = 0,

  /*
   * Draw the splitter with its metal appearance.
   */
  kHIThemeSplitterAdornmentMetal = 1
};

typedef UInt32                          HIThemeSplitterAdornment;
/* -------------------------------------------------------------------------- */
/*  Window Grow Box                                                           */
/* -------------------------------------------------------------------------- */

/*
 */
enum {

  /*
   * The grow box corner for a window that has no scroll bars.
   */
  kHIThemeGrowBoxKindNormal     = 0,

  /*
   * The grow box corner for a window that has no grow box. This sounds
   * paradoxical, but this type of grow box, formerly known as the
   * "NoGrowBox" is used to fill in the corner left blank by the
   * intersection of a horizontal and a vertical scroll bar.
   */
  kHIThemeGrowBoxKindNone       = 1
};

typedef UInt32                          HIThemeGrowBoxKind;

/*
 */
enum {

  /*
   * Draw the grow box for normal windows.
   */
  kHIThemeGrowBoxSizeNormal     = 0,

  /*
   * Draw the smaller grow box for utility or floating windows.
   */
  kHIThemeGrowBoxSizeSmall      = 1
};

typedef UInt32                          HIThemeGrowBoxSize;

/*
 */
enum {

  /*
   * The group box is drawn with the primary variant.
   */
  kHIThemeGroupBoxKindPrimary   = 0,

  /*
   * The group box is drawn with the secondary variant.
   */
  kHIThemeGroupBoxKindSecondary = 1,

  /*
   * The group box is drawn with the primary variant. This group box
   * draws opaque. This does not match the Mac OS X 10.3 appearance
   * 100%, as the boxes should be transparent, but draws this way for
   * the sake of compatibility. Please update to use the newer
   * transparent variant.
   */
  kHIThemeGroupBoxKindPrimaryOpaque = 3,

  /*
   * The group box is drawn with the secondary variant. This group box
   * draws opaque. This does not match the Mac OS X 10.3 appearance
   * 100%, as the boxes should be transparent, but draws this way for
   * the sake of compatibility. Please update to use the newer
   * transparent variant.
   */
  kHIThemeGroupBoxKindSecondaryOpaque = 4
};

typedef UInt32                          HIThemeGroupBoxKind;

/*
 */
enum {

  /*
   * A header drawn above window content that has no top border of its
   * own. (i.e. the same as the status bar in an icon view Finder
   * window).
   */
  kHIThemeHeaderKindWindow      = 0,

  /*
   * A header drawn above window content that has a top border of its
   * own. (i.e. the same as the status bar in an list view Finder
   * window).
   */
  kHIThemeHeaderKindList        = 1
};

typedef UInt32                          HIThemeHeaderKind;

/*
 */
enum {

  /*
   * The default sized square text field (like Edit Text).
   */
  kHIThemeFrameTextFieldSquare  = 0,
  kHIThemeFrameListBox          = 1,

  /*
   * The standard sized round text field, as typically used for search
   * fields. Available on Mac OS X 10.3 and later.
   */
  kHIThemeFrameTextFieldRound   = 1000,

  /*
   * The small size round text field, as typically used for search
   * fields. Available on Mac OS X 10.3 and later.
   */
  kHIThemeFrameTextFieldRoundSmall = 1001,

  /*
   * The mini size round text field, as typically used for search
   * fields. Available on Mac OS X 10.3 and later.
   */
  kHIThemeFrameTextFieldRoundMini = 1002
};

typedef UInt32                          HIThemeFrameKind;

/*
 */
enum {

  /*
   * Indicates that a menu title should be drawn in a condensed
   * appearance. This constant is used in the
   * HIThemeMenuTitleDrawInfo.attributes field.
   */
  kHIThemeMenuTitleDrawCondensed = 1 << 0
};

/* -------------------------------------------------------------------------- */
/*  DrawInfo                                                                  */
/* -------------------------------------------------------------------------- */

/*
 *  HIScrollBarTrackInfo
 *  
 *  Summary:
 *    Drawing parameters passed to scroll bar drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIScrollBarTrackInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeTrackEnableState for the scroll bar to be drawn.
   */
  ThemeTrackEnableState  enableState;

  /*
   * The ThemeTrackPressState for the scroll bar to be drawn.
   */
  ThemeTrackPressState  pressState;

  /*
   * The view range size.
   */
  CGFloat             viewsize;
};
typedef struct HIScrollBarTrackInfo     HIScrollBarTrackInfo;

/*
 *  HIThemeTrackDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to track drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3, but based on legacy TrackDrawInfo.
 */
struct HIThemeTrackDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;                /* current version is 0 */

  /*
   * The ThemeTrackKind of the track being drawn or measured.
   */
  ThemeTrackKind      kind;                   /* what kind of track this info is for */

  /*
   * An HIRect describing the bounds of the track being drawn or
   * measured.
   */
  HIRect              bounds;                 /* track basis rectangle */

  /*
   * The minimum allowable value for the track being drawn or measured.
   */
  SInt32              min;                    /* min track value */

  /*
   * The maximum allowable value for the track being drawn or measured.
   */
  SInt32              max;                    /* max track value */

  /*
   * The value for the track being drawn or measured.
   */
  SInt32              value;                  /* current thumb value */

  /*
   * Leave this reserved field set to 0.
   */
  UInt32              reserved;


  /*
   * A set of ThemeTrackAttributes for the track to be drawn or
   * measured.
   */
  ThemeTrackAttributes  attributes;           /* various track attributes */

  /*
   * A ThemeTrackEnableState describing the state of the track to be
   * drawn or measured.
   */
  ThemeTrackEnableState  enableState;         /* enable state */

  /*
   * Leave this reserved field set to 0.
   */
  UInt8               filler1;

  union {
    ScrollBarTrackInfo  scrollbar;
    SliderTrackInfo     slider;
    ProgressTrackInfo   progress;
  }                       trackInfo;
};
typedef struct HIThemeTrackDrawInfo     HIThemeTrackDrawInfo;

/*
 *  HIThemeAnimationTimeInfo
 *  
 *  Summary:
 *    Time parameters passed to button drawing and measuring theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeAnimationTimeInfo {

  /*
   * The CFAbsoluteTime of the beginning of the animation of the
   * button.  This only applies to buttons that animate -- currently
   * only kThemePushButton.  All other buttons will ignore this field. 
   * If there is to be no animation, set this field to 0.
   */
  CFAbsoluteTime      start;

  /*
   * The CFAbsoluteTime of the current animation frame of the button. 
   * This only applies to buttons that animate -- currently only
   * kThemePushButton.  All other buttons will ignore this field.  If
   * there is to be no animation, set this field to 0.
   */
  CFAbsoluteTime      current;
};
typedef struct HIThemeAnimationTimeInfo HIThemeAnimationTimeInfo;

/*
 *  HIThemeAnimationFrameInfo
 *  
 *  Summary:
 *    Frame parameters passed to button drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeAnimationFrameInfo {

  /*
   * The index of the frame of the animation to draw. If the index is
   * greater that the maximum number of animation frames, it will be
   * modded to calculate which frame to draw.
   */
  UInt32              index;
};
typedef struct HIThemeAnimationFrameInfo HIThemeAnimationFrameInfo;

/*
 *  HIThemeButtonDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to button drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeButtonDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState of the button being drawn or measured.
   */
  ThemeDrawState      state;

  /*
   * A ThemeButtonKind indicating the type of button to be drawn.
   */
  ThemeButtonKind     kind;

  /*
   * The ThemeButtonValue of the button being drawn or measured.
   */
  ThemeButtonValue    value;

  /*
   * The ThemeButtonAdornment(s) with which the button is being drawn
   * or measured.
   */
  ThemeButtonAdornment  adornment;
  union {
    HIThemeAnimationTimeInfo  time;
    HIThemeAnimationFrameInfo  frame;
  }                       animation;
};
typedef struct HIThemeButtonDrawInfo    HIThemeButtonDrawInfo;
typedef HIThemeButtonDrawInfo *         HIThemeButtonDrawInfoPtr;

/*
 *  HIThemeSplitterDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to splitter drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeSplitterDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState of the splitter being drawn or measured.
   */
  ThemeDrawState      state;

  /*
   * The HIThemeSplitterAdornments of the splitter being drawn or
   * measured.
   */
  HIThemeSplitterAdornment  adornment;
};
typedef struct HIThemeSplitterDrawInfo  HIThemeSplitterDrawInfo;
typedef HIThemeSplitterDrawInfo *       HIThemeSplitterDrawInfoPtr;

/*
 */
enum {

  /*
   * No tab adornments are to be drawn.
   */
  kHIThemeTabAdornmentNone      = 0,

  /*
   * A focus ring is to be drawn around the tab.
   */
  kHIThemeTabAdornmentFocus     = (1 << 2), /* to match button focus adornment */

  /*
   * If available, a leading separator is to be drawn on the tab,
   * either to the left or above, depending on orientation. Note that
   * tabs overlap and if the separators are drawn over top each other,
   * the shadows multiply undesirably. New in Mac OS X 10.4.
   */
  kHIThemeTabAdornmentLeadingSeparator = (1 << 3),

  /*
   * If available, a right separator is to be drawn on the tab, either
   * to the right or below, depending on the orientation. Note that
   * tabs overlap and if the separators are drawn over top each other,
   * the shadows multiply undesirably. New in Mac OS X 10.4.
   */
  kHIThemeTabAdornmentTrailingSeparator = (1 << 4)
};

typedef UInt32                          HIThemeTabAdornment;

/*
 *  Summary:
 *    These values are similar to kControlSize constants for
 *    convenience.
 */
enum {

  /*
   * The tabs are normal (large) sized.
   */
  kHIThemeTabSizeNormal         = 0,

  /*
   * The tabs are drawn as the small variant.
   */
  kHIThemeTabSizeSmall          = 1,

  /*
   * The tabs are drawn as the mini variant.
   */
  kHIThemeTabSizeMini           = 3
};

typedef UInt32                          HIThemeTabSize;

/*
 *  Summary:
 *    Available values for HIThemeTabPosition. These are positions of
 *    the tabs within the tab control. New in Mac OS X 10.4.
 */
enum {

  /*
   * The first position of a tab control. Left or top tab.
   */
  kHIThemeTabPositionFirst      = 0,

  /*
   * A middle tab.
   */
  kHIThemeTabPositionMiddle     = 1,

  /*
   * The last position of a tab control. Right or bottom tab.
   */
  kHIThemeTabPositionLast       = 2,

  /*
   * The only position of a tab control. It is simultaneously first and
   * last. You know, only. There is only one tab. It looks pretty much
   * like a button. Please don't use this if you can avoid it. It's
   * ugly.
   */
  kHIThemeTabPositionOnly       = 3
};

typedef UInt32                          HIThemeTabPosition;

/*
 *  Summary:
 *    Available values for HIThemeTabKind.
 */
enum {
  kHIThemeTabKindNormal         = 0
};

typedef UInt32                          HIThemeTabKind;

/*
 *  HIThemeTabDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to tab drawing and measuring theme APIs.
 *  
 *  Discussion:
 *    In Mac OS X 10.4, added kind and position fields.
 */
struct HIThemeTabDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 1.
   */
  UInt32              version;

  /*
   * A ThemeTabStyle describing the style of the tab to be drawn.
   */
  ThemeTabStyle       style;

  /*
   * A ThemeTabDirection describing the side on which the tab is being
   * drawn.
   */
  ThemeTabDirection   direction;

  /*
   * An HIThemeTabSize indicating what size of tab to draw.
   */
  HIThemeTabSize      size;

  /*
   * An HIThemeTabAdornment describing any additional adornments that
   * are to be drawn on the tab.
   */
  HIThemeTabAdornment  adornment;             /* various tab attributes */

  /*
   * An HIThemeTabKind indicating what kind of tab to draw.
   */
  HIThemeTabKind      kind;

  /*
   * The HIThemeTabPositions of the tab to be drawn or measured.
   */
  HIThemeTabPosition  position;
};
typedef struct HIThemeTabDrawInfo       HIThemeTabDrawInfo;

/*
 *  HIThemeTabDrawInfoVersionZero
 *  
 *  Summary:
 *    This structure is left here as a reference to the previous
 *    version of the tab drawing parameters. Please use the current
 *    version.
 *  
 *  Discussion:
 *    Shipped with Mac OS X 10.3.
 */
struct HIThemeTabDrawInfoVersionZero {
  UInt32              version;
  ThemeTabStyle       style;
  ThemeTabDirection   direction;
  HIThemeTabSize      size;
  HIThemeTabAdornment  adornment;             /* various tab attributes */
};
typedef struct HIThemeTabDrawInfoVersionZero HIThemeTabDrawInfoVersionZero;

/*
 *  Summary:
 *    Values for HIThemeTabPaneAdornment.
 */
enum {
  kHIThemeTabPaneAdornmentNormal = 0
};


typedef UInt32                          HIThemeTabPaneAdornment;

/*
 *  HIThemeTabPaneDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to tab pane drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    In Mac OS X 10.4, added kind and adornment fields.
 */
struct HIThemeTabPaneDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 1.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the tab pane to be drawn.
   */
  ThemeDrawState      state;

  /*
   * A ThemeTabDirection describing on which side of the pane the tabs
   * will be drawn.
   */
  ThemeTabDirection   direction;

  /*
   * An HIThemeTabSize indicating what size of tab pane to draw.
   */
  HIThemeTabSize      size;

  /*
   * An HIThemeTabKind indicating what kind of tab to draw this pane
   * for.
   */
  HIThemeTabKind      kind;

  /*
   * An HIThemeTabPaneAdornment describing any additional adornments
   * that are to be drawn on the tab pane.
   */
  HIThemeTabPaneAdornment  adornment;
};
typedef struct HIThemeTabPaneDrawInfo   HIThemeTabPaneDrawInfo;

/*
 *  HIThemeTabPaneDrawInfoVersionZero
 *  
 *  Summary:
 *    This structure is left here as a reference to the previous
 *    version of the tab pane drawing parameters. Please use the
 *    current version.
 *  
 *  Discussion:
 *    Shipped with Mac OS X 10.3.
 */
struct HIThemeTabPaneDrawInfoVersionZero {
  UInt32              version;
  ThemeDrawState      state;
  ThemeTabDirection   direction;
  HIThemeTabSize      size;
};
typedef struct HIThemeTabPaneDrawInfoVersionZero HIThemeTabPaneDrawInfoVersionZero;

/*
 */
enum {

  /*
   * Available in Mac OS X 10.3 and later. Valid fields for this
   * version are version and menuType.
   */
  kHIThemeMenuDrawInfoVersionZero = 0,

  /*
   * Available in Mac OS X 10.5 and later. Valid fields for this
   * version are all those in the zero version as well as the
   * menuDirection field.
   */
  kHIThemeMenuDrawInfoVersionOne = 1001
};


/*
 *  HIThemeMenuDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to menu drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3; revised in Mac OS X 10.5.
 */
struct HIThemeMenuDrawInfo {

  /*
   * The version of this data structure. Use
   * kHIThemeMenuDrawInfoVersionZero or kHIThemeMenuDrawInfoVersionOne.
   */
  UInt32              version;

  /*
   * A ThemeMenuType indicating which type of menu is to be drawn.
   */
  ThemeMenuType       menuType;

  /*
   * Must be zero.
   */
  unsigned long       reserved1;

  /*
   * Must be zero.
   */
  CGFloat             reserved2;

  /*
   * kHIMenuRightDirection or kHIMenuLeftDirection as declared in
   * <CarbonEvents.h>. Only interpreted if the version is
   * kHIThemeMenuDrawInfoVersionOne and the menu type is hierarchical.
   */
  UInt32              menuDirection;

  /*
   * Must be zero.
   */
  CGFloat             reserved3;

  /*
   * Must be zero.
   */
  CGFloat             reserved4;
};
typedef struct HIThemeMenuDrawInfo      HIThemeMenuDrawInfo;
typedef HIThemeMenuDrawInfo *           HIThemeMenuDrawInfoPtr;

/*
 *  HIThemeMenuDrawInfoVersionZero
 *  
 *  Summary:
 *    Drawing parameters passed to menu drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeMenuDrawInfoVersionZero {

  /*
   * The version of this data structure. Always 0.
   */
  UInt32              version;

  /*
   * A ThemeMenuType indicating which type of menu is to be drawn.
   */
  ThemeMenuType       menuType;
};
typedef struct HIThemeMenuDrawInfoVersionZero HIThemeMenuDrawInfoVersionZero;
typedef HIThemeMenuDrawInfoVersionZero * HIThemeMenuDrawInfoVersionZeroPtr;

/*
 *  HIThemeMenuItemDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to menu item drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeMenuItemDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * A ThemeMenuItemType indicating what type of menu item is to be
   * drawn.
   */
  ThemeMenuItemType   itemType;

  /*
   * The ThemeMenuState of the menu item to be drawn.
   */
  ThemeMenuState      state;
};
typedef struct HIThemeMenuItemDrawInfo  HIThemeMenuItemDrawInfo;
typedef HIThemeMenuItemDrawInfo *       HIThemeMenuItemDrawInfoPtr;

/*
 *  HIThemeFrameDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to frame drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeFrameDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The HIThemeFrameKind of the frame to be drawn.
   */
  HIThemeFrameKind    kind;

  /*
   * The ThemeDrawState of the frame to be drawn.
   */
  ThemeDrawState      state;

  /*
   * A Boolean indicating whether the frame is to be drawn with focus
   * or without.
   */
  Boolean             isFocused;
};
typedef struct HIThemeFrameDrawInfo     HIThemeFrameDrawInfo;
typedef HIThemeFrameDrawInfo *          HIThemeFrameDrawInfoPtr;

/*
 *  HIThemeGroupBoxDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to group box drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeGroupBoxDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the group box to be drawn.
   */
  ThemeDrawState      state;

  /*
   * An HIThemeGroupBoxKind indicating which type of group box is to be
   * drawn.
   */
  HIThemeGroupBoxKind  kind;
};
typedef struct HIThemeGroupBoxDrawInfo  HIThemeGroupBoxDrawInfo;
typedef HIThemeGroupBoxDrawInfo *       HIThemeGroupBoxDrawInfoPtr;

/*
 *  HIThemeGrabberDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to grabber drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeGrabberDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the grabber to be drawn.
   */
  ThemeDrawState      state;
};
typedef struct HIThemeGrabberDrawInfo   HIThemeGrabberDrawInfo;
typedef HIThemeGrabberDrawInfo *        HIThemeGrabberDrawInfoPtr;

/*
 *  HIThemePlacardDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to placard drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemePlacardDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the placard to be drawn.
   */
  ThemeDrawState      state;
};
typedef struct HIThemePlacardDrawInfo   HIThemePlacardDrawInfo;
typedef HIThemePlacardDrawInfo *        HIThemePlacardDrawInfoPtr;

/*
 *  HIThemeHeaderDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to header drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeHeaderDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the header to be drawn.
   */
  ThemeDrawState      state;

  /*
   * The HIThemeHeaderKind for the header to be drawn.
   */
  HIThemeHeaderKind   kind;
};
typedef struct HIThemeHeaderDrawInfo    HIThemeHeaderDrawInfo;
typedef HIThemeHeaderDrawInfo *         HIThemeHeaderDrawInfoPtr;

/*
 *  HIThemeMenuBarDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to HIThemeDrawMenuBarBackground.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeMenuBarDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeMenuBarState for the menu bar to be drawn.
   */
  ThemeMenuBarState   state;

  /*
   * The attributes of the menu bar to be drawn.
   */
  OptionBits          attributes;
};
typedef struct HIThemeMenuBarDrawInfo   HIThemeMenuBarDrawInfo;
typedef HIThemeMenuBarDrawInfo *        HIThemeMenuBarDrawInfoPtr;

/*
 *  HIThemeMenuTitleDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to HIThemeDrawMenuTitle.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeMenuTitleDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeMenuState for the menu title to be drawn.
   */
  ThemeMenuState      state;

  /*
   * The attributes of the menu title to be drawn. Must be either 0 or
   * kHIThemeMenuTitleDrawCondensed.
   */
  OptionBits          attributes;

  /*
   * The border space between the menu title rect and the menu title
   * text when the menu title spacing is being condensed. This field is
   * only observed by the Appearance Manager when the attributes field
   * contains kHIThemeMenuTitleDrawCondensed. The valid values for this
   * field range from the value returned by GetThemeMenuTitleExtra(
   * &extra, false ) to the value returned by GetThemeMenuTitleExtra(
   * &extra, true ). You may pass 0 in this field to use the minimum
   * condensed title extra.
   */
  CGFloat             condensedTitleExtra;
};
typedef struct HIThemeMenuTitleDrawInfo HIThemeMenuTitleDrawInfo;
typedef HIThemeMenuTitleDrawInfo *      HIThemeMenuTitleDrawInfoPtr;

/*
 *  HIThemeTickMarkDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to tick mark drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeTickMarkDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the tick mark to be drawn.
   */
  ThemeDrawState      state;
};
typedef struct HIThemeTickMarkDrawInfo  HIThemeTickMarkDrawInfo;
typedef HIThemeTickMarkDrawInfo *       HIThemeTickMarkDrawInfoPtr;

/*
 *  HIThemeWindowDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to window drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3, but based on legacy ThemeWindowMetrics.
 */
struct HIThemeWindowDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * A ThemeDrawState which describes the state of the window to be
   * drawn.
   */
  ThemeDrawState      state;

  /*
   * A ThemeWindowType specifying the type of window to be drawn.
   */
  ThemeWindowType     windowType;

  /*
   * The ThemeWindowAttributes describing the window to be drawn.
   */
  ThemeWindowAttributes  attributes;

  /*
   * The height of the title of the window.
   */
  CGFloat             titleHeight;

  /*
   * The width of the title of the window.
   */
  CGFloat             titleWidth;
};
typedef struct HIThemeWindowDrawInfo    HIThemeWindowDrawInfo;
typedef HIThemeWindowDrawInfo *         HIThemeWindowDrawInfoPtr;

/*
 *  HIThemeWindowWidgetDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to window widget drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3, but based on legacy ThemeWindowMetrics.
 */
struct HIThemeWindowWidgetDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * A ThemeDrawState which describes the state of the widget to be
   * drawn.
   */
  ThemeDrawState      widgetState;

  /*
   * A ThemeTitleBarWidget specifying the type of window widget to be
   * drawn.
   */
  ThemeTitleBarWidget  widgetType;

  /*
   * A ThemeDrawState which describes the state of the window for which
   * the widget is to be drawn.
   */
  ThemeDrawState      windowState;

  /*
   * A ThemeWindowType specifying the type of window to be drawn.
   */
  ThemeWindowType     windowType;

  /*
   * The ThemeWindowAttributes describing the window to be drawn.
   */
  ThemeWindowAttributes  attributes;

  /*
   * The height of the title of the window.
   */
  CGFloat             titleHeight;

  /*
   * The width of the title of the window.
   */
  CGFloat             titleWidth;
};
typedef struct HIThemeWindowWidgetDrawInfo HIThemeWindowWidgetDrawInfo;
typedef HIThemeWindowWidgetDrawInfo *   HIThemeWindowWidgetDrawInfoPtr;

/*
 *  HIThemeSeparatorDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to separator drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeSeparatorDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the separator to be drawn.
   */
  ThemeDrawState      state;
};
typedef struct HIThemeSeparatorDrawInfo HIThemeSeparatorDrawInfo;
typedef HIThemeSeparatorDrawInfo *      HIThemeSeparatorDrawInfoPtr;

/*
 *  HIThemeScrollBarDelimitersDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to separator drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeScrollBarDelimitersDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the separator to be drawn.
   */
  ThemeDrawState      state;

  /*
   * A ThemeWindowType specifying the type of window for which to draw
   * the delimiters.
   */
  ThemeWindowType     windowType;

  /*
   * The ThemeWindowAttributes of the window for which the scroll bar
   * delimters are to be drawn.
   */
  ThemeWindowAttributes  attributes;
};
typedef struct HIThemeScrollBarDelimitersDrawInfo HIThemeScrollBarDelimitersDrawInfo;
typedef HIThemeScrollBarDelimitersDrawInfo * HIThemeScrollBarDelimitersDrawInfoPtr;

/*
 *  HIThemeChasingArrowsDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to chasing arrows drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeChasingArrowsDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the chasing arrows to be drawn.
   */
  ThemeDrawState      state;

  /*
   * A UInt32 used to calculate which frame of the chasing arrow
   * animation is to be drawn.
   */
  UInt32              index;
};
typedef struct HIThemeChasingArrowsDrawInfo HIThemeChasingArrowsDrawInfo;
typedef HIThemeChasingArrowsDrawInfo *  HIThemeChasingArrowsDrawInfoPtr;

/*
 *  HIThemePopupArrowDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to popup arrow drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemePopupArrowDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the popup arrow to be drawn.
   */
  ThemeDrawState      state;

  /*
   * A ThemeArrowOrientation for the orientation of the popup arrow to
   * be drawn.
   */
  ThemeArrowOrientation  orientation;

  /*
   * A ThemePopupArrowSize for the size of the popup arrow to be drawn.
   */
  ThemePopupArrowSize  size;
};
typedef struct HIThemePopupArrowDrawInfo HIThemePopupArrowDrawInfo;
typedef HIThemePopupArrowDrawInfo *     HIThemePopupArrowDrawInfoPtr;

/*
 *  HIThemeGrowBoxDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to grow box drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3.
 */
struct HIThemeGrowBoxDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the grow box to be drawn.
   */
  ThemeDrawState      state;

  /*
   * A ThemeGrowBoxKind indicating in which kind of grow box to draw.
   */
  HIThemeGrowBoxKind  kind;

  /*
   * A ThemeGrowDirection indicating in which direction the window will
   * grow.
   */
  ThemeGrowDirection  direction;

  /*
   * An HIThemeGrowBoxSize describing the size of the grow box to draw.
   */
  HIThemeGrowBoxSize  size;
};
typedef struct HIThemeGrowBoxDrawInfo   HIThemeGrowBoxDrawInfo;
typedef HIThemeGrowBoxDrawInfo *        HIThemeGrowBoxDrawInfoPtr;

/*
 *  HIThemeBackgroundDrawInfo
 *  
 *  Discussion:
 *    New in Mac OS X 10.3, but based on legacy TrackDrawInfo.
 */
struct HIThemeBackgroundDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState of the background to be drawn.
   */
  ThemeDrawState      state;

  /*
   * The ThemeBackgroundKind with which to fill the background.
   */
  ThemeBackgroundKind  kind;
};
typedef struct HIThemeBackgroundDrawInfo HIThemeBackgroundDrawInfo;
typedef HIThemeBackgroundDrawInfo *     HIThemeBackgroundDrawInfoPtr;
/* -------------------------------------------------------------------------- */
/*  Buttons                                                                   */
/* -------------------------------------------------------------------------- */
/*
 *  HIThemeDrawButton()
 *  
 *  Summary:
 *    Draw a themed button.
 *  
 *  Discussion:
 *    This generic button drawing theme primitive draws not just a push
 *    button, but all of the kinds of buttons described by
 *    ThemeButtonKind.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      The HIRect in which to draw.  Note that this API may draw
 *      outside of its bounds.
 *    
 *    inDrawInfo:
 *      An HIThemeButtonDrawInfo describing the button that will be
 *      drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *    
 *    outLabelRect:
 *      A pointer to an HIRect into which to put the bounds of the
 *      label rect.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawButton(
  const HIRect *                 inBounds,
  const HIThemeButtonDrawInfo *  inDrawInfo,
  CGContextRef                   inContext,
  HIThemeOrientation             inOrientation,
  HIRect *                       outLabelRect)        /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetButtonShape()
 *  
 *  Summary:
 *    Get a shape of a themed button.
 *  
 *  Discussion:
 *    This API was mistakenly named as a "Get" API. It behaves as
 *    "Copy" API. THE CALLER IS RESPONSIBLE FOR RELEASING THE RETURNED
 *    SHAPE.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      An HIRect indicating where the button would be drawn.
 *    
 *    inDrawInfo:
 *      An HIThemeButtonDrawInfo describing the button that would be
 *      drawn.
 *    
 *    outShape:
 *      A pointer to an HIShapeRef which will be set to the shape of
 *      the button that would be drawn. It needs to be released by the
 *      caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetButtonShape(
  const HIRect *                 inBounds,
  const HIThemeButtonDrawInfo *  inDrawInfo,
  HIShapeRef *                   outShape)                    AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetButtonContentBounds()
 *  
 *  Summary:
 *    Get the bounds of a themed button's content.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      An HIRect indicating where the button would be drawn.
 *    
 *    inDrawInfo:
 *      An HIThemeButtonDrawInfo describing the button that would be
 *      drawn.
 *    
 *    outBounds:
 *      A pointer to an HIRect in which will be returned the rectangle
 *      of the button content bounds.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetButtonContentBounds(
  const HIRect *                 inBounds,
  const HIThemeButtonDrawInfo *  inDrawInfo,
  HIRect *                       outBounds)                   AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetButtonBackgroundBounds()
 *  
 *  Summary:
 *    Get the bounds of the background of a themed button.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      An HIRect indicating where the button would be drawn.
 *    
 *    inDrawInfo:
 *      An HIThemeButtonDrawInfo describing the button that would be
 *      drawn.
 *    
 *    outBounds:
 *      A pointer to an HIRect in which will be returned the rectangle
 *      of the button background bounds.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetButtonBackgroundBounds(
  const HIRect *                 inBounds,
  const HIThemeButtonDrawInfo *  inDrawInfo,
  HIRect *                       outBounds)                   AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawChasingArrows()
 *  
 *  Summary:
 *    Draw themed chasing arrows.
 *  
 *  Discussion:
 *    Draw a frame from the chasing arrows animation.  The animation
 *    frame is based on a modulo value calculated from the index.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      An HIRect indicating where the chasing arrows are to be drawn.
 *    
 *    inDrawInfo:
 *      An HIThemeChasingArrowsDrawInfo describing the chasing arrows
 *      to be drawn or measured.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawChasingArrows(
  const HIRect *                        inBounds,
  const HIThemeChasingArrowsDrawInfo *  inDrawInfo,
  CGContextRef                          inContext,
  HIThemeOrientation                    inOrientation)        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawPopupArrow()
 *  
 *  Summary:
 *    Draws a themed popup arrow.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      An HIThemePopupArrowDrawInfo describing the popup arrow to be
 *      drawn or measured.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawPopupArrow(
  const HIRect *                     inBounds,
  const HIThemePopupArrowDrawInfo *  inDrawInfo,
  CGContextRef                       inContext,
  HIThemeOrientation                 inOrientation)           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/* -------------------------------------------------------------------------- */
/*  Menus                                                                     */
/* -------------------------------------------------------------------------- */
/*
 *  HIThemeDrawMenuBarBackground()
 *  
 *  Summary:
 *    Draws the menu bar background for a given area.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      The HIThemeMenuBarDrawInfo of the menu bar to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawMenuBarBackground(
  const HIRect *                  inBounds,
  const HIThemeMenuBarDrawInfo *  inDrawInfo,
  CGContextRef                    inContext,
  HIThemeOrientation              inOrientation)              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawMenuTitle()
 *  
 *  Summary:
 *    Draws the menu title background for a menu.
 *  
 *  Discussion:
 *    This API draws the background of a menu title. It does not draw
 *    the menu title text; it is the caller's responsibility to draw
 *    the text after this API has returned. The text should be drawn
 *    into the bounds returned in the outLabelRect parameter; the
 *    caller should ensure that the text is not drawn outside of those
 *    bounds, either by truncating or clipping the text.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inMenuBarRect:
 *      An HIRect indicating the bounds of the whole menu bar for which
 *      the menu title is to be drawn.
 *    
 *    inTitleRect:
 *      An HIRect for the bounds of the menu title to be drawn.
 *    
 *    inDrawInfo:
 *      The HIThemeMenuTitleDrawInfo of the menu title to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *    
 *    outLabelRect:
 *      On exit, contains the bounds in which the menu title text
 *      should be drawn. May be NULL if you don't need this information.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawMenuTitle(
  const HIRect *                    inMenuBarRect,
  const HIRect *                    inTitleRect,
  const HIThemeMenuTitleDrawInfo *  inDrawInfo,
  CGContextRef                      inContext,
  HIThemeOrientation                inOrientation,
  HIRect *                          outLabelRect)        /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawMenuBackground()
 *  
 *  Summary:
 *    Draws the theme menu background in a rectangle.  This API may
 *    draw outside of the specified rectangle.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inMenuRect:
 *      An HIRect indicating the bounds of the whole menu for which the
 *      background is to be drawn.
 *    
 *    inMenuDrawInfo:
 *      An HIThemeMenuDrawInfo describing the menu to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawMenuBackground(
  const HIRect *               inMenuRect,
  const HIThemeMenuDrawInfo *  inMenuDrawInfo,
  CGContextRef                 inContext,
  HIThemeOrientation           inOrientation)                 AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawMenuItem()
 *  
 *  Summary:
 *    Draws a themed menu item.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inMenuRect:
 *      An HIRect indicating the bounds of the whole menu for which the
 *      menu item is to be drawn.
 *    
 *    inItemRect:
 *      An HIRect for the bounds of the menu item to be drawn.
 *    
 *    inItemDrawInfo:
 *      An HIThemeMenuItemDrawInfo describing the drawing
 *      characteristics of the menu item to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *    
 *    outContentRect:
 *      An HIRect that will be filled with the rectangle describing
 *      where the menu item content is to be drawn.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawMenuItem(
  const HIRect *                   inMenuRect,
  const HIRect *                   inItemRect,
  const HIThemeMenuItemDrawInfo *  inItemDrawInfo,
  CGContextRef                     inContext,
  HIThemeOrientation               inOrientation,
  HIRect *                         outContentRect)            AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawMenuSeparator()
 *  
 *  Summary:
 *    Draws a themed menu separator.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inMenuRect:
 *      An HIRect indicating the bounds of the whole menu for which the
 *      menu separator is to be drawn.
 *    
 *    inItemRect:
 *      An HIRect for the bounds of the menu separator to be drawn.
 *    
 *    inItemDrawInfo:
 *      An HIThemeMenuItemDrawInfo describing the drawing
 *      characteristics of the menu item to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawMenuSeparator(
  const HIRect *                   inMenuRect,
  const HIRect *                   inItemRect,
  const HIThemeMenuItemDrawInfo *  inItemDrawInfo,
  CGContextRef                     inContext,
  HIThemeOrientation               inOrientation)             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetMenuBackgroundShape()
 *  
 *  Summary:
 *    Gets the shape of the background for a themed menu.
 *  
 *  Discussion:
 *    This shape can extend outside of the bounds of the specified
 *    rectangle.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inMenuRect:
 *      An HIRect indicating the bounds of the menu for which the menu
 *      background is to be retrieved.
 *    
 *    inMenuDrawInfo:
 *      An HIThemeMenuDrawInfo describing the menu to be measured.
 *    
 *    outShape:
 *      A valid HIShape that will be cleared and filled with the shape
 *      of the menu background. It needs to be released by the caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetMenuBackgroundShape(
  const HIRect *               inMenuRect,
  const HIThemeMenuDrawInfo *  inMenuDrawInfo,
  HIShapeRef *                 outShape)                      AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/* -------------------------------------------------------------------------- */
/*  Segments                                                                  */
/* -------------------------------------------------------------------------- */

/*
 *  Summary:
 *    Available values for HIThemeSegmentPosition. These are positions
 *    of the segments within the segmented view.
 */
enum {

  /*
   * The first position of a segmented view.
   */
  kHIThemeSegmentPositionFirst  = 0,

  /*
   * A middle segment.
   */
  kHIThemeSegmentPositionMiddle = 1,

  /*
   * The last position of a segmented view.
   */
  kHIThemeSegmentPositionLast   = 2,

  /*
   * The only position of a segmented view. It is simultaneously first
   * and last. You know, only. There is only one segment. It looks
   * pretty much like a button. Please don't use this if you can avoid
   * it. It's ugly.
   */
  kHIThemeSegmentPositionOnly   = 3
};

typedef UInt32                          HIThemeSegmentPosition;

/*
 *  Summary:
 *    Available values for HIThemeSegmentKind available on Mac OS X
 *    10.4 a later.
 */
enum {

  /*
   * The segment to use on non-textured windows. Do not use on textured
   * windows.
   */
  kHIThemeSegmentKindNormal     = 0,

  /*
   * The textured segment. Use on textured windows.
   */
  kHIThemeSegmentKindTextured   = 1,

  /*
   * This is a synonym for kHIThemeSegmentKindTextured for code
   * compatibility. Please use kHIThemeSegmentKindTextured instead.
   */
  kHIThemeSegmentKindInset      = kHIThemeSegmentKindTextured
};

typedef UInt32                          HIThemeSegmentKind;

/*
 *  Summary:
 *    Available values for HIThemeSegmentSize.
 */
enum {

  /*
   * The normally sized segment.
   */
  kHIThemeSegmentSizeNormal     = 0,

  /*
   * The small segment. Not available as textured.
   */
  kHIThemeSegmentSizeSmall      = 1,

  /*
   * The mini segment. Not available as textured.
   */
  kHIThemeSegmentSizeMini       = 3
};

typedef UInt32                          HIThemeSegmentSize;

/*
 */
enum {

  /*
   * No segment adornments are to be drawn.
   */
  kHIThemeSegmentAdornmentNone  = 0,

  /*
   * A focus ring is to be drawn around the segment.
   */
  kHIThemeSegmentAdornmentFocus = (1 << 2), /* to match button focus adornment */

  /*
   * If available, a leading separator is to be drawn on the segment.
   * Note that segments overlap and if the separators are drawn over
   * top each other, the shadows multiply undesirably.
   */
  kHIThemeSegmentAdornmentLeadingSeparator = (1 << 3),

  /*
   * If available, a trailing separator is to be drawn on the segment.
   * Note that segments overlap and if the separators are drawn over
   * top each other, the shadows multiply undesirably.
   */
  kHIThemeSegmentAdornmentTrailingSeparator = (1 << 4)
};

typedef UInt32                          HIThemeSegmentAdornment;

/*
 *  HIThemeSegmentDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to segment drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.4.
 */
struct HIThemeSegmentDrawInfo {

  /*
   * The version of this data structure.  Currently, it is always 0.
   */
  UInt32              version;

  /*
   * The ThemeDrawState for the segment to be drawn or measured.
   */
  ThemeDrawState      state;

  /*
   * The ThemeButtonValue of the segment to be drawn or measured.
   */
  ThemeButtonValue    value;
  HIThemeSegmentSize  size;

  /*
   * The HIThemeSegmentKind of the segment to be drawn or measured.
   */
  HIThemeSegmentKind  kind;

  /*
   * The HIThemeSegmentPositions of the segment to be drawn or measured.
   */
  HIThemeSegmentPosition  position;

  /*
   * The HIThemeSegmentAdornment of the segment to be drawn or measured.
   */
  HIThemeSegmentAdornment  adornment;
};
typedef struct HIThemeSegmentDrawInfo   HIThemeSegmentDrawInfo;
typedef HIThemeSegmentDrawInfo *        HIThemeSegmentDrawInfoPtr;
/*
 *  HIThemeDrawSegment()
 *  
 *  Summary:
 *    Draw a piece of a segmented view.
 *  
 *  Discussion:
 *    New in Mac OS X 10.4. Please note that segments can draw a
 *    separator outside of the specified bounds and the adornments of
 *    the individual segments must coordinate their drawing of
 *    separators (with the adornment field of the passed in
 *    HIThemeSegmentDrawInfo) to avoid overdrawing.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      The bounds of the segment to be drawn.
 *    
 *    inDrawInfo:
 *      A HIThemeSegmentDrawInfo describing the segment to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawSegment(
  const HIRect *                  inBounds,
  const HIThemeSegmentDrawInfo *  inDrawInfo,
  CGContextRef                    inContext,
  HIThemeOrientation              inOrientation)              AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/* -------------------------------------------------------------------------- */
/*  Tabs                                                                      */
/* -------------------------------------------------------------------------- */
/*
 *  HIThemeDrawTabPane()
 *  
 *  Summary:
 *    Draws a themed tab pane.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw the pane.
 *    
 *    inDrawInfo:
 *      The HIThemeTabPaneDrawInfo of the tab pane to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawTabPane(
  const HIRect *                  inRect,
  const HIThemeTabPaneDrawInfo *  inDrawInfo,
  CGContextRef                    inContext,
  HIThemeOrientation              inOrientation)              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawTab()
 *  
 *  Summary:
 *    Draw a themed tab.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      An HIThemeTabDrawInfo describing the tab to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *    
 *    outLabelRect:
 *      An HIRect into which to put the bounds of the label rect.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawTab(
  const HIRect *              inRect,
  const HIThemeTabDrawInfo *  inDrawInfo,
  CGContextRef                inContext,
  HIThemeOrientation          inOrientation,
  HIRect *                    outLabelRect)        /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTabPaneDrawShape()
 *  
 *  Summary:
 *    Gets the shape of the draw area relative to the full bounds of
 *    the tab+pane.
 *  
 *  Discussion:
 *    This API was mistakenly named as a "Get" API. It behaves as
 *    "Copy" API. THE CALLER IS RESPONSIBLE FOR RELEASING THE RETURNED
 *    SHAPE.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      An HIRect indicating the entire tabs area for which the tab
 *      pane shape is to be retrieved.
 *    
 *    inDirection:
 *      A ThemeTabDirection describing on which side of the pane the
 *      tabs would be drawn.
 *    
 *    inTabSize:
 *      An HIThemeTabSize indicating the size of tab pane that would be
 *      drawn.
 *    
 *    outShape:
 *      A pointer to an HIShapeRef which will be set to the shape of
 *      the draw area. It needs to be released by the caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTabPaneDrawShape(
  const HIRect *      inRect,
  ThemeTabDirection   inDirection,
  HIThemeTabSize      inTabSize,
  HIShapeRef *        outShape)                               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTabPaneContentShape()
 *  
 *  Summary:
 *    Gets the shape of the content area relative to the full bounds of
 *    the tab+pane.
 *  
 *  Discussion:
 *    This API was mistakenly named as a "Get" API. It behaves as
 *    "Copy" API. THE CALLER IS RESPONSIBLE FOR RELEASING THE RETURNED
 *    SHAPE.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      An HIRect indicating the entire tabs area for which the tab
 *      content shape is to be retrieved.
 *    
 *    inDirection:
 *      A ThemeTabDirection describing on which side of the pane the
 *      tabs would be drawn.
 *    
 *    inTabSize:
 *      An HIThemeTabSize indicating what size of tab pane that would
 *      be drawn.
 *    
 *    outShape:
 *      A pointer to an HIShapeRef which will be set to the shape of
 *      the draw content. It needs to be released by the caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTabPaneContentShape(
  const HIRect *      inRect,
  ThemeTabDirection   inDirection,
  HIThemeTabSize      inTabSize,
  HIShapeRef *        outShape)                               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTabDrawShape()
 *  
 *  Summary:
 *    Gets the shape of the tab drawing area relative to the full
 *    bounds of the tab+pane.
 *  
 *  Discussion:
 *    This API was mistakenly named as a "Get" API. It behaves as
 *    "Copy" API. THE CALLER IS RESPONSIBLE FOR RELEASING THE RETURNED
 *    SHAPE.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      An HIRect indicating the entire tab+pane area for which the tab
 *      shape is to be retrieved.
 *    
 *    inDrawInfo:
 *      An HIThemeTabDrawInfo describing the tab that would be drawn.
 *    
 *    outShape:
 *      A pointer to an HIShapeRef which will be set to the shape of
 *      the tab drawing area. It needs to be released by the caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTabDrawShape(
  const HIRect *              inRect,
  const HIThemeTabDrawInfo *  inDrawInfo,
  HIShapeRef *                outShape)                       AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTabShape()
 *  
 *  Summary:
 *    Gets the shape for a themed tab.
 *  
 *  Discussion:
 *    This API was mistakenly named as a "Get" API. It behaves as
 *    "Copy" API. THE CALLER IS RESPONSIBLE FOR RELEASING THE RETURNED
 *    SHAPE.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      An HIRect indicating the entire tabs area for which the tab
 *      shape is to be retrieved.
 *    
 *    inDrawInfo:
 *      An HIThemeTabDrawInfo describing the tab that would be drawn.
 *    
 *    outShape:
 *      A pointer to an HIShapeRef which will be set to the shape of
 *      the tab. It needs to be released by the caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTabShape(
  const HIRect *              inRect,
  const HIThemeTabDrawInfo *  inDrawInfo,
  HIShapeRef *                outShape)                       AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/* -------------------------------------------------------------------------- */
/*  Text                                                                      */
/* -------------------------------------------------------------------------- */

/*
 */
enum {

  /*
   * Don't truncate the measured or drawn text.
   */
  kHIThemeTextTruncationNone    = 0,

  /*
   * During measure or draw, if the text will not fit within the
   * available bounds, truncate the text in the middle of the last
   * visible line.
   */
  kHIThemeTextTruncationMiddle  = 1,

  /*
   * During measure or draw, if the text will not fit within the
   * available bounds, truncate the text at the end of the last visible
   * line.
   */
  kHIThemeTextTruncationEnd     = 2,
  kHIThemeTextTruncationDefault = 3
};

typedef UInt32                          HIThemeTextTruncation;

/*
 */
enum {

  /*
   * The text will be drawn flush with the left side of the bounding
   * box.
   */
  kHIThemeTextHorizontalFlushLeft = 0,

  /*
   * The text will be centered within the bounding box.
   */
  kHIThemeTextHorizontalFlushCenter = 1,

  /*
   * The text will be drawn flush with the right side of the bounding
   * box.
   */
  kHIThemeTextHorizontalFlushRight = 2,
  kHIThemeTextHorizontalFlushDefault = 3
};

typedef UInt32                          HIThemeTextHorizontalFlush;

/*
 */
enum {

  /*
   * Draw the text vertically flush with the top of the box
   */
  kHIThemeTextVerticalFlushTop  = 0,

  /*
   * Vertically center the text
   */
  kHIThemeTextVerticalFlushCenter = 1,

  /*
   * Draw the text vertically flush with the bottom of the box
   */
  kHIThemeTextVerticalFlushBottom = 2,
  kHIThemeTextVerticalFlushDefault = 3
};

typedef UInt32                          HIThemeTextVerticalFlush;

/*
 */
enum {
  kHIThemeTextBoxOptionNone     = 0,

  /*
   * Is the text strongly vertical? This option bit is not correctly
   * respected and will have no effect if used.
   */
  kHIThemeTextBoxOptionStronglyVertical = (1 << 1),

  /*
   * Draw the text with an engraved look, suitable for use on the Mac
   * OS X 10.5 dark window backgrounds or on the bodies of some
   * controls.
   */
  kHIThemeTextBoxOptionEngraved = (1 << 2),

  /*
   * By default, HIThemeDrawTextBox will clip the text to the rectangle
   * specified by the inBounds parameter. With some fonts or styles,
   * text may draw outside of the drawing rectangle and will be
   * clipped. If this bit is set, HIThemeDrawTextBox will not clip
   * drawing to its inBounds parameter. Available in Mac OS X 10.4 and
   * later.
   */
  kHIThemeTextBoxOptionDontClip = (1 << 18)
};

typedef OptionBits                      HIThemeTextBoxOptions;

/*
 */
enum {

  /*
   * Available in Mac OS X 10.3 and later. Valid fields for this
   * version are version, state, fontID, horizontalFlushness,
   * verticalFlushness, options, truncationPosition, truncationMaxLines
   * and truncationHappened.
   */
  kHIThemeTextInfoVersionZero   = 0,

  /*
   * Available in Mac OS X 10.5 and later. Valid fields for this
   * version are all those in the zero version as well as the font
   * field.
   */
  kHIThemeTextInfoVersionOne    = 1
};


/*
 *  HIThemeTextInfo
 *  
 *  Summary:
 *    Drawing parameters passed to text drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    New in Mac OS X 10.3, this structure is used for measuring and
 *    drawing text with the HIThemeGetTextDimensions and
 *    HIThemeDrawTextBox APIs. If truncationPosition is
 *    kHIThemeTextTruncationNone, the other fields with the truncation
 *    prefix are ignored.
 */
struct HIThemeTextInfo {

  /*
   * The version of this data structure. Currently, it is always 1.
   */
  UInt32              version;

  /*
   * The theme draw state in which to draw the string.
   */
  ThemeDrawState      state;

  /*
   * The font in which to draw the string.
   */
  ThemeFontID         fontID;

  /*
   * The horizontal flushness of the text. One of the
   * kHIThemeTextHorizontalFlush[Left/Center/Right] constants. When
   * this structure is used for HIThemeGetTextDimensions, this field
   * has no effect on the returned dimensions. However, providing the
   * same flushness that will be used with a subsequent draw will
   * trigger a performance optimization.
   */
  HIThemeTextHorizontalFlush  horizontalFlushness;

  /*
   * The vertical flushness of the text. One of the
   * kHIThemeTextVerticalFlush[Top/Center/Bottom] constants. When this
   * paramblock is used for HIThemeGetTextDimensions, this field has no
   * effect on the returned dimensions. However, providing the same
   * flushness that will be used with a subsequent draw will trigger a
   * performance optimization.
   */
  HIThemeTextVerticalFlush  verticalFlushness;

  /*
   * Currently, the only option available is for strongly vertical text
   * with the kThemeTextBoxOptionStronglyVertical option bit.
   */
  HIThemeTextBoxOptions  options;

  /*
   * Specifies where truncation should occur. If this field is
   * kHIThemeTextTruncationNone, no truncation will occur, and all
   * fields with the truncation prefix will be ignored.
   */
  HIThemeTextTruncation  truncationPosition;

  /*
   * The maximum number of lines to measure or draw before truncation
   * occurs. Ignored if truncationPosition is
   * kHIThemeTextTruncationNone.
   */
  UInt32              truncationMaxLines;

  /*
   * On output, if the text has been truncated, this is set to true. If
   * the text fit completely within the parameters specified and the
   * text was not truncated, this is set to false.
   */
  Boolean             truncationHappened;
  UInt8               filler1;

  /*
   * If fontID is kThemeSpecifiedFont and the version is 1, this
   * CTFontRef will be used for measuring and rendering of the string.
   * If the fontID is anything other than kThemeSpecifiedFont or the
   * version is not 1, this field is ignored.
   */
  CTFontRef           font;
};
typedef struct HIThemeTextInfo          HIThemeTextInfo;
/*
 *  HIThemeGetTextDimensions()
 *  
 *  Summary:
 *    Get text dimensions of a string
 *  
 *  Discussion:
 *    This allows you to get various dimension characteristics of a
 *    string bound to certain criteria that you specify. It allows you
 *    to get the absolute bounds of a string laid out in a single line,
 *    or the bounds of a string constrained to a given width.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inString:
 *      A CFStringRef containing the unicode characters you wish to
 *      measure. You MUST NOT pass in a CFStringRef that was allocated
 *      with any of the "NoCopy" CFString creation APIs; a string
 *      created with a "NoCopy" API has transient storage which is
 *      incompatible with HIThemeGetTextDimensions's caches. 
 *      
 *      In Mac OS X 10.5 and later, this API may also be passed a
 *      CFAttributedStringRef.
 *    
 *    inWidth:
 *      The width to constrain the text before wrapping. If inWidth is
 *      0, the text will not wrap and will be laid out as a single
 *      line, unless it contains embedded carriage return or linefeed
 *      characters; CR/LF will cause the text to wrap and the resulting
 *      measurements will include space for multiple lines of text. If
 *      inWidth is not 0, the text will wrap at the given width and the
 *      measurements will be returned from the multi-line layout.
 *      
 *      
 *      To force single-line layout even in the presence of embedded
 *      CR/LF characters, pass FLT_MAX for inWidth,
 *      kHIThemeTextTruncationEnd for inTextInfo.truncationPosition,
 *      and 1 for inTextInfo.truncationMaxLines.
 *    
 *    inTextInfo:
 *      The HIThemeTextInfo parameter block specifying additional
 *      options for flushness and truncation. The truncationHappened
 *      field is the only field that will be written to by this API
 *      (and the reason for inTextInfo not being const).
 *    
 *    outWidth:
 *      On output, will contain the width of the string. This width may
 *      be smaller than the constrain inWidth parameter if the text has
 *      wrapped. It will return the true bounding width of the layout.
 *      Can be NULL.
 *    
 *    outHeight:
 *      On output, will contain the height of the string. Can be NULL.
 *    
 *    outBaseline:
 *      On output, will contain the baseline of the string. This is the
 *      delta from the top of the text to the baseline of the first
 *      line. Can be NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTextDimensions(
  CFTypeRef          inString,
  CGFloat            inWidth,
  HIThemeTextInfo *  inTextInfo,
  CGFloat *          outWidth,          /* can be NULL */
  CGFloat *          outHeight,         /* can be NULL */
  CGFloat *          outBaseline)       /* can be NULL */     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawTextBox()
 *  
 *  Summary:
 *    Draw the string into the given bounding box
 *  
 *  Discussion:
 *    Draw the string into the bounding box given. You can specify
 *    options such as truncation and justification as well as
 *    determining whether the text was truncated when it was drawn.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inString:
 *      A CFStringRef containing the unicode characters you wish to
 *      render. You MUST NOT pass in a CFStringRef that was allocated
 *      with any of the "NoCopy" CFString creation APIs; a string
 *      created with a "NoCopy" API has transient storage which is
 *      incompatible with HIThemeDrawTextBox's caches. 
 *      
 *      In Mac OS X 10.5 and later, this API may also be passed a
 *      CFAttributedStringRef.
 *    
 *    inBounds:
 *      The HIRect that bounds where the text is to be drawn
 *    
 *    inTextInfo:
 *      The HIThemeTextInfo parameter block specifying additional
 *      options for truncation and flushness. You can control the
 *      number of lines drawn by specifying a truncation of
 *      kHIThemeTextTruncationMiddle or kHIThemeTextTruncationEnd for
 *      the truncationPosition field and then specifying a maximum
 *      number of lines to draw before truncation occurs in the
 *      truncationMaxLines field. The truncationHappened field is the
 *      only field that will be written to by this API (and the reason
 *      for inTextInfo not being const).
 *    
 *    inContext:
 *      The CGContextRef into which to draw the text.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawTextBox(
  CFTypeRef            inString,
  const HIRect *       inBounds,
  HIThemeTextInfo *    inTextInfo,
  CGContextRef         inContext,
  HIThemeOrientation   inOrientation)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetUIFontType()
 *  
 *  Summary:
 *    Returns the CTFontUIFontType for a ThemeFontID
 *  
 *  Discussion:
 *    It is possible to create a CTFontRef that represents a
 *    ThemeFontID by using this API in conjunction with
 *    CTFontCreateUIFontForLanguage. 
 *    
 *    Suggested usage: 
 *    CTFontRef font = CTFontCreateUIFontForLanguage(
 *    HIThemeGetUIFontType( inFontID ), 0, NULL );
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inFontID:
 *      The ThemeFontID to map to a CTFontUIFontType.
 *  
 *  Result:
 *    The CTFontUIFontType that represents the ThemeFontID or
 *    kCTFontNoFontType if there is an error.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CTFontUIFontType 
HIThemeGetUIFontType(ThemeFontID inFontID)                    AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/* -------------------------------------------------------------------------- */
/*  Tracks                                                                    */
/* -------------------------------------------------------------------------- */
/*
 *  HIThemeDrawTrack()
 *  
 *  Summary:
 *    Draw a themed track item.
 *  
 *  Discussion:
 *    Used to draw any tracked element including sliders and scroll
 *    bars.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track that will be drawn.
 *    
 *    inGhostRect:
 *      An HIRect describing the location of the ghost indicator to be
 *      drawn. Generally, this should be NULL and the control using
 *      this primitive should support live feeback.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawTrack(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  const HIRect *                inGhostRect,         /* can be NULL */
  CGContextRef                  inContext,
  HIThemeOrientation            inOrientation)                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawTrackTickMarks()
 *  
 *  Summary:
 *    Draws the tick marks for a slider track.
 *  
 *  Discussion:
 *    This primitive only draws evenly distributed tick marks. 
 *    Internally, it calls HIThemeDrawTickMark to do the actual tick
 *    mark drawing, and any custom (non-even distribution) drawing of
 *    tick marks should be done with HIThemeDrawTickMark.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track tick marks that
 *      will be drawn.
 *    
 *    inNumTicks:
 *      A value indicating the number of tick marks to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawTrackTickMarks(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  ItemCount                     inNumTicks,
  CGContextRef                  inContext,
  HIThemeOrientation            inOrientation)                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawTickMark()
 *  
 *  Summary:
 *    Draws a single tick mark.
 *  
 *  Discussion:
 *    This primitive draws a single tick mark and can be used to draw
 *    custom tick marks that are not easily drawn by
 *    HIThemeDrawTrackTickMarks, which only draws evenly distributed
 *    tick marks.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      The HIThemeTickMarkDrawInfo of the tick mark to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawTickMark(
  const HIRect *                   inBounds,
  const HIThemeTickMarkDrawInfo *  inDrawInfo,
  CGContextRef                     inContext,
  HIThemeOrientation               inOrientation)             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTrackThumbShape()
 *  
 *  Summary:
 *    Get the thumb shape of a themed track.
 *  
 *  Discussion:
 *    This API was mistakenly named as a "Get" API. It behaves as
 *    "Copy" API. THE CALLER IS RESPONSIBLE FOR RELEASING THE RETURNED
 *    SHAPE.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track to be measured.
 *    
 *    outThumbShape:
 *      A pointer to an HIShapeRef which will be set to the shape of
 *      the themed track's thumb. It needs to be released by the caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTrackThumbShape(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  HIShapeRef *                  outThumbShape)                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeHitTestTrack()
 *  
 *  Summary:
 *    Hit test the themed track.
 *  
 *  Discussion:
 *    Returns true if the track was hit and fills in outPartHit. 
 *    Otherwise, returns false.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTabDrawInfo describing the tab that will be drawn.
 *    
 *    inMousePoint:
 *      An HIPoint which will be location basis for the test.
 *    
 *    outPartHit:
 *      A pointer to a ControlPartCode that will be filled with the
 *      part hit by the incoming point.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIThemeHitTestTrack(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  const HIPoint *               inMousePoint,
  ControlPartCode *             outPartHit)                   AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTrackBounds()
 *  
 *  Summary:
 *    Gets the track bounds of a themed track.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track that will be drawn.
 *    
 *    outBounds:
 *      A pointer to an HIRect in which will be returned the rectangle
 *      of the track bounds.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTrackBounds(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  HIRect *                      outBounds)                    AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTrackPartBounds()
 *  
 *  Summary:
 *    Returns measurements for the bounds of the a track part,
 *    according to the specifics of that track as specified in the
 *    incoming draw info record.
 *  
 *  Discussion:
 *    HIThemeGetTrackPartBounds allows you to get the boundaries of
 *    individual pieces of a track's theme layout.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track to be measured.
 *    
 *    inPartCode:
 *      A ControlPartCode describing which part to measure.  A list of
 *      available ControlPartCodes can be retrieved using
 *      HIThemeGetTrackParts.
 *    
 *    outPartBounds:
 *      The bounds of the specified part relative to the start
 *      rectangle specified in inDrawInfo.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTrackPartBounds(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  ControlPartCode               inPartCode,
  HIRect *                      outPartBounds)                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTrackParts()
 *  
 *  Summary:
 *    Counts the number of parts that make up a track.  Optionally
 *    returns an array of ControlPartCodes that describe each of the
 *    counted parts.
 *  
 *  Discussion:
 *    HIThemeGetTrackParts allows you to count the number of parts that
 *    make up a track.  This is useful if you need to iterate through
 *    the parts of a track and get information about them, i.e. using
 *    HIThemeGetTrackPartBounds.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track to be measured.
 *    
 *    outNumberOfParts:
 *      A pointer to a UInt32 in which to return the number of counted
 *      parts.
 *    
 *    inMaxParts:
 *      The maximum number of ControlPartCodes that can be copied into
 *      the supplied ioPartsBuffer.  This value is ignored if
 *      ioPartsBuffer is NULL.
 *    
 *    ioPartsBuffer:
 *      An pointer to an array into which HIThemeGetTrackPartBounds
 *      will copy ControlPartCodes that describe each of the counted
 *      parts.  This pointer can be NULL if you are just counting parts.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTrackParts(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  UInt32 *                      outNumberOfParts,
  UInt32                        inMaxParts,
  ControlPartCode *             ioPartsBuffer)                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTrackDragRect()
 *  
 *  Summary:
 *    Get the rectangle of the drag area of a themed track.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track to be measured.
 *    
 *    outDragRect:
 *      A pointer to an HIRect in which will be returned the rectangle
 *      of the drag area of the track.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTrackDragRect(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  HIRect *                      outDragRect)                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTrackThumbPositionFromOffset()
 *  
 *  Summary:
 *    Get the track's relative thumb position based on the offset.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track to be measured.
 *    
 *    inThumbOffset:
 *      An HIPoint describing the position of the thumb as an offset
 *      from the track bounds.
 *    
 *    outRelativePosition:
 *      On output, the track-relative position calculated from the
 *      thumb offset.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTrackThumbPositionFromOffset(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  const HIPoint *               inThumbOffset,
  CGFloat *                     outRelativePosition)          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTrackThumbPositionFromBounds()
 *  
 *  Summary:
 *    Get the themed track thumb position from its bounds.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      A pointer to an HIThemeTrackDrawInfo describing the track to be
 *      measured.
 *    
 *    inThumbBounds:
 *      The bounds of the thumb from which the postion is to be
 *      calculated.
 *    
 *    outRelativePosition:
 *      On output, the track-relative position calculated from the
 *      thumb location.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTrackThumbPositionFromBounds(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  const HIRect *                inThumbBounds,
  CGFloat *                     outRelativePosition)          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetTrackLiveValue()
 *  
 *  Summary:
 *    Get the themed track live value.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawInfo:
 *      An HIThemeTrackDrawInfo describing the track to be measured.
 *    
 *    inRelativePosition:
 *      An HIPoint describing the position of the thumb as an offset
 *      from the track bounds.
 *    
 *    outValue:
 *      On output, the track value as calculated from the relative
 *      postion of the thumb.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTrackLiveValue(
  const HIThemeTrackDrawInfo *  inDrawInfo,
  CGFloat                       inRelativePosition,
  SInt32 *                      outValue)                     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetScrollBarTrackRect()
 *  
 *  Summary:
 *    Gets the rectangle of the tracking area of a themed scroll bar.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      An HIRect indicating the area in which the scroll bar would be
 *      drawn.
 *    
 *    inTrackInfo:
 *      An HIScrollBarTrackInfo for the scroll bar that would be drawn.
 *      Currently, only the pressState and enableState fields are used.
 *    
 *    inIsHoriz:
 *      A Boolean where true means that the scroll bar would be
 *      horizontal and false means that the scroll bar would be
 *      vertical.
 *    
 *    outTrackBounds:
 *      A pointer to an HIRect in which will be returned the rectangle
 *      of the track area of the scroll bar.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetScrollBarTrackRect(
  const HIRect *                inBounds,
  const HIScrollBarTrackInfo *  inTrackInfo,
  Boolean                       inIsHoriz,
  HIRect *                      outTrackBounds)               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeHitTestScrollBarArrows()
 *  
 *  Summary:
 *    Hit test the theme scroll bar arrows to determine where the hit
 *    occurred.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inScrollBarBounds:
 *      An HIRect indicating the bounds of the scroll bar that will be
 *      hit tested.
 *    
 *    inTrackInfo:
 *      An HIScrollBarTrackInfo for the scroll bar to be drawn.
 *      Currently, only the version, pressState and enableState fields
 *      are used.
 *    
 *    inIsHoriz:
 *      A Boolean where true means that the scroll bar is to be
 *      horizontal and false means that the scroll bar is to be
 *      vertical.
 *    
 *    inPtHit:
 *      An HIPoint indicating where the control was hit and which will
 *      be used for hit testing.
 *    
 *    outTrackBounds:
 *      A pointer to an HIRect in which will be returned the rectangle
 *      of the track area of the scroll bar.  Can be NULL.
 *    
 *    outPartCode:
 *      A pointer to a ControlPartCode in which the part code of the
 *      hit part will be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIThemeHitTestScrollBarArrows(
  const HIRect *                inScrollBarBounds,
  const HIScrollBarTrackInfo *  inTrackInfo,
  Boolean                       inIsHoriz,
  const HIPoint *               inPtHit,
  HIRect *                      outTrackBounds,
  ControlPartCode *             outPartCode)                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawScrollBarDelimiters()
 *  
 *  Summary:
 *    Draw themed scroll bar delimiters.
 *  
 *  Discussion:
 *    Draws the grow lines delimiting the scroll bar areas.  Does not
 *    draw the size box.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inContRect:
 *      An HIRect indicating the rectangle of the content area of the
 *      window to be drawn.
 *    
 *    inDrawInfo:
 *      The HIThemeScrollBarDelimitersDrawInfo of the delimiters to be
 *      drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawScrollBarDelimiters(
  const HIRect *                              inContRect,
  const HIThemeScrollBarDelimitersDrawInfo *  inDrawInfo,
  CGContextRef                                inContext,
  HIThemeOrientation                          inOrientation)  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/* -------------------------------------------------------------------------- */
/*  Windows                                                                   */
/* -------------------------------------------------------------------------- */
/*
 *  HIThemeDrawWindowFrame()
 *  
 *  Summary:
 *    Draws a themed window frame.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inContRect:
 *      An HIRect indicating the rectangle of the content area of the
 *      window to be drawn.
 *    
 *    inDrawInfo:
 *      The HIThemeWindowDrawInfo of the window frame to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *    
 *    outTitleRect:
 *      A pointer to an HIRect into which to put the bounds of the
 *      title rect.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawWindowFrame(
  const HIRect *                 inContRect,
  const HIThemeWindowDrawInfo *  inDrawInfo,
  CGContextRef                   inContext,
  HIThemeOrientation             inOrientation,
  HIRect *                       outTitleRect)        /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawTitleBarWidget()
 *  
 *  Summary:
 *    Draws the requested theme title bar widget.
 *  
 *  Discussion:
 *    HIThemeDrawTitleBarWidget renders the requested theme title bar
 *    widget in the proper location of a window.  A common
 *    misconception when using this API is that the client must specify
 *    the exact location of the widget in the window. The widget will
 *    locate itself in the window based relative to the content rect
 *    passed in content rectangle -- the contRect parameter.  Another
 *    common problem is to ignore the window's attributes.  The
 *    attributes must be set up properly to describe the window for
 *    which the widget is to be drawn.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inContRect:
 *      A rectangle describing the window's content area.  The widget
 *      is drawn relative to the content rectangle of the window, so
 *      this parameter does not describe the actual widget bounds, it
 *      describes the window's content rectangle.
 *    
 *    inDrawInfo:
 *      The HIThemeWindowWidgetDrawInfo of the window widget to be
 *      drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawTitleBarWidget(
  const HIRect *                       inContRect,
  const HIThemeWindowWidgetDrawInfo *  inDrawInfo,
  CGContextRef                         inContext,
  HIThemeOrientation                   inOrientation)         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawGrowBox()
 *  
 *  Summary:
 *    Draws a theme grow box.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inOrigin:
 *      The origin from which to draw the grow box.
 *    
 *    inDrawInfo:
 *      An HIThemeGrowBoxDrawInfo describing the grow box to be drawn
 *      or measured.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawGrowBox(
  const HIPoint *                 inOrigin,
  const HIThemeGrowBoxDrawInfo *  inDrawInfo,
  CGContextRef                    inContext,
  HIThemeOrientation              inOrientation)              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetGrowBoxBounds()
 *  
 *  Summary:
 *    Gets the bounds for a grow box.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inOrigin:
 *      The origin from which to draw the grow box.
 *    
 *    inDrawInfo:
 *      An HIThemeGrowBoxDrawInfo describing the grow box to be
 *      measured. The state field is ignored.
 *    
 *    outBounds:
 *      A pointer to an HIRect in which will be returned the rectangle
 *      of the standalone grow box bounds.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetGrowBoxBounds(
  const HIPoint *                 inOrigin,
  const HIThemeGrowBoxDrawInfo *  inDrawInfo,
  HIRect *                        outBounds)                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetWindowShape()
 *  
 *  Summary:
 *    Obtains the specified window shape.
 *  
 *  Discussion:
 *    This API was mistakenly named as a "Get" API. It behaves as
 *    "Copy" API. THE CALLER IS RESPONSIBLE FOR RELEASING THE RETURNED
 *    SHAPE.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inContRect:
 *      An HIRect indicating the rectangle of the content area of the
 *      window that would be drawn.
 *    
 *    inDrawInfo:
 *      The HIThemeWindowDrawInfo of the window frame to be measured.
 *    
 *    inWinRegion:
 *      A WindowRegionCode indicating the desired region for which to
 *      return the shape.
 *    
 *    outShape:
 *      A pointer to an HIShapeRef which will be set to the shape of
 *      the requested window region. It needs to be released by the
 *      caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetWindowShape(
  const HIRect *                 inContRect,
  const HIThemeWindowDrawInfo *  inDrawInfo,
  WindowRegionCode               inWinRegion,
  HIShapeRef *                   outShape)                    AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeGetWindowRegionHit()
 *  
 *  Summary:
 *    Get the window region hit in a themed window.
 *  
 *  Discussion:
 *    Not that this call does not return a region, but a region code.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inContRect:
 *      An HIRect indicating the rectangle of the content area of the
 *      window that would be drawn.
 *    
 *    inDrawInfo:
 *      The HIThemeWindowDrawInfo of the window frame to be measured.
 *    
 *    inPoint:
 *      An HIPoint against which the test will occur.
 *    
 *    outRegionHit:
 *      The output WindowRegionCode of hit window region.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIThemeGetWindowRegionHit(
  const HIRect *                 inContRect,
  const HIThemeWindowDrawInfo *  inDrawInfo,
  const HIPoint *                inPoint,
  WindowRegionCode *             outRegionHit)                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/* -------------------------------------------------------------------------- */
/*  Miscellaneous                                                             */
/* -------------------------------------------------------------------------- */
/*
 *  HIThemeDrawFrame()
 *  
 *  Summary:
 *    Draws a variety of frames.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      An HIThemeFrameDrawInfo describing the frame to be drawn or
 *      measured.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawFrame(
  const HIRect *                inRect,
  const HIThemeFrameDrawInfo *  inDrawInfo,
  CGContextRef                  inContext,
  HIThemeOrientation            inOrientation)                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawGroupBox()
 *  
 *  Summary:
 *    Draws a themed primary group box.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      An HIThemeGroupBoxDrawInfo describing the group box to be drawn
 *      or measured.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawGroupBox(
  const HIRect *                   inRect,
  const HIThemeGroupBoxDrawInfo *  inDrawInfo,
  CGContextRef                     inContext,
  HIThemeOrientation               inOrientation)             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawGenericWell()
 *  
 *  Summary:
 *    Draws a themed generic well.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      An HIThemeButtonDrawInfo that describes attributes of the well
 *      to be drawn. Set the kThemeAdornmentDefault bit of the
 *      adornment field of this structure to also draw the center of
 *      the well.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawGenericWell(
  const HIRect *                 inRect,
  const HIThemeButtonDrawInfo *  inDrawInfo,
  CGContextRef                   inContext,
  HIThemeOrientation             inOrientation)               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawPaneSplitter()
 *  
 *  Summary:
 *    Draws a themed pane splitter.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      The HIThemeSplitterDrawInfo of the pane splitter to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawPaneSplitter(
  const HIRect *                   inRect,
  const HIThemeSplitterDrawInfo *  inDrawInfo,
  CGContextRef                     inContext,
  HIThemeOrientation               inOrientation)             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawGrabber()
 *  
 *  Summary:
 *    Draws a themed grabber.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      The HIThemeGrabberDrawInfo of the grabber to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawGrabber(
  const HIRect *                  inRect,
  const HIThemeGrabberDrawInfo *  inDrawInfo,
  CGContextRef                    inContext,
  HIThemeOrientation              inOrientation)              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawPlacard()
 *  
 *  Summary:
 *    Draws a themed placard.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      The HIThemePlacardDrawInfo of the placard to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawPlacard(
  const HIRect *                  inRect,
  const HIThemePlacardDrawInfo *  inDrawInfo,
  CGContextRef                    inContext,
  HIThemeOrientation              inOrientation)              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawHeader()
 *  
 *  Summary:
 *    Draws a themed window header in the specified rectangle.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      The HIThemeHeaderDrawInfo of the window frame to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawHeader(
  const HIRect *                 inRect,
  const HIThemeHeaderDrawInfo *  inDrawInfo,
  CGContextRef                   inContext,
  HIThemeOrientation             inOrientation)               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawFocusRect()
 *  
 *  Summary:
 *    Draws a themed focus rectangle in the specified rectangle.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inHasFocus:
 *      Pass in true to draw focus. Passing false effectively makes
 *      this API a no-op.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawFocusRect(
  const HIRect *       inRect,
  Boolean              inHasFocus,
  CGContextRef         inContext,
  HIThemeOrientation   inOrientation)                         AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



/*
 */
enum {

  /*
   * Draw the visual focus only, and not any of the draw operations
   * that form its shape.
   */
  kHIThemeFocusRingOnly         = 0,

  /*
   * Draw the visual focus above the results of the draw operations
   * that form its shape.
   */
  kHIThemeFocusRingAbove        = 1,

  /*
   * Draw the visual focus below the results of the draw operations
   * that form its shape.
   */
  kHIThemeFocusRingBelow        = 2
};


typedef UInt32                          HIThemeFocusRing;
/*
 *  HIThemeBeginFocus()
 *  
 *  Summary:
 *    Begin focus drawing.
 *  
 *  Discussion:
 *    Call HIThemeBeginFocus to begin focus drawing. All drawing
 *    operations in the specified context after this call will be drawn
 *    with a visual representation of focus. Currently, this is a
 *    theme-tinted halo resembling a glow. Note that nothing will be
 *    drawn in the specified context until HIThemeEndFocus is called. A
 *    call to HIThemeBeginFocus must always be paired with an
 *    HIThemeEndFocus call. Nesting these calls will not crash but the
 *    results will be odd and is definitely not recommended.
 *    HIThemeBegin/EndFocus is designed to replace
 *    DrawThemeFocusRegion. For efficiency, clipping the context to the
 *    bounds that will be drawn into is highly desirable.
 *    HIThemeBeginFocus may do some allocations that are affected by
 *    the size of the context's clip at the time it is called -- so an
 *    extremely large clip or an unset clip may cause a large,
 *    inefficient allocation.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inContext:
 *      The CG context in which the focus is to be drawn.
 *    
 *    inRing:
 *      An HIThemeFocusRing indicating which type of focus is to be
 *      drawn.
 *    
 *    inReserved:
 *      Always pass NULL for this parameter.
 *  
 *  Result:
 *    A result code indicating success or failure. Don't call
 *    HIThemeEndFocus on the context if HIThemeBeginFocus fails.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeBeginFocus(
  CGContextRef       inContext,
  HIThemeFocusRing   inRing,
  void *             inReserved)                              AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIThemeEndFocus()
 *  
 *  Summary:
 *    End focus drawing.
 *  
 *  Discussion:
 *    See HIThemeBeginFocus for focus drawing details. Calling
 *    HIThemeEndFocus indicates that the drawing operations to be
 *    focused are complete.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inContext:
 *      The CG context in which the focus is to be drawn. This needs to
 *      be the same context passed to the HIThemeBeginFocus call with
 *      which this call to HIThemeEndFocus is paired.
 *  
 *  Result:
 *    A result code indicating success or failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeEndFocus(CGContextRef inContext)                       AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIThemeDrawSeparator()
 *  
 *  Summary:
 *    Draw a themed separator element.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRect:
 *      The HIRect in which to draw.
 *    
 *    inDrawInfo:
 *      The HIThemeSeparatorDrawInfo of the window frame to be drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawSeparator(
  const HIRect *                    inRect,
  const HIThemeSeparatorDrawInfo *  inDrawInfo,
  CGContextRef                      inContext,
  HIThemeOrientation                inOrientation)            AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeSetFill()
 *  
 *  Summary:
 *    Set the context fill color to that specified by the requested
 *    brush.
 *  
 *  Discussion:
 *    Note that this call does not actually draw anything. It sets the
 *    passed context's fill color to that of the specified theme brush.
 *    Subsequent fills in the context will be with the color specified
 *    by the theme brush.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBrush:
 *      The ThemeBrush describing the requested fill color.
 *    
 *    inInfo:
 *      Not used. Should always be NULL.
 *    
 *    inContext:
 *      The CG context for which the fill color is to be set.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeSetFill(
  ThemeBrush           inBrush,
  void *               inInfo,
  CGContextRef         inContext,
  HIThemeOrientation   inOrientation)                         AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIThemeSetStroke()
 *  
 *  Summary:
 *    Set the context stroke color to that specified by the requested
 *    brush.
 *  
 *  Discussion:
 *    Note that this call does not actually draw anything. It sets the
 *    passed context's stroke color to that of the specified theme
 *    brush. Subsequent strokes in the context will be with the color
 *    specified by the theme brush.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBrush:
 *      The ThemeBrush describing the requested stroke color.
 *    
 *    inInfo:
 *      Not used. Should always be NULL.
 *    
 *    inContext:
 *      The CG context for which the stroke color is to be set.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeSetStroke(
  ThemeBrush           inBrush,
  void *               inInfo,
  CGContextRef         inContext,
  HIThemeOrientation   inOrientation)                         AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIThemeSetTextFill()
 *  
 *  Summary:
 *    Set the context text fill color to that specified by the
 *    requested brush.
 *  
 *  Discussion:
 *    Note that this call does not actually draw anything. It sets the
 *    passed context's text fill color to that of the specified
 *    ThemeTextColor. Subsequent text drawing in the context will be
 *    with the color specified by the ThemeTextColor.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inColor:
 *      A ThemeTextColor describing the requested text fill color.
 *    
 *    inInfo:
 *      Not used. Should always be NULL.
 *    
 *    inContext:
 *      The CG context for which the fill color is to be set.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeSetTextFill(
  ThemeTextColor       inColor,
  void *               inInfo,
  CGContextRef         inContext,
  HIThemeOrientation   inOrientation)                         AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIThemeApplyBackground()
 *  
 *  Summary:
 *    Apply a themed background for a rectangle.
 *  
 *  Discussion:
 *    Note that this call does not actually draw anything. It sets the
 *    passed context's fill color to the requested theme background.
 *    Subsequent fills in the context will fill with the theme
 *    background.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      An HIRect enclosing the whole background. This rectangle is
 *      used to calculate the pattern phase (if there is one) of the
 *      background as it is set up in the context.
 *    
 *    inDrawInfo:
 *      An HIThemeBackgroundDrawInfo describing the background.
 *    
 *    inContext:
 *      The CG context for which the background is to be set.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeApplyBackground(
  const HIRect *                     inBounds,
  const HIThemeBackgroundDrawInfo *  inDrawInfo,
  CGContextRef                       inContext,
  HIThemeOrientation                 inOrientation)           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeDrawBackground()
 *  
 *  Summary:
 *    Draw a themed background for a rectangle.
 *  
 *  Discussion:
 *    Currently, this call only works with kThemeBackgroundMetal.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      An HIRect indicating the bounds to fill with the background.
 *      For backgrounds that need pattern continuity, such as
 *      kThemeBackgroundMetal, this rectangle is the full bounds of the
 *      rectangle for which the filling is to occur. If drawing a
 *      sub-rectangle of that background, set the clip and draw the
 *      full rectangle. This routine has been optimized to not perform
 *      calculations on the non-clip part of the drawing bounds.
 *    
 *    inDrawInfo:
 *      An HIThemeBackgroundDrawInfo describing the background to be
 *      drawn.
 *    
 *    inContext:
 *      The CG context in which the drawing is to be done.
 *    
 *    inOrientation:
 *      An HIThemeOrientation that describes the orientation of the
 *      passed in context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeDrawBackground(
  const HIRect *                     inBounds,
  const HIThemeBackgroundDrawInfo *  inDrawInfo,
  CGContextRef                       inContext,
  HIThemeOrientation                 inOrientation)           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIThemeBrushCreateCGColor()
 *  
 *  Summary:
 *    Create a CGColor for a ThemeBrush.
 *  
 *  Discussion:
 *    Color is an ambiguous term. The color may be a pattern.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBrush:
 *      The ThemeBrush describing the requested color.
 *    
 *    outColor:
 *      A pointer to a CGColorRef that will be set to the newly created
 *      CGColor.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeBrushCreateCGColor(
  ThemeBrush    inBrush,
  CGColorRef *  outColor)                                     AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIThemeGetTextColorForThemeBrush()
 *  
 *  Summary:
 *    Returns an appropriate ThemeTextColor that matches a ThemeBrush.
 *  
 *  Discussion:
 *    Creates a ThemeTextColor for use with HIThemeSetTextFill.
 *    ThemeTextColors are currently availabe for these theme brushes:
 *    
 *    
 *    kThemeBrushDialogBackgroundActive/Inactive 
 *    
 *    kThemeBrushAlertBackgroundActive/Inactive 
 *    
 *    kThemeBrushModelessDialogBackgroundActive/Inactive 
 *    <BR> kThemeBrushNotificationWindowBackground
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBrush:
 *      The ThemeBrush describing the requested color.
 *    
 *    inWindowIsActive:
 *      Whether the text color should indicate an active or inactive
 *      state.
 *    
 *    outColor:
 *      A pointer to a ThemeTextColor that will be set to the matched
 *      color.
 *  
 *  Result:
 *    An operating system result code. themeNoAppropriateBrushErr will
 *    be returned if no matching ThemeTextColor exists.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIThemeGetTextColorForThemeBrush(
  ThemeBrush        inBrush,
  Boolean           inWindowIsActive,
  ThemeTextColor *  outColor)                                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/* The following routines were in Appearance.h prior to 10.6 */
/*
 *  GetThemeMenuSeparatorHeight()
 *  
 *  Summary:
 *    Returns the height of a menu item separator, in points.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outHeight:
 *      On exit, contains the height of a menu item separator, in
 *      points.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
GetThemeMenuSeparatorHeight(SInt16 * outHeight)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetThemeMenuItemExtra()
 *  
 *  Summary:
 *    Returns the extra width and height required for a menu item
 *    beyond the height of the text.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inItemType:
 *      The type of menu item. These are defined in Appearance.h.
 *    
 *    outHeight:
 *      Extra height, in points, for this item type.
 *    
 *    outWidth:
 *      Extra width, in points, for this item type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
GetThemeMenuItemExtra(
  ThemeMenuItemType   inItemType,
  SInt16 *            outHeight,
  SInt16 *            outWidth)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetThemeMenuTitleExtra()
 *  
 *  Summary:
 *    Returns the extra width for a menu title, in points.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outWidth:
 *      On exit, contains the extra menu title width, in points.
 *    
 *    inIsSquished:
 *      Indicates whether the menu title is being drawn with a
 *      condensed appearance.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
GetThemeMenuTitleExtra(
  SInt16 *  outWidth,
  Boolean   inIsSquished)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* The ThemeMetric values are defined in Appearance.h */

/*
 *  Summary:
 *    Theme metrics allow you to find out sizes of things in the
 *    current environment, such as how wide a scroll bar is, etc.
 *  
 *  Discussion:
 *    ThemeMetrics
 */
enum {

  /*
   * The width (or height if horizontal) of a scroll bar.
   */
  kThemeMetricScrollBarWidth    = 0,

  /*
   * The width (or height if horizontal) of a small scroll bar.
   */
  kThemeMetricSmallScrollBarWidth = 1,

  /*
   * The height of the non-label part of a check box control.
   */
  kThemeMetricCheckBoxHeight    = 2,

  /*
   * The height of the non-label part of a radio button control.
   */
  kThemeMetricRadioButtonHeight = 3,

  /*
   * The amount of white space surrounding the text Rect of the text
   * inside of an Edit Text control.  If you select all of the text in
   * an Edit Text control, you can see the white space. The metric is
   * the number of pixels, per side, that the text Rect is outset to
   * create the whitespace Rect.
   */
  kThemeMetricEditTextWhitespace = 4,

  /*
   * The thickness of the Edit Text frame that surrounds the whitespace
   * Rect (that is surrounding the text Rect). The metric is the number
   * of pixels, per side, that the frame Rect is outset from the
   * whitespace Rect.
   */
  kThemeMetricEditTextFrameOutset = 5,

  /*
   * The number of pixels that the list box frame is outset from the
   * content of the list box.
   */
  kThemeMetricListBoxFrameOutset = 6,

  /*
   * Describes how far outside of the input rect DrawThemeFocusRect and
   * HIThemeDrawFocusRect APIs will draw the focus.
   */
  kThemeMetricFocusRectOutset   = 7,

  /*
   * The thickness of the frame drawn by DrawThemeGenericWell.
   */
  kThemeMetricImageWellThickness = 8,

  /*
   * The number of pixels a scrollbar should overlap (actually
   * underlap) any bounding box which surrounds it and scrollable
   * content. This also includes the window frame when a scrolbar is
   * along an edge of the window.
   */
  kThemeMetricScrollBarOverlap  = 9,

  /*
   * The height of the large tab of a tab control.
   */
  kThemeMetricLargeTabHeight    = 10,

  /*
   * The width of the caps (end pieces) of the large tabs of a tab
   * control.
   */
  kThemeMetricLargeTabCapsWidth = 11,

  /*
   * The amount to add to the tab height (kThemeMetricLargeTabHeight)
   * to find out the rectangle height to use with the various Tab
   * drawing primitives. This amount is also the amount that each tab
   * overlaps the tab pane.
   */
  kThemeMetricTabFrameOverlap   = 12,

  /*
   * If less than zero, this indicates that the text should be centered
   * on each tab. If greater than zero, the text should be justified
   * (according to the system script direction) and the amount is the
   * offset from the appropriate edge at which the text should start
   * drawing.
   */
  kThemeMetricTabIndentOrStyle  = 13,

  /*
   * The amount of space that every tab's drawing rectangle overlaps
   * the one on either side of it.
   */
  kThemeMetricTabOverlap        = 14,

  /*
   * The height of the small tab of a tab control.  This includes the
   * pixels that overlap the tab pane and/or tab pane bar.
   */
  kThemeMetricSmallTabHeight    = 15,

  /*
   * The width of the caps (end pieces) of the small tabs of a tab
   * control.
   */
  kThemeMetricSmallTabCapsWidth = 16,

  /*
   * The height of the push button control.
   */
  kThemeMetricPushButtonHeight  = 19,

  /*
   * The height of the list header field of the data browser control.
   */
  kThemeMetricListHeaderHeight  = 20,

  /*
   * The height of a disclosure triangle control.  This triangle is the
   * not the center of the disclosure button, but its own control.
   */
  kThemeMetricDisclosureTriangleHeight = 25,

  /*
   * The width of a disclosure triangle control.
   */
  kThemeMetricDisclosureTriangleWidth = 26,

  /*
   * The height of a little arrows control.
   */
  kThemeMetricLittleArrowsHeight = 27,

  /*
   * The width of a little arrows control.
   */
  kThemeMetricLittleArrowsWidth = 28,

  /*
   * The height of a popup button control.
   */
  kThemeMetricPopupButtonHeight = 30,

  /*
   * The height of a small popup button control.
   */
  kThemeMetricSmallPopupButtonHeight = 31,

  /*
   * The height of the large progress bar, not including its shadow.
   */
  kThemeMetricLargeProgressBarThickness = 32,

  /*
   * This metric is not used.
   */
  kThemeMetricPullDownHeight    = 33,

  /*
   * This metric is not used.
   */
  kThemeMetricSmallPullDownHeight = 34,

  /*
   * The height of the window grow box control.
   */
  kThemeMetricResizeControlHeight = 38,

  /*
   * The height of the small grow box control, such as on utility
   * windows.
   */
  kThemeMetricSmallResizeControlHeight = 39,

  /*
   * The height of the horizontal slider control.
   */
  kThemeMetricHSliderHeight     = 41,

  /*
   * The height of the tick marks for a horizontal slider control.
   */
  kThemeMetricHSliderTickHeight = 42,

  /*
   * The width of the vertical slider control.
   */
  kThemeMetricVSliderWidth      = 45,

  /*
   * The width of the tick marks for a vertical slider control.
   */
  kThemeMetricVSliderTickWidth  = 46,

  /*
   * The height of the title bar widgets (grow, close, and zoom boxes)
   * for a document window.
   */
  kThemeMetricTitleBarControlsHeight = 49,

  /*
   * The width of the non-label part of a check box control.
   */
  kThemeMetricCheckBoxWidth     = 50,

  /*
   * The width of the non-label part of a radio button control.
   */
  kThemeMetricRadioButtonWidth  = 52,

  /*
   * The height of the normal bar, not including its shadow.
   */
  kThemeMetricNormalProgressBarThickness = 58,

  /*
   * The number of pixels of shadow depth drawn below the progress bar.
   */
  kThemeMetricProgressBarShadowOutset = 59,

  /*
   * The number of pixels of shadow depth drawn below the small
   * progress bar.
   */
  kThemeMetricSmallProgressBarShadowOutset = 60,

  /*
   * The number of pixels that the content of a primary group box is
   * from the bounds of the control.
   */
  kThemeMetricPrimaryGroupBoxContentInset = 61,

  /*
   * The number of pixels that the content of a secondary group box is
   * from the bounds of the control.
   */
  kThemeMetricSecondaryGroupBoxContentInset = 62,

  /*
   * Width allocated to draw the mark character in a menu.
   */
  kThemeMetricMenuMarkColumnWidth = 63,

  /*
   * Width allocated for the mark character in a menu item when the
   * menu has kMenuAttrExcludesMarkColumn.
   */
  kThemeMetricMenuExcludedMarkColumnWidth = 64,

  /*
   * Indent into the interior of the mark column at which the mark
   * character is drawn.
   */
  kThemeMetricMenuMarkIndent    = 65,

  /*
   * Whitespace at the leading edge of menu item text.
   */
  kThemeMetricMenuTextLeadingEdgeMargin = 66,

  /*
   * Whitespace at the trailing edge of menu item text.
   */
  kThemeMetricMenuTextTrailingEdgeMargin = 67,

  /*
   * Width per indent level (set by SetMenuItemIndent) of a menu item.
   */
  kThemeMetricMenuIndentWidth   = 68,

  /*
   * Whitespace at the trailing edge of a menu icon (if the item also
   * has text).
   */
  kThemeMetricMenuIconTrailingEdgeMargin = 69
};


/*
 *  Discussion:
 *    The following metrics are only available in OS X.
 */
enum {

  /*
   * The height of a disclosure button.
   */
  kThemeMetricDisclosureButtonHeight = 17,

  /*
   * The height and the width of the round button control.
   */
  kThemeMetricRoundButtonSize   = 18,

  /*
   * The height of the non-label part of a small check box control.
   */
  kThemeMetricSmallCheckBoxHeight = 21,

  /*
   * The width of a disclosure button.
   */
  kThemeMetricDisclosureButtonWidth = 22,

  /*
   * The height of a small disclosure button.
   */
  kThemeMetricSmallDisclosureButtonHeight = 23,

  /*
   * The width of a small disclosure button.
   */
  kThemeMetricSmallDisclosureButtonWidth = 24,

  /*
   * The height (or width if vertical) of a pane splitter.
   */
  kThemeMetricPaneSplitterHeight = 29,

  /*
   * The height of the small push button control.
   */
  kThemeMetricSmallPushButtonHeight = 35,

  /*
   * The height of the non-label part of a small radio button control.
   */
  kThemeMetricSmallRadioButtonHeight = 36,

  /*
   * The height of the relevance indicator control.
   */
  kThemeMetricRelevanceIndicatorHeight = 37,

  /*
   * The height and the width of the large round button control.
   */
  kThemeMetricLargeRoundButtonSize = 40,

  /*
   * The height of the small, horizontal slider control.
   */
  kThemeMetricSmallHSliderHeight = 43,

  /*
   * The height of the tick marks for a small, horizontal slider
   * control.
   */
  kThemeMetricSmallHSliderTickHeight = 44,

  /*
   * The width of the small, vertical slider control.
   */
  kThemeMetricSmallVSliderWidth = 47,

  /*
   * The width of the tick marks for a small, vertical slider control.
   */
  kThemeMetricSmallVSliderTickWidth = 48,

  /*
   * The width of the non-label part of a small check box control.
   */
  kThemeMetricSmallCheckBoxWidth = 51,

  /*
   * The width of the non-label part of a small radio button control.
   */
  kThemeMetricSmallRadioButtonWidth = 53,

  /*
   * The minimum width of the thumb of a small, horizontal slider
   * control.
   */
  kThemeMetricSmallHSliderMinThumbWidth = 54,

  /*
   * The minimum width of the thumb of a small, vertical slider control.
   */
  kThemeMetricSmallVSliderMinThumbHeight = 55,

  /*
   * The offset of the tick marks from the appropriate side of a small
   * horizontal slider control.
   */
  kThemeMetricSmallHSliderTickOffset = 56,

  /*
   * The offset of the tick marks from the appropriate side of a small
   * vertical slider control.
   */
  kThemeMetricSmallVSliderTickOffset = 57
};


/*
 *  Discussion:
 *    The following metrics are only available in Mac OS X 10.3 and
 *    later.
 */
enum {
  kThemeMetricComboBoxLargeBottomShadowOffset = 70,
  kThemeMetricComboBoxLargeRightShadowOffset = 71,
  kThemeMetricComboBoxSmallBottomShadowOffset = 72,
  kThemeMetricComboBoxSmallRightShadowOffset = 73,
  kThemeMetricComboBoxLargeDisclosureWidth = 74,
  kThemeMetricComboBoxSmallDisclosureWidth = 75,
  kThemeMetricRoundTextFieldContentInsetLeft = 76,
  kThemeMetricRoundTextFieldContentInsetRight = 77,
  kThemeMetricRoundTextFieldContentInsetBottom = 78,
  kThemeMetricRoundTextFieldContentInsetTop = 79,
  kThemeMetricRoundTextFieldContentHeight = 80,
  kThemeMetricComboBoxMiniBottomShadowOffset = 81,
  kThemeMetricComboBoxMiniDisclosureWidth = 82,
  kThemeMetricComboBoxMiniRightShadowOffset = 83,
  kThemeMetricLittleArrowsMiniHeight = 84,
  kThemeMetricLittleArrowsMiniWidth = 85,
  kThemeMetricLittleArrowsSmallHeight = 86,
  kThemeMetricLittleArrowsSmallWidth = 87,
  kThemeMetricMiniCheckBoxHeight = 88,
  kThemeMetricMiniCheckBoxWidth = 89,
  kThemeMetricMiniDisclosureButtonHeight = 90,
  kThemeMetricMiniDisclosureButtonWidth = 91,
  kThemeMetricMiniHSliderHeight = 92,
  kThemeMetricMiniHSliderMinThumbWidth = 93,
  kThemeMetricMiniHSliderTickHeight = 94,
  kThemeMetricMiniHSliderTickOffset = 95,
  kThemeMetricMiniPopupButtonHeight = 96,
  kThemeMetricMiniPullDownHeight = 97,
  kThemeMetricMiniPushButtonHeight = 98,
  kThemeMetricMiniRadioButtonHeight = 99,
  kThemeMetricMiniRadioButtonWidth = 100,
  kThemeMetricMiniTabCapsWidth  = 101,
  kThemeMetricMiniTabFrameOverlap = 102,
  kThemeMetricMiniTabHeight     = 103,
  kThemeMetricMiniTabOverlap    = 104,
  kThemeMetricMiniVSliderMinThumbHeight = 105,
  kThemeMetricMiniVSliderTickOffset = 106,
  kThemeMetricMiniVSliderTickWidth = 107,
  kThemeMetricMiniVSliderWidth  = 108,
  kThemeMetricRoundTextFieldContentInsetWithIconLeft = 109,
  kThemeMetricRoundTextFieldContentInsetWithIconRight = 110,
  kThemeMetricRoundTextFieldMiniContentHeight = 111,
  kThemeMetricRoundTextFieldMiniContentInsetBottom = 112,
  kThemeMetricRoundTextFieldMiniContentInsetLeft = 113,
  kThemeMetricRoundTextFieldMiniContentInsetRight = 114,
  kThemeMetricRoundTextFieldMiniContentInsetTop = 115,
  kThemeMetricRoundTextFieldMiniContentInsetWithIconLeft = 116,
  kThemeMetricRoundTextFieldMiniContentInsetWithIconRight = 117,
  kThemeMetricRoundTextFieldSmallContentHeight = 118,
  kThemeMetricRoundTextFieldSmallContentInsetBottom = 119,
  kThemeMetricRoundTextFieldSmallContentInsetLeft = 120,
  kThemeMetricRoundTextFieldSmallContentInsetRight = 121,
  kThemeMetricRoundTextFieldSmallContentInsetTop = 122,
  kThemeMetricRoundTextFieldSmallContentInsetWithIconLeft = 123,
  kThemeMetricRoundTextFieldSmallContentInsetWithIconRight = 124,
  kThemeMetricSmallTabFrameOverlap = 125,
  kThemeMetricSmallTabOverlap   = 126,

  /*
   * The height of a small pane splitter. Should only be used in a
   * window with thick borders, like a metal window.
   */
  kThemeMetricSmallPaneSplitterHeight = 127
};


/*
 *  Discussion:
 *    The following metrics are only available in Mac OS X 10.4 and
 *    later.
 */
enum {

  /*
   * The horizontal start offset for the first tick mark on a
   * horizontal slider.
   */
  kThemeMetricHSliderTickOffset = 128,

  /*
   * The vertical start offset for the first tick mark on a vertical
   * slider.
   */
  kThemeMetricVSliderTickOffset = 129,

  /*
   * The minimum thumb height for a thumb on a slider.
   */
  kThemeMetricSliderMinThumbHeight = 130,
  kThemeMetricSliderMinThumbWidth = 131,

  /*
   * The minimum thumb height for a thumb on a scroll bar.
   */
  kThemeMetricScrollBarMinThumbHeight = 132,

  /*
   * The minimum thumb width for a thumb on a scroll bar.
   */
  kThemeMetricScrollBarMinThumbWidth = 133,

  /*
   * The minimum thumb height for a thumb on a small scroll bar.
   */
  kThemeMetricSmallScrollBarMinThumbHeight = 134,

  /*
   * The minimum thumb width for a thumb on a small scroll bar.
   */
  kThemeMetricSmallScrollBarMinThumbWidth = 135,

  /*
   * The height of the round-ended button. (For example, the Kind
   * button in a Finder Search query.)
   */
  kThemeMetricButtonRoundedHeight = 136,

  /*
   * The height of the inset round-ended button. (For example, the
   * Servers button in a Finder Search query.)
   */
  kThemeMetricButtonRoundedRecessedHeight = 137
};


/*
 *  Discussion:
 *    The following metrics are only available in Mac OS X 10.5 and
 *    later.
 */
enum {

  /*
   * This metric refers to the appearance of the separator control.
   * That separator is drawn with the HIThemeDrawSeparator theme
   * primitive. This metric is the height of a horizontal separator or
   * the width of a vertical separator.
   */
  kThemeMetricSeparatorSize     = 138,

  /*
   * The height of the push button control variant that is designed to
   * be used in a textured window.
   */
  kThemeMetricTexturedPushButtonHeight = 139,

  /*
   * The height of the small push button control variant that is
   * designed to be used in a textured window.
   */
  kThemeMetricTexturedSmallPushButtonHeight = 140
};

typedef UInt32                          ThemeMetric;
/*
 *  GetThemeMetric()
 *  
 *  Summary:
 *    Returns a measurement in points for a specified type of user
 *    interface element.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inMetric:
 *      The metric to retrieve.
 *    
 *    outMetric:
 *      The size of the specified user interface element, in points.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetThemeMetric(
  ThemeMetric   inMetric,
  SInt32 *      outMetric)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CopyThemeIdentifier()
 *  
 *  Summary:
 *    Retrieves a string identifying the current theme variant, which
 *    may be Aqua or Graphite.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outIdentifier:
 *      On exit, contains the theme variant identifier. This string
 *      must be released by the caller.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CopyThemeIdentifier(CFStringRef * outIdentifier)              AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Obsolete symbolic names                                                                          */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeMetricCheckBoxGlyphHeight = kThemeMetricCheckBoxHeight,
  kThemeMetricRadioButtonGlyphHeight = kThemeMetricRadioButtonHeight,
  kThemeMetricDisclosureButtonSize = kThemeMetricDisclosureButtonHeight,
  kThemeMetricBestListHeaderHeight = kThemeMetricListHeaderHeight,
  kThemeMetricSmallProgressBarThickness = kThemeMetricNormalProgressBarThickness, /* obsolete */
  kThemeMetricProgressBarThickness = kThemeMetricLargeProgressBarThickness /* obsolete */
};


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __HITHEME__ */

