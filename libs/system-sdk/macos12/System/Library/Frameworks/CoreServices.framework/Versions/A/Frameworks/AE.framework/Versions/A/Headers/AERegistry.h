/*
     File:       AE/AERegistry.h
 
     Contains:   AppleEvents Registry Interface.
 
    
 
     Copyright:  � 1993-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AEREGISTRY__
#define __AEREGISTRY__

#ifndef __CARBONCORE__
#include <CarbonCore/CarbonCore.h>
#endif

#ifndef __APPLEEVENTS__
#include <AE/AppleEvents.h>
#endif



#include <os/availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#pragma pack(push, 2)

CF_ENUM(OSType) {
  cAEList                       = 'list', /*  0x6c697374  */
  cApplication                  = 'capp', /*  0x63617070  */
  cArc                          = 'carc', /*  0x63617263  */
  cBoolean                      = 'bool', /*  0x626f6f6c  */
  cCell                         = 'ccel', /*  0x6363656c  */
  cChar                         = 'cha ', /*  0x63686120  */
  cColorTable                   = 'clrt', /*  0x636c7274  */
  cColumn                       = 'ccol', /*  0x63636f6c  */
  cDocument                     = 'docu', /*  0x646f6375  */
  cDrawingArea                  = 'cdrw', /*  0x63647277  */
  cEnumeration                  = 'enum', /*  0x656e756d  */
  cFile                         = 'file', /*  0x66696c65  */
  cFixed                        = 'fixd', /*  0x66697864  */
  cFixedPoint                   = 'fpnt', /*  0x66706e74  */
  cFixedRectangle               = 'frct', /*  0x66726374  */
  cGraphicLine                  = 'glin', /*  0x676c696e  */
  cGraphicObject                = 'cgob', /*  0x63676f62  */
  cGraphicShape                 = 'cgsh', /*  0x63677368  */
  cGraphicText                  = 'cgtx', /*  0x63677478  */
  cGroupedGraphic               = 'cpic' /*  0x63706963  */
};

CF_ENUM(OSType) {
  cInsertionLoc                 = 'insl', /*  0x696e736c  */
  cInsertionPoint               = 'cins', /*  0x63696e73  */
  cIntlText                     = 'itxt', /*  0x69747874  */
  cIntlWritingCode              = 'intl', /*  0x696e746c  */
  cItem                         = 'citm', /*  0x6369746d  */
  cLine                         = 'clin', /*  0x636c696e  */
  cLongDateTime                 = 'ldt ', /*  0x6c647420  */
  cLongFixed                    = 'lfxd', /*  0x6c667864  */
  cLongFixedPoint               = 'lfpt', /*  0x6c667074  */
  cLongFixedRectangle           = 'lfrc', /*  0x6c667263  */
  cLongInteger                  = 'long', /*  0x6c6f6e67  */
  cLongPoint                    = 'lpnt', /*  0x6c706e74  */
  cLongRectangle                = 'lrct', /*  0x6c726374  */
  cMachineLoc                   = 'mLoc', /*  0x6d4c6f63  */
  cMenu                         = 'cmnu', /*  0x636d6e75  */
  cMenuItem                     = 'cmen', /*  0x636d656e  */
  cObject                       = 'cobj', /*  0x636f626a  */
  cObjectSpecifier              = 'obj ', /*  0x6f626a20  */
  cOpenableObject               = 'coob', /*  0x636f6f62  */
  cOval                         = 'covl' /*  0x636f766c  */
};

CF_ENUM(OSType) {
  cParagraph                    = 'cpar', /*  0x63706172  */
  cPICT                         = 'PICT', /*  0x50494354  */
  cPixel                        = 'cpxl', /*  0x6370786c  */
  cPixelMap                     = 'cpix', /*  0x63706978  */
  cPolygon                      = 'cpgn', /*  0x6370676e  */
  cProperty                     = 'prop', /*  0x70726f70  */
  cQDPoint                      = 'QDpt', /*  0x51447074  */
  cQDRectangle                  = 'qdrt', /*  0x71647274  */
  cRectangle                    = 'crec', /*  0x63726563  */
  cRGBColor                     = 'cRGB', /*  0x63524742  */
  cRotation                     = 'trot', /*  0x74726f74  */
  cRoundedRectangle             = 'crrc', /*  0x63727263  */
  cRow                          = 'crow', /*  0x63726f77  */
  cSelection                    = 'csel', /*  0x6373656c  */
  cShortInteger                 = 'shor', /*  0x73686f72  */
  cTable                        = 'ctbl', /*  0x6374626c  */
  cText                         = 'ctxt', /*  0x63747874  */
  cTextFlow                     = 'cflo', /*  0x63666c6f  */
  cTextStyles                   = 'tsty', /*  0x74737479  */
  cType                         = 'type' /*  0x74797065  */
};

CF_ENUM(OSType) {
  cVersion                      = 'vers', /*  0x76657273  */
  cWindow                       = 'cwin', /*  0x6377696e  */
  cWord                         = 'cwor', /*  0x63776f72  */
  enumArrows                    = 'arro', /*  0x6172726f  */
  enumJustification             = 'just', /*  0x6a757374  */
  enumKeyForm                   = 'kfrm', /*  0x6b66726d  */
  enumPosition                  = 'posi', /*  0x706f7369  */
  enumProtection                = 'prtn', /*  0x7072746e  */
  enumQuality                   = 'qual', /*  0x7175616c  */
  enumSaveOptions               = 'savo', /*  0x7361766f  */
  enumStyle                     = 'styl', /*  0x7374796c  */
  enumTransferMode              = 'tran', /*  0x7472616e  */
  kAEAbout                      = 'abou', /*  0x61626f75  */
  kAEAfter                      = 'afte', /*  0x61667465  */
  kAEAliasSelection             = 'sali', /*  0x73616c69  */
  kAEAllCaps                    = 'alcp', /*  0x616c6370  */
  kAEArrowAtEnd                 = 'aren', /*  0x6172656e  */
  kAEArrowAtStart               = 'arst', /*  0x61727374  */
  kAEArrowBothEnds              = 'arbo' /*  0x6172626f  */
};

CF_ENUM(OSType) {
  kAEAsk                        = 'ask ', /*  0x61736b20  */
  kAEBefore                     = 'befo', /*  0x6265666f  */
  kAEBeginning                  = 'bgng', /*  0x62676e67  */
  kAEBeginsWith                 = 'bgwt', /*  0x62677774  */
  kAEBeginTransaction           = 'begi', /*  0x62656769  */
  kAEBold                       = 'bold', /*  0x626f6c64  */
  kAECaseSensEquals             = 'cseq', /*  0x63736571  */
  kAECentered                   = 'cent', /*  0x63656e74  */
  kAEChangeView                 = 'view', /*  0x76696577  */
  kAEClone                      = 'clon', /*  0x636c6f6e  */
  kAEClose                      = 'clos', /*  0x636c6f73  */
  kAECondensed                  = 'cond', /*  0x636f6e64  */
  kAEContains                   = 'cont', /*  0x636f6e74  */
  kAECopy                       = 'copy', /*  0x636f7079  */
  kAECoreSuite                  = 'core', /*  0x636f7265  */
  kAECountElements              = 'cnte', /*  0x636e7465  */
  kAECreateElement              = 'crel', /*  0x6372656c  */
  kAECreatePublisher            = 'cpub', /*  0x63707562  */
  kAECut                        = 'cut ', /*  0x63757420  */
  kAEDelete                     = 'delo' /*  0x64656c6f  */
};

