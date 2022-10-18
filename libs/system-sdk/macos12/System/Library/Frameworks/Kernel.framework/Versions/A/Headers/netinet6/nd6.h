/*
 * Copyright (c) 2000-2020 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _NETINET6_ND6_H_
#define _NETINET6_ND6_H_
#include <sys/appleapiopts.h>
#include <net/net_kev.h>

/* see net/route.h, or net/if_inarp.h */
#ifndef RTF_ANNOUNCE
#define RTF_ANNOUNCE    RTF_PROTO2
#endif

#include <sys/queue.h>


#define ND6_LLINFO_PURGE        -3
#define ND6_LLINFO_NOSTATE      -2
/*
 * We don't need the WAITDELETE state any more, but we keep the definition
 * in a comment line instead of removing it. This is necessary to avoid
 * unintentionally reusing the value for another purpose, which might
 * affect backward compatibility with old applications.
 * (20000711 jinmei@kame.net)
 */
/* #define	ND6_LLINFO_WAITDELETE	-1 */
#define ND6_LLINFO_INCOMPLETE   0
#define ND6_LLINFO_REACHABLE    1
#define ND6_LLINFO_STALE        2
#define ND6_LLINFO_DELAY        3
#define ND6_LLINFO_PROBE        4


struct nd_ifinfo {
	u_int32_t linkmtu;              /* LinkMTU */
	u_int32_t maxmtu;               /* Upper bound of LinkMTU */
	u_int32_t basereachable;        /* BaseReachableTime */
	u_int32_t reachable;            /* Reachable Time */
	u_int32_t retrans;              /* Retrans Timer */
	u_int32_t flags;                /* Flags */
	int recalctm;                   /* BaseReacable re-calculation timer */
	u_int8_t chlim;                 /* CurHopLimit */
	u_int8_t receivedra;
	/* the following 3 members are for privacy extension for addrconf */
	u_int8_t randomseed0[8]; /* upper 64 bits of SHA1 digest */
	u_int8_t randomseed1[8]; /* lower 64 bits (usually the EUI64 IFID) */
	u_int8_t randomid[8];   /* current random ID */
};

#define ND6_IFF_PERFORMNUD              0x1
#define ND6_IFF_PROXY_PREFIXES          0x20
#define ND6_IFF_IGNORE_NA               0x40
#define ND6_IFF_REPLICATED              0x100   /* sleep proxy registered */
#define ND6_IFF_DAD                     0x200   /* Perform DAD on the interface */

extern int dad_enhanced;
#define ND6_DAD_ENHANCED_DEFAULT        1

struct in6_nbrinfo {
	char ifname[IFNAMSIZ];  /* if name, e.g. "en0" */
	struct in6_addr addr;   /* IPv6 address of the neighbor */
	long    asked;          /* # of queries already sent for this addr */
	int     isrouter;       /* if it acts as a router */
	int     state;          /* reachability state */
	int     expire;         /* lifetime for NDP state transition */
};


#define DRLSTSIZ 10
#define PRLSTSIZ 10

struct  in6_drlist {
	char ifname[IFNAMSIZ];
	struct {
		struct  in6_addr rtaddr;
		u_char  flags;
		u_short rtlifetime;
		u_long  expire;
		u_short if_index;
	} defrouter[DRLSTSIZ];
};


/* valid values for stateflags */
#define NDDRF_INSTALLED 0x01     /* installed in the routing table */
#define NDDRF_IFSCOPE   0x02     /* installed as a scoped route */
#define NDDRF_STATIC    0x04     /* for internal use only */
#define NDDRF_MAPPED    0x08     /* Default router addr is mapped to a different one for routing */
#define NDDRF_INELIGIBLE     0x10     /* Default router entry is ineligible for default router selection */

struct  in6_defrouter {
	struct  sockaddr_in6 rtaddr;
	u_char  flags;
	u_char  stateflags;
	u_short rtlifetime;
	u_long  expire;
	u_short if_index;
};


