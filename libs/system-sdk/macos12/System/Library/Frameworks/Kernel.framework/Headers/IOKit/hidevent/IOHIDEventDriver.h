/*
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_HID_IOHIDEVENTDRIVER_H
#define _IOKIT_HID_IOHIDEVENTDRIVER_H

#include <IOKit/IOService.h>
#include <IOKit/hidsystem/IOHIDTypes.h>
#include <IOKit/hid/IOHIDInterface.h>
#include <IOKit/hidevent/IOHIDEventService.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>

class DigitizerTransducer;
class EventElementCollection;
class IOHIDEvent;

/*! @class IOHIDEventDriver : public IOHIDEventService
    @abstract
    @discussion
*/
#if defined(KERNEL) && !defined(KERNEL_PRIVATE)
class __deprecated_msg("Use DriverKit") IOHIDEventDriver : public IOHIDEventService
#else
class IOHIDEventDriver : public IOHIDEventService
#endif
{
    OSDeclareDefaultStructors( IOHIDEventDriver )

private:
    IOHIDInterface *            _interface;
    void *                      _reservedVoid[3] __unused;
    
    OSArray *                   _supportedElements;
        
    UInt32                      _bootSupport;
    bool                        _multipleReports;
    bool                        _authenticatedDevice;
    bool                        _reservedBool   __unused;
    UInt32                      _reservedUInt32 __unused;
    bool                        _reservedBool1  __unused;
    
    
    struct ExpansionData {
        SInt32  absoluteAxisRemovalPercentage;
        SInt32  preferredAxisRemovalPercentage;
        
        struct {
            OSArray *           elements;
        } led;
        
        struct {
            OSArray *           elements;
        } scroll;
        
        struct {
            OSArray *           elements;
            bool                disabled;
        } relative;
        
        struct {
            OSArray *           elements;
            OSArray *           blessedUsagePairs;
            UInt8               bootMouseData[4];
            bool                appleVendorSupported;
            IOHIDElement *      keyboardPower;
        } keyboard;
        
        struct {
            IOHIDElement *      deviceModeElement;
            OSArray *           transducers;
            OSArray *           transducerStates;
            IOHIDElement *      touchCancelElement;
            bool                native;
            bool                collectionDispatch;
            IOFixed             centroidX;
            IOFixed             centroidY;
            IOHIDElement *      relativeScanTime;
            IOHIDElement *      surfaceSwitch;
            IOHIDElement *      reportRate;
            OSArray *           buttons;
        } digitizer;
        
        struct {
            OSArray *           elements;

            UInt32              capable;
            UInt32              sendingReportID;
            IOFixed             axis[6];
            UInt32              buttonState;
            IOOptionBits        options;            
            bool                disabled;
        } multiAxis;
        
        struct {
            IOHIDElement *      gestureStateElement;
            OSArray *           legacyElements;
            OSArray *           gesturesCandidates;
        } unicode;
        
        struct {
            bool                extended;
            bool                formFitting;
            OSArray *           elements;
            UInt32              capable;
            UInt32              sendingReportID;
            
            struct {
                IOFixed up;
                IOFixed down;
                IOFixed left;
                IOFixed right;
            } dpad;
            
            struct {
                IOFixed x;
                IOFixed y;
                IOFixed a;
                IOFixed b;
            }face;
            
            struct {
                IOFixed x;
                IOFixed y;
                IOFixed z;
                IOFixed rz;
            } joystick;
            
            struct {
                IOFixed l1;
                IOFixed l2;
                IOFixed r1;
                IOFixed r2;
            } shoulder;

            struct {
                unsigned int  left;
                unsigned int  right;
            } thumbstick;
            
        } gameController;
      
        struct {
            OSArray *           childElements;
            OSArray *           primaryElements;
            OSArray *           pendingEvents;
        } vendorMessage;
        
        struct {
            OSArray *           elements;
        } biometric;

        struct {
            OSArray *           elements;
        } accel;

        struct {
            OSArray *           elements;
        } gyro;
        
        struct {
            OSArray *           elements;
        } compass;

        struct {
            OSArray *           elements;
        } temperature;

        struct {
            IOHIDElement *      reportInterval;
            IOHIDElement *      maxFIFOEvents;
            IOHIDElement *      reportLatency;
            IOHIDElement *      sniffControl;
        } sensorProperty;

        struct {
            OSArray *           cmElements;
            OSArray *           tiltElements;
        } orientation;

        struct {
            OSArray *           phaseElements;
            IOHIDElement *      longPress;
            IOOptionBits        phaseFlags;
            IOOptionBits        prevPhaseFlags;
        } phase;

        struct {
            OSArray * elements;
        } proximity;

        UInt64  lastReportTime;

        IOWorkLoop *            workLoop;
        IOCommandGate *         commandGate;
    };

    ExpansionData *             _reserved;

    bool                    getBlessedUsagePairs();
    bool                    parseElements(OSArray * elementArray, UInt32 bootProtocol);
    bool                    parseRelativeElement(IOHIDElement * element);
    bool                    parseMultiAxisElement(IOHIDElement * element);
    bool                    parseGameControllerElement(IOHIDElement * element);
    bool                    parseDigitizerElement(IOHIDElement * element);
    bool                    parseDigitizerTransducerElement(IOHIDElement * element, IOHIDElement * parent=NULL);
    bool                    parseScrollElement(IOHIDElement * element);
    bool                    parseLEDElement(IOHIDElement * element);
    bool                    parseKeyboardElement(IOHIDElement * element);
    bool                    parseUnicodeElement(IOHIDElement * element);
    bool                    parseLegacyUnicodeElement(IOHIDElement * element);
    bool                    parseGestureUnicodeElement(IOHIDElement * element);
    bool                    parseVendorMessageElement(IOHIDElement * element);
    bool                    parseBiometricElement(IOHIDElement * element);
    bool                    parseAccelElement(IOHIDElement * element);
    bool                    parseGyroElement(IOHIDElement * element);
    bool                    parseCompassElement(IOHIDElement * element);
    bool                    parseTemperatureElement(IOHIDElement * element);
    bool                    parseSensorPropertyElement(IOHIDElement * element);
    bool                    parseDeviceOrientationElement(IOHIDElement * element);
    bool                    parsePhaseElement(IOHIDElement * element);
    bool                    parseProximityElement(IOHIDElement * element);

    void                    processLEDElements();
    void                    processDigitizerElements();
    void                    processMultiAxisElements();
    void                    processGameControllerElements();
    void                    processUnicodeElements();
    
    void                    setRelativeProperties();
    void                    setDigitizerProperties();
    void                    setGameControllerProperties();
    void                    setMultiAxisProperties();
    void                    setScrollProperties();
    void                    setLEDProperties();
    void                    setKeyboardProperties();
    void                    setUnicodeProperties();
    void                    setAccelerationProperties();
    void                    setVendorMessageProperties();
    void                    setBiometricProperties();
    void                    setAccelProperties();
    void                    setGyroProperties();
    void                    setCompassProperties();
    void                    setTemperatureProperties();
    void                    setSensorProperties();
    void                    setDeviceOrientationProperties();
    void                    setSurfaceDimensions();

    UInt32                  checkGameControllerElement(IOHIDElement * element);
    UInt32                  checkMultiAxisElement(IOHIDElement * element);
    
    static void             calibrateJustifiedPreferredStateElement(IOHIDElement * element, SInt32 removalPercentage);
    static void             calibrateCenteredPreferredStateElement(IOHIDElement * element, SInt32 removalPercentage);

    void                    handleBootPointingReport(AbsoluteTime timeStamp, IOMemoryDescriptor * report, UInt32 reportID);
    void                    handleRelativeReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleGameControllerReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleMultiAxisPointerReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleDigitizerCollectionReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleDigitizerReport(AbsoluteTime timeStamp, UInt32 reportID);
    IOHIDEvent*             handleDigitizerTransducerReport(DigitizerTransducer * transducer, AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleScrollReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleKeboardReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleUnicodeReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleUnicodeLegacyReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleUnicodeGestureReport(AbsoluteTime timeStamp, UInt32 reportID);
    IOHIDEvent *            handleUnicodeGestureCandidateReport(EventElementCollection * candidate, AbsoluteTime timeStamp, UInt32 reportID);

    void                    handleVendorMessageReport(AbsoluteTime timeStamp, IOMemoryDescriptor * report, UInt32 reportID, int phase);
    void                    handleBiometricReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleAccelReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleGyroReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleCompassReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleTemperatureReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleDeviceOrientationReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handlePhaseReport(AbsoluteTime timeStamp, UInt32 reportID);
    void                    handleProximityReport(AbsoluteTime, UInt32 reportID);

    bool                    serializeCharacterGestureState(void * ref, OSSerialize * serializer);
    bool                    conformTo (UInt32 usagePage, UInt32 usage);
    IOHIDEvent*             createDigitizerTransducerEventForReport(DigitizerTransducer * transducer, AbsoluteTime timeStamp, UInt32 reportID);
    bool                    serializeDebugState(void * ref, OSSerialize * serializer);
    UInt32                  getButtonStateFromElements(OSArray * elements);

protected:

    virtual void            free(void) APPLE_KEXT_OVERRIDE;
    
    virtual OSArray *       getReportElements(void) APPLE_KEXT_OVERRIDE;
    
    virtual bool            handleStart( IOService * provider ) APPLE_KEXT_OVERRIDE;

    virtual void            handleStop(  IOService * provider ) APPLE_KEXT_OVERRIDE;
    
    virtual void            handleInterruptReport (
                                AbsoluteTime                timeStamp,
                                IOMemoryDescriptor *        report,
                                IOHIDReportType             reportType,
                                UInt32                      reportID);

    virtual OSString *      getTransport (void) APPLE_KEXT_OVERRIDE;

    virtual UInt32          getLocationID (void) APPLE_KEXT_OVERRIDE;

    virtual UInt32          getVendorID (void) APPLE_KEXT_OVERRIDE;

    virtual UInt32          getVendorIDSource (void) APPLE_KEXT_OVERRIDE;

    virtual UInt32          getProductID (void) APPLE_KEXT_OVERRIDE;

    virtual UInt32          getVersion (void) APPLE_KEXT_OVERRIDE;

    virtual UInt32          getCountryCode (void) APPLE_KEXT_OVERRIDE;

    virtual OSString *      getManufacturer (void) APPLE_KEXT_OVERRIDE;

    virtual OSString *      getProduct (void) APPLE_KEXT_OVERRIDE;

    virtual OSString *      getSerialNumber (void) APPLE_KEXT_OVERRIDE;

    virtual IOReturn        setElementValue (
                                UInt32                      usagePage,
                                UInt32                      usage,
                                UInt32                      value ) APPLE_KEXT_OVERRIDE;
    
    virtual UInt32          getElementValue ( 
                                UInt32                      usagePage,
                                UInt32                      usage ) APPLE_KEXT_OVERRIDE;

    virtual void            dispatchEvent(IOHIDEvent * event, IOOptionBits options=0) APPLE_KEXT_OVERRIDE;

public:


    virtual bool            init( OSDictionary * dictionary = 0 ) APPLE_KEXT_OVERRIDE;

    virtual bool            didTerminate(
                                IOService *                 provider,
                                IOOptionBits                options,
                                bool *                      defer ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn        setProperties( OSObject * properties ) APPLE_KEXT_OVERRIDE;

    virtual IOHIDEvent *    copyEvent(
                                IOHIDEventType              type,
                                IOHIDEvent *                matching = 0,
                                IOOptionBits                options = 0) APPLE_KEXT_OVERRIDE;
    
    virtual IOHIDEvent *copyMatchingEvent(OSDictionary *matching) APPLE_KEXT_OVERRIDE;
  
  
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  0);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  1);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  2);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  3);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  4);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  5);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  6);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  7);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  8);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver,  9);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 10);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 11);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 12);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 13);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 14);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 15);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 16);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 17);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 18);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 19);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 20);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 21);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 22);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 23);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 24);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 25);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 26);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 27);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 28);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 29);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 30);
    OSMetaClassDeclareReservedUnused(IOHIDEventDriver, 31);

};

#endif /* !_IOKIT_HID_IOHIDEVENTDRIVER_H */
