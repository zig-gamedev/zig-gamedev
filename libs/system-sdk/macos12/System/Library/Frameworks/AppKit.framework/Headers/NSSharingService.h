/*
 NSSharingService.h
 Application Kit
 Copyright (c) 2011-2021, Apple Inc.
 All rights reserved.
 */

#import <AppKit/AppKitDefines.h>
#import <AppKit/NSPasteboard.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSItemProvider.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>

@class NSString, NSImage, NSView, NSError, NSWindow;
@class CKShare, CKContainer;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST


/**
 Built-in sharing services
 */
typedef NSString * NSSharingServiceName NS_TYPED_EXTENSIBLE_ENUM;


APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameComposeEmail API_AVAILABLE(macos(10.8));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameComposeMessage API_AVAILABLE(macos(10.8));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameSendViaAirDrop API_AVAILABLE(macos(10.8));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameAddToSafariReadingList API_AVAILABLE(macos(10.8));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameAddToIPhoto API_AVAILABLE(macos(10.8));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameAddToAperture API_AVAILABLE(macos(10.8));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameUseAsDesktopPicture API_AVAILABLE(macos(10.8));

// Formerly built-in sharing services. Deprecated as of 10.14

APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostOnFacebook API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostOnTwitter API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostOnSinaWeibo API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostOnTencentWeibo API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostOnLinkedIn API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameUseAsTwitterProfileImage API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameUseAsFacebookProfileImage API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameUseAsLinkedInProfileImage API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostImageOnFlickr API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostVideoOnVimeo API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostVideoOnYouku API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNamePostVideoOnTudou API_DEPRECATED("This service is no longer included with the system.", macos(10.8,10.14));

/**
 This service differs from other NSSharingServices in that it allows the user to establish a persistent sharing session for the specified items with potentially many participants, instead of sending a copy of the items. You can invoke this service with an NSItemProvider that has registered a CKShare & CKContainer via either -registerCloudKitShare:container: or -registerCloudKitShareWithPreparationHandler:. (Registering other types on the same provider to enable other sharing services is allowed.)
 
 When performed, this service gives the user the opportunity to invite participants and start sharing. If the content is already shared, the service instead allows the user to view or modify participation or stop sharing. To detect changes the service makes to the CKShare, implement -sharingService:didSaveShare: and -sharingService:didStopSharing:.
 */
APPKIT_EXTERN NSSharingServiceName const NSSharingServiceNameCloudSharing API_AVAILABLE(macos(10.12));


@protocol NSSharingServiceDelegate;

/**
 NSSharingService can be used to share items to different kinds of local and remote services. Items are objects which respond to the NSPasteboardWriting protocol, like NSURL, NSImage or NSString. If an NSURL is a file URL (point to a video for example), then the content of the file will be shared. If the URL is remote, then the URL itself will be shared.
 */
API_AVAILABLE(macos(10.8))
@interface NSSharingService : NSObject

@property (nullable, weak) id <NSSharingServiceDelegate> delegate;
@property (readonly, copy) NSString *title;
@property (readonly, strong) NSImage *image;
@property (nullable, readonly, strong) NSImage *alternateImage;

/**
 Title of the service in the Share menu. Can be modified.
 */
@property (copy) NSString *menuItemTitle API_AVAILABLE(macos(10.9));

#pragma mark -
// These properties are used for configuration of the service. They need to be set when the NSSharingService is created or in sharingService:willShareItems:


/// NSArray of NSString objects representing handles (example: email adresses)
@property (nullable, copy) NSArray<NSString *> *recipients API_AVAILABLE(macos(10.9));
@property (nullable, copy) NSString *subject API_AVAILABLE(macos(10.9));

// These read-only properties allow for querying of the shared content:

/// Message body as string
@property (nullable, readonly, copy) NSString *messageBody API_AVAILABLE(macos(10.9));

/// URL to access the post on Facebook, Twitter, Sina Weibo, etc. (also known as permalink)
@property (nullable, readonly, copy) NSURL *permanentLink API_AVAILABLE(macos(10.9));

/// Account name used for sending on Twitter or Sina Weibo
@property (nullable, readonly, copy) NSString *accountName API_AVAILABLE(macos(10.9));

/// NSArray of NSURL objects representing the files that were shared
@property (nullable, readonly, copy) NSArray<NSURL *> *attachmentFileURLs API_AVAILABLE(macos(10.9));

#pragma mark -

/**
 Returns a list of NSSharingServices which could share all the provided items together. sharingServicesForItems can be used to build a custom UI, or to populate a contextual NSMenu.
 The items represent the objects to be shared and must conform to the <NSPasteboardWriting> protocol or be an NSItemProvider or an NSDocument. (e.g. NSString, NSImage, NSURL, etc.)
 */
+ (NSArray<NSSharingService *> *)sharingServicesForItems:(NSArray *)items;


/**
 Returns an NSSharingService representing one of the built-in services.
 */
