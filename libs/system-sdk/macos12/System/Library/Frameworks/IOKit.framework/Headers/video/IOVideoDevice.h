/*
	File:		IOVideoDevice.h
	
	Contains:	

	Copyright:	© 2006-2013 by Apple Inc., all rights reserved.
*/

#if !defined(__IOKIT_IOVIDEODEVICE_H)
#define __IOKIT_IOVIDEODEVICE_H

#include <IOKit/IOService.h>
#include <IOKit/video/IOVideoTypes.h>
#include <IOKit/stream/IOStreamShared.h>

class IOVideoStream;

/*!
    @class		IOVideoDevice
    @abstract	A class representing a video device.
    @discussion The IOVideoDevice class defines a mechanism for accessing the IOVideoStreams that a device presents.

    Although it is expected that the client of an IOVideoDevice will be in user space, this is not required.

 */
class IOVideoDevice : public IOService
{
// Construction/Destruction
										OSDeclareDefaultStructors(IOVideoDevice);
public:
	virtual bool						init(OSDictionary* properties);
    virtual void						free(void);

/*!
	@function newUserClient
	@abstract See the documentation for the IOService method newUserClient.
*/
	virtual IOReturn					newUserClient(task_t owningTask, void* securityID,  UInt32 type, OSDictionary* properties, IOUserClient** handler);

/*!
	@function	getStreamCount
	@result		Returns the number of streams of the device.
*/
    virtual int							getStreamCount(void);
	
/*!
	@function	getStream
	@param		streamIndex
					The index for which the underlying stream is desired.
	@result		Returns the number of streams of the device.
*/
    virtual IOVideoStream*				getStream(UInt32 streamIndex);
    
/*!
	@function	startStream
	@abstract	Start sending data on a stream.
	@result		Returns kIOReturnSuccess if the stream was successfully started.
	@discussion	This must be implemented by a subclass.
*/
    virtual IOReturn					startStream(IOVideoStream* stream);

/*!
	@function	stopStream
	@abstract	Stop sending data on a stream.
	@result		Returns kIOReturnSuccess if the stream was successfully started.
	@discussion	This must be implemented by a subclass.
*/
    virtual IOReturn					stopStream(IOVideoStream* stream);

/*!
	@function	suspendStream
	@abstract	Temporarily suspend data flow on the stream.
	@result		Returns kIOReturnSuccess if the stream was successfully suspended.
	@discussion	This must be implemented by a subclass.
*/
    virtual IOReturn					suspendStream(IOVideoStream* stream);
    
/*!
	@function	setStreamMode
	@abstract	Sets the mode of the stream, either input or output.
	@discussion	This must be implemented by a subclass.
*/
    virtual IOReturn					setStreamMode(IOVideoStream* stream, IOStreamMode mode);
    virtual IOReturn					openStream(UInt32 streamIndex);
	virtual IOReturn					closeStream(UInt32 streamIndex);
    virtual IOReturn					startStream(UInt32 streamIndex);
    virtual IOReturn					stopStream(UInt32 streamIndex);
    virtual IOReturn					suspendStream(UInt32 streamIndex);
    
	
    virtual IOReturn					releaseStreams(void);
	virtual void						inputCallback(UInt32 token);
	virtual void						inputSyncCallback(UInt32 token);
    
	virtual IOReturn					registerNotificationPort(mach_port_t port, UInt32 type, UInt32 clientData);

	virtual void						sendSingleNotification(UInt32 notificationID, UInt32 objectID, UInt32 notificationArgument1, UInt32 notificationArgument2, UInt64 notificationArgument3, UInt64 notificationArgument4);
	virtual void						sendMultiNotification(UInt32 numberNotifications, const IOVideoDeviceNotification* notifications);

	virtual	IOReturn					setStreamFormat(UInt32 streamID, const IOVideoStreamDescription* newStreamFormat);

// Control methods
	virtual IOReturn					setControlValue(UInt32 controlID, UInt32 value, UInt32* newValue);
    
protected:
    virtual IOReturn					addStream(IOVideoStream* stream);
    virtual IOReturn					removeStream(UInt32 streamIndex);

    OSArray*							mStreams;
	IOVideoDeviceNotificationMessage*	mNotificationMessage;
	UInt32								mMaxNumberNotifications;
	UInt32								mOutstandingConfigChangeRequests;

// Future Expansion
public:
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 0);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 1);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 2);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 3);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 4);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 5);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 6);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 7);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 8);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 9);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 10);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 11);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 12);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 13);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 14);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 15);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 16);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 17);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 18);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 19);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 20);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 21);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 22);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 23);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 24);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 25);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 26);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 27);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 28);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 29);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 30);
    OSMetaClassDeclareReservedUnused(IOVideoDevice, 31);

protected:
    struct ExpansionData {};
    ExpansionData						*mReserved;
};

#endif
