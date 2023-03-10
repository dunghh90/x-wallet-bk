/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_Delay.c
 *  @brief	 Task delay process (delay value < 10ms)
 *  @date   2012/07/08 FFCS)niemsh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2012-
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
 
/****************************************************************************/
/* ç¹å¥ã£ç¹ç¹§ï½¤ç¹ï½³ç¹§ï½¯ç¹ï½«ç¹ï½¼ç¹                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRAå±éããããã¡ã¤ã«			*/

/**************************************************************************/
/**
 *  @brief Task delay process (delay value < 10ms)
 *  @note Task delay process (delay value < 10ms)
 *  @param  delayTim		[in]		åæ­¢æé(1msåä½)
 *  @param  FileNamep		[in]		ã³ã¼ã«åé¢æ°ãã¡ã¤ã«å
 *  @param  LineNo			[in]		ã³ã¼ã«åé¢æ°å®è¡ã©ã¤ã³
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2012/07/08 FFCS)niemsh create
 *  @date   2013/11/26 alpha)Kondou	zynqåå¯¾å¿
 */
/**************************************************************************/
VOID m_cmR_Delay(
			UINT		delayTim,
			const VOID*	FileNamep,
			UINT		LineNo )
{
	INT					iResult;		/*	ã¹ãã¼ã¿ã¹å¤æ°				*/
	struct timespec		tDelayTime;		/*	éå»¶æé					*/
	struct timespec		tRestTime;		/*	æ®ãæé					*/
	

	/* ã¿ã¤ãå¤ãè¨­å® */
	tDelayTime.tv_sec  = (UINT)(delayTim / 1000);
	tDelayTime.tv_nsec = (UINT)(delayTim % 1000)*1000000;
	
	/*	ã¿ã¤ããèµ·å	*/
	iResult = BPF_RU_ITCM_THREAD_DELAY(
				&tDelayTime,
				1,
				&tRestTime
			);
	
	/*	ERRORã®å ´å		*/
	if(iResult != 0){
		BPF_COM_LOG_ASSERT ( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_ITCM_THREAD_DELAY error code=%d", iResult );
	}
	
	return;
	
}

/* @} */
