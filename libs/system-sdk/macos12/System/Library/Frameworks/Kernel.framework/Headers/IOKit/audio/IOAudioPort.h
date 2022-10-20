/*
 * Copyright (c) 1998-2014 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_IOAUDIOPORT_H
#define _IOKIT_IOAUDIOPORT_H

#include <IOKit/IOService.h>
#include <AvailabilityMacros.h>

class IOAudioDevice;
class IOAudioControl;
class OSDictionary;

/*!
 * @class IOAudioPort
 * @abstract Represents a logical or physical port or functional unit in an audio device.
 * @discussion An IOAudioPort represents an element in the signal chain in the audio device.  It may contain
 *  one or more controls (represented by IOAudioControl) by which different attributes of the port may be
 *  represented and adjusted.
 *
 *  IOAudioPort objects are connected up in the IORegistry in the IOAudioPlane to represent the signal chain of
 *  the device.  They may be connected to other IOAudioPorts as well as IOAudioEngines to indicate they either
 *  feed into or are fed by one of the audio engines (i.e. they provide input to or take output from the computer).
 */
class IOAudioPort : public IOService
{
    friend class IOAudioDevice;

    OSDeclareDefaultStructors(IOAudioPort)

public:
    /* @var audioDevice The IOAudioDevice that this IOAudioPort belongs to. */
    IOAudioDevice *	audioDevice;
    /* @var audioControls A set containg all of the IOAudioControl instances that belong to the port. */
    OSSet *		audioControls;
    bool		isRegistered;

protected:
    struct ExpansionData { };
    
    ExpansionData *reserved;
    
private:
    OSMetaClassDeclareReservedUnused(IOAudioPort, 0);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 1);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 2);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 3);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 4);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 5);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 6);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 7);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 8);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 9);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 10);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 11);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 12);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 13);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 14);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 15);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 16);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 17);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 18);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 19);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 20);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 21);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 22);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 23);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 24);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 25);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 26);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 27);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 28);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 29);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 30);
    OSMetaClassDeclareReservedUnused(IOAudioPort, 31);

public:
    /*!
     * @function withAttributes
     * @abstract Allocates a new IOAudioPort instance with the given attributes
     * @discussion This static method allocates a new IOAudioPort and calls initWithAttributes() on it with
     *  the parameters passed in to it.
     * @param portType A readable string representing the type of port.  Common port types are defined in
     *  IOAudioTypes.h and are prefixed with 'kIOAudioPortType'.  Please provide feedback if there are
     *  other common port types that should be included.
     * @param portName A readable string representing the name of the port.  For example: 'Internal Speaker',
     *  'Line Out'.  This field is optional, but useful for providing information to the application/user.
     * @param subType Developer defined readable string representing a subtype for the port. (optional)
     * @param properties Standard property list passed to the init of any new IOService.  This dictionary
     *  gets stored in the registry for this instance. (optional)
     * @result Returns the newly allocated and initialized IOAudioPort instance.
     */
    static IOAudioPort *withAttributes(UInt32 portType, const char *portName = 0, UInt32 subType = 0, OSDictionary *properties = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function initWithAttributes
     * @abstract Initializes a newly allocated IOAudioPort instance with the given attributes
     * @discussion The properties parameter is passed on the superclass' init().  The portType, subType
     *  and properties parameters are optional, however portType is recommended.
     * @param portType A readable string representing the type of port.  Common port types are defined in
     *  IOAudioTypes.h and are prefixed with 'kIOAudioPortType'.  Please provide feedback if there are
     *  other common port types that should be included.
     * @param portName A readable string representing the name of the port.  For example: 'Internal Speaker',
     *  'Line Out'.  This field is optional, but useful for providing information to the application/user.
     * @param subType Developer defined readable string representing a subtype for the port. (optional)
     * @param properties Standard property list passed to the init of any new IOService.  This dictionary
     *  gets stored in the registry for this instance. (optional)
     * @result Returns true on success.
     */
    virtual bool initWithAttributes(UInt32 portType, const char *portName = 0, UInt32 subType = 0, OSDictionary *properties = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function free
     * @abstract Frees all of the resources allocated by the IOAudioPort.
     * @discussion Do not call this directly.  This is called automatically by the system when the instance's
     *  refcount goes to 0.  To decrement the refcount, call release() on the object.
     */
    virtual void free() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function start
     * @abstract Called to start a newly created IOAudioPort.
     * @discussion This is called automatically by IOAudioDevice when attachAudioPort() is called.
     * @param provider The IOAudioDevice that owns this port
     * @result Returns true on success
     */
    virtual bool start(IOService *provider) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function stop
     * @abstract Called when the IOAudioDevice is stopping when it is no longer available.
     * @discussion This method calls deactivateAudioControls() to shut down all of the controls associated with
     *  this port.
     * @param provider The IOAudioDevice that owns this port
     */
    virtual void stop(IOService *provider) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual void registerService(IOOptionBits options = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual IOAudioDevice *getAudioDevice() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function addAudioControl
     * @abstract Adds a newly created IOAudioControl instance to the port.
     * @discussion This method is responsible for starting the new IOAudioControl and adding it to the internal
     *  audioControls array.
     * @param control A newly created IOAudioControl instance that should belong to this port.
     * @result Returns true on successfully staring the IOAudioControl.
     */
    virtual IOReturn addAudioControl(IOAudioControl *control) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function deactivateAudioControls
     * @abstract Called to shut down all of the audio controls for this port.
     * @discussion This will stop all of the audio controls and release them so that the instances may be
     *  freed.  This is called from the free() method.
     */
    virtual void deactivateAudioControls() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

protected:
    virtual void setType(UInt32 portType) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void setSubType(UInt32 subType) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void setName(const char *name) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
};

#endif /* _IOKIT_IOAUDIOPORT_H */
