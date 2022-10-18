/*
     File:       AERegistry.r
 
     Contains:   AppleEvents Registry Interface.
 
    
 
     Copyright:  (c) 1993-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __AEREGISTRY_R__
#define __AEREGISTRY_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define cAEList 						'list'				/*   0x6c697374   */
#define cApplication 					'capp'				/*   0x63617070   */
#define cArc 							'carc'				/*   0x63617263   */
#define cBoolean 						'bool'				/*   0x626f6f6c   */
#define cCell 							'ccel'				/*   0x6363656c   */
#define cChar 							'cha '				/*   0x63686120   */
#define cColorTable 					'clrt'				/*   0x636c7274   */
#define cColumn 						'ccol'				/*   0x63636f6c   */
#define cDocument 						'docu'				/*   0x646f6375   */
#define cDrawingArea 					'cdrw'				/*   0x63647277   */
#define cEnumeration 					'enum'				/*   0x656e756d   */
#define cFile 							'file'				/*   0x66696c65   */
#define cFixed 							'fixd'				/*   0x66697864   */
#define cFixedPoint 					'fpnt'				/*   0x66706e74   */
#define cFixedRectangle 				'frct'				/*   0x66726374   */
#define cGraphicLine 					'glin'				/*   0x676c696e   */
#define cGraphicObject 					'cgob'				/*   0x63676f62   */
#define cGraphicShape 					'cgsh'				/*   0x63677368   */
#define cGraphicText 					'cgtx'				/*   0x63677478   */
#define cGroupedGraphic 				'cpic'				/*   0x63706963   */

#define cInsertionLoc 					'insl'				/*   0x696e736c   */
#define cInsertionPoint 				'cins'				/*   0x63696e73   */
#define cIntlText 						'itxt'				/*   0x69747874   */
#define cIntlWritingCode 				'intl'				/*   0x696e746c   */
#define cItem 							'citm'				/*   0x6369746d   */
#define cLine 							'clin'				/*   0x636c696e   */
#define cLongDateTime 					'ldt '				/*   0x6c647420   */
#define cLongFixed 						'lfxd'				/*   0x6c667864   */
#define cLongFixedPoint 				'lfpt'				/*   0x6c667074   */
#define cLongFixedRectangle 			'lfrc'				/*   0x6c667263   */
#define cLongInteger 					'long'				/*   0x6c6f6e67   */
#define cLongPoint 						'lpnt'				/*   0x6c706e74   */
#define cLongRectangle 					'lrct'				/*   0x6c726374   */
#define cMachineLoc 					'mLoc'				/*   0x6d4c6f63   */
#define cMenu 							'cmnu'				/*   0x636d6e75   */
#define cMenuItem 						'cmen'				/*   0x636d656e   */
#define cObject 						'cobj'				/*   0x636f626a   */
#define cObjectSpecifier 				'obj '				/*   0x6f626a20   */
#define cOpenableObject 				'coob'				/*   0x636f6f62   */
#define cOval 							'covl'				/*   0x636f766c   */

#define cParagraph 						'cpar'				/*   0x63706172   */
#define cPICT 							'PICT'				/*   0x50494354   */
#define cPixel 							'cpxl'				/*   0x6370786c   */
#define cPixelMap 						'cpix'				/*   0x63706978   */
#define cPolygon 						'cpgn'				/*   0x6370676e   */
#define cProperty 						'prop'				/*   0x70726f70   */
#define cQDPoint 						'QDpt'				/*   0x51447074   */
#define cQDRectangle 					'qdrt'				/*   0x71647274   */
#define cRectangle 						'crec'				/*   0x63726563   */
#define cRGBColor 						'cRGB'				/*   0x63524742   */
#define cRotation 						'trot'				/*   0x74726f74   */
#define cRoundedRectangle 				'crrc'				/*   0x63727263   */
#define cRow 							'crow'				/*   0x63726f77   */
#define cSelection 						'csel'				/*   0x6373656c   */
#define cShortInteger 					'shor'				/*   0x73686f72   */
#define cTable 							'ctbl'				/*   0x6374626c   */
#define cText 							'ctxt'				/*   0x63747874   */
#define cTextFlow 						'cflo'				/*   0x63666c6f   */
#define cTextStyles 					'tsty'				/*   0x74737479   */
#define cType 							'type'				/*   0x74797065   */

#define cVersion 						'vers'				/*   0x76657273   */
#define cWindow 						'cwin'				/*   0x6377696e   */
#define cWord 							'cwor'				/*   0x63776f72   */
#define enumArrows 						'arro'				/*   0x6172726f   */
#define enumJustification 				'just'				/*   0x6a757374   */
#define enumKeyForm 					'kfrm'				/*   0x6b66726d   */
#define enumPosition 					'posi'				/*   0x706f7369   */
#define enumProtection 					'prtn'				/*   0x7072746e   */
#define enumQuality 					'qual'				/*   0x7175616c   */
#define enumSaveOptions 				'savo'				/*   0x7361766f   */
#define enumStyle 						'styl'				/*   0x7374796c   */
#define enumTransferMode 				'tran'				/*   0x7472616e   */
#define kAEAbout 						'abou'				/*   0x61626f75   */
#define kAEAfter 						'afte'				/*   0x61667465   */
#define kAEAliasSelection 				'sali'				/*   0x73616c69   */
#define kAEAllCaps 						'alcp'				/*   0x616c6370   */
#define kAEArrowAtEnd 					'aren'				/*   0x6172656e   */
#define kAEArrowAtStart 				'arst'				/*   0x61727374   */
#define kAEArrowBothEnds 				'arbo'				/*   0x6172626f   */

#define kAEAsk 							'ask '				/*   0x61736b20   */
#define kAEBefore 						'befo'				/*   0x6265666f   */
#define kAEBeginning 					'bgng'				/*   0x62676e67   */
#define kAEBeginsWith 					'bgwt'				/*   0x62677774   */
#define kAEBeginTransaction 			'begi'				/*   0x62656769   */
#define kAEBold 						'bold'				/*   0x626f6c64   */
#define kAECaseSensEquals 				'cseq'				/*   0x63736571   */
#define kAECentered 					'cent'				/*   0x63656e74   */
#define kAEChangeView 					'view'				/*   0x76696577   */
#define kAEClone 						'clon'				/*   0x636c6f6e   */
#define kAEClose 						'clos'				/*   0x636c6f73   */
#define kAECondensed 					'cond'				/*   0x636f6e64   */
#define kAEContains 					'cont'				/*   0x636f6e74   */
#define kAECopy 						'copy'				/*   0x636f7079   */
#define kAECoreSuite 					'core'				/*   0x636f7265   */
#define kAECountElements 				'cnte'				/*   0x636e7465   */
#define kAECreateElement 				'crel'				/*   0x6372656c   */
#define kAECreatePublisher 				'cpub'				/*   0x63707562   */
#define kAECut 							'cut '				/*   0x63757420   */
#define kAEDelete 						'delo'				/*   0x64656c6f   */

