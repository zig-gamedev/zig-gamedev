/*
 * Copyright (c) 1998-2000 Apple Computer, Inc. All rights reserved.
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

#ifndef _IOKIT_IODISPLAY_H
#define _IOKIT_IODISPLAY_H

#include <sys/kdebug.h>
#include <sys/proc.h>

#include <IOKit/IOService.h>

#include <IOKit/graphics/IOFramebuffer.h>

extern const OSSymbol * gIODisplayParametersKey;
extern const OSSymbol * gIODisplayGUIDKey;

extern const OSSymbol * gIODisplayValueKey;
extern const OSSymbol * gIODisplayMinValueKey;
extern const OSSymbol * gIODisplayMaxValueKey;

extern const OSSymbol * gIODisplayBrightnessProbeKey;
extern const OSSymbol * gIODisplayLinearBrightnessProbeKey;
extern const OSSymbol * gIODisplayContrastKey;
extern const OSSymbol * gIODisplayBrightnessKey;
extern const OSSymbol * gIODisplayLinearBrightnessKey;
extern const OSSymbol * gIODisplayUsableLinearBrightnessKey;
extern const OSSymbol * gIODisplayBrightnessFadeKey;
extern const OSSymbol * gIODisplayHorizontalPositionKey;
extern const OSSymbol * gIODisplayHorizontalSizeKey;
extern const OSSymbol * gIODisplayVerticalPositionKey;
extern const OSSymbol * gIODisplayVerticalSizeKey;
extern const OSSymbol * gIODisplayTrapezoidKey;
extern const OSSymbol * gIODisplayPincushionKey;
extern const OSSymbol * gIODisplayParallelogramKey;
extern const OSSymbol * gIODisplayRotationKey;
extern const OSSymbol * gIODisplayOverscanKey;
extern const OSSymbol * gIODisplayVideoBestKey;
extern const OSSymbol * gIODisplaySelectedColorModeKey;

extern const OSSymbol * gIODisplayRedGammaScaleKey;
extern const OSSymbol * gIODisplayGreenGammaScaleKey;
extern const OSSymbol * gIODisplayBlueGammaScaleKey;
extern const OSSymbol * gIODisplayGammaScaleKey;

extern const OSSymbol * gIODisplayParametersTheatreModeKey;
extern const OSSymbol * gIODisplayParametersTheatreModeWindowKey;

extern const OSSymbol * gIODisplayMCCSVersionKey;
extern const OSSymbol * gIODisplayTechnologyTypeKey;
extern const OSSymbol * gIODisplayUsageTimeKey;
extern const OSSymbol * gIODisplayFirmwareLevelKey;

extern const OSSymbol * gIODisplaySpeakerVolumeKey;
extern const OSSymbol * gIODisplaySpeakerSelectKey;
extern const OSSymbol * gIODisplayMicrophoneVolumeKey;
extern const OSSymbol * gIODisplayAmbientLightSensorKey;
extern const OSSymbol * gIODisplayAudioMuteAndScreenBlankKey;
extern const OSSymbol * gIODisplayAudioTrebleKey;
extern const OSSymbol * gIODisplayAudioBassKey;
extern const OSSymbol * gIODisplayAudioBalanceLRKey;
extern const OSSymbol * gIODisplayAudioProcessorModeKey;
extern const OSSymbol * gIODisplayPowerModeKey;
extern const OSSymbol * gIODisplayManufacturerSpecificKey;

extern const OSSymbol * gIODisplayPowerStateKey;
extern const OSSymbol * gIODisplayControllerIDKey;
extern const OSSymbol * gIODisplayCapabilityStringKey;

extern const OSSymbol * gIODisplayParametersCommitKey;
extern const OSSymbol * gIODisplayParametersDefaultKey;
extern const OSSymbol * gIODisplayParametersFlushKey;

extern const OSSymbol * gIODisplayFadeTime1Key;
extern const OSSymbol * gIODisplayFadeTime2Key;
extern const OSSymbol * gIODisplayFadeTime3Key;
extern const OSSymbol * gIODisplayFadeStyleKey;

extern UInt32 gIODisplayFadeTime1;
extern UInt32 gIODisplayFadeTime2;
extern UInt32 gIODisplayFadeTime3;
extern UInt32 gIODisplayFadeStyle;

enum {
    kIODisplayNumPowerStates = 4,
    kIODisplayMaxPowerState  = kIODisplayNumPowerStates - 1
};

class IODisplayWrangler;
class IODisplayConnect : public IOService
{
    OSDeclareDefaultStructors(IODisplayConnect)
    friend class IODisplayWrangler;

private:
    IOIndex  fConnectIndex;
    uint64_t fFBRegID;

protected:
/*  Reserved for future use.  (Internal use only)  */
    struct ExpansionData { };

