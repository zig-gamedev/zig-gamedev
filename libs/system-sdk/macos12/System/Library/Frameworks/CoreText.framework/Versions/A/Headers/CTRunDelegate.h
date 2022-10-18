/*
 *  CTRunDelegate.h
 *  CoreText
 *
 *  Copyright (c) 2006-2018 Apple Inc. All rights reserved.
 *
 */

#ifndef __CTRUNDELEGATE__
#define __CTRUNDELEGATE__

#include <CoreText/CTRun.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/* --------------------------------------------------------------------------- */
/* Run Delegate Types */
/* --------------------------------------------------------------------------- */

typedef const struct CF_BRIDGED_TYPE(id) __CTRunDelegate * CTRunDelegateRef;

/*!
    @function   CTRunDelegateGetTypeID
    @abstract   Returns the CFType of CTRunDelegate objects.
*/

CFTypeID CTRunDelegateGetTypeID( void ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Run Delegate Callbacks */
/* --------------------------------------------------------------------------- */

/*!
    @typedef    CTRunDelegateDeallocateCallback
    @abstract   The callback invoked when a CTRunDelegate is deallocated.
    
    @param      refCon
                The value supplied to CTRunDelegateCreate.
*/

typedef void (*CTRunDelegateDeallocateCallback) (
    void * refCon );

/*!
    @typedef    CTRunDelegateGetAscentCallback
    @abstract   The callback used to determine typographic ascent.
    
    @param      refCon
                The value supplied to CTRunDelegateCreate.
                
    @result     A CGFloat value indicating the typographic ascent of glyphs to
                which a run delegate pertains.
*/

typedef CGFloat (*CTRunDelegateGetAscentCallback) (
    void * refCon );

/*!
    @typedef    CTRunDelegateGetDescentCallback
    @abstract   The callback used to determine typographic descent.
    
    @param      refCon
                The value supplied to CTRunDelegateCreate.
                
    @result     A CGFloat value indicating the typographic descent of glyphs to
                which a run delegate pertains.
*/

typedef CGFloat (*CTRunDelegateGetDescentCallback) (
    void * refCon );

/*!
    @typedef    CTRunDelegateGetWidthCallback
    @abstract   The callback used to determine width.
    
    @param      refCon
                The value supplied to CTRunDelegateCreate.
                
    @result     A CGFloat value indicating the width of glyphs to which a run
                delegate pertains. A value of 0.0 indicates that the glyphs
                should not be drawn.
*/

typedef CGFloat (*CTRunDelegateGetWidthCallback) (
    void * refCon );

/*!
    @typedef    CTRunDelegateCallbacks
    @abstract   Structure containing the callbacks of a CTRunDelegate.
    
    @discussion These callbacks are provided by the owner of a run delegate and
                are used to modify glyph metrics during layout. The values
                returned by a delegate are applied to each glyph in the run(s)
                corresponding to the attribute containing that delegate.
                
    @field      version
                The version number of the callbacks being passed in as a
                parameter to CTRunDelegateCreate. This field should always 
                be set to kCTRunDelegateCurrentVersion.
                
    @field      dealloc
                The callback used when a CTRunDelegate's retain count reaches
                0 and the CTRunDelegate is deallocated. This callback may be
                NULL.
                
    @field      getAscent
                The callback used to indicate the ascent of the
                CTRunDelegate. This callback may be NULL, which is equivalent
                to a getAscent callback that always returns 0.
                
    @field      getDescent
                The callback used to indicate the descent of the
                CTRunDelegate. This callback may be NULL, which is equivalent
                to a getDescent callback that always returns 0.
                
    @field      getWidth
                The callback used to indicate the width of the
                CTRunDelegate. This callback may be NULL, which is equivalent
                to a getWidth callback that always returns 0.
*/

typedef struct
{
    CFIndex                         version;
    CTRunDelegateDeallocateCallback dealloc;
    CTRunDelegateGetAscentCallback  getAscent;
    CTRunDelegateGetDescentCallback getDescent;
    CTRunDelegateGetWidthCallback   getWidth;
} CTRunDelegateCallbacks;

/* --------------------------------------------------------------------------- */
/* Run Delegate Versions */
/* --------------------------------------------------------------------------- */

/*!
    @enum       Run Delegate Versions
    @discussion Set version field of CTRunDelegateCallbacks to kCTRunDelegateCurrentVersion
                when calling CTRunDelegateCreate.
 */
enum {
    kCTRunDelegateVersion1 = 1,
    kCTRunDelegateCurrentVersion = kCTRunDelegateVersion1
};

/* --------------------------------------------------------------------------- */
/* Run Delegate Creation */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTRunDelegateCreate
    @abstract   Creates an immutable instance of a run delegate.

    @discussion This function creates an immutable instance of a run delegate
                that can be used for reserving space in a line or for eliding the
                glyphs for a range of text altogether. 
                
    @param      callbacks
                The callbacks for this run delegate.
                
    @result     If run delegate creation was successful, this function will
                return a valid reference to an immutable CTRunDelegate
                object. Otherwise, this function will return NULL.
*/

CTRunDelegateRef _Nullable CTRunDelegateCreate(
    const CTRunDelegateCallbacks* callbacks,
    void * _Nullable refCon ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Run Delegate Access */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTRunDelegateGetRefCon
    @abstract   Returns a run delegate's refCon value.

    @discussion This function returns the refCon value that a run delegate was
                created with.
                
    @param      runDelegate
                The run delegate to be queried.
                
    @result     The refCon value of the supplied run delegate.
*/

void * CTRunDelegateGetRefCon(
    CTRunDelegateRef runDelegate ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


CF_ASSUME_NONNULL_END
CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif
