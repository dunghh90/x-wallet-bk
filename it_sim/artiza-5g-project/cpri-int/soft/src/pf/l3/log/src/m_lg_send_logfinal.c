/*!
 * @skip   $Id$
 * @file   m_lg_send_logfinal.c
 * @brief  the processing for trouble log get complete notice sending.
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
 *  @brief  the processing function for trouble log get complete notice sending.
 *  @note   This function is processed as follows.
 *          1) send RE trouble log get complete notice to REC 
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 */

VOID m_lg_send_logfinal( USHORT a_rec_typ )
{
	/* タスク間フレームポインタ		*/
	CMT_TSKIF_CPRISND_FILSNDFINNTC *a_rsp_p = NULL ;
	UINT   a_bufget_rslt  = CMD_RES_OK;		/* バッファ獲得結果				*/
	UINT   a_ressnd_rslt  = CMD_RES_OK;		/* 応答送信結果					*/
	USHORT a_signal_kind  = CMD_CPRID_FILSNDFINNTC;   /* file data sending complete kind */

	cm_Assert(D_RRH_LOG_AST_LV_ENTER, a_rec_typ, "[m_lg_send_logfinal] enter");

	/*======================================================================*/
	/*																		*/
	/*	■ファイルデータ送信完了報告通知形式								*/
	/*																		*/
	/*		ファイルデータ送信完了報告通知	(   2Byte)	0x2310				*/
	/*		対象データ						(   2Byte)	4:障害ログ 固定		*/
	/*		チェックサム					(   2Byte)	Don't Care			*/
	/*																		*/
	/*======================================================================*/
	
	/* 送信用バッファ獲得	*/
	a_bufget_rslt = cm_L2BfGet(	CMD_BUFCA_LAPSND, 
								sizeof(CMT_TSKIF_CPRISND_FILSNDFINNTC),
								CMD_NUM1,
								(VOID **)&a_rsp_p ) ;
	
	/* バッファ獲得失敗		*/
	if(a_bufget_rslt != CMD_RES_OK)
	{
        /* アサートログ書込み									*/
		cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[ALMLOG] Bufget Ng");
		cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_send_logfinal] return") ;
		return ;
	}
	
	/* タスク間ヘッダ部編集									*/
	/* 送信元に返すのでテーブルに保存していたQueueに返す	*/
	a_rsp_p->head.uiDstPQueueID	= lgw_rspmng_tbl[a_rec_typ][LGD_L3_FILEDATA_FINISH].dstPQID;
	a_rsp_p->head.uiDstTQueueID	= lgw_rspmng_tbl[a_rec_typ][LGD_L3_FILEDATA_FINISH].dstTQID;
	a_rsp_p->head.uiSrcPQueueID	= D_RRH_PROCQUE_L3;
	a_rsp_p->head.uiSrcTQueueID	= D_RRH_THDQID_L3_LOG;
	
	
	/* CPRI通信用データ編集									*/
	/* 送信バッファに ファイルデータ送信完了報告通知を設定	*/
	/*--------------------------CPRI signal response for 3G or S3G----------------------------*/
    if( a_rec_typ == CMD_SYS_3G)
    {
	    a_rsp_p->cpridat_filsndfinntc.signal_kind = a_signal_kind ;    /*-----for 3G-----*/
    }
	else
	{
        a_signal_kind |= CMD_SYS_S3G ;
		a_rsp_p->cpridat_filsndfinntc.signal_kind = a_signal_kind ;    /*-----for S3G-----*/
	}
	/* 送信バッファに 障害ログを設定						*/
	a_rsp_p->cpridat_filsndfinntc.tgtdat = CMD_LOG ;
	
	/* Message送信する									*/
	/* 送信先はCMUなのでヘッダに設定した送信先と異なることに注意	*/
	a_ressnd_rslt = m_cm_L3MsgSend_REC( a_rsp_p );
	
	if( a_ressnd_rslt != 0 )
	{
		cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[m_lg_send_logfinal] error");
	}
	
	cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_send_logfinal] return") ;
	
	return ;
}

/* @} */

