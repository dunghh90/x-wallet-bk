/*!
 * @skip   $Id$
 * @file   m_dl_cm_CRDataEndRspSnd_S3G.c
 * @brief  the processing for sending the message of "file data sending complete report response" to S3G REC.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-008-001.
 * @date   2015/09/28 TDIPS)sasaki Update
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{ 
 */

#include "m_cm_header.h"															/* MDIF common head file			  */
#include "m_dl_header.h"															/* download management task head file */

/*!
 *  @brief  the processing function for sending the message of "file data sending complete report response" to S3G REC.
 *  @note   Send the CPRI signal sending notice(file data sending complete report response)to LAPB task.
 *          1) get the sending buffer
 *          2) construct the sending message
 *          3) Send "file data sending complete report response" message to LAPB task 
 *  @param  a_rsp_result		 	[in]  response result
 *  @param  a_sys_typ		 	    [in]  the type of REC system send to(3G or S3G)
 *  @param  primary_sys             [in]  the primary system	
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @date   2008/08/05 FFCS)Wuh Add the response structure for S3G,because the S3G's response has valid system.
 *  @date   2015/09/28 TDIPS)sasaki Update MKレビュー指摘No.245対応
 */

VOID m_dl_cm_CRDataEndRspSnd_S3G( USHORT a_rsp_result, USHORT a_sys_typ, USHORT primary_sys )
{
	UINT	a_rtn	 = CMD_RES_OK;																		/* 譬ꀀ蛯晄乾螂ｿ擺曄謔?*/
	UINT	retry_cnt;																					/* 儕蜒ｩ儔蜒蜒⊿C蜆棘E	*/
	USHORT	a_signal_kind = CMD_CPRID_FILSNDFINRES; 													/* File data sending complete kind */												/* 蜒ｶ蜒｢蜒ｼ蛯ｽ傾蜒ｪ蜆怜・	*/
	CMT_TSKIF_CPRISND_FILSNDFINRES_S3G	*a_filesnd_s3g_p = NULL;

	/****************************************************************************************/
	/* バッファ獲得ＮＧ時はリトライ処理を実施する(MAX３回)									*/
	/****************************************************************************************/
	for( retry_cnt = 0 ; retry_cnt <= DLD_BUFFGET_CNT ; retry_cnt++){

		/************************/
		/* 送信バッファ取得処理 */
		/************************/
		/* MSG送信用のバッファを取得 */
		a_rtn = cm_L2BfGet( CMD_BUFCA_LAPSND, 
						sizeof( CMT_TSKIF_CPRISND_FILSNDFINRES_S3G ), 
						CMD_NUM1, 
						(VOID**)&a_filesnd_s3g_p );

		/* 戻り値判定 */
		if( a_rtn == CMD_RES_OK ){

			/* ループ処理を抜け、送信メッセージ編集処理を実施 */
			break;

		}

		/* リトライ回数が３回を超えたならアボート処理 */
		if( retry_cnt == DLD_BUFFGET_CNT) {

			cm_MAbort( CMD_ALMCD_BUFGET, "m_dl_cm_CRDataEndRspSnd", 
				"cm_L2BfGet NG", 
				a_rtn, sizeof( CMT_TSKIF_CPRISND_FILSNDFINRES_S3G ), CMD_NUM0 );

		}

		/* アサート出力 */
		cm_Assert(CMD_ASL_USELOW, retry_cnt+CMD_NUM1, "cm_L2BfGet NG");
			/* ウェイト処理(500ms) */
		cm_MWait( DLD_BUFFGET_WAIT500 );
		
	}

	/*It make sure that SV bus respose within 200ms*/
	cm_MWait( CMD_NUM20);

	/**************************/
	/* 送信メッセージ編集処理 */
	/**************************/
	/* タスク間ヘッダ部 */
	a_filesnd_s3g_p->head.uiLength = 
			sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_CPRISIGSND_INF ) + sizeof( CMT_CPRIF_FILSNDFINRES_S3G );
	
	/* CPRI信号データ部 */
	a_signal_kind |= CMD_SYS_S3G;
	a_filesnd_s3g_p->cpridat_filsndfinres.signal_kind = a_signal_kind; /*-----the S3G CPRI signal-----*/
	a_filesnd_s3g_p->cpridat_filsndfinres.result = a_rsp_result;
	/* if the target data is systemparameter,add the primary system to response message */
//	if(cmw_file_inf_tbl[a_sys_typ].target_data == CMD_SYSPAR)
	if((*gt_dlw_rec_all_tbl[a_sys_typ]).rec_sta == EC_DLM_MAIN_STN_FILEDATATRANS_FINREP_SYSPARA_CON)
	{
		a_filesnd_s3g_p->cpridat_filsndfinres.primary_sys = primary_sys;
	}
	/**********************/
	/* メッセージ送信処理 */
	/**********************/
	/* ファイルデータ送信完了報告応答を送信 */
	m_dl_cm_RunHisSet(DLD_RUNHIS_FLG_SND, CMD_TSKID_LPB, CMD_TSKIF_CPRISNDNTC, (UCHAR *)a_filesnd_s3g_p);

	a_rtn = m_cm_L3MsgSend_REC( a_filesnd_s3g_p );
	
	return ;
}

/* @} */

