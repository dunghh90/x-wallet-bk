/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_mblm_table.c
 *	@brief	Buffer Management Data.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_MBLM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_mblm.h"

/** @addtogroup BPF_RU_MBLM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Buffer Management Data.
 *  @note   BPF internal function. 
 *  @param  None
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/

/* メモリブロック割付有無フラグ */
unsigned int  bpf_ru_mblm_assign_flag = BPF_RU_MBLM_FLG_OFF;

t_bpf_ru_mblm_systeminfo bpf_ru_mblm_system_data[BPF_RU_MBLM_BUFTYPE_NUM];

/* プロセス情報管理テーブル */
t_bpf_ru_mblm_prcinfo bpf_ru_mblm_prc_info;

/* バッファ管理情報 */
t_bpf_ru_mblm_buffer_mng *bpf_ru_mblm_buffer_mng;

unsigned int  mblm_memory_broken_detect = BPF_RU_MBLM_FLG_OFF;


/* @} */

/* @} */
