/*
    NSRulerView.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSView.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSScrollView, NSRulerMarker;

typedef NS_ENUM(NSUInteger, NSRulerOrientation) {
    NSHorizontalRuler,
    NSVerticalRuler
};

typedef NSString * NSRulerViewUnitName NS_TYPED_EXTENSIBLE_ENUM;
APPKIT_EXTERN NSRulerViewUnitName const NSRulerViewUnitInches API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSRulerViewUnitName const NSRulerViewUnitCentimeters API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSRulerViewUnitName const NSRulerViewUnitPoints API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSRulerViewUnitName const NSRulerViewUnitPicas API_AVAILABLE(macos(10.13));

@interface NSRulerView : NSView

/************************* Registering new units *************************/

+ (void)registerUnitWithName:(NSRulerViewUnitName)unitName abbreviation:(NSString *)abbreviation unitToPointsConversionFactor:(CGFloat)conversionFactor stepUpCycle:(NSArray<NSNumber *> *)stepUpCycle stepDownCycle:(NSArray<NSNumber *> *)stepDownCycle;

/**************************** Initialization ****************************/

- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithScrollView:(nullable NSScrollView *)scrollView orientation:(NSRulerOrientation)orientation NS_DESIGNATED_INITIALIZER;
    // The designated initializer.  A ruler's size is controlled by its NSScrollView. initWithFrame: is overridden to call this.  The view is initialized with an unrealistically small default frame which will be reset in due time by the NSScrollView.

/**************************** Basic setup ****************************/

@property (nullable, weak) NSScrollView *scrollView;
    // A ruler uses its scrollView to finds its document view to see whether it should be flipped.  The set method is generally called only by the scroll view itself.  You should not have to set this.

@property NSRulerOrientation orientation;
    // Either NSHorizontalRuler or NSVerticalRuler.  The set method is generally called by the ruler's scroll view.  You should not have to set this.

/**************************** Ruler geometry ****************************/

@property (readonly) CGFloat baselineLocation;
    // Returns the location of the baseline.  The location is a y position for horizontal rulers and an x position for vertical ones.  The value is based on the sizes of the various areas of the ruler, some of which can be set below.

@property (readonly) CGFloat requiredThickness;
    // The required height for a horizontal ruler or the required width for a vertical ruler.  Used by the scrollview when tiling.  The value is based on the sizes of the various areas of the ruler, some of which can be set below.

@property CGFloat ruleThickness;
    // The rule size is the height of the area used to draw the ruler hash marks in a horizontal ruler or the width of that area in a vertical ruler.  This value will be interpretted in the coordinate system of the ruler's superview (like the ruler's frame).

@property CGFloat reservedThicknessForMarkers;
    // This indicates to the ruler how much room it should leave for objects which sit above or to the left of the rule (such as tab stops, margins, etc...).  Default is 15.0.  If you expect that no view in your document view will ever put objects on the ruler, you can set this to 0.0.  If objects are ever set on the ruler, and not enough space was reserved for them, the ruler grows itself and retiles the scroll view.

@property CGFloat reservedThicknessForAccessoryView;
    // This indicates to the ruler how much room it should leave for the accessory view.  Default is 0.0.  If you expect that a view in your document view will put an accessory view in the ruler, you can set this to make room fror it from the start.  If an accessory view is ever set for the ruler, and space was not reserved for it or it is bigger than the space reserved, the ruler grows the reserved space to be big enough and retiles the scroll view.  If you know that several different accessory views will be used it is best to set this to the height of the tallest one for horizontal rulers or the width of the widest one for vertical rulers.  If the reserved thickness is larger than an actual accessory view set into the ruler, the accessory view is centered in the thickness.

/**************************** Rule configuration ****************************/

@property (copy) NSRulerViewUnitName measurementUnits;
    // The units of the ruler start out with the user's preferred measurement.  They can be changed if desired.  The units set must be registered with the ruler factory.  By default Inches, Centimeters, Picas, and Points are supported.

@property CGFloat originOffset;
    // How far to offset the ruler's zero hash mark (and label) from the document view's bounds origin.

/**************************** Client view setup ****************************/

@property (nullable, weak) NSView *clientView;

- (void)addMarker:(NSRulerMarker *)marker;
- (void)removeMarker:(NSRulerMarker *)marker;
@property (nullable, copy) NSArray<NSRulerMarker *> *markers;
    // Ruler objects sit on the ruler at a specific location.  They can be manipulatable by the user, and if they are manipulated the current client is informed of the change.  Once a bunch of objects has been set, objects can be added or removed individually.

- (BOOL)trackMarker:(NSRulerMarker *)marker withMouseEvent:(NSEvent *)event;
    // Given a ruler object and a mouse event (either MouseDown or MouseDragged) this will take over the tracking of the mouse until MouseUp.  While tracking it will snap to the baseline of the ruler when it gets within a certain distance.  At that point it becomes stuck to the baseline (this may happen immediately) and won't come off below (or to the right of) the ruler.  It may still be dragged off the baseline upward (or leftward) if its removable.  It is strongly recommended that any ruler object that acan be added to the ruler should be removable as well and it must be movable.  When the mouse goes up, if the object is currently stuck to the baseline it is added to the ruler (and the client object is notified), if its not stuck on the baseline, it is not added.

