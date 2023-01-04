/*!
 * @skip  $ld:$
 * @file  f_eio_msgExtTimOutInd.c
 * @brief Indication of external Input sv timeout.
 * @date 2019/03/19 FJT)Koshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019-
 */

/*!
** @addtogroup RRH_PF_EIO
* @{ */

#include "f_eio_header.h"			/* 5GDU-LLS外部IOタスクヘッダファイル			*/

/* @{ */

/*!
 *  @brief  Indication of external Input sv timeout
 *  @note   This function is external Input sv timeout.\n
 *  @param  none
 *  @return VOID
 *  @retval none
 *  @date 2019/03/19 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_msgExtTimOutInd( VOID *pMsg )
{
	T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND	sndMsg;
	UINT					regVal;
	UINT					lineIn;
	
	/*	タイマ生成前の場合	*/
	if(f_eiow_extIo.svStart != D_RRH_ON)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "exception Event occur(start=%d)", f_eiow_extIo.svStart );
		return;
	}
	
	/*	外部IO入力状態を読出	*/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_GPIO_DATA_2_RO, &regVal);
	
	/*	外部IO入力値がオープン(ON)	*/
	if((regVal & D_DU_REGBIT_OTH_GPIO_DATA_2_RO_LINEIN) != 0)
	{
		lineIn = D_EIO_EXTIO_OPEN;	/*	オープン(OFF)	*/
	}
	/*	外部IO入力値がショート(OFF)	*/
	else
	{
		lineIn = D_EIO_EXTIO_SHORT;	/*	ショート(ON)	*/
	}
	/*	読出結果が前回値と一致	*/
	if(f_eiow_extIo.lineInReg == lineIn)
	{
		/*	前回報告値と異なる	*/
		if(f_eiow_extIo.lineInRep != lineIn)
		{
			
			/* MSI送信処理 */
			sndMsg.port_num = D_EIO_EXTIO_PORT0;
			sndMsg.line_in  = lineIn;
			(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND,			/* 送信するメッセージID		*/
								 0,															/* リクエストID				*/
								 sizeof(sndMsg),											/* 送信するデータサイズ		*/
								 &sndMsg ,													/* 送信データのポインタ		*/
								 D_RRH_PROCQUE_PF ,											/* PFのプロセスID			*/
								 D_SYS_THDQID_PF_EIO ) ;									/* pf_eioのスレッドID		*/
			
			/*	LOG取得(for dbg)	*/
			f_eio_extIoSvCtrlLog("[TO]EXT Line_in chg:%d->%d(cnt=%d)", 
							f_eiow_extIo.lineInRep, lineIn, f_eiow_extIo.lineInDefCnt);
			
			/*	情報更新	*/
			f_eiow_extIo.lineInRep = lineIn;
		}
		/*	前回値不一致カウンタをクリアする(for dbg)	*/
		f_eiow_extIo.lineInDefLog = D_RRH_OFF;
		f_eiow_extIo.lineInDefCnt = 0;
	}
	/*	読出結果が前回値と不一致	*/
	else
	{
		/*	前回値不一致カウンタをインクリメントする(for dbg)	*/
		f_eiow_extIo.lineInDefCnt++;
		
		/*	前回値不一致が5回超過した場合(for dbg)	*/
		if((f_eiow_extIo.lineInDefLog == D_RRH_OFF)&&(f_eiow_extIo.lineInDefCnt > 5))
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "Line_in unMatch warning(cnt=%d)", f_eiow_extIo.lineInDefCnt );
			f_eiow_extIo.lineInDefLog = D_RRH_ON;
		}
		/*	情報更新	*/
		f_eiow_extIo.lineInReg = lineIn;
	}
	return ;
}

/* @} */

