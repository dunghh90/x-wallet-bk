/* sheet
 * 7-5 RE増設時ファイル更新
 * 7.5 update file when RE expand
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "m_rc_header.h"

/*-------7.5.1---------------------------------*/
/*--1		*/
/*ref: m_dl_idle_refw_dl_req.c		*/
/*--1.1		*/
/*ref: m_dl_fhm_idle_refw_dl_req.c		*/
/*--2		*/
/*ref: m_dl_rl_refw_dl_ng.c */
/*(B) block */
/*0x2110		*/
/*NOTE		*/
/*+ input msg not specify		*/
/*+ send new msg CPRI信号受信通知(ファームウェアファイル報告要求) to RE		*/
VOID m_dl_FWFileInfoRequestSnd(VOID* bufp, USHORT cpri_no)
{
	//input
	USHORT lw_CPRI_no = cpri_no;

	//new msg CPRI信号受信通知(ファームウェアファイル報告要求)
	CMT_TSKIF_CPRIRCV_FIRMFILEREQ lt_FW_file_report_req;
	
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileInfoRequestSnd] ENTER" );
	
	memset(&lt_FW_file_report_req, 0, sizeof(lt_FW_file_report_req));
	//REFW
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);

	/*更新中リンク情報の更新対象CPRI#ビットON
	*updating link info's update object CPRI# bit on
	*/
	/*(2)DLM*/
	set_updating_link_info(lw_CPRI_no, 1);

	/*実行中DL開始要因に2:REを設定
	*set starting DL start factor to 2
	*/
	set_executing_DL_start_factor(DLD_FACTOR_RE);

	/*(0x4001)RE Forwarding Notice(ファームウェアファイル報告要求)
	 * ファームウェアファイル報告要求(CMT_CPRIF_FIRMFILEREQ)
	 * 0x2111
	 * CMT_CPRIF_FIRMFILEREQ
	 * CPRI信号受信通知(ファームウェアファイル報告要求)
	 */
	lt_FW_file_report_req.head.uiSignalkind = CMD_CPRID_FIRMFILEREQ + CMD_NUM1;
	lt_FW_file_report_req.cprircv_inf.link_num = lw_CPRI_no;
	lt_FW_file_report_req.cpridat_firmfilereq.signal_kind = CMD_CPRID_FIRMFILEREQ + CMD_NUM1;
	RE_forwarding_notice(MSG_FW_FILE_REPORT_REQ, &lt_FW_file_report_req);

	/*ファームウェアファイル報告確認タイマ#n(※1)開始
	*firmware file report verify timer #n start
	*/
	m_dl_StartTimer(CMD_TIMID_FIRMFILE_REP, lw_CPRI_no);

	/* ファームウェアファイル報告応答結果を初期化 */
	gt_dlw_refw_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_IDLE;

	/*対RE状態#n(※1)をファームウェアファイル報告応答待ちへ遷移
	*change RE status to FW file report respond wait state
	*/
	set_RE_status(lw_REFW_index, DLD_RESTA_FWREP_WAIT);
	
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileInfoRequestSnd] RETURN" );
	
	
}

/*!
 *  @brief 
 *  @note   RE組み込み判定
 *  @param  bufp [in] Firmware File Report Response
 *  @param  cpri_no [in]  the CPRI no
 *  @param  save_idx [out]  the position RE File saved
 *  @param  result [out]  the result to REC thread
 *  @param  sndcmpntc_flg [out]  the flag FileDataSendCompleteNotice send or not
 *  @return UINT
 *  @retval Download Start or not (CMD_ON/CMD_OFF)
 *  @date   2015/11/04 TDIPS)sasaki Create IT3問処No.49対処
 */
