/*
     File:       AEDataModel.r
 
     Contains:   AppleEvent Data Model Interfaces.
 
    
 
     Copyright:  � 1996-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __AEDATAMODEL_R__
#define __AEDATAMODEL_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

/* Apple event descriptor types */
#define typeBoolean 					'bool'
#define typeChar 						'TEXT'				/*  Deprecated, use typeUTF8Text instead.  */

/*
 * The following descriptor types are deprecated due to their lack of
 * explicit encoding or byte order definition.  Please use
 * typeUTF16ExternalRepresentation or typeUTF8Text instead. */
#define typeStyledUnicodeText 			'sutx'				/*  Not implemented  */
#define typeEncodedString 				'encs'				/*  Not implemented  */
#define typeUnicodeText 				'utxt'				/*  native byte ordering, optional BOM  */
#define typeCString 					'cstr'				/*  MacRoman characters followed by a NULL byte  */
#define typePString 					'pstr'				/*  Unsigned length byte followed by MacRoman characters  */

/*
 * The preferred unicode text types.  In both cases, there is no explicit null termination or length byte.
 */
#define typeUTF16ExternalRepresentation  'ut16'				/*  big-endian 16 bit unicode with optional byte-order-mark, or little-endian 16 bit unicode with required byte-order-mark.  */
#define typeUTF8Text 					'utf8'				/*  8 bit unicode  */

/* Preferred numeric Apple event descriptor types */
#define typeSInt16 						'shor'				/*  SInt16 : signed, 16 bit integer  */
#define typeUInt16 						'ushr'				/*  UInt16 : unsigned, 16 bit integer  */
#define typeSInt32 						'long'				/*  SInt32 : signed, 32 bit integer  */
#define typeUInt32 						'magn'				/*  UInt32 : unsigned, 32 bit integer  */
#define typeSInt64 						'comp'				/*  SInt64 : signed, 64 bit integer  */
#define typeUInt64 						'ucom'				/*  UInt64 : unsigned, 64 bit integer  */
#define typeIEEE32BitFloatingPoint 		'sing'				/*  float  */
#define typeIEEE64BitFloatingPoint 		'doub'				/*  double  */
#define type128BitFloatingPoint 		'ldbl'
#define typeDecimalStruct 				'decm'

/* Non-preferred Apple event descriptor types */

#ifndef __LP64__
#define __LP64__ 0
#endif

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
#if __LP64__
#else
#define typeSMInt 						'shor'
#define typeShortInteger 				'shor'
#define typeInteger 					'long'
#define typeLongInteger 				'long'
#define typeMagnitude 					'magn'
#define typeComp 						'comp'
#define typeSMFloat 					'sing'
#define typeShortFloat 					'sing'
#define typeFloat 						'doub'
#define typeLongFloat 					'doub'
#define typeExtended 					'exte'

#endif  /* __LP64__ */

/* More Apple event descriptor types */
#define typeAEList 						'list'
#define typeAERecord 					'reco'
#define typeAppleEvent 					'aevt'
#define typeEventRecord 				'evrc'
#define typeTrue 						'true'
#define typeFalse 						'fals'
#define typeAlias 						'alis'				/*  AliasPtr, from a valid AliasHandle  */
#define typeEnumerated 					'enum'
#define typeType 						'type'				/*  OSType  */
#define typeAppParameters 				'appa'
#define typeProperty 					'prop'
#define typeFSRef 						'fsrf'				/*  FSRef  */
#define typeFileURL 					'furl'				/*  a UTF-8 encoded full path, using native path separators  */
#define typeBookmarkData 				'bmrk'				/*  the bytes of a CFURLBookmarkData  */
#define typeKeyword 					'keyw'				/*  OSType  */
#define typeSectionH 					'sect'
#define typeWildCard 					'****'
#define typeApplSignature 				'sign'				/*  OSType  */
#define typeQDRectangle 				'qdrt'
#define typeFixed 						'fixd'
#define typeProcessSerialNumber 		'psn '				/*  ProcessSerialNumber  */
#define typeApplicationURL 				'aprl'
#define typeNull 						'null'				/*  null or nonexistent data  */

/*
    FSSpecs are deprecated on Mac OS X, and their use in AppleEvents is discouraged.  You should change
    your code to use FSRefs.  In __LP64__ code, coercions into typeFSS is not supported,
    and coercion from typeFSS is not guaranteed to work correctly in all cases.
*/
#if !__LP64__
#define typeFSS 						'fss '				/*  FSSpec  */
#endif  /* !__LP64__ */

