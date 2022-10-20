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

/*!
 * @header IOAudioDevice
 */

#ifndef _IOKIT_IOAUDIODEVICE_H
#define _IOKIT_IOAUDIODEVICE_H

#include <IOKit/IOService.h>
#include <AvailabilityMacros.h>

#ifndef IOAUDIOFAMILY_SELF_BUILD
#include <IOKit/audio/IOAudioTypes.h>
#include <IOKit/audio/IOAudioStream.h>
#else
#include "IOAudioTypes.h"
#include "IOAudioStream.h"
#endif

class IOAudioEngine;
class IOAudioStream;
class IOAudioPort;
class IOAudioControl;
class OSDictionary;
class OSSet;
class OSArray;
class IOTimerEventSource;
class IOCommandGate;

/*!
 * @enum IOAudioDevicePowerState
 * @abstract Identifies the power state of the audio device
 * @discussion A newly created IOAudioDevices defaults to the idle state.
 * @constant kIOAudioDeviceSleep State set when the system is going to sleep
 * @constant kIOAudioDeviceIdle State when the system is awake but none of the IOAudioEngines are in use
 * @constant kIOAudioDeviceActive State when one ore more IOAudioEngines are in use.  This state transition must complete before the system will begin playing audio.
 */
typedef enum _IOAudioDevicePowerState {
    kIOAudioDeviceSleep 	= 0,	// When sleeping
    kIOAudioDeviceIdle		= 1,	// When no audio engines running
    kIOAudioDeviceActive 	= 2		// audio engines running
} IOAudioDevicePowerState;

/*!
 * @class IOAudioDevice
 * @abstract Abstract base class for a single piece of audio hardware.  The IOAudioDevice provides
 *  the central coordination point for an audio driver.
 * @discussion An audio driver is required to subclass IOAudioDevice in order to provide
 *  working audio to the system.  A single driver instance will contain a single instance of the 
 *  driver's IOAudioDevice subclass.  The subclass is responsible for mapping all hardware device
 *  resources from the service provider nub.  It must control access to the hardware so that the
 *  hardware doesn't get into an inconsistent state.  It is possible that different threads may make
 *  requests of the hardware at the same time.  The IOAudioDevice superclass provides an IOWorkLoop
 *  and IOCommandGate on the IOWorkLoop through which all hardware accesses may be synchronized.
 *  All entry points to all parts of the driver controlled by the IOAudioFamily will be synchronized
 *  through this one IOWorkLoop.
 *
 *  The IOAudioDevice subclass is responsible for creating the rest of the pieces of the driver.
 *  It must identify and create all IOAudioEngines that are not automatically created by the system
 *  (i.e. those that are not matched and instantiated by IOKit directly).
 *
 *  The IOAudioDevice subclass must enumerate and create all IOAudioControls to match
 *  the device capabilities.
 *
 *  It must also execute control value chages when requested by the system (i.e. volume adjustments).
 *
 *  In order to allow sleep and wake to work on the system, the IOAudioDevice subclass is responsible
 *  for performing the necessary actions to sleep and wake its hardware (and restore necessary state
 *  on wake).
 *
 *  The IOAudioDevice class provides timer services that allow different elements in the audio driver
 *  to receive timer notifications as needed.  These services were designed with the idea that most
 *  timed events in a typical audio driver need to be done at least as often as a certain interval.
 *  Further, it is designed with the idea that being called more often than the specified interval
 *  doesn't hurt anything - and in fact may help.  With this in mind, the timer services provided
 *  by the IOAudioDevice class allow different targets to register for timer callbacks at least as
 *  often as the specified interval.  The actual interval will be the smallest of the intervals of
 *  all of the callbacks.  This way, we avoid the overhead of having many timers in a single audio
 *  device.  As an example, each output IOAudioEngine has a timer to run the erase head.  It doesn't hurt
 *  to have the erase head run more often.  Also, a typical IOAudioDevice subclass may need to run a timer
 *  to check for device state changes (e.g. jack insertions).
 *
 *  There are a number of strings passed from the driver to the CoreAudio.framework and then into
 *  applications.  All of those strings should be localized by the driver.  In order to do that
 *  the kext bundle should have localized string files following the Mac OS X localization 
 *  instructions.  The IOAudioDevice should contain a property with the name of the bundle/kext
 *  that contains the localized string resources.  To do that, the driver's personality in 
 *  the bundle resources could have a property named 'IOAudioDeviceLocalizedBundle' with the path 
 *  of the bundle/kext relative to '/System/Library/Extensions'.  It could also be set by the 
 *  IOAudioDevice subclass in its initHardware() function.  To do so, call
 *  setProperty(kIOAudioDeviceLocalizedBundleKey, "Driver.kext").
 *
 *  In a typical driver, the IOAudioDevice subclass will implement initHardware() to perform the
 *  hardware initialization and driver construction.  Within that initialization it must create at 
 *  least one IOAudioEngine instance and activate it.  In order to activate a new IOAudioEngine
 *  activateAudioEngine() should be called for each one.  It must create the IOAudioControls 
 *  matching the hardware capabilities to allow the system to set volume, mute and input selection.  
 *  To add those controls to the driver, each control should be attached to the IOAudioEngine to
 *  which it applies by calling addDefaultAudioControl() on the IOAudioEngine.
 *  During initialization it should also call setDeviceName(), setDeviceShortName() and 
 *  setManufacturerName() with localized strings representing each of the attributes.
 *
 *  If the driver is to work properly after sleep/wake, it must implement performPowerStateChange()
 *  and deal with the sleep and wake transitions.  It may also deal with the idle state transitions
 *  to turn off device power when it isn't in use (especially useful for devices attached to a 
 *  portable running on battery power).
 */

