/*
 *  IOStreamUserClient.h
 *  IOStreamFamily
 *
 *  Copyright 2006 Apple Computer, Inc. All rights reserved.
 *
 */

#if !defined( __IOKIT_IOSTREAMUSERCLIENT_H )
#define __IOKIT_IOSTREAMUSERCLIENT_H


#include <IOKit/IOUserClient.h>

#include <IOKit/stream/IOStreamShared.h>
#include <IOKit/stream/IOStream.h>
#include <sys/cdefs.h>

class __exported IOStreamUserClient : public IOUserClient
{
    OSDeclareDefaultStructors( IOStreamUserClient )
    
protected:
    IOStream * _owner;
    task_t     _task;
    IOWorkLoop * fWL;
    IOCommandGate * fGate;
    
public:
    virtual bool initWithTask(
                                  task_t owningTask, void * securityToken, UInt32 type,
                                  OSDictionary * properties);
    virtual bool initWithTask(
                              task_t owningTask, void * securityToken, UInt32 type);
    
    virtual IOReturn clientClose( void );
    virtual IOReturn clientDied( void );

    virtual IOService * getService( void );
    
    virtual IOReturn registerNotificationPort(
                                              mach_port_t port, UInt32 type, UInt32 refCon );
    
    virtual IOReturn connectClient( IOUserClient * client );
    
    virtual IOExternalMethod * getTargetAndMethodForIndex(
                                                          IOService ** targetP, UInt32 index );
    
    virtual IOExternalTrap *
      getTargetAndTrapForIndex( IOService **targetP, UInt32 index );

    virtual IOReturn clientMemoryForType( UInt32 type,
                                          IOOptionBits * options,
                                          IOMemoryDescriptor ** memory );
    
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop(IOService *provider) APPLE_KEXT_OVERRIDE;
    
    virtual void free(void) APPLE_KEXT_OVERRIDE;
    
    
    
protected:
    virtual IOReturn openMethod(  char * dataIn, char * dataOut,
                                  IOByteCount inputSize, IOByteCount * outputSize );
    virtual IOReturn closeMethod( void );
    virtual IOReturn startMethod( void );
    virtual IOReturn stopMethod( void );
    virtual IOReturn suspendMethod( void );
    virtual IOReturn getModeMethod(  char * dataOut,
                                     IOByteCount * outputSize );
    virtual IOReturn setModeMethod(  char * dataIn, 
                                     IOByteCount inputSize );
    virtual IOReturn getBufferCountMethod(  char * dataOut,
                                            IOByteCount * outputSize );
    
    virtual IOReturn inputTrap( UInt32 token );
    virtual IOReturn inputSyncTrap( UInt32 token );
    
    IOReturn clientMemoryForTypeGated( UInt32 memoryType, IOOptionBits * flags, IOMemoryDescriptor ** memory );
    
    IOReturn getTargetAndMethodForIndexGated(IOService ** targetP, UInt32 index , IOExternalMethod ** method);

};

#endif /* ! __IOKIT_IOSTREAMUSERCLIENT_H */

