/*
vForce.h (from vecLib-794.0)
Copyright (c) 1999-2021 by Apple Inc. All rights reserved.

@APPLE_LICENSE_HEADER_START@

This file contains Original Code and/or Modifications of Original Code
as defined in and that are subject to the Apple Public Source License
Version 2.0 (the 'License'). You may not use this file except in
compliance with the License. Please obtain a copy of the License at
http://www.opensource.apple.com/apsl/ and read it before using this
file.

The Original Code and all software distributed under the License are
distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
Please see the License for the specific language governing rights and
limitations under the License.

@APPLE_LICENSE_HEADER_END@
*/

/*! @header
 *  vForce provides fast mathematical operations on large arrays.
 *
 *  There are several differences between vForce and the similar functions
 *  available in libm. They are:
 *  1) vForce can operate on arrays of any size (libm only works on scalars,
 *     and simd.h on small fixed size vectors).
 *  2) vForce may treat some or all denormal numbers as zero.
 *  3) vForce does not guarantee to set floating point flags correctly.
 *
 *  However, unlike some fast math alternatives, vForce respects the closure of
 *  the number system, therefore infinities and NaNs are correctly processed.
 *
 *  Developers should assume that the exact value returned and treatment of
 *  denormal values will vary across different microarchitectures and versions
 *  of the operating system.
 *
 *  For very small vectors, users may wish to consider using simd.h for
 *  increased performance.
 */

#ifndef __VFORCE_H
#define __VFORCE_H


#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <os/availability.h>


#if !defined __has_feature
    #define __has_feature(f)    0
#endif
#if __has_feature(assume_nonnull)
    _Pragma("clang assume_nonnull begin")
#endif


/*! @abstract Calculates the exponential function e**x for each element of a
 *            vector.
 *
 *  @param y (output) Output vector of size *n. y[i] is set to exp(x[i]).
 *
 *  @param x (input)  Input vector of size *n.
 *
 *  @param n (input)  The number of elements in the vectors x and y.          */
void vvexpf (float * /* y */, const float * /* x */, const int * /* n */) API_AVAILABLE(macos(10.4), ios(5.0));
#if __has_feature(assume_nonnull)
    _Pragma("clang assume_nonnull end")
#endif
	
#ifdef __cplusplus
}
#endif
#endif /* __VFORCE_H */
