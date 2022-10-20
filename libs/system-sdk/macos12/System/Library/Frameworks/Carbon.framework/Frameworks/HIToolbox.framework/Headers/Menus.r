/*
     File:       Menus.r
 
     Contains:   Menu Manager Interfaces.
 
     Copyright:  © 1985-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __MENUS_R__
#define __MENUS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kMenuStdMenuProc 				63
#define kMenuStdMenuBarProc 			63

#define kMenuNoModifiers 				0					/*  Mask for no modifiers */
#define kMenuShiftModifier 				0x01				/*  Mask for shift key modifier */
#define kMenuOptionModifier 			0x02				/*  Mask for option key modifier */
#define kMenuControlModifier 			0x04				/*  Mask for control key modifier */
#define kMenuNoCommandModifier 			0x08				/*  Mask for no command key modifier */

#define kMenuNoIcon 					0					/*  No icon */
#define kMenuIconType 					1					/*  Type for ICON */
#define kMenuShrinkIconType 			2					/*  Type for ICON plotted 16 x 16 */
#define kMenuSmallIconType 				3					/*  Type for SICN */
#define kMenuColorIconType 				4					/*  Type for cicn */
#define kMenuIconSuiteType 				5					/*  Type for Icon Suite */
#define kMenuIconRefType 				6					/*  Type for Icon Ref */
#define kMenuCGImageRefType 			7					/*  Type for a CGImageRef (Mac OS X only) */
#define kMenuSystemIconSelectorType 	8					/*  Type for an OSType identifying an IconRef registered with Icon Services under kSystemIconsCreator (Mac OS X 10.1 and later only) */
#define kMenuIconResourceType 			9					/*  Type for a CFStringRef with the full name of a .icns resource in the main bundle of the process (Mac OS X 10.1 and later only) */

