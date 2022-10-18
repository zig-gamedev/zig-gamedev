/*
     File:       OpenScripting/ASRegistry.h
 
     Contains:   AppleScript Registry constants.
 
     Version:    OpenScripting
 
     Copyright:  � 1991-2013 by Apple Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ASREGISTRY__
#define __ASREGISTRY__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif


#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif


enum {
  keyAETarget                   = 'targ',
  keySubjectAttr                = 'subj', /* Magic 'returning' parameter: */
  keyASReturning                = 'Krtn', /* AppleScript Specific Codes: */
  kASAppleScriptSuite           = 'ascr',
  kASScriptEditorSuite          = 'ToyS',
  kASTypeNamesSuite             = 'tpnm', /* dynamic terminologies */
  typeAETE                      = 'aete',
  typeAEUT                      = 'aeut',
  kGetAETE                      = 'gdte',
  kGetAEUT                      = 'gdut',
  kUpdateAEUT                   = 'udut',
  kUpdateAETE                   = 'udte',
  kCleanUpAEUT                  = 'cdut',
  kASComment                    = 'cmnt',
  kASLaunchEvent                = 'noop',
  keyScszResource               = 'scsz',
  typeScszResource              = 'scsz', /* subroutine calls */
  kASSubroutineEvent            = 'psbr',
  keyASSubroutineName           = 'snam',
  kASPrepositionalSubroutine    = 'psbr',
  keyASPositionalArgs           = 'parg'
};

enum {
                                        /* Add this parameter to a Get Data result if your app handled the 'as' parameter */
  keyAppHandledCoercion         = 'idas'
};

enum {
                                        /* Miscellaneous AppleScript commands */
  kASStartLogEvent              = 'log1',
  kASStopLogEvent               = 'log0',
  kASCommentEvent               = 'cmnt'
};


/* Operator Events: */
enum {
                                        /* Binary: */
  kASAdd                        = '+   ',
  kASSubtract                   = '-   ',
  kASMultiply                   = '*   ',
  kASDivide                     = '/   ',
  kASQuotient                   = 'div ',
  kASRemainder                  = 'mod ',
  kASPower                      = '^   ',
  kASEqual                      = kAEEquals,
  kASNotEqual                   = (int)0xAD202020,
  kASGreaterThan                = kAEGreaterThan,
  kASGreaterThanOrEqual         = kAEGreaterThanEquals,
  kASLessThan                   = kAELessThan,
  kASLessThanOrEqual            = kAELessThanEquals,
  kASComesBefore                = 'cbfr',
  kASComesAfter                 = 'cafr',
  kASConcatenate                = 'ccat',
  kASStartsWith                 = kAEBeginsWith,
  kASEndsWith                   = kAEEndsWith,
  kASContains                   = kAEContains
};

enum {
  kASAnd                        = kAEAND,
  kASOr                         = kAEOR, /* Unary: */
  kASNot                        = kAENOT,
  kASNegate                     = 'neg ',
  keyASArg                      = 'arg '
};

enum {
                                        /* event code for the 'error' statement */
  kASErrorEventCode             = 'err ',
  kOSAErrorArgs                 = 'erra',
  keyAEErrorObject              = 'erob', /* event code for the 'use' statement */
  kASUseEventCode               = 'use ',
  kASMinimumVersion             = 'minv',
  kASImporting                  = 'impr',
  kASExcluding                  = 'impx', /* Properties: */
  pLength                       = 'leng',
  pReverse                      = 'rvse',
  pRest                         = 'rest',
  pInherits                     = 'c@#^',
  pProperties                   = 'pALL', /* User-Defined Record Fields: */
  keyASUserRecordFields         = 'usrf',
  typeUserRecordFields          = typeAEList
};

