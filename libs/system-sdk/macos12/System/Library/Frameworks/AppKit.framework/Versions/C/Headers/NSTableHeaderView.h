/*
    NSTableHeaderView.h
    Application Kit
    Copyright (c) 1995-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSView.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSTableView;
@class NSColor;
@class NSImage;
@class NSCursor;

@interface NSTableHeaderView : NSView <NSViewToolTipOwner>

@property (nullable, weak) NSTableView *tableView;

// Returns -1 if there is no column being dragged
@property (readonly) NSInteger draggedColumn;
@property (readonly) CGFloat draggedDistance;

@property (readonly) NSInteger resizedColumn;
- (NSRect)headerRectOfColumn:(NSInteger)column;
- (NSInteger)columnAtPoint:(NSPoint)point;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
