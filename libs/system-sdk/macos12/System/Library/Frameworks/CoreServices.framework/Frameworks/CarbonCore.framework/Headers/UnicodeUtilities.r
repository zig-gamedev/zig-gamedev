/*
     File:       UnicodeUtilities.r
 
     Contains:   Types, constants, prototypes for Unicode Utilities (Unicode input and text utils)
 
     Copyright:  © 1997-2011 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __UNICODEUTILITIES_R__
#define __UNICODEUTILITIES_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define kUCKeyOutputStateIndexMask 		0x4000
#define kUCKeyOutputSequenceIndexMask 	0x8000
#define kUCKeyOutputTestForIndexMask 	0xC000				/*  test bits 14-15 */
#define kUCKeyOutputGetIndexMask 		0x3FFF				/*  get bits 0-13 */

#define kUCKeyStateEntryTerminalFormat 	0x0001
#define kUCKeyStateEntryRangeFormat 	0x0002

#define kUCKeyLayoutHeaderFormat 		0x1002
#define kUCKeyLayoutFeatureInfoFormat 	0x2001
#define kUCKeyModifiersToTableNumFormat  0x3001
#define kUCKeyToCharTableIndexFormat 	0x4001
#define kUCKeyStateRecordsIndexFormat 	0x5001
#define kUCKeyStateTerminatorsFormat 	0x6001
#define kUCKeySequenceDataIndexFormat 	0x7001

#define kUCKeyActionDown 				0					/*  key is going down */
#define kUCKeyActionUp 					1					/*  key is going up */
#define kUCKeyActionAutoKey 			2					/*  auto-key down */
#define kUCKeyActionDisplay 			3					/*  get information for key display (as in Key Caps)       */

#define kUCKeyTranslateNoDeadKeysBit 	0					/*  Prevents setting any new dead-key states */
#define kUCKeyTranslateNoDeadKeysMask 	0x00000001
#define kUnicodeCollationClass 			'ucol'
															/*  Sensitivity options */
#define kUCCollateComposeInsensitiveMask  0x00000002
#define kUCCollateWidthInsensitiveMask 	0x00000004
#define kUCCollateCaseInsensitiveMask 	0x00000008
#define kUCCollateDiacritInsensitiveMask  0x00000010		/*  Other general options  */
#define kUCCollatePunctuationSignificantMask  0x00008000	/*  Number-handling options  */
#define kUCCollateDigitsOverrideMask 	0x00010000
#define kUCCollateDigitsAsNumberMask 	0x00020000

#define kUCCollateStandardOptions 		0x00000006
#define kUCCollateTypeHFSExtended 		1
#define kUCCollateTypeSourceMask 		0x000000FF
#define kUCCollateTypeShiftBits 		24

#define kUCCollateTypeMask 				0xFF000000
#define kUCTSDirectionNext 				0
#define kUCTSDirectionPrevious 			1

#define kUCTSOptionsNoneMask 			0
#define kUCTSOptionsReleaseStringMask 	1
#define kUCTSOptionsDataIsOrderedMask 	2

#define kUCTypeSelectMaxListSize 		0xFFFFFFFF
#define kUnicodeTextBreakClass 			'ubrk'
#define kUCTextBreakCharMask 			0x00000001
#define kUCTextBreakClusterMask 		0x00000004
#define kUCTextBreakWordMask 			0x00000010
#define kUCTextBreakLineMask 			0x00000040
#define kUCTextBreakParagraphMask 		0x00000100

#define kUCTextBreakLeadingEdgeMask 	0x00000001
#define kUCTextBreakGoBackwardsMask 	0x00000002
#define kUCTextBreakIterateMask 		0x00000004


#endif /* __UNICODEUTILITIES_R__ */

