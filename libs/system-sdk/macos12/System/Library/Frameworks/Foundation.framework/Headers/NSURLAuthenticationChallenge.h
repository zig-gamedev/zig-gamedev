/*	
    NSURLAuthenticationChallenge.h
    Copyright (c) 2003-2019, Apple Inc. All rights reserved.    
    
    Public header file.
*/

#import <Foundation/NSObject.h>

@class NSURLAuthenticationChallenge;
@class NSURLCredential;
@class NSURLProtectionSpace;
@class NSURLResponse;
@class NSError;

NS_ASSUME_NONNULL_BEGIN

/*!  
    @protocol NSURLAuthenticationChallengeSender 
    @discussion This protocol represents the sender of an
    authentication challenge. It has methods to provide a credential,
    to continue without any credential, getting whatever failure
    result would happen in that case, cancel a challenge, perform the default
    action as defined by the system, or reject the currently supplied protection-space
    in the challenge.
*/

API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@protocol NSURLAuthenticationChallengeSender <NSObject>

/*!
    @method useCredential:forAuthenticationChallenge:
*/
- (void)useCredential:(NSURLCredential *)credential forAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;

/*!
    @method continueWithoutCredentialForAuthenticationChallenge:
*/
- (void)continueWithoutCredentialForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;

/*!
    @method cancelAuthenticationChallenge:
*/
- (void)cancelAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;

@optional
/*!
 @method performDefaultHandlingForAuthenticationChallenge:
 */
- (void)performDefaultHandlingForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;

/*!
 @method rejectProtectionSpaceAndContinueWithChallenge:
 */
- (void)rejectProtectionSpaceAndContinueWithChallenge:(NSURLAuthenticationChallenge *)challenge;

@end

@class NSURLAuthenticationChallengeInternal;

/*!
    @class NSURLAuthenticationChallenge
    @discussion This class represents an authentication challenge. It
    provides all the information about the challenge, and has a method
    to indicate when it's done.
*/

API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSURLAuthenticationChallenge : NSObject <NSSecureCoding>
{
@private
    NSURLAuthenticationChallengeInternal *_internal;
}

/*!
    @method initWithProtectionSpace:proposedCredential:previousFailureCount:failureResponse:error:
    @abstract Initialize an authentication challenge 
    @param space The NSURLProtectionSpace to use
    @param credential The proposed NSURLCredential for this challenge, or nil
    @param previousFailureCount A count of previous failures attempting access.
    @param response The NSURLResponse for the authentication failure, if applicable, else nil
    @param error The NSError for the authentication failure, if applicable, else nil
    @result An authentication challenge initialized with the specified parameters
*/
- (instancetype)initWithProtectionSpace:(NSURLProtectionSpace *)space proposedCredential:(nullable NSURLCredential *)credential previousFailureCount:(NSInteger)previousFailureCount failureResponse:(nullable NSURLResponse *)response error:(nullable NSError *)error sender:(id<NSURLAuthenticationChallengeSender>)sender;

/*!
    @method initWithAuthenticationChallenge:
    @abstract Initialize an authentication challenge copying all parameters from another one.
    @result A new challenge initialized with the parameters from the passed in challenge
    @discussion This initializer may be useful to subclassers that want to proxy
    one type of authentication challenge to look like another type.
*/
- (instancetype)initWithAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge sender:(id<NSURLAuthenticationChallengeSender>)sender;

/*!
    @abstract Get a description of the protection space that requires authentication
    @result The protection space that needs authentication
*/
@property (readonly, copy) NSURLProtectionSpace *protectionSpace;

/*!
    @abstract Get the proposed credential for this challenge
    @result The proposed credential
    @discussion proposedCredential may be nil, if there is no default
    credential to use for this challenge (either stored or in the
    URL). If the credential is not nil and returns YES for
    hasPassword, this means the NSURLConnection thinks the credential
    is ready to use as-is. If it returns NO for hasPassword, then the
    credential is not ready to use as-is, but provides a default
    username the client could use when prompting.
*/
@property (nullable, readonly, copy) NSURLCredential *proposedCredential;

/*!
    @abstract Get count of previous failed authentication attempts
    @result The count of previous failures
*/
@property (readonly) NSInteger previousFailureCount;

/*!
    @abstract Get the response representing authentication failure.
    @result The failure response or nil
    @discussion If there was a previous authentication failure, and
    this protocol uses responses to indicate authentication failure,
    then this method will return the response. Otherwise it will
    return nil.
*/
@property (nullable, readonly, copy) NSURLResponse *failureResponse;

/*!
    @abstract Get the error representing authentication failure.
    @discussion If there was a previous authentication failure, and
    this protocol uses errors to indicate authentication failure,
    then this method will return the error. Otherwise it will
    return nil.
*/
@property (nullable, readonly, copy) NSError *error;

/*!
    @abstract Get the sender of this challenge
    @result The sender of the challenge
    @discussion The sender is the object you should reply to when done processing the challenge.
*/
@property (nullable, readonly, retain) id<NSURLAuthenticationChallengeSender> sender;

@end

NS_ASSUME_NONNULL_END
