/*
     File:       CarbonCore/fp.h
 
     Contains:   FPCE Floating-Point Definitions and Declarations.
                 The contents of this header file are deprecated.
                 Use math.h instead.
 
     Copyright:  © 1987-2011 by Apple Inc. All rights reserved.
*/
#ifndef __FP__
#define __FP__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#include <math.h>
/********************************************************************************
*                                                                               *
*    A collection of numerical functions designed to facilitate a wide          *
*    range of numerical programming as required by C9X.                         *
*                                                                               *
*    The <fp.h> declares many functions in support of numerical programming.    *
*    It provides a superset of <math.h> and <SANE.h> functions.  Some           *
*    functionality previously found in <SANE.h> and not in the FPCE <fp.h>      *
*    can be found in this <fp.h> under the heading "__NOEXTENSIONS__".          *
*                                                                               *
*    All of these functions are IEEE 754 aware and treat exceptions, NaNs,      *
*    positive and negative zero and infinity consistent with the floating-      *
*    point standard.                                                            *
*                                                                               *
********************************************************************************/


#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/********************************************************************************
*                                                                               *
*                            Efficient types                                    *
*                                                                               *
*    float_t         Most efficient type at least as wide as float              *
*    double_t        Most efficient type at least as wide as double             *
*                                                                               *
*      CPU            float_t(bits)                double_t(bits)               *
*    --------        -----------------            -----------------             *
*    PowerPC          float(32)                    double(64)                   *
*    68K              long double(80/96)           long double(80/96)           *
*    x86              double(64)                   double(64)                   *
*                                                                               *
********************************************************************************/
#if TARGET_RT_MAC_MACHO
    /* math.h already defines almost everything in here.  Most of the */
    /* remainder of this file until #endif  TARGET_RT_MAC_MACHO is*/
    /* ignored. */
#elif (defined(__MWERKS__) && defined(__cmath__))
    /* many types were already defined in math.h */
#else
#if TARGET_CPU_PPC || TARGET_CPU_PPC64
typedef float                           float_t;
typedef double                          double_t;
#elif TARGET_CPU_X86 || TARGET_CPU_X86_64
typedef double                          float_t;
typedef double                          double_t;
#elif TARGET_CPU_MIPS
typedef double                          float_t;
typedef double                          double_t;
#elif TARGET_CPU_ALPHA
typedef double                          float_t;
typedef double                          double_t;
#elif TARGET_CPU_SPARC
typedef double                          float_t;
typedef double                          double_t;
#else
#error unsupported CPU
#endif  /*  */

/********************************************************************************
*                                                                               *
*                              Define some constants.                           *
*                                                                               *
*    HUGE_VAL            IEEE 754 value of infinity.                            *
*    INFINITY            IEEE 754 value of infinity.                            *
*    NAN                 A generic NaN (Not A Number).                          *
*    DECIMAL_DIG         Satisfies the constraint that the conversion from      *
*                        double to decimal and back is the identity function.   *
*                                                                               *
********************************************************************************/
#if TARGET_OS_MAC
  #if !TARGET_RT_MAC_MACHO
    #define   HUGE_VAL                __inf()
    #define   INFINITY                __inf()
    #define   NAN                     nan("255")
  #endif
#else
  #define     NAN                     sqrt(-1)
#endif

#if TARGET_CPU_PPC
  #define      DECIMAL_DIG              17 /* does not exist for double-double */
