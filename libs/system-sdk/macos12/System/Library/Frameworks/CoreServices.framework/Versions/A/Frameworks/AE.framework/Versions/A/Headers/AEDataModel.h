/*
     File:       AE/AEDataModel.h
 
     Contains:   AppleEvent Data Model Interfaces.
 
    
 
     Copyright:  � 1996-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AEDATAMODEL__
#define __AEDATAMODEL__

#ifndef __CARBONCORE__
#include <CarbonCore/CarbonCore.h>
#endif


#include <os/availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/* Apple event manager data types */
typedef ResType                         DescType;
typedef FourCharCode                    AEKeyword;

/* Apple event descriptor types */
CF_ENUM(DescType) {
  typeBoolean                   = 'bool',
  typeChar                      = 'TEXT' /* Deprecated, use typeUTF8Text instead. */
};

/*
 * The following descriptor types are deprecated due to their lack of
 * explicit encoding or byte order definition.  Please use
 * typeUTF16ExternalRepresentation or typeUTF8Text instead. */
CF_ENUM(DescType) {
  typeStyledUnicodeText         = 'sutx', /* Not implemented */
  typeEncodedString             = 'encs', /* Not implemented */
  typeUnicodeText               = 'utxt', /* native byte ordering, optional BOM */
  typeCString                   = 'cstr', /* MacRoman characters followed by a NULL byte */
  typePString                   = 'pstr' /* Unsigned length byte followed by MacRoman characters */
};

/*
 * The preferred unicode text types.  In both cases, there is no explicit null termination or length byte.
 */
CF_ENUM(DescType) {
  typeUTF16ExternalRepresentation = 'ut16', /* big-endian 16 bit unicode with optional byte-order-mark, or little-endian 16 bit unicode with required byte-order-mark. */
  typeUTF8Text                  = 'utf8' /* 8 bit unicode */
};

/* Preferred numeric Apple event descriptor types */
CF_ENUM(DescType) {
  typeSInt16                    = 'shor', /* SInt16 : signed, 16 bit integer */
  typeUInt16                    = 'ushr', /* UInt16 : unsigned, 16 bit integer */
  typeSInt32                    = 'long', /* SInt32 : signed, 32 bit integer */
  typeUInt32                    = 'magn', /* UInt32 : unsigned, 32 bit integer */
  typeSInt64                    = 'comp', /* SInt64 : signed, 64 bit integer */
  typeUInt64                    = 'ucom', /* UInt64 : unsigned, 64 bit integer */
  typeIEEE32BitFloatingPoint    = 'sing', /* float */
  typeIEEE64BitFloatingPoint    = 'doub', /* double */
  type128BitFloatingPoint       = 'ldbl',
  typeDecimalStruct             = 'decm'
};

/* Non-preferred Apple event descriptor types */
#if __LP64__
/*    On Mac OS X 64 bit, the following types have been removed because their meaning is unclear or confusing.
    For example, people have assumed that the appropriate data type for typeLongInteger would be long; but
    on 64 bit 'long' is a 64 bit value and the appropriate type should be typeComp.

    You should change your existing code to not use the following types, and use the more specific ones.  Check
    through your code to make sure that the datasize referenced by these types is the correct type.

        type constant       change to                   data type should be
        -------------       ---------                   -------------------
        typeSMInt           typeSInt16                  SInt16
        typeShortInteger    typeSInt16                  SInt16
        typeInteger         typeSInt32                  SInt32
        typeLongInteger     typeSInt32                  SInt32
        typeComp            typeSInt64                  SInt64 or Wide
        typeSMFloat         typeIEEE32BitFloatingPoint  Float32
        typeShortFloat      typeIEEE32BitFloatingPoint  Float32
        typeLongFloat       typeIEEE64BitFloatingPoint  Float64

    There is no good type on 64 bit to use for typeEntended; either change your code to pass typeIEEE64BitFloatingPoint
    for typeExtended and live with the reduction in range or use type128BitFloatingPoint.

*/
#else
CF_ENUM(OSType) {
  typeSMInt                     = typeSInt16,
  typeShortInteger              = typeSInt16,
  typeInteger                   = typeSInt32,
  typeLongInteger               = typeSInt32,
  typeMagnitude                 = typeUInt32,
  typeComp                      = typeSInt64,
  typeSMFloat                   = typeIEEE32BitFloatingPoint,
  typeShortFloat                = typeIEEE32BitFloatingPoint,
  typeFloat                     = typeIEEE64BitFloatingPoint,
  typeLongFloat                 = typeIEEE64BitFloatingPoint,
  typeExtended                  = 'exte'
};

#endif  /* __LP64__ */

/* More Apple event descriptor types */
CF_ENUM(DescType) {
  typeAEList                    = 'list',
  typeAERecord                  = 'reco',
  typeAppleEvent                = 'aevt',
  typeEventRecord               = 'evrc',
  typeTrue                      = 'true',
  typeFalse                     = 'fals',
  typeAlias                     = 'alis', /* AliasPtr, from a valid AliasHandle. Deprecated; use typeFileURL or typeBookmark data to refer to files */
  typeEnumerated                = 'enum',
  typeType                      = 'type', /* OSType */
  typeAppParameters             = 'appa',
  typeProperty                  = 'prop',
  typeFSRef                     = 'fsrf', /* FSRef.  Deprecated; use typeFileURL or typeBookmark data to refer to files in AppleEvents */
  typeFileURL                   = 'furl', /* a UTF-8 encoded full path, using native path separators */
  typeBookmarkData              = 'bmrk', /* the bytes of a CFURLBookmarkData */
  typeKeyword                   = 'keyw', /* OSType */
  typeSectionH                  = 'sect',
  typeWildCard                  = '****',
  typeApplSignature             = 'sign', /* OSType */
  typeQDRectangle               = 'qdrt', /* Deprecated */
  typeFixed                     = 'fixd',
  typeProcessSerialNumber       = 'psn ', /* ProcessSerialNumber */
  typeApplicationURL            = 'aprl',
  typeNull                      = 'null' /* null or nonexistent data */
};

#if !__LP64__
/*
    FSSpecs are deprecated on Mac OS X, and their use in AppleEvents is discouraged.  You should change
    your code to use typeFileURL or typeFileBookmark.  In __LP64__ code, coercions into typeFSS is not supported,
    and coercion from typeFSS is not guaranteed to work correctly in all cases.
*/
CF_ENUM(DescType) {
  typeFSS                       = 'fss ' /* FSSpec */
};

#endif  /* !__LP64__ */

CF_ENUM(DescType) {
  typeCFAttributedStringRef     = 'cfas',
  typeCFMutableAttributedStringRef = 'cfaa',
  typeCFStringRef               = 'cfst',
  typeCFMutableStringRef        = 'cfms',
  typeCFArrayRef                = 'cfar',
  typeCFMutableArrayRef         = 'cfma',
  typeCFDictionaryRef           = 'cfdc',
  typeCFMutableDictionaryRef    = 'cfmd',
  typeCFNumberRef               = 'cfnb',
  typeCFBooleanRef              = 'cftf',
  typeCFTypeRef                 = 'cfty'
};

/* New addressing modes for MacOS X */
CF_ENUM(DescType) {
  typeKernelProcessID           = 'kpid',
  typeMachPort                  = 'port'
};

	
CF_ENUM(DescType) {
	typeAuditToken			= 'tokn',	/* Mac OS X 10.8, returned as keyAuditTokenAttr and is a typedef audit_token_t */
};
										 
/* Targeting applications by bundle ID is only available in Mac OS X 10.3 or later. */
CF_ENUM(DescType) {
  typeApplicationBundleID       = 'bund'
};

/* Keywords for Apple event attributes */
CF_ENUM(AEKeyword) {
	keyTransactionIDAttr          = 'tran', /* AETransactionID */
	keyReturnIDAttr               = 'rtid', /* AEReturnID */
	keyEventClassAttr             = 'evcl', /* AEEventClass */
	keyEventIDAttr                = 'evid', /* AEEventID */
	keyAddressAttr                = 'addr',
	keyOptionalKeywordAttr        = 'optk',
	keyTimeoutAttr                = 'timo', /* SInt32 */
	keyInteractLevelAttr          = 'inte', /* this attribute is read only - will be set in AESend */
	keyEventSourceAttr            = 'esrc', /* this attribute is read only - returned as typeShortInteger */
	keyMissedKeywordAttr          = 'miss', /* this attribute is read only */
	keyOriginalAddressAttr        = 'from', /* new in 1.0.1 */
	keyAcceptTimeoutAttr          = 'actm', /* new for Mac OS X */
	keyReplyRequestedAttr         = 'repq', /* Was a reply requested for this event - returned as typeBoolean */
	#if __MAC_OS_X_VERSION_MIN_REQUIRED >= 1060
	keySenderEUIDAttr             = 'seid', /* read only, returned as typeSInt32.  Will be the euid of the sender of this event. */
	keySenderEGIDAttr             = 'sgid', /* read only, returned as typeSInt32.  Will be the egid of the sender of this event. */
	keySenderUIDAttr              = 'uids', /* read only, returned as typeSInt32.  Will be the uid of the sender of this event. */
	keySenderGIDAttr              = 'gids', /* read only, returned as typeSInt32.  Will be the gid of the sender of this event. */
	keySenderPIDAttr              = 'spid', /* read only, returned as typeSInt32.  Will be the pid of the sender of this event. */
	keySenderAuditTokenAttr		= 'tokn', /* read only, returned as an audit_token_t.  Will be the audit token of the sender of this event. */
	#endif
	#if __MAC_OS_X_VERSION_MIN_REQUIRED >= 1080
	keySenderApplescriptEntitlementsAttr		= 'entl', /* read only, an AEDesc containing opaque data representing the entitlements held by the sender. Interpreted by sandbox routines. */
	keySenderApplicationIdentifierEntitlementAttr = 'aiea',
	keySenderApplicationSandboxed = 'sssb', /* read-only, an AEDesc typeBoolean, true if the sender application was in an application sandbox */
	keyActualSenderAuditToken		= 'acat', /* read-only, an AEDesc typeAuditToken of the acual ( possibly over-ridden ) audit token for the sender of this event */
	#endif

	#if defined(__MAC_10_13) && __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_13
	keyAppleEventAttributesAttr = 'attr', /* read-only, an AEDescList of AEKeyword entries of the attributes on this event. */
	#endif
};

