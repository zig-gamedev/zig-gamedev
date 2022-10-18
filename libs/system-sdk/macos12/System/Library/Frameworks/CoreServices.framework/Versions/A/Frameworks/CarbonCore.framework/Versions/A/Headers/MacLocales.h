/*
     File:       CarbonCore/MacLocales.h
 
     Contains:   Types & prototypes for locale functions
 
     Copyright:  © 1998-2012 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MACLOCALES__
#define __MACLOCALES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MACERRORS__
#include <CarbonCore/MacErrors.h>
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
   -------------------------------------------------------------------------------------------------
   TYPES & CONSTANTS
   -------------------------------------------------------------------------------------------------
*/

typedef struct OpaqueLocaleRef*         LocaleRef;
typedef UInt32                          LocalePartMask;
enum {
                                        /* bit set requests the following:*/
  kLocaleLanguageMask           = 1L << 0, /* ISO 639-1 or -2 language code (2 or 3 letters)*/
  kLocaleLanguageVariantMask    = 1L << 1, /* custom string for language variant*/
  kLocaleScriptMask             = 1L << 2, /* ISO 15924 script code (2 letters)*/
  kLocaleScriptVariantMask      = 1L << 3, /* custom string for script variant*/
  kLocaleRegionMask             = 1L << 4, /* ISO 3166 country/region code (2 letters)*/
  kLocaleRegionVariantMask      = 1L << 5, /* custom string for region variant*/
  kLocaleAllPartsMask           = 0x0000003F /* all of the above*/
};

typedef FourCharCode                    LocaleOperationClass;
/* constants for LocaleOperationClass are in UnicodeUtilities interfaces*/
typedef FourCharCode                    LocaleOperationVariant;
struct LocaleAndVariant {
  LocaleRef           locale;
  LocaleOperationVariant  opVariant;
};
typedef struct LocaleAndVariant         LocaleAndVariant;

typedef UInt32                          LocaleNameMask;
enum {
                                        /* bit set requests the following:*/
  kLocaleNameMask               = 1L << 0, /* name of locale*/
  kLocaleOperationVariantNameMask = 1L << 1, /* name of LocaleOperationVariant*/
  kLocaleAndVariantNameMask     = 0x00000003 /* all of the above*/
};

/*
   -------------------------------------------------------------------------------------------------
   FUNCTION PROTOTYPES
   -------------------------------------------------------------------------------------------------
*/

/* Convert to or from LocaleRefs (and related utilities)*/

/*
 *  LocaleRefFromLangOrRegionCode()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleRefFromLangOrRegionCode(
  LangCode     lang,
  RegionCode   region,
  LocaleRef *  locale)                                        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  LocaleRefFromLocaleString()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleRefFromLocaleString(
  const char   localeString[],
  LocaleRef *  locale)                                        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  LocaleRefGetPartString()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleRefGetPartString(
  LocaleRef        locale,
  LocalePartMask   partMask,
  ByteCount        maxStringLen,
  char             partString[])                              __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



/*
 *  LocaleStringToLangAndRegionCodes()
 *  
 *  Summary:
 *    Map a CFLocale-style locale string to old Script Manager LangCode
 *    and RegionCode values.
 *  
 *  Parameters:
 *    
 *    localeString:
 *      A null-terminated C-string version of a CFLocale-style locale
 *      identifier of the sort that could be passed to
 *      CFLocaleCreateCanonicalLocaleIdentifierFromString, based on BCP
 *      47 language tags: <http://www.rfc-editor.org/rfc/bcp/bcp47.txt>.
 *    
 *    lang:
 *      A pointer to a LangCode to be set from the locale identifier;
 *      will be set to langUnspecified or -1 if no language code can be
 *      derived from the identifier. May be NULL if region is not also
 *      NULL.
 *    
 *    region:
 *      A pointer to a RegionCode to be set from the locale identifier;
 *      will be set to -1 if no language code can be derived from the
 *      identifier. May be NULL if lang is not also NULL.
 *  
 *  Result:
 *    OSStatus, noErr if operation successful, otherwise paramErr or
 *    possibly other errors.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 9.0 and later
 */
