/*
 *  Copyright 2006 Apple Computer, Inc. All rights reserved.
 *
 */

#if !defined(__IOKIT_IOSTREAM_H)
#define __IOKIT_IOSTREAM_H

#include <IOKit/IOService.h>
#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/IOUserClient.h>
#include <sys/cdefs.h>

#include <IOKit/stream/IOStreamShared.h>

/*!
    @header IOStream.h
    Definitions for IOStream family.
 */

class __exported IOStreamBuffer;

/*!
    @class IOStream
    @abstract
    A class representing a stream of data buffers passed from kernel to
    user space and back again.
    @discussion
    The IOStream class defines a mechanism for moving buffers of data from
    kernel space to user space or vice-versa.  The policy for which direction
    the data flows and the nature of the data is left up the the implementer
    of the driver which uses IOStream.

    Although it is expected that the client of an IOStream will be in user space,
    this is not required.
 
    References to "output" mean "from the IOStream to the user client", and
    "input" means "from the user client to the IOStream."

 */

class __exported IOStream : public IOService
{
    OSDeclareDefaultStructors(IOStream);

 protected:
    OSArray *               _buffers;       // IOStreamBuffer objects in this stream

    IOItemCount             _queueCount;    // Number of entries for the queues
    
    IOMemoryDescriptor *    _inputQueueDescriptor;
    IOMemoryMap *           _inputQueueMap;
    
    IOMemoryDescriptor *    _outputQueueDescriptor;
    IOMemoryMap *           _outputQueueMap;
    
    IOStreamBufferQueue *   _outputQueue;   // Shared memory for buffers out to user space
    IOStreamBufferQueue *   _inputQueue;    // Shared memory for buffers in from user space
    
    mach_port_t             _outputPort;
    mach_port_t             _inputPort;
    
    OSArray *               _memoryMaps;    // Maps for data queues
    
    IOStreamMode            _streamMode;
    
 public:
        
    /*!
        @functiongroup Creating IOStream objects
     */
        
    /*!
        @function withBuffers
     @param mode The initial mode of the stream, either output, input, or input/output.
     @param queueLength The nuber of queue entries to reserve in the input and output queue.
     Zero means to make the queues big enough to accommodate all the buffers at once.
     @param properties
     A dictionary of properties which will be set on the stream.
     @param buffers
     An array of IOStreamBuffer objects which will be the buffers for this stream.
     */
        
    static IOStream *withBuffers(OSArray *buffers, IOStreamMode mode = kIOStreamModeOutput, IOItemCount queueLength = 0, OSDictionary *properties = 0);
    
    virtual bool init(OSDictionary *properties = 0);
    
    /*!
        @function initWithBuffers
     @param mode The initial mode of the stream, either output, input, or input/output.
     @param queueLength The nuber of queue entries to reserve in the input and output queue.
     Zero means to make the queues big enough to accommodate all the buffers at once.
     @param properties
     A dictionary of properties which will be set on the stream.
     @param buffers
     An array of IOStreamBuffer objects which will be the buffers for this stream.
     */
    virtual bool initWithBuffers(OSArray *buffers, IOStreamMode mode = kIOStreamModeOutput, IOItemCount queueLength = 0, OSDictionary *properties = 0);

    /*!
        @function free
     */
    virtual void free(void);
    
    /*!
        @functiongroup Opening and closing streams
     */
    
    /*!
        @function handleOpen
        @abstract The handleOpen() method relies on the default IOService behavior
        to ensure that
        only one client has the stream open at a time.
        The shared input and output queues are created at open time.
     @param options
     @param arg
     */
    virtual bool handleOpen( 	IOService *	  forClient,
                                IOOptionBits	  options,
                                void *		  arg );
    
    /*!
        @function handleClose
        @abstract The handleClose method destroys the shared input and output
        queues.
     @param options
     */
    virtual void handleClose(   IOService *	  forClient,
                                IOOptionBits  options );

    /*!
        @functiongroup Managing shared queues
     */
    
    /*!
        @function createQueues
        @abstract Creates the shared input and output queues, without regard
        to whether the stream is open or not.
        Normally this is called by handleOpen().
     @param queueLength
     @param options
     @result Returns kIOReturnSuccess if the queues were successfully created.
     */
    virtual IOReturn createQueues( IOItemCount queueLength = 0, IOOptionBits options = 0 );
    
    /*!
        @function destroyQueues
        @abstract Releases the shared input and output queues.
     @result Returns kIOReturnSuccess if the queues were successfully destroyed.
     The queues cannot be destroyed while the stream is open by a client.
     */
    virtual IOReturn destroyQueues( void );
    
    /*!
        @function getOutputQueue
     @result A pointer to the output IOStreamBufferQueue structure for the stream,
     or NULL if the stream is not open and the queue has not been created yet.
     */
    virtual IOStreamBufferQueue *getOutputQueue(void);           // get shared-memory output queue
    
