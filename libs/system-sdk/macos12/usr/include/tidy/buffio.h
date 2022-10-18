#ifndef __TIDY_BUFFIO_H__
#define __TIDY_BUFFIO_H__

/** @file buffio.h - Treat buffer as an I/O stream.

  (c) 1998-2005 (W3C) MIT, ERCIM, Keio University
  See tidy.h for the copyright notice.

  CVS Info :

    $Author$ 
    $Date$ 
    $Revision$ 

  Requires buffer to automatically grow as bytes are added.
  Must keep track of current read and write points.

*/

#include "platform.h"
#include "tidy.h"

#ifdef __cplusplus
extern "C" {
#endif

/** TidyBuffer - A chunk of memory */
TIDY_STRUCT
struct _TidyBuffer 
{
    byte* bp;           /**< Pointer to bytes */
    uint  size;         /**< # bytes currently in use */
    uint  allocated;    /**< # bytes allocated */ 
    uint  next;         /**< Offset of current input position */
};

/** Zero out data structure */
TIDY_EXPORT void TIDY_CALL tidyBufInit( TidyBuffer* buf );

/** Free current buffer, allocate given amount, reset input pointer */
TIDY_EXPORT void TIDY_CALL tidyBufAlloc( TidyBuffer* buf, uint allocSize );

/** Expand buffer to given size. 
**  Chunk size is minimum growth. Pass 0 for default of 256 bytes.
*/
TIDY_EXPORT void TIDY_CALL tidyBufCheckAlloc( TidyBuffer* buf,
                                             uint allocSize, uint chunkSize );

/** Free current contents and zero out */
TIDY_EXPORT void TIDY_CALL tidyBufFree( TidyBuffer* buf );

/** Set buffer bytes to 0 */
TIDY_EXPORT void TIDY_CALL tidyBufClear( TidyBuffer* buf );

/** Attach to existing buffer */
TIDY_EXPORT void TIDY_CALL tidyBufAttach( TidyBuffer* buf, byte* bp, uint size );

/** Detach from buffer.  Caller must free. */
TIDY_EXPORT void TIDY_CALL tidyBufDetach( TidyBuffer* buf );


/** Append bytes to buffer.  Expand if necessary. */
TIDY_EXPORT void TIDY_CALL tidyBufAppend( TidyBuffer* buf, void* vp, uint size );

/** Append one byte to buffer.  Expand if necessary. */
TIDY_EXPORT void TIDY_CALL tidyBufPutByte( TidyBuffer* buf, byte bv );

/** Get byte from end of buffer */
TIDY_EXPORT int TIDY_CALL  tidyBufPopByte( TidyBuffer* buf );


/** Get byte from front of buffer.  Increment input offset. */
TIDY_EXPORT int TIDY_CALL  tidyBufGetByte( TidyBuffer* buf );

/** At end of buffer? */
TIDY_EXPORT Bool TIDY_CALL tidyBufEndOfInput( TidyBuffer* buf );

/** Put a byte back into the buffer.  Decrement input offset. */
TIDY_EXPORT void TIDY_CALL tidyBufUngetByte( TidyBuffer* buf, byte bv );


/**************
   TIDY
**************/

/* Forward declarations
*/

/** Initialize a buffer input source */
TIDY_EXPORT void TIDY_CALL tidyInitInputBuffer( TidyInputSource* inp, TidyBuffer* buf );

/** Initialize a buffer output sink */
TIDY_EXPORT void TIDY_CALL tidyInitOutputBuffer( TidyOutputSink* outp, TidyBuffer* buf );

#ifdef __cplusplus
}
#endif
#endif /* __TIDY_BUFFIO_H__ */