#endif      
#endif  /* (defined(__MWERKS__) && defined(__cmath__)) || (TARGET_RT_MAC_MACHO && defined(__MATH__)) */
/* MSL or math.h already defines these */
#if TARGET_RT_MAC_MACHO && defined(__MATH__)
#elif (!defined(__MWERKS__) || !defined(__cmath__))
/********************************************************************************
*                                                                               *
*                            Trigonometric functions                            *
*                                                                               *
*   acos        result is in [0,pi].                                            *
*   asin        result is in [-pi/2,pi/2].                                      *
*   atan        result is in [-pi/2,pi/2].                                      *
*   atan2       Computes the arc tangent of y/x in [-pi,pi] using the sign of   *
*               both arguments to determine the quadrant of the computed value. *
*                                                                               *
********************************************************************************/
/*
 *  cos()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  cos(double_t x);


/*
 *  sin()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  sin(double_t x);


/*
 *  tan()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  tan(double_t x);


/*
 *  acos()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  acos(double_t x);


/*
 *  asin()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  asin(double_t x);


/*
 *  atan()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  atan(double_t x);


/*
 *  atan2()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  atan2(double_t y, double_t x);




/********************************************************************************
*                                                                               *
*                              Hyperbolic functions                             *
*                                                                               *
********************************************************************************/
/*
 *  cosh()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  cosh(double_t x);


/*
 *  sinh()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  sinh(double_t x);


/*
 *  tanh()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  tanh(double_t x);


/*
 *  acosh()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  acosh(double_t x);


/*
 *  asinh()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  asinh(double_t x);


/*
 *  atanh()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  atanh(double_t x);




/********************************************************************************
*                                                                               *
*                              Exponential functions                            *
*                                                                               *
*   expm1       expm1(x) = exp(x) - 1.  But, for small enough arguments,        *
*               expm1(x) is expected to be more accurate than exp(x) - 1.       *
*   frexp       Breaks a floating-point number into a normalized fraction       *
*               and an integral power of 2.  It stores the integer in the       *
*               object pointed by *exponent.                                    *
*   ldexp       Multiplies a floating-point number by an integer power of 2.    *
*   log1p       log1p = log(1 + x). But, for small enough arguments,            *
*               log1p is expected to be more accurate than log(1 + x).          *
*   logb        Extracts the exponent of its argument, as a signed integral     *
*               value. A subnormal argument is treated as though it were first  *
*               normalized. Thus:                                               *
*                                  1   <=   x * 2^(-logb(x))   <   2            *
*   modf        Returns fractional part of x as function result and returns     *
*               integral part of x via iptr. Note C9X uses double not double_t. *
*   scalb       Computes x * 2^n efficently.  This is not normally done by      *
*               computing 2^n explicitly.                                       *
*                                                                               *
********************************************************************************/
/*
 *  exp()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  exp(double_t x);


/*
 *  expm1()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  expm1(double_t x);


/*
 *  exp2()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  exp2(double_t x);


/*
 *  frexp()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  frexp(double_t x, int *exponent);


/*
 *  ldexp()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  ldexp(double_t x, int n);


/*
 *  log()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  log(double_t x);


/*
 *  log2()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  log2(double_t x);


/*
 *  log1p()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  log1p(double_t x);


/*
 *  log10()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  log10(double_t x);


/*
 *  logb()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  logb(double_t x);


#if !TYPE_LONGDOUBLE_IS_DOUBLE
/*
 *  modfl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long double  modfl(long double x, long double *iptrl);


#endif  /* !TYPE_LONGDOUBLE_IS_DOUBLE */

/*
 *  modf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  modf(double_t x, double_t *iptr);


/*
 *  modff()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern float  modff(float x, float *iptrf);



/*
    Note: For compatiblity scalb(x,n) has n of type
            int  on Mac OS X 
            long on Mac OS
*/
typedef int                             _scalb_n_type;
/*
 *  scalb()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  scalb(double_t x, _scalb_n_type n);




/********************************************************************************
*                                                                               *
*                     Power and absolute value functions                        *
*                                                                               *
*   hypot       Computes the square root of the sum of the squares of its       *
*               arguments, without undue overflow or underflow.                 *
*   pow         Returns x raised to the power of y.  Result is more accurate    *
*               than using exp(log(x)*y).                                       *
*                                                                               *
********************************************************************************/
/*
 *  fabs()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  fabs(double_t x);


/*
 *  hypot()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  hypot(double_t x, double_t y);


/*
 *  pow()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 2.0 and later
 */
extern double_t  pow(double_t x, double_t y);


/*
 *  sqrt()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  sqrt(double_t x);




/********************************************************************************
*                                                                               *
*                        Gamma and Error functions                              *
*                                                                               *
*   erf         The error function.                                             *
*   erfc        Complementary error function.                                   *
*   gamma       The gamma function.                                             *
*   lgamma      Computes the base-e logarithm of the absolute value of          *
*               gamma of its argument x, for x > 0.                             *
*                                                                               *
********************************************************************************/
/*
 *  erf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  erf(double_t x);


/*
 *  erfc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  erfc(double_t x);


#if !defined( __STRICT_ANSI__) && !defined(_ANSI_SOURCE) && (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
/*
 *  gamma()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  gamma(double_t x);


#endif
/*
 *  lgamma()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  lgamma(double_t x);




/********************************************************************************
*                                                                               *
*                        Nearest integer functions                              *
*                                                                               *
*   rint        Rounds its argument to an integral value in floating point      *
*               format, honoring the current rounding direction.                *
*                                                                               *
*   nearbyint   Differs from rint only in that it does not raise the inexact    *
*               exception. It is the nearbyint function recommended by the      *
*               IEEE floating-point standard 854.                               *
*                                                                               *
*   rinttol     Rounds its argument to the nearest long int using the current   *
*               rounding direction.  NOTE: if the rounded value is outside      *
*               the range of long int, then the result is undefined.            *
*                                                                               *
*   round       Rounds the argument to the nearest integral value in floating   *
*               point format similar to the Fortran "anint" function. That is:  *
*               add half to the magnitude and chop.                             *
*                                                                               *
*   roundtol    Similar to the Fortran function nint or to the Pascal round.    *
*               NOTE: if the rounded value is outside the range of long int,    *
*               then the result is undefined.                                   *
*                                                                               *
*   trunc       Computes the integral value, in floating format, nearest to     *
*               but no larger in magnitude than its argument.   NOTE: on 68K    *
*               compilers when using -elems881, trunc must return an int        *
*                                                                               *
********************************************************************************/
/*
 *  ceil()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  ceil(double_t x);


/*
 *  floor()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  floor(double_t x);


/*
 *  rint()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  rint(double_t x);


/*
 *  nearbyint()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  nearbyint(double_t x);


/*
 *  rinttol()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  rinttol(double_t x);


/*
 *  round()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  round(double_t x);


/*
 *  roundtol()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  roundtol(double_t round);


typedef double_t                        _trunc_return_type;
/*
 *  trunc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern _trunc_return_type  trunc(double_t x);





/********************************************************************************
*                                                                               *
*                            Remainder functions                                *
*                                                                               *
*   remainder       IEEE 754 floating point standard for remainder.             *
*   remquo          SANE remainder.  It stores into 'quotient' the 7 low-order  *
*                   bits of the integer quotient x/y, such that:                *
*                       -127 <= quotient <= 127.                                *
*                                                                               *
********************************************************************************/
/*
 *  fmod()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  fmod(double_t x, double_t y);


/*
 *  remainder()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  remainder(double_t x, double_t y);


/*
 *  remquo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  remquo(double_t x, double_t y, int *quo);




/********************************************************************************
*                                                                               *
*                             Auxiliary functions                               *
*                                                                               *
*   copysign        Produces a value with the magnitude of its first argument   *
*                   and sign of its second argument.  NOTE: the order of the    *
*                   arguments matches the recommendation of the IEEE 754        *
*                   floating point standard,  which is opposite from the SANE   *
*                   copysign function.                                          *
*                                                                               *
*   nan             The call 'nan("n-char-sequence")' returns a quiet NaN       *
*                   with content indicated through tagp in the selected         *
*                   data type format.                                           *
*                                                                               *
*   nextafter       Computes the next representable value after 'x' in the      *
*                   direction of 'y'.  if x == y, then y is returned.           *
*                                                                               *
********************************************************************************/
/*
 *  copysign()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  copysign(double_t x, double_t y);


/*
 *  nan()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double  nan(const char * tagp);


/*
 *  nanf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern float  nanf(const char * tagp);



/*
 *  nanl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  nanl(const char * tagp);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double nanl(const char *tagp) { return (long double) nan(tagp); }
  #else
    #define nanl(tagp) ((long double) nan(tagp))
  #endif
#endif


/*
 *  nextafterd()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double  nextafterd(double x, double y);


/*
 *  nextafterf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern float  nextafterf(float x, float y);



/*
 *  nextafterl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  nextafterl(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double nextafterl(long double x, long double y) { return (long double) nextafterd((double)(x),(double)(y)); }
  #else
    #define nextafterl(x, y) ((long double) nextafterd((double)(x),(double)(y)))
  #endif
#endif




/*
 *  __fpclassifyd()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __fpclassifyd(double x);


/*
 *  __fpclassifyf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __fpclassifyf(float x);



/*
 *  __fpclassify()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long  __fpclassify(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long __fpclassify(long double x) { return __fpclassifyd((double)(x)); }
  #else
    #define __fpclassify(x) (__fpclassifyd((double)(x)))
  #endif
#endif


/*
 *  __isnormald()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __isnormald(double x);


/*
 *  __isnormalf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __isnormalf(float x);



/*
 *  __isnormal()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long  __isnormal(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long __isnormal(long double x) { return __isnormald((double)(x)); }
  #else
    #define __isnormal(x) (__isnormald((double)(x)))
  #endif
#endif



/*
 *  __isfinited()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __isfinited(double x);


/*
 *  __isfinitef()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __isfinitef(float x);



/*
 *  __isfinite()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long  __isfinite(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long __isfinite(long double x) { return __isfinited((double)(x)); }
  #else
    #define __isfinite(x) (__isfinited((double)(x)))
  #endif
#endif


/*
 *  __isnand()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __isnand(double x);


/*
 *  __isnanf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __isnanf(float x);



/*
 *  __isnan()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long  __isnan(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long __isnan(long double x) { return __isnand((double)(x)); }
  #else
    #define __isnan(x) (__isnand((double)(x)))
  #endif
#endif



/*
 *  __signbitd()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __signbitd(double x);


/*
 *  __signbitf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  __signbitf(float x);



/*
 *  __signbit()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long  __signbit(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long __signbit(long double x) { return __signbitd((double)(x)); }
  #else
    #define __signbit(x) (__signbitd((double)(x)))
  #endif
#endif


/*
 *  __inf()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  __inf(void);




/********************************************************************************
*                                                                               *
*                              Inquiry macros                                   *
*                                                                               *
*   fpclassify      Returns one of the FP_Å values.                             *
*   isnormal        Non-zero if and only if the argument x is normalized.       *
*   isfinite        Non-zero if and only if the argument x is finite.           *
*   isnan           Non-zero if and only if the argument x is a NaN.            *
*   signbit         Non-zero if and only if the sign of the argument x is       *
*                   negative.  This includes, NaNs, infinities and zeros.       *
*                                                                               *
********************************************************************************/
#define      fpclassify(x)    ( ( sizeof ( x ) == sizeof(double) ) ?           \
                              __fpclassifyd  ( x ) :                           \
                                ( sizeof ( x ) == sizeof(float) ) ?            \
                              __fpclassifyf ( x ) :                            \
                              __fpclassify  ( x ) )
