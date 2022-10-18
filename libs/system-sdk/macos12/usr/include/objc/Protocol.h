/*
 * Copyright (c) 1999-2003, 2006-2007 Apple Inc.  All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
	Protocol.h
	Copyright 1991-1996 NeXT Software, Inc.
*/

#ifndef _OBJC_PROTOCOL_H_
#define _OBJC_PROTOCOL_H_

#if !__OBJC__

// typedef Protocol is here:
#include <objc/runtime.h>


#elif __OBJC2__

#include <objc/NSObject.h>

// All methods of class Protocol are unavailable. 
// Use the functions in objc/runtime.h instead.

OBJC_AVAILABLE(10.0, 2.0, 9.0, 1.0, 2.0)
@interface Protocol : NSObject
@end

#endif

#endif /* _OBJC_PROTOCOL_H_ */
