/*!
 * @skip		$ld:$
 * @file		f_lgc_main.c
 * @brief		ログスレッドメイン処理
 * @author		ALPHA)yokoyama
 * @date		2010/05/24 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010-2013
 */

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */

#include "f_lgc_inc.h"

/*!
 * @brief		関数機能概要:ログスレッドメイン。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note		関数処理内容.
 *				-# ログスレッド状態を"初期状態"に設定する
 *				-# 初期化関数をCallする(f_lgc_init())
 *				-# MSG待ち無限Loop処理へ移行する
 *					-# MSG受信関数をCallする(BPF_RU_IPCM_MSGQ_RECV())
 *					-# 受信メッセージ解析関数をCallする(f_lgc_anlz())
 * @retval		none
 * @return		none
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2010/05/24 ALPHA)横山 Create
 * @date 2013/03/28 ALPHA)松延 B25 対処
 * @date 2018/08/24 KCN)若水 カスケードを削除
 */
VOID f_lgc_main(VOID)
{
	VOID			*pMsg;
	UINT			rcvSize;
	INT				ret;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_lgc_main] ENTER" );

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_lgcm", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,								/* ProcessID		*/
									D_SYS_THDID_PF_LGC,								/* ThreadID			*/
									D_SYS_THDQID_PF_LGC,							/* ThreadQueueID	*/
									(unsigned long int)pthread_self()) ;			/* Rhread ID        */

	/* スレッド状態初期化 */
	f_lgcw_thdState  = D_LGC_THD_STS_INIT;
	
	/* 初期化処理関数Call */
	(VOID)f_lgc_init(NULL);
	
	/********************************************************************************************/
	/* メッセージ受信待ち																		*/
	/********************************************************************************************/
	while(1)
	{
		/****************************************************************************************/
		/* メッセージ受信																		*/
		/****************************************************************************************/
		ret = BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_PF_LGC,			/* 受信MSGキューID			*/
					0,								/* 受信待ち時間				*/
					(VOID**)&pMsg,					/* メッセージ受信バッファ	*/
					&rcvSize	);					/* 受信Messageサイズ		*/
		if( ret != 0 )
		{
			cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "[f_lgc_main] RECIEVE" );
			break;
		}
		
		/****************************************************************************************/
		/* 受信メッセージ解析関数Call															*/
		/****************************************************************************************/
		(VOID)f_lgc_anlz((VOID*)pMsg);
		
		/* 受信バッファ解放	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)pMsg );
		if( ret != 0 )
		{
			cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "[f_lgc_main] FREE" );
			break;
		}
	}
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_main] RETURN" );
	
	return;
}

/* @} */