USHORT m_dl_check_download_continue(VOID* bufp, USHORT cpri_no, USHORT *save_idx, USHORT *result, USHORT *sndcmpntc_flg)
{
	T_RRH_LAYER3	lt_layer3;
	USHORT			lw_rsp_result = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.result;
	USHORT			lw_rsp_maker = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.makername;
	USHORT			lw_rsp_mjr = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.mjr_ver;
	USHORT			lw_rsp_min = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.mir_ver;
	USHORT			lw_sav_idx = search_FHMRE_maker_id(lw_rsp_maker);
	USHORT			lw_loop;

	/* 当該メーカ識別のREファイル保存位置設定 */
	*save_idx = lw_sav_idx;
	
	/* 更新開始要因チェック */
	if(gt_dlw_refw_tbl[cpri_no-1].start_factor == DLD_UPDATE_FACTOR_RESTART_REC_UNCON)
	{
		/* RE起動(REC未接続)ならREファイル更新なし */
		*result = CMD_OK;
		*sndcmpntc_flg = (lw_rsp_result == CMD_FRM_NOFILE_S3G ? CMD_OFF : CMD_ON);
		return CMD_OFF;
	}

	/* 共用メモリLayer3の呼び出し 取得用 */
	f_cmn_com_layer3_get(CMD_SYS_S3G, &lt_layer3);

	/****************************************************************************/
	/* 同一メーカ同一バージョン運用中REが他にあればダウンロードなしで組み込む	*/
	/****************************************************************************/
	for(lw_loop = 0; lw_loop < D_RRH_CPRINO_RE_MAX; lw_loop++)
	{
		if(lw_loop == (cpri_no - 1))
		{
			continue;
		}
		/* L3状態が運用中の同一メーカREについて */
		if( lt_layer3.layer3_re[lw_loop] == DLD_L3_STA_4 && 
			gt_dlw_refw_tbl[lw_loop].mk_jdg == lw_rsp_maker &&
			gt_dlw_refw_tbl[lw_loop].usmjr_ver == lw_rsp_mjr &&
			gt_dlw_refw_tbl[lw_loop].usmin_ver == lw_rsp_min )
		{
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "chkDwl(RE#%02d,mk_jdg=%x,V%02d%02d)", cpri_no, lw_rsp_maker, lw_rsp_mjr, lw_rsp_min);
			/* ファイル更新なしで増設可 */
			*result = CMD_OK;
			*sndcmpntc_flg = CMD_ON;
			return CMD_OFF;
		}
	}



	/****************************************************************************
	 * 同一メーカ同一バージョン運用中REが他に存在しないならダウンロードが必要。
	 * 原則、当該メーカ識別のREファイルを保持していなければ強制停止。
	 *
	 * 例外として当該メーカ識別のREファイルを保持しておらず
	 * 前回のファームウェアファイル報告とバージョン情報が一致する場合に限り
	 * ダウンロードなしで組み込む。(以下の特殊ケースを想定した対処)
	 *
	 * 【特殊ケース】
	 * 装置立ち上げ時、REC保持ファイルとREのバージョンが一致していると
	 * RECからファイルダウンロードされず、FHMがファイルを持たない状態で
	 * REが組み込まれてしまう。その後、当該REとのリンク断/復旧が発生しても
	 * ダウンロードなしで配下に組み込み直す必要がある。
	 ****************************************************************************/

	/* 当該メーカ識別のREファイルを保持していない場合 */
	if (lw_sav_idx == 0xFFFF)
	{
		/* 前回のバージョン情報と一致する場合 */
		if( gt_dlw_refw_tbl[cpri_no-1].mk_jdg == lw_rsp_maker &&
			gt_dlw_refw_tbl[cpri_no-1].usmjr_ver == lw_rsp_mjr && 
			gt_dlw_refw_tbl[cpri_no-1].usmin_ver == lw_rsp_min )
		{
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "chkDwl(RE#%02d,mk_jdg=%x,V%02d%02d,indx=0x%x)", cpri_no, lw_rsp_maker, lw_rsp_mjr, lw_rsp_min, lw_sav_idx);
			/* ファイル更新なしで増設可 */
			*result = CMD_OK;
			*sndcmpntc_flg = CMD_ON;
			return CMD_OFF;
		}

		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "chkDwl(RE#%02d,mk_jdg=%x,V%02d%02d,indx=0x%x)", cpri_no, lw_rsp_maker, lw_rsp_mjr, lw_rsp_min, lw_sav_idx);
		*result = CMD_NG;
		*sndcmpntc_flg = CMD_ON;
		return CMD_OFF;
	}

	/****************************************************************************/
	/* FHM保持REファイルとバージョンが異なる運用中同一メーカREの有無をチェック	*/
	/****************************************************************************/
	for(lw_loop = 0; lw_loop < D_RRH_CPRINO_RE_MAX; lw_loop++)
	{
		if(lw_loop == (cpri_no - 1))
		{
			continue;
		}

		/* L3状態が運用中の同一メーカREについて */
		if(lt_layer3.layer3_re[lw_loop] == DLD_L3_STA_4 && gt_dlw_refw_tbl[lw_loop].mk_jdg == lw_rsp_maker)
		{
			/* 異なるバージョンの配下REが他にある場合 */
			if( gt_dlw_refw_tbl[lw_loop].usmjr_ver != gt_dlw_mnt_reinf[lw_sav_idx].mjr_ver ||
				gt_dlw_refw_tbl[lw_loop].usmin_ver != gt_dlw_mnt_reinf[lw_sav_idx].min_ver )
			{
				BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "chkDwl(RE#%02d,mk_jdg=%x,V%02d%02d,Indx=0x%x)", cpri_no, lw_rsp_maker, gt_dlw_refw_tbl[lw_loop].usmjr_ver, gt_dlw_refw_tbl[lw_loop].usmin_ver, lw_loop);
				BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "chkDwl(        mk_jdg=%x,V%02d%02d,Indx=0x%x)", lw_rsp_maker, gt_dlw_mnt_reinf[lw_sav_idx].mjr_ver, gt_dlw_mnt_reinf[lw_sav_idx].min_ver, lw_sav_idx);
				*result = CMD_NG;
				*sndcmpntc_flg = CMD_ON;
				return CMD_OFF;
			}
			break;
		}
	}

	/* 同一メーカRE接続有無設定 */
	gt_dlw_mnt_reinf[lw_sav_idx].re_conn_exist |= (CMD_ON << (cpri_no -1));

	/* FHM保持REファイルとバージョン一致 */
	if( gt_dlw_mnt_reinf[lw_sav_idx].mjr_ver == lw_rsp_mjr && 
		gt_dlw_mnt_reinf[lw_sav_idx].min_ver == lw_rsp_min )
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "chkDwl(RE#%02d,mk_jdg=%x,V%02d%02d,Indx=0x%x,EXIST=0x%x)", 
								cpri_no, lw_rsp_maker, lw_rsp_mjr, lw_rsp_min, lw_sav_idx, gt_dlw_mnt_reinf[lw_sav_idx].re_conn_exist);
		*result = CMD_OK;
		*sndcmpntc_flg = CMD_ON;
		return CMD_OFF;
	}

	*result = CMD_OK;
	*sndcmpntc_flg = CMD_OFF;
	return CMD_ON;
}
/*!
 *  @brief 
 *  @note   7-5-2 ファームウェアファイル報告応答受信
 *  @param  bufp [in] Firmware File Report Response
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/10/08 FPT)Duong update MKレビュー指摘No.217,218対応
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 *  @date   2015/11/04 TDIPS)sasaki Update IT3問処No.49対処
 *  @date	2016/04/21 TDI)satou 自律DL後に他ベンダの自律DLを行うと、FlashROMを読み込まず、最初の自律DLで送信したファイルを送ってしまう不具合の修正
 */
