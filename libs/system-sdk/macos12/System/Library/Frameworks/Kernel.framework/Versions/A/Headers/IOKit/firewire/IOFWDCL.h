/*
*  IOFWDCL.h
*  IOFireWireFamily
*
*  Created by Niels on Fri Feb 21 2003.
*  Copyright (c) 2003 Apple Computer, Inc. All rights reserved.
*
*	$Log: not supported by cvs2svn $
*	Revision 1.19  2007/03/14 01:01:12  collin
*	*** empty log message ***
*	
*	Revision 1.18  2007/01/26 23:42:19  ayanowit
*	another fix for nuDCL rosetta mode
*	
*	Revision 1.17  2006/08/16 01:41:41  collin
*	*** empty log message ***
*	
*	Revision 1.16  2006/03/09 22:26:46  niels
*	fix 4466075
*	
*	Revision 1.15  2006/03/09 22:20:14  niels
*	fix 4466075
*	
*	Revision 1.14  2006/03/09 21:40:44  niels
*	fix 4466075
*	
*	Revision 1.13  2006/02/09 00:21:50  niels
*	merge chardonnay branch to tot
*	
*	Revision 1.12.4.1  2005/08/06 01:31:31  collin
*	*** empty log message ***
*	
*	Revision 1.12  2005/02/18 03:19:03  niels
*	fix isight
*	
*	Revision 1.11  2004/04/19 21:51:49  niels
*	*** empty log message ***
*	
*	Revision 1.10  2004/03/25 00:00:23  niels
*	fix panic allocating large physical address spaces
*	
*	Revision 1.9  2003/10/31 02:40:58  niels
*	*** empty log message ***
*	
*	Revision 1.8  2003/08/26 05:11:21  niels
*	*** empty log message ***
*	
*	Revision 1.7  2003/08/25 08:39:15  niels
*	*** empty log message ***
*	
*	Revision 1.6  2003/08/18 23:18:14  niels
*	*** empty log message ***
*	
*	Revision 1.5  2003/08/08 22:30:32  niels
*	*** empty log message ***
*	
*	Revision 1.4  2003/07/30 05:22:14  niels
*	*** empty log message ***
*	
*	Revision 1.3  2003/07/29 22:49:22  niels
*	*** empty log message ***
*	
*	Revision 1.2  2003/07/21 06:52:58  niels
*	merge isoch to TOT
*	
*	Revision 1.1.2.5  2003/07/18 00:17:41  niels
*	*** empty log message ***
*	
*	Revision 1.1.2.4  2003/07/14 22:08:53  niels
*	*** empty log message ***
*	
*	Revision 1.1.2.3  2003/07/11 18:15:33  niels
*	*** empty log message ***
*	
*	Revision 1.1.2.2  2003/07/03 22:10:24  niels
*	fix iidc/dv rcv
*	
*	Revision 1.1.2.1  2003/07/01 20:54:06  niels
*	isoch merge
*	
*/

#import <IOKit/firewire/IOFireWireFamilyCommon.h>

#import <libkern/c++/OSObject.h>
#import <libkern/c++/OSSet.h>
#import <IOKit/IOTypes.h>

class IODCLProgram ;
class OSIterator ;
class IOFireWireLink ;
class IOMemoryMap ;

/*! @class IOFWDCL
*/

class IOFWDCL : public OSObject
{
	OSDeclareAbstractStructors( IOFWDCL ) ;
	
	public:
	
		typedef void (*Callback)( void * refcon ) ;

		enum
		{
			kDynamic					= BIT(1)//kNuDCLDynamic,
			,kUpdateBeforeCallback		= BIT(2)//kNuDCLUpdateBeforeCallback
			,kUser						= BIT(18) // kNuDCLUser
			,kBigEndianUpdates			= BIT(19) // NOTE: Don't change this without making similar change to IOFireWireLib's NuDCL::Export(...)!
		} ;

