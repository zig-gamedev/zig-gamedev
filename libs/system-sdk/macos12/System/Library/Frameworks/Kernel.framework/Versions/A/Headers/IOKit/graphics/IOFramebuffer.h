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

#ifndef _IOKIT_IOFRAMEBUFFER_H
#define _IOKIT_IOFRAMEBUFFER_H

#include <IOKit/IOService.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#include <IOKit/IOInterruptEventSource.h>
#pragma clang diagnostic pop
#include <IOKit/IOLib.h>

#include <IOKit/graphics/IOGraphicsDevice.h>
#include <IOKit/graphics/IOFramebufferShared.h>


#define IOFRAMEBUFFER_REV           8


class IOFramebuffer;
class IOBufferMemoryDescriptor;
struct IOFBController;


typedef void (*CursorBlitProc)(
                    IOFramebuffer * inst,
                    void * shmem,
                    volatile unsigned char *vramPtr,
                    unsigned int cursStart,
                    unsigned int vramRow,
                    unsigned int cursRow,
                    int width,
                    int height );

typedef void (*CursorRemoveProc)(
                    IOFramebuffer * inst,
                    void * shmem,
                    volatile unsigned char *vramPtr,
                    unsigned int vramRow,
                    int width,
                    int height );


typedef void * IOFBCursorRef;

struct IOFBCursorControlCallouts {
    IOReturn    (*setCursorImage) (void * target, void * ref,
                                    IOHardwareCursorDescriptor * description, IOFBCursorRef cursorImage);
    IOReturn    (*setCursorState) (void * target, void * ref,
                                    SInt32 x, SInt32 y, bool visible);
    UInt32      reserved[30];
};
typedef struct IOFBCursorControlCallouts IOFBCursorControlCallouts;

struct IOFBCursorControlAttribute {
    void *                              inst;
    void *                              ref;
    const IOFBCursorControlCallouts *   callouts;
    UInt32                              reserved[29];
};
typedef struct IOFBCursorControlAttribute IOFBCursorControlAttribute;

// clock & data values
enum {
    kIODDCLow                           = 0,
    kIODDCHigh                          = 1,
    kIODDCTristate                      = 2
};
// ddcBlockType constants
enum {
    // EDID block type.
    kIODDCBlockTypeEDID                 = 0
};

// ddcFlags constants
enum {
    // Force a new read of the EDID.
    kIODDCForceRead                     = 0x00000001
};

enum {
    kDisabledInterruptState             = 0,
    kEnabledInterruptState              = 1
};

typedef void (*IOFBInterruptProc)( OSObject * target, void * ref );


typedef IOReturn (*IOFramebufferNotificationHandler)
        (OSObject * obj, void * ref,
        IOFramebuffer * framebuffer, IOIndex event,
        void * info);

// IOFramebufferNotificationHandler events
enum {
    kIOFBNotifyDisplayModeWillChange    = 1,
    kIOFBNotifyDisplayModeDidChange     = 2,

    kIOFBNotifyWillSleep        = 3,
    kIOFBNotifyDidWake          = 4,

    kIOFBNotifyDidPowerOff      = 5,
    kIOFBNotifyWillPowerOn      = 6,

    kIOFBNotifyDidSleep         = kIOFBNotifyDidPowerOff,
    kIOFBNotifyWillWake         = kIOFBNotifyWillPowerOn,

    kIOFBNotifyWillPowerOff     = 7,
    kIOFBNotifyDidPowerOn       = 8,

    kIOFBNotifyWillChangeSpeed  = 9,
    kIOFBNotifyDidChangeSpeed   = 10,

    kIOFBNotifyHDACodecWillPowerOn  = 11,   // since IOGRAPHICSTYPES_REV 68
    kIOFBNotifyHDACodecDidPowerOn   = 12,   // since IOGRAPHICSTYPES_REV 68

    kIOFBNotifyHDACodecWillPowerOff = 13,   // since IOGRAPHICSTYPES_REV 68
    kIOFBNotifyHDACodecDidPowerOff  = 14,   // since IOGRAPHICSTYPES_REV 68

    kIOFBNotifyClamshellChange  = 20,

    kIOFBNotifyCaptureChange    = 30,

    kIOFBNotifyOnlineChange     = 40,

    kIOFBNotifyDisplayDimsChange = 50,

    kIOFBNotifyProbed           = 60,

    kIOFBNotifyVRAMReady        = 70,

    kIOFBNotifyWillNotify       = 80,
    kIOFBNotifyDidNotify        = 81,

    // <rdar://problem/32063590> IOGraphics needs to send WSAAWillExitDefer and WSAADidExitDefer instead of single message
    kIOFBNotifyWSAAWillEnterDefer   = 90,   // since IOGRAPHICSTYPES_REV 53
    kIOFBNotifyWSAAWillExitDefer    = 91,   // since IOGRAPHICSTYPES_REV 53

    kIOFBNotifyWSAADidEnterDefer    = 92,   // since IOGRAPHICSTYPES_REV 53
    kIOFBNotifyWSAADidExitDefer     = 93,   // since IOGRAPHICSTYPES_REV 53

    kIOFBNotifyWSAAEnterDefer       = kIOFBNotifyWSAAWillEnterDefer,
    kIOFBNotifyWSAAExitDefer        = kIOFBNotifyWSAAWillExitDefer,

    kIOFBNotifyTerminated       = 100, // since IOGRAPHICSTYPES_REV 49
};