    /*!
        @function getOutputQueueMemoryDescriptor
     @result An IOMemoryDescriptor object repesenting the shared memory
     output queue buffer.
     */
    virtual IOMemoryDescriptor *getOutputQueueMemoryDescriptor(void);
    
    /*!
        @function getInputQueue
     @result A pointer to the input IOStreamBufferQueue structure for the stream,
     or NULL if the stream is not open and the queue has not been created yet.
     */
    virtual IOStreamBufferQueue *getInputQueue(void);            // get shared-memory input queue
    
    /*!
        @function getInputQueueMemoryDescriptor
     @result An IOMemoryDescriptor object repesenting the shared memory
     input queue buffer.
     */
    virtual IOMemoryDescriptor *getInputQueueMemoryDescriptor(void);
    
    
    /*!
        @functiongroup Managing buffers in an IOStream
     */
    
    /*!
        @function addBuffer
        @abstract Add a buffer to an IOStream.
        @param buffer
        @discussion
        Adds an IOStreamBuffer to an IOStream.  It will be added to the end
     of the buffer array, so the buffer ID of existing buffers will not change.
     */
    virtual IOReturn addBuffer(IOStreamBuffer *buffer);  // add a new buffer to the stream
    
    /*!
        @function addBuffers
     @param buffers
     @result
     */
    virtual IOReturn addBuffers(OSArray *buffers);

    /*!
        @function removeBuffer
     @param buffer A pointer to an IOStreamBuffer object in the stream.
     @result Returns kIOReturnSuccess if the buffer was removed, or kIOReturnNotFound
     if the buffer was not in this stream.
     @abstract
     Removes a buffer from the stream.  Buffers cannot be removed
     while the stream is open, as this will change the buffer IDs of existing
     buffers.
     */
    virtual IOReturn removeBuffer(IOStreamBuffer *buffer);
    
    /*!
        @function removeBuffer
     @param bufferID The ID of the buffer to remove.
     @result Returns kIOReturnSuccess if the buffer was removed.
     @abstract
     Removes a buffer from the stream.  Buffers cannot be removed
     while the stream is open, as this will change the buffer IDs of existing
     buffers.
     */
    virtual IOReturn removeBuffer(IOStreamBufferID bufferID);

    /*!
        @function removeAllBuffers
     @result
     */
    virtual IOReturn removeAllBuffers( void );
    
    /*!
        @function removeAllBuffers
     @result Returns kIOReturnSuccess if all the buffers were successfully
     removed.   Buffers cannot be removed
     while the stream is open, as this will change the buffer IDs of existing
     buffers.
     
     */
    virtual IOItemCount getBufferCount( void );
    
    /*!
        @function getBuffers
        @abstract Get an array containing all the buffers in the stream.
        @discussion
        Returns an OSArray containing all the buffers in the stream in order of their buffer ID.
     */
    virtual OSArray *getBuffers( void );
    
    
    /*!
        @function getBufferWithID
     @param bufferID
     The ID of the buffer to get.
     @result
     A pointer to an IOStreamBuffer object, or NULL if the
     buffer ID was invalid for this stream.
     */
    
    virtual IOStreamBuffer *getBufferWithID(IOStreamBufferID bufferID);

    /*!
        @functiongroup Managing notification ports
     */

    /*!
        @function getInputPort
        @abstract Get the Mach port used to receive notifications
        from user space that a buffer has been added to the input queue.
     */
    virtual mach_port_t getInputPort(void);

    /*!
        @function setInputPort
        @abstract Set the Mach port used to receive notifications from
        user space that a buffer has been added to the input queue.
        @param port
     */
    virtual IOReturn setInputPort(mach_port_t port);

    /*!
        @function getOutputPort
        @abstract Get the Mach port used to send notifications to user space
        that a buffer has been added to the output queue.
     */
    virtual mach_port_t getOutputPort(void);
    
    /*!
        @function setOutputPort
        @abstract Set the Mach port used to send notifications to user space
        that a buffer has been added to the output queue.
        @param port
     */
    virtual IOReturn setOutputPort(mach_port_t port);
    
    /*!
        @function sendOutputNotification
     @abstract Send a notification to the user client that data is available
     for reading on the output queue.  This will result in the user's output
     handler being called, if they registered one.
     @result Returns kIOReturnSuccess if the notification was successfully sent.
     */
    virtual IOReturn sendOutputNotification(void);
    
    /*!
        @functiongroup Queueing and dequeueing buffers
     */
    
