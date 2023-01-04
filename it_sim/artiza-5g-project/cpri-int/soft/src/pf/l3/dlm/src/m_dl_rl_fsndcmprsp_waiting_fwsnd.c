/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsndcmprsp_waiting_fwsnd.c
 * @brief  the processing for "ファイルデータ送信" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
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
 *	@date   2015/08/25 FPT)VietCV update
 *	@date   2015/09/22 FPT)Yen update comment
 *	@date   2015/11/26 TDIPS)sasaki M-RRU-ZSYS-02009 Update IT2問処No.197対処 分割番号1のファイルデータ送信を受信した際の再ダウンロード準備処理をファイル種別に依らず実行
 */
VOID m_dl_rl_fsndcmprsp_waiting_fwsnd(VOID* bufp, USHORT cpri_no)
{

	UINT							a_rtn		= CMD_RES_OK;	/* 戻り値格納変数	*/
	UINT							a_div_no	= CMD_NUM0;		/* 分割番号			*/
	UINT							a_datsize	= CMD_NUM0;		/* データサイズ		*/
	VOID*							a_fildat_p	= NULL;			/* ファイルデータ	*/
	CMT_TSKIF_CPRIRCV_FILEDATSND	*a_cpridat_p = NULL;		/* CPRI信号データ部	*/
	USHORT							lwReNo;
	USHORT							lwLinkInfo;

	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_rl_fsndcmprsp_waiting_fwsnd] ENTER");

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
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s a_div_no: %d, a_datsize: %d \n",__LINE__,__FUNCTION__,a_div_no, a_datsize);
	#endif
	/***********************************************/
	/* 条件チェック処理                            */
	/***********************************************/
	/* 実行中DL開始要因が0:未実行 */
	if( DLD_FACTOR_NONE == gt_dlw_dlm_tbl.dlstart_factor)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "dlstart_factor NG(fact=%d)", gt_dlw_dlm_tbl.dlstart_factor);
		return ;
	}
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s gw_dl_FileType: %d\n",__LINE__,__FUNCTION__,gw_dl_FileType);
	#endif

	/* 分割番号が1 */
	if(a_div_no == CMD_NUM1)
	{
	    /* 再ダウンロード準備 */
		m_dl_cm_DlRetry(CMD_SYS_S3G);

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
			BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "IN WRITING FLASH(flg=%d,div_no=%d)", gt_dlw_dlm_tbl.write_flg, a_div_no);
			return;
		}
	}

	/* 8: ファイルデータ送信(FW)待ち */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_FW_WAIT;
	
	/* 受信済み分割番号をインクリメント[*4] */
	gt_dlw_rec_tbl.rcv_div_no++;
	
	/* 受信済みサイズにデータサイズ加算[*5] */
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

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsndcmprsp_waiting_fwsnd] RETURN" );

	return;
}

/* @} */

