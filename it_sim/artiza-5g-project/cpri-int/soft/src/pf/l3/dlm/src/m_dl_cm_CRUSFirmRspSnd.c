/*!
 * @skip   $Id$
 * @file   m_dl_cm_CRUSFirmRspSnd.c
 * @brief  the processing for sending the message of "firmware information report response in running download" to REC.
 * @date   2009/03/21 FFCS)Wuh Create for eNB-008-001.
 * @date   2009/04/30  FFCS)Wuh modify for CR-xxx-xxx
 *                      move the process of FLASH erasing to file data sending
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{ 
 */

#include "m_cm_header.h"															/* MDIF雖滓頃蜒ｿ蜒｢蜒溷・蛯ｽ蜒蜆?			*/
#include "m_dl_header.h"															/* 蜒泗C蜆椈ﾔ乕蜒ｪ螽欄徐E蜒怜ル蜒ｿ蜒｢蜒溷・蛯ｽ蜒蜆?*/

/*!
 *  @brief  the processing function for sending the message of "firmware information report response in running download" to REC.
 *  @note   Send the CPRI signal sending notice(firmware information report response)to LAPB task.
 *          1) get the sending buffer
 *          2) construct the sending message
 *          3) Send "file data sending complete report response" message to LAPB task 
 *  @param  a_rsp_result		 	[in]  response result
 *  @param  a_mjr_usever            [in]  the used major version
 *  @param  a_min_usever            [in]  the used mini version
 *  @param  a_mjr_staver            [in]  the next startup major version
 *  @param  a_min_staver            [in]  the next startup mini version
 *  @return Void.
 *  @retval -
 *  @date   2009/03/21 FFCS)Wuh Create.
 *  @date   2013/12/11 FFCS)linb Modify for DCM ZYNQ.
 */


VOID m_dl_cm_CRUSFirmRspSnd( USHORT a_rsp_result, 
										USHORT a_mjr_usever, USHORT a_min_usever, USHORT a_mjr_staver, USHORT a_min_staver)
{
	UINT	a_rtn	 = CMD_RES_OK;																		/* 譬ꀀ蛯晄乾螂ｿ擺曄謔?*/
	UINT	retry_cnt;																					/* 儕蜒ｩ儔蜒蜒⊿C蜆棘E	*/
	USHORT	a_signal_kind = (CMD_CPRID_UFIRMFILERES | CMD_SYS_S3G); 													 /* firmware response kind */
	CMT_TSKIF_CPRISND_UFIRMFILERES *a_firmfile_p = NULL;												/* 蜒ｶ蜒｢蜒ｼ蛯ｽ傾蜒ｪ蜆怜・ */

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_cm_CRUSFirmRspSnd] ENTER" );		

	/****************************************************************************************/
	/* バッファ獲得ＮＧ時はリトライ処理を実施する(MAX３回)									*/
	/****************************************************************************************/
	for( retry_cnt = 0 ; retry_cnt <= DLD_BUFFGET_CNT ; retry_cnt++){

		/************************/
		/* 送信バッファ取得処理 */
		/************************/
		/* MSG送信用のバッファを取得 */
		a_rtn = cm_L2BfGet( CMD_BUFCA_LAPSND, 
							   sizeof( CMT_TSKIF_CPRISND_UFIRMFILERES ), 
							   CMD_NUM1, 
							   (VOID**)&a_firmfile_p );

		/* 戻り値判定 */
		if( a_rtn == CMD_RES_OK ){

			/* ループ処理を抜け、送信メッセージ編集処理を実施 */
			break;

		}

		/* リトライ回数が３回を超えたならアボート処理 */
		if( retry_cnt == DLD_BUFFGET_CNT) {

			cm_MAbort( CMD_ALMCD_BUFGET, "m_dl_cm_CRUSFirmRspSnd", 
					   "cm_L2BfGet NG", 
					   a_rtn, sizeof( CMT_TSKIF_CPRISND_UFIRMFILERES ), CMD_NUM0 );

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
			sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_CPRISIGSND_INF ) + sizeof( CMT_CPRIF_UFIRMFILERES );

	/* CPRI信号データ部 */
	a_firmfile_p->cpridat_ufirmfileres.signal_kind = a_signal_kind;
	/* if system is S3G,return makerjudge 'K' */
	/* start for 13B */
    a_firmfile_p->cpridat_ufirmfileres.firmfile.makername = dlr_makername_settbl[DLD_MJ4];
	/* end for 13B */

		
	a_firmfile_p->cpridat_ufirmfileres.result = a_rsp_result;
	
	a_firmfile_p->cpridat_ufirmfileres.firmfile.cur_mjr_ver = a_mjr_usever;
	a_firmfile_p->cpridat_ufirmfileres.firmfile.cur_mir_ver = a_min_usever;
	a_firmfile_p->cpridat_ufirmfileres.firmfile.nes_mjr_ver = a_mjr_staver;
	a_firmfile_p->cpridat_ufirmfileres.firmfile.nes_mir_ver = a_min_staver;
	
	/**********************/
	/* メッセージ送信処理 */
	/**********************/
	/* ファームウェアファイル報告応答を送信 */
	a_rtn = m_cm_L3MsgSend_REC( a_firmfile_p );

	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,"[m_dl_cm_CRUSFirmRspSnd] RETURN" );		
	return ;
}

/* @} */

