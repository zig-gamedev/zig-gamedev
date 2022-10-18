/*
	NSFontAssetRequest.h
	Application Kit
	Copyright (c) 2017-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSProgress.h>
#import <AppKit/AppKitDefines.h>

@class NSFontDescriptor;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_OPTIONS(NSUInteger, NSFontAssetRequestOptions) {
    NSFontAssetRequestOptionUsesStandardUI = 1 << 0, // Use standard system UI for downloading.
} API_AVAILABLE(macos(10.13));

/*
 The NSFontAssetRequest class is used to interact with the on demand font assets.

 The purpose of the system is to allow an application to download certain font assets on demand, when they are required. This also means that the system can purge any asset from disk when it is no longer required, which will save disk space. This class describes which assets are required, makes the request and reports progress, allows the app to specify how long during its execution that they are required.

 Assets are downloaded into the application container for sandboxed applications, or shared by all applications for non-sandboxed applications, and are made available via the standard NSFont lookup API.

 The request object itself is lightweight. You may create as many as you need, for example to request the same set of font descriptors in different components of your application.
*/
API_AVAILABLE(macos(10.13))
@interface NSFontAssetRequest : NSObject <NSProgressReporting>

- (instancetype)init NS_UNAVAILABLE;

/*
 An array of font descriptors is required argument to the init method. The fontDescriptors argument describes queries for downloadable font assets (list available at https://support.apple.com/en-us/HT206872#download). Any assets downloaded can be found using the standard NSFont lookup API once the request is completed.
 */
- (instancetype)initWithFontDescriptors:(NSArray<NSFontDescriptor *> *)fontDescriptors options:(NSFontAssetRequestOptions)options NS_DESIGNATED_INITIALIZER;

/*
 The downloaded font descriptors up to this point.
 */
@property (readonly, copy) NSArray<NSFontDescriptor *> *downloadedFontDescriptors;

/*
 Progress for the request. The progress object will be valid at initialization and begin updating after the -downloadFontAssetsWithCompletionHandler: method is called.
 */
@property (readonly, strong) NSProgress *progress;

/*
 Start the download process. completionHandler will be called whenever encounters any error, or when all download has been completed without error. completionHandler should return NO if you wish to stop downloading from that point, or YES if you wish to continue trying downloading the next font.
 */
- (void)downloadFontAssetsWithCompletionHandler:(BOOL (^)(NSError * _Nullable error))completionHandler;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
