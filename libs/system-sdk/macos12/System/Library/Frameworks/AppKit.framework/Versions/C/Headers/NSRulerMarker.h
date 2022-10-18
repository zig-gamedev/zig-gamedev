/*
        NSRulerMarker.h
        Application Kit
        Copyright (c) 1994-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSEvent, NSImage, NSRulerView;

@interface NSRulerMarker : NSObject <NSCopying, NSCoding>

/**************************** Initialization ****************************/

/* Designated initializer.  Location is expressed in the client view's bounds coordinates.  Location is the x position if the ruler is horizontal or the y position if the ruler is vertical.  The image will not be scaled or rotated.  The image origin indicates the point in the image that will be placed on the ruler's baseline at the given location and is expressed in the image's coordinate system.  NSRulerMarkers are movable but not removable by default.  A removable object should have its dimmed image set.
 */
- (instancetype)initWithRulerView:(NSRulerView *)ruler markerLocation:(CGFloat)location image:(NSImage *)image imageOrigin:(NSPoint)imageOrigin NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

/*********************** Query/Set basic attributes ***********************/

/* Returns the ruler.
 */
@property (readonly, weak) NSRulerView *ruler;


/* The location is set by the -init... method and the -setLocation: method.  Location is an x position for horizontal rulers or a y position for vertical rulers.  It is expressed in the client view's coordinate system.
 */
@property CGFloat markerLocation;

/* The image is what's drawn in the ruler.
 */
@property (strong) NSImage *image;

/* The image is drawn such that the image origin is on the baseline of the ruler at the object's location.
 */
@property NSPoint imageOrigin;

/* Objects are movable, but not removable by default.  Movable means the ruler object can be dragged by the user.  Removable means it can be deleted by the user.
 */
@property (getter=isMovable) BOOL movable;
@property (getter=isRemovable) BOOL removable;

/* Returns YES if the ruler object is currently being dragged.
 */
@property (getter=isDragging, readonly) BOOL dragging;

/* The representedObject of an NSRulerMarker is purely for the client's use.  It must be able to copy itself.  A represented object should be some small object.  The text object uses NSStrings for most ruler objects or NSTextTab objects for tab stops.
 */
@property (nullable, strong) id<NSCopying> representedObject;

/************************** Ruler facilities **************************/

/* Returns the rect that would be occupied by the object's image in the ruler's bounds coordinates.  This takes the flippedness of the ruler into account.
 */
@property (readonly) NSRect imageRectInRuler;

/* Returns the height or width (depending on the ruler's orientation) required in the ruler to display the object.
 */
@property (readonly) CGFloat thicknessRequiredInRuler;

/* Draws the object at it's current location.  Only rect needs to be drawn.
 */
- (void)drawRect:(NSRect)rect;

/* Handles the given mmouseDown event.  Performs a modal tracking loop until mouseUp allowing the object to be moved, if movable or removed, if removable.
 */
- (BOOL)trackMouse:(NSEvent *)mouseDownEvent adding:(BOOL)isAdding;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
