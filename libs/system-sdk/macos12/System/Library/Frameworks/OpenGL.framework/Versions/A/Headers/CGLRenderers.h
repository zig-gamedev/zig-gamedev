/*
	Copyright:	(c) 1999-2013 Apple Inc. All rights reserved.
*/

#ifndef _CGLRENDERERS_H
#define _CGLRENDERERS_H

#include <OpenGL/OpenGLAvailability.h>

/*
** Renderer ID numbers
*/
enum {
    kCGLRendererGenericID        OPENGL_ENUM_DEPRECATED(10_0, 10_5) = 0x00020200,
    kCGLRendererGenericFloatID   OPENGL_ENUM_AVAILABLE(10_3)        = 0x00020400, // Apple Software Renderer
    kCGLRendererAppleSWID                                           = 0x00020600,

    kCGLRendererATIRage128ID     OPENGL_ENUM_DEPRECATED(10_0, 10_5) = 0x00021000,
    kCGLRendererATIRadeonID      OPENGL_ENUM_DEPRECATED(10_0, 10_6) = 0x00021200,
    kCGLRendererATIRageProID     OPENGL_ENUM_DEPRECATED(10_0, 10_6) = 0x00021400,
    kCGLRendererATIRadeon8500ID  OPENGL_ENUM_DEPRECATED(10_2, 10_6) = 0x00021600,
    kCGLRendererATIRadeon9700ID  OPENGL_ENUM_DEPRECATED(10_2, 10_6) = 0x00021800,
    kCGLRendererATIRadeonX1000ID OPENGL_ENUM_DEPRECATED(10_4, 10_8) = 0x00021900, // Radeon X1xxx
    kCGLRendererATIRadeonX2000ID OPENGL_ENUM_AVAILABLE(10_4)        = 0x00021A00, // Radeon HD 2xxx and 4xxx
    kCGLRendererATIRadeonX3000ID OPENGL_ENUM_AVAILABLE(10_6)        = 0x00021B00, // Radeon HD 5xxx and 6xxx
    kCGLRendererATIRadeonX4000ID OPENGL_ENUM_AVAILABLE(10_8)        = 0x00021C00, // Radeon HD 7xxx

    kCGLRendererGeForce2MXID     OPENGL_ENUM_DEPRECATED(10_0, 10_6) = 0x00022000, // GeForce 2MX and 4MX
    kCGLRendererGeForce3ID       OPENGL_ENUM_DEPRECATED(10_0, 10_6) = 0x00022200, // GeForce 3 and 4Ti
    kCGLRendererGeForceFXID      OPENGL_ENUM_DEPRECATED(10_2, 10_8) = 0x00022400, // GeForce 5xxx, 6xxx and 7xxx, and
                                                                                  // Quadro FX 4500
    kCGLRendererGeForce8xxxID    OPENGL_ENUM_AVAILABLE(10_4)        = 0x00022600, // GeForce 8xxx, 9xxx, 1xx, 2xx, and 3xx, and
                                                                                  // Quadro 4800
    kCGLRendererGeForceID        OPENGL_ENUM_AVAILABLE(10_9)        = 0x00022700, // GeForce 6xx, and
                                                                                  // Quadro 4000, K5000

    kCGLRendererVTBladeXP2ID     OPENGL_ENUM_DEPRECATED(10_3, 10_6) = 0x00023000,

    kCGLRendererIntel900ID       OPENGL_ENUM_DEPRECATED(10_4, 10_8) = 0x00024000, // GMA 950
    kCGLRendererIntelX3100ID     OPENGL_ENUM_DEPRECATED(10_5, 10_8) = 0x00024200,
    kCGLRendererIntelHDID        OPENGL_ENUM_AVAILABLE(10_6)        = 0x00024300, // HD Graphics 3000
    kCGLRendererIntelHD4000ID    OPENGL_ENUM_AVAILABLE(10_7)        = 0x00024400,
    kCGLRendererIntelHD5000ID    OPENGL_ENUM_AVAILABLE(10_8)        = 0x00024500, // Iris

    kCGLRendererMesa3DFXID       OPENGL_ENUM_DEPRECATED(10_0, 10_6) = 0x00040000,
};

/*
** kCGLRendererIDMatchingMask gives the bits that are useful for matching a
** renderer ID (as returned by CGLDescribePixelFormat or CGLDescribeRenderer)
** with the assigned values above.  There should be no bits set in the assigned
** renderer ID's that are not set in this mask.
*/
#define kCGLRendererIDMatchingMask   0x00FE7F00


#endif /* _CGLRENDERERS_H */
