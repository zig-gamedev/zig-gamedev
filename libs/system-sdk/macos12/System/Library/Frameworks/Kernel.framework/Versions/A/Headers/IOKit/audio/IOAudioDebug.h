/*
 * Copyright (c) 1998-2014 Apple Computer, Inc. All rights reserved.
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

#ifndef _IOAUDIODEBUG_H
#define _IOAUDIODEBUG_H

#include <IOKit/IOTypes.h>
#include <sys/kdebug.h>


#ifdef DEBUG
	#define DEBUG_LEVEL 4						//<rdar://problem/9725460>
	//#define DEBUG_USE_FIRELOG 1
	#define DEBUG_USE_FIREWIRE_KPRINTF 1
	
	#ifdef DEBUG_USE_FIRELOG
	#include <IOKit/firewire/FireLog.h>
	#define audioDebugIOLog( level, message... ) \
		do {FireLog(  message ); FireLog("\n");} while (0)
    #define audioErrorIOLog( message... ) \
        do { FireLog( message ); FireLog("\n"); IOLog( message );} while (0)
	#endif

	#ifdef DEBUG_USE_IOUSBLOG
	#include <IOKit/usb/IOUSBLog.h>
	#define audioDebugIOLog( level, message... ) \
		do {USBLog( level, message );} while (0)
    #define audioErrorIOLog( level, message... ) \
        do { USBLog( DEBUG_LEVEL_BETA, message ); IOLog( message );} while (0)
	#endif

	#ifdef DEBUG_USE_FIREWIRE_KPRINTF
	#define audioDebugIOLog( level, message... ) \
		do { if (level <= DEBUG_LEVEL) kprintf( message );} while (0)
    #define audioErrorIOLog( message... ) \
        do { kprintf( message ); IOLog( message );} while (0)
	#endif

	#ifdef assert
		#undef assert

		#define AssertionMessage( cond, file, line ) \
			"assert \"" #cond "\" failed in " #file " at line " #line

		#define AssertionFailed( cond, file, line ) \
			panic(AssertionMessage( cond, file, line ));

		#define	assert( cond )								\
			if( !(cond) ) {									\
				AssertionFailed( cond, __FILE__, __LINE__ )	\
			}
	#endif
#else
    #define DEBUG_LEVEL 3

    #include <os/log.h>
    #define audioDebugIOLog( level, message... ) \
        do { if ( __builtin_expect(level <= DEBUG_LEVEL, 0) ) { os_log( OS_LOG_DEFAULT, message ); } } while (0)

    #define audioErrorIOLog( message... ) \
        do { os_log_error( OS_LOG_DEFAULT, message ); IOLog( message );} while (0)

#endif


#endif /* _IOAUDIODEBUG_H */
