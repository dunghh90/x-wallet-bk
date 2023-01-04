/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_hrtm_get.c
 *	@brief	Timer Management Timer Get.
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
 *  @brief  Timer Management Timer Get.
 *  @note   HRTM internal function. 
 *  @param  timerid      [out]  timer id
 *  @return result code
 *  @retval BPF_RU_HRTM_COMPLETE  normal end
 *  @retval BPF_RU_HRTM_ERR_BUSY  resource busy error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_hrtm_get(unsigned int* timid)
{
	/********************************************
	* timer id get processing.                  *
	********************************************/
	if(di_tim_tbl_p->next == HRTM_LAST){
		/* ALL BUSY!! */
		return BPF_RU_HRTM_ERR_BUSY;
	}

	*timid = di_tim_tbl_p->next;
	di_tim_tbl_p->usenum ++;

	if(di_tim_tbl_p->info[*timid].id_next == HRTM_LAST){
		
		/* 捕捉した結果 ALL BUSYとなった場合 */
		/* 払い出したIDNEXTがLAST識別であればNEXT/TAILともにLASTとしておく */
		
		di_tim_tbl_p->next = HRTM_LAST;
		di_tim_tbl_p->tail = HRTM_LAST;
		
	} else {
		/* 捕捉した結果 ALL BUSYではない場合 */
		
		di_tim_tbl_p->next = di_tim_tbl_p->info[*timid].id_next;
	}

	/* 払い出したIDのIDNEXTは使用中にしておく */
	di_tim_tbl_p->info[*timid].id_next = HRTM_USING;

	return BPF_RU_HRTM_COMPLETE;
}
/* @} */

/* @} */
