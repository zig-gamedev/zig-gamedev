/*
        NSAccessibilityColor.h
        Application Kit
        Copyright (c) 2020-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/NSColor.h>

NS_ASSUME_NONNULL_BEGIN

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
API_AVAILABLE(macos(11.0)) @protocol NSAccessibilityColor
@required
/*!
 @brief Returns a localized description of the color for use in accessibility attributes.
*/
@property(readonly) NSString *accessibilityName;
@end

@interface NSColor (NSAccessibilityColorConformance) <NSAccessibilityColor>
@end

NS_ASSUME_NONNULL_END
