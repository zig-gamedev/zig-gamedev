//
//  GTrace.hpp
//
//  Created by bparke on 3/17/17.
//

#ifndef GTrace_hpp
#define GTrace_hpp

#include <stdatomic.h>

#include <libkern/c++/OSObject.h>
#include <IOKit/IOLib.h>
#include <IOKit/IOReturn.h>

#include "IOKit/graphics/GTraceTypes.hpp"

// Kernel clients that haven't enabled C++11 yet
#if !GTRACE_ARCHAIC_CPP
#include <osmemory>
#endif

// TODO: Provide example of GTRACE() macro use.

// ----------------------------------------------------------------------------
// Main tracing Macros
// ----------------------------------------------------------------------------

// Use this to encode t0, for automatic decoding and start/end pairing
// Function has been moved and renamed, define old macro in terms of new
#define GTFuncTag(i, t, tag) GPACKFUNCTAG(i, t, tag)

#define GTRACERAW(tracer, t0, a0, t1, a1, t2, a2, t3, a3) do{                  \
    if (static_cast<bool>(tracer)){                                            \
        (tracer)->recordToken(__LINE__,                                        \
                MAKEGTRACETAG(t0), MAKEGTRACEARG(a0),                          \
                MAKEGTRACETAG(t1), MAKEGTRACEARG(a1),                          \
                MAKEGTRACETAG(t2), MAKEGTRACEARG(a2),                          \
                MAKEGTRACETAG(t3), MAKEGTRACEARG(a3));                         \
    }                                                                          \
}while(0)

// Convenience macro, used when all args are ordinary hex data fields.
#define GTRACERAWNT(tracer, a0, a1, a2, a3)                                    \
    GTRACERAW(tracer, 0, a0, 0, a1, 0, a2, 0, a3)

#define GTRACE(tracer, fid, ft, t0, a0, t1, a1, t2, a2, t3, a3)                \
    GTRACERAW(tracer, GTFuncTag(fid, ft, t0), a0, t1, a1, t2, a2, t3, a3)

// Convenience macro, used when all as are ordinary hex data fields.
#define GTRACE_NT(tracer, fid, ft, a0, a1, a2, a3)                             \
    GTRACERAW(tracer, GTFuncTag(fid, ft, 0), a0, 0, a1, 0, a2, 0, a3)

// GTrace for calls that are slow. The GTRACE_IFSLOW_START records the current
// time, then GTRACE_IFSLOW_END will record an entry if the duration between
// start and end is > the threshold.  Note `delayat` is in absolute time units.
#define GTRACE_IFSLOW_START(tracer, fid) do {                                  \
    const uint64_t _gtrace_ ## fid ## _start_                                  \
        = ((static_cast<bool>(tracer)) ? mach_continuous_time() : 0)

// Matches the do{ from GTRACE_IFSLOW_START, must be in same scope
#define GTRACE_IFSLOW_END(tracer, fid, ft, t0, a0, t1, a1, t2, a2, delayat)    \
    if(static_cast<bool>(tracer)) {                                            \
        const uint64_t _gtrace_ifslow_now_ = mach_continuous_time();           \
        const uint64_t _gtrace_delta_                                          \
            = _gtrace_ifslow_now_ - _gtrace_ ## fid ## _start_;                \
        if(_gtrace_delta_ > (delayat))                                         \
            (tracer)->recordToken(__LINE__, GTFuncTag(fid, ft, 0),             \
                                _gtrace_delta_, t0, a0, t1, a1, t2, a2,        \
                                _gtrace_ifslow_now_);                          \
    }                                                                          \
}while(0)

// Create a pair of GTrace records but only record them if the time duration
// was > than the absolute time threshold given in GTRACE_DEFER_END
#define GTRACE_DEFER_START(tracer, t0, a0, t1, a1, t2, a2, t3, a3)             \
do{                                                                            \
    const GTraceEntry _gtrace_start_ = (static_cast<bool>(tracer))             \
        ? (tracer)->formatToken(__LINE__, t0, a0, t1, a1, t2, a2, t3, a3)      \
        : GTraceEntry()

// Matches the do{ from GTRACE_DEFER_START, must be in same block scope
#define GTRACE_DEFER_END(tracer, t0, a0, t1, a1, t2, a2, t3, a3, delayat)      \
    if (static_cast<bool>(tracer)) {                                           \
        const uint64_t _gtrace_defer_now_ = mach_continuous_time();            \
        const uint64_t _gtrace_delta_                                          \
            = _gtrace_defer_now_ - _gtrace_start_.timestamp();                 \
        if(_gtrace_delta_ > (delayat)) {                                       \
            (tracer)->recordToken(_gtrace_start_);                             \
            (tracer)->recordToken(__LINE__, t0, a0, t1, a1,                    \
                                          t2, a2, t3, a3, _gtrace_defer_now_); \
        }                                                                      \
    }                                                                          \
}while(0)