#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
	#if __MAC_OS_X_VERSION_MIN_REQUIRED >= 101000
		#warning IOAudioDevice is deprecated use <CoreAudio/AudioServerPlugIn.h> instead.
	#endif
#endif

class IOAudioDevice : public IOService
{
    friend class IOAudioEngine;
    
    OSDeclareDefaultStructors(IOAudioDevice)

protected:
    /*! @var workLoop The IOWorkLoop for the driver - this is shared with the other objects in the driver */
    IOWorkLoop				*workLoop;
    /*! @var commandGate The IOCommandGate for this IOAudioDevice.  It is attached to workLoop */
    IOCommandGate			*commandGate;
    /*! @var timerEventSource An IOTimerEventSource attached to workLoop used for the timer services */
    IOTimerEventSource		*timerEventSource;

    /*! @var duringStartup State variable set to true while the driver is starting up and false all other times */
    bool			duringStartup;
    /*! @var familyManagePower Set to true if the family is supposed to manage power - this is the default state.  It can be changed early in the initialization process with a call to setFamilyManagePower(). */
    bool			familyManagePower;
    /*! @var asyncPowerStateChangeInProgress Set to true while an asynchronous power change is pending and false all other times. */
    bool			asyncPowerStateChangeInProgress;
    
    /*! @var numRunningAudioEngines The number of running IOAudioEngines.  This is used to maintain idle vs active power state. */
    UInt32			numRunningAudioEngines;
    
    /*! @var currentPowerState Used to track the existing power state - can be fetched by calling getPowerState() */
    IOAudioDevicePowerState	currentPowerState;
    /*! @var pendingPowerState If a power state change is in progress, this represents the pending power state.  All other times this is the same as the currentPowerState. */
    IOAudioDevicePowerState	pendingPowerState;

    /*! @var audioEngines The set of IOAudioEngine objects vended by the IOAudioDevice. */
    OSArray *			audioEngines;
    /*! @var timerEvents
     *  @abstract The set of timer events in use by the device.  
     *  @discussion The key for the dictionary is the target of the event.  This means that a single target may
     *   have only a single event associated with it.
     */
    OSDictionary *		timerEvents;
    /*! @var audioPorts The set of IOAudioPort objects associated with the IOAudioDevice */
    OSSet *			audioPorts;

    /*! @var minimumInterval The smallest timer interval requested by all timer event targets. */
    AbsoluteTime		minimumInterval;
    /*! @var previousTimerFire The time of the last timer event.
     *  @discussion This is used to schedule the next timer event.
     */
    AbsoluteTime		previousTimerFire;

public:
    /*! @var gIOAudioPlane
     *   A static IORegistryPlane representing the new IOAudioPlane that the IOAudioFamily uses
     *   to represent the signal chain of the device.
     */
    static const IORegistryPlane *gIOAudioPlane;

protected:
    struct ExpansionData {
		unsigned long long			idleSleepDelayTime;
		IOTimerEventSource *		idleTimer;
	};
    
