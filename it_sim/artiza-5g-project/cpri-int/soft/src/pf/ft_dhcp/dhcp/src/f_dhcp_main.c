/*!
 * @skip 		$ld:$
 * @file 		f_dhcp_main.c
 * @brief		pf_dhcpスレッド メイン処理
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dhcp_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_main
 * @note		pf_dhcpスレッドメイン。スレッド起動からMSG受信待ちのLoop処理を実施
 *				-# BPFへスレッド情報登録する
 *				-# DHCPスレッド状態を"初期状態"に設定する
 *				-# 初期化関数をCallする(f_dhcp_init())
 *				-# MSG待ち無限Loop処理へ移行する
 *					-# スレッド間通信キュー受信関数をCallする(BPF_RU_IPCM_MSGQ_RECV())
 *					-# 受信メッセージ解析関数をCallする(f_dhcp_anlz())
 *					-# 通信用共有メモリの解放(BPF_RU_IPCM_PROCMSG_ADDRFREE())
 * @param		N/A
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
VOID f_dhcp_main(void)
{
	VOID							*msgP;								/* 受信msgポインタ		*/
	UINT							rcvmsgsz;							/* 受信msgサイズ		*/
	INT								ret;								/* 戻り値				*/

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_dhcp", 0, 0, 0);

	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,						/* ProcessID			*/
								 D_SYS_THDID_PF_DHC,					/* ThreadID				*//*★*/
								 D_SYS_THDQID_PF_DHC,					/* ThreadQueueID		*//*★*/
								 (unsigned long int)pthread_self());	/* pThreadID			*/

	/* 初期化処理関数Call	*/
	(VOID)f_dhcp_InitReq( NULL );
	
	/************************************************************************************************************************/
	/* メッセージ受信待ち																									*/
	/************************************************************************************************************************/
	while(1)
	{
		/* メッセージ受信	*/
		ret = BPF_RU_IPCM_MSGQ_RECV(	D_SYS_THDQID_PF_DHC,			/* Thread Qid			*/
										0,								/* 応答待ち時間			*/
										(void **)&msgP,					/* 受信msgポインタ		*/
										&rcvmsgsz );					/* 受信msgサイズ		*/

		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}

		/* 受信メッセージ解析関数Call	*/
		(VOID)f_dhcp_anlz((VOID*)msgP);

		/* 通信用共有メモリの解放	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)msgP );
		
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}
	}
	return;
}

/* @} */
