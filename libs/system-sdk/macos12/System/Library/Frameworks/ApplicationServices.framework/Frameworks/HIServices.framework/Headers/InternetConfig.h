/*
     File:       HIServices/InternetConfig.h
 
     Contains:   Internet Config interfaces
 
     Copyright:  � 1999-2012 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

/*

    ***DEPRECATION NOTICE***

    The Internet Config APIs are officially deprecated in 10.7.

    You can replace your use of Internet Config APIs with LaunchServices APIs.
    For example, to find which application is currently preferred for a
    particular MIME type, use LSCopyApplicationForMIMEType().

*/

/*
    IMPORTANT NOTES ABOUT THE C HEADERS
    -----------------------------------

    o   When you see the parameter 'y *x', you should be aware that
        you *cannot pass in nil*.  In future this restriction may be eased,
        especially for the attr parameter to ICGetPref.  Parameters where nil
        is legal are declared using the explicit pointer type, ie 'yPtr x'.

    o   Strings are *Pascal* strings.  This means that they must be word aligned.
        MPW and Think C do this automatically.  The last time I checked, Metrowerks
        C does not.  If it still doesn't, then IMHO it's a bug in their compiler
        and you should report it to them.  [IC 1.4 and later no longer require
        word aligned strings.  You can ignore this warning if you require IC 1.4
        or greater.]
*/
/***********************************************************************************************/

#ifndef __INTERNETCONFIG__
#define __INTERNETCONFIG__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __AE__
#include <AE/AE.h>
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/************************************************************************************************
  IC error codes
 ************************************************************************************************/

enum {
  icPrefNotFoundErr             = -666, /* preference not found (duh!)  */
  icPermErr                     = -667, /* cannot set preference  */
  icPrefDataErr                 = -668, /* problem with preference data  */
  icInternalErr                 = -669, /* hmm, this is not good  */
  icTruncatedErr                = -670, /* more data was present than was returned  */
  icNoMoreWritersErr            = -671, /* you cannot begin a write session because someone else is already doing it  */
  icNothingToOverrideErr        = -672, /* no component for the override component to capture  */
  icNoURLErr                    = -673, /* no URL found  */
  icConfigNotFoundErr           = -674, /* no configuration was found  */
  icConfigInappropriateErr      = -675, /* incorrect manufacturer code  */
  icProfileNotFoundErr          = -676, /* profile not found  */
  icTooManyProfilesErr          = -677  /* too many profiles in database  */
};

/************************************************************************************************
  IC versions (not necessarily, but historically, from a component)
 ************************************************************************************************/

enum {
  kICComponentInterfaceVersion0 = 0x00000000, /* IC >= 1.0  */
  kICComponentInterfaceVersion1 = 0x00010000, /* IC >= 1.1  */
  kICComponentInterfaceVersion2 = 0x00020000, /* IC >= 1.2  */
  kICComponentInterfaceVersion3 = 0x00030000, /* IC >= 2.0  */
  kICComponentInterfaceVersion4 = 0x00040000, /* IC >= 2.5  */
  kICComponentInterfaceVersion  = kICComponentInterfaceVersion4 /* current version number is 4  */
};

/************************************************************************************************
  opaque type for preference reference
 ************************************************************************************************/

typedef struct OpaqueICInstance*        ICInstance;
#if !__LP64__
/************************************************************************************************
  a record that specifies a folder, an array of such records, and a pointer to such an array
 ************************************************************************************************/
struct ICDirSpec {
  short               vRefNum;
  long                dirID;
};
typedef struct ICDirSpec                ICDirSpec;
typedef ICDirSpec                       ICDirSpecArray[4];
typedef ICDirSpecArray *                ICDirSpecArrayPtr;
#endif  /* !__LP64__ */

/************************************************************************************************
  preference attributes type, bit number constants, and mask constants
 ************************************************************************************************/
typedef UInt32                          ICAttr;
enum {
  kICAttrLockedBit              = 0,
  kICAttrVolatileBit            = 1
};

enum {
  kICAttrNoChange               = (UInt32)0xFFFFFFFF, /* pass this to ICSetPref to tell it not to change the attributes  */
  kICAttrLockedMask             = 0x00000001,
  kICAttrVolatileMask           = 0x00000002
};

/************************************************************************************************
  permissions for use with ICBegin
 ************************************************************************************************/
typedef UInt8                           ICPerm;
enum {
  icNoPerm                      = 0,
  icReadOnlyPerm                = 1,
  icReadWritePerm               = 2
};


/************************************************************************************************
  profile IDs
 ************************************************************************************************/
typedef SInt32                          ICProfileID;
typedef ICProfileID *                   ICProfileIDPtr;
enum {
  kICNilProfileID               = 0
};

/************************************************************************************************
  other constants
 ************************************************************************************************/
enum {
  kICNoUserInteractionBit       = 0
};

