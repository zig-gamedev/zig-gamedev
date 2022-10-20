/*
     File:       HIToolbox/HIDataBrowser.h
 
     Contains:   API and type definitions related to Data Browser.
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIDATABROWSER__
#define __HIDATABROWSER__

#ifndef __HIVIEW__
#include <HIToolbox/HIView.h>
#endif

#ifndef __MACHELP__
#include <HIToolbox/MacHelp.h>
#endif


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*    ¥ DATA BROWSER                                                                    */
/*     (CDEF 29)                                                                        */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  This control implements a user interface component for browsing (optionally)        */
/*  hiearchical data structures. The browser supports multiple presentation styles      */
/*  including, but not limited to:                                                      */
/*                                                                                      */
/*      kDataBrowserListView   - items and item properties in                           */
/*                               multi-column (optionally outline) format               */
/*      kDataBrowserColumnView - in-place browsing using fixed navigation columns       */
/*                                                                                      */
/*  The browser manages all view styles through a single high-level interface.          */
/*  The high-level interface makes the following assumptions:                           */
/*                                                                                      */
/*      - Items have unique 32-bit identifiers (0 is reserved)                          */
/*                                                                                      */
/*      - Items have two kinds of named and typed properties:                           */
/*           - Predefined attribute properties ( < 1024 )                               */
/*             (including some display properties)                                      */
/*           - Client-defined display properties ( >= 1024 )                            */
/*                                                                                      */
/*      - Some items are containers of other items                                      */
/*      - Items may be sorted by any property                                           */
/*                                                                                      */
/*  Because a browser doesn't know all details about the type of objects it manages,    */
/*  some implementation responsibility is best handled by its client. The client must   */
/*  provide a set of callback routines which define the item hierarchy and help to      */
/*  populate the browser with items. The client may also provide callbacks for handling */
/*  custom data types and doing low-level event management.                             */
/*                                                                                      */
/*  The API is subdivided into a "universal" set of routines that applies to all view   */
/*  styles, and a set of routines unique to each view style. kDataBrowserListView and   */
/*  kDataBrowserColumnView share an (internal) TableView abstract base class. The       */
/*  TableView formatting options and API applies to both of these view styles.          */
/*                                                                                      */
/*  NOTE: This control is only available with CarbonLib 1.1.                            */
/*                                                                                      */
/*  NOTE: This control must be created with the CreateDataBrowserControl API in         */
/*        CarbonLib 1.1 through 1.4. In Mac OS X and CarbonLib 1.5 and later, you       */
/*        may use the control's procID (29) to create the control with NewControl       */
/*        or with a 'CNTL' resource.                                                    */
/* The HIObject class ID for the HIDataBrowser class. */

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

#define kHIDataBrowserClassID           CFSTR("com.apple.HIDataBrowser")
/* Control Kind Tag */
enum {
  kControlKindDataBrowser       = 'datb'
};

/* Error Codes */
enum {
  errDataBrowserNotConfigured   = -4970,
  errDataBrowserItemNotFound    = -4971,
  errDataBrowserItemNotAdded    = -4975,
  errDataBrowserPropertyNotFound = -4972,
  errDataBrowserInvalidPropertyPart = -4973,
  errDataBrowserInvalidPropertyData = -4974,
  errDataBrowserPropertyNotSupported = -4979 /* Return from DataBrowserGetSetItemDataProc */
};

enum {
                                        /* Generic Control Tags */
  kControlDataBrowserIncludesFrameAndFocusTag = 'brdr', /* Boolean */
  kControlDataBrowserKeyFilterTag = kControlKeyFilterTag,
  kControlDataBrowserEditTextKeyFilterTag = kControlDataBrowserKeyFilterTag,
  kControlDataBrowserEditTextValidationProcTag = 'vali' /* ControlEditTextValidationUPP. Called when a key filter can't be: after cut, paste, etc.*/
};

/* Data Browser View Styles */
typedef OSType                          DataBrowserViewStyle;
enum {
  kDataBrowserNoView            = 0x3F3F3F3F, /* Error State */
  kDataBrowserListView          = 'lstv',
  kDataBrowserColumnView        = 'clmv'
};

/* Selection Flags */
typedef UInt32 DataBrowserSelectionFlags;
enum {
  kDataBrowserDragSelect        = 1 << 0, /* Å ListMgr lNoRect */
  kDataBrowserSelectOnlyOne     = 1 << 1, /* Å ListMgr lOnlyOne */
  kDataBrowserResetSelection    = 1 << 2, /* Å ListMgr lNoExtend */
  kDataBrowserCmdTogglesSelection = 1 << 3, /* Å ListMgr lUseSense */
  kDataBrowserNoDisjointSelection = 1 << 4, /* Å ListMgr lNoDisjoint */
  kDataBrowserAlwaysExtendSelection = 1 << 5, /* Å ListMgr lExtendDrag */
  kDataBrowserNeverEmptySelectionSet = 1 << 6 /* Å ListMgr lNoNilHilite */
};

/* Data Browser Sorting */
typedef UInt16 DataBrowserSortOrder;
enum {
  kDataBrowserOrderUndefined    = 0,    /* Not currently supported */
  kDataBrowserOrderIncreasing   = 1,
  kDataBrowserOrderDecreasing   = 2
};

/* Data Browser Item Management */
typedef unsigned long                   DataBrowserItemID;
enum {
  kDataBrowserNoItem            = 0     /* Reserved DataBrowserItemID */
};

typedef UInt32 DataBrowserItemState;
enum {
  kDataBrowserItemNoState       = 0,
  kDataBrowserItemAnyState      = (unsigned int)(-1),
  kDataBrowserItemIsSelected    = 1 << 0,
  kDataBrowserContainerIsOpen   = 1 << 1,
  kDataBrowserItemIsDragTarget  = 1 << 2 /* During a drag operation */
};

/* Options for use with RevealDataBrowserItem */
typedef UInt8 DataBrowserRevealOptions;
enum {
  kDataBrowserRevealOnly        = 0,
  kDataBrowserRevealAndCenterInView = 1 << 0,
  kDataBrowserRevealWithoutSelecting = 1 << 1
};

/* Set operations for use with SetDataBrowserSelectedItems */
typedef UInt32 DataBrowserSetOption;
enum {
  kDataBrowserItemsAdd          = 0,    /* add specified items to existing set */
  kDataBrowserItemsAssign       = 1,    /* assign destination set to specified items */
  kDataBrowserItemsToggle       = 2,    /* toggle membership state of specified items */
  kDataBrowserItemsRemove       = 3     /* remove specified items from existing set */
};

/* Commands for use with MoveDataBrowserSelectionAnchor */
typedef UInt32 DataBrowserSelectionAnchorDirection;
enum {
  kDataBrowserSelectionAnchorUp = 0,
  kDataBrowserSelectionAnchorDown = 1,
  kDataBrowserSelectionAnchorLeft = 2,
  kDataBrowserSelectionAnchorRight = 3
};

/* Edit menu command IDs for use with Enable/ExecuteDataBrowserEditCommand */
typedef UInt32 DataBrowserEditCommand;
enum {
  kDataBrowserEditMsgUndo       = kHICommandUndo,
  kDataBrowserEditMsgRedo       = kHICommandRedo,
  kDataBrowserEditMsgCut        = kHICommandCut,
  kDataBrowserEditMsgCopy       = kHICommandCopy,
  kDataBrowserEditMsgPaste      = kHICommandPaste,
  kDataBrowserEditMsgClear      = kHICommandClear,
  kDataBrowserEditMsgSelectAll  = kHICommandSelectAll
};

/* Notifications used in DataBrowserItemNotificationProcPtr */
typedef UInt32 DataBrowserItemNotification;
enum {
  kDataBrowserItemAdded         = 1,    /* The specified item has been added to the browser */
  kDataBrowserItemRemoved       = 2,    /* The specified item has been removed from the browser */
  kDataBrowserEditStarted       = 3,    /* Starting an EditText session for specified item */
  kDataBrowserEditStopped       = 4,    /* Stopping an EditText session for specified item */
  kDataBrowserItemSelected      = 5,    /* Item has just been added to the selection set */
  kDataBrowserItemDeselected    = 6,    /* Item has just been removed from the selection set */
  kDataBrowserItemDoubleClicked = 7,
  kDataBrowserContainerOpened   = 8,    /* Container is open */
  kDataBrowserContainerClosing  = 9,    /* Container is about to close (and will real soon now, y'all) */
  kDataBrowserContainerClosed   = 10,   /* Container is closed (y'all come back now!) */
  kDataBrowserContainerSorting  = 11,   /* Container is about to be sorted (lock any volatile properties) */
  kDataBrowserContainerSorted   = 12,   /* Container has been sorted (you may release any property locks) */
  kDataBrowserUserToggledContainer = 16, /* _User_ requested container open/close state to be toggled */
  kDataBrowserTargetChanged     = 15,   /* The target has changed to the specified item */
  kDataBrowserUserStateChanged  = 13,   /* The user has reformatted the view for the target */
  kDataBrowserSelectionSetChanged = 14  /* The selection set has been modified (net result may be the same) */
};


/*
 *  DataBrowserPropertyID
 *  
 *  Discussion:
 *    Properties with values 0 through 1023 are reserved for Apple's
 *    use. Values greater than or equal to 1024 are for client use.
 */
enum {
                                        /* Predefined attribute properties, optional & non-display unless otherwise stated */
  kDataBrowserItemNoProperty    = 0,    /* The anti-property (no associated data) */
  kDataBrowserItemIsActiveProperty = 1, /* Boolean typed data (defaults to true) */
  kDataBrowserItemIsSelectableProperty = 2, /* Boolean typed data (defaults to true) */
  kDataBrowserItemIsEditableProperty = 3, /* Boolean typed data (defaults to false, used for editable properties) */
  kDataBrowserItemIsContainerProperty = 4, /* Boolean typed data (defaults to false) */
  kDataBrowserContainerIsOpenableProperty = 5, /* Boolean typed data (defaults to true) */
  kDataBrowserContainerIsClosableProperty = 6, /* Boolean typed data (defaults to true) */
  kDataBrowserContainerIsSortableProperty = 7, /* Boolean typed data (defaults to true) */
  kDataBrowserItemSelfIdentityProperty = 8, /* kDataBrowserIconAndTextType (display property; ColumnView only) */

  /*
   * kDataBrowserContainerAliasIDProperty is sent to your
   * DataBrowserItemDataProcPtr callback to give you a chance to follow
   * an alias or symlink that the item might represent. If the incoming
   * item is an alias to another item, you can call
   * SetDataBrowserItemDataItemID to let Data Browser know which other
   * item the incoming item points to. 
   * 
   * This is only sent from column view, and your support for it is
   * optional. It allows Data Browser to be more memory efficient with
   * its internal storage. If a given container item is an alias to an
   * item whose contents are already displayed in an existing column
   * view column, the contents can be shared between those two columns.
   */
  kDataBrowserContainerAliasIDProperty = 9, /* DataBrowserItemID (alias/symlink an item to a container item) */

  /*
   * kDataBrowserColumnViewPreviewProperty is sent to various callbacks
   * to give you a chance to draw or track in the preview column of
   * column view. 
   * 
   * You can also pass kDataBrowserColumnViewPreviewProperty in the
   * property parameter of RevealDataBrowserItem in conjunction with
   * the appropriate DataBrowserItemID of the item whose preview is
   * being displayed when you want to make sure the preview column is
   * visible to the user. 
   * 
   * kDataBrowserColumnViewPreviewProperty is only supported in column
   * view.
   */
  kDataBrowserColumnViewPreviewProperty = 10, /* kDataBrowserCustomType (display property; ColumnView only) */

  /*
   * kDataBrowserItemParentContainerProperty is sent to your
   * DataBrowserItemDataProcPtr callback when Data Browser needs to
   * know the parent container item for a given item. 
   * 
   * In column view, this allows the internals of SetDataBrowserTarget
   * to work. The target is the container whose contents you wish to
   * display, which is the rightmost column in column view. However,
   * unlike SetDataBrowserColumnViewPath, SetDataBrowserTarget doesn't
   * offer a way for you to communicate the DataBrowserItemIDs of the
   * rest of the column containers, so SetDataBrowserTarget needs to
   * ask for them explicitly by asking for the container's parent, then
   * the container's parent's parent, and so on. 
   * 
   * In list view, this allows you to pass a non-container to
   * SetDataBrowserTarget. In this situation, Data Browser will ask you
   * for the parent of the target so it knows which container to
   * display the contents of in the list view. 
   * 
   * In both list and column views, your DataBrowserItemDataProcPtr
   * callback might be called with
   * kDataBrowserItemParentContainerProperty at a variety of other
   * times, so you should be sure to support this property if your Data
   * Browser displays a containment hierarchy.
   */
  kDataBrowserItemParentContainerProperty = 11 /* DataBrowserItemID (the parent of the specified item, used by ColumnView) */
};

