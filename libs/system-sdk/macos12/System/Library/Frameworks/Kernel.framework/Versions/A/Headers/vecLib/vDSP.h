/*
    File:       vecLib/vDSP.h

    Contains:   AltiVec DSP Interfaces

    Version:    vecLib-794.0

    Copyright:  Copyright (c) 2000-2021 by Apple Inc. All rights reserved.

    For vDSP documentation, search for "vDSP" at <http://developer.apple.com>
    or search for one of the routine names below.

    Some documentation for vDSP routines is provided below.

    To report bugs, please use <http://developer.apple.com/bugreporter>.
*/
#ifndef __VDSP__
#define __VDSP__


// Tell compiler this file is idempotent (no need to process it more than once).
#if PRAGMA_ONCE
    #pragma once
#endif


/*  Documentation conventions:

        Many of the routines below are documented with C-like pseudocode that
        describes what they do.  For example, vDSP_vadd is declared with:

            extern void vDSP_vadd(
                const float *__A,
                vDSP_Stride  __IA,
                const float *__B,
                vDSP_Stride  __IB,
                float       *__C,
                vDSP_Stride  __IC,
                vDSP_Length  __N)
 					API_AVAILABLE(macos(10.0), ios(4.0));

        and is described with:

            for (n = 0; n < N; ++n)
                C[n] = A[n] + B[n];

        The pseudocode uses two important simplifications:

            Names are shortened.

                The prefix "__" is removed.  This prefix is used in this
                header file so that Apple parameter names do not conflict with
                other developer macro names that might be used in source files
                that include this header, as when a program might use "#define
                N 1024" to set a preprocessor macro "N" to expand to "1024".

            Vectors are simplified by omitting strides.

                The parameters A and IA (with the prefix omitted) represent a
                vector with its elements at memory locations A[i*IA], for
                appropriate values of i.  In the pseudocode, the stride IA
                is omitted; the vector is treated as a simple mathematical
                vector with elements A[i].

                This default map is assumed for all vDSP routines unless stated
                otherwise.  An array without a stride parameter has unit
                stride.  Some routines have more complicated maps.  These are
                documented with each routine.

    Default maps:

        These default maps are used unless documented otherwise for a routine.
        For real vectors:

            Pseudocode:     Memory:
            C[n]            C[n*IC]

        For complex vectors:

            Pseudocode:     Memory:
            C[n]            C->realp[n*IC] + i * C->imagp[n*IC]

        Observe that C[n] in the pseudocode is a complex number, with a real
        component and an imaginary component.


    Pseudocode:

        The pseudo-code used to describe routines is largely C with some
        additions:

            e, pi, and i are the usual mathematical constants, approximately
            2.71828182845, 3.1415926535, and sqrt(-1).

            "**" is exponentiation.  3**4 is 81.

            Re and Im are the real and imaginary parts of a complex number.
            Re(3+4*i) is 3, and Im(3+4*i) is 4.

            sum(f(j), 0 <= j < N) is the sum of f(j) evaluated for each integer
            j from 0 (inclusive) to N (exclusive).  sum(j**2, 0 <= j < 4) is
            0 + 1 + 4 + 9 = 14.  Multiple dimensions may be used.  Thus,
            sum(f(j, k), 0 <= j < M, 0 <= k < N) is the sum of f(j, k)
            evaluated for each pair of integers (j, k) satisfying the
            constraints.

            conj(z) is the complex conjugate of z (the imaginary part is
            negated).

            |x| is the absolute value of x.

   Exactness, IEEE 754 conformance:

        vDSP routines are not expected to produce results identical to the
        pseudo-code in the descriptions, because vDSP routines are free to
        rearrange calculations for better performance.  These rearrangements
        are mathematical identities, so they would produce identical results
        if exact arithmetic were used.  However, floating-point arithmetic
        is approximate, and the rounding errors will often be different when
        operations are rearranged.

        Generally, vDSP routines are not expected to conform to IEEE 754.
        Notably, results may be not correctly rounded to the last bit even for
        elementary operations, and operations involving infinities and NaNs may
        be handled differently than IEEE 754 specifies.

    Const:

        vDSP does not modify the contents of input arrays (including input
        scalars passed by address).  If the specification of a routine does not
        state that it alters the memory that a parameter points to, then the
        routine does not alter that memory through that parameter.  (It may of
        course alter the same memory if it is also pointed to by an output
        parameter.  Such in-place operation is permitted for some vDSP routines
        and not for others.)

        Unfortunately, C semantics make it impractical to add "const" to
        pointers inside structs, because such structs are type-incompatible
        with structs containing pointers that are not const.  Thus, vDSP
        routines with complex parameters accept those parameters via
        DSPSplitComplex and DSPDoubleSplitComplex structs (among other types)
        and not via const versions of those structures.

    Strides:

        (Note:  This section introduces strides.  For an issue using strides
        with complex data, see "Complex strides" below.)

        Many vDSP routines use strides, which specify that the vector operated
        on is embedded in a larger array in memory.  Consider an array A of
        1024 elements.  Then:

            Passing a vDSP routine:     Says to operate on:

            Address A and stride 1      Each element A[j]

            Address A and stride 2      Every other element, A[j*2]

            Address A+1 and stride 2    Every other element, starting
                                        with A[1], so A[j*2+1]

        Strides may be used to operate on columns of multi-dimensional arrays.
        For example, consider a 32*64 element array, A[32][64].  Then passing
        address A+13 and stride 64 instructs vDSP to operate on the elements of
        column 13.

        When strides are used, generally there is some accompanying parameter
        that specifies the length of the operation.  This length is typically
        the number of elements to be processed, not the number in the larger
        array.  (Some vDSP routines have interactions between parameters so
        that the length may specify some number of output elements but requires
        a different numbe of input elements.  This is documented with each
        routine.)

    Complex strides:

        Strides with complex data (interleaved complex data, not split
        complex data) are complicated by a legacy issue.  Originally, complex
        data was regarded as an array of individual elements, so that memory
        containing values to represent complex numbers 2 + 3i, 4 + 5i, 6 + 7i,
        and so on, contained individual floating-point elements:

            A[0] A[1] A[2] A[3] A[4] A[5]…
             2    3    4    5    6    7  …

        This arrangement was said to have a stride of two, indicating that a
        new complex number starts every two elements.  In the modern view, we
        would regard this as an array of struct with a stride of one struct.
        Unfortunately, the vDSP interface is bound by requirements of backward
        compatibility and must retain the original use.

        Adding to this issue, a parameter is declared as a pointer to DSPComplex
        or DSPDoubleComplex (both structures of two floating-point elements),
        but its stride is still passed as a stride of floating-point elements.
        This means that, in C, to refer to complex element i of a vector C with
        stride IC, you must divide the stride by 2, using C[i*IC/2].
        Essentially, the floating-point element stride passed to the vDSP
        routine, IA, should be twice the complex-number struct stride.
*/