enum {
  kICNoUserInteractionMask      = 0x00000001
};

enum {
  kICFileType                   = 'ICAp',
  kICCreator                    = 'ICAp'
};

/************************************************************************************************
  Apple event constants
 ************************************************************************************************/
enum {
  kInternetEventClass           = 'GURL',
  kAEGetURL                     = 'GURL',
  kAEFetchURL                   = 'FURL',
  keyAEAttaching                = 'Atch'
};

/* AERegistry.i defines a compatible keyAEDestination */
enum {
  kICEditPreferenceEventClass   = 'ICAp',
  kICEditPreferenceEvent        = 'ICAp',
  keyICEditPreferenceDestination = 'dest'
};

/************************************************************************************************
  constants for use with ICGetVersion
 ************************************************************************************************/
enum {
  kICComponentVersion           = 0,    /* Return a component version, comparable to kICComponentInterfaceVersion  */
  kICNumVersion                 = 1     /* Return a NumVersion structure  */
};

/************************************************************************************************
  types and constants for use with kICDocumentFont, et. al.
 ************************************************************************************************/
struct ICFontRecord {
  SInt16              size;
  Style               face;
  char                pad;
  Str255              font;
};
typedef struct ICFontRecord             ICFontRecord;
typedef ICFontRecord *                  ICFontRecordPtr;
typedef ICFontRecordPtr *               ICFontRecordHandle;

/************************************************************************************************
  types and constants for use with kICCharacterSet, et. al.
 ************************************************************************************************/
struct ICCharTable {
  unsigned char       netToMac[256];
  unsigned char       macToNet[256];
};
typedef struct ICCharTable              ICCharTable;
typedef ICCharTable *                   ICCharTablePtr;
typedef ICCharTablePtr *                ICCharTableHandle;

/************************************************************************************************
  types and constants for use with kICHelper, et. al.
 ************************************************************************************************/
struct ICAppSpec {
  OSType              fCreator;
  Str63               name;
};
typedef struct ICAppSpec                ICAppSpec;
typedef ICAppSpec *                     ICAppSpecPtr;
typedef ICAppSpecPtr *                  ICAppSpecHandle;
struct ICAppSpecList {
  SInt16              numberOfItems;
  ICAppSpec           appSpecs[1];
};
typedef struct ICAppSpecList            ICAppSpecList;
typedef ICAppSpecList *                 ICAppSpecListPtr;
typedef ICAppSpecListPtr *              ICAppSpecListHandle;

/************************************************************************************************
  types and constants for use with kICDownloadFolder, et. al.
 ************************************************************************************************/
struct ICFileSpec {
  Str31               volName;                /* this field should be ignored, use the alias */
  SInt32              volCreationDate;        /* this field should be ignored, use the alias */
  FSSpec              fss;                    /* this field should be ignored, use the alias */
  AliasRecord         alias;
                                              /* plus extra data, aliasSize 0 means no alias manager present when*/
                                              /* ICFileSpecification was created*/
};
typedef struct ICFileSpec               ICFileSpec;
typedef ICFileSpec *                    ICFileSpecPtr;
typedef ICFileSpecPtr *                 ICFileSpecHandle;
enum {
  kICFileSpecHeaderSize         = sizeof(ICFileSpec) - sizeof(AliasRecord)
};

/************************************************************************************************
  types and constants for use with ICMapFilename, et. al.
 ************************************************************************************************/
typedef SInt32                          ICMapEntryFlags;
typedef SInt16                          ICFixedLength;
struct ICMapEntry {
  SInt16              totalLength;
  ICFixedLength       fixedLength;
  SInt16              version;
  OSType              fileType;
  OSType              fileCreator;
  OSType              postCreator;
  ICMapEntryFlags     flags;
                                              /* variable part starts here*/
  Str255              extension;
  Str255              creatorAppName;
  Str255              postAppName;
  Str255              MIMEType;
  Str255              entryName;
};
typedef struct ICMapEntry               ICMapEntry;
typedef ICMapEntry *                    ICMapEntryPtr;
typedef ICMapEntryPtr *                 ICMapEntryHandle;
enum {
  kICMapFixedLength             = 22
};

enum {
  kICMapBinaryBit               = 0,    /* file should be transfered in binary as opposed to text mode*/
  kICMapResourceForkBit         = 1,    /* the resource fork of the file is significant*/
  kICMapDataForkBit             = 2,    /* the data fork of the file is significant*/
  kICMapPostBit                 = 3,    /* post process using post fields*/
  kICMapNotIncomingBit          = 4,    /* ignore this mapping for incoming files*/
  kICMapNotOutgoingBit          = 5     /* ignore this mapping for outgoing files*/
};

