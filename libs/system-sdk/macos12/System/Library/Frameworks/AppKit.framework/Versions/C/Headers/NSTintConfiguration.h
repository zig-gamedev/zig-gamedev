/*
        NSTintConfiguration.h
        Application Kit
        Copyright (c) 2020-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSColor.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

API_AVAILABLE(macos(11.0))
@interface NSTintConfiguration : NSObject <NSCopying, NSSecureCoding>

/*!
 Specifies that content should be tinted using the system default for its context.
 For example, a source list icon's default tint matches the active Accent Color.
 */
@property (class, readonly, strong) NSTintConfiguration *defaultTintConfiguration;

/*!
 Specifies that content should prefer a monochrome appearance.
 Monochrome content remains monochrome regardless of the system Accent Color.
 */
@property (class, readonly, strong) NSTintConfiguration *monochromeTintConfiguration;

/*!
 Specifies that content should be tinted with a particular color whenever the app's preferred Accent Color is in use, i.e. when the system Accent Color is configured to "Multicolor". If the system Accent Color is configured to any other color, this tint configuration defers to the Accent Color.

 This type of configuration should be used for custom colors that are designed to match an app-specific Accent Color, but would mismatch a user-selected color.
 */
+ (instancetype)tintConfigurationWithPreferredColor:(NSColor *)color;

/*!
 Specifies that content should be tinted with a specific color value.
 The specified color value is used regardless of the system Accent Color.
 */
+ (instancetype)tintConfigurationWithFixedColor:(NSColor *)color;

/*!
 The base NSColor supplied when creating the tint configuration object. If the receiver wasn't created using a base NSColor, this property returns nil.
 */
@property (readonly, nullable, strong) NSColor *baseTintColor;

/*!
 An equivalent NSColor matching the effective content tint of the receiver. If the receiver can't be represented as a NSColor, this property returns nil.
 */
@property (readonly, nullable, strong) NSColor *equivalentContentTintColor;

/*!
 If YES, the tint configuration alters its effect based on the user's preferred Accent Color. Otherwise, the tint configuration produces a constant effect regardless of the Accent Color preference.
 */
@property (readonly) BOOL adaptsToUserAccentColor;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
