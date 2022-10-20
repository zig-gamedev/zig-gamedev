/*
     File:       CarbonCore/HFSVolumes.h
 
     Contains:   On-disk data structures for HFS and HFS Plus volumes.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1984-2011 by Apple Inc. All rights reserved.
*/
#ifndef __HFSVOLUMES__
#define __HFSVOLUMES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <CarbonCore/Files.h>
#endif

#ifndef __FINDER__
#include <CarbonCore/Finder.h>
#endif


#include <hfs/hfs_format.h>

#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

/* CatalogNodeID is used to track catalog objects */
typedef UInt32                          HFSCatalogNodeID;


#endif /* __HFSVOLUMES__ */

