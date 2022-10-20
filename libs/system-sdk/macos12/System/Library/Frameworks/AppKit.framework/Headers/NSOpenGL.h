/*
        NSOpenGL.h
        Application Kit
        Copyright (c) 2000-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <Foundation/Foundation.h>
#import <OpenGL/CGLTypes.h>
#import <OpenGL/gltypes.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

#ifndef GL_SILENCE_DEPRECATION
#define NS_OPENGL_DEPRECATED(minVers, maxVers) API_DEPRECATED("OpenGL API deprecated; please use Metal and MetalKit.  (Define GL_SILENCE_DEPRECATION to silence these warnings.)", macos(minVers,maxVers))
#define NS_OPENGL_ENUM_DEPRECATED(minVers, maxVers) API_DEPRECATED("OpenGL API deprecated; please use Metal and MetalKit.  (Define GL_SILENCE_DEPRECATION to silence these warnings.)", macos(minVers,maxVers))
#define NS_OPENGL_CLASS_DEPRECATED(message, minVers, maxVers) API_DEPRECATED(message, macos(minVers,maxVers))
#else
#define NS_OPENGL_DEPRECATED(minVers, maxVers) API_AVAILABLE(macos(minVers))
#define NS_OPENGL_ENUM_DEPRECATED(minVers, maxVers) API_AVAILABLE(macos(minVers))
#define NS_OPENGL_CLASS_DEPRECATED(message, minVers, maxVers) API_AVAILABLE(macos(minVers))
#endif

@class NSData, NSView, NSScreen;

/* Option names for NSOpenGLSetOption() and NSOpenGLGetOption() */
typedef NS_ENUM(uint32_t, NSOpenGLGlobalOption) {
    NSOpenGLGOFormatCacheSize  NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 501,	/* Set the size of the pixel format cache        */
    NSOpenGLGOClearFormatCache NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 502,	/* Reset the pixel format cache if true          */
    NSOpenGLGORetainRenderers  NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 503,	/* Whether to retain loaded renderers in memory  */
    NSOpenGLGOUseBuildCache    NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 506,	/* Enable the function compilation block cache.  Off by default.  Must be enabled at startup. */
    
    NSOpenGLGOResetLibrary     API_DEPRECATED("", macos(10.0,10.4)) = 504
} NS_OPENGL_DEPRECATED(10.0, 10.14);


/*
 ** Library global options.
 */
APPKIT_EXTERN void NSOpenGLSetOption(NSOpenGLGlobalOption pname, GLint param)   NS_OPENGL_DEPRECATED(10.0, 10.14);
APPKIT_EXTERN void NSOpenGLGetOption(NSOpenGLGlobalOption pname, GLint * param) NS_OPENGL_DEPRECATED(10.0, 10.14);

/*
 ** Library version.
 */
APPKIT_EXTERN void NSOpenGLGetVersion(GLint * _Nullable major, GLint * _Nullable minor) NS_OPENGL_DEPRECATED(10.0, 10.14);


/*********************
 ** NSOpenGLPixelFormat
 *********************/

/*
 ** Attribute names for [NSOpenGLPixelFormat initWithAttributes]
 ** and [NSOpenGLPixelFormat getValues:forAttribute:forVirtualScreen].
 */
