/*
 * Copyright (c) 1998-2008 Apple Inc. All rights reserved.
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

#ifndef _IONETWORKINTERFACE_H
#define _IONETWORKINTERFACE_H

/*! @defined kIONetworkInterfaceClass
    @abstract The name of the IONetworkInterface class. 
*/

#define kIONetworkInterfaceClass  "IONetworkInterface"

/*! @defined kIONetworkData
    @abstract A property of IONetworkInterface objects. 
    @discussion The kIONetworkData property has an OSDictionary value and is a
        container for the set of IONetworkData objects managed by the interface.
        Each entry in the dictionary is a key/value pair consisting of
        the network data name, and an OSDictionary describing the
        contents of the network data. 
*/

#define kIONetworkData            "IONetworkData"

/*! @defined kIOInterfaceType
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOInterfaceType property has an OSNumber value that
        specifies the type of network interface that this interface represents.
        The type constants are defined in bsd/net/if_types.h. 
*/

#define kIOInterfaceType          "IOInterfaceType"

/*! @defined kIOMaxTransferUnit
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOMaxTransferUnit property has an OSNumber value that
        specifies the maximum transfer unit for the interface in bytes.
*/

#define kIOMaxTransferUnit        "IOMaxTransferUnit"

/*! @defined kIOMediaAddressLength
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOMediaAddressLength property has an OSNumber value that
        specifies the size of the media address in bytes. 
*/

#define kIOMediaAddressLength     "IOMediaAddressLength"

/*! @defined kIOMediaHeaderLength
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOMediaHeaderLength property has an OSNumber value that
        specifies the size of the media header in bytes. 
*/

#define kIOMediaHeaderLength      "IOMediaHeaderLength"

/*! @defined kIOInterfaceFlags
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOInterfaceFlags property has an OSNumber value that
        specifies the current value of the interface flags. The flag constants
        are defined in bsd/net/if.h. 
*/

#define kIOInterfaceFlags         "IOInterfaceFlags"

/*! @defined kIOInterfaceExtraFlags
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOInterfaceExtraFlags property has an OSNumber value that
        specifies the current value of the interface eflags. The eflag constants
        are defined in bsd/net/if.h. 
*/

#define kIOInterfaceExtraFlags    "IOInterfaceExtraFlags"

/*! @defined kIOInterfaceUnit
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOInterfaceUnit property has an OSNumber value that
        describes the unit number assigned to the interface object. 
*/

#define kIOInterfaceUnit          "IOInterfaceUnit"

/*! @defined kIOInterfaceState
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOInterfaceState property has an OSNumber value that
        describes the current state of the interface object. This property is
        not exported to BSD via the ifnet structure. 
*/

#define kIOInterfaceState         "IOInterfaceState"

/*! @defined kIOInterfaceNamePrefix
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOInterfaceNamePrefix property has an OSString value that
        describes the string prefix for the BSD name assigned to the interface. 
*/

#define kIOInterfaceNamePrefix    "IOInterfaceNamePrefix"

/*! @defined kIOPrimaryInterface
    @abstract A property of IONetworkInterface objects.
    @discussion The kIOInterfaceNamePrefix property has an OSBoolean value that
        describes whether the interface is the primary or the built-in network
        interface. 
*/

#define kIOPrimaryInterface       "IOPrimaryInterface"

/*! @defined kIOBuiltin
    @abstract kIOBuiltin is a property of IONetworkInterface
        objects. It has an OSBoolean value.
    @discussion The kIOBuiltin property describes whether the
        interface is built-in. 
*/

#define kIOBuiltin                "IOBuiltin"

/*! @defined kIOLocation
    @abstract kIOLocation is a property of IONetworkInterface
        objects. It has an OSString value.
    @discussion The kIOLocation property describes the physical 
        location of built-in interfaces. 
*/

#define kIOLocation               "IOLocation"

/*! @defined kIONetworkNoBSDAttachKey
    @abstract kIONetworkNoBSDAttachKey is a property of IONetworkInterface
        objects. It has an OSBoolean value.
    @discussion Adding a property with this key and the value kOSBooleanTrue
        before the interface is published will hold off the BSD attach.
        When the interface is ready to attach to BSD, remove the property
        and then re-publish the interface by calling registerService().
*/

#define kIONetworkNoBSDAttachKey  "IONetworkNoBSDAttach"

/*! @enum InterfaceObjectStates
    @discussion Constants used to encode the state of the interface object.
   @constant kIONetworkInterfaceRegisteredState The interface object has
        registered with the data link layer.
    @constant kIONetworkInterfaceOpenedState One or more clients have an
        open on the interface object.
    @constant kIONetworkInterfaceDisabledState The interface is temporarily
        unable to service its clients. This will occur when the network
        controller that is servicing the interface has entered a low power
        state that renders it unusable. 
*/

enum {
    kIONetworkInterfaceRegisteredState  = 0x1,
    kIONetworkInterfaceOpenedState      = 0x2,
    kIONetworkInterfaceDisabledState    = 0x4
};