enum {
  kICMapBinaryMask              = 0x00000001, /* file should be transfered in binary as opposed to text mode*/
  kICMapResourceForkMask        = 0x00000002, /* the resource fork of the file is significant*/
  kICMapDataForkMask            = 0x00000004, /* the data fork of the file is significant*/
  kICMapPostMask                = 0x00000008, /* post process using post fields*/
  kICMapNotIncomingMask         = 0x00000010, /* ignore this mapping for incoming files*/
  kICMapNotOutgoingMask         = 0x00000020 /* ignore this mapping for outgoing files*/
};

/************************************************************************************************
  types and constants for use with kICServices, et. al.
 ************************************************************************************************/
typedef SInt16                          ICServiceEntryFlags;
struct ICServiceEntry {
  Str255              name;
  SInt16              port;
  ICServiceEntryFlags  flags;
};
typedef struct ICServiceEntry           ICServiceEntry;
typedef ICServiceEntry *                ICServiceEntryPtr;
typedef ICServiceEntryPtr *             ICServiceEntryHandle;

enum {
  kICServicesTCPBit             = 0,
  kICServicesUDPBit             = 1     /* both bits can be set, which means the service is both TCP and UDP, eg daytime*/
};

enum {
  kICServicesTCPMask            = 0x00000001,
  kICServicesUDPMask            = 0x00000002 /* both bits can be set, which means the service is both TCP and UDP, eg daytime*/
};

struct ICServices {
  SInt16              count;
  ICServiceEntry      services[1];
};
typedef struct ICServices               ICServices;
typedef ICServices *                    ICServicesPtr;
typedef ICServicesPtr *                 ICServicesHandle;
/************************************************************************************************
  keys
 ************************************************************************************************/
