/*	
    NSURLResponse.h
    Copyright (c) 2003-2019, Apple Inc. All rights reserved.    
    
    Public header file.
*/


#import <Foundation/NSObject.h>

@class NSDictionary<KeyType, ObjectType>;
@class NSString;
@class NSURL;
@class NSURLRequest;
@class NSURLResponseInternal;

NS_ASSUME_NONNULL_BEGIN

#define NSURLResponseUnknownLength ((long long)-1)

/*!
    @class NSURLResponse

    @abstract An NSURLResponse object represents a URL load response in a
    manner independent of protocol and URL scheme.

    @discussion NSURLResponse encapsulates the metadata associated
    with a URL load. Note that NSURLResponse objects do not contain
    the actual bytes representing the content of a URL. See
    NSURLConnection and NSURLConnectionDelegate for more information
    about receiving the content data for a URL load.
*/
API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSURLResponse : NSObject <NSSecureCoding, NSCopying>
{
    @package
    NSURLResponseInternal *_internal;
}

/*!
    @method initWithURL:MIMEType:expectedContentLength:textEncodingName:
    @abstract Initialize an NSURLResponse with the provided values.
    @param URL the URL
    @param MIMEType the MIME content type of the response
    @param length the expected content length of the associated data
    @param name the name of the text encoding for the associated data, if applicable, else nil
    @result The initialized NSURLResponse.
    @discussion This is the designated initializer for NSURLResponse.
*/
- (instancetype)initWithURL:(NSURL *)URL MIMEType:(nullable NSString *)MIMEType expectedContentLength:(NSInteger)length textEncodingName:(nullable NSString *)name NS_DESIGNATED_INITIALIZER;

/*! 
    @abstract Returns the URL of the receiver.
    @result The URL of the receiver. 
*/
@property (nullable, readonly, copy) NSURL *URL;

/*! 
    @abstract Returns the MIME type of the receiver.
    @discussion The MIME type is based on the information provided
    from an origin source. However, that value may be changed or
    corrected by a protocol implementation if it can be determined
    that the origin server or source reported the information
    incorrectly or imprecisely. An attempt to guess the MIME type may
    be made if the origin source did not report any such information.
    @result The MIME type of the receiver.
*/
@property (nullable, readonly, copy) NSString *MIMEType;

/*! 
    @abstract Returns the expected content length of the receiver.
    @discussion Some protocol implementations report a content length
    as part of delivering load metadata, but not all protocols
    guarantee the amount of data that will be delivered in actuality.
    Hence, this method returns an expected amount. Clients should use
    this value as an advisory, and should be prepared to deal with
    either more or less data.
    @result The expected content length of the receiver, or -1 if
    there is no expectation that can be arrived at regarding expected
    content length.
*/
@property (readonly) long long expectedContentLength;

/*! 
    @abstract Returns the name of the text encoding of the receiver.
    @discussion This name will be the actual string reported by the
    origin source during the course of performing a protocol-specific
    URL load. Clients can inspect this string and convert it to an
    NSStringEncoding or CFStringEncoding using the methods and
    functions made available in the appropriate framework.
    @result The name of the text encoding of the receiver, or nil if no
    text encoding was specified. 
*/
@property (nullable, readonly, copy) NSString *textEncodingName;

/*!
    @abstract Returns a suggested filename if the resource were saved to disk.
    @discussion The method first checks if the server has specified a filename using the
    content disposition header. If no valid filename is specified using that mechanism,
    this method checks the last path component of the URL. If no valid filename can be
    obtained using the last path component, this method uses the URL's host as the filename.
    If the URL's host can't be converted to a valid filename, the filename "unknown" is used.
    In mose cases, this method appends the proper file extension based on the MIME type.
    This method always returns a valid filename.
    @result A suggested filename to use if saving the resource to disk.
*/
@property (nullable, readonly, copy) NSString *suggestedFilename;

@end



@class NSHTTPURLResponseInternal;

/*!
    @class NSHTTPURLResponse

    @abstract An NSHTTPURLResponse object represents a response to an
    HTTP URL load. It is a specialization of NSURLResponse which
    provides conveniences for accessing information specific to HTTP
    protocol responses.
*/
API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSHTTPURLResponse : NSURLResponse 
{
    @package
    NSHTTPURLResponseInternal *_httpInternal;
}

/*!
  @method	initWithURL:statusCode:HTTPVersion:headerFields:
  @abstract initializer for NSHTTPURLResponse objects.
  @param 	url the URL from which the response was generated.
  @param	statusCode an HTTP status code.
  @param	HTTPVersion The version of the HTTP response as represented by the server.  This is typically represented as "HTTP/1.1".
  @param 	headerFields A dictionary representing the header keys and values of the server response.
  @result 	the instance of the object, or NULL if an error occurred during initialization.
  @discussion This API was introduced in Mac OS X 10.7.2 and iOS 5.0 and is not available prior to those releases.
*/
- (nullable instancetype)initWithURL:(NSURL *)url statusCode:(NSInteger)statusCode HTTPVersion:(nullable NSString *)HTTPVersion headerFields:(nullable NSDictionary<NSString *, NSString *> *)headerFields API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/*! 
    @abstract Returns the HTTP status code of the receiver. 
    @result The HTTP status code of the receiver. 
*/
@property (readonly) NSInteger statusCode;

/*! 
    @abstract Returns a dictionary containing all the HTTP header fields
    of the receiver.
    @discussion By examining this header dictionary, clients can see
    the "raw" header information which was reported to the protocol
    implementation by the HTTP server. This may be of use to
    sophisticated or special-purpose HTTP clients.
    @result A dictionary containing all the HTTP header fields of the
    receiver.
*/
@property (readonly, copy) NSDictionary *allHeaderFields;

/*!
    @method valueForHTTPHeaderField:
    @abstract Returns the value which corresponds to the given header
    field. Note that, in keeping with the HTTP RFC, HTTP header field
    names are case-insensitive.
    @param field the header field name to use for the lookup
    (case-insensitive).
    @result the value associated with the given header field, or nil if
    there is no value associated with the given header field.
 */
- (nullable NSString *)valueForHTTPHeaderField:(NSString *)field API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*! 
    @method localizedStringForStatusCode:
    @abstract Convenience method which returns a localized string
    corresponding to the status code for this response.
    @param statusCode the status code to use to produce a localized string.
    @result A localized string corresponding to the given status code.
*/
+ (NSString *)localizedStringForStatusCode:(NSInteger)statusCode;

@end

NS_ASSUME_NONNULL_END
