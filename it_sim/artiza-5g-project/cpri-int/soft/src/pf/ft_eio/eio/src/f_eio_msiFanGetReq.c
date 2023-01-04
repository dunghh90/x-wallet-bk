/*!
 * @skip  $ld:$
 * @file  f_eio_msiFanGetReq.c
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
 *  @brief  Get fan request(SMI)
 *  @note   This function is getting fan request.\n
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

VOID f_eio_msiFanGetReq( UINT reqId )
{
	T_MPSW_MSI_O_RAN_FAN_GET_CFM				sndMsg;							/* 送信メッセージ	*/
	UINT									fanIdx;							/* FAN位置			*/
	UINT									fanSpeed;						/* FAN回転数[2*rps]	*/
	
	sndMsg.fanInfoNum = E_DU_FAN_MAXNO;
	for(fanIdx=E_DU_FAN_1; fanIdx<E_DU_FAN_MAXNO; fanIdx++)
	{
		sndMsg.fanInfo[fanIdx].locationNo = fanIdx+1;
		/*	FANユニット状態がNormalの場合	*/
		if(f_eiow_svctrl_mngp->fanSv.fanUnitFault == D_DU_VAL_FAN_NORMAL)
		{
			fanSpeed = f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeed[fanIdx];
			sndMsg.fanInfo[fanIdx].fanStatus = 0;		/*	normal		*/
			sndMsg.fanInfo[fanIdx].vendorCode = 0;		/*	0..7(TBD)	*/
			/*	FAN Max Speed超過の場合(Max Speedでクリップ)	*/
			if(fanSpeed > D_EIO_FANSPEED_MAX)
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "FAN Speed over(0x%08x)", fanSpeed);
				/*	100[%]	*/
				sndMsg.fanInfo[fanIdx].speedPercent = 100;
				/*	Max Speed[rpm]	*/
				sndMsg.fanInfo[fanIdx].targetSpeed  = D_EIO_FANSPEED_MAX * D_EIO_FANSPEED_CALRPM;
			}
			/*	FAN Max Speed以下の場合	*/
			else
			{
				/*	1..100[%]	*/
				sndMsg.fanInfo[fanIdx].speedPercent = ((fanSpeed * 100) / D_EIO_FANSPEED_MAX);
				/*	0..3800[rpm]	*/
				sndMsg.fanInfo[fanIdx].targetSpeed  = fanSpeed * D_EIO_FANSPEED_CALRPM;
			}
		}
		/*	FANユニット状態がFaultの場合	*/
		else
		{
			sndMsg.fanInfo[fanIdx].fanStatus = 1;		/*	fault		*/
			sndMsg.fanInfo[fanIdx].vendorCode = 0;		/*	Don't Care	*/
			sndMsg.fanInfo[fanIdx].speedPercent = 0;	/*	Don't Care	*/
			sndMsg.fanInfo[fanIdx].targetSpeed = 0;		/*	Don't Care	*/
		}
	}
	
	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_FAN_GET_CFM,						/* 送信するメッセージID		*/
						 reqId ,											/* リクエストID				*/
						 sizeof(sndMsg),									/* 送信するデータサイズ		*/
						 &sndMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EIO ) ;							/* pf_eioのスレッドID		*/
	
	return ;
}

/* @} */

