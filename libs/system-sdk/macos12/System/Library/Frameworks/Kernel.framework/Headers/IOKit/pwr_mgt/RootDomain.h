/*
 * Copyright (c) 1998-2020 Apple Inc. All rights reserved.
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
#ifndef _IOKIT_ROOTDOMAIN_H
#define _IOKIT_ROOTDOMAIN_H

#include <libkern/c++/OSPtr.h>
#include <IOKit/IOService.h>
#include <IOKit/pwr_mgt/IOPM.h>
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <sys/vnode.h>


/*!
 * Types for PM Assertions
 * For creating, releasing, and getting PM assertion levels.
 */

/*! IOPMDriverAssertionType
 * A bitfield describing a set of assertions. May be used to specify which assertions
 * to set with <link>IOPMrootDomain::createPMAssertion</link>; or to query which
 * assertions are set with <link>IOPMrootDomain::releasePMAssertion</link>.
 */
typedef uint64_t IOPMDriverAssertionType;

/* IOPMDriverAssertionID
 * Drivers may create PM assertions to request system behavior (keep the system awake,
 *  or keep the display awake). When a driver creates an assertion via
 *  <link>IOPMrootDomain::createPMAssertion</link>, PM returns a handle to
 *  the assertion of type IOPMDriverAssertionID.
 */
typedef uint64_t IOPMDriverAssertionID;
#define kIOPMUndefinedDriverAssertionID       0

/* IOPMDriverAssertionLevel
 * Possible values for IOPMDriverAssertionLevel are <link>kIOPMDriverAssertionLevelOff</link>
 * and <link>kIOPMDriverAssertionLevelOn</link>
 */
typedef uint32_t IOPMDriverAssertionLevel;
#define kIOPMDriverAssertionLevelOff          0
#define kIOPMDriverAssertionLevelOn           255

/*
 * Flags for get/setSleepSupported()
 */
enum {
	kRootDomainSleepNotSupported    = 0x00000000,
	kRootDomainSleepSupported         = 0x00000001,
	kFrameBufferDeepSleepSupported    = 0x00000002,
	kPCICantSleep                   = 0x00000004
};

/*
 * IOPMrootDomain registry property keys
 */
#define kRootDomainSupportedFeatures        "Supported Features"
#define kRootDomainSleepReasonKey           "Last Sleep Reason"
#define kRootDomainSleepOptionsKey          "Last Sleep Options"
#define kIOPMRootDomainWakeReasonKey        "Wake Reason"
#define kIOPMRootDomainWakeTypeKey          "Wake Type"
#define kIOPMRootDomainPowerStatusKey       "Power Status"

/*
 * Possible sleep reasons found under kRootDomainSleepReasonsKey
 */
#define kIOPMClamshellSleepKey              "Clamshell Sleep"
#define kIOPMPowerButtonSleepKey            "Power Button Sleep"
#define kIOPMSoftwareSleepKey               "Software Sleep"
#define kIOPMOSSwitchHibernationKey         "OS Switch Sleep"
#define kIOPMIdleSleepKey                   "Idle Sleep"
#define kIOPMLowPowerSleepKey               "Low Power Sleep"
#define kIOPMThermalEmergencySleepKey       "Thermal Emergency Sleep"
#define kIOPMMaintenanceSleepKey            "Maintenance Sleep"

/*
 * String constants for communication with PM CPU
 */
#define kIOPMRootDomainLidCloseCString      "LidClose"
#define kIOPMRootDomainBatPowerCString      "BatPower"

/*
 * Supported Feature bitfields for IOPMrootDomain::publishFeature()
 */
enum {
	kIOPMSupportedOnAC      = (1 << 0),
	kIOPMSupportedOnBatt    = (1 << 1),
	kIOPMSupportedOnUPS     = (1 << 2)
};

typedef IOReturn (*IOPMSettingControllerCallback)
(OSObject *target, const OSSymbol *type,
    OSObject *val, uintptr_t refcon);

__BEGIN_DECLS
IONotifier *    registerSleepWakeInterest(
	IOServiceInterestHandler, void *, void * = NULL);

IONotifier *    registerPrioritySleepWakeInterest(
	IOServiceInterestHandler handler,
	void * self, void * ref = NULL);

IOReturn        acknowledgeSleepWakeNotification(void * );

IOReturn        vetoSleepWakeNotification(void * PMrefcon);
__END_DECLS

#define IOPM_ROOTDOMAIN_REV        2

class IOPMrootDomain : public IOService
{
	OSDeclareFinalStructors(IOPMrootDomain);

public:
	static IOPMrootDomain * construct( void );

	virtual bool        start( IOService * provider ) APPLE_KEXT_OVERRIDE;
	virtual IOReturn    setAggressiveness( unsigned long, unsigned long ) APPLE_KEXT_OVERRIDE;
	virtual IOReturn    getAggressiveness( unsigned long, unsigned long * ) APPLE_KEXT_OVERRIDE;

