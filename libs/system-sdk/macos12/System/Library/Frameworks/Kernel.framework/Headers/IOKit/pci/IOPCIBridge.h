/*
 * Copyright (c) 1998-2021 Apple Computer, Inc. All rights reserved.
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
/*
 * Copyright (c) 1998-2000 Apple Computer, Inc.  All rights reserved. 
 *
 * HISTORY
 *
 */


#ifndef _IOKIT_IOPCIBRIDGE_H
#define _IOKIT_IOPCIBRIDGE_H

#include <IOKit/IOService.h>
#include <IOKit/IODeviceMemory.h>
#include <IOKit/IOFilterInterruptEventSource.h>
#include <IOKit/pwr_mgt/RootDomain.h>
#include <IOKit/pci/IOAGPDevice.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class IOPCIConfigurator;
class IOPCIDevice;
class IOPCIMessagedInterruptController;
class IOPCIHostBridgeData;

enum {
    kIOPCIResourceTypeMemory         = 0,
    kIOPCIResourceTypePrefetchMemory = 1,
    kIOPCIResourceTypeIO             = 2,
    kIOPCIResourceTypeBusNumber      = 3,
    kIOPCIResourceTypeCount          = 4,
};

typedef struct
{
    IOService * device;
    uintptr_t op;
    void * result;
    void * arg;
} configOpParams;

/*!
    @class IOPCIBridge
    @abstract   Base class for all PCI bridge drivers.
*/

__exported_push
class __kpi_deprecated("Use PCIDriverKit") IOPCIBridge : public IOService
{
    friend class IOPCIDevice;
    friend class IOPCI2PCIBridge;
    friend class IOPCIConfigurator;
    friend class IOPCIEventSource;
    friend class IOPCIHostBridge;

    OSDeclareAbstractStructors(IOPCIBridge)

private:
    static void initialize(void);
    IORegistryEntry * findMatching( OSIterator * in, IOPCIAddressSpace space );
    virtual bool isDTNub( IOPCIDevice * nub );
    bool checkProperties( IOPCIDevice * entry );

    void removeDevice( IOPCIDevice * device, IOOptionBits options = 0 );

    void restoreQEnter(IOPCIDevice * device);
    void restoreQRemove(IOPCIDevice * device);

    IOReturn restoreTunnelState(IOPCIDevice * rootDevice, IOOptionBits options, 
                                bool * didTunnelController);
    IOReturn restoreMachineState( IOOptionBits options, IOPCIDevice * device );
    void tunnelsWait(IOPCIDevice * device);

    IOReturn _restoreDeviceState( IOPCIDevice * device, IOOptionBits options );
    IOReturn _restoreDeviceDependents(IOPCIDevice * device, IOOptionBits options, IOPCIDevice * forDependent);

    IOReturn resolveInterrupts(IOPCIDevice * nub );
    IOReturn resolveLegacyInterrupts( IOService * provider, IOPCIDevice * nub );
    IOReturn resolveMSIInterrupts   ( IOService * provider, IOPCIDevice * nub );

    IOReturn relocate(IOPCIDevice * device, uint32_t options);
    void spaceFromProperties( IORegistryEntry * regEntry,
                              IOPCIAddressSpace * space );
    void updateWakeReason(IOPCIDevice * device);
    bool childPrefersMSIX( IOPCIDevice * device );

protected:
#if !defined(__arm64__)
    static SInt32 compareAddressCell( UInt32 cellCount, UInt32 cleft[], UInt32 cright[] );
#else
    static SInt64 compareAddressCell( UInt32 cellCount, UInt32 cleft[], UInt32 cright[] );
#endif
    IOReturn setDeviceASPMBits(IOPCIDevice * device, uint32_t bits);
    IOReturn setDeviceASPML1Bit(IOPCIDevice * device, uint32_t bits);
    IOReturn setDeviceL1PMBits(IOPCIDevice * device, uint32_t bits);
    IOReturn setDeviceCLKREQBits(IOPCIDevice * device, uint32_t bits);

    IOReturn setDevicePowerState(IOPCIDevice * device, IOOptionBits options, unsigned long prevState, unsigned long newState);
    IOReturn configOp(configOpParams *params);
    static void     deferredProbe(IOPCIDevice * device);

