/*
	NSEPSImageRep.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSImageRep.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSPDFImageRep;

@interface NSEPSImageRep : NSImageRep

+ (nullable instancetype)imageRepWithData:(NSData *)epsData;	/* Convenience of initWithData: */
- (nullable instancetype)initWithData:(NSData *)epsData;

/* prepareGState is unused */
- (void)prepareGState API_DEPRECATED("", macos(10.0,10.10));

@property (readonly, copy) NSData *EPSRepresentation;

@property (readonly) NSRect boundingBox;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

