/*
	NSLayoutGuide.h
	Application Kit
	Copyright (c) 2015-2021, Apple Inc.
	All rights reserved.
 */

#import <AppKit/NSView.h>
#import <AppKit/NSLayoutConstraint.h>
#import <AppKit/NSLayoutAnchor.h>
#import <AppKit/NSUserInterfaceItemIdentification.h>
#import <AppKit/AppKitDefines.h>

@class NSLayoutXAxisAnchor;
@class NSLayoutYAxisAnchor;
@class NSLayoutDimension;
@class NSLayoutConstraint;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

/* NSLayoutGuides will not show up in the view hierarchy, but may be used as items in an NSLayoutConstraint and represent a rectangle in the layout engine.
 */
API_AVAILABLE(macos(10.11))
@interface NSLayoutGuide : NSObject <NSCoding, NSUserInterfaceItemIdentification>

// The frame of the NSLayoutGuide in its owningView's coordinate system. Valid by the time the owningView is laid out.
@property (readonly) NSRect frame;

// A guide must be added to a view via -[NSView -addLayoutGuide:] before being used in a constraint.  The owningView setter is intended for subclasses to override, and should only be used directly by -addLayoutGuide and -removeLayoutGuide.
@property (weak, nullable) NSView *owningView;

// For ease of debugging.  'NS' prefix is reserved for system-created layout guides.
@property (copy) NSUserInterfaceItemIdentifier identifier;


/* 
 These properties aid concise creation of constraints.  Layout guides can be constrained using simple code like the following:
 [view.topAnchor constraintEqualTo:someLayoutGuide.topAnchor constant:10];
 
 See NSLayoutAnchor.h for more details.
 */
@property (readonly, strong) NSLayoutXAxisAnchor *leadingAnchor;
@property (readonly, strong) NSLayoutXAxisAnchor *trailingAnchor;
@property (readonly, strong) NSLayoutXAxisAnchor *leftAnchor;
@property (readonly, strong) NSLayoutXAxisAnchor *rightAnchor;
@property (readonly, strong) NSLayoutYAxisAnchor *topAnchor;
@property (readonly, strong) NSLayoutYAxisAnchor *bottomAnchor;
@property (readonly, strong) NSLayoutDimension *widthAnchor;
@property (readonly, strong) NSLayoutDimension *heightAnchor;
@property (readonly, strong) NSLayoutXAxisAnchor *centerXAnchor;
@property (readonly, strong) NSLayoutYAxisAnchor *centerYAnchor;

#if !TARGET_OS_IPHONE
// For debugging purposes:
@property (readonly) BOOL hasAmbiguousLayout API_AVAILABLE(macos(10.12));
- (NSArray<NSLayoutConstraint *> *)constraintsAffectingLayoutForOrientation:(NSLayoutConstraintOrientation)orientation API_AVAILABLE(macos(10.12));
#endif

@end


/* A layout guide can be used in place of a view for layout purposes.
 */
@interface NSView (NSLayoutGuideSupport)

- (void)addLayoutGuide:(NSLayoutGuide *)guide API_AVAILABLE(macos(10.11));
- (void)removeLayoutGuide:(NSLayoutGuide *)guide API_AVAILABLE(macos(10.11));

@property (readonly, copy) NSArray<NSLayoutGuide *> *layoutGuides API_AVAILABLE(macos(10.11)); // This array may contain guides owned by the system, and the ordering is not guaranteed.  Please be careful.

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
