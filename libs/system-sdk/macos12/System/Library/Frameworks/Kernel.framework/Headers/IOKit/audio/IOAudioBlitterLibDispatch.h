/*	Copyright: 	© Copyright 2011 Apple Computer, Inc. All rights reserved.
 
 Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
 ("Apple") in consideration of your agreement to the following terms, and your
 use, installation, modification or redistribution of this Apple software
 constitutes acceptance of these terms.  If you do not agree with these terms,
 please do not use, install, modify or redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and subject
 to these terms, Apple grants you a personal, non-exclusive license, under Apple’s
 copyrights in this original Apple software (the "Apple Software"), to use,
 reproduce, modify and redistribute the Apple Software, with or without
 modifications, in source and/or binary forms; provided that if you redistribute
 the Apple Software in its entirety and without modifications, you must retain
 this notice and the following text and disclaimers in all such redistributions of
 the Apple Software.  Neither the name, trademarks, service marks or logos of
 Apple Computer, Inc. may be used to endorse or promote products derived from the
 Apple Software without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or implied,
 are granted by Apple herein, including but not limited to any patent rights that
 may be infringed by your derivative works or by other works in which the Apple
 Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
 WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
 WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
 COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
 OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
 (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*!
 * @header IOAudioBlitterLibDispatch
 */

#ifndef __IOAudioBlitterLibDispatch_h__
#define __IOAudioBlitterLibDispatch_h__

#include <libkern/OSTypes.h>


#pragma mark -
#pragma mark Alternate names

// Alternate names for the functions below: these explicitly specify the endianness of the integer format instead of "native"/"swap"
#if TARGET_RT_BIG_ENDIAN
#define	IOAF_BEInt16ToFloat32	IOAF_NativeInt16ToFloat32
#define	IOAF_LEInt16ToFloat32	IOAF_SwapInt16ToFloat32
#define	IOAF_BEInt24ToFloat32	IOAF_NativeInt24ToFloat32
#define	IOAF_LEInt24ToFloat32	IOAF_SwapInt24ToFloat32
#define	IOAF_BEInt32ToFloat32	IOAF_NativeInt32ToFloat32
#define	IOAF_LEInt32ToFloat32	IOAF_SwapInt32ToFloat32

#define IOAF_Float32ToBEInt16	IOAF_Float32ToNativeInt16
#define IOAF_Float32ToLEInt16	IOAF_Float32ToSwapInt16
#define IOAF_Float32ToBEInt24	IOAF_Float32ToNativeInt24
#define IOAF_Float32ToLEInt24	IOAF_Float32ToSwapInt24
#define IOAF_Float32ToBEInt32	IOAF_Float32ToNativeInt32
#define IOAF_Float32ToLEInt32	IOAF_Float32ToSwapInt32
#else
#define	IOAF_LEInt16ToFloat32	IOAF_NativeInt16ToFloat32
#define	IOAF_BEInt16ToFloat32	IOAF_SwapInt16ToFloat32
#define	IOAF_LEInt24ToFloat32	IOAF_NativeInt24ToFloat32
#define	IOAF_BEInt24ToFloat32	IOAF_SwapInt24ToFloat32
#define	IOAF_LEInt32ToFloat32	IOAF_NativeInt32ToFloat32
#define	IOAF_BEInt32ToFloat32	IOAF_SwapInt32ToFloat32

#define IOAF_Float32ToLEInt16	IOAF_Float32ToNativeInt16
#define IOAF_Float32ToBEInt16	IOAF_Float32ToSwapInt16
#define IOAF_Float32ToLEInt24	IOAF_Float32ToNativeInt24
#define IOAF_Float32ToBEInt24	IOAF_Float32ToSwapInt24
#define IOAF_Float32ToLEInt32	IOAF_Float32ToNativeInt32
#define IOAF_Float32ToBEInt32	IOAF_Float32ToSwapInt32
#endif

/*!
 * @typedef Float32
 * @abstract Convenience type that represent a 32-bit floating point number
 */
typedef float	Float32;

/*!
 * @typedef Float64
 * @abstract Convenience type that represent a 64-bit floating point number
 */
typedef double	Float64;

#ifndef __OPEN_SOURCE__
/*!
 * @function IOAF_Int8ToFloat32
 * @abstract Converts 8-bit integer float to 32-bit float
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_Int8ToFloat32( const SInt8 *src, Float32 *dest, unsigned int count );
#endif

/*!
 * @function IOAF_NativeInt16ToFloat32
 * @abstract Converts native 16-bit integer float to 32-bit float
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_NativeInt16ToFloat32( const SInt16 *src, Float32 *dest, unsigned int count );

/*!
 * @function IOAF_SwapInt16ToFloat32
 * @abstract Converts non-native 16-bit integer float to 32-bit float
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_SwapInt16ToFloat32( const SInt16 *src, Float32 *dest, unsigned int count );

/*!
 * @function IOAF_NativeInt24ToFloat32
 * @abstract Converts native 24-bit integer float to 32-bit float
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_NativeInt24ToFloat32( const UInt8 *src, Float32 *dest, unsigned int count );

/*!
 * @function IOAF_SwapInt24ToFloat32
 * @abstract Converts non-native 24-bit integer float to 32-bit float
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_SwapInt24ToFloat32( const UInt8 *src, Float32 *dest, unsigned int count );

/*!
 * @function IOAF_NativeInt32ToFloat32
 * @abstract Converts native 32-bit integer float to 32-bit float
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_NativeInt32ToFloat32( const SInt32 *src, Float32 *dest, unsigned int count );

/*!
 * @function IOAF_SwapInt32ToFloat32
 * @abstract Converts non-native 32-bit integer float to 32-bit float
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_SwapInt32ToFloat32( const SInt32 *src, Float32 *dest, unsigned int count );

#ifndef __OPEN_SOURCE__
/*!
 * @function IOAF_Float32ToInt8
 * @abstract Converts 32-bit floating point to 8-bit integer
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_Float32ToInt8( const Float32 *src, SInt8 *dest, unsigned int count );
#endif

/*!
 * @function IOAF_Float32ToNativeInt16
 * @abstract Converts 32-bit floating point to native 16-bit integer
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_Float32ToNativeInt16( const Float32 *src, SInt16 *dest, unsigned int count );

/*!
 * @function IOAF_Float32ToSwapInt16
 * @abstract Converts 32-bit floating point to non-native 16-bit integer
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_Float32ToSwapInt16( const Float32 *src, SInt16 *dest, unsigned int count );

/*!
 * @function IOAF_Float32ToNativeInt24
 * @abstract Converts 32-bit floating point to native 24-bit integer
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_Float32ToNativeInt24( const Float32 *src, UInt8 *dest, unsigned int count );

/*!
 * @function IOAF_Float32ToSwapInt24
 * @abstract Converts 32-bit floating point to non-native 24-bit integer
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_Float32ToSwapInt24( const Float32 *src, UInt8 *dest, unsigned int count );

/*!
 * @function IOAF_Float32ToNativeInt32
 * @abstract Converts 32-bit floating point to native 32-bit integer
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_Float32ToNativeInt32( const Float32 *src, SInt32 *dest, unsigned int count );

/*!
 * @function IOAF_Float32ToSwapInt32
 * @abstract Converts 32-bit floating point to non-native 32-bit integer
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_Float32ToSwapInt32( const Float32 *src, SInt32 *dest, unsigned int count );

/*!
 * @function IOAF_bcopy_WriteCombine
 * @abstract An efficient bcopy from "write combine" memory to regular memory. It is safe to assume that all memory has been copied when the function has completed
 * @param src Pointer to the data to convert
 * @param dest Pointer to the converted data
 * @param count The number of items to convert
 */
extern void IOAF_bcopy_WriteCombine(const void *src, void *dest, unsigned int count );

#endif // __IOAudioBlitterLibDispatch_h__
