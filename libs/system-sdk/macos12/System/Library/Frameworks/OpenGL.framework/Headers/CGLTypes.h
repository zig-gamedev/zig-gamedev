/*
	Copyright:	(c) 1999-2013 Apple Inc. All rights reserved.
*/

#ifndef _CGLTYPES_H
#define _CGLTYPES_H

#include <OpenGL/OpenGLAvailability.h>

#ifdef __cplusplus
extern "C" {
#endif

#if __has_feature(assume_nonnull)
#define OPENGL_ASSUME_NONNULL_BEGIN _Pragma("clang assume_nonnull begin")
#define OPENGL_ASSUME_NONNULL_END _Pragma("clang assume_nonnull end")
#else
#define OPENGL_ASSUME_NONNULL_BEGIN
#define OPENGL_ASSUME_NONNULL_END
#endif

#if __has_feature(nullability)
#define OPENGL_NULLABLE __nullable
#define OPENGL_NONNULL __nonnull
#else
#define OPENGL_NULLABLE
#define OPENGL_NONNULL
#endif

#if __has_attribute(objc_bridge) && __has_feature(objc_bridge_id) && __has_feature(objc_bridge_id_on_typedefs)
#define OPENGL_BRIDGED_TYPE(T)		__attribute__((objc_bridge(T)))
#else
#define OPENGL_BRIDGED_TYPE(T)
#endif

#if __has_feature(objc_class_property)
#define OPENGL_SWIFT_NAME(name) __attribute__((swift_name(#name)))
#else
#define OPENGL_SWIFT_NAME(name)
#endif

/*
** CGL opaque data.
*/
typedef struct _CGLContextObject       *CGLContextObj;
typedef struct _CGLPixelFormatObject   *CGLPixelFormatObj;
typedef struct _CGLRendererInfoObject  *CGLRendererInfoObj;
typedef struct _CGLPBufferObject       *CGLPBufferObj OPENGL_DEPRECATED(10.3, 10.7);

/*
** Attribute names for CGLChoosePixelFormat and CGLDescribePixelFormat.
*/
typedef enum _CGLPixelFormatAttribute {
	kCGLPFAAllRenderers                                                   =   1, /* choose from all available renderers          */
	kCGLPFATripleBuffer OPENGL_ENUM_AVAILABLE(10_7)                       =   3, /* choose a triple buffered pixel format        */
	kCGLPFADoubleBuffer                                                   =   5, /* choose a double buffered pixel format        */
	kCGLPFAColorSize                                                      =   8, /* number of color buffer bits                  */
	kCGLPFAAlphaSize                                                      =  11, /* number of alpha component bits               */
	kCGLPFADepthSize                                                      =  12, /* number of depth buffer bits                  */
	kCGLPFAStencilSize                                                    =  13, /* number of stencil buffer bits                */
	kCGLPFAMinimumPolicy                                                  =  51, /* never choose smaller buffers than requested  */
	kCGLPFAMaximumPolicy                                                  =  52, /* choose largest buffers of type requested     */
	kCGLPFASampleBuffers OPENGL_ENUM_AVAILABLE(10_2)                      =  55, /* number of multi sample buffers               */
	kCGLPFASamples OPENGL_ENUM_AVAILABLE(10_2)                            =  56, /* number of samples per multi sample buffer    */
	kCGLPFAColorFloat OPENGL_ENUM_AVAILABLE(10_2)                         =  58, /* color buffers store floating point pixels    */
	kCGLPFAMultisample OPENGL_ENUM_AVAILABLE(10_3)                        =  59, /* choose multisampling                         */
	kCGLPFASupersample OPENGL_ENUM_AVAILABLE(10_3)                        =  60, /* choose supersampling                         */
	kCGLPFASampleAlpha OPENGL_ENUM_AVAILABLE(10_3)                        =  61, /* request alpha filtering                      */
	kCGLPFARendererID                                                     =  70, /* request renderer by ID                       */
	kCGLPFANoRecovery                                                     =  72, /* disable all failure recovery systems         */
	kCGLPFAAccelerated                                                    =  73, /* choose a hardware accelerated renderer       */
	kCGLPFAClosestPolicy                                                  =  74, /* choose the closest color buffer to request   */
	kCGLPFABackingStore                                                   =  76, /* back buffer contents are valid after swap    */
	kCGLPFABackingVolatile OPENGL_ENUM_AVAILABLE(10_7)                    =  77, /* back buffer contents are volatile after swap */
	kCGLPFADisplayMask                                                    =  84, /* mask limiting supported displays             */
	kCGLPFAAllowOfflineRenderers OPENGL_ENUM_AVAILABLE(10_5)              =  96, /* show offline renderers in pixel formats      */
	kCGLPFAAcceleratedCompute OPENGL_ENUM_AVAILABLE(10_6)                 =  97, /* choose a hardware accelerated compute device */
	kCGLPFAOpenGLProfile OPENGL_ENUM_AVAILABLE(10_7)                      =  99, /* specify an OpenGL Profile to use             */
	kCGLPFASupportsAutomaticGraphicsSwitching OPENGL_ENUM_AVAILABLE(10_8) = 101, /* responds to display changes                  */
	kCGLPFAVirtualScreenCount                                             = 128, /* number of virtual screens in this format     */

	/* Note: the following attributes are deprecated in Core Profile                    */
	kCGLPFAAuxBuffers                                  =   7, /* number of aux buffers                        */
	kCGLPFAAccumSize                                   =  14, /* number of accum buffer bits                  */
	kCGLPFAAuxDepthStencil OPENGL_ENUM_AVAILABLE(10_2) =  57, /* each aux buffer has its own depth stencil    */

	kCGLPFAStereo OPENGL_ENUM_DEPRECATED(10_0, 10_11)        =   6,
	kCGLPFAOffScreen OPENGL_ENUM_DEPRECATED(10_0, 10_7)      =  53,
	kCGLPFAWindow OPENGL_ENUM_DEPRECATED(10_0, 10_9)         =  80,
	kCGLPFACompliant OPENGL_ENUM_DEPRECATED(10_0, 10_9)      =  83,
	kCGLPFAPBuffer OPENGL_ENUM_DEPRECATED(10_3, 10_7)        =  90,
	kCGLPFARemotePBuffer OPENGL_ENUM_DEPRECATED(10_3, 10_9)  =  91,

	kCGLPFASingleRenderer OPENGL_ENUM_DEPRECATED(10_0, 10_9) =  71,
	kCGLPFARobust OPENGL_ENUM_DEPRECATED(10_0, 10_5)         =  75,
	kCGLPFAMPSafe OPENGL_ENUM_DEPRECATED(10_0, 10_5)         =  78,
	kCGLPFAMultiScreen OPENGL_ENUM_DEPRECATED(10_0, 10_5)    =  81,
	kCGLPFAFullScreen OPENGL_ENUM_DEPRECATED(10_0, 10_6)     =  54,
} CGLPixelFormatAttribute;

/*
** Property names for CGLDescribeRenderer.
*/
typedef enum _CGLRendererProperty {
	kCGLRPOffScreen                                          =  53,
	kCGLRPRendererID                                         =  70,
	kCGLRPAccelerated                                        =  73,
	kCGLRPBackingStore                                       =  76,
	kCGLRPWindow                                             =  80,
	kCGLRPCompliant                                          =  83,
	kCGLRPDisplayMask                                        =  84,
	kCGLRPBufferModes                                        = 100, /* a bitfield of supported buffer modes             */
	kCGLRPColorModes                                         = 103, /* a bitfield of supported color buffer formats     */
	kCGLRPAccumModes                                         = 104, /* a bitfield of supported accum buffer formats     */
	kCGLRPDepthModes                                         = 105, /* a bitfield of supported depth buffer depths      */
	kCGLRPStencilModes                                       = 106, /* a bitfield of supported stencil buffer depths    */
	kCGLRPMaxAuxBuffers                                      = 107, /* maximum number of auxilliary buffers             */
	kCGLRPMaxSampleBuffers OPENGL_ENUM_AVAILABLE(10_2)       = 108, /* maximum number of sample buffers                 */
	kCGLRPMaxSamples OPENGL_ENUM_AVAILABLE(10_2)             = 109, /* maximum number of samples                        */
	kCGLRPSampleModes OPENGL_ENUM_AVAILABLE(10_3)            = 110, /* a bitfield of supported sample modes             */
	kCGLRPSampleAlpha OPENGL_ENUM_AVAILABLE(10_3)            = 111, /* support for alpha sampling                       */
	kCGLRPGPUVertProcCapable OPENGL_ENUM_AVAILABLE(10_4)     = 122, /* renderer capable of GPU vertex processing        */
	kCGLRPGPUFragProcCapable OPENGL_ENUM_AVAILABLE(10_4)     = 123, /* renderer capable of GPU fragment processing      */
	kCGLRPRendererCount                                      = 128, /* the number of renderers in this renderer info    */
	kCGLRPOnline OPENGL_ENUM_AVAILABLE(10_5)                 = 129, /* a boolean stating if renderer is on/offline      */
	kCGLRPAcceleratedCompute OPENGL_ENUM_AVAILABLE(10_6)     = 130, /* hardware accelerated compute device              */
	kCGLRPVideoMemoryMegabytes OPENGL_ENUM_AVAILABLE(10_7)   = 131, /* total video memory (in megabytes)                */
	kCGLRPTextureMemoryMegabytes OPENGL_ENUM_AVAILABLE(10_7) = 132, /* video memory useable for textures (in megabytes) */
	kCGLRPMajorGLVersion OPENGL_ENUM_AVAILABLE(10_9)         = 133, /* maximum supported major GL revision              */

	kCGLRPRegistryIDLow OPENGL_ENUM_AVAILABLE(10_13)         = 140, /* Low 32-bits of registryID */
	kCGLRPRegistryIDHigh OPENGL_ENUM_AVAILABLE(10_13)        = 141, /* High 32-bits of registryID */
	kCGLRPRemovable OPENGL_ENUM_AVAILABLE(10_13)             = 142, /* renderer is removable (eGPU) */
	
	kCGLRPRobust OPENGL_ENUM_DEPRECATED(10_0, 10_5)      =  75,
	kCGLRPMPSafe OPENGL_ENUM_DEPRECATED(10_0, 10_5)      =  78,
	kCGLRPMultiScreen OPENGL_ENUM_DEPRECATED(10_0, 10_5) =  81,
	kCGLRPFullScreen OPENGL_ENUM_DEPRECATED(10_0, 10_6)  =  54,
	kCGLRPVideoMemory OPENGL_ENUM_DEPRECATED_MSG(10_0, 10_7, "Use kCGLRPVideoMemoryMegabytes") = 120,
	kCGLRPTextureMemory OPENGL_ENUM_DEPRECATED_MSG(10_0, 10_7, "Use kCGLRPTextureMemoryMegabytes") = 121,
} CGLRendererProperty;

/*
** Enable names for CGLEnable, CGLDisable, and CGLIsEnabled.
*/
typedef enum _CGLContextEnable {
	kCGLCESwapRectangle                                       = 201, /* Enable or disable the swap rectangle              */
	kCGLCESwapLimit OPENGL_ENUM_AVAILABLE(10_2)               = 203, /* Enable or disable the swap async limit            */
	kCGLCERasterization                                       = 221, /* Enable or disable all rasterization               */
	kCGLCEStateValidation                                     = 301, /* Validate state for multi-screen functionality     */
	kCGLCESurfaceBackingSize OPENGL_ENUM_AVAILABLE(10_3)      = 305, /* Enable or disable surface backing size override   */
	kCGLCEDisplayListOptimization OPENGL_ENUM_AVAILABLE(10_3) = 307, /* Ability to turn off display list optimizer        */
	kCGLCEMPEngine OPENGL_ENUM_AVAILABLE(10_4)                = 313, /* Enable or disable multi-threaded GL engine        */
	kCGLCECrashOnRemovedFunctions OPENGL_ENUM_AVAILABLE(10_7) = 316  /* Die on call to function removed from Core Profile */
} CGLContextEnable;

/*
** GPURestartStatus names
*/
typedef enum _CGLGPURestartStatus { /* GPU Restart Status */
	kCGLCPGPURestartStatusNone        = 0, /* current context has not caused recent GPU restart */
	kCGLCPGPURestartStatusCaused      = 1, /* current context caused recent GPU restart (auto-clear on query) */
	kCGLCPGPURestartStatusBlacklisted OPENGL_ENUM_DEPRECATED(10_0, 10_10) = 2, /* current context is being ignored for excessive GPU restarts (won't clear on query) */
    kCGLCPGPURestartStatusDenied      = 2, /* current context is being ignored for excessive GPU restarts (won't clear on query) */
} CGLGPURestartStatus;

/*
** Parameter names for CGLSetParameter and CGLGetParameter.
*/
typedef enum _CGLContextParameter {
	kCGLCPSwapRectangle                                      = 200, /* 4 params.  Set or get the swap rectangle {x, y, w, h}        */
	kCGLCPSwapInterval                                       = 222, /* 1 param.   0 -> Don't sync, 1 -> Sync to vertical retrace    */
	kCGLCPDispatchTableSize OPENGL_ENUM_AVAILABLE(10_3)      = 224, /* 1 param.   Get the dispatch table size                       */
	/* Note: kCGLCPClientStorage is always a pointer-sized parameter, even though the API claims GLint. */
	kCGLCPClientStorage                                      = 226, /* 1 param.   Context specific generic storage                  */
	kCGLCPSurfaceTexture OPENGL_ENUM_DEPRECATED(10_3, 10_9)  = 228, /* 3 params.  Context, target, internal_format                  */
/*  - Used by AGL - */
/*  AGL_STATE_VALIDATION                                       230 */
/*  AGL_BUFFER_NAME                                            231 */
/*  AGL_ORDER_CONTEXT_TO_FRONT                                 232 */
/*  AGL_CONTEXT_SURFACE_ID                                     233 */
/*  AGL_CONTEXT_DISPLAY_ID                                     234 */
	kCGLCPSurfaceOrder OPENGL_ENUM_AVAILABLE(10_2)           = 235, /* 1 param.   1 -> Above window, -1 -> Below Window             */
	kCGLCPSurfaceOpacity OPENGL_ENUM_AVAILABLE(10_2)         = 236, /* 1 param.   1 -> Surface is opaque (default), 0 -> non-opaque */
/*  - Used by AGL - */
/*  AGL_CLIP_REGION                                            254 */
/*  AGL_FS_CAPTURE_SINGLE                                      255 */
	kCGLCPSurfaceBackingSize OPENGL_ENUM_AVAILABLE(10_3)     = 304, /* 2 params.  Width/height of surface backing size              */
/*  AGL_SURFACE_VOLATILE                                       306 */
	kCGLCPSurfaceSurfaceVolatile OPENGL_ENUM_AVAILABLE(10_3) = 306, /* 1 param.   Surface volatile state                            */
	kCGLCPReclaimResources OPENGL_ENUM_AVAILABLE(10_4)       = 308, /* 0 params.                                                    */
	kCGLCPCurrentRendererID OPENGL_ENUM_AVAILABLE(10_4)      = 309, /* 1 param.   Retrieves the current renderer ID                 */
	kCGLCPGPUVertexProcessing OPENGL_ENUM_AVAILABLE(10_4)    = 310, /* 1 param.   Currently processing vertices with GPU (get)      */
	kCGLCPGPUFragmentProcessing OPENGL_ENUM_AVAILABLE(10_4)  = 311, /* 1 param.   Currently processing fragments with GPU (get)     */
	kCGLCPHasDrawable OPENGL_ENUM_AVAILABLE(10_5)            = 314, /* 1 param.   Boolean returned if drawable is attached			*/
	kCGLCPMPSwapsInFlight OPENGL_ENUM_AVAILABLE(10_5)        = 315, /* 1 param.   Max number of swaps queued by the MP GL engine	*/
	kCGLCPGPURestartStatus OPENGL_ENUM_AVAILABLE(10_10)      = 317, /* 1 param.   Retrieves and clears the current CGLGPURestartStatus */
    kCGLCPAbortOnGPURestartStatusBlacklisted OPENGL_ENUM_DEPRECATED(10_0, 10_10) = 318, /* 1 param.  Establish action to take upon blacklisting */
    kCGLCPAbortOnGPURestartStatusDenied OPENGL_ENUM_AVAILABLE(10_10) = 318, /* 1 param.  Establish action to take upon blacklisting */
	kCGLCPSupportGPURestart OPENGL_ENUM_AVAILABLE(10_10)     = 319, /* 1 param.   Does driver support auto-restart of GPU on hang/crash? */
	kCGLCPSupportSeparateAddressSpace OPENGL_ENUM_AVAILABLE(10_10) = 320, /* 1 param. Does context get its own GPU address space?   */
	kCGLCPContextPriorityRequest OPENGL_ENUM_AVAILABLE(10_10) = 608, /* 1 param. kCGLCPContextPriorityRequest[High|Normal|Low] 0|1|2 */
} CGLContextParameter;

typedef enum
{
	kCGLCPContextPriorityRequestHigh   = 0,
	kCGLCPContextPriorityRequestNormal = 1,
	kCGLCPContextPriorityRequestLow    = 2
} CGLCPContextPriorityRequest;

/*
** Option names for CGLSetOption and CGLGetOption.
*/
typedef enum _CGLGlobalOption {
	kCGLGOFormatCacheSize                           = 501, /* Set the size of the pixel format cache        */
	kCGLGOClearFormatCache                          = 502, /* Reset the pixel format cache if true          */
	kCGLGORetainRenderers                           = 503, /* Whether to retain loaded renderers in memory  */
	kCGLGOUseBuildCache OPENGL_ENUM_AVAILABLE(10_5) = 506, /* Enable the function compilation block cache.  */
	                                                       /* Off by default.  Must be enabled at startup.  */
	
	kCGLGOResetLibrary OPENGL_ENUM_DEPRECATED(10_0, 10_4)     = 504,
	kCGLGOUseErrorHandler OPENGL_ENUM_DEPRECATED(10_0, 10_7)  = 505,
} CGLGlobalOption;

/*
** OpenGL Implementation Profiles
*/
typedef enum _CGLOpenGLProfile {
	kCGLOGLPVersion_Legacy OPENGL_ENUM_AVAILABLE(10_7)   = 0x1000, /* choose a renderer compatible with GL1.0       */
	kCGLOGLPVersion_3_2_Core OPENGL_ENUM_AVAILABLE(10_7) = 0x3200, /* choose a renderer capable of GL3.2 or later   */
	kCGLOGLPVersion_GL3_Core OPENGL_ENUM_AVAILABLE(10_7) = 0x3200, /* choose a renderer capable of GL3.2 or later   */
	kCGLOGLPVersion_GL4_Core OPENGL_ENUM_AVAILABLE(10_9) = 0x4100, /* choose a renderer capable of GL4.1 or later   */
} CGLOpenGLProfile OPENGL_AVAILABLE(10_7);

/*
** Error return values from CGLGetError.
*/
typedef enum _CGLError {
	kCGLNoError            = 0,     /* no error */
	kCGLBadAttribute       = 10000,	/* invalid pixel format attribute  */
	kCGLBadProperty        = 10001,	/* invalid renderer property       */
	kCGLBadPixelFormat     = 10002,	/* invalid pixel format            */
	kCGLBadRendererInfo    = 10003,	/* invalid renderer info           */
	kCGLBadContext         = 10004,	/* invalid context                 */
	kCGLBadDrawable        = 10005,	/* invalid drawable                */
	kCGLBadDisplay         = 10006,	/* invalid graphics device         */
	kCGLBadState           = 10007,	/* invalid context state           */
	kCGLBadValue           = 10008,	/* invalid numerical value         */
	kCGLBadMatch           = 10009,	/* invalid share context           */
	kCGLBadEnumeration     = 10010,	/* invalid enumerant               */
	kCGLBadOffScreen       = 10011,	/* invalid offscreen drawable      */
	kCGLBadFullScreen      = 10012,	/* invalid fullscreen drawable     */
	kCGLBadWindow          = 10013,	/* invalid window                  */
	kCGLBadAddress         = 10014,	/* invalid pointer                 */
	kCGLBadCodeModule      = 10015,	/* invalid code module             */
	kCGLBadAlloc           = 10016,	/* invalid memory allocation       */
	kCGLBadConnection      = 10017 	/* invalid CoreGraphics connection */
} CGLError;


/* 
** Buffer modes
*/
#define kCGLMonoscopicBit   0x00000001
#define kCGLStereoscopicBit 0x00000002
#define kCGLSingleBufferBit 0x00000004
#define kCGLDoubleBufferBit 0x00000008
#define kCGLTripleBufferBit 0x00000010

/*
** Depth and stencil buffer depths
*/
#define kCGL0Bit            0x00000001
#define kCGL1Bit            0x00000002
#define kCGL2Bit            0x00000004
#define kCGL3Bit            0x00000008
#define kCGL4Bit            0x00000010
#define kCGL5Bit            0x00000020
#define kCGL6Bit            0x00000040
#define kCGL8Bit            0x00000080
#define kCGL10Bit           0x00000100
#define kCGL12Bit           0x00000200
#define kCGL16Bit           0x00000400
#define kCGL24Bit           0x00000800
#define kCGL32Bit           0x00001000
#define kCGL48Bit           0x00002000
#define kCGL64Bit           0x00004000
#define kCGL96Bit           0x00008000
#define kCGL128Bit          0x00010000

/*
** Color and accumulation buffer formats.
*/
#define kCGLRGB444Bit       0x00000040  /* 16 rgb bit/pixel,    R=11:8, G=7:4, B=3:0              */
#define kCGLARGB4444Bit     0x00000080  /* 16 argb bit/pixel,   A=15:12, R=11:8, G=7:4, B=3:0     */
#define kCGLRGB444A8Bit     0x00000100  /* 8-16 argb bit/pixel, A=7:0, R=11:8, G=7:4, B=3:0       */
#define kCGLRGB555Bit       0x00000200  /* 16 rgb bit/pixel,    R=14:10, G=9:5, B=4:0             */
#define kCGLARGB1555Bit     0x00000400  /* 16 argb bit/pixel,   A=15, R=14:10, G=9:5, B=4:0       */
#define kCGLRGB555A8Bit     0x00000800  /* 8-16 argb bit/pixel, A=7:0, R=14:10, G=9:5, B=4:0      */
#define kCGLRGB565Bit       0x00001000  /* 16 rgb bit/pixel,    R=15:11, G=10:5, B=4:0            */
#define kCGLRGB565A8Bit     0x00002000  /* 8-16 argb bit/pixel, A=7:0, R=15:11, G=10:5, B=4:0     */
#define kCGLRGB888Bit       0x00004000  /* 32 rgb bit/pixel,    R=23:16, G=15:8, B=7:0            */
#define kCGLARGB8888Bit     0x00008000  /* 32 argb bit/pixel,   A=31:24, R=23:16, G=15:8, B=7:0   */
#define kCGLRGB888A8Bit     0x00010000  /* 8-32 argb bit/pixel, A=7:0, R=23:16, G=15:8, B=7:0     */
#define kCGLRGB101010Bit    0x00020000  /* 32 rgb bit/pixel,    R=29:20, G=19:10, B=9:0           */
#define kCGLARGB2101010Bit  0x00040000  /* 32 argb bit/pixel,   A=31:30  R=29:20, G=19:10, B=9:0  */
#define kCGLRGB101010_A8Bit 0x00080000  /* 8-32 argb bit/pixel, A=7:0  R=29:20, G=19:10, B=9:0    */
#define kCGLRGB121212Bit    0x00100000  /* 48 rgb bit/pixel,    R=35:24, G=23:12, B=11:0          */
#define kCGLARGB12121212Bit 0x00200000  /* 48 argb bit/pixel,   A=47:36, R=35:24, G=23:12, B=11:0 */
#define kCGLRGB161616Bit    0x00400000  /* 64 rgb bit/pixel,    R=63:48, G=47:32, B=31:16         */
#define kCGLRGBA16161616Bit 0x00800000  /* 64 argb bit/pixel,   R=63:48, G=47:32, B=31:16, A=15:0 */
#define kCGLRGBFloat64Bit   0x01000000  /* 64 rgb bit/pixel,    half float                        */
#define kCGLRGBAFloat64Bit  0x02000000  /* 64 argb bit/pixel,   half float                        */
#define kCGLRGBFloat128Bit  0x04000000  /* 128 rgb bit/pixel,   ieee float                        */
#define kCGLRGBAFloat128Bit 0x08000000  /* 128 argb bit/pixel,  ieee float                        */
#define kCGLRGBFloat256Bit  0x10000000  /* 256 rgb bit/pixel,   ieee double                       */
#define kCGLRGBAFloat256Bit 0x20000000  /* 256 argb bit/pixel,  ieee double                       */

/*
** Sampling modes
*/
#define kCGLSupersampleBit 0x00000001
#define kCGLMultisampleBit 0x00000002

/* Obsolete */
#define kCGLARGB16161616Bit kCGLRGBA16161616Bit


#ifdef __cplusplus
}
#endif

#endif /* _CGLTYPES_H */