CF_ENUM(OSType) {
  kAEDoObjectsExist             = 'doex', /*  0x646f6578  */
  kAEDoScript                   = 'dosc', /*  0x646f7363  */
  kAEDrag                       = 'drag', /*  0x64726167  */
  kAEDuplicateSelection         = 'sdup', /*  0x73647570  */
  kAEEditGraphic                = 'edit', /*  0x65646974  */
  kAEEmptyTrash                 = 'empt', /*  0x656d7074  */
  kAEEnd                        = 'end ', /*  0x656e6420  */
  kAEEndsWith                   = 'ends', /*  0x656e6473  */
  kAEEndTransaction             = 'endt', /*  0x656e6474  */
  kAEEquals                     = '=   ', /*  0x3d202020  */
  kAEExpanded                   = 'pexp', /*  0x70657870  */
  kAEFast                       = 'fast', /*  0x66617374  */
  kAEFinderEvents               = 'FNDR', /*  0x464e4452  */
  kAEFormulaProtect             = 'fpro', /*  0x6670726f  */
  kAEFullyJustified             = 'full', /*  0x66756c6c  */
  kAEGetClassInfo               = 'qobj', /*  0x716f626a  */
  kAEGetData                    = 'getd', /*  0x67657464  */
  kAEGetDataSize                = 'dsiz', /*  0x6473697a  */
  kAEGetEventInfo               = 'gtei', /*  0x67746569  */
  kAEGetInfoSelection           = 'sinf' /*  0x73696e66  */
};

CF_ENUM(OSType) {
  kAEGetPrivilegeSelection      = 'sprv', /*  0x73707276  */
  kAEGetSuiteInfo               = 'gtsi', /*  0x67747369  */
  kAEGreaterThan                = '>   ', /*  0x3e202020  */
  kAEGreaterThanEquals          = '>=  ', /*  0x3e3d2020  */
  kAEGrow                       = 'grow', /*  0x67726f77  */
  kAEHidden                     = 'hidn', /*  0x6869646e  */
  kAEHiQuality                  = 'hiqu', /*  0x68697175  */
  kAEImageGraphic               = 'imgr', /*  0x696d6772  */
  kAEIsUniform                  = 'isun', /*  0x6973756e  */
  kAEItalic                     = 'ital', /*  0x6974616c  */
  kAELeftJustified              = 'left', /*  0x6c656674  */
  kAELessThan                   = '<   ', /*  0x3c202020  */
  kAELessThanEquals             = '<=  ', /*  0x3c3d2020  */
  kAELowercase                  = 'lowc', /*  0x6c6f7763  */
  kAEMakeObjectsVisible         = 'mvis', /*  0x6d766973  */
  kAEMiscStandards              = 'misc', /*  0x6d697363  */
  kAEModifiable                 = 'modf', /*  0x6d6f6466  */
  kAEMove                       = 'move', /*  0x6d6f7665  */
  kAENo                         = 'no  ', /*  0x6e6f2020  */
  kAENoArrow                    = 'arno' /*  0x61726e6f  */
};

CF_ENUM(OSType) {
  kAENonmodifiable              = 'nmod', /*  0x6e6d6f64  */
  kAEOpen                       = 'odoc', /*  0x6f646f63  */
  kAEOpenSelection              = 'sope', /*  0x736f7065  */
  kAEOutline                    = 'outl', /*  0x6f75746c  */
  kAEPageSetup                  = 'pgsu', /*  0x70677375  */
  kAEPaste                      = 'past', /*  0x70617374  */
  kAEPlain                      = 'plan', /*  0x706c616e  */
  kAEPrint                      = 'pdoc', /*  0x70646f63  */
  kAEPrintSelection             = 'spri', /*  0x73707269  */
  kAEPrintWindow                = 'pwin', /*  0x7077696e  */
  kAEPutAwaySelection           = 'sput', /*  0x73707574  */
  kAEQDAddOver                  = 'addo', /*  0x6164646f  */
  kAEQDAddPin                   = 'addp', /*  0x61646470  */
  kAEQDAdMax                    = 'admx', /*  0x61646d78  */
  kAEQDAdMin                    = 'admn', /*  0x61646d6e  */
  kAEQDBic                      = 'bic ', /*  0x62696320  */
  kAEQDBlend                    = 'blnd', /*  0x626c6e64  */
  kAEQDCopy                     = 'cpy ', /*  0x63707920  */
  kAEQDNotBic                   = 'nbic', /*  0x6e626963  */
  kAEQDNotCopy                  = 'ncpy' /*  0x6e637079  */
};

CF_ENUM(OSType) {
  kAEQDNotOr                    = 'ntor', /*  0x6e746f72  */
  kAEQDNotXor                   = 'nxor', /*  0x6e786f72  */
  kAEQDOr                       = 'or  ', /*  0x6f722020  */
  kAEQDSubOver                  = 'subo', /*  0x7375626f  */
  kAEQDSubPin                   = 'subp', /*  0x73756270  */
  kAEQDSupplementalSuite        = 'qdsp', /*  0x71647370  */
  kAEQDXor                      = 'xor ', /*  0x786f7220  */
  kAEQuickdrawSuite             = 'qdrw', /*  0x71647277  */
  kAEQuitAll                    = 'quia', /*  0x71756961  */
  kAERedo                       = 'redo', /*  0x7265646f  */
  kAERegular                    = 'regl', /*  0x7265676c  */
  kAEReopenApplication          = 'rapp', /*  0x72617070  */
  kAEReplace                    = 'rplc', /*  0x72706c63  */
  kAERequiredSuite              = 'reqd', /*  0x72657164  */
  kAERestart                    = 'rest', /*  0x72657374  */
  kAERevealSelection            = 'srev', /*  0x73726576  */
  kAERevert                     = 'rvrt', /*  0x72767274  */
  kAERightJustified             = 'rght', /*  0x72676874  */
  kAESave                       = 'save', /*  0x73617665  */
  kAESelect                     = 'slct', /*  0x736c6374  */
  kAESetData                    = 'setd' /*  0x73657464  */
};