		class InternalData 
		{
			public:
			
				IOFWDCL *			lastBranch ;
		} ;

	protected:
		
		IOFWDCL*			fBranch ;
		Callback			fCallback ;
		volatile UInt32 *	fTimeStampPtr ;
		UInt32				fRangeCount ;
		IOVirtualRange *	fRanges ;
		OSSet*				fUpdateList ;
		OSIterator *		fUpdateIterator ;
		volatile UInt32 *	fUserStatusPtr ;
		void*				fRefcon ;
		UInt32				fFlags ;
		
		InternalData *		fLoLevel ;

	public:

		//
		// IOFWDCL public API:
		//
		
		virtual bool		initWithRanges ( 
											OSSet * 				updateSet, 
											unsigned 				rangesCount = 0, 
											IOVirtualRange 			ranges [] = NULL ) ;

		void				setBranch( IOFWDCL* branch ) ;
		IOFWDCL*			getBranch()	const ;
		void				setTimeStampPtr ( UInt32* timeStampPtr ) ;
		UInt32*				getTimeStampPtr () const ;
		void				setCallback( Callback callback ) ;
		Callback			getCallback() const ;
		void				setStatusPtr( UInt32* statusPtr ) ;
		UInt32*				getStatusPtr() const ;
		void				setRefcon( void * refcon ) ;
		void *				getRefcon() const ;
		const OSSet*		getUpdateList() const ;
		
		virtual IOReturn	addRange ( IOVirtualRange& range ) ;
		virtual IOReturn	setRanges ( UInt32 numRanges, IOVirtualRange ranges[] ) ;
		virtual UInt32		getRanges( UInt32 maxRanges, IOVirtualRange ranges[] ) const ;
		virtual UInt32		countRanges() ;
		virtual IOReturn	getSpan( IOVirtualRange& result ) const ;
		virtual IOByteCount	getSize() const ;
		IOReturn			appendUpdateList( IOFWDCL* updateDCL ) ;
		IOReturn			setUpdateList( OSSet* updateList ) ;
		void				emptyUpdateList() ; 
		void				setFlags( UInt32 flags ) ;
		UInt32				getFlags() const ;
		

		virtual void		update() = 0 ;

		// OSObject
		
		virtual void		free (void) APPLE_KEXT_OVERRIDE;
		
	public:
		
		//
		// internal use only; please don't use... 
		//
		
		virtual IOReturn				compile( IODCLProgram & , bool & ) = 0 ;
		virtual void					link () = 0 ;

		OSMetaClassDeclareReservedUnused ( IOFWDCL, 4 ) ;		// used to be relink()

	public :
		virtual bool					interrupt( bool &, IOFWDCL * & ) = 0 ;
		virtual void					finalize ( IODCLProgram & ) ;
		virtual IOReturn				importUserDCL (
														UInt8 *				data,
														IOByteCount &		dataSize,
														IOMemoryMap *		bufferMap,
														const OSArray *		dcl ) ;
			
	protected :
	
		friend class IOFWDCLFriend ;
		
	public :
	
		// dump DCL info...
		virtual void					debug(void) ;

	public:
		
		//
		// internal use only; please don't use... 
		//
		
		virtual bool					checkForInterrupt() = 0 ;

    OSMetaClassDeclareReservedUsed ( IOFWDCL, 0 ) ;
    OSMetaClassDeclareReservedUnused ( IOFWDCL, 1 ) ;
    OSMetaClassDeclareReservedUnused ( IOFWDCL, 2 ) ;
    OSMetaClassDeclareReservedUnused ( IOFWDCL, 3 ) ;
//	OSMetaClassDeclareReservedUnused ( ***, 4 ) ;			// used above

} ;

#pragma mark -

/*! @class IOFWReceiveDCL
*/

class IOFWReceiveDCL : public IOFWDCL
{
	OSDeclareAbstractStructors( IOFWReceiveDCL )