	virtual IOReturn    sleepSystem( void );
	IOReturn            sleepSystemOptions( OSDictionary *options );

	virtual IOReturn    setProperties( OSObject * ) APPLE_KEXT_OVERRIDE;
	virtual bool        serializeProperties( OSSerialize * s ) const APPLE_KEXT_OVERRIDE;
	virtual OSPtr<OSObject>  copyProperty( const char * aKey ) const APPLE_KEXT_OVERRIDE;

/*! @function systemPowerEventOccurred
 *    @abstract Other drivers may inform IOPMrootDomain of system PM events
 *    @discussion systemPowerEventOccurred is a richer alternative to receivePowerNotification()
 *        Only Apple-owned kexts should have reason to call systemPowerEventOccurred.
 *    @param event An OSSymbol describing the type of power event.
 *    @param intValue A 32-bit integer value associated with the event.
 *    @result kIOReturnSuccess on success */

	IOReturn            systemPowerEventOccurred(
		const OSSymbol *event,
		uint32_t intValue );

	IOReturn            systemPowerEventOccurred(
		const OSSymbol *event,
		OSObject *value );

	void                claimSystemWakeEvent(
		IOService     *device,
		IOOptionBits  flags,
		const char    *reason,
		OSObject      *details = NULL );

	void                claimSystemBootEvent(
		IOService     *device,
		IOOptionBits  flags,
		const char    *reason,
		OSObject      *details = NULL );

	void                claimSystemShutdownEvent(
		IOService     *device,
		IOOptionBits  flags,
		const char    *reason,
		OSObject      *details = NULL );

	virtual IOReturn    receivePowerNotification( UInt32 msg );

	virtual void        setSleepSupported( IOOptionBits flags );

	virtual IOOptionBits getSleepSupported( void );

	void                wakeFromDoze( void );

	void                requestUserActive(IOService *driver, const char *reason);

// KEXT driver announces support of power management feature

	void                publishFeature( const char *feature );

// KEXT driver announces support of power management feature
// And specifies power sources with kIOPMSupportedOn{AC/Batt/UPS} bitfield.
// Returns a unique uint32_t identifier for later removing support for this
// feature.
// NULL is acceptable for uniqueFeatureID for kexts without plans to unload.

	void                publishFeature( const char *feature,
	    uint32_t supportedWhere,
	    uint32_t *uniqueFeatureID);

// KEXT driver announces removal of a previously published power management
// feature. Pass 'uniqueFeatureID' returned from publishFeature()

	IOReturn            removePublishedFeature( uint32_t removeFeatureID );

/*! @function copyPMSetting
 *   @abstract Copy the current value for a PM setting. Returns an OSNumber or
 *       OSData depending on the setting.
 *   @param whichSetting Name of the desired setting.
 *   @result OSObject value if valid, NULL otherwise. */

	OSPtr<OSObject>           copyPMSetting( OSSymbol *whichSetting );

/*! @function registerPMSettingController
 *   @abstract Register for callbacks on changes to certain PM settings.
 *   @param settings NULL terminated array of C strings, each string for a PM
 *       setting that the caller is interested in and wants to get callbacks for.
 *   @param callout C function ptr or member function cast as such.
 *   @param target The target of the callback, usually 'this'
 *   @param refcon Will be passed to caller in callback; for caller's use.
 *   @param handle Caller should keep the OSObject * returned here. If non-NULL,
 *       handle will have a retain count of 1 on return. To deregister, pass to
 *       unregisterPMSettingController()
 *   @result kIOReturnSuccess on success. */

	IOReturn            registerPMSettingController(
		const OSSymbol *settings[],
		IOPMSettingControllerCallback callout,
		OSObject   *target,
		uintptr_t  refcon,
		OSObject   **handle);                     // out param

/*! @function registerPMSettingController
 *   @abstract Register for callbacks on changes to certain PM settings.
 *   @param settings NULL terminated array of C strings, each string for a PM
 *       setting that the caller is interested in and wants to get callbacks for.
 *   @param supportedPowerSources bitfield indicating which power sources these
 *       settings are supported for (kIOPMSupportedOnAC, etc.)
 *   @param callout C function ptr or member function cast as such.
 *   @param target The target of the callback, usually 'this'
 *   @param refcon Will be passed to caller in callback; for caller's use.
 *   @param handle Caller should keep the OSObject * returned here. If non-NULL,
 *       handle will have a retain count of 1 on return. To deregister, pass to
 *       unregisterPMSettingController()
 *   @result kIOReturnSuccess on success. */

	IOReturn            registerPMSettingController(
		const OSSymbol *settings[],
		uint32_t   supportedPowerSources,
		IOPMSettingControllerCallback callout,
		OSObject   *target,
		uintptr_t  refcon,
		OSObject   **handle);                     // out param

	virtual OSPtr<IONotifier> registerInterest(
		const OSSymbol * typeOfInterest,
		IOServiceInterestHandler handler,
		void * target, void * ref = NULL ) APPLE_KEXT_OVERRIDE;

