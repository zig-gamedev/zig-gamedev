/*
     File:       HIToolbox/Appearance.h
 
     Contains:   Appearance Manager Interfaces.
 
     Copyright:  © 1994-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __APPEARANCE__
#define __APPEARANCE__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Appearance Manager constants, etc.                                               */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Appearance Manager Apple Events (1.1 and later)              */

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

enum {
  kAppearanceEventClass         = 'appr', /* Event Class */
  kAEAppearanceChanged          = 'thme', /* Appearance changed (e.g. platinum to hi-tech) */
  kAESystemFontChanged          = 'sysf', /* system font changed */
  kAESmallSystemFontChanged     = 'ssfn', /* small system font changed */
  kAEViewsFontChanged           = 'vfnt' /* views font changed */
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Appearance Manager file types                                                    */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeDataFileType            = 'thme', /* file type for theme files */
  kThemePlatinumFileType        = 'pltn', /* file type for platinum appearance */
  kThemeCustomThemesFileType    = 'scen', /* file type for user themes */
  kThemeSoundTrackFileType      = 'tsnd'
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Appearance Manager Supported Themes                                              */
/* Use CopyThemeIdentifier to get the current theme ID                              */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

#define kThemeAppearancePlatinum       CFSTR( "com.apple.theme.appearance.platinum" )
#define kThemeAppearanceAqua         CFSTR( "com.apple.theme.appearance.aqua" )
#define kThemeAppearanceAquaBlue     CFSTR( "com.apple.theme.appearance.aqua.blue" )
#define kThemeAppearanceAquaGraphite    CFSTR( "com.apple.theme.appearance.aqua.graphite" )


/*
 *  AppearancePartCode
 *  
 *  Summary:
 *    These are part codes returned by a few of the hit testing
 *    Appearance APIs. Many of the Control Manager's ControlPartCodes
 *    are based on these part codes.
 */
typedef SInt16 AppearancePartCode;
enum {

  /*
   * This represents the lack of a part. It will be returned when the
   * Appearance Manager's hit testing logic determines that the input
   * point is not in any part of the widget.
   */
  kAppearancePartMetaNone       = 0,

  /*
   * This represents a widget which is not currently clickable because
   * it is disabled.
   */
  kAppearancePartMetaDisabled   = 254,

  /*
   * This represents a widget which is inactive, presumably because it
   * is in a window that is inactive.
   */
  kAppearancePartMetaInactive   = 255,

  /*
   * The part of a widget which indicates the widget's value. Scroll
   * bar thumbs and slider thumbs are the two main examples.
   */
  kAppearancePartIndicator      = 129,

  /*
   * The part of a widget which moves its value visually upward. Scroll
   * bar up arrows are the main example.
   */
  kAppearancePartUpButton       = 20,

  /*
   * The part of a widget which moves its value visually downward.
   * Scroll bar down arrows are the main example.
   */
  kAppearancePartDownButton     = 21,

  /*
   * The part of a widget which moves its value visually leftward.
   * Scroll bar left arrows are the main example.
   */
  kAppearancePartLeftButton     = kAppearancePartUpButton,

  /*
   * The part of a widget which moves its value visually rightward.
   * Scroll bar right arrows are the main example.
   */
  kAppearancePartRightButton    = kAppearancePartDownButton,

  /*
   * The part of a widget which moves its value visually upward one
   * whole page. Scroll bar page up areas are the main example.
   */
  kAppearancePartPageUpArea     = 22,

  /*
   * The part of a widget which moves its value visually downward one
   * whole page. Scroll bar page down areas are the main example.
   */
  kAppearancePartPageDownArea   = 23,

  /*
   * The part of a widget which moves its value visually leftward one
   * whole page. Scroll bar page left areas are the main example.
   */
  kAppearancePartPageLeftArea   = kAppearancePartPageUpArea,

  /*
   * The part of a widget which moves its value visually rightward one
   * whole page. Scroll bar page right areas are the main example.
   */
  kAppearancePartPageRightArea  = kAppearancePartPageDownArea
};



/*
 *  AppearanceRegionCode
 *  
 *  Summary:
 *    These are region codes used by a few of window-related Appearance
 *    APIs. Many of the Window Manager's WindowRegionCodes are based on
 *    these region codes.
 */
typedef UInt16 AppearanceRegionCode;
enum {
  kAppearanceRegionTitleBar     = 0,
  kAppearanceRegionTitleText    = 1,
  kAppearanceRegionCloseBox     = 2,
  kAppearanceRegionZoomBox      = 3,
  kAppearanceRegionDrag         = 5,
  kAppearanceRegionGrow         = 6,
  kAppearanceRegionCollapseBox  = 7,
  kAppearanceRegionTitleProxyIcon = 8,  /* Mac OS 8.5 forward*/
  kAppearanceRegionStructure    = 32,
  kAppearanceRegionContent      = 33,   /* Content area of the window; empty when the window is collapsed*/
  kAppearanceRegionToolbarButton = 41
};



/*
 *  Discussion:
 *    ThemeBrushes
 */
enum {

  /*
   * Use with kModalWindowClass.
   */
  kThemeBrushDialogBackgroundActive = 1,

  /*
   * Use with kModalWindowClass.
   */
  kThemeBrushDialogBackgroundInactive = 2,

  /*
   * Use with kAlertWindowClass and kMovableAlertWindowClass.
   */
  kThemeBrushAlertBackgroundActive = 3,

  /*
   * Use with kAlertWindowClass and kMovableAlertWindowClass.
   */
  kThemeBrushAlertBackgroundInactive = 4,

  /*
   * Use with kDocumentWindowClass.
   */
  kThemeBrushModelessDialogBackgroundActive = 5,

  /*
   * Use with kDocumentWindowClass.
   */
  kThemeBrushModelessDialogBackgroundInactive = 6,

  /*
   * Use with kFloatingWindowClass and kUtilityWindowClass.
   */
  kThemeBrushUtilityWindowBackgroundActive = 7,

  /*
   * Use with kFloatingWindowClass and kUtilityWindowClass.
   */
  kThemeBrushUtilityWindowBackgroundInactive = 8,

  /*
   * The background used for Finder list views.
   */
  kThemeBrushListViewSortColumnBackground = 9,
  kThemeBrushListViewBackground = 10,
  kThemeBrushIconLabelBackground = 11,
  kThemeBrushListViewSeparator  = 12,
  kThemeBrushChasingArrows      = 13,
  kThemeBrushDragHilite         = 14,

  /*
   * Use with kDocumentWindowClass.
   */
  kThemeBrushDocumentWindowBackground = 15,
  kThemeBrushFinderWindowBackground = 16,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushScrollBarDelimiterActive = 17,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushScrollBarDelimiterInactive = 18,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushFocusHighlight     = 19,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushPopupArrowActive   = 20,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushPopupArrowPressed  = 21,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushPopupArrowInactive = 22,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushAppleGuideCoachmark = 23,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushIconLabelBackgroundSelected = 24,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushStaticAreaFill     = 25,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushActiveAreaFill     = 26,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonFrameActive  = 27,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonFrameInactive = 28,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonFaceActive   = 29,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonFaceInactive = 30,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonFacePressed  = 31,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonActiveDarkShadow = 32,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonActiveDarkHighlight = 33,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonActiveLightShadow = 34,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonActiveLightHighlight = 35,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonInactiveDarkShadow = 36,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonInactiveDarkHighlight = 37,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonInactiveLightShadow = 38,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonInactiveLightHighlight = 39,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonPressedDarkShadow = 40,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonPressedDarkHighlight = 41,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonPressedLightShadow = 42,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushButtonPressedLightHighlight = 43,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushBevelActiveLight   = 44,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushBevelActiveDark    = 45,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushBevelInactiveLight = 46,

  /*
   * Available in Appearance 1.1 (Mac OS 8.5) and later.
   */
  kThemeBrushBevelInactiveDark  = 47,

  /*
   * Available in Appearance 1.1.1 (Mac OS 9.0) and later.
   */
  kThemeBrushNotificationWindowBackground = 48,

  /*
   * Use with kMovableModalWindowClass windows. Available in Mac OS X,
   * and CarbonLib 1.3 and later.
   */
  kThemeBrushMovableModalBackground = 49,

  /*
   * Use with kSheetWindowClass and kSheetAlertWindowClass. This is the
   * backwardly compatible sheet background.
   * kThemeBrushSheetBackgroundTransparent is preferred. Available in
   * Mac OS X, and CarbonLib 1.3 and later.
   */
  kThemeBrushSheetBackgroundOpaque = 50,

  /*
   * Use with kDrawerWindowClass. Available in Mac OS X, and CarbonLib
   * 1.3 and later.
   */
  kThemeBrushDrawerBackground   = 51,

  /*
   * Use with kToolbarWindowClass. Available in Mac OS X, and Carbon
   * Lib 1.6 and later.
   */
  kThemeBrushToolbarBackground  = 52,

  /*
   * Use with kSheetWindowClass and kSheetAlertWindowClass. Not fully
   * transparent -- this brush is the semi-transparent background seen
   * with modern sheets. Available in Mac OS X 10.1 and CarbonLib 1.6,
   * and later.
   */
  kThemeBrushSheetBackgroundTransparent = 53,

  /*
   * Available in Mac OS X 10.1 and CarbonLib 1.6, and later.
   */
  kThemeBrushMenuBackground     = 54,

  /*
   * Available in Mac OS X 10.1 and CarbonLib 1.6, and later.
   */
  kThemeBrushMenuBackgroundSelected = 55,

  /*
   * For lists that use different colors as the background for odd and
   * even rows (like iTunes), this brush is the background for the odd
   * numbered rows. Available in Mac OS X 10.3 and later.
   */
  kThemeBrushListViewOddRowBackground = 56,

  /*
   * For lists that use different colors as the background for odd and
   * even rows (like iTunes), this brush is the background for the even
   * numbered rows. Available in Mac OS X 10.3 and later.
   */
  kThemeBrushListViewEvenRowBackground = 57,

  /*
   * The color for the divider lines drawn between columns in some list
   * views. This brush may have an alpha value associated with it.
   * Drawing this brush with QuickDraw or fetching it as an RGBColor
   * will not necessarily yield satisfactory results. Available in Mac
   * OS X 10.3 and later.
   */
  kThemeBrushListViewColumnDivider = 58,

  /*
   * kThemeBrushSheetBackgroundOpaque synonym.
   * kThemeBrushSheetBackgroundOpaque is preferred
   */
  kThemeBrushSheetBackground    = kThemeBrushSheetBackgroundOpaque,

  /*
   * This is a theme meta-brush. It is a specific color that does not
   * change from theme to theme. Use it instead of a direct RGB value.
   * Available in Mac OS 10.1 and CarbonLib 1.6, and later.
   */
  kThemeBrushBlack              = -1,

  /*
   * This is a theme meta-brush. It is a specific color that does not
   * change from theme to theme. Use it instead of a direct RGB value.
   * Available in Mac OS 10.1 and CarbonLib 1.6, and later.
   */
  kThemeBrushWhite              = -2,

  /*
   * This is a theme meta-brush. It is a specific color that does not
   * change from theme to theme. Use it instead of a direct RGB value.
   * Available in Mac OS 10.1 and CarbonLib 1.6, and later.
   */
  kThemeBrushPrimaryHighlightColor = -3,

  /*
   * This is a theme meta-brush. It is a specific color that does not
   * change from theme to theme. Use it instead of a direct RGB value.
   * Available in Mac OS 10.1 and CarbonLib 1.6, and later.
   */
  kThemeBrushSecondaryHighlightColor = -4,

  /*
   * This is a theme meta-brush. It is a specific color that does not
   * change from theme to theme. Use it instead of a direct RGB value.
   * Available in Mac OS 10.1 and CarbonLib 1.6, and later.
   */
  kThemeBrushAlternatePrimaryHighlightColor = -5
};

typedef SInt16                          ThemeBrush;
enum {
  kThemeTextColorDialogActive   = 1,
  kThemeTextColorDialogInactive = 2,
  kThemeTextColorAlertActive    = 3,
  kThemeTextColorAlertInactive  = 4,
  kThemeTextColorModelessDialogActive = 5,
  kThemeTextColorModelessDialogInactive = 6,
  kThemeTextColorWindowHeaderActive = 7,
  kThemeTextColorWindowHeaderInactive = 8,
  kThemeTextColorPlacardActive  = 9,
  kThemeTextColorPlacardInactive = 10,
  kThemeTextColorPlacardPressed = 11,
  kThemeTextColorPushButtonActive = 12,
  kThemeTextColorPushButtonInactive = 13,
  kThemeTextColorPushButtonPressed = 14,
  kThemeTextColorBevelButtonActive = 15,
  kThemeTextColorBevelButtonInactive = 16,
  kThemeTextColorBevelButtonPressed = 17,
  kThemeTextColorPopupButtonActive = 18,
  kThemeTextColorPopupButtonInactive = 19,
  kThemeTextColorPopupButtonPressed = 20,
  kThemeTextColorIconLabel      = 21,
  kThemeTextColorListView       = 22
};

/* Text Colors available in Appearance 1.0.1 or later */
enum {
  kThemeTextColorDocumentWindowTitleActive = 23,
  kThemeTextColorDocumentWindowTitleInactive = 24,
  kThemeTextColorMovableModalWindowTitleActive = 25,
  kThemeTextColorMovableModalWindowTitleInactive = 26,
  kThemeTextColorUtilityWindowTitleActive = 27,
  kThemeTextColorUtilityWindowTitleInactive = 28,
  kThemeTextColorPopupWindowTitleActive = 29,
  kThemeTextColorPopupWindowTitleInactive = 30,
  kThemeTextColorRootMenuActive = 31,
  kThemeTextColorRootMenuSelected = 32,
  kThemeTextColorRootMenuDisabled = 33,
  kThemeTextColorMenuItemActive = 34,
  kThemeTextColorMenuItemSelected = 35,
  kThemeTextColorMenuItemDisabled = 36,
  kThemeTextColorPopupLabelActive = 37,
  kThemeTextColorPopupLabelInactive = 38
};


/* Text colors available in Appearance 1.1 or later */
enum {
  kThemeTextColorTabFrontActive = 39,
  kThemeTextColorTabNonFrontActive = 40,
  kThemeTextColorTabNonFrontPressed = 41,
  kThemeTextColorTabFrontInactive = 42,
  kThemeTextColorTabNonFrontInactive = 43,
  kThemeTextColorIconLabelSelected = 44,
  kThemeTextColorBevelButtonStickyActive = 45,
  kThemeTextColorBevelButtonStickyInactive = 46
};

/* Text colors available in Appearance 1.1.1 or later */
enum {
  kThemeTextColorNotification   = 47
};


/* Text colors only available later than OS X 10.1.3 */
enum {
  kThemeTextColorSystemDetail   = 48
};

/* These values are specific colors that do not change from             */
/* theme to theme. You can use them instead of using direct RGB values. */
enum {
  kThemeTextColorBlack          = -1,
  kThemeTextColorWhite          = -2
};

typedef SInt16                          ThemeTextColor;
/* States to draw primitives: disabled, active, and pressed (hilited) */
enum {
  kThemeStateInactive           = 0,
  kThemeStateActive             = 1,
  kThemeStatePressed            = 2,
  kThemeStateRollover           = 6,
  kThemeStateUnavailable        = 7,
  kThemeStateUnavailableInactive = 8
};

/* obsolete name */
enum {
  kThemeStateDisabled           = 0
};

enum {
  kThemeStatePressedUp          = 2,    /* draw with up pressed     (increment/decrement buttons) */
  kThemeStatePressedDown        = 3     /* draw with down pressed (increment/decrement buttons) */
};

typedef UInt32                          ThemeDrawState;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Theme cursor selectors available in Appearance 1.1 or later                      */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
 */
enum {
  kThemeArrowCursor             = 0,
  kThemeCopyArrowCursor         = 1,
  kThemeAliasArrowCursor        = 2,
  kThemeContextualMenuArrowCursor = 3,
  kThemeIBeamCursor             = 4,
  kThemeCrossCursor             = 5,
  kThemePlusCursor              = 6,
  kThemeWatchCursor             = 7,    /* Can Animate */
  kThemeClosedHandCursor        = 8,
  kThemeOpenHandCursor          = 9,
  kThemePointingHandCursor      = 10,
  kThemeCountingUpHandCursor    = 11,   /* Can Animate */
  kThemeCountingDownHandCursor  = 12,   /* Can Animate */
  kThemeCountingUpAndDownHandCursor = 13, /* Can Animate */
  kThemeSpinningCursor          = 14,   /* Can Animate */
  kThemeResizeLeftCursor        = 15,
  kThemeResizeRightCursor       = 16,
  kThemeResizeLeftRightCursor   = 17,
  kThemeNotAllowedCursor        = 18,   /* available on Mac OS X 10.2 and later */

