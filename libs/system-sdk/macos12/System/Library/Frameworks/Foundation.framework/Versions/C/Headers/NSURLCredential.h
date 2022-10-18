/*	
    NSURLCredential.h
    Copyright (c) 2003-2019, Apple Inc. All rights reserved.    
    
    Public header file.
*/

#import <Foundation/NSObject.h>
#import <Security/Security.h>

@class NSString;
@class NSArray;

NS_ASSUME_NONNULL_BEGIN

/*!
    @enum NSURLCredentialPersistence
    @abstract Constants defining how long a credential will be kept around
    @constant NSURLCredentialPersistenceNone This credential won't be saved.
    @constant NSURLCredentialPersistenceForSession This credential will only be stored for this session.
    @constant NSURLCredentialPersistencePermanent This credential will be stored permanently. Note: Whereas in Mac OS X any application can access any credential provided the user gives permission, in iPhone OS an application can access only its own credentials.
    @constant NSURLCredentialPersistenceSynchronizable This credential will be stored permanently. Additionally, this credential will be distributed to other devices based on the owning AppleID.
        Note: Whereas in Mac OS X any application can access any credential provided the user gives permission, on iOS an application can 
        access only its own credentials.
*/

typedef NS_ENUM(NSUInteger, NSURLCredentialPersistence) {
    NSURLCredentialPersistenceNone,
    NSURLCredentialPersistenceForSession,
    NSURLCredentialPersistencePermanent,
    NSURLCredentialPersistenceSynchronizable API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0))
};

@class NSURLCredentialInternal;

/*!
    @class NSURLCredential
    @discussion This class is an immutable object representing an authentication credential.  The actual type of the credential is determined by the constructor called in the categories declared below.
*/

API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSURLCredential : NSObject <NSSecureCoding, NSCopying>
{
    @private
    __strong NSURLCredentialInternal *_internal;
}

/*!
    @abstract Determine whether this credential is or should be stored persistently
    @result A value indicating whether this credential is stored permanently, per session or not at all.
 */
@property (readonly) NSURLCredentialPersistence persistence;

@end

/*!
    @discussion This category defines the methods available to an NSURLCredential created to represent an internet password credential.  These are most commonly used for resources that require a username and password combination.
 */
@interface NSURLCredential(NSInternetPassword)

/*!
    @method initWithUser:password:persistence:
    @abstract Initialize a NSURLCredential with a user and password
    @param user the username
    @param password the password
    @param persistence enum that says to store per session, permanently or not at all
    @result The initialized NSURLCredential
*/
- (instancetype)initWithUser:(NSString *)user password:(NSString *)password persistence:(NSURLCredentialPersistence)persistence;

/*!
    @method credentialWithUser:password:persistence:
    @abstract Create a new NSURLCredential with a user and password
    @param user the username
    @param password the password
    @param persistence enum that says to store per session, permanently or not at all
    @result The new autoreleased NSURLCredential
*/
+ (NSURLCredential *)credentialWithUser:(NSString *)user password:(NSString *)password persistence:(NSURLCredentialPersistence)persistence;

/*!
    @abstract Get the username
    @result The user string
*/
@property (nullable, readonly, copy) NSString *user;

/*!
    @abstract Get the password
    @result The password string
    @discussion This method might actually attempt to retrieve the
    password from an external store, possible resulting in prompting,
    so do not call it unless needed.
*/
@property (nullable, readonly, copy) NSString *password;

/*!
    @abstract Find out if this credential has a password, without trying to get it
    @result YES if this credential has a password, otherwise NO
    @discussion If this credential's password is actually kept in an
    external store, the password method may return nil even if this
    method returns YES, since getting the password may fail, or the
    user may refuse access.
*/
@property (readonly) BOOL hasPassword;

@end

/*!
    @discussion This category defines the methods available to an NSURLCredential created to represent a client certificate credential.  Client certificates are commonly stored on the users computer in the keychain and must be presented to the server during a handshake.
*/
@interface NSURLCredential(NSClientCertificate)

/*!
    @method initWithIdentity:certificates:persistence:
    @abstract Initialize an NSURLCredential with an identity and array of at least 1 client certificates (SecCertificateRef)
    @param identity a SecIdentityRef object
    @param certArray an array containing at least one SecCertificateRef objects
    @param persistence enum that says to store per session, permanently or not at all
    @result the Initialized NSURLCredential
 */
- (instancetype)initWithIdentity:(SecIdentityRef)identity certificates:(nullable NSArray *)certArray persistence:(NSURLCredentialPersistence)persistence API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

/*!
    @method credentialWithIdentity:certificates:persistence:
    @abstract Create a new NSURLCredential with an identity and certificate array
    @param identity a SecIdentityRef object
    @param certArray an array containing at least one SecCertificateRef objects
    @param persistence enum that says to store per session, permanently or not at all
    @result The new autoreleased NSURLCredential
 */
+ (NSURLCredential *)credentialWithIdentity:(SecIdentityRef)identity certificates:(nullable NSArray *)certArray persistence:(NSURLCredentialPersistence)persistence API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

/*!
    @abstract Returns the SecIdentityRef of this credential, if it was created with a certificate and identity
    @result A SecIdentityRef or NULL if this is a username/password credential
 */
@property (nullable, readonly) SecIdentityRef identity;

/*!
    @abstract Returns an NSArray of SecCertificateRef objects representing the client certificate for this credential, if this credential was created with an identity and certificate.
    @result an NSArray of SecCertificateRef or NULL if this is a username/password credential
 */
@property (readonly, copy) NSArray *certificates API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

@end

@interface NSURLCredential(NSServerTrust)

/*!
    @method initWithTrust:
    @abstract Initialize a new NSURLCredential which specifies that the specified trust has been accepted.
    @result the Initialized NSURLCredential
 */
- (instancetype)initWithTrust:(SecTrustRef)trust API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

/*!
    @method credentialForTrust:
    @abstract Create a new NSURLCredential which specifies that a handshake has been trusted.
    @result The new autoreleased NSURLCredential
 */
+ (NSURLCredential *)credentialForTrust:(SecTrustRef)trust API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

@end

NS_ASSUME_NONNULL_END
