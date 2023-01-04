/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsndcmprsp_waiting_syspsndcmp_cnf_to.c
 * @brief  Event:システムパラメータ送信完了確認タイマタイムアウト通知, 対REC状態:ファイルデータ送信完了報告応答(シスパラ)集約中.
 * @date   2015/08/22 FPT)Yen Create
 * @date   2015/10/15 TDIPS)sasaki Update
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
/*!
 *  @brief Event:システムパラメータ送信完了確認タイマタイムアウト通知, 対REC状態:ファイルデータ送信完了報告応答(シスパラ)集約中.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01857
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/10/09 TDIPS)Lay M-RRU-ZSYS-01857 判定誤りを修正
 *  @date   2015/10/15 FPT)Duong Update review comment
 *  @date   2015/10/15 TDIPS)sasaki Update MKレビュー指摘No.261対応ミスを修正
 *  @date   2015/10/15 TDIPS)sasaki Update IT2問処No.91水平展開
 *  @date   2015/11/17 FPT)Yen update (その他未実装-015)
 *  @date	2016/03/29 TDI)satou M-RRU-ZSYS-02090 EEPROMに変数E,Fを保存する処理を追加
 *  @date	2016/04/05 TDI)satou M-RRU-ZSYS-02090 EEPROMに変数E,Fを保存する処理を削除
 */
VOID m_dl_rl_fsndcmprsp_waiting_syspsndcmp_cnf_to(VOID* bufp, USHORT cpri_no)
{
	USHORT		lwReNo;
	USHORT		lwSysType;
	USHORT		lwReResResult = CMD_ALLRE_NG;	/* 223:全配下RE応答NG */
	USHORT		lwResResult;
	USHORT		lwTimerId;
	USHORT		lwTimerVal;
	INT			ldwErrcd;
	INT			ldwResult;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsndcmprsp_waiting_syspsndcmp_cnf_to] ENTER" );

	lwSysType = (( CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;

	for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
	{
		/* 未応答REを対象に対RE状態遷移(REC主導)へ入力する */
		if( DLD_RESTA_FILE_COMP_SYSPRM_WAIT == gt_dlw_refw_all_tbl[lwSysType][lwReNo - CMD_NUM1].re_sta && 
			gt_dlw_refw_all_tbl[lwSysType][lwReNo - CMD_NUM1].filedata_trans_rslt == DLD_RESLT_IDLE )
		{
			m_dl_rec_main(bufp, lwReNo);
		}
		/* 1台以上の配下REから応答結果「0:正常」を受信 */
		if( DLD_RESLT_NML == gt_dlw_refw_all_tbl[lwSysType][lwReNo - CMD_NUM1].filedata_trans_rslt )
		{
			lwReResResult = CMD_NML;
		}
	}

	/* FHMのチェックがNGの場合 */
	if( CMD_NML != (*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt )
	{
		/* REからのOK/NGに関係なく、RECにはNG(NG要因は①チェック結果) */
		lwResResult = (*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt;
	}
	/* FHMのチェックがOKの場合 */
	else
	{
		/* REからのOK応答が一つでもあれば、RECにOK */
		/* REから全NGであれば、RECにNG(223:全配下RE応答NG) */
		lwResResult = lwReResResult;
	}

	/* ファイルデータ送信完了報告応答送信処理起動 */
	if(lwSysType == CMD_SYS_3G)
	{
		m_dl_cm_CRDataEndRspSnd(lwResResult, lwSysType);
	}
	else
	{
		m_dl_cm_CRDataEndRspSnd_S3G(lwResResult, lwSysType, dlw_prisys_tbl);
	}
	/* 全配下REからの応答受信済、かつOK応答有 */
	if( CMD_NML == lwResResult )
	{
		for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
		{
			gt_dlw_refw_all_tbl[lwSysType][lwReNo-1].re_sta = DLD_RESTA_IDLE;
		}
		/* Change 対REC状態 => アイドル */
		(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;
	}
	else
	{
		/* タイマ起動 システムパラメータ送信確認タイマ */
		lwTimerId = CMD_TIMID_SYSPRMSND_3G + lwSysType;
		lwTimerVal = cmw_sys_mng_tbl[lwSysType].sys_timer;
		/* タイマ起動 システムパラメータ情報報告確認タイマ */
		ldwResult = cm_TStat( 	lwTimerId,
								TIMVAL_GET(lwTimerVal),			/* 単位:10 msec */
								CMD_TIMMD_SINGLE,
								CMD_TSKIF_TIMOUTNTC, CMD_TSKID_DLM, &ldwErrcd);
		/*Start timer NG*/
		if(ldwResult != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_USELOW, lwTimerId, "システムパラメータ送信確認イマ Start NG" );
		}
		/* Change 対REC状態 => REC主導 ファイルデータ送信(シスパラ)待ち */
		(*gt_dlw_rec_all_tbl[lwSysType]).rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_SYSPARA_WAIT;
		
		/* 受信済み分割番号初期化(=0) */
		(*gt_dlw_rec_all_tbl[lwSysType]).rcv_div_no = CMD_NUM0;
		/* 受信済みデータサイズ初期化(=0) */
		(*gt_dlw_rec_all_tbl[lwSysType]).rcv_data_size = CMD_NUM0;
		/* REC向けファイルデータ送信完了報告応答結果設定 0:正常 */
		(*gt_dlw_rec_all_tbl[lwSysType]).filedata_trans_rslt = CMD_NML;
	}
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsndcmprsp_waiting_syspsndcmp_cnf_to] RETURN" );

	return ;
}


/* @} */