typedef unsigned long                   DataBrowserPropertyID;
/* DataBrowser Property Types (for display properties; i.e. ListView columns) */
/*      These are primarily presentation types (or styles) although         */
/*      they also imply a particular set of primitive types or structures.  */
typedef OSType                          DataBrowserPropertyType;
enum {
                                        /* == Corresponding data type or structure == */
  kDataBrowserCustomType        = 0x3F3F3F3F, /* No associated data, custom callbacks used */
  kDataBrowserIconType          = 'icnr', /* IconRef, IconTransformType, RGBColor */
  kDataBrowserTextType          = 'text', /* CFStringRef */
  kDataBrowserDateTimeType      = 'date', /* DateTime or LongDateTime */
  kDataBrowserSliderType        = 'sldr', /* Min, Max, Value */
  kDataBrowserCheckboxType      = 'chbx', /* ThemeButtonValue */
  kDataBrowserProgressBarType   = 'prog', /* Min, Max, Value */
  kDataBrowserRelevanceRankType = 'rank', /* Min, Max, Value */
  kDataBrowserPopupMenuType     = 'menu', /* MenuRef, Value */
  kDataBrowserIconAndTextType   = 'ticn' /* IconRef, CFStringRef, etc */
};

/* DataBrowser Property Parts */
/*      Visual components of a property type.      */
/*      For use with GetDataBrowserItemPartBounds. */
typedef OSType                          DataBrowserPropertyPart;
enum {
  kDataBrowserPropertyEnclosingPart = 0,
  kDataBrowserPropertyContentPart = '----',
  kDataBrowserPropertyDisclosurePart = 'disc',
  kDataBrowserPropertyTextPart  = kDataBrowserTextType,
  kDataBrowserPropertyIconPart  = kDataBrowserIconType,
  kDataBrowserPropertySliderPart = kDataBrowserSliderType,
  kDataBrowserPropertyCheckboxPart = kDataBrowserCheckboxType,
  kDataBrowserPropertyProgressBarPart = kDataBrowserProgressBarType,
  kDataBrowserPropertyRelevanceRankPart = kDataBrowserRelevanceRankType
};

/* Modify appearance/behavior of display properties */
typedef UInt32                          DataBrowserPropertyFlags;
/* Low 8 bits apply to all property types */
enum {
  kDataBrowserUniversalPropertyFlagsMask = 0xFF,
  kDataBrowserPropertyIsMutable = 1 << 0,
  kDataBrowserDefaultPropertyFlags = 0 << 0,
  kDataBrowserUniversalPropertyFlags = kDataBrowserUniversalPropertyFlagsMask, /* support for an old name*/
  kDataBrowserPropertyIsEditable = kDataBrowserPropertyIsMutable /* support for an old name*/
};

/* Next 8 bits contain property-specific modifiers */

/*
 *  Summary:
 *    Data Browser Property Flags
 */
enum {
  kDataBrowserPropertyFlagsOffset = 8,
  kDataBrowserPropertyFlagsMask = 0xFF << kDataBrowserPropertyFlagsOffset,
  kDataBrowserCheckboxTriState  = 1 << kDataBrowserPropertyFlagsOffset, /* kDataBrowserCheckboxType*/
  kDataBrowserDateTimeRelative  = 1 << (kDataBrowserPropertyFlagsOffset), /* kDataBrowserDateTimeType */
  kDataBrowserDateTimeDateOnly  = 1 << (kDataBrowserPropertyFlagsOffset + 1), /* kDataBrowserDateTimeType */
  kDataBrowserDateTimeTimeOnly  = 1 << (kDataBrowserPropertyFlagsOffset + 2), /* kDataBrowserDateTimeType */
  kDataBrowserDateTimeSecondsToo = 1 << (kDataBrowserPropertyFlagsOffset + 3), /* kDataBrowserDateTimeType */
  kDataBrowserSliderPlainThumb  = kThemeThumbPlain << kDataBrowserPropertyFlagsOffset, /* kDataBrowserSliderType */
  kDataBrowserSliderUpwardThumb = kThemeThumbUpward << kDataBrowserPropertyFlagsOffset, /* kDataBrowserSliderType */
  kDataBrowserSliderDownwardThumb = kThemeThumbDownward << kDataBrowserPropertyFlagsOffset, /* kDataBrowserSliderType */
  kDataBrowserDoNotTruncateText = 3 << kDataBrowserPropertyFlagsOffset, /* kDataBrowserTextType && kDataBrowserIconAndTextType */
  kDataBrowserTruncateTextAtEnd = 2 << kDataBrowserPropertyFlagsOffset, /* kDataBrowserTextType && kDataBrowserIconAndTextType */
  kDataBrowserTruncateTextMiddle = 0 << kDataBrowserPropertyFlagsOffset, /* kDataBrowserTextType && kDataBrowserIconAndTextType */
  kDataBrowserTruncateTextAtStart = 1 << kDataBrowserPropertyFlagsOffset, /* kDataBrowserTextType && kDataBrowserIconAndTextType */

  /*
   * This flag is only for use with columns of type
   * kDataBrowserPopupMenuType. This flag indicates that the popup be
   * drawn in a sleek buttonless fashion. The text will be drawn next
   * to a popup glyph, and the whole cell will be clickable. Available
   * on 10.4 and later.
   */
  kDataBrowserPopupMenuButtonless = 1 << kDataBrowserPropertyFlagsOffset, /* kDataBrowserPopupMenuType*/
  kDataBrowserPropertyModificationFlags = kDataBrowserPropertyFlagsMask, /* support for an old name*/
  kDataBrowserRelativeDateTime  = kDataBrowserDateTimeRelative /* support for an old name*/
};

/*
   Next 8 bits contain viewStyle-specific modifiers 
   See individual ViewStyle sections below for flag definitions 
*/
enum {
  kDataBrowserViewSpecificFlagsOffset = 16,
  kDataBrowserViewSpecificFlagsMask = 0xFF << kDataBrowserViewSpecificFlagsOffset,
  kDataBrowserViewSpecificPropertyFlags = kDataBrowserViewSpecificFlagsMask /* support for an old name*/
};

/* High 8 bits are reserved for client application use */
enum {
  kDataBrowserClientPropertyFlagsOffset = 24,
  kDataBrowserClientPropertyFlagsMask = 0xFF << kDataBrowserClientPropertyFlagsOffset
};

/* Client defined property description */
struct DataBrowserPropertyDesc {
  DataBrowserPropertyID  propertyID;
  DataBrowserPropertyType  propertyType;
  DataBrowserPropertyFlags  propertyFlags;
};
typedef struct DataBrowserPropertyDesc  DataBrowserPropertyDesc;
/* Callback definition for use with ForEachDataBrowserItem */
typedef CALLBACK_API( void , DataBrowserItemProcPtr )(DataBrowserItemID item, DataBrowserItemState state, void *clientData);
typedef STACK_UPP_TYPE(DataBrowserItemProcPtr)                  DataBrowserItemUPP;
/*
 *  NewDataBrowserItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemUPP
NewDataBrowserItemUPP(DataBrowserItemProcPtr userRoutine)     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemUPP(DataBrowserItemUPP userUPP)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserItemUPP(
  DataBrowserItemID     item,
  DataBrowserItemState  state,
  void *                clientData,
  DataBrowserItemUPP    userUPP)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline DataBrowserItemUPP                                   NewDataBrowserItemUPP(DataBrowserItemProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeDataBrowserItemUPP(DataBrowserItemUPP) { }
    inline void                                                 InvokeDataBrowserItemUPP(DataBrowserItemID item, DataBrowserItemState state, void * clientData, DataBrowserItemUPP userUPP) { (*userUPP)(item, state, clientData); }
  #else
    #define NewDataBrowserItemUPP(userRoutine)                  ((DataBrowserItemUPP)userRoutine)
    #define DisposeDataBrowserItemUPP(userUPP)
    #define InvokeDataBrowserItemUPP(item, state, clientData, userUPP) (*userUPP)(item, state, clientData)
  #endif
#endif

/* Creation/Configuration */
#if !__LP64__
/*
 *  CreateDataBrowserControl()
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
CreateDataBrowserControl(
  WindowRef              window,
  const Rect *           boundsRect,
  DataBrowserViewStyle   style,
  ControlRef *           outControl)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserViewStyle()
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
GetDataBrowserViewStyle(
  ControlRef              browser,
  DataBrowserViewStyle *  style)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserViewStyle()
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
SetDataBrowserViewStyle(
  ControlRef             browser,
  DataBrowserViewStyle   style)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Data Browser attributes
 *  
 *  Discussion:
 *    For use with DataBrowserChangeAttributes and
 *    DataBrowserGetAttributes. Available in Mac OS X 10.4 and later.
 */
enum {

  /*
   * A constant with value zero; the lack of any attributes.
   */
  kDataBrowserAttributeNone     = 0,

  /*
   * In Column View, this Data Browser is allowed to resize the owning
   * window whenever necessary. This includes, but is not necessarily
   * limited to, situations where column resize operations need more
   * visible space in the window. If you turn this attribute on, your
   * window must tolerate being resized behind your app's back. If your
   * window needs to react to bounds changes, use a
   * kEventWindowBoundsChanged event handler. If you need to constrain
   * your window's minimum and maximum bounds, use
   * kEventWindowGetMinimum/MaximumSize handlers, the
   * SetWindowResizeLimits API, or something similar.
   */
  kDataBrowserAttributeColumnViewResizeWindow = (1 << 0),

  /*
   * In List View, this Data Browser should draw alternating row
   * background colors.
   */
  kDataBrowserAttributeListViewAlternatingRowColors = (1 << 1),

  /*
   * In List View, this Data Browser should draw a vertical line
   * between the columns.
   */
  kDataBrowserAttributeListViewDrawColumnDividers = (1 << 2),

  /*
   * Whether the Data Browser should hide scroll bar when they are not
   * necessary, and use the additional space to draw more content. By
   * default this attribute is turned off. This attribute is available
   * on Mac OS X 10.5 and later. 
   * As of Mac OS X 10.5, this is only respected in list view, but it
   * may be respected in column view in the future. If you set this now
   * for a Data Browser instance that uses column view, you must be
   * prepared for the scroll bars to auto hide in the future. This
   * attribute is only respected in compositing mode; it will be
   * ignored in non-compositing mode. 
   * When this attribute is turned on for a Data Browser instance that
   * is allowed to use both horizontal and vertical scroll bars (see
   * SetDataBrowserHasScrollBars), the Data Browser will attempt to use
   * the square space at the bottom right of the scroll bar area
   * whenever necessary. In other words, when the vertical scroll bar
   * is not necessary, the Data Browser will stretch the horizontal
   * scroll bar across the entire width. If your code needs Data
   * Browser to *not* use that square space - possibly because your
   * window's grow box will overlap that area - you should also turn on
   * the kDataBrowserAttributeReserveGrowBoxSpace attribute whenever
   * you turn on scroll bar auto hiding.
   */
  kDataBrowserAttributeAutoHideScrollBars = (1 << 3),

  /*
   * Whether the Data Browser should avoid positioning either of its
   * scroll bars such that they overlap the very bottom right of the
   * content area, where you'd typically have a grow box. By default,
   * this attribute is turned off. This attribute is available on Mac
   * OS X 10.5 and later.
   */
  kDataBrowserAttributeReserveGrowBoxSpace = (1 << 4)
};

#if !__LP64__
/*
 *  DataBrowserChangeAttributes()
 *  
 *  Summary:
 *    Set the attributes for the given Data Browser.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDataBrowser:
 *      The Data Browser whose attributes to change.
 *    
 *    inAttributesToSet:
 *      The attributes to set.
 *    
 *    inAttributesToClear:
 *      The attributes to clear.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DataBrowserChangeAttributes(
  ControlRef   inDataBrowser,
  OptionBits   inAttributesToSet,
  OptionBits   inAttributesToClear)                           AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  DataBrowserGetAttributes()
 *  
 *  Summary:
 *    Returns the attributes of a given Data Browser.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDataBrowser:
 *      The Data Browser whose attributes to query.
 *    
 *    outAttributes:
 *      On exit, will contain the attributes of the Data Browser. This
 *      parameter cannot be NULL.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DataBrowserGetAttributes(
  ControlRef    inDataBrowser,
  OptionBits *  outAttributes)                                AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;



#endif  /* !__LP64__ */


/*
 *  Summary:
 *    DataBrowserMetric values
 *  
 *  Discussion:
 *    For use with DataBrowserSetMetric.
 */
enum {

  /*
   * The content (icon, text, etc.) within a cell is drawn a certain
   * amount in from the left & right edges of the cell. This metric
   * governs the amount of inset.
   */
  kDataBrowserMetricCellContentInset = 1,

  /*
   * This metric controls the space between the icon and text within a
   * column of type kDataBrowserIconAndTextType.
   */
  kDataBrowserMetricIconAndTextGap = 2,

  /*
   * In List View only, this metric is similar to
   * kDataBrowserMetricCellContentInset, but it only affects the
   * disclosure column and it only affects the side of the cell that
   * displays the disclosure triangle. In other words, this metric is
   * used instead of (not in addition to)
   * DataBrowserMetricCellContentInset for one side of the disclosure
   * column.
   */
  kDataBrowserMetricDisclosureColumnEdgeInset = 3,

  /*
   * In List View only, this metric controls the amount of space
   * between the disclosure triangle and the cell's content.
   */
  kDataBrowserMetricDisclosureTriangleAndContentGap = 4,

