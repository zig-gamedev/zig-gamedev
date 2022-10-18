/*
 *  Copyright 2006 Apple Computer, Inc. All rights reserved.
 *
 */

#if !defined(__IOKIT_IOSTREAMLIB_H)
#define __IOKIT_IOSTREAMLIB_H

/*!
    @header IOStreamLib.h
    @abstract IOCFPlugin library for using IOStream objects.
    @discussion
    The IOStream plugin provides a convenient set of functions for accessing and manipulating
    IOStream objects from user programs.
 */

#include <sys/cdefs.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <CoreFoundation/CoreFoundation.h>

#include <IOKit/stream/IOStreamShared.h>

__BEGIN_DECLS    
    
#pragma mark IOStream UUIDs

//   uuid: 57C89438-A0A2-4AFC-9FF2-DE64C02F815C

#define kIOStreamLibTypeID \
    CFUUIDGetConstantUUIDWithBytes( kCFAllocatorDefault, \
                                    0x57, 0xC8, 0x94, 0x38, 0xA0, 0xA2, 0x4A, 0xFC, \
                                    0x9F, 0xF2, 0xDE, 0x64, 0xC0, 0x2F, 0x81, 0x5C \
                                    )
    
// kIOStreamInterfaceID_v1
//   uuid: E2DFDF5F-3FAD-4487-9EEA-3FCF147EBB3D 
#define kIOStreamInterfaceID_v1	\
    CFUUIDGetConstantUUIDWithBytes( kCFAllocatorDefault, \
                                    0xE2, 0xDF, 0xDF, 0x5F, 0x3F, 0xAD, 0x44, 0x87, \
                                    0x9E, 0xEA, 0x3F, 0xCF, 0x14, 0x7E, 0xBB, 0x3D \
                                    )

#define kIOStreamInterfaceID kIOStreamInterfaceID_v1

/*!
@typedef IOStreamRef
 */
typedef struct IOStreamInterface_v1_t ** IOStreamRef;


/*!
    @typedef IOStreamCallback
 */
typedef void (*IOStreamOutputCallback) (
                                  IOStreamRef stream,
                                  void *context
                                  );

