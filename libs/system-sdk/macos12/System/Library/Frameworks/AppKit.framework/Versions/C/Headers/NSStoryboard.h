/*
        NSStoryboard.h
        Application Kit
        Copyright (c) 2013-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NSString * NSStoryboardName NS_SWIFT_BRIDGED_TYPEDEF;
typedef NSString * NSStoryboardSceneIdentifier NS_SWIFT_BRIDGED_TYPEDEF;

typedef id _Nullable (^NSStoryboardControllerCreator)(NSCoder *coder);

API_AVAILABLE(macos(10.10))
@interface NSStoryboard : NSObject

/* The main UI storyboard for the application, specified by the "NSMainStoryboardFile" key in its Info.plist */
@property (class, readonly, strong, nullable) NSStoryboard *mainStoryboard API_AVAILABLE(macos(10.13));

/* This method instantiates a storyboard to represent the file with 'name' in the given bundle. The bundle argument is used to resolve resource references, typically to images, in the archived controllers. The bundle argument can be nil indicating that the main bundle should be used. */
+ (instancetype)storyboardWithName:(NSStoryboardName)name bundle:(nullable NSBundle *)storyboardBundleOrNil;

/* Every storyboard may have an initial view or window controller. Each invocation of this method instantiates an instance of the initial controller. The developer chooses the initial controller in an inspector within Interface Builder. */
- (nullable id)instantiateInitialController;

/* Every storyboard may have an initial view or window controller. Each invocation of this method instantiates an instance of the initial controller. The developer chooses the initial controller in an inspector within Interface Builder. A creator can be specified for controller instantiation, giving the developer a chance to call an initalizer that takes an NSCoder and additional arguments. If the block returns nil, the initial view controller will be created using the standard initWithCoder: initializer. */
- (nullable id)instantiateInitialControllerWithCreator:(nullable NS_NOESCAPE NSStoryboardControllerCreator)block API_AVAILABLE(macos(10.15));

/* Controllers in Interface Builder have an optional identifier within the storyboard. That identifier is set from the inspector in Interface Builder. The identifiers are optional, meaning that the developer only has to assign an identifier to a controller if they want to be able to manually invoke this method from code. */
- (id)instantiateControllerWithIdentifier:(NSStoryboardSceneIdentifier)identifier;

/* Controllers in Interface Builder have an optional identifier within the storyboard. That identifier is set from the inspector in Interface Builder. The identifiers are optional, meaning that the developer only has to assign an identifier to a controller if they want to be able to manually invoke this method from code. A creator can be specified for controller instantiation, giving the developer a chance to call an initalizer that takes an NSCoder and additional arguments. If the block returns nil, the view controller will be created using the standard initWithCoder: initializer. */
- (id)instantiateControllerWithIdentifier:(NSStoryboardSceneIdentifier)identifier creator:(nullable NS_NOESCAPE NSStoryboardControllerCreator)block API_AVAILABLE(macos(10.15));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