  /*
   * In List View only, this metric controls the amount of space in the
   * disclosure column for each level of indentation in progressively
   * deeper hierarchies of disclosed items.
   */
  kDataBrowserMetricDisclosureColumnPerDepthGap = 5,
  kDataBrowserMetricLast        = kDataBrowserMetricDisclosureColumnPerDepthGap
};


typedef UInt32                          DataBrowserMetric;
#if !__LP64__
/*
 *  DataBrowserSetMetric()
 *  
 *  Summary:
 *    Sets a value for a specified Data Browser metric.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDataBrowser:
 *      The Data Browser instance whose metric is being changed.
 *    
 *    inMetric:
 *      The DataBrowserMetric whose value is being changed.
 *    
 *    inUseDefaultValue:
 *      A Boolean indicating whether you want the Data Browser instance
 *      to revert to the default value for the metric. If you pass
 *      true, inValue will be ignored and a suitable default value will
 *      be used. If you pass false, inValue will be used as the value
 *      of the metric.
 *    
 *    inValue:
 *      When you pass false for inUseDefaultValue, this parameter is
 *      the value to use for the metric.
 *  
 *  Result:
 *    An operating system status code. If the incoming ControlRef isn't
 *    a Data Browser instance, or if the incoming metric isn't known,
 *    this function will return paramErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DataBrowserSetMetric(
  ControlRef          inDataBrowser,
  DataBrowserMetric   inMetric,
  Boolean             inUseDefaultValue,
  CGFloat             inValue)                                AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  DataBrowserGetMetric()
 *  
 *  Summary:
 *    Gets the value for a specified Data Browser metric.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDataBrowser:
 *      The Data Browser instance whose metric value to get.
 *    
 *    inMetric:
 *      The DataBrowserMetric value to get.
 *    
 *    outUsingDefaultValue:
 *      On exit, this is a Boolean indicating whether the metric's
 *      value is determined by Data Browser's default values. You may
 *      pass NULL if you don't need this information.
 *    
 *    outValue:
 *      On exit, this is the value of the metric.
 *  
 *  Result:
 *    An operating system status code. If the incoming ControlRef isn't
 *    a Data Browser instance, or if the incoming metric isn't known,
 *    this function will return paramErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DataBrowserGetMetric(
  ControlRef          inDataBrowser,
  DataBrowserMetric   inMetric,
  Boolean *           outUsingDefaultValue,       /* can be NULL */
  CGFloat *           outValue)                               AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/* Item Manipulation */
/* Passing NULL for "items" argument to RemoveDataBrowserItems and */
/* UpdateDataBrowserItems refers to all items in the specified container. */
/* Passing NULL for "items" argument to AddDataBrowserItems means */
/* "generate IDs starting from 1." */
/*
 *  AddDataBrowserItems()
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
AddDataBrowserItems(
  ControlRef                 browser,
  DataBrowserItemID          container,
  ItemCount                  numItems,
  const DataBrowserItemID *  items,                 /* can be NULL */
  DataBrowserPropertyID      preSortProperty)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RemoveDataBrowserItems()
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
RemoveDataBrowserItems(
  ControlRef                 browser,
  DataBrowserItemID          container,
  ItemCount                  numItems,
  const DataBrowserItemID *  items,                 /* can be NULL */
  DataBrowserPropertyID      preSortProperty)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  UpdateDataBrowserItems()
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
UpdateDataBrowserItems(
  ControlRef                 browser,
  DataBrowserItemID          container,
  ItemCount                  numItems,
  const DataBrowserItemID *  items,                 /* can be NULL */
  DataBrowserPropertyID      preSortProperty,
  DataBrowserPropertyID      propertyID)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Edit Menu Enabling and Handling */
/*
 *  EnableDataBrowserEditCommand()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
EnableDataBrowserEditCommand(
  ControlRef               browser,
  DataBrowserEditCommand   command)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ExecuteDataBrowserEditCommand()
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
ExecuteDataBrowserEditCommand(
  ControlRef               browser,
  DataBrowserEditCommand   command)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserSelectionAnchor()
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
GetDataBrowserSelectionAnchor(
  ControlRef           browser,
  DataBrowserItemID *  first,
  DataBrowserItemID *  last)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  MoveDataBrowserSelectionAnchor()
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
MoveDataBrowserSelectionAnchor(
  ControlRef                            browser,
  DataBrowserSelectionAnchorDirection   direction,
  Boolean                               extendSelection)      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Container Manipulation */
/*
 *  OpenDataBrowserContainer()
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
OpenDataBrowserContainer(
  ControlRef          browser,
  DataBrowserItemID   container)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CloseDataBrowserContainer()
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
CloseDataBrowserContainer(
  ControlRef          browser,
  DataBrowserItemID   container)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SortDataBrowserContainer()
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
SortDataBrowserContainer(
  ControlRef          browser,
  DataBrowserItemID   container,
  Boolean             sortChildren)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Aggregate Item Access and Iteration */
/*
 *  GetDataBrowserItems()
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
GetDataBrowserItems(
  ControlRef             browser,
  DataBrowserItemID      container,
  Boolean                recurse,
  DataBrowserItemState   state,
  Handle                 items)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemCount()
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
GetDataBrowserItemCount(
  ControlRef             browser,
  DataBrowserItemID      container,
  Boolean                recurse,
  DataBrowserItemState   state,
  ItemCount *            numItems)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ForEachDataBrowserItem()
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
ForEachDataBrowserItem(
  ControlRef             browser,
  DataBrowserItemID      container,
  Boolean                recurse,
  DataBrowserItemState   state,
  DataBrowserItemUPP     callback,
  void *                 clientData)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Individual Item Access and Display */
/*
 *  IsDataBrowserItemSelected()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsDataBrowserItemSelected(
  ControlRef          browser,
  DataBrowserItemID   item)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemState()
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
GetDataBrowserItemState(
  ControlRef              browser,
  DataBrowserItemID       item,
  DataBrowserItemState *  state)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RevealDataBrowserItem()
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
RevealDataBrowserItem(
  ControlRef                 browser,
  DataBrowserItemID          item,
  DataBrowserPropertyID      propertyID,
  DataBrowserRevealOptions   options)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Selection Set Manipulation */
/*
 *  SetDataBrowserSelectedItems()
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
SetDataBrowserSelectedItems(
  ControlRef                 browser,
  ItemCount                  numItems,
  const DataBrowserItemID *  items,
  DataBrowserSetOption       operation)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* DataBrowser Attribute Manipulation */
/* The user customizable portion of the current view style settings */
/*
 *  SetDataBrowserUserState()
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
SetDataBrowserUserState(
  ControlRef        browser,
  CFDictionaryRef   stateInfo)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserUserState()
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
GetDataBrowserUserState(
  ControlRef         browser,
  CFDictionaryRef *  stateInfo)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* All items are active/enabled or not */
/*
 *  SetDataBrowserActiveItems()
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
SetDataBrowserActiveItems(
  ControlRef   browser,
  Boolean      active)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserActiveItems()
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
GetDataBrowserActiveItems(
  ControlRef   browser,
  Boolean *    active)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Inset the scrollbars within the DataBrowser bounds */
/*
 *  SetDataBrowserScrollBarInset()
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
SetDataBrowserScrollBarInset(
  ControlRef   browser,
  Rect *       insetRect)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserScrollBarInset()
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
GetDataBrowserScrollBarInset(
  ControlRef   browser,
  Rect *       insetRect)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* The "user focused" item */
/* For the ListView, this means the root container */
/* For the ColumnView, this means the rightmost container column */
/*
 *  SetDataBrowserTarget()
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
SetDataBrowserTarget(
  ControlRef          browser,
  DataBrowserItemID   target)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTarget()
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
GetDataBrowserTarget(
  ControlRef           browser,
  DataBrowserItemID *  target)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Current sort ordering */
/* ListView tracks this per-column */
/*
 *  SetDataBrowserSortOrder()
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
SetDataBrowserSortOrder(
  ControlRef             browser,
  DataBrowserSortOrder   order)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserSortOrder()
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
GetDataBrowserSortOrder(
  ControlRef              browser,
  DataBrowserSortOrder *  order)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Scrollbar values */
/*
 *  SetDataBrowserScrollPosition()
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
SetDataBrowserScrollPosition(
  ControlRef   browser,
  UInt32       top,
  UInt32       left)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserScrollPosition()
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
GetDataBrowserScrollPosition(
  ControlRef   browser,
  UInt32 *     top,
  UInt32 *     left)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Show/Hide each scrollbar */
/*
 *  SetDataBrowserHasScrollBars()
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
SetDataBrowserHasScrollBars(
  ControlRef   browser,
  Boolean      horiz,
  Boolean      vert)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserHasScrollBars()
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
GetDataBrowserHasScrollBars(
  ControlRef   browser,
  Boolean *    horiz,
  Boolean *    vert)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Property passed to sort callback (ListView sort column) */
/*
 *  SetDataBrowserSortProperty()
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
SetDataBrowserSortProperty(
  ControlRef              browser,
  DataBrowserPropertyID   property)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserSortProperty()
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
GetDataBrowserSortProperty(
  ControlRef               browser,
  DataBrowserPropertyID *  property)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Modify selection behavior */
/*
 *  SetDataBrowserSelectionFlags()
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
SetDataBrowserSelectionFlags(
  ControlRef                  browser,
  DataBrowserSelectionFlags   selectionFlags)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserSelectionFlags()
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
GetDataBrowserSelectionFlags(
  ControlRef                   browser,
  DataBrowserSelectionFlags *  selectionFlags)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Dynamically modify property appearance/behavior */
/*
 *  SetDataBrowserPropertyFlags()
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
SetDataBrowserPropertyFlags(
  ControlRef                 browser,
  DataBrowserPropertyID      property,
  DataBrowserPropertyFlags   flags)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserPropertyFlags()
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
GetDataBrowserPropertyFlags(
  ControlRef                  browser,
  DataBrowserPropertyID       property,
  DataBrowserPropertyFlags *  flags)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Text of current in-place edit session */
/*
 *  SetDataBrowserEditText()
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
SetDataBrowserEditText(
  ControlRef    browser,
  CFStringRef   text)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CopyDataBrowserEditText()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CopyDataBrowserEditText(
  ControlRef     browser,
  CFStringRef *  text)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserEditText()
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
GetDataBrowserEditText(
  ControlRef           browser,
  CFMutableStringRef   text)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Item/property currently being edited */
/*
 *  SetDataBrowserEditItem()
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
SetDataBrowserEditItem(
  ControlRef              browser,
  DataBrowserItemID       item,
  DataBrowserPropertyID   property)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserEditItem()
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
GetDataBrowserEditItem(
  ControlRef               browser,
  DataBrowserItemID *      item,
  DataBrowserPropertyID *  property)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Get the current bounds of a visual part of an item's property */
/*
 *  GetDataBrowserItemPartBounds()
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
GetDataBrowserItemPartBounds(
  ControlRef                browser,
  DataBrowserItemID         item,
  DataBrowserPropertyID     property,
  DataBrowserPropertyPart   part,
  Rect *                    bounds)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* DataBrowser ItemData Accessors (used within DataBrowserItemData callback) */
#endif  /* !__LP64__ */