CF_ENUM(OSType) {
  kAESetPosition                = 'posn', /*  0x706f736e  */
  kAEShadow                     = 'shad', /*  0x73686164  */
  kAEShowClipboard              = 'shcl', /*  0x7368636c  */
  kAEShutDown                   = 'shut', /*  0x73687574  */
  kAESleep                      = 'slep', /*  0x736c6570  */
  kAESmallCaps                  = 'smcp', /*  0x736d6370  */
  kAESpecialClassProperties     = 'c@#!', /*  0x63402321  */
  kAEStrikethrough              = 'strk', /*  0x7374726b  */
  kAESubscript                  = 'sbsc', /*  0x73627363  */
  kAESuperscript                = 'spsc', /*  0x73707363  */
  kAETableSuite                 = 'tbls', /*  0x74626c73  */
  kAETextSuite                  = 'TEXT', /*  0x54455854  */
  kAETransactionTerminated      = 'ttrm', /*  0x7474726d  */
  kAEUnderline                  = 'undl', /*  0x756e646c  */
  kAEUndo                       = 'undo', /*  0x756e646f  */
  kAEWholeWordEquals            = 'wweq', /*  0x77776571  */
  kAEYes                        = 'yes ', /*  0x79657320  */
  kAEZoom                       = 'zoom' /*  0x7a6f6f6d  */
};

/* events that can be sent to the "system" process (eg, loginwindow) on OS X 10.2 or later */
CF_ENUM(OSType) {
  kAELogOut                     = 'logo',
  kAEReallyLogOut               = 'rlgo',
  kAEShowRestartDialog          = 'rrst',
  kAEShowShutdownDialog         = 'rsdn'
};

/* EventRecord Classes and EventIDs */
CF_ENUM(OSType) {
  kAEMouseClass                 = 'mous',
  kAEDown                       = 'down',
  kAEUp                         = 'up  ',
  kAEMoved                      = 'move',
  kAEStoppedMoving              = 'stop',
  kAEWindowClass                = 'wind',
  kAEUpdate                     = 'updt',
  kAEActivate                   = 'actv',
  kAEDeactivate                 = 'dact',
  kAECommandClass               = 'cmnd', /* Modern Command Event Class */
  kAEKeyClass                   = 'keyc',
  kAERawKey                     = 'rkey', /* Modern Raw Key Event */
  kAEVirtualKey                 = 'keyc', /* Modern Virtual Key Event */
  kAENavigationKey              = 'nave', /* Modern Navigation Key Event */
  kAEAutoDown                   = 'auto',
  kAEApplicationClass           = 'appl',
  kAESuspend                    = 'susp',
  kAEResume                     = 'rsme',
  kAEDiskEvent                  = 'disk',
  kAENullEvent                  = 'null',
  kAEWakeUpEvent                = 'wake',
  kAEScrapEvent                 = 'scrp',
  kAEHighLevel                  = 'high'
};

CF_ENUM(AEKeyword) {
  keyAEAngle                    = 'kang', /*  0x6b616e67  */
  keyAEArcAngle                 = 'parc' /*  0x70617263  */
};

CF_ENUM(AEKeyword) {
  keyAEBaseAddr                 = 'badd', /*  0x62616464  */
  keyAEBestType                 = 'pbst', /*  0x70627374  */
  keyAEBgndColor                = 'kbcl', /*  0x6b62636c  */
  keyAEBgndPattern              = 'kbpt', /*  0x6b627074  */
  keyAEBounds                   = 'pbnd', /*  0x70626e64  */
  keyAECellList                 = 'kclt', /*  0x6b636c74  */
  keyAEClassID                  = 'clID', /*  0x636c4944  */
  keyAEColor                    = 'colr', /*  0x636f6c72  */
  keyAEColorTable               = 'cltb', /*  0x636c7462  */
  keyAECurveHeight              = 'kchd', /*  0x6b636864  */
  keyAECurveWidth               = 'kcwd', /*  0x6b637764  */
  keyAEDashStyle                = 'pdst', /*  0x70647374  */
  keyAEData                     = 'data', /*  0x64617461  */
  keyAEDefaultType              = 'deft', /*  0x64656674  */
  keyAEDefinitionRect           = 'pdrt', /*  0x70647274  */
  keyAEDescType                 = 'dstp', /*  0x64737470  */
  keyAEDestination              = 'dest', /*  0x64657374  */
  keyAEDoAntiAlias              = 'anta', /*  0x616e7461  */
  keyAEDoDithered               = 'gdit', /*  0x67646974  */
  keyAEDoRotate                 = 'kdrt' /*  0x6b647274  */
};

CF_ENUM(AEKeyword) {
  keyAEDoScale                  = 'ksca', /*  0x6b736361  */
  keyAEDoTranslate              = 'ktra', /*  0x6b747261  */
  keyAEEditionFileLoc           = 'eloc', /*  0x656c6f63  */
  keyAEElements                 = 'elms', /*  0x656c6d73  */
  keyAEEndPoint                 = 'pend', /*  0x70656e64  */
  keyAEEventClass               = 'evcl', /*  0x6576636c  */
  keyAEEventID                  = 'evti', /*  0x65767469  */
  keyAEFile                     = 'kfil', /*  0x6b66696c  */
  keyAEFileType                 = 'fltp', /*  0x666c7470  */
  keyAEFillColor                = 'flcl', /*  0x666c636c  */
  keyAEFillPattern              = 'flpt', /*  0x666c7074  */
  keyAEFlipHorizontal           = 'kfho', /*  0x6b66686f  */
  keyAEFlipVertical             = 'kfvt', /*  0x6b667674  */
  keyAEFont                     = 'font', /*  0x666f6e74  */
  keyAEFormula                  = 'pfor', /*  0x70666f72  */
  keyAEGraphicObjects           = 'gobs', /*  0x676f6273  */
  keyAEID                       = 'ID  ', /*  0x49442020  */
  keyAEImageQuality             = 'gqua', /*  0x67717561  */
  keyAEInsertHere               = 'insh', /*  0x696e7368  */
  keyAEKeyForms                 = 'keyf' /*  0x6b657966  */
};

CF_ENUM(AEKeyword) {
  keyAEKeyword                  = 'kywd', /*  0x6b797764  */
  keyAELevel                    = 'levl', /*  0x6c65766c  */
  keyAELineArrow                = 'arro', /*  0x6172726f  */
  keyAEName                     = 'pnam', /*  0x706e616d  */
  keyAENewElementLoc            = 'pnel', /*  0x706e656c  */
  keyAEObject                   = 'kobj', /*  0x6b6f626a  */
  keyAEObjectClass              = 'kocl', /*  0x6b6f636c  */
  keyAEOffStyles                = 'ofst', /*  0x6f667374  */
  keyAEOnStyles                 = 'onst', /*  0x6f6e7374  */
  keyAEParameters               = 'prms', /*  0x70726d73  */
  keyAEParamFlags               = 'pmfg', /*  0x706d6667  */
  keyAEPenColor                 = 'ppcl', /*  0x7070636c  */
  keyAEPenPattern               = 'pppa', /*  0x70707061  */
  keyAEPenWidth                 = 'ppwd', /*  0x70707764  */
  keyAEPixelDepth               = 'pdpt', /*  0x70647074  */
  keyAEPixMapMinus              = 'kpmm', /*  0x6b706d6d  */
  keyAEPMTable                  = 'kpmt', /*  0x6b706d74  */
  keyAEPointList                = 'ptlt', /*  0x70746c74  */
  keyAEPointSize                = 'ptsz', /*  0x7074737a  */
  keyAEPosition                 = 'kpos' /*  0x6b706f73  */
};

