/*!
 * @skip  $ld:$
 * @file  f_eio_msiExtOutEditReq.c
 * @brief Edit external output request(SMI).
 * @date 2019/03/19 FJT)Koshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019-
 */

/*!
** @addtogroup RRH_PF_EIO
* @{ */

#include "f_eio_header.h"			/* 5GDU-LLS外部IOタスクヘッダファイル			*/

/* @{ */

/*!
 *  @brief  Edit external output request(SMI)
 *  @note   This function is setting external IO request.\n
 *  @param  reqId         [in] request ID
 *  @param  *pMsg         [in] receive message pointer
 *  @return VOID
 *  @retval none
 *  @date 2019/03/19 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_msiExtOutEditReq( UINT reqId, VOID *pMsg )
{
	T_MPSW_MSI_O_RAN_EXTERNALIO_EDIT_REQ				*pRcvMsg;						/* 受信メッセージポインタ	*/
	UINT											lineOut;						/* Edit値		*/
	
	if(pMsg == NULL)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Message pointer NULL(0x%p)", pMsg );
		return;
	}
	pRcvMsg = (T_MPSW_MSI_O_RAN_EXTERNALIO_EDIT_REQ *)pMsg;
	
	/*	Edit値が範囲外の場合	*/
	if(pRcvMsg->line_out > D_EIO_EXTIO_SHORT)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "line_out Edit value Error(%d)", pRcvMsg->line_out );
		lineOut = (pRcvMsg->line_out & D_EIO_EXTIO_SHORT);
	}
	/*	Edit値が正常値の場合	*/
	else
	{
		lineOut = pRcvMsg->line_out;
	}
	/*	ショート(ON)の場合	*/
	if(lineOut == D_EIO_EXTIO_SHORT)
	{
		/*	レジスタ制御	*/
		BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_GPIO_DATA_2, D_DU_REGBIT_OTH_GPIO_DATA_2_LINEOUT);
	}
	/*	オープン(OFF)の場合	*/
	else
	{
		/*	レジスタ制御	*/
		BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_OTH_GPIO_DATA_2, D_DU_REGBIT_OTH_GPIO_DATA_2_LINEOUT);
	}
	
	/*	LOG取得(for dbg)	*/
	f_eio_extIoSvCtrlLog("[MSI]EXT Line_out edit:%d->%d", f_eiow_extIo.lineOutReg, lineOut);
	
	/*	テーブル値更新	*/
	f_eiow_extIo.lineOutReg = lineOut;
	
	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_CFM,				/* 送信するメッセージID		*/
						 reqId ,											/* リクエストID				*/
						 0,													/* 送信するデータサイズ		*/
						 NULL,												/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EIO ) ;							/* pf_eioのスレッドID		*/
	
	return ;
}

/* @} */