/* These bits are specified in the keyXMLDebuggingAttr (an SInt32) */
enum {
  kAEDebugPOSTHeader            = (1 << 0), /* headers of the HTTP post we sent - typeChar */
  kAEDebugReplyHeader           = (1 << 1), /* headers returned by the server */
  kAEDebugXMLRequest            = (1 << 2), /* the XML request we sent */
  kAEDebugXMLResponse           = (1 << 3), /* the XML reply from the server */
  kAEDebugXMLDebugAll           = (int)0xFFFFFFFF /* everything! */
};

/* These values can be added as a parameter to the direct object of a
   SOAP message to specify the serialization schema.  If not
   specified, kSOAP1999Schema is the default. These should be added as
   typeType. */
enum {
  kSOAP1999Schema               = 'ss99',
  kSOAP2001Schema               = 'ss01'
};

CF_ENUM(AEKeyword) {
                                        /* outgoing event attributes */
  keyUserNameAttr               = 'unam',
  keyUserPasswordAttr           = 'pass', /* not sent with the event */
  keyDisableAuthenticationAttr  = 'auth', /* When present and with a non zero value (that is, false, or integer 0), */
                                        /* AESend will not authenticate the user.  If not present, or with a non-zero*/
                                        /* value, AESend will prompt for authentication information from the user if the interaction level allows. */
  keyXMLDebuggingAttr           = 'xdbg', /* a bitfield of specifying which XML debugging data is to be returned with the event */
                                        /* Event class / id */
  kAERPCClass                   = 'rpc ', /* for outgoing XML events */
  kAEXMLRPCScheme               = 'RPC2', /* event ID: event should be sent to an XMLRPC endpoint */
  kAESOAPScheme                 = 'SOAP', /* event ID: event should be sent to a SOAP endpoint */
  kAESharedScriptHandler        = 'wscp', /* event ID: handler for incoming XML requests */
                                        /* these parameters exist as part of the direct object of the event for both incoming and outgoing requests */
  keyRPCMethodName              = 'meth', /* name of the method to call */
  keyRPCMethodParam             = 'parm', /* the list (or structure) of parameters */
  keyRPCMethodParamOrder        = '/ord', /* if a structure, the order of parameters (a list) */
                                        /* when keyXMLDebugginAttr so specifies, these additional parameters will be part of the reply. */
  keyAEPOSTHeaderData           = 'phed', /* what we sent to the server */
  keyAEReplyHeaderData          = 'rhed', /* what the server sent to us */
  keyAEXMLRequestData           = 'xreq', /* what we sent to the server */
  keyAEXMLReplyData             = 'xrep', /* what the server sent to us */
                                        /* additional parameters that can be specified in the direct object of the event */
  keyAdditionalHTTPHeaders      = 'ahed', /* list of additional HTTP headers (a list of 2 element lists) */
  keySOAPAction                 = 'sact', /* the SOAPAction header (required for SOAP messages) */
  keySOAPMethodNameSpace        = 'mspc', /* Optional namespace (defaults to m:) */
  keySOAPMethodNameSpaceURI     = 'mspu', /* Required namespace URI */
  keySOAPSchemaVersion          = 'ssch' /* Optional XML Schema version, defaults to kSOAP1999Schama */
};

/* 
   When serializing AERecords as SOAP structures, it is possible
   to specify the namespace and type of the structure.  To do this,
   add a keySOAPStructureMetaData record to the top level of the
   record to be serialized.  If present, this will be used to specify
   the structure namespace.  This will produce a structure elment that
   looks like:

    <myStruct
        xmlns:myNamespace="http://myUri.org/xsd",
        xsi:type="myNamespace:MyStructType">
        ...
    </myStruct>

*/
CF_ENUM(AEKeyword) {
  keySOAPStructureMetaData      = '/smd',
  keySOAPSMDNamespace           = 'ssns', /* "myNamespace"*/
  keySOAPSMDNamespaceURI        = 'ssnu', /* "http://myUri.org/xsd"*/
  keySOAPSMDType                = 'sstp' /* "MyStructType"*/
};

/* 
 * Web Services Proxy support.  Available only on Mac OS X 10.2 or later.
 * These constants should be added as attributes on the event that is
 * being sent (not part of the direct object.)
 */
enum {
                                        /* Automatically configure the proxy based on System Configuration */
  kAEUseHTTPProxyAttr           = 'xupr', /* a typeBoolean.  Defaults to true.*/
                                        /* manually specify the proxy host and port. */
  kAEHTTPProxyPortAttr          = 'xhtp', /* a typeSInt32*/
  kAEHTTPProxyHostAttr          = 'xhth' /* a typeChar*/
};

/*
 * Web Services SOCKS support.  kAEUseSocksAttr is a boolean that
 * specifies whether to automatically configure SOCKS proxies by
 * querying System Configuration.
 */
enum {
  kAESocks4Protocol             = 4,
  kAESocks5Protocol             = 5
};

enum {
  kAEUseSocksAttr               = 'xscs', /* a typeBoolean.  Defaults to true.*/
                                        /* This attribute specifies a specific SOCKS protocol to be used */
  kAESocksProxyAttr             = 'xsok', /* a typeSInt32*/
                                        /* if version >= 4 */
  kAESocksHostAttr              = 'xshs', /* a typeChar*/
  kAESocksPortAttr              = 'xshp', /* a typeSInt32*/
  kAESocksUserAttr              = 'xshu', /* a typeChar*/
                                        /* if version >= 5 */
  kAESocksPasswordAttr          = 'xshw' /* a typeChar*/
};


/*  Constants used for specifying the factoring of AEDescLists. */
enum {
  kAEDescListFactorNone         = 0,
  kAEDescListFactorType         = 4,
  kAEDescListFactorTypeAndSize  = 8
};

/* Constants used creating an AppleEvent */
enum {
                                        /* Constant for the returnID param of AECreateAppleEvent */
  kAutoGenerateReturnID         = -1,   /* AECreateAppleEvent will generate a session-unique ID */
                                        /* Constant for transaction IDs */
  kAnyTransactionID             = 0     /* no transaction is in use */
};

#if OPAQUE_TOOLBOX_STRUCTS
typedef struct OpaqueAEDataStorageType*  AEDataStorageType;
#else
typedef Ptr                             AEDataStorageType;
#endif  /* OPAQUE_TOOLBOX_STRUCTS */

typedef AEDataStorageType *             AEDataStorage;
struct AEDesc {
  DescType            descriptorType;
  AEDataStorage       dataHandle;
};
typedef struct AEDesc                   AEDesc;
typedef AEDesc *                        AEDescPtr;
struct AEKeyDesc {
  AEKeyword           descKey;
  AEDesc              descContent;
};
typedef struct AEKeyDesc                AEKeyDesc;
/* a list of AEDesc's is a special kind of AEDesc */

typedef AEDesc                          AEDescList;
/* AERecord is a list of keyworded AEDesc's */
typedef AEDescList                      AERecord;
/* an AEDesc which contains address data */
typedef AEDesc                          AEAddressDesc;
/* an AERecord that contains an AppleEvent, and related data types */
typedef AERecord                        AppleEvent;
typedef AppleEvent *                    AppleEventPtr;
typedef SInt16                          AEReturnID;
typedef SInt32                          AETransactionID;
typedef FourCharCode                    AEEventClass;
typedef FourCharCode                    AEEventID;
typedef SInt8                           AEArrayType;
enum {
  kAEDataArray                  = 0,
  kAEPackedArray                = 1,
  kAEDescArray                  = 3,
  kAEKeyDescArray               = 4
};


enum {
  kAEHandleArray                = 2
};

union AEArrayData {
  SInt16              kAEDataArray[1];
  char                kAEPackedArray[1];
  Handle              kAEHandleArray[1];
  AEDesc              kAEDescArray[1];
  AEKeyDesc           kAEKeyDescArray[1];
};
typedef union AEArrayData               AEArrayData;
typedef AEArrayData *                   AEArrayDataPointer;
/**************************************************************************
  These constants are used by AEMach and AEInteraction APIs.  They are not
  strictly part of the data format, but are declared here due to layering.
**************************************************************************/
typedef SInt16 AESendPriority;
enum {
  kAENormalPriority             = 0x00000000, /* post message at the end of the event queue */
  kAEHighPriority               = 0x00000001 /* post message at the front of the event queue (same as nAttnMsg) */
};


typedef SInt32 AESendMode;
enum {
  kAENoReply                    = 0x00000001, /* sender doesn't want a reply to event */
  kAEQueueReply                 = 0x00000002, /* sender wants a reply but won't wait */
  kAEWaitReply                  = 0x00000003, /* sender wants a reply and will wait */
  kAEDontReconnect              = 0x00000080, /* don't reconnect if there is a sessClosedErr from PPCToolbox */
  kAEWantReceipt                = 0x00000200, /* (nReturnReceipt) sender wants a receipt of message */
  kAENeverInteract              = 0x00000010, /* server should not interact with user */
  kAECanInteract                = 0x00000020, /* server may try to interact with user */
  kAEAlwaysInteract             = 0x00000030, /* server should always interact with user where appropriate */
  kAECanSwitchLayer             = 0x00000040, /* interaction may switch layer */
  kAEDontRecord                 = 0x00001000, /* don't record this event - available only in vers 1.0.1 and greater */
  kAEDontExecute                = 0x00002000, /* don't send the event for recording - available only in vers 1.0.1 and greater */
  kAEProcessNonReplyEvents      = 0x00008000, /* allow processing of non-reply events while awaiting synchronous AppleEvent reply */
  kAEDoNotAutomaticallyAddAnnotationsToEvent = 0x00010000, /* if set, don't automatically add any sandbox or other annotations to the event */
};


/* Constants for timeout durations */
enum {
  kAEDefaultTimeout             = -1,   /* timeout value determined by AEM */
  kNoTimeOut                    = -2    /* wait until reply comes back, however long it takes */
};