#if GTRACE_IMPL
// Macro used by IOGraphicsFamily internally, not required for third party use.
#define GTRACE_LOG_SYNCH(tracer, t0) do {                                      \
    if (static_cast<bool>(tracer)) {                                           \
        const uint32_t __gtrace_log_sync_ind__ = tracer->synchIndex();         \
        tracer->recordToken(__LINE__,                                          \
            MAKEGTRACETAG(t0),static_cast<uint64_t>(__gtrace_log_sync_ind__),  \
            0, 0, 0, 0, 0, 0);                                                 \
        IOLog("GTrace synchronization point %x\n", __gtrace_log_sync_ind__);   \
    }                                                                          \
}while(0)
#endif

class IOMemoryDescriptor;
class IOMemoryMap;
class GTraceBuffer final : public OSObject
{
    OSDeclareFinalStructors(GTraceBuffer);

    using super = OSObject;

public:

    // sizeP is an in/out variable, size will be <= 16KiB
    // The Breadcrumb function callout is a clean context that does not hold a
    // lock.
    using breadcrumb_func
        = IOReturn (*)(void* context, void* buffer, uint16_t* sizeP);

#if !GTRACE_ARCHAIC_CPP
    using shared_type = iog::OSSharedObject<GTraceBuffer>;

    /*!
     @function make
     @abstract Make a shared object of a new GTrace buffer.
     @discussion Creates an optimised lockfree algorithm for storing 64byte
                 records into a ring buffer. This buffer can be dumped by
                 `iogdiagnose` and then decoded using
                 /AppleInternal/AppleGraphicsControl/IOGDiagnoseDecode.

                 In addition to the ring buffer the client can also provide a
                 function that will copy up to 16KiB of additional data used by
                 the client's decode module. It is much better if this func is
                 LOCKFREE. If the function does not return within 1 second we
                 may abort the thread reading the data and discard the buffer.
                 Thus it is a data-looser to stall the fetch thread for too
                 long.

                 The breadcrumb_func is not called with GTraceBuffer locks
                 held.
     @param bufferName: Max 32 byte C string copied into fetch header.
     @param decoderName: Max 32 byte C string copied into fetch header, used by
            IOGDiagnoseDecode to find a decode module in
            /AppleInternal/AppleGraphicsControl/IOGDiagnose_modules directory.
     @param lineCount: Number of lines to store in ring buffer. Line count will
            be rounded up to a power of two and is bounded by
            kGTraceMinimumLineCount and kGTraceMaximumLineCount.
     @param bcf: breadcrumb_func. May be null. Function to call when the buffer
            is being fetched, to be used by your decode module. See discussion.
     @param context: Context to pass to the bcf function.
     @result OSSharedObject<GTraceBuffer> with the new created buffer. An empty
             buffer on failure.
     */
    static shared_type make(
            const char* decoderName, const char* bufferName,
            const uint32_t lineCount, breadcrumb_func bcf, void* context);

    /*! @function destroy
     @abstract Destroy a buffer shared object created with make.
     @discussion When a client is done with the gtrace buffer use
         destroy(iog::move(<your buffer>)) to destroy your shared object and
         stop further calls to the breadcrumb func. This will destroy your
         reference to the buffer. Before completing destruction the breadcrumb
         function if any will be called only once and then never called again
         after on return. The breadcrumb function is never called with any
         internal buffer locks held.  The GTraceBuffer guarantees that no
         further calls to the breadcrumb function will occur once this call
         completes.

         Note it is not necessary to destroy every copy of the shared_type
         (OSSharedObject<GTraceBuffer>), rather only call destroy when you no
         longer wish any further breadcrumb callouts during teardown of your
         master objects, essentially it is symetric with make() calls.
     @param bso An rvalue reference to the buffer shared object.
     */
    static void destroy(shared_type&& bso);
#endif // !GTRACE_ARCHAIC_CPP

#if GTRACE_ARCHAIC_CPP || GTRACE_IMPL
    /*! @function makeArchaicCpp
     @abstract Equivalent to make for archaic pre-C++11 projects. See make
               for more details.
     */
    static GTraceBuffer* makeArchaicCpp(
            const char* decoderName, const char* bufferName,
            const uint32_t lineCount, breadcrumb_func bcf, void* context)
        __attribute__((deprecated ("Use C++11 and GTraceBuffer::make.")));