VOID m_dl_FWFileReportRspRcv(VOID* bufp, USHORT cpri_no)
{
	USHORT	lw_dl_cont;
	USHORT	lw_sav_idx;
	USHORT	lw_result;
	USHORT	lw_sndcmpntc_flg;
	USHORT	lw_FHMRE_index = get_DownLoading_RE_file_info();
	INT     ldw_api_ret = E_API_RCD_OK; 
	USHORT  lw_REFW_index = convert_CPRI_no_REFW_index(cpri_no);
	USHORT  lw_DownLoading_RE_file_read_status = 0;
	USHORT	lw_DLM_maker_id;
	USHORT	lw_FHMRE_maker_id = 0;
	USHORT	lw_DLM_fw_read_stat = get_DownLoading_RE_file_read_status();
	
	cm_Assert(CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileReportRspRcv] ENTER");

	/* ファームウェアファイル報告確認タイマ停止 */
	m_dl_StopTimer(CMD_TIMID_FIRMFILE_REP, cpri_no);

	/* RE組み込み判定 */
	lw_dl_cont = m_dl_check_download_continue(bufp, cpri_no, &lw_sav_idx, &lw_result, &lw_sndcmpntc_flg);

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "[TS]cpri_no=%d,mk_jdg=%x,lw_DLM_fw_read_stat=%X,lw_FHMRE_index=%X",
			cpri_no, ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.makername, lw_DLM_fw_read_stat, lw_FHMRE_index);
	BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "[TS]cpri_no=%d,mk_jdg=%x,lw_DLM_fw_read_stat=%X,lw_FHMRE_index=%X",
			cpri_no, ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.makername, lw_DLM_fw_read_stat, lw_FHMRE_index);

	/* 組み込み可 */
	if(lw_result == CMD_OK)
	{
		/* 対RE-FW情報テーブルを応答内容で更新 */
		gt_dlw_refw_tbl[cpri_no-1].link_num = cpri_no;
		gt_dlw_refw_tbl[cpri_no-1].mk_jdg = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.makername;
		gt_dlw_refw_tbl[cpri_no-1].usmjr_ver = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.mjr_ver;
		gt_dlw_refw_tbl[cpri_no-1].usmin_ver = ((CMT_TSKIF_CPRIRCV_FIRMFILERES*)bufp)->cpridat_firmfileres.firmfile.mir_ver;
		gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_IDLE;
		if(lw_sav_idx != 0xFFFF)
		{
			gt_dlw_refw_tbl[cpri_no-1].fhmrefile_inf = lw_sav_idx;
			gt_dlw_mnt_reinf[lw_sav_idx].re_conn_exist |= (USHORT)(CMD_ON << (cpri_no-1));
		}
	}
	/* 組み込み不可 */
	else
	{
		if(lw_sav_idx != 0xFFFF)
		{
			gt_dlw_refw_tbl[cpri_no-1].fhmrefile_inf = 0xFFFF;
			gt_dlw_mnt_reinf[lw_sav_idx].re_conn_exist &= (USHORT)~(CMD_ON << (cpri_no-1));
		}
	}

	/* ファイル更新なし */
	if(lw_dl_cont == CMD_OFF)
	{
		m_dl_REFileNotUpdate(0, cpri_no, lw_result, lw_sndcmpntc_flg);
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end [cpri=%d][result=%d][sndcmpntc=%d] \n", __LINE__, __FUNCTION__, cpri_no, lw_result, lw_sndcmpntc_flg);
		#endif
	}
	else
	{
		/* 増設可											    */
		/*     ->REファイル読込未実施の場合はREファイル読込開始 */
		/*     ->REファイル読込済の場合はファイル情報報告通知   */
		lw_DLM_maker_id = get_REFW_maker_id((cpri_no - 1));
		/* DL中ファイルあり
		 * if DL file is existing
		 */
		if (lw_FHMRE_index != 0xFFFF)
		{
			/* DL中メーカ識別子!=[*1]
			 * check maker id
			 */
			lw_FHMRE_maker_id = get_FHMRE_maker_id(lw_FHMRE_index);
			if (lw_DLM_maker_id != lw_FHMRE_maker_id)
			{
				/*DL中REファイル読込状態を0:未実行に設定
				 *set DL RE file read status to 0 - NOT_EXECUTE
				 */
				set_DownLoading_RE_file_read_status(DLD_READ_NONE);
			}
			/*DL中REファイル読込状態が読込中止中
			 *if (DL RE file read status is READING_STOP)
			 */
			else if(lw_DLM_fw_read_stat == DLD_READ_STOP)
			{
	#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
				BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "firmWarefileRsp RCV(RE#%02d,mk_jdg=(%x=%x),Indx=0x%x,sts=%d)", 
										cpri_no, lw_DLM_maker_id, lw_FHMRE_maker_id, lw_FHMRE_index, lw_DLM_fw_read_stat);
				return;
			}
		}
		else
		{
			/*
			 * DL中REファイル読込状態を0:未実行に設定
			 * set DL RE file read status to 0 - NOT_EXECUTE
			 */
			set_DownLoading_RE_file_read_status(DLD_READ_NONE);
		}

		/*
		 * FHM保持REファイル情報#nをDL中ファイル情報に設定
		 * set DLM mng tbl's DLing file info = FHM tbl index #n
		 */
		lw_FHMRE_index = search_FHMRE_maker_id(lw_DLM_maker_id);
		set_DownLoading_RE_file_info(lw_FHMRE_index);
		
		/*	REファイル読出し状態を取得する	*/
		lw_DownLoading_RE_file_read_status = get_DownLoading_RE_file_read_status();
		
		switch(lw_DownLoading_RE_file_read_status)
		{
			case DLD_READ_NONE:     /* 0:未実行 */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"DownLoad Start[flash read] for fhmself(lnk=%d, mkName=%x, mjVer=%x, mnVer=%x)", 
				gt_dlw_refw_tbl[cpri_no-1].link_num, 
				gt_dlw_refw_tbl[cpri_no-1].mk_jdg, 
				gt_dlw_refw_tbl[cpri_no-1].usmjr_ver, 
				gt_dlw_refw_tbl[cpri_no-1].usmin_ver );
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO,"DownLoad Start[flash read] for fhmself(lnk=%d, mkName=%x, mjVer=%x, mnVer=%x)", 
				gt_dlw_refw_tbl[cpri_no-1].link_num, 
				gt_dlw_refw_tbl[cpri_no-1].mk_jdg, 
				gt_dlw_refw_tbl[cpri_no-1].usmjr_ver, 
				gt_dlw_refw_tbl[cpri_no-1].usmin_ver );
			
				/* REファイルデータ読込API発行 */
				ldw_api_ret = rrhApi_File_Mnr_LoadReFileData(D_RRH_PROCQUE_L3, lw_FHMRE_index);
				if(ldw_api_ret != E_API_RCD_OK)
				{
					cm_Assert(CMD_ASL_DBGHIGH, lw_FHMRE_index, "rrhApi_File_Mnr_LoadReFileData error");
					return;
				}
				/* DL中REファイル読込状態を1:読込中に設定 */
				set_DownLoading_RE_file_read_status(DLD_READ_ING);
				/* 対RE状態#n(※1)をREファイルデータ読込中へ遷移 */
				set_RE_status(lw_REFW_index, DLD_RESTA_RE_FILEREAD);
				break;
			case DLD_READ_ING:      /* 1:読込中 */
				/* 対RE状態#n(※1)をREファイルデータ読込中へ遷移 */
				set_RE_status(lw_REFW_index, DLD_RESTA_RE_FILEREAD);
				break;
			case DLD_READ_DONE:     /* 2:読込済 *reading complete */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"DownLoad Start[no flash read] for fhmself(lnk=%d, mkName=%x, mjVer=%x, mnVer=%x)", 
				gt_dlw_refw_tbl[cpri_no-1].link_num, 
				gt_dlw_refw_tbl[cpri_no-1].mk_jdg, 
				gt_dlw_refw_tbl[cpri_no-1].usmjr_ver, 
				gt_dlw_refw_tbl[cpri_no-1].usmin_ver );
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO,"DownLoad Start[no flash read] for fhmself(lnk=%d, mkName=%x, mjVer=%x, mnVer=%x)", 
				gt_dlw_refw_tbl[cpri_no-1].link_num, 
				gt_dlw_refw_tbl[cpri_no-1].mk_jdg, 
				gt_dlw_refw_tbl[cpri_no-1].usmjr_ver, 
				gt_dlw_refw_tbl[cpri_no-1].usmin_ver );
				/*	ファイル情報報告通知	*/
				m_dl_FileInfoReportNotice(cpri_no);
				break;
		}
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end \n", __LINE__, __FUNCTION__);
		#endif
	}
	return;
}
/*-------7.5.5---------------------------------*/
/*!
 *  @brief  the processing for "ファイル情報報告通知作成"
 *  @note   
 *  @param  aw_CPRI_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/10/09 TDIPS)sasaki Update MKレビュー指摘No.265,266,267,268対応
 *  @date   2015/10/12 TDIPS)sasaki Update MKレビュー指摘No.279対応
 */
/*ref:m_dl_rec_idle_finf_ntc.c		*/
/* 7-5-5 ファイル情報報告通知(LTE)
 * 7.5.5 file info report notice
 */
VOID m_dl_FileInfoReportNotice(USHORT aw_CPRI_no)
{
	CMT_TSKIF_CPRIRCV_FILEINFREQ	lt_file_info_report_notice;
	USHORT							lw_FHMRE_index;

	cm_Assert( CMD_ASL_NOLEVEL, aw_CPRI_no,"[m_dl_FileInfoReportNotice] ENTER" );


	lw_FHMRE_index = get_DownLoading_RE_file_info();

	/*DL中ファイル情報からファイル情報報告通知作成
	 *create file info report notice from downloading file info
	 */
	memset(&lt_file_info_report_notice, 0, sizeof(lt_file_info_report_notice));

	lt_file_info_report_notice.cprircv_inf.link_num = aw_CPRI_no;
	lt_file_info_report_notice.head.uiSignalkind 			  = CMD_CPRID_FILEINFREQ + CMD_NUM1;
	lt_file_info_report_notice.cpridat_fileinfreq.signal_kind = CMD_CPRID_FILEINFREQ + CMD_NUM1;
	memcpy(lt_file_info_report_notice.cpridat_fileinfreq.filename,
		get_FHMRE_file_name(lw_FHMRE_index),
		sizeof(lt_file_info_report_notice.cpridat_fileinfreq.filename));
	lt_file_info_report_notice.cpridat_fileinfreq.data = CMD_FRM;
	lt_file_info_report_notice.cpridat_fileinfreq.size = get_FHMRE_file_size(lw_FHMRE_index);

	/*(0x4001)RE Forwarding Notice(ファイル情報報告通知)
	 *file info report notice - FilInfRepNtc
	 */
	RE_forwarding_notice(MSG_FILE_INFO_REPORT_NOTICE, &lt_file_info_report_notice);

	/*ファームウェアファイル情報報告確認タイマ#n(※1)開始
	 *start FW file info report verify timer
	 */
	m_dl_StartTimer(CMD_TIMID_FIRMFILE_INF, aw_CPRI_no);

	/* ファイル情報報告応答結果を初期化 */
	gt_dlw_refw_tbl[aw_CPRI_no-1].fileinf_rep_rslt = DLD_RESLT_IDLE;

	/*対RE状態#n(※1)をファイル情報報告応答(FW)待ちへ遷移
	 *change RE status to FILE INFO REPORT RESPOND WAIT
	 */
	set_RE_status((aw_CPRI_no - 1), DLD_RESTA_FILEINF_WAIT);
	cm_Assert( CMD_ASL_NOLEVEL, aw_CPRI_no,"[m_dl_FileInfoReportNotice] RETURN" );
}