enum {
    NSOpenGLPFAAllRenderers          NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =   1,	/* choose from all available renderers          */
    NSOpenGLPFATripleBuffer          NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =   3,	/* choose a triple buffered pixel format        */
    NSOpenGLPFADoubleBuffer          NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =   5,	/* choose a double buffered pixel format        */
    NSOpenGLPFAAuxBuffers            NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =   7,	/* number of aux buffers                        */
    NSOpenGLPFAColorSize             NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =   8,	/* number of color buffer bits                  */
    NSOpenGLPFAAlphaSize             NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  11,	/* number of alpha component bits               */
    NSOpenGLPFADepthSize             NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  12,	/* number of depth buffer bits                  */
    NSOpenGLPFAStencilSize           NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  13,	/* number of stencil buffer bits                */
    NSOpenGLPFAAccumSize             NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  14,	/* number of accum buffer bits                  */
    NSOpenGLPFAMinimumPolicy         NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  51,	/* never choose smaller buffers than requested  */
    NSOpenGLPFAMaximumPolicy         NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  52,	/* choose largest buffers of type requested     */
    NSOpenGLPFASampleBuffers         NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  55,	/* number of multi sample buffers               */
    NSOpenGLPFASamples               NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  56,	/* number of samples per multi sample buffer    */
    NSOpenGLPFAAuxDepthStencil       NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  57,	/* each aux buffer has its own depth stencil    */
    NSOpenGLPFAColorFloat            NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  58,	/* color buffers store floating point pixels    */
    NSOpenGLPFAMultisample           NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  59,    /* choose multisampling                         */
    NSOpenGLPFASupersample           NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  60,    /* choose supersampling                         */
    NSOpenGLPFASampleAlpha           NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  61,    /* request alpha filtering                      */
    NSOpenGLPFARendererID            NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  70,	/* request renderer by ID                       */
    NSOpenGLPFANoRecovery            NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  72,	/* disable all failure recovery systems         */
    NSOpenGLPFAAccelerated           NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  73,	/* choose a hardware accelerated renderer       */
    NSOpenGLPFAClosestPolicy         NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  74,	/* choose the closest color buffer to request   */
    NSOpenGLPFABackingStore          NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  76,	/* back buffer contents are valid after swap    */
    NSOpenGLPFAScreenMask            NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  84,	/* bit mask of supported physical screens       */
    NSOpenGLPFAAllowOfflineRenderers NS_OPENGL_ENUM_DEPRECATED(10.5, 10.14)  =  96,  /* allow use of offline renderers               */
    NSOpenGLPFAAcceleratedCompute    NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  =  97,	/* choose a hardware accelerated compute device */
    NSOpenGLPFAOpenGLProfile         NS_OPENGL_ENUM_DEPRECATED(10.7, 10.14)  =  99,    /* specify an OpenGL Profile to use             */
    NSOpenGLPFAVirtualScreenCount    NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14)  = 128,	/* number of virtual screens in this format     */
    
    NSOpenGLPFAStereo                API_DEPRECATED("", macos(10.0,10.12))     =   6,
    NSOpenGLPFAOffScreen             API_DEPRECATED("", macos(10.0,10.7))      =  53,
    NSOpenGLPFAFullScreen            API_DEPRECATED("", macos(10.0,10.6))      =  54,
    NSOpenGLPFASingleRenderer        API_DEPRECATED("", macos(10.0,10.9))      =  71,
    NSOpenGLPFARobust                API_DEPRECATED("", macos(10.0,10.5))      =  75,
    NSOpenGLPFAMPSafe                API_DEPRECATED("", macos(10.0,10.5))      =  78,
    NSOpenGLPFAWindow                API_DEPRECATED("", macos(10.0,10.9))      =  80,
    NSOpenGLPFAMultiScreen           API_DEPRECATED("", macos(10.0,10.5))      =  81,
    NSOpenGLPFACompliant             API_DEPRECATED("", macos(10.0,10.9))      =  83,
    NSOpenGLPFAPixelBuffer           API_DEPRECATED("", macos(10.3,10.7))      =  90,
    NSOpenGLPFARemotePixelBuffer     API_DEPRECATED("", macos(10.3,10.7))      =  91,
};
typedef uint32_t NSOpenGLPixelFormatAttribute NS_OPENGL_DEPRECATED(10.0, 10.14);

/* NSOpenGLPFAOpenGLProfile values */
enum {
    NSOpenGLProfileVersionLegacy     NS_OPENGL_ENUM_DEPRECATED(10.7, 10.14)  = 0x1000,   /* choose a Legacy/Pre-OpenGL 3.0 Implementation */
    NSOpenGLProfileVersion3_2Core    NS_OPENGL_ENUM_DEPRECATED(10.7, 10.14)  = 0x3200,   /* choose an OpenGL 3.2 Core Implementation      */
    NSOpenGLProfileVersion4_1Core    NS_OPENGL_ENUM_DEPRECATED(10.10, 10.14) = 0x4100    /* choose an OpenGL 4.1 Core Implementation      */
};

/*
 ** NSOpenGLPixelFormat interface.
 */

NS_OPENGL_CLASS_DEPRECATED("Please use Metal or MetalKit.", 10.0, 10.14)
@interface NSOpenGLPixelFormat : NSObject <NSCoding>

- (nullable NSOpenGLPixelFormat *)initWithCGLPixelFormatObj:(CGLPixelFormatObj)format API_AVAILABLE(macos(10.6)) NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithAttributes:(const NSOpenGLPixelFormatAttribute *)attribs;
- (nullable id)initWithData:(null_unspecified NSData*)attribs API_DEPRECATED("Use -initWithAttributes: instead", macos(10.0,10.6));

