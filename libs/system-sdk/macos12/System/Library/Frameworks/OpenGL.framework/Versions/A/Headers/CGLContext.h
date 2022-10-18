/*
	Copyright:	(c) 1999-2012 Apple Inc. All rights reserved.
*/

#ifndef _CGLCONTEXT_H
#define _CGLCONTEXT_H

#include <OpenGL/gliContext.h>
#include <OpenGL/gliDispatch.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
** Opaque declaration for private CGLContext data.
*/
typedef struct _CGLPrivateObject *CGLPrivateObj;

/*
** CGLContext structure.
*/
struct _CGLContextObject {
	GLIContext            rend;
	GLIFunctionDispatch   disp;
	CGLPrivateObj         priv;
	void                 *stak;
};


#ifdef __cplusplus
}
#endif

#endif /* _CGLCONTEXT_H */