  /*
   * Available in Mac OS X 10.3 or later.
   */
  kThemeResizeUpCursor          = 19,

  /*
   * Available in Mac OS X 10.3 or later.
   */
  kThemeResizeDownCursor        = 20,

  /*
   * Available in Mac OS X 10.3 or later.
   */
  kThemeResizeUpDownCursor      = 21,

  /*
   * A special cursor to indicate that letting up the mouse will cause
   * a dragged item to go away. When the item goes away, a poof cloud
   * animation should occur. This cursor should be updated dynamically
   * depending on whether the mouse up action will remove the item.
   * Available in Mac OS X 10.3 or later.
   */
  kThemePoofCursor              = 22
};

typedef UInt32                          ThemeCursor;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Theme menu bar drawing states                                                    */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeMenuBarNormal           = 0,
  kThemeMenuBarSelected         = 1,
  kThemeMenuBarInactive			= 2
};

typedef UInt16                          ThemeMenuBarState;
/* attributes */
enum {
  kThemeMenuSquareMenuBar       = (1 << 0)
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Theme menu drawing states                                                        */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeMenuActive              = 0,
  kThemeMenuSelected            = 1,
  kThemeMenuDisabled            = 3
};

typedef UInt16                          ThemeMenuState;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* MenuType: add kThemeMenuTypeInactive to menu type for DrawThemeMenuBackground if entire  */
/* menu is inactive                                                                         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeMenuTypePullDown        = 0,
  kThemeMenuTypePopUp           = 1,
  kThemeMenuTypeHierarchical    = 2,
  kThemeMenuTypeInactive        = 0x0100
};

typedef UInt16                          ThemeMenuType;
enum {
  kThemeMenuItemPlain           = 0,
  kThemeMenuItemHierarchical    = 1,    /* item has hierarchical arrow*/
  kThemeMenuItemScrollUpArrow   = 2,    /* for scrollable menus, indicates item is scroller*/
  kThemeMenuItemScrollDownArrow = 3,
  kThemeMenuItemAtTop           = 0x0100, /* indicates item is being drawn at top of menu*/
  kThemeMenuItemAtBottom        = 0x0200, /* indicates item is being drawn at bottom of menu*/
  kThemeMenuItemHierBackground  = 0x0400, /* item is within a hierarchical menu*/
  kThemeMenuItemPopUpBackground = 0x0800, /* item is within a popped up menu*/
  kThemeMenuItemHasIcon         = 0x8000, /* add into non-arrow type when icon present*/
  kThemeMenuItemNoBackground    = 0x4000, /* don't draw the menu background while drawing this item (Mac OS X only)*/
  kThemeMenuItemAlignRight      = 0x2000 /* right-align the content of this menu item (SnowLeopard and later)*/
};

typedef UInt16                          ThemeMenuItemType;

/*
 *  Discussion:
 *    ThemeBackgroundKinds
 */
enum {

  /*
   * The theme background used to draw the fill of a tab pane.
   */
  kThemeBackgroundTabPane       = 1,

  /*
   * The theme background used to draw the fill of a window placard.
   */
  kThemeBackgroundPlacard       = 2,

  /*
   * The theme background used to draw the window header of a window of
   * a window that does not contain content that has a hard-lined top
   * edge. An example of this would be icon view in the non side panel
   * Finder.
   */
  kThemeBackgroundWindowHeader  = 3,

  /*
   * The theme background used to draw the window header of a window of
   * a window that does contain content that has a hard-lined top edge.
   * An example of this would be list view in the non side panel Finder.
   */
  kThemeBackgroundListViewWindowHeader = 4,

  /*
   * The theme background used to draw the fill of a secondary group
   * box.
   */
  kThemeBackgroundSecondaryGroupBox = 5,

  /*
   * A special theme brush for drawing metal backgrounds. Currently,
   * this brush only works with HIThemeDrawBackground.
   */
  kThemeBackgroundMetal         = 6
};

typedef UInt32                          ThemeBackgroundKind;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Theme Collection tags for Get/SetTheme                                                   */
/*                                                                                          */
/*  X ALERT: Please note that Get/SetTheme are severely neutered under Mac OS X at present. */
/*           The first group of tags below are available to get under both 9 and X. The     */
/*           second group is 9 only. None of the tags can be used in SetTheme on X, as it   */
/*           is completely inert on X, and will return unimpErr.                            */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeNameTag                 = 'name', /* Str255*/
  kThemeVariantNameTag          = 'varn', /* Str255*/
  kThemeVariantBaseTintTag      = 'tint', /* RGBColor (10.1 and later)*/
  kThemeHighlightColorTag       = 'hcol', /* RGBColor*/
  kThemeScrollBarArrowStyleTag  = 'sbar', /* ThemeScrollBarArrowStyle*/
  kThemeScrollBarThumbStyleTag  = 'sbth', /* ThemeScrollBarThumbStyle*/
  kThemeSoundsEnabledTag        = 'snds', /* Boolean*/
  kThemeDblClickCollapseTag     = 'coll' /* Boolean*/
};

enum {
  kThemeAppearanceFileNameTag   = 'thme', /* Str255*/
  kThemeSystemFontTag           = 'lgsf', /* Str255*/
  kThemeSmallSystemFontTag      = 'smsf', /* Str255*/
  kThemeViewsFontTag            = 'vfnt', /* Str255*/
  kThemeViewsFontSizeTag        = 'vfsz', /* SInt16*/
  kThemeDesktopPatternNameTag   = 'patn', /* Str255*/
  kThemeDesktopPatternTag       = 'patt', /* <variable-length data> (flattened pattern)*/
  kThemeDesktopPictureNameTag   = 'dpnm', /* Str255*/
  kThemeDesktopPictureAliasTag  = 'dpal', /* <alias handle>*/
  kThemeDesktopPictureAlignmentTag = 'dpan', /* UInt32 (see the Picture Alignments below)*/
  kThemeHighlightColorNameTag   = 'hcnm', /* Str255*/
  kThemeExamplePictureIDTag     = 'epic', /* SInt16*/
  kThemeSoundTrackNameTag       = 'sndt', /* Str255*/
  kThemeSoundMaskTag            = 'smsk', /* UInt32*/
  kThemeUserDefinedTag          = 'user', /* Boolean (this should _always_ be true if present - used by Control Panel).*/
  kThemeSmoothFontEnabledTag    = 'smoo', /* Boolean*/
  kThemeSmoothFontMinSizeTag    = 'smos' /* UInt16 (must be >= 12 and <= 24)*/
};

/* Picture Aligmnents that might be reported in the data for kThemeDesktopPictureAlignmentTag*/
enum {
  kTiledOnScreen                = 1,    /* draws picture repeatedly*/
  kCenterOnScreen               = 2,    /* "actual size", shows pattern on sides or clips picture if necessary*/
  kFitToScreen                  = 3,    /* shrinks if necessary*/
  kFillScreen                   = 4,    /* messes up aspect ratio if necessary*/
  kUseBestGuess                 = 5     /* heuristically determines the best way to display the picture based on picture and monitor sizes*/
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Theme Control Settings                                                                   */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeCheckBoxClassicX        = 0,    /* check box with an 'X'*/
  kThemeCheckBoxCheckMark       = 1     /* check box with a real check mark*/
};

typedef UInt16                          ThemeCheckBoxStyle;
enum {
  kThemeScrollBarArrowsSingle   = 0,    /* single arrow on each end*/
  kThemeScrollBarArrowsLowerRight = 1   /* double arrows only on right or bottom*/
};

typedef UInt16                          ThemeScrollBarArrowStyle;
enum {
  kThemeScrollBarThumbNormal    = 0,    /* normal, classic thumb size*/
  kThemeScrollBarThumbProportional = 1  /* proportional thumbs*/
};

typedef UInt16                          ThemeScrollBarThumbStyle;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Font constants                                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
 *  Summary:
 *    A ThemeFontID value is a virtual font ID which can be passed to
 *    one of the Appearance Manager's text-related routines. Within
 *    those routines, the ThemeFontID is mapped into the appropriate
 *    real font (or fonts), size, and style based on the system
 *    appearance (Platinum on Mac OS 9, Aqua on Mac OS X), the string
 *    to be rendered (if any), the language/ script that the app is
 *    running in, and possibly other factors. The ThemeFontIDs allow
 *    you to get the correct text appearance for the platform your app
 *    is currently running on.
 */
enum {

  /*
   * The font used to draw most interface elements. If you can't find a
   * more appropriate font from the list below, you should use this
   * one. This font is suitable for drawing titles on most custom
   * widgets/buttons, as well as most static text in dialogs and
   * windows.
   */
  kThemeSystemFont              = 0,

  /*
   * The font used to draw interface elements when space is at a
   * premium. It draws a slightly smaller font compared to
   * kThemeSystemFont.
   */
  kThemeSmallSystemFont         = 1,

  /*
   * Identical to kThemeSmallSystemFont, except it draws bolded (or
   * otherwise emphasized in some fashion appropriate to your
   * application's language/script).
   */
  kThemeSmallEmphasizedSystemFont = 2,

  /*
   * The font used to draw file and folder names in Finder windows or
   * other browsable lists.
   */
  kThemeViewsFont               = 3,

  /*
   * Identical to kThemeSystemFont, except it draws bolded (or
   * otherwise emphasized in some fashion appropriate to your
   * application's language/script). Only available on Mac OS X or
   * CarbonLib 1.3 or later.
   */
  kThemeEmphasizedSystemFont    = 4,

  /*
   * An analog to the Script Manager's notion of the Application Font.
   * This font is a suitable default choice for your application's
   * document-style text editing areas. Only available on Mac OS X or
   * CarbonLib 1.3 or later.
   */
  kThemeApplicationFont         = 5,

  /*
   * Generally smaller than kThemeSmallSystemFont, this font is
   * appropriate for drawing text labels next to image content that
   * reinforces the text's meaning (such as on a bevel button). Only
   * available on Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeLabelFont               = 6,
  kThemeSystemFontDetail        = 7,
  kThemeSystemFontDetailEmphasized = 8,

  /*
   * The font used to draw menu titles in the menu bar. Only available
   * on Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeMenuTitleFont           = 100,

  /*
   * The font used to draw menu items in the menus. Only available on
   * Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeMenuItemFont            = 101,

  /*
   * The font used to draw menu item marks in the menus. Only available
   * on Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeMenuItemMarkFont        = 102,

  /*
   * The font used to draw menu item command key equivalents in the
   * menus. Only available on Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeMenuItemCmdKeyFont      = 103,

  /*
   * The font used to draw text in most window title bars. Only
   * available on Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeWindowTitleFont         = 104,

  /*
   * The font used to draw text labels on push buttons. Only available
   * on Mac OS X or CarbonLib 1.3 or later.
   */
  kThemePushButtonFont          = 105,

  /*
   * The font used to draw text in utility window title bars. Only
   * available on Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeUtilityWindowTitleFont  = 106,

  /*
   * The font used to draw the first (and most important) message of an
   * alert window. Only available on Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeAlertHeaderFont         = 107,

  /*
   * The font used to draw the label of a toolbar item. Available in
   * Mac OS X 10.2 or later.
   */
  kThemeToolbarFont             = 108,

  /*
   * The appropriate system font for mini-sized controls. Available in
   * Mac OS X 10.3 or later.
   */
  kThemeMiniSystemFont          = 109,

  /*
   * Unlike the other ThemeFontIDs, this one doesn't map to a font
   * appropriate to your application's language or script. It maps
   * directly to the font, size, and style of the current Quickdraw
   * port. This allows you to get somewhat customized behavior out of
   * the APIs which take ThemeFontIDs. Note, however, that
   * kThemeCurrentPortFont does not (and will never) support all
   * Quickdraw styles on all platforms; in particular, outline and
   * shadow style are not supported on Mac OS X. Additionally,
   * kThemeCurrentPortFont is not (and will never be) completely
   * unicode savvy; use of kThemeCurrentPortFont may result in errors
   * having to do with the current port's font not being appropriate
   * for rendering or measuring all glyphs in a given unicode string.
   * Because of overhead associated with gathering Quickdraw font
   * information and converting it to the native font format on Mac OS
   * X, use of kThemeCurrentPortFont may slow down your text drawing
   * and measuring significantly compared to other ThemeFontIDs.
   * Instead of using kThemeCurrentPortFont, your application will
   * probably be better served by using one of the other ThemeFontIDs;
   * use kThemeCurrentPortFont only as a last resort. Only available on
   * Mac OS X or CarbonLib 1.3 or later.
   */
  kThemeCurrentPortFont         = 200,

  /*
   * Only to be used as a fontID parameter in the HIThemeTextInfo
   * structure. It allows the caller to measure and draw text using a
   * CTFontRef and is the correct method for using custom fonts due to
   * QuickDraw deprecation. When the fontID parameter of the
   * HIThemeTextInfo structure is kThemeSpecifiedFont, the version
   * field must be kHIThemeTextInfoVersionOne and the font field must
   * be a valid CTFontRef. Available in Mac OS X 10.5 or later.
   */
  kThemeSpecifiedFont           = 201
};

/* This is the total of the PUBLIC ThemeFontIDs!*/
enum {
  kPublicThemeFontCount         = 21
};

typedef UInt16                          ThemeFontID;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Tab constants                                                                            */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeTabNonFront             = 0,
  kThemeTabNonFrontPressed      = 1,
  kThemeTabNonFrontInactive     = 2,
  kThemeTabFront                = 3,
  kThemeTabFrontInactive        = 4,
  kThemeTabNonFrontUnavailable  = 5,
  kThemeTabFrontUnavailable     = 6
};

typedef UInt16                          ThemeTabStyle;
enum {
  kThemeTabNorth                = 0,
  kThemeTabSouth                = 1,
  kThemeTabEast                 = 2,
  kThemeTabWest                 = 3
};

typedef UInt16                          ThemeTabDirection;

/*
 *  Summary:
 *    Deprecated tab height and overlap constants.
 *  
 *  Discussion:
 *    These constants have been deprecated in favor of theme metrics.
 *    Please do not use them anymore. These constants will be removed
 *    in the next major release of OS X.
 */
enum {

  /*
   * Deprecated. Use kThemeMetricSmallTabHeight.
   */
  kThemeSmallTabHeight          = 16,

  /*
   * Deprecated. Use kThemeMetricLargeTabHeight.
   */
  kThemeLargeTabHeight          = 21,

  /*
   * Deprecated. Use kThemeMetricTabFrameOverlap.
   */
  kThemeTabPaneOverlap          = 3,

  /*
   * Deprecated. Use kThemeMetricSmallTabHeight and
   * kThemeMetricSmallTabFrameOverlap.
   */
  kThemeSmallTabHeightMax       = 19,

  /*
   * Deprecated. Use metric kThemeMetricLargeTabHeight and
   * kThemeMetricTabFrameOverlap.
   */
  kThemeLargeTabHeightMax       = 24
};


/*
 *  Discussion:
 *    ThemeTrackKinds
 */
enum {

  /*
   * This is the primitive used to draw the normal variant of the
   * scroll bar control.
   */
  kThemeScrollBarMedium         = 0,

  /*
   * This is the primitive used to draw the small variant of the scroll
   * bar control.
   */
  kThemeScrollBarSmall          = 1,

  /*
   * This is the primitive used to draw the normal variant of the
   * slider control.
   */
  kThemeSliderMedium            = 2,

  /*
   * This is the primitive used to draw the normal variant of the
   * progress bar control.
   */
  kThemeProgressBarMedium       = 3,

  /*
   * This is the primitive used to draw the normal variant of the
   * indeterminate progress bar control.
   */
  kThemeIndeterminateBarMedium  = 4,

  /*
   * This is the primitive used to draw the normal variant of the
   * relevance bar control.
   */
  kThemeRelevanceBar            = 5,

  /*
   * This is the primitive used to draw the small variant of the slider
   * control.
   */
  kThemeSliderSmall             = 6,

  /*
   * This is the primitive used to draw the large variant of the
   * progress bar control.
   */
  kThemeProgressBarLarge        = 7,

  /*
   * This is the primitive used to draw the large variant of the
   * indeterminate progress bar control.
   */
  kThemeIndeterminateBarLarge   = 8
};


/*
 *  Discussion:
 *    New ThemeTrackKinds on Mac OS X 10.3 and later. Not all of them
 *    are implemented.
 */
enum {

  /*
   * Not implemented. Will return paramErr if used.
   */
  kThemeScrollBarMini           = 9,

  /*
   * A miniature version of the slider.
   */
  kThemeSliderMini              = 10,

  /*
   * Not implemented. Will return paramErr if used.
   */
  kThemeProgressBarMini         = 11,

  /*
   * Not implemented. Will return paramErr if used.
   */
  kThemeIndeterminateBarMini    = 12
};


/*
 *  Discussion:
 *    These are legacy synonyms for previously defined ThemeTrackKinds.
 *    Please use the modern constant names.
 */
enum {
  kThemeMediumScrollBar         = kThemeScrollBarMedium,
  kThemeSmallScrollBar          = kThemeScrollBarSmall,
  kThemeMediumSlider            = kThemeSliderMedium,
  kThemeMediumProgressBar       = kThemeProgressBarMedium,
  kThemeMediumIndeterminateBar  = kThemeIndeterminateBarMedium,
  kThemeSmallSlider             = kThemeSliderSmall,
  kThemeLargeProgressBar        = kThemeProgressBarLarge,
  kThemeLargeIndeterminateBar   = kThemeIndeterminateBarLarge,
  kThemeMiniScrollBar           = kThemeScrollBarMini,
  kThemeMiniSlider              = kThemeSliderMini,
  kThemeMiniProgressBar         = kThemeProgressBarMini,
  kThemeMiniIndeterminateBar    = kThemeIndeterminateBarMini
};

typedef UInt16                          ThemeTrackKind;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Track enable states                                                                      */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
                                        /* track states */
  kThemeTrackActive             = 0,
  kThemeTrackDisabled           = 1,
  kThemeTrackNothingToScroll    = 2,
  kThemeTrackInactive           = 3
};

typedef UInt8                           ThemeTrackEnableState;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Track pressed states                                                                     */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
                                        /* press states (ignored unless track is active) */
  kThemeLeftOutsideArrowPressed = 0x01,
  kThemeLeftInsideArrowPressed  = 0x02,
  kThemeLeftTrackPressed        = 0x04,
  kThemeThumbPressed            = 0x08,
  kThemeRightTrackPressed       = 0x10,
  kThemeRightInsideArrowPressed = 0x20,
  kThemeRightOutsideArrowPressed = 0x40,
  kThemeTopOutsideArrowPressed  = kThemeLeftOutsideArrowPressed,
  kThemeTopInsideArrowPressed   = kThemeLeftInsideArrowPressed,
  kThemeTopTrackPressed         = kThemeLeftTrackPressed,
  kThemeBottomTrackPressed      = kThemeRightTrackPressed,
  kThemeBottomInsideArrowPressed = kThemeRightInsideArrowPressed,
  kThemeBottomOutsideArrowPressed = kThemeRightOutsideArrowPressed
};

typedef UInt8                           ThemeTrackPressState;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Thumb directions                                                                         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
                                        /* thumb direction */
  kThemeThumbPlain              = 0,
  kThemeThumbUpward             = 1,
  kThemeThumbDownward           = 2
};

typedef UInt8                           ThemeThumbDirection;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Track attributes                                                                         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
 *  Discussion:
 *    Theme track attributes control the look of the track elements as
 *    drawn by the DrawThemeTrackFoo as well as the region returned by
 *    GetThemeTrackFoo.
 */
enum {

  /*
   * The track is drawn horizontally.
   */
  kThemeTrackHorizontal         = (1 << 0),

  /*
   * The track progresses from right to left.
   */
  kThemeTrackRightToLeft        = (1 << 1),

  /*
   * The track's thumb should be drawn.
   */
  kThemeTrackShowThumb          = (1 << 2),

  /*
   * The provided thumbRgn should be drawn opaque, not as a ghost.
   */
  kThemeTrackThumbRgnIsNotGhost = (1 << 3),

  /*
   * The track scroll bar doesn't have arrows.  This attribute
   * currently has no effect
   */
  kThemeTrackNoScrollBarArrows  = (1 << 4),

  /*
   * The thumb has focus.  This attribute currently has effect only on
   * sliders.  Available only in Mac OS X 10.2 and later.
   */
  kThemeTrackHasFocus           = (1 << 5),

  /*
   * The track's track (i.e. the groove for a slider or the
   * gutter+arrows of a scroll bar) should not be drawn. Available only
   * in Mac OS X 10.5 and later.
   */
  kThemeTrackHideTrack          = (1 << 6)
};

typedef UInt16                          ThemeTrackAttributes;

/*
 *  ScrollBarTrackInfo
 *  
 *  Summary:
 *    Drawing parameters passed to scroll bar drawing and measuring
 *    theme APIs.
 *  
 *  Discussion:
 *    Used in both the old theme APIs and the newer HITheme APIs.
 */
struct ScrollBarTrackInfo {

  /*
   * The current view range size.
   */
  SInt32              viewsize;               /* current view range size */

  /*
   * ThemeTrackPressState bits that indicate the pressed parts state.
   */
  ThemeTrackPressState  pressState;           /* pressed parts state */
};
typedef struct ScrollBarTrackInfo       ScrollBarTrackInfo;

/*
 *  SliderTrackInfo
 *  
 *  Summary:
 *    Drawing parameters passed to slider drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    Used in both the old theme APIs and the newer HITheme APIs.
 */
struct SliderTrackInfo {

  /*
   * A ThemeThumbDirection indicating the thumb direction.
   */
  ThemeThumbDirection  thumbDir;              /* thumb direction */

  /*
   * ThemeTrackPressState bits that indicate the pressed parts state.
   */
  ThemeTrackPressState  pressState;           /* pressed parts state */
};
typedef struct SliderTrackInfo          SliderTrackInfo;

/*
 *  ProgressTrackInfo
 *  
 *  Summary:
 *    Drawing parameters passed to (indeterminate) progress bar drawing
 *    and measuring theme APIs.
 *  
 *  Discussion:
 *    Used in both the old theme APIs and the newer HITheme APIs.
 */
struct ProgressTrackInfo {

  /*
   * A UInt8 indicating the current phase of the track fill.
   */
  UInt8               phase;                  /* phase for indeterminate progress */
};
typedef struct ProgressTrackInfo        ProgressTrackInfo;

/*
 *  ThemeTrackDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to track drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    Use the newer HITheme APIs.
 */
struct ThemeTrackDrawInfo {

  /*
   * The ThemeTrackKind of the track being drawn or measured.
   */
  ThemeTrackKind      kind;

  /*
   * The bounds of the track is being drawn or measured.
   */
  Rect                bounds;

  /*
   * The minimum allowable value for the track being drawn or measured.
   */
  SInt32              min;

  /*
   * The maximum allowable value for the track being drawn or measured.
   */
  SInt32              max;

  /*
   * The value for the track being drawn or measured.
   */
  SInt32              value;

  /*
   * Leave this reserved field set to 0.
   */
  UInt32              reserved;


  /*
   * A set of ThemeTrackAttributes for the track to be drawn or
   * measured.
   */
  ThemeTrackAttributes  attributes;

  /*
   * A ThemeTrackEnableState describing the state of the track to be
   * drawn or measured.
   */
  ThemeTrackEnableState  enableState;

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
typedef struct ThemeTrackDrawInfo       ThemeTrackDrawInfo;

/*
 *  Summary:
 *    ThemeWindowAttributes
 *  
 *  Discussion:
 *    Theme window attributes control the look of the window elements
 *    as drawn by the DrawThemeWindowFrame.
 */
enum {

  /*
   * This bit means that the size of the window can be changed by the
   * user. The window is drawn with a grow box.
   */
  kThemeWindowHasGrow           = (1 << 0),

  /*
   * This bit means that the window can be zoomed horizontally. The
   * window is drawn with a zoom box.
   */
  kThemeWindowHasHorizontalZoom = (1 << 3),

  /*
   * This bit means that the window can be zoomed vertically. The
   * window is drawn with a zoom box.
   */
  kThemeWindowHasVerticalZoom   = (1 << 4),

  /*
   * This bit means that the window can be zoomed horizontally and
   * vertically. The window is drawn with a zoom box.
   */
  kThemeWindowHasFullZoom       = kThemeWindowHasHorizontalZoom + kThemeWindowHasVerticalZoom,

  /*
   * This bit means that the window can be closed. The window is drawn
   * with a close box.
   */
  kThemeWindowHasCloseBox       = (1 << 5),

  /*
   * This bit means that the window can be collapsed. The window is
   * drawn with a collapse box.
   */
  kThemeWindowHasCollapseBox    = (1 << 6),

  /*
   * This bit means that the window has title text and/or a title icon.
   */
  kThemeWindowHasTitleText      = (1 << 7),

  /*
   * This bit means that the window has been collapsed.
   */
  kThemeWindowIsCollapsed       = (1 << 8),

  /*
   * This bit means that the window has dirty content. Get your mind
   * out of the gutter. That means that the content of the window has
   * changed and needs to be saved. If the window is drawing a close
   * box, it will be drawn dirty.
   */
  kThemeWindowHasDirty          = (1 << 9),

  /*
   * This bit means that the window has a toolbar toggling button.
   */
  kThemeWindowHasToolbarButton  = (1 << 11)
};


typedef UInt32                          ThemeWindowAttributes;

/*
 *  Summary:
 *    ThemeWindowTypes
 *  
 *  Discussion:
 *    Window Types Supported by the Appearance Manager.
 */
enum {
  kThemeDocumentWindow          = 0,
  kThemeDialogWindow            = 1,
  kThemeMovableDialogWindow     = 2,
  kThemeAlertWindow             = 3,
  kThemeMovableAlertWindow      = 4,
  kThemePlainDialogWindow       = 5,
  kThemeShadowDialogWindow      = 6,
  kThemePopupWindow             = 7,
  kThemeUtilityWindow           = 8,
  kThemeUtilitySideWindow       = 9,
  kThemeSheetWindow             = 10,
  kThemeDrawerWindow            = 11
};

typedef UInt16                          ThemeWindowType;

/*
 *  Summary:
 *    ThemeTitleBarWidgets
 *  
 *  Discussion:
 *    Window Widgets Supported by the Appearance Manager.
 */
enum {
  kThemeWidgetCloseBox          = 0,
  kThemeWidgetZoomBox           = 1,
  kThemeWidgetCollapseBox       = 2,
  kThemeWidgetDirtyCloseBox     = 6,
  kThemeWidgetToolbarButton     = 7
};

typedef UInt16                          ThemeTitleBarWidget;

/*
 *  Summary:
 *    ThemeArrowOrientation
 *  
 *  Discussion:
 *    Popup arrow orientations.
 */
enum {
  kThemeArrowLeft               = 0,
  kThemeArrowDown               = 1,
  kThemeArrowRight              = 2,
  kThemeArrowUp                 = 3
};

typedef UInt16                          ThemeArrowOrientation;

/*
 *  Summary:
 *    ThemePopupArrowSizes
 *  
 *  Discussion:
 *    Popup arrow sizes.
 */
enum {
  kThemeArrow3pt                = 0,
  kThemeArrow5pt                = 1,
  kThemeArrow7pt                = 2,
  kThemeArrow9pt                = 3
};

typedef UInt16                          ThemePopupArrowSize;

/*
 *  Summary:
 *    ThemeGrowDirections
 *  
 *  Discussion:
 *    Grow box directions.
 */
enum {
  kThemeGrowLeft                = (1 << 0), /* can grow to the left */
  kThemeGrowRight               = (1 << 1), /* can grow to the right */
  kThemeGrowUp                  = (1 << 2), /* can grow up */
  kThemeGrowDown                = (1 << 3) /* can grow down */
};

typedef UInt16                          ThemeGrowDirection;

/*
 *  Discussion:
 *    ThemeButtonKinds
 */
enum {

  /*
   * Dynamically-sized push button kind. Prior to Mac OS X 10.3 all
   * push button sizes could be determined dynamically: either they
   * were smaller than normal size and therefore small or they were
   * normal size. This constant will never describe a mini push button,
   * regardless of associated bounds. Please use the explicitly-sized
   * kThemePushButton{Normal,Small,Mini} constants.
   */
  kThemePushButton              = 0,
  kThemeCheckBox                = 1,
  kThemeRadioButton             = 2,

  /*
   * Bevel button (obsolete)
   */
  kThemeBevelButton             = 3,

  /*
   * Popup button without text (no label). See ThemeButtonAdornment for
   * glyphs. The arrow button theme name is somewhat confusing. This is
   * the primitive used to draw the control known as the disclosure
   * button.
   */
  kThemeArrowButton             = 4,

  /*
   * Dynamically-sized popup button kind. Prior to Mac OS X 10.3 all
   * popup button sizes could be determined dynamically: either they
   * were smaller than normal size and therefore small or they were
   * normal size. This constant will never describe a mini popup
   * button, regardless of associated bounds. Please use the
   * explicitly-sized kThemePopupButton{Normal,Small,Mini} constants.
   */
  kThemePopupButton             = 5,

  /*
   * This is a disclosure triangle with a label beside it. Used to be
   * confusingly named kThemeDisclosureTriangle.
   */
  kThemeDisclosureTriangle      = 6,

  /*
   * Increment/decrement buttons (no label). This is the primitive used
   * to draw the LittleArrows control.
   */
  kThemeIncDecButton            = 7,

  /*
   * Small-shadow bevel button
   */
  kThemeBevelButtonSmall        = 8,

  /*
   * Med-shadow bevel button
   */
  kThemeBevelButtonMedium       = 3,

  /*
   * Large-shadow bevel button
   */
  kThemeBevelButtonLarge        = 9,

  /*
   * Sort button for top of a list. This is the theme primitive used to
   * draw the top of the columns in the data browser.
   */
  kThemeListHeaderButton        = 10,

  /*
   * This is the primitive used to draw the normal variant of the round
   * button control.
   */
  kThemeRoundButton             = 11,

  /*
   * This is the primitive used to draw the large variant of the round
   * button control.
   */
  kThemeRoundButtonLarge        = 12,

  /*
   * This is the primitive used to draw the small variant of the check
   * box control.
   */
  kThemeCheckBoxSmall           = 13,

  /*
   * This is the primitive used to draw the small variant of the radio
   * button control.
   */
  kThemeRadioButtonSmall        = 14,

  /*
   * This is the primitive used to draw the rounded-corner variant of
   * the bevel button control.
   */
  kThemeRoundedBevelButton      = 15,

  /*
   * This is the primitive used to draw the normal variant of the combo
   * box control.
   */
  kThemeComboBox                = 16,

  /*
   * This is the primitive used to draw the small variant of the combo
   * box control.
   */
  kThemeComboBoxSmall           = 17
};


/*
 *  Discussion:
 *    New ThemeButtonKinds available on Mac OS X 10.3 and later.
 */
enum {

  /*
   * This is the primitive used to draw the mini variant of the combo
   * box control.
   */
  kThemeComboBoxMini            = 18,

  /*
   * This is the primitive used to draw the mini variant of the check
   * box control.
   */
  kThemeCheckBoxMini            = 19,

  /*
   * This is the primitive used to draw the mini variant of the radio
   * button control.
   */
  kThemeRadioButtonMini         = 20,

  /*
   * This is the primitive used to draw the small variant of the
   * LittleArrows control.
   */
  kThemeIncDecButtonSmall       = 21,

  /*
   * This is the primitive used to draw the mini variant of the
   * LittleArrows control.
   */
  kThemeIncDecButtonMini        = 22,

