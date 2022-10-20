/*	NSByteOrder.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObjCRuntime.h>
#import <CoreFoundation/CFByteOrder.h>

enum {
    NS_UnknownByteOrder = CFByteOrderUnknown,
    NS_LittleEndian = CFByteOrderLittleEndian,
    NS_BigEndian = CFByteOrderBigEndian
};

NS_INLINE long NSHostByteOrder(void) {
    return CFByteOrderGetCurrent();
}

NS_INLINE unsigned short NSSwapShort(unsigned short inv) {
    return CFSwapInt16(inv);
}

NS_INLINE unsigned int NSSwapInt(unsigned int inv) {
    return CFSwapInt32(inv);
}

NS_INLINE unsigned long NSSwapLong(unsigned long inv) {
#if __LP64__
    return CFSwapInt64(inv);
#else
    return CFSwapInt32(inv);
#endif
}

NS_INLINE unsigned long long NSSwapLongLong(unsigned long long inv) {
    return CFSwapInt64(inv);
}

NS_INLINE unsigned short NSSwapBigShortToHost(unsigned short x) {
    return CFSwapInt16BigToHost(x);
}

NS_INLINE unsigned int NSSwapBigIntToHost(unsigned int x) {
    return CFSwapInt32BigToHost(x);
}

NS_INLINE unsigned long NSSwapBigLongToHost(unsigned long x) {
#if __LP64__
    return CFSwapInt64BigToHost(x);
#else
    return CFSwapInt32BigToHost(x);
#endif
}

NS_INLINE unsigned long long NSSwapBigLongLongToHost(unsigned long long x) {
    return CFSwapInt64BigToHost(x);
}

NS_INLINE unsigned short NSSwapHostShortToBig(unsigned short x) {
    return CFSwapInt16HostToBig(x);
}

NS_INLINE unsigned int NSSwapHostIntToBig(unsigned int x) {
    return CFSwapInt32HostToBig(x);
}

NS_INLINE unsigned long NSSwapHostLongToBig(unsigned long x) {
#if __LP64__
    return CFSwapInt64HostToBig(x);
#else
    return CFSwapInt32HostToBig(x);
#endif
}

NS_INLINE unsigned long long NSSwapHostLongLongToBig(unsigned long long x) {
    return CFSwapInt64HostToBig(x);
}

NS_INLINE unsigned short NSSwapLittleShortToHost(unsigned short x) {
    return CFSwapInt16LittleToHost(x);
}

NS_INLINE unsigned int NSSwapLittleIntToHost(unsigned int x) {
    return CFSwapInt32LittleToHost(x);
}

NS_INLINE unsigned long NSSwapLittleLongToHost(unsigned long x) {
#if __LP64__
    return CFSwapInt64LittleToHost(x);
#else
    return CFSwapInt32LittleToHost(x);
#endif
}

NS_INLINE unsigned long long NSSwapLittleLongLongToHost(unsigned long long x) {
    return CFSwapInt64LittleToHost(x);
}

NS_INLINE unsigned short NSSwapHostShortToLittle(unsigned short x) {
    return CFSwapInt16HostToLittle(x);
}

NS_INLINE unsigned int NSSwapHostIntToLittle(unsigned int x) {
    return CFSwapInt32HostToLittle(x);
}

NS_INLINE unsigned long NSSwapHostLongToLittle(unsigned long x) {
#if __LP64__
    return CFSwapInt64HostToLittle(x);
#else
    return CFSwapInt32HostToLittle(x);
#endif
}

NS_INLINE unsigned long long NSSwapHostLongLongToLittle(unsigned long long x) {
    return CFSwapInt64HostToLittle(x);
}


typedef struct {unsigned int v;} NSSwappedFloat;
typedef struct {unsigned long long v;} NSSwappedDouble;

NS_INLINE NSSwappedFloat NSConvertHostFloatToSwapped(float x) {
    union fconv {
	float number;
	NSSwappedFloat sf;
    };
    return ((union fconv *)&x)->sf;
}

NS_INLINE float NSConvertSwappedFloatToHost(NSSwappedFloat x) {
    union fconv {
	float number;
	NSSwappedFloat sf;
    };
    return ((union fconv *)&x)->number;
}

NS_INLINE NSSwappedDouble NSConvertHostDoubleToSwapped(double x) {
    union dconv {
	double number;
	NSSwappedDouble sd;
    };
    return ((union dconv *)&x)->sd;
}

NS_INLINE double NSConvertSwappedDoubleToHost(NSSwappedDouble x) {
    union dconv {
	double number;
	NSSwappedDouble sd;
    };
    return ((union dconv *)&x)->number;
}

NS_INLINE NSSwappedFloat NSSwapFloat(NSSwappedFloat x) {
    x.v = NSSwapInt(x.v);
    return x;
}

NS_INLINE NSSwappedDouble NSSwapDouble(NSSwappedDouble x) {
    x.v = NSSwapLongLong(x.v);
    return x;
}

#if defined(__BIG_ENDIAN__)

NS_INLINE double NSSwapBigDoubleToHost(NSSwappedDouble x) {
    return NSConvertSwappedDoubleToHost(x);
}

NS_INLINE float NSSwapBigFloatToHost(NSSwappedFloat x) {
    return NSConvertSwappedFloatToHost(x);
}

NS_INLINE NSSwappedDouble NSSwapHostDoubleToBig(double x) {
    return NSConvertHostDoubleToSwapped(x);
}

NS_INLINE NSSwappedFloat NSSwapHostFloatToBig(float x) {
    return NSConvertHostFloatToSwapped(x);
}

NS_INLINE double NSSwapLittleDoubleToHost(NSSwappedDouble x) {
    return NSConvertSwappedDoubleToHost(NSSwapDouble(x));
}

NS_INLINE float NSSwapLittleFloatToHost(NSSwappedFloat x) {
    return NSConvertSwappedFloatToHost(NSSwapFloat(x));
}

NS_INLINE NSSwappedDouble NSSwapHostDoubleToLittle(double x) {
    return NSSwapDouble(NSConvertHostDoubleToSwapped(x));
}

NS_INLINE NSSwappedFloat NSSwapHostFloatToLittle(float x) {
    return NSSwapFloat(NSConvertHostFloatToSwapped(x));
}

#elif defined(__LITTLE_ENDIAN__)

NS_INLINE double NSSwapBigDoubleToHost(NSSwappedDouble x) {
    return NSConvertSwappedDoubleToHost(NSSwapDouble(x));
}

NS_INLINE float NSSwapBigFloatToHost(NSSwappedFloat x) {
    return NSConvertSwappedFloatToHost(NSSwapFloat(x));
}

NS_INLINE NSSwappedDouble NSSwapHostDoubleToBig(double x) {
    return NSSwapDouble(NSConvertHostDoubleToSwapped(x));
}

NS_INLINE NSSwappedFloat NSSwapHostFloatToBig(float x) {
    return NSSwapFloat(NSConvertHostFloatToSwapped(x));
}

NS_INLINE double NSSwapLittleDoubleToHost(NSSwappedDouble x) {
    return NSConvertSwappedDoubleToHost(x);
}

NS_INLINE float NSSwapLittleFloatToHost(NSSwappedFloat x) {
    return NSConvertSwappedFloatToHost(x);
}

NS_INLINE NSSwappedDouble NSSwapHostDoubleToLittle(double x) {
    return NSConvertHostDoubleToSwapped(x);
}

NS_INLINE NSSwappedFloat NSSwapHostFloatToLittle(float x) {
    return NSConvertHostFloatToSwapped(x);
}

#else
#error Do not know the endianess of this architecture
#endif

