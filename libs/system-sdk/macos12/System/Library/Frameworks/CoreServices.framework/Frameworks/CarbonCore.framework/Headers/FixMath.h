/*
     File:       CarbonCore/FixMath.h
 
     Contains:   Fixed Math Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/
#ifndef __FIXMATH__
#define __FIXMATH__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define fixed1              ((Fixed) 0x00010000L)
#define fract1              ((Fract) 0x40000000L)
#define positiveInfinity    ((Fixed)  0x7FFFFFFFL)
#define negativeInfinity    ((Fixed)  -0x80000000L)

/*  The _IntSaturate macro converts a float to an int with saturation:

        If x <= -2**31, the result is 0x80000000.
        If -2**31 < x < 2**31, the result is x truncated to an integer.
        If 2**31 <= x, the result is 0x7fffffff.
*/
#if (defined (__i386__) || defined(__x86_64__)) && __GNUC__

    // For comments, see the annotated version below.
    #define _IntSaturate(x)     ({                  \
        int _Result = (int) (x);                    \
        __asm__("                                   \
                ucomisd %[LimitFloat], %[xx]    \n  \
                cmovae  %[LimitInt], %[_Result] "   \
            :   [_Result] "+r" (_Result)            \
            :   [LimitFloat] "mx" (0x1p31),     \
                [LimitInt] "mr" (0x7fffffff),       \
                [xx] "x" ((double)(x))          \
            :   "cc"                                \
            );                                      \
         _Result; })
    /*
        // Assume result will be x.
        int _Result = (x);
        __asm__("
                // Compare x to the floating-point limit.
                ucomisd %[LimitFloat], %[xx]    \n
                // If xx is too large, set _Result to the integer limit.
                cmovae  %[LimitInt], %[_Result]
                // _Result is input and output, in a general register.
            :   [_Result] "+r" (_Result)
                // LimitFloat is 0x1p31f and may be in memory or an XMM
                // register.
            :   [LimitFloat] "mx" (0x1p31f),
                // LimitInt is 0x7fffffff and may be in memory or a general
                // register.
                [LimitInt] "mr" (0x7fffffff),
                // xx is x and must be in an XMM register.
                [xx] "x" ((double)(x))
                // The condition code is changed.
            :   "cc"
            );
        // Return _Result.
         _Result;
    */

#elif defined __ppc__ || __ppc64__ || __arm64__

    #define _IntSaturate(x) ((int) (x))

#else

    #error "Unknown architecture."
    // To use unoptimized standard C code, remove above line.
    #define _IntSaturate(x) ((x) <= -0x1p31f ? (int) -0x80000000 : \
        0x1p31f <= (x) ? (int) 0x7fffffff : (int) (x))

#endif

#define FloatToFixed(a) (_IntSaturate((a) * fixed1))
#define FloatToFract(a) (_IntSaturate((a) * fract1))

