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

#ifndef _IOKIT_IOAUDIOLEVELCONTROL_H
#define _IOKIT_IOAUDIOLEVELCONTROL_H

#include <AvailabilityMacros.h>

#ifndef IOAUDIOFAMILY_SELF_BUILD
#include <IOKit/audio/IOAudioControl.h>
#else
#include "IOAudioControl.h"
#endif

class OSArray;

/*!
 * @class IOAudioLevelControl
 * @abstract
 * @discussion
 */

class IOAudioLevelControl : public IOAudioControl
{
    OSDeclareDefaultStructors(IOAudioLevelControl)

protected:
    SInt32	minValue;
    SInt32	maxValue;
    IOFixed	minDB;
    IOFixed	maxDB;
    
    OSArray *ranges;
    
protected:
    struct ExpansionData { };
    
    ExpansionData *reserved;

public:
	static IOAudioLevelControl *createPassThruVolumeControl (SInt32 initialValue,
                                                                SInt32 minValue,
                                                                SInt32 maxValue,
                                                                IOFixed minDB,
                                                                IOFixed maxDB,
                                                                UInt32 channelID,
                                                                const char *channelName,
                                                                UInt32 cntrlID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDefineReservedUnused(IOAudioLevelControl, 0);
    /*!
	 * @function setLinearScale
     * @abstract This function tells CoreAudio if it should apply a curve to the scaler representation of the volume.
	 * @param useLinearScale TRUE instructs CoreAudio to not apply a curve to the scaler representation of the volume,
	 * FALSE instructs CoreAudio to apply a curve, which is CoreAudio's default behavior.
     */
	virtual void setLinearScale(bool useLinearScale) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

private:
    OSMetaClassDeclareReservedUsed(IOAudioLevelControl, 0);

    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 1);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 2);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 3);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 4);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 5);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 6);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 7);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 8);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 9);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 10);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 11);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 12);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 13);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 14);
    OSMetaClassDeclareReservedUnused(IOAudioLevelControl, 15);

public:
    /*!
     * @function create
     * @abstract Allocates a new level control with the given attributes
     * @param initialValue The initial value of the control
     * @param minValue The lowest possible value the control may have 
     * @param maxValue The highest possible value the control may have
     * @param minDB A fixed point representation of the db value matching minValue 
     * @param maxDB A fixed point representation of the db value matching maxValue 
     * @param channelID The ID of the channel(s) that the control acts on.  Common IDs are located in IOAudioTypes.h.
     * @param channelName An optional name for the channel.  Common names are located in IOAudioTypes.h.
     * @param cntrlID An optional ID for the control that can be used to uniquely identify controls.
     * @result Returns a newly allocted and initialized level IOAudioControl
     */
    static IOAudioLevelControl *create(SInt32 initialValue,
                                       SInt32 minValue,
                                       SInt32 maxValue,
                                       IOFixed minDB,
                                       IOFixed maxDB,
                                       UInt32 channelID,
                                       const char *channelName = 0,
                                       UInt32 cntrlID = 0,
                                       UInt32 subType = 0,
                                       UInt32 usage = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
                                       
    static IOAudioLevelControl *createVolumeControl(SInt32 initialValue,
                                                    SInt32 minValue,
                                                    SInt32 maxValue,
                                                    IOFixed minDB,
                                                    IOFixed maxDB,
                                                    UInt32 channelID,
                                                    const char *channelName = 0,
                                                    UInt32 cntrlID = 0,
                                                    UInt32 usage = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function init
     * @abstract Initializes a newly allocated IOAudioLevelControl with the given attributes
     * @param initialValue The initial value of the control
     * @param minValue The lowest possible value the control may have 
     * @param maxValue The highest possible value the control may have
     * @param minDB A fixed point representation of the db value matching minValue 
     * @param maxDB A fixed point representation of the db value matching maxValue 
     * @param channelID The ID of the channel(s) that the control acts on.  Common IDs are located in IOAudioTypes.h.
     * @param channelName An optional name for the channel.  Common names are located in IOAudioTypes.h.
     * @param cntrlID An optional ID for the control that can be used to uniquely identify controls.
     * @param properties Standard property list passed to the init() function of any new IOService.  This dictionary
     *  gets stored in the registry entry for this service.
     * @result Returns true on success
     */
    virtual bool init(SInt32 initialValue,
                      SInt32 minValue,
                      SInt32 maxValue,
                      IOFixed minDB,
                      IOFixed maxDB,
                      UInt32 channelID,
                      const char *channelName = 0,
                      UInt32 cntrlID = 0,
                      UInt32 subType = 0,
                      UInt32 usage = 0,
                      OSDictionary *properties = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
                      
    virtual void free() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setMinValue
     * @abstract Sets the minimum value the control may have
     * @param minValue The minimum value for the control
     */
    virtual void setMinValue(SInt32 minValue) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual SInt32 getMinValue() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
     
    /*!
     * @function setMaxValue
     * @abstract Sets the maximum value the control may have
     * @param maxValue The maximum value for the control
     */ 
    virtual void setMaxValue(SInt32 maxValue) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual SInt32 getMaxValue() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
     
     /*!
     * @function setMinDB
     * @abstract Sets the minimum value in db that the control may have
     * @discussion This value is represented as an IOFixed value which is a fixed point number.  The IOFixed
     *  type is a 16.16 fixed point value.
     * @param minDB The minimum value in db for the control
     */
    virtual void setMinDB(IOFixed minDB) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual IOFixed getMinDB() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function setMaxDB
     * @abstract Sets the maximum value in db that the control may have
     * @discussion This value is represented as an IOFixed value which is a fixed point number.  The IOFixed
     *  type is a 16.16 fixed point value.
     * @param maxDB The maximum value in db for the control
     */
    virtual void setMaxDB(IOFixed maxDB) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual IOFixed getMaxDB() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual IOReturn addRange(SInt32 minValue, SInt32 maxValue, IOFixed minDB, IOFixed maxDB) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual IOReturn addNegativeInfinity(SInt32 negativeInfinityValue) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual IOReturn validateValue(OSObject *newValue) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
};

#endif /* _IOKIT_IOAUDIOLEVELCONTROL_H */
