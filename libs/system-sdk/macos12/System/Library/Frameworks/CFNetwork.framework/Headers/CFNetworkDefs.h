/*
	 File:	   CFNetwork/CFNetworkDefs.h
 
	 Contains:   CoreFoundation Network header
 
	 Copyright:  Copyright (c) 2001-2013 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/

#ifndef __CFNETWORK_DEFS_H_
#define __CFNETWORK_DEFS_H_


// To pick-up the CALLBACK_API_C definition.
#include <ConditionalMacros.h>
#include <CoreFoundation/CoreFoundation.h>

/* Standard incantation for exporting/importing DLL symbols */
#if defined(__WIN32__)
  #if defined(CFNETWORK_BUILDING_DLL)
		#define CFN_EXPORT __declspec(dllexport) extern
 #else
	  #define CFN_EXPORT __declspec(dllimport) extern
 #endif
#else
	#define CFN_EXPORT extern __attribute__((__visibility__("default")))
#endif

#endif //__CFNETWORK_DEFS_H_