#define      isnormal(x)      ( ( sizeof ( x ) == sizeof(double) ) ?           \
                              __isnormald ( x ) :                              \
                                ( sizeof ( x ) == sizeof(float) ) ?            \
                              __isnormalf ( x ) :                              \
                              __isnormal  ( x ) )
#define      isfinite(x)      ( ( sizeof ( x ) == sizeof(double) ) ?           \
                              __isfinited ( x ) :                              \
                                ( sizeof ( x ) == sizeof(float) ) ?            \
                              __isfinitef ( x ) :                              \
                              __isfinite  ( x ) )
#define      isnan(x)         ( ( sizeof ( x ) == sizeof(double) ) ?           \
                              __isnand ( x ) :                                 \
                                ( sizeof ( x ) == sizeof(float) ) ?            \
                              __isnanf ( x ) :                                 \
                              __isnan  ( x ) )
#define      signbit(x)       ( ( sizeof ( x ) == sizeof(double) ) ?           \
                              __signbitd ( x ) :                               \
                                ( sizeof ( x ) == sizeof(float) ) ?            \
                              __signbitf ( x ) :                               \
                              __signbit  ( x ) )



/********************************************************************************
*                                                                               *
*                      Max, Min and Positive Difference                         *
*                                                                               *
*   fdim        Determines the 'positive difference' between its arguments:     *
*               { x - y, if x > y }, { +0, if x <= y }.  If one argument is     *
*               NaN, then fdim returns that NaN.  if both arguments are NaNs,   *
*               then fdim returns the first argument.                           *
*                                                                               *
*   fmax        Returns the maximum of the two arguments.  Corresponds to the   *
*               max function in FORTRAN.  NaN arguments are treated as missing  *
*               data.  If one argument is NaN and the other is a number, then   *
*               the number is returned.  If both are NaNs then the first        *
*               argument is returned.                                           *
*                                                                               *
*   fmin        Returns the minimum of the two arguments.  Corresponds to the   *
*               min function in FORTRAN.  NaN arguments are treated as missing  *
*               data.  If one argument is NaN and the other is a number, then   *
*               the number is returned.  If both are NaNs then the first        *
*               argument is returned.                                           *
*                                                                               *
********************************************************************************/
/*
 *  fdim()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  fdim(double_t x, double_t y);


/*
 *  fmax()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  fmax(double_t x, double_t y);


/*
 *  fmin()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  fmin(double_t x, double_t y);


#endif /* TARGET_RT_MAC_MACHO */