#include <os/availability.h>
#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif


/*  The following was reproduced from CFAvailability.h, with “CF” changed to
    “vDSP”, to provide Swift-compatible enum declarations.  CFAvailability.h
    itself is not included to creating a dependency on it in the kernel version
    of vDSP.h, which cannot include CoreFoundation headers.
*/
#define __vDSP_ENUM_GET_MACRO(_1, _2, NAME, ...) NAME
#if (__cplusplus && __cplusplus >= 201103L && (__has_extension(cxx_strong_enums) || __has_feature(objc_fixed_enum))) || (!__cplusplus && __has_feature(objc_fixed_enum))
#define __vDSP_NAMED_ENUM(_type, _name)     enum _name : _type _name; enum _name : _type
#define __vDSP_ANON_ENUM(_type)             enum : _type
#else
#define __vDSP_NAMED_ENUM(_type, _name) _type _name; enum
#define __vDSP_ANON_ENUM(_type) enum
#endif
#define vDSP_ENUM(...) __vDSP_ENUM_GET_MACRO(__VA_ARGS__, __vDSP_NAMED_ENUM, __vDSP_ANON_ENUM)(__VA_ARGS__)


#if !defined __has_feature
    #define __has_feature(f)    0
#endif
#if __has_feature(assume_nonnull)
    _Pragma("clang assume_nonnull begin")
#else
    #define __nullable
    #define __nonnull
#endif


#pragma options align=power


/*  These symbols describe the vecLib version associated with this header.

    vDSP_Version0 is a major version number.
    vDSP_Version1 is a minor version number.
*/
#define vDSP_Version0   794
#define vDSP_Version1   0


