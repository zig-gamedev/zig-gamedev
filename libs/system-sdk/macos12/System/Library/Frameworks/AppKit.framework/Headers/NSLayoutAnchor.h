/*
 NSLayoutAnchor.h
 Application Kit
 Copyright (c) 2015-2021, Apple Inc.
 All rights reserved.
 */

#import <Foundation/NSGeometry.h>
#import <Foundation/NSArray.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN

#if !TARGET_OS_IPHONE

#if __cplusplus
#define NSLAYOUTANCHOR_EXTERN extern "C" __attribute((visibility("default")))
#else
#define NSLAYOUTANCHOR_EXTERN extern __attribute((visibility("default")))
#endif

@class NSLayoutConstraint, NSLayoutDimension;

/*
 An NSLayoutAnchor represents an edge or dimension of a layout item.  Its concrete subclasses allow concise creation of constraints.  The idea is that instead of invoking +[NSLayoutConstraint constraintWithItem: attribute: relatedBy: toItem: attribute: multiplier: constant:] directly, you can instead do something like this:
 
 [myView.topAnchor constraintEqualToAnchor:otherView.topAnchor constant:10];
 
 The -constraint* methods are available in multiple flavors to support use of different relations and omission of unused options.
 
 */
NSLAYOUTANCHOR_EXTERN API_AVAILABLE(macos(10.11), ios(9.0))
@interface NSLayoutAnchor<AnchorType> : NSObject
// NSLayoutAnchor conforms to <NSCopying> and <NSCoding> on macOS 10.12, iOS 10, and tvOS 10
#if (__MAC_OS_X_VERSION_MIN_REQUIRED >= 101200) || \
    (__IPHONE_OS_VERSION_MIN_REQUIRED >= 100000) || \
    (__TV_OS_VERSION_MIN_REQUIRED >= 100000)
        <NSCopying, NSCoding>
#endif

// These methods return an inactive constraint of the form thisAnchor = otherAnchor.
- (NSLayoutConstraint *)constraintEqualToAnchor:(NSLayoutAnchor<AnchorType> *)anchor __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintGreaterThanOrEqualToAnchor:(NSLayoutAnchor<AnchorType> *)anchor __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintLessThanOrEqualToAnchor:(NSLayoutAnchor<AnchorType> *)anchor __attribute__((warn_unused_result));

// These methods return an inactive constraint of the form thisAnchor = otherAnchor + constant.
- (NSLayoutConstraint *)constraintEqualToAnchor:(NSLayoutAnchor<AnchorType> *)anchor constant:(CGFloat)c __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintGreaterThanOrEqualToAnchor:(NSLayoutAnchor<AnchorType> *)anchor constant:(CGFloat)c __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintLessThanOrEqualToAnchor:(NSLayoutAnchor<AnchorType> *)anchor constant:(CGFloat)c __attribute__((warn_unused_result));

// These properties are for debugging only:
@property (readonly, copy) NSString *name API_AVAILABLE(macos(10.12)) API_UNAVAILABLE(ios, watchos, tvos);
@property (readonly, nullable, weak) id item API_AVAILABLE(macos(10.12)) API_UNAVAILABLE(ios, watchos, tvos);
@property (readonly) BOOL hasAmbiguousLayout API_AVAILABLE(macos(10.12)) API_UNAVAILABLE(ios, watchos, tvos);
@property (readonly) NSArray<NSLayoutConstraint *> *constraintsAffectingLayout API_AVAILABLE(macos(10.12)) API_UNAVAILABLE(ios, watchos, tvos);

@end

// Axis-specific subclasses for location anchors: top/bottom, leading/trailing, baseline, etc.
@class NSLayoutXAxisAnchor, NSLayoutYAxisAnchor, NSLayoutDimension;

NSLAYOUTANCHOR_EXTERN API_AVAILABLE(macos(10.11), ios(9.0))
@interface NSLayoutXAxisAnchor : NSLayoutAnchor<NSLayoutXAxisAnchor *>

// A composite anchor for creating constraints relating horizontal distances between locations.
- (NSLayoutDimension *)anchorWithOffsetToAnchor:(NSLayoutXAxisAnchor *)otherAnchor API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0));

/* Constraints of the form,
 receiver [= | ≥ | ≤] 'anchor' + 'multiplier' * system space,
 where the value of the system space is determined from information available from the anchors.
 The constraint affects how far the receiver will be positioned trailing 'anchor', per the effective user interface layout direction.
 */
