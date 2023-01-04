/*!
 * @skip		$ld:$
 * @file		f_ini_main_alm.c
 * @brief		ALMメイン処理
 * @author		ALPHA)yokoyama
 * @date		2014/02/01 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"

/*!
 * @brief		関数機能概要:ALMスレッドメイン。スレッド起動からMSG受信待ちのLoop処理を実施
 * @note		関数処理内容.
 *				-# 状態を"初期状態"に設定する
 *				-# 初期化関数をCallする(f_log_init())
 *				-# MSG待ち無限Loop処理へ移行する
 *					-# MSG受信関数をCallする(BPF_RU_IPCM_MSGQ_RECV())
 *					-# 受信メッセージ解析関数をCallする(f_log_anlz())
 * @retval		none
 * @return		none
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/02/01 ALPHA)横山 Create
 */
VOID f_ini_main_alm(VOID)
{
	VOID			*pMsg;
	UINT			rcvSize;
	INT				ret;

	f_iniw_assert_alm_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"Init alm thread");
	
	cmn_com_assert_th_get(f_iniw_assert_alm_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_main_alm] ENTER" );

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "ini_alm", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_INI,				/* ProcessID		*/
									D_SYS_THDID_INIT_ALM,			/* ThreadID			*/
									D_SYS_THDQID_INI_ALM,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());

	/* スレッド状態初期化 */
	f_iniw_sig_info_alm.state = 0;

	/********************************************************************************************/
	/* メッセージ受信待ち																		*/
	/********************************************************************************************/
	while(1)
	{
		/****************************************************************************************/
		/* メッセージ受信																		*/
		/****************************************************************************************/
		ret = BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_INI_ALM,			/* 受信MSGキューID			*/
					0,								/* 受信待ち時間				*/
					(VOID**)&pMsg,					/* メッセージ受信バッファ	*/
					&rcvSize	);					/* 受信Messageサイズ		*/
		if( ret != 0 )
		{
			cmn_com_assert_th_get(f_iniw_assert_alm_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_alm] RECEIVE" );
			break;
		}
		
		/****************************************************************************************/
		/* 受信メッセージ解析関数Call															*/
		/****************************************************************************************/
		if(((T_INIT_SIG_NTC*)pMsg)->head.uiEventNo == 0){
			(VOID)(*(f_inir_callSetRell_mtxtbl_alm[f_iniw_sig_info_alm.state][((T_INIT_SIG_NTC*)pMsg)->head.uiEventNo].eventProcess_addr))(pMsg , f_iniw_assert_alm_p);
		}else{
			cmn_com_assert_th_get(f_iniw_assert_alm_p, D_RRH_LOG_AST_LV_ERROR, "ERROR EVT:0x%08x", ((T_INIT_SIG_NTC*)pMsg)->head.uiEventNo);
		}
		/* 受信バッファ解放	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)pMsg );
		if( ret != 0 )
		{
			cmn_com_assert_th_get(f_iniw_assert_alm_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_alm] FREE" );
			break;
		}
	}
	
	cmn_com_assert_th_get(f_iniw_assert_alm_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_main_alm] RETURN" );
	
	return;
}

/* @} */
