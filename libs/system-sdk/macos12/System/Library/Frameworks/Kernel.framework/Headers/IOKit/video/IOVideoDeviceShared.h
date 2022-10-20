/*
	File:		IOVideoDeviceShared.h
	
	Contains:	

	Copyright:	© 2006-2012 by Apple Inc., all rights reserved.
*/

#if !defined(__IOKIT_IOVIDEODEVICESHARED_H)
#define __IOKIT_IOVIDEODEVICESHARED_H

#include <sys/cdefs.h>
#include <IOKit/IOTypes.h>

/*!
    @header IOVideoDeviceShared.h
    IOVideoDevice definitions shared between kernel and user space.
 */

__BEGIN_DECLS    
   
/*!
    @enum Mach port types
    @constant kIOVideoDevicePortTypeNotification
    @constant kIOVideoDevicePortTypeOutput
	@constant kIOVideoDevicePortTypeInput
    @abstract Port types used with IOConnectSetNotificationPort().
 */
enum
{
	kIOVideoDevicePortTypeNotification,
    kIOVideoDevicePortTypeOutput,
    kIOVideoDevicePortTypeInput
};

__END_DECLS

#endif /* ! __IOKIT_IOVIDEODEVICESHARED_H */

