/*
	Copyright:	(c) 1999-2012 Apple Inc. All rights reserved.
*/

#ifndef _GLUMACRO_H
#define _GLUMACRO_H

#include <OpenGL/CGLMacro.h>
#include <OpenGL/gluContext.h>

/********************* GLU Macros *********************/

#define gluBuild1DMipmaps(target, internalFormat, width, format, type, data) \
	gluBuild1DMipmapsCTX(cgl_ctx, target, internalFormat, width, format, type, data)

#define gluBuild2DMipmaps(target, internalFormat, width, height, format, type, data) \
	gluBuild2DMipmapsCTX(cgl_ctx, target, internalFormat, width, height, format, type, data)

#define gluBuild3DMipmaps(target, internalFormat, width, height, depth, format, type, data) \
	gluBuild3DMipmapsCTX(cgl_ctx, target, internalFormat, width, height, depth, format, type, data)

#define gluBuild1DMipmapLevels(target, internalFormat, width, format, type, level, base, max, data) \
	gluBuild1DMipmapLevelsCTX(cgl_ctx, target, internalFormat, width, format, type, level, base, max, data)

#define gluBuild2DMipmapLevels(target, internalFormat, width, height, format, type, level, base, max, data) \
	gluBuild2DMipmapLevelsCTX(cgl_ctx, target, internalFormat, width, height, format, type, level, base, max, data)

#define gluBuild3DMipmapLevels(target, internalFormat, width, height, depth, format, type, level, base, max, data) \
	gluBuild3DMipmapLevelsCTX(cgl_ctx, target, internalFormat, width, height, depth, format, type, level, base, max, data)

#define gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ) \
	gluLookAtCTX(cgl_ctx, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)

#define gluNewNurbsRenderer() \
	gluNewNurbsRendererCTX(cgl_ctx)

#define gluNewQuadric() \
	gluNewQuadricCTX(cgl_ctx)

#define gluOrtho2D(left, right, bottom, top) \
	gluOrtho2DCTX(cgl_ctx, left, right, bottom, top)

#define gluPerspective(fovy, aspect, znear, zfar) \
	gluPerspectiveCTX(cgl_ctx, fovy, aspect, znear, zfar)

#define gluPickMatrix(x, y, delx, dely, viewport) \
	gluPickMatrixCTX(cgl_ctx, x, y, delx, dely, viewport)

#define gluScaleImage(format, wIn, hIn, typeIn, dataIn, wOut, hOut, typeOut, dataOut) \
	gluScaleImageCTX(cgl_ctx, format, wIn, hIn, typeIn, dataIn, wOut, hOut, typeOut, dataOut)

#define gluCylinder(qobj, baseRadiusd, topRadiusd, heightd, slices, stacks) \
	gluCylinderCTX(cgl_ctx, qobj, baseRadiusd, topRadiusd, heightd, slices, stacks)

#define gluDisk(qobj, innerRadiusd, outerRadiusd, slices, loops) \
	gluDiskCTX(cgl_ctx, qobj, innerRadiusd, outerRadiusd, slices, loops)

#define gluPartialDisk(qobj, innerRadiusd, outerRadiusd, slices, loops, startAngled, sweepAngled) \
	gluPartialDiskCTX(cgl_ctx, qobj, innerRadiusd, outerRadiusd, slices, loops, startAngled, sweepAngled)

#define gluSphere(qobj, radiusd, slices, stacks) \
	gluSphereCTX(cgl_ctx, qobj, radiusd, slices, stacks)

#endif /* _GLUMACRO_H */