    /*! @function destroyArchaicCpp
     @abstract Equivalent to destroy for archaic pre-C++11 projects. See
               destroy for more details.
     @discussion Similar to destroy, destroyArchaicCpp releases one reference
                 on the buffer. Clients must use destroyArchaicCpp once
                 (mirroring the call to makeArchaicCpp).
     */
    static void destroyArchaicCpp(GTraceBuffer *buffer)
        __attribute__((deprecated ("Use C++11 and GTraceBuffer::destroy.")));
#endif

    /*! @function formatToken
     @abstract Format a token suitable for recording.
     @discussion GTrace supports tokenized tracing.  formatToken is used to
         create a tokenized KTrace style entry suitable for inserting into the
         ring buffer. Carefully written for C++ return value optimisation.
     @param line The line number associated with the token
     @param tag1 An implementation specific tag associated with arg1
     @param tag2 An implementation specific tag associated with arg2
     @param tag3 An implementation specific tag associated with arg3
     @param tag4 An implementation specific tag associated with arg4
     @param arg1 Component/implementation specific value.
     @param arg2 Component/implementation specific value.
     @param arg3 Component/implementation specific value.
     @param arg4 Component/implementation specific value.
     @param timestamp Supplied timestamp or current MCT
     @result GTraceEntry structure filled in with given data.
     */
    GTraceEntry formatToken(const uint16_t line,
                            const uint64_t tag1, const uint64_t arg1,
                            const uint64_t tag2, const uint64_t arg2,
                            const uint64_t tag3, const uint64_t arg3,
                            const uint64_t tag4, const uint64_t arg4,
                            const uint64_t timestamp = mach_continuous_time());

    /*! @function recordToken
     @abstract Add token data to token stream.
     @discussion recordToken() takes a formatted GTraceEntry and writes it to
         the ring buffer.
     @param entry a const GTraceEntry reference to be atomically added to token
         stream.
     */
    void recordToken(const GTraceEntry& entry);

    /*! @function recordToken
     @abstract Add the token data to the token stream
     @discussion GTrace supports tokenized tracing.  recordToken is used to
         insert a tokenized KTrace style trace into the trace buffer.
     @param line The line number associated with the token
     @param tag1 An implementation specific tag associated with arg1
     @param tag2 An implementation specific tag associated with arg2
     @param tag3 An implementation specific tag associated with arg3
     @param tag4 An implementation specific tag associated with arg4
     @param arg1 Component/implementation specific value.
     @param arg2 Component/implementation specific value.
     @param arg3 Component/implementation specific value.
     @param arg4 Component/implementation specific value.
     @param timestamp Supplied timestamp or current MCT
     */
    void recordToken(const uint16_t line,
                     const uint64_t tag1, const uint64_t arg1,
                     const uint64_t tag2, const uint64_t arg2,
                     const uint64_t tag3, const uint64_t arg3,
                     const uint64_t tag4, const uint64_t arg4,
                     const uint64_t timestamp = mach_continuous_time())
    {
        recordToken(formatToken(
            line, tag1, arg1, tag2, arg2, tag3, arg3, tag4, arg4, timestamp));
    }

#if GTRACE_IMPL
    /*! @function synchIndex
     @abstract Publishes the current gtrace token index.
     @discussion  Used by Decode to synchronize between os_log and GTrace. I
                  have chosen to share the atomic next index operation though
                  it may cause decode ambiguity. It is very light weight.
     */
    inline uint32_t synchIndex() const { return nextLine(); }