CF_ENUM(AEKeyword) {
  keyAEPropData                 = 'prdt', /*  0x70726474  */
  keyAEProperties               = 'qpro', /*  0x7170726f  */
  keyAEProperty                 = 'kprp', /*  0x6b707270  */
  keyAEPropFlags                = 'prfg', /*  0x70726667  */
  keyAEPropID                   = 'prop', /*  0x70726f70  */
  keyAEProtection               = 'ppro', /*  0x7070726f  */
  keyAERenderAs                 = 'kren', /*  0x6b72656e  */
  keyAERequestedType            = 'rtyp', /*  0x72747970  */
  keyAEResult                   = '----', /*  0x2d2d2d2d  */
  keyAEResultInfo               = 'rsin', /*  0x7273696e  */
  keyAERotation                 = 'prot', /*  0x70726f74  */
  keyAERotPoint                 = 'krtp', /*  0x6b727470  */
  keyAERowList                  = 'krls', /*  0x6b726c73  */
  keyAESaveOptions              = 'savo', /*  0x7361766f  */
  keyAEScale                    = 'pscl', /*  0x7073636c  */
  keyAEScriptTag                = 'psct', /*  0x70736374  */
  keyAESearchText               = 'stxt', /*  0x73747874  */
  keyAEShowWhere                = 'show', /*  0x73686f77  */
  keyAEStartAngle               = 'pang', /*  0x70616e67  */
  keyAEStartPoint               = 'pstp', /*  0x70737470  */
  keyAEStyles                   = 'ksty' /*  0x6b737479  */
};

CF_ENUM(AEKeyword) {
  keyAESuiteID                  = 'suit', /*  0x73756974  */
  keyAEText                     = 'ktxt', /*  0x6b747874  */
  keyAETextColor                = 'ptxc', /*  0x70747863  */
  keyAETextFont                 = 'ptxf', /*  0x70747866  */
  keyAETextPointSize            = 'ptps', /*  0x70747073  */
  keyAETextStyles               = 'txst', /*  0x74787374  */
  keyAETextLineHeight           = 'ktlh', /* type ShortInteger */
  keyAETextLineAscent           = 'ktas', /* type ShortInteger */
  keyAETheText                  = 'thtx', /*  0x74687478  */
  keyAETransferMode             = 'pptm', /*  0x7070746d  */
  keyAETranslation              = 'ptrs', /*  0x70747273  */
  keyAETryAsStructGraf          = 'toog', /*  0x746f6f67  */
  keyAEUniformStyles            = 'ustl', /*  0x7573746c  */
  keyAEUpdateOn                 = 'pupd', /*  0x70757064  */
  keyAEUserTerm                 = 'utrm', /*  0x7574726d  */
  keyAEWindow                   = 'wndw', /*  0x776e6477  */
  keyAEWritingCode              = 'wrcd' /*  0x77726364  */
};

CF_ENUM(AEKeyword) {
  keyMiscellaneous              = 'fmsc', /*  0x666d7363  */
  keySelection                  = 'fsel', /*  0x6673656c  */
  keyWindow                     = 'kwnd', /*  0x6b776e64  */
                                        /* EventRecord keys */
  keyWhen                       = 'when',
  keyWhere                      = 'wher',
  keyModifiers                  = 'mods',
  keyKey                        = 'key ',
  keyKeyCode                    = 'code',
  keyKeyboard                   = 'keyb',
  keyDriveNumber                = 'drv#',
  keyErrorCode                  = 'err#',
  keyHighLevelClass             = 'hcls',
  keyHighLevelID                = 'hid '
};

CF_ENUM(OSType) {
  pArcAngle                     = 'parc', /*  0x70617263  */
  pBackgroundColor              = 'pbcl', /*  0x7062636c  */
  pBackgroundPattern            = 'pbpt', /*  0x70627074  */
  pBestType                     = 'pbst', /*  0x70627374  */
  pBounds                       = 'pbnd', /*  0x70626e64  */
  pClass                        = 'pcls', /*  0x70636c73  */
  pClipboard                    = 'pcli', /*  0x70636c69  */
  pColor                        = 'colr', /*  0x636f6c72  */
  pColorTable                   = 'cltb', /*  0x636c7462  */
  pContents                     = 'pcnt', /*  0x70636e74  */
  pCornerCurveHeight            = 'pchd', /*  0x70636864  */
  pCornerCurveWidth             = 'pcwd', /*  0x70637764  */
  pDashStyle                    = 'pdst', /*  0x70647374  */
  pDefaultType                  = 'deft', /*  0x64656674  */
  pDefinitionRect               = 'pdrt', /*  0x70647274  */
  pEnabled                      = 'enbl', /*  0x656e626c  */
  pEndPoint                     = 'pend', /*  0x70656e64  */
  pFillColor                    = 'flcl', /*  0x666c636c  */
  pFillPattern                  = 'flpt', /*  0x666c7074  */
  pFont                         = 'font' /*  0x666f6e74  */
};

CF_ENUM(OSType) {
  pFormula                      = 'pfor', /*  0x70666f72  */
  pGraphicObjects               = 'gobs', /*  0x676f6273  */
  pHasCloseBox                  = 'hclb', /*  0x68636c62  */
  pHasTitleBar                  = 'ptit', /*  0x70746974  */
  pID                           = 'ID  ', /*  0x49442020  */
  pIndex                        = 'pidx', /*  0x70696478  */
  pInsertionLoc                 = 'pins', /*  0x70696e73  */
  pIsFloating                   = 'isfl', /*  0x6973666c  */
  pIsFrontProcess               = 'pisf', /*  0x70697366  */
  pIsModal                      = 'pmod', /*  0x706d6f64  */
  pIsModified                   = 'imod', /*  0x696d6f64  */
  pIsResizable                  = 'prsz', /*  0x7072737a  */
  pIsStationeryPad              = 'pspd', /*  0x70737064  */
  pIsZoomable                   = 'iszm', /*  0x69737a6d  */
  pIsZoomed                     = 'pzum', /*  0x707a756d  */
  pItemNumber                   = 'itmn', /*  0x69746d6e  */
  pJustification                = 'pjst', /*  0x706a7374  */
  pLineArrow                    = 'arro', /*  0x6172726f  */
  pMenuID                       = 'mnid', /*  0x6d6e6964  */
  pName                         = 'pnam' /*  0x706e616d  */
};