/*  Define types:

        vDSP_Length for numbers of elements in arrays and for indices of
        elements in arrays.  (It is also used for the base-two logarithm of
        numbers of elements, although a much smaller type is suitable for
        that.)

        vDSP_Stride for differences of indices of elements (which of course
        includes strides).
*/
typedef unsigned long vDSP_Length;
#if defined __arm64__ && !defined __LP64__
typedef long long     vDSP_Stride;
#else
typedef long          vDSP_Stride;
#endif

/*  A DSPComplex or DSPDoubleComplex is a pair of float or double values that
    together represent a complex value.
*/
typedef struct DSPComplex {
    float  real;
    float  imag;
} DSPComplex;
typedef struct DSPDoubleComplex {
    double real;
    double imag;
} DSPDoubleComplex;


/*  A DSPSplitComplex or DSPDoubleSplitComplex is a structure containing
    two pointers, each to an array of float or double.  These represent arrays
    of complex values, with the real components of the values stored in one
    array and the imaginary components of the values stored in a separate
    array.
*/
typedef struct DSPSplitComplex {
    float  * __nonnull realp;
    float  * __nonnull imagp;
} DSPSplitComplex;
typedef struct DSPDoubleSplitComplex {
    double * __nonnull realp;
    double * __nonnull imagp;
} DSPDoubleSplitComplex;


/*  The following statements declare a few simple types and constants used by
    various vDSP routines.
*/
typedef int FFTDirection;
typedef int FFTRadix;
enum {
    kFFTDirection_Forward         = +1,
    kFFTDirection_Inverse         = -1
};
enum {
    kFFTRadix2                    = 0,
    kFFTRadix3                    = 1,
    kFFTRadix5                    = 2
};
enum {
    vDSP_HALF_WINDOW              = 1,
    vDSP_HANN_DENORM              = 0,
    vDSP_HANN_NORM                = 2
};
    

/*  The following types define 24-bit data.
*/
typedef struct { uint8_t bytes[3]; } vDSP_uint24; // Unsigned 24-bit integer.
typedef struct { uint8_t bytes[3]; } vDSP_int24;  // Signed 24-bit integer.


/*  The following types are pointers to structures that contain data used
    inside vDSP routines to assist FFT and biquad filter operations.  The
    contents of these structures may change from release to release, so
    applications should manipulate the values only via the corresponding vDSP
    setup and destroy routines.
*/
typedef struct OpaqueFFTSetup           *FFTSetup;
typedef struct OpaqueFFTSetupD          *FFTSetupD;
typedef struct vDSP_biquad_SetupStruct  *vDSP_biquad_Setup;
typedef struct vDSP_biquad_SetupStructD *vDSP_biquad_SetupD;

    
/*  vDSP_biquadm_Setup or vDSP_biquadm_SetupD is a pointer to a filter object
    to be used with a multi-channel cascaded biquad IIR.  This object carries
    internal state which may be modified by any routine which uses it.  Upon
    creation, the state is initialized such that all delay elements are zero.
 
    Each filter object should only be used in a single thread at a time.
*/
typedef struct vDSP_biquadm_SetupStruct  *vDSP_biquadm_Setup;
typedef struct vDSP_biquadm_SetupStructD *vDSP_biquadm_SetupD;


/*  vDSP_create_fftsetup and vDSP_create_ffsetupD allocate memory and prepare
    constants used by single- and double-precision FFT routines, respectively.

    vDSP_destroy_fftsetup and vDSP_destroy_fftsetupD free the memory.  They
    may be passed a null pointer, in which case they have no effect.
*/
extern __nullable FFTSetup vDSP_create_fftsetup(
    vDSP_Length __Log2n,
    FFTRadix    __Radix)
		API_AVAILABLE(macos(10.0), ios(4.0));

extern void vDSP_destroy_fftsetup(__nullable FFTSetup __setup)
        API_AVAILABLE(macos(10.0), ios(4.0));

