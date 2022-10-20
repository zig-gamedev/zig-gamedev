/*
     File:       HITextViews.r
 
     Contains:   Definitions of text-display and text-editing views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HITEXTVIEWS_R__
#define __HITEXTVIEWS_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kControlStaticTextProc 			288
#define kControlKindStaticText 			'stxt'
#define kControlStaticTextStyleTag 		'font'
#define kControlStaticTextTextTag 		'text'
#define kControlStaticTextTextHeightTag  'thei'
#define kControlStaticTextTruncTag 		'trun'
#define kControlStaticTextCFStringTag 	'cfst'
#define kControlStaticTextIsMultilineTag  'stim'

#define kControlEditUnicodeTextProc 	912
#define kControlEditUnicodeTextPasswordProc  914

#define kControlKindEditUnicodeText 	'eutx'
#define kControlEditTextStyleTag 		'font'				/*  ControlFontStyleRec */
#define kControlEditTextTextTag 		'text'				/*  Buffer of chars - you supply the buffer */
#define kControlEditTextKeyFilterTag 	'fltr'
#define kControlEditTextSelectionTag 	'sele'				/*  ControlEditTextSelectionRec */
#define kControlEditTextPasswordTag 	'pass'				/*  The clear text password text */
#define kControlEditTextCharCount 		'chrc'				/*  [UInt32] Count of characters in the control's text */

#define kControlEditTextKeyScriptBehaviorTag  'kscr'		/*  ControlKeyScriptBehavior. Defaults to "PrefersRoman" for password fields, */
															/*        or "AllowAnyScript" for non-password fields. */
#define kControlEditTextLockedTag 		'lock'				/*  Boolean. Locking disables editability. */
#define kControlEditTextFixedTextTag 	'ftxt'				/*  Like the normal text tag, but fixes inline input first */
#define kControlEditTextValidationProcTag  'vali'			/*  ControlEditTextValidationUPP. Called when a key filter can't be: after cut, paste, etc. */

#define kControlEditTextCFStringTag 	'cfst'				/*  CFStringRef (Also available on CarbonLib 1.5) */
#define kControlEditTextPasswordCFStringTag  'pwcf'			/*  CFStringRef */

#define kControlEditTextSingleLineTag 	'sglc'				/*  data is a Boolean; indicates whether the control should always be single-line */
#define kControlEditTextInsertTextBufferTag  'intx'			/*  data is an array of char; get or set the control's text as WorldScript-encoded text */
#define kControlEditTextInsertCFStringRefTag  'incf'		/*  data is a CFStringRef; get or set the control's text as a CFStringRef. Caller should release CFString if getting. */
#define kControlEditUnicodeTextPostUpdateProcTag  'upup'	/*  data is a UnicodePostUpdateUPP; get or set the post-update proc */
#define kControlEditTextSpellCheckingTag  'spck'			/*  data is a Boolean; indicates whether the control wants to have spell checking support. Available in Leopard and later. */
#define kControlEditTextSpellCheckAsYouTypeTag  'scat'		/*  data is a Boolean; indicates whether you want to support spell-checking-as-you-type. Available in Leopard and later. */


#endif /* __HITEXTVIEWS_R__ */

