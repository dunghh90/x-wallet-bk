/*!
 * @skip   $Id$
 * @file   m_lg_init.c
 * @brief  the processing for log thread initialization.
 * @date   2008/07/15 FFCS)Wuh Create for eNB-009-004.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup TRA_LOG
 * @{
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  the processing function for log thread initialization.
 *  @note   This function is processed as follows.
 *          1) Initial the trouble log management table
 *          2) Send the response for task initial request
 *          3) change the state to using
 *  @param  *a_rcv_adr [in]  -  the received message
 *  @param  a_rec_typ [out]  -  the type of REC system,3G or S3G
 *  @return VOID
 *  @retval N/A
 *  @date   2008/07/15 FFCS)Wuh Create.
 *  @date   2013/11/04 ALPHA)yokoyama modify for ZYNQ
 */

VOID m_lg_init( UCHAR *a_rcv_adr, USHORT a_rec_typ )
{
	CMT_TSKIF_HEAD				*msgp;
	INT							ret;

	cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_init] enter") ;

	/* 障害ログ管理テーブル初期化 for 3G and S3G	*/
	cm_MemClr(&lgw_almlogmng_tbl[CMD_SYS_3G], sizeof(LGT_ALMLOGESC_T)*CMD_MAX_SYSNUM ) ;

	/* 通信用共有メモリを取得する	*/
	ret = cm_BReq(	E_BPF_RU_IPCM_BUFF_KIND_TASK,		/* Buffer種別		*/
					sizeof(CMT_TSKIF_HEAD),				/* Size				*/
					(VOID**)&msgp );					/* msgP				*/

	if( ret != D_RRH_OK )
	{
		/* アサートログ書込み			*/
		cm_Assert(D_RRH_LOG_AST_LV_ERROR, ret, "[m_lg_init] Breq NG") ;
		cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_init] return") ;
		return;
	}

	/* Message Header設定		*/
	msgp->uiEventNo		= CMD_TSKIF_INIRSP;
	msgp->uiDstPQueueID	= D_RRH_PROCQUE_L3;			/* L3 process	*/
	msgp->uiDstTQueueID	= D_RRH_THDQID_L3_MAIN;		/* MAIN thread	*/
	msgp->uiSrcPQueueID	= D_RRH_PROCQUE_L3;			/* L3 process	*/
	msgp->uiSrcTQueueID	= D_RRH_THDQID_L3_LOG;		/* LOG thread	*/

	/* 初期化応答送信						*/
	ret = cm_Enter(	D_RRH_THDQID_L3_MAIN,				/* 直近の送信先QueueID		*/
				0,									/* 未使用					*/
				0,									/* 未使用					*/
				(void *)msgp );						/* 送信Message				*/
	if(ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret, "cm_Enter NG" );
	}

	/* 状態遷移処理:初期状態→通常運用状態	*/
	lgw_almlogmng_tbl[CMD_SYS_3G].get_log_sta = LGD_GETFHMSTS_USE;     /*-----for 3G-----*/
	lgw_almlogmng_tbl[CMD_SYS_S3G].get_log_sta = LGD_GETFHMSTS_USE;    /*-----for S3G-----*/

	cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_init] return") ;
	return;
}

/* @} */

