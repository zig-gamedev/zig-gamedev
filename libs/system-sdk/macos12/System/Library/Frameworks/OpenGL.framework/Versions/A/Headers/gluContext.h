/*
	Copyright:	(c) 1999-2012 Apple Inc. All rights reserved.
*/

#ifndef _GLUCONTEXT_H
#define _GLUCONTEXT_H

#include <OpenGL/glu.h>
#include <OpenGL/CGLTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
** GLU Context routines
*/
extern GLint gluBuild1DMipmapsCTX(      struct _CGLContextObject *ctx,
	GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data) OPENGL_DEPRECATED(10.0, 10.9);
extern GLint gluBuild2DMipmapsCTX(      struct _CGLContextObject *ctx,
	GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data) OPENGL_DEPRECATED(10.0, 10.9);
extern GLint gluBuild3DMipmapsCTX(      struct _CGLContextObject *ctx,
	GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data) OPENGL_DEPRECATED(10.2, 10.9);
extern GLint gluBuild1DMipmapLevelsCTX (      struct _CGLContextObject *ctx,
        GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data) OPENGL_DEPRECATED(10.2, 10.9);
extern GLint gluBuild2DMipmapLevelsCTX (      struct _CGLContextObject *ctx,
        GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data) OPENGL_DEPRECATED(10.2, 10.9);
extern GLint gluBuild3DMipmapLevelsCTX (      struct _CGLContextObject *ctx,
        GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data) OPENGL_DEPRECATED(10.2, 10.9);

extern void gluLookAtCTX(               struct _CGLContextObject *ctx, GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
	GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ) OPENGL_DEPRECATED(10.0, 10.9);
extern GLUnurbs* gluNewNurbsRendererCTX(struct _CGLContextObject *ctx) OPENGL_DEPRECATED(10.0, 10.9);
extern GLUquadric* gluNewQuadricCTX(    struct _CGLContextObject *ctx) OPENGL_DEPRECATED(10.0, 10.9);
extern GLUtesselator* gluNewTessCTX(    struct _CGLContextObject *ctx) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluOrtho2DCTX(              struct _CGLContextObject *ctx, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluPerspectiveCTX(          struct _CGLContextObject *ctx, GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluPickMatrixCTX(           struct _CGLContextObject *ctx, GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport) OPENGL_DEPRECATED(10.0, 10.9);
extern GLint gluScaleImageCTX(          struct _CGLContextObject *ctx, GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn,
	GLsizei wOut, GLsizei hOut, GLenum typeOut, void* dataOut) OPENGL_DEPRECATED(10.0, 10.9);      

extern void gluCylinderCTX(             struct _CGLContextObject *ctx, GLUquadric *qobj, GLdouble baseRadiusd, GLdouble topRadiusd, GLdouble heightd, GLint slices, GLint stacks) OPENGL_DEPRECATED(10.9, 10.9);
extern void gluDiskCTX(                 struct _CGLContextObject *ctx, GLUquadric *qobj, GLdouble innerRadiusd, GLdouble outerRadiusd, GLint slices, GLint loops) OPENGL_DEPRECATED(10.9, 10.9);
extern void gluPartialDiskCTX(          struct _CGLContextObject *ctx, GLUquadric *qobj, GLdouble innerRadiusd, GLdouble outerRadiusd, GLint slices, GLint loops, GLdouble startAngled, GLdouble sweepAngled) OPENGL_DEPRECATED(10.9, 10.9);
extern void gluSphereCTX(               struct _CGLContextObject *ctx, GLUquadric *qobj, GLdouble radiusd, GLint slices, GLint stacks) OPENGL_DEPRECATED(10.9, 10.9);

#ifdef __cplusplus
}
#endif

#endif /* _GLUCONTEXT_H */