typedef void *                          DataBrowserItemDataRef;
#if !__LP64__
/*
 *  SetDataBrowserItemDataIcon()
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
SetDataBrowserItemDataIcon(
  DataBrowserItemDataRef   itemData,
  IconRef                  theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataIcon()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetDataBrowserItemDataIcon(
  DataBrowserItemDataRef   itemData,
  IconRef *                theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataText()
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
SetDataBrowserItemDataText(
  DataBrowserItemDataRef   itemData,
  CFStringRef              theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataText()
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
GetDataBrowserItemDataText(
  DataBrowserItemDataRef   itemData,
  CFStringRef *            theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetDataBrowserItemDataValue()
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
SetDataBrowserItemDataValue(
  DataBrowserItemDataRef   itemData,
  SInt32                   theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataValue()
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
GetDataBrowserItemDataValue(
  DataBrowserItemDataRef   itemData,
  SInt32 *                 theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataMinimum()
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
SetDataBrowserItemDataMinimum(
  DataBrowserItemDataRef   itemData,
  SInt32                   theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataMinimum()
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
GetDataBrowserItemDataMinimum(
  DataBrowserItemDataRef   itemData,
  SInt32 *                 theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataMaximum()
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
SetDataBrowserItemDataMaximum(
  DataBrowserItemDataRef   itemData,
  SInt32                   theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataMaximum()
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
GetDataBrowserItemDataMaximum(
  DataBrowserItemDataRef   itemData,
  SInt32 *                 theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataBooleanValue()
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
SetDataBrowserItemDataBooleanValue(
  DataBrowserItemDataRef   itemData,
  Boolean                  theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataBooleanValue()
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
GetDataBrowserItemDataBooleanValue(
  DataBrowserItemDataRef   itemData,
  Boolean *                theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataMenuRef()
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
SetDataBrowserItemDataMenuRef(
  DataBrowserItemDataRef   itemData,
  MenuRef                  theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataMenuRef()
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
GetDataBrowserItemDataMenuRef(
  DataBrowserItemDataRef   itemData,
  MenuRef *                theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataRGBColor()
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
SetDataBrowserItemDataRGBColor(
  DataBrowserItemDataRef   itemData,
  const RGBColor *         theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataRGBColor()
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
GetDataBrowserItemDataRGBColor(
  DataBrowserItemDataRef   itemData,
  RGBColor *               theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetDataBrowserItemDataDrawState()
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
SetDataBrowserItemDataDrawState(
  DataBrowserItemDataRef   itemData,
  ThemeDrawState           theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataDrawState()
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
GetDataBrowserItemDataDrawState(
  DataBrowserItemDataRef   itemData,
  ThemeDrawState *         theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataButtonValue()
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
SetDataBrowserItemDataButtonValue(
  DataBrowserItemDataRef   itemData,
  ThemeButtonValue         theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataButtonValue()
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
GetDataBrowserItemDataButtonValue(
  DataBrowserItemDataRef   itemData,
  ThemeButtonValue *       theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataIconTransform()
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
SetDataBrowserItemDataIconTransform(
  DataBrowserItemDataRef   itemData,
  IconTransformType        theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataIconTransform()
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
GetDataBrowserItemDataIconTransform(
  DataBrowserItemDataRef   itemData,
  IconTransformType *      theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetDataBrowserItemDataDateTime()
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
SetDataBrowserItemDataDateTime(
  DataBrowserItemDataRef   itemData,
  SInt32                   theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataDateTime()
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
GetDataBrowserItemDataDateTime(
  DataBrowserItemDataRef   itemData,
  SInt32 *                 theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserItemDataLongDateTime()
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
SetDataBrowserItemDataLongDateTime(
  DataBrowserItemDataRef   itemData,
  const LongDateTime *     theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataLongDateTime()
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
GetDataBrowserItemDataLongDateTime(
  DataBrowserItemDataRef   itemData,
  LongDateTime *           theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetDataBrowserItemDataItemID()
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
SetDataBrowserItemDataItemID(
  DataBrowserItemDataRef   itemData,
  DataBrowserItemID        theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataItemID()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetDataBrowserItemDataItemID(
  DataBrowserItemDataRef   itemData,
  DataBrowserItemID *      theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserItemDataProperty()
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
GetDataBrowserItemDataProperty(
  DataBrowserItemDataRef   itemData,
  DataBrowserPropertyID *  theData)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* Standard DataBrowser Callbacks */

/* Basic Item Management & Manipulation */
#endif  /* !__LP64__ */

typedef CALLBACK_API( OSStatus , DataBrowserItemDataProcPtr )(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, DataBrowserItemDataRef itemData, Boolean setValue);
typedef STACK_UPP_TYPE(DataBrowserItemDataProcPtr)              DataBrowserItemDataUPP;

/* Item Comparison */
typedef CALLBACK_API( Boolean , DataBrowserItemCompareProcPtr )(ControlRef browser, DataBrowserItemID itemOne, DataBrowserItemID itemTwo, DataBrowserPropertyID sortProperty);
typedef STACK_UPP_TYPE(DataBrowserItemCompareProcPtr)           DataBrowserItemCompareUPP;

/* ItemEvent Notification */
/*  A Very Important Note about DataBrowserItemNotificationProcPtr:                                     */
/*                                                                                                      */
/*  Under all currently shipping versions of CarbonLib (eg. up through 1.3), your callback is called    */
/*  just as the prototype appears in this header. It should only be expecting three parameters because  */
/*  DataBrowser will only pass three parameters.                                                        */
/*                                                                                                      */
/*  Under Mac OS X, your callback is called with an additional parameter. If you wish to interpret      */
/*  the additional parameter, your callback should have the same prototype as the                       */
/*  DataBrowserItemNotificationWithItemProcPtr (below). You may freely take a callback with this        */
/*  prototype and pass it to NewDataBrowserItemNotificationUPP in order to generate a                   */
/*  DataBrowserItemNotificationUPP that you can use just like any other DataBrowserItemNotificationUPP. */
/*                                                                                                      */
/*  If you use this technique under CarbonLib, you will *not* receive valid data in the fourth          */
/*  parameter, and any attempt to use the invalid data will probably result in a crash.                 */
typedef CALLBACK_API( void , DataBrowserItemNotificationWithItemProcPtr )(ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message, DataBrowserItemDataRef itemData);
typedef CALLBACK_API( void , DataBrowserItemNotificationProcPtr )(ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message);
typedef STACK_UPP_TYPE(DataBrowserItemNotificationWithItemProcPtr)  DataBrowserItemNotificationWithItemUPP;
typedef STACK_UPP_TYPE(DataBrowserItemNotificationProcPtr)      DataBrowserItemNotificationUPP;


/* Drag & Drop Processing */
typedef CALLBACK_API( Boolean , DataBrowserAddDragItemProcPtr )(ControlRef browser, DragReference theDrag, DataBrowserItemID item, ItemReference *itemRef);
typedef CALLBACK_API( Boolean , DataBrowserAcceptDragProcPtr )(ControlRef browser, DragReference theDrag, DataBrowserItemID item);
typedef CALLBACK_API( Boolean , DataBrowserReceiveDragProcPtr )(ControlRef browser, DragReference theDrag, DataBrowserItemID item);
typedef CALLBACK_API( void , DataBrowserPostProcessDragProcPtr )(ControlRef browser, DragReference theDrag, OSStatus trackDragResult);
typedef STACK_UPP_TYPE(DataBrowserAddDragItemProcPtr)           DataBrowserAddDragItemUPP;
typedef STACK_UPP_TYPE(DataBrowserAcceptDragProcPtr)            DataBrowserAcceptDragUPP;
typedef STACK_UPP_TYPE(DataBrowserReceiveDragProcPtr)           DataBrowserReceiveDragUPP;
typedef STACK_UPP_TYPE(DataBrowserPostProcessDragProcPtr)       DataBrowserPostProcessDragUPP;

/* Contextual Menu Support */
typedef CALLBACK_API( void , DataBrowserGetContextualMenuProcPtr )(ControlRef browser, MenuRef *menu, UInt32 *helpType, CFStringRef *helpItemString, AEDesc *selection);
typedef CALLBACK_API( void , DataBrowserSelectContextualMenuProcPtr )(ControlRef browser, MenuRef menu, UInt32 selectionType, SInt16 menuID, MenuItemIndex menuItem);
typedef STACK_UPP_TYPE(DataBrowserGetContextualMenuProcPtr)     DataBrowserGetContextualMenuUPP;
typedef STACK_UPP_TYPE(DataBrowserSelectContextualMenuProcPtr)  DataBrowserSelectContextualMenuUPP;

