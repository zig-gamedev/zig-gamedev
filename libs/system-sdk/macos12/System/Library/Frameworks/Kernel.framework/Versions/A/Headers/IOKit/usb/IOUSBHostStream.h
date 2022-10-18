/*
 * Copyright (c) 1998-2016 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

/*!
 @header     IOUSBHostStream.h
 @brief      IOUSBHostStream is an object representing an individual stream within a streaming endpoint.
 */

#ifndef IOUSBHostFamily_IOUSBHostStream_h
#define IOUSBHostFamily_IOUSBHostStream_h

#include <IOKit/usb/IOUSBHostFamily.h>
#include <IOKit/usb/IOUSBHostIOSource.h>

class IOUSBHostPipe;

/*!
 * @class       IOUSBHostStream
 * @brief       The OSObject representing an individual stream within a USB endpoint
 * @discussion  This class provides functionality to transfer data across USB.  Function drivers should not subclass IOUSBHostStream.
 */
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBHostStream : public IOUSBHostIOSource
{
    OSDeclareDefaultStructors(IOUSBHostStream)

    friend class IOUSBHostPipe;

public:
    /*!
     * @brief       Factory method for creating an IOUSBHostStream object
     * @discussion  This method should not be called by function drivers.  To create an IOUSBHostStream, use IOUSBHostPipe::copyStream(...)
     * @param       controller AppleUSBHostController to which the USB endpoint is associated
     * @param       device IOUSBHostDevice to which this endpoint belongs
     * @param       pipe IOUSBHostPipe to which this stream belongs
     * @param       streamID The stream ID for this instance
     * @return      Pointer to an IOUSBHostStream object if successful, otherwise NULL
     */
    IOUSBHostStream* withOwnersAndStreamID(AppleUSBHostController* controller, IOUSBHostDevice* device, IOUSBHostPipe* pipe, uint32_t streamID);

    virtual void free();
    
    /*!
     * @brief       Abort pending I/O requests.
     * @discussion  See IOUSBHostIOSource::abort for documentation.  A stream context must be set as non-active on the device via an out-of-band (class-defined) mechanism before this method is called (USB 3.1 8.12.1.4).  A non-active stream will not be selected by the device to become the current stream on the endpoint.
     */
    virtual IOReturn abort(IOOptionBits options = kAbortAsynchronous, IOReturn withError = kIOReturnAborted, IOService* forClient = NULL);

    /*!
     * @brief       Retrieve the IOUSBHostPipe to which this stream belongs
     * @return      IOUSBHostPipe pointer
     */
    virtual IOUSBHostPipe* getPipe() const { return _pipe; }

    /*!
     * @brief       Retrieve the sream ID for this instance
     * @return      Stream ID
     */
    virtual uint32_t getStreamID() const { return _streamID; }
    
     /*!
     * @brief       Enqueue a request on a stream
     * @discussion  See IOUSBHostIOSource::io for documentation
     * @param       completionTimeoutMs Must be 0 for streams.
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, uint32_t dataBufferLength, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs = 0);
    
    /*!
     * @brief       Perform a request on a stream
     * @discussion  See IOUSBHostIOSource::io for documentation
     * @param       completionTimeoutMs Must be 0 for streams.
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, uint32_t dataBufferLength, uint32_t& bytesTransferred, uint32_t completionTimeoutMs = 0);

    // Public pad slots for future expansion
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 0);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 1);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 2);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 3);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 4);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 5);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 6);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 7);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 8);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 9);

protected:
    virtual bool initWithOwnersAndStreamID(AppleUSBHostController* controller, IOUSBHostDevice* device, IOUSBHostPipe* pipe, uint32_t streamID);

    virtual IOReturn closeGated();
    virtual IOReturn destroyGated();

    virtual IOReturn abortGated(IOOptionBits options, IOReturn withError, IOService* forClient);

    // Protected pad slots for future expansion
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 10);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 11);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 12);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 13);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 14);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 15);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 16);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 17);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 18);
    OSMetaClassDeclareReservedUnused(IOUSBHostStream, 19);

protected:
    IOUSBHostPipe* _pipe;
    uint32_t       _streamID;
};

#endif