CF_ENUM(OSType) {
  pNewElementLoc                = 'pnel', /*  0x706e656c  */
  pPenColor                     = 'ppcl', /*  0x7070636c  */
  pPenPattern                   = 'pppa', /*  0x70707061  */
  pPenWidth                     = 'ppwd', /*  0x70707764  */
  pPixelDepth                   = 'pdpt', /*  0x70647074  */
  pPointList                    = 'ptlt', /*  0x70746c74  */
  pPointSize                    = 'ptsz', /*  0x7074737a  */
  pProtection                   = 'ppro', /*  0x7070726f  */
  pRotation                     = 'prot', /*  0x70726f74  */
  pScale                        = 'pscl', /*  0x7073636c  */
  pScript                       = 'scpt', /*  0x73637074  */
  pScriptTag                    = 'psct', /*  0x70736374  */
  pSelected                     = 'selc', /*  0x73656c63  */
  pSelection                    = 'sele', /*  0x73656c65  */
  pStartAngle                   = 'pang', /*  0x70616e67  */
  pStartPoint                   = 'pstp', /*  0x70737470  */
  pTextColor                    = 'ptxc', /*  0x70747863  */
  pTextFont                     = 'ptxf', /*  0x70747866  */
  pTextItemDelimiters           = 'txdl', /*  0x7478646c  */
  pTextPointSize                = 'ptps' /*  0x70747073  */
};

CF_ENUM(OSType) {
  pTextStyles                   = 'txst', /*  0x74787374  */
  pTransferMode                 = 'pptm', /*  0x7070746d  */
  pTranslation                  = 'ptrs', /*  0x70747273  */
  pUniformStyles                = 'ustl', /*  0x7573746c  */
  pUpdateOn                     = 'pupd', /*  0x70757064  */
  pUserSelection                = 'pusl', /*  0x7075736c  */
  pVersion                      = 'vers', /*  0x76657273  */
  pVisible                      = 'pvis' /*  0x70766973  */
};

CF_ENUM(DescType) {
  typeAEText                    = 'tTXT', /*  0x74545854  */
  typeArc                       = 'carc', /*  0x63617263  */
  typeBest                      = 'best', /*  0x62657374  */
  typeCell                      = 'ccel', /*  0x6363656c  */
  typeClassInfo                 = 'gcli', /*  0x67636c69  */
  typeColorTable                = 'clrt', /*  0x636c7274  */
  typeColumn                    = 'ccol', /*  0x63636f6c  */
  typeDashStyle                 = 'tdas', /*  0x74646173  */
  typeData                      = 'tdta', /*  0x74647461  */
  typeDrawingArea               = 'cdrw', /*  0x63647277  */
  typeElemInfo                  = 'elin', /*  0x656c696e  */
  typeEnumeration               = 'enum', /*  0x656e756d  */
  typeEPS                       = 'EPS ', /*  0x45505320  */
  typeEventInfo                 = 'evin' /*  0x6576696e  */
};

CF_ENUM(DescType) {
  typeFinderWindow              = 'fwin', /*  0x6677696e  */
  typeFixedPoint                = 'fpnt', /*  0x66706e74  */
  typeFixedRectangle            = 'frct', /*  0x66726374  */
  typeGraphicLine               = 'glin', /*  0x676c696e  */
  typeGraphicText               = 'cgtx', /*  0x63677478  */
  typeGroupedGraphic            = 'cpic', /*  0x63706963  */
  typeInsertionLoc              = 'insl', /*  0x696e736c  */
  typeIntlText                  = 'itxt', /*  0x69747874  */
  typeIntlWritingCode           = 'intl', /*  0x696e746c  */
  typeLongDateTime              = 'ldt ', /*  0x6c647420  */
  typeCFAbsoluteTime            = 'cfat',
  typeISO8601DateTime           = 'isot', /*  0x69736f74  data is ascii text of an ISO8601 date */
  typeLongFixed                 = 'lfxd', /*  0x6c667864  */
  typeLongFixedPoint            = 'lfpt', /*  0x6c667074  */
  typeLongFixedRectangle        = 'lfrc', /*  0x6c667263  */
  typeLongPoint                 = 'lpnt', /*  0x6c706e74  */
  typeLongRectangle             = 'lrct', /*  0x6c726374  */
  typeMachineLoc                = 'mLoc', /*  0x6d4c6f63  */
  typeOval                      = 'covl', /*  0x636f766c  */
  typeParamInfo                 = 'pmin', /*  0x706d696e  */
  typePict                      = 'PICT' /*  0x50494354  */
};

CF_ENUM(DescType) {
  typePixelMap                  = 'cpix', /*  0x63706978  */
  typePixMapMinus               = 'tpmm', /*  0x74706d6d  */
  typePolygon                   = 'cpgn', /*  0x6370676e  */
  typePropInfo                  = 'pinf', /*  0x70696e66  */
  typePtr                       = 'ptr ', /*  0x70747220  */
  typeQDPoint                   = 'QDpt', /*  0x51447074  */
  typeQDRegion                  = 'Qrgn', /*  0x51447074  (data is actual region data, including rectangle and size, _not_ region handle or ptr)*/
  typeRectangle                 = 'crec', /*  0x63726563  */
  typeRGB16                     = 'tr16', /*  0x74723136  */
  typeRGB96                     = 'tr96', /*  0x74723936  */
  typeRGBColor                  = 'cRGB', /*  0x63524742  */
  typeRotation                  = 'trot', /*  0x74726f74  */
  typeRoundedRectangle          = 'crrc', /*  0x63727263  */
  typeRow                       = 'crow', /*  0x63726f77  */
  typeScrapStyles               = 'styl', /*  0x7374796c  */
  typeScript                    = 'scpt', /*  0x73637074  */
  typeStyledText                = 'STXT', /*  0x53545854  */
  typeSuiteInfo                 = 'suin', /*  0x7375696e  */
  typeTable                     = 'ctbl', /*  0x6374626c  */
  typeTextStyles                = 'tsty' /*  0x74737479  */
};

CF_ENUM(DescType) {
  typeTIFF                      = 'TIFF', /*  0x54494646  */
  typeJPEG                      = 'JPEG',
  typeGIF                       = 'GIFf',
  typeVersion                   = 'vers' /*  0x76657273  */
};

CF_ENUM(OSType) {
  kAEMenuClass                  = 'menu',
  kAEMenuSelect                 = 'mhit',
  kAEMouseDown                  = 'mdwn',
  kAEMouseDownInBack            = 'mdbk',
  kAEKeyDown                    = 'kdwn',
  kAEResized                    = 'rsiz',
  kAEPromise                    = 'prom'
};

CF_ENUM(AEKeyword) {
  keyMenuID                     = 'mid ',
  keyMenuItem                   = 'mitm',
  keyCloseAllWindows            = 'caw ',
  keyOriginalBounds             = 'obnd',
  keyNewBounds                  = 'nbnd',
  keyLocalWhere                 = 'lwhr'
};

