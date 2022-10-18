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

#ifndef _IOKIT_IOAUDIOCONTROLUSERCLIENT_H
#define _IOKIT_IOAUDIOCONTROLUSERCLIENT_H

#include <AvailabilityMacros.h>
#include <IOKit/IOUserClient.h>

#ifndef IOAUDIOFAMILY_SELF_BUILD
#include <IOKit/audio/IOAudioTypes.h>
#else
#include "IOAudioTypes.h"
#endif

class IOAudioControl;

class IOAudioControlUserClient : public IOUserClient
{
    OSDeclareDefaultStructors(IOAudioControlUserClient)
    
protected:
    task_t 				clientTask;
    IOAudioControl *			audioControl;
    IOAudioNotificationMessage *	notificationMessage;

    virtual IOReturn clientClose( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual IOReturn clientDied( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

protected:
    struct ExpansionData { };
    
    ExpansionData *reserved;

public:
	virtual void sendChangeNotification(UInt32 notificationType ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    // OSMetaClassDeclareReservedUsed(IOAudioControlUserClient, 1);
    virtual bool initWithAudioControl(IOAudioControl *control, task_t owningTask, void *securityID, UInt32 type, OSDictionary *properties ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

private:
    OSMetaClassDeclareReservedUsed(IOAudioControlUserClient, 0);
    OSMetaClassDeclareReservedUsed(IOAudioControlUserClient, 1);

    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 2);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 3);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 4);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 5);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 6);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 7);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 8);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 9);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 10);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 11);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 12);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 13);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 14);
    OSMetaClassDeclareReservedUnused(IOAudioControlUserClient, 15);

public:
    static IOAudioControlUserClient *withAudioControl(IOAudioControl *control, task_t clientTask, void *securityID, UInt32 type ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    static IOAudioControlUserClient *withAudioControl(IOAudioControl *control, task_t clientTask, void *securityID, UInt32 type, OSDictionary *properties ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual bool initWithAudioControl(IOAudioControl *control, task_t owningTask, void *securityID, UInt32 type ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
    virtual void free( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual IOReturn registerNotificationPort(mach_port_t port, UInt32 type, UInt32 refCon ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual void sendValueChangeNotification( ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
};

#endif /* _IOKIT_IOAUDIOCONTROLUSERCLIENT_H */