#define FixedRound(a)       ((short)(((Fixed)(a) + fixed1/2) >> 16))
#define FixedSquareRoot(a)  (((Fixed)FractSquareRoot(a) + 64) >> 7)
#define FixedTruncate(a)    ((short)((Fixed)(a) >> 16))
#define FixedToFract(a)     ((Fract)(a) << 14)
#define FractToFixed(a)     (((Fixed)(a) + 8192L) >> 14)
#define FixedToInt(a)       ((short)(((Fixed)(a) + fixed1/2) >> 16))
#define IntToFixed(a)       ((Fixed)(a) << 16)
#define FixedToFloat(a)     ((float)(a) / fixed1)
#define FractToFloat(a)     ((float)(a) / fract1)
#define ColorToFract(a)     (((Fract) (a) << 14) + ((Fract)(a) + 2 >> 2))
#define FractToColor(a)     ((gxColorValue) ((a) - ((a) >> 16) + 8191 >> 14))
/* These macros were removed because of developer complaints of variable name collision. */
//#ifndef ff    /* ff is already defined on some platforms */
//#define ff(a)            IntToFixed(a)
//#define fl(a)            FloatToFixed(a)
//#endif
/*
    FixRatio, FixMul, and FixRound were previously in ToolUtils.h
*/
/*
 *  FixRatio()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fixed 
FixRatio(
  short   numer,
  short   denom)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FixMul()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fixed 
FixMul(
  Fixed   a,
  Fixed   b)                                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FixRound()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern short 
FixRound(Fixed x)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Fix2Frac()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fract 
Fix2Frac(Fixed x)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Fix2Long()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SInt32 
Fix2Long(Fixed x)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Long2Fix()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fixed 
Long2Fix(SInt32 x)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Frac2Fix()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fixed 
Frac2Fix(Fract x)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FracMul()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fract 
FracMul(
  Fract   x,
  Fract   y)                                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FixDiv()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fixed 
FixDiv(
  Fixed   x,
  Fixed   y)                                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FracDiv()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fract 
FracDiv(
  Fract   x,
  Fract   y)                                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FracSqrt()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fract 
FracSqrt(Fract x)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FracSin()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fract 
FracSin(Fixed x)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FracCos()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fract 
FracCos(Fixed x)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  FixATan2()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fixed 
FixATan2(
  SInt32   x,
  SInt32   y)                                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
    Frac2X, Fix2X, X2Fix, and X2Frac translate to and from
    the floating point type "extended" (that's what the X is for).
    On the original Mac this was 80-bits and the functions could be
    accessed via A-Traps.  When the 68881 co-processor was added,
    it used 96-bit floating point types, so the A-Traps could not 
    be used.  When PowerPC was added, it used 64-bit floating point
    types, so yet another prototype was added.
*/
/*
 *  Frac2X()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern double 
Frac2X(Fract x)                                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Fix2X()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern double 
Fix2X(Fixed x)                                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  X2Fix()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fixed 
X2Fix(double x)                                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  X2Frac()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fract 
X2Frac(double x)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideCompare()
 *  
 *  Parameters:
 *    
 *    target:
 *      a pointer to the first wide to compare
 *    
 *    source:
 *      a pointer to the second wide to compare
 *  
 *  Result:
 *    return 0 if the value in target == the value in source ; a value
 *    < 0 if *target < *source and a value > 0 if *target > *source
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern short 
WideCompare(
  const wide *  target,
  const wide *  source)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideAdd()
 *  
 *  Discussion:
 *    Adds the value in source to target and returns target.  Note that
 *    target is updated to the new value.
 *  
 *  Parameters:
 *    
 *    target:
 *      a pointer to the value to have source added to
 *    
 *    source:
 *      a pointer to the value to be added to target
 *  
 *  Result:
 *    returns the value target
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern wide * 
WideAdd(
  wide *        target,
  const wide *  source)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideSubtract()
 *  
 *  Discussion:
 *    Subtracts the value in source from target and returns target. 
 *    Note that target is updated to the new value.
 *  
 *  Parameters:
 *    
 *    target:
 *      a pointer to the value to have source subtracted from
 *    
 *    source:
 *      a pointer to the value to be substracted from target
 *  
 *  Result:
 *    returns the value target
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern wide * 
WideSubtract(
  wide *        target,
  const wide *  source)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideNegate()
 *  
 *  Discussion:
 *    Negates the value ( twos complement ) in target and returns
 *    target.  Note that target is updated to the new value.
 *  
 *  Parameters:
 *    
 *    target:
 *  
 *  Result:
 *    returns the value target
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern wide * 
WideNegate(wide * target)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideShift()
 *  
 *  Discussion:
 *    Shift the value in target by shift bits with upwards rounding of
 *    the remainder.    Note that target is updated to the new value.
 *  
 *  Parameters:
 *    
 *    target:
 *      the value to be shifted
 *    
 *    shift:
 *      the count of bits to shift, positive values shift right and
 *      negative values shift left
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern wide * 
WideShift(
  wide *   target,
  SInt32   shift)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideSquareRoot()
 *  
 *  Discussion:
 *    Return the closest integer value to the square root for the given
 *    number.
 *  
 *  Parameters:
 *    
 *    source:
 *      the value to calculate the root for
 *  
 *  Result:
 *    the closest integer value to the square root of source
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern UInt32 
WideSquareRoot(const wide * source)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideMultiply()
 *  
 *  Discussion:
 *    Returns the wide result of multipling two SInt32 values
 *  
 *  Parameters:
 *    
 *    multiplicand:
 *    
 *    multiplier:
 *    
 *    target:
 *      a pointer to where to put the result  of multiplying
 *      multiplicand and multiplier, must not be NULL
 *  
 *  Result:
 *    the value target
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern wide * 
WideMultiply(
  SInt32   multiplicand,
  SInt32   multiplier,
  wide *   target)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideDivide()
 *  
 *  Discussion:
 *    Returns the integer and remainder results after dividing a wide
 *    value by an SInt32. Will overflow to positiveInfinity or
 *    negativeInfinity if the result won't fit into an SInt32.  If
 *    remainder is (SInt32) -1 then any overflow rounds to
 *    negativeInfinity.
 *  
 *  Parameters:
 *    
 *    dividend:
 *      the value to be divided
 *    
 *    divisor:
 *      the value to divide by
 *    
 *    remainder:
 *      a pointer to where to put the remainder result, between 0 and
 *      divisor, after dividing divident by divisor. If NULL, no
 *      remainder is returned.  If (SInt32*) -1, then any overflow
 *      result will round to negativeInfinity.
 *  
 *  Result:
 *    the integer signed result of dividend / divisor
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SInt32 
WideDivide(
  const wide *  dividend,
  SInt32        divisor,
  SInt32 *      remainder)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideWideDivide()
 *  
 *  Discussion:
 *    Returns the wide integer and remainder results after dividing a
 *    wide value by an SInt32. Note that dividend is updated with the
 *    result.
 *  
 *  Parameters:
 *    
 *    dividend:
 *      the value to be divided
 *    
 *    divisor:
 *      the value to divide by
 *    
 *    remainder:
 *      a pointer to where to put the remainder result, between 0 and
 *      divisor, after dividing divident by divisor
 *  
 *  Result:
 *    the wide result of dividend / divisor
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern wide * 
WideWideDivide(
  wide *    dividend,
  SInt32    divisor,
  SInt32 *  remainder)                                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  WideBitShift()
 *  
 *  Discussion:
 *    Shift the value in target by shift bits.  Note that target is
 *    updated with the shifted result.
 *  
 *  Parameters:
 *    
 *    target:
 *      the value to be shifted
 *    
 *    shift:
 *      the count of bits to shift, positive values shift right and
 *      negative values shift left
 *  
 *  Result:
 *    return the value target
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern wide * 
WideBitShift(
  wide *   target,
  SInt32   shift)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  UnsignedFixedMulDiv()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern UnsignedFixed 
UnsignedFixedMulDiv(
  UnsignedFixed   value,
  UnsignedFixed   multiplier,
  UnsignedFixed   divisor)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);




#ifdef __cplusplus
}
#endif

#endif /* __FIXMATH__ */

