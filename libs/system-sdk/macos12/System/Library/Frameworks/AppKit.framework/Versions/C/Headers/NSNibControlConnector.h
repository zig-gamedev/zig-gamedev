/*
	NSNibControlConnector.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSNibConnector.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSNibControlConnector : NSNibConnector
- (void)establishConnection;
@end

API_UNAVAILABLE_END
