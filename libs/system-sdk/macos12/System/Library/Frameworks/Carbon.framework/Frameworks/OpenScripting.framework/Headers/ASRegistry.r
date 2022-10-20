/*
     File:       ASRegistry.r
 
     Contains:   AppleScript Registry constants.
 
     Version:    OpenScripting
 
     Copyright:  © 1991-2013 by Apple Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __ASREGISTRY_R__
#define __ASREGISTRY_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define keyAETarget 					'targ'
#define keySubjectAttr 					'subj'				/*  Magic 'returning' parameter:  */
#define keyASReturning 					'Krtn'				/*  AppleScript Specific Codes:  */
#define kASAppleScriptSuite 			'ascr'
#define kASScriptEditorSuite 			'ToyS'
#define kASTypeNamesSuite 				'tpnm'				/*  dynamic terminologies  */
#define typeAETE 						'aete'
#define typeAEUT 						'aeut'
#define kGetAETE 						'gdte'
#define kGetAEUT 						'gdut'
#define kUpdateAEUT 					'udut'
#define kUpdateAETE 					'udte'
#define kCleanUpAEUT 					'cdut'
#define kASComment 						'cmnt'
#define kASLaunchEvent 					'noop'
#define keyScszResource 				'scsz'
#define typeScszResource 				'scsz'				/*  subroutine calls  */
#define kASSubroutineEvent 				'psbr'
#define keyASSubroutineName 			'snam'
#define kASPrepositionalSubroutine 		'psbr'
#define keyASPositionalArgs 			'parg'

															/*  Add this parameter to a Get Data result if your app handled the 'as' parameter  */
#define keyAppHandledCoercion 			'idas'
															/*  Miscellaneous AppleScript commands  */
#define kASStartLogEvent 				'log1'
#define kASStopLogEvent 				'log0'
#define kASCommentEvent 				'cmnt'

															/*  Binary:  */
#define kASAdd 							'+   '
#define kASSubtract 					'-   '
#define kASMultiply 					'*   '
#define kASDivide 						'/   '
#define kASQuotient 					'div '
#define kASRemainder 					'mod '
#define kASPower 						'^   '
#define kASEqual 						'=   '
#define kASNotEqual 					'­   '
#define kASGreaterThan 					'>   '
#define kASGreaterThanOrEqual 			'>=  '
#define kASLessThan 					'<   '
#define kASLessThanOrEqual 				'<=  '
#define kASComesBefore 					'cbfr'
#define kASComesAfter 					'cafr'
#define kASConcatenate 					'ccat'
#define kASStartsWith 					'bgwt'
#define kASEndsWith 					'ends'
#define kASContains 					'cont'

#define kASAnd 							'AND '
#define kASOr 							'OR  '				/*  Unary:  */
#define kASNot 							'NOT '
#define kASNegate 						'neg '
#define keyASArg 						'arg '

															/*  event code for the 'error' statement  */
#define kASErrorEventCode 				'err '
#define kOSAErrorArgs 					'erra'
#define keyAEErrorObject 				'erob'				/* event code for the 'use' statement */
#define kASUseEventCode					'use '
#define kASMinimumVersion				'minv'
#define kASImporting					'impr'
#define kASExcluding					'impx'				/*  Properties:  */
#define pLength 						'leng'
#define pReverse 						'rvse'
#define pRest 							'rest'
#define pInherits 						'c@#^'
#define pProperties 					'pALL'				/*  User-Defined Record Fields:  */
#define keyASUserRecordFields 			'usrf'
#define typeUserRecordFields 			'list'

#define keyASPrepositionAt 				'at  '
#define keyASPrepositionIn 				'in  '
#define keyASPrepositionFrom 			'from'
#define keyASPrepositionFor 			'for '
#define keyASPrepositionTo 				'to  '
#define keyASPrepositionThru 			'thru'
#define keyASPrepositionThrough 		'thgh'
#define keyASPrepositionBy 				'by  '
#define keyASPrepositionOn 				'on  '
#define keyASPrepositionInto 			'into'
#define keyASPrepositionOnto 			'onto'
#define keyASPrepositionBetween 		'btwn'
#define keyASPrepositionAgainst 		'agst'
#define keyASPrepositionOutOf 			'outo'
#define keyASPrepositionInsteadOf 		'isto'
#define keyASPrepositionAsideFrom 		'asdf'
#define keyASPrepositionAround 			'arnd'
#define keyASPrepositionBeside 			'bsid'
#define keyASPrepositionBeneath 		'bnth'
#define keyASPrepositionUnder 			'undr'

#define keyASPrepositionOver 			'over'
#define keyASPrepositionAbove 			'abve'
#define keyASPrepositionBelow 			'belw'
#define keyASPrepositionApartFrom 		'aprt'
#define keyASPrepositionGiven 			'givn'
#define keyASPrepositionWith 			'with'
#define keyASPrepositionWithout 		'wout'
#define keyASPrepositionAbout 			'abou'
#define keyASPrepositionSince 			'snce'
#define keyASPrepositionUntil 			'till'

															/*  Terminology & Dialect things:  */