#ifdef KERNEL
#ifdef __cplusplus

#include <IOKit/IOService.h>
#include <IOKit/network/IONetworkData.h>
#include <IOKit/network/IONetworkStats.h>
#include <IOKit/network/IONetworkMedium.h>
#include <net/kpi_interface.h>

class  IONetworkController;
class  IONetworkStack;
class  IOCommandGate;
struct IOMbufQueue;

/*! @typedef IOOutputAction
    @discussion Prototype for an output packet handler that will process
    all outbound packets sent to the interface from the data link layer.
    An output handler is registered with the interface by calling
    registerOutputHandler().
    @param mbuf_t A packet mbuf.
    @param param A parameter for the output request. */

typedef UInt32 (OSObject::*IOOutputAction)(mbuf_t, void * param);

/*! @typedef BPF_FUNC
    @discussion Prototype for the BPF tap handler. This will disappear
    when the correct DLIL header file is included. */

typedef int (*BPF_FUNC)(struct ifnet *, struct mbuf *);

// Network event types recognized by inputEvent().
//
enum {
    /* DLIL defined event, argument must be a pointer to a
       kern_event_msg structure. */
    kIONetworkEventTypeDLIL                 = 0xff000001,

    /* Link up event, no argument */
    kIONetworkEventTypeLinkUp               = 0xff000002,

    /* Link down event, no argument */
    kIONetworkEventTypeLinkDown             = 0xff000003,

    /* Wake on LAN support changed, no argument */
    kIONetworkEventWakeOnLANSupportChanged  = 0xff000004,
    
    /* Link speed changed */
    kIONetworkEventTypeLinkSpeedChange      = 0xff000005
};


/*! @class IONetworkInterface
    @abstract Abstract class that manages the connection between an
    IONetworkController and the data link interface layer.
    @discussion An IONetworkInterface object manages the connection between
    an IONetworkController and the data link interface layer (DLIL).
    All interactions between the controller and DLIL must go through an
    interface object. Any data structures that are required by DLIL for a
    particular interface type shall be allocated and mantained by the
    interface object. IONetworkInterface is an abstract class that must be
    extended by a concrete subclass to specialize for a particular network
    type.

    Although most drivers will allocate a single interface object,
    it is possible for multiple interfaces to be attached to a single
    controller. This controller driver will be responsible for arbitrating
    access among its multiple interface clients.
    
    IONetworkInterface also maintains a dictionary of IONetworkData
    objects containing statistics structures. Controller drivers can
    ask for a particular data object by name and update the
    statistics counters within directly. This dictionary is added to
    the interface's property table and is visible outside of the kernel. 
*/


class __exported IONetworkInterface : public IOService
{
    OSDeclareAbstractStructors( IONetworkInterface )

    friend class IONetworkStack;


private:
    IONetworkController *   _driver;
    ifnet_t                 _backingIfnet;
    IOLock *                _privateLock;
    OSSet *                 _clientSet;
    OSNumber *              _stateBits;
    bpf_packet_func         _inputFilterFunc;
    bpf_packet_func         _outputFilterFunc;
    OSObject *              _outTarget;
    IOOutputAction          _outAction;
    UInt32                  _clientVar[4];
    OSDictionary *          _dataDict;
    IOMbufQueue *           _inputPushQueue;
    void *                  _unused1;
    UInt32                  _unused2;

    struct ExpansionData {
        int                         unit;
        int                         type;
        int                         mtu;
        int                         flags;
        int                         eflags;
        int                         addrlen;
        int                         hdrlen;
        int32_t                     loggingLevel;
        uint32_t                    outputQueueModel;
        IONetworkStats              driverStats;
        IONetworkStats              lastDriverStats;
        ifnet_stat_increment_param  inputDeltas;
        IORecursiveLock *           publicLock;
        char *                      remote_NMI_pattern;
        unsigned int                remote_NMI_len;
        IONetworkController *       controller;
        uint32_t                    configFlags;
        uint32_t                    txRingSize;
        uint32_t                    txPullOptions;
        uint32_t                    txQueueSize;
        uint32_t                    txSchedulingModel;
        uint32_t                    txTargetQdelay;
        uint32_t                    txThreadState;
        volatile UInt32             txThreadFlags;
        uint32_t                    txThreadSignal;
        uint32_t                    txThreadSignalLast;
        thread_t                    txStartThread;
        void *                      txStartAction;
        IOWorkLoop *                txWorkLoop;
        uint32_t                    rxRingSize;
        uint32_t                    rxPollOptions;
        uint32_t                    rxPollModel;
        void *                      rxPollAction;
        void *                      rxCtlAction;        
        uint64_t                    rxPollEmpty;
        uint64_t                    rxPollTotal;
        uint16_t                    txStartDelayQueueLength;	/* optional */
        uint16_t                    txStartDelayTimeout;        /* optional */
        IOOptionBits                clientBehavior;
        thread_call_t               inputEventThreadCall; // inputEvent() thread call
    };

