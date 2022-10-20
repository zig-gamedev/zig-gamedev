/* CoreAnimation - CAEmitterLayer.h

   Copyright (c) 2007-2021, Apple Inc.
   All rights reserved. */

/* Particle emitter layer.
 *
 * Each emitter has an array of cells, the cells define how particles
 * are emitted and rendered by the layer.
 *
 * Particle system is affected by layer's timing. The simulation starts
 * at layer's beginTime.
 *
 * The particles are drawn above the backgroundColor and border of the
 * layer. */

#import <QuartzCore/CALayer.h>

typedef NSString * CAEmitterLayerEmitterShape NS_TYPED_ENUM;
typedef NSString * CAEmitterLayerEmitterMode NS_TYPED_ENUM;
typedef NSString * CAEmitterLayerRenderMode NS_TYPED_ENUM;

@class CAEmitterCell;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0))
@interface CAEmitterLayer : CALayer

/* The array of emitter cells attached to the layer. Each object must
 * have the CAEmitterCell class. */

@property(nullable, copy) NSArray<CAEmitterCell *> *emitterCells;

/* The birth rate of each cell is multiplied by this number to give the
 * actual number of particles created every second. Default value is one.
 * Animatable. */

@property float birthRate;

/* The cell lifetime range is multiplied by this value when particles are
 * created. Defaults to one. Animatable. */

@property float lifetime;

/* The center of the emission shape. Defaults to (0, 0, 0). Animatable. */

@property CGPoint emitterPosition;
@property CGFloat emitterZPosition;

/* The size of the emission shape. Defaults to (0, 0, 0). Animatable.
 * Depending on the `emitterShape' property some of the values may be
 * ignored. */

@property CGSize emitterSize;
@property CGFloat emitterDepth;

/* A string defining the type of emission shape used. Current options are:
 * `point' (the default), `line', `rectangle', `circle', `cuboid' and
 * `sphere'. */

@property(copy) CAEmitterLayerEmitterShape emitterShape;

/* A string defining how particles are created relative to the emission
 * shape. Current options are `points', `outline', `surface' and
 * `volume' (the default). */

@property(copy) CAEmitterLayerEmitterMode emitterMode;

/* A string defining how particles are composited into the layer's
 * image. Current options are `unordered' (the default), `oldestFirst',
 * `oldestLast', `backToFront' (i.e. sorted into Z order) and
 * `additive'. The first four use source-over compositing, the last
 * uses additive compositing. */

@property(copy) CAEmitterLayerRenderMode renderMode;

/* When true the particles are rendered as if they directly inhabit the
 * three dimensional coordinate space of the layer's superlayer, rather
 * than being flattened into the layer's plane first. Defaults to NO.
 * If true, the effect of the `filters', `backgroundFilters' and shadow-
 * related properties of the layer is undefined. */

@property BOOL preservesDepth;

/* Multiplies the cell-defined particle velocity. Defaults to one.
 * Animatable. */

@property float velocity;

/* Multiplies the cell-defined particle scale. Defaults to one. Animatable. */

@property float scale;

/* Multiplies the cell-defined particle spin. Defaults to one. Animatable. */

@property float spin;

/* The seed used to initialize the random number generator. Defaults to
 * zero. Each layer has its own RNG state. For properties with a mean M
 * and a range R, random values of the properties are uniformly
 * distributed in the interval [M - R/2, M + R/2]. */

@property unsigned int seed;

@end

/** `emitterShape' values. **/

CA_EXTERN CAEmitterLayerEmitterShape const kCAEmitterLayerPoint
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerEmitterShape const kCAEmitterLayerLine
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerEmitterShape const kCAEmitterLayerRectangle
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerEmitterShape const kCAEmitterLayerCuboid
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerEmitterShape const kCAEmitterLayerCircle
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerEmitterShape const kCAEmitterLayerSphere
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));

/** `emitterMode' values. **/

CA_EXTERN CAEmitterLayerEmitterMode const kCAEmitterLayerPoints
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerEmitterMode const kCAEmitterLayerOutline
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerEmitterMode const kCAEmitterLayerSurface
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerEmitterMode const kCAEmitterLayerVolume
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));

/** `renderMode' values. **/

CA_EXTERN CAEmitterLayerRenderMode const kCAEmitterLayerUnordered
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerRenderMode const kCAEmitterLayerOldestFirst
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerRenderMode const kCAEmitterLayerOldestLast
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerRenderMode const kCAEmitterLayerBackToFront
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAEmitterLayerRenderMode const kCAEmitterLayerAdditive
    API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));

NS_ASSUME_NONNULL_END