/* 
    key reserved for use by Internet Config 
*/
#define kICReservedKey                  "\pkICReservedKey"
/*
    STR# -- formatted, list of Archie servers  
*/
#define kICArchieAll                    "\pArchieAll"
/*
    PString -- formatted, preferred Archie server   
*/
#define kICArchiePreferred              "\pArchiePreferred"
/*
    ICCharTable -- Mac-to-Net and Net-to-Mac character translation   
*/
#define kICCharacterSet                 "\pCharacterSet"
/*
    ICFontRecord -- font used for proportional text   
*/
#define kICDocumentFont                 "\pDocumentFont"
/*
    ICFileSpec -- where to put newly downloaded files   
*/
#define kICDownloadFolder               "\pDownloadFolder"
/*
    PString -- user@host.domain, email address of user, ie return address   
*/
#define kICEmail                        "\pEmail"
/*
    PString -- host.domain, default FTP server   
*/
#define kICFTPHost                      "\pFTPHost"
/*
    PString -- second level FTP proxy authorisation   
*/
#define kICFTPProxyAccount              "\pFTPProxyAccount"
/*
    PString -- host.domain   
*/
#define kICFTPProxyHost                 "\pFTPProxyHost"
/*
    PString -- scrambled, password for FTPProxyUser   
*/
#define kICFTPProxyPassword             "\pFTPProxyPassword"
/*
    PString -- first level FTP proxy authorisation   
*/
#define kICFTPProxyUser                 "\pFTPProxyUser"
/*
    PString -- host.domain, default finger server   
*/
#define kICFingerHost                   "\pFingerHost"
/*
    PString -- host.domain, default Gopher server   
*/
#define kICGopherHost                   "\pGopherHost"
/*
    PString -- host.domain, see note in Prog Docs   
*/
#define kICGopherProxy                  "\pGopherProxy"
/*
    PString -- host.domain   
*/
#define kICHTTPProxyHost                "\pHTTPProxyHost"
/*
    ICAppSpec -- helpers for URL schemes   
*/
#define kICHelper                       "\pHelper�"
/*
    PString -- description for URL scheme   
*/
#define kICHelperDesc                   "\pHelperDesc�"
/*
    ICAppSpecList -- list of common helpers for URL schemes   
*/
#define kICHelperList                   "\pHelperList�"
/*
    PString -- host.domain, Internet Relay Chat server   
*/
#define kICIRCHost                      "\pIRCHost"
/*
    STR# -- formatted, list of Info-Mac servers   
*/
#define kICInfoMacAll                   "\pInfoMacAll"
/*
    PString -- formatted, preferred Info-Mac server   
*/
#define kICInfoMacPreferred             "\pInfoMacPreferred"
/*
    PString -- string LDAP thing   
*/
#define kICLDAPSearchbase               "\pLDAPSearchbase"
/*
    PString -- host.domain   
*/
#define kICLDAPServer                   "\pLDAPServer"
/*
    ICFontRecord -- font used for lists of items (eg news article lists)   
*/
#define kICListFont                     "\pListFont"
/*
    PString -- host for MacSearch queries   
*/
#define kICMacSearchHost                "\pMacSearchHost"
/*
    PString -- user@host.domain, account from which to fetch mail   
*/
#define kICMailAccount                  "\pMailAccount"
/*
    TEXT -- extra headers for mail messages   
*/
#define kICMailHeaders                  "\pMailHeaders"
/*
    PString -- scrambled, password for MailAccount   
*/
#define kICMailPassword                 "\pMailPassword"
/*
    ICMapEntries -- file type mapping, see documentation   
*/
#define kICMapping                      "\pMapping"
/*
    PString -- host.domain, NNTP server   
*/
#define kICNNTPHost                     "\pNNTPHost"
/*
    PString -- host.domain, Network Time Protocol (NTP)   
*/
#define kICNTPHost                      "\pNTPHost"
/*
    Boolean   
*/
#define kICNewMailDialog                "\pNewMailDialog"
/*
    Boolean -- how to announce new mail   
*/
#define kICNewMailFlashIcon             "\pNewMailFlashIcon"
/*
    Boolean   
*/
#define kICNewMailPlaySound             "\pNewMailPlaySound"
/*
    PString   
*/
#define kICNewMailSoundName             "\pNewMailSoundName"
/*
    PString -- scrambled, password for NewsAuthUsername   
*/
#define kICNewsAuthPassword             "\pNewsAuthPassword"
/*
    PString -- user name for authorised news servers   
*/
#define kICNewsAuthUsername             "\pNewsAuthUsername"
/*
    TEXT -- extra headers for news messages   
*/
#define kICNewsHeaders                  "\pNewsHeaders"
/*
    STR# -- list of domains not to be proxied   
*/
#define kICNoProxyDomains               "\pNoProxyDomains"
/*
    PString -- for X-Organization string   
*/
#define kICOrganization                 "\pOrganization"
/*
    PString -- host.domain, default Ph server   
*/
#define kICPhHost                       "\pPhHost"
/*
    TEXT -- default response for finger servers   
*/
#define kICPlan                         "\pPlan"
/*
    ICFontRecord -- font used to print ScreenFont   
*/
#define kICPrinterFont                  "\pPrinterFont"
/*
    PString -- used to quote responses in news and mail   
*/
#define kICQuotingString                "\pQuotingString"
/*
    PString -- real name of user   
*/
#define kICRealName                     "\pRealName"
/*
    PString -- RTSP Proxy Host
*/
#define kICRTSPProxyHost                "\pRTSPProxyHost"
/*
    PString -- host.domain, SMTP server   
*/
#define kICSMTPHost                     "\pSMTPHost"
/*
    ICFontRecord -- font used for monospaced text (eg news articles)   
*/
#define kICScreenFont                   "\pScreenFont"
/*
    ICServices -- TCP and IP port-to-name mapping   
*/
#define kICServices                     "\pServices"
/*
    TEXT -- append to news and mail messages   
*/
#define kICSignature                    "\pSignature"
/*
    TEXT -- preferred mailing address   
*/
#define kICSnailMailAddress             "\pSnailMailAddress"
/*
    PString -- host.domain, remember that host.domain format allows ":port" and " port"  
*/
#define kICSocksHost                    "\pSocksHost"
/*
    PString -- host.domain, default Telnet address   
*/
#define kICTelnetHost                   "\pTelnetHost"
/*
    STR# -- formatted, list of UMich servers   
*/
#define kICUMichAll                     "\pUMichAll"
/*
    PString -- formatted, preferred UMich server   
*/
#define kICUMichPreferred               "\pUMichPreferred"
/*
    Boolean   
*/
#define kICUseFTPProxy                  "\pUseFTPProxy"
/*
    Boolean   
*/
#define kICUseGopherProxy               "\pUseGopherProxy"
/*
    Boolean   
*/
#define kICUseHTTPProxy                 "\pUseHTTPProxy"
/*
    Boolean -- use PASV command for FTP transfers   
*/
#define kICUsePassiveFTP                "\pUsePassiveFTP"
/*
    Boolean
*/
#define kICUseRTSPProxy                 "\pUseRTSPProxy"
/*
    Boolean   
*/
#define kICUseSocks                     "\pUseSocks"
/*
    PString -- no idea   
*/
#define kICWAISGateway                  "\pWAISGateway"
/*
    PString -- URL, users default WWW page   
*/
#define kICWWWHomePage                  "\pWWWHomePage"
/*
    RGBColor -- background colour for web pages   
*/
#define kICWebBackgroundColour          "\pWebBackgroundColour"
/*
    RGBColor -- colour for read links   
*/
#define kICWebReadColor                 "\p646F6777쩦ebReadColor"
/*
    PString -- URL, users default search page   
*/
#define kICWebSearchPagePrefs           "\pWebSearchPagePrefs"
/*
    RGBColor -- colour for normal text   
*/
#define kICWebTextColor                 "\pWebTextColor"
/*
    Boolean -- whether to underline links   
*/
#define kICWebUnderlineLinks            "\p646F6777쩦ebUnderlineLinks"
/*
    RGBColor -- colour for unread links   
*/
#define kICWebUnreadColor               "\p646F6777쩦ebUnreadColor"
/*
    PString -- host.domain, default whois server   
*/
#define kICWhoisHost                    "\pWhoisHost"