/*-------7.5.6---------------------------------*/
/* 7-5-6 ファイル情報報告応答受信
 * 7.5.6 file info report respond recv
 * (LTE)(0x2211)ファイル情報報告応答 - CMD_CPRID_FILEINFRES 0x2220
 * file info report respond
 * (0x4003)RE Forwarding Notice(ファイル情報報告応答)
 * file info report respond
 */
/*!
 *  @brief  the processing for "ファイル情報報告応答受信"
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *  @param  bufp [in]  the buffer
 *  @return Void.
 *  @retval -
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.172対応
 *  @date   2015/10/12 TDIPS)sasaki Update MKレビュー指摘No.283対応
 */
USHORT m_dl_REFileInfReportRspRcv(VOID* bufp, USHORT cpri_no)
{
	USHORT  lw_CPRI_no = cpri_no;
	USHORT  lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);
	USHORT  lw_DownLoading_RE_file_read_status = 0;
	USHORT  lw_ret = CMD_OK; 

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_REFileInfReportRspRcv] ENTER" );

	/*ファイル情報報告確認タイマ#n(※1)停止
	 *FW file info report verify timer stop
	 */
	m_dl_StopTimer(CMD_TIMID_FIRMFILE_INF, lw_CPRI_no);

	/*DL中REファイル読込状態チェック
	 *check DL RE file read status
	 */
	lw_DownLoading_RE_file_read_status = get_DownLoading_RE_file_read_status();
	switch(lw_DownLoading_RE_file_read_status)
	{
		case DLD_READ_DONE:     /* 2:読込済 *reading complete */
			/* 7-5-8 ファイルデータ送信(LTE) */
			m_dl_SendFileDataStart(lw_CPRI_no);
			m_dl_SendFileData(lw_CPRI_no);

			/* 対RE状態#n(※1)をファイルデータ送信中(FW)へ遷移 */
			set_RE_status(lw_REFW_index, DLD_RESTA_FILE_TRANS);
	        break;
	    default:
			cm_Assert(CMD_ASL_DBGHIGH, lw_CPRI_no, "lw_DownLoading_RE_file_read_status error");
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "lw_DownLoading_RE_file_read_status error(RE#%02d, sts=%d)", lw_CPRI_no, lw_DownLoading_RE_file_read_status);
	    	break;
	}

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_REFileInfReportRspRcv] RETURN" );
	return lw_ret;
}
/*!
 *  @brief  the processing for "ファイルデータ送信" receiving at the ファイル情報報告応答(FW)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/09/21 FPT)Yen Update comment
 */

VOID m_dl_rec_finfrsp_waiting_fwsnd(VOID* bufp, USHORT cpri_no)
{
	CHAR errdata[CMD_NUM48];
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_finfrsp_waiting_fwsnd] ENTER" );

	memset( errdata, 0, sizeof(errdata));
	snprintf(errdata, CMD_NUM48, "[%s][cpri_no:%d]", __FUNCTION__, cpri_no );
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0, errdata );

	// TODO
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_rec_finfrsp_waiting_fwsnd] RETURN" );

}

/*!
 *  @brief  the processing for "ファイルデータ送信転送指示作成"
 *  @note   
 *  @param  at_file_data_send_p		[out] receive message pointer
 *  @param  al_part					[in]  division number
 *  @param  al_size					[in]  data size
 *  @param  al_offset				[in]  RE_FILE data offset
 *  @return ULONG
 *  @retval size which set into at_file_data_send_p
 *  @date   2015/10/06 TDIPS)sasaki Update MKレビュー指摘No.255対応
 */

ULONG fill_msg_with_data(CMT_CPRIF_FILEDATSNDINT* at_file_data_send_p,
						ULONG al_part,
						ULONG al_size,
						ULONG al_offset)
{
	union
	{
		ULONG  value;
		VOID*  addr_p;
		struct {
			USHORT low;
			USHORT high;
		};
	} data_size, data_part;

	al_size = (al_size < CMD_FILEDAT_MAX) ? al_size : CMD_FILEDAT_MAX;
	data_size.value = al_size;
	data_part.value = al_part;


	at_file_data_send_p->sgmtno1 = data_part.high;
	at_file_data_send_p->sgmtno2 = data_part.low;

	at_file_data_send_p->datsize1 = data_size.high;
	at_file_data_send_p->datsize2 = data_size.low;

	at_file_data_send_p->offset = al_offset;
	
	return al_size;
}
/*--		*/
/* 7-5-8 ファイルデータ送信(LTE)
 * 7.5.8 File data send
 */
/*!
 *  @brief  the processing for "ファイルデータ送信開始処理"
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/10/09 TDIPS)sasaki Update MKレビュー指摘No.271対応
 */
/*NOTE		*/
/*+ input msg not spedify		*/
/*+ send msg ファイルデータ送信完了報告通知 to RE		*/
/*--send data 0/n		*/
VOID m_dl_SendFileDataStart(USHORT cpri_no)
{
	//input
	USHORT lw_CPRI_no = cpri_no;

	//REFW ファイルデータ送信完了報告通知
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_SendFileDataStart] ENTER" );
	
	/*ファイルデータ送信間隔タイマ#n(※1)開始
	 *start file data send interval timer
	 */
	m_dl_StartTimer(CMD_TIMID_FILEDATSND, lw_CPRI_no);

	/*対RE状態#n(※1)をファイルデータ送信中(FW)へ遷移
	 *change RE status to file data sending
	 */
	set_RE_status(lw_REFW_index, DLD_RESTA_FILE_TRANS);
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_SendFileDataStart] RETURN" );
}
/*--		*/
/* 7-5-8 ファイルデータ送信(LTE)
 * 7.5.8 File data send
 */
/*!
 *  @brief  the processing for "ファイルデータ送信転送指示"
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/10/26 TDIPS)sasaki Update IT2問処No.141対処
 */
