/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_CREATE.c
 *	@brief	Inter Thread Communication Management Library(THREAD).
 *	        Thread Create Function.
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
 *  @brief  Inter Thread Communication Management Thread Create.
 *  @note   API function. 
 *  @param  thread       [out] thread id
 *  @param  policy       [in]  scheduling policy
 *  @param  priority     [in]  thread priority
 *  @param  stack_size   [in]  thread stack size
 *  @param  start_routine[in]  start routine
 *  @param  arg          [in]  arg
 *  @param  errcd        [out] detail error code
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_EPARAM    parameter error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_ITCM_THREAD_CREATE(	unsigned long int *thread, int policy, int priority, unsigned int stacksize,
								void *start_routine, void *arg, int *errcd)
{
	pthread_attr_t thr_attr;
	struct sched_param thr_param;
	
	
	/********************************************
	* parameter check processing.               *
	********************************************/

	if( start_routine == NULL ){
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
						    "start_routine check NG. start_routine = NULL");
		
		return BPF_RU_ITCM_EPARAM;
	}
	if( policy == BPF_RU_ITCM_SCHED_OTHER ){
	    /* ポリシーがSCHED_OTHERの時はプライオリティは0のみ */
		if( priority != ITCM_THREAD_PRIO_ZERO ){
			bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
							    "priority range NG. policy = %d, priority = %d",
							    policy, priority);
			
			return BPF_RU_ITCM_EPARAM;
		}
	} else if( policy == BPF_RU_ITCM_SCHED_FIFO || policy == BPF_RU_ITCM_SCHED_RR ){
		/* ポリシーがSCHED_FIFOかSCHED_RRの時はプライオリティは1-99 */
		if( (priority < ITCM_THREAD_PRIO_MIN) || (priority > ITCM_THREAD_PRIO_MAX) ){
			bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
							    "priority range NG. policy = %d, priority = %d",
							    policy, priority);
			return BPF_RU_ITCM_EPARAM;
		}
	} else {
		/* 上記以外のポリシーが渡されたらNG */
			bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
							    "policy range NG. policy = %d, priority  = %d",
							    policy, priority);
			return BPF_RU_ITCM_EPARAM;
	}
	
	/********************************************
	* thread attribute initialize processing.   *
	********************************************/
	*errcd = pthread_attr_init(&thr_attr);
	if( *errcd != ITCM_THREAD_FUNC_OK ){
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
						    "pthread_attr_init failed. errno = %d",
						    *errcd);
		return BPF_RU_ITCM_NG;
	}
	
	/********************************************
	* thread state set processing.              *
	********************************************/
	/* デタッチ状態設定 */
	*errcd = pthread_attr_setdetachstate(&thr_attr, PTHREAD_CREATE_DETACHED);
	if( *errcd != ITCM_THREAD_FUNC_OK ){
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
							"pthread_attr_setdetachstate failed. errno = %d",
							*errcd);
		return BPF_RU_ITCM_NG;
	}

	/********************************************
	* thread stack size set processing.         *
	********************************************/
	if( stacksize == ITCM_THREAD_STACK_ZERO ){
		stacksize = ITCM_THREAD_SYS_STACKSIZE;
	}
	else if(stacksize < PTHREAD_STACK_MIN){
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
							"INFO:Stack size short,PTHREAD_STACK_MIN:%ubytes in:%ubytes adjust:%ubytes",
							PTHREAD_STACK_MIN,
							stacksize,
							ITCM_THREAD_SYS_STACKSIZE
							);
		stacksize = ITCM_THREAD_SYS_STACKSIZE;
	}else{
		;
	}
	
	*errcd = pthread_attr_setstacksize(&thr_attr, stacksize);
	if( *errcd != ITCM_THREAD_FUNC_OK ){
		bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,
							"pthread_attr_setstacksize failed. errno = %d stack=%u",
							*errcd,
							stacksize);
		return BPF_RU_ITCM_NG;
	}
	/********************************************
	* thread policy set processing.             *
	********************************************/
	*errcd = pthread_attr_setschedpolicy(&thr_attr, policy);
	if( *errcd != ITCM_THREAD_FUNC_OK ){
		/* ポリシーが異なることはありえないのでスーパーユーザ権限がない場合 */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
							"pthread_attr_setschedpolicy failed. errno = %d",
							*errcd);
		return BPF_RU_ITCM_NG;
	}

	/********************************************
	* thread priority set processing.           *
	********************************************/
	thr_param.sched_priority = priority;
	*errcd = pthread_attr_setschedparam(&thr_attr, &thr_param);
	if( *errcd != ITCM_THREAD_FUNC_OK ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
							"pthread_attr_setschedparam failed. errno = %d",
							*errcd);
		return BPF_RU_ITCM_NG;
	}

	/* スケジューリングポリシーを親から継承しないよう設定 */
	*errcd = pthread_attr_setinheritsched(&thr_attr, PTHREAD_EXPLICIT_SCHED);
	if( *errcd != ITCM_THREAD_FUNC_OK ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
							"pthread_attr_setinheritsched failed. errno = %d",
							*errcd);
		return BPF_RU_ITCM_NG;
	}

	/********************************************
	* thread create processing.                 *
	********************************************/
	*errcd = pthread_create(thread, &thr_attr, start_routine, arg);
	if( *errcd != ITCM_THREAD_FUNC_OK ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
							"pthread_create failed. errno = %d",
							*errcd);
		return BPF_RU_ITCM_NG;
	}
	
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"## THREAD_CREATE ## [%lu] pl:%d pr:%d sz:%d sr:%p\n",
			&thread,policy,priority,stacksize,start_routine);
	
	return BPF_RU_ITCM_OK;

}
/* @} */


/* @} */

/* @} */
