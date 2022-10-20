/*
	File:		IOBluetoothHIDDriver.h
	Contains:	Driver for generic Bluetooth HID devices.
	Copyright:	(c) 2001-2008 by Apple, all rights reserved.
*/

#ifndef IOBLUETOOTHHIDDRIVER_H
#define IOBLUETOOTHHIDDRIVER_H

#import <IOKit/hid/IOHIDDevice.h>
#import <IOKit/bluetooth/Bluetooth.h>

#import "IOKit/pwr_mgt/RootDomain.h"

//===========================================================================================================================
// Macros
//===========================================================================================================================

#define kBSKernelMonitor2Notification				'bsk2'

//===========================================================================================================================
// Forwards
//===========================================================================================================================

class IOTimerEventSource;
class IOWorkLoop;

class IOBluetoothL2CAPChannel;
class IOBluetoothDevice;
class IOWorkQueue;

//===========================================================================================================================
// IOBluetoothHIDDriver
//===========================================================================================================================

class IOBluetoothHIDDriver : public IOHIDDevice
{
	OSDeclareDefaultStructors( IOBluetoothHIDDriver )

    IOWorkLoop*					_workLoop;
	IOCommandGate*				_commandGate;
	
	IOWorkQueue					*_desyncWorkQueue;
	
	IOBluetoothL2CAPChannel*	_controlChannel;
    IOBluetoothL2CAPChannel*	_interruptChannel;

    IOBluetoothDevice*			_device;

	IOMemoryDescriptor*			_memDescriptor;
	IOMemoryDescriptor*			_getReportDescriptor;
	IONotifier*					_interruptOpenNotification;
	IOTimerEventSource*			_timer;
    IONotifier*					_sleepWakeNotifier;

	bool						_deviceReady;

	UInt8						_expectedReportID;
	UInt8						_expectedReportType;
	UInt8						_handshake;
	
	OSDictionary*				_deviceProperties;

	UInt16						_vendorIDSource;
	UInt16						_vendorID;
	UInt16						_productID;
	UInt16						_deviceVersion;
	uint32_t					_classOfDevice;
    UInt16                      _countryCode;
	
	BluetoothDeviceAddress		_deviceAddress;
	char						_deviceAddressString[20];
	
	uint32_t					_outstandingIO;
	bool						_sendOutstanding;
	
	// Debug / Behavior Modifiers
	UInt8						_verboseLevel;
	bool						_logPackets;
	bool						_decodePackets;
	bool						_logOutstandingIO;
	bool						_suppressDisconnectNotifications;
	bool						_suppressSetProtocol;
	bool						_driverIsAwake;
	bool						_reservedFlag4;
	UInt8						_reservedByte;
	
    struct ExpansionData 
    { 
        OSArray*				_sendQueue;
		
		uint8_t					*interruptBuffer;
		uint32_t				interruptBufferUsed;
		
		uint8_t					*controlBuffer;
		uint32_t				controlBufferUsed;

		uint8_t					deviceSupportsSuspend;

		uint32_t				getReportTimeoutMS;
		uint32_t				setReportTimeoutMS;
		
		uint32_t				outstandingMemoryBlockCount;
		bool					waitingForMemoryBlockCount;

		IOPMrootDomain *        fRootDomain;
		IOPMDriverAssertionID   fNoDeepSleepAssertionId;
		
		bool					mCommandGateCreated;
		bool					mCommandGateAdded;
		bool					mControlChannelRetained;
		bool					mWorkLoopRetained;
		
		bool					mCloseDownServicesCalled;
		
		bool					mGotNoDeepSleepAssertionID;
		
        OSString*				disconnectionNotificationString;
        OSString*				connectionNotificationString;
        
		IOTimerEventSource*		deviceConnectTimer;
		
		bool					mNeedToDropData;
		
		UInt32					mWakeTime;
		
		UInt32					mDriverLoadTime;
		
		IOTimerEventSource*		mReadyToSleepTimer;
		
		bool					mHandleStopBeingCalled;
		
		bool					mHandleStartCompleted;

		bool					mHIDSuspendSent;
		
		IOReturn				mExitHIDSuspendResult;
		
		bool					mPrintDebug;
		
		IOTimerEventSource*		mPrintDebugTimer;
		
		UInt32					mPrintDebugDuration;
		
		uint32_t				mCanSendData;
		
		bool					mReturnDeviceError;
		
		UInt8					mHandshakeTimeoutCounter;
		
		bool					mUseLongTimeout;
};
    ExpansionData	*_expansionData;
	
public:
	// Standard IOService Methods
    virtual IOService*  probe( IOService * provider, SInt32 * score );
	virtual bool		init( OSDictionary *properties );
	virtual void		free(void);
	virtual bool 		willTerminate( IOService * provider, IOOptionBits options );
	
	// Starting & Stopping
    virtual bool		handleStart( IOService * provider );
    virtual void		handleStop( IOService *  provider );
	virtual void		deviceReady(void);
	virtual void		closeDownServices(void);

	// Power Management
    virtual void		handleSleep(void);
    virtual void		handleWake(void);
    virtual void		handleShutdown(void);			// Does nothing
    virtual void		handleRestart(void);			// Does nothing
	