/*--send data i/n		*/
USHORT m_dl_SendFileData(USHORT cpri_no)
{
	//input
	USHORT lw_CPRI_no = cpri_no;

	//REFW ファイルデータ送信完了報告通知
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);
	USHORT lw_maker_id = 0;
	ULONG ll_REFW_send_part = 0;
	ULONG ll_REFW_send_size = 0;

	//FHMRE
	USHORT lw_FHMRE_index = 0;
	ULONG ll_FHMRE_data_part = 0;
	ULONG ll_FHMRE_file_size = 0;

	/* CPRI信号受信通知(ファイルデータ送信転送指示) */
	CMT_TSKIF_CPRIRCV_FILEDATSNDINT 	lt_file_data_send;

	ULONG ll_msg_data_size = 0;
	USHORT lw_is_last_part = 0;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_SendFileData] ENTER" );
	/*ファイルデータ送信転送指示(1/n)
	 *file data send transfer instruction
	 */
	//--get already sent data's part & size
	lw_maker_id = get_REFW_maker_id(lw_REFW_index);
	ll_REFW_send_part = get_transfer_complete_part_no(lw_REFW_index);
	ll_REFW_send_size = get_transfer_complete_data_size(lw_REFW_index);

	//--locate data in FHMRE
	lw_FHMRE_index = gt_dlw_dlm_tbl.dlrefile_inf;

	ll_FHMRE_data_part = get_number_part(lw_FHMRE_index);
	ll_FHMRE_file_size = get_FHMRE_file_size(lw_FHMRE_index);

	//--prepare msg
	memset(&lt_file_data_send, 0, sizeof(lt_file_data_send));
	lt_file_data_send.head.uiEventNo = CMD_TSKIF_FILEDATSND_TRANS_IND;
	lt_file_data_send.cpridat_filedatsndint.signal_kind = CMD_CPRID_FILEDATSND + CMD_SYS_S3G;
	lt_file_data_send.cprircv_inf.link_num = lw_CPRI_no;
	ll_msg_data_size = fill_msg_with_data(
		&lt_file_data_send.cpridat_filedatsndint,
		ll_REFW_send_part + 1,
		ll_FHMRE_file_size - ll_REFW_send_size,
		ll_REFW_send_size
		);
	//--send msg
	RE_forwarding_notice(MSG_FILE_DATA_SEND, &lt_file_data_send);

	//update REFW
	ll_REFW_send_part++;
	ll_REFW_send_size += ll_msg_data_size;
	set_transfer_complete_part_no(lw_REFW_index, ll_REFW_send_part);
	set_transfer_complete_data_size(lw_REFW_index, ll_REFW_send_size);

	//check is last part has already sent
	lw_is_last_part = (ll_REFW_send_part == ll_FHMRE_data_part)?1:0;
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_SendFileData] RETURN" );
	return lw_is_last_part;
}

/*!
 *  @brief  the processing for "ファイルデータ送信完了報告通知"
 *  @note   
 *  @param  cpri_no [in]  the cpri_no no
 *  @return Void.
 *  @retval -
 *  @date   2015/10/26 TDIPS)sasaki Update IT2問処No.144対処
 */
VOID m_dl_SendFileDataEnd(USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILSNDFINNTC	lt_msg;
	USHORT							lw_chk_sum = 0;
	USHORT							lw_FHMRE_index = search_FHMRE_maker_id(gt_dlw_refw_tbl[cpri_no-1].mk_jdg);
	ULONG							ll_data_size = gt_dlw_mnt_reinf[lw_FHMRE_index].file_size;
	UINT							a_rtn = CMD_RES_OK;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_SendFileDataEnd] ENTER" );

	/* save the handle of shared memory */
	a_rtn = rrhApi_File_Fsm_loadMemory((INT *)&dlw_handle);
	if(a_rtn != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Fsm_loadMemory return NG");
		return;
	}

	/* チェックサム算出 */
	cm_CheckSumCalcCpri(((UCHAR *)dlw_handle + sizeof(INT) * 8), ll_data_size, &lw_chk_sum);

	/* Unload shared memory */
	(VOID)rrhApi_File_Fsm_unLoadMemory(dlw_handle, ll_data_size, lw_chk_sum);

	/* ファイルデータ送信完了報告通知作成 */
	memset(&lt_msg, 0, sizeof(lt_msg));
	lt_msg.head.uiSignalkind = CMD_CPRID_FILSNDFINNTC;
	lt_msg.cprircv_inf.link_num = cpri_no;
	lt_msg.cpridat_filsndfinntc.signal_kind = CMD_CPRID_FILSNDFINNTC + CMD_NUM1;
	lt_msg.cpridat_filsndfinntc.tgtdat = CMD_FRM;
	lt_msg.cpridat_filsndfinntc.chksum = lw_chk_sum;

	/* ファイルデータ送信完了報告通知送信 */
	RE_forwarding_notice(MSG_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE, &lt_msg);

	/*ファイルデータ送信間隔タイマ#n(※1)停止 */
	m_dl_StopTimer(CMD_TIMID_FILEDATSND, cpri_no);
	/*ファームウェア送信完了確認タイマ#n(※1)開始 */
	m_dl_StartTimer(CMD_TIMID_FIRMFILE_SNDFIN, cpri_no);
	/* ファイルデータ送信完了報告応答結果を初期化 */
	gt_dlw_refw_tbl[cpri_no-1].filedata_trans_rslt = DLD_RESLT_IDLE;

	/*対RE状態#n(※1)をファイルデータ送信完了報告応答(FW)待ちへ遷移 */
	set_RE_status(cpri_no-1, DLD_RESTA_FILE_COMP_WAIT);
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_SendFileDataEnd] RETURN" );
}
/*-------7.5.9---------------------------------*/
/*--1		*/
/*ref: m_dl_fl_fsndcmp_rsp.c		*/
/*--1.1		*/
/*ref: m_dl_fhm_fsndcmprsp_waiting_fsndcmp_rsp.c		*/
/*--1.2		*/

/* 7-5-9 ファイルデータ送信完了報告応答受信(LTE)
 * 7.5.9 file data send complete report respond receive
 * (LTE)(0x2321)ファイルデータ送信完了報告応答
 * file data send complete report respond
 * (0x4003)RE Forwarding Notice(ファイルデータ送信完了報告応答)
 */
/*!
 *  @brief 
 *  @note   7-5-9 ファイルデータ送信完了報告応答受信(LTE)
 *  @param  bufp [in]
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/10/08 FPT)Duong update MKレビュー指摘No.256対応
 */
