/*
	NSNibLoading.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSBundle.h>
#import <AppKit/NSNib.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString, NSDictionary;

@interface NSBundle(NSNibLoading)

- (BOOL)loadNibNamed:(NSNibName)nibName owner:(nullable id)owner topLevelObjects:(NSArray * _Nullable * _Nullable)topLevelObjects API_AVAILABLE(macos(10.8));
    /* Load a nib from this bundle with the specified file name and owner. Upon success, the method returns YES and the optional out-parameter topLevelObjects is populated with the top level objects of the nib. The objects adhere to the standard Cocoa memory management rules and are autoreleased. IBOutlet properties to top level objects should be strong (retain) to demonstrate ownership and prevent deallocation. Alternatively, one may hold a strong reference to the top level objects array. */

@end

@interface NSObject (NSNibAwaking)
- (void)awakeFromNib;
    /* On Mac OS X 10.6 and later, NSObject provides an implementation of awakeFromNib. This means that you can safely call through to [super awakeFromNib] in an overridden implementation when running on Mac OS X 10.6 and later. */
- (void)prepareForInterfaceBuilder API_AVAILABLE(macos(10.10));
@end

@interface NSBundle (NSNibLoadingDeprecated)
+ (BOOL)loadNibFile:(null_unspecified NSString *)fileName externalNameTable:(null_unspecified NSDictionary *)context withZone:(null_unspecified NSZone *)zone API_DEPRECATED("", macos(10.0,10.8));
+ (BOOL)loadNibNamed:(null_unspecified NSString *)nibName owner:(null_unspecified id)owner API_DEPRECATED("", macos(10.0,10.8));
- (BOOL)loadNibFile:(null_unspecified NSString *)fileName externalNameTable:(null_unspecified NSDictionary *)context withZone:(null_unspecified NSZone *)zone API_DEPRECATED("", macos(10.0,10.8));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