/* Help Manager Support */
typedef CALLBACK_API( void , DataBrowserItemHelpContentProcPtr )(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentRec *ioHelpContent);
typedef STACK_UPP_TYPE(DataBrowserItemHelpContentProcPtr)       DataBrowserItemHelpContentUPP;
/*
 *  NewDataBrowserItemDataUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemDataUPP
NewDataBrowserItemDataUPP(DataBrowserItemDataProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserItemCompareUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemCompareUPP
NewDataBrowserItemCompareUPP(DataBrowserItemCompareProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserItemNotificationWithItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemNotificationWithItemUPP
NewDataBrowserItemNotificationWithItemUPP(DataBrowserItemNotificationWithItemProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  NewDataBrowserItemNotificationUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemNotificationUPP
NewDataBrowserItemNotificationUPP(DataBrowserItemNotificationProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserAddDragItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserAddDragItemUPP
NewDataBrowserAddDragItemUPP(DataBrowserAddDragItemProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserAcceptDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserAcceptDragUPP
NewDataBrowserAcceptDragUPP(DataBrowserAcceptDragProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserReceiveDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserReceiveDragUPP
NewDataBrowserReceiveDragUPP(DataBrowserReceiveDragProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserPostProcessDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserPostProcessDragUPP
NewDataBrowserPostProcessDragUPP(DataBrowserPostProcessDragProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserGetContextualMenuUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserGetContextualMenuUPP
NewDataBrowserGetContextualMenuUPP(DataBrowserGetContextualMenuProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserSelectContextualMenuUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserSelectContextualMenuUPP
NewDataBrowserSelectContextualMenuUPP(DataBrowserSelectContextualMenuProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewDataBrowserItemHelpContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemHelpContentUPP
NewDataBrowserItemHelpContentUPP(DataBrowserItemHelpContentProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserItemDataUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemDataUPP(DataBrowserItemDataUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserItemCompareUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemCompareUPP(DataBrowserItemCompareUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserItemNotificationWithItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemNotificationWithItemUPP(DataBrowserItemNotificationWithItemUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  DisposeDataBrowserItemNotificationUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemNotificationUPP(DataBrowserItemNotificationUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserAddDragItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserAddDragItemUPP(DataBrowserAddDragItemUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserAcceptDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserAcceptDragUPP(DataBrowserAcceptDragUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserReceiveDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserReceiveDragUPP(DataBrowserReceiveDragUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserPostProcessDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserPostProcessDragUPP(DataBrowserPostProcessDragUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserGetContextualMenuUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserGetContextualMenuUPP(DataBrowserGetContextualMenuUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserSelectContextualMenuUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserSelectContextualMenuUPP(DataBrowserSelectContextualMenuUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeDataBrowserItemHelpContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemHelpContentUPP(DataBrowserItemHelpContentUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserItemDataUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus
InvokeDataBrowserItemDataUPP(
  ControlRef              browser,
  DataBrowserItemID       item,
  DataBrowserPropertyID   property,
  DataBrowserItemDataRef  itemData,
  Boolean                 setValue,
  DataBrowserItemDataUPP  userUPP)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserItemCompareUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean
InvokeDataBrowserItemCompareUPP(
  ControlRef                 browser,
  DataBrowserItemID          itemOne,
  DataBrowserItemID          itemTwo,
  DataBrowserPropertyID      sortProperty,
  DataBrowserItemCompareUPP  userUPP)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserItemNotificationWithItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserItemNotificationWithItemUPP(
  ControlRef                              browser,
  DataBrowserItemID                       item,
  DataBrowserItemNotification             message,
  DataBrowserItemDataRef                  itemData,
  DataBrowserItemNotificationWithItemUPP  userUPP)            AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  InvokeDataBrowserItemNotificationUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserItemNotificationUPP(
  ControlRef                      browser,
  DataBrowserItemID               item,
  DataBrowserItemNotification     message,
  DataBrowserItemNotificationUPP  userUPP)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserAddDragItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean
InvokeDataBrowserAddDragItemUPP(
  ControlRef                 browser,
  DragReference              theDrag,
  DataBrowserItemID          item,
  ItemReference *            itemRef,
  DataBrowserAddDragItemUPP  userUPP)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserAcceptDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean
InvokeDataBrowserAcceptDragUPP(
  ControlRef                browser,
  DragReference             theDrag,
  DataBrowserItemID         item,
  DataBrowserAcceptDragUPP  userUPP)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserReceiveDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean
InvokeDataBrowserReceiveDragUPP(
  ControlRef                 browser,
  DragReference              theDrag,
  DataBrowserItemID          item,
  DataBrowserReceiveDragUPP  userUPP)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserPostProcessDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserPostProcessDragUPP(
  ControlRef                     browser,
  DragReference                  theDrag,
  OSStatus                       trackDragResult,
  DataBrowserPostProcessDragUPP  userUPP)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserGetContextualMenuUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserGetContextualMenuUPP(
  ControlRef                       browser,
  MenuRef *                        menu,
  UInt32 *                         helpType,
  CFStringRef *                    helpItemString,
  AEDesc *                         selection,
  DataBrowserGetContextualMenuUPP  userUPP)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserSelectContextualMenuUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserSelectContextualMenuUPP(
  ControlRef                          browser,
  MenuRef                             menu,
  UInt32                              selectionType,
  SInt16                              menuID,
  MenuItemIndex                       menuItem,
  DataBrowserSelectContextualMenuUPP  userUPP)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeDataBrowserItemHelpContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserItemHelpContentUPP(
  ControlRef                     browser,
  DataBrowserItemID              item,
  DataBrowserPropertyID          property,
  HMContentRequest               inRequest,
  HMContentProvidedType *        outContentProvided,
  HMHelpContentRec *             ioHelpContent,
  DataBrowserItemHelpContentUPP  userUPP)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline DataBrowserItemDataUPP                               NewDataBrowserItemDataUPP(DataBrowserItemDataProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserItemCompareUPP                            NewDataBrowserItemCompareUPP(DataBrowserItemCompareProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserItemNotificationWithItemUPP               NewDataBrowserItemNotificationWithItemUPP(DataBrowserItemNotificationWithItemProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserItemNotificationUPP                       NewDataBrowserItemNotificationUPP(DataBrowserItemNotificationProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserAddDragItemUPP                            NewDataBrowserAddDragItemUPP(DataBrowserAddDragItemProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserAcceptDragUPP                             NewDataBrowserAcceptDragUPP(DataBrowserAcceptDragProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserReceiveDragUPP                            NewDataBrowserReceiveDragUPP(DataBrowserReceiveDragProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserPostProcessDragUPP                        NewDataBrowserPostProcessDragUPP(DataBrowserPostProcessDragProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserGetContextualMenuUPP                      NewDataBrowserGetContextualMenuUPP(DataBrowserGetContextualMenuProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserSelectContextualMenuUPP                   NewDataBrowserSelectContextualMenuUPP(DataBrowserSelectContextualMenuProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserItemHelpContentUPP                        NewDataBrowserItemHelpContentUPP(DataBrowserItemHelpContentProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeDataBrowserItemDataUPP(DataBrowserItemDataUPP) { }
    inline void                                                 DisposeDataBrowserItemCompareUPP(DataBrowserItemCompareUPP) { }
    inline void                                                 DisposeDataBrowserItemNotificationWithItemUPP(DataBrowserItemNotificationWithItemUPP) { }
    inline void                                                 DisposeDataBrowserItemNotificationUPP(DataBrowserItemNotificationUPP) { }
    inline void                                                 DisposeDataBrowserAddDragItemUPP(DataBrowserAddDragItemUPP) { }
    inline void                                                 DisposeDataBrowserAcceptDragUPP(DataBrowserAcceptDragUPP) { }
    inline void                                                 DisposeDataBrowserReceiveDragUPP(DataBrowserReceiveDragUPP) { }
    inline void                                                 DisposeDataBrowserPostProcessDragUPP(DataBrowserPostProcessDragUPP) { }
    inline void                                                 DisposeDataBrowserGetContextualMenuUPP(DataBrowserGetContextualMenuUPP) { }
    inline void                                                 DisposeDataBrowserSelectContextualMenuUPP(DataBrowserSelectContextualMenuUPP) { }
    inline void                                                 DisposeDataBrowserItemHelpContentUPP(DataBrowserItemHelpContentUPP) { }
    inline OSStatus                                             InvokeDataBrowserItemDataUPP(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, DataBrowserItemDataRef itemData, Boolean setValue, DataBrowserItemDataUPP userUPP) { return (*userUPP)(browser, item, property, itemData, setValue); }
    inline Boolean                                              InvokeDataBrowserItemCompareUPP(ControlRef browser, DataBrowserItemID itemOne, DataBrowserItemID itemTwo, DataBrowserPropertyID sortProperty, DataBrowserItemCompareUPP userUPP) { return (*userUPP)(browser, itemOne, itemTwo, sortProperty); }
    inline void                                                 InvokeDataBrowserItemNotificationWithItemUPP(ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message, DataBrowserItemDataRef itemData, DataBrowserItemNotificationWithItemUPP userUPP) { (*userUPP)(browser, item, message, itemData); }
    inline void                                                 InvokeDataBrowserItemNotificationUPP(ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message, DataBrowserItemNotificationUPP userUPP) { (*userUPP)(browser, item, message); }
    inline Boolean                                              InvokeDataBrowserAddDragItemUPP(ControlRef browser, DragReference theDrag, DataBrowserItemID item, ItemReference * itemRef, DataBrowserAddDragItemUPP userUPP) { return (*userUPP)(browser, theDrag, item, itemRef); }
    inline Boolean                                              InvokeDataBrowserAcceptDragUPP(ControlRef browser, DragReference theDrag, DataBrowserItemID item, DataBrowserAcceptDragUPP userUPP) { return (*userUPP)(browser, theDrag, item); }
    inline Boolean                                              InvokeDataBrowserReceiveDragUPP(ControlRef browser, DragReference theDrag, DataBrowserItemID item, DataBrowserReceiveDragUPP userUPP) { return (*userUPP)(browser, theDrag, item); }
    inline void                                                 InvokeDataBrowserPostProcessDragUPP(ControlRef browser, DragReference theDrag, OSStatus trackDragResult, DataBrowserPostProcessDragUPP userUPP) { (*userUPP)(browser, theDrag, trackDragResult); }
    inline void                                                 InvokeDataBrowserGetContextualMenuUPP(ControlRef browser, MenuRef * menu, UInt32 * helpType, CFStringRef * helpItemString, AEDesc * selection, DataBrowserGetContextualMenuUPP userUPP) { (*userUPP)(browser, menu, helpType, helpItemString, selection); }
    inline void                                                 InvokeDataBrowserSelectContextualMenuUPP(ControlRef browser, MenuRef menu, UInt32 selectionType, SInt16 menuID, MenuItemIndex menuItem, DataBrowserSelectContextualMenuUPP userUPP) { (*userUPP)(browser, menu, selectionType, menuID, menuItem); }
    inline void                                                 InvokeDataBrowserItemHelpContentUPP(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, HMContentRequest inRequest, HMContentProvidedType * outContentProvided, HMHelpContentRec * ioHelpContent, DataBrowserItemHelpContentUPP userUPP) { (*userUPP)(browser, item, property, inRequest, outContentProvided, ioHelpContent); }
  #else
    #define NewDataBrowserItemDataUPP(userRoutine)              ((DataBrowserItemDataUPP)userRoutine)
    #define NewDataBrowserItemCompareUPP(userRoutine)           ((DataBrowserItemCompareUPP)userRoutine)
    #define NewDataBrowserItemNotificationWithItemUPP(userRoutine) ((DataBrowserItemNotificationWithItemUPP)userRoutine)
    #define NewDataBrowserItemNotificationUPP(userRoutine)      ((DataBrowserItemNotificationUPP)userRoutine)
    #define NewDataBrowserAddDragItemUPP(userRoutine)           ((DataBrowserAddDragItemUPP)userRoutine)
    #define NewDataBrowserAcceptDragUPP(userRoutine)            ((DataBrowserAcceptDragUPP)userRoutine)
    #define NewDataBrowserReceiveDragUPP(userRoutine)           ((DataBrowserReceiveDragUPP)userRoutine)
    #define NewDataBrowserPostProcessDragUPP(userRoutine)       ((DataBrowserPostProcessDragUPP)userRoutine)
    #define NewDataBrowserGetContextualMenuUPP(userRoutine)     ((DataBrowserGetContextualMenuUPP)userRoutine)
    #define NewDataBrowserSelectContextualMenuUPP(userRoutine)  ((DataBrowserSelectContextualMenuUPP)userRoutine)
    #define NewDataBrowserItemHelpContentUPP(userRoutine)       ((DataBrowserItemHelpContentUPP)userRoutine)
    #define DisposeDataBrowserItemDataUPP(userUPP)
    #define DisposeDataBrowserItemCompareUPP(userUPP)
    #define DisposeDataBrowserItemNotificationWithItemUPP(userUPP)
    #define DisposeDataBrowserItemNotificationUPP(userUPP)
    #define DisposeDataBrowserAddDragItemUPP(userUPP)
    #define DisposeDataBrowserAcceptDragUPP(userUPP)
    #define DisposeDataBrowserReceiveDragUPP(userUPP)
    #define DisposeDataBrowserPostProcessDragUPP(userUPP)
    #define DisposeDataBrowserGetContextualMenuUPP(userUPP)
    #define DisposeDataBrowserSelectContextualMenuUPP(userUPP)
    #define DisposeDataBrowserItemHelpContentUPP(userUPP)
    #define InvokeDataBrowserItemDataUPP(browser, item, property, itemData, setValue, userUPP) (*userUPP)(browser, item, property, itemData, setValue)
    #define InvokeDataBrowserItemCompareUPP(browser, itemOne, itemTwo, sortProperty, userUPP) (*userUPP)(browser, itemOne, itemTwo, sortProperty)
    #define InvokeDataBrowserItemNotificationWithItemUPP(browser, item, message, itemData, userUPP) (*userUPP)(browser, item, message, itemData)
    #define InvokeDataBrowserItemNotificationUPP(browser, item, message, userUPP) (*userUPP)(browser, item, message)
    #define InvokeDataBrowserAddDragItemUPP(browser, theDrag, item, itemRef, userUPP) (*userUPP)(browser, theDrag, item, itemRef)
    #define InvokeDataBrowserAcceptDragUPP(browser, theDrag, item, userUPP) (*userUPP)(browser, theDrag, item)
    #define InvokeDataBrowserReceiveDragUPP(browser, theDrag, item, userUPP) (*userUPP)(browser, theDrag, item)
    #define InvokeDataBrowserPostProcessDragUPP(browser, theDrag, trackDragResult, userUPP) (*userUPP)(browser, theDrag, trackDragResult)
    #define InvokeDataBrowserGetContextualMenuUPP(browser, menu, helpType, helpItemString, selection, userUPP) (*userUPP)(browser, menu, helpType, helpItemString, selection)
    #define InvokeDataBrowserSelectContextualMenuUPP(browser, menu, selectionType, menuID, menuItem, userUPP) (*userUPP)(browser, menu, selectionType, menuID, menuItem)
    #define InvokeDataBrowserItemHelpContentUPP(browser, item, property, inRequest, outContentProvided, ioHelpContent, userUPP) (*userUPP)(browser, item, property, inRequest, outContentProvided, ioHelpContent)
  #endif
#endif

/* Standard Callback (vtable) Structure */
enum {
  kDataBrowserLatestCallbacks   = 0
};

struct DataBrowserCallbacks {
  UInt32              version;                /* Use kDataBrowserLatestCallbacks */

  union {
    struct {
      DataBrowserItemDataUPP  itemDataCallback;
      DataBrowserItemCompareUPP  itemCompareCallback;
      DataBrowserItemNotificationUPP  itemNotificationCallback;

      DataBrowserAddDragItemUPP  addDragItemCallback;
      DataBrowserAcceptDragUPP  acceptDragCallback;
      DataBrowserReceiveDragUPP  receiveDragCallback;
      DataBrowserPostProcessDragUPP  postProcessDragCallback;

      DataBrowserItemHelpContentUPP  itemHelpContentCallback;
      DataBrowserGetContextualMenuUPP  getContextualMenuCallback;
      DataBrowserSelectContextualMenuUPP  selectContextualMenuCallback;
    }                       v1;
  }                       u;
};
typedef struct DataBrowserCallbacks     DataBrowserCallbacks;
#if !__LP64__
/*
 *  InitDataBrowserCallbacks()
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
InitDataBrowserCallbacks(DataBrowserCallbacks * callbacks)    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Macro for initializing callback structure */
#define InitializeDataBrowserCallbacks(callbacks, vers) \
{ (callbacks)->version = (vers); InitDataBrowserCallbacks(callbacks); }

/*
 *  GetDataBrowserCallbacks()
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
GetDataBrowserCallbacks(
  ControlRef              browser,
  DataBrowserCallbacks *  callbacks)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserCallbacks()
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
SetDataBrowserCallbacks(
  ControlRef                    browser,
  const DataBrowserCallbacks *  callbacks)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*--------------------------------------------------------------------------------------*/
/*  DataBrowser Events                                                                  */
/*--------------------------------------------------------------------------------------*/
#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Event class
 */
enum {

  /*
   * Events related to DataBrowser.
   */
  kEventClassDataBrowser        = 'hidb'
};

/*
    kEventClassDataBrowser quick reference:
    
    kEventDataBrowserDrawCustomItem         = 1
*/

/* DataBrowser parameters*/

enum {
  kEventParamDataBrowserItemID  = 'dbid', /* typeRefCon*/
  kEventParamDataBrowserPropertyID = 'dbpd', /* typeRefCon*/
  kEventParamDataBrowserItemState = 'dbis' /* typeUInt32*/
};

/*
 *  kEventClassDataBrowser / kEventDataBrowserDrawCustomItem
 *  
 *  Summary:
 *    DataBrowser is requesting a custom item be drawn.
 *  
 *  Discussion:
 *    If the client has installed a custom item draw handler on a data
 *    browser containing custom items, as indicated by the
 *    kDataBrowserCustomType property, this message will be sent at
 *    data browser draw time for all custom items currently showing.
 *    This event does not propagate past the Data Browser control and
 *    is only sent in compositing mode. If a result other than noErr is
 *    returned the DataBrowserDrawItemProcPtr will be invoked if it has
 *    bee installed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamDirectObject (in, typeControlRef)
 *          The DataBrowser requesting a custom item be drawn.
 *    
 *    --> kEventParamDataBrowserItemID (in, typeRefCon)
 *          The item ID for the item to draw.
 *    
 *    --> kEventParamDataBrowserPropertyID (in, typeRefCon)
 *          The property ID for the item. In list view, this is the
 *          four-character sequence that you previously assigned to the
 *          column. In column view, this is the property
 *          kDataBrowserItemSelfIdentityProperty.
 *    
 *    --> kEventParamDataBrowserItemState (in, typeUInt32)
 *          The state to use when drawing the item.
 *    
 *    --> kEventParamBounds (in, typeHIRect)
 *          The bounding rectangle that specifies where in the provided
 *          context to draw the item. This rectangle is the content
 *          rectangle, not the enclosing rectangle.
 *    
 *    --> kEventParamCGContextRef (in, typeCGContext)
 *          The context into which the custom drawing should be
 *          performed. The context will be automatically flipped to use
 *          a top left orientation similar to the kEventControlDraw
 *          event.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventDataBrowserDrawCustomItem = 1
};


/* Custom Format Callbacks (kDataBrowserCustomType display properties) */

typedef UInt32                          DataBrowserDragFlags;
typedef SInt16 DataBrowserTrackingResult;
enum {
  kDataBrowserContentHit        = 1,
  kDataBrowserNothingHit        = 0,
  kDataBrowserStopTracking      = -1
};