#define kAEDoObjectsExist 				'doex'				/*   0x646f6578   */
#define kAEDoScript 					'dosc'				/*   0x646f7363   */
#define kAEDrag 						'drag'				/*   0x64726167   */
#define kAEDuplicateSelection 			'sdup'				/*   0x73647570   */
#define kAEEditGraphic 					'edit'				/*   0x65646974   */
#define kAEEmptyTrash 					'empt'				/*   0x656d7074   */
#define kAEEnd 							'end '				/*   0x656e6420   */
#define kAEEndsWith 					'ends'				/*   0x656e6473   */
#define kAEEndTransaction 				'endt'				/*   0x656e6474   */
#define kAEEquals 						'=   '				/*   0x3d202020   */
#define kAEExpanded 					'pexp'				/*   0x70657870   */
#define kAEFast 						'fast'				/*   0x66617374   */
#define kAEFinderEvents 				'FNDR'				/*   0x464e4452   */
#define kAEFormulaProtect 				'fpro'				/*   0x6670726f   */
#define kAEFullyJustified 				'full'				/*   0x66756c6c   */
#define kAEGetClassInfo 				'qobj'				/*   0x716f626a   */
#define kAEGetData 						'getd'				/*   0x67657464   */
#define kAEGetDataSize 					'dsiz'				/*   0x6473697a   */
#define kAEGetEventInfo 				'gtei'				/*   0x67746569   */
#define kAEGetInfoSelection 			'sinf'				/*   0x73696e66   */

#define kAEGetPrivilegeSelection 		'sprv'				/*   0x73707276   */
#define kAEGetSuiteInfo 				'gtsi'				/*   0x67747369   */
#define kAEGreaterThan 					'>   '				/*   0x3e202020   */
#define kAEGreaterThanEquals 			'>=  '				/*   0x3e3d2020   */
#define kAEGrow 						'grow'				/*   0x67726f77   */
#define kAEHidden 						'hidn'				/*   0x6869646e   */
#define kAEHiQuality 					'hiqu'				/*   0x68697175   */
#define kAEImageGraphic 				'imgr'				/*   0x696d6772   */
#define kAEIsUniform 					'isun'				/*   0x6973756e   */
#define kAEItalic 						'ital'				/*   0x6974616c   */
#define kAELeftJustified 				'left'				/*   0x6c656674   */
#define kAELessThan 					'<   '				/*   0x3c202020   */
#define kAELessThanEquals 				'<=  '				/*   0x3c3d2020   */
#define kAELowercase 					'lowc'				/*   0x6c6f7763   */
#define kAEMakeObjectsVisible 			'mvis'				/*   0x6d766973   */
#define kAEMiscStandards 				'misc'				/*   0x6d697363   */
#define kAEModifiable 					'modf'				/*   0x6d6f6466   */
#define kAEMove 						'move'				/*   0x6d6f7665   */
#define kAENo 							'no  '				/*   0x6e6f2020   */
#define kAENoArrow 						'arno'				/*   0x61726e6f   */

#define kAENonmodifiable 				'nmod'				/*   0x6e6d6f64   */
#define kAEOpen 						'odoc'				/*   0x6f646f63   */
#define kAEOpenSelection 				'sope'				/*   0x736f7065   */
#define kAEOutline 						'outl'				/*   0x6f75746c   */
#define kAEPageSetup 					'pgsu'				/*   0x70677375   */
#define kAEPaste 						'past'				/*   0x70617374   */
#define kAEPlain 						'plan'				/*   0x706c616e   */
#define kAEPrint 						'pdoc'				/*   0x70646f63   */
#define kAEPrintSelection 				'spri'				/*   0x73707269   */
#define kAEPrintWindow 					'pwin'				/*   0x7077696e   */
#define kAEPutAwaySelection 			'sput'				/*   0x73707574   */
#define kAEQDAddOver 					'addo'				/*   0x6164646f   */
#define kAEQDAddPin 					'addp'				/*   0x61646470   */
#define kAEQDAdMax 						'admx'				/*   0x61646d78   */
#define kAEQDAdMin 						'admn'				/*   0x61646d6e   */
#define kAEQDBic 						'bic '				/*   0x62696320   */
#define kAEQDBlend 						'blnd'				/*   0x626c6e64   */
#define kAEQDCopy 						'cpy '				/*   0x63707920   */
#define kAEQDNotBic 					'nbic'				/*   0x6e626963   */
#define kAEQDNotCopy 					'ncpy'				/*   0x6e637079   */

#define kAEQDNotOr 						'ntor'				/*   0x6e746f72   */
#define kAEQDNotXor 					'nxor'				/*   0x6e786f72   */
#define kAEQDOr 						'or  '				/*   0x6f722020   */
#define kAEQDSubOver 					'subo'				/*   0x7375626f   */
#define kAEQDSubPin 					'subp'				/*   0x73756270   */
#define kAEQDSupplementalSuite 			'qdsp'				/*   0x71647370   */
#define kAEQDXor 						'xor '				/*   0x786f7220   */
#define kAEQuickdrawSuite 				'qdrw'				/*   0x71647277   */
#define kAEQuitAll 						'quia'				/*   0x71756961   */
#define kAERedo 						'redo'				/*   0x7265646f   */
#define kAERegular 						'regl'				/*   0x7265676c   */
#define kAEReopenApplication 			'rapp'				/*   0x72617070   */
#define kAEReplace 						'rplc'				/*   0x72706c63   */
#define kAERequiredSuite 				'reqd'				/*   0x72657164   */
#define kAERestart 						'rest'				/*   0x72657374   */
#define kAERevealSelection 				'srev'				/*   0x73726576   */
#define kAERevert 						'rvrt'				/*   0x72767274   */
#define kAERightJustified 				'rght'				/*   0x72676874   */
#define kAESave 						'save'				/*   0x73617665   */
#define kAESelect 						'slct'				/*   0x736c6374   */
#define kAESetData 						'setd'				/*   0x73657464   */

#define kAESetPosition 					'posn'				/*   0x706f736e   */
#define kAEShadow 						'shad'				/*   0x73686164   */
#define kAEShowClipboard 				'shcl'				/*   0x7368636c   */
#define kAEShutDown 					'shut'				/*   0x73687574   */
#define kAESleep 						'slep'				/*   0x736c6570   */
#define kAESmallCaps 					'smcp'				/*   0x736d6370   */
#define kAESpecialClassProperties 		'c@#!'				/*   0x63402321   */
#define kAEStrikethrough 				'strk'				/*   0x7374726b   */
#define kAESubscript 					'sbsc'				/*   0x73627363   */
#define kAESuperscript 					'spsc'				/*   0x73707363   */
#define kAETableSuite 					'tbls'				/*   0x74626c73   */
#define kAETextSuite 					'TEXT'				/*   0x54455854   */
#define kAETransactionTerminated 		'ttrm'				/*   0x7474726d   */
#define kAEUnderline 					'undl'				/*   0x756e646c   */
#define kAEUndo 						'undo'				/*   0x756e646f   */
#define kAEWholeWordEquals 				'wweq'				/*   0x77776571   */
#define kAEYes 							'yes '				/*   0x79657320   */
#define kAEZoom 						'zoom'				/*   0x7a6f6f6d   */

#define kAELogOut 						'logo'
#define kAEReallyLogOut 				'rlgo'
#define kAEShowRestartDialog 			'rrst'
#define kAEShowShutdownDialog 			'rsdn'

#define kAEMouseClass 					'mous'
#define kAEDown 						'down'
#define kAEUp 							'up  '
#define kAEMoved 						'move'
#define kAEStoppedMoving 				'stop'
#define kAEWindowClass 					'wind'
#define kAEUpdate 						'updt'
#define kAEActivate 					'actv'
#define kAEDeactivate 					'dact'
#define kAECommandClass 				'cmnd'				/*  Modern Command Event Class  */
#define kAEKeyClass 					'keyc'
#define kAERawKey 						'rkey'				/*  Modern Raw Key Event  */
#define kAEVirtualKey 					'keyc'				/*  Modern Virtual Key Event  */
#define kAENavigationKey 				'nave'				/*  Modern Navigation Key Event  */
#define kAEAutoDown 					'auto'
#define kAEApplicationClass 			'appl'
#define kAESuspend 						'susp'
#define kAEResume 						'rsme'
#define kAEDiskEvent 					'disk'
#define kAENullEvent 					'null'
#define kAEWakeUpEvent 					'wake'
#define kAEScrapEvent 					'scrp'
#define kAEHighLevel 					'high'

#define keyAEAngle 						'kang'				/*   0x6b616e67   */
#define keyAEArcAngle 					'parc'				/*   0x70617263   */

#define keyAEBaseAddr 					'badd'				/*   0x62616464   */
#define keyAEBestType 					'pbst'				/*   0x70627374   */
#define keyAEBgndColor 					'kbcl'				/*   0x6b62636c   */
#define keyAEBgndPattern 				'kbpt'				/*   0x6b627074   */
#define keyAEBounds 					'pbnd'				/*   0x70626e64   */
#define keyAECellList 					'kclt'				/*   0x6b636c74   */
#define keyAEClassID 					'clID'				/*   0x636c4944   */
#define keyAEColor 						'colr'				/*   0x636f6c72   */
#define keyAEColorTable 				'cltb'				/*   0x636c7462   */
#define keyAECurveHeight 				'kchd'				/*   0x6b636864   */
#define keyAECurveWidth 				'kcwd'				/*   0x6b637764   */
#define keyAEDashStyle 					'pdst'				/*   0x70647374   */
#define keyAEData 						'data'				/*   0x64617461   */
#define keyAEDefaultType 				'deft'				/*   0x64656674   */
#define keyAEDefinitionRect 			'pdrt'				/*   0x70647274   */
#define keyAEDescType 					'dstp'				/*   0x64737470   */
#define keyAEDestination 				'dest'				/*   0x64657374   */
#define keyAEDoAntiAlias 				'anta'				/*   0x616e7461   */
#define keyAEDoDithered 				'gdit'				/*   0x67646974   */
#define keyAEDoRotate 					'kdrt'				/*   0x6b647274   */

#define keyAEDoScale 					'ksca'				/*   0x6b736361   */
#define keyAEDoTranslate 				'ktra'				/*   0x6b747261   */
#define keyAEEditionFileLoc 			'eloc'				/*   0x656c6f63   */
#define keyAEElements 					'elms'				/*   0x656c6d73   */
#define keyAEEndPoint 					'pend'				/*   0x70656e64   */
#define keyAEEventClass 				'evcl'				/*   0x6576636c   */
#define keyAEEventID 					'evti'				/*   0x65767469   */
#define keyAEFile 						'kfil'				/*   0x6b66696c   */
#define keyAEFileType 					'fltp'				/*   0x666c7470   */
#define keyAEFillColor 					'flcl'				/*   0x666c636c   */
#define keyAEFillPattern 				'flpt'				/*   0x666c7074   */
#define keyAEFlipHorizontal 			'kfho'				/*   0x6b66686f   */
#define keyAEFlipVertical 				'kfvt'				/*   0x6b667674   */
#define keyAEFont 						'font'				/*   0x666f6e74   */
#define keyAEFormula 					'pfor'				/*   0x70666f72   */
#define keyAEGraphicObjects 			'gobs'				/*   0x676f6273   */
#define keyAEID 						'ID  '				/*   0x49442020   */
#define keyAEImageQuality 				'gqua'				/*   0x67717561   */
#define keyAEInsertHere 				'insh'				/*   0x696e7368   */
#define keyAEKeyForms 					'keyf'				/*   0x6b657966   */

