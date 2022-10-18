/*
	NSFileWrapperExtensions.h
	Application Kit
	Copyright (c) 2002-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSFileWrapper.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSImage;

@interface NSFileWrapper(NSExtensions)

/* The icon. -icon may return nil even if the receiver is the result of reading a parent from the file system (use NSFileWrapperReadingImmediately if appropriate to prevent that). The image that's returned might be shared by many NSFileWrappers so your application must not mutate it. If your application needs to mutate the returned image it should make a copy and mutate the copy instead.
*/
@property (nullable, strong) NSImage *icon;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