  /*
   * The arrow button theme name is somewhat confusing. This is the
   * primitive used to draw the small variant of the control known as
   * the disclosure button.
   */
  kThemeArrowButtonSmall        = 23,

  /*
   * The arrow button theme name is somewhat confusing. This is the
   * primitive used to draw the mini variant of the control known as
   * the disclosure button.
   */
  kThemeArrowButtonMini         = 24,

  /*
   * Explicitly-sized normal push button kind. Prior to Mac OS X 10.3
   * all push button sizes could be determined dynamically: either they
   * were smaller than normal size and therefore small or they were
   * normal size. Using this constant, an explicitly-sized normal push
   * button can be specified.
   */
  kThemePushButtonNormal        = 25,

  /*
   * Explicitly-sized small push button kind. Prior to Mac OS X 10.3
   * all push button sizes could be determined dynamically: either they
   * were smaller than normal size and therefore small or they were
   * normal size. Using this constant, an explicitly-sized small push
   * button can be specified.
   */
  kThemePushButtonSmall         = 26,

  /*
   * Explicitly-sized mini push button kind. Prior to Mac OS X 10.3 all
   * push button sizes could be determined dynamically: either they
   * were smaller than normal size and therefore small or they were
   * normal size. Since a mini variant was introduced in Mac OS X 10.3,
   * smaller than normal size is can also mean mini. To avoid confusion
   * with existing code, the mini variant will never be implicitly
   * determined and must be explicitly requested with the
   * kThemePushButtonMini constant.
   */
  kThemePushButtonMini          = 27,

  /*
   * Explicitly-sized normal popup button kind. Prior to Mac OS X 10.3
   * all popup button sizes could be determined dynamically: either
   * they were smaller than normal size and therefore small or they
   * were normal size. Using this constant, an explicitly-sized normal
   * popup button can be specified.
   */
  kThemePopupButtonNormal       = 28,

  /*
   * Explicitly-sized small popup button kind. Prior to Mac OS X 10.3
   * all popup button sizes could be determined dynamically: either
   * they were smaller than normal size and therefore small or they
   * were normal size. Using this constant, an explicitly-sized small
   * popup button can be specified.
   */
  kThemePopupButtonSmall        = 29,

  /*
   * Explicitly-sized mini popup button kind. Prior to Mac OS X 10.3
   * all popup button sizes could be determined dynamically: either
   * they were smaller than normal size and therefore small or they
   * were normal size. Since a mini variant was introduced in Mac OS X
   * 10.3, smaller than normal size is can also mean mini. To avoid
   * confusion with existing code, the mini variant will never be
   * implicitly determined and must be explicitly requested with the
   * kThemePopupButtonMini constant.
   */
  kThemePopupButtonMini         = 30
};


/*
 *  Discussion:
 *    New ThemeButtonKinds available on Mac OS X 10.4 and later.
 */
enum {

  /*
   * This is the primitive used to draw the inset variant of the bevel
   * button control.
   */
  kThemeBevelButtonInset        = 31,

  /*
   * This is the primitive used to draw the textured variant of the
   * push button. Similar to kThemePushButtonNormal, but textured, to
   * be rendered on a textured window.
   */
  kThemePushButtonTextured      = 32,

  /*
   * This is the primitive used to draw the small, textured variant of
   * the push button. Similar to kThemePushButtonNormal, but textured,
   * to be rendered on a textured window.
   */
  kThemePushButtonTexturedSmall = 33,

  /*
   * This is the primitive used to draw the help variant of the round
   * button.
   */
  kThemeRoundButtonHelp         = 34,

  /*
   * This is a synonym for kThemePushButtonTextured for code
   * compatibility. Please use kThemePushButtonTextured instead.
   */
  kThemePushButtonInset         = kThemePushButtonTextured,

  /*
   * This is a synonym for kThemePushButtonTexturedSmall for code
   * compatibility. Please use kThemePushButtonTexturedSmall instead.
   */
  kThemePushButtonInsetSmall    = kThemePushButtonTexturedSmall
};



/*
 *  Discussion:
 *    These are legacy synonyms for previously defined
 *    ThemeButtonKinds. Please use the modern constant names.
 */
enum {
  kThemeNormalCheckBox          = kThemeCheckBox,
  kThemeNormalRadioButton       = kThemeRadioButton,
  kThemeLargeBevelButton        = kThemeBevelButtonLarge,
  kThemeMediumBevelButton       = kThemeBevelButtonMedium,
  kThemeMiniCheckBox            = kThemeCheckBoxMini,
  kThemeMiniRadioButton         = kThemeRadioButtonMini,
  kThemeSmallBevelButton        = kThemeBevelButtonSmall,
  kThemeSmallCheckBox           = kThemeCheckBoxSmall,
  kThemeSmallRadioButton        = kThemeRadioButtonSmall,
  kThemeLargeRoundButton        = kThemeRoundButtonLarge,
  kThemeDisclosureButton        = kThemeDisclosureTriangle
};

typedef UInt16                          ThemeButtonKind;

/*
 *  Summary:
 *    ThemeButtonValues
 *  
 *  Discussion:
 *    Common button values
 */
enum {
  kThemeButtonOff               = 0,
  kThemeButtonOn                = 1,
  kThemeButtonMixed             = 2,
  kThemeDisclosureRight         = 0,
  kThemeDisclosureDown          = 1,
  kThemeDisclosureLeft          = 2
};

typedef UInt16                          ThemeButtonValue;


/*
 *  Summary:
 *    ThemeButtonAdornments
 *  
 *  Discussion:
 *    Use these adornments when drawing buttons using the theme
 *    primitive. Note that the bits starting at (1 << 6) are recycled
 *    and have different meanings depending on ThemeButtonKind.
 */
enum {

  /*
   * No adornments.
   */
  kThemeAdornmentNone           = 0,

  /*
   * Draw default ornamentation (for push button and generic well).
   */
  kThemeAdornmentDefault        = (1 << 0),

  /*
   * Draw focus.
   */
  kThemeAdornmentFocus          = (1 << 2),

  /*
   * Draw right to left label.
   */
  kThemeAdornmentRightToLeft    = (1 << 4),

  /*
   * Don't draw or erase label (radio, check, disclosure).
   */
  kThemeAdornmentDrawIndicatorOnly = (1 << 5),

  /*
   * Draw the left border of the button as selected (list header button
   * only).
   */
  kThemeAdornmentHeaderButtonLeftNeighborSelected = (1 << 6),

  /*
   * Draw the right border of the button (list header button only).
   */
  kThemeAdornmentHeaderButtonRightNeighborSelected = (1 << 7),

  /*
   * Draw the sort indicator pointing upward (list header button only).
   */
  kThemeAdornmentHeaderButtonSortUp = (1 << 8),

  /*
   * Draw as a header menu button (list header button only).
   */
  kThemeAdornmentHeaderMenuButton = (1 << 9),

  /*
   * Draw the non-shadow area of the button (list header button only).
   */
  kThemeAdornmentHeaderButtonNoShadow = (1 << 10),

  /*
   * Draw the only the shadow area of the button (list header button
   * only).
   */
  kThemeAdornmentHeaderButtonShadowOnly = (1 << 11),

  /*
   * When drawn selected, do not draw the sort direction arrow (list
   * header button only). Mac OS X 10.4 and later.
   */
  kThemeAdornmentHeaderButtonNoSortArrow = (1 << 12),

  /*
   * Draw a left arrow on the arrow button or draw left disclosure
   * triangle. Can be combined with kThemeAdornmentArrowDownArrow to
   * draw an intermediate disclosure triangle.
   */
  kThemeAdornmentArrowLeftArrow = (1 << 6),

  /*
   * Draw a down arrow on the arrow button or draw right disclosure
   * triangle. Can be combined with kThemeAdornmentArrowLeftArrow to
   * draw an intermediate disclosure triangle.
   */
  kThemeAdornmentArrowDownArrow = (1 << 7),

  /*
   * Draw a double arrow on the arrow button.
   */
  kThemeAdornmentArrowDoubleArrow = (1 << 8),

  /*
   * Draw a up arrow on the arrow button.
   */
  kThemeAdornmentArrowUpArrow   = (1 << 9),

  /*
   * Draw a right disclosure triangle. Can be combined with
   * kThemeAdornmentArrowDownArrow to draw an intermediate disclosure
   * triangle.
   */
  kThemeAdornmentArrowRightArrow = (1 << 10)
};


/*
 *  Discussion:
 *    These are legacy synonyms for previously defined
 *    ThemeButtonAdornments. Please use the modern constant names.
 */
enum {
  kThemeAdornmentNoShadow       = kThemeAdornmentHeaderButtonNoShadow,
  kThemeAdornmentShadowOnly     = kThemeAdornmentHeaderButtonShadowOnly
};

typedef UInt16                          ThemeButtonAdornment;

/*
 *  ThemeButtonDrawInfo
 *  
 *  Summary:
 *    Drawing parameters passed to button drawing and measuring theme
 *    APIs.
 *  
 *  Discussion:
 *    Use the newer HITheme APIs.
 */
struct ThemeButtonDrawInfo {

  /*
   * The ThemeDrawState of the button being drawn or measured.
   */
  ThemeDrawState      state;

  /*
   * The ThemeButtonValue of the button being drawn or measured.
   */
  ThemeButtonValue    value;

  /*
   * The ThemeButtonAdornment(s) with which the button is being drawn
   * or measured.
   */
  ThemeButtonAdornment  adornment;
};
typedef struct ThemeButtonDrawInfo      ThemeButtonDrawInfo;
typedef ThemeButtonDrawInfo *           ThemeButtonDrawInfoPtr;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Sound Support                                                                            */
/*                                                                                          */
/*  X ALERT: Please note that none of the theme sound APIs currently function on X.         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Sound classes                                                                            */
/*                                                                                          */
/* You can use the constants below to set what sounds are active using the SetTheme API.    */
/* Use these with the kThemeSoundMask tag.                                                  */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeNoSounds                = 0,
  kThemeWindowSoundsMask        = (1 << 0),
  kThemeMenuSoundsMask          = (1 << 1),
  kThemeControlSoundsMask       = (1 << 2),
  kThemeFinderSoundsMask        = (1 << 3)
};


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Drag Sounds                                                                              */
/*                                                                                          */
/* Drag sounds are looped for the duration of the drag.                                     */
/*                                                                                          */
/* Call BeginThemeDragSound at the start of the drag.                                       */
/* Call EndThemeDragSound when the drag has finished.                                       */
/*                                                                                          */
/* Note that in order to maintain a consistent user experience, only one drag sound may     */
/* occur at a time.  The sound should be attached to a mouse action, start after the        */
/* mouse goes down and stop when the mouse is released.                                     */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeDragSoundNone           = 0,
  kThemeDragSoundMoveWindow     = 'wmov',
  kThemeDragSoundGrowWindow     = 'wgro',
  kThemeDragSoundMoveUtilWindow = 'umov',
  kThemeDragSoundGrowUtilWindow = 'ugro',
  kThemeDragSoundMoveDialog     = 'dmov',
  kThemeDragSoundMoveAlert      = 'amov',
  kThemeDragSoundMoveIcon       = 'imov',
  kThemeDragSoundSliderThumb    = 'slth',
  kThemeDragSoundSliderGhost    = 'slgh',
  kThemeDragSoundScrollBarThumb = 'sbth',
  kThemeDragSoundScrollBarGhost = 'sbgh',
  kThemeDragSoundScrollBarArrowDecreasing = 'sbad',
  kThemeDragSoundScrollBarArrowIncreasing = 'sbai',
  kThemeDragSoundDragging       = 'drag'
};

typedef OSType                          ThemeDragSoundKind;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* State-change sounds                                                      */
/*                                                                          */
/* State-change sounds are played asynchronously as a one-shot.             */
/*                                                                          */
/* Call PlayThemeSound to play the sound.  The sound will play              */
/* asynchronously until complete, then stop automatically.                  */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kThemeSoundNone               = 0,
  kThemeSoundMenuOpen           = 'mnuo', /* menu sounds */
  kThemeSoundMenuClose          = 'mnuc',
  kThemeSoundMenuItemHilite     = 'mnui',
  kThemeSoundMenuItemRelease    = 'mnus',
  kThemeSoundWindowClosePress   = 'wclp', /* window sounds */
  kThemeSoundWindowCloseEnter   = 'wcle',
  kThemeSoundWindowCloseExit    = 'wclx',
  kThemeSoundWindowCloseRelease = 'wclr',
  kThemeSoundWindowZoomPress    = 'wzmp',
  kThemeSoundWindowZoomEnter    = 'wzme',
  kThemeSoundWindowZoomExit     = 'wzmx',
  kThemeSoundWindowZoomRelease  = 'wzmr',
  kThemeSoundWindowCollapsePress = 'wcop',
  kThemeSoundWindowCollapseEnter = 'wcoe',
  kThemeSoundWindowCollapseExit = 'wcox',
  kThemeSoundWindowCollapseRelease = 'wcor',
  kThemeSoundWindowDragBoundary = 'wdbd',
  kThemeSoundUtilWinClosePress  = 'uclp', /* utility window sounds */
  kThemeSoundUtilWinCloseEnter  = 'ucle',
  kThemeSoundUtilWinCloseExit   = 'uclx',
  kThemeSoundUtilWinCloseRelease = 'uclr',
  kThemeSoundUtilWinZoomPress   = 'uzmp',
  kThemeSoundUtilWinZoomEnter   = 'uzme',
  kThemeSoundUtilWinZoomExit    = 'uzmx',
  kThemeSoundUtilWinZoomRelease = 'uzmr',
  kThemeSoundUtilWinCollapsePress = 'ucop',
  kThemeSoundUtilWinCollapseEnter = 'ucoe',
  kThemeSoundUtilWinCollapseExit = 'ucox',
  kThemeSoundUtilWinCollapseRelease = 'ucor',
  kThemeSoundUtilWinDragBoundary = 'udbd',
  kThemeSoundWindowOpen         = 'wopn', /* window close and zoom action */
  kThemeSoundWindowClose        = 'wcls',
  kThemeSoundWindowZoomIn       = 'wzmi',
  kThemeSoundWindowZoomOut      = 'wzmo',
  kThemeSoundWindowCollapseUp   = 'wcol',
  kThemeSoundWindowCollapseDown = 'wexp',
  kThemeSoundWindowActivate     = 'wact',
  kThemeSoundUtilWindowOpen     = 'uopn',
  kThemeSoundUtilWindowClose    = 'ucls',
  kThemeSoundUtilWindowZoomIn   = 'uzmi',
  kThemeSoundUtilWindowZoomOut  = 'uzmo',
  kThemeSoundUtilWindowCollapseUp = 'ucol',
  kThemeSoundUtilWindowCollapseDown = 'uexp',
  kThemeSoundUtilWindowActivate = 'uact',
  kThemeSoundDialogOpen         = 'dopn',
  kThemeSoundDialogClose        = 'dlgc',
  kThemeSoundAlertOpen          = 'aopn',
  kThemeSoundAlertClose         = 'altc',
  kThemeSoundPopupWindowOpen    = 'pwop',
  kThemeSoundPopupWindowClose   = 'pwcl',
  kThemeSoundButtonPress        = 'btnp', /* button */
  kThemeSoundButtonEnter        = 'btne',
  kThemeSoundButtonExit         = 'btnx',
  kThemeSoundButtonRelease      = 'btnr',
  kThemeSoundDefaultButtonPress = 'dbtp', /* default button */
  kThemeSoundDefaultButtonEnter = 'dbte',
  kThemeSoundDefaultButtonExit  = 'dbtx',
  kThemeSoundDefaultButtonRelease = 'dbtr',
  kThemeSoundCancelButtonPress  = 'cbtp', /* cancel button */
  kThemeSoundCancelButtonEnter  = 'cbte',
  kThemeSoundCancelButtonExit   = 'cbtx',
  kThemeSoundCancelButtonRelease = 'cbtr',
  kThemeSoundCheckboxPress      = 'chkp', /* checkboxes */
  kThemeSoundCheckboxEnter      = 'chke',
  kThemeSoundCheckboxExit       = 'chkx',
  kThemeSoundCheckboxRelease    = 'chkr',
  kThemeSoundRadioPress         = 'radp', /* radio buttons */
  kThemeSoundRadioEnter         = 'rade',
  kThemeSoundRadioExit          = 'radx',
  kThemeSoundRadioRelease       = 'radr',
  kThemeSoundScrollArrowPress   = 'sbap', /* scroll bars */
  kThemeSoundScrollArrowEnter   = 'sbae',
  kThemeSoundScrollArrowExit    = 'sbax',
  kThemeSoundScrollArrowRelease = 'sbar',
  kThemeSoundScrollEndOfTrack   = 'sbte',
  kThemeSoundScrollTrackPress   = 'sbtp',
  kThemeSoundSliderEndOfTrack   = 'slte', /* sliders */
  kThemeSoundSliderTrackPress   = 'sltp',
  kThemeSoundBalloonOpen        = 'blno', /* help balloons */
  kThemeSoundBalloonClose       = 'blnc',
  kThemeSoundBevelPress         = 'bevp', /* Bevel buttons */
  kThemeSoundBevelEnter         = 'beve',
  kThemeSoundBevelExit          = 'bevx',
  kThemeSoundBevelRelease       = 'bevr',
  kThemeSoundLittleArrowUpPress = 'laup', /* Little Arrows */
  kThemeSoundLittleArrowDnPress = 'ladp',
  kThemeSoundLittleArrowEnter   = 'lare',
  kThemeSoundLittleArrowExit    = 'larx',
  kThemeSoundLittleArrowUpRelease = 'laur',
  kThemeSoundLittleArrowDnRelease = 'ladr',
  kThemeSoundPopupPress         = 'popp', /* Popup Buttons */
  kThemeSoundPopupEnter         = 'pope',
  kThemeSoundPopupExit          = 'popx',
  kThemeSoundPopupRelease       = 'popr',
  kThemeSoundDisclosurePress    = 'dscp', /* Disclosure Buttons */
  kThemeSoundDisclosureEnter    = 'dsce',
  kThemeSoundDisclosureExit     = 'dscx',
  kThemeSoundDisclosureRelease  = 'dscr',
  kThemeSoundTabPressed         = 'tabp', /* Tabs */
  kThemeSoundTabEnter           = 'tabe',
  kThemeSoundTabExit            = 'tabx',
  kThemeSoundTabRelease         = 'tabr',
  kThemeSoundDragTargetHilite   = 'dthi', /* drag manager */
  kThemeSoundDragTargetUnhilite = 'dtuh',
  kThemeSoundDragTargetDrop     = 'dtdr',
  kThemeSoundEmptyTrash         = 'ftrs', /* finder */
  kThemeSoundSelectItem         = 'fsel',
  kThemeSoundNewItem            = 'fnew',
  kThemeSoundReceiveDrop        = 'fdrp',
  kThemeSoundCopyDone           = 'fcpd',
  kThemeSoundResolveAlias       = 'fral',
  kThemeSoundLaunchApp          = 'flap',
  kThemeSoundDiskInsert         = 'dski',
  kThemeSoundDiskEject          = 'dske',
  kThemeSoundFinderDragOnIcon   = 'fdon',
  kThemeSoundFinderDragOffIcon  = 'fdof'
};

