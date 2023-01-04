/*!
 * @skip   $Id$
 * @file   m_dl_cm_CRFirmRspSnd.c
 * @brief  the processing for sending the message of "firmware information report response" to REC.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-008-001.
 *
 * @date   2009/01/28  FFCS)wuh M-S3G-eNBPF-01235
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{ 
 */

#include "m_cm_header.h"															/* MDIF雖滓頃蜒ｿ蜒｢蜒溷・蛯ｽ蜒蜆?			*/
#include "m_dl_header.h"															/* 蜒泗C蜆椈ﾔ乕蜒ｪ螽欄徐E蜒怜ル蜒ｿ蜒｢蜒溷・蛯ｽ蜒蜆?*/

/*!
 *  @brief  the processing function for sending the message of "firmware information report response" to REC.
 *  @note   Send the CPRI signal sending notice(firmware information report response)to LAPB task.
 *          1) get the sending buffer
 *          2) construct the sending message
 *          3) Send "file data sending complete report response" message to LAPB task 
 *  @param  a_rsp_result		 	[in]  response result
 *  @param  a_mjr_ver               [in]  the major version
 *  @param  a_min_ver               [in]  the mini version
 *  @param  a_sys_typ		 	    [in]  the type of REC system send to(3G or S3G)
 *  @return Void.
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @date   2013/12/11 FFCS)linb Modify for ZYNQ.
 */


VOID m_dl_cm_CRFirmRspSnd( USHORT a_rsp_result, 
										USHORT a_mjr_ver, USHORT a_min_ver, USHORT a_sys_typ)
{
	UINT	a_rtn	 = CMD_RES_OK;																		/* 譬黴蛯晄乾螂ｿ擺曄謔?*/
	UINT	retry_cnt;																					/* 儕蜒ｩ儔蜒蜒⊿C蜆棘E	*/
	USHORT	a_signal_kind = CMD_CPRID_FIRMFILERES;														/* firmware response kind */
	CMT_TSKIF_CPRISND_FIRMFILERES *a_firmfile_p = NULL; 												/* 蜒ｶ蜒｢蜒ｼ蛯ｽ傾蜒ｪ蜆怜・ */


	/****************************************************************************************/
	/* バッファ獲得ＮＧ時はリトライ処理を実施する(MAX３回)									*/
	/****************************************************************************************/
	for( retry_cnt = 0 ; retry_cnt <= DLD_BUFFGET_CNT ; retry_cnt++){

		/************************/
		/* 送信バッファ取得処理 */
		/************************/
		/* MSG送信用のバッファを取得 */
		a_rtn = cm_L2BfGet( CMD_BUFCA_LAPSND, 
							   sizeof( CMT_TSKIF_CPRISND_FIRMFILERES ), 
							   CMD_NUM1, 
							   (VOID**)&a_firmfile_p );

		/* 戻り値判定 */
		if( a_rtn == CMD_RES_OK ){

			/* ループ処理を抜け、送信メッセージ編集処理を実施 */
			break;

		}

		/* リトライ回数が３回を超えたならアボート処理 */
		if( retry_cnt == DLD_BUFFGET_CNT) {

			cm_MAbort( CMD_ALMCD_BUFGET, "m_dl_cm_CRFirmRspSnd", 
					   "cm_L2BfGet NG", 
					   a_rtn, sizeof( CMT_TSKIF_CPRISND_FIRMFILERES ), CMD_NUM0 );

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
	a_firmfile_p->head.uiLength = 
			sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_CPRISIGSND_INF ) + sizeof( CMT_CPRIF_FIRMFILERES );

	/* CPRI信号データ部 */
	if(a_sys_typ == CMD_SYS_3G)
	{
		a_firmfile_p->cpridat_firmfileres.signal_kind = a_signal_kind; /*-----the 3G CPRI signal-----*/
	}
	else
	{
		a_signal_kind |= CMD_SYS_S3G;
		a_firmfile_p->cpridat_firmfileres.signal_kind = a_signal_kind; /*-----the S3G CPRI signal-----*/
	}
	/* if system is S3G,return makerjudge 1 */
	/* start for 13B */
	a_firmfile_p->cpridat_firmfileres.firmfile.makername = dlr_makername_settbl[DLD_MJ4];
	/* end for 13B */		
	/*--------------------------------------------------------------------*/
	a_firmfile_p->cpridat_firmfileres.result = a_rsp_result;
	a_firmfile_p->cpridat_firmfileres.firmfile.mjr_ver = a_mjr_ver;
	a_firmfile_p->cpridat_firmfileres.firmfile.mir_ver = a_min_ver;
	
	/**********************/
	/* メッセージ送信処理 */
	/**********************/
	/* ファームウェアファイル報告応答を送信 */
	m_dl_cm_RunHisSet(DLD_RUNHIS_FLG_SND, CMD_TSKID_LPB, CMD_TSKIF_CPRISNDNTC, (UCHAR *)a_firmfile_p);

	a_rtn = m_cm_L3MsgSend_REC( a_firmfile_p );
	
	return ;
}

/* @} */

