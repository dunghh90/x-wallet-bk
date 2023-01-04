/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufsndcmprsp_waiting_ufwsnd.c
 * @brief  the processing for "ファイルデータ送信" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/11/19 TDIPS)sasaki Update
 * @date   2015/11/26 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファイルデータ送信" receiving at the ファイルデータ送信完了報告応答(REC←FHM) 送信待ち REC主導
 *  ファームウェア報告応答集約中 state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-02009
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/08/25 FPT)VietCV update
 *  @date   2015/11/19 TDIPS)sasaki Update REC CNT-N/E切替問題対応
 *  @date   2015/11/26 TDIPS)sasaki M-RRU-ZSYS-02009 Update IT2問処No.197対処 分割番号1のファイルデータ送信を受信した際の再ダウンロード準備処理をファイル種別に依らず実行
 */
VOID m_dl_rl_ufsndcmprsp_waiting_ufwsnd(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRIRCV_UFILEDATSND*	a_cpridat_p = NULL;
	USHORT							lwReNo;
	USHORT							lwLinkInfo;
	UINT							a_rtn = CMD_RES_OK;
	UINT							a_div_no = CMD_NUM0;
	UINT							a_datsize = CMD_NUM0;
	VOID*							a_fildat_p = NULL;

	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_ufsndcmprsp_waiting_ufwsnd] ENTER");

	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_UFILEDATSND*)bufp;
	/* 分割番号取得 */
	a_div_no = (a_cpridat_p->cpridat_ufiledatsnd.sgmtno1 << DLD_BIT16) | 
				a_cpridat_p->cpridat_ufiledatsnd.sgmtno2;

	/* データサイズ取得 */
	a_datsize = (a_cpridat_p->cpridat_ufiledatsnd.datsize1 << DLD_BIT16) | 
				 a_cpridat_p->cpridat_ufiledatsnd.datsize2;

	/* ファイルデータアドレス取得 */
	a_fildat_p = a_cpridat_p->cpridat_ufiledatsnd.fildat;

	/***********************************************/
	/* 条件チェック処理                            */
	/***********************************************/
	/* 実行中DL開始要因が0:未実行 */
	if( DLD_FACTOR_NONE == gt_dlw_dlm_tbl.dlstart_factor)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "dlstart_factor NG(fact=%d)", gt_dlw_dlm_tbl.dlstart_factor);
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return ;
	}

	/* 分割番号が1、かつフラッシュ書込フラグがON */
	if(a_div_no == CMD_NUM1)
	{
		/* 再ダウンロード準備 */
		m_dl_cm_USDlRetry();

		/* フラッシュ書込中の場合 */
		if(gt_dlw_dlm_tbl.write_flg == CMD_ON)
		{
			/* FHM file の場合 */
			if(DLD_FILENAME_FHM == gw_dl_FileType)
			{
				/* FWファイル保存中止API発行 */
				a_rtn = rrhApi_File_Mnt_CancelUpdFirm(D_RRH_PROCQUE_L3, CMD_NUM0, NULL);
				if(a_rtn != E_API_RCD_OK)		
				{
					cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Mnt_CancelUpdFirm return NG");
					return;
				}
				cm_Assert(CMD_ASL_DBGLOW, a_rtn, "rrhApi_File_Mnt_CancelUpdFirm OK");
			}
			/* フラッシュ書込フラグOFF */
			gt_dlw_dlm_tbl.write_flg = CMD_OFF;
		}
		/* get the handle again */
		if(dlw_handle == CMD_NUM0)
		{
			/* save the handle of shared memory */
			a_rtn = rrhApi_File_Fsm_loadMemory((INT *)&dlw_handle);
			if(a_rtn != E_API_RCD_OK)		
			{
				cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Fsm_loadMemory return NG");
				return;
			}
		}
	}
	else
	{
		if(gt_dlw_dlm_tbl.write_flg == CMD_ON)
		{
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "IN WRITING FLASH(divNo=%d)", a_div_no);
			return;
		}
	}

	/* 12: 運用中ファイルデータ送信(FW)待ち */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_USE_FILEDATATRANS_FW_WAIT;

	/* 受信済み分割番号をインクリメント */
	gt_dlw_rec_tbl.rcv_div_no++;

	/* 受信済みサイズにデータサイズ加算 */
	gt_dlw_rec_tbl.rcv_data_size += a_datsize;

	/* ファイルデータをバッファリング */
	a_rtn = rrhApi_File_Fsm_SaveData(dlw_handle,a_fildat_p,a_datsize);
	if (a_rtn != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Fsm_SaveData NG");
	}

	/* RE file の場合 */
	if (DLD_FILENAME_RE == gw_dl_FileType)
	{
		for(lwReNo = CMD_NUM1; lwReNo <= CMD_NUM16; lwReNo++)
		{
			/* Get 更新中リンク情報 */
			lwLinkInfo = get_updating_link_info(lwReNo);
			/* 更新中リンク情報ONの場合 */
			if( CMD_ON == lwLinkInfo )
			{
				m_dl_rec_main(bufp, lwReNo);
			}
		}
	}

	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsndcmprsp_waiting_ufwsnd] RETURN");
	return ;
}

/* @} */

