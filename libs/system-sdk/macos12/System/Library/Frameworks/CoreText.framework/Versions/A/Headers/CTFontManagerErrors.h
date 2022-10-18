/*
 *  CTFontManagerErrors.h
 *  CoreText
 *
 *  Copyright (c) 2008-2019 Apple Inc. All rights reserved.
 *
 */

#ifndef __CTFONTMANAGERERRORS__
#define __CTFONTMANAGERERRORS__

#include <CoreText/CTDefines.h>

#include <CoreFoundation/CoreFoundation.h>

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/*!
    @const      kCTFontManagerErrorDomain
    @abstract   CFError domain for CTFontManager errors
    @discussion CFErrors with this domain will have error codes corresponding to one of the CTFontManagerErrors above.
*/
CT_EXPORT const CFStringRef kCTFontManagerErrorDomain CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @constant   kCTFontManagerErrorFontURLsKey
    @abstract   User info key to be used with CFError references returned from registration functions.
    @discussion The value associated with this key in the user info dictionary of a CFError is a CFArray of font URLs that failed with given error.
 */
CT_EXPORT const CFStringRef kCTFontManagerErrorFontURLsKey CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @constant   kCTFontManagerErrorFontDescriptorsKey
    @abstract   User info key to be used with CFError references returned from registration functions.
    @discussion The value associated with this key in the user info dictionary of a CFError is a CFArray of font descriptors that failed with given error.
 */
CT_EXPORT const CFStringRef kCTFontManagerErrorFontDescriptorsKey CT_AVAILABLE(ios(13.0)) API_UNAVAILABLE(macos, watchos, tvos);

/*!
    @constant   kCTFontManagerErrorFontAssetNameKey
    @abstract   User info key to be used with CFError references returned from registration functions.
    @discussion The value associated with this key in the user info dictionary of a CFError is a CFArray of font asset name strings that failed with given error.
 */
CT_EXPORT const CFStringRef kCTFontManagerErrorFontAssetNameKey CT_AVAILABLE(ios(13.0)) API_UNAVAILABLE(macos, watchos, tvos);

/*!
    @enum
    @abstract   Font registration errors
    @discussion Errors that would prevent registration of fonts for a specified font file URL.
    @constant   kCTFontManagerErrorFileNotFound
                The file does not exist at the specified URL.
    @constant   kCTFontManagerErrorInsufficientPermissions
                Cannot access the file due to insufficient permissions.
    @constant   kCTFontManagerErrorUnrecognizedFormat
                The file is not a recognized or supported font file format.
    @constant   kCTFontManagerErrorInvalidFontData
                The file contains invalid font data that could cause system problems.
    @constant   kCTFontManagerErrorAlreadyRegistered
                The file has already been registered in the specified scope.
    @discussion Errors that would prevent un-registration of fonts for a specified font file URL.
    @constant   kCTFontManagerErrorNotRegistered
                The file is not registered in the specified scope.
    @constant   kCTFontManagerErrorInUse
                The font file is actively in use and cannot be unregistered.
    @constant   kCTFontManagerErrorSystemRequired
                The file is required by the system and cannot be unregistered.
    @constant   kCTFontManagerErrorRegistrationFailed
                The file could not be processed due to an unexpected FontProvider error.
    @constant   kCTFontManagerErrorMissingEntitlement
                The file could not be processed because the provider does not have a necessary entitlement.
    @constant   kCTFontManagerErrorInsufficientInfo
                The font descriptor does not have information to specify a font file.
    @constant   kCTFontManagerErrorCancelledByUser
                The operation was cancelled by the user.
    @constant   kCTFontManagerErrorDuplicatedName
                The file could not be registered because of a duplicated font name.
    @constant   kCTFontManagerErrorInvalidFilePath
                The file is not in an allowed location. It must be either in the application's bundle or an on-demand resource.
    @constant   kCTFontManagerErrorExceededResourceLimit
                The operation failed due to a system limitation.
    @constant   kCTFontManagerErrorUnsupportedScope
                The specified scope is not supported.

*/
typedef CF_ENUM(CFIndex, CTFontManagerError) {
    kCTFontManagerErrorFileNotFound             = 101,
    kCTFontManagerErrorInsufficientPermissions  = 102,
    kCTFontManagerErrorUnrecognizedFormat       = 103,
    kCTFontManagerErrorInvalidFontData          = 104,
    kCTFontManagerErrorAlreadyRegistered        = 105,
    kCTFontManagerErrorExceededResourceLimit    = 106,
    kCTFontManagerErrorAssetNotFound            = 107,
    kCTFontManagerErrorNotRegistered            = 201,
    kCTFontManagerErrorInUse                    = 202,
    kCTFontManagerErrorSystemRequired           = 203,
    kCTFontManagerErrorRegistrationFailed       = 301,
    kCTFontManagerErrorMissingEntitlement       = 302,
    kCTFontManagerErrorInsufficientInfo         = 303,
    kCTFontManagerErrorCancelledByUser          = 304,
    kCTFontManagerErrorDuplicatedName           = 305,
    kCTFontManagerErrorInvalidFilePath          = 306,
    kCTFontManagerErrorUnsupportedScope         = 307,
};

CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif
