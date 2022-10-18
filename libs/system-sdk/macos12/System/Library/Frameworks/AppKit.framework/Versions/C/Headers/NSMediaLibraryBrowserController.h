/* 
    NSMediaLibraryBrowserController.h
    Application Kit
    Copyright (c) 2012-2021, Apple Inc.
    All rights reserved.
*/

/*  NSMediaLibraryBrowserController

    An instance of an NSMediaLibraryBrowserController configures and displays a Media Library Browser Panel. A User can drag and drop media files from the Media Library Browser into views in their application.
*/

/*
    typedef NSMediaLibrary:

    NSMediaLibrary enums are masks used to configure a Media Library Browser to display specific types of media. Combined masks are not yet supported.  In other words, only one non zero mask value is supported at a time.  If masks are combined, the lowest mask value will be used.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_OPTIONS(NSUInteger, NSMediaLibrary) {
    NSMediaLibraryAudio = 1UL << 0,
    NSMediaLibraryImage = 1UL << 1,
    NSMediaLibraryMovie = 1UL << 2
} API_AVAILABLE(macos(10.9));

/*
    class:

    NSMediaLibraryBrowserController configures and displays a Media Library Browser Panel.
    
    discussion:
 
    NSMediaLibraryBrowserController is not an NSPanel!  It has NSPanel like methods to remotely control the Media Library Browser.  Clients have no direct programmatic access to the panel displaying the Media Library Browser.
*/

API_AVAILABLE(macos(10.9))
@interface NSMediaLibraryBrowserController : NSObject

@property (getter=isVisible) BOOL visible;
@property NSRect frame;
@property NSMediaLibrary mediaLibraries;

@property (class, readonly, strong) NSMediaLibraryBrowserController *sharedMediaLibraryBrowserController;
- (IBAction)togglePanel:(nullable id)sender;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
