/*
        NSAffineTransform.h
        Application Kit
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSAffineTransform.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSBezierPath;

@interface NSAffineTransform (NSAppKitAdditions)
// Transform a path
- (NSBezierPath *)transformBezierPath:(NSBezierPath *)path;

// Setting a transform in NSGraphicsContext
- (void)set;
- (void)concat;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