+ (nullable NSSharingService *)sharingServiceNamed:(NSSharingServiceName)serviceName;


/**
 Creates a custom NSSharingService object. Custom sharing services can be added to the NSSharingServicePicker with the sharingServicePicker:sharingServicesForItems:proposedSharingServices: delegate method.
 */
- (instancetype)initWithTitle:(NSString *)title image:(NSImage *)image alternateImage:(nullable NSImage *)alternateImage handler:(void (^)(void))block NS_DESIGNATED_INITIALIZER;

/**
 Use -initWithTitle:image:alternateImage:handler: instead
 */
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns whether a service can do something with all the provided items. This can be used to validate a custom UI such as a dedicated Twitter button. If items is nil, the method will return YES when the service is configured. Therefore you could call it once at launch time with nil items to check whether to display the button or not, and then with real items to enable and disable the button depending on the context or selection.

 The items represent the objects to be shared and must conform to the <NSPasteboardWriting> protocol or be an NSItemProvider or an NSDocument. (e.g. NSString, NSImage, NSURL, etc.)
 */
- (BOOL)canPerformWithItems:(nullable NSArray *)items;


/**
 Manually performs the service on the provided items. In most cases this will display a sharing window.

 The items represent the objects to be shared and must conform to the <NSPasteboardWriting> protocol or be an NSItemProvider or an NSDocument. (e.g. NSString, NSImage, NSURL, etc.)
 */
- (void)performWithItems:(NSArray *)items;

@end


/**
 Use the sharing scope to specify the nature of the things you are sharing.

 The sharing scope can be modified from the default value of NSSharingContentScopeItem by setting a different value in the out parameter in sharingService:sourceWindowForShareItems:sharingContentScope:.
 */
typedef NS_ENUM(NSInteger, NSSharingContentScope) {
    
    /// NSSharingContentScopeItem: use when sharing a clearly identified item, e.g. a file represented by its icon.
    NSSharingContentScopeItem,

    /// NSSharingContentScopePartial: use when sharing a portion of a more global content, e.g. part of a webpage
    NSSharingContentScopePartial,

    /// NSSharingContentScopeFull: use when sharing the whole content of the current document, e.g. the url of the webpage
    NSSharingContentScopeFull
}  API_AVAILABLE(macos(10.8));

@protocol NSSharingServiceDelegate <NSObject>
@optional
- (void)sharingService:(NSSharingService *)sharingService willShareItems:(NSArray *)items;
- (void)sharingService:(NSSharingService *)sharingService didFailToShareItems:(NSArray *)items error:(NSError *)error;
- (void)sharingService:(NSSharingService *)sharingService didShareItems:(NSArray *)items;


// The following methods are invoked when the service is performed and the sharing window pops up, to present a transition between the original items and the sharing window.

- (NSRect)sharingService:(NSSharingService *)sharingService sourceFrameOnScreenForShareItem:(id)item;

/**
 When non-nil, the image returned would be used for the transitioning animation. When nil, the transitioning animation is disabled.
 */
- (nullable NSImage *)sharingService:(NSSharingService *)sharingService transitionImageForShareItem:(id)item contentRect:(NSRect *)contentRect;

- (nullable NSWindow *)sharingService:(NSSharingService *)sharingService sourceWindowForShareItems:(NSArray *)items sharingContentScope:(NSSharingContentScope *)sharingContentScope;

/**
 The following method is invoked when the service is performed and wants to display its contents in a popover. The delegate should return the view that will act as the anchor of the popover, along with the target rectangle within the bounds of that view and preferred edge of that rectangle for the popover to appear. The delegate may also return nil, indicating that there is no anchoring view currently available, in which case the service may attempt to display the service via some other means.
 
 The service named NSSharingServiceNameCloudSharing prefers to display itself using a popover anchored to an "Add People" or "Share" button. If no such button is available or visible, return nil.
 */
- (nullable NSView *)anchoringViewForSharingService:(NSSharingService *)sharingService showRelativeToRect:(NSRect *)positioningRect preferredEdge:(NSRectEdge *)preferredEdge;

@end


typedef NS_OPTIONS(NSUInteger, NSCloudKitSharingServiceOptions) {
    /// Allow the user to configure the share with the standard set of options.
    NSCloudKitSharingServiceStandard = 0,

    /// The user is allowed to share publicly.
    NSCloudKitSharingServiceAllowPublic = 1 << 0,

    /// The user is allowed to share privately.
    NSCloudKitSharingServiceAllowPrivate = 1 << 1,

    /// The user is allowed to grant participants read-only permissions.
    NSCloudKitSharingServiceAllowReadOnly = 1 << 4,

    /// The user is allowed to grant participants read/write permissions.
    NSCloudKitSharingServiceAllowReadWrite = 1 << 5,
    
} API_AVAILABLE(macos(10.12));

@protocol NSCloudSharingServiceDelegate <NSSharingServiceDelegate>
@optional