    ExpansionData *reserved;
    
public:
	static void idleAudioSleepHandlerTimer(OSObject *owner, IOTimerEventSource *sender ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn setAggressiveness(unsigned long type, unsigned long newLevel ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioDevice, 0);
	virtual void setDeviceTransportType(const UInt32 transportType ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioDevice, 1);
    /*!
	 * @function setIdleAudioSleepTime
     * @abstract This function is to be called by a driver that doesn't want to be told about the audio
	 * going idle immediately, but at some point in the future.
	 * @discussion This is useful if the device will want to power down its hardware into an idle sleep
	 * state, but doesn't want to do that unless audio hasn't been used for a while.  Calling this function
	 * immediately changes the idle sleep timer and queues it up if the idle is different from the previous
	 * idle time.  The idle time defaults to 0, which means be called immediately (backwards compatible with
	 * previous versions of IOAudioFamily).  A value of 0xffffffffffffffffULL means don't ever tell the
	 * driver about going idle.
     * @param sleepDelay The amount of time, in nanoseconds, before the hardware should be told to go idle.
     */
	virtual void setIdleAudioSleepTime(unsigned long long sleepDelay ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioDevice, 2);
	virtual void scheduleIdleAudioSleep(void ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioDevice, 3);
    /*!
	 * @function setConfigurationApplicationBundle
     * @abstract This function is to be called if an external configuration application is available to set
	 * which application to launch.
	 * @discussion This is useful for device drivers that are too complex to be represented by the Sound Preferences
	 * panel.  The bundle ID is a more flexible way of specifying where the application is than a hard coded path.
     * @param bundleID The bundle ID of the application to be launched by the HAL for configuration of the device and its engine(s).
     */
	virtual void setConfigurationApplicationBundle(const char *bundleID ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioDevice, 4);
    /*!
	 * @function setDeviceCanBeDefault
     * @abstract This function is to be called to tell CoreAudio if this device shouldn't be a default device.
	 * @discussion This is useful for device drivers that don't want to be a default device.  Can be called with
	 * kIOAudioDeviceCanBeDefaultNothing to prevent CoreAudio from allowing this device to be any default device, or it
	 * can be called with any combination of kIOAudioDeviceCanBeDefaultInput, kIOAudioDeviceCanBeDefaultOutput, or
	 * kIOAudioDeviceCanBeSystemOutput.  The default is 
	 * (kIOAudioDeviceCanBeDefaultInput | kIOAudioDeviceCanBeDefaultOutput | kIOAudioDeviceCanBeSystemOutput).
     * @param defaultsFlags The flags to instruct CoreAudio to allow this device to be only the indicated default devices.
     */
	virtual void setDeviceCanBeDefault(UInt32 defaultsFlags ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	// OSMetaClassDeclareReservedUsed(IOAudioDevice, 5);
	virtual void setDeviceModelName(const char * modelName ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

private:
	OSMetaClassDeclareReservedUsed(IOAudioDevice, 0);
	OSMetaClassDeclareReservedUsed(IOAudioDevice, 1);
	OSMetaClassDeclareReservedUsed(IOAudioDevice, 2);
	OSMetaClassDeclareReservedUsed(IOAudioDevice, 3);
	OSMetaClassDeclareReservedUsed(IOAudioDevice, 4);
	OSMetaClassDeclareReservedUsed(IOAudioDevice, 5);

	OSMetaClassDeclareReservedUnused(IOAudioDevice, 6);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 7);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 8);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 9);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 10);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 11);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 12);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 13);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 14);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 15);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 16);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 17);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 18);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 19);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 20);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 21);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 22);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 23);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 24);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 25);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 26);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 27);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 28);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 29);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 30);
	OSMetaClassDeclareReservedUnused(IOAudioDevice, 31);
	

