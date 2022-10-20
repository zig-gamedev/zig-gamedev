/*
	 File:	   CFNetwork/CFFTPStream.h
 
	 Contains:   CoreFoundation FTP stream header
  
	 Copyright:  Copyright (c) 2001-2013 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFFTPSTREAM__
#define __CFFTPSTREAM__

#ifndef __CFNETWORKDEFS__
#include <CFNetwork/CFNetworkDefs.h>
#endif

#ifndef __CFSTREAM__
#include <CoreFoundation/CFStream.h>
#endif


#if PRAGMA_ONCE
#pragma once
#endif

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/*
 *  kCFStreamErrorDomainFTP
 *  
 *  Discussion:
 *	Result code returned by FTP server
 *
 */
CFN_EXPORT const SInt32 kCFStreamErrorDomainFTP CF_AVAILABLE(10_3, 2_0);


/*
 *  FTP Stream Property keys.
 *  These keys can be passed to the stream property "set/get" functions,
 *  such as CFReadStreamSetProperty/CFReadStreamCopyProperty, or to a
 *  CFDictionary creator or an item accessor/mutator.  The comment before
 *  each key declaration (treated as definition) indicates the value type
 *  of the property.
 */


/*
 *  kCFStreamPropertyFTPUserName
 *  
 *  Discussion:
 *	Stream property key, for both set and copy operations.  CFString
 *	type to hold login user name.  Don't set this property if you
 *	want anonymous FTP.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPUserName CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPPassword
 *  
 *  Discussion:
 *	Stream property key, for both set and copy operations. CFString
 *	type to hold login password.  Don't set this property if you want
 *	anonymous FTP.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPPassword CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPUsePassiveMode
 *  
 *  Discussion:
 *	Stream property key, for both set and copy operations. CFBoolean
 *	type. kCFBooleanTrue means use passive mode, kCFBooleanFalse
 *	otherwise
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPUsePassiveMode CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPResourceSize
 *  
 *  Discussion:
 *	Stream property key, for read stream copy operations.  CFNumber
 *	of kCFNumberLongLongType to hold resource size in bytes.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPResourceSize CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPFetchResourceInfo
 *  
 *  Discussion:
 *	Stream property key, for both set and copy operations.  CFBoolean
 *	type.  TRUE means that resource info, such as size, must be
 *	provided before download starts at higher cost.  Don't set if
 *	resource size/other info is unnecessary.  Initially, only
 *	resource size is implemented.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPFetchResourceInfo CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPFileTransferOffset
 *  
 *  Discussion:
 *	Stream property key, for both set and copy operations.  CFNumber
 *	of kCFNumberLongLongType for the file offset to start transfer at.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPFileTransferOffset CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPAttemptPersistentConnection
 *  
 *  Discussion:
 *	Stream property key, for both set and copy operations.  CFBoolean
 *	type.  TRUE by default, set to FALSE to avoid reusing existing
 *	server connections.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPAttemptPersistentConnection CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPProxy
 *  
 *  Discussion:
 *	Stream property key, for both set and copy operations. 
 *	CFDictionary type that holds key-value pairs of proxy dictionary.
 *	The dictionary returned by SystemConfiguration can also be
 *	passed directly as the value.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPProxy CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPProxyHost
 *  
 *  Discussion:
 *	Stream property key or FTP Proxy dictionary key, for both set and
 *	copy operations.  It matches kSCPropNetProxiesFTPProxy defined in
 *	SCSchemaDefinitions.h.  CFString for proxy server host name. 
 *	This property can be set and copied individually or via a
 *	CFDictionary.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPProxyHost CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFStreamPropertyFTPProxyPort
 *  
 *  Discussion:
 *	Stream property key or FTP Proxy dictionary key, for both set and
 *	copy operations.  It matches kSCPropNetProxiesFTPPort defined in
 *	SCSchemaDefinitions.h.  CFNumber of kCFNumberIntType for proxy
 *	server port number.  This property can be set and copied
 *	individually or via a CFDictionary.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPProxyPort CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");

/*
 *  kCFStreamPropertyFTPProxyUser
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPProxyUser CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");

/*
 *  kCFStreamPropertyFTPProxyPassword
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyFTPProxyPassword CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  CFDictionary keys for resource information.  The information is
 *  extracted from a line of the directory list by function
 *  CFFTPCreateParsedResourceListing.
 */


/*
 *  kCFFTPResourceMode
 *  
 *  Discussion:
 *	CFDictionary key, for get value operation.  CFNumber to hold the
 *	resource access permission defined in sys/types.h.
 *  
 */
CFN_EXPORT const CFStringRef kCFFTPResourceMode CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFFTPResourceName
 *  
 *  Discussion:
 *	CFDictionary key, for get value operation.  CFString that holds
 *	the resource name.
 *  
 */
CFN_EXPORT const CFStringRef kCFFTPResourceName CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFFTPResourceOwner
 *  
 *  Discussion:
 *	CFDictionary key, for get value operation.  CFString that holds
 *	the resource owner's name.
 *  
 */
