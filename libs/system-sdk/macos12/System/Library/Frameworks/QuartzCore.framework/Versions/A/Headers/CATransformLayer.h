/* CoreAnimation - CATransformLayer.h

   Copyright (c) 2006-2021, Apple Inc.
   All rights reserved. */

#import <QuartzCore/CALayer.h>

NS_ASSUME_NONNULL_BEGIN

/* "Transform" layers are used to create true 3D layer hierarchies.
 *
 * Unlike normal layers, transform layers do not project (i.e. flatten)
 * their sublayers into the plane at Z=0. However due to this neither
 * do they support many features of the 2D compositing model:
 *
 * - only their sublayers are rendered (i.e. no background, contents,
 *   border)
 *
 * - filters, backgroundFilters, compositingFilter, mask, masksToBounds
 *   and shadow related properties are ignored (they all assume 2D
 *   image processing of the projected layer)
 *
 * - opacity is applied to each sublayer individually, i.e. the transform
 *   layer does not form a compositing group.
 *
 * Also, the -hitTest: method should never be called on transform
 * layers (they do not have a 2D coordinate space into which to map the
 * supplied point.) CALayer will pass over transform layers directly to
 * their sublayers, applying the effects of the transform layer's
 * geometry when hit-testing each sublayer. */

API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0))
@interface CATransformLayer : CALayer
@end

NS_ASSUME_NONNULL_END
