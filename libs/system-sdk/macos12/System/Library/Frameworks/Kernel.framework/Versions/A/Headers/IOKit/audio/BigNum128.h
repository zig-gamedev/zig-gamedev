/*
 * Copyright (c) 1998-2012 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef __BIGNUM128_H__
#define __BIGNUM128_H__

#include <libkern/OSTypes.h>
#include <stdint.h>

class U128
{
public:
	U128(uint64_t _lo = 0) : lo(_lo), hi(0)				{ };
	U128(uint64_t _hi, uint64_t _lo)	: lo(_lo), hi(_hi)	{ };
	inline bool operator==( const U128 &A ) const	 	{ return ( A.hi == hi ) && ( A.lo == lo ); }
	inline bool operator>( const U128 &A ) const		{ return ( ( A.hi > hi ) || ( ( A.hi == hi ) && ( A.lo > lo ) ) ); }
	inline bool operator<( const U128 &A ) const 		{ return !( ( A.hi > hi ) || ( ( A.hi == hi ) && ( A.lo > lo ) ) ); }

	U128 operator++( int )
	{
		if ( ++lo==0 )
			hi++;

		return *this;
	}
	
	U128 operator--( int )
	{
		if ( 0 == lo-- )
		{
			hi--;
		}
		return *this;
	}

	U128& operator=( const U128 &A )
	{
		hi = A.hi;
		lo = A.lo;
		
		return *this;
	}
	
	U128 operator+( const U128 &A ) const
	{
		U128	result(A.hi + hi, A.lo + lo);
		
		if ( ( result.lo < A.lo ) || ( result.lo < lo ) )
		{
			result.hi++;
		}
		
		return result;
	}
	
	U128& operator+=( const U128 &A )
	{
		U128	result(A.hi + hi, A.lo + lo);
		
		if ( ( result.lo < A.lo ) || ( result.lo < lo ) )
		{
			result.hi++;
		}
		
		*this = result;

		return *this;
	}

	friend U128 operator-( const U128 &A, const U128 &B )		// assumes A >= B
	{
		U128 C = A;

		C.hi -= B.hi;
		C.lo -= B.lo;

		if ( C.lo > A.lo )		 // borrow ?
		{
			C.hi--;
		}

		return C;
	}

	
	friend U128 operator<<( const U128& A, int n )
	{
		U128 res = A;
		
		while ( n-- )
		{
			res.hi <<= 1;
			res.hi |= ( ( res.lo & MSB64 ) ? 1 : 0 );
			res.lo <<= 1;
		}
		
		return res;
	}
	
	friend U128 operator>>( const U128& A, int n )
	{
		U128 res = A;

		while ( n-- )
		{
			res.lo >>= 1;
			res.lo |= ( ( res.hi & 0x1 ) ? MSB64 : 0 );
			res.hi >>= 1;
		}
		
		return res;
	}

public:

#ifdef __BIG_ENDIAN__
	uint64_t		hi;
	uint64_t		lo;
#else
	uint64_t		lo;
	uint64_t		hi;
#endif

private:
	enum { MSB64 = 0x8000000000000000ULL };
};

extern U128 UInt64mult(const uint64_t A, const uint64_t B);

#endif			//__BIGNUM128_H__





