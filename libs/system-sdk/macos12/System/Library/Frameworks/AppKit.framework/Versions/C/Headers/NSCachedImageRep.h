/*
	NSCachedImageRep.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSImageRep.h>
#import <AppKit/NSGraphics.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSWindow;

// this entire class is deprecated.  Please see the 10.6 AppKit release notes for more information.
API_DEPRECATED("", macos(10.0,10.6))
@interface NSCachedImageRep : NSImageRep

/* References the specified rect within the window; the window is retained */
- (id)initWithWindow:(NSWindow *)win rect:(NSRect)rect API_DEPRECATED("", macos(10.0,10.6));

/* Creates a location in some window. Will be freed when rep is freed */
- (id)initWithSize:(NSSize)size depth:(NSWindowDepth)depth separate:(BOOL)flag alpha:(BOOL)alpha API_DEPRECATED("", macos(10.0,10.6));

- (NSWindow *)window API_DEPRECATED("", macos(10.0,10.6));
- (NSRect)rect API_DEPRECATED("", macos(10.0,10.6));

@end

API_UNAVAILABLE_END
