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

#ifndef _IOKIT_IOAUDIOCONTROL_H
#define _IOKIT_IOAUDIOCONTROL_H

#include <IOKit/IOService.h>
#include <AvailabilityMacros.h>
#ifndef IOAUDIOFAMILY_SELF_BUILD
#include <IOKit/audio/IOAudioEngine.h>
#else
#include "IOAudioEngine.h"
#endif

class IOAudioPort;
class OSDictionary;
class OSSet;
class IOAudioUserClient;
class IOAudioControlUserClient;
class IOWorkLoop;
class IOCommandGate;

/*!
 * @class IOAudioControl
 * @abstract Represents any controllable attribute of an IOAudioDevice.
 * @discussion An IOAudioControl instance may belong to one IOAudioPort.  Additionally, it may associated 
 *  with an IOAudioEngine as a default control for that IOAudioEngine.
 *
 *  When its value changes, it sends a notification to the CoreAudio.framework (HAL).  It also makes a call
 *  to the ValueChangeHandler.
 *
 *  The base IOAudioControl class contains a type, a value and a channel ID representing the channel(s) which 
 *  the control acts on.  It may also contain a readable format for the name of the channel as well as a 
 *  control ID that can be used to identify unique controls.  Additionally it may contain a subType and a usage.  
 *  Each type has its own set of possible subTypes.  There currently four different control types defined: 
 *  kIOAudioControlTypeLevel, kIOAudioControlTypeToggle, kIOAudioControlTypeSelector.  
 *  Each one is represented by a subclass of IOAudioControl: IOAudioLevelControl, IOAudioToggleControl, 
 *  IOAudioSelectorControl.  The level control defines a range of allowed values and has 
 *  a defined subtype of kIOAudioLevelControlSubTypeVolume used to define a volume control.  The toggle control 
 *  allows for a boolean value and has a defined subtype kIOAudioToggleControlSubTypeMute for a mute control.  The 
 *  selector control has a list of allowed selections with a value and description for each allowed selection and 
 *  has the following sub types: kIOAudioSelectorControlSubTypeOutput for an output selector and 
 *  kIOAudioSelectorControlSubTypeInput for an input selector.  See the subclass documentation for a more 
 *  complete description of each 
 * 
 *  There are enums for default channel ID values and common channel names in IOAudioTypes.h.  The channel ID 
 *  values are prefixed with 'kIOAudioControlChannelID' and the common channel names are prefixed with
 *  'kIOAudioControlChannelName'.  All of the attributes of the IOAudioControl are stored in the registry.
 *  The key used for each attribute is defined in IOAudioTypes.h with the define matching the following
 *  pattern: 'kIOAudioControl<attribute name>Key'.  For example: kIOAudioControlChannelIDKey.
 *
 *  In addition to the existing defined control types, drivers can define their own as well for other purposes.
 *
 *  Changes to the IOAudioControl's value made by the CoreAudio.framework are done through the IORegistry.  
 *  When the CoreAudio.framework initiates a value change, the control receives a setProperties() message. 
 *  The setProperties() implementation looks for the property 'IOAudioControlValue' and if present, calls 
 *  setValue() on the driver's IOWorkLoop with the new value.  The setValue() function first checks to see 
 *  if the new value is different.  If so, it calls performValueChange() to call through to the driver
 *  to make the change in the hardware.  If that call succeeds the value is changed and the new value is set
 *  in the registry.  Additionally notifications are sent to all clients that have registered for them.
 */
class IOAudioControl : public IOService
{
    friend class IOAudioPort;
    friend class IOAudioDevice;
    friend class IOAudioEngine;
    
    OSDeclareDefaultStructors(IOAudioControl)
    
public:

    /*!
     * @typedef IntValueChangeHandler
     * @abstract Handler function used to make a notification when a value is to be changed.
     * @param target Reference supplied when the handler was registered.
     * @param audioControl The IOAudioControl that is changing.
	 * @param oldValue The old value of the control.
     * @param newValue The new value the control is being changed to.
	 * @result Must return kIOReturnSuccess when the hardware is successfully updated.
     */
    typedef IOReturn (*IntValueChangeHandler)(OSObject *target, IOAudioControl *audioControl, SInt32 oldValue, SInt32 newValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    typedef IOReturn (*DataValueChangeHandler)(OSObject *target, IOAudioControl *audioControl, const void *oldData, UInt32 oldDataSize, const void *newData, UInt32 newDataSize ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    typedef IOReturn (*ObjectValueChangeHandler)(OSObject *target, IOAudioControl *audioControl, OSObject *oldValue, OSObject *newValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
protected:
    /*! @var workLoop 
        The IOWorkLoop for the audio driver - shared from the IOAudioDevice. 
    */
    IOWorkLoop 		*workLoop;
    /*! @var commandGate 
        The IOCommandGate for this control - attached to the driver's IOWorkLoop.
    */
    IOCommandGate	*commandGate;
    
    /*! @var isStarted 
        Internal state keeping track of when the IOAudioControl has been started. 
    */
    bool		isStarted;

    /*! @var controlID 
        An optional identifier that can be used to identify the control. 
    */
    UInt32 		controlID;
    /*! @var channelID 
        The ID of the channel this control affects - may be kIOAudioControlChannelIDAll if it represents all channels.
    */
    UInt32		channelID;
    
    UInt32		type;
    UInt32		subType;
    UInt32		usage;
    
    OSObject	*value;
    
    typedef enum {
        kIntValueChangeHandler,
        kDataValueChangeHandler,
        kObjectValueChangeHandler
    } ValueChangeHandlerType;
    
    ValueChangeHandlerType valueChangeHandlerType;

    union {
        IntValueChangeHandler		intHandler;
        DataValueChangeHandler		dataHandler;
        ObjectValueChangeHandler	objectHandler;
    } valueChangeHandler;
    
    OSObject	*valueChangeTarget;
    
    /*! @var clients
        A list of user clients that have requested value change notifications. 
    */ 
    OSSet		*userClients;

protected:
    struct ExpansionData {
		IOAudioEngine *				providerEngine;
		OSArray *					notificationQueue;
		UInt32						commandGateStatus;			// <rdar://8518215>
		SInt32						commandGateUsage;			// <rdar://8518215>
	};
    
    ExpansionData *reserved;
    
public:
	// OSMetaClassDeclareReservedUsed(IOAudioControl, 0);
	virtual void sendChangeNotification(UInt32 notificationType ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioControl, 1);
    /*!
	 * @function setReadOnlyFlag
     * @abstract Call this function to say that a control is read only.
	 * This call cannot be undone, so if a control is only temporarily unsetable,
	 * do not use this call but instead return an error from the control handler.
     */
	virtual void setReadOnlyFlag( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioControl, 2);
	virtual void sendQueuedNotifications(void ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioControl, 3);
    /*!
     * @function createUserClient
     * @abstract Creates a new IOAudioControlUserClient instance.
     * @discussion This function is called by newUserClient() to create a new IOAudioControlUserClient instance.  This function may be overridden by subclasses that need to add functionality
     *  to the IOAudioControlUserClient.  In that case, they must subclass IOAudioControlUserClient
     *  and return a new, initialized instance of that subclass.
	 *  A derived class that requires overriding of createUserClient should override the version with the properties
	 *  parameter for Intel targets, and without the properties parameter for PPC targets.  The #if __i386__ directive
	 *  can be used to select between the two behaviors.
     * @param task The task requesting the new user client.
     * @param securityID Optional security paramater passed in by the client - ignored.
     * @param type Optional user client type passed in by the client.
     * @param newUserClient The IOAudioControlUserClient * must be stored in this param on a successful
     *  completion.
     * @param properties A dictionary of additional properties for the connection.
     * @result Returns kIOReturnSuccess on success.
     */
    virtual IOReturn createUserClient(task_t task, void *securityID, UInt32 taskType, IOAudioControlUserClient **newUserClient, OSDictionary *properties) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

private:
    OSMetaClassDeclareReservedUsed(IOAudioControl, 0);
    OSMetaClassDeclareReservedUsed(IOAudioControl, 1);
    OSMetaClassDeclareReservedUsed(IOAudioControl, 2);
    OSMetaClassDeclareReservedUsed(IOAudioControl, 3);

    OSMetaClassDeclareReservedUnused(IOAudioControl, 4);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 5);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 6);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 7);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 8);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 9);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 10);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 11);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 12);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 13);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 14);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 15);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 16);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 17);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 18);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 19);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 20);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 21);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 22);
    OSMetaClassDeclareReservedUnused(IOAudioControl, 23);
    
