/*
 *  IOStreamShared.h
 *  IOStreamFamily
 *
 *  Copyright 2006 Apple Computer, Inc. All rights reserved.
 *
 */

#if !defined(__IOKIT_IOSTREAMSHARED_H)
#define __IOKIT_IOSTREAMSHARED_H

#include <sys/cdefs.h>

#include <IOKit/IOTypes.h>

/*!
    @header IOStreamShared.h
    IOStream definitions shared between kernel and user space.
 */

__BEGIN_DECLS    
   
// Buffer numbers are guaranteed to go from 0 to bufferCount - 1.
/*!
    @typedef IOStreamBufferID
 */
typedef UInt32 IOStreamBufferID;
#define kIOStreamBufferIDInvalid (~0)

// This is the header for the shared-memory queue used to send buffer notifications
// from kernel to user space or vice versa.

// Queue entry in input or output queue

#ifdef __cplusplus

/*!
    @struct IOStreamBufferQueueEntry
    @field bufferID     The ID of the buffer passed in this queue entry.
    @field dataLength   The length of the valid data in the buffer.
    @field reserved     Reserved for future use.
*/
    
struct IOStreamBufferQueueEntry {
    IOStreamBufferID    bufferID;
    UInt32              dataOffset;
    UInt32              dataLength;
    UInt32              controlOffset;
    UInt32              controlLength;
    UInt32              reserved[3];
};


/*!
    @struct IOStreamBufferQueue
    @field entryCount   The number of queue entries in the queue.
    @field headIndex    The index of the next queue slot that will be filled in by the queue writer.
    @field tailIndex    The index of the next queue slot that can be read by the queue reader.
    @field reserved     Reserved for future use.
    @field queue        The array of queue entries.
 */

struct IOStreamBufferQueue {
    UInt32                      entryCount;
    volatile UInt32             headIndex;
    volatile UInt32             tailIndex;
    UInt32                      reserved;
    IOStreamBufferQueueEntry    queue[0];
};

#else

typedef struct __IOStreamBufferQueueEntry {
    IOStreamBufferID    bufferID;
    UInt32              dataOffset;
    UInt32              dataLength;
    UInt32              controlOffset;
    UInt32              controlLength;
    UInt32              reserved[3];
} IOStreamBufferQueueEntry;

typedef struct __IOStreamBufferQueue {
    UInt32                      entryCount;
    volatile UInt32             headIndex;
    volatile UInt32             tailIndex;
    UInt32                      reserved;
    IOStreamBufferQueueEntry    queue[0];
} IOStreamBufferQueue;

#endif

/*!
    @enum Memory mapping types
    @constant kIOStreamMemoryTypeOutputQueue
    @constant kIOStreamMemoryTypeInputQueue
    @constant kIOStreamMemoryTypeBufferData
    @constant kIOStreamMemoryTypeBufferControl
    @constant kIOStreamBufferIDMask
    @constant kIOStreamMemoryTypeMask
    @abstract Memory types used with IOConnectMapMemory().
    @discussion
 */
enum {
    kIOStreamMemoryTypeOutputQueue   = 0x10000000,
    kIOStreamMemoryTypeInputQueue    = 0x20000000,
    kIOStreamMemoryTypeBufferData    = 0x30000000,
    kIOStreamMemoryTypeBufferControl = 0x40000000,
    kIOStreamBufferIDMask            = 0x0FFFFFFF,
    kIOStreamMemoryTypeMask          = 0xF0000000
};

/*!
    @enum Mach port types
    @constant kIOStreamPortTypeOutput
    @constant kIOStreamPortTypeInput
    @abstract Port types used with IOConnectSetNotificationPort().
    @discussion
 */
enum {
    kIOStreamPortTypeOutput,
    kIOStreamPortTypeInput
};

/*!
    @enum IOStream open options
    @discussion
 */
 
enum {
    kIOStreamOptionOpenExclusive = 0x00010000,
    kIOStreamOptionOpenShared    = 0x00020000
};

/*!
    @enum User client methods
    @constant kIOStreamMethodOpen
    @constant kIOStreamMethodClose
    @constant kIOStreamMethodStart
    @constant kIOStreamMethodStop
    @constant kIOStreamMethodSuspend
    @constant kIOStreamMethodGetMode
    @constant kIOStreamMethodSetMode
 
    @abstract Client method numbers used with IOConnectMethod...() functions.
    @discussion
 */
enum {
    kIOStreamMethodOpen,
    kIOStreamMethodClose,
    kIOStreamMethodStart,
    kIOStreamMethodStop,
    kIOStreamMethodSuspend,
    kIOStreamMethodGetMode,
    kIOStreamMethodSetMode,
    kIOStreamMethodGetBufferCount
};

/*!
    @enum User client traps
    @constant kIOStreamEnqueueInputTrap
    @constant kIOStreamEnqueueInputSyncTrap
    @abstract Client trap numbers used with IOConnectTrap..() functions.
    @discussion
 */
enum {
    kIOStreamEnqueueInputTrap,
    kIOStreamEnqueueInputSyncTrap
};

typedef enum {
    kIOStreamModeInput,         // From user to kernel space
    kIOStreamModeOutput,        // From kernel to user space
    kIOStreamModeInputOutput    // bidirectional
} IOStreamMode;


__END_DECLS

#endif /* ! __IOKIT_IOSTREAMSHARED_H */

