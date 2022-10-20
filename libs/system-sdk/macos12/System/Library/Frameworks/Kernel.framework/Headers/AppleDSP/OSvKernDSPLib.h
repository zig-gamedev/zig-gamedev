#ifndef __OSvKernDSPLib__
#define __OSvKernDSPLib__

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *	OSvKernDSPLib.h
 *
 *	Created by Paul Chang on Fri Mar 26 2004.
 *	Copyright 2004 Apple Computer. All rights reserved.
 *
 */


#include <sys/types.h>
#include <stdint.h>
typedef __darwin_ptrdiff_t ptrdiff_t;

typedef unsigned int	vUInt32 __attribute__((__vector_size__(16)));
typedef signed int		vSInt32 __attribute__((__vector_size__(16)));
typedef unsigned char	vUInt8  __attribute__((__vector_size__(16)));

// Types for 24 bit data
typedef struct { uint8_t bytes[3];} vDSP_uint24;
typedef struct { uint8_t bytes[3];} vDSP_int24;
    
void 
vDSP_conv(
	const float	vDSP_signal[],
	ptrdiff_t	vDSP_signalStride,
	const float	vDSP_filter[],
	ptrdiff_t	vDSP_strideFilter,
	float		vDSP_result[],
	ptrdiff_t	vDSP_strideResult,
	size_t		vDSP_lenResult,
	size_t		vDSP_lenFilter,
	char		*temp);

void 
vDSP_deq22(
	float       vDSP_input1[], 
	ptrdiff_t   vDSP_stride1, 
	float       vDSP_input2[], 
	float       vDSP_result[], 
	ptrdiff_t   vDSP_strideResult,
	size_t      vDSP_size);

void 
vDSP_maxmgv(
    const float vDSP_input1[],
    ptrdiff_t   vDSP_stride1,
    float       vDSP_result[],
    size_t      vDSP_size);

void 
vDSP_rmsqv(
    const float vDSP_input1[],
    ptrdiff_t	vDSP_stride1,
    float       vDSP_result[],
    size_t      vDSP_size);

void 
vDSP_svesq(
    const float	vDSP_input1[],
    ptrdiff_t	vDSP_stride1,
    float       vDSP_result[],
    size_t      vDSP_size);

void 
vDSP_svs(
	const float vDSP_input1[],
	ptrdiff_t   vDSP_stride1,
	float       vDSP_result[],
	size_t      vDSP_size);

void 
vDSP_vabs(
	const float vDSP_input1[], 
    ptrdiff_t   vDSP_stride1,
	float       vDSP_result[], 
    ptrdiff_t   vDSP_strideResult,
	size_t      vDSP_size);
                    
void 
vDSP_vadd(
	const float	vDSP_input1[],
	ptrdiff_t	vDSP_stride1,
	const float	vDSP_input2[],
	ptrdiff_t	vDSP_stride2,
	float		vDSP_result[],
	ptrdiff_t	vDSP_strideResult,
	size_t		vDSP_size);

void 
vDSP_vdiv(
	const float vDSP_input1[],
	ptrdiff_t	vDSP_stride1,
	const float vDSP_input2[],
	ptrdiff_t	vDSP_stride2,
	float		vDSP_result[],
	ptrdiff_t	vDSP_strideResult,
	size_t		vDSP_size);
    
void
vDSP_vfix32(float const *vDSP_input1, ptrdiff_t vDSP_stride1,
            int *vDSP_input2, ptrdiff_t vDSP_stride2,
            size_t vDSP_size);
    
void
vDSP_vfix16(float const *vDSP_input1, ptrdiff_t vDSP_stride1,
            int16_t *vDSP_input2, ptrdiff_t vDSP_stride2,
            size_t vDSP_size);
    
void
vDSP_vflt32(int const *vDSP_input1, ptrdiff_t vDSP_stride1,
            float *vDSP_input2, ptrdiff_t vDSP_stride2,
            size_t vDSP_size);
    
void
vDSP_vflt16(int16_t const *vDSP_input1, ptrdiff_t vDSP_stride1,
            float *vDSP_input2, ptrdiff_t vDSP_stride2,
            size_t vDSP_size);
    
void
vDSP_vsmfix24(
      const float vDSP_input1[],
      ptrdiff_t   vDSP_stride1,
      const float vDSP_input2[],
      vDSP_int24* vDSP_input3,
      ptrdiff_t	  vDSP_stride2,
      size_t	  vDSP_size);
    
void
vDSP_vsmfixu24(
    const float     vDSP_input1[],
    ptrdiff_t       vDSP_stride1,
    const float     vDSP_input2[],
    vDSP_uint24*    vDSP_input3,
    ptrdiff_t       vDSP_stride2,
    size_t          vDSP_size);
    
void
vDSP_vflt24(
    const vDSP_int24*   vDSP_input1,
    ptrdiff_t           vDSP_stride1,
    float*              vDSP_input2,
    ptrdiff_t           vDSP_stride2,
    size_t              vDSP_size);
    
void
vDSP_vfltu24(
    const vDSP_uint24*  vDSP_input1,
    ptrdiff_t           vDSP_stride1,
    float*              vDSP_input2,
    ptrdiff_t           vDSP_stride2,
    size_t              vDSP_size);
    
void
vDSP_vma(
    const float vDSP_input1[],
    ptrdiff_t   vDSP_stride1,
    const float vDSP_input2[],
    ptrdiff_t   vDSP_stride2,
    const float vDSP_input3[],
    ptrdiff_t   vDSP_stride3,
    float       vDSP_result[],
    ptrdiff_t   vDSP_strideResult,
    size_t      vDSP_size);
    
void 
vDSP_vmul(
	const float	vDSP_input1[],
	ptrdiff_t	vDSP_stride1,
	const float	vDSP_input2[],
	ptrdiff_t	vDSP_stride2,
	float		vDSP_result[],
	ptrdiff_t	vDSP_strideResult,
	size_t		vDSP_size);

void 
vDSP_vsmul(
	const float vDSP_input1[], 
    ptrdiff_t   vDSP_stride1,
	const float vDSP_input2[],
	float       vDSP_result[], 
    ptrdiff_t   vDSP_strideResult,
	size_t      vDSP_size);
        
void 
vDSP_vsub(
	const float	vDSP_input1[],
	ptrdiff_t	vDSP_stride1,
	const float	vDSP_input2[],
	ptrdiff_t	vDSP_stride2,
	float		vDSP_result[],
	ptrdiff_t	vDSP_strideResult,
	size_t		vDSP_size);

typedef struct vDSP_biquad_SetupStruct *vDSP_biquad_Setup;
    
typedef int     IIRChannel;
enum {
    vDSP_IIRStereo      = 0, // = vDSP_IIRMonoLeft && vDSP_IIRMonoRight
    vDSP_IIRMonoLeft    = 1,
    vDSP_IIRMonoRight   = 2
};
vDSP_biquad_Setup
    vDSP_biquad2_CreateSetup(const double*, const size_t, const IIRChannel);
    
void vDSP_biquad2_DestroySetup(vDSP_biquad_Setup);
    
void
    vDSP_biquad2(const struct vDSP_biquad_SetupStruct*,
                 const float*, float*, size_t);
    
void vDSP_biquad2_ResetState(vDSP_biquad_Setup);
void vDSP_biquad2_CopyState(vDSP_biquad_Setup, vDSP_biquad_Setup);

//-----------------------------------
//		vBasicOps

//// Routines from vMul.c.

vSInt32
vS64FullMulOdd(
	vSInt32   vA,
	vSInt32   vB);

vUInt32 
vU64FullMulOdd(
	vUInt32   vA,
	vUInt32   vB);

//// Routines from vSub.c.

vUInt32 
vU128Sub(
	vUInt32   vA,
	vUInt32   vB);

vUInt32 
vU128SubS(
	vUInt32   vA,
	vUInt32   vB);

vSInt32 
vS128Sub(
	vSInt32   vA,
	vSInt32   vB);

vSInt32 
vS128SubS(
	vSInt32   vA,
	vSInt32   vB);

//// Routines from vAdd.c.

vUInt32 
vU128Add(
	vUInt32   vA,
	vUInt32   vB);

vUInt32 
vU128AddS(
	vUInt32   vA,
	vUInt32   vB);

vSInt32 
vS128Add(
	vSInt32   vA,
	vSInt32   vB);

vSInt32
vS128AddS(
	vSInt32   vA,
	vSInt32   vB);

//// Routines from vShift.c.

vUInt32 
vLL128Shift(
	vUInt32   vA,
	vUInt8    vShiftFactor);

vUInt32 
vLR128Shift(
	vUInt32   vA,
	vUInt8    vShiftFactor);

vUInt32
vA128Shift(
	vUInt32   vA,
	vUInt8    vShiftFactor);

//		vBasicOps
//-----------------------------------

extern float expf(float);
extern float logf(float);
extern float log10f(float);
extern float sqrtf(float);
extern float sinf(float);
extern float cosf(float);
extern float __sinpif(float);
extern float __cospif(float);

void vvexpf (float * /* y */, const float * /* x */, const int * /* n */);

#ifdef __cplusplus
}
#endif

#endif	// __OSvKernDSPLib__
