/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_HcRsp.c
 *  @brief  Health check response handle function
 *  @date   2015/08/03 TDIPS)ikeda create
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
* @brief Health check response handle function
* @param linkno         [in] CPRI link number
* @param a_system_type  [in] the system that response health check
* @return None
* @Bug_No M-RRU-ZSYS-01769
* @date 2015/08/03 TDIPS)ikeda create
* @date 2015/09/04 TDI)satou SV制御レジスタの設定を追加
* @date 2015/10/12 TDI)satou M-RRU-ZSYS-01769 問処番号No.74対応
*/
VOID m_cr_HcRsp(USHORT linkno, USHORT a_system_type  )
{
	CRT_HC_CTL_MST_TBL* hctbl;
	INT							errcd;
	USHORT			a_prioind;	/* CPRI state check priority indication */
	UINT            svRegAdr;   /* SV制御レジスタアドレス */
	UINT            svRegDat;   /* SV制御レジスタ設定値 */

	if (CMD_SYS_S3G == a_system_type) {
	    svRegAdr = M_RRH_REG_CNT_S3G_L3_ERR_ADR(linkno);
	    svRegDat = M_RRH_REG_CNT_S3G_L3_ERR(linkno);
	} else {
	    svRegAdr = M_RRH_REG_CNT_3G_L3_ERR_ADR(linkno);
	    svRegDat = M_RRH_REG_CNT_3G_L3_ERR(linkno);
	}

	/* ヘルスチェックテーブル */
	hctbl = &crw_hc_ctl_mst_tbl[linkno][a_system_type];

	/* ヘルスチェック要求を送信している場合 */
	if (hctbl->hlc_sndsts == CRD_RECSTATE_SENDING)
	{
		/* ヘルスチェック応答確認タイマ停止 */
		if( cm_TStop( M_RRH_TIMID_HCRSP_CNF(linkno, a_system_type),&errcd )  != CMD_RES_OK)
	 		cm_Assert( CMD_ASL_DBGLOW, M_RRH_TIMID_HCRSP_CNF(linkno, a_system_type),
						 "Health check confirm timer stop NG" );

		/* ヘルスチェック送信状態に"ヘルスチェック未送信”を設定 */
		hctbl->hlc_sndsts = CRD_RECSTATE_NOSEND;

		/* 該当CPRIリンク番号のヘルスチェック無応答回数を0クリア */
		hctbl->hlc_nores_cnt = CMD_NUM0;

		/* 該当CPRIリンクがヘルスチェック起動中且つ	*/
		/* ヘルスチェック確立以外の場合				*/
		if ( (hctbl->hlc_srtupsts == CRD_RECSTATE_START) &&
			(hctbl->hlc_eststs != CRD_RECSTATE_EST) )
		{
			/* check priority with existing CPRI link state */
			m_cr_CpriStaPriorityChk( linkno, CRD_LINKEV_L3HCOK, CMD_NUM0, a_system_type, &a_prioind);

			/* below processing is for high priority ; perform CPRI disconnection */
			if (a_prioind == CRD_STAPRIO_HIGH)
			{
                /* 対REエラーレジスタオフ */
                m_cm_ErrCntRegSetRE(linkno, CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
				
				/* HC完了通知 */
				m_cr_HcStateChgNtc(linkno, a_system_type,CMD_ON);
				
				/* L3OK をRCTへ送信 */
				m_cr_CpriConnectNtc(linkno, a_system_type);
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FHM<->RE#%d L3 Health Check OK(system=%d)", linkno, a_system_type);
			}
			/* ヘルスチェック確立状態に"ヘルスチェック確立”を設定 */
			hctbl->hlc_eststs = CRD_RECSTATE_EST;
		}
		else
		{
			/* ヘルスチェック停止中、又はヘルスチェック確立中の場合、無処理 */
		}
	}
	else
	{
		/* ヘルスチェック要求未送信時の場合、無処理 */
	}

	return ;
}
/* @} */
/* @} */