/************************************************************************************************

      FUNCTIONS

      What do the annotations after each API mean?
      --------------------------------------------

      [r1] Requires IC 1.1 or higher.
      [r2] Requires IC 1.2 or higher.
      [r3] Requires IC 2.0 or higher.
      [r4] Requires IC 2.5 or higher.
      
      IMPORTANT:

      In IC 2.5, instances automatically use the default configuration.
      You no longer need to configure an instance explicitly, except
      if your code might run with an older version of IC.  So the following
      notes only apply to IC 2.0 and earlier.

      [c1]  You must have specified a configuration before calling this routine.
      
      [c2]  You must have specified the default configuration before calling this
            routine.
      
      [c3]  You do not need to specify a configuration before calling this routine.
      
      [b1]  You must be inside a Begin/End pair when calling this routine.
      
      [b2]  You must be inside a Begin/End read/write pair when calling this routine.
      
      [b3]  You do not need to be inside a Begin/End pair when calling this routine.
      
      [b4]  If you are getting or setting multiple preferences, you should make this
            call inside a Begin/End pair. If you do not make this call inside a Begin/End
            pair, the call will automatically do it for you.
      
      [b5]  It is illegal to call this routine inside a Begin/End pair.

 ************************************************************************************************/

/* ***** Starting Up and Shutting Down *****  */
/*
 *  ICStart()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICStart(
  ICInstance *  inst,
  OSType        signature)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* Call this at application initialisation. Set signature to a value
   * which has been regsitered with DTS to allow for future expansion
   * of the IC system. Returns inst as a connection to the IC system.
   */
/*
 *  ICStop()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICStop(ICInstance inst)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [b5] 
   * Call this at application initialisation, after which inst
   * is no longer valid connection to IC.
   */
/*
 *  ICGetVersion()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetVersion(
  ICInstance   inst,
  long         whichVersion,
  UInt32 *     version)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r4] [c3] [b3] 
   * Returns the version of Internet Config.  Pass kICComponentVersion
   * to get the version as previously returned by GetComponenVerson.
   * Pass kICNumVersion to get a NumVersion structure.
   */
/*
 *  ICGetConfigName()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetConfigName(
  ICInstance   inst,
  Boolean      longname,
  Str255       name)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r2] [c1] [b3] 
   * Returns a string that describes the current configuration at a user
   * level. Set longname to true if you want a long name, up to 255
   * characters, or false if you want a short name, typically about 32
   * characters.
   * The returned string is for user display only. If you rely on the
   * exact format of it, you will conflict with any future IC
   * implementation that doesn't use explicit preference files.
   */
/* ***** Getting Information *****  */
/*
 *  ICGetSeed()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetSeed(
  ICInstance   inst,
  long *       seed)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c3] [b3] 
   * Returns the current seed for the IC prefs database.
   * This seed changes each time a non-volatile preference is changed.
   * You can poll this to determine if any cached preferences change.
   */
/*
 *  ICGetPerm()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetPerm(
  ICInstance   inst,
  ICPerm *     perm)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c3] [b3] 
   * Returns the access permissions currently associated with this instance.
   * While applications normally know what permissions they have,
   * this routine is designed for use by override components.
   */
/* ***** Reading and Writing Preferences *****  */
/*
 *  ICBegin()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICBegin(
  ICInstance   inst,
  ICPerm       perm)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c1] [b5] 
   * Starting reading or writing multiple preferences.
   * A call to this must be balanced by a call to ICEnd.
   * Do not call WaitNextEvent between these calls.
   * The perm specifies whether you intend to read or read/write.
   * Only one writer is allowed per instance.
   * Note that this may open resource files that are not closed
   * until you call ICEnd.
   */
/*
 *  ICGetPref()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetPref(
  ICInstance         inst,
  ConstStr255Param   key,
  ICAttr *           attr,
  void *             buf,
  long *             size)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c1] [b4] 
   * Reads the preference specified by key from the IC database to the
   * buffer pointed to by buf and size.
   * key must not be the empty string.
   * If buf is nil then no data is returned.
   * size must be non-negative.
   * attr and size are always set on return. On errors (except icTruncatedErr)
   * attr is set to ICattr_no_change and size is set to 0.
   * size is the actual size of the data.
   * attr is set to the attributes associated with the preference.
   * If this routine returns icTruncatedErr then the other returned
   * values are valid except that only the first size bytes have been
   * return. size is adjusted to reflect the true size of the preference.
   * Returns icPrefNotFound if there is no preference for the key.
   */