#define keyAEKeyword 					'kywd'				/*   0x6b797764   */
#define keyAELevel 						'levl'				/*   0x6c65766c   */
#define keyAELineArrow 					'arro'				/*   0x6172726f   */
#define keyAEName 						'pnam'				/*   0x706e616d   */
#define keyAENewElementLoc 				'pnel'				/*   0x706e656c   */
#define keyAEObject 					'kobj'				/*   0x6b6f626a   */
#define keyAEObjectClass 				'kocl'				/*   0x6b6f636c   */
#define keyAEOffStyles 					'ofst'				/*   0x6f667374   */
#define keyAEOnStyles 					'onst'				/*   0x6f6e7374   */
#define keyAEParameters 				'prms'				/*   0x70726d73   */
#define keyAEParamFlags 				'pmfg'				/*   0x706d6667   */
#define keyAEPenColor 					'ppcl'				/*   0x7070636c   */
#define keyAEPenPattern 				'pppa'				/*   0x70707061   */
#define keyAEPenWidth 					'ppwd'				/*   0x70707764   */
#define keyAEPixelDepth 				'pdpt'				/*   0x70647074   */
#define keyAEPixMapMinus 				'kpmm'				/*   0x6b706d6d   */
#define keyAEPMTable 					'kpmt'				/*   0x6b706d74   */
#define keyAEPointList 					'ptlt'				/*   0x70746c74   */
#define keyAEPointSize 					'ptsz'				/*   0x7074737a   */
#define keyAEPosition 					'kpos'				/*   0x6b706f73   */

#define keyAEPropData 					'prdt'				/*   0x70726474   */
#define keyAEProperties 				'qpro'				/*   0x7170726f   */
#define keyAEProperty 					'kprp'				/*   0x6b707270   */
#define keyAEPropFlags 					'prfg'				/*   0x70726667   */
#define keyAEPropID 					'prop'				/*   0x70726f70   */
#define keyAEProtection 				'ppro'				/*   0x7070726f   */
#define keyAERenderAs 					'kren'				/*   0x6b72656e   */
#define keyAERequestedType 				'rtyp'				/*   0x72747970   */
#define keyAEResult 					'----'				/*   0x2d2d2d2d   */
#define keyAEResultInfo 				'rsin'				/*   0x7273696e   */
#define keyAERotation 					'prot'				/*   0x70726f74   */
#define keyAERotPoint 					'krtp'				/*   0x6b727470   */
#define keyAERowList 					'krls'				/*   0x6b726c73   */
#define keyAESaveOptions 				'savo'				/*   0x7361766f   */
#define keyAEScale 						'pscl'				/*   0x7073636c   */
#define keyAEScriptTag 					'psct'				/*   0x70736374   */
#define keyAESearchText 				'stxt'				/*   0x73747874   */
#define keyAEShowWhere 					'show'				/*   0x73686f77   */
#define keyAEStartAngle 				'pang'				/*   0x70616e67   */
#define keyAEStartPoint 				'pstp'				/*   0x70737470   */
#define keyAEStyles 					'ksty'				/*   0x6b737479   */

#define keyAESuiteID 					'suit'				/*   0x73756974   */
#define keyAEText 						'ktxt'				/*   0x6b747874   */
#define keyAETextColor 					'ptxc'				/*   0x70747863   */
#define keyAETextFont 					'ptxf'				/*   0x70747866   */
#define keyAETextPointSize 				'ptps'				/*   0x70747073   */
#define keyAETextStyles 				'txst'				/*   0x74787374   */
#define keyAETextLineHeight 			'ktlh'				/*  type ShortInteger  */
#define keyAETextLineAscent 			'ktas'				/*  type ShortInteger  */
#define keyAETheText 					'thtx'				/*   0x74687478   */
#define keyAETransferMode 				'pptm'				/*   0x7070746d   */
#define keyAETranslation 				'ptrs'				/*   0x70747273   */
#define keyAETryAsStructGraf 			'toog'				/*   0x746f6f67   */
#define keyAEUniformStyles 				'ustl'				/*   0x7573746c   */
#define keyAEUpdateOn 					'pupd'				/*   0x70757064   */
#define keyAEUserTerm 					'utrm'				/*   0x7574726d   */
#define keyAEWindow 					'wndw'				/*   0x776e6477   */
#define keyAEWritingCode 				'wrcd'				/*   0x77726364   */

#define keyMiscellaneous 				'fmsc'				/*   0x666d7363   */
#define keySelection 					'fsel'				/*   0x6673656c   */
#define keyWindow 						'kwnd'				/*   0x6b776e64   */
															/*  EventRecord keys  */
#define keyWhen 						'when'
#define keyWhere 						'wher'
#define keyModifiers 					'mods'
#define keyKey 							'key '
#define keyKeyCode 						'code'
#define keyKeyboard 					'keyb'
#define keyDriveNumber 					'drv#'
#define keyErrorCode 					'err#'
#define keyHighLevelClass 				'hcls'
#define keyHighLevelID 					'hid '

#define pArcAngle 						'parc'				/*   0x70617263   */
#define pBackgroundColor 				'pbcl'				/*   0x7062636c   */
#define pBackgroundPattern 				'pbpt'				/*   0x70627074   */
#define pBestType 						'pbst'				/*   0x70627374   */
#define pBounds 						'pbnd'				/*   0x70626e64   */
#define pClass 							'pcls'				/*   0x70636c73   */
#define pClipboard 						'pcli'				/*   0x70636c69   */
#define pColor 							'colr'				/*   0x636f6c72   */
#define pColorTable 					'cltb'				/*   0x636c7462   */
#define pContents 						'pcnt'				/*   0x70636e74   */
#define pCornerCurveHeight 				'pchd'				/*   0x70636864   */
#define pCornerCurveWidth 				'pcwd'				/*   0x70637764   */
#define pDashStyle 						'pdst'				/*   0x70647374   */
#define pDefaultType 					'deft'				/*   0x64656674   */
#define pDefinitionRect 				'pdrt'				/*   0x70647274   */
#define pEnabled 						'enbl'				/*   0x656e626c   */
#define pEndPoint 						'pend'				/*   0x70656e64   */
#define pFillColor 						'flcl'				/*   0x666c636c   */
#define pFillPattern 					'flpt'				/*   0x666c7074   */
#define pFont 							'font'				/*   0x666f6e74   */

