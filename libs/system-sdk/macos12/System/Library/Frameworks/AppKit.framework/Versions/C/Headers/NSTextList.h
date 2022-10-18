#if !__has_include(<UIFoundation/NSTextList.h>)
/*
    NSTextList.h
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.

    Class to represent text lists.
*/

#import <Foundation/NSObject.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

typedef NSString * NSTextListMarkerFormat NS_TYPED_EXTENSIBLE_ENUM;
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerBox API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerCheck API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerCircle API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerDiamond API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerDisc API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerHyphen API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerSquare API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerLowercaseHexadecimal API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerUppercaseHexadecimal API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerOctal API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerLowercaseAlpha API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerUppercaseAlpha API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerLowercaseLatin API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerUppercaseLatin API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerLowercaseRoman API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerUppercaseRoman API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSTextListMarkerFormat const NSTextListMarkerDecimal API_AVAILABLE(macos(10.13));

typedef NS_OPTIONS(NSUInteger, NSTextListOptions) {
    NSTextListPrependEnclosingMarker = (1 << 0)
};

@interface NSTextList : NSObject <NSSecureCoding, NSCopying>

- (instancetype)initWithMarkerFormat:(NSTextListMarkerFormat)format options:(NSUInteger)mask;
@property (readonly, copy) NSTextListMarkerFormat markerFormat;
@property (readonly) NSTextListOptions listOptions;
- (NSString *)markerForItemNumber:(NSInteger)itemNum;
@property NSInteger startingItemNumber API_AVAILABLE(macos(10.6));

@end



#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSTextList.h>
#endif
