/*!
 * @skip   $Id$
 * @file   m_lg_send_stopres.c
 * @brief  the processing for trouble log get stop response sending.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-009-004.
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
 *  @brief  the processing function for trouble log get stop response sending.
 *  @note   This function is processed as follows.
 *          1) send RE trouble log get stop response to REC 
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @param  a_rescode  [in]  response code
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 */

VOID m_lg_send_stopres( USHORT a_rec_typ, UINT a_rescode )
{
											/* タスク間フレームポインタ		*/
	CMT_TSKIF_CPRISND_REOBLOGSTPRES *a_rsp_p = NULL ;
	UINT   a_bufget_rslt  = CMD_RES_OK ;	/* バッファ獲得結果				*/
	UINT   a_ressnd_rslt  = CMD_RES_OK ;	/* 応答送信結果					*/
	USHORT a_signal_kind  = CMD_CPRID_REOBLOGSTPRES ;   /*trouble log get stop response kind*/
	
	cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_send_stopres] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
	
	/*======================================================================*/
	/*																		*/
	/*	■RE障害ログ取得中止応答形式										*/
	/*																		*/
	/*		RE障害ログ取得中止応答	(   2Byte)	0x5D20						*/
	/*		応答結果				(   2Byte)								*/
	/*																		*/
	/*======================================================================*/
	
	/* 送信用バッファ獲得	*/
	a_bufget_rslt = cm_L2BfGet(	CMD_BUFCA_LAPSND, 
								sizeof(CMT_TSKIF_CPRISND_REOBLOGSTPRES), 
								CMD_NUM1,
								(VOID**)&a_rsp_p ) ;
	
	/* バッファ獲得失敗		*/
	if(a_bufget_rslt != CMD_RES_OK)
	{
        /* アサートログ書込み									*/
		cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[ALMLOG] m_lg_send_stopres");
		cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_send_stopres] return") ;
		return ;
	}
	
	/* タスク間ヘッダ部編集 */
	/* 送信元に返すのでテーブルに保存していたQueueに返す	*/
	a_rsp_p->head.uiDstPQueueID	= lgw_rspmng_tbl[a_rec_typ][LGD_L3_TROUBLELOG_STOP].dstPQID;
	a_rsp_p->head.uiDstTQueueID	= lgw_rspmng_tbl[a_rec_typ][LGD_L3_TROUBLELOG_STOP].dstTQID;
	a_rsp_p->head.uiSrcPQueueID	= D_RRH_PROCQUE_L3;
	a_rsp_p->head.uiSrcTQueueID	= D_RRH_THDQID_L3_LOG;
	
	
	/* CPRI通信用データ編集	*/
	/* 信号種別設定			*/
    /*--------------------CPRI signal response for 3G or S3G-----------------------*/
	if( a_rec_typ== CMD_SYS_3G )
    {
	    a_rsp_p->cpridat_reoblogstpres.signal_kind = a_signal_kind ;     /*-----for 3G-----*/
    }
    else
    {
        a_signal_kind |= CMD_SYS_S3G ;
		a_rsp_p->cpridat_reoblogstpres.signal_kind = a_signal_kind ;     /*-----for S3G-----*/
	}
	
	/*------------------------------------------------------------------------------*/
	/* CPRI応答コード設定	*/
	a_rsp_p->cpridat_reoblogstpres.result = (USHORT)a_rescode ;
	
	/* Message送信する									*/
	/* 送信先はCMUなのでヘッダに設定した送信先と異なることに注意	*/
	a_ressnd_rslt = m_cm_L3MsgSend_REC( a_rsp_p );
	
	if( a_ressnd_rslt != 0 )
	{
		cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[m_lg_send_stopres] error");
	}
	
	cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_send_stopres] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
	
	return ;
}

/* @} */