/*
 Discussion: addFramebufferNotificationWithOptions

 Clients that want to take advantage of the fine grain IOFramebuffer event notifications must register their framebuffer notification handler with addFramebufferNotificationWithOptions and provide a single kIOFBNotifyGroup ID, one or more kIOFBNotifyEvents and optionally a desired kIOFBNotifyPriority value.  If no priority is required, clients must set groupPriority to 0.  Clients must not register with any predefined kIOFBNotifyGroup that they do not belong to.  kIOFBNotifyGroup values within the range of 0x8000 to 0xFFFF are available for general and third party software use.  Where multiple independent software components end up using the same kIOFBNotifyGroup identification numbers, the callout order is determined first by priority and then by registration order.  Clients that request a priority that is outside of the range specified by kIOFBNotifyPriority_Min/kIOFBNotifyPriority_Max will be clamped to the range limits.

 Handler invocation by IOGraphics:

 Clients are only called for the kIOFBNotifyEvents that they requested at time of registration.
 Multiple clients with the same kIOFBNotifyGroup are called in the order based on the requested priority.
 Multiple clients with the same kIOFBNotifyGroup and the same priority are called in a "first registered, first served" order.
 IOGraphics will invoke the handlers based on group, then priority, the registration order.  The ordering of the group callouts is private and will change as required/desired by Apple.  Clients must NOT rely on group callout order to determine system or intercomponent behavior.  If there is a strong need to be called before a specific component, please file a radar with the Component of "IOGraphics" and the Version of "all".

 Discussion: addFramebufferNotification
 Legacy and existing software that relies upon addFramebufferNotification() will behave as if they had called with the following options: addFramebufferNotificationWithOptions(..., kIOFBNotifyGroupID_Legacy, 0, kIOFBNotifyEvent_All);
 Legacy clients are called after kIOFBNotifyGroup IDs within the range of 0x0 to 0x7FFF have been called, but before IDs in the range of 0x8000 to 0xFFFF.
 */

// addFramebufferNotificationWithOptions event types
enum {
    kIOFBNotifyEvent_None                   = (0ULL << 0),
    kIOFBNotifyEvent_DisplayModeChange      = (1ULL << 0),
    kIOFBNotifyEvent_SleepWake              = (1ULL << 1),  // Only for events: kIOFBNotifyWillSleep & kIOFBNotifyDidWake
    kIOFBNotifyEvent_PowerOnOff             = (1ULL << 2),
    kIOFBNotifyEvent_ChangeSpeed            = (1ULL << 3),
    kIOFBNotifyEvent_ClamshellChange        = (1ULL << 4),
    kIOFBNotifyEvent_CaptureChange          = (1ULL << 5),
    kIOFBNotifyEvent_OnlineChange           = (1ULL << 6),
    kIOFBNotifyEvent_DisplayDimsChange      = (1ULL << 7),
    kIOFBNotifyEvent_Probed                 = (1ULL << 8),
    kIOFBNotifyEvent_VRAMReady              = (1ULL << 9),
    kIOFBNotifyEvent_Notify                 = (1ULL << 10), // kIOFBNotifyWillNotify & kIOFBNotifyDidNotify
    kIOFBNotifyEvent_WSAADefer              = (1ULL << 11),
    kIOFBNotifyEvent_Terminated             = (1ULL << 12),
    kIOFBNotifyEvent_HDACodecPowerOnOff     = (1ULL << 13), // since IOFRAMEBUFFER_REV 7

    kIOFBNotifyEvent_Last                   = (1ULL << 14),
    kIOFBNotifyEvent_All                    = (kIOFBNotifyEvent_Last - 1)
};

enum {
    // 0x0 - 0xFF - Private: Reserved for IOGraphics.
    kIOFBNotifyGroupID_Legacy                         = 0x001,
    kIOFBNotifyGroupID_IODisplay                      = 0x010,

    // 0x100 - 0x1FF - Reserved for Apple kernel extensions
    kIOFBNotifyGroupID_AppleGraphicsDevicePolicy      = 0x100,
    kIOFBNotifyGroupID_AppleGraphicsMGPUPowerControl  = 0x110,
    kIOFBNotifyGroupID_AppleGraphicsMUXControl        = 0x120,
    kIOFBNotifyGroupID_AppleGraphicsControl           = 0x130,
    kIOFBNotifyGroupID_AppleGraphicsDisplayPolicy     = 0x140,
    kIOFBNotifyGroupID_AppleGraphicsPowerManagement   = 0x150,
    kIOFBNotifyGroupID_AppleHDAController             = 0x160,
    kIOFBNotifyGroupID_AppleIOAccelDisplayPipe        = 0x170,
    kIOFBNotifyGroupID_AppleMCCSControl               = 0x180,

    // 0x200 - 0x2FF - Reserved for vendor kernel extensions
    kIOFBNotifyGroupID_VendorIntel                    = 0x200,

    // 0x300 - 0x3FF - Reserved for vendor kernel extensions
    kIOFBNotifyGroupID_VendorNVIDIA                   = 0x300,

    // 0x400 - 0x4FF - Reserved for vendor kernel extensions
    kIOFBNotifyGroupID_VendorAMD                      = 0x400,

    // 0x500 - 0x7FFF - Reserved for future use

    // 0x8000+ - Reserved for third party software
    kIOFBNotifyGroupID_ThirdParty                     = 0x8000,

    kIOFBNotifyGroupID_Count                          = 15
};

// addFramebufferNotificationWithOptions group priority range
enum {
    kIOFBNotifyPriority_Min = -32768,
    kIOFBNotifyPriority_Max = 32767,
};

struct IOFramebufferNotificationNotify
{
	IOIndex event;
	void *  info;
};

enum {
    kFBDisplayUsablePowerState          = 0x80000000,
    kFBDisplayPowerStateMask            = 0x0000ffff
};

#define kIOFBDependentIDKey     "IOFBDependentID"
#define kIOFBDependentIndexKey  "IOFBDependentIndex"

#ifndef _OPEN_SOURCE_
// GTrace V2 support for AGDC markers
#define HAS_AGDCGTRACETOKEN 1
extern void agdcGTraceToken(
        const IOFramebuffer* fb, const uint16_t line, const bool useController,
        const uint16_t fnID, const uint8_t fnType,
        const uint16_t tag1, const uint64_t arg1,
        const uint16_t tag2, const uint64_t arg2,
        const uint16_t tag3, const uint64_t arg3);
#endif // !_OPEN_SOURCE_

struct StdFBShmem_t;
class IOFramebufferUserClient;
class IODisplay;