    /*!
        @function enqueueOutputBuffer
        @abstract A convenience method for enqueueing a buffer.
        @param buffer
     @param dataOffset
     @param dataLength
     @param controlOffset
     @param controlLength
     @result
     */
    virtual IOReturn enqueueOutputBuffer(IOStreamBuffer *buffer,
                                         IOByteCount dataOffset = 0,
                                         IOByteCount dataLength = 0,
                                         IOByteCount controlOffset = 0,
                                         IOByteCount controlLength = 0);

    /*!
        @function enqueueOutputEntry
        @param entry
        @result
     */
    virtual IOReturn enqueueOutputEntry( IOStreamBufferQueueEntry *entry );
    
    /*!
        @function dequeueInputEntry
        @param entry
     @result
     */
    virtual IOReturn dequeueInputEntry( IOStreamBufferQueueEntry *entry );
    
    /*!
        @functiongroup Managing notifications
     */
    
    /*!
        @function inputCallback
        @abstract Input callback function to be implemented by a subclass;
        @param token
        A 32-bit token value that can be used by convention to communicate
        from user space to the stream.  The semantics of this value are
        defined by the IOStream subclass.
        @discussion
        The inputCallback() method is called as a result of a fast trap
        from user space.  It is called on the same thread as the user request,
        but the subclass should implement this call as a notification sent
        to a workloop so that the request is asynchronous.
     */
    virtual void inputCallback( UInt32 token );
    
    /*!
        @function inputSyncCallback
        @abstract Input callback function to be implemented by a subclass.
        @param token
        A 32-bit token value that can be used by convention to communicate
        from user space to the stream.  The semantics of this value are
        defined by the IOStream subclass.
        @discussion
        The inputSyncCallback() method is called as a result of a fast trap
        from user space.  It is called on the same thread as the user request,
        so no context switch is necessary.
     */
    virtual void inputSyncCallback( UInt32 token );

    /*!
        @functiongroup Stream control
     */

    /*!
        @function getStreamMode
     @abstract Returns the mode of the stream, either input or output.
     @result The mode of the stream, either kIOStreamModeInput
     (from user space to kernel space) or the default
     kIOStreamModeOutput (from kernel space to user space).
     */
    virtual IOStreamMode getStreamMode(void);

    /*!
        @function setStreamMode
     @abstract Sets the mode of the stream, either input or output.
     @discussion Subclasses may define whether it is possible
     to change the mode of a stream.
     */
    virtual IOReturn setStreamMode(IOStreamMode mode);

    /*!
        @function startStream
     @abstract Start sending data on a stream.
     @result Returns kIOReturnSuccess if the stream was successfully started.
     @discussion This must be implemented by a subclass.
     */
    virtual IOReturn startStream(void);
    
    /*!
        @function stopStream
     @abstract Stop sending data on a stream.
     @result Returns kIOReturnSuccess if the stream was successfully stopped.
     @discussion This must be implemented by a subclass.
     */
    virtual IOReturn stopStream(void);
    
    /*!
        @function suspendStream
     @abstract Temporarily suspend data flow on the stream.
     @result Returns kIOReturnSuccess if the stream was successfully suspended.
     @discussion This must be implemented by a subclass.
     */
    virtual IOReturn suspendStream(void);

    /*!
        @functiongroup Managing user clients
     */

    /*!
        @function newUserClient
     @abstract See the documentation for the IOService method newUserClient.
     */
    virtual IOReturn newUserClient( task_t owningTask, void * securityID,
                                    UInt32 type,  OSDictionary * properties,
                                    IOUserClient ** handler );


    /* Reserved slots */

    OSMetaClassDeclareReservedUnused(IOStream, 0);
    OSMetaClassDeclareReservedUnused(IOStream, 1);
    OSMetaClassDeclareReservedUnused(IOStream, 2);
    OSMetaClassDeclareReservedUnused(IOStream, 3);
    OSMetaClassDeclareReservedUnused(IOStream, 4);
    OSMetaClassDeclareReservedUnused(IOStream, 5);
    OSMetaClassDeclareReservedUnused(IOStream, 6);
    OSMetaClassDeclareReservedUnused(IOStream, 7);
    OSMetaClassDeclareReservedUnused(IOStream, 8);
    OSMetaClassDeclareReservedUnused(IOStream, 9);
    OSMetaClassDeclareReservedUnused(IOStream, 10);
    OSMetaClassDeclareReservedUnused(IOStream, 11);
    OSMetaClassDeclareReservedUnused(IOStream, 12);
    OSMetaClassDeclareReservedUnused(IOStream, 13);
    OSMetaClassDeclareReservedUnused(IOStream, 14);
    OSMetaClassDeclareReservedUnused(IOStream, 15);
    OSMetaClassDeclareReservedUnused(IOStream, 16);
    OSMetaClassDeclareReservedUnused(IOStream, 17);
    OSMetaClassDeclareReservedUnused(IOStream, 18);
    OSMetaClassDeclareReservedUnused(IOStream, 19);
    OSMetaClassDeclareReservedUnused(IOStream, 20);
    OSMetaClassDeclareReservedUnused(IOStream, 21);
    OSMetaClassDeclareReservedUnused(IOStream, 22);
    OSMetaClassDeclareReservedUnused(IOStream, 23);
    OSMetaClassDeclareReservedUnused(IOStream, 24);
    OSMetaClassDeclareReservedUnused(IOStream, 25);
    OSMetaClassDeclareReservedUnused(IOStream, 26);
    OSMetaClassDeclareReservedUnused(IOStream, 27);
    OSMetaClassDeclareReservedUnused(IOStream, 28);
    OSMetaClassDeclareReservedUnused(IOStream, 29);
    OSMetaClassDeclareReservedUnused(IOStream, 30);
    OSMetaClassDeclareReservedUnused(IOStream, 31);
};


