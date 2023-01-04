/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_EXIT.c
 *	@brief	Inter Thread Communication Management Library(THREAD).
 *	        Exit Thread Function.
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
 *  @brief  Inter Thread Communication Management Thread Exit.
 *  @note   API function. 
 *  @param  None
 *  @return None
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
void BPF_RU_ITCM_THREAD_EXIT(void)
{
	/********************************************
	* thread exit processing.                  *
	********************************************/
	pthread_exit(NULL);
	return;
}
/* @} */

/* @} */
