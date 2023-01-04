/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_hrtm_init.c
 *	@brief	Timer Management Timer Initialize.
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
 *  @brief  Timer Management Timer Initialize.
 *  @note   HRTM internal function. 
 *  @param  process_desc [in]  process descriptor
 *  @param  ipc_key      [in]  IPC key for management table
 *  @param  ipc_key      [in]  IPC key for information table
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
void bpf_ru_hrtm_init(unsigned int process_desc,key_t ipc_key_mng,key_t ipc_key_info)
{
	unsigned int counter;
	
	/********************************************
	* timer table initialize processing.        *
	********************************************/
	memset(di_tim_tbl_p,0,sizeof(t_bpf_ru_hrtm_tim_tbl));
	memset(di_tim_info_p,0,sizeof(t_bpf_ru_hrtm_tim_info) * (di_tim_max + 1));
	
	di_tim_tbl_p->process_desc	= process_desc;
	di_tim_tbl_p->tim_max_id	= di_tim_max;
	di_tim_tbl_p->tim_ipc_key	= ipc_key_mng;
	di_tim_tbl_p->info_ipc_key	= ipc_key_info;
	di_tim_tbl_p->next			= HRTMID_MIN;
	di_tim_tbl_p->tail			= di_tim_max;
	di_tim_tbl_p->usenum		= 0;
	di_tim_tbl_p->info			= di_tim_info_p;

	di_tim_tbl_p->info[0].id_next = HRTM_USING;         /* [0]は使用しない */

	for(counter=HRTMID_MIN;counter<di_tim_max;counter++){
		di_tim_tbl_p->info[counter].id_next = counter + 1;
	}

	di_tim_tbl_p->info[di_tim_max].id_next = HRTM_LAST; /* [di_tim_max]は最終 */	
}
/* @} */

/* @} */