@property (nullable, strong) NSView *accessoryView;
    // A rulers accessory view is drawn below or to the right of the rule.  It can contain arbitrary controls.

- (void)moveRulerlineFromLocation:(CGFloat)oldLocation toLocation:(CGFloat)newLocation;
    // This method can be used to draw temporary lines in the rule.  NSRulerMarkers use this during dragging to draw the part of the line they draw from themselves across the scroll views contents which appears in the rule area.  You can use this method to draw ticks in the ruler (for example) to show the position or extent of an object while it is being dragged in the document.  oldLocation is the last position that the line was at or -1.0 if this is the first time it is being drawn.  newLocation is where the line should be or -1.0 if you are just trying to get an old line erased. 

/*********************** Drawing and hash invalidation ***********************/

- (void)invalidateHashMarks;
    // Forces recalculation of the hash mark dictionary next time any display is done.  You should not generally have to call this method.

- (void)drawHashMarksAndLabelsInRect:(NSRect)rect;
    // You should never need to call this, but you might want to override it.  This method is called by -drawRect: to draw the actual hash marks and labels on the ruler.

- (void)drawMarkersInRect:(NSRect)rect;
    // You should never need to call this, but you might want to override it.  This method is called by -drawRect: to draw the ruler objects on the ruler.  If you want to do something different you can override this.

/**************************** Key overrides ****************************/

@property (getter=isFlipped, readonly) BOOL flipped;
    // A vertical ruler mimics the flippedness of the documentView of it's scrollView.  A horizontal ruler is always flipped (for ease of drawing).

@end

// This informal protocol defines the communication between a ruler and its client view during mouse tracking.
@interface NSView (NSRulerMarkerClientViewDelegation)

- (BOOL)rulerView:(NSRulerView *)ruler shouldMoveMarker:(NSRulerMarker *)marker;
    // This is sent when a drag operation is just beginning for a ruler marker already on the ruler.  If the ruler object should be allowed to either move or remove, return YES.  If you return NO, all tracking is abandoned and nothing happens.

- (CGFloat)rulerView:(NSRulerView *)ruler willMoveMarker:(NSRulerMarker *)marker toLocation:(CGFloat)location;
    // This is sent continuously while the mouse is being dragged.  The client can constrian the movement by returning a different location.  Receipt of one or more of these messages does not guarantee that the corresponding "did" method will be called.  Only movable objects will send this message.

- (void)rulerView:(NSRulerView *)ruler didMoveMarker:(NSRulerMarker *)marker;
    // This is called if the NSRulerMarker actually ended up with a different location than it started with after the drag completes.  It is not called if the object gets removed, or if the object gets dragged around and dropped right where it was.  Only movable objects will send this message.

- (BOOL)rulerView:(NSRulerView *)ruler shouldRemoveMarker:(NSRulerMarker *)marker;
    // This is sent each time the object is dragged off the baseline enough that if it were dropped, it would be removed.  It can be sent multiple times in the course of a drag.  Return YES if it's OK to remove the object, NO if not.  Receipt of this message does not guarantee that the corresponding "did" method will be called.  Only removable objects will send this message.

- (void)rulerView:(NSRulerView *)ruler didRemoveMarker:(NSRulerMarker *)marker;
    // This is sent if the object is actually removed.  The object has been removed from the ruler when this message is sent.

- (BOOL)rulerView:(NSRulerView *)ruler shouldAddMarker:(NSRulerMarker *)marker;
    // This is sent when a drag operation is just beginning for a ruler marker that is being added.  If the ruler object should be allowed to add, return YES.  If you return NO, all tracking is abandoned and nothing happens.

- (CGFloat)rulerView:(NSRulerView *)ruler willAddMarker:(NSRulerMarker *)marker atLocation:(CGFloat)location;
    // This is sent continuously while the mouse is being dragged during an add operation and the new object is stuck on the baseline.  The client can constrian the movement by returning a different location.  Receipt of one or more of these messages does not guarantee that the corresponding "did" method will be called.  Any object sending these messages is not yet added to the ruler it is being dragged on.

- (void)rulerView:(NSRulerView *)ruler didAddMarker:(NSRulerMarker *)marker;
    // This is sent after the object has been added to the ruler.

- (void)rulerView:(NSRulerView *)ruler handleMouseDown:(NSEvent *)event;
    // This is sent when the user clicks in the rule area of the ruler.  The "rule" area is the area below the baseline where the hash marks and labels are drawn.  A common use for this method would be to make clicking in the rule be a shortcut for adding the most common type of ruler object for a particuar client.  NSTextView will use this to insert a new left tab (as a short cut to dragging one out of the well in the accessory view).

- (void)rulerView:(NSRulerView *)ruler willSetClientView:(NSView *)newClient;
    // This is sent to the existing client before it is replaced by the new client.  The existing client can catch this to clean up any cached state it keeps while it is the client of a ruler.

- (CGFloat)rulerView:(NSRulerView *)ruler locationForPoint:(NSPoint)point API_AVAILABLE(macos(10.7));
- (NSPoint)rulerView:(NSRulerView *)ruler pointForLocation:(CGFloat)point API_AVAILABLE(macos(10.7));
    // This additional mapping allows mapping between location and point for clients with rotated coordinate system (e.g., vertical text view).

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