CF_ENUM(DescType) {
  typeHIMenu                    = 'mobj',
  typeHIWindow                  = 'wobj'
};

CF_ENUM(OSType) {
  kAEQuitPreserveState          = 'stat', /* in a kAEQuitApplication event, this optional parameter hints the application as to whether it should write out */
                                        /* persistent state which may be restored on the next launch. The possible values are kAENo ( the default ), or kAEYes */
  kAEQuitReason                 = 'why?' /* in a kAEQuitApplication event, this parameter if present is the reason the quit is being sent.  The possible values are kAEQuitAll, kAEShutDown, kAERestart, kAEReallyLogOut */
};

enum {
  kBySmallIcon                  = 0,
  kByIconView                   = 1,
  kByNameView                   = 2,
  kByDateView                   = 3,
  kBySizeView                   = 4,
  kByKindView                   = 5,
  kByCommentView                = 6,
  kByLabelView                  = 7,
  kByVersionView                = 8
};

enum {
  kAEInfo                       = 11,
  kAEMain                       = 0,
  kAESharing                    = 13
};

enum {
  kAEZoomIn                     = 7,
  kAEZoomOut                    = 8
};

CF_ENUM(OSType) {
  kTextServiceClass             = 'tsvc',
  kUpdateActiveInputArea        = 'updt', /* update the active input area */
  kShowHideInputWindow          = 'shiw', /* show or hide the input window */
  kPos2Offset                   = 'p2st', /* converting global coordinates to char position */
  kOffset2Pos                   = 'st2p', /* converting char position to global coordinates */
  kUnicodeNotFromInputMethod    = 'unim', /* Unicode text when event not handled by Input Method or no Input Method */
  kGetSelectedText              = 'gtxt', /* Get text for current selection */
  keyAETSMDocumentRefcon        = 'refc', /* TSM document refcon, typeLongInteger */
  keyAEServerInstance           = 'srvi', /* component instance */
  keyAETheData                  = 'kdat', /* typeText */
  keyAEFixLength                = 'fixl', /* fix len */
  keyAEUpdateRange              = 'udng', /* typeTextRangeArray */
  keyAECurrentPoint             = 'cpos', /* current point */
  keyAEBufferSize               = 'buff', /* buffer size to get the text */
  keyAEMoveView                 = 'mvvw', /* move view flag */
  keyAENextBody                 = 'nxbd', /* next or previous body */
  keyAETSMScriptTag             = 'sclg',
  keyAETSMTextFont              = 'ktxf', /* FMFontFamily or FOND ID */
  keyAETSMTextFMFont            = 'ktxm', /* FMFont */
  keyAETSMTextPointSize         = 'ktps',
  keyAETSMEventRecord           = 'tevt', /* Low level Event Record, typeLowLevelEventRecord */
  keyAETSMEventRef              = 'tevr', /* Carbon EventRef, typeEventRef */
  keyAETextServiceEncoding      = 'tsen', /* Text Service encoding, mac or Unicode in UpdateActiveInputArea or GetSelectedText events. */
  keyAETextServiceMacEncoding   = 'tmen', /* Target mac encoding for TSM conversion of text from Unicode text service. */
  keyAETSMGlyphInfoArray        = 'tgia', /* typeGlyphInfoArray */
  typeTextRange                 = 'txrn', /* TextRange */
  typeComponentInstance         = 'cmpi', /* server instance */
  typeOffsetArray               = 'ofay', /* offset array */
  typeTextRangeArray            = 'tray',
  typeLowLevelEventRecord       = 'evtr', /* Low Level Event Record */
  typeGlyphInfoArray            = 'glia', /* Glyph/FMFont info array for sub ranges of Unicode text.  See GlyphInfoArray in TextServices.h  */
  typeEventRef                  = 'evrf', /* Carbon EventRef */
  typeText                      = typeChar /* Plain text */
};


/* Desc type constants */
enum {
  kTSMOutsideOfBody             = 1,
  kTSMInsideOfBody              = 2,
  kTSMInsideOfActiveInputArea   = 3
};

enum {
  kNextBody                     = 1,
  kPreviousBody                 = 2
};

struct TextRange {
  SInt32              fStart;
  SInt32              fEnd;
  SInt16              fHiliteStyle;
};
typedef struct TextRange                TextRange;
typedef TextRange *                     TextRangePtr;
typedef TextRangePtr *                  TextRangeHandle;
struct TextRangeArray {
  SInt16              fNumOfRanges;           /* specify the size of the fRange array */
  TextRange           fRange[1];              /* when fNumOfRanges > 1, the size of this array has to be calculated */
};
typedef struct TextRangeArray           TextRangeArray;
typedef TextRangeArray *                TextRangeArrayPtr;
typedef TextRangeArrayPtr *             TextRangeArrayHandle;
struct OffsetArray {
  SInt16              fNumOfOffsets;          /* specify the size of the fOffset array */
  SInt32              fOffset[1];             /* when fNumOfOffsets > 1, the size of this array has to be calculated */
};
typedef struct OffsetArray              OffsetArray;
typedef OffsetArray *                   OffsetArrayPtr;
typedef OffsetArrayPtr *                OffsetArrayHandle;
struct WritingCode {
  ScriptCode          theScriptCode;
  LangCode            theLangCode;
};
typedef struct WritingCode              WritingCode;
struct IntlText {
  ScriptCode          theScriptCode;
  LangCode            theLangCode;
  char                theText[1];             /* variable length data */
};
typedef struct IntlText                 IntlText;

/* Hilite styles */
enum {
  kTSMHiliteCaretPosition       = 1,    /* specify caret position */
  kTSMHiliteRawText             = 2,    /* specify range of raw text */
  kTSMHiliteSelectedRawText     = 3,    /* specify range of selected raw text */
  kTSMHiliteConvertedText       = 4,    /* specify range of converted text */
  kTSMHiliteSelectedConvertedText = 5,  /* specify range of selected converted text */
  kTSMHiliteBlockFillText       = 6,    /* Block Fill hilite style */
  kTSMHiliteOutlineText         = 7,    /* Outline hilite style */
  kTSMHiliteSelectedText        = 8,    /* Selected hilite style */
  kTSMHiliteNoHilite            = 9     /* specify range of non-hilited text */
};

#if OLDROUTINENAMES
/* Hilite styles */
enum {
  kCaretPosition                = kTSMHiliteCaretPosition,
  kRawText                      = kTSMHiliteRawText,
  kSelectedRawText              = kTSMHiliteSelectedRawText,
  kConvertedText                = kTSMHiliteConvertedText,
  kSelectedConvertedText        = kTSMHiliteSelectedConvertedText,
  kBlockFillText                = kTSMHiliteBlockFillText,
  kOutlineText                  = kTSMHiliteOutlineText,
  kSelectedText                 = kTSMHiliteSelectedText
};

