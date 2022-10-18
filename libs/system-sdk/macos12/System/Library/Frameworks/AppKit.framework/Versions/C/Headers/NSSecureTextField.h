/*
        NSSecureTextField.h
        Application Kit
        Copyright (c) 1995-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSTextField.h>
#import <AppKit/NSTextFieldCell.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSSecureTextField : NSTextField
@end

@interface NSSecureTextFieldCell : NSTextFieldCell

@property BOOL echosBullets;

@end

API_UNAVAILABLE_END
