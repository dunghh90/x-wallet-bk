/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_HcReq.c
 *  @brief  Health check request handle function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/09/10 FFCS)niemsh modified for M-S3G-eNBPF-02452 
 *  @date   2010/01/04 QNET)Kabasima ADD M-S3G-eNBPF-02869:[品質改善]REデバッグコマンド機能拡充
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"			/* RE監視タスクヘッダファイル			*/

/* @{ */
/**
* @brief Health check request handle function
* @note Clear L3 link err, send Health check response, and start Health check confirm timer.\n
* @param a_system_type [in] the system that request health check
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2015/08/04 TDI)ikeda 17リンク対応
* @date 2015/09/04 TDI)satou FHM用にSV制御レジスタ設定を見直し
* @date 2021/03/03 M&C) Update src based on No.27 (seq 9-1,2) - Respond NG if 3G
*/
VOID m_cr_HcReq(  USHORT a_system_type  )
{
	/*======================================================================*/
	/* ■ヘルスチェック要求(初回受信時)の処理								*/
	/*  ・ヘルスチェック応答を送信											*/
	/*  ・L3 ERR解除														*/
	/*  ・RE状態を"REアイドル状態"→"RE運用中状態"に遷移					*/
	/*  ・L3ヘルスチェック確立通知を送信									*/
	/*  ・ヘルスチェック要求確認タイマを起動								*/
	/*																		*/
	/* ■ヘルスチェック要求(２回目以降の受信時)の処理						*/
	/*  ・ヘルスチェック要求確認タイマを停止								*/
	/*  ・ヘルスチェック応答を送信											*/
	/*  ・ヘルスチェック要求確認タイマを起動								*/
	/*======================================================================*/
	
	UINT	a_rtn			= CMD_RES_OK;	/* 戻り値格納変数				*/
	UINT	a_L3rtn __attribute__((unused))			= CMD_RES_OK;	/* 戻り値格納変数				*/
	USHORT	a_tim_val		= CMD_NUM0;		/* ヘルスチェック確認タイマ値	*/
	CMT_TSKIF_CPRISND_HCRES	*a_hc_res_p;	/* ヘルスチェック応答アドレス	*/
	UINT		sigchk_rslt;				/*  result of checking signal */
	USHORT		sigsys_rslt;				/*  system by checking signal */
	USHORT		signal_kind;				/*  checking signal kind */
	USHORT		prioind;
	INT			errcd;
	UINT		bMntNtc = CMD_OFF;
	UINT        svRegAdr;                   /* SV制御レジスタアドレス */
	UINT        svRegDat;                   /* SV制御レジスタ設定値 */

	if((cmw_filedatsnd_flag !=0)&&(a_system_type == 1))
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "hc req Start(system=%d):flag=%d", a_system_type, cmw_filedatsnd_flag);
	}
	/* validate signal */
	signal_kind = CMD_CPRID_HCREQ + a_system_type;
	sigchk_rslt = m_cm_chksig(signal_kind,&sigsys_rslt);
	if(sigchk_rslt!= CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
					signal_kind, "validate signal NG");

		if (sigsys_rslt == CMD_SYS_3G) {
			/* タスク間フレームのデータ作成 */
			a_hc_res_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
			a_hc_res_p->head.uiSrcTQueueID = CMD_TSKID_CPR;
			a_hc_res_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(USHORT) ;
			a_hc_res_p->cpridat_hcres.signal_kind = CMD_CPRID_HCRES;
			a_hc_res_p->cpridat_hcres.result = CMD_OTHR_ERR_RE;

			/* LAPBタスクへヘルスチェック応答を送信(L3信号転送) */
			a_L3rtn = m_cm_L3MsgSend_REC( a_hc_res_p);
			if (a_L3rtn != CMD_L3_OK) {
				cm_Assert(CMD_ASL_USELOW, a_L3rtn, "m_cr_HcReq MSG Snd NG");
			}
		}

		return;
	}

	if (CMD_SYS_S3G == a_system_type) {
	    svRegAdr = M_RRH_REG_CNT_S3G_L3_ERR_ADR(D_RRH_CPRINO_REC);
	    svRegDat = M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_REC);
	} else {
	    svRegAdr = M_RRH_REG_CNT_3G_L3_ERR_ADR(D_RRH_CPRINO_REC);
	    svRegDat = M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_REC);
	}

	/* タイムアウトカウント値をクリア */
	crw_hc_ctl_tbl[a_system_type].to_count = 0;
	
	/************************************************/
	/* ヘルスチェック要求(２回目以降の受信時)の処理 */
	/************************************************/
	if( crw_hc_ctl_tbl[a_system_type].re_state == CRD_RESTATE_OPERATE ){
		/* ヘルスチェック要求確認タイマを停止する */
		a_rtn = cm_TStop( crr_hctimno[a_system_type],&errcd );
	}
	
	/*==================================*/
	/* ヘルスチェック応答のフレーム作成 */
	/*==================================*/
	/* バッファ取得 */
	a_rtn = cm_L2BfGet( CMD_BUFCA_LAPSND, 
						   sizeof( CMT_TSKIF_CPRISND_HCRES ), 
						   CMD_NUM1, 
						   (VOID **)&a_hc_res_p );
	/* 戻り値判定 */
	if( a_rtn != CMD_RES_OK ){
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					a_rtn,"m_rs_HcReq cm_L2BfGet NG");
		return;
	}
	/******************************************************/
	/* ヘルスチェック要求初回受信時でL3転送が成功した場合 */
	/******************************************************/
	if(( crw_hc_ctl_tbl[a_system_type].re_state == CRD_RESTATE_IDLE ) &&
	   ( a_rtn == CMD_RES_OK )){

		/* L3 ERRを解除 */
		m_cm_ErrCntRegSet( CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY );
		
		/* RE状態を"RE運用中状態"に設定 */
		crw_hc_ctl_tbl[a_system_type].re_state = CRD_RESTATE_OPERATE;
		f_cmn_com_layer3_set_each(D_RRH_CPRINO_REC, a_system_type, E_RRH_LAYER3_REC_OPE);
		
		m_cr_CpriStaPriorityChk(D_RRH_CPRINO_REC, CRD_LINKEV_L3HCOK, CMD_NUM0, a_system_type, &prioind);

		cmw_hcsta[D_RRH_CPRINO_REC][a_system_type] = CMD_ON;
		
		bMntNtc = CMD_ON;


		cm_Assert(CMD_ASL_USELOW,a_system_type,"L3 Health Check OK");
	}

	/* タスク間フレームのデータ作成 */
	a_hc_res_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	a_hc_res_p->head.uiSrcTQueueID = CMD_TSKID_CPR;
	a_hc_res_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(USHORT) ;
	a_hc_res_p->cpridat_hcres.signal_kind = (CMD_CPRID_HCRES + a_system_type);

	/* LAPBタスクへヘルスチェック応答を送信(L3信号転送) */
	a_L3rtn = m_cm_L3MsgSend_REC( a_hc_res_p );

	/* システムパラメータ情報管理テーブルよりタイマ値を取得 */
	a_tim_val = cmw_sys_mng_tbl[a_system_type].hls_timer;
	
	/* ヘルスチェック要求確認タイマを起動する */
	a_rtn = cm_TStat( crr_hctimno[a_system_type], TIMVAL_GET( a_tim_val ), CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC,CMD_TSKID_CPR,(INT*)&errcd);
	if(bMntNtc == CMD_ON)
	{
		m_cr_HcStateChgNtc(D_RRH_CPRINO_REC, a_system_type,CMD_ON);
	}
	if((cmw_filedatsnd_flag !=0)&&(a_system_type == 1))
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "hc req End(system=%d):flag=%d", a_system_type, cmw_filedatsnd_flag);
	}
	
	return ;
}
/* @} */
/* @} */