#endif  /* OLDROUTINENAMES */

CF_ENUM(AEKeyword) {
  keyAEHiliteRange              = 'hrng', /* typeTextRangeArray for System 7, typeHiliteRangeArray for System 8 */
  keyAEPinRange                 = 'pnrg', /* typeTextRange for System 7, typeTextRegionRange for System 8   */
  keyAEClauseOffsets            = 'clau', /* typeOffsetArray for System 7, typeClauseOffsetArray for System 8 */
  keyAEOffset                   = 'ofst', /* typeLongInteger for System 7, typeByteOffset for System 8  */
  keyAEPoint                    = 'gpos', /* typePoint for System 7, typeQDPoint for System 8 */
  keyAELeftSide                 = 'klef', /* typeBoolean */
  keyAERegionClass              = 'rgnc', /* typeShortInteger for System 7, typeRegionClass for System 8 */
  keyAEDragging                 = 'bool' /* typeBoolean */
};


#if OLDROUTINENAMES
enum {
  keyAELeadingEdge              = keyAELeftSide
};

#endif  /* OLDROUTINENAMES */

CF_ENUM(DescType) {
                                        /* AppleScript 1.3: Unit types */
  typeMeters                    = 'metr', /* Base Unit */
  typeInches                    = 'inch',
  typeFeet                      = 'feet',
  typeYards                     = 'yard',
  typeMiles                     = 'mile',
  typeKilometers                = 'kmtr',
  typeCentimeters               = 'cmtr',
  typeSquareMeters              = 'sqrm', /* Base Unit */
  typeSquareFeet                = 'sqft',
  typeSquareYards               = 'sqyd',
  typeSquareMiles               = 'sqmi',
  typeSquareKilometers          = 'sqkm',
  typeLiters                    = 'litr', /* Base Unit */
  typeQuarts                    = 'qrts',
  typeGallons                   = 'galn',
  typeCubicMeters               = 'cmet', /* Base Unit */
  typeCubicFeet                 = 'cfet',
  typeCubicInches               = 'cuin',
  typeCubicCentimeter           = 'ccmt',
  typeCubicYards                = 'cyrd',
  typeKilograms                 = 'kgrm', /* Base Unit */
  typeGrams                     = 'gram',
  typeOunces                    = 'ozs ',
  typePounds                    = 'lbs ',
  typeDegreesC                  = 'degc', /* Base Unit */
  typeDegreesF                  = 'degf',
  typeDegreesK                  = 'degk'
};

CF_ENUM(OSType) {
                                        /* AppleScript 1.3: Folder Actions */
  kFAServerApp                  = 'ssrv', /* Creator code for Folder Actions Server*/
  kDoFolderActionEvent          = 'fola', /* Event the Finder sends to the Folder Actions FBA*/
  kFolderActionCode             = 'actn', /* Parameter that contains the Folder Action*/
  kFolderOpenedEvent            = 'fopn', /* Value of kFolderActionCode parameter; sent to script as event*/
  kFolderClosedEvent            = 'fclo',
  kFolderWindowMovedEvent       = 'fsiz',
  kFolderItemsAddedEvent        = 'fget',
  kFolderItemsRemovedEvent      = 'flos',
  kItemList                     = 'flst', /* List parameter for added and removed items*/
  kNewSizeParameter             = 'fnsz', /* Parameter for moved window*/
  kFASuiteCode                  = 'faco', /* Suite code for the following events*/
  kFAAttachCommand              = 'atfa', /* Attach event id*/
  kFARemoveCommand              = 'rmfa', /* Remove event id*/
  kFAEditCommand                = 'edfa', /* Edit event id*/
  kFAFileParam                  = 'faal', /* Key for file parameter for Attach*/
  kFAIndexParam                 = 'indx' /* Key for index (0-based) parameter for Remove and Edit*/
};

/* AppleScript 1.3 Internet Suite */
CF_ENUM(OSType) {
                                        /* Suite code */
  kAEInternetSuite              = 'gurl',
  kAEISWebStarSuite             = 0x575757BD
};

CF_ENUM(OSType) {
                                        /* Events */
  kAEISGetURL                   = 'gurl',
  KAEISHandleCGI                = 'sdoc'
};

CF_ENUM(OSType) {
                                        /* Classes */
  cURL                          = 'url ',
  cInternetAddress              = 'IPAD',
  cHTML                         = 'html',
  cFTPItem                      = 'ftp '
};

CF_ENUM(OSType) {
                                        /* Parameters */
  kAEISHTTPSearchArgs           = 'kfor',
  kAEISPostArgs                 = 'post',
  kAEISMethod                   = 'meth',
  kAEISClientAddress            = 'addr',
  kAEISUserName                 = 'user',
  kAEISPassword                 = 'pass',
  kAEISFromUser                 = 'frmu',
  kAEISServerName               = 'svnm',
  kAEISServerPort               = 'svpt',
  kAEISScriptName               = 'scnm',
  kAEISContentType              = 'ctyp',
  kAEISReferrer                 = 'refr',
  kAEISUserAgent                = 'Agnt',
  kAEISAction                   = 'Kact',
  kAEISActionPath               = 'Kapt',
  kAEISClientIP                 = 'Kcip',
  kAEISFullRequest              = 'Kfrq'
};

CF_ENUM(OSType) {
                                        /* Properties */
  pScheme                       = 'pusc',
  pHost                         = 'HOST',
  pPath                         = 'FTPc',
  pUserName                     = 'RAun',
  pUserPassword                 = 'RApw',
  pDNSForm                      = 'pDNS',
  pURL                          = 'pURL',
  pTextEncoding                 = 'ptxe',
  pFTPKind                      = 'kind'
};

CF_ENUM(OSType) {
                                        /* Scheme enumerations */
  eScheme                       = 'esch',
  eurlHTTP                      = 'http', /* RFC 2068 */
  eurlHTTPS                     = 'htps',
  eurlFTP                       = 'ftp ', /* RFC 1738 */
  eurlMail                      = 'mail', /* RFC 2638 */
  eurlFile                      = 'file', /* RFC 1738 */
  eurlGopher                    = 'gphr', /* RFC 1738 */
  eurlTelnet                    = 'tlnt', /* RFC 1738 */
  eurlNews                      = 'news', /* RFC 1738 */
  eurlSNews                     = 'snws',
  eurlNNTP                      = 'nntp', /* RFC 1738 */
  eurlMessage                   = 'mess',
  eurlMailbox                   = 'mbox',
  eurlMulti                     = 'mult',
  eurlLaunch                    = 'laun',
  eurlAFP                       = 'afp ',
  eurlAT                        = 'at  ',
  eurlEPPC                      = 'eppc',
  eurlRTSP                      = 'rtsp', /* RFC 2326 */
  eurlIMAP                      = 'imap', /* RFC 2192 */
  eurlNFS                       = 'unfs', /* RFC 2224 */
  eurlPOP                       = 'upop', /* RFC 2384 */
  eurlLDAP                      = 'uldp', /* RFC 2255 */
  eurlUnknown                   = 'url?'
};