/*  vDSP_biquadm_CreateSetup (for float) or vDSP_biquadm_CreateSetupD (for
    double) allocates memory and prepares the coefficients for processing a
    multi-channel cascaded biquad IIR filter.  Delay values are set to zero.

    Unlike some other setup objects in vDSP, a vDSP_biquadm_Setup or
    vDSP_biquadm_SetupD contains data that is modified during a vDSP_biquadm or
    vDSP_biquadmD call, and it therefore may not be used more than once
    simultaneously, as in multiple threads.
 
    vDSP_biquadm_DestroySetup (for single) or vDSP_biquadm_DestroySetupD (for
    double) frees the memory allocated by the corresponding create-setup
    routine.
*/
extern __nullable vDSP_biquadm_Setup vDSP_biquadm_CreateSetup(
    const double *__coeffs,
    vDSP_Length   __M,
    vDSP_Length   __N)
        API_AVAILABLE(macos(10.9), ios(7.0));
extern __nullable vDSP_biquadm_SetupD vDSP_biquadm_CreateSetupD(
    const double *__coeffs,
    vDSP_Length   __M,
    vDSP_Length   __N)
        API_AVAILABLE(macos(10.10), ios(8.0));
extern void vDSP_biquadm_DestroySetup(
    vDSP_biquadm_Setup __setup)
        API_AVAILABLE(macos(10.9), ios(7.0));
extern void vDSP_biquadm_DestroySetupD(
    vDSP_biquadm_SetupD __setup)
        API_AVAILABLE(macos(10.10), ios(8.0));

/*  vDSP_biquadm_CopyState (for float) or vDSP_biquadm_CopyStateD (for double)
    copies the current state between two biquadm setup objects.  The two
    objects must have been created with the same number of channels and
    sections.
 
    vDSP_biquadm_ResetState (for float) or vDSP_biquadm_ResetStateD (for
    double) sets the delay values of a biquadm setup object to zero.
*/
extern void vDSP_biquadm_CopyState(
    vDSP_biquadm_Setup                     __dest,
    const struct vDSP_biquadm_SetupStruct *__src)
        API_AVAILABLE(macos(10.9), ios(7.0));
extern void vDSP_biquadm_CopyStateD(
    vDSP_biquadm_SetupD                     __dest,
    const struct vDSP_biquadm_SetupStructD *__src)
        API_AVAILABLE(macos(10.10), ios(8.0));
extern void vDSP_biquadm_ResetState(vDSP_biquadm_Setup __setup)
        API_AVAILABLE(macos(10.9), ios(7.0));
extern void vDSP_biquadm_ResetStateD(vDSP_biquadm_SetupD __setup)
        API_AVAILABLE(macos(10.10), ios(8.0));

/*
    vDSP_biquadm_SetCoefficientsDouble will
    update the filter coefficients within a valid vDSP_biquadm_Setup object.
 */
    
extern void vDSP_biquadm_SetCoefficientsDouble(
    vDSP_biquadm_Setup                  __setup,
    const double                       *__coeffs,
    vDSP_Length                         __start_sec,
    vDSP_Length                         __start_chn,
    vDSP_Length                         __nsec,
    vDSP_Length                         __nchn)
        API_AVAILABLE(macos(10.11), ios(9.0));
    
/*
    vDSP_biquadm_SetTargetsDouble will
    set the target coefficients within a valid vDSP_biquadm_Setup object.
 */
    
extern void vDSP_biquadm_SetTargetsDouble(
    vDSP_biquadm_Setup                  __setup,
    const double                       *__targets,
    float                               __interp_rate,
    float                               __interp_threshold,
    vDSP_Length                         __start_sec,
    vDSP_Length                         __start_chn,
    vDSP_Length                         __nsec,
    vDSP_Length                         __nchn)
        API_AVAILABLE(macos(10.11), ios(9.0));
    
/*
    vDSP_biquadm_SetCoefficientsSingle will
    update the filter coefficients within a valid vDSP_biquadm_Setup object.
 
    Coefficients are specified in single precision.
 */
    
extern void vDSP_biquadm_SetCoefficientsSingle(
    vDSP_biquadm_Setup                  __setup,
    const float                         *__coeffs,
    vDSP_Length                         __start_sec,
    vDSP_Length                         __start_chn,
    vDSP_Length                         __nsec,
    vDSP_Length                         __nchn)
        API_AVAILABLE(macos(10.11), ios(9.0));
    