    ExpansionData *         _reserved;

    bool            _syncNetworkDataDict();
    SInt32          syncSIOCSIFMEDIA(IONetworkController * ctr, struct ifreq * ifr);
    SInt32          syncSIOCGIFMEDIA(IONetworkController * ctr, struct ifreq * ifr,
                                unsigned long cmd);
    SInt32          syncSIOCSIFMTU(IONetworkController * ctr, struct ifreq * ifr);
    void            drainOutputQueue(ifnet_t ifp, IONetworkController * driver);
    IOReturn        haltOutputThread(uint32_t stateBit);
    void            pushInputQueue( IOMbufQueue * queue );
    void            pushInputPacket( mbuf_t packet, uint32_t length );
    int             if_start_precheck( ifnet_t ifp );
    static int      performGatedCommand(void *, void *, void *, void *, void *);
    static void     powerChangeHandler(void *, void *, void *);
    static errno_t  if_ioctl(ifnet_t ifp, unsigned long cmd, void * data);
    static int      if_output(ifnet_t ifp, mbuf_t);
    static errno_t  if_set_bpf_tap(ifnet_t ifp, bpf_tap_mode, bpf_packet_func);
	static void     if_detach(ifnet_t ifp);
    static void     if_start(ifnet_t ifp);
    static void     if_start_gated(ifnet_t ifp);
    static void     if_input_poll(ifnet_t ifp, uint32_t flags,
                                uint32_t max_count,
                                mbuf_t * first_packet, mbuf_t * last_packet,
                                uint32_t *  cnt, uint32_t * len);
    static void     if_input_poll_gated(ifnet_t ifp, uint32_t flags,
                                uint32_t max_count,
                                mbuf_t * first_packet, mbuf_t * last_packet,
                                uint32_t *  cnt, uint32_t * len);
    void            notifyDriver( uint32_t type, void * data );
    static void     handleNetworkInputEvent(thread_call_param_t param0, thread_call_param_t param1);

public:

/*! @function isPrimaryInterface
    @abstract Queries whether the interface is the primary network interface
    on the system.
    @discussion The definition of a primary interface and its discovery is
    platform specific.
    @result Returns true if the interface is the primary interface,
    false otherwise.
*/
    virtual bool     isPrimaryInterface( void ) const;

/*! @function init
    @abstract Initializes the <code>IONetworkInterface</code> object.
    @discussion Resources are allocated, but an <code>ifnet_t</code> will not
    be allocated until the interface is assigned a BSD name and attached to the
    network stack.
    @param controller A network controller object that will service the
    the interface.
    @result Returns true on success, false otherwise.
*/
    virtual bool     init( IONetworkController * controller );

/*! @function isRegistered
    @abstract Queries if the interface has attached to the BSD network stack.
    @discussion Once attached a <code>kIOBSDNameKey</code> property is added
    to the interface object with the assigned BSD name.
    @result Returns true if interface is registered and attached to the network
    stack, false otherwise.
*/
    virtual bool     isRegistered( void ) const;

/*! @function getInterfaceState
    @abstract Reports the current state of the interface object.
    @result Returns the current interface state flags.
*/
    virtual UInt32   getInterfaceState( void ) const;

/*! @function getController
    @abstract Gets the <code>IONetworkController</code> object that created
    this interface.
    @discussion The controller object passed to init() will be retained until
    the interface closes the controller. Subclasses can safely call this method
    before the controller is closed.
    @result Returns the parent <code>IONetworkController</code> object.
*/
    virtual IONetworkController * getController( void ) const;

/*! @function inputPacket
    @abstract For drivers to submit a received packet to the network stack.
    @discussion The packet provided to this method may be added to an input
    queue managed by the interface object, which the driver can use to postpone
    the packet handoff to the network stack, until all received packets have been
    added to the input queue. A subsequent call to <code>flushInputQueue()</code>,
    will transfer the entire contents of the input queue to the network stack.
    This input queue is not protected by a lock. Drivers that leverage this
    input queue must either access the queue from a single thread, or enforce
    serialized access.
    @param mbuf_t The mbuf containing the received packet.
    @param length Specify the size of the received packet in the mbuf.
    The mbuf length fields are updated with this value. If zero, then the mbuf
    length fields are not updated.
    @param options Pass <code>kInputOptionQueuePacket</code> to enqueue the
    input packet. Pass zero to bypass the input queue, and immediately submit
    the packet to the network stack.
    @param param A parameter provided by the driver. Not used.
    @result Returns the number of packets that were submitted to the network
    stack, or zero if the packet was enqueued.
*/
    virtual UInt32   inputPacket(
                        mbuf_t          packet,
                        UInt32          length  = 0,
                        IOOptionBits    options = 0,
                        void *          param   = 0 );

/*! @enum InputOptionQueuePacket
    @discussion Options for <code>inputPacket()</code>.
    @constant kInputOptionQueuePacket Enqueue the input packet provided to the
    input packet queue. Calls to <code>inputPacket()</code> must be serialized.
*/
    enum {
        kInputOptionQueuePacket = 0x1
    };

/*! @function flushInputQueue
    @abstract Submit all packets held in the input queue to the network stack.
    @discussion Allow drivers to remove all packets from the input queue and
    submit them to the network stack. This method should be used in concert
    with the <code>inputPacket()</code> method, to flush the input queue after
    queueing a number of received packets.
    @result Returns the number of packets that were submitted to the network
    stack. Returns zero if the queue was empty.
*/
    virtual UInt32   flushInputQueue( void );

/*! @function clearInputQueue
    @abstract Discards all packets in the input queue.
    @discussion This method removes all packets from the input queue and
    releases them back to the free mbuf pool. It is unusual for a driver to
    call this method.
    @result Returns the number of packets freed.
*/
    virtual UInt32   clearInputQueue( void );

/*! @function inputEvent
    @abstract Sends an event to the network stack.
    @discussion This method can be used by the driver to send an event to the
    network stack.
    @param type A constant describing the event type.
    @param data An optional data associated with the event.
    @result Returns true if the event was delivered, false if the event type
    specified is invalid, or if the event delivery failed.
*/
    virtual bool     inputEvent( UInt32 type, void * data );

/*! @function registerOutputHandler
    @abstract Registers a target/action to handle outbound packets.
    @discussion The interface object will forward all output packets sent from
    the network stack to the target and action registered using this method.
    The registration must occur before the interface is registered and opened
    by <code>IONetworkStack</code>, otherwise the default handler will be used.
    The default target and action is set by <code>init()</code> as the
    controller, and the handler returned by the controller's
    <code>getOutputHandler()</code> method.
    @param target Object that implements the output handler.
    @param action The function that will handle output packets.
    @result Returns true if the target/action provided was accepted,
    false otherwise.
*/
    virtual bool     registerOutputHandler(
                            OSObject *      target,
                            IOOutputAction  action );

/*! @function getNamePrefix
    @abstract Returns the BSD name prefix as a C-string.
    @discussion The BSD name for each interface object is generated by
    concatenating the string returned by this method, along with an unit number
    assigned by <code>IONetworkStack</code>. A concrete interface subclass must
    implement this method and return a distinct name prefix for its instances.
    @result Returns a pointer to a constant C-string.
*/
    virtual const char * getNamePrefix() const = 0;

/*! @function getInterfaceType
    @abstract Gets the interface type.
    @discussion This method returns the interface type previously set by
    <code>setInterfaceType</code>.
    @result Returns a constant defined in <code>bsd/net/if_types.h</code>
    that describes the interface type.
*/
    virtual UInt8    getInterfaceType( void ) const;

/*! @function getMaxTransferUnit
    @abstract Gets the maximum transfer unit for this interface.
    @discussion This method calls <code>ifnet_mtu</code> and returns the
    maximum transfer unit.
    @result Returns the interface MTU size in bytes.
*/
    virtual UInt32   getMaxTransferUnit( void ) const;

/*! @function getFlags
    @abstract Gets the current interface flags.
    @discussion This method calls <code>ifnet_flags</code> and returns the
    current interface flags.
    @result Returns the interface flags.
*/
    virtual UInt16   getFlags( void ) const;

/*! @function getExtraFlags
    @abstract Gets the current interface eflags.
    @discussion This method calls <code>ifnet_eflags</code> and returns the
    current interface eflags.
    @result Returns the value of the interface eflags.
*/
    virtual UInt32   getExtraFlags( void ) const;

/*! @function getMediaAddressLength
    @abstract Gets the size of the media (MAC-layer) address.
    @discussion This method calls <code>ifnet_addrlen</code> and returns the
    media address length.
    @result Returns the size of the media address in bytes.
*/
    virtual UInt8    getMediaAddressLength( void ) const;

/*! @function getMediaHeaderLength
    @abstract Gets the size of the media header.
    @discussion This method calls <code>ifnet_hdrlen</code> and returns the
    media header length.
    @result Returns the size of the media header in bytes.
*/
    virtual UInt8    getMediaHeaderLength( void ) const;

/*! @function getUnitNumber
    @abstract Gets the unit number assigned to this interface object.
    @discussion This method calls <code>ifnet_unit</code> and returns the
    unit number assigned by <code>IONetworkStack</code>.
    @result Returns the assigned interface unit number.
*/
    virtual UInt16   getUnitNumber( void ) const;

/*! @function addNetworkData
    @abstract Adds an <code>IONetworkData</code> object to the interface.
    @discussion The <code>IONetworkData</code> object is added to a
    collection using the key from <code>IONetworkData::getKey()<code>.
    The object provided is retained.
    @param aData The <code>IONetworkData</code> object.
    @result Returns true if the object was added, false otherwise.
*/
    virtual bool     addNetworkData( IONetworkData * aData );

/*! @function removeNetworkData
    @abstract Removes an <code>IONetworkData</code> object from the interface.
    @discussion This method removes an <code>IONetworkData</code> object from
    the collection managed by the interface. The object removed is released.
    @param aKey An OSSymbol identifying the object to be removed.
    @result Returns true if the object was found and removed, false otherwise.
*/
    virtual bool     removeNetworkData( const OSSymbol * aKey );

/*! @function removeNetworkData
    @abstract Removes an <code>IONetworkData</code> object from the interface.
    @discussion This method removes an <code>IONetworkData</code> object from
    the collection managed by the interface. The object removed is released.
    @param aKey A C-string identifying the object to be removed.
    @result Returns true if the object was found and removed, false otherwise.
*/
    virtual bool     removeNetworkData( const char * aKey );

/*! @function getNetworkData
    @abstract Gets an <code>IONetworkData</code> object from the interface.
    @discussion Returns a reference to an <code>IONetworkData</code> object
    that was previously added to the interface, and is associated with the
    provided key.
    @param aKey A C-string identifying the object.
    @result Returns a reference to the matching <code>IONetworkData</code>
    object, or NULL if no match was found.
*/
    virtual IONetworkData * getNetworkData( const char * aKey ) const;

/*! @function getNetworkData
    @abstract Gets an <code>IONetworkData</code> object from the interface.
    @discussion Returns a reference to an <code>IONetworkData</code> object
    that was previously added to the interface, and is associated with the
    provided key.
    @param aKey An OSSymbol identifying the object.
    @result Returns a reference to the matching <code>IONetworkData</code>
    object, or NULL if no match was found.
*/
    virtual IONetworkData * getNetworkData(const OSSymbol * aKey) const;