#define pFormula 						'pfor'				/*   0x70666f72   */
#define pGraphicObjects 				'gobs'				/*   0x676f6273   */
#define pHasCloseBox 					'hclb'				/*   0x68636c62   */
#define pHasTitleBar 					'ptit'				/*   0x70746974   */
#define pID 							'ID  '				/*   0x49442020   */
#define pIndex 							'pidx'				/*   0x70696478   */
#define pInsertionLoc 					'pins'				/*   0x70696e73   */
#define pIsFloating 					'isfl'				/*   0x6973666c   */
#define pIsFrontProcess 				'pisf'				/*   0x70697366   */
#define pIsModal 						'pmod'				/*   0x706d6f64   */
#define pIsModified 					'imod'				/*   0x696d6f64   */
#define pIsResizable 					'prsz'				/*   0x7072737a   */
#define pIsStationeryPad 				'pspd'				/*   0x70737064   */
#define pIsZoomable 					'iszm'				/*   0x69737a6d   */
#define pIsZoomed 						'pzum'				/*   0x707a756d   */
#define pItemNumber 					'itmn'				/*   0x69746d6e   */
#define pJustification 					'pjst'				/*   0x706a7374   */
#define pLineArrow 						'arro'				/*   0x6172726f   */
#define pMenuID 						'mnid'				/*   0x6d6e6964   */
#define pName 							'pnam'				/*   0x706e616d   */

#define pNewElementLoc 					'pnel'				/*   0x706e656c   */
#define pPenColor 						'ppcl'				/*   0x7070636c   */
#define pPenPattern 					'pppa'				/*   0x70707061   */
#define pPenWidth 						'ppwd'				/*   0x70707764   */
#define pPixelDepth 					'pdpt'				/*   0x70647074   */
#define pPointList 						'ptlt'				/*   0x70746c74   */
#define pPointSize 						'ptsz'				/*   0x7074737a   */
#define pProtection 					'ppro'				/*   0x7070726f   */
#define pRotation 						'prot'				/*   0x70726f74   */
#define pScale 							'pscl'				/*   0x7073636c   */
#define pScript 						'scpt'				/*   0x73637074   */
#define pScriptTag 						'psct'				/*   0x70736374   */
#define pSelected 						'selc'				/*   0x73656c63   */
#define pSelection 						'sele'				/*   0x73656c65   */
#define pStartAngle 					'pang'				/*   0x70616e67   */
#define pStartPoint 					'pstp'				/*   0x70737470   */
#define pTextColor 						'ptxc'				/*   0x70747863   */
#define pTextFont 						'ptxf'				/*   0x70747866   */
#define pTextItemDelimiters 			'txdl'				/*   0x7478646c   */
#define pTextPointSize 					'ptps'				/*   0x70747073   */

#define pTextStyles 					'txst'				/*   0x74787374   */
#define pTransferMode 					'pptm'				/*   0x7070746d   */
#define pTranslation 					'ptrs'				/*   0x70747273   */
#define pUniformStyles 					'ustl'				/*   0x7573746c   */
#define pUpdateOn 						'pupd'				/*   0x70757064   */
#define pUserSelection 					'pusl'				/*   0x7075736c   */
#define pVersion 						'vers'				/*   0x76657273   */
#define pVisible 						'pvis'				/*   0x70766973   */

#define typeAEText 						'tTXT'				/*   0x74545854   */
#define typeArc 						'carc'				/*   0x63617263   */
#define typeBest 						'best'				/*   0x62657374   */
#define typeCell 						'ccel'				/*   0x6363656c   */
#define typeClassInfo 					'gcli'				/*   0x67636c69   */
#define typeColorTable 					'clrt'				/*   0x636c7274   */
#define typeColumn 						'ccol'				/*   0x63636f6c   */
#define typeDashStyle 					'tdas'				/*   0x74646173   */
#define typeData 						'tdta'				/*   0x74647461   */
#define typeDrawingArea 				'cdrw'				/*   0x63647277   */
#define typeElemInfo 					'elin'				/*   0x656c696e   */
#define typeEnumeration 				'enum'				/*   0x656e756d   */
#define typeEPS 						'EPS '				/*   0x45505320   */
#define typeEventInfo 					'evin'				/*   0x6576696e   */

#define typeFinderWindow 				'fwin'				/*   0x6677696e   */
#define typeFixedPoint 					'fpnt'				/*   0x66706e74   */
#define typeFixedRectangle 				'frct'				/*   0x66726374   */
#define typeGraphicLine 				'glin'				/*   0x676c696e   */
#define typeGraphicText 				'cgtx'				/*   0x63677478   */
#define typeGroupedGraphic 				'cpic'				/*   0x63706963   */
#define typeInsertionLoc 				'insl'				/*   0x696e736c   */
#define typeIntlText 					'itxt'				/*   0x69747874   */
#define typeIntlWritingCode 			'intl'				/*   0x696e746c   */
#define typeLongDateTime 				'ldt '				/*   0x6c647420   */
#define typeCFAbsoluteTime 				'cfat'
#define typeISO8601DateTime 			'isot'				/*   0x69736f74  data is ascii text of an ISO8601 date  */
#define typeLongFixed 					'lfxd'				/*   0x6c667864   */
#define typeLongFixedPoint 				'lfpt'				/*   0x6c667074   */
#define typeLongFixedRectangle 			'lfrc'				/*   0x6c667263   */
#define typeLongPoint 					'lpnt'				/*   0x6c706e74   */
#define typeLongRectangle 				'lrct'				/*   0x6c726374   */
#define typeMachineLoc 					'mLoc'				/*   0x6d4c6f63   */
#define typeOval 						'covl'				/*   0x636f766c   */
#define typeParamInfo 					'pmin'				/*   0x706d696e   */
#define typePict 						'PICT'				/*   0x50494354   */

#define typePixelMap 					'cpix'				/*   0x63706978   */
#define typePixMapMinus 				'tpmm'				/*   0x74706d6d   */
#define typePolygon 					'cpgn'				/*   0x6370676e   */
#define typePropInfo 					'pinf'				/*   0x70696e66   */
#define typePtr 						'ptr '				/*   0x70747220   */
#define typeQDPoint 					'QDpt'				/*   0x51447074   */
#define typeQDRegion 					'Qrgn'				/*   0x51447074  (data is actual region data, including rectangle and size, _not_ region handle or ptr) */
#define typeRectangle 					'crec'				/*   0x63726563   */
#define typeRGB16 						'tr16'				/*   0x74723136   */
#define typeRGB96 						'tr96'				/*   0x74723936   */
#define typeRGBColor 					'cRGB'				/*   0x63524742   */
#define typeRotation 					'trot'				/*   0x74726f74   */
#define typeRoundedRectangle 			'crrc'				/*   0x63727263   */
#define typeRow 						'crow'				/*   0x63726f77   */
#define typeScrapStyles 				'styl'				/*   0x7374796c   */
#define typeScript 						'scpt'				/*   0x73637074   */
#define typeStyledText 					'STXT'				/*   0x53545854   */
#define typeSuiteInfo 					'suin'				/*   0x7375696e   */
#define typeTable 						'ctbl'				/*   0x6374626c   */
#define typeTextStyles 					'tsty'				/*   0x74737479   */

