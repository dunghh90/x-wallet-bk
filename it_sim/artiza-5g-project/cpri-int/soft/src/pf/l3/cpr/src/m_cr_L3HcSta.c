/*!
 *  @skip   $Id$
 *  @file   m_cr_L3HcSta.c
 *  @brief  L3ヘルスチェック開始通知受信処理
 *  @author TDIPS)ikeda
 *  @date   2015/08/03 TDIPS)ikeda create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2009
 */

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"	/* CPRI管理 task header file */
/* @{ */

/*!
*  @brief  L3ヘルスチェック開始通知受信処理
*  @note   REに対してヘルスチェック要求送信を開始する ¥n
*  @param  linkno           [in] CPRI link number
*  @param  a_system_type    [in] system type
*  @return none
*  @date   2015/08/03 TDIPS)ikeda create
*  @date   2015/10/15 TDI)satou FHM技説-QA-090
*/
VOID m_cr_L3HcSta(USHORT linkno, USHORT a_system_type  )
{
	CMT_TSKIF_CPRISND_HCREQ*		a_hc_req_p;		/* ヘルスチェック要求	*/
	UINT		a_rtn				= CMD_RES_OK;
	UINT		a_timVal			= CMD_NUM0;		/*	タイマ値 			*/
	UINT		errcd;
	CMT_TSKIF_CPRISND_HCREQ			data;
	CRT_HC_CTL_MST_TBL* hctbl;

	/* ヘルスチェックテーブル */
	hctbl = &crw_hc_ctl_mst_tbl[linkno][a_system_type];

	if (hctbl->hlc_srtupsts == CRD_RECSTATE_STOP)
	{
		/* 該当CPRIリンク番号のヘルスチェック無応答回数をクリア */
		hctbl->hlc_nores_cnt = CMD_NUM0;

		/*==================================*/
		/* ヘルスチェック要求のフレーム作成 */
		/*==================================*/
		/* バッファ取得 */
		a_rtn = cm_L2BfGet( CMD_BUFCA_LAPSND,
							   sizeof( CMT_TSKIF_CPRISND_HCREQ ),
							   CMD_NUM1,
							   (VOID **)&a_hc_req_p );
		/* 戻り値判定 */
		if( a_rtn != CMD_RES_OK ){
			/* ASSERT */
			cm_Assert(	CMD_ASL_USELOW, a_rtn,"m_rs_HcReq cm_L2BfGet NG");
			return;
		}

		/* タスク間フレームのデータ作成 */
		a_hc_req_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
		a_hc_req_p->head.uiSrcTQueueID = CMD_TSKID_CPR;
		a_hc_req_p->head.uiLength = sizeof(CMT_TSKIF_CPRISND_HCREQ) ;
		a_hc_req_p->cprisnd_inf.link_num = linkno;
		a_hc_req_p->cpridat_hcreq.signal_kind = (CMD_CPRID_HCREQ + a_system_type);

		cm_MemCpy( &data, a_hc_req_p, a_hc_req_p->head.uiLength );

		m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_LPB, CMD_TSKIF_CPRISNDNTC, (UCHAR*)&data);

		/* CPRI信号送信通知(ヘルスチェック要求)送信 */
		a_rtn = m_cm_L3MsgSend_RE(linkno, a_hc_req_p);

		/* 送信OKの場合、応答確認タイマ設定 */
		if ( a_rtn == CMD_L3_OK )
		{
			/* set default value */
			a_timVal = D_TCM_SYSDFT_HCRSP_CNFTIM;

			/* ヘルスチェック応答確認タイマ起動 */
			a_rtn = cm_TStat( M_RRH_TIMID_HCRSP_CNF(linkno, a_system_type),
					TIMVAL_GET( a_timVal ), CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTRSPNTC,CMD_TSKID_CPR,(INT*)&errcd);
			if( a_rtn != CMD_RES_OK)
			{
				cm_Assert( CMD_ASL_DBGLOW, M_RRH_TIMID_HCRSP_CNF(linkno, a_system_type),
							"Health check re confirm timer start NG" );
			}
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FHM<->RE#%d L3 Health Check Start(system=%d)", linkno, a_system_type);

			/* ヘルスチェック送信状態に"ヘルスチェック送信中”を設定 */
			hctbl->hlc_sndsts = CRD_RECSTATE_SENDING;
		}
		
		/*
		 * ヘルスチェック送信間隔タイマ起動
		 * システムパラメータ変数D「ヘルスチェック要求確認タイマ」に応じて送信間隔を決定する.
		 * 変数－200msで600msより下回らない場合は、変数－200msとする.
		 * 変数－200msで600msより下回る場合は、600msとする.
		 */
		a_timVal =  cmw_sys_mng_tbl[a_system_type].hls_timer;
		if (a_timVal > (D_TCM_SYSDFT_HCRSP_ITVTIM_MIN + D_TCM_SYSDFT_HCRSP_ITVTIM_MINUS)) {
		    a_timVal -= D_TCM_SYSDFT_HCRSP_ITVTIM_MINUS;
		} else {
		    a_timVal = D_TCM_SYSDFT_HCRSP_ITVTIM_MIN;
		}

		a_rtn = cm_TStat( M_RRH_TIMID_HCSND_CNF(linkno, a_system_type),
				TIMVAL_GET(a_timVal), CMD_TIMMD_SINGLE,
				CMD_TSKIF_TIMOUTSNDNTC,CMD_TSKID_CPR,(INT*)&errcd);
		if( a_rtn != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, M_RRH_TIMID_HCRSP_CNF(linkno, a_system_type),
						"Health check re send confirm timer start NG" );
		}

		/* ヘルスチェック起動状態に"ヘルスチェック起動中"を設定 */
		hctbl->hlc_srtupsts = CRD_RECSTATE_START;
	}

	return;
}

/* @} */
/* @} */
 
