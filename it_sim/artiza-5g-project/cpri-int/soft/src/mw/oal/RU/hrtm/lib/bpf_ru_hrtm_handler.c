/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_hrtm_handler.c
 *	@brief	Timer Management Timer Handler.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_HRTM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_hrtm.h"

/** @addtogroup BPF_RU_HRTM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Timer Management Timer Handler.
 *  @note   HRTM internal function. 
 *  @param  arg          [in]  handler execution parameter
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
void bpf_ru_hrtm_handler(sigval_t arg)
{
	int ret;
	unsigned int			hrtm_id;
	void (*exec)(bpf_ru_hrtm_key_inf_t*,unsigned int);
	bpf_ru_hrtm_key_inf_t	e_keyinf;

	/* タイマ識別子引継ぎ */
	hrtm_id = arg.sival_int;
	if((hrtm_id < HRTMID_MIN) || (hrtm_id > di_tim_max)){
		return;
	}
	
	/********************************************
	* mutex lock processing.                    *
	********************************************/
	ret = pthread_mutex_lock(&bpf_ru_hrtm_mutexobj);
	if( ret != 0 ){
		return;
	}
	/* CancelFlgが立っていたら何もしない	*/
	if( di_tim_tbl_p->info[hrtm_id].cancelFlg == HRTM_FLG_ON ){
		pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
		return;
	}
	
	if(di_tim_tbl_p->info[hrtm_id].id_next != HRTM_USING){
		pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
		return;
	}
	exec=di_tim_tbl_p->info[hrtm_id].entry;
	memcpy(&e_keyinf,&di_tim_tbl_p->info[hrtm_id].keyinf,sizeof(e_keyinf));
	e_keyinf.timerId_os = di_tim_tbl_p->info[hrtm_id].timerId;
	if(di_tim_tbl_p->info[hrtm_id].type == BPF_RU_HRTM_REG_ONCE){
		;	/* do nothing */
	}else{
		/* BPF_RU_HRTM_REG_PERIODIC */
		di_tim_tbl_p->info[hrtm_id].keyinf.seqno++;
		(void)clock_gettime( CLOCK_MONOTONIC_RAW , &(di_tim_tbl_p->info[hrtm_id].keyinf.tcount) );
	}
	/********************************************
	* entry execution processing.               *
	********************************************/
	(*exec)(&e_keyinf,hrtm_id);
	/********************************************
	* mutex unlock processing.                  *
	********************************************/
	pthread_mutex_unlock(&bpf_ru_hrtm_mutexobj);
	return;
}
/* @} */

/* @} */
