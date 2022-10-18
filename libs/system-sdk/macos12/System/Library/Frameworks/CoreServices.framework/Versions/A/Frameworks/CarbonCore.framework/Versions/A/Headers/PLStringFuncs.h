/*
     File:       CarbonCore/PLStringFuncs.h
 
     Contains:   Pascal string manipulation routines that parallel ANSI C string.h
                 The contents of this header file are deprecated.
 
     Copyright:  © 1999-2011 by Apple Inc. All rights reserved.
*/
#ifndef __PLSTRINGFUNCS__
#define __PLSTRINGFUNCS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  PLstrcmp()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Compare two pascal strings
 *  
 *  Discussion:
 *    This function compares two pascal strings, and returns a value <
 *    0 if the first string is lexicographically less than the second
 *    string, or 0 if the two strings are identical, or a value > 0 if
 *    the first string is lexicographically greater than the second.
 *    This function should be deprecated since pascal strings are
 *    obsolete on MacOSX and CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1 or str2.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the first pascal string
 *    
 *    str2:
 *      the second pascal string
 *  
 *  Result:
 *    This function returns an integer greater than, equal to, or less
 *    than 0, according as the string str1 is greater than, equal to,
 *    or less than the string str2.  The comparison is done using
 *    unsigned characters, so that `\200' is greater than `\0'.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
PLstrcmp(
  ConstStr255Param   str1,
  ConstStr255Param   str2)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrncmp()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Compare two pascal strings
 *  
 *  Discussion:
 *    This function compares two pascal strings, and returns a value <
 *    0 if the first string is lexicographically less than the second
 *    string, or 0 if the two strings are identical, or a value > 0 if
 *    the first string is lexicographically greater than the second. 
 *    This function compares not more than num characters of either
 *    string, even if their lengths are greater than num.  Two strings
 *    whose first num characters are identical will return 0 when
 *    compared. This function should be deprecated since pascal strings
 *    are obsolete on MacOSX and CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1 or str2.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the first pascal string
 *    
 *    str2:
 *      the second pascal string
 *    
 *    num:
 *      the maximum number of characters to compare
 *  
 *  Result:
 *    This function returns an integer greater than, equal to, or less
 *    than 0, according as the string str1 is greater than, equal to,
 *    or less than the string str2.  The comparison is done using
 *    unsigned characters, so that `\200' is greater than `\0'.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
PLstrncmp(
  ConstStr255Param   str1,
  ConstStr255Param   str2,
  short              num)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrcpy()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Copy a pascal string
 *  
 *  Discussion:
 *    This function copies the string source to dest (including the
 *    initial length byte ). The caller must ensure that neither source
 *    or dest are NULL, and that dest is large enough to hold the
 *    entire contents of source. This function should be deprecated
 *    since pascal strings are obsolete on MacOSX and CFString should
 *    be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    dest:
 *      the destination pascal string
 *    
 *    source:
 *      the source pascal string
 *  
 *  Result:
 *    This function returns dest.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern StringPtr 
PLstrcpy(
  StringPtr          dest,
  ConstStr255Param   source)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrncpy()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Copy a pascal string
 *  
 *  Discussion:
 *    This function copies the string source to dest (including the
 *    initial length byte ), provided the length of source is <= num. 
 *    If the length of source is > num, then the first num characters
 *    of source are copied into dest, and the length of dest is set to
 *    num.  The caller must ensure that neither source or dest are
 *    NULL, and that dest is large enough to hold the entire contents
 *    of source. This function should be deprecated since pascal
 *    strings are obsolete on MacOSX and CFString should be used
 *    instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying source.
 *  
 *  Parameters:
 *    
 *    dest:
 *      the destination pascal string
 *    
 *    source:
 *      the source pascal string
 *    
 *    num:
 *      the maximum number of bytes to copy
 *  
 *  Result:
 *    This function returns dest.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern StringPtr 
PLstrncpy(
  StringPtr          dest,
  ConstStr255Param   source,
  short              num)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrcat()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Append a pascal string to another pascal string
 *  
 *  Discussion:
 *    This function append a copy of the pascal string append to the
 *    end of the pascal string str.  If the length of str plus the
 *    length of append is greater than 255 ( the maximum size of a
 *    pascal string ) then only enough characters are copied to str to
 *    reach the 255 character limit, and the length of str is set to
 *    255.  The caller must ensure that neither str nor append are
 *    NULL, and that str is large enough to hold the entire contents of
 *    append. This function should be deprecated since pascal strings
 *    are obsolete on MacOSX and CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str or append.
 *  
 *  Parameters:
 *    
 *    str:
 *      the destination pascal string
 *    
 *    append:
 *      the pascal string to append
 *  
 *  Result:
 *    This function returns s.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern StringPtr 
PLstrcat(
  StringPtr          str,
  ConstStr255Param   append)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrncat()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Append up to num bytes of a pascal string to another pascal string
 *  
 *  Discussion:
 *    This function append up to the first num bytes of the pascal
 *    string append to the end of the pascal string s.  If the length
 *    of str plus the length of append is greater than 255 ( the
 *    maximum size of a pascal string ) then only enough characters are
 *    copied to str to reach the 255 character limit, and the length of
 *    str is set to 255.  The caller must ensure that neither str nor
 *    append are NULL, and that str is large enough to hold the entire
 *    contents of append. This function should be deprecated since
 *    pascal strings are obsolete on MacOSX and CFString should be used
 *    instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1 or append.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the destination pascal string
 *    
 *    append:
 *      the pascal string to append
 *    
 *    num:
 *      the maximum number of bytes of append to append onto s
 *  
 *  Result:
 *    This function returns str.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern StringPtr 