    /* Compatibility methods */
    IONetworkData * getParameter(const char * aKey) const;
	bool setExtendedFlags(UInt32 flags, UInt32 clear = 0);

    /* Override IOService::message() */
    virtual IOReturn message( UInt32 type, IOService * provider, void * argument ) APPLE_KEXT_OVERRIDE;

/*! @function debuggerRegistered
    @abstract Tells the <code>IONetworkData</code> that this interface will be
    used by the debugger.
*/
    void debuggerRegistered( void );

protected:
/*! @function setInterfaceType
    @abstract Sets the interface type.
    @discussion Sets the interface type before the interface is attached to
    the network stack. See <code>bsd/net/if_types.h</code> for defined types.
    The <code>kIOInterfaceType</code> is also updated using the provided type.
    @param type A constant defined in <code>bsd/net/if_types.h</code> that
    describes the interface type.
    @result Returns true to indicate success if the interface has not yet
    attached to the network stack, otherwise returns false.
*/
    virtual bool     setInterfaceType( UInt8 type );

/*! @function setMaxTransferUnit
    @abstract Sets the maximum transfer unit for this interface.
    @discussion Calls <code>ifnet_set_mtu</code> if the interface is attached
    to the network stack, and updates the <code>kIOMaxTransferUnit</code>
    property using the provided value.
    @param mtu The interface MTU size in bytes.
    @result Always returns true.
*/
    virtual bool     setMaxTransferUnit( UInt32 mtu );

/*! @function setFlags
    @abstract Performs a read-modify-write operation on the current
    interface flags value.
    @discussion Calls <code>ifnet_set_flags</code> if the interface is attached
    to the network stack, and updates the <code>kIOInterfaceFlags</code>
    property using the provided value. See <code>bsd/net/if.h</code> header
    file for the flag constants.
    @param flags The bits that should be set.
    @param clear The bits that should be cleared. If zero, then non
    of the flags are cleared and the result is formed by OR'ing the
    original flags value with the new flags.
    @result Always returns true.
*/
    virtual bool     setFlags( UInt16 flags, UInt16 clear = 0 );

