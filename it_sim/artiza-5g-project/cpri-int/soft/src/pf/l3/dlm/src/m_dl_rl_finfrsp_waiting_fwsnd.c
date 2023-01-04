/*!
 * @skip   $Id$
 * @file   m_dl_rl_finfrsp_waiting_fwsnd.c
 * @brief  the processing for "ファイルデータ送信" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/09/29 TDIPS)sasaki Update
 * @date   2015/09/30 TDIPS)sasaki Update
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/21 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"
/*!
 *  @brief  the processing for "ファイルデータ送信" receiving at the ファイルデータ転送先決定待ち REC主導
 *  ファームウェア報告応答集約中 state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01818
 *  @Bug_No M-RRU-ZSYS-02012
 *  @date   2015/08/18 FPT)DuongCD Create
 *	@date   2015/08/25 FPT)VietCV Update
 *	@date   2015/09/17 FPT)VietCV Update (fix bug IT1)
 *	@date   2015/09/29 TDIPS)sasaki Update FWバージョンの保存処理誤りを修正
 *	@date   2015/09/30 TDIPS)sasaki Update 「運用中REファイル更新なし」処理呼出の条件誤りを修正
 *	@date   2015/10/02 TDIPS)sasaki Update ダウンロード準備処理を修正
 * 	@date   2015/10/21 TDIPS)sasaki Update バージョン一致する場合でもダウンロードされる問題対処
 *	@date   2015/11/27 FPT)Yen M-RRU-ZSYS-02012 fix bug IT2 199 REへ流すチェック条件を修正
 */
