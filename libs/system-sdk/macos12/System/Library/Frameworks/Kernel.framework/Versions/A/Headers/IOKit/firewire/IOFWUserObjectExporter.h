/*
 * Copyright (c) 1998-2008 Apple Computer, Inc. All rights reserved.
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

/*! @class IOFWUserObjectExporter
	@discussion An IOFWUserObjectExporter is for internal use only. You should never subclass IOFWUserObjectExporter 
*/

	namespace IOFireWireLib
	{
		typedef UInt32      UserObjectHandle;
	}

#ifdef KERNEL

	class IOFWUserObjectExporter : public OSObject
	{
		OSDeclareDefaultStructors (IOFWUserObjectExporter )

		public :
		
			typedef void (*CleanupFunction)( const OSObject * obj );
			typedef void (*CleanupFunctionWithExporter)( const OSObject * obj, IOFWUserObjectExporter * );
			
		private :
		
			unsigned							fCapacity;
			unsigned							fObjectCount;
			const OSObject **					fObjects;
			CleanupFunctionWithExporter *		fCleanupFunctions;
			IOLock *							fLock;
			OSObject *							fOwner;
			
		public :
		
			static IOFWUserObjectExporter *		createWithOwner( OSObject * owner );
			bool								initWithOwner( OSObject * owner );

			virtual bool			init(void) APPLE_KEXT_OVERRIDE;
	
			virtual void			free (void) APPLE_KEXT_OVERRIDE;
			virtual bool			serialize ( OSSerialize * s ) const APPLE_KEXT_OVERRIDE;
			
			// me
			IOReturn				addObject ( OSObject * obj, CleanupFunction cleanup, IOFireWireLib::UserObjectHandle * outHandle );
			void					removeObject ( IOFireWireLib::UserObjectHandle handle );
			
			// the returned object is retained! This is for thread safety.. if someone else released
			// the object from the pool after you got it, you be in for Trouble
			// Release the returned value when you're done!!
			const OSObject *		lookupObject ( IOFireWireLib::UserObjectHandle handle ) const;
			const OSObject *		lookupObjectForType( IOFireWireLib::UserObjectHandle handle, const OSMetaClass * toType ) const;
			void					removeAllObjects ();

			void					lock () const;
			void					unlock () const;
			
			OSObject *				getOwner() const;
		
			// *** WARNING: (when building SBP2) 'const' type qualifier on return type has no effect
			const IOFireWireLib::UserObjectHandle lookupHandle ( OSObject * object ) const;
		
			// don't subclass, but just in case someone does...
			
		private:
		
			OSMetaClassDeclareReservedUnused(IOFWUserObjectExporter, 0);
			OSMetaClassDeclareReservedUnused(IOFWUserObjectExporter, 1);
			OSMetaClassDeclareReservedUnused(IOFWUserObjectExporter, 2);
			OSMetaClassDeclareReservedUnused(IOFWUserObjectExporter, 3);
			OSMetaClassDeclareReservedUnused(IOFWUserObjectExporter, 4);
			OSMetaClassDeclareReservedUnused(IOFWUserObjectExporter, 5);
			OSMetaClassDeclareReservedUnused(IOFWUserObjectExporter, 6);
			OSMetaClassDeclareReservedUnused(IOFWUserObjectExporter, 7);
				
	};

#endif
