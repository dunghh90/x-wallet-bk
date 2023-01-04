/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_com_log_data.c
 *	@brief	COM LOG Data.
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_COM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_com.h"

/** @addtogroup BPF_RU_HRTM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  COM LOG Data.
 *  @note   BPF internal function. 
 *  @param  None
 *  @return None
 */
/********************************************************************************************************************/

    /********************************************
    * global data definition.                   *
    ********************************************/
unsigned int  di_log_assign_flag = LOG_FLG_OFF;
t_bpf_ru_log_data* di_log_data_p;

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */