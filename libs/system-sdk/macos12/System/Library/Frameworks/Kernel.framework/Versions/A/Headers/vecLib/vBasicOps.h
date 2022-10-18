/*
     File:       vecLib/vBasicOps.h
 
     Contains:   Basic Algebraic Operations for AltiVec
 
     Version:    vecLib-794.0
 
     Copyright:  Copyright (c) 1999-2021 by Apple Inc. All rights reserved.
 
     Bugs:       For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __VBASICOPS__
#define __VBASICOPS__

#include <stdint.h>

#include "vecLibTypes.h"

#include <os/availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


// There are certain routines (namely vS64Add and vU64Add) with 1
// instruction implementations. There is no point in having a function
// call occur and then return after executing 1 instruction. Thus we
// introduce this define to allow for certain inline
// attributes to be defined.
#if defined __SSE2__
	#include <immintrin.h>
#elif defined (__arm64__)
	#include <arm_neon.h>
#endif // defined __SSE2__
#define __VBASICOPS_INLINE_ATTR__ __attribute__((__always_inline__, __nodebug__))

#if !defined __has_feature
	#define __has_feature(f)    0
#endif
#if __has_feature(assume_nonnull)
	_Pragma("clang assume_nonnull begin")
#else
	#define __nullable
	#define __nonnull
#endif


/*                                                                                  
  This section is a collection of algebraic functions that uses the SIMD
  instruction set, and is designed to facilitate vector processing in             
  mathematical programming. Following table indicates which functions are covered
  by SIMD instruction set and which ones are performed by vBasicOps library:

Legend:
    H/W   = Hardware
    LIB  = vBasicOps Library
    NRel  = Next Release of vBasicOps Library
    N/A   = Not Applicable

+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
| Data Type/    | U8  | S8  | U16 | S16 | U32 | S32 | U64 | S64 | U128 | S128 |
| Function      |     |     |     |     |     |     |     |     |      |      |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|    Add        | H/W | H/W | H/W | H/W | H/W | H/W | LIB | LIB | LIB  |  LIB |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|    AddS       | H/W | H/W | H/W | H/W | H/W | H/W | LIB | LIB | LIB  | LIB  |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|    Sub        | H/W | H/W | H/W | H/W | H/W | H/W | LIB | LIB | LIB  | LIB  |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|    SubS       | H/W | H/W | H/W | H/W | H/W | H/W | LIB | LIB | LIB  | LIB  |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|  Mul(Half)    | LIB | LIB | LIB | LIB | LIB | LIB | LIB | LIB | LIB  | LIB  |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|Mul Even (Full)| H/W | H/W | H/W | H/W | LIB | LIB | LIB | LIB |  N/A |  N/A |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|Mul Odd  (Full)| H/W | H/W | H/W | H/W | LIB | LIB | LIB | LIB |  N/A |  N/A |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|    Divide     | LIB | LIB | LIB | LIB | LIB | LIB | LIB |NRel | LIB  | LIB  |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|    Shift      | H/W | H/W | H/W | H/W | H/W | H/W | LIB | LIB | LIB  | LIB  |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+
|    Rotate     | H/W | H/W | H/W | H/W | H/W | H/W | LIB | LIB | LIB  | LIB  |
+---------------+-----+-----+-----+-----+-----+-----+-----+-----+------+------+



Following is a short description of functions in this section:
                                                                         
      Add:      It takes two vectors of data elements and adds each element         
                of the second vector to the corresponding element of the first      
                vector and puts the result in the associated data element of the    
                destination register.

      Subtract: It takes two vectors of data elements and subtracts each element    
                of the second vector from the corresponding element of the first    
                vector and puts the result in the associated data element of the    
                destination register.

      Multiply: It takes two vectors of data elements and multiplies each element   
                of the first vector by the corresponding element of the second      
                vector and puts the result in the associated data element of the    
                destination register. 

      Divide:   It takes two vectors of data elements and divides each element      
                of the first vector by the corresponding element of the second      
                vector and puts the result in the associated data element of the    
                destination register. A pointer is passed to the function to get   
                the remainder.

      Shift:    It takes a vector of two 64-bit data elements or one 128-bit
                data element and shifts it to right or left, in a logical or 
                algebraic manner, using a shift factor that is passed as an
                arguement to the function.

      Rotate:   It takes a vector of two 64-bit data elements or one 128-bit
                data element and rotates it to right or left, using a shift 
               factor that is passed as an arguement to the function.


   Following abbreviations are used in the names of functions in this section:   
                                                                                 
      v            Vector                                                        
      U            Unsigned                                                      
      S            Signed                                                        
      8            8-bit                                                         
      16           16-bit                                                        
      32           32-bit                                                        
      64           64-bit                                                        
      128          128-bit                                                       
      Add          Addition                                                      
      AddS         Addition with Saturation                                      
      Sub          Subtraction                                                   
      SubS         Subtraction with Saturation                                   
      Mul          Multiplication                                                
      Divide       Division                                                      
      Half         Half (multiplication, width of result is the same as width of 
                      operands)                                                  
      Full         Full (multiplication, width of result is twice width of each  
                      operand)                                                   
      Even         Multiplication is performed on EVEN data elements of vector   
                      (Please note that Big endian is used. So the left-most     
                      data element is labled as element 0)                       
      Odd          Multiplication is performed on ODD  data elements of vector.  
      A            Algebraic      
      LL           Logical Left     
      LR           Logical Right     
      Shift        Shift by one factor     
      Shift2       Shift by two factors( only apply to 64 bit operation )     
      Rotate       Rotate by one factor     
      Rotate2      Rotate by two factors( only apply to 64 bit operation )     
                                                                                 
*/
/*
 *  vU64FullMulOdd()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vUInt32 
vU64FullMulOdd(
  vUInt32   vA,
  vUInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);
/*
 *  vS64FullMulOdd()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vSInt32 
vS64FullMulOdd(
  vSInt32   vA,
  vSInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);
/*
 *  vU128Sub()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vUInt32 
vU128Sub(
  vUInt32   vA,
  vUInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);


/*
 *  vU128SubS()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vUInt32 
vU128SubS(
  vUInt32   vA,
  vUInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);
/*
 *  vS128Sub()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vSInt32 
vS128Sub(
  vSInt32   vA,
  vSInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);


/*
 *  vS64SubS()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vSInt32 
vS64SubS(
  vSInt32   vA,
  vSInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);


/*
 *  vS128SubS()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vSInt32 
vS128SubS(
  vSInt32   vA,
  vSInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);
/*
 *  vU128Add()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vUInt32 
vU128Add(
  vUInt32   vA,
  vUInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);


/*
 *  vU128AddS()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vUInt32 
vU128AddS(
  vUInt32   vA,
  vUInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);
/*
 *  vS128Add()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vSInt32 
vS128Add(
  vSInt32   vA,
  vSInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);


/*
 *  vS128AddS()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vSInt32 
vS128AddS(
  vSInt32   vA,
  vSInt32   vB) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);
/*
 *  vLL128Shift()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vUInt32 
vLL128Shift(
  vUInt32   vA,
  vUInt8    vShiftFactor) API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);
/*
 *  vLR128Shift()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vUInt32 
vLR128Shift(
  vUInt32   vA,
  vUInt8    vShiftFactor) API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);


/*
 *  vA128Shift()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in vecLib.framework
 *    CarbonLib:        not in Carbon, but vecLib is compatible with CarbonLib
 *    Non-Carbon CFM:   in vecLib 1.0 and later
 */
extern vUInt32 
vA128Shift(
  vUInt32   vA,
  vUInt8    vShiftFactor) API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);


#if __has_feature(assume_nonnull)
    _Pragma("clang assume_nonnull end")
#endif


#ifdef __cplusplus
}
#endif

#endif /* __VBASICOPS__ */