#define typeCFAttributedStringRef 		'cfas'
#define typeCFMutableAttributedStringRef  'cfaa'
#define typeCFStringRef 				'cfst'
#define typeCFMutableStringRef 			'cfms'
#define typeCFArrayRef 					'cfar'
#define typeCFMutableArrayRef 			'cfma'
#define typeCFDictionaryRef 			'cfdc'
#define typeCFMutableDictionaryRef 		'cfmd'
#define typeCFNumberRef 				'cfnb'
#define typeCFBooleanRef 				'cftf'
#define typeCFTypeRef 					'cfty'

/* New addressing modes for MacOS X */
#define typeKernelProcessID 			'kpid'
#define typeMachPort 					'port'

/* Targeting applications by bundle ID is only available in Mac OS X 10.3 or later. */
#define typeApplicationBundleID 		'bund'
/* Keywords for Apple event attributes */
#define keyTransactionIDAttr 			'tran'				/*  AETransactionID  */
#define keyReturnIDAttr 				'rtid'				/*  AEReturnID  */
#define keyEventClassAttr 				'evcl'				/*  AEEventClass  */
#define keyEventIDAttr 					'evid'				/*  AEEventID  */
#define keyAddressAttr 					'addr'
#define keyOptionalKeywordAttr 			'optk'
#define keyTimeoutAttr 					'timo'				/*  SInt32  */
#define keyInteractLevelAttr 			'inte'				/*  this attribute is read only - will be set in AESend  */
#define keyEventSourceAttr 				'esrc'				/*  this attribute is read only - returned as typeShortInteger  */
#define keyMissedKeywordAttr 			'miss'				/*  this attribute is read only  */
#define keyOriginalAddressAttr 			'from'				/*  new in 1.0.1  */
#define keyAcceptTimeoutAttr 			'actm'				/*  new for Mac OS X  */
#define keyReplyRequestedAttr 			'repq'				/*  Was a reply requested for this event - returned as typeBoolean  */
#define keySenderEUIDAttr 				'seid'				/*  read only, returned as typeSInt32.  Will be the euid of the sender of this event.  */
#define keySenderEGIDAttr 				'sgid'				/*  read only, returned as typeSInt32.  Will be the egid of the sender of this event.  */
#define keySenderUIDAttr 				'uids'				/*  read only, returned as typeSInt32.  Will be the uid of the sender of this event.  */
#define keySenderGIDAttr 				'gids'				/*  read only, returned as typeSInt32.  Will be the gid of the sender of this event.  */
#define keySenderPIDAttr 				'spid'				/*  read only, returned as typeSInt32.  Will be the pid of the sender of this event.  */

/* These bits are specified in the keyXMLDebuggingAttr (an SInt32) */
#define kAEDebugPOSTHeader 				0x01				/*  headers of the HTTP post we sent - typeChar  */
#define kAEDebugReplyHeader 			0x02				/*  headers returned by the server  */
#define kAEDebugXMLRequest 				0x04				/*  the XML request we sent  */
#define kAEDebugXMLResponse 			0x08				/*  the XML reply from the server  */
#define kAEDebugXMLDebugAll 			0xFFFFFFFF			/*  everything!  */

/* These values can be added as a parameter to the direct object of a
   SOAP message to specify the serialization schema.  If not
   specified, kSOAP1999Schema is the default. These should be added as
   typeType. */
#define kSOAP1999Schema 				'ss99'
#define kSOAP2001Schema 				'ss01'

															/*  outgoing event attributes  */
#define keyUserNameAttr 				'unam'
#define keyUserPasswordAttr 			'pass'				/*  not sent with the event  */
#define keyDisableAuthenticationAttr 	'auth'				/*  When present and with a non zero value (that is, false, or integer 0),  */
															/*  AESend will not authenticate the user.  If not present, or with a non-zero */
															/*  value, AESend will prompt for authentication information from the user if the interaction level allows.  */
#define keyXMLDebuggingAttr 			'xdbg'				/*  a bitfield of specifying which XML debugging data is to be returned with the event  */
															/*  Event class / id  */
#define kAERPCClass 					'rpc '				/*  for outgoing XML events  */
#define kAEXMLRPCScheme 				'RPC2'				/*  event ID: event should be sent to an XMLRPC endpoint  */
#define kAESOAPScheme 					'SOAP'				/*  event ID: event should be sent to a SOAP endpoint  */
#define kAESharedScriptHandler 			'wscp'				/*  event ID: handler for incoming XML requests  */
															/*  these parameters exist as part of the direct object of the event for both incoming and outgoing requests  */
