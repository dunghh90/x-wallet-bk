/*!
 * @skip   $Id$
 * @file   m_lg_Main.c
 * @brief  LOG main thread function
 * @date   2008/07/15 FFCS)Wuh Create for eNB-009-004.
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
*/
/********************************************************************************************************************/


/*
 * @addtogroup TRA_LOG
 * @{
 */
 
#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  LOG thread main function
 *  @note   This function is processed as follows.
 *          1) Call the initialization function
 *          2) Receive message and analysis the message
 *		    3) Call the jump function
 *  
 *  
 *  
 *  @return VOID
 *  @retval -
 *  @date   2008/07/15 FFCS)Wuh Create for eNB-008-001.
 *  @date   2013/11/04 ALPHA)yokoyama modify for ZYNQ
 */
VOID m_lg_main(VOID)
{
	VOID			*pMsg;
	UINT			rcvmsgsz;
	INT				ret;
	
	cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_main] enter") ;
	
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "l3_logm", 0, 0, 0);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_L3,				/* ProcessID		*/
									D_RRH_THDID_L3_LOG,				/* ThreadID			*/
									D_RRH_THDQID_L3_LOG,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());	/* 2020/12/28 M&C) Merge 4G FHM src (add 4th param)	*/
	
	/* 状態遷移処理:通常運用状態	*/
	lgw_almlogmng_tbl[CMD_SYS_LTE].get_log_sta = LGD_GETFHMSTS_USE;
	lgw_almlogmng_tbl[CMD_SYS_3G].get_log_sta = LGD_GETFHMSTS_USE;
	
	/* 初期化処理関数Call */
	(void)m_lg_init(NULL, 0);
	printf("\n C9 debug file %s ,line %d \n",__FILE__,__LINE__);//C9 add code debug
	/********************************************************************************************/
	/* メッセージ受信待ち																		*/
	/********************************************************************************************/
	while(1)
	{
		/****************************************************************************************/
		/* メッセージ受信																		*/
		/****************************************************************************************/
		ret = cm_Recieve(
			D_RRH_THDQID_L3_LOG,							/* 受信MSGキューID			*/
			0,												/* 応答待ち時間				*/
			&pMsg,											/* メッセージ受信バッファ	*/
			&rcvmsgsz);										/* 受信Message長			*/
		
		if( ret != 0 )
		{
			cm_Assert(D_RRH_LOG_AST_LV_ERROR, 0, "[m_lg_anlz] cm_Recieve NG") ;
			break;
		}
		
		/****************************************************************************************/
		/* 受信メッセージ解析関数Call															*/
		/****************************************************************************************/
		(void)m_lg_anlz(pMsg);
		
		/* 受信バッファ解放	*/
		cm_BFree( pMsg );
	}
	
	cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_main] return") ;
	
	return;
}

/* @} */

