/*
     File:       CarbonCore/StringCompare.h
 
     Contains:   Public interfaces for String Comparison and related operations
                 The contents of this header file are deprecated.
 
     Copyright:  © 1985-2011 by Apple Inc., all rights reserved.
*/
#ifndef __STRINGCOMPARE__
#define __STRINGCOMPARE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __TEXTCOMMON__
#include <CarbonCore/TextCommon.h>
#endif

#ifndef __SCRIPT__
#include <CarbonCore/Script.h>
#endif


#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*

    Here are the current System 7 routine names and the translations to the older forms.
    Please use the newer forms in all new code and migrate the older names out of existing
    code as maintenance permits.
    
    NEW NAME                    OLD NAME                    OBSOLETE FORM (no handle)
    
    CompareString (Str255)      IUCompPString (hp only)     IUCompString (hp only)
    CompareText (ptr/len)       IUMagPString                IUMagString
    IdenticalString (Str255)    IUEqualPString (hp only)    IUEqualString  (hp only)
    IdenticalText (ptr/len)     IUMagIDPString              IUMagIDString
    LanguageOrder               IULangOrder
    ScriptOrder                 IUScriptOrder
    StringOrder (Str255)        IUStringOrder (hp only)
    TextOrder (ptr/len)         IUTextOrder

    RelString
    CmpString (a only)                  
    EqualString (hp only)
    
    ReplaceText

    Carbon only supports the new names.  The old names are undefined for Carbon targets.

    InterfaceLib always has exported the old names.  For C macros have been defined to allow
    the use of the new names.  For Pascal and Assembly using the new names will result
    in link errors. 
    
*/

enum {
                                        /* Special language code values for Language Order*/
  systemCurLang                 = -2,   /* current (itlbLang) lang for system script*/
  systemDefLang                 = -3,   /* default (table) lang for system script*/
  currentCurLang                = -4,   /* current (itlbLang) lang for current script*/
  currentDefLang                = -5,   /* default lang for current script*/
  scriptCurLang                 = -6,   /* current (itlbLang) lang for specified script*/
  scriptDefLang                 = -7    /* default language for a specified script*/
};

/* obsolete names*/
enum {
  iuSystemCurLang               = systemCurLang,
  iuSystemDefLang               = systemDefLang,
  iuCurrentCurLang              = currentCurLang,
  iuCurrentDefLang              = currentDefLang,
  iuScriptCurLang               = scriptCurLang,
  iuScriptDefLang               = scriptDefLang
};


/*
 *  These routines are available in Carbon with the new names.
 */
#if !__LP64__
/*
 *  [Mac]ReplaceText()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringReplace instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringReplace instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
#if TARGET_OS_MAC
    #define MacReplaceText ReplaceText
#endif
extern short 
MacReplaceText(
  Handle   baseText,
  Handle   substitutionText,
  Str15    key)                                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  ScriptOrder()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare or UCCompareText instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare or UCCompareText instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern short 
ScriptOrder(
  ScriptCode   script1,
  ScriptCode   script2)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  [Mac]CompareString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare or UCCompareText instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare or UCCompareText instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
#if TARGET_OS_MAC
    #define MacCompareString CompareString
#endif
extern short 
MacCompareString(
  ConstStr255Param   aStr,
  ConstStr255Param   bStr,
  Handle             itl2Handle)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  IdenticalString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
IdenticalString(
  ConstStr255Param   aStr,
  ConstStr255Param   bStr,
  Handle             itl2Handle)                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  StringOrder()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare or UCCompareText instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare or UCCompareText instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
StringOrder(
  ConstStr255Param   aStr,
  ConstStr255Param   bStr,
  ScriptCode         aScript,
  ScriptCode         bScript,
  LangCode           aLang,
  LangCode           bLang)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  CompareText()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare or UCCompareText instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare or UCCompareText instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
CompareText(
  const void *  aPtr,
  const void *  bPtr,
  short         aLen,
  short         bLen,
  Handle        itl2Handle)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  IdenticalText()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
IdenticalText(
  const void *  aPtr,
  const void *  bPtr,
  short         aLen,
  short         bLen,
  Handle        itl2Handle)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  TextOrder()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare or UCCompareText instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare or UCCompareText instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
TextOrder(
  const void *  aPtr,
  const void *  bPtr,
  short         aLen,
  short         bLen,
  ScriptCode    aScript,
  ScriptCode    bScript,
  LangCode      aLang,
  LangCode      bLang)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  LanguageOrder()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare or UCCompareText instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare or UCCompareText instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
LanguageOrder(
  LangCode   language1,
  LangCode   language2)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/*
 *  These routines are available in InterfaceLib with old names.
 *  Macros are provided for C to allow source code use to the new names.
 */
#endif  /* !__LP64__ */

/*
 *  IUMagPString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUMagIDPString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUTextOrder()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IULangOrder()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */



/*
 *  IUScriptOrder()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUMagString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUMagIDString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUCompPString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUEqualPString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUStringOrder()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUCompString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUEqualString()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


#if CALL_NOT_IN_CARBON
#if TARGET_OS_MAC
    #define CompareString(aStr, bStr, itl2Handle) \
             IUCompPString(aStr, bStr, itl2Handle)
#endif
#define CompareText(aPtr, bPtr, aLen, bLen, itl2Handle) \
         IUMagPString(aPtr, bPtr, aLen, bLen, itl2Handle)
#define IdenticalString(aStr, bStr, itl2Handle) \
         IUEqualPString(aStr, bStr, itl2Handle)
#define IdenticalText(aPtr, bPtr, aLen, bLen, itl2Handle) \
         IUMagIDPString(aPtr, bPtr, aLen, bLen, itl2Handle)
#define StringOrder(aStr, bStr, aScript, bScript, aLang, bLang) \
         IUStringOrder(aStr, bStr, aScript, bScript, aLang, bLang)
#define TextOrder(aPtr, bPtr, aLen, bLen, aScript, bScript, aLang, bLang) \
         IUTextOrder(aPtr, bPtr, aLen, bLen, aScript, bScript, aLang, bLang)
#define LanguageOrder(language1, language2) \
         IULangOrder(language1, language2)
#endif /* CALL_NOT_IN_CARBON */
/*
 *  iucomppstring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iuequalpstring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iustringorder()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iucompstring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iuequalstring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */



#if !__LP64__
/*
 *  RelString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare or UCCompareText instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare or UCCompareText instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern short 
RelString(
  ConstStr255Param   str1,
  ConstStr255Param   str2,
  Boolean            caseSensitive,
  Boolean            diacSensitive)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  EqualString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
EqualString(
  ConstStr255Param   str1,
  ConstStr255Param   str2,
  Boolean            caseSensitive,
  Boolean            diacSensitive)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  relstring()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCompare or UCCompareText instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCompare or UCCompareText instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern short 
relstring(
  const char *  str1,
  const char *  str2,
  Boolean       caseSensitive,
  Boolean       diacSensitive)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  equalstring()
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


#endif  /* !__LP64__ */




#ifdef __cplusplus
}
#endif

#endif /* __STRINGCOMPARE__ */