#define keyRPCMethodName 				'meth'				/*  name of the method to call  */
#define keyRPCMethodParam 				'parm'				/*  the list (or structure) of parameters  */
#define keyRPCMethodParamOrder 			'/ord'				/*  if a structure, the order of parameters (a list)  */
															/*  when keyXMLDebugginAttr so specifies, these additional parameters will be part of the reply.  */
#define keyAEPOSTHeaderData 			'phed'				/*  what we sent to the server  */
#define keyAEReplyHeaderData 			'rhed'				/*  what the server sent to us  */
#define keyAEXMLRequestData 			'xreq'				/*  what we sent to the server  */
#define keyAEXMLReplyData 				'xrep'				/*  what the server sent to us  */
															/*  additional parameters that can be specified in the direct object of the event  */
#define keyAdditionalHTTPHeaders 		'ahed'				/*  list of additional HTTP headers (a list of 2 element lists)  */
#define keySOAPAction 					'sact'				/*  the SOAPAction header (required for SOAP messages)  */
#define keySOAPMethodNameSpace 			'mspc'				/*  Optional namespace (defaults to m:)  */
#define keySOAPMethodNameSpaceURI 		'mspu'				/*  Required namespace URI  */
#define keySOAPSchemaVersion 			'ssch'				/*  Optional XML Schema version, defaults to kSOAP1999Schama  */

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
#define keySOAPStructureMetaData 		'/smd'
#define keySOAPSMDNamespace 			'ssns'				/*  "myNamespace" */
#define keySOAPSMDNamespaceURI 			'ssnu'				/*  "http://myUri.org/xsd" */
#define keySOAPSMDType 					'sstp'				/*  "MyStructType" */

/* 
 * Web Services Proxy support.  Available only on Mac OS X 10.2 or later.
 * These constants should be added as attributes on the event that is
 * being sent (not part of the direct object.)
 */
															/*  Automatically configure the proxy based on System Configuration  */
#define kAEUseHTTPProxyAttr 			'xupr'				/*  a typeBoolean.  Defaults to true. */
															/*  manually specify the proxy host and port.  */
#define kAEHTTPProxyPortAttr 			'xhtp'				/*  a typeSInt32 */
#define kAEHTTPProxyHostAttr 			'xhth'				/*  a typeChar */

/*
 * Web Services SOCKS support.  kAEUseSocksAttr is a boolean that
 * specifies whether to automatically configure SOCKS proxies by
 * querying System Configuration.
 */
#define kAESocks4Protocol 				4
#define kAESocks5Protocol 				5

#define kAEUseSocksAttr 				'xscs'				/*  a typeBoolean.  Defaults to true. */
															/*  This attribute specifies a specific SOCKS protocol to be used  */
#define kAESocksProxyAttr 				'xsok'				/*  a typeSInt32 */
															/*  if version >= 4  */
#define kAESocksHostAttr 				'xshs'				/*  a typeChar */
#define kAESocksPortAttr 				'xshp'				/*  a typeSInt32 */
#define kAESocksUserAttr 				'xshu'				/*  a typeChar */
															/*  if version >= 5  */
#define kAESocksPasswordAttr 			'xshw'				/*  a typeChar */

#define kAENormalPriority 				0x00000000			/*  post message at the end of the event queue  */
#define kAEHighPriority 				0x00000001			/*  post message at the front of the event queue (same as nAttnMsg)  */

#define kAENoReply 						0x00000001			/*  sender doesn't want a reply to event  */
#define kAEQueueReply 					0x00000002			/*  sender wants a reply but won't wait  */
#define kAEWaitReply 					0x00000003			/*  sender wants a reply and will wait  */
#define kAEDontReconnect 				0x00000080			/*  don't reconnect if there is a sessClosedErr from PPCToolbox  */
#define kAEWantReceipt 					0x00000200			/*  (nReturnReceipt) sender wants a receipt of message  */
#define kAENeverInteract 				0x00000010			/*  server should not interact with user  */
#define kAECanInteract 					0x00000020			/*  server may try to interact with user  */
#define kAEAlwaysInteract 				0x00000030			/*  server should always interact with user where appropriate  */
#define kAECanSwitchLayer 				0x00000040			/*  interaction may switch layer  */
#define kAEDontRecord 					0x00001000			/*  don't record this event - available only in vers 1.0.1 and greater  */
#define kAEDontExecute 					0x00002000			/*  don't send the event for recording - available only in vers 1.0.1 and greater  */
#define kAEProcessNonReplyEvents 		0x00008000			/*  allow processing of non-reply events while awaiting synchronous AppleEvent reply  */
#define kAEDoNotAutomaticallyAddAnnotationsToEvent  0x00010000 /*  if set, don't automatically add any sandbox or other annotations to the event  */


#endif /* __AEDATAMODEL_R__ */

