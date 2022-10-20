/* CoreImage - CoreImageDefines.h
 
 Copyright (c) 2014 Apple, Inc.
 All rights reserved. */

#ifndef COREIMAGEDEFINES_H
#define COREIMAGEDEFINES_H


#include <TargetConditionals.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#if TARGET_OS_IPHONE
#include <CoreGraphics/CoreGraphics.h>
#else
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifdef __cplusplus
 #define CI_EXTERN_C_BEGIN  extern "C" {
 #define CI_EXTERN_C_END  }
#else
 #define CI_EXTERN_C_BEGIN
 #define CI_EXTERN_C_END
#endif

#ifdef __cplusplus
# define CORE_IMAGE_EXPORT extern "C" __attribute__((visibility("default")))
#else
# define CORE_IMAGE_EXPORT extern __attribute__((visibility("default")))
#endif

#define CORE_IMAGE_CLASS_EXPORT __attribute__((visibility("default")))


#ifdef CI_SILENCE_GL_DEPRECATION
  #define CI_GL_DEPRECATED_IOS(from, to)  NS_AVAILABLE_IOS(from)
  #define CI_GL_DEPRECATED_MAC(from, to)  NS_AVAILABLE_MAC(from)
  #define CI_GL_DEPRECATED(fromM,toM, fromI,toI)  NS_AVAILABLE(fromM,fromI)
  #define CIKL_DEPRECATED(fromM,toM, fromI,toI)  NS_AVAILABLE(fromM,fromI)
#else
  #define CI_GL_DEPRECATED_IOS(from, to)  NS_DEPRECATED_IOS(from, to, "Core Image OpenGLES API deprecated. (Define CI_SILENCE_GL_DEPRECATION to silence these warnings)")
  #define CI_GL_DEPRECATED_MAC(from, to)  NS_DEPRECATED_MAC(from, to, "Core Image OpenGL API deprecated. (Define CI_SILENCE_GL_DEPRECATION to silence these warnings)")
  #define CI_GL_DEPRECATED(fromM,toM, fromI,toI)  NS_DEPRECATED(fromM,toM, fromI,toI, "Core Image OpenGL API deprecated. (Define CI_SILENCE_GL_DEPRECATION to silence these warnings)")
  #define CIKL_DEPRECATED(fromM,toM, fromI,toI)  NS_DEPRECATED(fromM,toM, fromI,toI, "Core Image Kernel Language API deprecated. (Define CI_SILENCE_GL_DEPRECATION to silence these warnings)")
#endif

#if TARGET_OS_MAC || !TARGET_OS_SIMULATOR || defined(__IPHONE_13_0)
  #define COREIMAGE_SUPPORTS_IOSURFACE 1
#else
  #define COREIMAGE_SUPPORTS_IOSURFACE 0
#endif

#endif /* COREIMAGEDEFINES_H */