/*  Reserved for future use.  (Internal use only)  */
    ExpansionData * reserved;

    // Support for IODisplayWrangler
    virtual bool initWithConnection(const uint64_t fbRegID,
                                    const IOIndex connection);

public:
    // IODisplayConnect interface
    virtual IOFramebuffer * getFramebuffer( void );
    virtual IOIndex getConnection( void );
    virtual IOReturn getAttributeForConnection( IOSelect selector, uintptr_t * value );
    virtual IOReturn setAttributeForConnection( IOSelect selector, uintptr_t value );
    virtual void joinPMtree ( IOService * driver ) APPLE_KEXT_OVERRIDE;

#ifndef _OPEN_SOURCE_
    // Support for IODisplay
    static void recordGTraceToken(
            const uint16_t line,
            const uint16_t tag0, const uint64_t fbRegID,
            const uint16_t tag1, const uint64_t arg1,
            const uint16_t tag2, const uint64_t arg2,
            const uint16_t tag3, const uint64_t arg3);
    void recordGTraceToken(const uint16_t line,
                           const uint16_t fnID, const uint8_t fnType,
                           const uint16_t tag1, const uint64_t arg1,
                           const uint16_t tag2, const uint64_t arg2,
                           const uint16_t tag3, const uint64_t arg3);
    inline uint64_t getFBRegistryID() { return fFBRegID; }
#endif // !_OPEN_SOURCE_
};

class IODisplay : public IOService
{
    OSDeclareAbstractStructors(IODisplay)

public:
    static void initialize( void );

protected:
    // used to query the framebuffer controller
    IODisplayConnect *                  fConnection;
    class IODisplayParameterHandler *   fParameterHandler;
    void *                      __resv;
    IONotifier *                        fNotifier;

    // pointer to protected instance variables for power management
    struct IODisplayPMVars *              fDisplayPMVars;

public:
    uintptr_t                           fWSAADeferState;
protected:
    // reserved for future expansion
    void *                              _IODisplay_reserved[31];

public:
    virtual IOService * probe(  IOService *     provider,
                                SInt32 *        score ) APPLE_KEXT_OVERRIDE;

    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;

    virtual IODisplayConnect * getConnection( void );

    virtual IOReturn getConnectFlagsForDisplayMode(
                IODisplayModeID mode, UInt32 * flags );

    virtual IOReturn getGammaTableByIndex(
        UInt32 * channelCount, UInt32 * dataCount,
        UInt32 * dataWidth, void ** data );

    virtual IOReturn readFramebufferEDID( void );

    virtual IOReturn framebufferEvent( IOFramebuffer * framebuffer, 
                                        IOIndex event, void * info ); 
    // parameter setting
    virtual bool setProperty(const OSSymbol* aKey, OSObject* anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const OSString* aKey, OSObject* anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char* aKey, OSObject* anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char* aKey, const char* aString) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char* aKey, bool aBoolean) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char* aKey, unsigned long long aValue,
            unsigned int aNumberOfBits) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char* aKey, void *bytes, unsigned int length) APPLE_KEXT_OVERRIDE;

    virtual IOReturn setProperties( OSObject * properties ) APPLE_KEXT_OVERRIDE;
    virtual bool setForKey( OSDictionary * params, const OSSymbol * key,
                            SInt32 value, SInt32 min, SInt32 max );

    static bool addParameter( OSDictionary * params, const OSSymbol * paramName, SInt32 min, SInt32 max );
    static bool setParameter( OSDictionary * params, const OSSymbol * paramName, SInt32 value );
    static OSDictionary * getIntegerRange( OSDictionary * params, const OSSymbol * sym,
                                      SInt32 * value, SInt32 * min, SInt32 * max );
    
    void loadPrefs( uint64_t edidHash, uint32_t vendor, uint32_t product, uint32_t serial );

    // low level set/get
    virtual bool doIntegerSet( OSDictionary * params,
                               const OSSymbol * paramName, UInt32 value );
    virtual bool doDataSet( const OSSymbol * paramName, OSData * value );
    virtual bool doUpdate( void );

    // power management methods
    virtual IOReturn setPowerState( unsigned long, IOService * ) APPLE_KEXT_OVERRIDE;
    virtual unsigned long maxCapabilityForDomainState( IOPMPowerFlags ) APPLE_KEXT_OVERRIDE;
    virtual unsigned long initialPowerStateForDomainState( IOPMPowerFlags ) APPLE_KEXT_OVERRIDE;
    virtual unsigned long powerStateForDomainState( IOPMPowerFlags ) APPLE_KEXT_OVERRIDE;

    // 
    virtual void initPowerManagement( IOService * provider);
    virtual void dropOneLevel( void );
    virtual void makeDisplayUsable( void );
    void setDisplayPowerState(unsigned long state);