/* Prepositions: */
enum {
  keyASPrepositionAt            = 'at  ',
  keyASPrepositionIn            = 'in  ',
  keyASPrepositionFrom          = 'from',
  keyASPrepositionFor           = 'for ',
  keyASPrepositionTo            = 'to  ',
  keyASPrepositionThru          = 'thru',
  keyASPrepositionThrough       = 'thgh',
  keyASPrepositionBy            = 'by  ',
  keyASPrepositionOn            = 'on  ',
  keyASPrepositionInto          = 'into',
  keyASPrepositionOnto          = 'onto',
  keyASPrepositionBetween       = 'btwn',
  keyASPrepositionAgainst       = 'agst',
  keyASPrepositionOutOf         = 'outo',
  keyASPrepositionInsteadOf     = 'isto',
  keyASPrepositionAsideFrom     = 'asdf',
  keyASPrepositionAround        = 'arnd',
  keyASPrepositionBeside        = 'bsid',
  keyASPrepositionBeneath       = 'bnth',
  keyASPrepositionUnder         = 'undr'
};

enum {
  keyASPrepositionOver          = 'over',
  keyASPrepositionAbove         = 'abve',
  keyASPrepositionBelow         = 'belw',
  keyASPrepositionApartFrom     = 'aprt',
  keyASPrepositionGiven         = 'givn',
  keyASPrepositionWith          = 'with',
  keyASPrepositionWithout       = 'wout',
  keyASPrepositionAbout         = 'abou',
  keyASPrepositionSince         = 'snce',
  keyASPrepositionUntil         = 'till'
};

enum {
                                        /* Terminology & Dialect things: */
  kDialectBundleResType         = 'Dbdl', /* AppleScript Classes and Enums: */
  cConstant                     = typeEnumerated,
  cClassIdentifier              = pClass,
  cObjectBeingExamined          = typeObjectBeingExamined,
  cList                         = typeAEList,
  cSmallReal                    = typeIEEE32BitFloatingPoint,
  cReal                         = typeIEEE64BitFloatingPoint,
  cRecord                       = typeAERecord,
  cReference                    = cObjectSpecifier,
  cUndefined                    = 'undf',
  cMissingValue                 = 'msng',
  cSymbol                       = 'symb',
  cLinkedList                   = 'llst',
  cVector                       = 'vect',
  cEventIdentifier              = 'evnt',
  cKeyIdentifier                = 'kyid',
  cUserIdentifier               = 'uid ',
  cPreposition                  = 'prep',
  cKeyForm                      = enumKeyForm,
  cScript                       = 'scpt',
  cHandler                      = 'hand',
  cProcedure                    = 'proc',
  cFramework                    = 'frmk',
  cDynamicLibrary               = 'dylb',
  cScriptingAddition            = 'osax'
};

enum {
  cHandleBreakpoint             = 'brak'
};

enum {
  cClosure                      = 'clsr',
  cRawData                      = 'rdat',
  cStringClass                  = typeChar,
  cNumber                       = 'nmbr',
  cListElement                  = 'celm',
  cListOrRecord                 = 'lr  ',
  cListOrString                 = 'ls  ',
  cListRecordOrString           = 'lrs ',
  cNumberOrString               = 'ns  ',
  cNumberOrDateTime             = 'nd  ',
  cNumberDateTimeOrString       = 'nds ',
  cAliasOrString                = 'sf  ',
  cSeconds                      = 'scnd',
  typeSound                     = 'snd ',
  enumBooleanValues             = 'boov', /*  Use this instead of typeBoolean to avoid with/without conversion  */
  kAETrue                       = typeTrue,
  kAEFalse                      = typeFalse,
  enumMiscValues                = 'misc',
  kASCurrentApplication         = 'cura', /* User-defined property ospecs: */
  formUserPropertyID            = 'usrp'
};

enum {
  cString                       = cStringClass /* old name for cStringClass - can't be used in .r files*/
};

enum {
                                        /* Global properties: */
  pASIt                         = 'it  ',
  pASMe                         = 'me  ',
  pASResult                     = 'rslt',
  pASSpace                      = 'spac',
  pASReturn                     = 'ret ',
  pASTab                        = 'tab ',
  pASPi                         = 'pi  ',
  pASParent                     = 'pare',
  kASInitializeEventCode        = 'init',
  pASPrintLength                = 'prln',
  pASPrintDepth                 = 'prdp',
  pASTopLevelScript             = 'ascr',
  pASRequiredImportItems        = 'pimr'
};

