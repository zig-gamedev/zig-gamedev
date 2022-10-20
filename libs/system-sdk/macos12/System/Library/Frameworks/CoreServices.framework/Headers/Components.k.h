/*
     File:       Components.k.h
 
     Contains:   Component Manager Interfaces.
 
     Version:    CarbonCore-515.1~16
 
     Copyright:  © 1991-2003 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __COMPONENTS_K__
#define __COMPONENTS_K__

#include <CarbonCore/Components.h>
/*
	Example usage:

		#define CALLCOMPONENT_BASENAME()	Fred
		#define CALLCOMPONENT_GLOBALS()	FredGlobalsHandle
		#include <CoreServices/Components.k.h>

	To specify that your component implementation does not use globals, do not #define CALLCOMPONENT_GLOBALS
*/
#ifdef CALLCOMPONENT_BASENAME
	#ifndef CALLCOMPONENT_GLOBALS
		#define CALLCOMPONENT_GLOBALS() 
		#define ADD_CALLCOMPONENT_COMMA 
	#else
		#define ADD_CALLCOMPONENT_COMMA ,
	#endif
	#define CALLCOMPONENT_GLUE(a,b) a##b
	#define CALLCOMPONENT_STRCAT(a,b) CALLCOMPONENT_GLUE(a,b)
	#define ADD_CALLCOMPONENT_BASENAME(name) CALLCOMPONENT_STRCAT(CALLCOMPONENT_BASENAME(),name)

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(Open) (CALLCOMPONENT_GLOBALS() ADD_CALLCOMPONENT_COMMA ComponentInstance  self);

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(Close) (CALLCOMPONENT_GLOBALS() ADD_CALLCOMPONENT_COMMA ComponentInstance  self);

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(CanDo) (CALLCOMPONENT_GLOBALS() ADD_CALLCOMPONENT_COMMA short  ftnNumber);

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(Version) (CALLCOMPONENT_GLOBALS());

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(Register) (CALLCOMPONENT_GLOBALS());

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(Target) (CALLCOMPONENT_GLOBALS() ADD_CALLCOMPONENT_COMMA ComponentInstance  target);

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(Unregister) (CALLCOMPONENT_GLOBALS());

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(GetMPWorkFunction) (CALLCOMPONENT_GLOBALS() ADD_CALLCOMPONENT_COMMA ComponentMPWorkFunctionUPP * workFunction, void ** refCon);

	EXTERN_API( ComponentResult  ) ADD_CALLCOMPONENT_BASENAME(GetPublicResource) (CALLCOMPONENT_GLOBALS() ADD_CALLCOMPONENT_COMMA OSType  resourceType, short  resourceID, Handle * resource);

#endif	/* CALLCOMPONENT_BASENAME */


#endif /* __COMPONENTS_K__ */

