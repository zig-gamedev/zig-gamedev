/*
     File:       CarbonCore/NumberFormatting.h
 
     Contains:   Utilites for formatting numbers
 
     Copyright:  © 1996-2012 by Apple Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __NUMBERFORMATTING__
#define __NUMBERFORMATTING__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __INTLRESOURCES__
#include <CarbonCore/IntlResources.h>
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
    
    New Name                    Old Name(s)
    
    ExtendedToString            FormatX2Str
    FormatRecToString           Format2Str
    NumToString             
    StringToExtended            FormatStr2X
    StringToFormatRec           Str2Format
    StringToNum             

*/
struct NumFormatString {
  UInt8               fLength;
  UInt8               fVersion;
  char                data[254];              /* private data */
};
typedef struct NumFormatString          NumFormatString;
typedef NumFormatString                 NumFormatStringRec;
typedef short                           FormatStatus;
enum {
  fVNumber                      = 0     /* first version of NumFormatString */
};

typedef SInt8                           FormatClass;
enum {
  fPositive                     = 0,
  fNegative                     = 1,
  fZero                         = 2
};

typedef SInt8                           FormatResultType;
enum {
  fFormatOK                     = 0,
  fBestGuess                    = 1,
  fOutOfSynch                   = 2,
  fSpuriousChars                = 3,
  fMissingDelimiter             = 4,
  fExtraDecimal                 = 5,
  fMissingLiteral               = 6,
  fExtraExp                     = 7,
  fFormatOverflow               = 8,
  fFormStrIsNAN                 = 9,
  fBadPartsTable                = 10,
  fExtraPercent                 = 11,
  fExtraSeparator               = 12,
  fEmptyFormatString            = 13
};

struct FVector {
  short               start;
  short               length;
};
typedef struct FVector                  FVector;
/* index by [fPositive..fZero] */
typedef FVector                         TripleInt[3];
/*
 *  stringtonum()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  numtostring()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Depending on requirements, use CFStringCreateWithFormat / CFStringAppendFormat,
 *    CFNumberFormatterCreate + CFNumberFormatterCreateStringWithNumber, etc.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use alternates such as
 *    (depending on requirements) CFStringCreateWithFormat / CFStringAppendFormat,
 *    CFNumberFormatterCreate + CFNumberFormatterCreateStringWithNumber, etc.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
numtostring(
  long    theNum,
  char *  theString)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#if !__LP64__
/*
 *  StringToNum()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringGetIntValue instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringGetIntValue instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
StringToNum(
  ConstStr255Param   theString,
  long *             theNum)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  NumToString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFStringCreateWithFormat instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFStringCreateWithFormat instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
NumToString(
  long     theNum,
  Str255   theString)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  ExtendedToString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFNumberFormatterCreateStringWithNumber instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFNumberFormatterCreateStringWithNumber instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern FormatStatus 
ExtendedToString(
  const extended80 *       x,
  const NumFormatString *  myCanonical,
  const NumberParts *      partsTable,
  Str255                   outString)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  StringToExtended()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFNumberFormatterCreateNumberFromString instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFNumberFormatterCreateNumberFromString instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern FormatStatus 
StringToExtended(
  ConstStr255Param         source,
  const NumFormatString *  myCanonical,
  const NumberParts *      partsTable,
  extended80 *             x)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  StringToFormatRec()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFNumberFormatterSetFormat instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFNumberFormatterSetFormat instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern FormatStatus 
StringToFormatRec(
  ConstStr255Param     inString,
  const NumberParts *  partsTable,
  NumFormatString *    outString)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  FormatRecToString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFNumberFormatterSetFormat instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFNumberFormatterSetFormat instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern FormatStatus 
FormatRecToString(
  const NumFormatString *  myCanonical,
  const NumberParts *      partsTable,
  Str255                   outString,
  TripleInt                positions)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



#endif  /* !__LP64__ */

#if OLDROUTINENAMES
#define FormatX2Str(x, myCanonical, partsTable, outString)  \
         ExtendedToString( x, myCanonical, partsTable, outString)
#define FormatStr2X(source, myCanonical, partsTable, x)  \
         StringToExtended( source, myCanonical, partsTable, x)
#define Str2Format(inString, partsTable, outString)  \
         StringToFormatRec(inString, partsTable, outString)
#define Format2Str(myCanonical, partsTable, outString, positions)  \
         FormatRecToString(myCanonical, partsTable, outString, positions)
#endif  /* OLDROUTINENAMES */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __NUMBERFORMATTING__ */