/*******************************************************************************
*                                Constants                                     *
*******************************************************************************/
/*
 *  pi
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern const double_t pi                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);
/********************************************************************************
*                                                                               *
*                              Non NCEG extensions                              *
*                                                                               *
********************************************************************************/
#ifndef __NOEXTENSIONS__
/********************************************************************************
*                                                                               *
*                              Financial functions                              *
*                                                                               *
*   compound        Computes the compound interest factor "(1 + rate)^periods"  *
*                   more accurately than the straightforward computation with   *
*                   the Power function.  This is SANE's compound function.      *
*                                                                               *
*   annuity         Computes the present value factor for an annuity            *
*                   "(1 - (1 + rate)^(-periods)) /rate" more accurately than    *
*                   the straightforward computation with the Power function.    *
*                   This is SANE's annuity function.                            *
*                                                                               *
********************************************************************************/
/*
 *  compound()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double  compound(double rate, double periods)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  annuity()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double  annuity(double rate, double periods)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);




/********************************************************************************
*                                                                               *
*                              Random function                                  *
*                                                                               *
*   randomx         A pseudorandom number generator.  It uses the iteration:    *
*                               (7^5*x)mod(2^31-1)                              *
*                                                                               *
********************************************************************************/
/*
 *  randomx()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  randomx(double_t * x)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);




/*******************************************************************************
*                              Relational operator                             *
*******************************************************************************/
/*      relational operator      */
typedef short                           relop;
enum {
  GREATERTHAN                   = 0,
  LESSTHAN                      = 1,
  EQUALTO                       = 2,
  UNORDERED                     = 3
};

#if !defined(__MWERKS__) || !defined(__cmath__)
/*
 *  relation()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern relop  relation(double_t x, double_t y)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#endif /* !defined(__MWERKS__) || !defined(__cmath__) */


/********************************************************************************
*                                                                               *
*                         Binary to decimal conversions                         *
*                                                                               *
*   SIGDIGLEN   Significant decimal digits.                                     *
*                                                                               *
*   decimal     A record which provides an intermediate unpacked form for       *
*               programmers who wish to do their own parsing of numeric input   *
*               or formatting of numeric output.                                *
*                                                                               *
*   decform     Controls each conversion to a decimal string.  The style field  *
*               is either FLOATDECIMAL or FIXEDDECIMAL. If FLOATDECIMAL, the    *
*               value of the field digits is the number of significant digits.  *
*               If FIXEDDECIMAL value of the field digits is the number of      *
*               digits to the right of the decimal point.                       *
*                                                                               *
*   num2dec     Converts a double_t to a decimal record using a decform.        *
*   dec2num     Converts a decimal record d to a double_t value.                *
*   dec2str     Converts a decform and decimal to a string using a decform.     *
*   str2dec     Converts a string to a decimal struct.                          *
*   dec2d       Similar to dec2num except a double is returned (68k only).      *
*   dec2f       Similar to dec2num except a float is returned.                  *
*   dec2s       Similar to dec2num except a short is returned.                  *
*   dec2l       Similar to dec2num except a long is returned.                   *
*                                                                               *
********************************************************************************/
#if TARGET_CPU_PPC || TARGET_CPU_X86 || TARGET_CPU_PPC64 || TARGET_CPU_X86_64 || TARGET_CPU_ARM64
    #define SIGDIGLEN      36  
#endif
#define      DECSTROUTLEN   80               /* max length for dec2str output */
#define      FLOATDECIMAL   ((char)(0))
#define      FIXEDDECIMAL   ((char)(1))
struct decimal {
    char                            sgn;                        /* sign 0 for +, 1 for - */
    char                            unused;
    short                           exp;                        /* decimal exponent */
    struct {
        unsigned char                   length;
        unsigned char                   text[SIGDIGLEN];        /* significant digits */
        unsigned char                   unused;
    }                               sig;
};
typedef struct decimal decimal;

struct decform {
    char                            style;                      /*  FLOATDECIMAL or FIXEDDECIMAL */
    char                            unused;
    short                           digits;
};
typedef struct decform decform;
/*
 *  num2dec()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern void  num2dec(const decform *f, double_t x, decimal *d)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  dec2num()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern double_t  dec2num(const decimal * d)                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  dec2str()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern void  dec2str(const decform *f, const decimal *d, char *s)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  str2dec()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern void  str2dec(const char *s, short *ix, decimal *d, short *vp)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  dec2f()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern float  dec2f(const decimal * d)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  dec2s()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern short  dec2s(const decimal * d)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  dec2l()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later
 */
extern long  dec2l(const decimal * d)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#endif  /* !defined(__NOEXTENSIONS__) */

/********************************************************************************
*                                                                               *
*                         PowerPC-only Function Prototypes                      *
*                                                                               *
********************************************************************************/