CFN_EXPORT const CFStringRef kCFFTPResourceOwner CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFFTPResourceGroup
 *  
 *  Discussion:
 *	CFDictionary key, for get value operation.  CFString to hold the
 *	name of the group that shares the resource.
 *  
 */
CFN_EXPORT const CFStringRef kCFFTPResourceGroup CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFFTPResourceLink
 *  
 *  Discussion:
 *	CFDictionary key, for get value operation.  CFString to hold
 *	symbolic link information.  If the item is a symbolic link the
 *	string will contain the path to the item the link references.
 *  
 */
CFN_EXPORT const CFStringRef kCFFTPResourceLink CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFFTPResourceSize
 *  
 *  Discussion:
 *	CFDictionary key, for get value operation.  CFNumber of
 *	kCFNumberLongLongType to hold the resource length in bytes.
 *  
 */
CFN_EXPORT const CFStringRef kCFFTPResourceSize CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFFTPResourceType
 *  
 *  Discussion:
 *	CFDictionary key, for get value operation.  CFNumber to hold the
 *	resource type as defined in sys/dirent.h.
 *  
 */
CFN_EXPORT const CFStringRef kCFFTPResourceType CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  kCFFTPResourceModDate
 *  
 *  Discussion:
 *	CFDictionary key, for get value operation.  CFDate to hold the
 *	last modification date and time information.
 *  
 */
CFN_EXPORT const CFStringRef kCFFTPResourceModDate CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


/*
 *  CFReadStreamCreateWithFTPURL()
 *  
 *  DEPRECATED: Use NSURLSessionAPI for ftp requests
 *
 *  Discussion:
 *	Create an FTP read stream for downloading operation from an FTP
 *	URL. If the URL refers to a directory, the stream is a filtered
 *	line-at-a-time read stream corresponding to the listing results
 *	provided by the server. If it's a file, then the stream is a
 *	regular read stream providing the data for that file.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CF read stream and its storage for values. If
 *	  this reference is not a valid CFAllocator, the behavior is
 *	  undefined.
 *	
 *	ftpURL:
 *	  A pointer to a CFURL structure created by CFURLCreateWithString
 *	  function.  If this parameter is not a pointer to a valid CFURL
 *	  structure, the behavior is undefined.
 *  
 *  Result:
 *	A pointer to the CF read stream created, or NULL if failed. It is
 *	caller's responsibilty to release the memory allocated for the
 *	read stream.
 *  
 */
CFN_EXPORT CFReadStreamRef 
CFReadStreamCreateWithFTPURL(CFAllocatorRef __nullable alloc, CFURLRef ftpURL) CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");



/*
 *  CFFTPCreateParsedResourceListing()
 *  
 *  DEPRECATED: Use NSURLSessionAPI for ftp requests
 *
 *  Discussion:
 *	Parse a line of file or folder listing of Unix format, and store
 *	the extracted result in a CFDictionary.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CFDictionary to hold resource info.  If this
 *	  reference is not a valid CFAllocator, the behavior is undefined.
 *	
 *	buffer:
 *	  A pointer to a buffer that may hold lines of resource listing,
 *	  but only the first line starting from buffer[0] will be parsed
 *	  each call.
 *	
 *	bufferLength:
 *	  The maximum buffer size in bytes started from the location
 *	  pointed by "buffer."
 *	
 *	parsed:
 *	  A pointer to a CFDictionary pointer.  The dictionary holds the
 *	  extracted resource information.  When parsing fails, a NULL
 *	  pointer will be returned.  It is caller's responsibilty to
 *	  release the memory allocated for the dictionary.
 *  
 *  Result:
 *	The number of bytes consumed from buffer, 0 if there are not
 *	enough bytes, or -1 if a parse failure occurs.
 *  
 */
CFN_EXPORT CFIndex 
CFFTPCreateParsedResourceListing(CFAllocatorRef __nullable alloc, const UInt8 *buffer, CFIndex bufferLength, CFDictionaryRef __nullable *  __nullable parsed) CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");



/*
 *  CFWriteStreamCreateWithFTPURL()
 *
 *  DEPRECATED: Use NSURLSessionAPI for ftp requests
 *
 *  Discussion:
 *	Create an FTP write stream for uploading operation to a FTP URL.
 *	If the URL specifies a directory, the open will be followed by a
 *	close event/state and the directory will have been created. 
 *	Intermediary directory structure is not created.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CF read stream and its storage for values. If
 *	  this reference is not a valid CFAllocator, the behavior is
 *	  undefined.
 *	
 *	ftpURL:
 *	  A pointer to a CFURL structure created by CFURLCreateWithString
 *	  function.  If this parameter is not a pointer to a valid CFURL
 *	  structure, the behavior is undefined.
 *  
 *  Result:
 *	A pointer to the CF write stream created, or NULL if failed. It
 *	is caller's responsibilty to release the memory allocated for the
 *	write stream.
 *  
 */
CFN_EXPORT CFWriteStreamRef
CFWriteStreamCreateWithFTPURL(CFAllocatorRef __nullable alloc, CFURLRef ftpURL) CF_DEPRECATED(10_3, 10_11, 2_0, 9_0 , "Use NSURLSessionAPI for ftp requests");


CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif /* __CFFTPSTREAM__ */