VOID m_dl_REFileDataSndCompleteReportRspRcv(VOID* bufp, USHORT cpri_no)
{
	//input
	//ファイルデータ送信完了報告応答(CMT_CPRIF_FILSNDFINRES)
	//0x2320
	//CPRI信号受信通知(ファイルデータ送信完了報告応答)
	CMT_TSKIF_CPRISND_FILSNDFINRES* lt_file_data_send_complete_report_respond_p = 
		(CMT_TSKIF_CPRISND_FILSNDFINRES*)bufp;
	USHORT lw_CPRI_no = cpri_no;
	USHORT lw_result = lt_file_data_send_complete_report_respond_p->cpridat_filsndfinres.result;

	USHORT lw_major_version = gt_dlw_mnt_reinf[gt_dlw_dlm_tbl.dlrefile_inf].mjr_ver;
	USHORT lw_minor_version = gt_dlw_mnt_reinf[gt_dlw_dlm_tbl.dlrefile_inf].min_ver;

	/* REFW */
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);

	if (CMD_NML == lw_result)
	{
		//msg REファイル更新応答(CMT_TSKIF_REFILNEWRES)
		CMT_TSKIF_REFILNEWRES lt_RE_file_update_respond;
		cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_REFileDataSndCompleteReportRspRcv] ENTER" );
		memset(&lt_RE_file_update_respond, 0, sizeof(lt_RE_file_update_respond));


		/*ファームウェア送信完了確認タイマ#n(※1)停止
		 *FW send complete verify timer stop
		 */
		 m_dl_StopTimer(CMD_TIMID_FIRMFILE_SNDFIN, lw_CPRI_no);

		/*対RE状態#n(※1)をアイドルへ遷移
		 *change RE status to idle
		 */
		set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);

		/*当該対RE-FW情報テーブルの起動用バージョン#nを更新
		 * update REFW info table's version
		 */
		set_REFW_major_version_startup(lw_REFW_index, lw_major_version);
		set_REFW_minor_version_startup(lw_REFW_index, lw_minor_version);

		/*更新開始要因#nを初期化
		 *init update start factor
		 */
		set_update_start_factor(lw_REFW_index, DLD_UPDATE_FACTOR_NONE);

		/*更新中リンク情報の更新対象CPRI#ビットOFF
		 *set updateing link info bit to OFF
		 */
		set_updating_link_info(lw_CPRI_no, 0);

		/*REファイル更新応答(OK,更新対象CPRI#,更新あり)
		 *RE file update respond
		 */
		lt_RE_file_update_respond.head.uiEventNo = CMD_TSKIF_REFIRM_DL_RSP;
		lt_RE_file_update_respond.cpri_no = lw_CPRI_no;
		lt_RE_file_update_respond.result_code = lw_result;
		lt_RE_file_update_respond.reset_jdg = CMD_ON;
		lt_RE_file_update_respond.dummy = lt_file_data_send_complete_report_respond_p->cpridat_filsndfinres.signal_kind & CMD_SYS_CHK_CPRID;
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"DownLoad End for fhmself(lnk=%d, ret=%x, system=%d)", lw_CPRI_no, lw_result, lt_RE_file_update_respond.dummy );
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO,"DownLoad End for fhmself(lnk=%d, ret=%x, system=%d)", lw_CPRI_no, lw_result, lt_RE_file_update_respond.dummy );
		send_msg_to_RE(MSG_RE_FILE_UPDATE_RESPOND, &lt_RE_file_update_respond);

		//NOTE
		//A()>>B(): send msg: 0x2110 CPRI信号受信通知(ファームウェアファイル報告要求) to RE
		m_dl_ResumeWaitRE();
	}
	else
	{	/* NG応答の場合 */
		m_dl_FileDataSendCopmleteReportRespondNG(bufp, cpri_no);
	}

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_REFileDataSndCompleteReportRspRcv] RETURN" );

	return;
}

/*A function		*/
/*NOTE		*/
/*+ call B to send msg: 0x2110 CPRI信号受信通知(ファームウェアファイル報告要求) to RE		*/
VOID m_dl_ResumeWaitRE()
{
//	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_ResumeWaitRE] ENTER" );
	/*
	*全ての対RE状態チェック
	*check all RE status
	*/
	//TBD
	if (all_RE_idle())
	{
		/*DL中ファイル情報初期化
		*init DL file info
		*/
		set_DownLoading_RE_file_info(0xFFFF);
	}
//	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_ResumeWaitRE] RETURN" );
}
/*-------7.5.10--------------------------------*/
/*!
 *  @brief  the processing for "ファイルデータ送信完了報告通知(更新なし)送信" and "REファイル更新応答送信" 
 *  @note   input msg: not specify
 *			call A()>>B() send msg 0x2110 CPRI信号受信通知(ファームウェアファイル報告要求) to RE
 *  @param  bufp [in]  the buffer
 *  @param  cpri_no [in]  the CPRI no
 *  @param  aw_res [in] result code
 *  @param  sndcmpntc_flg [in]  ファイルデータ送信完了報告通知送信要否
 *  @return Void.
 *  @Bug_No M-RRU-ZSYS-01780
 *  @date   2015/10/13 TDIPS)sasaki Update MKレビュー指摘No.287対応
 *  @date   2015/10/19 TDIPS)sasaki M-RRU-ZSYS-01780 Update IT2問処No.85対処
 */
VOID m_dl_REFileNotUpdate(VOID* bufp, USHORT cpri_no, USHORT aw_res, USHORT sndcmpntc_flg)
{
	CMT_TSKIF_CPRIRCV_FILSNDFINNTC	lt_file_data_send_complete_notice;
	CMT_TSKIF_REFILNEWRES			lt_RE_file_update_respond;
	USHORT							lw_CPRI_no = cpri_no;
	USHORT							lw_result = aw_res;
	USHORT							lw_updating_link_exist = 0;
	USHORT							lw_RE_status = 0;
	USHORT							lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_REFileNotUpdate] ENTER" );

	if(sndcmpntc_flg == CMD_ON)
	{
		/*(0x4001)RE Forwarding Notice(ファイルデータ送信完了報告通知(更新なし))
		*file data send complete report notice
		* (LTE)(0x2311)ファイルデータ送信完了報告通知(更新なし)
		* file data send complete notice
		* CMT_CPRIF_FILSNDFINNTC	0x2310
		*/
		memset(&lt_file_data_send_complete_notice, 0, sizeof(lt_file_data_send_complete_notice));
		lt_file_data_send_complete_notice.cprircv_inf.link_num = lw_CPRI_no;
		lt_file_data_send_complete_notice.cpridat_filsndfinntc.signal_kind = CMD_CPRID_FILSNDFINNTC + CMD_NUM1;
		lt_file_data_send_complete_notice.head.uiSignalkind = CMD_CPRID_FILSNDFINNTC + CMD_NUM1;
		lt_file_data_send_complete_notice.cpridat_filsndfinntc.tgtdat = CMD_FRM_NONEW;
		RE_forwarding_notice(MSG_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE, &lt_file_data_send_complete_notice);
	}

	/*更新開始要因#nを初期化
	 *init update start factor
	 */
	set_update_start_factor(lw_REFW_index, DLD_UPDATE_FACTOR_NONE);

	/*更新中リンク情報の更新対象CPRI#ビットOFF
	 *updating link info's update object CPRI bit OFF
	 */
	set_updating_link_info(lw_CPRI_no, 0);

	/*更新中リンクなし
	 *updating link not exist
	 */
	lw_updating_link_exist = is_updating_link_exist();
	if (lw_updating_link_exist == 0)
	{
		/*実行中DL開始要因に0:未実行を設定
		 *set executing DL start factor to 0
		 */
		set_executing_DL_start_factor(DLD_FACTOR_NONE);
	}

	/*REファイル更新応答(結果(OK or NG),更新なし)
	 *RE file update respond
	 */
	memset(&lt_RE_file_update_respond, 0, sizeof(lt_RE_file_update_respond));
	lt_RE_file_update_respond.head.uiEventNo = CMD_TSKIF_REFIRM_DL_RSP;
	lt_RE_file_update_respond.cpri_no = lw_CPRI_no;
	lt_RE_file_update_respond.result_code = lw_result;
	lt_RE_file_update_respond.reset_jdg = CMD_OFF;
	lt_RE_file_update_respond.dummy = CMD_SYS_S3G;
	RE_forwarding_notice(MSG_RE_FILE_UPDATE_RESPOND, &lt_RE_file_update_respond);

	/*対RE状態#nがアイドル以外
	 *RE status not IDLE
	 */
	 lw_RE_status = get_RE_status(lw_REFW_index);
	if (lw_RE_status != DLD_RESTA_IDLE)
	{
		/*対RE状態#n(※1)をアイドルへ遷移
		 *change to IDLE
		 */
		set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);
	}

	/*do (A)*/
	//NOTE
	//A()>>B(): send msg: 0x2110 CPRI信号受信通知(ファームウェアファイル報告要求) to RE
	m_dl_ResumeWaitRE();
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_REFileNotUpdate] RETURN" );
}