#if TARGET_CPU_PPC || TARGET_CPU_PPC64
#if ! defined( __MWERKS__ ) && ! defined ( __MATH__ )   /* Metrowerks does not support double double, and math.h defines all of these */

/*
 *  cosl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  cosl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double cosl(long double x) { return (long double) cos((double)(x)); }
  #else
    #define cosl(x) ((long double) cos((double)(x)))
  #endif
#endif



/*
 *  sinl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  sinl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double sinl(long double x) { return (long double) sin((double)(x)); }
  #else
    #define sinl(x) ((long double) sin((double)(x)))
  #endif
#endif



/*
 *  tanl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  tanl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double tanl(long double x) { return (long double) tan((double)(x)); }
  #else
    #define tanl(x) ((long double) tan((double)(x)))
  #endif
#endif



/*
 *  acosl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  acosl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double acosl(long double x) { return (long double) acos((double)(x)); }
  #else
    #define acosl(x) ((long double) acos((double)(x)))
  #endif
#endif



/*
 *  asinl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  asinl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double asinl(long double x) { return (long double) asin((double)(x)); }
  #else
    #define asinl(x) ((long double) asin((double)(x)))
  #endif
#endif



/*
 *  atanl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  atanl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double atanl(long double x) { return (long double) atan((double)(x)); }
  #else
    #define atanl(x) ((long double) atan((double)(x)))
  #endif
#endif



/*
 *  atan2l()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  atan2l(long double y, long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double atan2l(long double y, long double x) { return (long double) atan2((double)(y), (double)(x)); }
  #else
    #define atan2l(y, x) ((long double) atan2((double)(y), (double)(x)))
  #endif
#endif



/*
 *  coshl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  coshl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double coshl(long double x) { return (long double) cosh((double)(x)); }
  #else
    #define coshl(x) ((long double) cosh((double)(x)))
  #endif
#endif



/*
 *  sinhl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  sinhl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double sinhl(long double x) { return (long double) sinh((double)(x)); }
  #else
    #define sinhl(x) ((long double) sinh((double)(x)))
  #endif
#endif



/*
 *  tanhl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  tanhl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double tanhl(long double x) { return (long double) tanh((double)(x)); }
  #else
    #define tanhl(x) ((long double) tanh((double)(x)))
  #endif
#endif



/*
 *  acoshl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  acoshl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double acoshl(long double x) { return (long double) acosh((double)(x)); }
  #else
    #define acoshl(x) ((long double) acosh((double)(x)))
  #endif
#endif



/*
 *  asinhl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  asinhl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double asinhl(long double x) { return (long double) asinh((double)(x)); }
  #else
    #define asinhl(x) ((long double) asinh((double)(x)))
  #endif
#endif



/*
 *  atanhl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  atanhl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double atanhl(long double x) { return (long double) atanh((double)(x)); }
  #else
    #define atanhl(x) ((long double) atanh((double)(x)))
  #endif
#endif



/*
 *  expl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  expl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double expl(long double x) { return (long double) exp((double)(x)); }
  #else
    #define expl(x) ((long double) exp((double)(x)))
  #endif
#endif



/*
 *  expm1l()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  expm1l(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double expm1l(long double x) { return (long double) expm1((double)(x)); }
  #else
    #define expm1l(x) ((long double) expm1((double)(x)))
  #endif
#endif



/*
 *  exp2l()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  exp2l(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double exp2l(long double x) { return (long double) exp2((double)(x)); }
  #else
    #define exp2l(x) ((long double) exp2((double)(x)))
  #endif
#endif



/*
 *  frexpl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  frexpl(long double x, int *exponent);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double frexpl(long double x, int *exponent) { return (long double) frexp((double)(x), (exponent)); }
  #else
    #define frexpl(x, exponent) ((long double) frexp((double)(x), (exponent)))
  #endif
#endif



/*
 *  ldexpl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  ldexpl(long double x, int n);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double ldexpl(long double x, int n) { return (long double) ldexp((double)(x), (n)); }
  #else
    #define ldexpl(x, n) ((long double) ldexp((double)(x), (n)))
  #endif
#endif



/*
 *  logl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  logl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double logl(long double x) { return (long double) log((double)(x)); }
  #else
    #define logl(x) ((long double) log((double)(x)))
  #endif
#endif



/*
 *  log1pl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  log1pl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double log1pl(long double x) { return (long double) log1p((double)(x)); }
  #else
    #define log1pl(x) ((long double) log1p((double)(x)))
  #endif
#endif



/*
 *  log10l()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  log10l(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double log10l(long double x) { return (long double) log10((double)(x)); }
  #else
    #define log10l(x) ((long double) log10((double)(x)))
  #endif
#endif



/*
 *  log2l()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  log2l(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double log2l(long double x) { return (long double) log2((double)(x)); }
  #else
    #define log2l(x) ((long double) log2((double)(x)))
  #endif
#endif



/*
 *  logbl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  logbl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double logbl(long double x) { return (long double) logb((double)(x)); }
  #else
    #define logbl(x) ((long double) logb((double)(x)))
  #endif
#endif


/*
 *  scalbl()
 *  
 * scalbl( x, n ) returns x * rn, where r is the radix of the machine's floating point
 *  arithmetic. In CarbonLib, n has been defined as an long, whereas some other systems often
 *    pass a double as the second argument.  gcc 4.0 also assumes if you use the built-in math
 * functions that scalbl() takes a double as the second argument and may show a warning
 * for those situations.
 *
 * Because of this, you'll need to define the macro below in order for fp.h to export a
 * definition of scalbl.  Or, you could just do the calculation yourself.
 *
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */

#ifdef __APPLE_FP_H_SHOULD_DEFINE_scalbl__
extern double scalbl ( double, double n );
#endif

/*
 *  fabsl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  fabsl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double fabsl(long double x) { return (long double) fabs((double)(x)); }
  #else
    #define fabsl(x) ((long double) fabs((double)(x)))
  #endif
#endif



/*
 *  hypotl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  hypotl(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double hypotl(long double x, long double y) { return (long double) hypot((double)(x), (double)(y)); }
  #else
    #define hypotl(x, y) ((long double) hypot((double)(x), (double)(y)))
  #endif
#endif



/*
 *  powl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  powl(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double powl(long double x, long double y) { return (long double) pow((double)(x), (double)(y)); }
  #else
    #define powl(x, y) ((long double) pow((double)(x), (double)(y)))
  #endif
#endif



/*
 *  sqrtl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  sqrtl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double sqrtl(long double x) { return (long double) sqrt((double)(x)); }
  #else
    #define sqrtl(x) ((long double) sqrt((double)(x)))
  #endif
#endif



/*
 *  erfl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  erfl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double erfl(long double x) { return (long double) erf((double)(x)); }
  #else
    #define erfl(x) ((long double) erf((double)(x)))
  #endif
#endif



/*
 *  erfcl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  erfcl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double erfcl(long double x) { return (long double) erfc((double)(x)); }
  #else
    #define erfcl(x) ((long double) erfc((double)(x)))
  #endif
#endif



/*
 *  gammal()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  gammal(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double gammal(long double x) { return (long double) tgamma((double)(x)); }
  #else
    #define gammal(x) ((long double) tgamma((double)(x)))
  #endif
#endif



/*
 *  lgammal()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  lgammal(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double lgammal(long double x) { return (long double) lgamma((double)(x)); }
  #else
    #define lgammal(x) ((long double) lgamma((double)(x)))
  #endif
#endif



/*
 *  ceill()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 2.0 and later or as macro/inline
 */