/*
    vDSP_biquadm_SetTargetsSingle will
    set the target coefficients within a valid vDSP_biquadm_Setup object.
    The target values are specified in single precision.
 */
    
extern void vDSP_biquadm_SetTargetsSingle(
    vDSP_biquadm_Setup                  __setup,
    const float                        *__targets,
    float                               __interp_rate,
    float                               __interp_threshold,
    vDSP_Length                         __start_sec,
    vDSP_Length                         __start_chn,
    vDSP_Length                         __nsec,
    vDSP_Length                         __nchn)
        API_AVAILABLE(macos(10.11), ios(9.0));
/*
    vDSP_biquadm_SetActiveFilters will set the overall active/inactive filter
    state of a valid vDSP_biquadm_Setup object.
 */
extern void vDSP_biquadm_SetActiveFilters(
    vDSP_biquadm_Setup                  __setup,
    const bool                         *__filter_states)
        API_AVAILABLE(macos(10.11), ios(9.0));

// Convert a complex array to a complex-split array.
extern void vDSP_ctoz(
    const DSPComplex      *__C,
    vDSP_Stride            __IC,
    const DSPSplitComplex *__Z,
    vDSP_Stride            __IZ,
    vDSP_Length            __N)
        API_AVAILABLE(macos(10.0), ios(4.0));
    /*  Map:

            Pseudocode:     Memory:
            C[n]            C[n*IC/2].real + i * C[n*IC/2].imag
            Z[n]            Z->realp[n*IZ] + i * Z->imagp[n*IZ]

        These compute:

            for (n = 0; n < N; ++n)
                Z[n] = C[n];
    */


//  Convert a complex-split array to a complex array.
extern void vDSP_ztoc(
    const DSPSplitComplex *__Z,
    vDSP_Stride            __IZ,
    DSPComplex            *__C,
    vDSP_Stride            __IC,
    vDSP_Length            __N)
        API_AVAILABLE(macos(10.0), ios(4.0));
    /*  Map:

            Pseudocode:     Memory:
            Z[n]            Z->realp[n*IZ] + i * Z->imagp[n*IZ]
            C[n]            C[n*IC/2].real + i * C[n*IC/2].imag

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = Z[n];
    */



/*  In-place real-to-complex Discrete Fourier Transform routines, with and
    without temporary memory.  We suggest you use the DFT routines instead of
    these.
*/
extern void vDSP_fft_zrip(
    FFTSetup               __Setup,
    const DSPSplitComplex *__C,
    vDSP_Stride            __IC,
    vDSP_Length            __Log2N,
    FFTDirection           __Direction)
        API_AVAILABLE(macos(10.0), ios(4.0));
    /*  Maps:

            For this routine, strides are shown explicitly; the default maps
            are not used.

        These compute:

        N = 1 << Log2N;

        If Direction is +1, a real-to-complex transform is performed, taking
        input from a real vector that has been coerced into the complex
        structure:

            scale = 2;

            // Define a real vector, h:
            for (j = 0; j < N/2; ++j)
            {
                h[2*j + 0] = C->realp[j*IC];
                h[2*j + 1] = C->imagp[j*IC];
            }

            // Perform Discrete Fourier Transform.
            for (k = 0; k < N; ++k)
                H[k] = scale *
                    sum(h[j] * e**(-Direction*2*pi*i*j*k/N), 0 <= j < N);

            // Pack DC and Nyquist components into C->realp[0] and C->imagp[0].
            C->realp[0*IC] = Re(H[ 0 ]).
            C->imagp[0*IC] = Re(H[N/2]).

            // Store regular components:
            for (k = 1; k < N/2; ++k)
            {
                C->realp[k*IC] = Re(H[k]);
                C->imagp[k*IC] = Im(H[k]);
            }

            Note that, for N/2 < k < N, H[k] is not stored.  However, since
            the input is a real vector, the output has symmetry that allows the
            unstored elements to be derived from the stored elements:  H[k] =
            conj(H(N-k)).  This symmetry also implies the DC and Nyquist
            components are real, so their imaginary parts are zero.

        If Direction is -1, a complex-to-real inverse transform is performed,
        producing a real output vector coerced into the complex structure:

            scale = 1./N;

            // Define a complex vector, h:
            h[ 0 ] = C->realp[0*IC];
            h[N/2] = C->imagp[0*IC];
            for (j = 1; j < N/2; ++j)
            {
                h[ j ] = C->realp[j*IC] + i * C->imagp[j*IC];
                h[N-j] = conj(h[j]);
            }

            // Perform Discrete Fourier Transform.
            for (k = 0; k < N; ++k)
                H[k] = scale *
                    sum(h[j] * e**(-Direction*2*pi*i*j*k/N), 0 <= j < N);

            // Coerce real results into complex structure:
            for (k = 0; k < N/2; ++k)
            {
                C->realp[k*IC] = H[2*k+0];
                C->imagp[k*IC] = H[2*k+1];
            }

            Note that, mathematically, the symmetry in the input vector compels
            every H[k] to be real, so there are no imaginary components to be
            stored.

        Setup must have been properly created by a call to vDSP_create_fftsetup
        (for single precision) or vDSP_create_fftsetupD (for double precision)
        and not subsequently destroyed.

        Direction must be +1 or -1.

        The temporary buffer versions perform the same operation but are
        permitted to use the temporary buffer for improved performance.  Each
        of Buffer->realp and Buffer->imagp must contain N/2 * sizeof *C->realp
        bytes and is preferably 16-byte aligned or better.
    */



