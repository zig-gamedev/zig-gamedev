/*
 *  CoreVideo.h
 *  CoreVideo
 *
 *  Copyright (c) 2004-2015 Apple Inc. All rights reserved.
 *
 */
/*!
    @header CoreVideo
	@copyright 2004-2014 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later, and iOS 4.0 or later
    @abstract   Umbrella header for the CoreVideo framework
    @discussion This header includes all necesssary headers for the CoreVideo API
*/

#include <TargetConditionals.h>
#include <Availability.h>
#include <AvailabilityMacros.h>

#include <CoreVideo/CVReturn.h>
#include <CoreVideo/CVBase.h>

#if COREVIDEO_SUPPORTS_DISPLAYLINK
#include <CoreVideo/CVHostTime.h>
#include <CoreVideo/CVDisplayLink.h>
#endif

#include <CoreVideo/CVBuffer.h>
#include <CoreVideo/CVPixelBuffer.h>
#include <CoreVideo/CVPixelBufferPool.h>


#if COREVIDEO_SUPPORTS_OPENGL
#include <CoreVideo/CVOpenGLBuffer.h>
#include <CoreVideo/CVOpenGLBufferPool.h>
#include <CoreVideo/CVOpenGLTexture.h>
#include <CoreVideo/CVOpenGLTextureCache.h>
#endif // COREVIDEO_SUPPORTS_OPENGL
#if COREVIDEO_SUPPORTS_OPENGLES || TARGET_OS_WATCH
#if __has_include(<OpenGLES/gltypes.h>)
#include <CoreVideo/CVOpenGLESTexture.h>
#include <CoreVideo/CVOpenGLESTextureCache.h>
#endif
#endif // COREVIDEO_SUPPORTS_OPENGLES
#include <CoreVideo/CVPixelFormatDescription.h>

#if COREVIDEO_SUPPORTS_DIRECT3D
#include <CoreVideo/CVDirect3DBuffer.h>
#include <CoreVideo/CVDirect3DBufferPool.h>
#include <CoreVideo/CVDirect3DTexture.h>
#include <CoreVideo/CVDirect3DTextureCache.h>
#endif

#if !0
#if TARGET_OS_MAC
#include <CoreVideo/CVMetalTexture.h>
#include <CoreVideo/CVMetalTextureCache.h>
#endif
#endif
