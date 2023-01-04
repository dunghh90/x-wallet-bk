/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_HcTimeoutNtcSND.c
 *  @brief  Health check T.O notice handle  function
 *  @date   2015/07/30 TDIPS)ikead create
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
* @Bug_No M-RRU-ZSYS-01965
* @date 2015/07/30 TDIPS)ikeda create trif.t_lc_L3hcSndTO
* @date 2015/10/15 TDI)satou FHM技説-QA-090
* @date 2015/11/07 koshida M-RRU-ZSYS-01965 タイマ起動の条件見直し 
*/
VOID m_cr_HcTimeoutNtcSND( UINT * buff_adr )
{
	CMT_TSKIF_CPRISND_HCREQ*		a_hc_req_p;		/* ヘルスチェック要求	*/
	UINT		a_timVal			= CMD_NUM0;		/* タイマ値 			*/
	UINT		a_rtn				= CMD_RES_OK;
	UINT		errcd;
	CMT_TSKIF_HEAD		*head_pt;					/* タスク共通ヘッダ		*/
	UINT		a_system_type;
	USHORT		linkno;
	CMT_TSKIF_CPRISND_HCREQ			data;
	CRT_HC_CTL_MST_TBL* hctbl;

	/* タイマーIDからリンク番号と信号種別を計算 */
	head_pt = (CMT_TSKIF_HEAD*) (buff_adr);
	if(head_pt->uiSrcPQueueID >= E_RRH_TIMID_HCSND1_CNF_S3G)
	{
		linkno = (head_pt->uiSrcPQueueID - E_RRH_TIMID_HCSND1_CNF_S3G) + D_RRH_CPRINO_RE_MIN;
		a_system_type = CMD_SYS_S3G;
	}
	else
	{
		linkno = (head_pt->uiSrcPQueueID - E_RRH_TIMID_HCSND1_CNF_3G) + D_RRH_CPRINO_RE_MIN;
		a_system_type = CMD_SYS_3G;
	}

	/* ヘルスチェックテーブル */
	hctbl = &crw_hc_ctl_mst_tbl[linkno][a_system_type];

	/* 該当リンクがヘルスチェック起動の場合 */
	if(hctbl->hlc_srtupsts == CRD_RECSTATE_START)
	{
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
		/* ヘルスチェック要求送信を未送信中の場合 */
		if(hctbl->hlc_sndsts == CRD_RECSTATE_NOSEND)
		{
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

				/* ヘルスチェック送信状態に"ヘルスチェック送信中”を設定 */
				hctbl->hlc_sndsts = CRD_RECSTATE_SENDING;

			}
			else
			{
				m_cr_cm_RunHisNG(linkno);

				cm_Assert(CMD_ASL_USELOW, linkno, "L3 message send NG" );
			}
		}
		else
		{
			m_cr_cm_RunHisNG(linkno);

			cm_Assert(CMD_ASL_USELOW, linkno, "L3 message send NG" );
		}
	}
	else
	{
		/* ヘルスチェック停止中、又はヘルスチェック送信中の場合、無処理 */
	}

	return;
}
/* @} */

