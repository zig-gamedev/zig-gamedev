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
 * IOHIDevice.h - Common Event Source object class.
 *
 * HISTORY
 * 22 May 1992    Mike Paquette at NeXT
 *      Created. 
 * 4  Aug 1993	  Erik Kay at NeXT
 *	API cleanup
 * 5  Aug 1993	  Erik Kay at NeXT
 *	added ivar space for future expansion
 */

#ifndef _IOHIDEVICE_H
#define _IOHIDEVICE_H

#include <IOKit/IOService.h>
#include <IOKit/IOLocks.h>

typedef enum {
  kHIUnknownDevice          = 0,
  kHIKeyboardDevice         = 1,
  kHIRelativePointingDevice = 2
} IOHIDKind;

#if defined(KERNEL) && !defined(KERNEL_PRIVATE)
class __deprecated_msg("Use DriverKit") IOHIDevice : public IOService
#else
class IOHIDevice : public IOService
#endif
{
  OSDeclareDefaultStructors(IOHIDevice);

public:
  virtual bool init(OSDictionary * properties = 0) APPLE_KEXT_OVERRIDE;
  virtual void free(void) APPLE_KEXT_OVERRIDE;
  virtual bool start(IOService * provider) APPLE_KEXT_OVERRIDE;
  virtual bool open(  IOService *    forClient,
                      IOOptionBits   options = 0,
                      void *         arg = 0 ) APPLE_KEXT_OVERRIDE;

  virtual UInt32    deviceType();
  virtual IOHIDKind hidKind();
  virtual UInt32    interfaceID();
  virtual bool 	    updateProperties(void);
  virtual IOReturn  setProperties( OSObject * properties ) APPLE_KEXT_OVERRIDE;
  virtual IOReturn  setParamProperties(OSDictionary * dict);
  virtual UInt64    getGUID();
  
  static SInt32		GenerateKey(OSObject *object);
};

#endif /* !_IOHIDEVICE_H */
