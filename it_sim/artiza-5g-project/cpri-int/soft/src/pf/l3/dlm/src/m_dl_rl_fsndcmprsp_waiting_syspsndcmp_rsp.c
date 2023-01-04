/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsndcmprsp_waiting_syspsndcmp_rsp.c
 * @brief  Event:ファイルデータ送信完了報告応答, 対REC状態:ファイルデータ送信完了報告応答(シスパラ)集約中.
 * @date   2015/08/22 FPT)Yen Create
 * @date   2015/09/26 TDIPS)sasaki Update
 * @date   2015/09/29 TDIPS)sasaki Update 
 * @date   2015/10/15 TDIPS)sasaki Update
 * @date   2015/10/26 TDIPS)sasaki Update

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
 *  @brief  Event:ファイルデータ送信完了報告応答, 対REC状態:ファイルデータ送信完了報告応答(シスパラ)集約中
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/08/22 FPT)Yen Create
 */

VOID m_dl_rl_fsndcmprsp_waiting_syspsndcmp_rsp(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILSNDFINRES*		ltFilSndFinRes_p;
	USHORT								lwSysType;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsndcmprsp_waiting_syspsndcmp_rsp] ENTER" );

	ltFilSndFinRes_p = (CMT_TSKIF_CPRIRCV_FILSNDFINRES *)bufp;
	lwSysType = ltFilSndFinRes_p->cpridat_filsndfinres.signal_kind & CMD_SYS_CHK_CPRID;

	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	m_dl_rl_fsndcmprsp_waiting_sysp_proc(lwSysType);
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsndcmprsp_waiting_syspsndcmp_rsp] RETURN" );
	return ;
}

/*!
 *  @brief  Event:ファイルデータ送信完了報告応答, 対REC状態:ファイルデータ送信完了報告応答(シスパラ)集約中.
 *  @note   
 *  @param  awSysType       [in]    system type
 *  
 *  @return Void.
 *  @Bug_No M-RRU-ZSYS-01727
 *  @Bug_No M-RRU-ZSYS-01786
 *  @Bug_No M-RRU-ZSYS-01906
 *  @date   2015/08/22 FPT)Yen Create
 *  @date   2015/09/26 TDIPS)sasaki Update MKレビュー指摘No.205対応
 *  @date   2015/09/29 TDIPS)sasaki M-RRU-ZSYS-01727 Update 正常終了時にも受信済み分割番号とデータサイズを初期化するよう修正
 *  @date   2015/10/09 FPT)Duong update MKレビュー指摘No.207対応
 *  @date   2015/10/08 FPT)Duong update MKレビュー指摘No.198対応
 *  @date   2015/10/15 TDIPS)sasaki M-RRU-ZSYS-01786 Update IT2問処No.91対応 対REC状態のアイドル遷移時に全対RE状態をアイドルへ遷移
 *  @date   2015/10/26 TDIPS)sasaki M-RRU-ZSYS-01906 Update IT2問処No.139対処
 *  @date   2015/11/17 FPT)Yen update (その他未実装-015)
 *  @date	2016/03/29 TDI)satou M-RRU-ZSYS-02090 EEPROMに変数E,Fを保存する処理を追加
 *  @date	2016/04/05 TDI)satou M-RRU-ZSYS-02090 EEPROMに変数E,Fを保存する処理を削除
 */
VOID m_dl_rl_fsndcmprsp_waiting_sysp_proc(USHORT awSysType)
{
	USHORT								lwReNo;
	USHORT								lwReResResult = CMD_ALLRE_NG;	/* 223:全配下RE応答NG */
	USHORT								lwResResult;
	USHORT								lwTimerId;
	USHORT								lwTimerVal;
	USHORT								lwDiscLinks = 0;
	INT									llErrcd;
	INT									llResult;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsndcmprsp_waiting_sysp_proc] ENTER" );

	/* 応答(REC向け応答結果) */