extern long double  ceill(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double ceill(long double x) { return (long double) ceil((double)(x)); }
  #else
    #define ceill(x) ((long double) ceil((double)(x)))
  #endif
#endif



/*
 *  floorl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  floorl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double floorl(long double x) { return (long double) floor((double)(x)); }
  #else
    #define floorl(x) ((long double) floor((double)(x)))
  #endif
#endif



/*
 *  rintl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  rintl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double rintl(long double x) { return (long double) rint((double)(x)); }
  #else
    #define rintl(x) ((long double) rint((double)(x)))
  #endif
#endif



/*
 *  nearbyintl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  nearbyintl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double nearbyintl(long double x) { return (long double) nearbyint((double)(x)); }
  #else
    #define nearbyintl(x) ((long double) nearbyint((double)(x)))
  #endif
#endif



/*
 *  rinttoll()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long  rinttoll(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long rinttoll(long double x) { return rinttol((double)(x)); }
  #else
    #define rinttoll(x) (rinttol((double)(x)))
  #endif
#endif



/*
 *  roundl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  roundl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double roundl(long double x) { return (long double) round((double)(x)); }
  #else
    #define roundl(x) ((long double) round((double)(x)))
  #endif
#endif



/*
 *  roundtoll()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long  roundtoll(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long roundtoll(long double x) { return roundtol((double)(x)); }
  #else
    #define roundtoll(x) (roundtol((double)(x)))
  #endif
#endif



/*
 *  truncl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  truncl(long double x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double truncl(long double x) { return (long double) trunc((double)(x)); }
  #else
    #define truncl(x) ((long double) trunc((double)(x)))
  #endif
#endif



/*
 *  remainderl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  remainderl(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double remainderl(long double x, long double y) { return (long double) remainder((double)(x), (double)(y)); }
  #else
    #define remainderl(x, y) ((long double) remainder((double)(x), (double)(y)))
  #endif
#endif



/*
 *  remquol()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  remquol(long double x, long double y, int *quo);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double remquol(long double x, long double y, int *quo) { return (long double) remquo((double)(x), (double)(y), (quo)); }
  #else
    #define remquol(x, y, quo) ((long double) remquo((double)(x), (double)(y), (quo)))
  #endif
#endif



/*
 *  copysignl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  copysignl(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double copysignl(long double x, long double y) { return (long double) copysign((double)(x), (double)(y)); }
  #else
    #define copysignl(x, y) ((long double) copysign((double)(x), (double)(y)))
  #endif
#endif



/*
 *  fdiml()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  fdiml(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double fdiml(long double x, long double y) { return (long double) fdim((double)(x), (double)(y)); }
  #else
    #define fdiml(x, y) ((long double) fdim((double)(x), (double)(y)))
  #endif
#endif



/*
 *  fmaxl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  fmaxl(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double fmaxl(long double x, long double y) { return (long double) fmax((double)(x), (double)(y)); }
  #else
    #define fmaxl(x, y) ((long double) fmax((double)(x), (double)(y)))
  #endif
#endif



/*
 *  fminl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  fminl(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double fminl(long double x, long double y) { return (long double) fmin((double)(x), (double)(y)); }
  #else
    #define fminl(x, y) ((long double) fmin((double)(x), (double)(y)))
  #endif
#endif


#endif /* __MWERKS__ */
#endif  /* TARGET_CPU_PPC || TARGET_CPU_PPC64 */

#ifndef __NOEXTENSIONS__
/*
 *  relationl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern relop  relationl(long double x, long double y);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline relop relationl(long double x, long double y) { return relation((double)(x), (double)(y)); }
  #else
    #define relationl(x, y) (relation((double)(x), (double)(y)))
  #endif
#endif



/*
 *  num2decl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern void  num2decl(const decform *f, long double x, decimal *d);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline void num2decl(const decform *f, long double x, decimal *d) { num2dec((f), (double)(x), (d)); }
  #else
    #define num2decl(f, x, d) (num2dec((f), (double)(x), (d)))
  #endif
#endif



/*
 *  dec2numl()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern long double  dec2numl(const decimal * d);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline long double dec2numl(const decimal *d) { return (long double) dec2num(d); }
  #else
    #define dec2numl(d) ((long double) dec2num(d))
  #endif
#endif


#endif  /* !defined(__NOEXTENSIONS__) */

#ifndef __NOEXTENSIONS__
/*    
        MathLib v2 has two new transfer functions: x80tod and dtox80.  They can 
        be used to directly transform 68k 80-bit extended data types to double
        and back for PowerPC based machines without using the functions
        x80told or ldtox80.  Double rounding may occur. 
    */
/*
 *  x80tod()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 2.0 and later
 */
extern double  x80tod(const extended80 * x80)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  dtox80()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 2.0 and later
 */
extern void  dtox80(const double *x, extended80 *x80)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);




#if __cplusplus && TYPE_LONGDOUBLE_IS_DOUBLE
#define x80told       __inline_x80told
#define ldtox80        __inline_ldtox80
#endif


/*
 *  x80told()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern void  x80told(const extended80 *x80, long double *x);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline void x80told(const extended80 *x80, long double *x) { *(x) = (long double) x80tod(x80); }
  #else
    #define x80told(x80, x) (*(x) = (long double) x80tod(x80))
  #endif
#endif



/*
 *  ldtox80()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in MathLib 1.0 and later or as macro/inline
 */
extern void  ldtox80(const long double *x, extended80 *x80);
#if TYPE_LONGDOUBLE_IS_DOUBLE
  #ifdef __cplusplus
    inline void ldtox80(const long double *x, extended80 *x80) { double d = (double) *(x); dtox80(&d, (x80)); }
  #else
    #define ldtox80(x, x80) do { double d = (double) *(x); dtox80(&d, (x80)); } while (false)
  #endif
#endif


#endif  /* !defined(__NOEXTENSIONS__) */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __FP__ */