	virtual IOReturn    callPlatformFunction(
		const OSSymbol *functionName,
		bool waitForFunction,
		void *param1, void *param2,
		void *param3, void *param4 ) APPLE_KEXT_OVERRIDE;

/*! @function createPMAssertion
 *   @abstract Creates an assertion to influence system power behavior.
 *   @param whichAssertionsBits A bitfield specify the assertion that the caller requests.
 *   @param assertionLevel An integer detailing the initial assertion level, kIOPMDriverAssertionLevelOn
 *       or kIOPMDriverAssertionLevelOff.
 *   @param ownerService A pointer to the caller's IOService class, for tracking.
 *   @param ownerDescription A reverse-DNS string describing the caller's identity and reason.
 *   @result On success, returns a new assertion of type IOPMDriverAssertionID
 */
	IOPMDriverAssertionID createPMAssertion(
		IOPMDriverAssertionType whichAssertionsBits,
		IOPMDriverAssertionLevel assertionLevel,
		IOService *ownerService,
		const char *ownerDescription);

/* @function setPMAssertionLevel
 *  @abstract Modify the level of a pre-existing assertion.
 *  @discussion Change the value of a PM assertion to influence system behavior,
 *   without undergoing the work required to create or destroy an assertion. Suggested
 *   for clients who will assert and de-assert needs for PM behavior several times over
 *   their lifespan.
 *  @param assertionID An assertion ID previously returned by <link>createPMAssertion</link>
 *  @param assertionLevel The new assertion level.
 *  @result kIOReturnSuccess if it worked; kIOReturnNotFound or other IOReturn error on failure.
 */
	IOReturn setPMAssertionLevel(IOPMDriverAssertionID assertionID, IOPMDriverAssertionLevel assertionLevel);

/*! @function getPMAssertionLevel
 *   @absract Returns the active level of the specified assertion(s).
 *   @discussion Returns <link>kIOPMDriverAssertionLevelOff</link> or
 *       <link>kIOPMDriverAssertionLevelOn</link>. If multiple assertions are specified
 *       in the bitfield, only returns <link>kIOPMDriverAssertionLevelOn</link>
 *       if all assertions are active.
 *   @param whichAssertionBits Bits defining the assertion or assertions the caller is interested in
 *       the level of. If in doubt, pass <link>kIOPMDriverAssertionCPUBit</link> as the argument.
 *   @result Returns <link>kIOPMDriverAssertionLevelOff</link> or
 *       <link>kIOPMDriverAssertionLevelOn</link> indicating the specified assertion's levels, if available.
 *       If the assertions aren't supported on this machine, or aren't recognized by the OS, the
 *       result is undefined.
 */
	IOPMDriverAssertionLevel getPMAssertionLevel(IOPMDriverAssertionType whichAssertionBits);

/*! @function releasePMAssertion
 *   @abstract Removes an assertion to influence system power behavior.
 *   @result On success, returns a new assertion of type IOPMDriverAssertionID *
 */
	IOReturn releasePMAssertion(IOPMDriverAssertionID releaseAssertion);

/*! @function restartWithStackshot
 *   @abstract Take a stackshot of the system and restart the system.
 *   @result Return kIOReturnSuccess if it work, kIOReturnError if the service is not available.
 */
	IOReturn restartWithStackshot();

	IOReturn    setWakeTime(uint64_t wakeContinuousTime);

private:
	unsigned long getRUN_STATE(void);

	virtual IOReturn    changePowerStateTo( unsigned long ordinal ) APPLE_KEXT_COMPATIBILITY_OVERRIDE;
	virtual IOReturn    changePowerStateToPriv( unsigned long ordinal );
	virtual IOReturn    requestPowerDomainState( IOPMPowerFlags, IOPowerConnection *, unsigned long ) APPLE_KEXT_OVERRIDE;
	virtual void        powerChangeDone( unsigned long ) APPLE_KEXT_OVERRIDE;
	virtual bool        tellChangeDown( unsigned long ) APPLE_KEXT_OVERRIDE;
	virtual bool        askChangeDown( unsigned long ) APPLE_KEXT_OVERRIDE;
	virtual void        tellChangeUp( unsigned long ) APPLE_KEXT_OVERRIDE;
	virtual void        tellNoChangeDown( unsigned long ) APPLE_KEXT_OVERRIDE;
	virtual IOReturn configureReport(IOReportChannelList   *channels,
	    IOReportConfigureAction action,
	    void                    *result,
	    void                    *destination) APPLE_KEXT_OVERRIDE;
	virtual IOReturn updateReport(IOReportChannelList      *channels,
	    IOReportUpdateAction     action,
	    void                     *result,
	    void                     *destination) APPLE_KEXT_OVERRIDE;

	void             configureReportGated(uint64_t channel_id,
	    uint64_t action,
	    void     *result);
	IOReturn         updateReportGated(uint64_t ch_id,
	    void *result,
	    IOBufferMemoryDescriptor *dest);

};


#endif /* _IOKIT_ROOTDOMAIN_H */
