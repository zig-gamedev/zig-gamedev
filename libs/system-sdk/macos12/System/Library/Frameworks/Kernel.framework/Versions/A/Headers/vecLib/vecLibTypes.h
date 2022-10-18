/*
     File:       vecLib/vecLibTypes.h
 
     Contains:   Master include for vecLib framework
 
     Version:    vecLib-794.0
 
     Copyright:  Copyright (c) 2000-2021 by Apple Inc. All rights reserved.
 
     Bugs:       For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
     Various types are defined here to use with vecLib.  For more generally
     useful vector/SIMD types, please consider the headers in /usr/include/simd.
*/
#ifndef __VECLIBTYPES__
#define __VECLIBTYPES__

#include <os/availability.h>

#if PRAGMA_ONCE
#pragma once
#endif


	typedef unsigned char       vUInt8  __attribute__((__vector_size__(16)));
	typedef signed char         vSInt8  __attribute__((__vector_size__(16)));
	typedef unsigned short      vUInt16 __attribute__((__vector_size__(16)));
	typedef signed short        vSInt16 __attribute__((__vector_size__(16)));
	typedef unsigned int        vUInt32 __attribute__((__vector_size__(16)));
	typedef signed int          vSInt32 __attribute__((__vector_size__(16)));
    typedef long long           vSInt64 __attribute__((__vector_size__(16)));
    typedef unsigned long long  vUInt64 __attribute__((__vector_size__(16)));
	typedef float               vFloat  __attribute__((__vector_size__(16)));
	typedef double              vDouble __attribute__((__vector_size__(16)));
	typedef unsigned int        vBool32 __attribute__((__vector_size__(16)));


#endif /* __VECLIBTYPES__ */