#define kDialectBundleResType 			'Dbdl'				/*  AppleScript Classes and Enums:  */
#define cConstant 						'enum'
#define cClassIdentifier 				'pcls'
#define cObjectBeingExamined 			'exmn'
#define cList 							'list'
#define cSmallReal 						'sing'
#define cReal 							'doub'
#define cRecord 						'reco'
#define cReference 						'obj '
#define cUndefined 						'undf'
#define cMissingValue 					'msng'
#define cSymbol 						'symb'
#define cLinkedList 					'llst'
#define cVector 						'vect'
#define cEventIdentifier 				'evnt'
#define cKeyIdentifier 					'kyid'
#define cUserIdentifier 				'uid '
#define cPreposition 					'prep'
#define cKeyForm 						'kfrm'
#define cScript 						'scpt'
#define cHandler 						'hand'
#define cProcedure 						'proc'
#define cFramework						'frmk'
#define cDynamicLibrary					'dylb'
#define cScriptingAddition				'osax'

#define cHandleBreakpoint 				'brak'
#define cClosure 						'clsr'
#define cRawData 						'rdat'
#define cStringClass 					'TEXT'
#define cNumber 						'nmbr'
#define cListElement 					'celm'
#define cListOrRecord 					'lr  '
#define cListOrString 					'ls  '
#define cListRecordOrString 			'lrs '
#define cNumberOrString 				'ns  '
#define cNumberOrDateTime 				'nd  '
#define cNumberDateTimeOrString 		'nds '
#define cAliasOrString 					'sf  '
#define cSeconds 						'scnd'
#define typeSound 						'snd '
#define enumBooleanValues 				'boov'				/*   Use this instead of typeBoolean to avoid with/without conversion   */
#define kAETrue 						'true'
#define kAEFalse 						'fals'
#define enumMiscValues 					'misc'
#define kASCurrentApplication 			'cura'				/*  User-defined property ospecs:  */
#define formUserPropertyID 				'usrp'

															/*  Global properties:  */
#define pASIt 							'it  '
#define pASMe 							'me  '
#define pASResult 						'rslt'
#define pASSpace 						'spac'
#define pASReturn 						'ret '
#define pASTab 							'tab '
#define pASPi 							'pi  '
#define pASParent 						'pare'
#define kASInitializeEventCode 			'init'
#define pASPrintLength 					'prln'
#define pASPrintDepth 					'prdp'
#define pASTopLevelScript 				'ascr'
#define pASRequiredImportItems			'pimr'

															/*  Considerations  */
#define kAECase 						'case'
#define kAEDiacritic 					'diac'
#define kAEWhiteSpace 					'whit'
#define kAEHyphens 						'hyph'
#define kAEExpansion 					'expa'
#define kAEPunctuation 					'punc'
#define kAEZenkakuHankaku 				'zkhk'
#define kAESmallKana 					'skna'
#define kAEKataHiragana 				'hika'
#define kASConsiderReplies 				'rmte'
#define kASNumericStrings 				'nume'
#define enumConsiderations 				'cons'

#define cCoercion 						'coec'
#define cCoerceUpperCase 				'txup'
#define cCoerceLowerCase 				'txlo'
#define cCoerceRemoveDiacriticals 		'txdc'
#define cCoerceRemovePunctuation 		'txpc'
#define cCoerceRemoveHyphens 			'txhy'
#define cCoerceOneByteToTwoByte 		'txex'
#define cCoerceRemoveWhiteSpace 		'txws'
#define cCoerceSmallKana 				'txsk'
#define cCoerceZenkakuhankaku 			'txze'
#define cCoerceKataHiragana 			'txkh'				/*  Lorax things:  */
#define cZone 							'zone'
#define cMachine 						'mach'
#define cAddress 						'addr'
#define cRunningAddress 				'radd'
#define cStorage 						'stor'

															/*  DateTime things:  */
#define pASWeekday 						'wkdy'
#define pASMonth 						'mnth'
#define pASDay 							'day '
#define pASYear 						'year'
#define pASTime 						'time'
#define pASDateString 					'dstr'
#define pASTimeString 					'tstr'				/*  Months  */
#define cMonth 							'mnth'
#define cJanuary 						'jan '
#define cFebruary 						'feb '
#define cMarch 							'mar '
#define cApril 							'apr '
#define cMay 							'may '
#define cJune 							'jun '
#define cJuly 							'jul '
#define cAugust 						'aug '
#define cSeptember 						'sep '
#define cOctober 						'oct '
#define cNovember 						'nov '
#define cDecember 						'dec '

															/*  Weekdays  */
#define cWeekday 						'wkdy'
#define cSunday 						'sun '
#define cMonday 						'mon '
#define cTuesday 						'tue '
#define cWednesday 						'wed '
#define cThursday 						'thu '
#define cFriday 						'fri '
#define cSaturday 						'sat '				/*  AS 1.1 Globals:  */
#define pASQuote 						'quot'
#define pASSeconds 						'secs'
#define pASMinutes 						'min '
#define pASHours 						'hour'
#define pASDays 						'days'
#define pASWeeks 						'week'				/*  Writing Code things:  */
#define cWritingCodeInfo 				'citl'
#define pScriptCode 					'pscd'
#define pLangCode 						'plcd'				/*  Magic Tell and End Tell events for logging:  */
#define kASMagicTellEvent 				'tell'
#define kASMagicEndTellEvent 			'tend'


#endif /* __ASREGISTRY_R__ */