    /*! @function fetch
     @abstract
         Copies token buffer into provided memory descriptor.
     @discussion
         Copies the tokens data from the internal buffers into the provided
         IOMemoryDescriptor, which must be prepared. Data is copied up to the
         buffer size or the internal buffer size (which ever is less). Once the
         data is copied a post processing step that obfuscates pointers is run.
     @param bso OSSharedObject<GTraceBuffer> (i.e. shared_type) of GTraceBuffer
     @param outDesc pointer to a prepared memory descriptor.
     @result kIOReturnSuccess if the copy was successful, else an error.
     */
    static IOReturn fetch(shared_type bso, IOMemoryDescriptor* outDesc);

#ifndef _OPEN_SOURCE_
    /*!
     @function makeUser
     @abstract Record a user land gtrace buffer.
     @discussion The block of memory provided by the client process will be
         copied out in sysdiagnose by the iogdiagnose CLI tool. User buffers do
         NOT support breadcrumb functions. However, a client process can
         designate a block of the buffer as breadcrumb space, but note no
         inter-locking will be provided between recording data into it and
         fetch. Destroy buffers in the usual way.
     @param bufMD: Block of memory provided by client process.
     @param errP: Pointer to a IOReturn return code, only written on failure.
     @result OSSharedObject<GTraceBuffer> with the new created buffer. An empty
             buffer on failure.
     */
    static shared_type makeUser(IOMemoryDescriptor *bufMD, IOReturn* errP);

protected:
    IOReturn copyOutUser(iog::OSUniqueObject<IOMemoryMap> map) const;
public:
#endif // !_OPEN_SOURCE_


    /* Getters for constant header information */
    size_t decoderName(char *name, const int len) const;  // truncates
    size_t bufferName(char *name, const int len) const;   // truncates
    uint32_t lineMask() const  { return fLineMask;  }
    uint32_t lineCount() const { return fLineCount; }
    uint32_t bufferID() const  { return fHeader.fBufferID; }

protected:
    // OSObject overrides
    bool init() APPLE_KEXT_OVERRIDE;
    void free() APPLE_KEXT_OVERRIDE;

    // Internal functions
    // See make() for details on arguments
    IOReturn init(
        const GTraceHeader& header, IOMemoryDescriptor* userBufferMD,
        breadcrumb_func bcf, const void* context);
    static shared_type makeFromHeader(
            const GTraceHeader& header, IOMemoryDescriptor* userBufferMD,
            breadcrumb_func bcf, void* context, IOReturn* errP);
    static GTraceHeader buildHeader(
            const char* decoderName, const char* bufferName,
            const uint32_t count);

    inline uint32_t getNextLine(void)
        { return atomic_fetch_add(&fNextLine, 1) & fLineMask; }

    // Complicated const casting because atomic_load will not take a const
    // pointer, problem mixing C and C++, but C++ <atomic> doesn't exist
    // in the kernel yet and probably never will as it is deep STL.
    inline uint32_t nextLine() const
        { return atomic_load(&(const_cast<GTraceBuffer*>(this)->fNextLine)); }

    IOReturn copyOut(
            iog::OSUniqueObject<IOMemoryMap> map, OSData* bcData) const;

    // APIs for IODisplayWranglerUserClients.cpp, also used by unit tests
    friend class IOGDiagnosticGTraceClient;

    /*! @function fetch
     @abstract
         Copies the token buffer into the provided memory descriptor.
         IOGDiagnosticUserClient interface
     @discussion
         Copies the tokens data from the internal buffers into the provided
         IOMemoryDescriptor, which must be prepared. Data is copied up to the
         buffer size or the internal buffer size (which ever is less). Once the
         data is copied a post processing step that obfuscates pointers is run.

         Releases buffer if the globally cached OSSharedObject is unique,
         that is it has only one outstanding reference AND the complete buffer
         was sucessfully copied into outDesc.
     @param index Index of buffer in buffer pool cache
     @param outDesc pointer to a prepared memory descriptor.
     @result kIOReturnSuccess if the copy was successful, else an error.
     */
    static IOReturn fetch(const uint32_t index, IOMemoryDescriptor* outDesc);

private:
    // Header that is copied out on demand
    GTraceHeader         fHeader;
    atomic_uint_fast32_t fNextLine;
    GTraceEntry*         fBuffer;

#ifndef _OPEN_SOURCE_
    IOMemoryDescriptor*  fUserBufferMD;
#endif // !_OPEN_SOURCE_

    breadcrumb_func      fBreadcrumbFunc;
    const void*          fBCFContext; // Context to be passed to fBreadcrumbFunc
    OSData*              fBCData;
    uint32_t             fBCActiveCount;

    uint32_t             fLineMask;
    uint32_t             fLineCount;
    bool                 fWrapped;

    // Workaround for pre-C++11 clients, which can't see OSSharedObject.
    shared_type          fArchaicCPPSharedObjectHack;
#endif // GTRACE_IMPL
};

#endif /* GTrace_h */
