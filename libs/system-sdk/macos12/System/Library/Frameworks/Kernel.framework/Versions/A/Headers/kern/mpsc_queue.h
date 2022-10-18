/*
 * Copyright (c) 2018 Apple Inc. All rights reserved.
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

#ifndef _KERN_MPSC_QUEUE_H_
#define _KERN_MPSC_QUEUE_H_


#include <sys/cdefs.h>

__BEGIN_DECLS

/*!
 * @typedef struct mpsc_queue_chain
 *
 * @brief
 * Type for the intrusive linkage used by MPSC queues.
 */
typedef struct mpsc_queue_chain {
	struct mpsc_queue_chain *_Atomic mpqc_next;
} *mpsc_queue_chain_t;

/*!
 * @typedef struct mpsc_queue_head
 *
 * @brief
 * The type for a multi-producer single-consumer queue.
 *
 * @discussion
 * MPSC queues allow for producers to not be affected by other producers or the
 * consumer. Which means in turn that having producers in interrupt context
 * does not require that other producers disable interrupts like a traditional
 * spinlock based approach would require.
 *
 * These queues shine when data is produced from the entire system and is
 * consumed from a single serial context (logging, tracing, ...).
 * mpsc_daemon_queue_t is provided as a fully ready/easy-to-use pre-packaged
 * solution for these common use cases.
 *
 * - mpsc_queue_append() can be used to append a single item
 * - mpsc_queue_append_list() can be used to append a batch of items at once.
 *
 * Functions for the consumer side assume proper serialization that is not
 * provided by the MPSC queue itself. Dequeuing doesn't require preemption
 * to be disabled.
 *
 * <h2>Algorithm</h2>
 *
 * The base of the enqueue algorithm is a single atomic exchange (first half,
 * called __mpsc_queue_append_update_tail) and a list fixup (2nd half, called
 * __mpsc_queue_append_update_prev).
 *
 * Graphically, enqueuing `X` looks like this, with each step being done
 * atomically (for the empty queue case, `tail` points to `head`):
 *
 *     | orig state          | update_tail         | update_prev         |
 *     +---------------------+---------------------+---------------------+
 *     |                     |                     |                     |
 *     | head -> e1 -> e2 -. | head -> e1 -> e2 -. | head -> e1 -> e2 -. |
 *     |                   | |                   | |                   | |
 *     |         ,- ... <--' |         ,- ... <--' |         ,- ... <--' |
 *     |         |           |         |           |         |           |
 *     |         v           |         v           |         v           |
 *     | tail -> eN -> NULL  | tail    eN -> NULL  | tail    eN          |
 *     |                     |   |                 |   |     |           |
 *     |                     |   |                 |   |     v           |
 *     |         X -> NULL   |   `---> X -> NULL   |   '---> X -> NULL   |
 *     |                     |                     |                     |
 *     +---------------------+---------------------+---------------------+
 *
 *
 * There is a small 1-instruction gap of inconsistency which makes the chosen
 * algorithm non linearizable, and requires enqueuers to disable preemption
 * during the enqueue so as not to starve the consumer forever.
 *
 * As far as memory visibility is concerned, enqueuing uses a release fence in
 * update_tail which pairs with memory fences in mpsc_queue_dequeue_batch().
 *
 * Note: as far as the data structure in memory, its layout is equivalent to
 *       a BSD <sys/queue.h> STAILQ. However because of this inconsistency
 *       window and memory ordering concerns, it is incorrect to use STAILQ
 *       macros on an MPSC queue.
 */
typedef struct mpsc_queue_head {
	struct mpsc_queue_chain mpqh_head;
	struct mpsc_queue_chain *_Atomic mpqh_tail;
} *mpsc_queue_head_t;

/*!
 * @macro MPSC_QUEUE_INITIALIZER
 *
 * @brief
 * Macro to use in static initializers for mpsc queues.
 *
 * @param head
 * The name of the variable to initialize.
 */
#define MPSC_QUEUE_INITIALIZER(head)   { .mpqh_tail = &(head).mpqh_head }


__END_DECLS

#endif /* _KERN_MPSC_QUEUE_H_ */