typedef OSType                          ThemeSoundKind;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Window Metrics                                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Window metrics are used by the Appearance manager to fill in the blanks necessary to    */
/*  draw windows. If a value is not appropriate for the type of window, be sure to fill in  */
/*  the slot in the structure with zero.    For the popupTabOffset parameter, you can pass a*/
/*  real value based on the left edge of the window. This value might be interpreted in a   */
/*  different manner when depending on the value of the popupTabPosition field. The values  */
/*  you can pass into popupTabPosition are:                                                 */
/*                                                                                          */
/*  kThemePopupTabNormalPosition                                                            */
/*      Starts the tab left edge at the position indicated by the popupTabOffset field.     */
/*                                                                                          */
/*  kThemePopupTabCenterOnWindow                                                            */
/*      tells us to ignore the offset field and instead simply center the width of the      */
/*      handle on the window.                                                               */
/*                                                                                          */
/*  kThemePopupTabCenterOnOffset                                                            */
/*      tells us to center the width of the handle around the value passed in offset.       */
/*                                                                                          */
/*  The Appearance Manager will try its best to accomodate the requested placement, but may */
/*  move the handle slightly to make it fit correctly.                                      */
/*                                                                                          */
enum {
  kThemePopupTabNormalPosition  = 0,
  kThemePopupTabCenterOnWindow  = 1,
  kThemePopupTabCenterOnOffset  = 2
};

struct ThemeWindowMetrics {
  UInt16              metricSize;             /* should be always be sizeof( ThemeWindowMetrics )*/
  SInt16              titleHeight;
  SInt16              titleWidth;
  SInt16              popupTabOffset;
  SInt16              popupTabWidth;
  UInt16              popupTabPosition;
};
typedef struct ThemeWindowMetrics       ThemeWindowMetrics;
typedef ThemeWindowMetrics *            ThemeWindowMetricsPtr;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Drawing State                                                                            */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
typedef struct OpaqueThemeDrawingState*  ThemeDrawingState;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Callback procs                                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
 *  ThemeTabTitleDrawProcPtr
 *  
 *  Summary:
 *    Callback function for drawing the label text of a tab.
 */
typedef CALLBACK_API( void , ThemeTabTitleDrawProcPtr )(const Rect *bounds, ThemeTabStyle style, ThemeTabDirection direction, SInt16 depth, Boolean isColorDev, URefCon userData);

/*
 *  ThemeEraseProcPtr
 *  
 *  Summary:
 *    Callback function for erasing the background behind a theme
 *    primitive.
 */
typedef CALLBACK_API( void , ThemeEraseProcPtr )(const Rect *bounds, URefCon eraseData, SInt16 depth, Boolean isColorDev);

/*
 *  ThemeButtonDrawProcPtr
 *  
 *  Summary:
 *    Callback function for drawing the label text of a button.
 */
typedef CALLBACK_API( void , ThemeButtonDrawProcPtr )(const Rect *bounds, ThemeButtonKind kind, const ThemeButtonDrawInfo *info, URefCon userData, SInt16 depth, Boolean isColorDev);

/*
 *  WindowTitleDrawingProcPtr
 *  
 *  Summary:
 *    Callback function for drawing the title text of a window.
 */
typedef CALLBACK_API( void , WindowTitleDrawingProcPtr )(const Rect *bounds, SInt16 depth, Boolean colorDevice, URefCon userData);

/*
 *  ThemeIteratorProcPtr
 *  
 *  Summary:
 *    Callback function for iterating over all known themes.
 */
typedef CALLBACK_API( Boolean , ThemeIteratorProcPtr )(ConstStr255Param inFileName, SInt16 resID, Collection inThemeSettings, PRefCon inUserData);
typedef STACK_UPP_TYPE(ThemeTabTitleDrawProcPtr)                ThemeTabTitleDrawUPP;
typedef STACK_UPP_TYPE(ThemeEraseProcPtr)                       ThemeEraseUPP;
typedef STACK_UPP_TYPE(ThemeButtonDrawProcPtr)                  ThemeButtonDrawUPP;
typedef STACK_UPP_TYPE(WindowTitleDrawingProcPtr)               WindowTitleDrawingUPP;
typedef STACK_UPP_TYPE(ThemeIteratorProcPtr)                    ThemeIteratorUPP;
/*
 *  NewThemeTabTitleDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ThemeTabTitleDrawUPP
NewThemeTabTitleDrawUPP(ThemeTabTitleDrawProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  NewThemeEraseUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ThemeEraseUPP
NewThemeEraseUPP(ThemeEraseProcPtr userRoutine)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  NewThemeButtonDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ThemeButtonDrawUPP
NewThemeButtonDrawUPP(ThemeButtonDrawProcPtr userRoutine)     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  NewWindowTitleDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern WindowTitleDrawingUPP
NewWindowTitleDrawingUPP(WindowTitleDrawingProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  NewThemeIteratorUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ThemeIteratorUPP
NewThemeIteratorUPP(ThemeIteratorProcPtr userRoutine)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeThemeTabTitleDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeThemeTabTitleDrawUPP(ThemeTabTitleDrawUPP userUPP)     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeThemeEraseUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeThemeEraseUPP(ThemeEraseUPP userUPP)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeThemeButtonDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeThemeButtonDrawUPP(ThemeButtonDrawUPP userUPP)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeWindowTitleDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeWindowTitleDrawingUPP(WindowTitleDrawingUPP userUPP)   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeThemeIteratorUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeThemeIteratorUPP(ThemeIteratorUPP userUPP)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeThemeTabTitleDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeThemeTabTitleDrawUPP(
  const Rect *          bounds,
  ThemeTabStyle         style,
  ThemeTabDirection     direction,
  SInt16                depth,
  Boolean               isColorDev,
  URefCon               userData,
  ThemeTabTitleDrawUPP  userUPP)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeThemeEraseUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeThemeEraseUPP(
  const Rect *   bounds,
  URefCon        eraseData,
  SInt16         depth,
  Boolean        isColorDev,
  ThemeEraseUPP  userUPP)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeThemeButtonDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeThemeButtonDrawUPP(
  const Rect *                 bounds,
  ThemeButtonKind              kind,
  const ThemeButtonDrawInfo *  info,
  URefCon                      userData,
  SInt16                       depth,
  Boolean                      isColorDev,
  ThemeButtonDrawUPP           userUPP)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeWindowTitleDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeWindowTitleDrawingUPP(
  const Rect *           bounds,
  SInt16                 depth,
  Boolean                colorDevice,
  URefCon                userData,
  WindowTitleDrawingUPP  userUPP)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeThemeIteratorUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeThemeIteratorUPP(
  ConstStr255Param  inFileName,
  SInt16            resID,
  Collection        inThemeSettings,
  PRefCon           inUserData,
  ThemeIteratorUPP  userUPP)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

#if __MACH__
  #ifdef __cplusplus
    inline ThemeTabTitleDrawUPP                                 NewThemeTabTitleDrawUPP(ThemeTabTitleDrawProcPtr userRoutine) { return userRoutine; }
    inline ThemeEraseUPP                                        NewThemeEraseUPP(ThemeEraseProcPtr userRoutine) { return userRoutine; }
    inline ThemeButtonDrawUPP                                   NewThemeButtonDrawUPP(ThemeButtonDrawProcPtr userRoutine) { return userRoutine; }
    inline WindowTitleDrawingUPP                                NewWindowTitleDrawingUPP(WindowTitleDrawingProcPtr userRoutine) { return userRoutine; }
    inline ThemeIteratorUPP                                     NewThemeIteratorUPP(ThemeIteratorProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeThemeTabTitleDrawUPP(ThemeTabTitleDrawUPP) { }
    inline void                                                 DisposeThemeEraseUPP(ThemeEraseUPP) { }
    inline void                                                 DisposeThemeButtonDrawUPP(ThemeButtonDrawUPP) { }
    inline void                                                 DisposeWindowTitleDrawingUPP(WindowTitleDrawingUPP) { }
    inline void                                                 DisposeThemeIteratorUPP(ThemeIteratorUPP) { }
    inline void                                                 InvokeThemeTabTitleDrawUPP(const Rect * bounds, ThemeTabStyle style, ThemeTabDirection direction, SInt16 depth, Boolean isColorDev, URefCon userData, ThemeTabTitleDrawUPP userUPP) { (*userUPP)(bounds, style, direction, depth, isColorDev, userData); }
    inline void                                                 InvokeThemeEraseUPP(const Rect * bounds, URefCon eraseData, SInt16 depth, Boolean isColorDev, ThemeEraseUPP userUPP) { (*userUPP)(bounds, eraseData, depth, isColorDev); }
    inline void                                                 InvokeThemeButtonDrawUPP(const Rect * bounds, ThemeButtonKind kind, const ThemeButtonDrawInfo * info, URefCon userData, SInt16 depth, Boolean isColorDev, ThemeButtonDrawUPP userUPP) { (*userUPP)(bounds, kind, info, userData, depth, isColorDev); }
    inline void                                                 InvokeWindowTitleDrawingUPP(const Rect * bounds, SInt16 depth, Boolean colorDevice, URefCon userData, WindowTitleDrawingUPP userUPP) { (*userUPP)(bounds, depth, colorDevice, userData); }
    inline Boolean                                              InvokeThemeIteratorUPP(ConstStr255Param inFileName, SInt16 resID, Collection inThemeSettings, PRefCon inUserData, ThemeIteratorUPP userUPP) { return (*userUPP)(inFileName, resID, inThemeSettings, inUserData); }
  #else
    #define NewThemeTabTitleDrawUPP(userRoutine)                ((ThemeTabTitleDrawUPP)userRoutine)
    #define NewThemeEraseUPP(userRoutine)                       ((ThemeEraseUPP)userRoutine)
    #define NewThemeButtonDrawUPP(userRoutine)                  ((ThemeButtonDrawUPP)userRoutine)
    #define NewWindowTitleDrawingUPP(userRoutine)               ((WindowTitleDrawingUPP)userRoutine)
    #define NewThemeIteratorUPP(userRoutine)                    ((ThemeIteratorUPP)userRoutine)
    #define DisposeThemeTabTitleDrawUPP(userUPP)
    #define DisposeThemeEraseUPP(userUPP)
    #define DisposeThemeButtonDrawUPP(userUPP)
    #define DisposeWindowTitleDrawingUPP(userUPP)
    #define DisposeThemeIteratorUPP(userUPP)
    #define InvokeThemeTabTitleDrawUPP(bounds, style, direction, depth, isColorDev, userData, userUPP) (*userUPP)(bounds, style, direction, depth, isColorDev, userData)
    #define InvokeThemeEraseUPP(bounds, eraseData, depth, isColorDev, userUPP) (*userUPP)(bounds, eraseData, depth, isColorDev)
    #define InvokeThemeButtonDrawUPP(bounds, kind, info, userData, depth, isColorDev, userUPP) (*userUPP)(bounds, kind, info, userData, depth, isColorDev)
    #define InvokeWindowTitleDrawingUPP(bounds, depth, colorDevice, userData, userUPP) (*userUPP)(bounds, depth, colorDevice, userData)
    #define InvokeThemeIteratorUPP(inFileName, resID, inThemeSettings, inUserData, userUPP) (*userUPP)(inFileName, resID, inThemeSettings, inUserData)
  #endif
#endif

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Menu Drawing callbacks                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
 *  MenuTitleDrawingProcPtr
 *  
 *  Summary:
 *    Callback function for drawing the title text of a menu.
 */
typedef CALLBACK_API( void , MenuTitleDrawingProcPtr )(const Rect *inBounds, SInt16 inDepth, Boolean inIsColorDevice, SRefCon inUserData);

/*
 *  MenuItemDrawingProcPtr
 *  
 *  Summary:
 *    Callback function for drawing the text of a menu item.
 */
