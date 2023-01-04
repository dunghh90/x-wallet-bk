/*!
 * @skip   $Id$
 * @file   m_dl_cm_CRFileInfRspSnd.c
 * @brief  the processing for sending the message of "file information report response" to REC.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-008-001.
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
 *  @brief  the processing function for sending the message of "file information report response" to REC.
 *  @note   Send the CPRI signal sending notice(file information report response)to LAPB task.
 *          1) get the sending buffer
 *          2) construct the sending message
 *          3) Send "file information report response" message to LAPB task 
 *  @param  a_rsp_result		 	[in]  response result
 *  @param  a_sys_typ		 	    [in]  the type of REC system send to(3G or S3G)
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 */

VOID m_dl_cm_CRFileInfRspSnd( USHORT a_rsp_result, USHORT a_sys_typ )
{
	UINT	a_rtn	 = CMD_RES_OK;																		/* 戻り値格納変数	*/
	UINT	retry_cnt;																					/* リトライカウンタ	*/
	USHORT	a_signal_kind = CMD_CPRID_FILEINFRES;														/* file information response kind */
	CMT_TSKIF_CPRISND_FILEINFRES *a_fileinf_p = NULL;													/* バッファアドレス	*/
	
	/****************************************************************************************/
	/* バッファ獲得ＮＧ時はリトライ処理を実施する(MAX３回)									*/
	/****************************************************************************************/
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	for( retry_cnt = 0 ; retry_cnt <= DLD_BUFFGET_CNT ; retry_cnt++){

		/************************/
		/* 送信バッファ取得処理 */
		/************************/
		/* MSG送信用のバッファを取得 */
		a_rtn = cm_L2BfGet( CMD_BUFCA_LAPSND, 
							   sizeof( CMT_TSKIF_CPRISND_FILEINFRES ), 
							   CMD_NUM1, 
							   (VOID **)&a_fileinf_p );

		/* 戻り値判定 */
		if( a_rtn == CMD_RES_OK ){

			/* ループ処理を抜け、送信メッセージ編集処理を実施 */
			break;

		}

		/* リトライ回数が３回を超えたならアボート処理 */
		if( retry_cnt == DLD_BUFFGET_CNT) {

			cm_MAbort( CMD_ALMCD_BUFGET, "m_dl_cm_CRFileInfRspSnd", 
					   "cm_L2BfGet NG", 
					   a_rtn, sizeof( CMT_TSKIF_CPRISND_FILEINFRES ), CMD_NUM0 );

		}

		/* アサート出力 */
		cm_Assert(CMD_ASL_USELOW, retry_cnt+CMD_NUM1,"cm_L2BfGet NG");
		/* ウェイト処理(100ms) */
		cm_MWait( DLD_BUFFGET_WAIT100 );

	}

	/**************************/
	/* 送信メッセージ編集処理 */
	/**************************/
	/* タスク間ヘッダ部 */
	a_fileinf_p->head.uiLength= 
							sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_FILEINFRES);

	/* CPRI信号データ部 */
	/*-----judge which side to send 3G or S3G-----*/
	if(a_sys_typ == CMD_SYS_3G)
	{
		a_fileinf_p->cpridat_fileinfres.signal_kind = a_signal_kind; /*-----the 3G CPRI signal-----*/
	}
	else
	{
		a_signal_kind |= CMD_SYS_S3G;
		a_fileinf_p->cpridat_fileinfres.signal_kind = a_signal_kind; /*-----the S3G CPRI signal-----*/
	}
	/*--------------------------------------------------------------------*/
	a_fileinf_p->cpridat_fileinfres.result = a_rsp_result;
	
	/**********************/
	/* メッセージ送信処理 */
	/**********************/
	/* ファイル情報報告応答を送信する */
	m_dl_cm_RunHisSet(DLD_RUNHIS_FLG_SND, CMD_TSKID_LPB, CMD_TSKIF_CPRISNDNTC, (UCHAR *)a_fileinf_p);
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s retry_cnt[%d], signal_kind[%x], result[%d], \n",__LINE__,__FUNCTION__, retry_cnt, a_fileinf_p->cpridat_fileinfres.signal_kind, a_fileinf_p->cpridat_fileinfres.result);
	#endif
	a_rtn = m_cm_L3MsgSend_REC( a_fileinf_p );
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return ;
}

/* @} */

