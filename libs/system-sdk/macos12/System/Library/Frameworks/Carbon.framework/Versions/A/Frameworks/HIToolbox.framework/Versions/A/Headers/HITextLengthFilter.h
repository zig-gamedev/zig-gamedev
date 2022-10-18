/*
     File:       HIToolbox/HITextLengthFilter.h
 
     Contains:   Header file for HITextLengthFilter object.
 
     Copyright:  © 1984-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
/*
    HITextLengthFilter
    
    This object enforces a maximum character length on a text field. It is typically attached as a
    delegate to an editable text view, such as an EditUnicodeText, HIComboBox, HISearchField, or
    HITextView object.
    
    This object handles the following Carbon events:
        
        kEventClassTextField
            kEventTextShouldChangeInRange
            
    This object does not send or post any Carbon events.
    
    This object has no dependencies on other AppObjects.
    
    This object is customizable to specify the maximum length for the text field.
    Your application customizes the maximum length for the text field by specifying
    a value for the kEventParamTextLength param of the kEventHIObjectInitialize event
    used when creating an instance of this object.
*/
#ifndef __HITEXTLENGTHFILTER__
#define __HITEXTLENGTHFILTER__


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif


/*
 *  kHITextLengthFilterClassID
 *  
 *  Discussion:
 *    HIObject class ID for the HITextLengthFilter object.
 */
#define kHITextLengthFilterClassID      CFSTR("com.apple.appobjects.HITextLengthFilter")

/*
 *  Summary:
 *    Initialization event parameters for HITextLengthFilter objects.
 */
enum {

  /*
   * [typeUInt32]  The maximum text length in characters that is
   * allowed by this object.
   */
  kEventParamTextLength         = 'TLEN'
};



#endif /* __HITEXTLENGTHFILTER__ */

