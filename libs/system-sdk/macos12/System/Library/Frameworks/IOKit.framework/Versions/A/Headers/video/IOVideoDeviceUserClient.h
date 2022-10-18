/*
	File:		IOVideoDeviceUserClient.h
	
	Contains:	

	Copyright:	© 2006-2012 by Apple Inc., all rights reserved.
*/

#if !defined(__IOKIT_IOVIDEODEVICEUSERCLIENT_H)
#define __IOKIT_IOVIDEODEVICEUSERCLIENT_H

/*!
 @enum User client methods
 @constant kIOVideoDeviceMethodOpen
 @constant kIOVideoDeviceMethodClose
 @constant kIOVideoDeviceMethodGetMode
 @constant kIOVideoDeviceMethodSetControlValue
 @constant kIOVideoDeviceMethodOpenStream
 @constant kIOVideoDeviceMethodCloseStream
 @constant kIOVideoDeviceMethodSetStreamFormat
 @constant kIOVideoDeviceMethodStartStream
 @constant kIOVideoDeviceMethodStopStream
 @constant kIOVideoDeviceMethodSuspendStream
 
 @abstract Client method numbers used with IOConnectMethod...() functions.
 */
enum
{
    kIOVideoDeviceMethodOpen = 0,
    kIOVideoDeviceMethodClose,
    kIOVideoDeviceMethodGetMode,
    kIOVideoDeviceMethodSetMode,
    kIOVideoDeviceMethodSetControlValue,
	kIOVideoDeviceMethodOpenStream,
	kIOVideoDeviceMethodCloseStream,
    kIOVideoDeviceMethodSetStreamFormat,
    kIOVideoDeviceMethodStartStream,
    kIOVideoDeviceMethodStopStream,
    kIOVideoDeviceMethodSuspendStream,
	
	kIOVideoDeviceMethodCount
};

#ifdef KERNEL
#ifdef __cplusplus

#include <IOKit/IOUserClient.h>
#include <IOKit/stream/IOStreamShared.h>
#include <IOKit/video/IOVideoTypes.h>

class IOVideoDevice;

class IOVideoDeviceUserClient : public IOUserClient
{
// Construction/Destruction
									OSDeclareDefaultStructors(IOVideoDeviceUserClient)

// Static versions of external methods
private:
    static IOReturn					sOpen(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
    static IOReturn					sClose(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
    static IOReturn					sGetMode(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
    static IOReturn					sSetMode(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
	static IOReturn					sSetControlValue(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
    static IOReturn					sOpenStream(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
    static IOReturn					sCloseStream(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
	static IOReturn					sSetStreamFormat(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
    static IOReturn					sStartStream(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
    static IOReturn					sStopStream(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
    static IOReturn					sSuspendStream(IOVideoDeviceUserClient* target, void* reference, IOExternalMethodArguments* arguments);
	
protected:
	const IOExternalMethodDispatch*	mExternalMethods;
    IOVideoDevice*					mDevice;
    task_t							mTask;
    
// IOUserClient overriden methods
public:
    virtual bool					initWithTask(task_t owningTask, void* securityToken, UInt32 type);
	virtual bool					initWithTask(task_t owningTask, void* securityToken, UInt32 type, OSDictionary* properties);
    virtual IOReturn				clientClose(void);
    virtual IOReturn				clientDied(void);
    virtual IOService*				getService(void);
 	virtual	IOReturn				registerNotificationPort(mach_port_t port,  UInt32 portType, UInt32 refCon);
	virtual IOReturn				connectClient(IOUserClient* client);
	virtual IOReturn				externalMethod(uint32_t selector, IOExternalMethodArguments* arguments, IOExternalMethodDispatch* dispatch, OSObject* target, void* reference);

// IOService overriden methods
    virtual bool					start(IOService* provider);
    
// external methods
public:
    virtual IOReturn				open(IOOptionBits options);
    virtual IOReturn				close(void);
    virtual IOReturn				getMode(UInt32 streamIndex, IOStreamMode* mode);
    virtual IOReturn				setMode(UInt32 streamIndex, IOStreamMode mode);
	virtual	IOReturn				setControlValue(UInt32 controlID, UInt32 value, UInt32* newValue);
	virtual IOReturn				setStreamFormat(UInt32 streamID, const IOVideoStreamDescription* newStreamFormat);
    virtual IOReturn				openStream(UInt32 streamIndex);
    virtual IOReturn				closeStream(UInt32 streamIndex);
    virtual IOReturn				startStream(UInt32 streamIndex);
    virtual IOReturn				stopStream(UInt32 streamIndex);
    virtual IOReturn				suspendStream(UInt32 streamIndex);
};

#endif /* __cplusplus */
#endif /* KERNEL */

#endif