/**************************************************************************
  These calls are used to set up and modify the coercion dispatch table.
**************************************************************************/
typedef CALLBACK_API( OSErr , AECoerceDescProcPtr )(const AEDesc *fromDesc, DescType toType, SRefCon handlerRefcon, AEDesc *toDesc);
typedef CALLBACK_API( OSErr , AECoercePtrProcPtr )(DescType typeCode, const void *dataPtr, Size dataSize, DescType toType, SRefCon handlerRefcon, AEDesc *result);
typedef STACK_UPP_TYPE(AECoerceDescProcPtr)                     AECoerceDescUPP;
typedef STACK_UPP_TYPE(AECoercePtrProcPtr)                      AECoercePtrUPP;
/*
 *  NewAECoerceDescUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern AECoerceDescUPP
NewAECoerceDescUPP(AECoerceDescProcPtr userRoutine)           API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  NewAECoercePtrUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern AECoercePtrUPP
NewAECoercePtrUPP(AECoercePtrProcPtr userRoutine)             API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  DisposeAECoerceDescUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeAECoerceDescUPP(AECoerceDescUPP userUPP)               API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  DisposeAECoercePtrUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeAECoercePtrUPP(AECoercePtrUPP userUPP)                 API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  InvokeAECoerceDescUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeAECoerceDescUPP(
  const AEDesc *   fromDesc,
  DescType         toType,
  SRefCon          handlerRefcon,
  AEDesc *         toDesc,
  AECoerceDescUPP  userUPP)                                   API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  InvokeAECoercePtrUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeAECoercePtrUPP(
  DescType        typeCode,
  const void *    dataPtr,
  Size            dataSize,
  DescType        toType,
  SRefCon         handlerRefcon,
  AEDesc *        result,
  AECoercePtrUPP  userUPP)                                    API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

#if __MACH__
  #ifdef __cplusplus
    inline AECoerceDescUPP                                      NewAECoerceDescUPP(AECoerceDescProcPtr userRoutine) { return userRoutine; }
    inline AECoercePtrUPP                                       NewAECoercePtrUPP(AECoercePtrProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeAECoerceDescUPP(AECoerceDescUPP) { }
    inline void                                                 DisposeAECoercePtrUPP(AECoercePtrUPP) { }
    inline OSErr                                                InvokeAECoerceDescUPP(const AEDesc * fromDesc, DescType toType, SRefCon handlerRefcon, AEDesc * toDesc, AECoerceDescUPP userUPP) { return (*userUPP)(fromDesc, toType, handlerRefcon, toDesc); }
    inline OSErr                                                InvokeAECoercePtrUPP(DescType typeCode, const void * dataPtr, Size dataSize, DescType toType, SRefCon handlerRefcon, AEDesc * result, AECoercePtrUPP userUPP) { return (*userUPP)(typeCode, dataPtr, dataSize, toType, handlerRefcon, result); }
  #else
    #define NewAECoerceDescUPP(userRoutine)                     ((AECoerceDescUPP)userRoutine)
    #define NewAECoercePtrUPP(userRoutine)                      ((AECoercePtrUPP)userRoutine)
    #define DisposeAECoerceDescUPP(userUPP)
    #define DisposeAECoercePtrUPP(userUPP)
    #define InvokeAECoerceDescUPP(fromDesc, toType, handlerRefcon, toDesc, userUPP) (*userUPP)(fromDesc, toType, handlerRefcon, toDesc)
    #define InvokeAECoercePtrUPP(typeCode, dataPtr, dataSize, toType, handlerRefcon, result, userUPP) (*userUPP)(typeCode, dataPtr, dataSize, toType, handlerRefcon, result)
  #endif
#endif

/* a AECoercionHandlerUPP is by default a AECoerceDescUPP.  If you are registering a 
    Ptr based coercion handler you will have to add a cast to AECoerceDescUPP from 
    your AECoercePtrUPP type.  A future release of the interfaces will fix this by
    introducing seperate Desc and Ptr coercion handler installation/remove/query routines. */
