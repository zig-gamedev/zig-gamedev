/*
        NSStatusBar.h
        Application Kit
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor;
@class NSFont;
@class NSStatusItem;
@class NSMutableArray;

static const CGFloat NSVariableStatusItemLength = -1.0;
static const CGFloat NSSquareStatusItemLength = -2.0;

@interface NSStatusBar : NSObject

@property (class, readonly, strong) NSStatusBar *systemStatusBar;

- (NSStatusItem*)statusItemWithLength:(CGFloat)length;
- (void)removeStatusItem:(NSStatusItem*)item;

@property (getter=isVertical, readonly) BOOL vertical;
@property (readonly) CGFloat thickness;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