- (NSLayoutConstraint *)constraintEqualToSystemSpacingAfterAnchor:(NSLayoutXAxisAnchor *)anchor multiplier:(CGFloat)multiplier __attribute__((warn_unused_result)) API_AVAILABLE(macos(11.0), ios(11.0), tvos(11.0));
- (NSLayoutConstraint *)constraintGreaterThanOrEqualToSystemSpacingAfterAnchor:(NSLayoutXAxisAnchor *)anchor multiplier:(CGFloat)multiplier __attribute__((warn_unused_result)) API_AVAILABLE(macos(11.0), ios(11.0), tvos(11.0));
- (NSLayoutConstraint *)constraintLessThanOrEqualToSystemSpacingAfterAnchor:(NSLayoutXAxisAnchor *)anchor multiplier:(CGFloat)multiplier __attribute__((warn_unused_result)) API_AVAILABLE(macos(11.0), ios(11.0), tvos(11.0));

@end

NSLAYOUTANCHOR_EXTERN API_AVAILABLE(macos(10.11), ios(9.0))
@interface NSLayoutYAxisAnchor : NSLayoutAnchor<NSLayoutYAxisAnchor *>

// A composite anchor for creating constraints relating vertical distances between locations.
- (NSLayoutDimension *)anchorWithOffsetToAnchor:(NSLayoutYAxisAnchor *)otherAnchor API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0));

/* Constraints of the form,
 receiver [= | ≥ | ≤] 'anchor' + 'multiplier' * system space,
 where the value of the system space is determined from information available from the anchors.
 The constraint affects how far the receiver will be positioned below 'anchor'.
 */
- (NSLayoutConstraint *)constraintEqualToSystemSpacingBelowAnchor:(NSLayoutYAxisAnchor *)anchor multiplier:(CGFloat)multiplier __attribute__((warn_unused_result)) API_AVAILABLE(macos(11.0), ios(11.0), tvos(11.0));
- (NSLayoutConstraint *)constraintGreaterThanOrEqualToSystemSpacingBelowAnchor:(NSLayoutYAxisAnchor *)anchor multiplier:(CGFloat)multiplier __attribute__((warn_unused_result)) API_AVAILABLE(macos(11.0), ios(11.0), tvos(11.0));
- (NSLayoutConstraint *)constraintLessThanOrEqualToSystemSpacingBelowAnchor:(NSLayoutYAxisAnchor *)anchor multiplier:(CGFloat)multiplier __attribute__((warn_unused_result)) API_AVAILABLE(macos(11.0), ios(11.0), tvos(11.0));

@end

// This layout anchor subclass is used for sizes (width & height).
NSLAYOUTANCHOR_EXTERN API_AVAILABLE(macos(10.11), ios(9.0))
@interface NSLayoutDimension : NSLayoutAnchor<NSLayoutDimension *>

// These methods return an inactive constraint of the form thisVariable = constant.
- (NSLayoutConstraint *)constraintEqualToConstant:(CGFloat)c __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintGreaterThanOrEqualToConstant:(CGFloat)c __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintLessThanOrEqualToConstant:(CGFloat)c __attribute__((warn_unused_result));

// These methods return an inactive constraint of the form thisAnchor = otherAnchor * multiplier.
- (NSLayoutConstraint *)constraintEqualToAnchor:(NSLayoutDimension *)anchor multiplier:(CGFloat)m __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintGreaterThanOrEqualToAnchor:(NSLayoutDimension *)anchor multiplier:(CGFloat)m __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintLessThanOrEqualToAnchor:(NSLayoutDimension *)anchor multiplier:(CGFloat)m __attribute__((warn_unused_result));

// These methods return an inactive constraint of the form thisAnchor = otherAnchor * multiplier + constant.
- (NSLayoutConstraint *)constraintEqualToAnchor:(NSLayoutDimension *)anchor multiplier:(CGFloat)m constant:(CGFloat)c __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintGreaterThanOrEqualToAnchor:(NSLayoutDimension *)anchor multiplier:(CGFloat)m constant:(CGFloat)c __attribute__((warn_unused_result));
- (NSLayoutConstraint *)constraintLessThanOrEqualToAnchor:(NSLayoutDimension *)anchor multiplier:(CGFloat)m constant:(CGFloat)c __attribute__((warn_unused_result));

@end


#endif // !TARGET_OS_IPHONE

NS_ASSUME_NONNULL_END
