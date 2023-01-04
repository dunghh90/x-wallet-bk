/*!
 * @skip  $ld:$
 * @file  f_ini_logget_finish.c
 * @brief SIGUSR1受信時処理
 * @date  2014/02/01 ALPHA) yokoyama Create RRH-007-000 初期化
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-2016
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include    "BPF_COM_END.h"
#include <sys/wait.h> 

/*!
 * @brief	f_ini_logget_finish
 * @note 	関数処理内容.
 *			-# RESET実施の場合は以下の処理を行う
 *				-# プロセスKILL
 *				-# 障害ログ退避
 *				-# RESET実施
 *				-# WDT停止
 *			-# RESET実施無しの場合は以下の処理を行う
 *				-# 障害ログ退避(PF)
 * @param  signal info				[in] Signal情報
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2014/02/01 ALPHA) yokoyama Create
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @data  2016/03/14 ALPHA) fujjiiy modify for M-RRU-ZSYS-02088
 */
VOID f_ini_logget_finish( T_INIT_SIG_INFO_TBL* infoTbl ,VOID* buf_p )
{
	/********************************************************************/
	/* ログ保存処理														*/
	/********************************************************************/
	/* RESET制御する場合は、サイクリックログ/障害ログの保存処理を行う	*/
	/* ただし、REST制御の2回目はRESETが抑止される。						*/
	/* また、以下のシグナルにおいては、サイクリックログの保存処理を行う */
	/* ・WDT T.O														*/
	if(infoTbl->uiResetFlg_define == D_INI_SIG_RESET_ON){

		/* M-planeパケットの送受信割り込みをマスクし余計なパケットが送信されないようにする */
		BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_ETH_IRQMPM, 0x3);

		/* 既にDEADしているプロセスもいるかもしれないが全プロセスをKILLする	*/
		/* wait関数で待ってしまうと余計なログが出力されるのでwaitはしない	*/
		/* SYSTEM関数を使用すると遅いのでSignalを送信する	*/
		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_MPSW_NSA, SIGKILL, 0 );
		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_MPSW_SAA, SIGKILL, 0 );

		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_PF, SIGKILL, 0 );
		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_PTP, SIGKILL, 0 );
		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_L2, SIGKILL, 0 );
		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_MT, SIGKILL, 0 );
		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_L3, SIGKILL, 0 );
		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_F_PF, SIGKILL, 0 );
		BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_RE, SIGKILL, 0 );

		/* プロセスをKILLしてもFlashのmutexがLOCKされっぱなしになってしまう事象があるため	*/
		/* mutex lock無し書き込みを行う														*/
		(VOID)BPF_HM_DEVC_FORCE_ACCESS( D_RRH_ON );		

		/* 障害ログ退避	*/
		if( infoTbl->trouble_logget == D_RRH_ON )
		{
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "TROUBLE LOG SAVE START");
			f_ini_AccidentlogSave( infoTbl->logType , buf_p );
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "TROUBLE LOG SAVE END");
		}
		
		/* RESET処理を行う	*/
		/* 関数内でサイクリックログの保存を行っている	*/
		/* 実際にRESETさせるか否かはフラグによる		*/
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "Reset Start resetReason = 0x%08x. addInfo = 0x%08x",
													infoTbl->resetReason, infoTbl->addInfo );
		f_ini_reset( infoTbl->resetReason,infoTbl->addInfo, buf_p );

	}
	/* 一連の処理が完了したのでテーブルをクリアする	*/
	memset( infoTbl, 0, sizeof(T_INIT_SIG_INFO_TBL) );
	return;
}

/* @} */