/* 7-5-14 ファームウェアファイル報告応答NG
 * 7.5.14 FW file report respond
 */
/*NOTE		*/
/*input msg not specify		*/
/*+ call 7.5.10:		*/
/*  send msg ファイルデータ送信完了報告通知(更新なし) to RE		*/
/*  send REファイル更新応答(結果(OK or NG),更新なし) to RE		*/
/*+ send msg ファームウェアファイル報告要求 0x2110 to RE		*/
/*!
 *  @brief 
 *  @note   7-5-14 ファームウェアファイル報告応答NG
 *  @param  bufp [in]
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/10/08 FPT)Duong update MKレビュー指摘No.215,216対応
 */
VOID m_dl_FWFileReportRespondNG(VOID* bufp, USHORT cpri_no)
{
	//input
	USHORT lw_CPRI_no = cpri_no;

	//REFW
	USHORT lw_resend_times = 0;
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);

	//new msg 0x2110 send to RE
	//CMD_CPRID_FIRMFILEREQ	0x2110
	//CPRI信号送信通知(ファームウェアファイル報告要求)
	CMT_TSKIF_CPRISND_FIRMFILEREQ lt_FW_file_report_request;
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileReportRespondNG] ENTER" );
	memset(&lt_FW_file_report_request, 0, sizeof(lt_FW_file_report_request));

	/*ファームウェアファイル報告確認タイマ#n(※1)停止
	 *stop FW file report verify timer
	 */
	m_dl_StopTimer(CMD_TIMID_FIRMFILE_REP, lw_CPRI_no);

	/*再送回数＜再送回数最大値
	 *resend times < max resend times
	 */
	lw_resend_times = get_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ);
	if (lw_resend_times < DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ_MAX)
	{
		/*再送回数[ファームウェアファイル報告要求]#nを加算
		 *inc counter: FW file report request
		 */
		set_resend_counter(lw_REFW_index,
			DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ,
			lw_resend_times + 1);

		/*ファームウェアファイル報告確認タイマ#n(※1)開始
		 *start timer: FW file report verify
		 */
		m_dl_StartTimer(CMD_TIMID_FIRMFILE_REP, lw_CPRI_no);

		/*(0x4001)RE Forwarding Notice(ファームウェアファイル報告要求)
		 *FW file report request
		 *NOTE
		 *  after that: REC send (LTE)(0x2111)ファームウェアファイル報告要求 to RE
		 */
		lt_FW_file_report_request.head.uiSignalkind = CMD_CPRID_FIRMFILEREQ + CMD_NUM1;
		lt_FW_file_report_request.cprisnd_inf.link_num = lw_CPRI_no;
		lt_FW_file_report_request.cpridat_firmfilereq.signal_kind = CMD_CPRID_FIRMFILEREQ + CMD_NUM1;
		RE_forwarding_notice(MSG_FW_FILE_REPORT_REQ, &lt_FW_file_report_request);

		/* ファームウェアファイル報告応答結果を初期化 */
		gt_dlw_refw_tbl[cpri_no-1].fw_rep_rslt = DLD_RESLT_IDLE;

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/*当該対RE-FW情報テーブルのファームウェアファイル報告結果にNGを設定
	*set NG: RE FW info table's FW file report result
	*/
	set_report_result(lw_REFW_index, REPORT_RESULT_FW_FILE, REPORT_RESULT_NG);

	/*当該対RE-FW情報テーブルの使用中タイマを初期化
	*init timer: RE FW info table's using timer
	*/
	set_REFW_using_timer(lw_REFW_index, DLD_RERETIM_NONE);

	/*再送回数[ファームウェアファイル報告要求]#n初期化
	*init counter: FW file report request
	*/
	set_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FW_FILE_REPORT_REQ, 0);

	/*更新開始要因#nを初期化
	*init update start factor
	*/
	set_update_start_factor(lw_REFW_index, DLD_UPDATE_FACTOR_NONE);

	/*更新中リンク情報のNG応答CPRI#ビットOFF
	*updating link info's NG respond
	*/
	set_updating_link_info(lw_CPRI_no, 0);

	/*7-5-10 REファイル更新なし(パラメータ:NG)
	*do 7.5.10 RE file update no
	*/
	m_dl_REFileNotUpdate(0, lw_CPRI_no, CMD_NG, CMD_OFF);
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FWFileReportRespondNG] RETURN" );
}

/* 7-5-15 ファイル情報報告応答NG
 * 7.5.15 file info report respond NG
 */
/*NOTE		*/
/*input msg not specify		*/
/*+ call 7.5.10:		*/
/*  send msg ファイルデータ送信完了報告通知(更新なし) to RE		*/
/*  send REファイル更新応答(結果(OK or NG),更新なし) to RE		*/
VOID m_dl_FileInfoReportRespondNG(VOID* bufp, USHORT cpri_no)
{
	//input
	USHORT lw_CPRI_no = cpri_no;

	//REFW
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);
	USHORT lw_resend_times = 0;
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FileInfoReportRespondNG] ENTER" );

	/*ファームウェアファイル情報報告確認タイマ#n(※1)停止
	 *stop timer: FW file info report verify timer
	 */
	m_dl_StopTimer(CMD_TIMID_FIRMFILE_INF, lw_CPRI_no);

	/*再送回数＜再送回数最大値
	 *resend times < max resend times
	 */
	lw_resend_times = get_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE);
	if (lw_resend_times < DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE_MAX)
	{
		/*再送回数[ファイル情報報告通知]#nを加算
		 *inc counter: file info report notice
		 */
		set_resend_counter(lw_REFW_index,
			DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE,
			lw_resend_times + 1);

		/*7-5-5 ファイル情報報告通知(LTE)
		 *do 7.5.5 file info report notice
		 */
		m_dl_FileInfoReportNotice(lw_CPRI_no);

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/*対RE-FW情報テーブルのファイル情報報告結果にNGを設定
	*set NG: RE FW info table's file info report result
	*/
	set_report_result(lw_REFW_index, REPORT_RESULT_FILE_INFO, REPORT_RESULT_NG);

	/*対RE-FW情報テーブルの使用中タイマを初期化
	*init using timer
	*/
	set_REFW_using_timer(lw_REFW_index, DLD_RERETIM_NONE);

	/*再送回数[ファイル情報報告通知]#n初期化
	*init counter: file info report notice
	*/
	set_resend_counter(lw_REFW_index, DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE, 0);

	/*更新開始要因#nを初期化
	*init update start factor
	*/
	set_update_start_factor(lw_REFW_index, DLD_UPDATE_FACTOR_NONE);

	/*更新中リンク情報のNG応答CPRI#ビットOFF
	*updating link info's NG respondCPRI bit OFF
	*/
	set_updating_link_info(lw_CPRI_no, 0);

	/*7-5-10 REファイル更新なし(パラメータ:NG)
	 *do 7.5.10 RE file update no
	*/
	m_dl_REFileNotUpdate(0, lw_CPRI_no, CMD_NG, CMD_OFF);
	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FileInfoReportRespondNG] RETURN" );
}
/* 7-5-16 ファイルデータ送信完了報告応答NG
 * 7.5.16 file data send complete report respond NG
 */
