/*
 * @APPLE_LICENSE_HEADER_START@
 *
 * Copyright (c) 1999-2009 Apple Computer, Inc.  All Rights Reserved.
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
/* 	Copyright (c) 1992 NeXT Computer, Inc.  All rights reserved.
 *
 * EventDriver.h - Exported Interface Event Driver object.
 *
 *		The EventDriver is a pseudo-device driver.
 *
 * HISTORY
 * 19 Mar 1992    Mike Paquette at NeXT
 *      Created.
 * 4  Aug 1993	  Erik Kay at NeXT
 *	API cleanup
 */

#ifndef	_IOHIDSYSTEM_H
#define _IOHIDSYSTEM_H

#include <TargetConditionals.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOInterruptEventSource.h>
#include <IOKit/IOService.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOUserClient.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/pwr_mgt/IOPM.h>
#include "IOHIDWorkLoop.h"
#if TARGET_OS_IPHONE
class IOGraphicsDevice;
#else
#include <IOKit/graphics/IOGraphicsDevice.h>
#endif
#include <IOKit/hidsystem/IOHIDevice.h>
#include <IOKit/hidsystem/IOHIDShared.h>
#include <IOKit/hidsystem/IOHIDTypes.h>
#include <IOKit/hidsystem/IOLLEvent.h>
#include <IOKit/IOSharedDataQueue.h>
#include <IOKit/hidsystem/ev_keymap.h>		/* For NX_NUM_SCANNED_SPECIALKEYS */

// The following messages should be unique across the entire system
#ifndef sub_iokit_hidsystem
#define sub_iokit_hidsystem                     err_sub(14)
#endif

#define kIOHIDSystem508MouseClickMessage        iokit_family_msg(sub_iokit_hidsystem, 1)
#define kIOHIDSystemDeviceSeizeRequestMessage	iokit_family_msg(sub_iokit_hidsystem, 2)
#define kIOHIDSystem508SpecialKeyDownMessage    iokit_family_msg(sub_iokit_hidsystem, 3)
// 4 is used by IOHIDSecurePrompt
#define kIOHIDSystemActivityTickle              iokit_family_msg(sub_iokit_hidsystem, 5)
#define kIOHIDSystemUserHidActivity             iokit_family_msg(sub_iokit_hidsystem, 6)

class IOHistogramReporter;
class IOHIDKeyboardDevice;
class IOHIDPointingDevice;
class IOHIDEvent;
class IOFixedPoint64;
struct _evScreen;

typedef volatile _evScreen EVScreen;