- (null_unspecified NSData*)attributes API_DEPRECATED("", macos(10.0,10.6));
- (void)setAttributes:(null_unspecified NSData*)attribs API_DEPRECATED("", macos(10.0,10.6));

- (void)getValues:(GLint *)vals forAttribute:(NSOpenGLPixelFormatAttribute)attrib forVirtualScreen:(GLint)screen;
@property (readonly) GLint numberOfVirtualScreens;

@property (nullable, readonly) CGLPixelFormatObj CGLPixelFormatObj NS_RETURNS_INNER_POINTER;

@end

/*********************
 ** NSOpenGLPixelBuffer
 *********************/

/* NOTE: PBuffers should be considered deprecated as of 10.7.  Use GL_EXT_framebuffer_object instead.
 */
API_DEPRECATED("Use GL_EXT_framebuffer_object instead", macos(10.2,10.7))
@interface NSOpenGLPixelBuffer : NSObject

/*
 ** size width and height must be powers of two for 1D or 2D or CUBE_MAP targets
 ** size width and height must also be equal for CUBE_MAP target
 ** target should be one of GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP or GL_TEXTURE_RECTANGLE_EXT
 ** internalFormat should be GL_RGB, GL_RGBA or GL_DEPTH_COMPONENT
 ** maxLevel specifies the desired maximum mipmap level, starting with 0.  Must be 0 for
 ** TEXTURE_RECTANGLE targets.
 */
