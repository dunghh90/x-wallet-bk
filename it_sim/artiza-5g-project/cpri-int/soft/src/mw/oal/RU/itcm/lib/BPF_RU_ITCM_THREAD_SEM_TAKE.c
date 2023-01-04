/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_SEM_TAKE.c
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
#include "bpf_i_ru_itcm.h"
#include "bpf_i_ru_itcm_macro.h"

/** @addtogroup BPF_RU_ITCM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Semaphore Take.
 *  @note   API function. 
 *  @param  thr_sem_num  [in]  semaphore number
 *  @param  thr_sem_flag [in]  wait flag
 *  @param  timeout      [in]  timer value
 *  @return result code
 *  @retval BPF_RU_ITCM_OK           normal end
 *  @retval BPF_RU_ITCM_NG           abnormal end
 *  @retval BPF_RU_ITCM_EPARAM       parameter error
 *  @retval BPF_RU_ITCM_ENOSEMNUM    no semaphore number error
 *  @retval BPF_RU_ITCM_ENOSEMGET    semaphore get error
 *  @retval BPF_RU_ITCM_ETIMEOUT     timeout error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_ITCM_THREAD_SEM_TAKE(unsigned int thr_sem_num, int thr_sem_flag, struct bpf_timespec *timeout)
{
	/***********************************************************
	* semaphore give processing.(call internal routine)        *
	***********************************************************/
	return bpf_ru_itcm_thread_sem_take2(ITCM_SEM_USER, thr_sem_num, thr_sem_flag, timeout);
	
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Semaphore Take2.
 *  @note   BPF internal function. 
 *  @param  thr_sem_type [in]  semaphore type
 *  @param  thr_sem_num  [in]  semaphore number
 *  @param  thr_sem_flag [in]  wait flag
 *  @param  timeout      [in]  timer value
 *  @return result code
 *  @retval BPF_RU_ITCM_OK           normal end
 *  @retval BPF_RU_ITCM_NG           abnormal end
 *  @retval BPF_RU_ITCM_EPARAM       parameter error
 *  @retval BPF_RU_ITCM_ENOSEMNUM    no semaphore number error
 *  @retval BPF_RU_ITCM_ENOSEMGET    semaphore get error
 *  @retval BPF_RU_ITCM_ETIMEOUT     timeout error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_itcm_thread_sem_take2(enum itcm_sem_type sem_type, unsigned int thr_sem_num, 
									int thr_sem_flag,struct bpf_timespec *timeout)
{
	int ret;
	struct bpf_timespec dtime;
	pthread_mutex_t *sem = NULL;
	
	
	/********************************************
	* parameter check processing.               *
	********************************************/
	/* Invalid cases, Macro return  */
	ITCM_SEM_TYPECHECK(sem_type);		/* Semaphore Type Check   */
										
	ITCM_SEM_NUMCHECK(thr_sem_num);		/* Semaphore number Check */
	
	ITCM_SEM_FLAGCHECK(thr_sem_flag);	/* Semaphore wait flag Check */
	
	ITCM_SEM_USEDCHECK(thr_sem_num);	/* Semaphore status Check */
	
	
	/********************************************
	* semaphore lock processing.                *
	********************************************/
	sem = &(itcm_sem.data[thr_sem_num].sem);
	
	if( thr_sem_flag == BPF_RU_ITCM_WAIT ){
		if( timeout == NULL ){	
			/* Infinite waiting  */
			ret = pthread_mutex_lock(sem);
		} else {
			/* Get current time */
			clock_gettime(CLOCK_REALTIME, (struct timespec *)&dtime);
			/* Set time val */
            ITCM_TS_ADD(&dtime, &dtime, timeout);
			ret = pthread_mutex_timedlock(sem, (struct timespec *)&dtime);
		}
		if( ret != ITCM_THREAD_FUNC_OK ){
			
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
								"pthread_mutex_(timed)lock failed. errno = %d",
								ret);
			
			if( ret == ETIMEDOUT ){ /* Time Out */
				return BPF_RU_ITCM_ETIMEOUT;
			} else {
				return BPF_RU_ITCM_NG;
			}
		}
	} else {
		/* BPF_RU_ITCM_NOWAIT */
		ret = pthread_mutex_trylock(sem);
		if( ret != ITCM_THREAD_FUNC_OK ){ /* NG */
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
								"pthread_mutex_trylock failed. errno = %d",
								ret);
			if( ret == EBUSY ){
				/* Somebody has already get the mutex */
				return BPF_RU_ITCM_ENOSEMGET;
			}
			return BPF_RU_ITCM_NG;
		}
	}
	
	/* Success */
    return BPF_RU_ITCM_OK;
}
/* @} */

/* @} */