/*  vDSP_biquadm (for float) or vDSP_biquadmD (for double) applies a
    multi-channel biquadm IIR filter created with vDSP_biquadm_CreateSetup or
    vDSP_biquadm_CreateSetupD, respectively.
 */
extern void vDSP_biquadm(
    vDSP_biquadm_Setup       __Setup,
    const float * __nonnull * __nonnull __X, vDSP_Stride __IX,
    float       * __nonnull * __nonnull __Y, vDSP_Stride __IY,
    vDSP_Length              __N)
        API_AVAILABLE(macos(10.9), ios(7.0));
extern void vDSP_biquadmD(
     vDSP_biquadm_SetupD       __Setup,
     const double * __nonnull * __nonnull __X, vDSP_Stride __IX,
     double       * __nonnull * __nonnull __Y, vDSP_Stride __IY,
     vDSP_Length               __N)
        API_AVAILABLE(macos(10.10), ios(8.0));
    /*  These routines perform the same function as M calls to vDSP_biquad or
        vDSP_biquadD, where M, the delay values, and the biquad setups are
        derived from the biquadm setup:

            for (m = 0; m < M; ++M)
                vDSP_biquad(
                    setup derived from vDSP_biquadm setup,
                    delays derived from vDSP_biquadm setup,
                    X[m], IX,
                    Y[m], IY,
                    N);
    */


/*  Convolution and correlation.
*/
extern void vDSP_conv(
    const float *__A,  // Input signal.
    vDSP_Stride  __IA,
    const float *__F,  // Filter.
    vDSP_Stride  __IF,
    float       *__C,  // Output signal.
    vDSP_Stride  __IC,
    vDSP_Length  __N,  // Output length.
    vDSP_Length  __P)  // Filter length.
        API_AVAILABLE(macos(10.0), ios(4.0));
/*  Split-complex matrix multiply.
*/
extern void vDSP_zmmul(
    const DSPSplitComplex *__A,
    vDSP_Stride            __IA,
    const DSPSplitComplex *__B,
    vDSP_Stride            __IB,
    const DSPSplitComplex *__C,
    vDSP_Stride            __IC,
    vDSP_Length            __M,
    vDSP_Length            __N,
    vDSP_Length            __P)
        API_AVAILABLE(macos(10.2), ios(4.0));
    /*  Maps:

            Pseudocode:     Memory:
            A[m][p]         A->realp[(m*P+p)*IA] + i * A->imagp[(m*P+p)*IA].
            B[p][n]         B->realp[(p*N+n)*IB] + i * B->imagp[(p*N+n)*IB].
            C[m][n]         C->realp[(m*N+n)*IC] + i * C->imagp[(m*N+n)*IC].

        These compute:

            for (m = 0; m < M; ++m)
            for (n = 0; n < N; ++n)
                C[m][n] = sum(A[m][p] * B[p][n], 0 <= p < P);
    */


// Vector add.
extern void vDSP_vadd(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    vDSP_Stride  __IB,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.0), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[n] + B[n];
    */


