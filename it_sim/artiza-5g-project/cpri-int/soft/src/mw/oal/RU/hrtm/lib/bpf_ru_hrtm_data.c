/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_hrtm_data.c
 *	@brief	Timer Management Data.
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
 *  @brief  Timer Management Data.
 *  @note   BPF internal function. 
 *  @param  None
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/

    /********************************************
    * global data definition.                   *
    ********************************************/
unsigned int di_hrtm_assign_flag = HRTM_FLG_OFF;
t_bpf_ru_hrtm_tim_tbl *di_tim_tbl_p = NULL;
t_bpf_ru_hrtm_tim_info *di_tim_info_p = NULL;
unsigned int di_tim_max = 0;
t_bpf_ru_hrtm_sg_data *bpf_ru_hrtm_sg_data=NULL;

pthread_mutex_t bpf_ru_hrtm_mutexobj = PTHREAD_MUTEX_INITIALIZER;

/* @} */

/* @} */
