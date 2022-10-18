/*
     File:       CarbonCore/TextUtils.h
 
     Contains:   Text Utilities Interfaces.
 
     Copyright:  © 1985-2011 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTUTILS__
#define __TEXTUTILS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __NUMBERFORMATTING__
#include <CarbonCore/NumberFormatting.h>
#endif

#ifndef __STRINGCOMPARE__
#include <CarbonCore/StringCompare.h>
#endif

#ifndef __DATETIMEUTILS__
#include <CarbonCore/DateTimeUtils.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/*

    Here are the current System 7 routine names and the translations to the older forms.
    Please use the newer forms in all new code and migrate the older names out of existing
    code as maintainance permits.
    
    NEW NAME                    OLD NAMEs                   OBSOLETE FORM (no script code)

    FindScriptRun
    FindWordBreaks                                          NFindWord, FindWord
    GetIndString            
    GetString
    Munger
    NewString               
    SetString               
    StyledLineBreak
    TruncString
    TruncText

    UpperString ($A054)         UprString, UprText
    UppercaseText               SCUpperText (a only)        UpperText ($A456)
    LowercaseText                                           LwrString, LowerText, LwrText ($A056)
    StripDiacritics                                         StripText ($A256)
    UppercaseStripDiacritics                                StripUpperText ($A656)


*/

/* TruncCode, StyledLineBreakCode, and truncation constants moved to QuickDrawText.i */
#if !__LP64__
struct ScriptRunStatus {
  SInt8               script;
  SInt8               runVariant;
};
typedef struct ScriptRunStatus          ScriptRunStatus;
struct BreakTable {
  char                charTypes[256];
  short               tripleLength;
  short               triples[1];
};
typedef struct BreakTable               BreakTable;
typedef BreakTable *                    BreakTablePtr;
struct NBreakTable {
  SInt8               flags1;
  SInt8               flags2;
  short               version;
  short               classTableOff;
  short               auxCTableOff;
  short               backwdTableOff;
  short               forwdTableOff;
  short               doBackup;
  short               length;                 /* length of NBreakTable */
  char                charTypes[256];
  short               tables[1];
};
typedef struct NBreakTable              NBreakTable;
typedef NBreakTable *                   NBreakTablePtr;
#endif  /* !__LP64__ */

/* The following functions are new names that work on 68k and PowerPC*/

/*
 *  Munger()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CFStringFindAndReplace or CFDataFind plus CFDataReplaceBytes
 *    (or strstr plus memmove), depending on usage; see discussion.
 *  
 *  Discussion:
 *    For text operations performed with Munger, use
 *    CFStringFindAndReplace instead. 
 *    
 *    For operations on arbitrary bytes performed with Munger, use
 *    CFDataFind plus CFDataReplaceBytes instead (Another option is to
 *    use strstr plus memmove).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
Munger(
  Handle        h,
  long          offset,
  const void *  ptr1,
  long          len1,
  const void *  ptr2,
  long          len2)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_NA, __IPHONE_NA);


#if !__LP64__
/*
 *  NewString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCreateCopy instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCreateCopy instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern StringHandle 
NewString(ConstStr255Param theString)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCreateWithPascalString and CFStringReplaceAll instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCreateWithPascalString and CFStringReplaceAll instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetString(
  StringHandle       theString,
  ConstStr255Param   strNew)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFBundleCopyLocalizedString instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFBundleCopyLocalizedString instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern StringHandle 
GetString(short stringID)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  GetIndString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFBundleCopyLocalizedString instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFBundleCopyLocalizedString instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
GetIndString(
  Str255   theString,
  short    strListID,
  short    itemIndex)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  setstring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


#if !__LP64__
/*
 *  newstring()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCreateCopy instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCreateCopy instead.
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only] but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


#endif  /* !__LP64__ */

/*
 *  getindstring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


#if !__LP64__
/*
 *  FindWordBreaks()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use UCFindTextBreak instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCFindTextBreak instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
FindWordBreaks(
  Ptr             textPtr,
  short           textLength,
  short           offset,
  Boolean         leadingEdge,
  BreakTablePtr   breaks,
  OffsetTable     offsets,
  ScriptCode      script)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  LowercaseText()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringLowercase instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringLowercase instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
LowercaseText(
  Ptr          textPtr,
  short        len,
  ScriptCode   script)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  UppercaseText()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringUppercase instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringUppercase instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
UppercaseText(
  Ptr          textPtr,
  short        len,
  ScriptCode   script)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  StripDiacritics()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringTransform instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringTransform instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
StripDiacritics(
  Ptr          textPtr,
  short        len,
  ScriptCode   script)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  UppercaseStripDiacritics()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringTransform instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringTransform instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
UppercaseStripDiacritics(
  Ptr          textPtr,
  short        len,
  ScriptCode   script)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  FindScriptRun()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    No longer needed on MacOS X.
 *  
 *  Discussion:
 *    This function is obsolate.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ScriptRunStatus 
FindScriptRun(
  Ptr     textPtr,
  long    textLen,
  long *  lenUsed)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
    The following functions are old names, but are required for PowerPC builds
    because InterfaceLib exports these names, instead of the new ones.
*/

#endif  /* !__LP64__ */

/*
 *  FindWord()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  NFindWord()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
   On 68K machines, LwrText, LowerText, StripText, UpperText and StripUpperText
   return an error code in register D0, but System 7 PowerMacs do not emulate
   this properly, so checking D0 is unreliable.
*/

/*
 *  LwrText()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  LowerText()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  StripText()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  UpperText()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  StripUpperText()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */



/* The following are new names which are exported by InterfaceLib*/

#if !__LP64__
/*
 *  UpperString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringUppercase instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringUppercase instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
UpperString(
  Str255    theString,
  Boolean   diacSensitive)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  upperstring()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringUppercase instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringUppercase instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
upperstring(
  char *    theString,
  Boolean   diacSensitive)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/* The following are macros which map old names to the names exported by InterfaceLib*/
#if OLDROUTINENAMES
#define UprString(theString, diacSensitive)  \
         UpperString(theString, diacSensitive)
#endif  /* OLDROUTINENAMES */

/* Old routine name but no new names are mapped to it:*/
/*
 *  UprText()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
    Functions for converting between C and Pascal Strings
    (Previously in Strings.h)
    
    Note: CopyPascalStringToC, CopyCStringToPascal, c2pstrcpy, and p2cstrcpy
          are written to allow inplace conversion.  That is, the src and dst
          parameters can point to the memory location.  These functions
          are available in CarbonLib and CarbonAccessors.o.
          
    Note: c2pstr, C2PStr, p2cstr, and P2CStr are all deprecated.  These functions
          only do inplace conversion and often require casts to call them.  This can
          cause bugs because you can easily cast away a const and change the 
          contents of a read-only buffer.  These functions are available
          in InterfaceLib, or when building for Carbon if you #define OLDP2C,
          then they are available as a macro.
    
*/
#if !__LP64__
/*
 *  c2pstrcpy()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFString
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0.2 and later
 */
extern void 
c2pstrcpy(
  Str255        dst,
  const char *  src)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  p2cstrcpy()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFString
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0.2 and later
 */
extern void 
p2cstrcpy(
  char *             dst,
  ConstStr255Param   src)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  CopyPascalStringToC()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFString
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0.2 and later
 */
extern void 
CopyPascalStringToC(
  ConstStr255Param   src,
  char *             dst)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  CopyCStringToPascal()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFString instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFString
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0.2 and later
 */
extern void 
CopyCStringToPascal(
  const char *  src,
  Str255        dst)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  c2pstr()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern StringPtr 
c2pstr(char * aStr)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  C2PStr()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern StringPtr 
C2PStr(Ptr cString)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  p2cstr()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern char * 
p2cstr(StringPtr aStr)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  P2CStr()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Ptr 
P2CStr(StringPtr pString)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !TARGET_OS_MAC
    /* Added for QuickTime 3.0 */
    #define C2PStr(a)  (StringPtr)c2pstr((Ptr)(a))
    #define P2CStr(a)  (Ptr)p2cstr(a)

    #define CopyPascalStringToC(src,dst) p2cstrcpy(dst,src)
    #define CopyCStringToPascal(src,dst) c2pstrcpy(dst,src)
#endif
#if TARGET_OS_MAC && TARGET_API_MAC_CARBON && OLDP2C
    /* macros to help source code that uses deprecated inplace  */
    /* conversion routines to compiler for carbon */
    #define p2cstr(aStr) (p2cstrcpy((char *) aStr, aStr) , (char *) aStr)
    #define c2pstr(aStr) (c2pstrcpy((StringPtr)aStr, aStr) , (StringPtr) aStr)

    #define C2PStr(a)  (StringPtr)c2pstr((Ptr)(a))
    #define P2CStr(a)  (Ptr)p2cstr(a)
#endif



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __TEXTUTILS__ */