typedef struct IOStreamInterface_v1_t
{
    IUNKNOWN_C_GUTS;
    UInt32 Version;
    UInt32 Revision;

    /*!
        @functiongroup Opening and closing streams
     */
    
    /*!
    @function Open
     @abstract Open an IOStream from user space.
     @discussion The Open function opens the stream and maps the memory
     for the shared
     input and output queues into the current task, as well as the memory
     for all the buffers in the queue.  It also creates the notification
     ports for input and output.
     @param stream The reference to the stream returned by QueryInterface.
     @param options Open options.  Currently unused.
     @result If the stream could not be opened or the buffers could not
     be mapped, an error will be returned.
     */
    
    IOReturn (*Open)( IOStreamRef stream, IOOptionBits options );

    /*!
    @function Close
     @abstract Closes an IOStream.
     @discussion Calling Close umaps all shared memory for the stream
     and frees all user resources used by the stream.
     @param stream The IOStreamRef of the stream to close.
     @result Returns kIOReturnSuccess if the stream was successfully
     closed and the buffers unmapped.
     */
    IOReturn (*Close)( IOStreamRef stream );
    
    /*!
        @functiongroup Buffer information
     */
    
    /*!
        @function GetBufferCount
     @abstract Gets the number of buffers in the stream.
     @param stream The IOStreamRef of the stream to operate on.
     @result Returns then number of buffers in the stream.
     */
    
    IOItemCount (*GetBufferCount)( IOStreamRef stream );
    
    /*!
        @function GetBufferInfo
     @abstract Gets information about a buffer in an IOStream.
     @param stream The IOStreamRef of the stream to operate on.
     @param bufferID
     @param dataBufferAddressOut
     @param dataBufferSizeOut
     @param controlBufferAddressOut
     @param controlBufferSizeOut
     @result Returns kIOReturnSuccess if the buffer ID was valid.
     */
    
    IOReturn (*GetBufferInfo)( IOStreamRef stream,
                               IOStreamBufferID bufferID,
                               void **dataBufferAddressOut,  IOByteCount *dataBufferSizeOut,
                               void **controlBufferAddressOut, IOByteCount *controlBufferSizeOut );
    
    
    /*!
        @functiongroup Input and output
     */
    
    /*!
        @function GetOutputPort
     @abstract Get the notification port for buffers moving out from kernel
     to user space.
     @param stream The IOStreamRef of the stream to operate on.
     @result A CFMachPortRef of the output notification port.
     */
    CFMachPortRef (*GetOutputPort)( IOStreamRef stream );
    
    /*!
    @function GetInputPort
     @abstract Get the notification port for buffers moving in from user
     to kernel space.
     @param stream The IOStreamRef of the stream to operate on.
     @result A CFMachPortRef for the input notification port.
     */
    CFMachPortRef (*GetInputPort)( IOStreamRef stream );
    
    /*!
        @function GetOutputQueue
     @abstract Get the shared memory queue for buffers moving out from kernel
     to user space.
     @param stream The IOStreamRef of the stream to operate on.
     @result A pointer to the shared memory IOStreamBufferQueue structure for the output queue.
     @discussion   If you use the notification functions, you should never need
     to access the queues directly.
     */
    IOStreamBufferQueue *(*GetOutputQueue)( IOStreamRef stream );
    
    /*!
        @function GetInputQueue
     @abstract Get the shared memory queue for buffers moving in from user
     to kernel space.
     @param stream The IOStreamRef of the stream to operate on.
     @result A pointer to the shared memory IOStreamBufferQueue structure for the input queue.
     @discussion   If you use the notification functions, you should never need
     to access the queues directly.
     */
    IOStreamBufferQueue *(*GetInputQueue)( IOStreamRef stream );
    
    /*!
        @function SetOutputCallback
     @abstract Set the callback function to be called when a new buffer
     is available from the kernel.
     @param stream The IOStreamRef of the stream to operate on.
     Pass NULL to remove the callback.
     @result Returns kIOReturnSuccess if the callback was successfully
     set or removed.
     
     */
    IOReturn
        (*SetOutputCallback)( IOStreamRef stream,
                             IOStreamOutputCallback callback,
                             void *context );
    
    /*!
        @functiongroup Run loop operations
     */
    
    /*!
        @function GetRunLoopSource
     @abstract Gets a CFRunLoopSource for the CFMachPort used for notifications
     from the kernel that data is ready.
     @param stream The IOStreamRef of the stream to operate on.
     @result The CFRunLoopSourceRef for the run loop source, or NULL if
     there was an error creating the source.
     */
    
    CFRunLoopSourceRef
        (*GetRunLoopSource)( IOStreamRef stream );
    
    /*!
        @function AddToRunLoop
     @abstract Add the CFRunLoopSource for the notification port to a run loop.
     @param stream The IOStreamRef of the stream to operate on.
     @param runLoop The run loop to which to add the notification source.
     @result Returns kIOReturnSuccess if the source was successfully added
     to the run loop.
     */
    
    IOReturn
        (*AddToRunLoop)( IOStreamRef stream,
                              CFRunLoopRef runLoop );
    
    /*!
        @function RemoveFromRunLoop
     @abstract Remove the CFRunLoopSource for the notification port from a run loop.
     @param stream The IOStreamRef of the stream to operate on.
     @param runLoop The run loop from which to remove the notification source.
     @result Returns kIOReturnSuccess if the source was successfully removed
     from the run loop.
     
     */
    IOReturn
        (*RemoveFromRunLoop)( IOStreamRef stream,
                                   CFRunLoopRef runLoop );
    
    /*!
        @functiongroup Buffer operations on streams
     */
    
    /*!
        @function DequeueOutputEntry
     @abstract Get the next IOStreamBufferQueueEntry available from the output queue.
     @param stream The IOStreamRef of the stream to operate on.
     @param entry A pointer to an IOStreamBufferQueueEntry structure.  The next available
     entry from the output queue will be copied into it.
     @result The IOStreamBufferID of the next available buffer, or
     kInvalidIOStreamBufferID if no buffer is in the queue.
     
     */
    IOReturn (*DequeueOutputEntry)( IOStreamRef stream, IOStreamBufferQueueEntry *entry );
    
    /*!
        @function EnqueueInputEntry
     */

    IOReturn (*EnqueueInputEntry)( IOStreamRef stream, IOStreamBufferQueueEntry *entry);
    
    /*!
        @function EnqueueInputBuffer
     @abstract Send a buffer to the kernel side of the IOStream on the input queue.
     @param stream The IOStreamRef of the stream to operate on.
     @param bufferID The IOStreamBufferID of the buffer to place on the queue.
     @param dataLength The length of the valid data in the buffer.
     @result Returns kIOReturnSuccess if the buffer was successfully placed
     on the input queue.
     
     */
    
    IOReturn (*EnqueueInputBuffer)( IOStreamRef stream, IOStreamBufferID bufferID,
                                    IOByteCount dataOffset, IOByteCount dataLength,
                                    IOByteCount controlOffset, IOByteCount controlLength );
    
    /*!
        @functiongroup Notifications
     */
    
    /*!
        @function SendInputNotification
     @abstract Send a notification to the kernel side of the IOStream
     that data is available in the input queue.
     @param stream The IOStreamRef of the stream to operate on.
     @param token A value to pass to the stream's notification function.
     This is unused by IOStream but may be used by subclasses.
     @result Returns kIOReturnSuccess if the notification was successfully sent.
     */
    
    IOReturn (*SendInputNotification)( IOStreamRef stream, UInt32 token );
    
    /*!
        @function SendInputSyncNotification
     @abstract Notify the kernel side of the stream that input is ready
     by using a fast trap to call directly into the stream user client driver.
     This will transfer control to the kernel and continue executing
     on your same thread, rather than sending the notification to a separate thread.
     @param stream The IOStreamRef of the stream to operate on.
     @param token A value to pass to the stream's notification function.
     This is unused by IOStream but may be used by subclasses.
     @result Returns kIOReturnSuccess if the notification was successfully sent.

     */
    IOReturn (*SendInputSyncNotification)( IOStreamRef stream, UInt32 token );
    
    /*!
        @functiongroup Buffer convenience functions
     */
        
    /*!
        @function GetDataBuffer
     @abstract Get a pointer to the data area of an IOStreamBuffer.
     @param stream The IOStreamRef of the stream owning the buffer.
     @param bufferID The IOStreamBufferID of the buffer to operate on.
     @result A pointer to the data buffer area of the IOStreamBuffer,
     or NULL if it does not have a data buffer.
     
     */
    void *(*GetDataBuffer)( IOStreamRef stream, IOStreamBufferID bufferID );
    
    /*!
        @function GetDataBufferLength
     @abstract Get the length of the data area of an IOStreamBuffer.
     @param stream The IOStreamRef of the stream owning the buffer.
     @param bufferID The IOStreamBufferID of the buffer to operate on.
     @result The length in bytes of the data buffer, or 0 if there is no
     data buffer.

     */
    IOByteCount (*GetDataBufferLength)( IOStreamRef stream, IOStreamBufferID bufferID );
    
    /*!
        @function GetControlBuffer
     @abstract Get a pointer to the control area of an IOStreamBuffer.
     @param stream The IOStreamRef of the stream owning the buffer.
     @param bufferID The IOStreamBufferID of the buffer to operate on.
     @result A pointer to the control buffer area of the IOStreamBuffer,
     or NULL if it does not have a control buffer.
     
     */
    void *(*GetControlBuffer)( IOStreamRef stream, IOStreamBufferID bufferID );
    
    /*!
        @function GetControlBufferLength
     @abstract Get the length of the control area of an IOStreamBuffer.
     @param stream The IOStreamRef of the stream owning the buffer.
     @param bufferID The IOStreamBufferID of the buffer to operate on.
     @result The length in bytes of the control buffer, or 0 if there is no
     control buffer.
     
     */
    IOByteCount (*GetControlBufferLength)( IOStreamRef stream, IOStreamBufferID bufferID );
    
    /*!
        @function StartStream
    */
    IOReturn (*StartStream)( IOStreamRef stream );
    
    /*!
        @function StopStream
     */
    IOReturn (*StopStream)( IOStreamRef stream );
    
    /*!
        @function SuspendStream
     */
    IOReturn (*SuspendStream)( IOStreamRef stream );
    
    /*!
        @function GetMode
     */
    IOStreamMode (*GetMode)( IOStreamRef stream );
    
    /*!
        @function SetMode
     */
    IOReturn (*SetMode)( IOStreamRef stream, IOStreamMode mode );
    
} IOStreamInterface;


__END_DECLS

#endif /* ! __IOKIT_IOSTREAMLIB_H */