public:
    // Initialization
    
    /*!
     * @function init
     * @abstract Initialize a newly created instance of IOAudioDevice.
     * @discussion This implementation initializes all of the data structures and variables used by the
     *  IOAudioDevice.  The currentPowerState and pendingPowerState variables are set to kIOAudioDeviceIdle.
     *  A subclass that overrides this method must call the superclass' implementation.
     * @param properties
     *  An OSDictionary of the device properties that gets passed to super::init and set
     *  in the IORegistry.
     * @result true if initialization was successful
     */
    virtual bool init(OSDictionary *properties ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function free
     * @abstract Frees resources used by the IOAudioDevice instance
     * @discussion This method will deactivate all audio audio engines and release the audioEngines OSSet.
     *  It will also deactivate all of the audio ports and release the audioPorts OSSet.  It will release
     *  the timerEvents OSDictionary as well as cancel any outstanding timer callbacks.  It will clean up
     *  all of the event sources and the workLoop.
     *
     *  Do not call this directly.  This is called automatically by the system when the instance's
     *  refcount goes to 0.  To decrement the refcount, call release() on the object.
     */
     
    virtual void free( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function start
     * @abstract This function is called automatically by the system to tell the driver to start vending
     *  services to the rest of the system.
     * @discussion The start() implementation in IOAudioDevice starts by calling start() on its superclass.
     *  It then calls initHardware() which the subclass should override to properly initialize itself and 
     *  the hardware.  If that call succeeds, it sets up power management if the family is supposed to 
     *  manage power (checking the familyManagePower variable).  Then finally it calls registerService() 
     *  to make the IOAudioDevice visible in the IORegistry.
     * @param provider
     *  This is the service provider nub that provides access to the hardware resources.
     * @result Returns true on success
     */
    virtual bool start(IOService *provider ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function stop
     * @abstract This is responsible for stopping the device after the system is done with it (or
     *  if the device is removed from the system).
     * @discussion The IOAudioDevice implentation of stop() disables the timer services, deactivates 
     *  all of the audio audio engines and audio ports and stops power management of the device.
     *  The audio engine and port deactivation causes all of the audio engines to get stopped and 
     *  all of the audio engine and port resources and objects to be released.  A subclass' implementation 
     *  may could shut down hardware here if necessary.  If this function is overridden by a subclass,
     *  the superclass' implementation must be called.
     * @param provider
     *  The service provider nub for the device.
     */
    virtual void stop(IOService *provider ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual bool willTerminate(IOService *provider, IOOptionBits options ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function initHardware
     * @abstract This function is called by start() to provide a convenient place for the subclass to 
     *  perform its initialization.
     * @discussion In a typical implementation, a driver would implementation this function and perform
     *  a number of tasks.  Those include mapping hardware resources, initializing the hardware to a known
     *  state, creating the IOAudioEngines, IOAudioControls and IOAudioStreams.  Additionally it 
     *  should also call setDeviceName(), setDeviceShortName(), setManufacturerName().  Upon return of
     *  this function, the device should be ready to begin vending services to the system.
     * @param provider
     *  The service provider nub for the device.
     * @result This function should return true on a successful initialization.
     */
    virtual bool initHardware(IOService *provider ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function setDeviceName
     * @abstract Sets the name of the device
     * @discussion This method should be called during initialization or startup.  It should
     *  be set by the time initHardware() completes.  The device name is used by the 
     *  CoreAudio.framework to identify the particular piece of hardware.  This string should
     *  should be localized by the driver.
     */
    virtual void setDeviceName(const char *deviceName ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function setDeviceShortName
     * @abstract Sets the short name of the device
     * @discussion The short name is a shorter representation of the device name.  It may be used
     *  by applications when the device name is too long.  It should be set by the time initHardware()
     *  completes.  The string should be localized by the driver.
     */
    virtual void setDeviceShortName(const char *shortName ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function setManufacturerName
     * @abstract Sets the manufacturer name of the device
     * @discussion This method should be called during initialization or startup.  This should be 
     *  called by the time initHardware() completes.  The string should be localized by the driver.
     */
    virtual void setManufacturerName(const char *manufacturerName ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    
    // IOWorkLoop, IOCommandGate management
    
    /*!
     * @function getWorkLoop
     * @abstract Returns the IOWorkLoop for the driver
     * @discussion The IOWorkLoop is used to synchronized all critical aspects of the driver.  This
     *  includes all entry points to the driver as well as power management.
     */
    virtual IOWorkLoop *getWorkLoop() const AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function getCommandGate
     * @abstract Returns the IOCommandGate for this IOAudioDevice
     * @discussion This IOCommandGate allows calls into this IOAudioDevice to be synchronized on 
     *  the IOWorkLoop.
     */
    virtual IOCommandGate *getCommandGate() const AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    
    // IOAudioEngine management
    
    /*!
     * @function activateAudioEngine
     * @abstract This simply calls activateAudioEngine(IOAudioEngine *audioEngine, 
     *  bool shouldStartAudioEngine) with a value of true for shouldStartAudioEngine.
     * @param audioEngine
     *  The IOAudioEngine instance to be activated.  It is treated as a newly 
     *  allocated instance.
     * @result Returns true if the audio engine was successfully activated.
     */
    virtual IOReturn activateAudioEngine(IOAudioEngine *audioEngine ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function activateAudioEngine
     * @abstract This is called to add a new IOAudioEngine object to the IOAudioDevice.
     * @discussion Once the IOAudioEngine has been activated by this function, it is ready
     *  to begin moving audio data.  This should be called either during the subclass' initHardware()
     *  implementation for each IOAudioEngine the device creates.  Or it should be called by
     *  the IOAudioEngine itself if the audio engine was automatically created by IOKit's matching
     *  process.  The system won't be able to properly track and control IOAudioEngines if
     *  they are not activated though this function.
     *  This implementation will retain the IOAudioEngine while it maintains control of it.  
     *  When the audio engine is deactivated, the IOAudioEngine will be released.  If the 
     *  IOAudioDevice subclass is passing a newly instantiated IOAudioEngine, it will need to release 
     *  the audio engine after it has been activated.  This will insure that the refCount on the audio engine 
     *  is correct when it gets deactivated when the driver is stopped.  That allows the audio engine to be
     *  freed when it is no longer needed.
     * @param audioEngine
     *  The IOAudioEngine instance to be activated.
     * @param shouldStartAudioEngine
     *  If true, the audio engine is treated as a newly allocated IOAudioEngine
     *  instance and is appropriately attached and started according to IOKit convention.  If it is false
     *  it is assumed that some other process (possibly the IOKit matching process) has started the
     *  IOAudioEngine and will skip that step.
     * @result Returns true if the audio engine was successfully activated.
     */
    virtual IOReturn activateAudioEngine(IOAudioEngine *audioEngine, bool shouldStartAudioEngine ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function deactivateAllAudioEngines
     * @abstract Deactivates all of the audio engines in the device.
     * @discussion This is called by the stop() and free() methods in IOAudioDevice to completely
     *  shut down all audio engines as the driver is being shut down.
     */
    virtual void deactivateAllAudioEngines( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    
    // Power management
    
    /*!
     * @function setFamilyManagePower
     * @abstract Called set whether or not the family should manage the device power throught the
     *  IOService power management APIs.
     * @discussion The default behavior is for the family to manage power.  It is only necessary to 
     *  call this function if the driver does not want the family to manage power.  It is not
     *  recommended that this function be called because it makes power management much more
     *  difficult for the driver.  If this function is to be called, it must be called before
     *  initHardware() completes.  Immediately after initHardware() is called by start(), 
     *  the power management system is initialized if the family is to manage power.
     * @param manage Set to false if it is not desired that the family does the power management
     */
    virtual void setFamilyManagePower(bool manage ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function setPowerState
     * @abstract Called by the power management system in IOService when the power state of this 
     *  service needs to change.
     * @discussion The default implementation of IOAudioDevice sets up two power states for IOService
     *  to use.  State 0 is sleep and state 1 is wake.  This function should not be called directly.
     *  It is only supposed to be used by the IOService power management services.
     * @param powerStateOrdinal
     *  The number of the power state as defined by the IOAudioDevice - 
     *  0 for sleep, 1 for wake.
     * @param device The power management policy maker.
     * @result Returns kIOPMAckImplied (0) when the power state change is complete.  Otherwise the an
     *  upper bound on the number of microseconds until the state change is complete is returned.
     */
    virtual IOReturn setPowerState(unsigned long powerStateOrdinal, IOService *device ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function setPowerStateAction
     * @abstract IOCommandGate Action which calls protectedSetPowerState() while holding the IOCommandGate
     * @discussion This is needed to allow protectedSetPowerState() to be called on the IOWorkLoop
     * @param owner The owner of the IOCommandGate (the IOAudioDevice in this case)
     * @param arg1 The powerStateOrdinal to be passed to protectedSetPowerState()
     * @param arg2 The device to be passed to protectedSetPowerState()
     * @result Returns the result of protectedSetPowerState()
     */
    static IOReturn setPowerStateAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4 ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
	static IOReturn _setPowerStateAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3 ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://8508064>

    /*!
     * @function protectedSetPowerState
     * @abstract Called by setPowerStateAction() to deal with a power state change from the IOService
     *  power management facility.
     * @discussion This function is responsible for performing the necessary sleep and wake tasks
     *  when the system is sleeping or waking.  If an outstanding power state change is in progress,
     *  it will wait until the state changes has completed.  While sleeping, all audio engines are
     *  stopped before calling initiatePowerStateChange() to call performPowerStateChange() to let
     *  the driver deal with the sleep request.  When waking, it determines if the device should be
     *  idle or active and continues to call initiatePowerStateChange().  If initiatePowerStateChange()
     *  indicates that the power state change is occuring asynchronously, it returns the number of
     *  microseconds.  This function must be called on the IOWorkLoop, but should not be called
     *  directly.
     * @param powerStateOrdinal Param passed to setPowerState() - 0 for sleep, 1 for wake
     * @param device Param passed to setPowerState - the device initiating the power state change
     * @result Returns 0 if the power state change is complete - the number of microseconds until 
     *  complete if its asynchronous.
     */
    virtual IOReturn protectedSetPowerState(unsigned long powerStateOrdinal, IOService *device ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function performPowerStateChange
     * @abstract This function is called by the IOAudioDevice when a power state change is needed.
     * @discussion In order to deal with power state changes, a subclass must override this function.
     *  Any combination of old and new power states may be passed to this function.  If work is to
     *  be performed while transitioning to sleep, check for a newPowerState of kIOAudioDeviceSleep.
     *  If work is to be performed while transitioning from sleep, check for an oldPowerState of
     *  kIOAudioDeviceSleep.  A power state of kIOAudioDeviceIdle means the system is awake, but
     *  no clients are currently playing or recording audio (i.e. no IOAudioEngines are active).
     *  A power state of kIOAudioDeviceActive means that at least one IOAudioEngine is active.
     *  It is possible for a power state change to be performed synchronously or asynchronously.
     *  In the case of a synchronous power state change, simple leave microsecondsUntilComplete
     *  alone and return kIOReturnSuccess.  If an asynchronous power state change is needed the 
     *  driver should do whatever needed to schedule another thread to finish the state change
     *  and set the microsecondsUntilComplete to an upper bound on the amount of time it will
     *  take to complete the power state change.  Then when the power state change is complete,
     *  a call must be made to completePowerStateChange().  During an asynchronous power state
     *  change, the current power state will remain the same as before the transition began, 
     *  and the pendingPowerState is set to the new power state that will be set when the 
     *  change is complete.
     * @param oldPowerState The power state before the power state change
     * @param newPowerState The power state being transitioned to
     * @param microsecondsUntilComplete
     *  A pointer to a value representing an upper bound on
     *  the number of microseconds to complete an asynchronous power state change.  It points
     *  to a value of zero at the start and if it remains zero, the state change is complete
     *  upon a successful return from the function.
     * @result Returns kIOReturnSuccess on a successful completion
     */
    virtual IOReturn performPowerStateChange(IOAudioDevicePowerState oldPowerState,
                                                IOAudioDevicePowerState newPowerState,
                                                UInt32 *microsecondsUntilComplete ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
                                                    
    /*!
     * @function completePowerStateChange
     * @abstract Called when a power state change is complete
     * @discussion In the case of an asynchronous power state change, a subclass is responsible
     *  for calling this function.  It is safe to call this function if not on the IOWorkLoop.
     *  This function calls protectedCompletePowerStateChange() through the IOCommandGate and
     *  completePowerStateChangeAction().  If the call is already on the IOWorkLoop, it is safe
     *  to call protectedCompletePowerStateChange() directly.
     * @result Returns kIOReturnSuccess on a successful completion
     */
    virtual IOReturn completePowerStateChange( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function completePowerStateChangeAction
     * @abstract IOCommandGate Action which calls protectedCompletePowerStateChange() while holding the
     *  IOCommandGate.
     * @discussion This is needed to allow protectedCompletePowerStateChange() to be called on the IOWorkLoop.
     * @param owner The owner of the IOCommandGate (the IOAudioDevice in this case)
     * @result Returns the result of protectedCompletePowerStateChange()
     */
    static IOReturn completePowerStateChangeAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4 ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function protectedCompletePowerStateChange
     * @abstract Called on the IOWorkLoop when a power state change is complete.
     * @discussion This function does the work to complete a power state change (both synchronous and
     *  asynchronous).  If the system is waking from sleep, the timer system is restarted and the 
     *  audio engines are resumed.  If this was called as a result of an asynchronous power state changed
     *  it makes the IOService power management call acknowledgePowerChange() and resets the
     *  asyncPowerStateChangeInProgress variable.  Finally it sets the currentPowerState to the
     *  pendingPowerState.  This function must be called on the IOWorkLoop.  If a subclass is not
     *  on the IOWorkLoop (e.g. holding the IOCommandGate), call completePowerStateChange() instead.
     * @result Returns kIOReturnSuccess on success
     */
    virtual IOReturn protectedCompletePowerStateChange( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function getPowerState
     * @abstract Returns the current power state (the old power state if a change is in progress).
     * @result The current power state
     */
    virtual IOAudioDevicePowerState getPowerState( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function getPendingPowerState
     * @abstract Returns the pending power state if a state change is in progress.  Otherwise it 
     *  returns the current power state change.
     * @result The pending power state
     */
    virtual IOAudioDevicePowerState getPendingPowerState( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function waitForPendingPowerStateChange
     * @abstract Called internally to wait until a pending power state change is complete.
     * @discussion This is only used by internal functions to wait during pending power
     *  state changes.  It is used to prevent multiple power state changes at the same time.
     *  This function must be called while holding the IOCommandGate.  If an asynchronous 
     *  power state change is in progress this function will block until the state change
     *  if complete.  Once complete, it will return while still holding the IOCommandGate.
     */
    virtual void waitForPendingPowerStateChange( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function initiatePowerStateChange
     * @abstract Called internally to execute a power state change
     * @discussion This function must be called on the IOWorkLoop.  It calls performPowerStateChange()
     *  to let the driver process the state change.  If the state change is completed synchronously
     *  by the driver (subclass) it calls protectedCompletePowerStateChange().  If done asynchronously
     *  it returns the microsecondsUntilComplete that was set by performPowerStateChange().  This
     *  function should not be called directly.
     * @param microsecondsUntilComplete
     *  Pointer to the microsecondsUntilComplete that should be set
     *  by performPowerStateChange if an asynchronous power state change was started.
     * @result Returns kIOReturnSuccess on success
     */
    virtual IOReturn initiatePowerStateChange(UInt32 *microsecondsUntilComplete = NULL ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    

    // IOAudioControl management
    
    /*!
     * @function flushAudioControls
     * @abstract Forces each IOAudioControl in the driver to have its value flushed out to the hardware.
     *  That will cause either the IOAudioControl's ValueChangeHandler to be called.
     * @discussion This can be used to force the hardware to get updated with the current value 
     *  of each control.  It may be useful during wake for example.
     */
    virtual void flushAudioControls( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;


    // Timer services
    
    /*!
     * @typedef TimerEvent
     * @abstract Generic timer event callback for IOAudioDevice timer targets
     * @discussion TimerEvent callback function takes two arguments; the target of
     *  the timer event and the IOAudioDevice sending the event.
     * @param target The target of the timer event - passed in when the timer event was registered
     * @param audioDevice The IOAudioDevice sending the event
     */
    typedef void (*TimerEvent)(OSObject *target, IOAudioDevice *audioDevice ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function addTimerEvent
     * @abstract Adds a TimerEvent callback for the given target called at least as often
     *  as specified in interval.
     * @discussion The frequency of the timer event callbacks will be the smallest interval
     *  specified by all targets.  Only one interval and callback may be specified per target.
     *  If a addTimerEvent is called twice with the same target, the second one overrides the
     *  first.  There is currently a bug triggered if the first call had the smallest interval.
     *  In that case, that smallest interval would still be used.
     * @param target This parameter is the target object of the TimerEvent.
     * @param event The callback function called each time the timer fires.
     * @param interval The callback will be called at least this often.
     * @result Returns kIOReturnSuccess if the timer event was successfully added.
     */
    virtual IOReturn addTimerEvent(OSObject *target, TimerEvent event, AbsoluteTime interval ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function removeTimerEvent
     * @abstract Removes the timer event for the given target.
     * @discussion If the interval for the target to be removed is the smallest interval,
     *  the timer interval is recalculated based on the remaining targets.  The next fire
     *  time is readjusted based on the new interval compared to the last fire time.
     * @param target The target whose timer event will be removed.
     */
    virtual void removeTimerEvent(OSObject *target ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function removeAllTimerEvents
     * @abstract Removes all timer events and stops the timer
     * @discussion Called during teardown of the driver
     */    
    virtual void removeAllTimerEvents( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    
    // IOAudioPort management

    /*!
     * @function attachAudioPort
     * @abstract Adds the port to the IOAudioDevice's list of ports and attaches the port to its parent
     *  and attaches the child to the port.
     * @discussion This function provides the functionality to represent the device's signal chain in the
     *  IOAudioPlane in the IORegistry.  An IOAudioPort's parent(s) are before it in the signal chain
     *  and its children are after it.  This method may be called multiple times for a single IOAudioPort.
     *  This is necessary when there are many children or parents.  Once a relationship is made, it is not
     *  necessary to make the reverse relationship.  A NULL value may be passed in for either the parent
     *  or child or both.
     *  The IOAudioPort passed in should be a newly allocated IOAudioPort instance.  This function will
     *  appropriately attach and start the port object.  NOTE: It is not necessary to use IOAudioPorts
     *  in a fully functional audio driver.
     * @param port The newly created IOAudioPort instance to be activated.
     * @param parent A parent IOAudioPort or IOAudioEngine of the given port.
     * @param child A child IOAudioPort or IOAudioEngine of the given port.
     * @result Returns true when the port has been successfully added and attached.
     */
    virtual IOReturn attachAudioPort(IOAudioPort *port, IORegistryEntry *parent, IORegistryEntry *child ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function detachAllAudioPorts
     * @abstract Deactivates all of the ports in the device.
     * @discussion This is called by the stop() and free() methods in IOAudioDevice to completely
     *  shut down all ports as the driver is being shut down.
     */
    virtual void detachAllAudioPorts( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

protected:
    /*!
     * @function timerFired
     * @abstract Internal static function called when the timer fires.
     * @discussion This function simply calls dispatchTimerEvents() on the IOAudioDevice to do just that.
     * @param target The IOAudioDevice instance that initiated the timer callback.
     * @param sender The IOTimerEventSources calling this callback
     */
    static void timerFired(OSObject *target, IOTimerEventSource *sender ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function dispatchTimerEvents
     * @abstract Called by timerFired() to cause the timer event callbacks to be called.
     * @discussion This method iterates through all of the timer event targets and calls
     *  the callback on each.  Unless the force flag is set to true, the timer events will
     *  only be dispatched if the power state is not kIOAudioDeviceSleep.  This prevents
     *  unexpected timer firings while making wake->sleep->wake transitions.  This function must
     *  be called on the IOWorkLoop.
     * @param force
     *  A bool param to allow the timer events to be dispatched even if the 
     *  device is in the kIOAudioDeviceSleep power state.
     */
    virtual void dispatchTimerEvents(bool force ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    /*!
     * @function audioEngineStarting
     * @abstract Called by IOAudioEngine when it is starting up
     * @discussion This should only be called while on the IOWorkLoop.  It is not intended to be called
     *  directly.  It is called when an IOAudioEngine is starting up to allow the IOAudioDevice
     *  to keep track of running audio engines and change the power state from kIOAudioDeviceIdle to
     *  kIOAudioDeviceActive when the first audio engine starts up.  If the state change is done
     *  asynchronously, it waits for the state change to complete.  This is to ensure that the
     *  system doesn't start playing audio until the IOAudioDevice has completed its transition
     *  to kIOAudioDeviceActive.
     */
    virtual void audioEngineStarting( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    /*!
     * @function audioEngineStopped
     * @abstract Called by IOAudioEngine when it has stopped
     * @discussion This should only be called while on the IOWorkLoop.  It is not intended to be called
     *  directly.  It is called when an IOAudioEngine has stopped to allow the IOAudioDevice
     *  to keep track of running audio engines and change the power state from kIOAudioDeviceActive
     *  to kIOAudioDeviceIdle when the last audio engine stops.  If the state change is done 
     *  asynchronously, it waits for the state change to complete.
     */
    virtual void audioEngineStopped( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
};

#endif /* _IOKIT_IOAUDIODEVICE_H */