typedef AECoerceDescUPP                 AECoercionHandlerUPP;
/*
 *  AEInstallCoercionHandler()
 *  
 *  Summary:
 *    Installs a coercion handler in either the application or system
 *    coercion handler dispatch table.
 *  
 *  Discussion:
 *    Before using AEInstallCoercionHandler to install a handler for a
 *    particular descriptor type, you can use the AEGetCoercionHandler
 *    function to determine whether the table already contains a
 *    coercion handler for that type. 
 *    Version Notes
 *    See the Version Notes section for the AECoercePtr function for
 *    information on when to use descriptor-based versus pointer-based
 *    coercion handlers starting in Mac OS X version 10.2.
 *    Your application should not install a coercion handler in a
 *    system coercion handler dispatch table with the goal that the
 *    handler will get called when other applications perform coercions
 *    - this won't work in Mac OS X. For more information, see "Writing
 *    and Installing Coercion Handlers" in Apple Events Programming
 *    Guide.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    fromType:
 *      The descriptor type of the data coerced by the handler.
 *    
 *    toType:
 *      The descriptor type of the resulting data.
 *      If there was already an entry in the specified coercion handler
 *      table for the same source descriptor type and result descriptor
 *      type, the existing entry is replaced.
 *    
 *    handler:
 *      A universal procedure pointer to the coercion handler function
 *      to install.
 *    
 *    handlerRefcon:
 *      A reference constant. The Apple Event Manager passes this value
 *      to the handler each time it calls it. If your handler doesn't
 *      require a reference constant, pass 0 for this parameter.
 *    
 *    fromTypeIsDesc:
 *      Specifies the form of the data to coerce. Pass TRUE if the
 *      coercion handler expects the data as a descriptor or FALSE if
 *      the coercion handler expects a pointer to the data. The Apple
 *      Event Manager can provide a pointer to data more efficiently
 *      than it can provide a descriptor, so all coercion functions
 *      should accept a pointer to data if possible.
 *    
 *    isSysHandler:
 *      Specifies the coercion table to add the handler to. Pass TRUE
 *      to add the handler to the system coercion table or FALSE to add
 *      the handler to your application's coercion table. Use of the
 *      system coercion table is not recommended.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEInstallCoercionHandler(
  DescType               fromType,
  DescType               toType,
  AECoercionHandlerUPP   handler,
  SRefCon                handlerRefcon,
  Boolean                fromTypeIsDesc,
  Boolean                isSysHandler)                        API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AERemoveCoercionHandler()
 *  
 *  Discussion:
 *    Removes a coercion handler from a coercion handler dispatch table.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    fromType:
 *      The descriptor type of the data coerced by the handler.
 *    
 *    toType:
 *      The descriptor type of the resulting data.
 *    
 *    handler:
 *      A universal procedure pointer to the coercion handler to
 *      remove. Although the parameters fromType and toType are
 *      sufficient to identify the handler, you can identify the
 *      handler explicitly as a safeguard. If you pass NULL for this
 *      parameter, the Apple Event Manager relies solely on the event
 *      class and event ID to identify the handler.
 *    
 *    isSysHandler:
 *      Specifies the coercion table to remove the handler from. Pass
 *      TRUE to remove the handler from the system coercion table or
 *      FALSE to remove the handler from your application's coercion
 *      table. Use of the system coercion table is not recommended.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AERemoveCoercionHandler(
  DescType               fromType,
  DescType               toType,
  AECoercionHandlerUPP   handler,
  Boolean                isSysHandler)                        API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetCoercionHandler()
 *  
 *  Discussion:
 *    Gets the coercion handler for a specified descriptor type.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    fromType:
 *      The descriptor type of the data coerced by the handler.
 *    
 *    toType:
 *      The descriptor type of the resulting data.
 *    
 *    handler:
 *      A universal procedure pointer. On return, a pointer to the
 *      specified handler, if a coercion table entry exists that
 *      exactly matches the values supplied in the parameters fromType
 *      and toType. See AECoercionHandlerUPP.
 *    
 *    handlerRefcon:
 *      A pointer to a reference constant. On return, the reference
 *      constant from the coercion table entry for the specified
 *      coercion handler. The Apple Event Manager passes this reference
 *      constant to the handler each time it calls the handler. The
 *      reference constant may have a value of 0.
 *    
 *    fromTypeIsDesc:
 *      A pointer to a Boolean value. The AEGetCoercionHandler function
 *      returns a value of TRUE in this parameter if the coercion
 *      handler expects the data as a descriptor or FALSE, if the
 *      coercion handler expects a pointer to the data.
 *    
 *    isSysHandler:
 *      Specifies the coercion table to get the handler from. Pass TRUE
 *      to get the handler from the system coercion table or FALSE to
 *      get the handler from your application's coercion table. Use of
 *      the system coercion table is not recommended.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetCoercionHandler(
  DescType                fromType,
  DescType                toType,
  AECoercionHandlerUPP *  handler,
  SRefCon *               handlerRefcon,
  Boolean *               fromTypeIsDesc,
  Boolean                 isSysHandler)                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/**************************************************************************
  The following calls provide for a coercion interface.
**************************************************************************/
/*
 *  AECoercePtr()
 *  
 *  Discussion:
 *    Coerces data to a desired descriptor type and creates a
 *    descriptor containing the newly coerced data.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    typeCode:
 *      The descriptor type of the source data.
 *    
 *    dataPtr:
 *      A pointer to the data to coerce.
 *    
 *    dataSize:
 *      The length, in bytes, of the data to coerce.
 *    
 *    toType:
 *      The desired descriptor type of the resulting descriptor.
 *    
 *    result:
 *      A pointer to a descriptor. On successful return, a descriptor
 *      containing the coerced data and matching the descriptor type
 *      specified in toType. On error, a null descriptor. If the
 *      function returns successfully, your application should call the
 *      AEDisposeDesc function to dispose of the resulting descriptor
 *      after it has finished using it. See AEDesc.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AECoercePtr(
  DescType      typeCode,
  const void *  dataPtr,
  Size          dataSize,
  DescType      toType,
  AEDesc *      result)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AECoerceDesc()
 *  
 *  Discussion:
 *    Coerces the data in a descriptor to another descriptor type and
 *    creates a descriptor containing the newly coerced data.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDesc:
 *      A pointer to the descriptor containing the data to coerce.
 *    
 *    toType:
 *      The desired descriptor type of the resulting descriptor.
 *    
 *    result:
 *      A pointer to a descriptor. On successful return, a descriptor
 *      containing the coerced data and matching the descriptor type
 *      specified in toType. On error, a null descriptor. If the
 *      function returns successfully, your application should call the
 *      AEDisposeDesc function to dispose of the resulting descriptor
 *      after it has finished using it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AECoerceDesc(
  const AEDesc *  theAEDesc,
  DescType        toType,
  AEDesc *        result)                                     API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/**************************************************************************
 The following calls apply to any AEDesc. Every 'result' descriptor is
 created for you, so you will be responsible for memory management
 (including disposing) of the descriptors so created.  
**************************************************************************/
/* . */
/*
 *  AEInitializeDesc()
 *  
 *  Discussion:
 *    The function sets the type of the descriptor to typeNull and sets
 *    the data handle to NULL. If you need to initialize a descriptor
 *    that already has some data in it, use AEDisposeDesc to deallocate
 *    the memory and initialize the descriptor.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    desc:
 *      A pointer to a new descriptor.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
AEInitializeDesc(AEDesc * desc)                               API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



#ifdef __cplusplus
    inline void AEInitializeDescInline(AEDesc* d) { d->descriptorType = typeNull; d->dataHandle = NULL; }
#else
    #define AEInitializeDescInline(__d) do { AEDesc* d = __d; d->descriptorType = typeNull; d->dataHandle = NULL; } while (0)
#endif

/*
 *  AECreateDesc()
 *  
 *  Summary:
 *    Creates a new descriptor that incorporates the specified data.
 *  
 *  Discussion:
 *    While it is possible to create an Apple event descriptor or a
 *    descriptor list or a descriptor with the AECreateDesc function
 *    (assuming you have access to the raw data for an Apple event,
 *    list, or descriptor), you typically create these structured
 *    objects with their specific creation routines -
 *    AECreateAppleEvent, AECreateList, or AECreateDesc.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    typeCode:
 *      The descriptor type for the new descriptor.
 *    
 *    dataPtr:
 *      A pointer to the data for the new descriptor. This data is
 *      copied into a newly-allocated block of memory for the
 *      descriptor that is created. To minimize copying overhead,
 *      consider using AECreateDescFromExternalPtr.
 *    
 *    dataSize:
 *      The length, in bytes, of the data for the new descriptor.
 *    
 *    result:
 *      A pointer to a descriptor. On successful return, a descriptor
 *      that incorporates the data specified by the dataPtr parameter.
 *      On error, a null descriptor. If the function returns
 *      successfully, your application should call the AEDisposeDesc
 *      function to dispose of the resulting descriptor after it has
 *      finished using it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AECreateDesc(
  DescType      typeCode,
  const void *  dataPtr,
  Size          dataSize,
  AEDesc *      result)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEDisposeDesc()
 *  
 *  Summary:
 *    Deallocates the memory used by a descriptor.
 *  
 *  Discussion:
 *    The AEDisposeDesc function deallocates the memory used by a
 *    descriptor. After calling this method, the descriptor becomes an
 *    empty descriptor with a type of typeNULL. Because all Apple event
 *    structures (except for keyword-specified descriptors) are
 *    descriptors, you can use AEDisposeDesc for any of them.
 *    Do not call AEDisposeDesc on a descriptor obtained from another
 *    Apple Event Manager function (such as the reply event from a call
 *    to AESend) unless that function returns successfully.
 *    Special Considerations
 *    If the AEDesc might contain an OSL token, dispose of it with
 *    AEDisposeToken.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDesc:
 *      A pointer to the descriptor to deallocate. On return, a null
 *      descriptor. If you pass a null descriptor in this parameter,
 *      AEDisposeDesc returns noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEDisposeDesc(AEDesc * theAEDesc)                             API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEDuplicateDesc()
 *  
 *  Summary:
 *    Creates a copy of a descriptor.
 *  
 *  Discussion:
 *    It is common for applications to send Apple events that have one
 *    or more attributes or parameters in common. For example, if you
 *    send a series of Apple events to the same application, the
 *    address attribute is the same. In these cases, the most efficient
 *    way to create the necessary Apple events is to make a template
 *    Apple event that you can then copy - by calling the
 *    AEDuplicateDesc function - as needed. You then fill in or change
 *    the remaining parameters and attributes of the copy, send the
 *    copy by calling the AESend function and, after AESend returns a
 *    result code, dispose of the copy by calling AEDisposeDesc. You
 *    can use this approach to prepare structures of type AEDesc,
 *    AEDescList, AERecord, and AppleEvent.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDesc:
 *      A pointer to the descriptor to duplicate. See AEDesc.
 *    
 *    result:
 *      A pointer to a descriptor. On return, the descriptor contains a
 *      copy of the descriptor specified by the theAEDesc parameter. If
 *      the function returns successfully, your application should call
 *      the AEDisposeDesc function to dispose of the resulting
 *      descriptor after it has finished using it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEDuplicateDesc(
  const AEDesc *  theAEDesc,
  AEDesc *        result)                                     API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



typedef CALLBACK_API( void , AEDisposeExternalProcPtr )(const void *dataPtr, Size dataLength, SRefCon refcon);
typedef STACK_UPP_TYPE(AEDisposeExternalProcPtr)                AEDisposeExternalUPP;
/*
 *  AECreateDescFromExternalPtr()
 *  
 *  Summary:
 *    Creates a new descriptor that uses a memory buffer supplied by
 *    the caller.
 *  
 *  Discussion:
 *    This function is different than AECreateDesc, in that it creates
 *    a descriptor that uses the data block provided by the caller "in
 *    place," rather than allocate a block of memory and copy the data
 *    to it. This function can provide dramatically improved
 *    performance if you're working with large chunks of data. It
 *    attempts to copy the descriptor to the address space of any
 *    recipient process using virtual memory APIs, avoiding an actual
 *    memory copy. For example, you might want to use this function to
 *    pass a large image in an Apple event.
 *    You can use the AEGetDescDataRange function to access a specific
 *    section of a large block of data.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    descriptorType:
 *      The descriptor type for the new descriptor.
 *    
 *    dataPtr:
 *      A pointer to the data for the new descriptor. The memory that
 *      is pointed to cannot be a Handle (which may move in memory),
 *      cannot be modified by the caller, and must be preserved in
 *      place (and not freed), until the disposeCallback function is
 *      called.
 *      If possible, the descriptor will be mapped into the address
 *      space of the recipient using shared memory, avoiding an actual
 *      memory copy.
 *      The pointer that is passed in does not need to be aligned to
 *      any particular boundary, but is optimized to transfer data on a
 *      page boundary. You can get the current page size (4096 on all
 *      current Mac OS X systems) with the getpagesize(3) call. (Type
 *      man 3 getpagesize in a Terminal window for documentation.)
 *    
 *    dataLength:
 *      The length, in bytes, of the data for the new descriptor.
 *    
 *    disposeCallback:
 *      A universal procedure pointer to a dispose callback function of
 *      type AEDisposeExternalProcPtr. Your callback function will be
 *      called when the block of memory provided by dataPtr is no
 *      longer needed by the Apple Event Manager. The function can be
 *      called at any time, including during creation of the descriptor.
 *    
 *    disposeRefcon:
 *      A reference constant the Apple Event Manager passes to the
 *      disposeCallback function whenever it calls the function. If
 *      your dispose function doesn't require a reference constant,
 *      pass 0 for this parameter.
 *    
 *    theDesc:
 *      A pointer to a descriptor. On successful return, a descriptor
 *      that incorporates the data specified by the dataPtr parameter.
 *      On error, a null descriptor. If the function returns
 *      successfully, your application should call the AEDisposeDesc
 *      function to dispose of the resulting descriptor after it has
 *      finished using it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AECreateDescFromExternalPtr(
  OSType                 descriptorType,
  const void *           dataPtr,
  Size                   dataLength,
  AEDisposeExternalUPP   disposeCallback,
  SRefCon                disposeRefcon,
  AEDesc *               theDesc)                             API_AVAILABLE( macos(10.2) ) API_UNAVAILABLE( ios, tvos, watchos );

	

	
#ifndef AVAILABLE_MAC_OS_X_VERSION_10_8_AND_LATER
#define	AVAILABLE_MAC_OS_X_VERSION_10_8_AND_LATER
#endif
	
	
	/*
	 *  AECompareDesc()
	 *  
	 *  Discussion:
	 *    Compare two AEDesc descriptors and return whether they are
	 *	  identical or not.
	 *  
	 *  Mac OS X threading:
	 *    Thread safe since version 10.8
	 *  
	 *  Parameters:
	 *    
	 *    desc1, desc2:
	 *      A pointer to an AEDesc to be compared.
	 *
	 *	  resultP:
	 *		If non-NULL, on a noErr return will be filled in with
	 *		true or false indicating whether the descriptors are
	 *		equilavent or not.
	 *
	 *  Two descriptors are identical if they are the same type and have
	 *  the same data; typeAEList descriptors must contain the same number
	 *  of items and every item in each list must itself be identical;
	 *  typeAERecord descriptors must contain the same number of keys
	 *  and values and each key/value must match between the two.
	 *  typeAppleEvents match like typeAERecord and also require that
	 *	most attributes of the two events are identical.
	 *  
	 *  Availability:
	 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
	 *    CarbonLib:        in CarbonLib 1.0 and later
	 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
	 */
	extern OSStatus
	AECompareDesc( const AEDesc * desc1, const AEDesc* desc2, Boolean* resultP ) API_AVAILABLE( macos(10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

/**************************************************************************
  The following calls apply to AEDescList. Since AEDescList is a subtype of
  AEDesc, the calls in the previous section can also be used for AEDescList.
  All list and array indices are 1-based. If the data was greater than
  maximumSize in the routines below, then actualSize will be greater than
  maximumSize, but only maximumSize bytes will actually be retrieved.
**************************************************************************/
/*
 *  AECreateList()
 *  
 *  Discussion:
 *    Creates an empty descriptor list or Apple event record.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    factoringPtr:
 *      A pointer to the data at the beginning of each descriptor that
 *      is the same for all descriptors in the list. If there is no
 *      common data, or if you decide not to isolate the common data,
 *      pass NULL as the value of this parameter.
 *    
 *    factoredSize:
 *      The size of the common data. If there is no common data, or if
 *      you decide not to isolate the common data, pass 0 as the value
 *      of this parameter. (See the Discussion section for more
 *      information.)
 *    
 *    isRecord:
 *      A Boolean value that specifies the kind of list to create. Pass
 *      a value of TRUE to create an Apple event record (a data
 *      structure of type AERecord) or FALSE to create a descriptor
 *      list.
 *    
 *    resultList:
 *      A pointer to a descriptor list variable. On successful return,
 *      the descriptor list or Apple event record that the AECreateList
 *      function creates. On error, a null descriptor. See AEDescList.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AECreateList(
  const void *  factoringPtr,
  Size          factoredSize,
  Boolean       isRecord,
  AEDescList *  resultList)                                   API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AECountItems()
 *  
 *  Discussion:
 *    Counts the number of descriptors in a descriptor list.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to count
 *    
 *    theCount:
 *      A pointer to a count variable. On return, the number of
 *      descriptors in the specified descriptor list.  Currently an
 *      AEDescList is limited to 2^31 items.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AECountItems(
  const AEDescList *  theAEDescList,
  long *              theCount)                               API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEPutPtr()
 *  
 *  Discussion:
 *    Puts data specified in a buffer to a descriptor list as a
 *    descriptor, possibly replacing an existing descriptor in the list.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to add a descriptor to. See
 *      AEDescList.
 *    
 *    index:
 *      A one-based positive integer indicating the position to insert
 *      the descriptor at. If there is already a descriptor in the
 *      specified position, it is replaced.  You can pass a value of
 *      zero or count + 1 to add the descriptor at the end of the list.
 *      AEPutPtr returns an error (AEIllegalIndex) if you pass a
 *      negative number or a value that is out of range. Currently the
 *      upper limit on index is 2^31 items.
 *    
 *    typeCode:
 *      The descriptor type for the descriptor to be put into the list.
 *    
 *    dataPtr:
 *      A pointer to the data for the descriptor to add.
 *    
 *    dataSize:
 *      The length, in bytes, of the data for the descriptor to add.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEPutPtr(
  AEDescList *  theAEDescList,
  long          index,
  DescType      typeCode,
  const void *  dataPtr,
  Size          dataSize)                                     API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEPutDesc()
 *  
 *  Discussion:
 *    Adds a descriptor to any descriptor list, possibly replacing an
 *    existing descriptor in the list.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to add a descriptor to. See
 *      AEDescList.
 *    
 *    index:
 *      A one-based positive integer indicating the position to insert
 *      the descriptor at. If there is already a descriptor in the
 *      specified position, it is replaced.  You can pass a value of
 *      zero or count + 1 to add the descriptor at the end of the list.
 *      AEPutPtr returns an error (AEIllegalIndex) if you pass a
 *      negative number or a value that is out of range. Currently the
 *      upper limit on index is 2^31 items.
 *    
 *    theAEDesc:
 *      A pointer to the descriptor to add to the list.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEPutDesc(
  AEDescList *    theAEDescList,
  long            index,
  const AEDesc *  theAEDesc)                                  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetNthPtr()
 *  
 *  Discussion:
 *    Gets a copy of the data from a descriptor at a specified position
 *    in a descriptor list; typically used when your application needs
 *    to work with the extracted data directly.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to add a descriptor to. See
 *      AEDescList.
 *    
 *    index:
 *      A one-based positive integer indicating the position in the
 *      descriptor list of the descriptor to get the data from.
 *      AEGetNthPtr returns an error if you pass zero, a negative
 *      number, or a value that is out of range. Currently the upper
 *      limit on index is 2^31 items.
 *    
 *    desiredType:
 *      The desired descriptor type for the copied data. For a list of
 *      AppleScript's predefined descriptor types. If the descriptor
 *      specified by the index parameter is not of the desired type,
 *      AEGetNthPtr attempts to coerce the data to this type. If you
 *      pass a value of typeWildCard, no coercion is performed, and the
 *      descriptor type of the copied data is the same as the
 *      descriptor type of the original descriptor.
 *    
 *    theAEKeyword:
 *      A pointer to a keyword or NULL. On return, the keyword for the
 *      specified descriptor, if you are getting data from a list of
 *      keyword-specified descriptors; otherwise, AEGetNthPtr returns
 *      the value typeWildCard.
 *    
 *    typeCode:
 *      A pointer to a descriptor type or NULL. On return, specifies
 *      the descriptor type of the data pointed to by dataPtr.
 *    
 *    dataPtr:
 *      A pointer to a buffer, local variable, or other storage
 *      location created and disposed of by your application. The size
 *      in bytes must be at least as large as the value you pass in the
 *      maximumSize parameter. On return, contains the data from the
 *      descriptor at the position in the descriptor list specified by
 *      the index parameter.
 *    
 *    maximumSize:
 *      The maximum length, in bytes, of the expected data. The
 *      AEGetNthPtr function will not return more data than you specify
 *      in this parameter.
 *    
 *    actualSize:
 *      A pointer to a size variable or NULL. On return, the length, in
 *      bytes, of the data for the specified descriptor. If this value
 *      is larger than the value of the maximumSize parameter, the
 *      buffer pointed to by dataPtr was not large enough to contain
 *      all of the data for the descriptor, though AEGetNthPtr does not
 *      write beyond the end of the buffer. If the buffer was too
 *      small, you can resize it and call AEGetNthPtr again.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetNthPtr(
  const AEDescList *  theAEDescList,
  long                index,
  DescType            desiredType,
  AEKeyword *         theAEKeyword,        /* can be NULL */
  DescType *          typeCode,            /* can be NULL */
  void *              dataPtr,
  Size                maximumSize,
  Size *              actualSize)          /* can be NULL */  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetNthDesc()
 *  
 *  Discussion:
 *    Copies a descriptor from a specified position in a descriptor
 *    list into a specified descriptor; typically used when your
 *    application needs to pass the extracted data to another function
 *    as a descriptor.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to add a descriptor to. See
 *      AEDescList.
 *    
 *    index:
 *      A one-based positive integer indicating the position in the
 *      descriptor list of the descriptor to get the data from.
 *      AEGetNthDesc returns an error if you pass zero, a negative
 *      number, or a value that is out of range. Currently the upper
 *      limit on index is 2^31 items.
 *    
 *    desiredType:
 *      The desired descriptor type for the copied data. For a list of
 *      AppleScript's predefined descriptor types.  If the descriptor
 *      specified by the index parameter is not of the desired type,
 *      AEGetNthDesc attempts to coerce the data to this type. If you
 *      pass a value of typeWildCard, no coercion is performed, and the
 *      descriptor type of the copied data is the same as the
 *      descriptor type of the original descriptor.
 *    
 *    theAEKeyword:
 *      A pointer to a keyword or NULL. On return, the keyword for the
 *      specified descriptor, if you are getting data from a list of
 *      keyword-specified descriptors; otherwise, AEGetNthDesc returns
 *      the value typeWildCard.
 *    
 *    result:
 *      A pointer to a descriptor. On successful return, a copy of the
 *      descriptor specified by the index parameter, coerced, if
 *      necessary, to the descriptor type specified by the desiredType
 *      parameter. On error, a null descriptor. If the function returns
 *      successfully, your application should call the AEDisposeDesc
 *      function to dispose of the resulting descriptor after it has
 *      finished using it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetNthDesc(
  const AEDescList *  theAEDescList,
  long                index,
  DescType            desiredType,
  AEKeyword *         theAEKeyword,        /* can be NULL */
  AEDesc *            result)                                 API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AESizeOfNthItem()
 *  
 *  Discussion:
 *    Gets the data size and descriptor type of the descriptor at a
 *    specified position in a descriptor list.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to add a descriptor to. See
 *      AEDescList.
 *    
 *    index:
 *      A one-based positive integer indicating the position in the
 *      descriptor list of the descriptor to get the data from.
 *      AESizeOfNthItem returns an error if you pass zero, a negative
 *      number, or a value that is out of range. Currently the upper
 *      limit on index is 2^31 items.
 *    
 *    typeCode:
 *      A pointer to a descriptor type or NULL. On return, specifies
 *      the descriptor type of the descriptor.
 *    
 *    dataSize:
 *      A pointer to a size variable or NULL. On return, the length (in
 *      bytes) of the data in the descriptor.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AESizeOfNthItem(
  const AEDescList *  theAEDescList,
  long                index,
  DescType *          typeCode,            /* can be NULL */
  Size *              dataSize)            /* can be NULL */  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetArray()
 *  
 *  Discussion:
 *    Extracts data from an Apple event array created with the
 *    AEPutArray function and stores it as a standard array of fixed
 *    size items in the specified buffer.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to add a descriptor to. See
 *      AEDescList.
 *    
 *    arrayType:
 *      The Apple event array type to convert. Pass one of the
 *      constants:  kAEDataArray, kAEPackedArray, kAEDescArray,
 *      kAEKeyDescArray
 *    
 *    arrayPtr:
 *      A pointer to a buffer, allocated and disposed of by your
 *      application, for storing the array. The size in bytes must be
 *      at least as large as the value you pass in the maximumSize
 *      parameter. On return, the buffer contains the array of
 *      fixed-size items.
 *    
 *    maximumSize:
 *      The maximum length, in bytes, of the expected data. The
 *      AEGetArray function will not return more data than you specify
 *      in this parameter.
 *    
 *    itemType:
 *      A pointer to a descriptor type. On return, for arrays of type
 *      kAEDataArray, kAEPackedArray, or kAEHandleArray, the descriptor
 *      type of the items in the returned array. The AEGetArray
 *      function doesn't supply a value in itemType for arrays of type
 *      kAEDescArray and kAEKeyDescArray because they may contain
 *      descriptors of different types.
 *    
 *    itemSize:
 *      A pointer to a size variable. On return, for arrays of type
 *      kAEDataArray or kAEPackedArray, the size (in bytes) of each
 *      item in the returned array. You don't get an item size for
 *      arrays of type kAEDescArray, kAEKeyDescArray, or kAEHandleArray
 *      because descriptors and handles (though not the data they point
 *      to) have a known size.
 *    
 *    itemCount:
 *      A pointer to a size variable. On return, the number of items in
 *      the returned array.  Currently the upper limit on the size of
 *      an array is 2^31 items.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetArray(
  const AEDescList *   theAEDescList,
  AEArrayType          arrayType,
  AEArrayDataPointer   arrayPtr,
  Size                 maximumSize,
  DescType *           itemType,
  Size *               itemSize,
  long *               itemCount)                             API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEPutArray()
 *  
 *  Discussion:
 *    Extracts data from an Apple event array created with the
 *    AEPutArray function and stores it as a standard array of fixed
 *    size items in the specified buffer.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to add a descriptor to. See
 *      AEDescList.
 *    
 *    arrayType:
 *      The Apple event array type to convert. Pass one of the
 *      constants:  kAEDataArray, kAEPackedArray, kAEDescArray,
 *      kAEKeyDescArray
 *    
 *    arrayPtr:
 *      A pointer to a buffer, local variable, or other storage
 *      location, created and disposed of by your application, that
 *      contains the array to put into the descriptor list.
 *    
 *    itemType:
 *      For arrays of type kAEDataArray, kAEPackedArray, or
 *      kAEHandleArray, the descriptor type of the array items to
 *      create. Use one of the constants such as typeLongInteger. You
 *      don't need to specify an item type for arrays of type
 *      kAEDescArray or kAEKeyDescArray because the data is already
 *      stored in descriptors which contain a descriptor type.
 *    
 *    itemSize:
 *      For arrays of type kAEDataArray or kAEPackedArray, the size (in
 *      bytes) of the array items to create. You don't need to specify
 *      an item size for arrays of type kAEDescArray, kAEKeyDescArray,
 *      or kAEHandleArray because their descriptors (though not the
 *      data they point to) have a known size.
 *    
 *    itemCount:
 *      A pointer to a size variable. On return, the number of items in
 *      the returned array.  Currently the upper limit on the size of
 *      an array is 2^31 items.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEPutArray(
  AEDescList *         theAEDescList,
  AEArrayType          arrayType,
  const AEArrayData *  arrayPtr,
  DescType             itemType,
  Size                 itemSize,
  long                 itemCount)                             API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEDeleteItem()
 *  
 *  Discussion:
 *    Deletes a descriptor from a descriptor list, causing all
 *    subsequent descriptors to move up one place.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDescList:
 *      A pointer to the descriptor list to add a descriptor to. See
 *      AEDescList.
 *    
 *    index:
 *      A one-based positive integer indicating the position in the
 *      descriptor list of the descriptor to delete. AEDeleteItem
 *      returns an error if you pass zero, a negative number, or a
 *      value that is out of range. Currently the upper limit on index
 *      is 2^31 items.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEDeleteItem(
  AEDescList *  theAEDescList,
  long          index)                                        API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/**************************************************************************
 The following calls apply to AERecord. Since AERecord is a subtype of
 AEDescList, the calls in the previous sections can also be used for
 AERecord an AERecord can be created by using AECreateList with isRecord
 set to true. 
**************************************************************************/
/*************************************************************************
 AERecords can have an abitrary descriptorType.  This allows you to
 check if desc is truly an AERecord
************************************************************************/
/*
 *  AECheckIsRecord()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
AECheckIsRecord(const AEDesc * theDesc)                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
  Note: The following #defines map "key" calls on AERecords into "param" calls on 
  AppleEvents.  Although no errors are currently returned if AERecords are 
  passed to "param" calls and AppleEvents to "key" calls, the behavior of 
  this type of API-mixing is not explicitly documented in Inside Macintosh.  
  It just happens that the "key" calls have the same functionality as their 
  "param" counterparts.  Since none of the "key" calls are currently available 
  in the PowerPC IntefaceLib, the #defines exploit the fact that "key" and 
  "param" routines can be used interchangeably, and makes sure that every 
  invocation of a "key" API becomes an invocation of a "param" API.
*/
#define AEPutKeyPtr(theAERecord, theAEKeyword, typeCode, dataPtr, dataSize) \
    AEPutParamPtr((theAERecord), (theAEKeyword), (typeCode), (dataPtr), (dataSize))
#define AEPutKeyDesc(theAERecord, theAEKeyword, theAEDesc) \
    AEPutParamDesc((theAERecord), (theAEKeyword), (theAEDesc))
#define AEGetKeyPtr(theAERecord, theAEKeyword, desiredType, typeCode, dataPtr, maxSize, actualSize) \
    AEGetParamPtr((theAERecord), (theAEKeyword), (desiredType), (typeCode), (dataPtr), (maxSize), (actualSize))
#define AEGetKeyDesc(theAERecord, theAEKeyword, desiredType, result) \
    AEGetParamDesc((theAERecord), (theAEKeyword), (desiredType), (result))
#define AESizeOfKeyDesc(theAERecord, theAEKeyword, typeCode, dataSize) \
    AESizeOfParam((theAERecord), (theAEKeyword), (typeCode), (dataSize))
#define AEDeleteKeyDesc(theAERecord, theAEKeyword) \
    AEDeleteParam((theAERecord), (theAEKeyword))
/**************************************************************************
  The following calls create and manipulate the AppleEvent data type.
**************************************************************************/
/*
 *  AECreateAppleEvent()
 *  
 *  Summary:
 *    Creates an Apple event with several important attributes but no
 *    parameters.
 *  
 *  Discussion:
 *    The AECreateAppleEvent function creates an empty Apple event. You
 *    can add parameters to the Apple event after you create it with
 *    the functions described in "Adding Parameters and Attributes to
 *    an Apple Event".
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEEventClass:
 *      The event class of the Apple event to create. This parameter
 *      becomes accessible through the keyEventClassAttr attribute of
 *      the Apple event. Some event classes are described in "Event
 *      Class Constants".
 *    
 *    theAEEventID:
 *      The event ID of the Apple event to create. This parameter
 *      becomes accessible through the keyEventIDAttr attribute of the
 *      Apple event.
 *    
 *    target:
 *      A pointer to an address descriptor. Before calling
 *      AECreateAppleEvent, you set the descriptor to identify the
 *      target (or server) application for the Apple event. This
 *      parameter becomes accessible through the keyAddressAttr
 *      attribute of the Apple event.
 *    
 *    returnID:
 *      The return ID for the created Apple event. If you pass a value
 *      of kAutoGenerateReturnID, the Apple Event Manager assigns the
 *      created Apple event a return ID that is unique to the current
 *      session. If you pass any other value, the Apple Event Manager
 *      assigns that value for the ID. This parameter becomes
 *      accessible through the keyReturnIDAttr attribute of the Apple
 *      event. The return ID constant is described in "ID Constants for
 *      the AECreateAppleEvent Function".
 *    
 *    transactionID:
 *      The transaction ID for this Apple event. A transaction is a
 *      sequence of Apple events that are sent back and forth between
 *      the client and server applications, beginning with the client's
 *      initial request for a service. All Apple events that are part
 *      of a transaction must have the same transaction ID. You can
 *      specify the kAnyTransactionID constant if the Apple event is
 *      not one of a series of interdependent Apple events. This
 *      parameter becomes accessible through the keyTransactionIDAttr
 *      attribute of the Apple event.
 *    
 *    result:
 *      A pointer to an Apple event. On successful return, the new
 *      Apple event. On error, a null descriptor (one with descriptor
 *      type typeNull). If the function returns successfully, your
 *      application should call the AEDisposeDesc function to dispose
 *      of the resulting Apple event after it has finished using it.
 *      See the AppleEvent data type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AECreateAppleEvent(
  AEEventClass           theAEEventClass,
  AEEventID              theAEEventID,
  const AEAddressDesc *  target,                /* can be NULL */
  AEReturnID             returnID,
  AETransactionID        transactionID,
  AppleEvent *           result)                              API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/**************************************************************************
  The following calls are used to pack and unpack parameters from records
  of type AppleEvent. Since AppleEvent is a subtype of AERecord, the calls
  in the previous sections can also be used for variables of type
  AppleEvent. The next six calls are in fact identical to the six calls
  for AERecord.
**************************************************************************/
/*
 *  AEPutParamPtr()
 *  
 *  Discussion:
 *    Puts a pointer to data, a descriptor type, and a keyword into an
 *    Apple event or Apple event record as an Apple event parameter.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to add a parameter to. See the
 *      AppleEvent data type.
 *    
 *    theAEKeyword:
 *      The keyword for the parameter to add. If the Apple event
 *      already includes an parameter with this keyword, the parameter
 *      is replaced.
 *    
 *    typeCode:
 *      The descriptor type for the parameter to add.
 *    
 *    dataPtr:
 *      A pointer to the data for the parameter to add.
 *    
 *    dataSize:
 *      The length, in bytes, of the data for the parameter to add.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEPutParamPtr(
  AppleEvent *  theAppleEvent,
  AEKeyword     theAEKeyword,
  DescType      typeCode,
  const void *  dataPtr,
  Size          dataSize)                                     API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEPutParamDesc()
 *  
 *  Discussion:
 *    Puts a descriptor and a keyword into an Apple event or Apple
 *    event record as an Apple event parameter.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to add a parameter to.
 *    
 *    theAEKeyword:
 *      The keyword specifying the parameter to add. If the Apple event
 *      already has a parameter with this keyword, the parameter is
 *      replaced.
 *    
 *    theAEDesc:
 *      A pointer to the descriptor for the parameter to add. See
 *      AEDesc.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEPutParamDesc(
  AppleEvent *    theAppleEvent,
  AEKeyword       theAEKeyword,
  const AEDesc *  theAEDesc)                                  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetParamPtr()
 *  
 *  Summary:
 *    Gets a copy of the data for a specified Apple event parameter
 *    from an Apple event or an Apple event record (type AERecord);
 *    typically used when your application needs to work with the
 *    extracted data directly.
 *  
 *  Discussion:
 *    You should use this function only to extract data from value
 *    descriptors such as typeUTF8Text.
 *    Because this function allows you to specify a desired type, it
 *    can result in coercion. When used correctly, this has the
 *    positive effect of returning the data in the desired format.
 *    However, it can have side effects you may not be expecting, such
 *    as the overhead of calls to coercion handlers. See also the
 *    Version Notes section below for possible problems with
 *    coercion.
 *    To get Apple event parameter data for your application to use
 *    directly, call AEGetParamPtr. To get a descriptor for an Apple
 *    event parameter to pass on to another Apple Event Manager
 *    routine, call AEGetParamDesc.
 *    Before calling AEGetParamPtr, you can call the AESizeOfParam
 *    function to determine a size for the dataPtr buffer. However,
 *    unless you specify typeWildCard for the desiredType parameter,
 *    AEGetParamPtr may coerce the data, which may cause the size of
 *    the data to change.
 *    In some cases, you may get improved efficiency extracting
 *    information from an Apple event with the AEGetDescDataRange
 *    function.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to get the parameter data from.
 *      See AppleEvent.
 *    
 *    theAEKeyword:
 *      The keyword that specifies the desired Apple event parameter.
 *    
 *    desiredType:
 *      The desired descriptor type for the copied data. If the
 *      descriptor specified by the theAEKeyword parameter is not of
 *      the desired type, AEGetParamPtr attempts to coerce the data to
 *      this type. However, if the desired type is typeWildCard, no
 *      coercion is performed. On return, you can determine the actual
 *      descriptor type by examining the typeCode parameter.
 *    
 *    actualType:
 *      A pointer to a descriptor type. On return, specifies the
 *      descriptor type of the data pointed to by dataPtr. The returned
 *      type is either the same as the type specified by the
 *      desiredType parameter or, if the desired type was typeWildcard,
 *      the true type of the descriptor. Specify NULL if you do not
 *      care about this return value.
 *    
 *    dataPtr:
 *      A pointer to a buffer, local variable, or other storage
 *      location created and disposed of by your application. The size
 *      in bytes must be at least as large as the value you pass in the
 *      maximumSize parameter. On return, contains the parameter data.
 *      Specify NULL if you do not care about this return value.
 *    
 *    maximumSize:
 *      The maximum length, in bytes, of the expected Apple event
 *      parameter data. The AEGetParamPtr function will not return more
 *      data than you specify in this parameter.
 *    
 *    actualSize:
 *      A pointer to a variable of type Size. On return, the length, in
 *      bytes, of the data for the specified Apple event parameter. If
 *      this value is larger than the value you passed in the
 *      maximumSize parameter, the buffer pointed to by dataPtr was not
 *      large enough to contain all of the data for the parameter,
 *      though AEGetParamPtr does not write beyond the end of the
 *      buffer. If the buffer was too small, you can resize it and call
 *      AEGetParamPtr again. Specify NULL if you do not care about this
 *      return value.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetParamPtr(
  const AppleEvent *  theAppleEvent,
  AEKeyword           theAEKeyword,
  DescType            desiredType,
  DescType *          actualType,          /* can be NULL */
  void *              dataPtr,
  Size                maximumSize,
  Size *              actualSize)          /* can be NULL */  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetParamDesc()
 *  
 *  Summary:
 *    Gets a copy of the descriptor for a keyword-specified Apple event
 *    parameter from an Apple event or an Apple event record (type
 *    AERecord); typically used when your application needs to pass the
 *    extracted data to another function as a descriptor.
 *  
 *  Discussion:
 *    To get Apple event parameter data for your application to use
 *    directly, call AEGetParamPtr. To get a descriptor for an Apple
 *    event parameter to pass on to another Apple Event Manager
 *    routine, call AEGetParamDesc.
 *    If the actual parameter you are getting with AEGetParamDesc is a
 *    record, AEGetParamDesc will only allow you to request it as a
 *    typeAERecord, typeAEList, or typeWildcard. For any other type, it
 *    will return errAECoercionFail.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to get the parameter descriptor
 *      from.
 *    
 *    theAEKeyword:
 *      A keyword that specifies the desired Apple event parameter.
 *    
 *    desiredType:
 *      The descriptor type for the desired Apple event parameter. If
 *      the requested Apple event parameter is not of the desired type,
 *      the Apple Event Manager attempts to coerce it to the desired
 *      type. However, if you pass a value of typeWildCard, no coercion
 *      is performed, and the descriptor type of the returned
 *      descriptor is the same as the descriptor type of the Apple
 *      event parameter.
 *    
 *    result:
 *      A pointer to a descriptor. On successful return, a copy of the
 *      descriptor for the specified Apple event parameter, coerced, if
 *      necessary, to the descriptor type specified by the desiredType
 *      parameter. On error, a null descriptor. If the function returns
 *      successfully, your application should call the AEDisposeDesc
 *      function to dispose of the resulting descriptor after it has
 *      finished using it. See AEDesc.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetParamDesc(
  const AppleEvent *  theAppleEvent,
  AEKeyword           theAEKeyword,
  DescType            desiredType,
  AEDesc *            result)                                 API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AESizeOfParam()
 *  
 *  Discussion:
 *    Gets the size and descriptor type of an Apple event parameter
 *    from a descriptor of type AERecord or AppleEvent.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to get the parameter data from.
 *      See AppleEvent.
 *    
 *    theAEKeyword:
 *      The keyword that specifies the desired parameter.
 *    
 *    typeCode:
 *      A pointer to a descriptor type. On return, specifies the
 *      descriptor type of the Apple event parameter.
 *    
 *    dataSize:
 *      A pointer to a size variable. On return, the length, in bytes,
 *      of the data in the Apple event parameter.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AESizeOfParam(
  const AppleEvent *  theAppleEvent,
  AEKeyword           theAEKeyword,
  DescType *          typeCode,            /* can be NULL */
  Size *              dataSize)            /* can be NULL */  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEDeleteParam()
 *  
 *  Discussion:
 *    Deletes a keyword-specified parameter from an Apple event or
 *    Apple event record.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event or Apple event record to delete
 *      the parameter from.
 *    
 *    theAEKeyword:
 *      The keyword that specifies the parameter to delete.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEDeleteParam(
  AppleEvent *  theAppleEvent,
  AEKeyword     theAEKeyword)                                 API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );




/**************************************************************************
 The following calls also apply to type AppleEvent. Message attributes are
 far more restricted, and can only be accessed through the following 5
 calls. The various list and record routines cannot be used to access the
 attributes of an event. 
**************************************************************************/

/*
 *  AEGetAttributePtr()
 *  
 *  Summary:
 *    Gets a copy of the data for a specified Apple event attribute
 *    from an Apple event; typically used when your application needs
 *    to work with the data directly.
 *  
 *  Discussion:
 *    To get Apple event attribute data for your application to use
 *    directly, call AEGetAttributePtr. To get a descriptor for an
 *    Apple event attribute to pass on to another Apple Event Manager
 *    routine, call AEGetAttributeDesc.
 *    Before calling AEGetAttributePtr, you can call the
 *    AESizeOfAttribute function to determine a size for the dataPtr
 *    buffer. However, unless you specify typeWildCard for the
 *    desiredType parameter, AEGetAttributePtr may coerce the data,
 *    which may cause the size of the data to change.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to get the attribute data from.
 *    
 *    theAEKeyword:
 *      The keyword that specifies the desired attribute.
 *    
 *    desiredType:
 *      The desired descriptor type for the copied data. If the
 *      descriptor specified by the theAEKeyword parameter is not of
 *      the desired type, AEGetAttributePtr attempts to coerce the data
 *      to this type. However, if you pass a value of typeWildCard, no
 *      coercion is performed, and the descriptor type of the returned
 *      data is the same as the descriptor type of the Apple event
 *      attribute. On return, you can determine the actual descriptor
 *      type by examining the typeCode parameter.
 *    
 *    typeCode:
 *      A pointer to a descriptor type. On return, specifies the
 *      descriptor type of the attribute data pointed to by dataPtr.
 *      The returned type is either the same as the type specified by
 *      the desiredType parameter or, if the desired type was type
 *      wildcard, the true type of the descriptor. For a list of
 *      AppleScript's predefined descriptor types, see "Descriptor Type
 *      Constants". See DescType.
 *    
 *    dataPtr:
 *      A pointer to a buffer, local variable, or other storage
 *      location, created and disposed of by your application. The size
 *      in bytes must be at least as large as the value you pass in the
 *      maximumSize parameter. On return, contains the attribute data.
 *    
 *    maximumSize:
 *      The maximum length, in bytes, of the expected attribute data.
 *      The AEGetAttributePtr function will not return more data than
 *      you specify in this parameter.
 *    
 *    actualSize:
 *      A pointer to a size variable. On return, the length, in bytes,
 *      of the data for the specified Apple event attribute. If this
 *      value is larger than the value you passed in the maximumSize
 *      parameter, the buffer pointed to by dataPtr was not large
 *      enough to contain all of the data for the attribute, though
 *      AEGetAttributePtr does not write beyond the end of the buffer.
 *      If the buffer was too small, you can resize it and call
 *      AEGetAttributePtr again.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetAttributePtr(
  const AppleEvent *  theAppleEvent,
  AEKeyword           theAEKeyword,
  DescType            desiredType,
  DescType *          typeCode,            /* can be NULL */
  void *              dataPtr,
  Size                maximumSize,
  Size *              actualSize)          /* can be NULL */  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetAttributeDesc()
 *  
 *  Summary:
 *    Gets a copy of the descriptor for a specified Apple event
 *    attribute from an Apple event; typically used when your
 *    application needs to pass the descriptor on to another function.
 *  
 *  Discussion:
 *    To get Apple event attribute data for your application to use
 *    directly, call AEGetAttributePtr. To get a descriptor for an
 *    Apple event attribute to pass on to another Apple Event Manager
 *    routine, call AEGetAttributeDesc.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to get the attribute descriptor
 *      from. See AppleEvent.
 *    
 *    theAEKeyword:
 *      The keyword that specifies the desired attribute.
 *    
 *    desiredType:
 *      The desired descriptor type for the copied data. If the
 *      descriptor specified by the theAEKeyword parameter is not of
 *      the desired type, AEGetAttributePtr attempts to coerce the data
 *      to this type. However, if you pass a value of typeWildCard, no
 *      coercion is performed, and the descriptor type of the returned
 *      data is the same as the descriptor type of the Apple event
 *      attribute. On return, you can determine the actual descriptor
 *      type by examining the typeCode parameter.
 *    
 *    result:
 *      A pointer to a descriptor. On successful return, a copy of the
 *      specified Apple event attribute, coerced, if necessary, to the
 *      descriptor type specified in desiredType. On error, a null
 *      descriptor. If the function returns successfully, your
 *      application should call the AEDisposeDesc function to dispose
 *      of the resulting descriptor after it has finished using it. See
 *      AEDesc.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetAttributeDesc(
  const AppleEvent *  theAppleEvent,
  AEKeyword           theAEKeyword,
  DescType            desiredType,
  AEDesc *            result)                                 API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AESizeOfAttribute()
 *  
 *  Discussion:
 *    Gets the size and descriptor type of an Apple event attribute
 *    from a descriptor of type AppleEvent.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to get the attribute data from.
 *    
 *    theAEKeyword:
 *      The keyword that specifies the attribute.
 *    
 *    typeCode:
 *      A pointer to a descriptor type. On return, specifies the
 *      descriptor type of the attribute. Can be NULL.
 *    
 *    dataSize:
 *      A pointer to a size variable. On return, the length, in bytes,
 *      of the data in the attribute. Can be NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AESizeOfAttribute(
  const AppleEvent *  theAppleEvent,
  AEKeyword           theAEKeyword,
  DescType *          typeCode,            /* can be NULL */
  Size *              dataSize)            /* can be NULL */  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  AEPutAttributePtr()
 *  
 *  Discussion:
 *    Adds a pointer to data, a descriptor type, and a keyword to an
 *    Apple event as an attribute.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to add an attribute to. See the
 *      AppleEvent data type.
 *    
 *    theAEKeyword:
 *      The keyword for the attribute to add. If the Apple event
 *      already includes an attribute with this keyword, the attribute
 *      is replaced.
 *    
 *    typeCode:
 *      The descriptor type for the attribute to add.
 *    
 *    dataPtr:
 *      A pointer to the data for the attribute to add.
 *    
 *    dataSize:
 *      The length, in bytes, of the data for the attribute to add.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEPutAttributePtr(
  AppleEvent *  theAppleEvent,
  AEKeyword     theAEKeyword,
  DescType      typeCode,
  const void *  dataPtr,
  Size          dataSize)                                     API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEPutAttributeDesc()
 *  
 *  Summary:
 *    Adds a descriptor and a keyword to an Apple event as an attribute.
 *  
 *  Discussion:
 *    The AEPutAttributeDesc function takes a descriptor and a keyword
 *    and adds them to an Apple event as an attribute. If the
 *    descriptor type required for the attribute is different from the
 *    descriptor type of the descriptor, the Apple Event Manager
 *    attempts to coerce the descriptor into the required type, with
 *    one exception: the Apple Event Manager does not attempt to coerce
 *    the data for an address attribute, thereby allowing applications
 *    to use their own address types.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAppleEvent:
 *      A pointer to the Apple event to add an attribute to. See the
 *      AppleEvent data type.
 *    
 *    theAEKeyword:
 *      The keyword for the attribute to add. If the Apple event
 *      already includes an attribute with this keyword, the attribute
 *      is replaced.
 *    
 *    theAEDesc:
 *      A pointer to the descriptor to assign to the attribute. The
 *      descriptor type of the specified descriptor should match the
 *      defined descriptor type for that attribute.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEPutAttributeDesc(
  AppleEvent *    theAppleEvent,
  AEKeyword       theAEKeyword,
  const AEDesc *  theAEDesc)                                  API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/**************************************************************************
 AppleEvent Serialization Support

    AESizeOfFlattenedDesc, AEFlattenDesc, AEUnflattenDesc
    
    These calls will work for all AppleEvent data types and between different
    versions of the OS (including between Mac OS 9 and X)
    
    Basic types, AEDesc, AEList and AERecord are OK, but AppleEvent records
    themselves may not be reliably flattened for storage.
**************************************************************************/
/*
 *  AESizeOfFlattenedDesc()
 *  
 *  Discussion:
 *    Returns the amount of buffer space needed to flatten the AEDesc.
 *    Call this before AEFlattenDesc to make sure your buffer has
 *    enough room for the operation.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDesc:
 *      A pointer to the descriptor to be flattened.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern Size 
AESizeOfFlattenedDesc(const AEDesc * theAEDesc)               API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEFlattenDesc()
 *  
 *  Discussion:
 *    Fills a buffer with a flattened representation of the AEDesc and
 *    returns the amount of buffer used in actualSize. If bufferSize
 *    was too small it returns errAEBufferTooSmall (-1741) and does not
 *    fill in any of the buffer. The resulting buffer is only useful
 *    with an AEUnflattenDescFromBytes call. 
 *    Note: if you pass a NULL buffer pointer it returns noErr but
 *    fills in the actualSize field anyway.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDesc:
 *      A pointer to the descriptor to be flattened.
 *    
 *    buffer:
 *      A pointer to memory, allocated by the application, where the
 *      flattened data will be stored. See the bufferSize parameter for
 *      information on how large a buffer you should allocate.
 *    
 *    bufferSize:
 *      The size of the buffer pointed to by buffer. Prior to calling
 *      AEFlattenDesc, you call the AESizeOfFlattenedDesc function to
 *      determine the required size of the buffer for the flatten
 *      operation.
 *      If bufferSize is too small, AEFlattenDesc returns
 *      errAEBufferTooSmall and doesn't store any data in the buffer.
 *    
 *    actualSize:
 *      A pointer to a size variable. On return, the variable contains
 *      the actual size of the flattened data. You can specify NULL for
 *      this parameter if you do not care about the returned size.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEFlattenDesc(
  const AEDesc *  theAEDesc,
  Ptr             buffer,
  Size            bufferSize,
  Size *          actualSize)                                 API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


extern OSStatus
AEUnflattenDesc(
  const void *  buffer,
  AEDesc *      result)                                       API_DEPRECATED_WITH_REPLACEMENT("AEUnflattenDescFromBytes", macos(10.0,11.0)) API_UNAVAILABLE( ios, tvos, watchos );

/**
 * Allocates an AEDesc (given a Null Desc) constructed from a flattened data
 * buffer produced by calling AEFlattenDesc.
 *
 * @param buffer A pointer to data produced by `AEFlattenDesc`
 * @param bufferLen The size of the data referenced by `buffer`
 * @param result On successful completion, a pointer to an `AEDesc*` containing the unflattened descriptor. The caller is responsible for disposing of it.
 * @return `noErr` on success, `paramErr` if the buffer could not be parsed, or `memFullErr` for irrational memory sizes.
 */
