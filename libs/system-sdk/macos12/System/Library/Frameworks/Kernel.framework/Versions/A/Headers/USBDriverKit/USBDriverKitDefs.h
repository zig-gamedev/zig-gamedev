/*
 * Copyright (c) 2018-2019 Apple Inc. All rights reserved.
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

#ifndef USBDriverKitDefs_h
#define USBDriverKitDefs_h

/*!
 * @enum        IOUSBAbortOptions
 * @brief       Options for <code>abort()</code>
 * @constant    kIOUSBAbortAsynchronous abort() should return immediately without waiting for the aborted IO to complete
 * @constant    kIOUSBAbortSynchronous abort() should not return until the aborted IO has completed
 */
enum IOUSBAbortOptions
{
    kIOUSBAbortAsynchronous = 0x0,
    kIOUSBAbortSynchronous  = 0x1
};

/*!
 * @brief       Free a descriptor returned by a copy descriptor method.
 * @discussion  Copy descriptor methods belonging to the IOUSBHost* classes will return
 *              newly allocated buffers containing the requested descriptor.
 *              These buffers must be freed using IOUSBHostFreeDescriptors().
 *              e.g.  Descriptors returned by the following methods all need to be freed:
 *              CopyDescriptor(), CopyConfigurationDescriptor(), CopyDeviceDescriptor(), ...
 * @param       descriptor to be freed.
 */

void IOUSBHostFreeDescriptor(const IOUSBDeviceDescriptor * descriptor);
void IOUSBHostFreeDescriptor(const IOUSBConfigurationDescriptor * descriptor);
void IOUSBHostFreeDescriptor(const IOUSBBOSDescriptor * descriptor);
void IOUSBHostFreeDescriptor(const IOUSBStringDescriptor * descriptor);


#endif /* USBDriverKitDefs_h */
