/*
 * Copyright (c) 2007 Apple Computer, Inc. All rights reserved.
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
 
#ifndef _IOFWPHYPACKETLISTENER_H_
#define _IOFWPHYPACKETLISTENER_H_

#include <libkern/c++/OSObject.h>

class IOFireWireController;

// Callback when phy packet is received
typedef void (*FWPHYPacketCallback)( void *refcon, UInt32 data1, UInt32 data2 );

/*! @class IOFWPHYPacketListener
*/

class IOFWPHYPacketListener : public OSObject
{
	OSDeclareDefaultStructors( IOFWPHYPacketListener );

	friend class IOFireWireController;
	
protected:
	
	IOFireWireController *		fControl;
	FWPHYPacketCallback			fCallback;
	void *						fRefCon;

	static IOFWPHYPacketListener * createWithController( IOFireWireController * controller );

    virtual bool initWithController( IOFireWireController * control );
    virtual void free( void ) APPLE_KEXT_OVERRIDE;

public:

	virtual IOReturn activate( void );
	virtual void deactivate( void );

	virtual void setCallback( FWPHYPacketCallback callback );
	virtual void setRefCon( void * refcon );
	virtual void * getRefCon( void );

protected:
	virtual void processPHYPacket( UInt32 data1, UInt32 data2 );

    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 0 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 1 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 2 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 3 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 4 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 5 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 6 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 7 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 8 );
    OSMetaClassDeclareReservedUnused( IOFWPHYPacketListener, 9 );
};

#endif