VOID m_dl_rl_finfrsp_waiting_fwsnd(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_FILEDATSND*	a_cpridat_p = NULL;			/* CPRI信号データ部			*/
	CMT_TSKIF_UREFILNEWSTAREQ		lt_reFileUpdateRequest;
	T_SYS_RCVDATA_MKHEAD_S3G*		p_head;
	USHORT							rcv_mjr_ver = 0;
	USHORT							rcv_min_ver = 0;
	USHORT							rcv_year = 0;
	USHORT							lwLoop;
	USHORT							re_dl_flag = CMD_OFF;
	UINT							a_rtn		= CMD_RES_OK;	/* 戻り値格納変数			*/
	UINT							a_div_no	= CMD_NUM0;		/* 分割番号					*/
	UINT							a_datsize	= CMD_NUM0;		/* データサイズ				*/
	VOID*							a_fildat_p	= NULL;			/* ファイルデータ			*/

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
		
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_finfrsp_waiting_fwsnd] ENTER" );

	/******************/
	/* データ取得処理 */
	/******************/
	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_FILEDATSND*)bufp;
	/* 分割番号取得 */
	a_div_no = (a_cpridat_p->cpridat_filedatsnd.sgmtno1 << DLD_BIT16) | 
				a_cpridat_p->cpridat_filedatsnd.sgmtno2;
	
	/* データサイズ取得 */
	a_datsize = (a_cpridat_p->cpridat_filedatsnd.datsize1 << DLD_BIT16) | 
				 a_cpridat_p->cpridat_filedatsnd.datsize2;
	
	/* ファイルデータアドレス取得 */
	a_fildat_p = a_cpridat_p->cpridat_filedatsnd.fildat;
	
	/***********************************************/
	/* 条件チェック処理                            */
	/***********************************************/
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s gt_dlw_dlm_tbl.dlstart_factor[%d]\n",__LINE__,__FUNCTION__, gt_dlw_dlm_tbl.dlstart_factor);
	#endif
	/* 実行中DL開始要因が0:未実行 */
	if( DLD_FACTOR_NONE == gt_dlw_dlm_tbl.dlstart_factor)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "dlstart_factor NG(fact=%d)", gt_dlw_dlm_tbl.dlstart_factor);
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return ;
	}
	
	/* 分割番号が1、かつフラッシュ書込フラグがON */
	if(a_div_no == CMD_NUM1)
	{
		/* get the handle again */
		if(dlw_handle == CMD_NUM0)
		{
			/* save the handle of shared memory */
			a_rtn = rrhApi_File_Fsm_loadMemory((INT *)&dlw_handle);
			if(a_rtn != E_API_RCD_OK)		
			{
				cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Fsm_loadMemory return NG");
				#ifdef FHM_DLM_FOR_DEBUG_IT1
				printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
				#endif
				return;
			}
		}

		gt_dlw_rec_tbl.rcv_div_no = 0;
		gt_dlw_rec_tbl.rcv_data_size = 0;
		
		/* 8: ファイルデータ送信(FW)待ち */
		gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_FW_WAIT;
	
	}
	
	/* RECからの転送データ格納用領域(FHM保持REファイル情報[0])に以下の情報を保存
		・メーカ識別子
		・バージョン情報
	*/

	p_head = (T_SYS_RCVDATA_MKHEAD_S3G*)a_fildat_p;
	rcv_mjr_ver = ntohs(p_head->mjr_ver);
	rcv_min_ver = ntohs(p_head->min_ver);
	rcv_year = ntohs(p_head->year);

	gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg = p_head->mk_jdg;
	gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver = (((rcv_mjr_ver >> 8) - '0') * 10) + ((rcv_mjr_ver - '0') & 0xF);
	gt_dlw_mnt_reinf[CMD_NUM0].min_ver = (((rcv_min_ver >> 8) - '0') * 10) + ((rcv_min_ver - '0') & 0xF);
	gt_dlw_mnt_reinf[CMD_NUM0].year = ((rcv_year >> 12) * 1000) + (((rcv_year >> 8) & 0xF) * 100) + (((rcv_year >> 4) & 0xF) * 10) + (rcv_year & 0xF);
	gt_dlw_mnt_reinf[CMD_NUM0].month = ((p_head->month >> 4) * 10) + (p_head->month & 0xF);
	gt_dlw_mnt_reinf[CMD_NUM0].day = ((p_head->day >> 4) * 10) + (p_head->day & 0xF);

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s mk_jdg[%d], min_ver[%x], mjr_ver[%d]\n",__LINE__,__FUNCTION__,
			gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg,
				gt_dlw_mnt_reinf[CMD_NUM0].min_ver,
					gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver);
	#endif
	
	/* 受信済み分割番号をインクリメント[*4] */
	gt_dlw_rec_tbl.rcv_div_no++;
	
	/* 受信済みサイズにデータサイズ加算[*5] */
	gt_dlw_rec_tbl.rcv_data_size += a_datsize;
	
	/* ファイルデータをバッファリング */
	a_rtn = rrhApi_File_Fsm_SaveData(dlw_handle,a_fildat_p,a_datsize);
	if(a_rtn != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Fsm_SaveData NG");
	}
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s a_rtn[%d], dlw_handle[%x], gw_dl_FileType[%d]\n",__LINE__,__FUNCTION__,a_rtn, dlw_handle, gw_dl_FileType);
	#endif
	
	if ((DLD_FILENAME_RE == gw_dl_FileType) && (a_div_no == CMD_NUM1))
	{
		for (lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
		{
			if(DLD_RESLT_NML != gt_dlw_refw_tbl[lwLoop].fw_rep_rslt)
			{
				continue;
			}
			#ifdef FHM_DLM_FOR_DEBUG_IT1
			printf( "[%d]%s gt_dlw_refw_tbl[%d].mk_jdg[%d], gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg[%d]\n",__LINE__,__FUNCTION__,
										lwLoop,
												gt_dlw_refw_tbl[lwLoop].mk_jdg,
														gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg);
			#endif
			/* ファイルデータ転送先決定 */
			if (gt_dlw_refw_tbl[lwLoop].mk_jdg == gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg && 
				(gt_dlw_refw_tbl[lwLoop].usmjr_ver != gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver ||
				 gt_dlw_refw_tbl[lwLoop].usmin_ver != gt_dlw_mnt_reinf[CMD_NUM0].min_ver))
			{
				/* 運用中REファイル更新開始通知(更新対象CPRI#) */
				lt_reFileUpdateRequest.cpri_no = lwLoop + CMD_NUM1;
				lt_reFileUpdateRequest.head.uiEventNo = CMD_TSKIF_REFILE_UPDATE_START_NTC;
				lt_reFileUpdateRequest.head.uiSignalkind = 0;
				
				l3_com_sendMsg( CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0,  &lt_reFileUpdateRequest, sizeof(CMT_TSKIF_UREFILNEWSTAREQ));

				/* RE起動DL実施時は動作フラグを設定 */
				re_dl_flag =CMD_ON;
				
			}
		}
	}

	BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "[m_dl_rl_finfrsp_waiting_fwsnd] re_dl_flag(%d)", re_dl_flag);

	/* RE起動DL実施時は運用中REファイル更新なし通知を抑止 */
	/* (RE起動DL中のFHM-RE間LTE周期監視動作を抑止対応 */
	if(re_dl_flag != CMD_ON)
	{
		if ((DLD_FILENAME_RE == gw_dl_FileType) && (a_div_no == CMD_NUM1))
		{
			for (lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
			{
				if(DLD_RESLT_NML != gt_dlw_refw_tbl[lwLoop].fw_rep_rslt)
				{
					continue;
				}
				if(gt_dlw_refw_tbl[lwLoop].fw_rep_rslt == DLD_RESLT_NML)
				{
					/* 運用中REファイル更新なし */
					m_dl_reFileUpdateNone(bufp, lwLoop+1);
				}
			}
		}
	}

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s rcv_div_no[%d], rcv_data_size[%d] \n",__LINE__,__FUNCTION__, gt_dlw_rec_tbl.rcv_div_no, gt_dlw_rec_tbl.rcv_data_size);
	
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return ;
}

/* @} */