extern OSStatus AEUnflattenDescFromBytes( const void* buffer, size_t bufferLen, AEDesc* result ) API_AVAILABLE( macos(11.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/**************************************************************************
 The following calls are necessary to deal with opaque data in AEDescs, because the
 traditional way of dealing with a basic AEDesc has been to dereference the dataHandle
 directly.  This is not supported under Carbon.
**************************************************************************/
/*
 *  AEGetDescData()
 *  
 *  Discussion:
 *    Gets the data from the specified descriptor.  AEGetDescData no
 *    longer supports automatic coercion. If you'd like to coerce the
 *    descriptor use AECoerceDesc.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDesc:
 *      A pointer to the descriptor to get the data from.
 *    
 *    dataPtr:
 *      A pointer to a buffer, local variable, or other storage
 *      location created and disposed of by your application. The size
 *      in bytes should be the same as the value you pass in the
 *      maximumSize parameter. On return, contains the data from the
 *      descriptor.
 *    
 *    maximumSize:
 *      The length, in bytes, of the expected descriptor data. The
 *      AEGetDescData function will not return more data than you
 *      specify in this parameter. You typically determine the maximum
 *      size by calling AEGetDescDataSize.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later
 */
extern OSErr 
AEGetDescData(
  const AEDesc *  theAEDesc,
  void *          dataPtr,
  Size            maximumSize)                                API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetDescDataSize()
 *  
 *  Discussion:
 *    Gets the size, in bytes, of the data in the specified descriptor.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    theAEDesc:
 *      A pointer to the descriptor to obtain the data size for. See
 *      AEDesc.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later
 */
extern Size 
AEGetDescDataSize(const AEDesc * theAEDesc)                   API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEReplaceDescData()
 *  
 *  Discussion:
 *    Copies the specified data into the specified descriptor,
 *    replacing any previous data.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    typeCode:
 *      Specifies the descriptor type of the data pointed to by dataPtr.
 *    
 *    dataPtr:
 *      A pointer to the data to store in the specified descriptor.
 *    
 *    dataSize:
 *      The size, in bytes, of the data pointed to by the dataSize
 *      parameter.
 *    
 *    theAEDesc:
 *      A pointer to a descriptor. On return, contains the copied data.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later
 */
extern OSErr 
AEReplaceDescData(
  DescType      typeCode,
  const void *  dataPtr,
  Size          dataSize,
  AEDesc *      theAEDesc)                                    API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetDescDataRange()
 *  
 *  Discussion:
 *    Retrieve a range of bytes from an AEDesc.  This obviates the need
 *    to retrieve the entire data from the event using AEGetDescData.
 *    This is only valid for data type AEDescs.  If the requested
 *    length and offset are such that they do not fit entirely with the
 *    data of the desc, errAEBufferTooSmall is returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    dataDesc:
 *      A pointer to the descriptor to get the data from.
 *    
 *    buffer:
 *      A pointer to a buffer, local variable, or other storage
 *      location created and disposed of by your application. The size
 *      in bytes should be at least as large as the value you pass in
 *      the length parameter. On return, contains the specified data
 *      from the descriptor.
 *    
 *    offset:
 *      The zero-based offset to the data to be retrieved from the
 *      descriptor.
 *    
 *    length:
 *      The number of bytes of contiguous data to retrieve.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEGetDescDataRange(
  const AEDesc *  dataDesc,
  void *          buffer,
  Size            offset,
  Size            length)                                     API_AVAILABLE( macos(10.2) ) API_UNAVAILABLE( ios, tvos, watchos );


/**************************************************************************
  A AEEventHandler is installed to process an AppleEvent 
**************************************************************************/
typedef CALLBACK_API( OSErr , AEEventHandlerProcPtr )(const AppleEvent *theAppleEvent, AppleEvent *reply, SRefCon handlerRefcon);
typedef STACK_UPP_TYPE(AEEventHandlerProcPtr)                   AEEventHandlerUPP;
/*
 *  NewAEDisposeExternalUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern AEDisposeExternalUPP
NewAEDisposeExternalUPP(AEDisposeExternalProcPtr userRoutine) API_AVAILABLE( macos(10.2) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  NewAEEventHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern AEEventHandlerUPP
NewAEEventHandlerUPP(AEEventHandlerProcPtr userRoutine)       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  DisposeAEDisposeExternalUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeAEDisposeExternalUPP(AEDisposeExternalUPP userUPP)     API_AVAILABLE( macos(10.2) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  DisposeAEEventHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeAEEventHandlerUPP(AEEventHandlerUPP userUPP)           API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  InvokeAEDisposeExternalUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeAEDisposeExternalUPP(
  const void *          dataPtr,
  Size                  dataLength,
  SRefCon               refcon,
  AEDisposeExternalUPP  userUPP)                              API_AVAILABLE( macos(10.2) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  InvokeAEEventHandlerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeAEEventHandlerUPP(
  const AppleEvent *  theAppleEvent,
  AppleEvent *        reply,
  SRefCon             handlerRefcon,
						AEEventHandlerUPP   userUPP)                                API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );

#if __MACH__
  #ifdef __cplusplus
    inline AEDisposeExternalUPP                                 NewAEDisposeExternalUPP(AEDisposeExternalProcPtr userRoutine) { return userRoutine; }
    inline AEEventHandlerUPP                                    NewAEEventHandlerUPP(AEEventHandlerProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeAEDisposeExternalUPP(AEDisposeExternalUPP) { }
    inline void                                                 DisposeAEEventHandlerUPP(AEEventHandlerUPP) { }
    inline void                                                 InvokeAEDisposeExternalUPP(const void * dataPtr, Size dataLength, SRefCon refcon, AEDisposeExternalUPP userUPP) { (*userUPP)(dataPtr, dataLength, refcon); }
    inline OSErr                                                InvokeAEEventHandlerUPP(const AppleEvent * theAppleEvent, AppleEvent * reply, SRefCon handlerRefcon, AEEventHandlerUPP userUPP) { return (*userUPP)(theAppleEvent, reply, handlerRefcon); }
  #else
    #define NewAEDisposeExternalUPP(userRoutine)                ((AEDisposeExternalUPP)userRoutine)
    #define NewAEEventHandlerUPP(userRoutine)                   ((AEEventHandlerUPP)userRoutine)
    #define DisposeAEDisposeExternalUPP(userUPP)
    #define DisposeAEEventHandlerUPP(userUPP)
    #define InvokeAEDisposeExternalUPP(dataPtr, dataLength, refcon, userUPP) (*userUPP)(dataPtr, dataLength, refcon)
    #define InvokeAEEventHandlerUPP(theAppleEvent, reply, handlerRefcon, userUPP) (*userUPP)(theAppleEvent, reply, handlerRefcon)
  #endif
#endif



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __AEDATAMODEL__ */