- (nullable instancetype)initWithTextureTarget:(GLenum)target textureInternalFormat:(GLenum)format textureMaxMipMapLevel:(GLint)maxLevel pixelsWide:(GLsizei)pixelsWide pixelsHigh:(GLsizei)pixelsHigh API_DEPRECATED("", macos(10.2,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
- (nullable NSOpenGLPixelBuffer *)initWithCGLPBufferObj:(CGLPBufferObj)pbuffer API_DEPRECATED("", macos(10.6,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
@property (nullable, readonly) CGLPBufferObj CGLPBufferObj NS_RETURNS_INNER_POINTER API_AVAILABLE(macos(10.6)) API_DEPRECATED("", macos(10.6,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
@property (readonly) GLsizei pixelsWide API_DEPRECATED("", macos(10.2,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
@property (readonly) GLsizei pixelsHigh API_DEPRECATED("", macos(10.2,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
@property (readonly) GLenum textureTarget API_DEPRECATED("", macos(10.2,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
@property (readonly) GLenum textureInternalFormat API_DEPRECATED("", macos(10.2,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
@property (readonly) GLint textureMaxMipMapLevel API_DEPRECATED("", macos(10.2,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
@end


/*****************
 ** NSOpenGLContext
 *****************/

/* Parameter names for NSOpenGLContext -setValues:forParameter: and -getValues:forParameter: */
typedef NS_ENUM(NSInteger, NSOpenGLContextParameter) {
    NSOpenGLContextParameterSwapInterval           NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 222, /* 1 param.  0 -> Don't sync, 1 -> Sync to vertical retrace     */
    NSOpenGLContextParameterSurfaceOrder           NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 235, /* 1 param.  1 -> Above Window (default), -1 -> Below Window    */
    NSOpenGLContextParameterSurfaceOpacity         NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 236, /* 1 param.  1-> Surface is opaque (default), 0 -> non-opaque   */
    NSOpenGLContextParameterSurfaceBackingSize     NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 304, /* 2 params.  Width/height of surface backing size              */
    NSOpenGLContextParameterReclaimResources       NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 308, /* 0 params.                                                    */
    NSOpenGLContextParameterCurrentRendererID      NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 309, /* 1 param.   Retrieves the current renderer ID                 */
    NSOpenGLContextParameterGPUVertexProcessing    NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 310, /* 1 param.   Currently processing vertices with GPU (get)      */
    NSOpenGLContextParameterGPUFragmentProcessing  NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 311, /* 1 param.   Currently processing fragments with GPU (get)     */
    NSOpenGLContextParameterHasDrawable            NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 314, /* 1 param.   Boolean returned if drawable is attached          */
    NSOpenGLContextParameterMPSwapsInFlight        NS_OPENGL_ENUM_DEPRECATED(10.0, 10.14) = 315, /* 1 param.   Max number of swaps queued by the MP GL engine    */
    
    NSOpenGLContextParameterSwapRectangle API_DEPRECATED("", macos(10.0,10.14)) = 200, /* 4 params.  Set or get the swap rectangle {x, y, w, h} */
    NSOpenGLContextParameterSwapRectangleEnable API_DEPRECATED("", macos(10.0,10.14)) = 201, /* Enable or disable the swap rectangle */
    NSOpenGLContextParameterRasterizationEnable API_DEPRECATED("", macos(10.0,10.14)) = 221, /* Enable or disable all rasterization */
    NSOpenGLContextParameterStateValidation API_DEPRECATED("", macos(10.0,10.14)) = 301, /* Validate state for multi-screen functionality */
    NSOpenGLContextParameterSurfaceSurfaceVolatile API_DEPRECATED("", macos(10.0,10.14))  = 306, /* 1 param.   Surface volatile state */
} NS_OPENGL_DEPRECATED(10.0, 10.14);

NS_OPENGL_CLASS_DEPRECATED("Please use Metal or MetalKit.", 10.0, 10.14)
@interface NSOpenGLContext : NSObject <NSLocking>

/* Context creation */
- (nullable instancetype)initWithFormat:(NSOpenGLPixelFormat *)format shareContext:(nullable NSOpenGLContext *)share;
- (nullable NSOpenGLContext *)initWithCGLContextObj:(CGLContextObj)context API_AVAILABLE(macos(10.6));

@property (readonly, strong) NSOpenGLPixelFormat *pixelFormat API_AVAILABLE(macos(10.10));

/* Drawable management */
@property (nullable, weak) NSView *view NS_SWIFT_UI_ACTOR API_DEPRECATED("", macos(10.0,10.14));
- (void)setView:(nullable NSView *)view NS_SWIFT_UI_ACTOR API_DEPRECATED("Use NSOpenGLView to provide OpenGL content in a Cocoa app.", macos(10.0,10.14));
- (void)setFullScreen API_DEPRECATED("Use a fullscreen NSOpenGLView instead", macos(10.0,10.7));

/* Deprecated in 10.7.  The -setOffScreen:width:height:rowbytes: API forces use of the software rasterizer, which is much slower than GPU rendering.  It is generally much better nowadays to use a normal pixel format with either an off-screen window or an FBO (GL_EXT_framebuffer_object), and then call glReadPixels() to read the rendered result back to CPU memory (if that's where it is needed).
 */
- (void)setOffScreen:(void *)baseaddr width:(GLsizei)width height:(GLsizei)height rowbytes:(GLint)rowbytes API_DEPRECATED("", macos(10.0,10.7));

- (void)clearDrawable;
- (void)update NS_SWIFT_UI_ACTOR;

/* Flush draw buffer */
- (void)flushBuffer;

/* Current context control */
- (void)makeCurrentContext;
+ (void)clearCurrentContext;
@property (class, readonly, nullable, strong) NSOpenGLContext *currentContext;

/* Copy attributes from another context */
- (void)copyAttributesFromContext:(NSOpenGLContext *)context withMask:(GLbitfield)mask API_DEPRECATED("", macos(10.0,10.8));

/* Context Parameter handling */
- (void)setValues:(const GLint *)vals forParameter:(NSOpenGLContextParameter)param;
- (void)getValues:(GLint *)vals forParameter:(NSOpenGLContextParameter)param;


/* virtual screens */
@property GLint currentVirtualScreen;

/* creating textures */
- (void)createTexture:(GLenum)target fromView:(NSView *)view internalFormat:(GLenum)format API_DEPRECATED("", macos(10.2,10.8));

@property (nullable, readonly) CGLContextObj CGLContextObj NS_RETURNS_INNER_POINTER;

@end

@interface NSOpenGLContext (NSOpenGLPixelBuffer)
/*
 ** NOTE: PBuffers have been Deprecated as of 10.7.  Use GL_EXT_framebuffer_object instead.
 **
 ** Attach context to an NSOpenGLPixelBuffer instance.
 ** For GL_CUBE_MAP target pixel buffers, face should be one of GL_TEXTURE_CUBE_MAP_POSITIVE_X,
 ** GL_TEXTURE_CUBE_MAP_POSITIVE_Y, etc.  Otherwise it should be 0.
 ** Level specifies the desired mipmap level you want to render to.  It must less than or equal to the maxLevel
 ** parameter of the pixel buffer.
 ** If applicable, the virtual screen should be set to the same value as the current virtual screen you are using
 ** to render on-screen with.
 */
- (void)setPixelBuffer:(NSOpenGLPixelBuffer *)pixelBuffer cubeMapFace:(GLenum)face mipMapLevel:(GLint)level currentVirtualScreen:(GLint)screen API_DEPRECATED("", macos(10.3,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
- (nullable NSOpenGLPixelBuffer *)pixelBuffer API_DEPRECATED("", macos(10.3,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
- (GLenum)pixelBufferCubeMapFace API_DEPRECATED("", macos(10.3,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
- (GLint)pixelBufferMipMapLevel API_DEPRECATED("", macos(10.3,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */

/*
 ** NOTE: PBuffers have been Deprecated as of 10.7.  Use GL_EXT_framebuffer_object instead.
 **
 ** This call is a mirror of CGLTexImagePBuffer.  This essentially "binds" the given pixel buffer's image data
 ** to the currently bound texture object.   Source specifies which of the PBuffer's color buffers should be used,
 ** and should be one of GL_FRONT, GL_BACK, GL_AUX0, etc.
 */
- (void)setTextureImageToPixelBuffer:(NSOpenGLPixelBuffer *)pixelBuffer colorBuffer:(GLenum)source API_DEPRECATED("", macos(10.3,10.7)); /* Use IOSurface instead of NSOpenGLPixelBuffer on Mac OS 10.7 and newer. */
@end


static const NSOpenGLContextParameter NSOpenGLCPSwapInterval API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterSwapInterval", macos(10.5,10.14)) = NSOpenGLContextParameterSwapInterval;
static const NSOpenGLContextParameter NSOpenGLCPSurfaceOrder API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterSurfaceOrder", macos(10.0,10.14)) = NSOpenGLContextParameterSurfaceOrder;
static const NSOpenGLContextParameter NSOpenGLCPSurfaceOpacity API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterSurfaceOpacity", macos(10.0,10.14)) = NSOpenGLContextParameterSurfaceOpacity;
static const NSOpenGLContextParameter NSOpenGLCPSurfaceBackingSize API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterSurfaceBackingSize", macos(10.0,10.14)) = NSOpenGLContextParameterSurfaceBackingSize;
static const NSOpenGLContextParameter NSOpenGLCPReclaimResources API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterReclaimResources", macos(10.0,10.14)) = NSOpenGLContextParameterReclaimResources;
static const NSOpenGLContextParameter NSOpenGLCPCurrentRendererID API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterCurrentRendererID", macos(10.0,10.14)) = NSOpenGLContextParameterCurrentRendererID;
static const NSOpenGLContextParameter NSOpenGLCPGPUVertexProcessing API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterGPUVertexProcessing", macos(10.0,10.14)) = NSOpenGLContextParameterGPUVertexProcessing;
static const NSOpenGLContextParameter NSOpenGLCPGPUFragmentProcessing API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterGPUFragmentProcessing", macos(10.0,10.14)) = NSOpenGLContextParameterGPUFragmentProcessing;
static const NSOpenGLContextParameter NSOpenGLCPHasDrawable API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterHasDrawable", macos(10.0,10.14)) = NSOpenGLContextParameterHasDrawable;
static const NSOpenGLContextParameter NSOpenGLCPMPSwapsInFlight API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterMPSwapsInFlight", macos(10.0,10.14)) = NSOpenGLContextParameterMPSwapsInFlight;

static const NSOpenGLContextParameter NSOpenGLCPSwapRectangle API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterSwapRectangle", macos(10.0,10.14)) = NSOpenGLContextParameterSwapRectangle;
static const NSOpenGLContextParameter NSOpenGLCPSwapRectangleEnable API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterSwapRectangleEnable", macos(10.0,10.14)) = NSOpenGLContextParameterSwapRectangleEnable;
static const NSOpenGLContextParameter NSOpenGLCPRasterizationEnable API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterRasterizationEnable", macos(10.0,10.14)) = NSOpenGLContextParameterRasterizationEnable;
static const NSOpenGLContextParameter NSOpenGLCPStateValidation API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterStateValidation", macos(10.0,10.14)) = NSOpenGLContextParameterStateValidation;
static const NSOpenGLContextParameter NSOpenGLCPSurfaceSurfaceVolatile API_DEPRECATED_WITH_REPLACEMENT("NSOpenGLContextParameterSurfaceSurfaceVolatile", macos(10.0,10.14)) = NSOpenGLContextParameterSurfaceSurfaceVolatile;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
