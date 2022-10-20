/*
     File:       AEObjects.r
 
     Contains:   Object Support Library Interfaces.
 
    
 
     Copyright:  � 1991-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __AEOBJECTS_R__
#define __AEOBJECTS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

															/* *** LOGICAL OPERATOR CONSTANTS  *** */
#define kAEAND 							'AND '				/*   0x414e4420   */
#define kAEOR 							'OR  '				/*   0x4f522020   */
#define kAENOT 							'NOT '				/*   0x4e4f5420   */
															/* *** ABSOLUTE ORDINAL CONSTANTS  *** */
#define kAEFirst 						'firs'				/*   0x66697273   */
#define kAELast 						'last'				/*   0x6c617374   */
#define kAEMiddle 						'midd'				/*   0x6d696464   */
#define kAEAny 							'any '				/*   0x616e7920   */
#define kAEAll 							'all '				/*   0x616c6c20   */
															/* *** RELATIVE ORDINAL CONSTANTS  *** */
#define kAENext 						'next'				/*   0x6e657874   */
#define kAEPrevious 					'prev'				/*   0x70726576   */
															/* *** KEYWORD CONSTANT    *** */
#define keyAECompOperator 				'relo'				/*   0x72656c6f   */
#define keyAELogicalTerms 				'term'				/*   0x7465726d   */
#define keyAELogicalOperator 			'logc'				/*   0x6c6f6763   */
#define keyAEObject1 					'obj1'				/*   0x6f626a31   */
#define keyAEObject2 					'obj2'				/*   0x6f626a32   */
															/*     ... for Keywords for getting fields out of object specifier records.  */
#define keyAEDesiredClass 				'want'				/*   0x77616e74   */
#define keyAEContainer 					'from'				/*   0x66726f6d   */
#define keyAEKeyForm 					'form'				/*   0x666f726d   */
#define keyAEKeyData 					'seld'				/*   0x73656c64   */

															/*     ... for Keywords for getting fields out of Range specifier records.  */
#define keyAERangeStart 				'star'				/*   0x73746172   */
#define keyAERangeStop 					'stop'				/*   0x73746f70   */
															/*     ... special handler selectors for OSL Callbacks.  */
#define keyDisposeTokenProc 			'xtok'				/*   0x78746f6b   */
#define keyAECompareProc 				'cmpr'				/*   0x636d7072   */
#define keyAECountProc 					'cont'				/*   0x636f6e74   */
#define keyAEMarkTokenProc 				'mkid'				/*   0x6d6b6964   */
#define keyAEMarkProc 					'mark'				/*   0x6d61726b   */
#define keyAEAdjustMarksProc 			'adjm'				/*   0x61646a6d   */
#define keyAEGetErrDescProc 			'indc'				/*   0x696e6463   */

															/*     ... possible values for the keyAEKeyForm field of an object specifier.  */
#define formAbsolutePosition 			'indx'				/*   0x696e6478   */
#define formRelativePosition 			'rele'				/*   0x72656c65   */
#define formTest 						'test'				/*   0x74657374   */
#define formRange 						'rang'				/*   0x72616e67   */
#define formPropertyID 					'prop'				/*   0x70726f70   */
#define formName 						'name'				/*   0x6e616d65   */
#define formUniqueID 					'ID  '				/*   0x49442020   */
															/*     ... relevant types (some of these are often pared with forms above).  */
#define typeObjectSpecifier 			'obj '				/*   0x6f626a20   */
#define typeObjectBeingExamined 		'exmn'				/*   0x65786d6e   */
#define typeCurrentContainer 			'ccnt'				/*   0x63636e74   */
#define typeToken 						'toke'				/*   0x746f6b65   */
#define typeRelativeDescriptor 			'rel '				/*   0x72656c20   */
#define typeAbsoluteOrdinal 			'abso'				/*   0x6162736f   */
#define typeIndexDescriptor 			'inde'				/*   0x696e6465   */
#define typeRangeDescriptor 			'rang'				/*   0x72616e67   */
#define typeLogicalDescriptor 			'logi'				/*   0x6c6f6769   */
#define typeCompDescriptor 				'cmpd'				/*   0x636d7064   */
#define typeOSLTokenList 				'ostl'				/*   0x6F73746C   */

#define typeWhoseDescriptor 			'whos'				/*   0x77686f73   */
#define formWhose 						'whos'				/*   0x77686f73   */
#define typeWhoseRange 					'wrng'				/*   0x77726e67   */
#define keyAEWhoseRangeStart 			'wstr'				/*   0x77737472   */
#define keyAEWhoseRangeStop 			'wstp'				/*   0x77737470   */
#define keyAEIndex 						'kidx'				/*   0x6b696478   */
#define keyAETest 						'ktst'				/*   0x6b747374   */


#endif /* __AEOBJECTS_R__ */

