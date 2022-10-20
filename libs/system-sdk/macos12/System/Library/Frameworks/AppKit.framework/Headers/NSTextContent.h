/*
        NSTextContent.h
        Application Kit
        Copyright (c) 2020-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>
#import <Availability.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NSString* NSTextContentType NS_TYPED_ENUM;

APPKIT_EXTERN API_AVAILABLE(macos(11.0)) NSTextContentType const NSTextContentTypeUsername;
APPKIT_EXTERN API_AVAILABLE(macos(11.0)) NSTextContentType const NSTextContentTypePassword;
APPKIT_EXTERN API_AVAILABLE(macos(11.0)) NSTextContentType const NSTextContentTypeOneTimeCode;

@protocol NSTextContent
@property (nullable, copy) NSTextContentType contentType API_AVAILABLE(macos(11.0));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