// Vector subtract.
extern void vDSP_vsub(
    const float *__B,  // Caution:  A and B are swapped!
    vDSP_Stride  __IB,
    const float *__A,  // Caution:  A and B are swapped!
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.0), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[n] - B[n];
    */


// Vector multiply.
extern void vDSP_vmul(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    vDSP_Stride  __IB,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.0), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[n] * B[n];
    */


// Vector divide.
extern void vDSP_vdiv(
    const float *__B,  // Caution:  A and B are swapped!
    vDSP_Stride  __IB,
    const float *__A,  // Caution:  A and B are swapped!
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
extern void vDSP_zvdiv(
    const DSPSplitComplex *__B,    // Caution:  A and B are swapped!
    vDSP_Stride            __IB,
    const DSPSplitComplex *__A,    // Caution:  A and B are swapped!
    vDSP_Stride            __IA,
    const DSPSplitComplex *__C,
    vDSP_Stride            __IC,
    vDSP_Length            __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[n] / B[n];
    */


// Vector-scalar multiply.
extern void vDSP_vsmul(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.0), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[n] * B[0];
    */


// Vector multiply and add.
extern void vDSP_vma(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    vDSP_Stride  __IB,
    const float *__C,
    vDSP_Stride  __IC,
    float       *__D,
    vDSP_Stride  __ID,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                D[n] = A[n] * B[n] + C[n];
    */


// Complex multiplication with optional conjugation.
extern void vDSP_zvmul(
    const DSPSplitComplex *__A,
    vDSP_Stride            __IA,
    const DSPSplitComplex *__B,
    vDSP_Stride            __IB,
    const DSPSplitComplex *__C,
    vDSP_Stride            __IC,
    vDSP_Length            __N,
    int                    __Conjugate)
        API_AVAILABLE(macos(10.0), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            If Conjugate is +1:

                for (n = 0; n < N; ++n)
                    C[n] = A[n] * B[n];

            If Conjugate is -1:

                for (n = 0; n < N; ++n)
                    C[n] = conj(A[n]) * B[n];
    */



// Vector absolute value.
extern void vDSP_vabs(
    const float *__A,
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
extern void vDSP_zvabs(
    const DSPSplitComplex *__A,
    vDSP_Stride            __IA,
    float                 *__C,
    vDSP_Stride            __IC,
    vDSP_Length            __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = |A[n]|;
    */


// Vector fill.
extern void vDSP_vfill(
    const float *__A,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[0];
    */


// Vector-scalar add.
extern void vDSP_vsadd(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[n] + B[0];
    */


// Complex-split vector move.
extern void vDSP_zvmov(
    const DSPSplitComplex *__A,
    vDSP_Stride            __IA,
    const DSPSplitComplex *__C,
    vDSP_Stride            __IC,
    vDSP_Length            __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[n];
    */



// Difference equation, 2 poles, 2 zeros.
extern void vDSP_deq22(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 2; n < N+2; ++n)   // Note outputs start with C[2].
                C[n] =
                    + A[n-0]*B[0]
                    + A[n-1]*B[1]
                    + A[n-2]*B[2]
                    - C[n-1]*B[3]
                    - C[n-2]*B[4];
    */

// Maximum magnitude of vector.
extern void vDSP_maxmgv(
    const float *__A,
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        C[0] is set to the greatest value of |A[n]| for 0 <= n < N.
    */

// Maximum value of vector.
extern void vDSP_maxv(
    const float *__A,
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        C[0] is set to the greatest value of A[n] for 0 <= n < N.
    */


// Minimum value of vector.
extern void vDSP_minv(
    const float *__A,
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        C[0] is set to the least value of A[n] for 0 <= n < N.
    */



// Root-mean-square of vector.
extern void vDSP_rmsqv(
    const float *__A,
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            C[0] = sqrt(sum(A[n] ** 2, 0 <= n < N) / N);
    */


// Scalar-vector divide.
extern void vDSP_svdiv(
    const float *__A,
    const float *__B,
    vDSP_Stride  __IB,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = A[0] / B[n];

        When A[0] is not zero or NaN and B[n] is zero, C[n] is set to an
        infinity.
    */


// Sum of vector elements.
extern void vDSP_sve(
    const float *__A,
    vDSP_Stride  __I,
    float       *__C,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            C[0] = sum(A[n], 0 <= n < N);
    */



// Sum of vector elements' squares.
extern void vDSP_svesq(
    const float *__A,
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            C[0] = sum(A[n] ** 2, 0 <= n < N);
    */


// Sum of vector elements and sum of vector elements' squares.
extern void vDSP_sve_svesq(
    const float  *__A,
    vDSP_Stride   __IA,
    float        *__Sum,
    float        *__SumOfSquares,
    vDSP_Length   __N)
        API_AVAILABLE(macos(10.8), ios(6.0));
    /*  Maps:  The default maps are used.

        These compute:

            Sum[0]          = sum(A[n],      0 <= n < N);
            SumOfSquares[0] = sum(A[n] ** 2, 0 <= n < N);
    */


// Sum of vector elements' signed squares.
extern void vDSP_svs(
    const float *__A,
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            C[0] = sum(A[n] * |A[n]|, 0 <= n < N);
    */

// Vector clip.
extern void vDSP_vclip(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    const float *__C,
    float       *__D,
    vDSP_Stride  __ID,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
            {
                D[n] = A[n];
                if (D[n] < B[0]) D[n] = B[0];
                if (C[0] < D[n]) D[n] = C[0];
            }
    */


// Vector clear.
extern void vDSP_vclr(
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = 0;
    */


// Vector convert to decibels, power, or amplitude.
extern void vDSP_vdbcon(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N,
    unsigned int __F)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            If Flag is 1:
                alpha = 20;
            If Flag is 0:
                alpha = 10;

            for (n = 0; n < N; ++n)
                C[n] = alpha * log10(A[n] / B[0]);
    */


// Vector maxima.
extern void vDSP_vmax(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    vDSP_Stride  __IB,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = B[n] <= A[n] ? A[n] : B[n];
    */


// Vector maximum magnitude.
extern void vDSP_vmaxmg(
    const float *__A,
    vDSP_Stride  __IA,
    const float *__B,
    vDSP_Stride  __IB,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N)
        API_AVAILABLE(macos(10.4), ios(4.0));
    /*  Maps:  The default maps are used.

        These compute:

            for (n = 0; n < N; ++n)
                C[n] = |B[n]| <= |A[n]| ? |A[n]| : |B[n]|;
    */


// Vector sliding window maxima.
extern void vDSP_vswmax(
    const float *__A,
    vDSP_Stride  __IA,
    float       *__C,
    vDSP_Stride  __IC,
    vDSP_Length  __N,
    vDSP_Length  __WindowLength)
        API_AVAILABLE(macos(10.10), ios(8.0));
    /*  Maps:  The default maps are used.

        These compute the maximum value within a window to the input vector.
        A maximum is calculated for each window position:

            for (n = 0; n < N; ++n)
                C[n] = the greatest value of A[w] for n <= w < n+WindowLength.

        A must contain N+WindowLength-1 elements, and C must contain space for
        N+WindowLength-1 elements.  Although only N outputs are provided in C,
        the additional elements may be used for intermediate computation.

        A and C may not overlap.

        WindowLength must be positive (zero is not supported).
    */


#ifndef USE_NON_APPLE_STANDARD_DATATYPES
#define USE_NON_APPLE_STANDARD_DATATYPES 1
#endif  /* !defined(USE_NON_APPLE_STANDARD_DATATYPES) */

#if USE_NON_APPLE_STANDARD_DATATYPES
enum {
    FFT_FORWARD = kFFTDirection_Forward,
    FFT_INVERSE = kFFTDirection_Inverse
};

enum {
    FFT_RADIX2  = kFFTRadix2,
    FFT_RADIX3  = kFFTRadix3,
    FFT_RADIX5  = kFFTRadix5
};

typedef DSPComplex                      COMPLEX;
typedef DSPSplitComplex                 COMPLEX_SPLIT;
typedef DSPDoubleComplex                DOUBLE_COMPLEX;
typedef DSPDoubleSplitComplex           DOUBLE_COMPLEX_SPLIT;
#endif  /* USE_NON_APPLE_STANDARD_DATATYPES */


#pragma options align=reset


#if __has_feature(assume_nonnull)
    _Pragma("clang assume_nonnull end")
#endif


#ifdef __cplusplus
}
#endif


#endif // __VDSP__