    void * __reserved1;
    void * __reserved2;

/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the IOPCIBridge in the future.
*/    
    struct ExpansionData
    {
        struct IOPCIRange * rangeLists[kIOPCIResourceTypeCount];
        IOPCIMessagedInterruptController *messagedInterruptController;
        IOPCIHostBridgeData *hostBridgeData;
        IOSimpleLock *lock; // Synchronizes access to 'started'
        bool started;
    };

/*! @var reserved
    Reserved for future use.  (Internal use only)  
*/
private:
    ExpansionData *reserved;

protected:
    IOWorkLoop * getConfiguratorWorkLoop(void) const;

public:
    static IOPCIEventSource * createEventSource(OSObject * owner, IOPCIEventSource::Action action, uint32_t options);

public:
    virtual void probeBus( IOService * provider, UInt8 busNum );

    virtual UInt8 firstBusNum( void );
    virtual UInt8 lastBusNum( void );

    virtual void spaceFromProperties( OSDictionary * propTable,
                                        IOPCIAddressSpace * space );
    virtual OSDictionary * constructProperties( IOPCIAddressSpace space );

    virtual IOPCIDevice * createNub( OSDictionary * from );

    virtual bool initializeNub( IOPCIDevice * nub, OSDictionary * from );

    virtual bool publishNub( IOPCIDevice * nub, UInt32 index );

    virtual bool addBridgeMemoryRange( IOPhysicalAddress start,
                                        IOPhysicalLength length, bool host );

    virtual bool addBridgeIORange( IOByteCount start, IOByteCount length );


private:
    virtual bool constructRange( IOPCIAddressSpace * flags,
                                 IOPhysicalAddress64 phys, IOPhysicalLength64 len,
                                 OSArray * array );

    virtual bool matchNubWithPropertyTable( IOService * nub,
                                            OSDictionary * propertyTable,
                                            SInt32 * score );

    virtual bool compareNubName( const IOService * nub, OSString * name,
                                 OSString ** matched = 0 ) const;

    virtual bool pciMatchNub( IOPCIDevice * nub,
                                OSDictionary * table, SInt32 * score);

    virtual bool matchKeys( IOPCIDevice * nub, const char * keys,
                                UInt32 defaultMask, UInt8 regNum );

    virtual IOReturn getNubResources( IOService * nub );

    virtual IOReturn getNubAddressing( IOPCIDevice * nub );

    virtual IOReturn getDTNubAddressing( IOPCIDevice * nub );

public:
    virtual void free( void );

    virtual bool start( IOService * provider );

    virtual void stop( IOService * provider );

    virtual bool configure( IOService * provider );

    virtual IOReturn setProperties(OSObject * properties);

    virtual IOReturn newUserClient(task_t owningTask, void * securityID,
                                   UInt32 type,  OSDictionary * properties,
                                   IOUserClient ** handler);

    virtual unsigned long maxCapabilityForDomainState ( IOPMPowerFlags domainState );
    virtual unsigned long initialPowerStateForDomainState ( IOPMPowerFlags domainState );
    virtual unsigned long powerStateForDomainState ( IOPMPowerFlags domainState );

    virtual IOReturn callPlatformFunction(const OSSymbol * functionName,
                                          bool waitForFunction,
                                          void * param1, void * param2,
                                          void * param3, void * param4);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    virtual IODeviceMemory * ioDeviceMemory( void ) = 0;

    virtual UInt32 configRead32( IOPCIAddressSpace space, UInt8 offset ) = 0;
    virtual void configWrite32( IOPCIAddressSpace space,
                                        UInt8 offset, UInt32 data ) = 0;
    virtual UInt16 configRead16( IOPCIAddressSpace space, UInt8 offset ) = 0;
    virtual void configWrite16( IOPCIAddressSpace space,
                                        UInt8 offset, UInt16 data ) = 0;
    virtual UInt8 configRead8( IOPCIAddressSpace space, UInt8 offset ) = 0;
    virtual void configWrite8( IOPCIAddressSpace space,
                                        UInt8 offset, UInt8 data ) = 0;

    virtual IOPCIAddressSpace getBridgeSpace( void ) = 0;

    virtual UInt32 findPCICapability( IOPCIAddressSpace space,
                                      UInt8 capabilityID, UInt8 * offset = 0 );

    virtual IOReturn setPowerState(unsigned long powerStateOrdinal,
                                    IOService * whatDevice) APPLE_KEXT_OVERRIDE;
    virtual IOReturn setDevicePowerState( IOPCIDevice * device,
                                          unsigned long whatToDo );
    virtual IOReturn saveDeviceState( IOPCIDevice * device,
                                      IOOptionBits options = 0 );
    virtual IOReturn restoreDeviceState( IOPCIDevice * device,
                                         IOOptionBits options = 0 );

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    virtual IOReturn createAGPSpace( IOAGPDevice * master,
                                     IOOptionBits options,
                                     IOPhysicalAddress * address, 
                                     IOPhysicalLength * length );