//	lwResResult = (*gt_dlw_rec_all_tbl[awSysType]).filedata_trans_rslt;

	for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
	{
		/* 未応答有り: 対RE状態がファイルデータ送信完了報告応答(シスパラ)待ち */
		if( DLD_RESTA_FILE_COMP_SYSPRM_WAIT == gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].re_sta && 
			gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].filedata_trans_rslt == DLD_RESLT_IDLE )
		{
			/* 対REC状態:REC主導ファイルデータ送信完了報告応答(シスパラ)集約中 */
			/* 遷移なし */
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
			return;
		}
		if( DLD_RESTA_IDLE == gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].re_sta &&
			DLD_RESLT_IDLE == gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].fileinf_rep_rslt)
		{
			lwDiscLinks++;
		}

		/* 1台以上の配下REから応答結果「0:正常」を受信 */
		if( DLD_RESLT_NML == gt_dlw_refw_all_tbl[awSysType][lwReNo - CMD_NUM1].filedata_trans_rslt )
		{
			lwReResResult = CMD_NML;
		}
	}
	/* 配下REなし */
	if(lwDiscLinks == D_RRH_CPRINO_RE_MAX)
	{
		lwReResResult = CMD_NML;
	}

	/* タイマ停止 システムパラメータ送信完了確認タイマ */
	/* All Re use 1 timer -> Fix Cpri = 1 */
	lwTimerId = CMD_TIMID_SYSPRM_SNDFIN + CMD_NUM1 +  D_RRH_CPRINO_RE_MAX * awSysType - CMD_NUM1;
	cm_TStop( lwTimerId, &llErrcd );

	/* FHMのチェックがNGの場合 */
	if( CMD_NML != (*gt_dlw_rec_all_tbl[awSysType]).filedata_trans_rslt )
	{
		/* REからのOK/NGに関係なく、RECにはNG(NG要因は①チェック結果) */
		lwResResult = (*gt_dlw_rec_all_tbl[awSysType]).filedata_trans_rslt;
	}
	/* FHMのチェックがOKの場合 */
	else
	{
		/* REからのOK応答が一つでもあれば、RECにOK */
		/* REから全NGであれば、RECにNG(223:全配下RE応答NG) */
		lwResResult = lwReResResult;
	}

	/* ファイルデータ送信完了報告応答送信処理起動 */
	if(awSysType == CMD_SYS_3G)
	{
		m_dl_cm_CRDataEndRspSnd(lwResResult, awSysType);
	}
	else
	{
		m_dl_cm_CRDataEndRspSnd_S3G(lwResResult, awSysType, dlw_prisys_tbl);
	}
	/* 全配下REからの応答受信済、かつOK応答有 */
	if( CMD_NML == lwResResult )
	{
		for(lwReNo = D_RRH_CPRINO_RE_MIN; lwReNo <= D_RRH_CPRINO_RE_MAX; lwReNo++)
		{
			gt_dlw_refw_all_tbl[awSysType][lwReNo-1].re_sta = DLD_RESTA_IDLE;
		}
		/* Change 対REC状態 => アイドル */
		(*gt_dlw_rec_all_tbl[awSysType]).rec_sta = EC_DLM_MAIN_STN_IDLE;
	}
	else
	{
		/* タイマ起動 システムパラメータ送信確認タイマ */
		lwTimerId = CMD_TIMID_SYSPRMSND_3G + awSysType;
		lwTimerVal = cmw_sys_mng_tbl[awSysType].sys_timer;
		/* タイマ起動 システムパラメータ情報報告確認タイマ */
		llResult = cm_TStat( 	lwTimerId,
								TIMVAL_GET(lwTimerVal),			/* 単位:10 msec */
								CMD_TIMMD_SINGLE,
								CMD_TSKIF_TIMOUTNTC, CMD_TSKID_DLM, &llErrcd);
		/*Start timer NG*/
		if(llResult != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_USELOW, lwTimerId, "システムパラメータ送信確認イマ Start NG" );
		}
		/* Change 対REC状態 => REC主導 ファイルデータ送信(シスパラ)待ち */
		(*gt_dlw_rec_all_tbl[awSysType]).rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_SYSPARA_WAIT;

		/* REC向けファイルデータ送信完了報告応答結果設定 0:正常 */
		(*gt_dlw_rec_all_tbl[awSysType]).filedata_trans_rslt = CMD_NML;
	}

	/* 受信済み分割番号初期化(=0) */
	(*gt_dlw_rec_all_tbl[awSysType]).rcv_div_no = CMD_NUM0;
	/* 受信済みデータサイズ初期化(=0) */
	(*gt_dlw_rec_all_tbl[awSysType]).rcv_data_size = CMD_NUM0;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_rl_fsndcmprsp_waiting_sysp_proc] RETURN" );

}
/* @} */

