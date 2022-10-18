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

#ifndef _IOKIT_IOAUDIOTOGGLECONTROL_H
#define _IOKIT_IOAUDIOTOGGLECONTROL_H

#include <AvailabilityMacros.h>

#ifndef IOAUDIOFAMILY_SELF_BUILD
#include <IOKit/audio/IOAudioControl.h>
#else
#include "IOAudioControl.h"
#endif

/*!
 * @class IOAudioToggleControl
 */

class IOAudioToggleControl : public IOAudioControl
{
    OSDeclareDefaultStructors(IOAudioToggleControl)

protected:
    struct ExpansionData { };
    
    ExpansionData *reserved;

// New code added here
public:
    /*!
     * @function createPassThruMuteControl
     * @abstract Allocates a new pass through mute control with the given attributes
     * @param initialValue The initial value of the control
     * @param channelID The ID of the channel(s) that the control acts on.  Common IDs are located in IOAudioTypes.h.
     * @param channelName An optional name for the channel.  Common names are located in IOAudioPort.h.
     * @param cntrlID An optional ID for the control that can be used to uniquely identify controls
     * @result Returns a newly allocated and initialized mute IOAudioControl
     */
	static IOAudioToggleControl *createPassThruMuteControl (bool initialValue,
																UInt32 channelID,
																const char *channelName,
																UInt32 cntrlID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

private:
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 0);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 1);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 2);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 3);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 4);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 5);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 6);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 7);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 8);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 9);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 10);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 11);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 12);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 13);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 14);
    OSMetaClassDeclareReservedUnused(IOAudioToggleControl, 15);

public:
    /*!
     * @function create
     * @abstract Allocates a new mute control with the given attributes
     * @param initialValue The initial value of the control
     * @param channelID The ID of the channel(s) that the control acts on.  Common IDs are located in IOAudioTypes.h.
     * @param channelName An optional name for the channel.  Common names are located in IOAudioPort.h.
     * @param cntrlID An optional ID for the control that can be used to uniquely identify controls
     * @result Returns a newly allocated and initialized mute IOAudioControl
     */
    static IOAudioToggleControl *create(bool initialValue,
                                        UInt32 channelID,
                                        const char *channelName = 0,
                                        UInt32 cntrlID = 0,
                                        UInt32 subType = 0,
                                        UInt32 usage = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
                                      
    static IOAudioToggleControl *createMuteControl(bool initialValue,
                                                    UInt32 channelID,
                                                    const char *channelName = 0,
                                                    UInt32 cntrlID = 0,
                                                    UInt32 usage = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function init
     * @abstract Initializes a newly allocated IOAudioToggleControl with the given attributes
     * @param initialValue The initial value of the control
     * @param channelID The ID of the channel(s) that the control acts on.  Common IDs are located in IOAudioTypes.h.
     * @param channelName An optional name for the channel.  Common names are located in IOAudioPort.h.
     * @param cntrlID An optional ID for the control that can be used to uniquely identify controls
     * @result Returns truen on success
     */
    virtual bool init(bool initialValue,
                      UInt32 channelID, 
                      const char *channelName = 0,
                      UInt32 cntrlID = 0,
                      UInt32 subType = 0,
                      UInt32 usage = 0,
                      OSDictionary *properties = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

};

#endif /* _IOKIT_IOAUDIOTOGGLECONTROL_H */