enum {
                                        /* Considerations */
  kAECase                       = 'case',
  kAEDiacritic                  = 'diac',
  kAEWhiteSpace                 = 'whit',
  kAEHyphens                    = 'hyph',
  kAEExpansion                  = 'expa',
  kAEPunctuation                = 'punc',
  kAEZenkakuHankaku             = 'zkhk',
  kAESmallKana                  = 'skna',
  kAEKataHiragana               = 'hika',
  kASConsiderReplies            = 'rmte',
  kASNumericStrings             = 'nume',
  enumConsiderations            = 'cons'
};

/* Considerations bit masks */
enum {
  kAECaseConsiderMask           = 0x00000001,
  kAEDiacriticConsiderMask      = 0x00000002,
  kAEWhiteSpaceConsiderMask     = 0x00000004,
  kAEHyphensConsiderMask        = 0x00000008,
  kAEExpansionConsiderMask      = 0x00000010,
  kAEPunctuationConsiderMask    = 0x00000020,
  kASConsiderRepliesConsiderMask = 0x00000040,
  kASNumericStringsConsiderMask = 0x00000080,
  kAECaseIgnoreMask             = 0x00010000,
  kAEDiacriticIgnoreMask        = 0x00020000,
  kAEWhiteSpaceIgnoreMask       = 0x00040000,
  kAEHyphensIgnoreMask          = 0x00080000,
  kAEExpansionIgnoreMask        = 0x00100000,
  kAEPunctuationIgnoreMask      = 0x00200000,
  kASConsiderRepliesIgnoreMask  = 0x00400000,
  kASNumericStringsIgnoreMask   = 0x00800000,
  enumConsidsAndIgnores         = 'csig'
};

enum {
  cCoercion                     = 'coec',
  cCoerceUpperCase              = 'txup',
  cCoerceLowerCase              = 'txlo',
  cCoerceRemoveDiacriticals     = 'txdc',
  cCoerceRemovePunctuation      = 'txpc',
  cCoerceRemoveHyphens          = 'txhy',
  cCoerceOneByteToTwoByte       = 'txex',
  cCoerceRemoveWhiteSpace       = 'txws',
  cCoerceSmallKana              = 'txsk',
  cCoerceZenkakuhankaku         = 'txze',
  cCoerceKataHiragana           = 'txkh', /* Lorax things: */
  cZone                         = 'zone',
  cMachine                      = 'mach',
  cAddress                      = 'addr',
  cRunningAddress               = 'radd',
  cStorage                      = 'stor'
};

enum {
                                        /* DateTime things: */
  pASWeekday                    = 'wkdy',
  pASMonth                      = 'mnth',
  pASDay                        = 'day ',
  pASYear                       = 'year',
  pASTime                       = 'time',
  pASDateString                 = 'dstr',
  pASTimeString                 = 'tstr', /* Months */
  cMonth                        = pASMonth,
  cJanuary                      = 'jan ',
  cFebruary                     = 'feb ',
  cMarch                        = 'mar ',
  cApril                        = 'apr ',
  cMay                          = 'may ',
  cJune                         = 'jun ',
  cJuly                         = 'jul ',
  cAugust                       = 'aug ',
  cSeptember                    = 'sep ',
  cOctober                      = 'oct ',
  cNovember                     = 'nov ',
  cDecember                     = 'dec '
};

enum {
                                        /* Weekdays */
  cWeekday                      = pASWeekday,
  cSunday                       = 'sun ',
  cMonday                       = 'mon ',
  cTuesday                      = 'tue ',
  cWednesday                    = 'wed ',
  cThursday                     = 'thu ',
  cFriday                       = 'fri ',
  cSaturday                     = 'sat ', /* AS 1.1 Globals: */
  pASQuote                      = 'quot',
  pASSeconds                    = 'secs',
  pASMinutes                    = 'min ',
  pASHours                      = 'hour',
  pASDays                       = 'days',
  pASWeeks                      = 'week', /* Writing Code things: */
  cWritingCodeInfo              = 'citl',
  pScriptCode                   = 'pscd',
  pLangCode                     = 'plcd', /* Magic Tell and End Tell events for logging: */
  kASMagicTellEvent             = 'tell',
  kASMagicEndTellEvent          = 'tend'
};



#endif /* __ASREGISTRY__ */