	// HID Properties
	virtual OSString*	newTransportString(void) const;
	virtual OSString*	newManufacturerString(void) const;
	virtual OSString*	newProductString(void) const;
	virtual OSNumber*	newVendorIDSourceNumber(void) const;
	virtual OSNumber*	newVendorIDNumber(void) const;
	virtual OSNumber*	newProductIDNumber(void) const;
	virtual OSNumber*	newVersionNumber(void) const;
    virtual IOReturn	newReportDescriptor( IOMemoryDescriptor ** descriptor ) const;
	virtual OSString*	newSerialNumberString(void) const;
	virtual OSNumber*	newLocationIDNumber(void) const;
    virtual OSNumber*	newCountryCodeNumber(void) const;
	virtual OSNumber*	newReportIntervalNumber(void) const;

	// Main UserLand Entry Points
    virtual IOReturn	getReport( IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options = 0 );
    virtual IOReturn 	setReport( IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options = 0 );
    virtual IOReturn	setProperties( OSObject* properties );

	// General IO
	virtual IOReturn	sendData( IOBluetoothL2CAPChannel* theChannel, void* theData, IOByteCount theSize );
    virtual void		processControlData( UInt8 *buffer, UInt16 length );
    virtual void		processInterruptData( UInt8 *buffer, UInt16 length );
	virtual IOReturn	waitForData( IOMemoryDescriptor* report, UInt8 btReportType, UInt8 reportID );
	virtual IOReturn	waitForHandshake(void);
	
	// HID Transaction Methods
	virtual IOReturn	hidControl( UInt8 controlOperation );
	virtual int			getProtocol(void);
	virtual IOReturn	setProtocol( UInt8 protocol );
	virtual int			getIdle(void);
	virtual IOReturn	setIdle( UInt8 idleRate );

	// Device Introspection
	virtual bool		isKeyboard(void);
	virtual bool		isMouse(void);
	
	// Misc
	virtual IOReturn	setPowerState( unsigned long powerStateOrdinal, IOService* whatDevice );
	virtual IOReturn	createCommandGate( IOService* provider );
	virtual IOReturn	getDeviceProperties( IOService* provider );
	virtual	bool		readDeviceName(void);
	
	// Command Gate Actions
	static	IOReturn	staticCloseDownServicesAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticSendToAction( OSObject* owner, void* theChannel, void* theData, void *theSize, void* arg4);
	static	IOReturn	staticPrepControlChannelAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticInterruptChannelOpeningAction( OSObject* owner, void* newService, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticWillTerminateAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	
	// Work Loop Methods
	virtual void		closeDownServicesWL(void);
	virtual IOReturn	prepInterruptChannelWL(void);
	virtual IOReturn	getReportWL( IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options );
	virtual IOReturn	setReportWL( IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options );
	virtual IOReturn	processCommandWL( OSString* command, OSNumber* commandParameter );
	virtual IOReturn	getDevicePropertiesWL( IOService* provider );
	virtual IOReturn	interruptChannelOpeningWL( IOBluetoothL2CAPChannel* theChannel );

	// Timeout Handler
	static	void		deviceConnectTimerFired( OSObject* owner, IOTimerEventSource* sender );
	static	void		timerFired( OSObject* owner, IOTimerEventSource* sender );
	virtual void		handleTimeout(void);
	
	// IO Counting
	virtual void		incrementOutstandingIO(void);
	virtual void		decrementOutstandingIO(void);
	
	// ReadyToSleepTimeout Handler
	static	void		ReadyToSleepTimerFired( OSObject* owner, IOTimerEventSource* sender );
	
	// mPrintDebugTimer Handler
	static	void		debugPrintTimerFired( OSObject* owner, IOTimerEventSource* sender );

	
private:
	// Lazy Interrupt Channel Methods
	static	bool		interruptChannelOpeningCallback(	void* me, void* ignoreMe, IOService* newService, IONotifier *notifier );
    static	IOReturn 	powerStateHandler( void *target, void *refCon, UInt32 messageType, IOService *service, void *messageArgument, vm_size_t argSize );
	static	UInt32		ConvertAddressToUInt32 (void	*	address);

public:
    OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  0 );
    virtual void        	sendDeviceDisconnectNotifications( void );

	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  1 );
	virtual IOReturn		setPowerStateWL( unsigned long powerStateOrdinal, IOService* whatDevice ); 
    
	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  2 );
    virtual void        	sendDeviceConnectNotifications( void );

	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  3 );
    virtual void        	decrementOutstandingMemoryBlockCount( void );

	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  4 );
    virtual IOReturn       	willTerminateWL( void );

	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  5 );
	virtual void			messageClientsWithString( UInt32 type, OSString* message );
	
    OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  6 );
	virtual void			waitForInterruptChannel( void );
	
    OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  7 );
	virtual void			handleStopWL( IOService *  provider );
	
    OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  8 );
	virtual UInt32			GetCurrentTime( void );
	
    OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  9 );
	virtual void			handleReadyToSleepTimerFired(void);

    OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver, 10 );
	virtual	IOReturn		HIDCommandSleep (void		*	event,
											 UInt32			milliseconds,
											 char		*	calledByFunction,
											 bool			panicMachine);
	
    OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver, 11 );
	virtual	void			SuppressConnectionLostNotification(void);
	
	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver, 12 );
	virtual void			handleDebugPrintTimerFired(void);

	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver, 13 );
	virtual void			DeviceTerminated(void);

 	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver, 14 );
	virtual IOReturn		waitForOkToSend(void);

	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver, 15 );
	virtual void			processOkToSendNotification(void);
	
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 16 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 17 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 18 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 19 );
};

#endif	// IOBLUETOOTHHIDDRIVER_H
