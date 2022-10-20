
#ifndef _IOKIT_IOHIDWORKLOOP_H
#define _IOKIT_IOHIDWORKLOOP_H

#include <IOKit/IOWorkLoop.h>
class __kpi_deprecated ("Use DriverKit") IOHIDWorkLoop : public IOWorkLoop
{
    OSDeclareDefaultStructors(IOHIDWorkLoop)
    
protected:
	lck_grp_t *	fLockGroup;
	
	bool init ( void ) APPLE_KEXT_OVERRIDE;
	void free ( void ) APPLE_KEXT_OVERRIDE;
	
public:
    // Create a workloop
    static IOHIDWorkLoop * workLoop();
};

#endif /* ! _IOKIT_IOHIDWORKLOOP_H */

