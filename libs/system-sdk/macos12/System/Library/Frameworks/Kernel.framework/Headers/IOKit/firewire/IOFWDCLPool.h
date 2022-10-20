/*
 *  IOFWNuDCLPool.h
 *  IOFireWireFamily
 *
 *  Created by Niels on Fri Mar 07 2003.
 *  Copyright (c) 2003 Apple Computer, Inc. All rights reserved.
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.14  2008/03/03 23:31:42  ayanowit
 *	another gcc-42 fix.
 *	
 *	Revision 1.13  2007/01/26 20:52:31  ayanowit
 *	changes to user-space isoch stuff to support 64-bit apps.
 *	
 *	Revision 1.12  2006/02/09 00:21:50  niels
 *	merge chardonnay branch to tot
 *	
 *	Revision 1.11  2005/04/12 20:09:13  niels
 *	fix memory leak importing NuDCL programs from user space
 *	
 *	Revision 1.10.20.2  2006/01/31 04:49:50  collin
 *	*** empty log message ***
 *	
 *	Revision 1.10  2003/11/07 21:24:28  niels
 *	*** empty log message ***
 *	
 *	Revision 1.9  2003/11/07 21:01:18  niels
 *	*** empty log message ***
 *	
 *	Revision 1.8  2003/08/25 08:39:15  niels
 *	*** empty log message ***
 *	
 *	Revision 1.7  2003/08/22 18:15:16  niels
 *	*** empty log message ***
 *	
 *	Revision 1.6  2003/08/15 04:36:55  niels
 *	*** empty log message ***
 *	
 *	Revision 1.5  2003/07/30 05:22:14  niels
 *	*** empty log message ***
 *	
 *	Revision 1.4  2003/07/21 08:48:20  niels
 *	*** empty log message ***
 *	
 *	Revision 1.3  2003/07/21 07:52:13  niels
 *	*** empty log message ***
 *	
 *	Revision 1.2  2003/07/21 06:52:58  niels
 *	merge isoch to TOT
 *	
 *	Revision 1.1.2.3  2003/07/11 18:15:34  niels
 *	*** empty log message ***
 *	
 *	Revision 1.1.2.2  2003/07/09 21:24:00  niels
 *	*** empty log message ***
 *	
 *	Revision 1.1.2.1  2003/07/01 20:54:06  niels
 *	isoch merge
 *	
 */

#import <libkern/c++/OSObject.h>
#import <libkern/c++/OSArray.h>
#import <IOKit/IOTypes.h>

class IOFireWireLink ;
class IOFWDCL ;
class IOFWReceiveDCL ;
class IOFWSendDCL ;
class IOFWSkipCycleDCL ;
class IOFireWireUserClient ;
class IOMemoryDescriptor ;
class IOMemoryMap ;
class OSSet;

/*! @class IOFWDCLPool
	@discussion You should never subclass IOFWDCLPool 
*/

class IOFWDCLPool : public OSObject
{
	OSDeclareAbstractStructors( IOFWDCLPool )

	friend class IOFireWireUserClient ;
	friend class IOFWUserLocalIsochPort ;
	
	protected:
	
		class Expansion*		fReserved ;		// for class expansion

		IOFireWireLink *		fLink ;
		UInt8					fCurrentTag ;
		UInt8					fCurrentSync ;
		OSArray*				fProgram ;
		DCLNuDCLLeader			fLeader ;
	
	public:
		
		// OSObject
		
		virtual void						free(void) APPLE_KEXT_OVERRIDE;
		
		// me
		
		virtual bool	 					initWithLink ( IOFireWireLink& link, UInt32 capacity ) ;
		
		virtual void						setCurrentTagAndSync ( UInt8 tag, UInt8 sync ) ;
		
		virtual IOFWReceiveDCL*				appendReceiveDCL ( 
													OSSet * 				updateSet, 
													UInt8 					headerBytes,
													UInt32					rangesCount,
													IOVirtualRange			ranges[] ) ;
		virtual IOFWSendDCL*				appendSendDCL ( 
													OSSet * 				updateSet, 
													UInt32					rangesCount,
													IOVirtualRange			ranges[] ) ;
		virtual IOFWSkipCycleDCL*			appendSkipCycleDCL () ;
		virtual const OSArray *				getProgramRef () const ;
		
	protected :
	
		IOReturn							importUserProgram (
													IOMemoryDescriptor *	userExportDesc,
													unsigned				bufferRangeCount,
													IOAddressRange			bufferRanges[],
													IOMemoryMap *			bufferMap ) ;
		IOReturn							importUserDCL(
													IOFWDCL *				dcl,
													void * 					importData,
													IOByteCount &			dataSize,
													IOMemoryMap *			bufferMap ) ;
													

	protected :
	
		virtual IOFWReceiveDCL *			allocReceiveDCL () = 0 ;
		virtual IOFWSendDCL *				allocSendDCL () = 0 ;
		virtual IOFWSkipCycleDCL *			allocSkipCycleDCL () = 0 ;

	private :
	
		void								appendDCL( IOFWDCL * dcl ) ;

	public :
	
		DCLCommand *						getProgram() ;
													
    OSMetaClassDeclareReservedUnused ( IOFWDCLPool, 0);
    OSMetaClassDeclareReservedUnused ( IOFWDCLPool, 1);
    OSMetaClassDeclareReservedUnused ( IOFWDCLPool, 2);
    OSMetaClassDeclareReservedUnused ( IOFWDCLPool, 3);
    OSMetaClassDeclareReservedUnused ( IOFWDCLPool, 4);
    OSMetaClassDeclareReservedUnused ( IOFWDCLPool, 5);
    OSMetaClassDeclareReservedUnused ( IOFWDCLPool, 6);
    OSMetaClassDeclareReservedUnused ( IOFWDCLPool, 7);		
} ;