#define kMenuNullGlyph 					0x00				/*  Null (always glyph 1) */
#define kMenuTabRightGlyph 				0x02				/*  Tab to the right key (for left-to-right script systems) */
#define kMenuTabLeftGlyph 				0x03				/*  Tab to the left key (for right-to-left script systems) */
#define kMenuEnterGlyph 				0x04				/*  Enter key */
#define kMenuShiftGlyph 				0x05				/*  Shift key */
#define kMenuControlGlyph 				0x06				/*  Control key */
#define kMenuOptionGlyph 				0x07				/*  Option key */
#define kMenuSpaceGlyph 				0x09				/*  Space (always glyph 3) key */
#define kMenuDeleteRightGlyph 			0x0A				/*  Delete to the right key (for right-to-left script systems) */
#define kMenuReturnGlyph 				0x0B				/*  Return key (for left-to-right script systems) */
#define kMenuReturnR2LGlyph 			0x0C				/*  Return key (for right-to-left script systems) */
#define kMenuNonmarkingReturnGlyph 		0x0D				/*  Nonmarking return key */
#define kMenuPencilGlyph 				0x0F				/*  Pencil key */
#define kMenuDownwardArrowDashedGlyph 	0x10				/*  Downward dashed arrow key */
#define kMenuCommandGlyph 				0x11				/*  Command key */
#define kMenuCheckmarkGlyph 			0x12				/*  Checkmark key */
#define kMenuDiamondGlyph 				0x13				/*  Diamond key */
#define kMenuAppleLogoFilledGlyph 		0x14				/*  Apple logo key (filled) */
#define kMenuParagraphKoreanGlyph 		0x15				/*  Unassigned (paragraph in Korean) */
#define kMenuDeleteLeftGlyph 			0x17				/*  Delete to the left key (for left-to-right script systems) */
#define kMenuLeftArrowDashedGlyph 		0x18				/*  Leftward dashed arrow key */
#define kMenuUpArrowDashedGlyph 		0x19				/*  Upward dashed arrow key */
#define kMenuRightArrowDashedGlyph 		0x1A				/*  Rightward dashed arrow key */
#define kMenuEscapeGlyph 				0x1B				/*  Escape key */
#define kMenuClearGlyph 				0x1C				/*  Clear key */
#define kMenuLeftDoubleQuotesJapaneseGlyph  0x1D			/*  Unassigned (left double quotes in Japanese) */
#define kMenuRightDoubleQuotesJapaneseGlyph  0x1E			/*  Unassigned (right double quotes in Japanese) */
#define kMenuTrademarkJapaneseGlyph 	0x1F				/*  Unassigned (trademark in Japanese) */
#define kMenuBlankGlyph 				0x61				/*  Blank key */
#define kMenuPageUpGlyph 				0x62				/*  Page up key */
#define kMenuCapsLockGlyph 				0x63				/*  Caps lock key */
#define kMenuLeftArrowGlyph 			0x64				/*  Left arrow key */
#define kMenuRightArrowGlyph 			0x65				/*  Right arrow key */
#define kMenuNorthwestArrowGlyph 		0x66				/*  Northwest arrow key */
#define kMenuHelpGlyph 					0x67				/*  Help key */
#define kMenuUpArrowGlyph 				0x68				/*  Up arrow key */
#define kMenuSoutheastArrowGlyph 		0x69				/*  Southeast arrow key */
#define kMenuDownArrowGlyph 			0x6A				/*  Down arrow key */
#define kMenuPageDownGlyph 				0x6B				/*  Page down key */
#define kMenuAppleLogoOutlineGlyph 		0x6C				/*  Apple logo key (outline) */
#define kMenuContextualMenuGlyph 		0x6D				/*  Contextual menu key */
#define kMenuPowerGlyph 				0x6E				/*  Power key */
#define kMenuF1Glyph 					0x6F				/*  F1 key */
#define kMenuF2Glyph 					0x70				/*  F2 key */
#define kMenuF3Glyph 					0x71				/*  F3 key */
#define kMenuF4Glyph 					0x72				/*  F4 key */
#define kMenuF5Glyph 					0x73				/*  F5 key */
#define kMenuF6Glyph 					0x74				/*  F6 key */
#define kMenuF7Glyph 					0x75				/*  F7 key */
#define kMenuF8Glyph 					0x76				/*  F8 key */
#define kMenuF9Glyph 					0x77				/*  F9 key */
#define kMenuF10Glyph 					0x78				/*  F10 key */
#define kMenuF11Glyph 					0x79				/*  F11 key */
#define kMenuF12Glyph 					0x7A				/*  F12 key */
#define kMenuF13Glyph 					0x87				/*  F13 key */
#define kMenuF14Glyph 					0x88				/*  F14 key */
#define kMenuF15Glyph 					0x89				/*  F15 key */
#define kMenuControlISOGlyph 			0x8A				/*  Control key (ISO standard) */
#define kMenuEjectGlyph 				0x8C				/*  Eject key (available on Mac OS X 10.2 and later) */
#define kMenuEisuGlyph 					0x8D				/*  Japanese eisu key (available in Mac OS X 10.4 and later) */
#define kMenuKanaGlyph 					0x8E				/*  Japanese kana key (available in Mac OS X 10.4 and later) */
#define kMenuF16Glyph 					0x8F				/*  F16 key (available in SnowLeopard and later) */
#define kMenuF17Glyph 					0x90				/*  F17 key (available in SnowLeopard and later) */
#define kMenuF18Glyph 					0x91				/*  F18 key (available in SnowLeopard and later) */
#define kMenuF19Glyph 					0x92				/*  F19 key (available in SnowLeopard and later) */

#define kMenuAttrExcludesMarkColumn 	0x01
#define kMenuAttrAutoDisable 			0x04
#define kMenuAttrUsePencilGlyph 		0x08
#define kMenuAttrHidden 				0x10
#define kMenuAttrCondenseSeparators 	0x20
#define kMenuAttrDoNotCacheImage 		0x40
#define kMenuAttrDoNotUseUserCommandKeys  0x80

#define kMenuItemAttrDisabled 			0x01
#define kMenuItemAttrIconDisabled 		0x02
#define kMenuItemAttrSubmenuParentChoosable  0x04
#define kMenuItemAttrDynamic 			0x08
#define kMenuItemAttrNotPreviousAlternate  0x10
#define kMenuItemAttrHidden 			0x20
#define kMenuItemAttrSeparator 			0x40
#define kMenuItemAttrSectionHeader 		0x80
#define kMenuItemAttrIgnoreMeta 		0x0100
#define kMenuItemAttrAutoRepeat 		0x0200
#define kMenuItemAttrUseVirtualKey 		0x0400
#define kMenuItemAttrCustomDraw 		0x0800
#define kMenuItemAttrIncludeInCmdKeyMatching  0x1000
#define kMenuItemAttrAutoDisable 		0x2000
#define kMenuItemAttrUpdateSingleItem 	0x4000

#define gestaltContextualMenuAttr 		'cmnu'
#define gestaltContextualMenuUnusedBit 	0
#define gestaltContextualMenuTrapAvailable  1
#define gestaltContextualMenuHasAttributeAndModifierKeys  2	/*  Contextual Menu Manager supports keyContextualMenuAttributes and keyContextualMenuModifiers  */
#define gestaltContextualMenuHasUnicodeSupport  3			/*  Contextual Menu Manager supports typeUnicodeText and typeCFStringRef  */