/*! @class IOFramebuffer : public IOGraphicsDevice
    @abstract The base class for graphics devices to be made available as part of the desktop.
    @discussion The IOFramebuffer base class defines APIs used to publish a linear framebuffer device. Device driver writers should subclass this class to provide a X native driver. Mac OS X will also utilize 'ndrv' drivers via a subclass of IOFramebuffer IONDRVFramebuffer that does not require device driver writers to provide a X native driver.
    
    There are no in kernel clients of IOFramebuffer aside from rudimentary console and panic UI supported by the IOFramebuffer class. The IOFramebuffer class provides the IOUserClient implementation to allow the CoreGraphics server to provide the user accessible interface to all displays on a Mac OS X system, and this is further layered underneath application frameworks. Device driver writers should not need any knowledge of this part of the interfaces. Similarly the instance variables of IOFramebuffer are mostly used for cursor rendering which is handled by the IOFramebuffer class, and should be avoided by subclass implementors. Only IOFramebuffer methods with header documentation in this header are designed for subclasses to implement.

    IOFramebuffer provides simple dumb framebuffer operation - accceleration for 2D, 3D and video may be provided by a separate implementation of the IOAccelerator class.
*/

class IOFramebuffer : public IOGraphicsDevice
{
    friend class IOFramebufferUserClient;
    friend class IOFramebufferSharedUserClient;
    friend class IOFramebufferDiagnosticUserClient;
    friend class IOFramebufferParameterHandler;
    friend class IODisplay;
#ifndef _OPEN_SOURCE_
    friend void agdcGTraceToken(
            const IOFramebuffer*, const uint16_t, const bool, const uint16_t,
            const uint8_t, const uint16_t, const uint64_t, const uint16_t,
            const uint64_t, const uint16_t, const uint64_t);
#endif // !_OPEN_SOURCE_

    OSDeclareDefaultStructors(IOFramebuffer);

protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of this class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData * reserved;

private:

protected:
    StdFBShmem_t *                      priv;
    int                                 shmemClientVersion;
    IOBufferMemoryDescriptor *          sharedCursor;

    union {
        struct {
            /* Mapping tables used in cursor drawing to 5-5-5 displays. */
            unsigned char *     _bm34To35SampleTable;
            unsigned char *     _bm35To34SampleTable;
            /* Mapping tables used in cursor drawing to 8-bit RGB displays. */
            unsigned int *      _bm256To38SampleTable;
            unsigned char *     _bm38To256SampleTable;
        }                               t;
        UInt8 *                         tables[ 4 ];
    }                                   colorConvert;
    
    /* cursor blitting vars */
    CursorBlitProc                      cursorBlitProc;
    CursorRemoveProc                    cursorRemoveProc;

    IOGSize                             maxCursorSize;
    void *                              _IOFramebuffer_reservedE[6];
    uint32_t                            _IOFramebuffer_reservedF;
    uint32_t                            thisNameLen;
    const char *                        thisName;
    volatile unsigned char *            cursorSave;
    unsigned int                        white;

    IOGPoint                            nextCursorLoc;
    int                                 nextCursorFrame;
    SInt32                              connectChange;
    semaphore_t                         vblSemaphore;

    /* memory ranges */
    volatile unsigned char *            fFrameBuffer;
    unsigned int                        fTotalWidth;
    unsigned int                        rowBytes;
    unsigned int                        bytesPerPixel;

    IOMemoryMap *                       fVramMap;
    IOByteCount                         fVramMapOffset;
    OSArray *                           userAccessRanges;
    unsigned int                        suspended:1;
    unsigned int                        captured:1;
    unsigned int                        sleepConnectCheck:1;
    unsigned int                        messaged:1;
    unsigned int                        cursorEnable:1;
    unsigned int                        _IOFramebuffer_reservedC:27;
    IOFramebuffer *                     nextDependent;
    OSArray *                           fFBNotifications;

    class IOFramebufferUserClient *       fServerConnect;
    class IOFramebufferSharedUserClient * fSharedConnect;

    unsigned int                        opened:1;
    unsigned int                        dead:1;
    unsigned int                        configPending:1;
    unsigned int                        doNotUseServerNotified:1;  // Deprecated
    unsigned int                        doNotUseServerState:1;     // Deprecated
    unsigned int                        doNotUseServerPendingAck:1;// Deprecated
    unsigned int                        isUsable:1;
    unsigned int                        mirrored:1;
    unsigned int                        pendingPowerState:4;
    unsigned int                        pendingPowerChange:1;
    unsigned int                        pagingState:1;
    unsigned int                        mirrorPrimary:1;
    unsigned int                        mirrorSWCursor:1;

    bool                                clutValid;
    bool                                currentMono;
    bool                                needCursorService;
    bool                                haveVBLService;
    bool                                haveHWCursor;
    bool                                hwCursorLoaded;

    void *                              serverMsg;
    IOInterruptEventSource *            deferredEvents;

    /* Reserved for future expansion. */
    int                                 _IOFramebuffer_reserved[5];

private:
    struct IOFramebufferPrivate *       __private;

public:
/*! @function doI2CRequest
    @abstract Carry out an I2C request.
    @discussion IOFramebuffer subclasses may optionally implement this method to perform I2C bus requests on one of the buses they support. Alternatively they may implement the setDDCClock(), setDDCData(), readDDCClock(), readDDCData() methods and respond from getAttributeForConnection() to the kConnectionSupportsLLDDCSense attribute with success, in which case IOFramebuffer::doI2CRequest() will carry out a software implementation of I2C using the low level routines and conforming to the timing constraints passed in the timing parameter. Subclasses may pass timing parameters tuned for the specific bus, otherwise VESA DDC defaults will apply.
    @timing event Subclasses may pass timing parameters tuned for the specific bus, otherwise if NULL, VESA DDC defaults will apply.
    @param request An IOI2CRequest structure. The request should be carried out synchronously if the completion routine is NULL, otherwise it may optionally be carried out asynchronously. The completion routine should be called if supplied.
    @result an IOReturn code. If kIOReturnSuccess, the result of the transaction is returned in the requests result field.
*/
    virtual IOReturn doI2CRequest( UInt32 bus, struct IOI2CBusTiming * timing, struct IOI2CRequest * request );
    OSMetaClassDeclareReservedUsed(IOFramebuffer, 0);