    virtual IOReturn destroyAGPSpace( IOAGPDevice * master );

    virtual IORangeAllocator * getAGPRangeAllocator( IOAGPDevice * master );

    virtual IOOptionBits getAGPStatus( IOAGPDevice * master,
                                       IOOptionBits options = 0 );
    virtual IOReturn resetAGPDevice( IOAGPDevice * master,
                                     IOOptionBits options = 0 );

    virtual IOReturn getAGPSpace( IOAGPDevice * master,
                                  IOPhysicalAddress * address, 
                                  IOPhysicalLength * length );

    virtual IOReturn commitAGPMemory( IOAGPDevice * master, 
                                      IOMemoryDescriptor * memory,
                                      IOByteCount agpOffset,
                                      IOOptionBits options );

    virtual IOReturn releaseAGPMemory(  IOAGPDevice * master, 
                                        IOMemoryDescriptor * memory, 
                                        IOByteCount agpOffset,
                                        IOOptionBits options );

protected:
    OSMetaClassDeclareReservedUsed(IOPCIBridge, 0);
private:
    virtual bool addBridgePrefetchableMemoryRange( IOPhysicalAddress start,
                                                   IOPhysicalLength length,
                                                   bool host );
protected:
    bool addBridgePrefetchableMemoryRange( addr64_t start, addr64_t length );
    IOReturn kernelRequestProbe(IOPCIDevice * device, uint32_t options);
    IOReturn protectDevice(IOPCIDevice * device, uint32_t space, uint32_t prot);

    OSMetaClassDeclareReservedUsed(IOPCIBridge, 1);
    virtual UInt32 extendedFindPCICapability( IOPCIAddressSpace space,
                                              UInt32 capabilityID, IOByteCount * offset = 0 );

    OSMetaClassDeclareReservedUsed(IOPCIBridge, 2);
    virtual IOReturn setDeviceASPMState(IOPCIDevice * device,
                                IOService * client, IOOptionBits state);

    OSMetaClassDeclareReservedUsed(IOPCIBridge, 3);
    virtual IOReturn checkLink(uint32_t options = 0);

    OSMetaClassDeclareReservedUsed(IOPCIBridge, 4);
    virtual IOReturn enableLTR(IOPCIDevice * device, bool enable);

    OSMetaClassDeclareReservedUsed(IOPCIBridge, 5);
    virtual IOPCIEventSource * createEventSource(IOPCIDevice * device,
                                             OSObject * owner, IOPCIEventSource::Action action, uint32_t options);

    OSMetaClassDeclareReservedUsed(IOPCIBridge,  6);
    virtual UInt32 extendedFindPCICapability(struct IOPCIConfigEntry * entry,
                                             UInt32 capabilityID,
                                             IOByteCount * offset = NULL);

public:
    virtual bool init( OSDictionary *  propTable );

    // Unused Padding
    OSMetaClassDeclareReservedUnused(IOPCIBridge,  7);
    OSMetaClassDeclareReservedUnused(IOPCIBridge,  8);
    OSMetaClassDeclareReservedUnused(IOPCIBridge,  9);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 10);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 11);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 12);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 13);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 14);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 15);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 16);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 17);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 18);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 19);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 20);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 21);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 22);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 23);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 24);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 25);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 26);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 27);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 28);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 29);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 30);
    OSMetaClassDeclareReservedUnused(IOPCIBridge, 31);

#if TARGET_CPU_ARM || TARGET_CPU_ARM64
protected:

    virtual IOReturn deviceMemoryRead(IOMemoryDescriptor* sourceBase,
                                     IOByteCount         sourceOffset,
                                     IOMemoryDescriptor* destinationBase,
                                     IOByteCount         destinationOffset,
                                     IOByteCount         size);

    virtual IOReturn deviceMemoryRead(IOMemoryDescriptor* sourceBase,
                                      IOByteCount         sourceOffset,
                                      void*               destination,
                                      IOByteCount         size);