/*
 *  ICSetPref()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICSetPref(
  ICInstance         inst,
  ConstStr255Param   key,
  ICAttr             attr,
  const void *       buf,
  long               size)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c1] [b4] 
   * Sets the preference specified by key from the IC database to the
   * value pointed to by buf and size.
   * key must not be the empty string.
   * size must be non-negative. 
   * If buf is nil then the preference value is not set and size is ignored.
   * If buf is not nil then the preference value is set to the size
   * bytes pointed to by buf.
   * If attr is ICattr_no_change then the preference attributes are not set.
   * Otherwise the preference attributes are set to attr.
   * Returns icPermErr if the previous ICBegin was passed icReadOnlyPerm.
   * Returns icPermErr if current attr is locked, new attr is locked and buf <> nil.
   */
/*
 *  ICFindPrefHandle()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICFindPrefHandle(
  ICInstance         inst,
  ConstStr255Param   key,
  ICAttr *           attr,
  Handle             prefh)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r2] [c1] [b4] 
   * This routine effectively replaces ICGetPrefHandle.
   * Reads the preference specified by key from the IC database into
   * a handle, prefh.
   * key must not be the empty string.
   * attr is set to the attributes associated with the preference.
   * You must set prefh to a non-nil handle before calling this routine.
   * If the preference does not exist, icPrefNotFoundErr is returned.
   */
/*
 *  ICGetPrefHandle()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetPrefHandle(
  ICInstance         inst,
  ConstStr255Param   key,
  ICAttr *           attr,
  Handle *           prefh)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b4] 
   * This routine is now obsolete. Use ICFindPrefHandle instead.
   * Reads the preference specified by key from the IC database into
   * a newly created handle, prefh.
   * key must not be the empty string.
   * attr is set to the attributes associated with the preference.
   * The incoming value of prefh is ignored.
   * A new handle is created in the current heap and returned in prefh.
   * If the routine returns an error, prefh is set to nil.
   * If the preference does not exist, no error is returned and prefh is set
   * to an empty handle.
   */
/*
 *  ICSetPrefHandle()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICSetPrefHandle(
  ICInstance         inst,
  ConstStr255Param   key,
  ICAttr             attr,
  Handle             prefh)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b4] 
   * Sets the preference specified by key from the IC database to the
   * value contained in prefh.
   * key must not be the empty string.
   * If prefh is nil then the preference value is not set.
   * If prefh is not nil then the preference value is set to the data
   * contained in it.
   * If attr is ICattr_no_change then the preference attributes are not set.
   * Otherwise the preference attributes are set to attr.
   * Returns icPermErr if the previous ICBegin was passed icReadOnlyPerm.
   * Returns icPermErr if current attr is locked, new attr is locked and prefh <> nil.
   */
/*
 *  ICCountPref()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICCountPref(
  ICInstance   inst,
  long *       count)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c1] [b1] 
   * Counts the total number of preferences.
   * If the routine returns an error, count is set to 0.
   */
/*
 *  ICGetIndPref()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetIndPref(
  ICInstance   inst,
  long         index,
  Str255       key)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c1] [b1] 
   * Returns the key of the index'th preference.
   * index must be positive.
   * Returns icPrefNotFoundErr if index is greater than the total number of preferences.
   * If the routine returns an error, key is undefined.
   */
/*
 *  ICDeletePref()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICDeletePref(
  ICInstance         inst,
  ConstStr255Param   key)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c1] [b2] 
   * Deletes the preference specified by key.
   * key must not be the empty string.
   * Returns icPrefNotFound if the preference specified by key is not present.
   */
/*
 *  ICEnd()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICEnd(ICInstance inst)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [c1] [b1] 
   * Terminates a preference session, as started by ICBegin.
   * You must have called ICBegin before calling this routine.
   */
/*
 *  ICGetDefaultPref()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetDefaultPref(
  ICInstance         inst,
  ConstStr255Param   key,
  Handle             prefH)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r4] [c3] [b5] 
   * Returns a default preference value for the specified key.  You
   * must pass in a valid prefH, which is resized to fit the data.
   */
/* ***** User Interface Stuff *****  */
/*
 *  ICEditPreferences()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICEditPreferences(
  ICInstance         inst,
  ConstStr255Param   key)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Instructs IC to display the user interface associated with editing
   * preferences and focusing on the preference specified by key.
   * If key is the empty string then no preference should be focused upon.
   * You must have specified a configuration before calling this routine.
   * You do not need to call ICBegin before calling this routine.
   * In the current implementation this launches the IC application
   * (or brings it to the front) and displays the window containing
   * the preference specified by key.
   * It may have a radically different implementation in future
   * IC systems.
   */
