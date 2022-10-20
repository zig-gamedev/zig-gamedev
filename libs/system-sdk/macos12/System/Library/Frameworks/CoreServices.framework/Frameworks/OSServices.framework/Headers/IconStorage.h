/*
     File:       OSServices/IconStorage.h

     Contains:   Services to load and share icon family data.

     Copyright:  (c) 2000-2011 Apple Inc. All rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

*/

#ifndef __ICONSTORAGE__
#define __ICONSTORAGE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#pragma pack(push, 2)

/* virtual ARGB icon types. Each type will be split into separate 24 bit RGB data and 8 bit mask for storage in icns container */
/* the ARGB bitmap must be non-premultiplied */
enum {
    kIconServices16PixelDataARGB  = 'ic04', /* uses kSmall32BitIconIndex and kSmallDeepMaskIconIndex*/
    kIconServices32PixelDataARGB  = 'ic05', /* uses kLarge32BitIconIndex and kLargeDeepMaskIconIndex*/
    kIconServices48PixelDataARGB  = 'ic06', /* uses kHuge32BitIconIndex and kHugeDeepMaskIconIndex*/
    kIconServices128PixelDataARGB = 'ic07' /* uses kThumbnailDataIndex and kThumbnailMaskIndex*/
};

/* The following icon types can only be used as an icon element */
/* inside a 'icns' icon family */
enum {
    kIconServices256PixelDataARGB = 'ic08', /* non-premultiplied 256x256 ARGB bitmap*/
    kIconServices512PixelDataARGB = 'ic09', /* non-premultiplied 512x512 ARGB bitmap*/
    kIconServices1024PixelDataARGB = 'ic10', /* non-premultiplied 1024x1024 ARGB bitmap*/
    kThumbnail32BitData           = 'it32',
    kThumbnail8BitMask            = 't8mk'
};

enum {
    kHuge1BitMask                 = 'ich#',
    kHuge4BitData                 = 'ich4',
    kHuge8BitData                 = 'ich8',
    kHuge32BitData                = 'ih32',
    kHuge8BitMask                 = 'h8mk'
};

/* The following icon types can be used as a resource type */
/* or as an icon element type inside a 'icns' icon family */
enum {
    kLarge1BitMask                = 'ICN#',
    kLarge4BitData                = 'icl4',
    kLarge8BitData                = 'icl8',
    kLarge32BitData               = 'il32',
    kLarge8BitMask                = 'l8mk',
    kSmall1BitMask                = 'ics#',
    kSmall4BitData                = 'ics4',
    kSmall8BitData                = 'ics8',
    kSmall32BitData               = 'is32',
    kSmall8BitMask                = 's8mk',
    kMini1BitMask                 = 'icm#',
    kMini4BitData                 = 'icm4',
    kMini8BitData                 = 'icm8'
};

/* Obsolete. Use names defined above. */
enum {
    large1BitMask                 = kLarge1BitMask,
    large4BitData                 = kLarge4BitData,
    large8BitData                 = kLarge8BitData,
    small1BitMask                 = kSmall1BitMask,
    small4BitData                 = kSmall4BitData,
    small8BitData                 = kSmall8BitData,
    mini1BitMask                  = kMini1BitMask,
    mini4BitData                  = kMini4BitData,
    mini8BitData                  = kMini8BitData
};

/*
    IconFamily 'icns' resources contain an entire IconFamily (all sizes and depths).
   For custom icons, icns IconFamily resources of the custom icon resource ID are fetched first before
   the classic custom icons (individual 'ics#, ICN#, etc) are fetched.  If the fetch of the icns resource
   succeeds then the icns is looked at exclusively for the icon data.
   For custom icons, new icon features such as 32-bit deep icons are only fetched from the icns resource.
   This is to avoid incompatibilities with cut & paste of new style icons with an older version of the
   MacOS Finder.
   DriverGestalt is called with code kdgMediaIconSuite by IconServices after calling FSM to determine a
   driver icon for a particular device.  The result of the kdgMediaIconSuite call to DriverGestalt should
   be a pointer an an IconFamily.  In this manner driver vendors can provide rich, detailed drive icons
   instead of the 1-bit variety previously supported.
   The IconFamilyElement and IconFamilyResource data types (which also include the data types
   IconFamilyPtr and IconFamilyHandle) are always big-endian.
*/

enum {
    kIconFamilyType               = 'icns'
};

struct IconFamilyElement {
	OSType              elementType;            /* 'ICN#', 'icl8', etc...*/
	SInt32              elementSize;            /* Size of this element*/
	unsigned char       elementData[1];
};
typedef struct IconFamilyElement        IconFamilyElement;
struct IconFamilyResource {
	OSType              resourceType;           /* Always 'icns'*/
	SInt32              resourceSize;           /* Total size of this resource*/
	IconFamilyElement   elements[1];

};
typedef struct IconFamilyResource       IconFamilyResource;
typedef IconFamilyResource*             IconFamilyPtr;
typedef IconFamilyPtr*                  IconFamilyHandle;

/*  Icon Variants */
/* These can be used as an element of an 'icns' icon family */
/* or as a parameter to GetIconRefVariant */
enum {
    kTileIconVariant              = 'tile',
    kRolloverIconVariant          = 'over',
    kDropIconVariant              = 'drop',
    kOpenIconVariant              = 'open',
    kOpenDropIconVariant          = 'odrp'
};


#pragma pack(pop)

#endif /* __ICONSTORAGE__ */