    /* Deprecated. No replacement. */
    virtual bool     setExtraFlags( UInt32 flags, UInt32 clear = 0 );

/*! @function setMediaAddressLength
    @abstract Sets the size of the media (MAC-layer) address.
    @discussion Calls <code>ifnet_set_addrlen</code> if interface is attached
    to the network stack, and updates the <code>kIOMediaAddressLength</code>
    property using the provided value.
    @param length The size of the media address in bytes.
    @result Always returns true.
*/
    virtual bool     setMediaAddressLength( UInt8 length );

/*! @function setMediaHeaderLength
    @abstract Sets the size of the media header.
    @discussion Calls <code>ifnet_set_hdrlen</code> if interface is attached
    to the network stack, and updates the <code>kIOMediaHeaderLength</code>
    property using the provided value.
    @param length The size of the media header in bytes.
    @result Always returns true.
*/
    virtual bool     setMediaHeaderLength( UInt8 length );

/*! @function setUnitNumber
    @abstract Assigns an unique unit number to this interface.
    @discussion This method is called internally before the interface is
    attached to the network stack, to assign an unique unit number to the
    interface object. The <code>kIOInterfaceUnit</code> property is also
    updated using the provided value.
    @param unit The unit number assigned to this interface object.
    @result Returns true to indicate success if the interface has not yet
    attached to the network stack, otherwise returns false.
*/
    virtual bool     setUnitNumber( UInt16 unit );

/*! @function free
    @abstract Frees the <code>IONetworkInterface</code> object.
    @discussion Resource allocated by init() are released, and
    clearInputQueue() is called to ensure that the input queue is empty.
    The interface should have already detached from the network stack.
*/
    virtual void     free( void ) APPLE_KEXT_OVERRIDE;