#define typeTIFF 						'TIFF'				/*   0x54494646   */
#define typeJPEG 						'JPEG'
#define typeGIF 						'GIFf'
#define typeVersion 					'vers'				/*   0x76657273   */

#define kAEMenuClass 					'menu'
#define kAEMenuSelect 					'mhit'
#define kAEMouseDown 					'mdwn'
#define kAEMouseDownInBack 				'mdbk'
#define kAEKeyDown 						'kdwn'
#define kAEResized 						'rsiz'
#define kAEPromise 						'prom'

#define keyMenuID 						'mid '
#define keyMenuItem 					'mitm'
#define keyCloseAllWindows 				'caw '
#define keyOriginalBounds 				'obnd'
#define keyNewBounds 					'nbnd'
#define keyLocalWhere 					'lwhr'

#define typeHIMenu 						'mobj'
#define typeHIWindow 					'wobj'

#define kAEQuitPreserveState 			'stat'				/*  in a kAEQuitApplication event, this optional parameter hints the application as to whether it should write out  */
															/*  persistent state which may be restored on the next launch. The possible values are kAENo ( the default ), or kAEYes  */
#define kAEQuitReason 					'why?'				/*  in a kAEQuitApplication event, this parameter if present is the reason the quit is being sent.  The possible values are kAEQuitAll, kAEShutDown, kAERestart, kAEReallyLogOut  */

#define kBySmallIcon 					0
#define kByIconView 					1
#define kByNameView 					2
#define kByDateView 					3
#define kBySizeView 					4
#define kByKindView 					5
#define kByCommentView 					6
#define kByLabelView 					7
#define kByVersionView 					8

#define kAEInfo 						11
#define kAEMain 						0
#define kAESharing 						13

#define kAEZoomIn 						7
#define kAEZoomOut 						8

#define kTextServiceClass 				'tsvc'
#define kUpdateActiveInputArea 			'updt'				/*  update the active input area  */
#define kShowHideInputWindow 			'shiw'				/*  show or hide the input window  */
#define kPos2Offset 					'p2st'				/*  converting global coordinates to char position  */
#define kOffset2Pos 					'st2p'				/*  converting char position to global coordinates  */
#define kUnicodeNotFromInputMethod 		'unim'				/*  Unicode text when event not handled by Input Method or no Input Method  */
#define kGetSelectedText 				'gtxt'				/*  Get text for current selection  */
#define keyAETSMDocumentRefcon 			'refc'				/*  TSM document refcon, typeLongInteger  */
#define keyAEServerInstance 			'srvi'				/*  component instance  */
#define keyAETheData 					'kdat'				/*  typeText  */
#define keyAEFixLength 					'fixl'				/*  fix len  */
#define keyAEUpdateRange 				'udng'				/*  typeTextRangeArray  */
#define keyAECurrentPoint 				'cpos'				/*  current point  */
#define keyAEBufferSize 				'buff'				/*  buffer size to get the text  */
#define keyAEMoveView 					'mvvw'				/*  move view flag  */
#define keyAENextBody 					'nxbd'				/*  next or previous body  */
#define keyAETSMScriptTag 				'sclg'
#define keyAETSMTextFont 				'ktxf'				/*  FMFontFamily or FOND ID  */
#define keyAETSMTextFMFont 				'ktxm'				/*  FMFont  */
#define keyAETSMTextPointSize 			'ktps'
#define keyAETSMEventRecord 			'tevt'				/*  Low level Event Record, typeLowLevelEventRecord  */
#define keyAETSMEventRef 				'tevr'				/*  Carbon EventRef, typeEventRef  */
#define keyAETextServiceEncoding 		'tsen'				/*  Text Service encoding, mac or Unicode in UpdateActiveInputArea or GetSelectedText events.  */
#define keyAETextServiceMacEncoding 	'tmen'				/*  Target mac encoding for TSM conversion of text from Unicode text service.  */
#define keyAETSMGlyphInfoArray 			'tgia'				/*  typeGlyphInfoArray  */
#define typeTextRange 					'txrn'				/*  TextRange  */
#define typeComponentInstance 			'cmpi'				/*  server instance  */
#define typeOffsetArray 				'ofay'				/*  offset array  */
#define typeTextRangeArray 				'tray'
#define typeLowLevelEventRecord 		'evtr'				/*  Low Level Event Record  */
#define typeGlyphInfoArray 				'glia'				/*  Glyph/FMFont info array for sub ranges of Unicode text.  See GlyphInfoArray in TextServices.h   */
#define typeEventRef 					'evrf'				/*  Carbon EventRef  */
#define typeText 						'TEXT'				/*  Plain text  */

#define kTSMOutsideOfBody 				1
#define kTSMInsideOfBody 				2
#define kTSMInsideOfActiveInputArea 	3

#define kNextBody 						1
#define kPreviousBody 					2

#define kTSMHiliteCaretPosition 		1					/*  specify caret position  */
#define kTSMHiliteRawText 				2					/*  specify range of raw text  */
#define kTSMHiliteSelectedRawText 		3					/*  specify range of selected raw text  */
#define kTSMHiliteConvertedText 		4					/*  specify range of converted text  */
#define kTSMHiliteSelectedConvertedText  5					/*  specify range of selected converted text  */
#define kTSMHiliteBlockFillText 		6					/*  Block Fill hilite style  */
#define kTSMHiliteOutlineText 			7					/*  Outline hilite style  */
#define kTSMHiliteSelectedText 			8					/*  Selected hilite style  */
#define kTSMHiliteNoHilite 				9					/*  specify range of non-hilited text  */

#if OLDROUTINENAMES
#define kCaretPosition 					1
#define kRawText 						2
#define kSelectedRawText 				3
#define kConvertedText 					4
#define kSelectedConvertedText 			5
#define kBlockFillText 					6
#define kOutlineText 					7
#define kSelectedText 					8

#endif  /* OLDROUTINENAMES */

