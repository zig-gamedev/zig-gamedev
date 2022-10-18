/*
 NSColorSampler.h
 Application Kit
 Copyright (c) 2018-2021, Apple Inc.
 All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor;

/*!
 * Manages a color sampling interface to allow the user to select a color from their screen.
 */
NS_CLASS_AVAILABLE_MAC(10_15)
@interface NSColorSampler : NSObject

/*!
 * The primary method for NSColorSampler.
 *
 * Begins or attaches to an existing color sampling session which presents UI to the user for selecting a color from their screen. The handler will be called on the main thread when the user completes the session (either by selection, or cancelation). In the event of user-cancellation, `colorSelectionHandler` will be called with `nil`.
 *
 * The calling NSColorSampler instance is retained until the sampling session is completed.
 */
- (void)showSamplerWithSelectionHandler:(void (^)(NSColor * _Nullable selectedColor))selectionHandler NS_SWIFT_ASYNC_NAME(sample());

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