typedef CALLBACK_API( void , DataBrowserDrawItemProcPtr )(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, DataBrowserItemState itemState, const Rect *theRect, SInt16 gdDepth, Boolean colorDevice);
typedef CALLBACK_API( Boolean , DataBrowserEditItemProcPtr )(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, CFStringRef theString, Rect *maxEditTextRect, Boolean *shrinkToFit);
typedef CALLBACK_API( Boolean , DataBrowserHitTestProcPtr )(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, const Rect *theRect, const Rect *mouseRect);
typedef CALLBACK_API( DataBrowserTrackingResult , DataBrowserTrackingProcPtr )(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, const Rect *theRect, Point startPt, EventModifiers modifiers);
typedef CALLBACK_API( void , DataBrowserItemDragRgnProcPtr )(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, const Rect *theRect, RgnHandle dragRgn);
typedef CALLBACK_API( DataBrowserDragFlags , DataBrowserItemAcceptDragProcPtr )(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, const Rect *theRect, DragReference theDrag);
typedef CALLBACK_API( Boolean , DataBrowserItemReceiveDragProcPtr )(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, DataBrowserDragFlags dragFlags, DragReference theDrag);
typedef STACK_UPP_TYPE(DataBrowserDrawItemProcPtr)              DataBrowserDrawItemUPP;
typedef STACK_UPP_TYPE(DataBrowserEditItemProcPtr)              DataBrowserEditItemUPP;
typedef STACK_UPP_TYPE(DataBrowserHitTestProcPtr)               DataBrowserHitTestUPP;
typedef STACK_UPP_TYPE(DataBrowserTrackingProcPtr)              DataBrowserTrackingUPP;
typedef STACK_UPP_TYPE(DataBrowserItemDragRgnProcPtr)           DataBrowserItemDragRgnUPP;
typedef STACK_UPP_TYPE(DataBrowserItemAcceptDragProcPtr)        DataBrowserItemAcceptDragUPP;
typedef STACK_UPP_TYPE(DataBrowserItemReceiveDragProcPtr)       DataBrowserItemReceiveDragUPP;
/*
 *  NewDataBrowserDrawItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserDrawItemUPP
NewDataBrowserDrawItemUPP(DataBrowserDrawItemProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  NewDataBrowserEditItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserEditItemUPP
NewDataBrowserEditItemUPP(DataBrowserEditItemProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  NewDataBrowserHitTestUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserHitTestUPP
NewDataBrowserHitTestUPP(DataBrowserHitTestProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  NewDataBrowserTrackingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserTrackingUPP
NewDataBrowserTrackingUPP(DataBrowserTrackingProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  NewDataBrowserItemDragRgnUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemDragRgnUPP
NewDataBrowserItemDragRgnUPP(DataBrowserItemDragRgnProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  NewDataBrowserItemAcceptDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemAcceptDragUPP
NewDataBrowserItemAcceptDragUPP(DataBrowserItemAcceptDragProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  NewDataBrowserItemReceiveDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserItemReceiveDragUPP
NewDataBrowserItemReceiveDragUPP(DataBrowserItemReceiveDragProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  DisposeDataBrowserDrawItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserDrawItemUPP(DataBrowserDrawItemUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  DisposeDataBrowserEditItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserEditItemUPP(DataBrowserEditItemUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  DisposeDataBrowserHitTestUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserHitTestUPP(DataBrowserHitTestUPP userUPP)   AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  DisposeDataBrowserTrackingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserTrackingUPP(DataBrowserTrackingUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  DisposeDataBrowserItemDragRgnUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemDragRgnUPP(DataBrowserItemDragRgnUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  DisposeDataBrowserItemAcceptDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemAcceptDragUPP(DataBrowserItemAcceptDragUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  DisposeDataBrowserItemReceiveDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeDataBrowserItemReceiveDragUPP(DataBrowserItemReceiveDragUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  InvokeDataBrowserDrawItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserDrawItemUPP(
  ControlRef              browser,
  DataBrowserItemID       item,
  DataBrowserPropertyID   property,
  DataBrowserItemState    itemState,
  const Rect *            theRect,
  SInt16                  gdDepth,
  Boolean                 colorDevice,
  DataBrowserDrawItemUPP  userUPP)                            AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  InvokeDataBrowserEditItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean
InvokeDataBrowserEditItemUPP(
  ControlRef              browser,
  DataBrowserItemID       item,
  DataBrowserPropertyID   property,
  CFStringRef             theString,
  Rect *                  maxEditTextRect,
  Boolean *               shrinkToFit,
  DataBrowserEditItemUPP  userUPP)                            AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  InvokeDataBrowserHitTestUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean
InvokeDataBrowserHitTestUPP(
  ControlRef             browser,
  DataBrowserItemID      itemID,
  DataBrowserPropertyID  property,
  const Rect *           theRect,
  const Rect *           mouseRect,
  DataBrowserHitTestUPP  userUPP)                             AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  InvokeDataBrowserTrackingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserTrackingResult
InvokeDataBrowserTrackingUPP(
  ControlRef              browser,
  DataBrowserItemID       itemID,
  DataBrowserPropertyID   property,
  const Rect *            theRect,
  Point                   startPt,
  EventModifiers          modifiers,
  DataBrowserTrackingUPP  userUPP)                            AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  InvokeDataBrowserItemDragRgnUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void
InvokeDataBrowserItemDragRgnUPP(
  ControlRef                 browser,
  DataBrowserItemID          itemID,
  DataBrowserPropertyID      property,
  const Rect *               theRect,
  RgnHandle                  dragRgn,
  DataBrowserItemDragRgnUPP  userUPP)                         AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  InvokeDataBrowserItemAcceptDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern DataBrowserDragFlags
InvokeDataBrowserItemAcceptDragUPP(
  ControlRef                    browser,
  DataBrowserItemID             itemID,
  DataBrowserPropertyID         property,
  const Rect *                  theRect,
  DragReference                 theDrag,
  DataBrowserItemAcceptDragUPP  userUPP)                      AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  InvokeDataBrowserItemReceiveDragUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean
InvokeDataBrowserItemReceiveDragUPP(
  ControlRef                     browser,
  DataBrowserItemID              itemID,
  DataBrowserPropertyID          property,
  DataBrowserDragFlags           dragFlags,
  DragReference                  theDrag,
  DataBrowserItemReceiveDragUPP  userUPP)                     AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline DataBrowserDrawItemUPP                               NewDataBrowserDrawItemUPP(DataBrowserDrawItemProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserEditItemUPP                               NewDataBrowserEditItemUPP(DataBrowserEditItemProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserHitTestUPP                                NewDataBrowserHitTestUPP(DataBrowserHitTestProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserTrackingUPP                               NewDataBrowserTrackingUPP(DataBrowserTrackingProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserItemDragRgnUPP                            NewDataBrowserItemDragRgnUPP(DataBrowserItemDragRgnProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserItemAcceptDragUPP                         NewDataBrowserItemAcceptDragUPP(DataBrowserItemAcceptDragProcPtr userRoutine) { return userRoutine; }
    inline DataBrowserItemReceiveDragUPP                        NewDataBrowserItemReceiveDragUPP(DataBrowserItemReceiveDragProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeDataBrowserDrawItemUPP(DataBrowserDrawItemUPP) { }
    inline void                                                 DisposeDataBrowserEditItemUPP(DataBrowserEditItemUPP) { }
    inline void                                                 DisposeDataBrowserHitTestUPP(DataBrowserHitTestUPP) { }
    inline void                                                 DisposeDataBrowserTrackingUPP(DataBrowserTrackingUPP) { }
    inline void                                                 DisposeDataBrowserItemDragRgnUPP(DataBrowserItemDragRgnUPP) { }
    inline void                                                 DisposeDataBrowserItemAcceptDragUPP(DataBrowserItemAcceptDragUPP) { }
    inline void                                                 DisposeDataBrowserItemReceiveDragUPP(DataBrowserItemReceiveDragUPP) { }
    inline void                                                 InvokeDataBrowserDrawItemUPP(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, DataBrowserItemState itemState, const Rect * theRect, SInt16 gdDepth, Boolean colorDevice, DataBrowserDrawItemUPP userUPP) { (*userUPP)(browser, item, property, itemState, theRect, gdDepth, colorDevice); }
    inline Boolean                                              InvokeDataBrowserEditItemUPP(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, CFStringRef theString, Rect * maxEditTextRect, Boolean * shrinkToFit, DataBrowserEditItemUPP userUPP) { return (*userUPP)(browser, item, property, theString, maxEditTextRect, shrinkToFit); }
    inline Boolean                                              InvokeDataBrowserHitTestUPP(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, const Rect * theRect, const Rect * mouseRect, DataBrowserHitTestUPP userUPP) { return (*userUPP)(browser, itemID, property, theRect, mouseRect); }
    inline DataBrowserTrackingResult                            InvokeDataBrowserTrackingUPP(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, const Rect * theRect, Point startPt, EventModifiers modifiers, DataBrowserTrackingUPP userUPP) { return (*userUPP)(browser, itemID, property, theRect, startPt, modifiers); }
    inline void                                                 InvokeDataBrowserItemDragRgnUPP(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, const Rect * theRect, RgnHandle dragRgn, DataBrowserItemDragRgnUPP userUPP) { (*userUPP)(browser, itemID, property, theRect, dragRgn); }
    inline DataBrowserDragFlags                                 InvokeDataBrowserItemAcceptDragUPP(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, const Rect * theRect, DragReference theDrag, DataBrowserItemAcceptDragUPP userUPP) { return (*userUPP)(browser, itemID, property, theRect, theDrag); }
    inline Boolean                                              InvokeDataBrowserItemReceiveDragUPP(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, DataBrowserDragFlags dragFlags, DragReference theDrag, DataBrowserItemReceiveDragUPP userUPP) { return (*userUPP)(browser, itemID, property, dragFlags, theDrag); }
  #else
    #define NewDataBrowserDrawItemUPP(userRoutine)              ((DataBrowserDrawItemUPP)userRoutine)
    #define NewDataBrowserEditItemUPP(userRoutine)              ((DataBrowserEditItemUPP)userRoutine)
    #define NewDataBrowserHitTestUPP(userRoutine)               ((DataBrowserHitTestUPP)userRoutine)
    #define NewDataBrowserTrackingUPP(userRoutine)              ((DataBrowserTrackingUPP)userRoutine)
    #define NewDataBrowserItemDragRgnUPP(userRoutine)           ((DataBrowserItemDragRgnUPP)userRoutine)
    #define NewDataBrowserItemAcceptDragUPP(userRoutine)        ((DataBrowserItemAcceptDragUPP)userRoutine)
    #define NewDataBrowserItemReceiveDragUPP(userRoutine)       ((DataBrowserItemReceiveDragUPP)userRoutine)
    #define DisposeDataBrowserDrawItemUPP(userUPP)
    #define DisposeDataBrowserEditItemUPP(userUPP)
    #define DisposeDataBrowserHitTestUPP(userUPP)
    #define DisposeDataBrowserTrackingUPP(userUPP)
    #define DisposeDataBrowserItemDragRgnUPP(userUPP)
    #define DisposeDataBrowserItemAcceptDragUPP(userUPP)
    #define DisposeDataBrowserItemReceiveDragUPP(userUPP)
    #define InvokeDataBrowserDrawItemUPP(browser, item, property, itemState, theRect, gdDepth, colorDevice, userUPP) (*userUPP)(browser, item, property, itemState, theRect, gdDepth, colorDevice)
    #define InvokeDataBrowserEditItemUPP(browser, item, property, theString, maxEditTextRect, shrinkToFit, userUPP) (*userUPP)(browser, item, property, theString, maxEditTextRect, shrinkToFit)
    #define InvokeDataBrowserHitTestUPP(browser, itemID, property, theRect, mouseRect, userUPP) (*userUPP)(browser, itemID, property, theRect, mouseRect)
    #define InvokeDataBrowserTrackingUPP(browser, itemID, property, theRect, startPt, modifiers, userUPP) (*userUPP)(browser, itemID, property, theRect, startPt, modifiers)
    #define InvokeDataBrowserItemDragRgnUPP(browser, itemID, property, theRect, dragRgn, userUPP) (*userUPP)(browser, itemID, property, theRect, dragRgn)
    #define InvokeDataBrowserItemAcceptDragUPP(browser, itemID, property, theRect, theDrag, userUPP) (*userUPP)(browser, itemID, property, theRect, theDrag)
    #define InvokeDataBrowserItemReceiveDragUPP(browser, itemID, property, dragFlags, theDrag, userUPP) (*userUPP)(browser, itemID, property, dragFlags, theDrag)
  #endif
#endif

/* Custom Callback (vtable) Structure */
enum {
  kDataBrowserLatestCustomCallbacks = 0
};

struct DataBrowserCustomCallbacks {

  UInt32              version;                /* Use kDataBrowserLatestCustomCallbacks */

  union {
    struct {
      DataBrowserDrawItemUPP  drawItemCallback;
      DataBrowserEditItemUPP  editTextCallback;
      DataBrowserHitTestUPP  hitTestCallback;
      DataBrowserTrackingUPP  trackingCallback;