/*!
    @class IOStreamBuffer
    A class representing a data buffer that is part of an IOStream.
 */

class __exported IOStreamBuffer : public OSObject
{
    OSDeclareDefaultStructors(IOStreamBuffer);

protected:
    IOMemoryDescriptor *     _dataBuffer;        // The data buffer is expected to be filled by hardware.
    IOMemoryDescriptor *     _controlBuffer;     // The control buffer is expected to be defined by the hardware driver.
        
    IOStreamBufferID         _bufferID;          // Client handle for this buffer.
    
    OSArray *                _clientMemoryMaps;  // Maps for clients who are sharing these buffers.
    
    SInt32                   _clientReferenceCount;    // Count of client uses of this buffer.  May be negative.
    
public:
    /*!
        @function initWithMemoryDescriptors
     */        
    virtual bool initWithMemoryDescriptors(IOMemoryDescriptor *dataBuffer,
                                           IOMemoryDescriptor *controlBuffer,
                                           IOStreamBufferID bufferID = 0);
    
    /*!
        @function withMemoryDescriptors
     */    
    static IOStreamBuffer *withMemoryDescriptors(IOMemoryDescriptor *dataBuffer,
                                                 IOMemoryDescriptor *controlBuffer,
                                                 IOStreamBufferID bufferID = 0);
    
    virtual void free(void);
    
    /*!
        @function getBufferID
        @abstract Gets the buffer identifier for the IOStreamBuffer object.
        @discussion The buffer identifier is unique across all buffers in a stream.
     */
    virtual IOStreamBufferID getBufferID(void);
    
    /*!
        @function setBufferID
        @abstract Sets the buffer identifier for the IOStreamBuffer object.
     */
    
    virtual void setBufferID( IOStreamBufferID bufferID );
    
    /*!
        @function setDataBuffer
        @abstract Sets the data buffer for the IOStreamBuffer object.
     */
    virtual void setDataBuffer(IOMemoryDescriptor *dataBuffer);
    
    /*!
        @function setControlBuffer
        @abstract Sets the control buffer for the IOStreamBuffer object.
     */
    virtual void setControlBuffer(IOMemoryDescriptor *controlBuffer);
    
    /*!
        @function getDataBuffer
     @result A pointer to the IOMemoryDescriptor for the data buffer.
     */
    virtual IOMemoryDescriptor *getDataBuffer(void);
    
    /*!
        @function getControlBuffer
     @result A pointer to the IOMemoryDescriptor for the control buffer.
     */
    virtual IOMemoryDescriptor *getControlBuffer(void);
    
    /*!
        @function getClientReferenceCount
     @result The count of client references to this buffer.  It may be positive
     or negative, depending on whether the client is sending data into the kernel,
     or the kernel is sending data out to the client.
     */
    virtual SInt32 getClientReferenceCount( void );
    
    /*!
        @function sendClientReference
     @param offset The offset in the buffer of the data sent to the client.
     @param length The length of the data sent to the client.
     @result The new client reference count.
     @abstract
     */
    virtual SInt32 sendClientReference( IOByteCount offset = 0, IOByteCount length = 0 );
    
    /*!
        @function receiveClientReference
     @param offset The offset in the buffer of the data from the client.
     @param length The length of the data from the client.
     @abstract
     */
    virtual SInt32 receiveClientReference( IOByteCount offset = 0, IOByteCount length = 0 );
    
    /* Reserved slots */
    
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 0);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 1);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 2);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 3);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 4);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 5);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 6);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 7);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 8);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 9);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 10);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 11);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 12);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 13);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 14);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 15);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 16);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 17);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 18);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 19);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 20);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 21);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 22);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 23);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 24);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 25);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 26);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 27);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 28);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 29);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 30);
    OSMetaClassDeclareReservedUnused(IOStreamBuffer, 31);
    
};

#endif /* ! __IOKIT_IOSTREAM_H */