#endif
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class __kpi_deprecated("Use PCIDriverKit") IOPCI2PCIBridge : public IOPCIBridge
{
    friend class IOPCIEventSource;
    friend class IOPCIDevice;

    OSDeclareDefaultStructors(IOPCI2PCIBridge)

protected:
    IOInterruptEventSource * fBridgeInterruptSource;

private:
    IOPCIDevice *                  fBridgeDevice;
    IOTimerEventSource *           fTimerProbeES;
    IOWorkLoop *                   fWorkLoop;
    IOPMDriverAssertionID          fPMAssertion;
    IOSimpleLock *                 fISRLock;
    struct IOPCIAERRoot *          fAERRoot;
    uint32_t                       __resvA[6];
    int32_t                        fTunnelL1EnableCount;
    uint32_t                       fHotplugCount;

    uint8_t                        _resvA[3];
    uint8_t                        fHotPlugInts;
    uint8_t                        fIntsPending;
    uint8_t                        fIsAERRoot;

    uint8_t                        fPresence;
    uint8_t                        fWaitingLinkEnable;
    uint8_t                        fLinkChangeOnly;
    uint8_t                        fBridgeInterruptEnablePending;
    uint8_t                        fNeedProbe;
    uint8_t                        fPresenceInt;
    uint8_t                        fBridgeMSI;
    uint8_t                        fNoDevice;
    uint8_t                        fLinkControlWithPM;
    uint8_t                        fPowerState;
    char                           fLogName[32];
;

/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData {};

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

public:

    virtual UInt8 firstBusNum( void ) override;
    virtual UInt8 lastBusNum( void ) override;

public:
    virtual void free() override;

    virtual bool serializeProperties( OSSerialize * serialize ) const override;

    virtual IOService * probe(  IOService *     provider,
                                SInt32 *        score ) override;

    virtual bool start( IOService * provider ) override;

    virtual void stop( IOService * provider ) override;

    virtual bool configure( IOService * provider ) override;

    virtual void probeBus( IOService * provider, UInt8 busNum ) override;

    virtual IOReturn requestProbe( IOOptionBits options ) override;

    virtual void systemWillShutdown(IOOptionBits specifier) override;

    virtual void saveBridgeState( void );

    virtual void restoreBridgeState( void );

    IOReturn setPowerState( unsigned long powerState,
                            IOService * whatDevice ) override;

    void adjustPowerState(unsigned long state);

    virtual IOReturn saveDeviceState( IOPCIDevice * device,
                                      IOOptionBits options = 0 ) override;

    virtual bool publishNub( IOPCIDevice * nub, UInt32 index ) override;

    virtual IODeviceMemory * ioDeviceMemory( void ) override;

    virtual IOPCIAddressSpace getBridgeSpace( void ) override;

    virtual UInt32 configRead32( IOPCIAddressSpace space, UInt8 offset ) override;
    virtual void configWrite32( IOPCIAddressSpace space,
                                        UInt8 offset, UInt32 data ) override;
    virtual UInt16 configRead16( IOPCIAddressSpace space, UInt8 offset ) override;
    virtual void configWrite16( IOPCIAddressSpace space,
                                        UInt8 offset, UInt16 data ) override;
    virtual UInt8 configRead8( IOPCIAddressSpace space, UInt8 offset ) override;
    virtual void configWrite8( IOPCIAddressSpace space,
                                        UInt8 offset, UInt8 data ) override;

    virtual IOReturn setDeviceASPMState(IOPCIDevice * device,
                                IOService * client, IOOptionBits state) override;

    virtual IOReturn checkLink(uint32_t options = 0) override;

    virtual IOReturn enableLTR(IOPCIDevice * device, bool enable) override;

    virtual IOPCIEventSource * createEventSource(IOPCIDevice * device,
                                OSObject * owner, IOPCIEventSource::Action action, uint32_t options) override;

    // Unused Padding
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  0);
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  1);
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  2);
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  3);
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  4);
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  5);
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  6);
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  7);
    OSMetaClassDeclareReservedUnused(IOPCI2PCIBridge,  8);

protected:
    void allocateBridgeInterrupts(IOService * provider);
    void startBridgeInterrupts(IOService * provider);
    void enableBridgeInterrupts(void);
    void disableBridgeInterrupts(void);

private:
    IOReturn setTunnelL1Enable(IOPCIDevice * device, IOService * client, bool l1Enable);

public:
    void startBootDefer(IOService * provider);

    void handleInterrupt( IOInterruptEventSource * source,
                             int                      count );
    void timerProbe(IOTimerEventSource * es);
};
__exported_pop

#define kIOPCI2PCIBridgeName	"IOPP"

#endif /* ! _IOKIT_IOPCIBRIDGE_H */