typedef CALLBACK_API( void , MenuItemDrawingProcPtr )(const Rect *inBounds, SInt16 inDepth, Boolean inIsColorDevice, SRefCon inUserData);
typedef STACK_UPP_TYPE(MenuTitleDrawingProcPtr)                 MenuTitleDrawingUPP;
typedef STACK_UPP_TYPE(MenuItemDrawingProcPtr)                  MenuItemDrawingUPP;
/*
 *  NewMenuTitleDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern MenuTitleDrawingUPP
NewMenuTitleDrawingUPP(MenuTitleDrawingProcPtr userRoutine)   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  NewMenuItemDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern MenuItemDrawingUPP
NewMenuItemDrawingUPP(MenuItemDrawingProcPtr userRoutine)     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeMenuTitleDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeMenuTitleDrawingUPP(MenuTitleDrawingUPP userUPP)       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeMenuItemDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeMenuItemDrawingUPP(MenuItemDrawingUPP userUPP)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeMenuTitleDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeMenuTitleDrawingUPP(
  const Rect *         inBounds,
  SInt16               inDepth,
  Boolean              inIsColorDevice,
  SRefCon              inUserData,
  MenuTitleDrawingUPP  userUPP)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeMenuItemDrawingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeMenuItemDrawingUPP(
  const Rect *        inBounds,
  SInt16              inDepth,
  Boolean             inIsColorDevice,
  SRefCon             inUserData,
  MenuItemDrawingUPP  userUPP)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

#if __MACH__
  #ifdef __cplusplus
    inline MenuTitleDrawingUPP                                  NewMenuTitleDrawingUPP(MenuTitleDrawingProcPtr userRoutine) { return userRoutine; }
    inline MenuItemDrawingUPP                                   NewMenuItemDrawingUPP(MenuItemDrawingProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeMenuTitleDrawingUPP(MenuTitleDrawingUPP) { }
    inline void                                                 DisposeMenuItemDrawingUPP(MenuItemDrawingUPP) { }
    inline void                                                 InvokeMenuTitleDrawingUPP(const Rect * inBounds, SInt16 inDepth, Boolean inIsColorDevice, SRefCon inUserData, MenuTitleDrawingUPP userUPP) { (*userUPP)(inBounds, inDepth, inIsColorDevice, inUserData); }
    inline void                                                 InvokeMenuItemDrawingUPP(const Rect * inBounds, SInt16 inDepth, Boolean inIsColorDevice, SRefCon inUserData, MenuItemDrawingUPP userUPP) { (*userUPP)(inBounds, inDepth, inIsColorDevice, inUserData); }
  #else
    #define NewMenuTitleDrawingUPP(userRoutine)                 ((MenuTitleDrawingUPP)userRoutine)
    #define NewMenuItemDrawingUPP(userRoutine)                  ((MenuItemDrawingUPP)userRoutine)
    #define DisposeMenuTitleDrawingUPP(userUPP)
    #define DisposeMenuItemDrawingUPP(userUPP)
    #define InvokeMenuTitleDrawingUPP(inBounds, inDepth, inIsColorDevice, inUserData, userUPP) (*userUPP)(inBounds, inDepth, inIsColorDevice, inUserData)
    #define InvokeMenuItemDrawingUPP(inBounds, inDepth, inIsColorDevice, inUserData, userUPP) (*userUPP)(inBounds, inDepth, inIsColorDevice, inUserData)
  #endif
#endif

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Appearance Manager APIs                                                         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Registering Appearance-Savvy Applications */
#if !__LP64__
/*
 *  RegisterAppearanceClient()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This function does nothing on Mac OS X. Do not call it.
 *  
 *  Summary:
 *    Registers a process as a client of the Appearance Manager.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
RegisterAppearanceClient(void)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  UnregisterAppearanceClient()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This function does nothing on Mac OS X. Do not call it.
 *  
 *  Summary:
 *    Unregisters a process as a client of the Appearance Manager.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
UnregisterAppearanceClient(void)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IsAppearanceClient()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This function always returns true on Mac OS X. Do not call it.
 *  
 *  Summary:
 *    Indicates whether a process is registered with the Appearance
 *    Manager.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern Boolean 
IsAppearanceClient(const ProcessSerialNumber * process)       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*****************************************************************************
    NOTES ON THEME BRUSHES
    Theme brushes can be either colors or patterns, depending on the theme.
    Because of this, you should be prepared to handle the case where a brush
    is a pattern and save and restore the pnPixPat and bkPixPat fields of
    your GrafPorts when saving the fore and back colors. Also, since patterns
    in bkPixPat override the background color of the window, you should use
    BackPat to set your background pattern to a normal white pattern. This
    will ensure that you can use RGBBackColor to set your back color to white,
    call EraseRect and get the expected results.
*****************************************************************************/

/*
 *  SetThemePen()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.4 and later, use HIThemeSetStroke, and draw using
 *    CoreGraphics.
 *  
 *  Summary:
 *    Sets the foreground color and/or pen pattern of the current
 *    GrafPort to allow drawing with a specified theme brush.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
SetThemePen(
  ThemeBrush   inBrush,
  SInt16       inDepth,
  Boolean      inIsColorDevice)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetThemeBackground()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.4 and later, use HIThemeSetFill, and draw using
 *    CoreGraphics.
 *  
 *  Summary:
 *    Sets the background color and/or fill pattern of the current
 *    GrafPort to allow drawing with a specified theme brush.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
SetThemeBackground(
  ThemeBrush   inBrush,
  SInt16       inDepth,
  Boolean      inIsColorDevice)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetThemeTextColor()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.4 and later, use HIThemeTextFill, and draw with
 *    CoreGraphics, ATSUI, or HIThemeDrawTextBox.
 *  
 *  Summary:
 *    Sets the foreground color and/or pen pattern of the current
 *    GrafPort to allow drawing with a specified theme text color.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
SetThemeTextColor(
  ThemeTextColor   inColor,
  SInt16           inDepth,
  Boolean          inIsColorDevice)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/* Window Placards, Headers and Frames */
/*
 *  DrawThemeWindowHeader()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawHeader.
 *  
 *  Summary:
 *    Draws a window header theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeWindowHeader(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeWindowListViewHeader()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawHeader.
 *  
 *  Summary:
 *    Draws a window header theme primitive that is suitable for use at
 *    the top of a list view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeWindowListViewHeader(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemePlacard()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawPlacard.
 *  
 *  Summary:
 *    Draws a placard theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemePlacard(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeEditTextFrame()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawFrame.
 *  
 *  Summary:
 *    Draws a frame theme primitive suitable for use around an edit
 *    text control.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeEditTextFrame(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeListBoxFrame()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawFrame.
 *  
 *  Summary:
 *    Draws a frame theme primitive suitable for use around a list box.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeListBoxFrame(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/* Keyboard Focus Drawing */
/*
 *  DrawThemeFocusRect()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawFocusRect. This API
 *    erases the focus ring when called with inHasFocus equal to false,
 *    but HIThemeDrawFocsuRect does not; to simulate the behavior of
 *    DrawThemeFocusRect in this case, simply erase the desired area
 *    yourself.
 *  
 *  Summary:
 *    Draws a focus ring around a specified rectangle.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeFocusRect(
  const Rect *  inRect,
  Boolean       inHasFocus)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/* Dialog Group Boxes and Separators */
/*
 *  DrawThemePrimaryGroup()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawGroupBox.
 *  
 *  Summary:
 *    Draws a primary group theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemePrimaryGroup(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeSecondaryGroup()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawGroupBox.
 *  
 *  Summary:
 *    Draws a secondary group theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeSecondaryGroup(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeSeparator()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawSeparator.
 *  
 *  Summary:
 *    Draws a separator theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeSeparator(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ BEGIN APPEARANCE 1.0.1 ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* The following Appearance Manager APIs are only available in Appearance 1.0.1 or later */
/*
 *  DrawThemeModelessDialogFrame()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawWindowFrame.
 *  
 *  Summary:
 *    Draws a modeless dialog window frame theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeModelessDialogFrame(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeGenericWell()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawGenericWell.
 *  
 *  Summary:
 *    Draws an image well theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeGenericWell(
  const Rect *     inRect,
  ThemeDrawState   inState,
  Boolean          inFillCenter)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeFocusRegion()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawFocusRect. This API
 *    erases the focus ring when called with inHasFocus equal to false,
 *    but HIThemeDrawFocsuRect does not; to simulate the behavior of
 *    DrawThemeFocusRect in this case, simply erase the desired area
 *    yourself.
 *  
 *  Summary:
 *    Draws a focus ring around a specified region.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeFocusRegion(
  RgnHandle   inRegion,
  Boolean     inHasFocus)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IsThemeInColor()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X, the Aqua theme is always drawn in color, and this
 *    API always returns true.
 *  
 *  Summary:
 *    Indicates whether the current theme would be drawn in color or
 *    black and white.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern Boolean 
IsThemeInColor(
  SInt16    inDepth,
  Boolean   inIsColorDevice)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeAccentColors()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This function always returns appearanceThemeHasNoAccents on Mac
 *    OS X. Do not call it.
 *  
 *  Summary:
 *    Returns the accent colors for the current theme.
 *  
 *  Discussion:
 *    GetThemeAccentColors will only work in the platinum theme on Mac
 *    OS 8.x and 9.x. It returns an error on Mac OS X, since the Aqua
 *    theme does not support accent colors.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
GetThemeAccentColors(CTabHandle * outColors)                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeMenuBarBackground()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawMenuBarBackground.
 *  
 *  Summary:
 *    Draws a menubar background theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeMenuBarBackground(
  const Rect *        inBounds,
  ThemeMenuBarState   inState,
  UInt32              inAttributes)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeMenuTitle()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawMenuTitle.
 *  
 *  Summary:
 *    Draws a menu title theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeMenuTitle(
  const Rect *          inMenuBarRect,
  const Rect *          inTitleRect,
  ThemeMenuState        inState,
  UInt32                inAttributes,
  MenuTitleDrawingUPP   inTitleProc,         /* can be NULL */
  URefCon               inTitleData)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/*
 *  GetThemeMenuBarHeight()
 *  
 *  Summary:
 *    Returns the height of the menubar, in points.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outHeight:
 *      On exit, contains the height of the menubar, in points.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
GetThemeMenuBarHeight(SInt16 * outHeight)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DrawThemeMenuBackground()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawMenuBackground.
 *  
 *  Summary:
 *    Draws a menu background theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeMenuBackground(
  const Rect *    inMenuRect,
  ThemeMenuType   inMenuType)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeMenuBackgroundRegion()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeGetMenuBackgroundShape.
 *  
 *  Summary:
 *    Constructs a region that covers the entire area of a menu.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
GetThemeMenuBackgroundRegion(
  const Rect *    inMenuRect,
  ThemeMenuType   menuType,
  RgnHandle       region)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeMenuItem()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawMenuItem.
 *  
 *  Summary:
 *    Draws a menu item theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeMenuItem(
  const Rect *         inMenuRect,
  const Rect *         inItemRect,
  SInt16               inVirtualMenuTop,
  SInt16               inVirtualMenuBottom,
  ThemeMenuState       inState,
  ThemeMenuItemType    inItemType,
  MenuItemDrawingUPP   inDrawProc,                /* can be NULL */
  URefCon              inUserData)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeMenuSeparator()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawMenuSeparator.
 *  
 *  Summary:
 *    Draws a menu item separator theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
DrawThemeMenuSeparator(const Rect * inItemRect)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ BEGIN APPEARANCE 1.1 ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ THEME SWITCHING ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*                                                                                                  */
/*  X ALERT: Please note that Get/SetTheme are severely neutered under Mac OS X at present.         */
/*           See the note above regarding what collection tags are supported under X.               */