PLstrncat(
  StringPtr          str1,
  ConstStr255Param   append,
  short              num)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrchr()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Return a pointer to the first occurrence of ch1 in str.
 *  
 *  Discussion:
 *    The PLstrrchr() function locates the first occurrence of ch1
 *    (converted to an unsigned char) in the string s.  If ch1 does not
 *    occur in the string, this returns NULL. This function should be
 *    deprecated since pascal strings are obsolete on MacOSX and
 *    CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the pascal string
 *    
 *    ch1:
 *      the character to find
 *  
 *  Result:
 *    A pointer to the first occurrence of ch1 in str1, or NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Ptr 
PLstrchr(
  ConstStr255Param   str1,
  short              ch1)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrrchr()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Return a pointer to the last occurrence of ch1 in str.
 *  
 *  Discussion:
 *    The PLstrrchr() function locates the last occurrence of ch1
 *    (converted to an unsigned char) in the string s.  If ch1 does not
 *    occur in the string, this returns NULL. This function should be
 *    deprecated since pascal strings are obsolete on MacOSX and
 *    CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the pascal string
 *    
 *    ch1:
 *      the character to find
 *  
 *  Result:
 *    A pointer to the last occurrence of ch1 in str1, or NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Ptr 
PLstrrchr(
  ConstStr255Param   str1,
  short              ch1)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrpbrk()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Return a pointer to the first occurrence in str of any character
 *    in charSet.
 *  
 *  Discussion:
 *    The PLstrpbrk() function returns a pointer to the first
 *    occurrence in str of any character in searchStr.  If none of the
 *    characters in searchStr can be found in str, then NULL is
 *    returned. This function should be deprecated since pascal strings
 *    are obsolete on MacOSX and CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1 or charSet.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the pascal string
 *    
 *    charSet:
 *      the character to find
 *  
 *  Result:
 *    A pointer to the first occurrence of any character in charSet in
 *    str1, or NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Ptr 
PLstrpbrk(
  ConstStr255Param   str1,
  ConstStr255Param   charSet)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrspn()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Spans the initial part of str1 as long as the characters from
 *    str1 occur in string charset
 *  
 *  Discussion:
 *    The PLstrspn() function spans the initial part of the pascal
 *    string str1 as long as the characters from s occur in string
 *    charset. In effect, this returns a count of the number of
 *    characters at the beginning of the pascal string str1 which are
 *    in charset. This function should be deprecated since pascal
 *    strings are obsolete on MacOSX and CFString should be used
 *    instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1 or charSet.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the pascal string
 *    
 *    charSet:
 *      the character to find
 *  
 *  Result:
 *    The count of characters at the beginning of str1 which are in
 *    charSet.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
PLstrspn(
  ConstStr255Param   str1,
  ConstStr255Param   charSet)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/*
 *  PLstrstr()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Returns a pointer to the first occurrence of searchStr in str1
 *  
 *  Discussion:
 *    The PLstrstr() function returns a pointer to the first occurrence
 *    of searchStr in str1, or NULL if searchStr does not exist in
 *    str1. This function should be deprecated since pascal strings are
 *    obsolete on MacOSX and CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1 or
 *    searchStr.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the pascal string
 *    
 *    searchStr:
 *      the string to find
 *  
 *  Result:
 *    The count of characters at the beginning of str1 which are in
 *    charSet.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Ptr 
PLstrstr(
  ConstStr255Param   str1,
  ConstStr255Param   searchStr)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLstrlen()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Returns the length of the pascal string
 *  
 *  Discussion:
 *    The PLstrlen() function returns the length of the pascal string
 *    str. This function should be deprecated since pascal strings are
 *    obsolete on MacOSX and CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str.
 *  
 *  Parameters:
 *    
 *    str:
 *      the pascal string
 *  
 *  Result:
 *    The length of the pascal string str.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
PLstrlen(ConstStr255Param str)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PLpos()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Summary:
 *    Returns the offset to the first occurrence of searchStr in str1
 *  
 *  Discussion:
 *    The PLpos() function returns the offset of the string searchStr
 *    in str1, or 0 if searchStr does not occur in str1.  For example,
 *    if str1 is "\pHello World" and searchStr is "\pWorld", then this
 *    function will return the value 7. This function should be
 *    deprecated since pascal strings are obsolete on MacOSX and
 *    CFString should be used instead.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *    Thread safe provided no other thread is modifying str1 or
 *    searchStr.
 *  
 *  Parameters:
 *    
 *    str1:
 *      the pascal string
 *    
 *    searchStr:
 *      the string to find
 *  
 *  Result:
 *    The count of characters at the beginning of str1 which are in
 *    charSet.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
PLpos(
  ConstStr255Param   str1,
  ConstStr255Param   searchStr)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);




#ifdef __cplusplus
}
#endif

#endif /* __PLSTRINGFUNCS__ */

