/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RU_HRTM.h
 *  @brief  Definitions for BPF_RU_HRTM API
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#ifndef _BPF_RU_HRTM_H_
#define _BPF_RU_HRTM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"

/********************************************************************************************************************/
/* definitions                                                                                                      */
/********************************************************************************************************************/

/* Return code */
#define BPF_RU_HRTM_COMPLETE	0
#define BPF_RU_HRTM_ERR_PARAM	1
#define BPF_RU_HRTM_ERR_BUSY	2
#define BPF_RU_HRTM_ERR_REG		3
#define BPF_RU_HRTM_ERR_NOENT	4
#define BPF_RU_HRTM_ERR_CAN		5

/* notify_type */
typedef enum{
	BPF_RU_HRTM_REG_ONCE		=   1,
	BPF_RU_HRTM_REG_PERIODIC
}bpf_ru_hrtm_notify_type_t;

/* keyinf */
typedef struct{
	unsigned int		keyinf1;
	unsigned int		keyinf2;
	unsigned int		queueID;
	unsigned int		eventNo;
	timer_t				timerId_os;
	unsigned int		seqno;
	unsigned int		uservalue;
	unsigned int		reserve;
	struct bpf_timespec	tcount;
}bpf_ru_hrtm_key_inf_t;

/* Function prototype */
int BPF_RU_HRTM_REGISTER (
			bpf_ru_hrtm_notify_type_t,	/* notify type */
			unsigned int,				/* timer ID		*/
			unsigned int,				/* time val msec */
			unsigned int,				/* event no		*/
			unsigned int,				/* queue id		*/
			int*);						/* errno */


/* Function prototype */
int BPF_RU_HRTM_REGISTER_ID (
			bpf_ru_hrtm_notify_type_t,	/* notify type */
			unsigned int,				/* timer ID		*/
			unsigned int,				/* time val msec */
			unsigned int,				/* event no		*/
			unsigned int,				/* queue id		*/
			int*,						/* errno */
			unsigned int*,				/* bpf timer id		*/
			timer_t*,					/* os timer id		*/
			unsigned int				/* user value		*/
			);

int BPF_RU_HRTM_CANCEL(unsigned int,int*);
int BPF_RU_HRTM_CANCEL_FORCE(unsigned int,int*);



/********************************************* EOF ******************************************************************/
#endif /* _BPF_RU_HRTM_H_ */