CF_ENUM(OSType) {
                                        /* AppleScript 1.3: Connectivity Suite in aeut */
  kConnSuite                    = 'macc',
  cDevSpec                      = 'cdev',
  cAddressSpec                  = 'cadr',
  cADBAddress                   = 'cadb',
  cAppleTalkAddress             = 'cat ',
  cBusAddress                   = 'cbus',
  cEthernetAddress              = 'cen ',
  cFireWireAddress              = 'cfw ',
  cIPAddress                    = 'cip ',
  cLocalTalkAddress             = 'clt ',
  cSCSIAddress                  = 'cscs',
  cTokenRingAddress             = 'ctok',
  cUSBAddress                   = 'cusb', /* */
                                        /* Properties */
  pDeviceType                   = 'pdvt',
  pDeviceAddress                = 'pdva',
  pConduit                      = 'pcon',
  pProtocol                     = 'pprt', /* cde 4/27/98 was 'ppro' conflicted with DB suite */
  pATMachine                    = 'patm',
  pATZone                       = 'patz',
  pATType                       = 'patt',
  pDottedDecimal                = 'pipd',
  pDNS                          = 'pdns',
  pPort                         = 'ppor',
  pNetwork                      = 'pnet',
  pNode                         = 'pnod',
  pSocket                       = 'psoc',
  pSCSIBus                      = 'pscb',
  pSCSILUN                      = 'pslu', /* cde 5/22/98 per WWDC developer request */
                                        /* Enumerations and enumerators */
  eDeviceType                   = 'edvt',
  eAddressSpec                  = 'eads',
  eConduit                      = 'econ',
  eProtocol                     = 'epro',
  eADB                          = 'eadb',
  eAnalogAudio                  = 'epau',
  eAppleTalk                    = 'epat',
  eAudioLineIn                  = 'ecai',
  eAudioLineOut                 = 'ecal', /* cde 4/24/98 changed from 'ecao' to not conflict */
  eAudioOut                     = 'ecao',
  eBus                          = 'ebus',
  eCDROM                        = 'ecd ',
  eCommSlot                     = 'eccm',
  eDigitalAudio                 = 'epda',
  eDisplay                      = 'edds',
  eDVD                          = 'edvd',
  eEthernet                     = 'ecen',
  eFireWire                     = 'ecfw',
  eFloppy                       = 'efd ',
  eHD                           = 'ehd ',
  eInfrared                     = 'ecir',
  eIP                           = 'epip',
  eIrDA                         = 'epir',
  eIRTalk                       = 'epit',
  eKeyboard                     = 'ekbd',
  eLCD                          = 'edlc',
  eLocalTalk                    = 'eclt',
  eMacIP                        = 'epmi',
  eMacVideo                     = 'epmv',
  eMicrophone                   = 'ecmi',
  eModemPort                    = 'ecmp',
  eModemPrinterPort             = 'empp',
  eModem                        = 'edmm',
  eMonitorOut                   = 'ecmn',
  eMouse                        = 'emou',
  eNuBusCard                    = 'ednb',
  eNuBus                        = 'enub',
  ePCcard                       = 'ecpc',
  ePCIbus                       = 'ecpi',
  ePCIcard                      = 'edpi',
  ePDSslot                      = 'ecpd',
  ePDScard                      = 'epds',
  ePointingDevice               = 'edpd',
  ePostScript                   = 'epps',
  ePPP                          = 'eppp',
  ePrinterPort                  = 'ecpp',
  ePrinter                      = 'edpr',
  eSvideo                       = 'epsv',
  eSCSI                         = 'ecsc',
  eSerial                       = 'epsr',
  eSpeakers                     = 'edsp',
  eStorageDevice                = 'edst',
  eSVGA                         = 'epsg',
  eTokenRing                    = 'etok',
  eTrackball                    = 'etrk',
  eTrackpad                     = 'edtp',
  eUSB                          = 'ecus',
  eVideoIn                      = 'ecvi',
  eVideoMonitor                 = 'edvm',
  eVideoOut                     = 'ecvo'
};

CF_ENUM(OSType) {
                                        /* AppleScript 1.3: Keystroke class */
  cKeystroke                    = 'kprs',
  pKeystrokeKey                 = 'kMsg',
  pModifiers                    = 'kMod',
  pKeyKind                      = 'kknd',
  eModifiers                    = 'eMds',
  eOptionDown                   = 'Kopt',
  eCommandDown                  = 'Kcmd',
  eControlDown                  = 'Kctl',
  eShiftDown                    = 'Ksft',
  eCapsLockDown                 = 'Kclk',
  eKeyKind                      = 'ekst', /* */
                                        /* Special keys all start with 'ks' */
  eEscapeKey                    = 0x6B733500, /* Third byte is virtual key code byte        */
  eDeleteKey                    = 0x6B733300, /* (see IM Mac Toolbox Essentials, pp. 2-43) */
  eTabKey                       = 0x6B733000,
  eReturnKey                    = 0x6B732400,
  eClearKey                     = 0x6B734700,
  eEnterKey                     = 0x6B734C00,
  eUpArrowKey                   = 0x6B737E00,
  eDownArrowKey                 = 0x6B737D00,
  eLeftArrowKey                 = 0x6B737B00,
  eRightArrowKey                = 0x6B737C00,
  eHelpKey                      = 0x6B737200,
  eHomeKey                      = 0x6B737300,
  ePageUpKey                    = 0x6B737400,
  ePageDownKey                  = 0x6B737900,
  eForwardDelKey                = 0x6B737500,
  eEndKey                       = 0x6B737700,
  eF1Key                        = 0x6B737A00,
  eF2Key                        = 0x6B737800,
  eF3Key                        = 0x6B736300,
  eF4Key                        = 0x6B737600,
  eF5Key                        = 0x6B736000,
  eF6Key                        = 0x6B736100,
  eF7Key                        = 0x6B736200,
  eF8Key                        = 0x6B736400,
  eF9Key                        = 0x6B736500,
  eF10Key                       = 0x6B736D00,
  eF11Key                       = 0x6B736700,
  eF12Key                       = 0x6B736F00,
  eF13Key                       = 0x6B736900,
  eF14Key                       = 0x6B736B00,
  eF15Key                       = 0x6B737100
};

CF_ENUM(AEKeyword) {
  keyAELaunchedAsLogInItem      = 'lgit', /* If present in a kAEOpenApplication event, application was launched as a login item and probably shouldn't open up untitled documents, etc. Mac OS X 10.4 and later. */
  keyAELaunchedAsServiceItem    = 'svit' /* If present in a kAEOpenApplication event, application was launched as a service item and probably shouldn't open up untitled documents, etc. Mac OS X 10.4 and later. */
};


#pragma pack(pop)


#endif /* __AEREGISTRY__ */

