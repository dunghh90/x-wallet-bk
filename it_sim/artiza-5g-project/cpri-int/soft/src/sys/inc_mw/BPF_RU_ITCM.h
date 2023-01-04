/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RU_ITCM.h
 *  @brief  Definitions for BPF_RU_ITCM API
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_RU_ITCM_H_
#define _BPF_RU_ITCM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"
#include <pthread.h>

/********************************************************************************************************************/
/* definitions                                                                                                      */
/********************************************************************************************************************/

/* Return code */

#define BPF_RU_ITCM_OK			0
#define BPF_RU_ITCM_NG			1
#define BPF_RU_ITCM_EPARAM		2
#define BPF_RU_ITCM_EINTR		3
#define BPF_RU_ITCM_ENOTHR		4
#define BPF_RU_ITCM_ENOSEMGET	5
#define BPF_RU_ITCM_ENOSPACE	6
#define BPF_RU_ITCM_ESIZEOVER	7
#define BPF_RU_ITCM_ENOQNUM		8
#define BPF_RU_ITCM_EADDR		9
#define BPF_RU_ITCM_ETIMEOUT	10
#define BPF_RU_ITCM_EQFULL		11
#define BPF_RU_ITCM_ENOMSG		12
#define BPF_RU_ITCM_ENOSEMNUM	13

/* Scheduling policies */
#define BPF_RU_ITCM_SCHED_OTHER		SCHED_OTHER
#define BPF_RU_ITCM_SCHED_FIFO		SCHED_FIFO
#define BPF_RU_ITCM_SCHED_RR		SCHED_RR

/* Flags for THREAD_DELAY */
#define BPF_RU_ITCM_CNT			1
#define BPF_RU_ITCM_RET			2

/* ITCM Wait Flags */
#define BPF_RU_ITCM_WAIT		3
#define BPF_RU_ITCM_NOWAIT		4


/* Function prototype */

/* THREAD */
int BPF_RU_ITCM_THREAD_CREATE(unsigned long int *, int, int, unsigned int, void *, void *, int *);
int BPF_RU_ITCM_THREAD_DELAY(struct bpf_timespec *, int, struct bpf_timespec *);
void BPF_RU_ITCM_THREAD_EXIT(void);
int BPF_RU_ITCM_THREAD_DELETE(unsigned long int);
int BPF_RU_ITCM_THREAD_IDSELF(unsigned long int *);

/* MSGQ */
int BPF_RU_ITCM_THREAD_MSGQ_SEND(unsigned int, unsigned int, const void *, int, struct bpf_timespec *);
int BPF_RU_ITCM_THREAD_MSGQ_RECEIVE(unsigned int, unsigned int, void **, unsigned int *, int, struct bpf_timespec *);
int BPF_RU_ITCM_THREAD_MSGQ_NUMMSGS(unsigned int, unsigned int*);

/* MUTEX */
int BPF_RU_ITCM_THREAD_SEM_TAKE(unsigned int, int, struct bpf_timespec *);
int BPF_RU_ITCM_THREAD_SEM_GIVE(unsigned int);


/********************************************* EOF ******************************************************************/
#endif /* _BPF_RU_ITCM_H_ */

