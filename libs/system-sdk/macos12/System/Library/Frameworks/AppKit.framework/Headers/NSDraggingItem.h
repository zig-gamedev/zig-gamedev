/*
	NSDraggingItem.h
	Application Kit
	Copyright (c) 2010-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>

@protocol NSPasteboardWriting;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString;

typedef NSString * NSDraggingImageComponentKey NS_TYPED_EXTENSIBLE_ENUM;
APPKIT_EXTERN NSDraggingImageComponentKey const NSDraggingImageComponentIconKey API_AVAILABLE(macos(10.7));
APPKIT_EXTERN NSDraggingImageComponentKey const NSDraggingImageComponentLabelKey API_AVAILABLE(macos(10.7));

API_AVAILABLE(macos(10.7))
@interface NSDraggingImageComponent : NSObject
+ (NSDraggingImageComponent *)draggingImageComponentWithKey:(NSDraggingImageComponentKey)key;

/* Designated initializer */
- (instancetype)initWithKey:(NSDraggingImageComponentKey)key NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

/* key must be unique for each component in an NSDraggingItem. You can create your own named components, but the following names have special meaning. NSDraggingImageComponentIconKey is an image of the item being dragged. NSDraggingImageComponentLabelKey represents a textual label associate with the item, for example, a file name.
*/
@property (copy) NSDraggingImageComponentKey key;

/* An object providing the image contents of the component, typically you set an NSImage, but may be anything CALayer accepts.
*/
@property (nullable, strong) id contents;

/* The coordinate space is the bounds of the parent NSDraggingItem. That is, {{0,0}, {draggingFrame.size.width, draggingFrame.size.height}} Note: NSDraggingItem does not clip its components.
*/
@property NSRect frame;

@end

API_AVAILABLE(macos(10.7))
@interface NSDraggingItem : NSObject

/* The designated initializer. When creating an NSDraggingItem the pasteboardWriter must implement the NSPasteboardWriting protocol.
*/
- (instancetype)initWithPasteboardWriter:(id <NSPasteboardWriting>)pasteboardWriter NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

/* When you create an NSDraggingItem, item is the pasteboardWriter passed to initWithPasteboardWriter. However, when enumerating dragging items in an NSDraggingSession or NSDraggingInfo object, item is not the original pasteboardWriter. It is an instance of one of the classes provided to the enumeration method.
*/
@property (readonly, strong) id item;

/* The dragging frame that provides the spatial relationship between NSDraggingItems in the NSDraggingFormationNone. Note: The exact coordinate space of this rect depends on where it is used. See NSDraggingSession and NSDraggingInfo. 
*/
@property NSRect draggingFrame;

/* The dragging image is the composite of an array of NSDraggingImageComponents. The dragging image components may not be set directly. Instead, provide a block to generate the components and the block will be called if necessary. The block may be set to nil, meaning that this drag item has no image. Generally, only dragging destinations do this, and only if there is at least one valid item in the drop, and this is not it. The components are composited in painting order. That is, each component in the array is painted on top of the previous components in the array.
*/
@property (nullable, copy) NSArray<NSDraggingImageComponent *> * _Nonnull (^imageComponentsProvider)(void);

/* Alternate single image component setter. This method simplifies modifiying the components of an NSDraggingItem when there is only one component. This method will set the draggingFrame and imageComponentsProvider properties. frame is in the same coordinate space that the draggingFrame property is. To hide this item, set contents to nil.
*/
- (void)setDraggingFrame:(NSRect)frame contents:(nullable id)contents;

/* An array of NSDraggingImageComponents that are used to create the drag image. Note: the array contains copies of the components. Changes made to these copies are not reflected in the drag. If needed, the imageComponentsProvider block is called to generate the image components.
*/
@property (nullable, readonly, copy) NSArray<NSDraggingImageComponent *> *imageComponents;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
