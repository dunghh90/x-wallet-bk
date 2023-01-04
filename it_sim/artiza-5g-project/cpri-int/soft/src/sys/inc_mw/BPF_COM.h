/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_COM.h
 *  @brief  Definitions for BPF Common
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#ifndef _BPF_COM_H_
#define _BPF_COM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#ifndef _LITS_OSE_H
#include <netinet/in.h>
#endif
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "f_rrh_inc.h"


/********************************************************************************************************************/
/* External Definition                                                                                              */
/********************************************************************************************************************/

/* Related to time */
#define bpf_timeval timeval
#define bpf_timespec timespec
#define bpf_tm tm

/********************************************************************************************************************/
/* Function Prototype                                                                                               */
/********************************************************************************************************************/
void BPF_HM_COM_OUTPUT_LOG(unsigned char *buffer , unsigned int max_size ,unsigned int *wite_size);

/********************************************* EOF ******************************************************************/
#endif /* _BPF_COM_H_ */

