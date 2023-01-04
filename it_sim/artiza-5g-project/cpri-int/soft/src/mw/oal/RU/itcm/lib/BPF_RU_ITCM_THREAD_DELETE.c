/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_DELETE.c
 *	@brief	Inter Thread Communication Management Library(THREAD).
 *	        Delete Thread Function.
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
 *  @brief  Inter Thread Communication Management Thread Delete.
 *  @note   API function. 
 *  @param  thread       [in]  thread id
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_ENOTHR    thread id error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_ITCM_THREAD_DELETE(unsigned long int thread)
{
	int ret;
	
	/********************************************
	* thread delete processing.                 *
	********************************************/
	ret = pthread_kill(thread, SIGRTMIN); 
	if( ret != ITCM_THREAD_FUNC_OK ){ /* 失敗 */
		if( ret == ESRCH ){
			/* スレッドがない */
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
								"Thread ID %lu is not found",thread);
			return BPF_RU_ITCM_ENOTHR;
		} else {
			/* それ以外の要因でNG */
			/* それ以外はシグナル番号がおかしいくらいしかないのでここは */
			/* まず通らない */
			bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
								"Unexpected error occurred. errno = %d, thread ID = %lu",
								ret, thread);
			return BPF_RU_ITCM_NG;
    	}
	}
	return BPF_RU_ITCM_OK;
}
/* @} */

/* @} */
