/*
     File:       HIDataBrowser.r
 
     Contains:   API and type definitions related to Data Browser.
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HIDATABROWSER_R__
#define __HIDATABROWSER_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlKindDataBrowser 		'datb'
#define kDataBrowserNoView 				'????'				/*  Error State  */
#define kDataBrowserListView 			'lstv'
#define kDataBrowserColumnView 			'clmv'

#define kDataBrowserDragSelect 			0x01				/*  Å ListMgr lNoRect  */
#define kDataBrowserSelectOnlyOne 		0x02				/*  Å ListMgr lOnlyOne  */
#define kDataBrowserResetSelection 		0x04				/*  Å ListMgr lNoExtend  */
#define kDataBrowserCmdTogglesSelection  0x08				/*  Å ListMgr lUseSense  */
#define kDataBrowserNoDisjointSelection  0x10				/*  Å ListMgr lNoDisjoint  */
#define kDataBrowserAlwaysExtendSelection  0x20				/*  Å ListMgr lExtendDrag  */
#define kDataBrowserNeverEmptySelectionSet  0x40			/*  Å ListMgr lNoNilHilite  */

#define kDataBrowserOrderUndefined 		0					/*  Not currently supported  */
#define kDataBrowserOrderIncreasing 	1
#define kDataBrowserOrderDecreasing 	2

															/*  == Corresponding data type or structure ==  */
#define kDataBrowserCustomType 			'????'				/*  No associated data, custom callbacks used  */
#define kDataBrowserIconType 			'icnr'				/*  IconRef, IconTransformType, RGBColor  */
#define kDataBrowserTextType 			'text'				/*  CFStringRef  */
#define kDataBrowserDateTimeType 		'date'				/*  DateTime or LongDateTime  */
#define kDataBrowserSliderType 			'sldr'				/*  Min, Max, Value  */
#define kDataBrowserCheckboxType 		'chbx'				/*  ThemeButtonValue  */
#define kDataBrowserProgressBarType 	'prog'				/*  Min, Max, Value  */
#define kDataBrowserRelevanceRankType 	'rank'				/*  Min, Max, Value  */
#define kDataBrowserPopupMenuType 		'menu'				/*  MenuRef, Value  */
#define kDataBrowserIconAndTextType 	'ticn'				/*  IconRef, CFStringRef, etc  */

#define kDataBrowserPropertyEnclosingPart  0
#define kDataBrowserPropertyContentPart  '----'
#define kDataBrowserPropertyDisclosurePart  'disc'
#define kDataBrowserPropertyTextPart 	'text'
#define kDataBrowserPropertyIconPart 	'icnr'
#define kDataBrowserPropertySliderPart 	'sldr'
#define kDataBrowserPropertyCheckboxPart  'chbx'
#define kDataBrowserPropertyProgressBarPart  'prog'
#define kDataBrowserPropertyRelevanceRankPart  'rank'

#define kDataBrowserUniversalPropertyFlagsMask  0xFF
#define kDataBrowserPropertyIsMutable 	0x01
#define kDataBrowserDefaultPropertyFlags  0x00
#define kDataBrowserUniversalPropertyFlags  0xFF			/*  support for an old name */
#define kDataBrowserPropertyIsEditable 	0x01				/*  support for an old name */

#define kDataBrowserPropertyFlagsOffset  8
#define kDataBrowserPropertyFlagsMask 	0xFF00
#define kDataBrowserCheckboxTriState 	0x0100				/*  kDataBrowserCheckboxType */
#define kDataBrowserDateTimeRelative 	0x0100				/*  kDataBrowserDateTimeType  */
#define kDataBrowserDateTimeDateOnly 	0x0200				/*  kDataBrowserDateTimeType  */
#define kDataBrowserDateTimeTimeOnly 	0x0400				/*  kDataBrowserDateTimeType  */
#define kDataBrowserDateTimeSecondsToo 	0x0800				/*  kDataBrowserDateTimeType  */
#define kDataBrowserSliderPlainThumb 	0x00				/*  kDataBrowserSliderType  */
#define kDataBrowserSliderUpwardThumb 	0x0100				/*  kDataBrowserSliderType  */
#define kDataBrowserSliderDownwardThumb  0x0200				/*  kDataBrowserSliderType  */
#define kDataBrowserDoNotTruncateText 	0x0300				/*  kDataBrowserTextType && kDataBrowserIconAndTextType  */
#define kDataBrowserTruncateTextAtEnd 	0x0200				/*  kDataBrowserTextType && kDataBrowserIconAndTextType  */
#define kDataBrowserTruncateTextMiddle 	0x00				/*  kDataBrowserTextType && kDataBrowserIconAndTextType  */
#define kDataBrowserTruncateTextAtStart  0x0100				/*  kDataBrowserTextType && kDataBrowserIconAndTextType  */
#define kDataBrowserPopupMenuButtonless  0x0100				/*  kDataBrowserPopupMenuType */
#define kDataBrowserPropertyModificationFlags  0xFF00		/*  support for an old name */
#define kDataBrowserRelativeDateTime 	0x0100				/*  support for an old name */

#define kDataBrowserViewSpecificFlagsOffset  16
#define kDataBrowserViewSpecificFlagsMask  0x00FF0000
#define kDataBrowserViewSpecificPropertyFlags  0x00FF0000	/*  support for an old name */

#define kDataBrowserClientPropertyFlagsOffset  24
#define kDataBrowserClientPropertyFlagsMask  0xFF000000

															/*  kDataBrowserTableView DataBrowserPropertyFlags  */
#define kDataBrowserTableViewSelectionColumn  0x00010000
#define kDataBrowserListViewSelectionColumn  0x00010000
#define kDataBrowserListViewMovableColumn  0x00020000
#define kDataBrowserListViewSortableColumn  0x00040000
#define kDataBrowserListViewTypeSelectColumn  0x00080000
#define kDataBrowserListViewNoGapForIconInHeaderButton  0x00100000
#define kDataBrowserListViewDefaultColumnFlags  0x00060000


#endif /* __HIDATABROWSER_R__ */