/**
 When an NSSharingServiceNameCloudSharing sharing service is dismissed it will invoke this method on the delegate, with an error if there was any. If the delegate implements this method, NSSharingServiceNameCloudSharing will not send -sharingService:didFailToShareItems:error: or -sharingService:didShareItems:.
 */
- (void)sharingService:(NSSharingService *)sharingService didCompleteForItems:(NSArray *)items error:(nullable NSError *)error;


/**
 The options returned by this method describe how the user is allowed to configure the share: whether the share is public or private, and whether participants have read-only or read/write permissions. If this method is not implemented, NSCloudKitSharingServiceStandard is assumed.
 */
- (NSCloudKitSharingServiceOptions)optionsForSharingService:(NSSharingService *)cloudKitSharingService shareProvider:(NSItemProvider *)provider;


/**
 When an NSSharingServiceNameCloudSharing sharing service successfully saves modifications to the CKShare, it will invoke this method on the delegate with the last-known state of the CKShare on the server.
 */
- (void)sharingService:(NSSharingService *)sharingService didSaveShare:(CKShare *)share;

/**
 When an NSSharingServiceNameCloudSharing sharing service stops sharing it will delete the CKShare from the server, then invoke this method on the delegate with the last-known state of the CKShare.
 */
- (void)sharingService:(NSSharingService *)sharingService didStopSharing:(CKShare *)share;

@end



@interface NSItemProvider (NSCloudKitSharing)

/**
 Use this method when you want to share a collection of CKRecords but don't currently have a CKShare. When the preparationHandler is called, you should create a new CKShare with the appropriate root CKRecord. After ensuring the share and all records have been saved to the server, invoke the preparationCompletionHandler with either the resulting CKShare and its CKContainer, or an NSError if saving failed. Invoking the service with a CKShare registered with this method will prompt the user to start sharing.
 */
- (void)registerCloudKitShareWithPreparationHandler:(void (^_Nonnull)(void (^ _Nonnull preparationCompletionHandler)(CKShare * _Nullable, CKContainer * _Nullable, NSError * _Nullable)))preparationHandler API_AVAILABLE(macos(10.12));

/**
 Use this method when you have a CKShare that is already saved to the server. Invoking the service with a CKShare registerd with this method will allow the owner to make modifications to the share settings, or will allow a participant to view the share settings.
 */
- (void)registerCloudKitShare:(CKShare *)share container:(CKContainer *)container API_AVAILABLE(macos(10.12));

@end




@protocol NSSharingServicePickerDelegate;

API_AVAILABLE(macos(10.8))
@interface NSSharingServicePicker : NSObject 

@property (nullable, weak) id <NSSharingServicePickerDelegate> delegate;

/**
 Returns a new picker. The items represent the objects to be shared and must conform to the <NSPasteboardWriting> protocol or be an NSItemProvider or an NSDocument. (e.g. NSString, NSImage, NSURL, etc.)
 */
- (instancetype)initWithItems:(NSArray *)items NS_DESIGNATED_INITIALIZER;

/**
 Use initWithItems: instead.
 */
- (instancetype)init NS_UNAVAILABLE;

/**
 Shows the picker, populated with sharing services related to the instance items. When the user selects one of the sharing services, the sharing service will be performed. Note that this method must be called on mouseDown.
 */
- (void)showRelativeToRect:(NSRect)rect ofView:(NSView *)view preferredEdge:(NSRectEdge)preferredEdge;

@end


@protocol NSSharingServicePickerDelegate <NSObject>
@optional

/**
 Allows the delegate to customize exactly what appears in the sharing service picker by reordering or removing the services before the picker is presented. It's possible to add custom services by mutating the proposedSharingServices array and adding new NSSharingService instances:

 ```objc
 NSMutableArray *sharingServices = [proposedServices mutableCopy];
 NSSharingService * customService = [[NSSharingService alloc] initWithTitle:@"Service Title"
                                                   image:image alternateImage:alternateImage
                                                  handler:^{ [self doCustomServiceWithItems:items]; } ];
 [sharingServices addObject:customService];
 return [sharingServices autorelease];
 ```
 The items represent the objects to be shared and must conform to the <NSPasteboardWriting> protocol or be an NSItemProvider or an NSDocument. (e.g. NSString, NSImage, NSURL, etc.)
 */
- (NSArray<NSSharingService *> *)sharingServicePicker:(NSSharingServicePicker *)sharingServicePicker sharingServicesForItems:(NSArray *)items proposedSharingServices:(NSArray<NSSharingService *> *)proposedServices;


/**
 Sent when the user has selected a service and the picker is about to execute it.
 */
- (nullable id <NSSharingServiceDelegate>)sharingServicePicker:(NSSharingServicePicker *)sharingServicePicker delegateForSharingService:(NSSharingService *)sharingService;


/**
 Sent when the user has selected a service and before it is executed. Service will be nil if the picker was dismissed.
 */
- (void)sharingServicePicker:(NSSharingServicePicker *)sharingServicePicker didChooseSharingService:(nullable NSSharingService *)service;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

