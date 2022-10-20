/*
	NSAppleScriptExtensions.h
	Application Kit
	Copyright (c) 2002-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSAppleScript.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSAttributedString;



@interface NSAppleScript(NSExtensions)

// Return the syntax-highlighted source code of the script if the script has been compiled and its source code is available, nil otherwise.  It is possible for an NSAppleScript that has been instantiated with -initWithContentsOfURL:error: to be a script for which the source code is not available, but is nonetheless executable.
@property (nullable, readonly, strong) NSAttributedString *richTextSource;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