/* ***** URL Handling *****  */
/*
 *  ICLaunchURL()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICLaunchURL(
  ICInstance         inst,
  ConstStr255Param   hint,
  const void *       data,
  long               len,
  long *             selStart,
  long *             selEnd)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Parses a URL out of the specified text and feeds it off to the
   * appropriate helper.
   * hint indicates the default scheme for URLs of the form "name@address".
   * If hint is the empty string then URLs of that form are not allowed.
   * data points to the start of the text. It must not be nil.
   * len indicates the length of the text. It must be non-negative.
   * selStart and selEnd should be passed in as the current selection of
   * the text. This selection is given in the same manner as TextEdit,
   * ie if selStart = selEnd then there is no selection only an insertion
   * point. Also selStart � selEnd and 0 � selStart � len and 0 � selEnd � len.
   * selStart and selEnd are returned as the bounds of the URL. If the
   * routine returns an error then these new boundaries may be
   * invalid but they will be close.
   * The URL is parsed out of the text and passed off to the appropriate
   * helper using the GURL AppleEvent.
   */
/*
 *  ICParseURL()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICParseURL(
  ICInstance         inst,
  ConstStr255Param   hint,
  const void *       data,
  long               len,
  long *             selStart,
  long *             selEnd,
  Handle             url)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Parses a URL out of the specified text and returns it in a canonical form
   * in a handle.
   * hint indicates the default scheme for URLs of the form "name@address".
   * If hint is the empty string then URLs of that form are not allowed.
   * data points to the start of the text. It must not be nil.
   * len indicates the length of the text. It must be non-negative.
   * selStart and selEnd should be passed in as the current selection of
   * the text. This selection is given in the same manner as TextEdit,
   * ie if selStart = selEnd then there is no selection only an insertion
   * point. Also selStart � selEnd and 0 � selStart � len and 0 � selEnd � len.
   * selStart and selEnd are returned as the bounds of the URL. If the
   * routine returns an error then these new boundaries may be
   * invalid but they will be close.
   * The incoming url handle must not be nil.  The resulting URL is normalised
   * and copied into the url handle, which is resized to fit.
   */
/*
 *  ICCreateGURLEvent()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICCreateGURLEvent(
  ICInstance    inst,
  OSType        helperCreator,
  Handle        urlH,
  AppleEvent *  theEvent)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r4] [c1] [b3] 
   * Creates a GURL Apple event, targetted at the application whose creator
   * code is helperCreator, with a direct object containing the URL text from urlH.
   */
/*
 *  ICSendGURLEvent()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICSendGURLEvent(
  ICInstance    inst,
  AppleEvent *  theEvent)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r4] [c1] [b3] 
   * Sends theEvent to the target application.
   */
/* ***** Mappings Routines *****
 * 
 * Routines for interrogating mappings database.
 * 
 * ----- High Level Routines -----
  */
/*
 *  ICMapFilename()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICMapFilename(
  ICInstance         inst,
  ConstStr255Param   filename,
  ICMapEntry *       entry)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b4] 
   * Takes the name of an incoming file and returns the most appropriate
   * mappings database entry, based on its extension.
   * filename must not be the empty string.
   * Returns icPrefNotFoundErr if no suitable entry is found.
   */
/*
 *  ICMapTypeCreator()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICMapTypeCreator(
  ICInstance         inst,
  OSType             fType,
  OSType             fCreator,
  ConstStr255Param   filename,
  ICMapEntry *       entry)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b4] 
   * Takes the type and creator (and optionally the name) of an outgoing
   * file and returns the most appropriate mappings database entry.
   * The filename may be either the name of the outgoing file or
   * the empty string.
   * Returns icPrefNotFoundErr if no suitable entry found.
   */
/* ----- Mid Level Routines -----  */
/*
 *  ICMapEntriesFilename()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICMapEntriesFilename(
  ICInstance         inst,
  Handle             entries,
  ConstStr255Param   filename,
  ICMapEntry *       entry)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Takes the name of an incoming file and returns the most appropriate
   * mappings database entry, based on its extension.
   * entries must be a handle to a valid IC mappings database preference.
   * filename must not be the empty string.
   * Returns icPrefNotFoundErr if no suitable entry is found.
   */
/*
 *  ICMapEntriesTypeCreator()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICMapEntriesTypeCreator(
  ICInstance         inst,
  Handle             entries,
  OSType             fType,
  OSType             fCreator,
  ConstStr255Param   filename,
  ICMapEntry *       entry)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Takes the type and creator (and optionally the name) of an outgoing
   * file and returns the most appropriate mappings database entry.
   * entries must be a handle to a valid IC mappings database preference.
   * The filename may be either the name of the outgoing file or
   * the empty string.
   * Returns icPrefNotFoundErr if no suitable entry found.
   */
/* ----- Low Level Routines -----  */
/*
 *  ICCountMapEntries()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICCountMapEntries(
  ICInstance   inst,
  Handle       entries,
  long *       count)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Counts the number of entries in the mappings database.
   * entries must be a handle to a valid IC mappings database preference.
   * count is set to the number of entries.
   */
