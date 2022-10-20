/*
	Copyright:	(c) 1999-2012 Apple Inc. All rights reserved.
*/

#ifndef _OPENGL_H
#define _OPENGL_H

#include <OpenGL/OpenGLAvailability.h>
#include <OpenGL/CGLCurrent.h>
#include <OpenGL/CGLDevice.h>
#include <OpenGL/CGLRenderers.h>
#include <OpenGL/CGLIOSurface.h>
#include <OpenGL/gltypes.h>
#ifdef __cplusplus
extern "C" {
#endif

OPENGL_ASSUME_NONNULL_BEGIN

/*
** CGL API version.
*/
#define CGL_VERSION_1_0  1
#define CGL_VERSION_1_1  1
#define CGL_VERSION_1_2  1
#define CGL_VERSION_1_3  1

/*
** Pixel format functions
*/
extern CGLError CGLChoosePixelFormat(const CGLPixelFormatAttribute *attribs, CGLPixelFormatObj OPENGL_NULLABLE * OPENGL_NONNULL pix, GLint *npix) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLDestroyPixelFormat(CGLPixelFormatObj pix) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLDescribePixelFormat(CGLPixelFormatObj pix, GLint pix_num, CGLPixelFormatAttribute attrib, GLint *value) OPENGL_DEPRECATED(10.0, 10.14);
extern void CGLReleasePixelFormat(CGLPixelFormatObj pix) OPENGL_AVAILABLE(10_5) OPENGL_DEPRECATED(10.5, 10.14);
extern CGLPixelFormatObj CGLRetainPixelFormat(CGLPixelFormatObj pix) OPENGL_AVAILABLE(10_5) OPENGL_DEPRECATED(10.5, 10.14); 
extern GLuint CGLGetPixelFormatRetainCount(CGLPixelFormatObj pix) OPENGL_AVAILABLE(10_5) OPENGL_DEPRECATED(10.5, 10.14);

/*
** Renderer information functions
*/
extern CGLError CGLQueryRendererInfo(GLuint display_mask, CGLRendererInfoObj OPENGL_NULLABLE * OPENGL_NONNULL rend, GLint *nrend) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLDestroyRendererInfo(CGLRendererInfoObj rend) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLDescribeRenderer(CGLRendererInfoObj rend, GLint rend_num, CGLRendererProperty prop, GLint * OPENGL_NULLABLE value) OPENGL_DEPRECATED(10.0, 10.14);

/*
** Context functions
*/
extern CGLError CGLCreateContext(CGLPixelFormatObj pix, CGLContextObj OPENGL_NULLABLE share, CGLContextObj OPENGL_NULLABLE * OPENGL_NONNULL ctx) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLDestroyContext(CGLContextObj ctx) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLCopyContext(CGLContextObj src, CGLContextObj dst, GLbitfield mask) OPENGL_DEPRECATED(10.0, 10.8);
extern CGLContextObj CGLRetainContext(CGLContextObj ctx) OPENGL_AVAILABLE(10_5) OPENGL_DEPRECATED(10.4, 10.14);
extern void CGLReleaseContext(CGLContextObj ctx) OPENGL_AVAILABLE(10_5) OPENGL_DEPRECATED(10.4, 10.14);
extern GLuint CGLGetContextRetainCount(CGLContextObj ctx) OPENGL_AVAILABLE(10_5) OPENGL_DEPRECATED(10.5, 10.14);
extern CGLPixelFormatObj OPENGL_NULLABLE CGLGetPixelFormat(CGLContextObj ctx) OPENGL_AVAILABLE(10_5) OPENGL_DEPRECATED(10.5, 10.14);

/*
** PBuffer functions
*/
extern CGLError CGLCreatePBuffer(GLsizei width, GLsizei height, GLenum target, GLenum internalFormat, GLint max_level, CGLPBufferObj OPENGL_NULLABLE * OPENGL_NONNULL pbuffer) OPENGL_DEPRECATED(10.3, 10.7);
extern CGLError CGLDestroyPBuffer(CGLPBufferObj pbuffer) OPENGL_DEPRECATED(10.3, 10.7);
extern CGLError CGLDescribePBuffer(CGLPBufferObj obj, GLsizei *width, GLsizei *height, GLenum *target, GLenum *internalFormat, GLint *mipmap) OPENGL_DEPRECATED(10.3, 10.7);
extern CGLError CGLTexImagePBuffer(CGLContextObj ctx, CGLPBufferObj pbuffer, GLenum source) OPENGL_DEPRECATED(10.3, 10.7);
extern CGLPBufferObj CGLRetainPBuffer(CGLPBufferObj pbuffer) OPENGL_DEPRECATED(10.5, 10.7);
extern void CGLReleasePBuffer(CGLPBufferObj pbuffer) OPENGL_DEPRECATED(10.5, 10.7);
extern GLuint CGLGetPBufferRetainCount(CGLPBufferObj pbuffer) OPENGL_DEPRECATED(10.5, 10.7);

/*
** Drawable Functions
*/
extern CGLError CGLSetOffScreen(CGLContextObj ctx, GLsizei width, GLsizei height, GLint rowbytes, void *baseaddr) OPENGL_DEPRECATED(10.0, 10.7);
extern CGLError CGLGetOffScreen(CGLContextObj ctx, GLsizei *width, GLsizei *height, GLint *rowbytes, void * OPENGL_NULLABLE * OPENGL_NONNULL baseaddr) OPENGL_DEPRECATED(10.0, 10.7);
extern CGLError CGLSetFullScreen(CGLContextObj ctx) OPENGL_DEPRECATED(10.0, 10.6);
extern CGLError CGLSetFullScreenOnDisplay(CGLContextObj ctx, GLuint display_mask) OPENGL_DEPRECATED(10.5, 10.7);

extern CGLError CGLSetPBuffer(CGLContextObj ctx, CGLPBufferObj pbuffer, GLenum face, GLint level, GLint screen) OPENGL_DEPRECATED(10.3, 10.7);
extern CGLError CGLGetPBuffer(CGLContextObj ctx, CGLPBufferObj OPENGL_NULLABLE * OPENGL_NONNULL pbuffer, GLenum *face, GLint *level, GLint *screen) OPENGL_DEPRECATED(10.3, 10.7);

extern CGLError CGLClearDrawable(CGLContextObj ctx) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLFlushDrawable(CGLContextObj ctx) OPENGL_DEPRECATED(10.0, 10.14);

/*
** Per context enables and parameters
*/
extern CGLError CGLEnable(CGLContextObj ctx, CGLContextEnable pname) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLDisable(CGLContextObj ctx, CGLContextEnable pname) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLIsEnabled(CGLContextObj ctx, CGLContextEnable pname, GLint *enable) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLSetParameter(CGLContextObj ctx, CGLContextParameter pname, const GLint *params) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLGetParameter(CGLContextObj ctx, CGLContextParameter pname, GLint *params) OPENGL_DEPRECATED(10.0, 10.14);

/*
** Virtual screen functions
*/
extern CGLError CGLSetVirtualScreen(CGLContextObj ctx, GLint screen) OPENGL_DEPRECATED(10.0, 10.14);
extern CGLError CGLGetVirtualScreen(CGLContextObj ctx, GLint *screen) OPENGL_DEPRECATED(10.0, 10.14);

extern CGLError CGLUpdateContext(CGLContextObj ctx) OPENGL_AVAILABLE(10_7) OPENGL_DEPRECATED(10.7, 10.14);

/*
** Global library options
*/
extern CGLError CGLSetGlobalOption(CGLGlobalOption pname, const GLint * OPENGL_NULLABLE params) OPENGL_AVAILABLE(10_6) OPENGL_DEPRECATED(10.6, 10.14);
extern CGLError CGLGetGlobalOption(CGLGlobalOption pname, GLint *params) OPENGL_AVAILABLE(10_6) OPENGL_DEPRECATED(10.6, 10.14);

extern CGLError CGLSetOption(CGLGlobalOption pname, GLint param)  OPENGL_DEPRECATED(10.0, 10.14);  /* Use CGLSetGlobalOption */
extern CGLError CGLGetOption(CGLGlobalOption pname, GLint *param) OPENGL_DEPRECATED(10.0, 10.14); /* Use CGLGetGlobalOption */

/*
** Locking functions
*/
extern CGLError CGLLockContext(CGLContextObj ctx) OPENGL_AVAILABLE(10_4) OPENGL_DEPRECATED(10.4, 10.14);

extern CGLError CGLUnlockContext(CGLContextObj ctx) OPENGL_AVAILABLE(10_4) OPENGL_DEPRECATED(10.4, 10.14);

/*
** Version numbers
*/
extern void CGLGetVersion(GLint * OPENGL_NULLABLE majorvers, GLint * OPENGL_NULLABLE minorvers) OPENGL_DEPRECATED(10.0, 10.14);

/*
** Convert an error code to a string
*/
const char *CGLErrorString(CGLError error) OPENGL_DEPRECATED(10.0, 10.14);

OPENGL_ASSUME_NONNULL_END

#ifdef __cplusplus
}
#endif

#endif /* _OPENGL_H */

