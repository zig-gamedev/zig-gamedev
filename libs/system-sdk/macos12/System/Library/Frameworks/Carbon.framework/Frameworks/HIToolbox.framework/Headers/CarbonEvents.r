/*
     File:       CarbonEvents.r
 
     Contains:   Carbon Event Manager
 
     Copyright:  © 1999-2008 by Apple Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __CARBONEVENTS_R__
#define __CARBONEVENTS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kHICommandOK 					'ok  '
#define kHICommandCancel 				'not!'
#define kHICommandQuit 					'quit'
#define kHICommandQuitAndDiscardWindows  'qudw'
#define kHICommandQuitAndKeepWindows 	'qukw'
#define kHICommandUndo 					'undo'
#define kHICommandRedo 					'redo'
#define kHICommandCut 					'cut '
#define kHICommandCopy 					'copy'
#define kHICommandPaste 				'past'
#define kHICommandClear 				'clea'
#define kHICommandSelectAll 			'sall'
#define kHICommandHide 					'hide'
#define kHICommandHideOthers 			'hido'
#define kHICommandShowAll 				'shal'
#define kHICommandPreferences 			'pref'
#define kHICommandZoomWindow 			'zoom'
#define kHICommandMinimizeWindow 		'mini'
#define kHICommandMinimizeAll 			'mina'
#define kHICommandMaximizeWindow 		'maxi'
#define kHICommandMaximizeAll 			'maxa'
#define kHICommandArrangeInFront 		'frnt'
#define kHICommandBringAllToFront 		'bfrt'
#define kHICommandWindowListSeparator 	'wldv'
#define kHICommandWindowListTerminator 	'wlst'
#define kHICommandSelectWindow 			'swin'
#define kHICommandRotateWindowsForward 	'rotw'
#define kHICommandRotateWindowsBackward  'rotb'
#define kHICommandRotateFloatingWindowsForward  'rtfw'
#define kHICommandRotateFloatingWindowsBackward  'rtfb'
#define kHICommandToggleFullScreen 		'fsm '
#define kHICommandAbout 				'abou'
#define kHICommandNew 					'new '
#define kHICommandOpen 					'open'
#define kHICommandClose 				'clos'
#define kHICommandCloseAll 				'cloa'
#define kHICommandCloseFile 			'clof'
#define kHICommandSave 					'save'
#define kHICommandSaveAs 				'svas'
#define kHICommandRevert 				'rvrt'
#define kHICommandPrint 				'prnt'
#define kHICommandPageSetup 			'page'
#define kHICommandAppHelp 				'ahlp'
#define kHICommandShowCharacterPalette 	'chrp'
#define kHICommandShowSpellingPanel 	'shsp'
#define kHICommandCheckSpelling 		'cksp'
#define kHICommandChangeSpelling 		'chsp'
#define kHICommandCheckSpellingAsYouType  'aspc'
#define kHICommandIgnoreSpelling 		'igsp'
#define kHICommandLearnWord 			'lrwd'
#define kHICommandStartDictation 		'sdct'


#endif /* __CARBONEVENTS_R__ */