extern OSStatus 
LocaleStringToLangAndRegionCodes(
  const char    localeString[],
  LangCode *    lang,
  RegionCode *  region)                                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



/* Enumerate locales for a LocaleOperationClass */

/*
 *  LocaleOperationCountLocales()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFLocaleCopyAvailableLocaleIdentifiers instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFLocaleCopyAvailableLocaleIdentifiers instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleOperationCountLocales(
  LocaleOperationClass   opClass,
  ItemCount *            localeCount)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  LocaleOperationGetLocales()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFXxxxx instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFLocaleCopyAvailableLocaleIdentifiers instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleOperationGetLocales(
  LocaleOperationClass   opClass,
  ItemCount              maxLocaleCount,
  ItemCount *            actualLocaleCount,
  LocaleAndVariant       localeVariantList[])                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/* Get names for a locale (or a region's language)*/

/*
 *  LocaleGetName()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFLocaleCopyDisplayNameForPropertyValue instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFLocaleCopyDisplayNameForPropertyValue instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleGetName(
  LocaleRef                locale,
  LocaleOperationVariant   opVariant,
  LocaleNameMask           nameMask,
  LocaleRef                displayLocale,
  UniCharCount             maxNameLen,
  UniCharCount *           actualNameLen,
  UniChar                  displayName[])                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  LocaleCountNames()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFLocaleCopyAvailableLocaleIdentifiers instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFLocaleCopyAvailableLocaleIdentifiers instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleCountNames(
  LocaleRef                locale,
  LocaleOperationVariant   opVariant,
  LocaleNameMask           nameMask,
  ItemCount *              nameCount)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  LocaleGetIndName()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFLocaleCopyAvailableLocaleIdentifiers and
 *    CFLocaleCopyDisplayNameForPropertyValue instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFLocaleCopyAvailableLocaleIdentifiers and
 *    CFLocaleCopyDisplayNameForPropertyValue instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleGetIndName(
  LocaleRef                locale,
  LocaleOperationVariant   opVariant,
  LocaleNameMask           nameMask,
  ItemCount                nameIndex,
  UniCharCount             maxNameLen,
  UniCharCount *           actualNameLen,
  UniChar                  displayName[],
  LocaleRef *              displayLocale)                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



#if !__LP64__
/*
 *  LocaleGetRegionLanguageName()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFLocaleCreateCanonicalLocaleIdentifierFromScriptManagerCodes
 *    and CFLocaleCopyDisplayNameForPropertyValue instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFLocaleCreateCanonicalLocaleIdentifierFromScriptManagerCodes and
 *    CFLocaleCopyDisplayNameForPropertyValue instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 9.0 and later
 */
extern OSStatus 
LocaleGetRegionLanguageName(
  RegionCode   region,
  Str255       languageName)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);



/* Get names for a LocaleOperationClass*/

#endif  /* !__LP64__ */

/*
 *  LocaleOperationGetName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleOperationGetName(
  LocaleOperationClass   opClass,
  LocaleRef              displayLocale,
  UniCharCount           maxNameLen,
  UniCharCount *         actualNameLen,
  UniChar                displayName[])                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  LocaleOperationCountNames()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleOperationCountNames(
  LocaleOperationClass   opClass,
  ItemCount *            nameCount)                           __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  LocaleOperationGetIndName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in LocalesLib 8.6 and later
 */
extern OSStatus 
LocaleOperationGetIndName(
  LocaleOperationClass   opClass,
  ItemCount              nameIndex,
  UniCharCount           maxNameLen,
  UniCharCount *         actualNameLen,
  UniChar                displayName[],
  LocaleRef *            displayLocale)                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __MACLOCALES__ */

