#if !__has_include(<UIFoundation/NSGlyphInfo.h>)
/*
	NSGlyphInfo.h
	Copyright (c) 2002-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSString.h>
#import <AppKit/NSFont.h>

NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

@interface NSGlyphInfo : NSObject <NSCopying, NSSecureCoding>

+ (nullable NSGlyphInfo *)glyphInfoWithCGGlyph:(CGGlyph)glyph forFont:(NSFont *)font baseString:(NSString *)string API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

@property (readonly) CGGlyph glyphID API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));
@property (readonly, copy) NSString *baseString API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

@end

// Non-CGGlyph NSGlyphInfo support is now deprecated. The following API will be formally deprecated in a future version of macOS
typedef NS_ENUM(NSUInteger, NSCharacterCollection) {
    NSIdentityMappingCharacterCollection = 0, // Identity mapping (CID == NSGlyph)
    NSAdobeCNS1CharacterCollection = 1, // Adobe-CNS1
    NSAdobeGB1CharacterCollection = 2, // Adobe-GB1
    NSAdobeJapan1CharacterCollection = 3, // Adobe-Japan1
    NSAdobeJapan2CharacterCollection = 4, // Adobe-Japan2
    NSAdobeKorea1CharacterCollection = 5, // Adobe-Korea1
}; // Deprecated.

@interface NSGlyphInfo (NSGlyphInfo_Deprecated)
+ (nullable NSGlyphInfo *)glyphInfoWithGlyphName:(NSString *)glyphName forFont:(NSFont *)font baseString:(NSString *)string;
+ (nullable NSGlyphInfo *)glyphInfoWithGlyph:(NSGlyph)glyph forFont:(NSFont *)font baseString:(NSString *)string;
+ (nullable NSGlyphInfo *)glyphInfoWithCharacterIdentifier:(NSUInteger)cid collection:(NSCharacterCollection)characterCollection baseString:(NSString *)string ;
@property (nullable, readonly, copy) NSString *glyphName;
@property (readonly) NSUInteger characterIdentifier;
@property (readonly) NSCharacterCollection characterCollection;
@end



#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSGlyphInfo.h>
#endif

