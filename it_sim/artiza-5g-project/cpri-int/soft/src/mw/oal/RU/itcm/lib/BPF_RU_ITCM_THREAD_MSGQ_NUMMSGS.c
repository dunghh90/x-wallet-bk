/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_ITCM_THREAD_MSGQ_NUMMSGS.c
 *	@brief	Inter Thread Communication Management Library(MSGQ).
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
 *  @brief  Inter Thread Communication Management Number of Messages Get.
 *  @note   API function. 
 *  @param  thr_q_num    [in]  queue number
 *  @param  msgnum       [out] number of messages
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_EPARAM    parameter error
 *  @retval BPF_RU_ITCM_ENOQNUM   queue number error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_ITCM_THREAD_MSGQ_NUMMSGS(unsigned int thr_q_num, unsigned int *msgnum)
{
	/***********************************************************
	* number of message get processing.(call internal routine) *
	***********************************************************/
	return bpf_ru_itcm_thread_msgq_nummsgs2(ITCM_MSGQ_USER, thr_q_num, msgnum);

}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Inter Thread Communication Management Number of Messages Get2.
 *  @note   BPF internal function. 
 *  @param  q_type       [in]  queue type
 *  @param  thr_q_num    [in]  queue number
 *  @param  msgnum       [out] number of messages
 *  @return result code
 *  @retval BPF_RU_ITCM_OK        normal end
 *  @retval BPF_RU_ITCM_NG        abnormal end
 *  @retval BPF_RU_ITCM_EPARAM    parameter error
 *  @retval BPF_RU_ITCM_ENOQNUM   queue number error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_itcm_thread_msgq_nummsgs2(enum itcm_msgq_type q_type, 
									unsigned int thr_q_num, unsigned int *msgnum)
{
	
	/********************************************
	* parameter check processing.               *
	********************************************/
	/* Invalid cases, Macro return  */
	ITCM_MSGQ_TYPECHECK(q_type);			/* Msgq Type Check        */
	
	ITCM_MSGQ_NUMCHECK(thr_q_num);			/* Msgq number Check      */
	
	/********************************************
	* message queue check processing.           *
	********************************************/
	
	ITCM_MSGQ_USEDCHECK(thr_q_num);			/* Msgq used status Check */
	
	
	/********************************************
	* output parameter set processing.          *
	********************************************/
	*msgnum = itcm_msg.msgq[thr_q_num].msgcount;
	
	return BPF_RU_ITCM_OK;
}
/* @} */


/* @} */