/*NOTE		*/
/*input msg not specify		*/
/*+ call 7.5.10:		*/
/*  send msg ファイルデータ送信完了報告通知(更新なし) to RE		*/
/*  send REファイル更新応答(結果(OK or NG),更新なし) to RE		*/
VOID m_dl_FileDataSendCopmleteReportRespondNG(VOID* bufp, USHORT cpri_no)
{
	//input
	USHORT lw_CPRI_no = cpri_no;

	//REFW
	USHORT lw_REFW_index = convert_CPRI_no_REFW_index(lw_CPRI_no);
	USHORT lw_resend_times = 0;
	ULONG lw_send_complete_part = 0;
	ULONG lw_send_complete_size = 0;
	USHORT lw_is_last_part;

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FileDataSendCopmleteReportRespondNG] ENTER" );

	/*ファームウェア送信完了確認タイマ#n(※1)停止
	 *stop timer: FW send complete verify timer
	 */
	m_dl_StopTimer(CMD_TIMID_FIRMFILE_SNDFIN, lw_CPRI_no);

	lw_resend_times = get_resend_counter(lw_REFW_index,
		DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE);
	if (lw_resend_times < DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE_MAX)
	{
		/*再送回数[ファイルデータ送信完了報告通知]#nを加算
		 *inc counter: file data send complete report notice
		 */
		set_resend_counter(lw_REFW_index, 
			DLD_RESEND_COUNTER_FILE_DATA_SEND_COMPLETE_REPORT_NOTICE,
			lw_resend_times + 1);

		/*対RE-FW情報テーブルの下記情報を初期化
		 *・送信済み分割番号#n
		 *・送信済みデータサイズ#n
		 *init: RE FW info table's following info
		 *+ send complete partial n
		 *+ send complete data size
		 */
		set_transfer_complete_part_no(lw_REFW_index, lw_send_complete_part);
		set_transfer_complete_data_size(lw_REFW_index, lw_send_complete_size);

		/*7-5-8 ファイルデータ送信(LTE)
		 *do 7.5.8 file data send
		 */
		m_dl_SendFileDataStart(lw_CPRI_no);
		//REFW status change to file data sending
		set_RE_status(lw_REFW_index, DLD_RESTA_FILE_TRANS);

		lw_is_last_part = m_dl_SendFileData(lw_CPRI_no);
		if (lw_is_last_part)
		{	/* ファイルデータ送信間隔タイマは、以下の関数内で停止している */
			m_dl_SendFileDataEnd(lw_CPRI_no);
		}

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/*当該対RE-FW情報テーブルのファイルデータ送信完了報告結果にNGを設定
	*set NG: RE FW info table's file data send complete report result
	*/
	set_report_result(lw_REFW_index, REPORT_RESULT_FILE_DATA_SEND_COMPLETE,
		REPORT_RESULT_NG);

	/*当該対RE-FW情報テーブルの使用中タイマを初期化
	*init timer: RE FW info table's using timer
	*/
	set_REFW_using_timer(lw_REFW_index, DLD_RERETIM_NONE);

	/*再送回数[ファイルデータ送信完了報告通知]#n初期化
	*init counter: file data send complete report notice
	*/
	set_resend_counter(lw_REFW_index, DLD_RESEND_COUNTER_FILE_INFO_REPORT_NOTICE, 0);

	/*更新開始要因#nを初期化
	*init update start factor
	*/
	set_update_start_factor(lw_REFW_index, DLD_UPDATE_FACTOR_NONE);

	/*更新中リンク情報のNG応答CPRI#ビットOFF
	*updating link info's NG respondCPRI bit OFF
	*/
	set_updating_link_info(lw_CPRI_no, 0);

	/*7-5-10 REファイル更新なし(パラメータ:NG)
	 *do 7.5.10 RE file update no
	*/
	m_dl_REFileNotUpdate(0, lw_CPRI_no, CMD_NG, CMD_OFF);

	//REFW status change to idle
	set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);

	cm_Assert( CMD_ASL_NOLEVEL, cpri_no,"[m_dl_FileDataSendCopmleteReportRespondNG] RETURN" );
}

/*!
 *  @brief  the processing for "配下RE CPRIリンク断"
 *  @note   
 *  @param  bufp		[in]  receive message pointer
 *  @param  cpri_no		[in]  the CPRI no
 *  @param  system_type	[in]  the System Type
 *  @return USHORT.
 *  @retval D_RRH_OK
 *  @date   2015/11/18 TDIPS)sasaki Update REC CNT-N/E切替問題対応
 */

USHORT m_dl_RELinkDisconect(VOID* bufp, USHORT cpri_no, USHORT system_type)
{
	DLT_REFW_TBL*	pReFwInfo = &gt_dlw_refw_all_tbl[system_type][cpri_no-1];
	USHORT			lwTimerId;
	INT				ldwerrcd;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_RELinkDisconect] ENTER");

	/* 対RE状態#nチェック */
	switch(pReFwInfo->re_sta)
	{
		case DLD_RESTA_FWREP_WAIT:
			/* ファームウェアファイル報告確認タイマ停止 */
			lwTimerId =  CMD_TIMID_FIRMFILE_REP_3G + (cpri_no - 1) +  D_RRH_CPRINO_RE_MAX * system_type;
			cm_TStop(lwTimerId, &ldwerrcd);
			break;
		case DLD_RESTA_FILEINF_WAIT:
			/* ファームウェアファイル情報報告確認タイマ停止 */
			m_dl_StopTimer(CMD_TIMID_FIRMFILE_INF, cpri_no);
			break;
		case DLD_RESTA_FILE_TRANS:
			/* ファイルデータ送信間隔タイマ停止 */
			m_dl_StopTimer(CMD_TIMID_FILEDATSND, cpri_no);
			break;
		case DLD_RESTA_FILE_COMP_WAIT:
			/* ファームウェア送信完了確認タイマ停止 */
			m_dl_StopTimer(CMD_TIMID_FIRMFILE_SNDFIN, cpri_no);
			break;
		case DLD_RESTA_FILEINF_SYSPRM_WAIT:
			/* タイマ停止 システムパラメータ情報報告確認タイマ停止 */
			lwTimerId = CMD_TIMID_SYSPRM_INF + (cpri_no - 1) + D_RRH_CPRINO_RE_MAX * system_type;
			cm_TStop(lwTimerId, &ldwerrcd);
			break;
		/* ファイルデータ送信完了報告応答(シスパラ)待ち */
		case DLD_RESTA_FILE_COMP_SYSPRM_WAIT:
			/* タイマ停止 システムパラメータ送信完了確認タイマ */
			lwTimerId = CMD_TIMID_SYSPRM_SNDFIN + (cpri_no - 1) + D_RRH_CPRINO_RE_MAX * system_type;
			cm_TStop(lwTimerId, &ldwerrcd);
			break;
		default:
			break;
	}
	BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "RELinkDisconect(RE#%02d,re_sta=%d)", cpri_no, pReFwInfo->re_sta);

	/* FHM保持REファイル情報の同一メーカRE接続有無を更新 */
	if(pReFwInfo->fhmrefile_inf != 0xFFFF)
	{
		gt_dlw_mnt_reinf[pReFwInfo->fhmrefile_inf].re_conn_exist &= (USHORT)~(CMD_ON << (cpri_no - 1));
	}

	/* 当該対RE-FW情報初期化 */
	REFW_init(pReFwInfo);

	if(CMD_SYS_S3G == system_type)
	{
		/* 更新中リンク情報のリンク断CPRI#ビットOFF */
		gt_dlw_dlm_tbl.link_inf &= (USHORT)~(CMD_ON << (cpri_no - 1));
		m_dl_ResumeWaitRE();
	}

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_RELinkDisconect] RETURN");
	return D_RRH_OK;
}
