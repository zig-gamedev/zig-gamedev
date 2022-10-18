/* Portions of this file are subject to the following copyright(s).  See
 * the Net-SNMP's COPYING file for more details and other copyrights
 * that may apply:
 */
/*
 * Portions of this file are copyrighted by:
 * Copyright © 2003 Sun Microsystems, Inc. All rights reserved.
 * Use is subject to license terms specified in the COPYING file
 * distributed with the Net-SNMP package.
 */

/*
 *  CMU compatability definitions and structures.
 *
 *    (since the UCD package split from the CMU one
 *     about 8 years ago, that's probably enough time
 *     for people to convert any outstanding code!)
 */

#ifndef NET_SNMP_CMU_COMPAT_H
#define NET_SNMP_CMU_COMPAT_H

#ifdef __cplusplus
extern          "C" {
#endif

    /*
     * ============================
     * *
     * * Enumerations and definitions
     * *
     * * ============================ 
     */
#include <net-snmp/definitions.h>
    /*
     * From parse.h 
     */
#define ACCESS_READONLY		MIB_ACCESS_READONLY
#define ACCESS_READWRITE	MIB_ACCESS_READWRITE
#define ACCESS_WRITEONLY	MIB_ACCESS_WRITEONLY
#define ACCESS_NOACCESS		MIB_ACCESS_NOACCESS
#define ACCESS_NOTIFY		MIB_ACCESS_NOTIFY
#define ACCESS_CREATE		MIB_ACCESS_CREATE
#define STATUS_MANDATORY	MIB_STATUS_MANDATORY
#define STATUS_OPTIONAL		MIB_STATUS_OPTIONAL
#define STATUS_OBSOLETE		MIB_STATUS_OBSOLETE
#define STATUS_DEPRECATED	MIB_STATUS_DEPRECATED
#define STATUS_CURRENT		MIB_STATUS_CURRENT
    /*
     * From smnp_impl.h 
     */
#define INTEGER	    ASN_INTEGER
#define STRING	    ASN_OCTET_STR
#define OBJID	    ASN_OBJECT_ID
#define NULLOBJ	    ASN_NULL
#define BITSTRING   ASN_BIT_STR /* HISTORIC - don't use */
#define IPADDRESS   ASN_IPADDRESS
#define COUNTER	    ASN_COUNTER
#define GAUGE	    ASN_GAUGE
#define UNSIGNED    ASN_UNSIGNED
#define TIMETICKS   ASN_TIMETICKS
#define ASNT_OPAQUE ASN_OPAQUE
#define NSAP	    ASN_NSAP
#define COUNTER64   ASN_COUNTER64
#define UINTEGER    ASN_UINTEGER
    /*
     * From snmp.h 
     */
#define GET_REQ_MSG	    SNMP_MSG_GET
#define GETNEXT_REQ_MSG	    SNMP_MSG_GETNEXT
#define GET_RSP_MSG	    SNMP_MSG_RESPONSE
#define SET_REQ_MSG	    SNMP_MSG_SET
#define TRP_REQ_MSG	    SNMP_MSG_TRAP
#define BULK_REQ_MSG	    SNMP_MSG_GETBULK
#define INFORM_REQ_MSG	    SNMP_MSG_INFORM
#define TRP2_REQ_MSG	    SNMP_MSG_TRAP2
#define REPORT_RSP_MSG	    SNMP_MSG_REPORT
    /*
     * since CMU V1.5 
     */
#define SNMP_PDU_GET	    SNMP_MSG_GET
#define SNMP_PDU_GETNEXT    SNMP_MSG_GETNEXT
#define SNMP_PDU_RESPONSE   SNMP_MSG_RESPONSE
#define SNMP_PDU_SET        SNMP_MSG_SET
#define SNMP_PDU_GETBULK    SNMP_MSG_GETBULK
#define SNMP_PDU_INFORM     SNMP_MSG_INFORM
#define SNMP_PDU_V2TRAP     SNMP_MSG_TRAP2
#define SNMP_PDU_REPORT     SNMP_MSG_REPORT
#define SNMP_TRAP_AUTHENTICATIONFAILURE SNMP_TRAP_AUTHFAIL
#define SMI_INTEGER     ASN_INTEGER
#define SMI_STRING      ASN_OCTET_STR
#define SMI_OBJID       ASN_OBJECT_ID
#define SMI_NULLOBJ     ASN_NULL
#define SMI_IPADDRESS   ASN_IPADDRESS
#define SMI_COUNTER32	ASN_COUNTER
#define SMI_GAUGE32	ASN_GAUGE
#define SMI_UNSIGNED32  SMI_GAUGE32
#define SMI_TIMETICKS   ASN_TIMETICKS
#define SMI_OPAQUE      ASN_OPAQUE
#define SMI_COUNTER64   ASN_COUNTER64
    /*
     * ============================
     * *
     * *     Function declarations
     * *     and global variables
     * *
     * * ============================ 
     */
extern int      snmp_dump_packet;       /* from snmp_api.h */
extern int      quick_print;    /* from snmp_api.h */

#include <net-snmp/types.h>     /* for snmp_pdu & snmp_session */

int             mib_TxtToOid(char *, oid **, size_t *); /* from snmp.h */
int             mib_OidToTxt(oid *, size_t, char *, size_t);    /* from snmp.h */
u_char         *cmu_snmp_parse(netsnmp_session * session, netsnmp_pdu *pdu, u_char * data, size_t length);      /* from snmp.h */




        /*
         * ============================
         * *
         * * MIB-II data structures
         * *    (from mib.h) 
         * *
         * * ============================ 
         */


struct mib_system {
    char            sysDescr[32];       /* textual description */
    u_char          sysObjectID[16];    /* OBJECT IDENTIFIER of system */
    u_char          ObjIDLen;   /* length of sysObjectID */
    u_int           sysUpTime;  /* Uptime in 100/s of a second */
};

struct mib_interface {
    int             ifNumber;   /* number of interfaces */
};

struct mib_ifEntry {
    int             ifIndex;    /* index of this interface  */
    char            ifDescr[32];        /* english description of interface */
    int             ifType;     /* network type of device   */
    int             ifMtu;      /* size of largest packet in bytes  */
    u_int           ifSpeed;    /* bandwidth in bits/sec    */
    u_char          ifPhysAddress[11];  /* interface's address */
    u_char          PhysAddrLen;        /* length of physAddr */
    int             ifAdminStatus;      /* desired state of interface */
    int             ifOperStatus;       /* current operational status */
    u_int           ifLastChange;       /* value of sysUpTime when current state entered */
    u_int           ifInOctets; /* number of octets received on interface */
    u_int           ifInUcastPkts;      /* number of unicast packets delivered */
    u_int           ifInNUcastPkts;     /* number of broadcasts or multicasts */
    u_int           ifInDiscards;       /* number of packets discarded with no error */
    u_int           ifInErrors; /* number of packets containing errors */
    u_int           ifInUnknownProtos;  /* number of packets with unknown protocol */
    u_int           ifOutOctets;        /* number of octets transmitted */
    u_int           ifOutUcastPkts;     /* number of unicast packets sent */
    u_int           ifOutNUcastPkts;    /* number of broadcast or multicast pkts */
    u_int           ifOutDiscards;      /* number of packets discarded with no error */
    u_int           ifOutErrors;        /* number of pkts discarded with an error */
    u_int           ifOutQLen;  /* number of packets in output queue */
};

struct mib_atEntry {
    int             atIfIndex;  /* interface on which this entry maps */
    u_char          atPhysAddress[11];  /* physical address of destination */
    u_char          PhysAddressLen;     /* length of atPhysAddress */
    u_int           atNetAddress;       /* IP address of physical address */
};

struct mib_ip {
    int             ipForwarding;       /* 1 if gateway, 2 if host */
    int             ipDefaultTTL;       /* default TTL for pkts originating here */
    u_int           ipInReceives;       /* no. of IP packets received from interfaces */
    u_int           ipInHdrErrors;      /* number of pkts discarded due to header errors */
    u_int           ipInAddrErrors;     /* no. of pkts discarded due to bad address */
    u_int           ipForwDatagrams;    /* number pf pkts forwarded through this entity */
    u_int           ipInUnknownProtos;  /* no. of local-addressed pkts w/unknown proto */
    u_int           ipInDiscards;       /* number of error-free packets discarded */
    u_int           ipInDelivers;       /* number of datagrams delivered to upper level */
    u_int           ipOutRequests;      /* number of IP datagrams originating locally */
    u_int           ipOutDiscards;      /* number of error-free output IP pkts discarded */
    u_int           ipOutNoRoutes;      /* number of IP pkts discarded due to no route */
    int             ipReasmTimeout;     /* seconds fragment is held awaiting reassembly */
    u_int           ipReasmReqds;       /* no. of fragments needing reassembly (here) */
    u_int           ipReasmOKs; /* number of fragments reassembled */
    u_int           ipReasmFails;       /* number of failures in IP reassembly */
    u_int           ipFragOKs;  /* number of datagrams fragmented here */
    u_int           ipFragFails;        /* no. pkts unable to be fragmented here */
    u_int           ipFragCreates;      /* number of IP fragments created here */
};

struct mib_ipAddrEntry {
    u_int           ipAdEntAddr;        /* IP address of this entry */
    int             ipAdEntIfIndex;     /* IF for this entry */
    u_int           ipAdEntNetMask;     /* subnet mask of this entry */
    int             ipAdEntBcastAddr;   /* read the MIB for this one */
    int 	    ipAdEntReasmMaxSize;
		/* max size for dg reassembly   {ipAddrEntry 5} */
};

struct mib_ipRouteEntry {
    u_int           ipRouteDest;        /* destination IP addr for this route */
    int             ipRouteIfIndex;     /* index of local IF for this route */
    int             ipRouteMetric1;     /* Primary routing metric */
    int             ipRouteMetric2;     /* Alternate routing metric */
    int             ipRouteMetric3;     /* Alternate routing metric */
    int             ipRouteMetric4;     /* Alternate routing metric */
    u_int           ipRouteNextHop;     /* IP addr of next hop */
    int             ipRouteType;        /* Type of this route */
    int             ipRouteProto;       /* How this route was learned */
    int             ipRouteAge; /* No. of seconds since updating this route */
};

struct mib_icmp {
    u_int           icmpInMsgs; /* Total of ICMP msgs received */
    u_int           icmpInErrors;       /* Total of ICMP msgs received with errors */
    u_int           icmpInDestUnreachs;
    u_int           icmpInTimeExcds;
    u_int           icmpInParmProbs;
    u_int           icmpInSrcQuenchs;
    u_int           icmpInRedirects;
    u_int           icmpInEchos;
    u_int           icmpInEchoReps;
    u_int           icmpInTimestamps;
    u_int           icmpInTimestampReps;
    u_int           icmpInAddrMasks;
    u_int           icmpInAddrMaskReps;
    u_int           icmpOutMsgs;
    u_int           icmpOutErrors;
    u_int           icmpOutDestUnreachs;
    u_int           icmpOutTimeExcds;
    u_int           icmpOutParmProbs;
    u_int           icmpOutSrcQuenchs;
    u_int           icmpOutRedirects;
    u_int           icmpOutEchos;
    u_int           icmpOutEchoReps;
    u_int           icmpOutTimestamps;
    u_int           icmpOutTimestampReps;
    u_int           icmpOutAddrMasks;
    u_int           icmpOutAddrMaskReps;
};

struct mib_tcp {
    int             tcpRtoAlgorithm;    /* retransmission timeout algorithm */
    int             tcpRtoMin;  /* minimum retransmission timeout (mS) */
    int             tcpRtoMax;  /* maximum retransmission timeout (mS) */
    int             tcpMaxConn; /* maximum tcp connections possible */
    u_int           tcpActiveOpens;     /* number of SYN-SENT -> CLOSED transitions */
    u_int           tcpPassiveOpens;    /* number of SYN-RCVD -> LISTEN transitions */
    u_int           tcpAttemptFails;    /*(SYN-SENT,SYN-RCVD)->CLOSED or SYN-RCVD->LISTEN */
    u_int           tcpEstabResets;     /* (ESTABLISHED,CLOSE-WAIT) -> CLOSED */
    u_int           tcpCurrEstab;       /* number in ESTABLISHED or CLOSE-WAIT state */
    u_int           tcpInSegs;  /* number of segments received */
    u_int           tcpOutSegs; /* number of segments sent */
    u_int           tcpRetransSegs;     /* number of retransmitted segments */
};

struct mib_tcpConnEntry {
    int             tcpConnState;       /* State of this connection */
    u_int           tcpConnLocalAddress;        /* local IP address for this connection */
    int             tcpConnLocalPort;   /* local port for this connection */
    u_int           tcpConnRemAddress;  /* remote IP address for this connection */
    int             tcpConnRemPort;     /* remote port for this connection */
};

struct mib_udp {
    u_int           udpInDatagrams;     /* No. of UDP datagrams delivered to users */
    u_int           udpNoPorts; /* No. of UDP datagrams to port with no listener */
    u_int           udpInErrors;        /* No. of UDP datagrams unable to be delivered */
    u_int           udpOutDatagrams;    /* No. of UDP datagrams sent from this entity */
};

struct mib_egp {
    u_int           egpInMsgs;  /* No. of EGP msgs received without error */
    u_int           egpInErrors;        /* No. of EGP msgs received with error */
    u_int           egpOutMsgs; /* No. of EGP msgs sent */
    u_int           egpOutErrors;       /* No. of (outgoing) EGP msgs dropped due to error */
};

struct mib_egpNeighEntry {
    int             egpNeighState;      /* local EGP state with this entry's neighbor */
    u_int           egpNeighAddr;       /* IP address of this entry's neighbor */
};

#ifdef __cplusplus
}
#endif
#endif                          /* NET_SNMP_CMU_COMPAT_H */
