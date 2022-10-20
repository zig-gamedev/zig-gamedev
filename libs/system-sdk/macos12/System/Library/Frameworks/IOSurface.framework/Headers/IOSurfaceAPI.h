/*
 *  IOSurfaceAPI.h
 *  IOSurface
 *
 *  Copyright 2006-2017 Apple Inc. All rights reserved.
 *
 */

#ifndef IOSURFACE_API_H
#define IOSURFACE_API_H 1

/* For legacy reasons, IOSurfaceAPI.h must also include <IOKit/IOKitLib.h> and <xpc/xpc.h> */
#include <xpc/xpc.h>
#include <IOKit/IOKitLib.h>
#include <IOSurface/IOSurfaceBase.h>
#include <IOSurface/IOSurfaceTypes.h>
#include <IOSurface/IOSurfaceRef.h>

__BEGIN_DECLS

CF_IMPLICIT_BRIDGING_ENABLED
CF_ASSUME_NONNULL_BEGIN

/* This call lets you get an xpc_object_t that holds a reference to the IOSurface.
 Note: Any live XPC objects created from an IOSurfaceRef implicity increase the IOSurface's global use
 count by one until the object is destroyed. */
xpc_object_t IOSurfaceCreateXPCObject(IOSurfaceRef aSurface) XPC_RETURNS_RETAINED API_AVAILABLE(macos(10.7), ios(11.0), watchos(4.0), tvos(11.0));
/* This call lets you take an xpc_object_t created via IOSurfaceCreatePort() and recreate an IOSurfaceRef from it. */
IOSurfaceRef _Nullable IOSurfaceLookupFromXPCObject(xpc_object_t xobj) CF_RETURNS_RETAINED API_AVAILABLE(macos(10.7), ios(11.0), watchos(4.0), tvos(11.0));

__END_DECLS

CF_ASSUME_NONNULL_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif
