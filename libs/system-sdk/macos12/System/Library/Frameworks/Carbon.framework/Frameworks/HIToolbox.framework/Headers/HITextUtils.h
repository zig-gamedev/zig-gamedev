/*
     File:       HIToolbox/HITextUtils.h
 
     Contains:   Text utilities provided by HIToolbox.
 
     Copyright:  © 2002-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HITEXTUTILS__
#define __HITEXTUTILS__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !__LP64__
/*
 *  GetTextAndEncodingFromCFString()
 *  
 *  Summary:
 *    Converts the contents of a CFString to the one- or two-byte
 *    encoding that most accurately represents the original Unicode
 *    characters in the string.
 *  
 *  Discussion:
 *    Because the Macintosh toolbox has had, until Carbon, little
 *    support for Unicode, you may often find that your applications
 *    need to translate a CFString (containing Unicode characters) back
 *    to a one- or two-byte encoding of the string in order to draw,
 *    measure, or otherwise use the text in the string.
 *    GetTextAndEncodingFromCFString is a utility that may be helpful
 *    to do this, in some circumstances. The Carbon toolbox uses this
 *    API itself when translating CFStrings to Pascal strings.
 *    GetTextAndEncodingFromCFString attempts to convert the Unicode
 *    characters in the string to the closest non-Unicode
 *    representation of the string. For example, a string containing
 *    only Unicode characters that are present in the MacRoman one-byte
 *    encoding will be translated to MacRoman, and
 *    kTextEncodingMacRoman will be returned as the encoding of the
 *    text. A string containing Unicode characters that are present in
 *    the MacJapanese two-byte encoding will be translated to
 *    MacJapanese (Shift-JIS), and kTextEncodingMacJapanese will be
 *    returned as the encoding of the text.
 *    GetTextAndEncodingFromCFString is designed to be used for simple
 *    strings which contain only text from a single language. It is not
 *    designed to translate strings with multiple runs of text from
 *    different languages, and will only return the first run of such a
 *    string; for example, if you pass it a string containing a run of
 *    Roman characters followed by a run of Japanese characters, it
 *    will only return the Roman characters in the output buffer. For
 *    more complex text translation, you should call the Unicode
 *    converter directly (see ConvertFromUnicodeToTextRun, which
 *    GetTextAndEncodingFromCFString uses internally).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inString:
 *      The string to convert.
 *    
 *    outText:
 *      The output buffer in which to place the converted text.
 *    
 *    inTextMaxLength:
 *      The length in bytes of the input buffer.
 *    
 *    outTextLength:
 *      On exit, contains the length in bytes of the text that was
 *      placed into the output buffer.
 *    
 *    outEncoding:
 *      On exit, contains the encoding of the text that was placed into
 *      the output buffer.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.2.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetTextAndEncodingFromCFString(
  CFStringRef     inString,
  BytePtr         outText,
  ByteCount       inTextMaxLength,
  ByteCount *     outTextLength,
  TextEncoding *  outEncoding)           /* can be NULL */    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HITEXTUTILS__ */

