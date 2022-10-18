/*
     File:       HIComboBox.r
 
     Contains:   Definition of the combo box view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __HICOMBOBOX_R__
#define __HICOMBOBOX_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kHIComboBoxListTag 				'cbls'				/*  CFArrayRef; bumps the refCount on get/retains on set */
#define kHIComboBoxListPixelWidthTag 	'cblw'				/*  UInt32  */
#define kHIComboBoxListPixelHeightTag 	'cblh'				/*  UInt32 */
#define kHIComboBoxNumVisibleItemsTag 	'cbni'				/*  UInt32 */


#endif /* __HICOMBOBOX_R__ */