struct  in6_prlist {
	char ifname[IFNAMSIZ];
	struct {
		struct  in6_addr prefix;
		struct prf_ra raflags;
		u_char  prefixlen;
		u_char  origin;
		u_long  vltime;
		u_long  pltime;
		u_long  expire;
		u_short if_index;
		u_short advrtrs; /* number of advertisement routers */
		struct  in6_addr advrtr[DRLSTSIZ]; /* XXX: explicit limit */
	} prefix[PRLSTSIZ];
};


struct in6_prefix {
	struct  sockaddr_in6 prefix;
	struct prf_ra raflags;
	u_char  prefixlen;
	u_char  origin;
	u_long  vltime;
	u_long  pltime;
	u_long  expire;
	u_int32_t flags;
	int refcnt;
	u_short if_index;
	u_short advrtrs; /* number of advertisement routers */
	/* struct sockaddr_in6 advrtr[] */
};


struct  in6_ondireq {
	char ifname[IFNAMSIZ];
	struct {
		u_int32_t linkmtu;      /* LinkMTU */
		u_int32_t maxmtu;       /* Upper bound of LinkMTU */
		u_int32_t basereachable; /* BaseReachableTime */
		u_int32_t reachable;    /* Reachable Time */
		u_int32_t retrans;      /* Retrans Timer */
		u_int32_t flags;        /* Flags */
		int recalctm;           /* BaseReacable re-calculation timer */
		u_int8_t chlim;         /* CurHopLimit */
		/* Number of routers learned on the  interface */
		u_int8_t receivedra;
		/*
		 * The current collision count value
		 * being used for secure address generation.
		 */
		u_int8_t collision_count;
	} ndi;
};

struct  in6_ndireq {
	char ifname[IFNAMSIZ];
	struct nd_ifinfo ndi;
};

struct  in6_ndifreq {
	char ifname[IFNAMSIZ];
	u_long ifindex;
};

#define MAX_RTR_SOLICITATION_DELAY      1       /* 1sec */
#define RTR_SOLICITATION_INTERVAL       4       /* 4sec */


/* Prefix status */
#define NDPRF_ONLINK            0x1
#define NDPRF_DETACHED          0x2
#define NDPRF_STATIC            0x100
#define NDPRF_IFSCOPE           0x1000
#define NDPRF_PRPROXY           0x2000

/* protocol constants */
#define MAX_RTR_SOLICITATION_DELAY      1       /* 1sec */
#define RTR_SOLICITATION_INTERVAL       4       /* 4sec */
#define MAX_RTR_SOLICITATIONS           3

#define ND6_INFINITE_LIFETIME           0xffffffff
#define ND6_MAX_LIFETIME                0x7fffffff





/*
 *	@function nd6_lookup_ipv6
 *	@discussion This function will check the routing table for a cached
 *		neighbor discovery entry or trigger an neighbor discovery query
 *		to resolve the IPv6 address to a link-layer address.
 *		nd entries are stored in the routing table. This function will
 *		lookup the IPv6 destination in the routing table. If the
 *		destination requires forwarding to a gateway, the route of the
 *		gateway will be looked up. The route entry is inspected to
 *		determine if the link layer destination address is known. If
 *		unknown, neighbor discovery will be used to resolve the entry.
 *	@param interface The interface the packet is being sent on.
 *	@param ip6_dest The IPv6 destination of the packet.
 *	@param ll_dest On output, the link-layer destination.
 *	@param ll_dest_len The length of the buffer for ll_dest.
 *	@param hint Any routing hint passed down from the protocol.
 *	@param packet The packet being transmitted.
 *	@result May return an error such as EHOSTDOWN or ENETUNREACH. If
 *		this function returns EJUSTRETURN, the packet has been queued
 *		and will be sent when the address is resolved. If any other
 *		value is returned, the caller is responsible for disposing of
 *		the packet.
 */
extern errno_t nd6_lookup_ipv6(ifnet_t interface,
    const struct sockaddr_in6 *ip6_dest, struct sockaddr_dl *ll_dest,
    size_t ll_dest_len, route_t hint, mbuf_t packet);


/* nd6_send.c */
#endif /* _NETINET6_ND6_H_ */
