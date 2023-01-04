/*!
 * @skip		$ld:$
 * @file		f_ini_main_run.c
 * @brief		RUNメイン処理
 * @author		ALPHA)yokoyama
 * @date		2014/02/01 ALPHA)横山 Create
 * @date  2015/07/28 TDIPS) maruyama Create(PFからの移動）
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_PF_INI
 * @{
 */

#include "f_ini_inc.h"

/*!
 * @brief		関数機能概要:RUNスレッドメイン。スレッド起動からMSG受信待ちのLoop処理を実施
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
VOID f_ini_main_run(VOID)
{
	VOID			*pMsg;
	UINT			rcvSize;
	INT				ret;
	INT				run_kind = 0;


	f_iniw_assert_run_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"Init run thread");

	cmn_com_assert_th_get(f_iniw_assert_run_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_main_run] ENTER" );

	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "ini_run", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_INI,				/* ProcessID		*/
									D_SYS_THDID_INIT_RUN,			/* ThreadID			*/
									D_SYS_THDQID_INI_RUN,			/* ThreadQueueID	*/
									(unsigned long int)pthread_self());
	
	/* スレッド状態初期化 */
	f_iniw_sig_info_run[0].state = 0;
	f_iniw_sig_info_run[1].state = 0;

	/********************************************************************************************/
	/* メッセージ受信待ち																		*/
	/********************************************************************************************/
	while(1)
	{
		/****************************************************************************************/
		/* メッセージ受信																		*/
		/****************************************************************************************/
		ret = BPF_RU_IPCM_MSGQ_RECV(
					D_SYS_THDQID_INI_RUN,			/* 受信MSGキューID			*/
					0,								/* 受信待ち時間				*/
					(VOID**)&pMsg,					/* メッセージ受信バッファ	*/
					&rcvSize	);					/* 受信Messageサイズ		*/
		if( ret != 0 )
		{
			cmn_com_assert_th_get(f_iniw_assert_run_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_run] RECEIVE" );
			break;
		}
		
		if( (((T_INIT_SIG_NTC*)pMsg)->sigInfo.si_status == D_RRH_SIGUSR_OPT_RUNLOG_REQ)||
		    (((T_INIT_SIG_NTC*)pMsg)->sigInfo.si_status == D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_RUN))
		{
			run_kind = 0;
		}
		else
		{
			run_kind = 1;
		}
		
		
		
		/****************************************************************************************/
		/* 受信メッセージ解析関数Call															*/
		/****************************************************************************************/
		(VOID)(*(f_inir_callSetRell_mtxtbl_run[f_iniw_sig_info_run[run_kind].state][((T_INIT_SIG_NTC*)pMsg)->head.uiEventNo].eventProcess_addr))(&((T_INIT_SIG_NTC*)pMsg)->sigInfo, f_iniw_assert_run_p );
		
		/* 受信バッファ解放	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)pMsg );
		if( ret != 0 )
		{
			cmn_com_assert_th_get(f_iniw_assert_run_p, D_RRH_LOG_AST_LV_ERROR, "[f_ini_main_run] FREE" );
			break;
		}
	}
	
	cmn_com_assert_th_get(f_iniw_assert_run_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_main_run] RETURN" );
	
	return;
}

/* @} */
