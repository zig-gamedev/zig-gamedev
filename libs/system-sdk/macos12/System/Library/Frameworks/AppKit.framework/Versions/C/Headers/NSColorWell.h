/*
	NSColorWell.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSColorWell : NSControl

- (void)deactivate;
- (void)activate:(BOOL)exclusive;
@property (getter=isActive, readonly) BOOL active;

- (void)drawWellInside:(NSRect)insideRect;

@property (getter=isBordered) BOOL bordered;

- (void)takeColorFrom:(nullable id)sender;
@property (copy) NSColor *color;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
