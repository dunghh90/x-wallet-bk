/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_itcm_data.c
 *	@brief	Inter Thread Communication Management Data.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_ITCM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_itcm.h"

/** @addtogroup BPF_RU_ITCM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Data.
 *  @note   BPF internal function. 
 *  @param  None
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/

	/********************************************
	* global data definition.                   *
	********************************************/
	struct itcm_sem_tbl_t itcm_sem;
	struct itcm_msg_tbl_t itcm_msg;
	t_bpf_ru_itcm_sg_data* bpf_ru_itcm_sg_data_p;
	t_bpf_ru_itcm_sg_data sg;
	
	/* 未使用のメッセージデータリスト */
	struct itcm_msg_data_t itcm_msg_flist = {0};
	
	/* スレッド間キュー統計情報 共有メモリアドレス */
    struct itcm_msgq_statis_t *itcm_msgq_statis_p=NULL;
	
/* @} */

/* @} */
