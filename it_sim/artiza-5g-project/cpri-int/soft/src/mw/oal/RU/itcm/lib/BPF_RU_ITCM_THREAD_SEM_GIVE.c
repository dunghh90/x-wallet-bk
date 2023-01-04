/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_SEM_GIVE.c
 *	@brief	Inter Thread Communication Management Library(SEM).
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_ITCM /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_ru_itcm.h"
#include "bpf_i_ru_itcm_macro.h"


/** @addtogroup BPF_RU_ITCM
 * @{ */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Semaphore Give.
 *  @note   API function. 
 *  @param  thr_sem_num  [in]  semaphore number
 *  @return result code
 *  @retval BPF_RU_ITCM_OK           normal end
 *  @retval BPF_RU_ITCM_NG           abnormal end
 *  @retval BPF_RU_ITCM_EPARAM       parameter error
 *  @retval BPF_RU_ITCM_ENOSEMNUM    no semaphore number error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_ITCM_THREAD_SEM_GIVE(unsigned int thr_sem_num)
{
	/***********************************************************
	* semaphore give processing.(call internal routine)        *
	***********************************************************/
	return bpf_ru_itcm_thread_sem_give2(ITCM_SEM_USER, thr_sem_num);
	
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Semaphore Give2.
 *  @note   BPF internal function. 
 *  @param  sem_type     [in]  semaphore type
 *  @param  thr_sem_num  [in]  semaphore number
 *  @return result code
 *  @retval BPF_RU_ITCM_OK           normal end
 *  @retval BPF_RU_ITCM_NG           abnormal end
 *  @retval BPF_RU_ITCM_EPARAM       parameter error
 *  @retval BPF_RU_ITCM_ENOSEMNUM    no semaphore number error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_itcm_thread_sem_give2(enum itcm_sem_type sem_type, unsigned int thr_sem_num)
{
	pthread_mutex_t *sem = NULL;
	int ret;
	
	/********************************************
	* parameter check processing.               *
	********************************************/
	/* Invalid cases, Macro return  */
	ITCM_SEM_TYPECHECK(sem_type);		/* Semaphore Type Check   */
	
	ITCM_SEM_NUMCHECK(thr_sem_num);		/* Semaphore number Check */
	
	ITCM_SEM_USEDCHECK(thr_sem_num);	/* Semaphore status Check */
	
	/********************************************
	* semaphore unlock processing.              *
	********************************************/

	sem = &(itcm_sem.data[thr_sem_num].sem);
	
	ret = pthread_mutex_unlock(sem);
	if( ret != ITCM_THREAD_FUNC_OK ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
							"pthread_mutex_unlock failed. semafo num = %u",
							thr_sem_num);
		return BPF_RU_ITCM_NG;
	}
	
	return BPF_RU_ITCM_OK;
}
/* @} */


/* @} */