#ifndef _OPEN_SOURCE_
// GTrace V2 support for IOBacklightDisplay tracing
#define HAS_IBDGTRACETOKEN 1
    void recordGTraceToken(const uint16_t line,
                           const uint16_t fnID, const uint8_t fnType,
                           const uint16_t tag1, const uint64_t arg1,
                           const uint16_t tag2, const uint64_t arg2,
                           const uint16_t tag3, const uint64_t arg3);
#endif // !_OPEN_SOURCE_

private:
    OSMetaClassDeclareReservedUnused(IODisplay, 0);
    OSMetaClassDeclareReservedUnused(IODisplay, 1);
    OSMetaClassDeclareReservedUnused(IODisplay, 2);
    OSMetaClassDeclareReservedUnused(IODisplay, 3);
    OSMetaClassDeclareReservedUnused(IODisplay, 4);
    OSMetaClassDeclareReservedUnused(IODisplay, 5);
    OSMetaClassDeclareReservedUnused(IODisplay, 6);
    OSMetaClassDeclareReservedUnused(IODisplay, 7);
    OSMetaClassDeclareReservedUnused(IODisplay, 8);
    OSMetaClassDeclareReservedUnused(IODisplay, 9);
    OSMetaClassDeclareReservedUnused(IODisplay, 10);
    OSMetaClassDeclareReservedUnused(IODisplay, 11);
    OSMetaClassDeclareReservedUnused(IODisplay, 12);
    OSMetaClassDeclareReservedUnused(IODisplay, 13);
    OSMetaClassDeclareReservedUnused(IODisplay, 14);
    OSMetaClassDeclareReservedUnused(IODisplay, 15);
    OSMetaClassDeclareReservedUnused(IODisplay, 16);
    OSMetaClassDeclareReservedUnused(IODisplay, 17);
    OSMetaClassDeclareReservedUnused(IODisplay, 18);
    OSMetaClassDeclareReservedUnused(IODisplay, 19);

private:
    static IOReturn _framebufferEvent( OSObject * osobj, void * ref,
                    IOFramebuffer *framebuffer, IOIndex event, void * info );

    void searchParameterHandlers(IORegistryEntry * entry);
    bool addParameterHandler( IODisplayParameterHandler * parameterHandler );
    bool removeParameterHandler( IODisplayParameterHandler * parameterHandler );
    static bool updateNumber( OSDictionary * params, const OSSymbol * key, SInt32 value );
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class IOBacklightDisplay : public IODisplay
{
    OSDeclareDefaultStructors(IOBacklightDisplay)

    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 0);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 1);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 2);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 3);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 4);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 5);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 6);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 7);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 8);
    OSMetaClassDeclareReservedUnused(IOBacklightDisplay, 9);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class IODisplayParameterHandler : public IOService
{
    OSDeclareDefaultStructors(IODisplayParameterHandler)

public:
    virtual bool setDisplay( IODisplay * display ) = 0;
    virtual bool doIntegerSet( OSDictionary * params,
                               const OSSymbol * paramName, UInt32 value ) = 0;
    virtual bool doDataSet( const OSSymbol * paramName, OSData * value ) = 0;
    virtual bool doUpdate( void ) = 0;

private:
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 0);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 1);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 2);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 3);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 4);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 5);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 6);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 7);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 8);
    OSMetaClassDeclareReservedUnused(IODisplayParameterHandler, 9);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif /* ! _IOKIT_IODISPLAY_H */