/*
 *  ICGetIndMapEntry()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetIndMapEntry(
  ICInstance    inst,
  Handle        entries,
  long          index,
  long *        pos,
  ICMapEntry *  entry)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Gets the index'th entry in the mappings database.
   * entries must be a handle to a valid IC mappings database preference.
   * index must be in the range from 1 to the number of entries in the database.
   * The value of pos is ignored on input. pos is set to the position of
   * the index'th entry in the database and is suitable for passing back
   * into ICSetMapEntry.
   * Does not return any user data associated with the entry.
   */
/*
 *  ICGetMapEntry()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetMapEntry(
  ICInstance    inst,
  Handle        entries,
  long          pos,
  ICMapEntry *  entry)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Returns the entry located at position pos in the mappings database.
   * entries must be a handle to a valid IC mappings database preference.
   * pos should be 0 to get the first entry. To get the subsequent entries, add
   * entry.total_size to pos and iterate.
   * Does not return any user data associated with the entry.
   */
/*
 *  ICSetMapEntry()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICSetMapEntry(
  ICInstance          inst,
  Handle              entries,
  long                pos,
  const ICMapEntry *  entry)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Sets the entry located at position pos in the mappings database.
   * entries must be a handle to a valid IC mappings database preference.
   * pos should be either a value returned from ICGetIndMapEntry or a value
   * calculated using ICGetMapEntry.
   * entry is a var parameter purely for stack space reasons. It is not
   * modified in any way.
   * Any user data associated with the entry is unmodified.
   */
/*
 *  ICDeleteMapEntry()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICDeleteMapEntry(
  ICInstance   inst,
  Handle       entries,
  long         pos)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Deletes the mappings database entry at pos.
   * entries must be a handle to a valid IC mappings database preference.
   * pos should be either a value returned from ICGetIndMapEntry or a value
   * calculated using ICGetMapEntry.
   * Also deletes any user data associated with the entry.
   */
/*
 *  ICAddMapEntry()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICAddMapEntry(
  ICInstance          inst,
  Handle              entries,
  const ICMapEntry *  entry)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r1] [c1] [b3] 
   * Adds an entry to the mappings database.
   * entries must be a handle to a valid IC mappings database preference.
   * The entry is added to the end of the entries database.
   * No user data is added.
   */
/* ***** Profile Management Routines *****  */
/*
 *  ICGetCurrentProfile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetCurrentProfile(
  ICInstance     inst,
  ICProfileID *  currentID)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r3] [c1] [b3] 
   * Returns the profile ID of the current profile.
   */
/*
 *  ICSetCurrentProfile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICSetCurrentProfile(
  ICInstance    inst,
  ICProfileID   newID)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r3] [c1] [b3] 
   * Sets the current profile to the profile specified in newProfile.
   */
/*
 *  ICCountProfiles()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICCountProfiles(
  ICInstance   inst,
  long *       count)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r3] [c1] [b1] 
   * Returns the total number of profiles.
   */
/*
 *  ICGetIndProfile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetIndProfile(
  ICInstance     inst,
  long           index,
  ICProfileID *  thisID)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r3] [c1] [b1] 
   * Returns the profile ID of the index'th profile.  index must be positive.
   * Returns icProfileNotFoundErr if index is greater than the total number
   * of profiles.
   */
/*
 *  ICGetProfileName()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICGetProfileName(
  ICInstance    inst,
  ICProfileID   thisID,
  Str255        name)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r3] [c1] [b3] 
   * Returns the name of a profile given its ID.  The name may not uniquely
   * identify the profile.  [That's what the profile ID is for!]  The name
   * is assumed to be in the system script.
   */
/*
 *  ICSetProfileName()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICSetProfileName(
  ICInstance         inst,
  ICProfileID        thisID,
  ConstStr255Param   name)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r3] [c1] [b3] 
   * This routine sets the name of the specified profile.  Profile names
   * need not be unique.  The name should be in the system script.
   */
/*
 *  ICAddProfile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICAddProfile(
  ICInstance     inst,
  ICProfileID    prototypeID,
  ICProfileID *  newID)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r3] [c1] [b2] 
   * If prototypeID = kICNilProfileID, this routine
   * creates a default profile, otherwise it creates a
   * profile by cloning the prototype profile.  The ID
   * of the new profile is returned in newID.
   * The new profile will be give a new, unique, name.
   * This does not switch to the new profile.
   */
/*
 *  ICDeleteProfile()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InternetConfig 2.5 and later
 */
extern OSStatus 
ICDeleteProfile(
  ICInstance    inst,
  ICProfileID   thisID)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/* [r3] [c1] [b2] 
   * This routine deletes the profile specified by
   * thisID.  Attempting to delete the current profile
   * or the last profile will return error.
   */

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __INTERNETCONFIG__ */

