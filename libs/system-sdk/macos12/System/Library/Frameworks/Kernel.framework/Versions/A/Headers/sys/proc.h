/*
 * Copyright (c) 2000-2018 Apple Inc. All rights reserved.
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
/* Copyright (c) 1995, 1997 Apple Computer, Inc. All Rights Reserved */
/*-
 * Copyright (c) 1986, 1989, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)proc.h	8.15 (Berkeley) 5/19/95
 */

#ifndef _SYS_PROC_H_
#define _SYS_PROC_H_

#include <sys/appleapiopts.h>
#include <sys/cdefs.h>
#include <sys/select.h>                 /* For struct selinfo. */
#include <sys/queue.h>
#include <sys/lock.h>
#include <sys/param.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/kernel_types.h>
#include <uuid/uuid.h>
#include <mach/boolean.h>




__BEGIN_DECLS

extern proc_t kernproc;

extern int proc_is_classic(proc_t p);
extern bool proc_is_exotic(proc_t p);
extern bool proc_is_alien(proc_t p);
proc_t current_proc_EXTERNAL(void);

extern int      msleep(void *chan, lck_mtx_t *mtx, int pri, const char *wmesg, struct timespec * ts );
extern int      msleep0(void *chan, lck_mtx_t *mtx, int pri, const char *wmesg, int timo, int (*continuation)(int));
extern void     wakeup(void *chan);
extern void wakeup_one(caddr_t chan);

/* proc kpis */
/* this routine returns the pid of the current process */
extern int proc_selfpid(void);
/* this routine returns the pid of the parent of the current process */
extern int proc_selfppid(void);
/* this routine returns the csflags of the current process */
extern uint64_t proc_selfcsflags(void);
/* this routine populates the given flags param with the csflags of the given process. Returns 0 on success, -1 on error. */
extern int proc_csflags(proc_t p, uint64_t* flags);
/* this routine returns sends a signal signum to the process identified by the pid */
extern void proc_signal(int pid, int signum);
/* this routine checks whether any signal identified by the mask are pending in the  process identified by the pid. The check is  on all threads of the process. */
extern int proc_issignal(int pid, sigset_t mask);
/* this routine returns 1 if the pid1 is inferior of pid2 */
extern int proc_isinferior(int pid1, int pid2);
/* this routine copies the process's name of the executable to the passed in buffer. It
 * is always null terminated. The size of the buffer is to be passed in as well. This
 * routine is to be used typically for debugging
 */
void proc_name(int pid, char * buf, int size);
/* returns the 32-byte name if it exists, otherwise returns the 16-byte name */
extern char *proc_best_name(proc_t p);
/* This routine is simillar to proc_name except it returns for current process */
void proc_selfname(char * buf, int size);

/* find a process with a given pid. This comes with a reference which needs to be dropped by proc_rele */
extern proc_t proc_find(int pid);
/* find a process with a given process identity */
extern proc_t proc_find_ident(struct proc_ident const *i);
/* returns a handle to current process which is referenced. The reference needs to be dropped with proc_rele */
extern proc_t proc_self(void);
/* releases the held reference on the process */
extern int proc_rele(proc_t p);
/* returns the pid of the given process */
extern int proc_pid(proc_t);
/* returns the pid of the parent of a given process */
extern int proc_ppid(proc_t);
/* returns the original pid of the parent of a given process */
extern int proc_original_ppid(proc_t);
/* returns the start time of the given process */
extern int proc_starttime(proc_t, struct timeval *);
/* returns whether the given process is on simulated platform */
extern boolean_t proc_is_simulated(const proc_t);
/* returns the platform (macos, ios, watchos, tvos, ...) of the given process */
extern uint32_t proc_platform(const proc_t);
/* returns the minimum sdk version used by the current process */
extern uint32_t proc_min_sdk(proc_t);
/* returns the sdk version used by the current process */
extern uint32_t proc_sdk(proc_t);
/* returns 1 if the process is marked for no remote hangs */
extern int proc_noremotehang(proc_t);
/* returns 1 if the process is marked for force quota */
extern int proc_forcequota(proc_t);
/* returns 1 if the process is chrooted */
extern int proc_chrooted(proc_t);
/* returns TRUE if a sync EXC_RESOURCE should be sent for the process */
extern boolean_t proc_send_synchronous_EXC_RESOURCE(proc_t p);

/* this routine returns 1 if the process is running with a 64bit address space, else 0 */
extern int proc_is64bit(proc_t);
/* this routine returns 1 if the process is running with a 64bit register state, else 0 */
extern int proc_is64bit_data(proc_t);
/* this routine returns 1 if the process is initproc */
extern int proc_isinitproc(proc_t);
/* is this process exiting? */
extern int proc_exiting(proc_t);
/* returns whether the process has started down proc_exit() */
extern int proc_in_teardown(proc_t);
/* this routine returns error if the process is not one with super user privileges */
int proc_suser(proc_t p);
/* returns the cred assicaited with the process; temporary api */
kauth_cred_t proc_ucred(proc_t p);
/* returns 1 if the process is tainted by uid or gid changes,e else 0 */
extern int proc_issetugid(proc_t p);

extern int proc_tbe(proc_t);

/*!
 *  @function proc_gettty
 *  @abstract Copies the associated tty vnode for a given process if it exists. The caller needs to decrement the iocount of the vnode.
 *  @return 0 on success. ENOENT if the process has no associated TTY. EINVAL if arguments are NULL or vnode_getwithvid fails.
 */
extern int proc_gettty(proc_t p, vnode_t *vp);

/* this routine populates the associated tty device for a given process if it exists, returns 0 on success or else returns EINVAL */
extern int proc_gettty_dev(proc_t p, dev_t *dev);

/*!
 *  @function proc_selfpgrpid
 *  @abstract Get the process group id for the current process, as with proc_pgrpid().
 *  @return pgrpid of current process.
 */
pid_t proc_selfpgrpid(void);

/*!
 *  @function proc_pgrpid
 *  @abstract Get the process group id for the passed-in process.
 *  @param p Process whose pgrpid to grab.
 *  @return pgrpid for "p".
 */
pid_t proc_pgrpid(proc_t p);

/*!
 *  @function proc_sessionid
 *  @abstract Get the process session id for the passed-in process.
 *  @param p Process whose session id to grab.
 *  @return session id of current process.
 */
pid_t proc_sessionid(proc_t p);




__END_DECLS



#endif  /* !_SYS_PROC_H_ */