	protected :
	
		UInt8		fHeaderBytes ;
		bool		fWait ;
	
	public:

		// me
		virtual bool		initWithParams( 
											OSSet *				updateSet, 
											UInt8				headerBytes, 
											unsigned			rangesCount, 
											IOVirtualRange		ranges [] ) ;	
		IOReturn			setWaitControl( bool wait ) ;

	public :

		// internal use only:
		virtual IOReturn				importUserDCL (
														UInt8 *				data,
														IOByteCount &		dataSize,
														IOMemoryMap *		bufferMap,
														const OSArray *		dcl ) APPLE_KEXT_OVERRIDE;
	
	protected :
	
		virtual void		debug(void) APPLE_KEXT_OVERRIDE;

} ;

#pragma mark -

/*! @class IOFWSendDCL
*/

class IOFWSendDCL : public IOFWDCL
{
	OSDeclareAbstractStructors( IOFWSendDCL )

	protected:
	
		UInt32 * 	fUserHeaderPtr ;			// pointer to 2 quadlets containing isoch header for this packet
		UInt32 *	fUserHeaderMaskPtr ;		// pointer to 2 quadlets; used to mask header quadlets
		IOFWDCL *	fSkipBranchDCL ;
		Callback	fSkipCallback ;
		void *		fSkipRefcon ;
		UInt8		fSync ;
		UInt8		fTag ;

	public:

		// OSObject
		virtual void		free(void) APPLE_KEXT_OVERRIDE;
		
		// IOFWDCL
		virtual IOReturn	addRange ( IOVirtualRange& range ) APPLE_KEXT_OVERRIDE;
		virtual IOReturn	setRanges ( UInt32 numRanges, IOVirtualRange ranges[] ) APPLE_KEXT_OVERRIDE;

		// me
		virtual bool		initWithParams( OSSet * 				updateSet, 
											unsigned 				rangesCount = 0, 
											IOVirtualRange 			ranges [] = NULL,
											UInt8					sync = 0,
											UInt8					tag = 0 ) ;
		
		void				setUserHeaderPtr( UInt32* userHeaderPtr, UInt32 * maskPtr ) ;
		UInt32 *			getUserHeaderPtr() ;
		UInt32 *			getUserHeaderMask() ;
		void				setSkipBranch( IOFWDCL * skipBranchDCL ) ;
		IOFWDCL *			getSkipBranch() const ;
		void				setSkipCallback( Callback callback ) ;
		Callback			getSkipCallback() const ;
		void				setSkipRefcon( void * refcon = 0 ) ;
		void *				getSkipRefcon() const ;										
		void				setSync( UInt8 sync ) ;
		UInt8				getSync() const ;												
		void				setTag( UInt8 tag ) ;											
		UInt8				getTag() const ;

	public :
	
		// internal use only:
		virtual IOReturn				importUserDCL (
														UInt8 *				data,
														IOByteCount &		dataSize,
														IOMemoryMap *		bufferMap,
														const OSArray *		dcl ) APPLE_KEXT_OVERRIDE;
	protected :
	
		virtual void		debug(void) APPLE_KEXT_OVERRIDE;
} ;

#pragma mark -

/*! @class IOFWSkipCycleDCL
*/

class IOFWSkipCycleDCL : public IOFWDCL
{
	OSDeclareAbstractStructors( IOFWSkipCycleDCL )

	public:
	
		virtual bool		init(void) APPLE_KEXT_OVERRIDE;
		
		virtual IOReturn	addRange ( IOVirtualRange& range ) APPLE_KEXT_OVERRIDE;
		virtual IOReturn	setRanges ( UInt32 numRanges, IOVirtualRange ranges[] ) APPLE_KEXT_OVERRIDE;
		virtual IOReturn	getSpan( IOVirtualRange& result ) ;

	protected :
	
		virtual void		debug(void) APPLE_KEXT_OVERRIDE;
} ;