#define keyAEHiliteRange 				'hrng'				/*  typeTextRangeArray for System 7, typeHiliteRangeArray for System 8  */
#define keyAEPinRange 					'pnrg'				/*  typeTextRange for System 7, typeTextRegionRange for System 8    */
#define keyAEClauseOffsets 				'clau'				/*  typeOffsetArray for System 7, typeClauseOffsetArray for System 8  */
#define keyAEOffset 					'ofst'				/*  typeLongInteger for System 7, typeByteOffset for System 8   */
#define keyAEPoint 						'gpos'				/*  typePoint for System 7, typeQDPoint for System 8  */
#define keyAELeftSide 					'klef'				/*  typeBoolean  */
#define keyAERegionClass 				'rgnc'				/*  typeShortInteger for System 7, typeRegionClass for System 8  */
#define keyAEDragging 					'bool'				/*  typeBoolean  */

#if OLDROUTINENAMES
#define keyAELeadingEdge 				'klef'
#endif  /* OLDROUTINENAMES */

															/*  AppleScript 1.3: Unit types  */
#define typeMeters 						'metr'				/*  Base Unit  */
#define typeInches 						'inch'
#define typeFeet 						'feet'
#define typeYards 						'yard'
#define typeMiles 						'mile'
#define typeKilometers 					'kmtr'
#define typeCentimeters 				'cmtr'
#define typeSquareMeters 				'sqrm'				/*  Base Unit  */
#define typeSquareFeet 					'sqft'
#define typeSquareYards 				'sqyd'
#define typeSquareMiles 				'sqmi'
#define typeSquareKilometers 			'sqkm'
#define typeLiters 						'litr'				/*  Base Unit  */
#define typeQuarts 						'qrts'
#define typeGallons 					'galn'
#define typeCubicMeters 				'cmet'				/*  Base Unit  */
#define typeCubicFeet 					'cfet'
#define typeCubicInches 				'cuin'
#define typeCubicCentimeter 			'ccmt'
#define typeCubicYards 					'cyrd'
#define typeKilograms 					'kgrm'				/*  Base Unit  */
#define typeGrams 						'gram'
#define typeOunces 						'ozs '
#define typePounds 						'lbs '
#define typeDegreesC 					'degc'				/*  Base Unit  */
#define typeDegreesF 					'degf'
#define typeDegreesK 					'degk'

															/*  AppleScript 1.3: Folder Actions  */
#define kFAServerApp 					'ssrv'				/*  Creator code for Folder Actions Server */
#define kDoFolderActionEvent 			'fola'				/*  Event the Finder sends to the Folder Actions FBA */
#define kFolderActionCode 				'actn'				/*  Parameter that contains the Folder Action */
#define kFolderOpenedEvent 				'fopn'				/*  Value of kFolderActionCode parameter; sent to script as event */
#define kFolderClosedEvent 				'fclo'
#define kFolderWindowMovedEvent 		'fsiz'
#define kFolderItemsAddedEvent 			'fget'
#define kFolderItemsRemovedEvent 		'flos'
#define kItemList 						'flst'				/*  List parameter for added and removed items */
#define kNewSizeParameter 				'fnsz'				/*  Parameter for moved window */
#define kFASuiteCode 					'faco'				/*  Suite code for the following events */
#define kFAAttachCommand 				'atfa'				/*  Attach event id */
#define kFARemoveCommand 				'rmfa'				/*  Remove event id */
#define kFAEditCommand 					'edfa'				/*  Edit event id */
#define kFAFileParam 					'faal'				/*  Key for file parameter for Attach */
#define kFAIndexParam 					'indx'				/*  Key for index (0-based) parameter for Remove and Edit */

															/*  Suite code  */
#define kAEInternetSuite 				'gurl'
#define kAEISWebStarSuite 				'WWW½'

															/*  Events  */
#define kAEISGetURL 					'gurl'
#define KAEISHandleCGI 					'sdoc'

															/*  Classes  */
#define cURL 							'url '
#define cInternetAddress 				'IPAD'
#define cHTML 							'html'
#define cFTPItem 						'ftp '

															/*  Parameters  */
#define kAEISHTTPSearchArgs 			'kfor'
#define kAEISPostArgs 					'post'
#define kAEISMethod 					'meth'
#define kAEISClientAddress 				'addr'
#define kAEISUserName 					'user'
#define kAEISPassword 					'pass'
#define kAEISFromUser 					'frmu'
#define kAEISServerName 				'svnm'
#define kAEISServerPort 				'svpt'
#define kAEISScriptName 				'scnm'
#define kAEISContentType 				'ctyp'
#define kAEISReferrer 					'refr'
#define kAEISUserAgent 					'Agnt'
#define kAEISAction 					'Kact'
#define kAEISActionPath 				'Kapt'
#define kAEISClientIP 					'Kcip'
#define kAEISFullRequest 				'Kfrq'

															/*  Properties  */
#define pScheme 						'pusc'
#define pHost 							'HOST'
#define pPath 							'FTPc'
#define pUserName 						'RAun'
#define pUserPassword 					'RApw'
#define pDNSForm 						'pDNS'
#define pURL 							'pURL'
#define pTextEncoding 					'ptxe'
#define pFTPKind 						'kind'

															/*  Scheme enumerations  */
#define eScheme 						'esch'
#define eurlHTTP 						'http'				/*  RFC 2068  */
#define eurlHTTPS 						'htps'
#define eurlFTP 						'ftp '				/*  RFC 1738  */
#define eurlMail 						'mail'				/*  RFC 2638  */
#define eurlFile 						'file'				/*  RFC 1738  */
#define eurlGopher 						'gphr'				/*  RFC 1738  */
#define eurlTelnet 						'tlnt'				/*  RFC 1738  */
#define eurlNews 						'news'				/*  RFC 1738  */
#define eurlSNews 						'snws'
#define eurlNNTP 						'nntp'				/*  RFC 1738  */
#define eurlMessage 					'mess'
#define eurlMailbox 					'mbox'
#define eurlMulti 						'mult'
#define eurlLaunch 						'laun'
#define eurlAFP 						'afp '
#define eurlAT 							'at  '
#define eurlEPPC 						'eppc'
#define eurlRTSP 						'rtsp'				/*  RFC 2326  */
#define eurlIMAP 						'imap'				/*  RFC 2192  */
#define eurlNFS 						'unfs'				/*  RFC 2224  */
#define eurlPOP 						'upop'				/*  RFC 2384  */
#define eurlLDAP 						'uldp'				/*  RFC 2255  */
#define eurlUnknown 					'url?'

															/*  AppleScript 1.3: Connectivity Suite in aeut  */