#if defined(KERNEL) && !defined(KERNEL_PRIVATE)
class __deprecated_msg("Use DriverKit") IOHIDSystem : public IOService
#else
class IOHIDSystem : public IOService
#endif
{
	OSDeclareDefaultStructors(IOHIDSystem);

	friend class IOHIDUserClient;
	friend class IOHIDParamUserClient;
	friend class IOHIDEventSystemUserClient;

private:
	IOHIDWorkLoop           *workLoop;
    IOInterruptEventSource  *keyboardEQES;
    IOCommandGate           *cmdGate;
    IONotifier              *publishNotify;
	void                    *eventMsg;	// Msg to be sent to Window Server.
	void                    *stackShotMsg;	// Msg to be sent to Stack Shot.

	// Shared memory area information
    IOBufferMemoryDescriptor * globalMemory;
	uintptr_t	shmem_addr;	// kernel address of shared memory
	vm_size_t	shmem_size;	// size of shared memory

	// Pointers to structures which occupy the shared memory area.
	volatile EvGlobals  *evg;	// Pointer to EvGlobals (shmem)
	// Internal variables related to the shared memory area
	int                 lleqSize;	// # of entries in low-level queue
                        // FIXME: why is this ivar lleqSize an ivar? {Dan]

	// Screens list
	EVScreen * evScreen;	// array of screens known to driver
	int             screens;	// running total of allocated screens
	UInt32          cursorScreens;	// bit mask of screens with cursor present
    UInt32          cursorPinScreen;// a screen to pin against
	IOGBounds		cursorPin;	// Range to which cursor is pinned
					// while on this screen.
	IOGBounds		workSpace;	// IOGBounds of full workspace.
	// Event Status state - This includes things like event timestamps,
	// time til screen dim, and related things manipulated through the
	// Event Status API.
	//
    struct ExpansionData;
    ExpansionData   *_privateData;

    IOGPoint	clickLoc;       // location of last mouse click
    IOGPoint    clickSpaceThresh;// max mouse delta to be a doubleclick

//	int	clickState;	// Current click state

	bool evOpenCalled;	// Has the driver been opened?
	bool evInitialized;	// Has the first-open-only initialization run?
	bool eventsOpen;	// Boolean: has evmmap been called yet?
	bool cursorStarted;	// periodic events running?
	bool cursorEnabled;	// cursor positioning ok?
	bool cursorCoupled;	// cursor positioning on pointer moves ok?
	bool cursorPinned;	// cursor positioning on pointer moves ok?

    uint64_t lastUndimEvent;

    struct SetFixedMouseLocData {
        uint64_t    origTs;
        uint64_t    callTs;
        int32_t     x;
        int32_t     y;
        int32_t     pid;
    } __attribute__((packed));

    struct Diags {
        /* Cursor actions slower than this are tracked for diagnostics */
        static const uint32_t SLOW_CURSOR_MS = 20;
        
        typedef enum {
            kCursorActionShow = 0,
            kCursorActionHide,
            kCursorActionMove,

            kCursorActionCount
        } CursorAction;

        static const char * cursorStrings[kCursorActionCount];

        uint64_t    cursorWorkloopTime;
        uint64_t    lastActionTimes[kCursorActionCount];
        uint8_t     lastCursorActionsMask;

        IOHistogramReporter * cursorTotalHistReporter;
        IOHistogramReporter * cursorGraphicsHistReporter;

    } _diags;

    IOPMrootDomain *    rootDomain;
    UInt32              powerState;

    OSDictionary *  savedParameters;	// keep user settings

    UInt32		maxWaitCursorFrame;	// animation frames
	UInt32		firstWaitCursorFrame;	//

    IOHIDPointingDevice * _hidPointingDevice;
    IOHIDKeyboardDevice * _hidKeyboardDevice;

    OSObject * lastSender;


    bool setParamPropertiesInProgress;

    OSSet * dataQueueSet;
  

private:


  static IOReturn powerStateHandler( void *target, void *refCon,
            UInt32 messageType, IOService *service, void *messageArgument, vm_size_t argSize );
  void updatePowerState(UInt32 messageType);

 /* Resets */
    void    _setScrollCountParameters(OSDictionary *newSettings = NULL);

  /* Initialize the shared memory area */
  void     initShmem(bool clean);
  /* Dispatch low level events through shared memory to the WindowServer */
  void postEvent(       int        what,
          /* at */      IOFixedPoint64 *       location,
          /* atTime */  AbsoluteTime  ts,
          /* withData */NXEventData * myData,
          /* sender */  OSObject *    sender   = 0,
          /* pid */     UInt32        extPID   = 0,
          /* processKEQ*/bool         processKEQ = true,
          /* options*/   UInt32       options    = 0
                          );
  /* Dispatch mechanisms for screen state changes */
  void evDispatch(
            /* command */ EvCmd evcmd);

  OSDictionary * createFilteredParamPropertiesForService(IOService * service, OSDictionary * dict);

  static void _periodicEvents(IOHIDSystem * self,
                              IOTimerEventSource *timer);

  static void doProcessKeyboardEQ(IOHIDSystem * self);
  static void processKeyboardEQ(IOHIDSystem * self, AbsoluteTime * deadline = 0);

  void doProcessNotifications();
  
  bool genericNotificationHandler(void * ref, IOService * newService, IONotifier * notifier );

  static bool handlePublishNotification( void * target, IOService * newService );
  static bool handleTerminationNotification( void * target, IOService * newService );

  static void makeNumberParamProperty( OSDictionary * dict, const char * key,
                            unsigned long long number, unsigned int bits );

  static void makeInt32ArrayParamProperty( OSDictionary * dict, const char * key,
                            UInt32 * array, unsigned int count );

/*
 * HISTORICAL NOTE:
 *   The following methods were part of the IOHIDSystem(Input) category;
 *   the declarations have now been merged directly into this class.
 *
 * Exported Interface Event Driver object input services.
 */

private:
  // Schedule next periodic run based on current event system state.
  void scheduleNextPeriodicEvent();
  // Message invoked to run periodic events.  This method runs in the workloop.
  void periodicEvents(IOTimerEventSource *timer);
  // Start the cursor running.
  bool startCursor();
  // Repin cursor location.
  bool resetCursor();
  // Wait Cursor machinery.
  void showWaitCursor();
  void hideWaitCursor();
  void animateWaitCursor();
  void changeCursor(int frame);
  // Return screen number a point lies on.
  int  pointToScreen(IOGPoint * p);

  inline void showCursor();
  inline void hideCursor();
  inline void moveCursor();
  void enableContinuousCursor();
  void disableContinuousCursor();
  void _onScreenCursorPin();

  bool registerEventSource(IOService * source);

  // Set abs cursor position.
  void setCursorPosition(IOGPoint * newLoc, bool external, OSObject * sender=0);
  void _setCursorPosition(bool external = false, bool proximityChange = false, OSObject * sender=0);

  static bool _idleTimeSerializerCallback(void * target, void * ref, OSSerialize *s);
  static bool _cursorStateSerializerCallback(void * target, void * ref, OSSerialize *s);
  static bool _displaySerializerCallback(void * target, void * ref, OSSerialize *s);

  IOReturn _recordCursorAction(uint64_t origTS, uint64_t entryTS);

  virtual IOReturn configureReport(IOReportChannelList *channels,
                                   IOReportConfigureAction action,
                                   void *result,
                                   void *destination) APPLE_KEXT_OVERRIDE;
  virtual IOReturn updateReport(IOReportChannelList      *channels,
                                IOReportUpdateAction      action,
                                void                     *result,
                                void                     *destination) APPLE_KEXT_OVERRIDE;

  void createParameters( void );

/* END HISTORICAL NOTE */
  
//  void updateUnificationFlags ();
  
public:
  static IOHIDSystem * instance();     /* Return the current instance of the */
				       /* EventDriver, or 0 if none. */

  virtual bool init(OSDictionary * properties = 0) APPLE_KEXT_OVERRIDE;
  virtual IOHIDSystem * probe(IOService *    provider,
                              SInt32 * score) APPLE_KEXT_OVERRIDE;
  virtual bool start(IOService * provider) APPLE_KEXT_OVERRIDE;
  virtual IOReturn message(UInt32 type, IOService * provider,
				void * argument) APPLE_KEXT_OVERRIDE;
  virtual void free(void) APPLE_KEXT_OVERRIDE;
  virtual bool attach( IOService * provider ) APPLE_KEXT_OVERRIDE;
  virtual void detach( IOService * provider ) APPLE_KEXT_OVERRIDE;

  virtual IOWorkLoop *getWorkLoop(void) const APPLE_KEXT_OVERRIDE;

  virtual IOReturn evOpen(void);
  virtual IOReturn evClose(void);

  virtual IOReturn  setProperties( OSObject * properties ) APPLE_KEXT_OVERRIDE;
  virtual IOReturn  setParamProperties(OSDictionary * dict);

  /* Create the shared memory area */
  virtual IOReturn createShmem(void*,void*,void*,void*,void*,void*);

  /* register the IOSharedDataQueue for the new user events */
  virtual IOReturn registerEventQueue(IOSharedDataQueue * queue);

  /* Unregister the IOSharedDataQueue for the new user events */
  virtual IOReturn unregisterEventQueue(IOSharedDataQueue * queue);

public:

  virtual IOReturn newUserClient(task_t         owningTask,
                 /* withToken */ void *         security_id,
                 /* ofType */    UInt32         type,
                 /* withProps*/  OSDictionary *  properties,
                 /* client */    IOUserClient ** handler) APPLE_KEXT_OVERRIDE;

/*
 * HISTORICAL NOTE:
 *   The following methods were part of the IOHIPointingEvents protocol;
 *   the declarations have now been merged directly into this class.
 */

public:
/*
 * HISTORICAL NOTE:
 *   The following methods were part of the IOHIKeyboardEvents protocol;
 *   the declarations have now been merged directly into this class.
 */

public:
  virtual void keyboardEvent(unsigned   eventType,
      /* flags */            unsigned   flags,
      /* keyCode */          unsigned   key,
      /* charCode */         unsigned   charCode,
      /* charSet */          unsigned   charSet,
      /* originalCharCode */ unsigned   origCharCode,
      /* originalCharSet */  unsigned   origCharSet,
      /* keyboardType */     unsigned   keyboardType,
      /* repeat */           bool       repeat,
      /* atTime */           AbsoluteTime ts);

  virtual void keyboardSpecialEvent(   unsigned   eventType,
                    /* flags */        unsigned   flags,
                    /* keyCode  */     unsigned   key,
                    /* specialty */    unsigned   flavor,
                    /* guid */ 	       UInt64     guid,
                    /* repeat */       bool       repeat,
                    /* atTime */       AbsoluteTime ts);

  virtual void updateEventFlags(unsigned flags);  /* Does not generate events */




private:

  static void _keyboardEvent(       IOHIDSystem * self,
                                    unsigned   eventType,
            /* flags */             unsigned   flags,
            /* keyCode */           unsigned   key,
            /* charCode */          unsigned   charCode,
            /* charSet */           unsigned   charSet,
            /* originalCharCode */  unsigned   origCharCode,
            /* originalCharSet */   unsigned   origCharSet,
            /* keyboardType */      unsigned   keyboardType,
            /* repeat */            bool       repeat,
            /* atTime */            AbsoluteTime ts,
                                    OSObject * sender,
                                    void *     refcon);

  static void _keyboardSpecialEvent(IOHIDSystem * self,
                                    unsigned   eventType,
                /* flags */         unsigned   flags,
                /* keyCode  */      unsigned   key,
                /* specialty */     unsigned   flavor,
                /* guid */          UInt64     guid,
                /* repeat */        bool       repeat,
                /* atTime */        AbsoluteTime ts,
                                    OSObject * sender,
                                    void *     refcon);

  static void _updateEventFlags(    IOHIDSystem * self,
                                    unsigned   flags,
                                    OSObject * sender,
                                    void *     refcon);  /* Does not generate events */


/*
 * HISTORICAL NOTE:
 *   The following methods were part of the IOUserClient protocol;
 *   the declarations have now been merged directly into this class.
 */

public:

  virtual IOReturn setEventsEnable(void*,void*,void*,void*,void*,void*);
  virtual IOReturn setCursorEnable(void*,void*,void*,void*,void*,void*);
  virtual IOReturn extPostEvent(void*,void*,void*,void*,void*,void*);
  virtual IOReturn extSetMouseLocation(void*,void*,void*,void*,void*,void*);
  virtual IOReturn extGetButtonEventNum(void*,void*,void*,void*,void*,void*);
    
    IOReturn extSetBounds(void*,void*,void*,void*,void*,void*);
    IOReturn extSetOnScreenBounds(void*,void*,void*,void*,void*,void*);
    IOReturn setBounds(IOGBounds * bounds, IOGPoint * screenPoint, bool onScreen);

    IOReturn extGetStateForSelector(void*,void*,void*,void*,void*,void*);
    IOReturn extSetStateForSelector(void*,void*,void*,void*,void*,void*);
    IOReturn extRegisterVirtualDisplay(void*,void*,void*,void*,void*,void*);
    IOReturn extUnregisterVirtualDisplay(void*,void*,void*,void*,void*,void*);
    IOReturn extSetVirtualDisplayBounds(void*,void*,void*,void*,void*,void*);
    IOReturn extGetUserHidActivityState(void*,void*,void*,void*,void*,void*);
    IOReturn setContinuousCursorEnable(void*,void*,void*,void*,void*,void*);
/*
 * HISTORICAL NOTE:
 *   The following methods were part of the IOScreenRegistration protocol;
 *   the declarations have now been merged directly into this class.
 *
 * Methods exported by the EventDriver for display systems.
 *
 *	The screenRegister protocol is used by frame buffer drivers to register
 *	themselves with the Event Driver.  These methods are called in response
 *	to an _IOGetParameterInIntArray() call with "IO_Framebuffer_Register" or
 *	"IO_Framebuffer_Unregister".
 */

public:
  virtual int registerScreen(IOGraphicsDevice * instance,
        /* bounds */         IOGBounds * bp,
        /* virtual bounds */ IOGBounds * vbp);
private:
    static IOReturn doRegisterScreen(IOHIDSystem *self, IOGraphicsDevice *io_gd, IOGBounds *bp, IOGBounds * vbp, void *arg3);
    IOReturn        registerScreenGated(IOGraphicsDevice *io_gd, IOGBounds *bp, IOGBounds * vbp, SInt32 *index);
public:


  virtual void unregisterScreen(int index);

/*
 * HISTORICAL NOTE:
 *   The following methods were part of the IOWorkspaceBounds protocol;
 *   the declarations have now been merged directly into this class.
 *
 * Absolute position input devices and some specialized output devices
 * may need to know the bounding rectangle for all attached displays.
 * The following method returns a IOGBounds* for the workspace.  Please note
 * that the bounds are kept as signed values, and that on a multi-display
 * system the minx and miny values may very well be negative.
 */

public:
  virtual IOGBounds * workspaceBounds();

/* END HISTORICAL NOTES */

private:
void keyboardEvent(unsigned   eventType,
      /* flags */            unsigned   flags,
      /* keyCode */          unsigned   key,
      /* charCode */         unsigned   charCode,
      /* charSet */          unsigned   charSet,
      /* originalCharCode */ unsigned   origCharCode,
      /* originalCharSet */  unsigned   origCharSet,
      /* keyboardType */     unsigned   keyboardType,
      /* repeat */           bool       repeat,
      /* atTime */           AbsoluteTime ts,
      /* sender */           OSObject * sender);

void keyboardSpecialEvent(   unsigned   eventType,
      /* flags */        unsigned   flags,
      /* keyCode  */     unsigned   key,
      /* specialty */    unsigned   flavor,
      /* guid */         UInt64     guid,
      /* repeat */       bool       repeat,
      /* atTime */       AbsoluteTime ts,
      /* sender */       OSObject * sender);

void updateEventFlags(unsigned flags, OSObject * sender);


/*
 * COMMAND GATE COMPATIBILITY:
 *   The following method is part of the work needed to make IOHIDSystem
 *   compatible with IOCommandGate.  The use of IOCommandQueue has been
 *   deprecated, thus requiring this move.  This should allow for less
 *   context switching as all actions formerly run on the I/O Workloop
 *   thread, will now be run on the caller thread.  The static methods
 *   will be called from cmdGate->runAction and returns the appropriate
 *   non-static helper method.  Arguments are stored in the void* array,
 *   args, and are passed through.   Since we are returning in the static
 *   function, gcc3 should translate that to one instruction, thus
 *   minimizing cost.
 */

static	IOReturn	doEvClose (IOHIDSystem *self);
        IOReturn	evCloseGated (void);

static	IOReturn	doUnregisterScreen (IOHIDSystem *self, void * arg0);
        IOReturn	unregisterScreenGated (int index);

static	IOReturn	doSetDisplayBounds (IOHIDSystem *self, void * arg0, void * arg1);
        IOReturn	setDisplayBoundsGated (UInt32 index, IOGBounds *bounds);

static	IOReturn	doCreateShmem (IOHIDSystem *self, void * arg0);
        IOReturn	createShmemGated (void * p1);

static    IOReturn    doRegisterEventQueue (IOHIDSystem *self, void * arg0);
        IOReturn    registerEventQueueGated (void * p1);

static    IOReturn    doUnregisterEventQueue (IOHIDSystem *self, void * arg0);
        IOReturn    unregisterEventQueueGated (void * p1);


static	IOReturn	doKeyboardEvent (IOHIDSystem *self, void * args);
        void		keyboardEventGated (unsigned   eventType,
                                            unsigned   flags,
                                            unsigned   key,
                                            unsigned   charCode,
                                            unsigned   charSet,
                                            unsigned   origCharCode,
                                            unsigned   origCharSet,
                                            unsigned   keyboardType,
                                            bool       repeat,
                                            AbsoluteTime ts,
                                            OSObject * sender);

static	IOReturn	doKeyboardSpecialEvent (IOHIDSystem *self, void * args);
        void		keyboardSpecialEventGated (
                                            unsigned   eventType,
                                            unsigned   flags,
                                            unsigned   key,
                                            unsigned   flavor,
                                            UInt64     guid,
                                            bool       repeat,
                                            AbsoluteTime ts,
                                            OSObject * sender);

static	IOReturn	doUpdateEventFlags (IOHIDSystem *self, void * args);
        void		updateEventFlagsGated (unsigned flags, OSObject * sender);

static	IOReturn	doNewUserClient (IOHIDSystem *self, void * args);
        IOReturn 	newUserClientGated (task_t owningTask,
                                            void * security_id,
                                            UInt32 type,
                                            OSDictionary *  properties,
                                            IOUserClient ** handler);

static	IOReturn	doSetCursorEnable (IOHIDSystem *self, void * arg0);
        IOReturn	setCursorEnableGated (void * p1);

static	IOReturn	doSetContinuousCursorEnable (IOHIDSystem *self, void * arg0);
        IOReturn	setContinuousCursorEnableGated (void * p1);

static	IOReturn	doExtPostEvent(IOHIDSystem *self, void * arg0, void * arg1, void * arg2, void * arg3);
        IOReturn	extPostEventGated (void * p1, void * p2, void * p3);

static	IOReturn	doExtSetMouseLocation (IOHIDSystem *self, void * args);
        IOReturn	extSetMouseLocationGated (void * args);

static	IOReturn	doExtGetButtonEventNum (IOHIDSystem *self, void * arg0, void * arg1);
        IOReturn	extGetButtonEventNumGated (void * p1, void * p2);

static	IOReturn	doSetParamPropertiesPre (IOHIDSystem *self, void * arg0, void * arg1);
        IOReturn	setParamPropertiesPreGated (OSDictionary * dict, OSIterator ** pOpenIter);

static	IOReturn	doSetParamPropertiesPost (IOHIDSystem *self, void * arg0);
        IOReturn	setParamPropertiesPostGated (OSDictionary * dict);

        IOReturn  updateParamPropertiesGated(IOService * source);

static	IOReturn	doExtGetStateForSelector (IOHIDSystem *self, void *p1, void *p2);
static	IOReturn	doExtSetStateForSelector (IOHIDSystem *self, void *p1, void *p2);

/* END COMMAND GATE COMPATIBILITY */

public:
    virtual UInt32 eventFlags();
    virtual void   sleepDisplayTickle(IOService * requester);

    virtual void dispatchEvent(IOHIDEvent *event, IOOptionBits options=0);

    void updateHidActivity();
    void hidActivityChecker();
    static void reportUserHidActivity(IOHIDSystem *self, void *args );
    void reportUserHidActivityGated(void *args );

    static IOReturn getUserHidActivityState(IOHIDSystem *self, void *arg0);
    IOReturn getUserHidActivityStateGated(void *state);

};

#endif /* !_IOHIDSYSTEM_H */