    /*! @function diagnoseReport
     @abstract Provide private diagnostic information.
     @discussion Allow vendor implementations to provide diagnostic information when a catastrophic failure has been encounterd.
     */
    virtual IOReturn diagnoseReport( void * param1, void * param2, void * param3, void * param4 );
    OSMetaClassDeclareReservedUsed(IOFramebuffer, 1);

/*! @function setGammaTable
    @abstract Set the gamma table to be used by the framebuffer.
    @discussion IOFramebuffer subclasses should implement this method to allow a gamma table to be set.
    @param channelCount Defines the number of channels in the supplied data. OS X will pass three for separate R, G, B data, or one if the same data should apply to all channels.
    @param dataCount The number of data entries per channel.
    @param dataWidth The number of bits in each entry. 8 for Mac OS X 10.1 and earlier, 16 for later releases.
    @param data The packed array of correction data. Data is passed for the R (or single) channel followed by the G & B channels. Each entry is one or two bytes (if dataWidth > 8).
    @param syncToVBL 0 don't sync to VBL, else sync.
    @result an IOReturn code.
 */
    virtual IOReturn setGammaTable( UInt32 channelCount, UInt32 dataCount,
                                   UInt32 dataWidth, void * data, bool syncToVBL );
    OSMetaClassDeclareReservedUsed(IOFramebuffer, 2);

private:

    OSMetaClassDeclareReservedUnused(IOFramebuffer, 3);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 4);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 5);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 6);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 7);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 8);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 9);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 10);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 11);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 12);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 13);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 14);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 15);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 16);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 17);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 18);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 19);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 20);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 21);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 22);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 23);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 24);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 25);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 26);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 27);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 28);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 29);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 30);
    OSMetaClassDeclareReservedUnused(IOFramebuffer, 31);


