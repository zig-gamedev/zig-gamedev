/*
     File:       Quickdraw.r
 
     Contains:   Interface to Quickdraw Graphics
 
     Version:    Quickdraw-291~1
 
     Copyright:  © 1985-2008 by Apple, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __QUICKDRAW_R__
#define __QUICKDRAW_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define QD_HEADERS_ARE_PRIVATE 1

type 'CURS' {
hex string [32];                             /* Data         */
hex string [32];                             /* Mask         */
point;                                           /* Hot spot     */
};


/*----------------------------------- common macros ---------------------------------------*/

#define PixMapHeight    ( $$BitField(Bounds, 32, 16) /* bottom */           \
- $$BitField(Bounds, 0, 16) /* top */ )
#define   PixMapWidth     ( $$BitField(Bounds, 48, 16) /* right */            \
- $$BitField(Bounds, 16, 16) /* left */ )
#define PixMapBounds    integer = $$BitField(Bounds, 0, 16) /* top */;      \
integer = $$BitField(Bounds, 16, 16) /* left */;  \
integer = $$BitField(Bounds, 32, 16) /* bottom */;    \
integer = $$BitField(Bounds, 48, 16) /* right */
#define  PixMapRowBytes  (((PixMapWidth * $$Word(PixelSize) + 15) / 16) * 2)
#define BitMapRowBytes  (((PixMapWidth + 15) / 16) * 2)

#define    PixMapHeightIndexed     ( $$BitField(Bounds[$$ArrayIndex(PixPatArray)], 32, 16) /* bottom */            \
- $$BitField(Bounds[$$ArrayIndex(PixPatArray)], 0, 16) /* top */ )
#define    PixMapWidthIndexed      ( $$BitField(Bounds[$$ArrayIndex(PixPatArray)], 48, 16) /* right */         \
- $$BitField(Bounds[$$ArrayIndex(PixPatArray)], 16, 16) /* left */ )
#define  PixMapRowBytesIndexed   (((PixMapWidthIndexed * $$Word(PixelSize[$$ArrayIndex(PixPatArray)]) + 15) / 16) * 2)


/* RGB colors */
#define whiteRGB $FFFF, $FFFF, $FFFF
#define blackRGB 0, 0, 0

/* colors for 'crsr' resources */
#define transparentRGB $FFFF, $FFFF, $FFFF
#define invertRGB 0, 0, 0


type 'PICT' {
unsigned integer;
rect;
hex string;
};


#endif /* __QUICKDRAW_R__ */

