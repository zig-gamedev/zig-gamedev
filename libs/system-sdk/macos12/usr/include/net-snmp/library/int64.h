#ifndef INT64_INCLUDED
#define INT64_INCLUDED

#ifdef __cplusplus
extern          "C" {
#endif

    typedef struct counter64 NS_U64;
 
#define I64CHARSZ 21

    void            divBy10(NS_U64, NS_U64 *, unsigned int *);
    void            multBy10(NS_U64, NS_U64 *);
    void            incrByU16(NS_U64 *, unsigned int);
    void            incrByU32(NS_U64 *, unsigned int);
    NETSNMP_IMPORT
    void            zeroU64(NS_U64 *);
    int             isZeroU64(const NS_U64 *);
    NETSNMP_IMPORT
    void            printU64(char *, const NS_U64 *);
    NETSNMP_IMPORT
    void            printI64(char *, const NS_U64 *);
    int             read64(NS_U64 *, const char *);
    NETSNMP_IMPORT
    void            u64Subtract(const NS_U64 * pu64one, const NS_U64 * pu64two,
                                NS_U64 * pu64out);
    void            u64Incr(NS_U64 * pu64out, const NS_U64 * pu64one);
    void            u64UpdateCounter(NS_U64 * pu64out, const NS_U64 * pu64one,
                                     const NS_U64 * pu64two);
    void            u64Copy(NS_U64 * pu64one, const NS_U64 * pu64two);

    int             netsnmp_c64_check_for_32bit_wrap(NS_U64 *old_val, NS_U64 *new_val,
                                                     int adjust);
    NETSNMP_IMPORT
    int             netsnmp_c64_check32_and_update(struct counter64 *prev_val,
                                                   struct counter64 *new_val,
                                                   struct counter64 *old_prev_val,
                                                   int *need_wrap_check);

#ifdef __cplusplus
}
#endif
#endif
