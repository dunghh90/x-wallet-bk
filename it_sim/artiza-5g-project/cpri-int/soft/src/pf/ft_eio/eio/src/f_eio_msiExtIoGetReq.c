/*!
 * @skip  $ld:$
 * @file  f_eio_msiExtIoGetReq.c
 * @brief Get external Input request(SMI).
 * @date 2019/03/19 FJT)Koshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019-
 */

/*!
** @addtogroup RRH_PF_EIO
* @{ */

#include "f_eio_header.h"			/* 5GDU-LLS外部IOタスクヘッダファイル			*/

/* @{ */

/*!
 *  @brief  Get external Input request(SMI)
 *  @note   This function is getting external IO request.\n
 *  @param  reqId         [in] request ID
 *  @return VOID
 *  @retval none
 *  @date 2019/03/19 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_msiExtIoGetReq( UINT reqId )
{
	T_MPSW_MSI_O_RAN_EXTERNALIO_GET_CFM				sndMsg;							/* 送信メッセージ	*/
	
	/*	1回目の報告前の場合	*/
	if((f_eiow_extIo.svStart != D_RRH_ON) || (f_eiow_extIo.lineInRep == D_EIO_EXTIO_INVALID))
	{
		/*	前回レジスタ読出値またはテーブル初期値(0)を報告	*/
		sndMsg.line_in = f_eiow_extIo.lineInReg;
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "EXT LINE IN before NTFY(start=%d,rep=0x%08x)", f_eiow_extIo.svStart, f_eiow_extIo.lineInRep );
	}
	else
	{
		/*	前回報告値を報告	*/
		sndMsg.line_in = f_eiow_extIo.lineInRep;
	}
	sndMsg.line_out = f_eiow_extIo.lineOutReg;
	
	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_EXTERNALIO_GET_CFM,					/* 送信するメッセージID		*/
						 reqId ,											/* リクエストID				*/
						 sizeof(sndMsg),									/* 送信するデータサイズ		*/
						 &sndMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EIO ) ;							/* pf_eioのスレッドID		*/
	
	return ;
}

/* @} */