/*
 *  GetTheme()
 *  
 *  Summary:
 *    Returns information about the current theme.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    ioCollection:
 *      On entry, a valid empty collection. On exit, contains
 *      information about the current theme. See Theme Collection Tags
 *      for the set of collection tags that are used to retrieve
 *      information from this collection.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetTheme(Collection ioCollection)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetTheme()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This API does not modify the current theme in any way on Mac OS X.
 *  
 *  Summary:
 *    Sets information about the current theme.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
SetTheme(Collection ioCollection)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IterateThemes()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This API does nothing on Mac OS X; it does not call the theme
 *    iterator callback function.
 *  
 *  Summary:
 *    Iterates over all known themes.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
IterateThemes(
  ThemeIteratorUPP   inProc,
  PRefCon            inUserData)       /* can be NULL */      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ TABS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  DrawThemeTabPane()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawTabPane.
 *  
 *  Summary:
 *    Draws a tab pane theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeTabPane(
  const Rect *     inRect,
  ThemeDrawState   inState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeTab()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawTab.
 *  
 *  Summary:
 *    Draws a tab theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeTab(
  const Rect *           inRect,
  ThemeTabStyle          inStyle,
  ThemeTabDirection      inDirection,
  ThemeTabTitleDrawUPP   labelProc,         /* can be NULL */
  URefCon                userData)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTabRegion()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetTabDrawShape.
 *  
 *  Summary:
 *    Constructs a region that covers the area of a tab.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeTabRegion(
  const Rect *        inRect,
  ThemeTabStyle       inStyle,
  ThemeTabDirection   inDirection,
  RgnHandle           ioRgn)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ CURSORS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  SetThemeCursor()
 *  
 *  Summary:
 *    Sets the current cursor to a specified theme-compliant cursor.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inCursor:
 *      The cursor to set.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
SetThemeCursor(ThemeCursor inCursor)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetAnimatedThemeCursor()
 *  
 *  Summary:
 *    Sets the current cursor to the next frame of a specified
 *    theme-compliant cursor.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inCursor:
 *      The cursor to set.
 *    
 *    inAnimationStep:
 *      A value indicating which frame of animation should be used. The
 *      exact value does not matter, but the value should increase by 1
 *      before each call to this API. Not all cursors support
 *      animation; those that do not will ignore this parameter.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
SetAnimatedThemeCursor(
  ThemeCursor   inCursor,
  UInt32        inAnimationStep)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ CONTROL STYLE SETTINGS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  GetThemeScrollBarThumbStyle()
 *  
 *  Summary:
 *    Retrieves the current scrollbar thumb style, which may be
 *    proportional or non-proportional.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outStyle:
 *      On exit, contains the current scrollbar thumb style.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeScrollBarThumbStyle(ThemeScrollBarThumbStyle * outStyle) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetThemeScrollBarArrowStyle()
 *  
 *  Summary:
 *    Retrieves the current scrollbar arrow style. Arrows may be single
 *    at each end of the scrollbar, double at one end, or double at
 *    both ends.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outStyle:
 *      On exit, contains the current scrollbar arrow style.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeScrollBarArrowStyle(ThemeScrollBarArrowStyle * outStyle) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetThemeCheckBoxStyle()
 *  
 *  Summary:
 *    Retrieves the current checkbox style, which may use an X or a
 *    checkmark.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outStyle:
 *      On exit, contains the current checkbox style.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeCheckBoxStyle(ThemeCheckBoxStyle * outStyle)          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ FONTS/TEXT ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  UseThemeFont()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIThemeDrawTextBox instead.
 *  
 *  Summary:
 *    Sets the font of the current GrafPort to allow drawing with a
 *    specified theme font.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
UseThemeFont(
  ThemeFontID   inFontID,
  ScriptCode    inScript)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeFont()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Some theme fonts cannot be drawn using QuickDraw. Use
 *    HIThemeDrawTextBox instead.
 *  
 *  Summary:
 *    Retrieves the QuickDraw font name, size, and style for a
 *    specified theme font.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeFont(
  ThemeFontID   inFontID,
  ScriptCode    inScript,
  Str255        outFontName,       /* can be NULL */
  SInt16 *      outFontSize,
  Style *       outStyle)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeTextBox()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawTextBox.
 *  
 *  Summary:
 *    Draws text into the area you specify.
 *  
 *  Discussion:
 *    DrawThemeTextBox allows you to draw theme-savvy (ie. Aqua-savvy
 *    on Mac OS X) text. It is unicode savvy (although only partially
 *    so under CarbonLib), and allows you to customize certain text
 *    rendering characteristics such as the font, wrapping behavior,
 *    and justification. The text is drawn into the CGContextRef you
 *    provide, or into the current Quickdraw port if no CGContextRef is
 *    provided. None of DrawThemeTextBox's parameters imply a color, so
 *    you must set up the desired text color separately before calling
 *    DrawThemeTextBox. If you provide a CGContextRef, its fill color
 *    will be used to draw the text. If you do not provide a
 *    CGContextRef, a color based on the current Quickdraw port's
 *    foreground color and the grayishTextOr mode (if set) will be used
 *    to draw the text.
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
 *      incompatible with DrawThemeTextBox's caches.
 *    
 *    inFontID:
 *      The ThemeFontID describing the font you'd like to render the
 *      text with. See the discussion of ThemeFontIDs elsewhere in this
 *      header.
 *    
 *    inState:
 *      The ThemeDrawState describing the the state of the interface
 *      element you are drawing the text for. If, for example, you are
 *      drawing text for an inactive window, you would pass
 *      kThemeStateInactive. The ThemeDrawState is generally only used
 *      to determine the shadow characteristics for the text on Mac OS
 *      X. Note that the ThemeDrawState does NOT imply a color. It is
 *      NOT used as a mechanism for graying the text. If you wish to
 *      draw grayed text, you must set up the desired gray color and
 *      apply it to either the current Quickdraw port or the
 *      CGContextRef as appropriate.
 *    
 *    inWrapToWidth:
 *      A Boolean indicating whether you want to draw multiple lines of
 *      text wrapped to a bounding box. False indicates that only one
 *      line of text should be drawn without any sort of wrapping.
 *    
 *    inBoundingBox:
 *      The rectangle (in coordinates relative to the current Quickdraw
 *      port) describing the area to draw the text within. The first
 *      line of text will be top-justified to this rectangle. Wrapping
 *      (if desired) will happen at the horizontal extent of this
 *      rectangle. Regardless of the amount of text in your
 *      CFStringRef, all drawn text will be clipped to this rectangle.
 *    
 *    inJust:
 *      The horizontal justification you would like for your text. You
 *      can use one of the standard justification constants from
 *      TextEdit.h.
 *    
 *    inContext:
 *      The CGContextRef into which you would like to draw the text. On
 *      Mac OS X, all text drawing happens in CGContextRefs; if you
 *      pass NULL, a transient CGContextRef will be allocated and
 *      deallocated for use within the single API call. Relying on the
 *      system behavior if transiently creating CGContextRefs may
 *      result in performance problems. On Mac OS 9, the CGContextRef
 *      parameter is ignored.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DrawThemeTextBox(
  CFStringRef      inString,
  ThemeFontID      inFontID,
  ThemeDrawState   inState,
  Boolean          inWrapToWidth,
  const Rect *     inBoundingBox,
  SInt16           inJust,
  CGContextRef     inContext)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  TruncateThemeText()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetTextDimensions or
 *    HIThemeDrawTextBox. There is currently no replacement API that
 *    actually alters the string.
 *  
 *  Summary:
 *    Truncates text to fit within the width you specify.
 *  
 *  Discussion:
 *    TruncateThemeText alters a unicode string to fit within a width
 *    that you specify. It is unicode savvy (although only partially so
 *    under CarbonLib), and makes its calculations (and any subsequent
 *    string alteration) based on the font and state you specify. If
 *    the string needs to be truncated, it will be reduced to the
 *    maximum number of characters which (with the addition of an
 *    ellipsis character) fits within the specified width.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inString:
 *      A CFMutableStringRef containing the unicode characters you wish
 *      to truncate. On output, inString may have been altered to fit
 *      within the specified width. You MUST NOT pass in a CFStringRef
 *      that was allocated with any of the "NoCopy" CFString creation
 *      APIs (see note in DrawThemeTextBox above).
 *    
 *    inFontID:
 *      The ThemeFontID to use for text measurements. See the
 *      discussion of ThemeFontIDs elsewhere in this header.
 *    
 *    inState:
 *      The ThemeDrawState which matches the state you will ultimately
 *      render the string width. This may affect text measurement
 *      during truncation, so you should be sure the value you pass to
 *      TruncateThemeText matches the value you will eventually use for
 *      drawing.
 *    
 *    inPixelWidthLimit:
 *      The maximum width (in pixels) that the resulting truncated
 *      string may have.
 *    
 *    inTruncWhere:
 *      A TruncCode indicating where you would like truncation to occur.
 *    
 *    outTruncated:
 *      On output, this Boolean value indicates whether the string was
 *      truncated. True means the string was truncated. False means the
 *      string was not (and did not need to be) truncated.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TruncateThemeText(
  CFMutableStringRef   inString,
  ThemeFontID          inFontID,
  ThemeDrawState       inState,
  SInt16               inPixelWidthLimit,
  TruncCode            inTruncWhere,
  Boolean *            outTruncated)            /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTextDimensions()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetTextDimensions.
 *  
 *  Summary:
 *    Tells you the height, width, and baseline for a string.
 *  
 *  Discussion:
 *    GetThemeTextDimensions measures the given string using a font and
 *    state you specify. It always reports the actual height and
 *    baseline. It sometimes reports the actual width (see below). It
 *    can measure a string that wraps. It is unicode savvy (although
 *    only partially so under CarbonLib).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inString:
 *      A CFStringRef containing the unicode characters you wish to
 *      measure. You MUST NOT pass in a CFStringRef that was allocated
 *      with any of the "NoCopy" CFString creation APIs (see note in
 *      DrawThemeTextBox above).
 *    
 *    inFontID:
 *      The ThemeFontID describing the font you'd like to measure the
 *      text with. See the discussion of ThemeFontIDs elsewhere in this
 *      header.
 *    
 *    inState:
 *      The ThemeDrawState which matches the state you will ultimately
 *      render the string width. This may affect text measurement, so
 *      you should be sure the value you pass to TruncateThemeText
 *      matches the value you will eventually use for drawing.
 *    
 *    inWrapToWidth:
 *      A Boolean indicating whether you want the measurements based on
 *      wrapping the text to a specific width. If you pass true, you
 *      must specify the desired width in ioBounds->h.
 *    
 *    ioBounds:
 *      On output, ioBounds->v contains the height of the text. If you
 *      pass false to inWrapToWidth, ioBounds->h will contain the width
 *      of the text on output. If you pass true to inWrapToWidth,
 *      ioBounds->h must (on input) contain the desired width for
 *      wrapping; on output, ioBounds->h contains the same value you
 *      specified on input.
 *    
 *    outBaseline:
 *      On output, outBaseline contains the offset (in Quickdraw space)
 *      from the bottom edge of the last line of text to the baseline
 *      of the first line of text. outBaseline will generally be a
 *      negative value. On Mac OS X 10.2 and later, you may pass NULL
 *      if you don't want this information.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetThemeTextDimensions(
  CFStringRef      inString,
  ThemeFontID      inFontID,
  ThemeDrawState   inState,
  Boolean          inWrapToWidth,
  Point *          ioBounds,
  SInt16 *         outBaseline)         /* can be NULL */     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTextShadowOutset()
 *  
 *  Summary:
 *    Tells you the amount of space taken up by the shadow for a given
 *    font/state combination.
 *  
 *  Discussion:
 *    GetThemeTextShadowOutset passes back the maximum amount of space
 *    the shadow will take up for text drawn in the specified font and
 *    state. While GetThemeTextDimensions tells you how much space is
 *    taken up by the character glyphs themselves, it does not
 *    incorporate the font/state shadow into its calculations. If you
 *    need to know how much total space including the shadow will be
 *    taken up, call GetThemeTextDimensions followed by
 *    GetThemeTextShadowOutset.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inFontID:
 *      The ThemeFontID describing the font you'd like the shadow
 *      characteristics of. Font and state both determine the amount of
 *      shadow that will be used on rendered text. See the discussion
 *      of ThemeFontIDs elsewhere in this header.
 *    
 *    inState:
 *      The ThemeDrawState which matches the state you'd like the
 *      shadow characteristics of. Font and state both determine the
 *      amount of shadow that will be used on rendered text.
 *    
 *    outOutset:
 *      On output, outOutset contains the amount of space the shadow
 *      will take up beyond each edge of the text bounding rectangle
 *      returned by GetThemeTextDimensions. The fields of outOutset
 *      will either be positive values or zero.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetThemeTextShadowOutset(
  ThemeFontID      inFontID,
  ThemeDrawState   inState,
  Rect *           outOutset)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ TRACKS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  DrawThemeTrack()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawTrack.
 *  
 *  Summary:
 *    Draws a track theme primitive, such as a slider or scrollbar.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeTrack(
  const ThemeTrackDrawInfo *  drawInfo,
  RgnHandle                   rgnGhost,        /* can be NULL */
  ThemeEraseUPP               eraseProc,       /* can be NULL */
  URefCon                     eraseData)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  HitTestThemeTrack()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeHitTestTrack.
 *  
 *  Summary:
 *    Determines the part of a track that is is under a specified point.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern Boolean 
HitTestThemeTrack(
  const ThemeTrackDrawInfo *  drawInfo,
  Point                       mousePoint,
  AppearancePartCode *        partHit)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTrackBounds()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetTrackBounds.
 *  
 *  Summary:
 *    Retrieves the bounds of a track theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeTrackBounds(
  const ThemeTrackDrawInfo *  drawInfo,
  Rect *                      bounds)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTrackThumbRgn()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetTrackThumbShape.
 *  
 *  Summary:
 *    Constructs a region that covers the area of a track thumb.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeTrackThumbRgn(
  const ThemeTrackDrawInfo *  drawInfo,
  RgnHandle                   thumbRgn)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTrackDragRect()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetTrackDragRect.
 *  
 *  Summary:
 *    Retrieves the bounds of a track dragging area.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeTrackDragRect(
  const ThemeTrackDrawInfo *  drawInfo,
  Rect *                      dragRect)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeTrackTickMarks()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawTrackTickMarks.
 *  
 *  Summary:
 *    Draws the tick marks for a track theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeTrackTickMarks(
  const ThemeTrackDrawInfo *  drawInfo,
  ItemCount                   numTicks,
  ThemeEraseUPP               eraseProc,       /* can be NULL */
  URefCon                     eraseData)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTrackThumbPositionFromOffset()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use
 *    HIThemeGetTrackThumbPositionFromOffset.
 *  
 *  Summary:
 *    Retrieves a track's thumb position based on its offset in the
 *    track bounds.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeTrackThumbPositionFromOffset(
  const ThemeTrackDrawInfo *  drawInfo,
  Point                       thumbOffset,
  SInt32 *                    relativePosition)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTrackThumbPositionFromRegion()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use
 *    HIThemeGetTrackThumbPositionFromBounds.
 *  
 *  Summary:
 *    Retrieves a track's thumb position based on a region covering the
 *    thumb.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeTrackThumbPositionFromRegion(
  const ThemeTrackDrawInfo *  drawInfo,
  RgnHandle                   thumbRgn,
  SInt32 *                    relativePosition)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeTrackLiveValue()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetTrackLiveValue.
 *  
 *  Summary:
 *    Retrieves the value that a track would have based on a thumb
 *    position.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeTrackLiveValue(
  const ThemeTrackDrawInfo *  drawInfo,
  SInt32                      relativePosition,
  SInt32 *                    value)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ SCROLLBAR ARROWS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  DrawThemeScrollBarArrows()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawTrack, which will draw
 *    the entire scrollbar, including both the track and arrows.
 *  
 *  Summary:
 *    Draws the arrows at each or both ends of a scrollbar theme
 *    primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeScrollBarArrows(
  const Rect *            bounds,
  ThemeTrackEnableState   enableState,
  ThemeTrackPressState    pressState,
  Boolean                 isHoriz,
  Rect *                  trackBounds)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeScrollBarTrackRect()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetScrollBarTrackRect.
 *  
 *  Summary:
 *    Retrieves the bounds of a scrollbar track.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeScrollBarTrackRect(
  const Rect *            bounds,
  ThemeTrackEnableState   enableState,
  ThemeTrackPressState    pressState,
  Boolean                 isHoriz,
  Rect *                  trackBounds)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  HitTestThemeScrollBarArrows()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeHitTestScrollBarArrows.
 *  
 *  Summary:
 *    Determines which scrollbar arrow is under a specified point.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern Boolean 
HitTestThemeScrollBarArrows(
  const Rect *            scrollBarBounds,
  ThemeTrackEnableState   enableState,
  ThemeTrackPressState    pressState,
  Boolean                 isHoriz,
  Point                   ptHit,
  Rect *                  trackBounds,
  AppearancePartCode *    partcode)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ WINDOWS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  GetThemeWindowRegion()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetWindowShape.
 *  
 *  Summary:
 *    Constructs a region that covers a specified part of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeWindowRegion(
  ThemeWindowType             flavor,
  const Rect *                contRect,
  ThemeDrawState              state,
  const ThemeWindowMetrics *  metrics,
  ThemeWindowAttributes       attributes,
  AppearanceRegionCode        winRegion,
  RgnHandle                   rgn)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeWindowFrame()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawWindowFrame.
 *  
 *  Summary:
 *    Draws a window frame theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeWindowFrame(
  ThemeWindowType             flavor,
  const Rect *                contRect,
  ThemeDrawState              state,
  const ThemeWindowMetrics *  metrics,
  ThemeWindowAttributes       attributes,
  WindowTitleDrawingUPP       titleProc,        /* can be NULL */
  URefCon                     titleData)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeTitleBarWidget()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawTitleBarWidget.
 *  
 *  Summary:
 *    Draws the requested theme title bar widget.
 *  
 *  Discussion:
 *    DrawThemeTitleBarWidget renders the requested theme title bar
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
 *    flavor:
 *      A valid ThemeWindowtype describing the type of theme window for
 *      which you would like to draw a widget.
 *    
 *    contRect:
 *      A rectangle describing the window's content area.  The widget
 *      is drawn relative to the content rectangle of the window, so
 *      this parameter does not describe the actual widget bounds, it
 *      describes the window's content rectangle.
 *    
 *    state:
 *      A valid ThemeDrawState which describes the state of the window
 *      for which the widget is to be drawn.
 *    
 *    metrics:
 *      A pointer to a set of valid ThemeWindowMetrics.  At this time,
 *      none of the fields of the metrics are pertinent to the widgets,
 *      so the only important field is the metricSize field to mark the
 *      structure as valid.
 *    
 *    attributes:
 *      A valid ThemeWindowAttributes set which describes the window
 *      for which the widget is to be drawn.
 *    
 *    widget:
 *      A valid ThemeTitleBarWidget set which describes which widget to
 *      draw.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeTitleBarWidget(
  ThemeWindowType             flavor,
  const Rect *                contRect,
  ThemeDrawState              state,
  const ThemeWindowMetrics *  metrics,
  ThemeWindowAttributes       attributes,
  ThemeTitleBarWidget         widget)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeWindowRegionHit()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetWindowRegionHit.
 *  
 *  Summary:
 *    Determines the window part that is under a specified point.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern Boolean 
GetThemeWindowRegionHit(
  ThemeWindowType             flavor,
  const Rect *                inContRect,
  ThemeDrawState              state,
  const ThemeWindowMetrics *  metrics,
  ThemeWindowAttributes       inAttributes,
  Point                       inPoint,
  AppearanceRegionCode *      outRegionHit)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeScrollBarDelimiters()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.3 and later, use HIThemeDrawScrollBarDelimiters.
 *  
 *  Summary:
 *    Draws the scrollbar delimeters in a window frame.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeScrollBarDelimiters(
  ThemeWindowType         flavor,
  const Rect *            inContRect,
  ThemeDrawState          state,
  ThemeWindowAttributes   attributes)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ BUTTONS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  DrawThemeButton()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawButton.
 *  
 *  Summary:
 *    Draws a button theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeButton(
  const Rect *                 inBounds,
  ThemeButtonKind              inKind,
  const ThemeButtonDrawInfo *  inNewInfo,
  const ThemeButtonDrawInfo *  inPrevInfo,        /* can be NULL */
  ThemeEraseUPP                inEraseProc,       /* can be NULL */
  ThemeButtonDrawUPP           inLabelProc,       /* can be NULL */
  URefCon                      inUserData)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeButtonRegion()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetButtonShape.
 *  
 *  Summary:
 *    Constructs a region that covers the area of a button.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeButtonRegion(
  const Rect *                 inBounds,
  ThemeButtonKind              inKind,
  const ThemeButtonDrawInfo *  inNewInfo,
  RgnHandle                    outRegion)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeButtonContentBounds()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetButtonContentBounds.
 *  
 *  Summary:
 *    Retrieves the bounds of a button's content.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeButtonContentBounds(
  const Rect *                 inBounds,
  ThemeButtonKind              inKind,
  const ThemeButtonDrawInfo *  inDrawInfo,
  Rect *                       outBounds)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeButtonBackgroundBounds()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetButtonBackgroundBounds.
 *  
 *  Summary:
 *    Retrieves the bounds of a button's background.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeButtonBackgroundBounds(
  const Rect *                 inBounds,
  ThemeButtonKind              inKind,
  const ThemeButtonDrawInfo *  inDrawInfo,
  Rect *                       outBounds)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ INTERFACE SOUNDS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*                                                                                                  */
