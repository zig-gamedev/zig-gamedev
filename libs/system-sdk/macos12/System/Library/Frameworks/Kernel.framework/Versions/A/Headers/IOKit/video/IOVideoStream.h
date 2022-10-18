/*
	File:		IOVideoStream.h

	Contains:	

	Copyright:	© 2006-2012 by Apple Inc., all rights reserved.
*/

#ifndef __IOKIT_IOVIDEOSTREAM_H
#define __IOKIT_IOVIDEOSTREAM_H

#include <IOKit/stream/IOStream.h>

class IOVideoDevice;

/*!
    @class		IOVideoStream
    @abstract	A class representing a stream of video data buffers passed from kernel to user space and back again.
    @discussion	The IOVideoStream class defines a mechanism for moving buffers of data from kernel space to user space or vice-versa.  The policy for which direction the data flows and the
	nature of the data is left up the the implementer of the driver which uses IOStream.

    Although it is expected that the client of an IOVideoStream will be in user space, this is not required.
 
    References to "output" mean "from the IOVideoStream to the user client", and "input" means "from the user client to the IOVideoStream."

 */
class IOVideoStream : public IOStream
{
// Construction/Destruction
							OSDeclareDefaultStructors(IOVideoStream);

protected:
    IOStreamMode			mStreamMode;
    
public:

/*!
	@function	withBuffers
	@param		buffers
					An array of IOStreamBuffer objects which will be the buffers for this stream.
	@param		mode
					The initial mode of the video stream, either output, input, or input/output.
	@param		queueLength
					The nuber of queue entries to reserve in the input and output queue. Zero means to make the queues big enough to accommodate all the buffers at once.
	@param		properties
					A dictionary of properties which will be set on the video stream.
*/
    static IOVideoStream*	withBuffers(OSArray* buffers, IOStreamMode mode = kIOStreamModeOutput, IOItemCount queueLength = 0, OSDictionary* properties = 0);    


/*!
	@function	initWithBuffers
	@param		buffers
					An array of IOStreamBuffer objects which will be the buffers for this stream.
	@param		mode
					The initial mode of the video stream, either output, input, or input/output.
	@param		queueLength
					The nuber of queue entries to reserve in the input and output queue. Zero means to make the queues big enough to accommodate all the buffers at once.
	@param		properties
					A dictionary of properties which will be set on the video stream.
*/
    virtual bool			initWithBuffers(OSArray* buffers, IOStreamMode mode = kIOStreamModeOutput, IOItemCount queueLength = 0, OSDictionary* properties = 0);

    virtual IOVideoDevice*	getDevice(void);	// Returns provider cast to an IOVideoDevice
    
/*!
	@function	getStreamMode
	@abstract	Returns the mode of the stream, either input or output.
	@result		The mode of the stream, either kIOStreamModeInput (from user space to kernel space) or the default kIOStreamModeOutput (from kernel space to user space).
*/
    virtual IOStreamMode	getStreamMode(void);

/*!
	@function	setStreamMode
	@abstract	Sets the mode of the stream, either input or output.
*/
    virtual IOReturn		setStreamMode(IOStreamMode mode);

/*!
	@function	startStream
	@abstract	Start sending data on a stream.
	@result		Returns kIOReturnSuccess if the stream was successfully started.
*/
	virtual IOReturn		startStream(void);

/*!
	@function	stopStream
	@abstract	Stop sending data on a stream.
	@result		Returns kIOReturnSuccess if the stream was successfully started.
*/
    virtual IOReturn		stopStream(void);

/*!
	@function	suspendStream
	@abstract	Temporarily suspend data flow on the stream.
	@result		Returns kIOReturnSuccess if the stream was successfully suspended.
*/
    virtual IOReturn		suspendStream(void);

// Future Expansion
public:
    OSMetaClassDeclareReservedUnused(IOVideoStream, 0);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 1);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 2);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 3);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 4);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 5);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 6);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 7);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 8);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 9);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 10);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 11);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 12);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 13);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 14);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 15);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 16);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 17);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 18);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 19);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 20);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 21);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 22);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 23);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 24);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 25);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 26);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 27);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 28);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 29);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 30);
    OSMetaClassDeclareReservedUnused(IOVideoStream, 31);

protected:
    struct ExpansionData {};
    ExpansionData			*mReserved;
};

#endif
