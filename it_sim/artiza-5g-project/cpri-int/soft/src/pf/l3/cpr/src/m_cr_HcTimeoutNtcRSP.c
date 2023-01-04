/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_HcTimeoutNtcRSP.c
 *  @brief  Health check responce T.O notice handle  function
 *  @date   2015/07/30 TDIPS)ikead create
 *  @date   2015/09/03 TDIPS)ikeda 安定化追加
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/
/** @addtogroup RRH_L3_CPR
* @{ */
#include "m_cm_header.h"
#include "m_cr_header.h"
#include "rrhApi_L2.h"
/**
* @brief Health check confirm T.O notice handle  function
* @note T.O count vaule add up, perform L3 link disconnect if T.O count value over the limitation.\n
* @param buff_adr [in] pointer of received message
* @return None
* @Bug_No M-RRU-ZSYS-01770
* @Bug_No M-RRU-ZSYS-01803
* @Bug_No M-RRU-ZSYS-01986
* @date 2015/07/30 TDIPS)ikeda create trif.t_lc_L3hcRspTO
* @date 2015/09/04 TDI)satou SV制御レジスタ設定値をFHM用に変更
* @date 2015/10/12 TDI)satou M-RRU-ZSYS-01770 問処番号No.75対応
* @date 2015/10/26 TDIPS)ikeda M-RRU-ZSYS-01803 ヘルスチェック送信が再開しない不具合を修正
* @date 2015/11/12 TDIPS)sasaki M-RRU-ZSYS-01986 IT2問処No.179対処 ヘルスチェック無応答回数のカウントアップ条件変更
*/
VOID m_cr_HcTimeoutNtcRSP( UINT * buff_adr )
{
	CMT_TSKIF_HEAD		*head_pt;					/**<  タスク共通ヘッダ		 */
	UINT		a_system_type;
	USHORT		linkno;
	CRT_HC_CTL_MST_TBL* hctbl;
	UINT                svRegAdr;                   /* SV制御レジスタアドレス */
	UINT                svRegDat;                   /* SV制御レジスタ設定値 */

	/* タイマーIDからリンク番号と信号種別を計算 */
	head_pt = (CMT_TSKIF_HEAD*) (buff_adr);
	if(head_pt->uiSrcPQueueID >= E_RRH_TIMID_HCRSP1_CNF_S3G)
	{
		linkno = (head_pt->uiSrcPQueueID - E_RRH_TIMID_HCRSP1_CNF_S3G) + D_RRH_CPRINO_RE_MIN;
		a_system_type = CMD_SYS_S3G;
		svRegAdr = M_RRH_REG_CNT_S3G_L3_ERR_ADR(linkno);
		svRegDat = M_RRH_REG_CNT_S3G_L3_ERR(linkno);
	}
	else
	{
		linkno = (head_pt->uiSrcPQueueID - E_RRH_TIMID_HCRSP1_CNF_3G) + D_RRH_CPRINO_RE_MIN;
		a_system_type = CMD_SYS_3G;
		svRegAdr = M_RRH_REG_CNT_3G_L3_ERR_ADR(linkno);
		svRegDat = M_RRH_REG_CNT_3G_L3_ERR(linkno);
	}

	/* ヘルスチェックテーブル */
	hctbl = &crw_hc_ctl_mst_tbl[linkno][a_system_type];

	/* ヘルスチェック送信状態に"要求未送信"を設定 */
	hctbl->hlc_sndsts = CRD_RECSTATE_NOSEND;

	/* 該当リンクのヘルスチェックが起動中且つヘルスチェック断以外の場合 */
	if( (hctbl->hlc_srtupsts == CRD_RECSTATE_START) &&
		(hctbl->hlc_eststs != CRD_RECSTATE_DISCONNT) )
	{
		/* 該当CPRIリンク番号のヘルスチェック無応答回数をインクリメント */
		hctbl->hlc_nores_cnt++;

		/* 無応答回数が保護段数以上の場合、ヘルスチェック断を検出 */
		if( hctbl->hlc_nores_cnt > D_TCM_SYSDFT_HCRSP_UNCNFNUM)
		{
			/* L3 CPRI ERR register Write */
			m_cm_ErrCntRegSetRE(linkno, CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);

			/* ヘルスチェック確立状態に"ヘルスチェック断”を設定 */
			hctbl->hlc_eststs = CRD_RECSTATE_DISCONNT;

			/* ヘルスチェック停止(REC/RE共用) */
			cmw_hcsta[linkno][a_system_type] = CMD_OFF;

			/* ヘルスチェック状態変更通知 */
			m_cr_HcStateChgNtc(linkno, a_system_type, CMD_OFF);

			/* 切断通知 */
			m_cr_CpriDisConnectNtc(linkno, a_system_type, E_RRH_RAYER_KIND_3);
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FHM<->RE#%d L3 Health Check NG(system=%d)", linkno, a_system_type);
		}
		else
		{
			/* ヘルスチェック応答未確認保護段数(REC)未満の場合、無処理 */
		}
	}
	else
	{
		/* ヘルスチェック停止中、又はヘルスチェック断の場合、無処理 */
	}

	return ;


}
/* @} */

