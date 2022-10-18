/*
	File:		IOVideoDeviceClientInit.h
	
	Contains:		This class is used to add an IOProviderMergeProperties dictionary entry to a provider's property list, thus providing a tie between hardware and a CFBundle at hardware
					load time. This property usually contains the user client class name and the CFPlugInTypes UUID's but it can contain other properties.


	Copyright:	© 2006-2012 by Apple Inc., all rights reserved.
*/

#ifndef __IOVIDEODEVICECLIENTINIT_H
#define __IOVIDEODEVICECLIENTINIT_H

#include <IOKit/IOService.h>

class IOVideoDeviceUserClientInit : public IOService 
{
					OSDeclareDefaultStructors(IOVideoDeviceUserClientInit);
    
public:
    virtual bool	start(IOService* provider) ;
    virtual bool	MergeDictionaryIntoProvider(IOService* provider, OSDictionary* mergeDicttionary);
    virtual bool	MergeDictionaryIntoDictionary(OSDictionary* sourceDictionary, OSDictionary* targetDictionary);
};

#endif
