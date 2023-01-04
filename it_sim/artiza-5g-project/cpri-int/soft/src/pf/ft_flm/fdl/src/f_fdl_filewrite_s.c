/*!
 * @skip  $ld:$
 * @file  f_fdl_filewrite_s.c
 * @brief FDLタスク　ファイル更新書込み応答(スレーブ)
 * @date  2019/02/23 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_fdl_inc.h"

/*!
 * @brief 関数機能概要:スレーブ側のファイル更新応答を受信する
 * @note  関数処理内容.
 *		-# スレーブ側のファイル更新応答を受信しファイル更新エラーの有無をチェック
 *		-# ファイル更新エラーがあればnotificatio(エラー)を返却
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/03/06 KCN)上本 Create
 */
INT	f_fdl_filewrite_s(VOID* rcvMsg_p)
{
	INT				rtn = D_SYS_OK ;
	int				ret ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "f_fdl_filewrite_s start.") ;

	if(f_fdl_swact_data[D_DFL_MASTR_ACSS] == D_FDL_ACT_STATE_ERROR)
	{
		f_fdl_thdInfo.thdState = D_FDL_TASK_STS_ACT ;
	}else {
		ret = f_fdl_install_check(rcvMsg_p , E_FDL_FLSH_FPGAS) ;
		if(ret != D_SYS_OK)
		{
			/* エラー処理 */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , " FPGA(Slave) Install Error") ;
			f_fdl_swact_data[D_DFL_SLAVE_ACSS] = D_FDL_ACT_STATE_ERROR ;
			rtn = D_SYS_NG ;
		}else {
			/* ファイル更新終了チェック確認 */
			ret = f_fdl_file_check() ;
			if(ret == D_SYS_OK)
			{
				/* 総合情報(ベンダコード/ビルドファイル名/バージョン)の更新 */
				ret = f_fdl_install_data_save() ;
				if( ret != D_SYS_OK)
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "Install Data Set Error") ;
					f_fdl_send_MSInotif(E_FDL_ERR_BUFERR , f_fdl_inst_state_data.slot ) ;
					rtn = D_SYS_NG ;
					f_fdl_thdInfo.thdState = D_FDL_TASK_STS_ACT ;
				}
				else {
					/* OK応答(Notification通知)の送信 */
					f_fdl_send_MSInotif(E_FDL_ERR_OK , f_fdl_inst_state_data.slot ) ;
					f_fdl_thdInfo.thdState = D_FDL_TASK_STS_ACT ;
				}
			}
		}
	}

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "f_fdl_filewrite_s end.") ;
	return rtn ;
}
/* @} */