#define kCMHelpItemNoHelp 				0
#define kCMHelpItemAppleGuide 			1
#define kCMHelpItemOtherHelp 			2
#define kCMHelpItemRemoveHelp 			3

#define kCMNothingSelected 				0
#define kCMMenuItemSelected 			1
#define kCMShowHelpSelected 			3


/*----------------------------MENU ¥ Menu-----------------------------------------------*/
type 'MENU' {
      integer;                                                /* Menu ID              */
     fill word[2];
      integer         textMenuProc = 0;                       /* ID of menu def proc  */
     fill word;
     unsigned hex bitstring[31]
                     allEnabled = 0x7FFFFFFF;                /* Enable flags         */
     boolean         disabled, enabled;                      /* Menu enable          */
     pstring         apple = "\0x14";                        /* Menu Title           */
     wide array {
               pstring;                                        /* Item title           */
             byte            noIcon;                         /* Icon number          */
             char            noKey = "\0x00",                /* Key equivalent or    */
                             hierarchicalMenu = "\0x1B";     /* hierarchical menu    */
             char            noMark = "\0x00",               /* Marking char or id   */
                             check = "\0x12";                /* of hierarchical menu */
             fill bit;
              unsigned bitstring[7]
                              plain;                          /* Style                */
     };
     byte = 0;
};

/*----------------------------MBAR ¥ Menu Bar-------------------------------------------*/
type 'MBAR' {
      integer = $$CountOf(MenuArray);                         /* Number of menus      */
     wide array MenuArray{
              integer;                                        /* Menu resource ID     */
     };
};

/*----------------------------mctb ¥ Menu Color Lookup Table----------------------------*/
type 'mctb' {
      integer = $$CountOf(MCTBArray);                         /* Color table count    */
     wide array MCTBArray {
         integer             mctbLast = -99;                 /* Menu resource ID     */
         integer;                                            /* Menu Item            */
         wide array [4] {
                   unsigned integer;                           /* RGB: red             */
                 unsigned integer;                           /*      green           */
                 unsigned integer;                           /*      blue            */
         };
         fill word;                                          /* Reserved word        */
     };
};


/*-------------------------------xmnu ¥ Extended menu resource---------------------------*/
type 'xmnu'
{
 switch
 {
      case versionZero:
          key integer = 0;    /* version */

         integer = $$Countof(ItemExtensions);
           array ItemExtensions
           {
              switch
             {
                  case skipItem:
                     key integer=0;
                     
                   case dataItem:
                     key integer=1;
                     unsigned longint;                       /* Command ID */
                       unsigned hex byte;                      /* modifiers */
                        fill byte;                              /* icon type placeholder */
                        fill long;                              /* icon handle placeholder */
                      unsigned longint sysScript=-1,          /* text encoding */
                                         currScript=-2;         /*  (use currScript for default)*/
                     unsigned longint;                       /* refCon */
                       unsigned longint;                       /* refCon2 */
                      unsigned integer noHierID=0;            /* hierarchical menu ID */
                     unsigned integer sysFont=0;             /* font ID */
                      integer naturalGlyph=0;                 /* keyboard glyph */
               };
         };

        case versionOne:
           key integer = 1;    /* version */

         integer = $$Countof(ItemExtensions);
           array ItemExtensions
           {
              switch
             {
                  case skipItem:
                     key integer=0;
                     
                   case dataItem:
                     key integer=1;
                     unsigned longint;                       /* Command ID */
                       unsigned hex byte;                      /* modifiers */
                        unsigned hex byte noVKey=0;             /* virtual keycode for command key */
                      unsigned hex longint noAttributes=0;    /* MenuItemAttributes */
                       unsigned longint sysScript=-1,          /* text encoding */
                                         currScript=-2;         /*  (use currScript for default)*/
                     unsigned longint;                       /* refCon */
                       unsigned longint noIndent=0;            /* indent */
                       unsigned integer noHierID=0;            /* hierarchical menu ID */
                     unsigned integer sysFont=0;             /* font ID */
                      integer naturalGlyph=0;                 /* keyboard glyph */
               };
         };
 };
};

#if TARGET_OS_WIN32

/*----------------------------MENA ¥ Menu Accessory Key---------------------------------*/
type 'MENA' {
      integer = $$CountOf(MenuArray);                         /* Number of keys       */
     longint;                                                /* flags                */
     wide array MenuArray{
              char;                                           /* key                  */
     };
};

#endif  /* TARGET_OS_WIN32 */


#endif /* __MENUS_R__ */