    /* Override IOService::handleOpen() */
    virtual bool     handleOpen( IOService *  client,
                                 IOOptionBits options,
                                 void *       argument ) APPLE_KEXT_OVERRIDE;

    /* Override IOService::handleClose() */
    virtual void     handleClose( IOService * client, IOOptionBits options ) APPLE_KEXT_OVERRIDE;

    /* Override IOService::handleIsOpen() */
    virtual bool     handleIsOpen( const IOService * client ) const APPLE_KEXT_OVERRIDE;

/*! @function lock
    @abstract Acquires a recursive lock owned by the interface.
    @discussion A recursive lock is allocated and initialized in
    <code>init()</code>. This lock is otherwise not used by the
    <code>IONetworkInterface</code> class. This method call acquires
    the lock and must be balanced with an <code>unlock()</code>.
*/
    virtual void     lock( void );

/*! @function unlock
    @abstract Releases the recursive lock owned by the interface.
    @discussion A recursive lock is allocated and initialized in
    <code>init()</code>. This lock is otherwise not used by the
    <code>IONetworkInterface</code> class. This method call releases
    the lock to balance a prior <code>lock()</code>.
*/
    virtual void     unlock( void );

/*! @function controllerDidOpen
    @abstract Sends a notification that the interface has opened the network
    controller.
    @discussion This method is called by <code>handleOpen()</code> to notify
    subclasses that the controller was opened. The open on the controller
    occurs when the interface receives the initial open request from a client.
    Subclasses can override this method and inspect the controller before
    allowing the client open. This method is called with the arbitration lock
    held, hence issuing I/O to the controller must be avoided to eliminate the
    possibility of a deadlock.
    @param controller The controller that was opened.
    @result Must return true in order for handleOpen() to accept the client
    open. If the return is false, then the controller will be closed and the
    client open will fail.
*/
    virtual bool     controllerDidOpen( IONetworkController * controller );

/*! @function controllerWillClose
    @abstract Sends a notification that the interface will close the network
    controller.
    @discussion This method is called by <code>handleClose()</code> after
    receiving a close from the last interface client, and just before the
    controller is closed. Subclasses can override this method to perform any
    cleanup action before the controller is closed. This method is called with
    the arbitration lock held, hence issuing I/O to the controller should be
    avoided to eliminate the possibility of a deadlock.
    @param controller The controller that is about to be closed.
*/
    virtual void     controllerWillClose( IONetworkController * controller );

/*! @function performCommand
    @abstract Handles an ioctl command sent to the network interface.
    @discussion This method handles socket ioctl commands sent to the
    network interface from DLIL.
    IONetworkInterface handles commands that are common for all network
    interface types. A subclass of IONetworkInterface may override this
    method to override the command handling in IONetworkInterface, or
    to extend the command processing to handle additional commands.
    The ioctl commands handled by IONetworkInterface are
        <code>SIOCGIFMTU</code> (Get interface MTU size),
        <code>SIOCSIFMTU</code> (Set interface MTU size),
        <code>SIOCSIFMEDIA</code> (Set media), and
        <code>SIOCGIFMEDIA</code> (Get media and link status).
    @param controller The controller object.
    @param cmd The ioctl command code.
    @param arg0 Command argument 0. Generally a pointer to an ifnet structure
        associated with the interface.
    @param arg1 Command argument 1.
    @result Returns a BSD return value defined in <code>bsd/sys/errno.h</code>.
*/
    virtual SInt32   performCommand(
                                IONetworkController * controller,
                                unsigned long         cmd,
                                void *                arg0,
                                void *                arg1 );

public:

/*! @function getIfnet
    @abstract Returns the <code>ifnet_t</code> allocated by the interface object.
    @discussion Gets the interface's <code>ifnet_t</code>, which is managed
    primarily by <code>IONetworkInterface</code>, however subclasses or drivers
    can use this method to obtain a reference to the <code>ifnet_t</code> for
    interface KPI calls.
    @result Returns the <code>ifnet_t</code> after the interface has attached
    to the network stack and before the interface is detached, otherwise returns
    NULL.
*/
    virtual ifnet_t  getIfnet( void ) const;

protected:

    /* Deprecated. Use <code>initIfnetParams</code> instead. */
    virtual bool     initIfnet( struct ifnet * ifp );

/*! @function handleClientOpen
    @abstract Handles a client open on the interface.
    @discussion This method is called by <code>handleOpen()</code> to allow a
    subclass to handle a client close. The arbitration lock is held.
    @param client The client object requesting the open.
    @param options Options passed to <code>handleOpen()</code>.
    @param argument Argument passed to <code>handleOpen()</code>.
    @result Returns true to accept the client open, false to reject the open.
*/
    virtual bool     handleClientOpen( IOService *  client,
                                       IOOptionBits options,
                                       void *       argument );

/*! @function handleClientClose
    @abstract Handles a client close on the interface.
    @discussion This method is called by <code>handleClose()</code> to allow a
    subclass to handle a client close. The arbitration lock is held.
    @param client The client object requesting the close.
    @param options Options same options passed to <code>handleClose()</code>.
*/
    virtual void     handleClientClose( IOService *  client,
                                        IOOptionBits options );

    /* Override IOService::newUserClient() */
    virtual IOReturn newUserClient( task_t           owningTask,
                                    void *           security_id,
                                    UInt32           type,
                                    IOUserClient **  handler ) APPLE_KEXT_OVERRIDE;

/*! @function setInterfaceState
    @abstract Updates the interface object state flags.
    @discussion The flags reflect the current state of the interface, and is
    also published through the <code>kIOInterfaceState</code> property.
    @param set The flags that should be set.
    @param clear The flags that should be cleared.
    @result Returns the new interface state flags.
*/
    virtual UInt32   setInterfaceState( UInt32 set, UInt32 clear = 0 );

/*! @function powerStateWillChangeTo
    @abstract Handles a pre-change power interest notification from the
    network controller.
    @discussion The <code>controllerWillChangePowerState()</code> method is
    called by this handler. Subclasses are not expected to override this method.
    @param flags Flags that describe the capability of the controller in the
    new power state.
    @param stateNumber An index to a state in the network controller's
    power state array that the controller is switching to.
    @param policyMaker A reference to the network controller's policy-maker,
    and is also the originator of this notification.
    @result Returns <code>IOPMAckImplied</code> to indicate synchronous completion.
*/
    virtual IOReturn powerStateWillChangeTo(
                                IOPMPowerFlags  flags,
                                unsigned long   stateNumber,
                                IOService *     policyMaker ) APPLE_KEXT_OVERRIDE;

/*! @function powerStateDidChangeTo
    @abstract Handles a post-change power interest notification from the
    network controller.
    @discussion The <code>controllerDidChangePowerState()</code> method is
    called by this handler. Subclasses are not expected to override this method.
    @param flags Flags that describe the capability of the controller in the
    new power state.
    @param stateNumber An index to a state in the network controller's
    power state array that the controller has switched to.
    @param policyMaker A reference to the network controller's policy-maker,
    and is also the originator of this notification.
    @result Returns <code>IOPMAckImplied</code> to indicate synchronous completion.
*/
    virtual IOReturn powerStateDidChangeTo(
                                IOPMPowerFlags  flags,
                                unsigned long   stateNumber,
                                IOService *     policyMaker ) APPLE_KEXT_OVERRIDE;

/*! @function controllerWillChangePowerState
    @abstract Handles a notification that the network controller servicing
    this interface object will transition to a new power state.
    @param controller The network controller object.
    @param flags Flags that describe the capability of the controller in the
    new power state.
    @param stateNumber An index to a state in the network controller's
    power state array that the controller is switching to.
    @param policyMaker A reference to the network controller's policy-maker,
    and is also the originator of this notification.
    @result The return value is always <code>kIOReturnSuccess</code>.
*/
    virtual IOReturn controllerWillChangePowerState(
                                IONetworkController * controller,
                                IOPMPowerFlags        flags,
                                UInt32                stateNumber,
                                IOService *           policyMaker );

/*! @function controllerDidChangePowerState
    @abstract Handles a notification that the network controller servicing
    this interface object has transitioned to a new power state.
    @param controller The network controller object.
    @param flags Flags that describe the capability of the controller in the
    new power state.
    @param stateNumber An index to a state in the network controller's
    power state array that the controller has switched to.
    @param policyMaker A reference to the network controller's policy-maker,
    and is also the originator of this notification.
    @result The return value is always <code>kIOReturnSuccess</code>.
*/
    virtual IOReturn controllerDidChangePowerState(
                                IONetworkController * controller,
                                IOPMPowerFlags        flags,
                                UInt32                stateNumber,
                                IOService *           policyMaker );

public:
    /* Override IOService::willTerminate() */
    virtual bool     willTerminate(
                                IOService *  provider,
                                IOOptionBits options ) APPLE_KEXT_OVERRIDE;

