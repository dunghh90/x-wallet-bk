/*!
 * @skip   $Id$
 * @file   m_lg_send_getres.c
 * @brief  the processing for trouble log get response sending.
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
#include "f_sys_inc.h"

/*!
 *  @brief  the processing function for trouble log get response sending.
 *  @note   This function is processed as follows.
 *          1) send RE trouble log get response data to REC 
 *  @param  a_rescode  [in]  response code
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @param  a_datasize [in]  data size
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 */

VOID m_lg_send_getres( UINT a_rescode, USHORT a_rec_typ, UINT a_datasize )
{
											/* タスク間フレームポインタ		*/
	CMT_TSKIF_CPRISND_REOBSLOGRES *a_rsp_p = NULL ;
	UINT	a_bufget_rslt  = CMD_RES_OK;		/* バッファ獲得結果				*/
	UINT	a_ressnd_rslt  = CMD_RES_OK;		/* 応答送信結果					*/
	USHORT	a_signal_kind  = CMD_CPRID_REOBSLOGRES;    /* trouble log get response kind */
	UINT				keyID;
	
	cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_send_getres] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
	
	/*======================================================================*/
	/*																		*/
	/*	■RE障害ログ取得応答形式											*/
	/*																		*/
	/*		RE障害ログ取得応答	(   2Byte)	0x5C20							*/
	/*		応答結果			(   2Byte)									*/
	/*		ファイル名			( 256Byte)	要求時に指定されたファイル名	*/
	/*		トータルデータサイズ(   4Byte)	転送総データサイズ				*/
	/*																		*/
	/*======================================================================*/
	
	/* 送信用バッファ獲得 */
	a_bufget_rslt = cm_L2BfGet( CMD_BUFCA_LAPSND, 
								sizeof(CMT_TSKIF_CPRISND_REOBSLOGRES),
								CMD_NUM1,
								(void **)&a_rsp_p ) ;
	
	/* バッファ獲得失敗 */
	if(a_bufget_rslt != CMD_RES_OK)
	{
        /* アサートログ書込み									*/
		cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[ALMLOG] m_lg_send_getres");
		cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_send_getres] return") ;
		return ;
	}
	
	/* ヘッダ部編集										*/
	/* 送信元に返すのでテーブルに保存していたQueueに返す	*/
	a_rsp_p->head.uiDstPQueueID	= lgw_rspmng_tbl[a_rec_typ][LGD_L3_TROUBLELOG_GET_RSP].dstPQID;
	a_rsp_p->head.uiDstTQueueID	= lgw_rspmng_tbl[a_rec_typ][LGD_L3_TROUBLELOG_GET_RSP].dstTQID;
	a_rsp_p->head.uiSrcPQueueID	= D_RRH_PROCQUE_L3;
	a_rsp_p->head.uiSrcTQueueID	= D_RRH_THDQID_L3_LOG;
	
	/* CPRI通信用データ編集								*/
	/* 送信バッファに RE障害ログ取得応答を設定			*/
    /*--------------------------judge the signal kind sended,3G or S3G------------------*/
	if( a_rec_typ == CMD_SYS_3G)
    {
	    a_rsp_p->cpridat_reobslogres.signal_kind = a_signal_kind ;    /*-----for 3G-----*/
    }
	else
	{
        a_signal_kind |= CMD_SYS_S3G ;
		a_rsp_p->cpridat_reobslogres.signal_kind = a_signal_kind ;    /*-----for S3G-----*/
	}
	/*----------------------------------------------------------------------------------*/
	/* 応答コード設定									*/
	a_rsp_p->cpridat_reobslogres.result   = (USHORT)a_rescode ;
	
	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)a_rsp_p)->uiKeyID;

	/* テーブルのファイル名を送信バッファのファイル名に設定 */
	cm_MemCpy( a_rsp_p->cpridat_reobslogres.filename, 
			   lgw_almlogmng_tbl[a_rec_typ].logfile_name, 
			   (sizeof(UCHAR) * LGD_MAX_FILNAME_NUM) ) ;
	
	((T_SYS_COMMON_THDIF*)a_rsp_p)->uiKeyID = keyID; 

	/* 送信バッファに トータルサイズを設定				*/
	a_rsp_p->cpridat_reobslogres.datsize  = a_datasize ;
	
	/* Message送信する									*/
	/* 送信先はCMUなのでヘッダに設定した送信先と異なることに注意	*/
	a_ressnd_rslt = m_cm_L3MsgSend_REC( a_rsp_p );
	
	if( a_ressnd_rslt != 0 )
	{
		cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[ALMLOG] m_lg_send_getres");
	}
	
	cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_send_getres] return") ;
	
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
	return ;
}

/* @} */

