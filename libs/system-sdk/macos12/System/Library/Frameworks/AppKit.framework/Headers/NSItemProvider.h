/*
	NSItemProvider.h
	Application Kit
	Copyright (c) 2014-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSGeometry.h>
#import <Foundation/NSItemProvider.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

// Item information support for Services-with-UI.
@interface NSItemProvider (NSItemSourceInfo)

// "sourceFrame" is the screen-space rectangle that the item’s representation occupies in the host application source window.  If the item is clipped (scrolled to only partially visible, for example), this is the screen rectangle that the fully displayed item would occupy if there was no clipping.  Modified by "containerFrame" (below), which defines an enclosing clip rectangle, this provides the frame the item’s representation will start from, if it is animated into the service's UI.
@property(readonly) NSRect sourceFrame API_AVAILABLE(macos(10.10));

// "containerFrame" is the screen-space rectangle of the item's enclosing clipping area.  The item will be understood to be fully visible, partially clipped, or not visible at all in the source window, according to whether this rectangle contains, intersects, or does not intersect the item's "sourceFrame".  The system may use this information to gradually reveal items as they are transitioned into a service UI.  Items that originate from the same view will typically have the same enclosing containerFrame, but making this per-item allows for gathering items from various enclosing source areas.  Defaults to NSZeroRect when an enclosing clip rectangle cannot be determined.  The system interprets that to mean that the item is fully visible (not clipped at all).
@property(readonly) NSRect containerFrame API_AVAILABLE(macos(10.10));

// "preferredPresentationSize" is the suggested ideal presentation size for the item, that the service UI is encouraged to display it at if possible.  This might be different from the size at which the item is depicted in the host app source window (sourceFrame.size).  For an image, for example, this will be image’s natural size, since if the service UI provides editing functionality, natural size is likely to be the most appropriate or convenient scale for editing.  If the item is of a type whose size can be automatically determined (image, video, PDF, etc.), this property will be automatically derived when the item's primary representation is loaded.  When this is NSZeroSize, assume that the item's sourceFrame gives an appropriate size for displaying the item in the service UI.
@property(readonly) NSSize preferredPresentationSize API_AVAILABLE(macos(10.10));

@end

// UTI subtypes for data detector types. They are sub-UTIs of com.apple.structured-text which conforms to public.plain-text
APPKIT_EXTERN NSString * const NSTypeIdentifierDateText NS_AVAILABLE(10_10, 8_0); // NSTextCheckingTypeDate, com.apple.structured-text.date
APPKIT_EXTERN NSString * const NSTypeIdentifierAddressText NS_AVAILABLE(10_10, 8_0); // NSTextCheckingTypeAddress, com.apple.structured-text.address
APPKIT_EXTERN NSString * const NSTypeIdentifierPhoneNumberText NS_AVAILABLE(10_10, 8_0); // NSTextCheckingTypePhoneNumber, com.apple.structured-text.telephone-number
APPKIT_EXTERN NSString * const NSTypeIdentifierTransitInformationText NS_AVAILABLE(10_10, 8_0); // NSTextCheckingTypeTransitInformation, com.apple.structured-text.transit-information

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