    /* Override IOService::requestTerminate() */
    virtual bool     requestTerminate(
                                IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;

    /* Override IOService::serializeProperties() */
    virtual bool     serializeProperties( OSSerialize * s ) const APPLE_KEXT_OVERRIDE;

/*! @function attachToDataLinkLayer
    @abstract Attach the network interface to the BSD data link layer.
    @discussion This method is called internally to attach the network
    interface to the BSD data link layer, after an unit number has been
    assigned. The calling context is not synchronized against the driver's
    work loop. Subclasses may override this method to perform additional
    setup before the network stack attach. The <code>getIfnet()</code> method
    will return the BSD interface being attached.
    @param options Options for the attach call. None are currently defined.
    @param parameter Parameter for the attach call. Not currently used.
    @result Returns <code>kIOReturnSuccess</code> on success.
*/
    virtual IOReturn attachToDataLinkLayer( IOOptionBits options,
                                            void *       parameter );

    OSMetaClassDeclareReservedUsed(IONetworkInterface, 0);

/*! @function detachFromDataLinkLayer
    @abstract Detach the network interface from the BSD data link layer.
    @discussion This method is called internally to detach the network
    interface from the BSD data link layer, after the interface has been
    terminated and before the last client close. This method will block until
    the detach operation is complete. The calling context is not synchronized
    against the driver's work loop. Subclasses may override this method to
    perform additional cleanup before or after detaching from the network
    stack. The <code>getIfnet()</code> method will return NULL after detach.
    @param options Options for the detach call. None are currently defined.
    @param parameter Parameter for the detach call. Not currently used.
*/
    virtual void     detachFromDataLinkLayer( IOOptionBits options,
                                              void *       parameter );

    OSMetaClassDeclareReservedUsed(IONetworkInterface, 1);

protected:
/*! @function feedPacketInputTap
    @abstract Feed received packets to the BPF
    @discussion This function is called internally to send input packets to
    the BPF input tap when it is enabled. Subclasses are not expected to
    override this method.
    @param mbuf_t Pointer to the input packet.
*/
    virtual void     feedPacketInputTap( mbuf_t );

	OSMetaClassDeclareReservedUsed(IONetworkInterface, 2);

/*! @function feedPacketOutputTap
    @abstract Feed output packets to the BPF
    @discussion This function is called internally to send output packets to
    the BPF output tap when it is enabled. Subclasses are not expected to
    override this method.
    @param mbuf_t Pointer to the output packet.
*/
	virtual void     feedPacketOutputTap( mbuf_t );

	OSMetaClassDeclareReservedUsed(IONetworkInterface, 3);

/*! @function initIfnetParams
    @abstract Allows a subclass to provide ifnet initialization parameters
    specific to an interface type.
    @discussion This method initializes the parameters that are common to all
    network interfaces. An interface subclass is expected to override this
    method, call the superclass implementation first, then initialize the
    parameters specific to that interface type. This method is called after
    an unit number has been assigned to the interface, and just before the
    interface is attached to BSD.
    @param params Pointer to an <code>ifnet_init_params</code> allocated by
    the caller.
    @result Returns <code>true</code> on success, <code>false</code> otherwise.
*/
	virtual bool     initIfnetParams( struct ifnet_init_params * params );

    OSMetaClassDeclareReservedUsed(IONetworkInterface, 4);
    
/*! @function configureOutputStartDelay
    @abstract Configure the output start delay
    @discussion This optional routine, if used, needs to be called after 
    IONetworkInterface::init() and before IONetworkInterface::attachToDataLinkLayer().
    This allows for over-riding ifnet_init_eparams.start_delay_qlen and 
    ifnet_init_eparams.start_delay_timeout.
    @param outputStartDelayQueueLength, maps to ifnet_init_eparams.start_delay_qlen
    @param outputStartDelayTimeout, maps to ifnet_init_eparams.start_delay_timeout
    @result <code>kIOReturnSuccess</code> if interface was successfully
    configured.
 */
    IOReturn configureOutputStartDelay( uint16_t outputStartDelayQueueLength,
                                        uint16_t outputStartDelayTimeout );

public:
    OSMetaClassDeclareReservedUnused( IONetworkInterface,  5);
    OSMetaClassDeclareReservedUnused( IONetworkInterface,  6);
    OSMetaClassDeclareReservedUnused( IONetworkInterface,  7);
    OSMetaClassDeclareReservedUnused( IONetworkInterface,  8);
    OSMetaClassDeclareReservedUnused( IONetworkInterface,  9);
    OSMetaClassDeclareReservedUnused( IONetworkInterface, 10);
    OSMetaClassDeclareReservedUnused( IONetworkInterface, 11);
    OSMetaClassDeclareReservedUnused( IONetworkInterface, 12);
    OSMetaClassDeclareReservedUnused( IONetworkInterface, 13);
    OSMetaClassDeclareReservedUnused( IONetworkInterface, 14);
    OSMetaClassDeclareReservedUnused( IONetworkInterface, 15);
};

#endif /* __cplusplus */
#endif /* KERNEL */
#endif /* !_IONETWORKINTERFACE_H */