      DataBrowserItemDragRgnUPP  dragRegionCallback;
      DataBrowserItemAcceptDragUPP  acceptDragCallback;
      DataBrowserItemReceiveDragUPP  receiveDragCallback;
    }                       v1;
  }                       u;
};
typedef struct DataBrowserCustomCallbacks DataBrowserCustomCallbacks;
#if !__LP64__
/*
 *  InitDataBrowserCustomCallbacks()
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
InitDataBrowserCustomCallbacks(DataBrowserCustomCallbacks * callbacks) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Macro for initializing custom callback structure */
#define InitializeDataBrowserCustomCallbacks(callbacks, vers) \
{ (callbacks)->version = (vers); InitDataBrowserCustomCallbacks(callbacks); }

/*
 *  GetDataBrowserCustomCallbacks()
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
GetDataBrowserCustomCallbacks(
  ControlRef                    browser,
  DataBrowserCustomCallbacks *  callbacks)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserCustomCallbacks()
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
SetDataBrowserCustomCallbacks(
  ControlRef                          browser,
  const DataBrowserCustomCallbacks *  callbacks)              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;





/* TableView Formatting */
#endif  /* !__LP64__ */

typedef UInt32 DataBrowserTableViewHiliteStyle;
enum {
  kDataBrowserTableViewMinimalHilite = 0,
  kDataBrowserTableViewFillHilite = 1
};

typedef UInt32 DataBrowserTableViewPropertyFlags;
enum {
                                        /* kDataBrowserTableView DataBrowserPropertyFlags */
  kDataBrowserTableViewSelectionColumn = 1 << kDataBrowserViewSpecificFlagsOffset
};

/* The row and column indices are zero-based */

typedef unsigned long                   DataBrowserTableViewRowIndex;
typedef unsigned long                   DataBrowserTableViewColumnIndex;
typedef DataBrowserPropertyID           DataBrowserTableViewColumnID;
typedef DataBrowserPropertyDesc         DataBrowserTableViewColumnDesc;
/* TableView API */
/* Use when setting column position */
enum {
  kDataBrowserTableViewLastColumn = -1
};

#if !__LP64__
/*
 *  RemoveDataBrowserTableViewColumn()
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
RemoveDataBrowserTableViewColumn(
  ControlRef                     browser,
  DataBrowserTableViewColumnID   column)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewColumnCount()
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
GetDataBrowserTableViewColumnCount(
  ControlRef   browser,
  UInt32 *     numColumns)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetDataBrowserTableViewHiliteStyle()
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
SetDataBrowserTableViewHiliteStyle(
  ControlRef                        browser,
  DataBrowserTableViewHiliteStyle   hiliteStyle)              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewHiliteStyle()
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
GetDataBrowserTableViewHiliteStyle(
  ControlRef                         browser,
  DataBrowserTableViewHiliteStyle *  hiliteStyle)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetDataBrowserTableViewRowHeight()
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
SetDataBrowserTableViewRowHeight(
  ControlRef   browser,
  UInt16       height)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewRowHeight()
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
GetDataBrowserTableViewRowHeight(
  ControlRef   browser,
  UInt16 *     height)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserTableViewColumnWidth()
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
SetDataBrowserTableViewColumnWidth(
  ControlRef   browser,
  UInt16       width)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewColumnWidth()
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
GetDataBrowserTableViewColumnWidth(
  ControlRef   browser,
  UInt16 *     width)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserTableViewItemRowHeight()
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
SetDataBrowserTableViewItemRowHeight(
  ControlRef          browser,
  DataBrowserItemID   item,
  UInt16              height)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewItemRowHeight()
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
GetDataBrowserTableViewItemRowHeight(
  ControlRef          browser,
  DataBrowserItemID   item,
  UInt16 *            height)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserTableViewNamedColumnWidth()
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
SetDataBrowserTableViewNamedColumnWidth(
  ControlRef                     browser,
  DataBrowserTableViewColumnID   column,
  UInt16                         width)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewNamedColumnWidth()
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
GetDataBrowserTableViewNamedColumnWidth(
  ControlRef                     browser,
  DataBrowserTableViewColumnID   column,
  UInt16 *                       width)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserTableViewGeometry()
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
SetDataBrowserTableViewGeometry(
  ControlRef   browser,
  Boolean      variableWidthColumns,
  Boolean      variableHeightRows)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewGeometry()
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
GetDataBrowserTableViewGeometry(
  ControlRef   browser,
  Boolean *    variableWidthColumns,
  Boolean *    variableHeightRows)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetDataBrowserTableViewItemID()
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
GetDataBrowserTableViewItemID(
  ControlRef                     browser,
  DataBrowserTableViewRowIndex   row,
  DataBrowserItemID *            item)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserTableViewItemRow()
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
SetDataBrowserTableViewItemRow(
  ControlRef                     browser,
  DataBrowserItemID              item,
  DataBrowserTableViewRowIndex   row)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewItemRow()
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
GetDataBrowserTableViewItemRow(
  ControlRef                      browser,
  DataBrowserItemID               item,
  DataBrowserTableViewRowIndex *  row)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserTableViewColumnPosition()
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
SetDataBrowserTableViewColumnPosition(
  ControlRef                        browser,
  DataBrowserTableViewColumnID      column,
  DataBrowserTableViewColumnIndex   position)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewColumnPosition()
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
GetDataBrowserTableViewColumnPosition(
  ControlRef                         browser,
  DataBrowserTableViewColumnID       column,
  DataBrowserTableViewColumnIndex *  position)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserTableViewColumnProperty()
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
GetDataBrowserTableViewColumnProperty(
  ControlRef                        browser,
  DataBrowserTableViewColumnIndex   column,
  DataBrowserTableViewColumnID *    property)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;





/* kDataBrowserListView Formatting */
#endif  /* !__LP64__ */


/*
 *  Discussion:
 *    DataBrowserPropertyFlags that are specific to kDataBrowserListView
 */
enum {
  kDataBrowserListViewSelectionColumn = kDataBrowserTableViewSelectionColumn,
  kDataBrowserListViewMovableColumn = 1 << (kDataBrowserViewSpecificFlagsOffset + 1),
  kDataBrowserListViewSortableColumn = 1 << (kDataBrowserViewSpecificFlagsOffset + 2),

  /*
   * kDataBrowserListViewTypeSelectColumn marks a column as
   * type-selectable. If one or more of your list view columns are
   * marked as type-selectable, Data Browser will do type-selection for
   * you automatically. Data Browser applies the typing to the first
   * column (in the system direction) with this property flag. This
   * flag only intended for use with columns of type
   * kDataBrowserTextType, kDataBrowserIconAndTextType, and
   * kDataBrowserDateTimeType; if you set it for a column of another
   * type, the type-selection behavior is undefined. Turning on this
   * flag also causes Data Browser to gather all keyboard input via a
   * carbon event handler instead of relying on calls to
   * HandleControlKey; therefore, you will never see these keyboard
   * events come out of WaitNextEvent. Only available on 10.3 and later.
   */
  kDataBrowserListViewTypeSelectColumn = 1 << (kDataBrowserViewSpecificFlagsOffset + 3),

  /*
   * Normally the text in a header button for a column of type
   * kDataBrowserIconAndTextType is aligned as though it has an icon
   * next to it even if no icon is specified for the header button; in
   * other words, space is reserved for an icon in the header button
   * even if no icon is displayed. However, this flag indicates that
   * space should not be reserved for an icon if no icon is provided
   * for the header button. This flag allows a client to justify the
   * left edge of the text in a header button to the left edge of the
   * icon in the cells beneath it. Available on 10.4 and later.
   */
  kDataBrowserListViewNoGapForIconInHeaderButton = 1 << (kDataBrowserViewSpecificFlagsOffset + 4),
  kDataBrowserListViewDefaultColumnFlags = kDataBrowserListViewMovableColumn + kDataBrowserListViewSortableColumn
};

typedef DataBrowserPropertyFlags        DataBrowserListViewPropertyFlags;
enum {
  kDataBrowserListViewLatestHeaderDesc = 0
};

struct DataBrowserListViewHeaderDesc {
  UInt32              version;                /* Use kDataBrowserListViewLatestHeaderDesc */

  UInt16              minimumWidth;
  UInt16              maximumWidth;

  SInt16              titleOffset;
  CFStringRef         titleString;
  DataBrowserSortOrder  initialOrder;
  ControlFontStyleRec  btnFontStyle;
  ControlButtonContentInfo  btnContentInfo;
};
typedef struct DataBrowserListViewHeaderDesc DataBrowserListViewHeaderDesc;
struct DataBrowserListViewColumnDesc {
  DataBrowserTableViewColumnDesc  propertyDesc;
  DataBrowserListViewHeaderDesc  headerBtnDesc;
};
typedef struct DataBrowserListViewColumnDesc DataBrowserListViewColumnDesc;
/* DataBrowserListView API */
enum {
  kDataBrowserListViewAppendColumn = kDataBrowserTableViewLastColumn
};

#if !__LP64__
/*
 *  AutoSizeDataBrowserListViewColumns()
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
AutoSizeDataBrowserListViewColumns(ControlRef browser)        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  AddDataBrowserListViewColumn()
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
AddDataBrowserListViewColumn(
  ControlRef                        browser,
  DataBrowserListViewColumnDesc *   columnDesc,
  DataBrowserTableViewColumnIndex   position)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserListViewHeaderDesc()
 *  
 *  Summary:
 *    Returns information about a specified column header in a list
 *    view.
 *  
 *  Discussion:
 *    Note that this API does not correctly use CoreFoundation naming
 *    conventions. Although the API name begins with "Get", implying
 *    that you do not need to release the CFStringRef and IconRef
 *    returned by this API, in fact you do actually need to release
 *    these objects.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    browser:
 *      The data browser for which you need header information.
 *    
 *    column:
 *      The column ID for which you need header information.
 *    
 *    desc:
 *      On exit, contains header information for the specified column.
 *      You must release the CFStringRef and IconRef contained in this
 *      structure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetDataBrowserListViewHeaderDesc(
  ControlRef                       browser,
  DataBrowserTableViewColumnID     column,
  DataBrowserListViewHeaderDesc *  desc)                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  SetDataBrowserListViewHeaderDesc()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetDataBrowserListViewHeaderDesc(
  ControlRef                       browser,
  DataBrowserTableViewColumnID     column,
  DataBrowserListViewHeaderDesc *  desc)                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  SetDataBrowserListViewHeaderBtnHeight()
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
SetDataBrowserListViewHeaderBtnHeight(
  ControlRef   browser,
  UInt16       height)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserListViewHeaderBtnHeight()
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
GetDataBrowserListViewHeaderBtnHeight(
  ControlRef   browser,
  UInt16 *     height)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserListViewUsePlainBackground()
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
SetDataBrowserListViewUsePlainBackground(
  ControlRef   browser,
  Boolean      usePlainBackground)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserListViewUsePlainBackground()
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
GetDataBrowserListViewUsePlainBackground(
  ControlRef   browser,
  Boolean *    usePlainBackground)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserListViewDisclosureColumn()
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
SetDataBrowserListViewDisclosureColumn(
  ControlRef                     browser,
  DataBrowserTableViewColumnID   column,
  Boolean                        expandableRows)              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserListViewDisclosureColumn()
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
GetDataBrowserListViewDisclosureColumn(
  ControlRef                      browser,
  DataBrowserTableViewColumnID *  column,
  Boolean *                       expandableRows)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* DataBrowserColumnView API */
/*
 *  GetDataBrowserColumnViewPath()
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
GetDataBrowserColumnViewPath(
  ControlRef   browser,
  Handle       path)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserColumnViewPathLength()
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
GetDataBrowserColumnViewPathLength(
  ControlRef   browser,
  UInt32 *     pathLength)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserColumnViewPath()
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
SetDataBrowserColumnViewPath(
  ControlRef                 browser,
  UInt32                     length,
  const DataBrowserItemID *  path)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDataBrowserColumnViewDisplayType()
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
SetDataBrowserColumnViewDisplayType(
  ControlRef                browser,
  DataBrowserPropertyType   propertyType)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDataBrowserColumnViewDisplayType()
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
GetDataBrowserColumnViewDisplayType(
  ControlRef                 browser,
  DataBrowserPropertyType *  propertyType)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* DataBrowser UPP macros */