#define kConnSuite 						'macc'
#define cDevSpec 						'cdev'
#define cAddressSpec 					'cadr'
#define cADBAddress 					'cadb'
#define cAppleTalkAddress 				'cat '
#define cBusAddress 					'cbus'
#define cEthernetAddress 				'cen '
#define cFireWireAddress 				'cfw '
#define cIPAddress 						'cip '
#define cLocalTalkAddress 				'clt '
#define cSCSIAddress 					'cscs'
#define cTokenRingAddress 				'ctok'
#define cUSBAddress 					'cusb'				/*   */
															/*  Properties  */
#define pDeviceType 					'pdvt'
#define pDeviceAddress 					'pdva'
#define pConduit 						'pcon'
#define pProtocol 						'pprt'				/*  cde 4/27/98 was 'ppro' conflicted with DB suite  */
#define pATMachine 						'patm'
#define pATZone 						'patz'
#define pATType 						'patt'
#define pDottedDecimal 					'pipd'
#define pDNS 							'pdns'
#define pPort 							'ppor'
#define pNetwork 						'pnet'
#define pNode 							'pnod'
#define pSocket 						'psoc'
#define pSCSIBus 						'pscb'
#define pSCSILUN 						'pslu'				/*  cde 5/22/98 per WWDC developer request  */
															/*  Enumerations and enumerators  */
#define eDeviceType 					'edvt'
#define eAddressSpec 					'eads'
#define eConduit 						'econ'
#define eProtocol 						'epro'
#define eADB 							'eadb'
#define eAnalogAudio 					'epau'
#define eAppleTalk 						'epat'
#define eAudioLineIn 					'ecai'
#define eAudioLineOut 					'ecal'				/*  cde 4/24/98 changed from 'ecao' to not conflict  */
#define eAudioOut 						'ecao'
#define eBus 							'ebus'
#define eCDROM 							'ecd '
#define eCommSlot 						'eccm'
#define eDigitalAudio 					'epda'
#define eDisplay 						'edds'
#define eDVD 							'edvd'
#define eEthernet 						'ecen'
#define eFireWire 						'ecfw'
#define eFloppy 						'efd '
#define eHD 							'ehd '
#define eInfrared 						'ecir'
#define eIP 							'epip'
#define eIrDA 							'epir'
#define eIRTalk 						'epit'
#define eKeyboard 						'ekbd'
#define eLCD 							'edlc'
#define eLocalTalk 						'eclt'
#define eMacIP 							'epmi'
#define eMacVideo 						'epmv'
#define eMicrophone 					'ecmi'
#define eModemPort 						'ecmp'
#define eModemPrinterPort 				'empp'
#define eModem 							'edmm'
#define eMonitorOut 					'ecmn'
#define eMouse 							'emou'
#define eNuBusCard 						'ednb'
#define eNuBus 							'enub'
#define ePCcard 						'ecpc'
#define ePCIbus 						'ecpi'
#define ePCIcard 						'edpi'
#define ePDSslot 						'ecpd'
#define ePDScard 						'epds'
#define ePointingDevice 				'edpd'
#define ePostScript 					'epps'
#define ePPP 							'eppp'
#define ePrinterPort 					'ecpp'
#define ePrinter 						'edpr'
#define eSvideo 						'epsv'
#define eSCSI 							'ecsc'
#define eSerial 						'epsr'
#define eSpeakers 						'edsp'
#define eStorageDevice 					'edst'
#define eSVGA 							'epsg'
#define eTokenRing 						'etok'
#define eTrackball 						'etrk'
#define eTrackpad 						'edtp'
#define eUSB 							'ecus'
#define eVideoIn 						'ecvi'
#define eVideoMonitor 					'edvm'
#define eVideoOut 						'ecvo'

															/*  AppleScript 1.3: Keystroke class  */
#define cKeystroke 						'kprs'
#define pKeystrokeKey 					'kMsg'
#define pModifiers 						'kMod'
#define pKeyKind 						'kknd'
#define eModifiers 						'eMds'
#define eOptionDown 					'Kopt'
#define eCommandDown 					'Kcmd'
#define eControlDown 					'Kctl'
#define eShiftDown 						'Ksft'
#define eCapsLockDown 					'Kclk'
#define eKeyKind 						'ekst'				/*   */
															/*  Special keys all start with 'ks'  */
#define eEscapeKey 						0x6B733500			/*  Third byte is virtual key code byte         */
#define eDeleteKey 						0x6B733300			/*  (see IM Mac Toolbox Essentials, pp. 2-43)  */
#define eTabKey 						0x6B733000
#define eReturnKey 						0x6B732400
#define eClearKey 						0x6B734700
#define eEnterKey 						0x6B734C00
#define eUpArrowKey 					0x6B737E00
#define eDownArrowKey 					0x6B737D00
#define eLeftArrowKey 					0x6B737B00
#define eRightArrowKey 					0x6B737C00
#define eHelpKey 						0x6B737200
#define eHomeKey 						0x6B737300
#define ePageUpKey 						0x6B737400
#define ePageDownKey 					0x6B737900
#define eForwardDelKey 					0x6B737500
#define eEndKey 						0x6B737700
#define eF1Key 							0x6B737A00
#define eF2Key 							0x6B737800
#define eF3Key 							0x6B736300
#define eF4Key 							0x6B737600
#define eF5Key 							0x6B736000
#define eF6Key 							0x6B736100
#define eF7Key 							0x6B736200
#define eF8Key 							0x6B736400
#define eF9Key 							0x6B736500
#define eF10Key 						0x6B736D00
#define eF11Key 						0x6B736700
#define eF12Key 						0x6B736F00
#define eF13Key 						0x6B736900
#define eF14Key 						0x6B736B00
#define eF15Key 						0x6B737100

#define keyAELaunchedAsLogInItem 		'lgit'				/*  If present in a kAEOpenApplication event, application was launched as a login item and probably shouldn't open up untitled documents, etc. Mac OS X 10.4 and later.  */
#define keyAELaunchedAsServiceItem 		'svit'				/*  If present in a kAEOpenApplication event, application was launched as a service item and probably shouldn't open up untitled documents, etc. Mac OS X 10.4 and later.  */


#endif /* __AEREGISTRY_R__ */