/*  X ALERT: Please note that the sound APIs do not work on Mac OS X at present.                    */
/*
 *  PlayThemeSound()
 *  
 *  Summary:
 *    Plays a theme-compliant sound. Note that this API does not play
 *    sound on any version of Mac OS X.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    kind:
 *      The kind of sound to play.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
PlayThemeSound(ThemeSoundKind kind)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  BeginThemeDragSound()
 *  
 *  Summary:
 *    Begins playing a theme-compliant sound to indicate that a drag is
 *    occuring. Note that this API does not play sound on any version
 *    of Mac OS X.
 *  
 *  Discussion:
 *    You should call EndThemeDragSound when the drag is complete.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    kind:
 *      The kind of drag sound to play.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
BeginThemeDragSound(ThemeDragSoundKind kind)                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  EndThemeDragSound()
 *  
 *  Summary:
 *    Stops playing a theme-compliant sound to indicate that a drag is
 *    occuring.
 *  
 *  Discussion:
 *    This API should only be called after BeginThemeDragSound has been
 *    called.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
EndThemeDragSound(void)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ PRIMITIVES ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  DrawThemeTickMark()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawTickMark.
 *  
 *  Summary:
 *    Draws a tick mark theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeTickMark(
  const Rect *     bounds,
  ThemeDrawState   state)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeChasingArrows()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawChasingArrows.
 *  
 *  Summary:
 *    Draws a chasing arrows theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeChasingArrows(
  const Rect *     bounds,
  UInt32           theIndex,
  ThemeDrawState   state,
  ThemeEraseUPP    eraseProc,       /* can be NULL */
  URefCon          eraseData)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemePopupArrow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawPopupArrow.
 *  
 *  Summary:
 *    Draws a popup arrow theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemePopupArrow(
  const Rect *            bounds,
  ThemeArrowOrientation   orientation,
  ThemePopupArrowSize     size,
  ThemeDrawState          state,
  ThemeEraseUPP           eraseProc,         /* can be NULL */
  URefCon                 eraseData)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeStandaloneGrowBox()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawGrowBox.
 *  
 *  Summary:
 *    Draws a grow box theme primitive that is designed to be detached
 *    from the window frame.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeStandaloneGrowBox(
  Point                origin,
  ThemeGrowDirection   growDirection,
  Boolean              isSmall,
  ThemeDrawState       state)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawThemeStandaloneNoGrowBox()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeDrawGrowBox.
 *  
 *  Summary:
 *    Draws a blank area that can be used to fill in the corner of two
 *    scrollbars, when no grow box is required.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DrawThemeStandaloneNoGrowBox(
  Point                origin,
  ThemeGrowDirection   growDirection,
  Boolean              isSmall,
  ThemeDrawState       state)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeStandaloneGrowBoxBounds()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeGetGrowBoxBounds.
 *  
 *  Summary:
 *    Retrieves the bounds of a standalone grow box theme primitive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeStandaloneGrowBoxBounds(
  Point                origin,
  ThemeGrowDirection   growDirection,
  Boolean              isSmall,
  Rect *               bounds)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ DRAWING STATE ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* The following routines help you save and restore the drawing state in a theme-savvy manner. With */
/* these weapons in your arsenal, there is no grafport you cannot tame. Use ThemeGetDrawingState to */
/* get the current drawing settings for the current port. It will return an opaque object for you   */
/* to pass into SetThemeDrawingState later on. When you are finished with the state, call the       */
/* DisposeThemeDrawingState routine. You can alternatively pass true into the inDisposeNow          */
/* parameter of the SetThemeDrawingState routine.  You can use this routine to copy the drawing     */
/* state from one port to another as well.                                                          */
/*                                                                                                  */
/* As of this writing (Mac OS 9.1 and Mac OS X), Get/SetThemeDrawingState will save and restore     */
/* this data in the port:                                                                           */
/*                                                                                                  */
/*      pen size                                                                                    */
/*      pen location                                                                                */
/*      pen mode                                                                                    */
/*      pen Pattern and PixPat                                                                      */
/*      background Pattern and PixPat                                                               */
/*      RGB foreground and background colors                                                        */
/*      text mode                                                                                   */
/*      pattern origin                                                                              */
/*                                                                                                  */
/* Get/SetThemeDrawingState may save and restore additional port state in the future, but you can   */
/* rely on them to always save at least this port state.                                            */
/*
 *  NormalizeThemeDrawingState()
 *  
 *  Summary:
 *    Normalizes the drawing state of the current GrafPort.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
NormalizeThemeDrawingState(void)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetThemeDrawingState()
 *  
 *  Summary:
 *    Retrieves the drawing state of the current port.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outState:
 *      On exit, contains the saved drawing state. This state must be
 *      released by calling either DisposeThemeDrawingState or
 *      SetThemeDrawingState with inDisposeNow equal to true.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeDrawingState(ThemeDrawingState * outState)            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetThemeDrawingState()
 *  
 *  Summary:
 *    Restores saved drawing state of the current port.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inState:
 *      The state to restore.
 *    
 *    inDisposeNow:
 *      Whether the drawing state should be disposed by this API.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
SetThemeDrawingState(
  ThemeDrawingState   inState,
  Boolean             inDisposeNow)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DisposeThemeDrawingState()
 *  
 *  Summary:
 *    Releases the memory used by a saved drawing state object.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inState:
 *      The state to release.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
DisposeThemeDrawingState(ThemeDrawingState inState)           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ MISCELLANEOUS ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  ApplyThemeBackground()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.3 and later, use HIThemeApplyBackground.
 *  
 *  Summary:
 *    Sets up the background color and/or pattern in the current
 *    GrafPort so that an embedded control may erase its content bounds
 *    appropriately for its containing control.
 *  
 *  Discussion:
 *    This API is normally called by embedding controls in response to
 *    a kEventControlApplyBackground Carbon event. It is only needed
 *    when an embedding control is contained in a non-composited
 *    window. The standard controls call this API to ensure a correct
 *    background for the current theme prior to erasing their content
 *    bounds (in a non-composited window). You should pass in the same
 *    rectangle you would use if you were calling the drawing primitive
 *    that corresponds to the ThemeBackgroundKind that you provide to
 *    this API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inKind:
 *      The type of theme background for which to set the current
 *      GrafPort's background color and/or pattern.
 *    
 *    bounds:
 *      The bounds of the area that would be covered by the theme
 *      background. This parameter is used to determine the appropriate
 *      pattern offset, if necessary.
 *    
 *    inState:
 *      The theme drawing state that should be used by the theme
 *      background.
 *    
 *    inDepth:
 *      The depth of the GDevice containing the current GrafPort. On
 *      Mac OS X, this parameter is ignored, and you should always pass
 *      32.
 *    
 *    inColorDev:
 *      Whether the GDevice containing the current GrafPort is color or
 *      black and white. On Mac OS X, this parameter is ignored, and
 *      you should always pass true.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
ApplyThemeBackground(
  ThemeBackgroundKind   inKind,
  const Rect *          bounds,
  ThemeDrawState        inState,
  SInt16                inDepth,
  Boolean               inColorDev)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IsValidAppearanceFileType()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This API always returns false on Mac OS X.
 *  
 *  Summary:
 *    Indicates whether a specified file type is used for theme files.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern Boolean 
IsValidAppearanceFileType(OSType fileType)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetThemeBrushAsColor()
 *  
 *  Summary:
 *    Retrieves the color that most closely matches a theme brush.
 *  
 *  Discussion:
 *    Note that the color may not exactly reproduce the theme brush,
 *    since many brushes are drawn using a pattern rather than a solid
 *    color.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBrush:
 *      The brush whose color to retrieve.
 *    
 *    inDepth:
 *      The depth of the GDevice containing the current GrafPort. On
 *      Mac OS X, this parameter is ignored, and you should always pass
 *      32.
 *    
 *    inColorDev:
 *      Whether the GDevice containing the current GrafPort is color or
 *      black and white. On Mac OS X, this parameter is ignored, and
 *      you should always pass true.
 *    
 *    outColor:
 *      On exit, contains the color that corresponds to the specified
 *      brush.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeBrushAsColor(
  ThemeBrush   inBrush,
  SInt16       inDepth,
  Boolean      inColorDev,
  RGBColor *   outColor)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetThemeTextColor()
 *  
 *  Summary:
 *    Retrieves the color that is used to draw a specified type of text.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inColor:
 *      The text color to retrieve.
 *    
 *    inDepth:
 *      The depth of the GDevice containing the current GrafPort. On
 *      Mac OS X, this parameter is ignored, and you should always pass
 *      32.
 *    
 *    inColorDev:
 *      Whether the GDevice containing the current GrafPort is color or
 *      black and white. On Mac OS X, this parameter is ignored, and
 *      you should always pass true.
 *    
 *    outColor:
 *      On exit, contains the specified text color.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
GetThemeTextColor(
  ThemeTextColor   inColor,
  SInt16           inDepth,
  Boolean          inColorDev,
  RGBColor *       outColor)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*--------------------------------------- MOVED ROUTINES -------------------------------------------*/
/* The following routines have been moved to HITheme.h                                              */
/* GetThemeMenuSeparatorHeight                                                                      */
/* GetThemeMenuItemExtra                                                                            */
/* GetThemeMenuTitleExtra                                                                           */
/* GetThemeMetric                                                                                   */
/* CopyThemeIdentifier                                                                              */
/*--------------------------------------------------------------------------------------------------*/

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ BEGIN CARBON ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Obsolete symbolic names                                                                          */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Obsolete error codes - use the new ones, s'il vous plait / kudasai */
#endif  /* !__LP64__ */

enum {
  appearanceBadBrushIndexErr    = themeInvalidBrushErr, /* pattern index invalid */
  appearanceProcessRegisteredErr = themeProcessRegisteredErr,
  appearanceProcessNotRegisteredErr = themeProcessNotRegisteredErr,
  appearanceBadTextColorIndexErr = themeBadTextColorErr,
  appearanceThemeHasNoAccents   = themeHasNoAccentsErr,
  appearanceBadCursorIndexErr   = themeBadCursorIndexErr
};

enum {
  kThemeActiveDialogBackgroundBrush = kThemeBrushDialogBackgroundActive,
  kThemeInactiveDialogBackgroundBrush = kThemeBrushDialogBackgroundInactive,
  kThemeActiveAlertBackgroundBrush = kThemeBrushAlertBackgroundActive,
  kThemeInactiveAlertBackgroundBrush = kThemeBrushAlertBackgroundInactive,
  kThemeActiveModelessDialogBackgroundBrush = kThemeBrushModelessDialogBackgroundActive,
  kThemeInactiveModelessDialogBackgroundBrush = kThemeBrushModelessDialogBackgroundInactive,
  kThemeActiveUtilityWindowBackgroundBrush = kThemeBrushUtilityWindowBackgroundActive,
  kThemeInactiveUtilityWindowBackgroundBrush = kThemeBrushUtilityWindowBackgroundInactive,
  kThemeListViewSortColumnBackgroundBrush = kThemeBrushListViewSortColumnBackground,
  kThemeListViewBackgroundBrush = kThemeBrushListViewBackground,
  kThemeIconLabelBackgroundBrush = kThemeBrushIconLabelBackground,
  kThemeListViewSeparatorBrush  = kThemeBrushListViewSeparator,
  kThemeChasingArrowsBrush      = kThemeBrushChasingArrows,
  kThemeDragHiliteBrush         = kThemeBrushDragHilite,
  kThemeDocumentWindowBackgroundBrush = kThemeBrushDocumentWindowBackground,
  kThemeFinderWindowBackgroundBrush = kThemeBrushFinderWindowBackground
};

enum {
  kThemeActiveScrollBarDelimiterBrush = kThemeBrushScrollBarDelimiterActive,
  kThemeInactiveScrollBarDelimiterBrush = kThemeBrushScrollBarDelimiterInactive,
  kThemeFocusHighlightBrush     = kThemeBrushFocusHighlight,
  kThemeActivePopupArrowBrush   = kThemeBrushPopupArrowActive,
  kThemePressedPopupArrowBrush  = kThemeBrushPopupArrowPressed,
  kThemeInactivePopupArrowBrush = kThemeBrushPopupArrowInactive,
  kThemeAppleGuideCoachmarkBrush = kThemeBrushAppleGuideCoachmark
};

enum {
  kThemeActiveDialogTextColor   = kThemeTextColorDialogActive,
  kThemeInactiveDialogTextColor = kThemeTextColorDialogInactive,
  kThemeActiveAlertTextColor    = kThemeTextColorAlertActive,
  kThemeInactiveAlertTextColor  = kThemeTextColorAlertInactive,
  kThemeActiveModelessDialogTextColor = kThemeTextColorModelessDialogActive,
  kThemeInactiveModelessDialogTextColor = kThemeTextColorModelessDialogInactive,
  kThemeActiveWindowHeaderTextColor = kThemeTextColorWindowHeaderActive,
  kThemeInactiveWindowHeaderTextColor = kThemeTextColorWindowHeaderInactive,
  kThemeActivePlacardTextColor  = kThemeTextColorPlacardActive,
  kThemeInactivePlacardTextColor = kThemeTextColorPlacardInactive,
  kThemePressedPlacardTextColor = kThemeTextColorPlacardPressed,
  kThemeActivePushButtonTextColor = kThemeTextColorPushButtonActive,
  kThemeInactivePushButtonTextColor = kThemeTextColorPushButtonInactive,
  kThemePressedPushButtonTextColor = kThemeTextColorPushButtonPressed,
  kThemeActiveBevelButtonTextColor = kThemeTextColorBevelButtonActive,
  kThemeInactiveBevelButtonTextColor = kThemeTextColorBevelButtonInactive,
  kThemePressedBevelButtonTextColor = kThemeTextColorBevelButtonPressed,
  kThemeActivePopupButtonTextColor = kThemeTextColorPopupButtonActive,
  kThemeInactivePopupButtonTextColor = kThemeTextColorPopupButtonInactive,
  kThemePressedPopupButtonTextColor = kThemeTextColorPopupButtonPressed,
  kThemeIconLabelTextColor      = kThemeTextColorIconLabel,
  kThemeListViewTextColor       = kThemeTextColorListView
};

enum {
  kThemeActiveDocumentWindowTitleTextColor = kThemeTextColorDocumentWindowTitleActive,
  kThemeInactiveDocumentWindowTitleTextColor = kThemeTextColorDocumentWindowTitleInactive,
  kThemeActiveMovableModalWindowTitleTextColor = kThemeTextColorMovableModalWindowTitleActive,
  kThemeInactiveMovableModalWindowTitleTextColor = kThemeTextColorMovableModalWindowTitleInactive,
  kThemeActiveUtilityWindowTitleTextColor = kThemeTextColorUtilityWindowTitleActive,
  kThemeInactiveUtilityWindowTitleTextColor = kThemeTextColorUtilityWindowTitleInactive,
  kThemeActivePopupWindowTitleColor = kThemeTextColorPopupWindowTitleActive,
  kThemeInactivePopupWindowTitleColor = kThemeTextColorPopupWindowTitleInactive,
  kThemeActiveRootMenuTextColor = kThemeTextColorRootMenuActive,
  kThemeSelectedRootMenuTextColor = kThemeTextColorRootMenuSelected,
  kThemeDisabledRootMenuTextColor = kThemeTextColorRootMenuDisabled,
  kThemeActiveMenuItemTextColor = kThemeTextColorMenuItemActive,
  kThemeSelectedMenuItemTextColor = kThemeTextColorMenuItemSelected,
  kThemeDisabledMenuItemTextColor = kThemeTextColorMenuItemDisabled,
  kThemeActivePopupLabelTextColor = kThemeTextColorPopupLabelActive,
  kThemeInactivePopupLabelTextColor = kThemeTextColorPopupLabelInactive
};

enum {
  kAEThemeSwitch                = kAEAppearanceChanged /* Event ID's: Theme Switched */
};

enum {
  kThemeNoAdornment             = kThemeAdornmentNone,
  kThemeDefaultAdornment        = kThemeAdornmentDefault,
  kThemeFocusAdornment          = kThemeAdornmentFocus,
  kThemeRightToLeftAdornment    = kThemeAdornmentRightToLeft,
  kThemeDrawIndicatorOnly       = kThemeAdornmentDrawIndicatorOnly
};

enum {
  kThemeBrushPassiveAreaFill    = kThemeBrushStaticAreaFill
};

enum {
  kThemeScrollBar               = kThemeMediumScrollBar,
  kThemeSlider                  = kThemeMediumSlider,
  kThemeProgressBar             = kThemeMediumProgressBar,
  kThemeIndeterminateBar        = kThemeMediumIndeterminateBar
};


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __APPEARANCE__ */