/*
    Customizing Data Browser Accessibility Information
    
    Warning: The following assumes you already understand how to handle the
    Accessibility Carbon Events described in CarbonEvents.h.
    
    Data Browser automatically handles the various Accessibility Carbon
    Events to provide a large amount of Accessibility information. However,
    your application may need to override or augment the default information
    that Data Browser provides.
    
    Though it is already possible for your application to install various
    Accessibility Carbon Event handlers on a Data Browser instance, it is
    impossible to interpret the AXUIElementRefs contained in the events
    without the help of the Data Browser. A given AXUIElementRef that is
    passed to Data Browser list view in an Accessibility Carbon Event could
    represent a row, a cell, or the list view as a whole. If your
    application needs to add an attribute to only the rows in a list view,
    your application will need to ask Data Browser what any given
    AXUIElementRef represents. The AXUIElementGetDataBrowserItemInfo allows
    your application to ask that question.
    
    Additionally, your application may want to generate its own AXUIElementRefs
    that represent children of or point to various rows or cells of a Data Browser
    instance. The AXUIElementCreateWithDataBrowserAndItemInfo API allows your
    application to manufacture AXUIElementRefs that represent certain parts of a
    Data Browser so you can provide them in your Accessibility Carbon Event
    handlers.
    
    Typical Usage Scenario: You want to add an Accessibility attribute to
    all rows in a Data Browser list view.
    
        Step 1: Install the appropriate Accessibility Carbon Event handlers
        on your Data Browser instance. Call InstallEventHandler or a similar
        API to install a handler onto your Data Browser ControlRef for the
        kEventAccessibleGetAllAttributeNames,
        kEventAccessibleGetNamedAttribute, and other appropriate events.
        
        Step 2: Your handler should find out what part of the Data Browser
        is being asked for its accessibility information. Extract the
        kEventParamAccessibleObject parameter out of the Carbon Event and
        pass it to AXUIElementGetDataBrowserItemInfo. See that API
        description for more usage information and calling requirements.
        Examine the DataBrowserAccessibilityItemInfo structure that is
        filled out to determine whether it represents the part of the Data
        Browser you are interested in adding an attribute to. In this case,
        you are looking for a row, so you would make sure the item field is
        not kDataBrowserNoItem, and that the columnProperty is
        kDataBrowserItemNoProperty.
        
        Step 3: Your event handler should call CallNextEventHandler to allow
        the Data Browser to do the default handling of the event. This is
        particularly important if the AXUIElementRef did not represent a
        row, since you don't want to disrupt the Data Browser's handling of
        the event for parts other than rows.
        
        Step 4: If you determined that the part was a row in step 2, your
        handler should now do whatever custom work it deems necessary. For
        the kEventAccessibleGetAllAttributeNames, your handler would extract
        the kEventParamAccessibleAttributeNames parameter out of the event
        and add your custom attribute name to the array. For the
        kEventAccessibleGetNamedAttribute event, your handler would test the
        kEventParamAccessibleAttributeName parameter to see if it matches
        your custom attribute name; if so, your handler would put its custom
        data in the kEventParamAccessibleAttributeValue parameter. Any other
        events would be handled similarly.
        
        Step 5: Your event handler should return an appropriate result code.
        In cases where the AXUIElementRef does not represent a row or when
        the attribute name is not your custom attribute, your handler can
        return the same result code that was returned by
        CallNextEventHandler in step 3. In cases where your handler decided
        to augment or override the default handling of the event, your
        handler will typically want to return noErr. See the Carbon Event
        documentation for more details on the meanings of result codes
        returned by event handlers.
*/
#endif  /* !__LP64__ */


/*
 *  DataBrowserAccessibilityItemInfoV0
 *  
 *  Summary:
 *    A specific description of Data Browser accessibility item
 *    information.
 *  
 *  Discussion:
 *    If you fill this structure as part of a
 *    DataBrowserAccessibilityItemInfo, you must set the
 *    DataBrowserAccessibilityItemInfo's version field to zero.
 */
struct DataBrowserAccessibilityItemInfoV0 {

  /*
   * The DataBrowserItemID of the container the AXUIElementRef
   * represents or lives within. Even kDataBrowserNoItem might be
   * meaningful, since it is the root container ID if you haven't
   * overridden it via SetDataBrowserTarget. In list view, the
   * container helps narrow down the AXUIElementRef to either a
   * disclosed child of another row, or the list as a whole. In column
   * view, the container helps narrow down the AXUIElementRef to a
   * column; also see the columnProperty description below.
   */
  DataBrowserItemID   container;

  /*
   * The DataBrowserItemID of the item the AXUIElementRef represents or
   * lives within. If item is kDataBrowserNoItem, the AXUIElementRef
   * represents just the container. In list view, the item helps narrow
   * down the AXUIElementRef to either a row, or the root container as
   * a whole. In column view, the item helps narrow down the
   * AXUIElementRef to either a cell, or a column as a whole; also see
   * the columnProperty description below.
   */
  DataBrowserItemID   item;

  /*
   * The DataBrowserPropertyID of the column the AXUIElementRef
   * represents or lives within. If columnProperty is
   * kDataBrowserItemNoProperty and item is not kDataBrowserNoItem, the
   * AXUIElementRef represents a whole row. In list view, this field
   * helps narrow down the AXUIElementRef to either a cell, or a row as
   * a whole. In column view, the columnProperty will/must always be
   * set to kDataBrowserItemNoProperty unless the AXUIElementRef
   * represents the preview column. When the AXUIElementRef represents
   * the preview column, the columnProperty will/must always be set to
   * kDataBrowserColumnViewPreviewProperty, and the other fields of
   * this structure will/must be set to zero or the equivalent constant.
   */
  DataBrowserPropertyID  columnProperty;

  /*
   * The DataBrowserPropertyPart of the sub-cell part the
   * AXUIElementRef represents. Examples include the disclosure
   * triangle in a cell, the text in a cell, and the check box in a
   * cell. If propertyPart is kDataBrowserPropertyEnclosingPart and
   * columnProperty is not kDataBrowserItemNoProperty, the
   * AXUIElementRef represents the cell as a whole. In both list view
   * and column view, this field helps narrow down the AXUIElementRef
   * to either a sub-cell part, or a cell as a whole. For column view,
   * also see the columnProperty description above.
   */
  DataBrowserPropertyPart  propertyPart;
};
typedef struct DataBrowserAccessibilityItemInfoV0 DataBrowserAccessibilityItemInfoV0;

/*
 *  DataBrowserAccessibilityItemInfoV1
 *  
 *  Summary:
 *    A specific description of Data Browser accessibility item
 *    information.
 *  
 *  Discussion:
 *    If you fill this structure as part of a
 *    DataBrowserAccessibilityItemInfo, you must set the
 *    DataBrowserAccessibilityItemInfo's version field to one. 
 *     
 *    This structure is identical to the V0 structure except for the
 *    inclusion of row and column indices. These indices may be useful
 *    to clients who call AXUIElementGetDataBrowserItemInfo. 
 *    If your Data Browser instance allows a given item and/or
 *    container to be displayed more than once at a given point in
 *    time, you can use the row and column indices to differentiate the
 *    particular visual occurances of that item when calling
 *    AXUIElementCreateWithDataBrowserAndItemInfo. See the additional
 *    details in the rowIndex and columnIndex discussions below.
 */
struct DataBrowserAccessibilityItemInfoV1 {

  /*
   * The DataBrowserItemID of the container the AXUIElementRef
   * represents or lives within. Even kDataBrowserNoItem might be
   * meaningful, since it is the root container ID if you haven't
   * overridden it via SetDataBrowserTarget. In list view, the
   * container helps narrow down the AXUIElementRef to either a
   * disclosed child of another row, or the list as a whole. In column
   * view, the container helps narrow down the AXUIElementRef to a
   * column; also see the columnProperty description below.
   */
  DataBrowserItemID   container;

  /*
   * The DataBrowserItemID of the item the AXUIElementRef represents or
   * lives within. If item is kDataBrowserNoItem, the AXUIElementRef
   * represents just the container. In list view, the item helps narrow
   * down the AXUIElementRef to either a row, or the root container as
   * a whole. In column view, the item helps narrow down the
   * AXUIElementRef to either a cell, or a column as a whole; also see
   * the columnProperty description below.
   */
  DataBrowserItemID   item;

  /*
   * The DataBrowserPropertyID of the column the AXUIElementRef
   * represents or lives within. If columnProperty is
   * kDataBrowserItemNoProperty and item is not kDataBrowserNoItem, the
   * AXUIElementRef represents a whole row. In list view, this field
   * helps narrow down the AXUIElementRef to either a cell, or a row as
   * a whole. In column view, the columnProperty will/must always be
   * set to kDataBrowserItemNoProperty unless the AXUIElementRef
   * represents the preview column. When the AXUIElementRef represents
   * the preview column, the columnProperty will/must always be set to
   * kDataBrowserColumnViewPreviewProperty, and the other fields of
   * this structure will/must be set to zero or the equivalent constant.
   */
  DataBrowserPropertyID  columnProperty;

  /*
   * The DataBrowserPropertyPart of the sub-cell part the
   * AXUIElementRef represents. Examples include the disclosure
   * triangle in a cell, the text in a cell, and the check box in a
   * cell. If propertyPart is kDataBrowserPropertyEnclosingPart and
   * columnProperty is not kDataBrowserItemNoProperty, the
   * AXUIElementRef represents the cell as a whole. In both list view
   * and column view, this field helps narrow down the AXUIElementRef
   * to either a sub-cell part, or a cell as a whole. For column view,
   * also see the columnProperty description above.
   */
  DataBrowserPropertyPart  propertyPart;

  /*
   * The zero-based DataBrowserTableViewRowIndex of the row specified
   * by the other parts of this structure. If the other parts of this
   * structure do not specify a row or a part thereof, this field
   * will/must be set to zero; because this field is zero based, you
   * must test the other parts this structure to see whether this field
   * is meaningful. In list view, when the other parts of this
   * structure specify an item or part thereof, this field will/must be
   * set to the row index at which the specified item can be found. In
   * column view, when the other parts of this structure specify a cell
   * or part thereof, this field will/must be set to the row index at
   * which the specified cell can be found.
   */
  DataBrowserTableViewRowIndex  rowIndex;

  /*
   * The zero-based DataBrowserTableViewColumnIndex of the column
   * specified by the other parts of this structure. If the other parts
   * of this structure do not specify a column or a part thereof, this
   * field will/must be set to zero; because this field is zero based,
   * you must test the other parts this structure to see whether this
   * field is meaningful. In list view, when the other parts of this
   * structure specify a cell or part thereof, this field will/must be
   * set to the column index at which the specified cell can be found.
   * In column view, when the other parts of this structure specify a
   * column or part thereof, this field will/must be set to the column
   * index at which the specified cell can be found.
   */
  DataBrowserTableViewColumnIndex  columnIndex;
};
typedef struct DataBrowserAccessibilityItemInfoV1 DataBrowserAccessibilityItemInfoV1;

/*
 *  DataBrowserAccessibilityItemInfo
 *  
 *  Summary:
 *    A generalized description of Data Browser accessibility item
 *    information.
 *  
 *  Discussion:
 *    Pass this structure to AXUIElementGetDataBrowserItemInfo or
 *    AXUIElementCreateWithDataBrowserAndItemInfo.
 */
struct DataBrowserAccessibilityItemInfo {

  /*
   * A UInt32 which identifies how to interpret the following union.
   * Set this field to zero if you fill out the union's data in the
   * form of a DataBrowserAccessibilityItemInfoV0 structure. Set this
   * field to one if you fill out the union's data in the form of a
   * DataBrowserAccessibilityItemInfoV1 structure.
   */
  UInt32              version;
  union {
    DataBrowserAccessibilityItemInfoV0  v0;
    DataBrowserAccessibilityItemInfoV1  v1;
  }                       u;
};
typedef struct DataBrowserAccessibilityItemInfo DataBrowserAccessibilityItemInfo;
#if !__LP64__
/*
 *  AXUIElementGetDataBrowserItemInfo()
 *  
 *  Summary:
 *    Gets a description of the part of a Data Browser represented by a
 *    given AXUIElementRef.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inElement:
 *      An AXUIElementRef representing part of a Data Browser.
 *    
 *    inDataBrowser:
 *      A Data Browser ControlRef.
 *    
 *    inDesiredInfoVersion:
 *      A UInt32 indicating the the version you want the ioInfo
 *      structure passed back as.
 *    
 *    outInfo:
 *      A DataBrowserAccessibilityItemInfo that will be filled in with
 *      a description of the part of the Data Browser that the
 *      AXUIElementRef represents.
 *  
 *  Result:
 *    An OSStatus result code. The function will return noErr if it was
 *    able to generate a description of the AXUIElementRef. If the
 *    AXUIElementRef does not represent the Data Browser you passed in,
 *    the function will return paramErr. If the AXUIElementRef
 *    represents some non-item part of the Data Browser, the function
 *    will return errDataBrowserItemNotFound.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AXUIElementGetDataBrowserItemInfo(
  AXUIElementRef                      inElement,
  ControlRef                          inDataBrowser,
  UInt32                              inDesiredInfoVersion,
  DataBrowserAccessibilityItemInfo *  outInfo)                AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;



/*
 *  AXUIElementCreateWithDataBrowserAndItemInfo()
 *  
 *  Summary:
 *    Creates an AXUIElementRef to represent some part of a Data
 *    Browser accessibility hierarchy.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDataBrowser:
 *      A Data Browser ControlRef.
 *    
 *    inInfo:
 *      A DataBrowserAccessibilityItemInfo describing the part of the
 *      Data Browser for which you want to create an AXUIElementRef.
 *  
 *  Result:
 *    An AXUIElementRef representing the part, or NULL if one cannot be
 *    created to represent the part you specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern AXUIElementRef 
AXUIElementCreateWithDataBrowserAndItemInfo(
  ControlRef                                inDataBrowser,
  const DataBrowserAccessibilityItemInfo *  inInfo)           AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


#endif  /* !__LP64__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __HIDATABROWSER__ */

