/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_hrtm_del.c
 *	@brief	Timer Management Timer Delete.
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
 *  @brief  Timer Management Delete.
 *  @note   HRTM internal function. 
 *  @param  timerid      [in]  timer id
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE  normal end
 *  @retval BPF_RU_HRTM_ERR_NOENT no entry id
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_hrtm_del(unsigned int timid)
{
	if(di_tim_tbl_p->info[timid].id_next != HRTM_USING){
		return BPF_RU_HRTM_ERR_NOENT;
	}
	/*******************************************************
	* free the timer id and set the reuse link processing. *
	********************************************************/
	
	if(di_tim_tbl_p->tail == HRTM_LAST){
		/* ALL BUSY状態での解放時 */
		/* NEXT/TAIL共に自身を設定 */
		di_tim_tbl_p->tail = timid;
		di_tim_tbl_p->next = timid;
	} else {
		/* 空きありでの解放時 */
		/* 旧TAILのNEXTIDへ自身を繋げ、TAILへ自身を設定 */
		di_tim_tbl_p->info[di_tim_tbl_p->tail].id_next = timid;
		di_tim_tbl_p->tail = timid;
	}
	
	/* 自身のIDNEXTへLAST識別を設定しておく */
	di_tim_tbl_p->info[timid].id_next = HRTM_LAST;
	
	/* 自身のKeyInfo情報を削除する	*/
	memset( &di_tim_tbl_p->info[timid].keyinf, 0, sizeof( bpf_ru_hrtm_key_inf_t ) );
	
	di_tim_tbl_p->usenum --;

	return BPF_RU_HRTM_COMPLETE;
}
/* @} */

/* @} */
