/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_l_ru_hrtm.h
 *  @brief  Definitions for BPF hrtm library
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_L_RU_HRTM_H_
#define _BPF_L_RU_HRTM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_RU_HRTM.h"
#define BPF_RU_HRTM_OK        0    /* OK */
#define BPF_RU_HRTM_NG        1    /* NG */
#define BPF_RU_HRTM_ERR_ASSIGN 2   /* ASSIGN NG */


/* Function Prototype */
int bpf_ru_hrtm_register2(
			bpf_ru_hrtm_notify_type_t,	/* notify type */
			unsigned int,				/* time val msec */
			bpf_ru_hrtm_key_inf_t*,		/* keyinf */
										/* entry(keyinf,timerid) */
			void (*entry)(bpf_ru_hrtm_key_inf_t*, unsigned int),
			unsigned int*,				/* timerid */
			int*);						/* errno */

int bpf_ru_hrtm_cancel2(unsigned int,int*,unsigned int);
int bpf_ru_hrtm_assign(unsigned int);

/********************************************* EOF ******************************************************************/
#endif /* _BPF_L_RU_HRTM_H_ */