public:
    // IOKit overrides
    static void initialize();

    virtual bool attach( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual bool requestTerminate( IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    virtual bool terminate( IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    virtual bool willTerminate( IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    virtual bool didTerminate( IOService * provider, IOOptionBits options, bool * defer ) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual IOWorkLoop * getWorkLoop() const APPLE_KEXT_OVERRIDE;
#ifndef _OPEN_SOURCE_
    virtual IOReturn setProperties(OSObject* properties) APPLE_KEXT_OVERRIDE;
#endif

    IOWorkLoop * getGraphicsSystemWorkLoop() const;
    IOWorkLoop * getControllerWorkLoop() const;

    virtual IOReturn requestProbe( IOOptionBits options ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn powerStateWillChangeTo ( IOPMPowerFlags, unsigned long, IOService* ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn powerStateDidChangeTo ( IOPMPowerFlags, unsigned long, IOService* ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn setPowerState( unsigned long powerStateOrdinal, IOService * device) APPLE_KEXT_OVERRIDE;
    virtual IOReturn setAggressiveness( unsigned long type, unsigned long newLevel ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn getAggressiveness( unsigned long type, unsigned long * currentLevel ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn newUserClient( task_t              owningTask,
                                    void *              security_id,
                                    UInt32              type,
                                    IOUserClient **     clientH ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn callPlatformFunction( const OSSymbol * functionName,
                                    bool waitForFunction,
                                    void *p1, void *p2,
                                    void *p3, void *p4 ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn message(UInt32 type, IOService *provider, void *argument = 0) APPLE_KEXT_OVERRIDE;

    virtual void hideCursor( void ) APPLE_KEXT_OVERRIDE;
    virtual void showCursor( IOGPoint * cursorLoc, int frame ) APPLE_KEXT_OVERRIDE;
    virtual void moveCursor( IOGPoint * cursorLoc, int frame ) APPLE_KEXT_OVERRIDE;
    // virtual
    void resetCursor( void );

    virtual void getVBLTime( AbsoluteTime * time, AbsoluteTime * delta ) APPLE_KEXT_OVERRIDE;

    virtual void getBoundingRect ( IOGBounds ** bounds ) APPLE_KEXT_OVERRIDE;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
    virtual IOReturn open( void );
    
    virtual void close( void );
#pragma clang diagnostic pop

    virtual bool isConsoleDevice( void );

    virtual IOReturn setupForCurrentConfig( void );

    virtual bool serializeInfo( OSSerialize * s );
    virtual bool setNumber( OSDictionary * dict, const char * key,
                                UInt32 number );

    IONotifier * addFramebufferNotification(
            IOFramebufferNotificationHandler handler,
            OSObject * obj, void * ref);

/*! @function getApertureRange
    @abstract Return reference to IODeviceMemory object representing memory range of framebuffer.
    @discussion IOFramebuffer subclasses must implement this method to describe the memory used by the framebuffer in the current mode. The OS will map this memory range into user space for client access - the range should only include vram memory not hardware registers.
    @param aperture The system will only access the aperture kIOFBSystemAperture.
    @result an IODeviceMemory instance. A reference will be consumed by the caller for each call of this method - the implementatation should create a new instance of IODeviceMemory for each call, or return one instance with a retain for each call.
*/

    virtual IODeviceMemory * getApertureRange( IOPixelAperture aperture ) = 0;

/*! @function getVRAMRange
    @abstract Return reference to IODeviceMemory object representing memory range of all the cards vram.
    @discussion IOFramebuffer subclasses should implement this method to describe all the vram memory available on the card. The OS will map this memory range into user space for client access - the range should only include vram memory not hardware registers.
    @result an IODeviceMemory instance. A reference will be consumed by the caller for each call of this method - the implementatation should create a new instance of IODeviceMemory for each call, or return one instance with a retain for each call.
*/

    virtual IODeviceMemory * getVRAMRange( void );

protected:

/*! @function handleEvent
    @abstract Notify IOFramebuffer superclass code of events.
    @discussion IOFramebuffer subclasses should call this IOFramebuffer method on certain power state changes.
    @param event The event that has occurred:<br>
    kIOFBNotifyWillPowerOff call before entering a state other than the maximum.<br>
    kIOFBNotifyDidPowerOn call after entering the maximum power state.<br>
    kIOFBNotifyWillPowerOff call before entering a state other than the maximum.<br>
    kIOFBNotifyDidPowerOn call after entering a state other than the maximum.<br>
    @param info None of the above events require additional info, pass zero.
    @result an IOReturn code, safely ignored.
*/

    IOReturn handleEvent( IOIndex event, void * info = 0 );


    IOReturn deliverFramebufferNotification(
                    IOIndex event, void * info = 0 );

    static void handleVBL(IOFramebuffer * inst, void * unused);

#ifdef IOFRAMEBUFFER_PRIVATE
#include <IOKit/graphics/IOFramebufferPrivate.h>
#endif

public:
/*! @function enableController
    @abstract Perform first time setup of the framebuffer.
    @discussion IOFramebuffer subclasses should perform their initialization of the hardware here. The IOService start() method is not called at a time appropriate for this initialization.
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from using the device.
*/

    virtual IOReturn enableController( void );

/*! @function getPixelFormats
    @abstract List the pixel formats the framebuffer supports.
    @discussion IOFramebuffer subclasses must implement this method to return an array of strings representing the possible pixel formats available in the framebuffer.
    @result A const char * pointer. The string consists of a concatenation of each pixel format string separated by the NULL character. The commonly supported pixel formats for Mac OS X are defined as IO8BitIndexedPixels, IO16BitDirectPixels, IO32BitDirectPixels.
*/

    virtual const char * getPixelFormats( void ) = 0;

/*! @function getDisplayModeCount
    @abstract Return the number of display modes the framebuffer supports.
    @discussion IOFramebuffer subclasses must implement this method to return a count of the display modes available. This count should change unless a connection change is posted for the device indicated the framebuffer and/or display configuration has changed.
    @result A count of the display modes available.
*/

    virtual IOItemCount getDisplayModeCount( void ) = 0;

/*! @function getDisplayModes
    @abstract Return the number of display modes the framebuffer supports.
    @discussion IOFramebuffer subclasses must implement this method to return an array of display mode IDs available for the framebuffer. The IDs are defined by the driver in the range 0x00000001 - 0x7fffffff, and should be constant for a given display mode. 
    @param allDisplayModes A caller allocated buffer with the size given by the result of getDisplayModeCount().
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from using the device.
*/
 
    virtual IOReturn getDisplayModes( IODisplayModeID * allDisplayModes ) = 0;

/*! @function getInformationForDisplayMode
    @abstract Return information about a given display mode.
    @discussion IOFramebuffer subclasses must implement this method to return information in the IODisplayModeInformation structure for the display mode with the passed ID. 
    @param displayMode A display mode ID previously returned by getDisplayModes().
    @param info Pointer to a structure of type IODisplayModeInformation to be filled out by the driver. IODisplayModeInformation is documented in IOGraphicsTypes.h.
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from using the device.
*/

    virtual IOReturn getInformationForDisplayMode( IODisplayModeID displayMode,
                    IODisplayModeInformation * info ) = 0;

/*! @function getPixelFormatsForDisplayMode
    @abstract Obsolete.
    @discussion IOFramebuffer subclasses must implement this method to return zero. 
    @param displayMode Ignored.
    @param depth Ignored.
    @result Return zero.
*/

    virtual UInt64  getPixelFormatsForDisplayMode( IODisplayModeID displayMode,
                    IOIndex depth ) = 0;

/*! @function getPixelInformation
    @abstract Return information about the framebuffer format for a given display mode and depth.
    @discussion IOFramebuffer subclasses must implement this method to return information in the IOPixelInformation structure for the display mode with the passed ID, depth index and aperture. The aperture utilized by the system is always kIOFBSystemAperture. Drivers may define alternative apertures, being a view of the framebuffer in a different pixel format from the default.
    @param displayMode A display mode ID previously returned by getDisplayModes().
    @param depth An index from zero to the value of the maxDepthIndex field from the IODisplayModeInformation structure (inclusive).
    @param info Pointer to a structure of type IOPixelInformation to be filled out by the driver. IOPixelInformation is documented in IOGraphicsTypes.h.
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from using the device.
*/

    virtual IOReturn getPixelInformation(
        IODisplayModeID displayMode, IOIndex depth,
        IOPixelAperture aperture, IOPixelInformation * pixelInfo ) = 0;

/*! @function getCurrentDisplayMode
    @abstract Return the framebuffers current display mode and depth.
    @discussion IOFramebuffer subclasses must implement this method to return the current mode and depth.
    @param displayMode A display mode ID representing the current mode.
    @param depth An index indicating the depth configuration of the framebuffer. The index should range from zero to the value of the maxDepthIndex field from the IODisplayModeInformation structure for the display mode.
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from using the device.
*/

    virtual IOReturn getCurrentDisplayMode( IODisplayModeID * displayMode,
                            IOIndex * depth ) = 0;

/*! @function setCurrentDisplayMode
    @abstract Set the framebuffers current display mode and depth.
    @discussion IOFramebuffer subclasses should implement this method to set the current mode and depth. Other than at enableController() time, this is the only method that should change the framebuffer format and is synchronized with clients and attached accelerators to make sure access to the device is disallowed during the change.
    @param displayMode A display mode ID representing the new mode.
    @param depth An index indicating the new depth configuration of the framebuffer. The index should range from zero to the value of the maxDepthIndex field from the IODisplayModeInformation structure for the display mode.
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from using the device.
*/

    virtual IOReturn setDisplayMode( IODisplayModeID displayMode,
                            IOIndex depth );

/*! @function setApertureEnable
    @abstract Enable an aperture on the framebuffer (usually unimplemented, no OS usage).
    @discussion IOFramebuffer subclasses may implement this method to set enable a non standard aperture. The system does not call this method.
    @param aperture A device specific aperture index.
    @param enable Device specific mask of options.
    @result an IOReturn code.
*/

    virtual IOReturn setApertureEnable( IOPixelAperture aperture,
                    IOOptionBits enable );

/*! @function setStartupDisplayMode
    @abstract Set the framebuffers display mode and depth to be used during boot and at startup.
    @discussion IOFramebuffer subclasses should implement this method to set the mode and depth to be used during boot and at startup, to reduce needed mode changes during boot when the display connection type is the same. If possible this mode should also be used by the OpenFirmware driver for the card.
    @param displayMode A display mode ID representing the new startup mode.
    @param depth An index indicating the new startup depth configuration of the framebuffer. The index should range from zero to the value of the maxDepthIndex field from the IODisplayModeInformation structure for the display mode.
    @result an IOReturn code.
*/

    virtual IOReturn setStartupDisplayMode( IODisplayModeID displayMode,
                            IOIndex depth );

/*! @function getCurrentDisplayMode
    @abstract Return the framebuffers display mode and depth to be used during boot and at startup.
    @discussion IOFramebuffer subclasses should implement this method to return the current mode and depth.
    @param displayMode A display mode ID representing the mode used during startup.
    @param depth An index indicating the depth configuration of the framebuffer used during startup. The index should range from zero to the value of the maxDepthIndex field from the IODisplayModeInformation structure for the display mode.
    @result an IOReturn code.
*/

    virtual IOReturn getStartupDisplayMode( IODisplayModeID * displayMode,
                            IOIndex * depth );

/*! @function setCLUTWithEntries
    @abstract Set the color lookup table to be used by the framebuffer in indexed modes.
    @discussion IOFramebuffer subclasses may implement this method to allow a palette to be set for indexed display modes. It will not be called on framebuffers in direct display modes.
    @param colors A pointer to an array of numEntries RGB color entries.
    @param index The index of the first entry to set.
    @param numEntries The number of entries in the table.
    @param options Options controlling the operation. <br>
    kSetCLUTByValue is set if the index field of each entry should be used to set the table sparsely, otherwise consecutive entries from the index parameter should be set.<br>
    kSetCLUTImmediately is set if the CLUT set should not be synchronized with the vertical blank, otherwise it should.<br>
    kSetCLUTWithLuminance is set if the CLUT should be set to a gray value equivalent in luminance to the passed color entry.<br>
    @result an IOReturn code.
*/

    virtual IOReturn setCLUTWithEntries( IOColorEntry * colors, UInt32 index,
                UInt32 numEntries, IOOptionBits options );

/*! @function setGammaTable
    @abstract Set the gamma table to be used by the framebuffer.
    @discussion IOFramebuffer subclasses should implement this method to allow a gamma table to be set. 
    @param channelCount Defines the number of channels in the supplied data. OS X will pass three for separate R, G, B data, or one if the same data should apply to all channels.
    @param dataCount The number of data entries per channel.
    @param dataWidth The number of bits in each entry. 8 for Mac OS X 10.1 and earlier, 16 for later releases.
    @param data The packed array of correction data. Data is passed for the R (or single) channel followed by the G & B channels. Each entry is one or two bytes (if dataWidth > 8).
    @result an IOReturn code.
*/

    virtual IOReturn setGammaTable( UInt32 channelCount, UInt32 dataCount,
                    UInt32 dataWidth, void * data );

/*! @function setAttribute
    @abstract Generic method to set some attribute of the framebuffer device.
    @discussion IOFramebuffer subclasses may implement this method to allow arbitrary attribute/value pairs to be set. 
    @param attribute Defines the attribute to be set. Some defined attributes are:<br> 
    kIOPowerAttribute The IOFramebuffer class implements most power management (IOService) methods. It calls the subclass to carry out the power management state change with this attribute. When carrying out power state changes, the subclass should call IOFramebuffer::handleEvent for certain changes - set that method for more information.
    @param value The new value for the attribute.
    @result an IOReturn code.
*/

    virtual IOReturn setAttribute( IOSelect attribute, uintptr_t value );

/*! @function getAttribute
    @abstract Generic method to retrieve some attribute of the framebuffer device.
    @discussion IOFramebuffer subclasses may implement this method to allow arbitrary attribute/value pairs to be returned. 
    @param attribute Defines the attribute to be set. Some defined attributes are:<br> 
    kIOHardwareCursorAttribute If the device supports a hardware cursor and implements the setCursorImage() and setCursorState() calls it should return true for this attribute.
    @param value Returns the value for the attribute.
    @result an IOReturn code.
*/

    virtual IOReturn getAttribute( IOSelect attribute, uintptr_t * value );

/*! @function getTimingInfoForDisplayMode
    @abstract Returns a timing description for a display mode.
    @discussion IOFramebuffer subclasses should implement this method to return timing information for a display mode. This allows the OS to enable display modes based on its knowledge of the connected display type. Two types of timing information are defined, by Apple defined constant, or by a detailed description of the timing parameters of the mode. 
    @param displayMode A display mode ID representing the mode to examine.
    @param info The driver returns the information for the display mode in this structure.<br>
    If the mode has an Apple defined constant, such as timingVESA_1024x768_75hz, it should be returned in the appleTimingID field. Otherwise the field should be set to timingInvalid.<br>
    If the driver is able to supply detailed timing information, it should return it in the detailedInfo.v2 field of the structure, otherwise the driver should clear the kIODetailedTimingValid flag from the flags field.<br>
    The IODetailedTimingInformationV2 structure is documented in IOGraphicsTypes.h
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from using the device.
*/

    virtual IOReturn getTimingInfoForDisplayMode(
                IODisplayModeID displayMode, IOTimingInformation * info );

/*! @function validateDetailedTiming
    @abstract Reports whether a detailed timing is able to be programmed with the device.
    @discussion IOFramebuffer subclasses may implement programmable mode functionality where the OS is able to install modes described by a detailed timing into the driver. 
    @param description A pointer to a IODetailedTimingInformationV2 structure. The driver should examine this description and change any fields that it cannot implement to reflect its closest possible implementation.
    @param descripSize sizeof(IODetailedTimingInformationV2)
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from installing the programmable mode.
*/

    virtual IOReturn validateDetailedTiming(
                    void * description, IOByteCount descripSize );

/*! @function setDetailedTimings
    @abstract Installs an array of OS programmed detailed timings to be made available by the driver.
    @discussion IOFramebuffer subclasses may implement programmable mode functionality where the OS is able to install modes described by a detailed timing into the driver. The driver needs to add these modes to its internal mode list if it provides this functionality.
    @param array An OSArray of OSData objects. Each OSData contains one IODetailedTimingInformationV2 structure. All the data described by the array should be copied or retained by this call until the next invocation of this method.
    @result an IOReturn code. A return other than kIOReturnSuccess will prevent the system from installing the programmable modes.
*/

    virtual IOReturn setDetailedTimings( OSArray * array );

/*! @function getConnectionCount
    @abstract Reports the number of display connections the device supports, driven from one framebuffer.
    @discussion IOFramebuffer subclasses may implement functionality where a single framebuffer drives multiple displays. This is not recommended or fully supported and instead multihead cards should implement multiple instances of IOFramebuffer objects to provide full functionality.
    @result A count of the number of display connections reported by the framebuffer. Current versions of OS X only support one connection completely.
*/

    virtual IOItemCount getConnectionCount( void );


/*! @function setAttributeForConnection
    @abstract Generic method to set some attribute of the framebuffer device, specific to one display connection.
    @discussion IOFramebuffer subclasses may implement this method to allow arbitrary attribute/value pairs to be set, specific to one display connection. 
    @param attribute Defines the attribute to be set. Some defined attributes are:<br> 
    kIOCapturedAttribute If the device supports hotplugging displays, it should disable the generation of hot plug interrupts when the attribute kIOCapturedAttribute is set to true.
    @param value The new value for the attribute.
    @result an IOReturn code.
*/

    virtual IOReturn setAttributeForConnection( IOIndex connectIndex,
                    IOSelect attribute, uintptr_t value );

/*! @function getAttributeForConnection
    @abstract Generic method to retrieve some attribute of the framebuffer device, specific to one display connection.
    @discussion IOFramebuffer subclasses may implement this method to allow arbitrary attribute/value pairs to be returned, specific to one display connection. 
    @param attribute Defines the attribute to be returned. Some defined attributes are:<br> 
    kConnectionSupportsHLDDCSense If the framebuffer supports the DDC methods hasDDCConnect() and getDDCBlock() it should return success (and no value) for this attribute.<br>
    kConnectionSupportsLLDDCSense If the framebuffer wishes to make use of IOFramebuffer::doI2CRequest software implementation of I2C it should implement the I2C methods setDDCClock(), setDDCData(), readDDCClock(), readDDCData(), and it should return success (and no value) for this attribute.<br>
    @param value Returns the value for the attribute.
    @result an IOReturn code.
*/

    virtual IOReturn getAttributeForConnection( IOIndex connectIndex,
                    IOSelect attribute, uintptr_t * value );

/*! @function convertCursorImage
    @abstract Utility method of IOFramebuffer to convert cursor image to a hardware cursor format.
    @discussion IOFramebuffer subclasses may implement hardware cursor functionality, if so they should pass the cursor image given by the  setCursorImage() method, with a description of their hardware cursor format, to this helper function to this routine to convert the image to one suitable for the hardware.
    @param cursorImage Opaque cursor parameter from the setCursorImage() call.
    @param description Describes the cursor format supported by the driver.
    @param cursor Structure describing the drivers allocated buffer to receive the converted image.
    @result a bool indicating the conversion was successful.
*/

    virtual bool convertCursorImage( void * cursorImage,
                IOHardwareCursorDescriptor * description,
                IOHardwareCursorInfo * cursor );

/*! @function setCursorImage
    @abstract Set a new image for the hardware cursor.
    @discussion IOFramebuffer subclasses may implement hardware cursor functionality, if so they should implement this method to change the hardware cursor image. The image should be passed to the convertCursorImage() method with each type of cursor format the hardware supports until success, if all fail the hardware cursor should be hidden and kIOReturnUnsupported returned.
    @param cursorImage Opaque cursor description. This should be passed to the convertCursorImage() method to convert to a format specific to the hardware.
    @result An IOReturn code.
*/

    virtual IOReturn setCursorImage( void * cursorImage );

/*! @function setCursorState
    @abstract Set a new position and visibility for the hardware cursor.
    @discussion IOFramebuffer subclasses may implement hardware cursor functionality, if so they should implement this method to change the position and visibility of the cursor.
    @param x Left coordinate of the cursor image. A signed value, will be negative if the cursor's hot spot and position place it partly offscreen.
    @param y Top coordinate of the cursor image. A signed value, will be negative if the cursor's hot spot and position place it partly offscreen.
    @param visible Visible state of the cursor.
    @result An IOReturn code.
*/

    virtual IOReturn setCursorState( SInt32 x, SInt32 y, bool visible );

/*! @function flushCursor
    @abstract Perform any needed cache flushing after software cursor rendering.
    @discussion IOFramebuffer implements software cursor functionality when a hardware cursor is unavailable. Some hardware may need to flush a cache after the processor has finished lifting and dropping the software cursor.
*/

    virtual void flushCursor( void );

/*! @function getAppleSense
    @abstract Return display sense information for legacy Apple sensing.
    @discussion Hardware that supports simple display sensing, or the classic 3 pin Apple sensing described in Designing Cards and Drivers, should implement this method to return sense information.
    @param connectIndex Index of the display connection, from zero to the value of getConnectionCount().
    @param senseType Return zero to indicate legacy Apple sensing.
    @param primary Return the value of the primary Apple sense code.
    @param extended Return the value of the secondary Apple sense code.
    @param displayType Return an Apple defined constant for the type of display sensed. For example, kVGAConnect, kNTSCConnect, kPALConnect etc.
    @result An IOReturn code.
*/

    virtual IOReturn getAppleSense( IOIndex connectIndex,
            UInt32 * senseType,
            UInt32 * primary,
            UInt32 * extended,
            UInt32 * displayType );

/*! @function connectFlags
    @abstract Return display sense information for legacy Apple sensing.
    @discussion Hardware that supports simple display sensing, or the classic 3 pin Apple sensing described in Designing Cards and Drivers, should implement this method to return mode flags relative to the sensed display. If this method is unimplemented, all modes have are given the flags kDisplayModeValidFlag | kDisplayModeSafeFlag.
    @param connectIndex Index of the display connection, from zero to the value of getConnectionCount().
    @param displayMode A display mode ID.
    @param flags Return the flags value for the given mode with the connected display. Flags are:<br>
    kDisplayModeValidFlag - mode is considered valid for the connected display by the driver.
    kDisplayModeSafeFlag - mode is considered safe (not requiring mode change confirmation) for the connected display by the driver.
    kDisplayModeDefaultFlag - mode is considered default for the connected display by the driver.
    @result An IOReturn code.
*/

    virtual IOReturn connectFlags( IOIndex connectIndex,
                    IODisplayModeID displayMode, IOOptionBits * flags );

    //// IOLowLevelDDCSense

/*! @function setDDCClock
    @abstract Sets the state of the I2C clock line on a bus.
    @discussion Framebuffers making use of the IOFramebuffer::doI2CRequest() software implementation of I2C should implement this method to set the state of the I2C clock line on the given bus. Otherwise may be unimplemented.
    @param bus Index of the bus.
    @param value One of kIODDCLow, kIODDCHigh, kIODDCTristate.
*/

    virtual void setDDCClock( IOIndex bus, UInt32 value );

/*! @function setDDCData
    @abstract Sets the state of the I2C data line on a bus.
    @discussion Framebuffers making use of the IOFramebuffer::doI2CRequest() software implementation of I2C should implement this method to set the state of the I2C data line on the given bus. Otherwise may be unimplemented.
    @param bus Index of the bus.
    @param value One of kIODDCLow, kIODDCHigh, kIODDCTristate.
*/
    virtual void setDDCData( IOIndex bus, UInt32 value );

/*! @function readDDCClock
    @abstract Reads the input state of the I2C clock line on a bus.
    @discussion Framebuffers making use of the IOFramebuffer::doI2CRequest() software implementation of I2C should implement this method to return the input state of the I2C clock line on the given bus. Otherwise may be unimplemented.
    @param bus Index of the bus.
    @result A boolean reflecting the current state of the clock line on the given bus.
*/
    virtual bool readDDCClock( IOIndex bus );

/*! @function readDDCData
    @abstract Reads the input state of the I2C data line on a bus.
    @discussion Framebuffers making use of the IOFramebuffer::doI2CRequest() software implementation of I2C should implement this method to return the input state of the I2C data line on the given bus. Otherwise may be unimplemented.
    @param bus Index of the bus.
    @result A boolean reflecting the current state of the data line on the given bus.
*/
    virtual bool readDDCData( IOIndex bus );

    virtual IOReturn enableDDCRaster( bool enable );

/*! @function hasDDCConnect
    @abstract Return display DDC connect state.
    @discussion Hardware that supports DDC/EDID display sensing should implement this method to return true if a DDC display is detected. They should also return success for the connection attribute kConnectionSupportsHLDDCSense (from getAttributeForConnection()).
    @param connectIndex Index of the display connection, from zero to the value of getConnectionCount().
    @result True if a DDC display is detected.
*/

    virtual bool hasDDCConnect( IOIndex connectIndex );

/*! @function getDDCBlock
    @abstract Return display EDID data.
    @discussion Hardware that supports DDC/EDID display sensing should implement this method to return EDID data in 128 byte blocks.
    @param connectIndex Index of the display connection, from zero to the value of getConnectionCount().
    @param blockNumber Block number, ranging from one to the number of blocks return by the display.
    @param blockType kIODDCBlockTypeEDID will be passed. 
    @param options No options are currently defined.
    @param data Caller allocated buffer to receive the blocks data.
    @param length In/out parameter - callers allocated buffer size, driver returns actual size.
    @result An IOReturn code.
*/

    virtual IOReturn getDDCBlock( IOIndex connectIndex, UInt32 blockNumber,
                    IOSelect blockType, IOOptionBits options,
                    UInt8 * data, IOByteCount * length );

/*! @function registerForInterruptType
    @abstract Set callbacks for driver to call on interrupt events.
    @discussion The IOFramebuffer class will call its subclasses to set callbacks to be called on interrupt events generated by hardware events. Only two are currently in use - vertical blank interrupts and connection changed interrupts.
    @param interruptType One of these constants:<br>
    kIOFBVBLInterruptType Specifying a vertical blanking interrupt.
    kIOFBConnectInterruptType Specify the display connection should be resensed.
    @param proc C callback to be called by the driver when the specified event occurs.
    @param target Target parameter for the callback proc.
    @param ref Ref parameter for the callback proc.
    @param interruptRef The subclass should return an opaque reference to the installed interrupt handler, for use with unregisterInterrupt() and setInterruptState().
    @result An IOReturn code.
*/

    virtual IOReturn registerForInterruptType( IOSelect interruptType,
                    IOFBInterruptProc proc, OSObject * target, void * ref,
                    void ** interruptRef );

/*! @function unregisterInterrupt
    @abstract Remove a callback previously installed by registerForInterruptType().
    @discussion Remove a callback previously installed by registerForInterruptType().
    @param interruptRef The interruptRef returned from the registerForInterruptType call that installed the interrupt.
    @result An IOReturn code.
*/

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
    virtual IOReturn unregisterInterrupt( void * interruptRef );
#pragma clang diagnostic pop

/*! @function unregisterInterrupt
    @abstract Enable or disable a callback previously installed by registerForInterruptType().
    @discussion Enable or disable a callback previously installed by registerForInterruptType().
    @param state True or false to enable the callback.
    @result An IOReturn code.
*/

    virtual IOReturn setInterruptState( void * interruptRef, UInt32 state );

    virtual IOReturn getNotificationSemaphore( IOSelect interruptType,
                                               semaphore_t * semaphore );

	IOReturn setBackingFramebuffer(const IOPixelInformation * info,
									uint32_t bufferCount,
									void * mappedAddress[]);
	IOReturn switchBackingFramebuffer(uint32_t bufferIndex);

/*  non WL clients apis
*/
    IOReturn setAttributeExt( IOSelect attribute, uintptr_t value );
    
    IOReturn getAttributeExt( IOSelect attribute, uintptr_t * value );
    
    IOReturn setAttributeForConnectionExt( IOIndex connectIndex,
               IOSelect attribute, uintptr_t value );
    
    IOReturn getAttributeForConnectionExt( IOIndex connectIndex,
            IOSelect attribute, uintptr_t * value );
    
    /* New */
    IONotifier * addFramebufferNotificationWithOptions(IOFramebufferNotificationHandler handler,
                                                       OSObject * target, void * ref,
                                                       IOSelect groupID, IOIndex groupPriority, IOSelect events);

    void resetLimitState(void);
    void sendLimitState(const uint32_t line);
};

#endif /* ! _IOKIT_IOFRAMEBUFFER_H */