public:

    /*!
     * @function withAttributes
     * @abstract Static function that allocates a new IOAudioControl with the given attributes.
     * @param type The type of the control.  Common, known types are defined in IOAudioTypes.h.  They currently
     *  consist of kIOAudioControlTypeLevel, kIOAudioControlTypeToggle, kIOAudioControlTypeSelector.
     * @param channelID The ID of the channel(s) that the control acts on.  Common IDs are located in IOAudioTypes.h.
     * @param channelName An optional name for the channel.  Common names are located in IOAudioDefines.h.  Any name not
     *  defined in IOAudioDefines.h must be localized in order to be properly displayed in multiple languages.
     * @param cntrlID An optional ID for the control that can be used to uniquely identify controls
     * @param subType An optional subType specific to the given type
     * @param usage An optional value specifying how the control will be used.  Currently defined usages are kIOAudioControlUsageInput, 
     *  kIOAudioControlUsageOutput and kIOAudioControlUsagePassThru.  This value is used when a control is set as a default control
     *  on an IOAudioEngine.
     * @result Returns a newly allocated and initialized IOAudioControl.
     */
    static IOAudioControl *withAttributes(UInt32 type,
                                          OSObject *initialValue,
                                          UInt32 channelID,
                                          const char *channelName = 0,
                                          UInt32 cntrlID = 0,
                                          UInt32 subType = 0,
                                          UInt32 usage = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10 ;

    /*!
     * @function init
     * @abstract Initializes a newly allocated IOAudioControl with the given attributes.
     * @param type The type of the control.  Common, known types are defined in IOAudioTypes.h.  They currently
     *  consist of kIOAudioControlTypeLevel, kIOAudioControlTypeToggle, kIOAudioControlTypeSelector.
     * @param channelID The ID of the channel(s) that the control acts on.  Common IDs are located in IOAudioTypes.h.
     * @param channelName An optional name for the channel.  Common names are located in IOAudioDefines.h.  Any name not
     *  defined in IOAudioDefines.h must be localized in order to be properly displayed in multiple languages.
     * @param cntrlID An optional ID for the control that can be used to uniquely identify controls
     * @param subType An optional subType specific to the given type
     * @param usage An optional value specifying how the control will be used.  Currently defined usages are kIOAudioControlUsageInput, 
     *  kIOAudioControlUsageOutput and kIOAudioControlUsagePassThru.  This value is used when a control is set as a default control
     *  on an IOAudioEngine.
     * @param properties Standard property list passed to the init() function of any new IOService.  This dictionary
     *  gets stored in the registry entry for this instance.
     * @result Returns true on success.
     */
    virtual bool init(UInt32 type,
                      OSObject *initialValue,
                      UInt32 channelID,
                      const char *channelName = 0,
                      UInt32 cntrlID = 0,
                      UInt32 subType = 0,
                      UInt32 usage = 0,
                      OSDictionary *properties = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10 ;
                      
    /*!
     * @function free
     * @abstract Frees all of the resources allocated by the IOAudioControl.
     * @discussion Do not call this directly.  This is called automatically by the system when the instance's
     *  refcount goes to 0.  To decrement the refcount, call release() on the object.
     */
    virtual void free( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function start
     * @abstract Starts a newly created IOAudioControl.
     * @discussion This is called automatically by IOAudioPort when addAudioControl() is called or by IOAudioEngine 
     *  when addDefaultAudioControl() is called.  It will only be called by the first call to either addAudioControl() or
     *  addDefaultAudioControl().
     * @param provider The IOAudioPort or IOAudioEngine that owns this control.
     * @result Returns true on success.
     */
    virtual bool start(IOService *provider) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
	virtual bool attachAndStart(IOService *provider) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function getIsStarted
     * @abstract Returns true after start() has been called.
     * @discussion Used by IOAudioPort and IOAudioEngine to decide if the control needs to be started.
     */
    virtual bool getIsStarted() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function stop
     * @abstract Stops the control when the provider is going away.
     * @param provider The IOAudioPort or IOAudioEngine that owns this control.
     */
    virtual void stop(IOService *provider) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function getWorkLoop
     * @abstract Returns the IOWorkLoop for the whole audio driver.
     */
    virtual IOWorkLoop *getWorkLoop() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function getCommandGate
     * @abstract Returns the IOCommandGate for this IOAudioControl.
     */
    virtual IOCommandGate *getCommandGate() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10 ;

    /*!
     * @function newUserClient
     * @abstract Creates a new user client object for this IOAudioControl instance.
     * @discussion This is called automatically by I/O Kit when a user process opens a connection to this
     *  IOAudioControl.  This is typically done when the user process needs to register for value change
     *  notifications.  This implementation allocates a new IOAudioControlUserClient object.  There is no
     *  need to call this directly.
	 *  A derived class that requires overriding of newUserClient should override the version with the properties
	 *  parameter for Intel targets, and without the properties parameter for PPC targets.  The #if __i386__ directive
	 *  can be used to select between the two behaviors.
     * @param task The task requesting the new user client.
     * @param securityID Optional security paramater passed in by the client - ignored.
     * @param type Optional user client type passed in by the client - 0 for the default user client type.
     * @param handler The new IOUserClient * must be stored in this param on a successful completion.
     * @param properties A dictionary of additional properties for the connection.
     * @result Returns kIOReturnSuccess on success.  May also result kIOReturnError or kIOReturnNoMemory.
     */
    virtual IOReturn newUserClient(task_t task, void *securityID, UInt32 type, IOUserClient **handler) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual IOReturn newUserClient(task_t task, void *securityID, UInt32 type, OSDictionary *properties, IOUserClient **handler) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function createUserClient
     * @abstract Creates a new IOAudioControlUserClient instance.
     * @discussion This function is called by newUserClient() to create a new IOAudioControlUserClient instance.  This function may be overridden by subclasses that need to add functionality
     *  to the IOAudioControlUserClient.  In that case, they must subclass IOAudioControlUserClient
     *  and return a new, initialized instance of that subclass.
	 *  A derived class that requires overriding of createUserClient should override the version with the properties
	 *  parameter for Intel targets, and without the properties parameter for PPC targets.  The #if __i386__ directive
	 *  can be used to select between the two behaviors.
     * @param task The task requesting the new user client.
     * @param securityID Optional security paramater passed in by the client - ignored.
     * @param type Optional user client type passed in by the client.
     * @param newUserClient The IOAudioControlUserClient * must be stored in this param on a successful
     *  completion.
     * @result Returns kIOReturnSuccess on success.
     */
    virtual IOReturn createUserClient(task_t task, void *securityID, UInt32 type, IOAudioControlUserClient **newUserClient) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10 ;
    
    /*!
     * @function clientClosed
     * @abstract Called automatically by the IOAudioControlUserClient when a user client closes its
     *  connection to the control.
     * @param client The user client object that has disconnected.
     */
    virtual void clientClosed(IOAudioControlUserClient *client) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setProperties
     * @abstract Changes a property of this IOService. 
     * @discussion This is called when the user client changes a property of this 
     *  IOAudioControl.  In this case it is used to change the value.  This function
     *  looks for that property and then calls setValue() through the IOCommandGate and
     *  setValueAction().
     * @param properties An OSDictionary containing the properties to change.
     * @result Returns kIOReturnSuccess on success.
     */
    virtual IOReturn setProperties(OSObject *properties) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void setValueChangeHandler(IntValueChangeHandler intValueChangeHandler, OSObject *target) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void setValueChangeHandler(DataValueChangeHandler dataValueChangeHandler, OSObject *target) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void setValueChangeHandler(ObjectValueChangeHandler objectValueChangeHandler, OSObject *target) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual void setValueChangeTarget(OSObject *target) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function flushValue
     * @abstract Forces the control to be flushed out to the hardware.
     * @discussion This function calls performValueChange() directly with the current value of the IOAudioControl.
     * @result Returns the result of performValueChange() - kIOReturnSuccess on success.
     */
    virtual IOReturn flushValue() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setValueAction
     * @abstract IOCommandGate Action which calls setValue() while holding the IOCommandGate.
     * @discussion This is needed to allow setValue() to be called on the IOWorkLoop.
     * @param owner The owner of the IOCommandGate (the IOAudioControl in this case).
     * @param arg1 The new value for the IOAudioControl.
     * @result Returns the result of setValue() - kIOReturnSuccess on success.
     */
    static IOReturn setValueAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	static IOReturn _setValueAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>
	
    /*!
     * @function setValue
     * @abstract Sets the value for this control.
     * @discussion When the control's value is changed, a call is made to performValueChange().  If that call
     *  succeeds, the value is set and sendValueChangeNotification() is called to send a notification to the
     *  user clients.  This function must be called on the IOWorkLoop.
     * @param newValue The new value for this control.
     * @result Returns kIOReturnSuccess if the value is successfully set.
     */
    virtual IOReturn setValue(OSObject *newValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual IOReturn setValue(SInt32 intValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function hardwareValueChanged
     * @abstract Updates the value for this control and sends out the value changed notification.
     * @discussion This is designed to be called by the driver when it detects that the hardware's value has
     *  changed without driver intervention (e.g. when an external event causes the change).  The difference between
     *  hardwareValueChanged() and setValue() is that hardwareValueChanged() doesn't call performValueChange() which 
     *  sends a message back to the driver to cause it to change the hardware with the new value.  This function must
     *  be called on the IOWorkLoop.
     * @param newValue The new value for this control.
     * @result Returns kIOReturnSuccess if the value is successfully updated.
     */
    virtual IOReturn hardwareValueChanged(OSObject *newValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function getValue 
     * @abstract Returns the current value of the control.
     */
    virtual OSObject *getValue( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual SInt32 getIntValue( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual const void *getDataBytes( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual UInt32 getDataLength( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function getControlID 
     * @abstract Returns the control ID for the control.
     */
    virtual UInt32 getControlID( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function getChannelID 
     * @abstract Returns the channel ID for the control.
     */
    virtual UInt32 getChannelID( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual UInt32 getType( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual UInt32 getSubType( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual UInt32 getUsage( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual void setCoreAudioPropertyID(UInt32 propertyID ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	void setWorkLoop(IOWorkLoop *wl ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

protected:
    /*!
     * @function sendValueChangeNotification
     * @abstract Called when the value has changed for the control.
     * @discussion This function sends out the value change notification to the user clients.
     */
    virtual void sendValueChangeNotification( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function setChannelName 
     * @abstract Called at init time to set the channel name for this IOAudioControl.
     */
    virtual void setChannelName(const char *channelName ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setChannelID 
     * @abstract Called at init time to set the channel ID for this IOAudioControl.
     */
    virtual void setChannelID(UInt32 newChannelID ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void setChannelNumber(SInt32 channelNumber ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setSubType 
     * @abstract Called at init time to set the control subType.
     */
    virtual void setType(UInt32 type ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setType 
     * @abstract Called at init time to set the control type.
     */
    virtual void setSubType(UInt32 subType ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setUsage 
     * @abstract Called at init time to set the control usage.
     */
    virtual void setUsage(UInt32 usage ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setControlID
     * @abstract Sets the controlID for this control.
     * @discussion The control ID is an optional attribute that can be used to track IOAudioControls.  A typical
     *  use is for the IOAudioDevice to assign a unique controlID to each control that it creates and then
     *  do a switch statement on the id of the control when it gets an audioControlValueChanged() notification.
     *  Typically the control ID is set when the object is created and doesn't need to be called again.
     * @param cntrlID The control ID for the control.
     */
    virtual void setControlID(UInt32 cntrlID ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function validateValue
     * @abstract Called by setValue() to verify that the value is valid.
     * @param newValue The new value to be verified.
     * @result Returns kIOReturnSuccess if the value is valid.
     */
    virtual IOReturn validateValue(OSObject *newValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function updateValue
     * @abstract Called by setValue() in order to update the value and the registry.
     * @discussion It also calls
     *  sendValueChangedNotification() to send notifications to the user clients.
     * @param newValue The new value to b updated.
     * @result Returns kIOReturnSuccess if the value is successfully updated.
     */
    virtual IOReturn updateValue(OSObject *newValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual IOReturn _setValue(OSObject *newValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function performValueChange
     * @abstract Called by setValue() to make the call to the valueChangeHandler
     *  to update the hardware.
     * @result Returns the result of the handler call (or kIOReturnError on an error).
     */
    virtual IOReturn performValueChange(OSObject *newValue ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function addUserClientAction
     * @abstract IOCommandGate Action which calls addUserClient() while holding the IOCommandGate.
     * @discussion This is needed to allow addUserClient() to be called on the IOWorkLoop.
     * @param owner The owner of the IOCommandGate (the IOAudioControl in this case).
     * @param arg1 The IOAudioControlUserClient to be added.
     * @result Returns the result of addUserClient() - kIOReturnSuccess on success.
     */
    static IOReturn addUserClientAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4 ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
	static IOReturn _addUserClientAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3 ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>

    /*!
     * @function removeUserClientAction
     * @abstract IOCommandGate Action which calls removeUserClient() while holding the IOCommandGate.
     * @discussion This is needed to allow removeUserClient() to be called on the IOWorkLoop.
     * @param owner The owner of the IOCommandGate (the IOAudioControl in this case).
     * @param arg1 The IOAudioControlUserClient to be removed.
     * @result Returns the result of removeUserClient() - kIOReturnSuccess on success.
     */
    static IOReturn removeUserClientAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4 ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
	static IOReturn _removeUserClientAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3 ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>

    /*!
     * @function detachUserClientsAction
     */
     static IOReturn detachUserClientsAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4 ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function addUserClient
     * @abstract Called on the IOWorkLoop to add a new IOAudioControlUserClient.
     * @discussion There is no need to call this directly.  It is called on the workLoop 
     *  by newUserClient() through addUserClientAction().
     * @param newUserClient The IOAudioControlUserClientto be added.
     * @result Returns kIOReturnSuccess on success.
     */
    virtual IOReturn addUserClient(IOAudioControlUserClient *newUserClient ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function removeUserClient
     * @abstract Called on the IOWorkLoop to remove an IOAudioControlUserClient.
     * @discussion This is called on the IOWorkLoop by clientClosed() through
     *  removeUserClientAction() when the user client is going away.  It should
     *  not be called directly. 
     * @param userClient The IOAudioControlUserClient to be removed.
     * @result Returns kIOReturnSuccess on success.
     */
    virtual IOReturn removeUserClient(IOAudioControlUserClient *userClient ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual IOReturn detachUserClients( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	static void setCommandGateUsage(IOAudioControl *control, bool increment ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;		// <rdar://8518215>

};

#endif /* _IOKIT_IOAUDIOCONTROL_H */

